#include "WebGUIServer.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <SPI.h>
#include <WiFi.h>
#include "DisplayTools.h"
#include "webFiles.h"
#include "SDcardFunctions.h"  // Add SD card functions header
#include "FirmwareStores.h"
#include <ArduinoJson.h>

// Configuration structure
struct Config {
    String httpuser;
    String httppassword;   // password to access web admin
    int webserverporthttp; // http port number for web admin
};

// Global variables
Config config;
AsyncWebServer* server = nullptr;
const char* host = "snailos";
bool shouldReboot = false;
const int default_webserverporthttp = 80;

// Check if user is authenticated
bool checkUserWebAuth(AsyncWebServerRequest *request) {
    bool isAuthenticated = false;
    if (request->authenticate(config.httpuser.c_str(), config.httppassword.c_str())) {
        isAuthenticated = true;
    }
    return isAuthenticated;
}

void configureWebServer() {
    // Initialize mDNS
    if (!MDNS.begin(host)) {
        Serial.println("Error setting up mDNS responder!");
    } else {
        Serial.println("mDNS responder started");
        Serial.println("You can now connect to http://" + String(host) + ".local");
    }

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    // Handle not found
    server->onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("/");
    });

    // Handle logout
    server->on("/logout", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->requestAuthentication();
        request->send(401);
    });

    // Handle logged out page
    server->on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request) {
        String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
        Serial.println(logmessage);
        request->send(200, "text/html", "<h1>Logged Out</h1><p>You have been logged out.</p>");
    });

    // Handle system info
    server->on("/systeminfo", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            String response = "{\"status\":\"connected\",\"ip\":\"" + WiFi.localIP().toString() + "\"}";
            request->send(200, "application/json", response);
        } else {
            return request->requestAuthentication();
        }
    });

    // Handle WiFi configuration
    server->on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            if (request->hasParam("ssid") && request->hasParam("pwd")) {
                const char* ssid = request->getParam("ssid")->value().c_str();
                const char* pwd = request->getParam("pwd")->value().c_str();
                request->send(200, "text/plain", "WiFi configured: " + String(ssid));
            }
        } else {
            return request->requestAuthentication();
        }
    });

    // Handle reboot
    server->on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            shouldReboot = true;
            request->send(200, "text/html", "Rebooting...");
        } else {
            return request->requestAuthentication();
        }
    });

    // Handle file listing
    server->on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            String folder = "/";
            if (request->hasParam("folder")) {
                folder = request->getParam("folder")->value();
            }
            
            FileEntry entries[MAX_FILES];
            int count = 0;
            readDirectory(folder, entries, count);
            sortFileList(entries, count);
            
            String response = "pa:" + folder + ":0\n";  // Parent directory info
            for (int i = 0; i < count; i++) {
                String type = entries[i].isDirectory ? "Fo" : "Fi";
                String size = entries[i].isDirectory ? "0" : String(SD.open(entries[i].path).size());
                response += type + ":" + entries[i].name + ":" + size + "\n";
            }
            
            request->send(200, "text/plain", response);
        } else {
            return request->requestAuthentication();
        }
    });

    // Handle file operations (download, delete, create folder)
    server->on("/file", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            if (request->hasParam("name") && request->hasParam("action")) {
                String fileName = request->getParam("name")->value();
                String action = request->getParam("action")->value();
                
                if (action == "download") {
                    request->send(SD, fileName, "application/octet-stream");
                } else if (action == "delete") {
                    if (deleteFileOrFolder(fileName)) {
                        request->send(200, "text/plain", "Deleted: " + fileName);
                    } else {
                        request->send(500, "text/plain", "Failed to delete: " + fileName);
                    }
                } else if (action == "create") {
                    if (createFolder(fileName)) {
                        request->send(200, "text/plain", "Created folder: " + fileName);
                    } else {
                        request->send(500, "text/plain", "Failed to create folder: " + fileName);
                    }
                } else {
                    request->send(400, "text/plain", "Invalid action");
                }
            } else {
                request->send(400, "text/plain", "Missing parameters");
            }
        } else {
            return request->requestAuthentication();
        }
    });

    // Handle file rename
    server->on("/rename", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            if (request->hasParam("filePath", true) && request->hasParam("fileName", true)) {
                String filePath = request->getParam("filePath", true)->value();
                String newName = request->getParam("fileName", true)->value();
                
                if (renameFileOrFolder(filePath, newName)) {
                    request->send(200, "text/plain", "Renamed: " + filePath + " to " + newName);
                } else {
                    request->send(500, "text/plain", "Failed to rename: " + filePath);
                }
            } else {
                request->send(400, "text/plain", "Missing parameters");
            }
        } else {
            return request->requestAuthentication();
        }
    });

    // Handle file upload
    server->on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            request->send(200, "text/plain", "Upload complete");
        } else {
            return request->requestAuthentication();
        }
    }, [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!checkUserWebAuth(request)) return;
        
        static String uploadFolder = "/";
        static File uploadFile;
        
        if (index == 0) {
            // Start of upload
            if (request->hasParam("folder", true)) {
                uploadFolder = request->getParam("folder", true)->value();
            }
            String filePath = uploadFolder + "/" + filename;
            uploadFile = SD.open(filePath, FILE_WRITE);
            if (!uploadFile) {
                request->send(500, "text/plain", "Failed to create file");
                return;
            }
        }
        
        if (uploadFile) {
            if (len) {
                uploadFile.write(data, len);
            }
            if (final) {
                uploadFile.close();
            }
        }
    });

    // Add endpoint for display status updates
    server->on("/api/display/status", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("data", true)) {
            request->send(400, "application/json", "{\"error\":\"Missing data\"}");
            return;
        }

        String data = request->getParam("data", true)->value();
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, data);

        if (error) {
            request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }

        const char* status = doc["status"];
        bool isDownloading = doc["isDownloading"] | false;

        if (isDownloading) {
            // Show download status on the device
            M5.Display.clear();
            M5.Display.setTextColor(TFT_WHITE);
            M5.Display.setTextSize(2);
            M5.Display.setCursor(10, 10);
            M5.Display.println("Downloading...");
            M5.Display.setTextSize(1);
            M5.Display.setCursor(10, 40);
            M5.Display.println(status);
        } else {
            // Return to normal display
            M5.Display.clear();
            M5.Display.setTextColor(TFT_WHITE);
            M5.Display.setTextSize(2);
            M5.Display.setCursor(10, 10);
            M5.Display.println("WebGUI Active");
            M5.Display.setTextSize(1);
            M5.Display.setCursor(10, 40);
            M5.Display.println("Connected to: " + WiFi.localIP().toString());
        }

        request->send(200, "application/json", "{\"success\":true}");
    });

    // Add endpoint for file uploads
    server->on("/api/files/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        // This is a multipart form upload, so we need to handle it differently
        request->send(400, "application/json", "{\"error\":\"Please use the file upload form\"}");
    }, [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
        static String currentFolder;
        static File uploadFile;
        
        if (index == 0) {
            // First chunk - get the folder parameter and prepare the file
            if (!request->hasParam("folder", true)) {
                request->send(400, "application/json", "{\"error\":\"Missing folder parameter\"}");
                return;
            }
            
            currentFolder = request->getParam("folder", true)->value();
            
            // Ensure the folder exists
            if (!SD.exists(currentFolder)) {
                if (!SD.mkdir(currentFolder)) {
                    request->send(500, "application/json", "{\"error\":\"Could not create folder\"}");
                    return;
                }
            }
            
            // Create the full path
            String filepath = currentFolder + "/" + filename;
            
            // Open the file for writing
            uploadFile = SD.open(filepath, FILE_WRITE);
            if (!uploadFile) {
                request->send(500, "application/json", "{\"error\":\"Could not open file for writing\"}");
                return;
            }
        }
        
        // Write the data chunk
        if (len > 0 && uploadFile) {
            if (uploadFile.write(data, len) != len) {
                uploadFile.close();
                request->send(500, "application/json", "{\"error\":\"Error writing file\"}");
                return;
            }
        }
        
        // Final chunk - close the file and send success response
        if (final) {
            uploadFile.close();
            String filepath = currentFolder + "/" + filename;
            request->send(200, "application/json", "{\"success\":true,\"path\":\"" + filepath + "\"}");
        }
    });

    // Serve the main page
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (checkUserWebAuth(request)) {
            AsyncWebServerResponse *response = request->beginResponse(200, "text/html", (const uint8_t*)index_html, index_html_size);
            request->send(response);
        } else {
            return request->requestAuthentication();
        }
    });
}

