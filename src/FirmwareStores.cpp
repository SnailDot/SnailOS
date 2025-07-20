#include "FirmwareStores.h"
#include "DisplayTools.h"
#include "SDcardFunctions.h"
#include "WiFiFunctions.h"
#include "MenuStates.h"
#include <M5Cardputer.h>
#include <utility/Keyboard.h>
#include <SPIFFS.h>

// Constants
const char* STORE_INFO_PATH = "/M5Store.txt";  // This will be in the data directory

// Global variables
int firmwareStoreViewportStart = 0;
int selectedFirmware = 0;
const int FIRMWARE_STORE_VISIBLE_ITEMS = 2;
std::vector<FirmwareInfo> firmwareList;

// Define the number of firmware items
const int NUM_FIRMWARE_ITEMS = 10;

// Hardcoded firmware list - make it accessible to other files
const FirmwareInfo FIRMWARE_ITEMS[NUM_FIRMWARE_ITEMS] = {
    {
        "1",
        "Audio Spectrum",
        "An audio spectrum based on your mic",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/AudioSpectrum.bin"
    },
    {
        "2",
        "Car Dashboard",
        "A car dashboard for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/CarDashboard-Volos.bin"
    },
    {
        "3",
        "Web Radio",
        "A web radio for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/M5Card_WebRadio.ino.bin"
    },
    {
        "4",
        "TV Remote",
        "A TV remote for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/Volos-TV-Remote.bin"
    },
    {
        "5",
        "Pawnagotchi",
        "A pawnagotchi for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/Pawnagochi.bin"
    },
    {
        "6",
        "Neom",
        "A neom for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/Nemo-EN-2.6.bin"
    },
    {
        "7",
        "Bruce",
        "A bruce for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/Bruce-0.7.bin"
    },
    {
        "8",
        "Saturn",
        "A saturn for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/Saturn-v1.2.1.en-us.bin"
    },
    {
        "9",
        "Evil",
        "A wifi pentesting OS for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/Evil-Cardputer%201.1.7.bin"
    },
    {
        "10",
        "User Demo",
        "A user demo for your M5Cardputer",
        "https://raw.githubusercontent.com/bmorcelli/Launcher/main/Compatible%20BINs/M5Cardputer/UserDemo.bin"
    }
};

void initFirmwareStore() {
    // First check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        handleWiFiConnection();
        if (!isWiFiConnected()) {
            drawError("WiFi connection required");
            delay(2000);
            return;
        }
    }

    // Initialize SPIFFS if needed
    if (!SPIFFS.begin(true)) {
        drawError("Failed to initialize storage");
        delay(2000);
        return;
    }

    // Initialize SD card if needed (only for downloads)
    if (!initSDCard()) {
        drawError("SD Card not found");
        delay(2000);
        return;
    }
    
    // Create downloads directory if it doesn't exist
    if (!SD.exists("/downloads")) {
        if (!SD.mkdir("/downloads")) {
            drawError("Failed to create downloads folder");
            delay(2000);
            return;
        }
    }
    
    // Load firmware list from hardcoded array
    firmwareList.clear();
    for (const auto& item : FIRMWARE_ITEMS) {
        firmwareList.push_back(item);
    }
    
    // Reset viewport and selection
    firmwareStoreViewportStart = 0;
    selectedFirmware = 0;
    
    // Draw the initial store view
    drawFirmwareStore();
}

bool loadFirmwareList() {
    if (!SPIFFS.exists(STORE_INFO_PATH)) {
        Serial.println("Store info file not found: " + String(STORE_INFO_PATH));
        return false;
    }
    
    File file = SPIFFS.open(STORE_INFO_PATH, "r");
    if (!file) {
        Serial.println("Failed to open store info file");
        return false;
    }
    
    firmwareList.clear();
    
    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() == 0) continue;
        
        // Parse line in format: "Number: Name - Description - URL"
        int colonPos = line.indexOf(':');
        int firstDashPos = line.indexOf(" - ");
        int secondDashPos = line.indexOf(" - ", firstDashPos + 3);
        
        if (colonPos == -1 || firstDashPos == -1 || secondDashPos == -1) {
            Serial.println("Invalid line format: " + line);
            continue;
        }
        
        FirmwareInfo info;
        String temp;
        
        // Extract number
        temp = line.substring(0, colonPos);
        temp.trim();
        info.number = temp;
        
        // Extract name
        temp = line.substring(colonPos + 1, firstDashPos);
        temp.trim();
        info.name = temp;
        
        // Extract description
        temp = line.substring(firstDashPos + 3, secondDashPos);
        temp.trim();
        info.description = temp;
        
        // Extract URL
        temp = line.substring(secondDashPos + 3);
        temp.trim();
        info.downloadUrl = temp;
        
        firmwareList.push_back(info);
        Serial.println("Loaded firmware: " + info.name);
    }
    
    file.close();
    Serial.println("Loaded " + String(firmwareList.size()) + " firmware items");
    return firmwareList.size() > 0;
}

void drawFirmwareStore() {
    M5.Display.fillScreen(TFT_GREEN);
    
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("M5 Firmware Store");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Check if we have any firmware items
    if (firmwareList.size() == 0) {
        M5.Display.setTextSize(1);
        M5.Display.setCursor(10, 60);
        M5.Display.println("No firmware available");
        M5.Display.setCursor(10, 80);
        M5.Display.println("Press ESC to return");
        return;
    }
    
    int y = 60;
    for (int i = 0; i < FIRMWARE_STORE_VISIBLE_ITEMS; i++) {
        int itemIndex = firmwareStoreViewportStart + i;
        if (itemIndex >= firmwareList.size()) break;
        
        const FirmwareInfo& firmware = firmwareList[itemIndex];
        
        if (itemIndex == selectedFirmware) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(firmware.name);
        y += 50;
    }
    
    if (firmwareList.size() > FIRMWARE_STORE_VISIBLE_ITEMS) {
        M5.Display.setTextSize(1);
        M5.Display.setTextColor(TFT_BLACK);
        M5.Display.setCursor(M5.Display.width() - 20, 10);
        M5.Display.printf("%d/%d", selectedFirmware + 1, firmwareList.size());
    }
}

void updateFirmwareStoreViewport() {
    if (firmwareList.size() == 0) return;
    
    if (selectedFirmware < firmwareStoreViewportStart) {
        firmwareStoreViewportStart = selectedFirmware;
    } else if (selectedFirmware >= firmwareStoreViewportStart + FIRMWARE_STORE_VISIBLE_ITEMS) {
        firmwareStoreViewportStart = selectedFirmware - FIRMWARE_STORE_VISIBLE_ITEMS + 1;
    }
    
    // Ensure viewport doesn't go beyond list size
    if (firmwareStoreViewportStart + FIRMWARE_STORE_VISIBLE_ITEMS > firmwareList.size()) {
        firmwareStoreViewportStart = firmwareList.size() - FIRMWARE_STORE_VISIBLE_ITEMS;
        if (firmwareStoreViewportStart < 0) firmwareStoreViewportStart = 0;
    }
}

void handleFirmwareStoreInput() {
    if (firmwareList.size() == 0) return;
    
    const FirmwareInfo& firmware = firmwareList[selectedFirmware];
    
    // Show firmware details and download confirmation
    M5.Display.fillScreen(TFT_GREEN);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Firmware Details");
    
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, 40);
    M5.Display.println("Name: " + firmware.name);
    M5.Display.setCursor(10, 60);
    M5.Display.println("Description: " + firmware.description);
    M5.Display.setCursor(10, 80);
    M5.Display.println("Download this firmware?");
    M5.Display.setCursor(10, 100);
    M5.Display.println("Press Enter to download");
    M5.Display.setCursor(10, 120);
    M5.Display.println("Press ESC to cancel");
    
    while (true) {
        M5.update();
        M5Cardputer.update();
        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {  // Enter
                // Extract filename from URL
                String url = firmware.downloadUrl;
                int lastSlash = url.lastIndexOf('/');
                String filename = url.substring(lastSlash + 1);
                
                // Download the firmware
                downloadFirmware(firmware.downloadUrl, firmware.name);
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed(0x60)) {  // ESC
                drawFirmwareStore();
                break;
            }
        }
        delay(50);
    }
}