void startWebGUIServer() {
    // Initialize firmware list
    firmwareList.clear();
    for (int i = 0; i < NUM_FIRMWARE_ITEMS; i++) {
        firmwareList.push_back(FIRMWARE_ITEMS[i]);
    }

    // Set default credentials
    config.httpuser = "admin";
    config.httppassword = "snailos";
    config.webserverporthttp = default_webserverporthttp;

    // Create and configure web server
    server = new AsyncWebServer(config.webserverporthttp);
    configureWebServer();

    // Start server
    server->begin();
    Serial.println("WebGUI server started");
    Serial.println("Access the WebGUI at:");
    Serial.println("http://" + String(host) + ".local");
    Serial.println("or");
    Serial.println("http://" + WiFi.localIP().toString());
    Serial.println("Username: " + config.httpuser);
    Serial.println("Password: " + config.httppassword);

    // Display server info on screen
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(10, 10);
    M5.Display.println("WebGUI Server");
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, 40);
    M5.Display.println("Access via:");
    M5.Display.println("http://" + String(host) + ".local");
    M5.Display.println("or");
    M5.Display.println("http://" + WiFi.localIP().toString());
    M5.Display.println("\nUsername: " + config.httpuser);
    M5.Display.println("Password: " + config.httppassword);
    M5.Display.println("\nPress ESC to stop server");
}

void stopWebGUIServer() {
    if (server != nullptr) {
        server->end();
        delete server;
        server = nullptr;
    }
    MDNS.end();  // Stop mDNS service
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void updateWebGUIServer() {
    if (shouldReboot) {
        ESP.restart();
    }
} 