void downloadFirmware(String fileAddr, String fileName) {
    fileName = replaceChars(fileName);
    
    // Fix GitHub raw URL if needed
    if (fileAddr.indexOf("github.com") != -1 && fileAddr.indexOf("/blob/") != -1) {
        fileAddr.replace("github.com", "raw.githubusercontent.com");
        fileAddr.replace("/blob/", "/");
    }
    
    M5.Display.fillScreen(TFT_GREEN);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Downloading...");
    M5.Display.setTextSize(1);
    M5.Display.drawString(fileName, M5.Display.width()/2, M5.Display.height()/2 - 20, 2);
    
    WiFiClientSecure *client = new WiFiClientSecure;
    if (!client) {
        drawError("Failed to create client");
        delay(2000);
        drawFirmwareStore();
        return;
    }
    
    client->setInsecure();
    
    HTTPClient http;
    http.begin(*client, fileAddr);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    
    int httpCode = http.GET();
    if (httpCode <= 0) {
        String error = "Download failed: ";
        error += http.errorToString(httpCode);
        drawError(error);
        http.end();
        delete client;
        delay(2000);
        drawFirmwareStore();
        return;
    }
    
    if (httpCode != HTTP_CODE_OK) {
        String error = "HTTP error: ";
        error += String(httpCode);
        drawError(error);
        http.end();
        delete client;
        delay(2000);
        drawFirmwareStore();
        return;
    }
    
    File file = SD.open("/downloads/" + fileName + ".bin", FILE_WRITE);
    if (!file) {
        drawError("Failed to create file");
        http.end();
        delete client;
        delay(2000);
        drawFirmwareStore();
        return;
    }
    
    int totalSize = http.getSize();
    if (totalSize <= 0) {
        drawError("Invalid file size");
        file.close();
        http.end();
        delete client;
        delay(2000);
        drawFirmwareStore();
        return;
    }
    
    uint8_t buff[128] = { 0 };
    WiFiClient * stream = http.getStreamPtr();
    int downloaded = 0;
    
    while (http.connected() && (downloaded < totalSize)) {
        // Check if there's data available
        if (!stream->available()) {
            delay(1);
            continue;
        }
        
        // Read data
        size_t available = stream->available();
        size_t bytesRead = stream->readBytes(buff, min(available, sizeof(buff)));
        
        // Write to file
        if (bytesRead > 0) {
            file.write(buff, bytesRead);
            downloaded += bytesRead;
            
            // Update progress
            int progress = (downloaded * 100) / totalSize;
            M5.Display.fillRect(10, M5.Display.height()/2 + 10, M5.Display.width() - 20, 20, TFT_DARKGREEN);
            M5.Display.fillRect(10, M5.Display.height()/2 + 10, (M5.Display.width() - 20) * progress / 100, 20, TFT_CYAN);
        }
        
        // Small delay to prevent watchdog issues
        delay(1);
    }
    
    // Clean up resources
    file.close();
    http.end();
    delete client;
    
    // Show completion message
    M5.Display.fillScreen(TFT_GREEN);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Download Complete!");
    
    // Wait a moment to show the message
    delay(1000);
    
    // Return to store menu
    drawFirmwareStore();
    
    // Reset any state that might cause issues
    firmwareStoreViewportStart = 0;
    selectedFirmware = 0;
}

String replaceChars(String input) {
    const char charsToReplace[] = {'/', '\\', '\"', '\'', '`'};
    const char replacementChar = '_';
    
    for (size_t i = 0; i < sizeof(charsToReplace); i++) {
        input.replace(String(charsToReplace[i]), String(replacementChar));
    }
    return input;
}

void installFirmware(String fileAddr, uint32_t app_size, bool spiffs, uint32_t spiffs_offset, uint32_t spiffs_size, bool nb, bool fat, uint32_t fat_offset[2], uint32_t fat_size[2]) {
    drawError("Installation not implemented yet");
    delay(2000);
}

bool installFAT_OTA(WiFiClientSecure *client, String fileAddr, uint32_t offset, uint32_t size, const char *label) {
    return false;
} 