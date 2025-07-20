#include "WiFiFunctions.h"
#include "MenuStates.h"
#include "DisplayTools.h"
#include "InputFunctions.h"

// Global variables
std::vector<WiFiNetwork> availableNetworks;
int selectedNetworkIndex = 0;
bool isScanning = false;

void scanWiFiNetworks() {
    isScanning = true;
    availableNetworks.clear();
    
    // Set WiFi to station mode
    WiFi.mode(WIFI_STA);
    
    // Start scanning
    int numNetworks = WiFi.scanNetworks();
    
    // Store network information
    for (int i = 0; i < numNetworks; i++) {
        WiFiNetwork network;
        network.ssid = WiFi.SSID(i);
        network.rssi = WiFi.RSSI(i);
        network.encryptionType = WiFi.encryptionType(i);
        availableNetworks.push_back(network);
    }
    
    isScanning = false;
}

void displayWiFiNetworks() {
    // Clear screen
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(2);
    
    // Display title
    M5.Display.setCursor(10, 10);
    M5.Display.println("Available Networks:");
    
    // Display networks
    M5.Display.setTextSize(1);
    int yPos = 40;
    int maxNetworks = min(8, (int)availableNetworks.size()); // Show max 8 networks at a time
    
    for (int i = 0; i < maxNetworks; i++) {
        int index = selectedNetworkIndex - (selectedNetworkIndex % 8) + i;
        if (index >= availableNetworks.size()) break;
        
        // Highlight selected network
        if (index == selectedNetworkIndex) {
            M5.Display.fillRect(5, yPos - 2, M5.Display.width() - 10, 20, TFT_BLUE);
            M5.Display.setTextColor(TFT_BLACK);
        } else {
            M5.Display.setTextColor(TFT_WHITE);
        }
        
        // Display network info
        M5.Display.setCursor(10, yPos);
        M5.Display.print(availableNetworks[index].ssid);
        M5.Display.print(" (");
        M5.Display.print(availableNetworks[index].rssi);
        M5.Display.print(" dBm)");
        
        // Show encryption type
        M5.Display.setCursor(10, yPos + 12);
        switch (availableNetworks[index].encryptionType) {
            case WIFI_AUTH_OPEN:
                M5.Display.print("Open");
                break;
            case WIFI_AUTH_WEP:
                M5.Display.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                M5.Display.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                M5.Display.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                M5.Display.print("WPA/WPA2");
                break;
            default:
                M5.Display.print("Unknown");
        }
        
        yPos += 30;
    }
    
    // Show navigation help
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(10, M5.Display.height() - 30);
    M5.Display.println("Up/Down: Select  Enter: Connect");
}

void connectToWiFi(const String& ssid, const String& password) {
    // Clear screen and show connecting message
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(10, M5.Display.height() / 2 - 20);
    M5.Display.println("Connecting to:");
    M5.Display.println(ssid);
    
    // Attempt to connect
    WiFi.begin(ssid.c_str(), password.c_str());
    
    // Wait for connection with timeout
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        M5.Display.print(".");
        attempts++;
    }
    
    // Show result
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(10, M5.Display.height() / 2 - 20);
    if (WiFi.status() == WL_CONNECTED) {
        M5.Display.println("Connected!");
        M5.Display.println("IP: " + WiFi.localIP().toString());
    } else {
        M5.Display.println("Connection failed!");
    }
    
    delay(2000);
}

void handleWiFiConnection() {
    if (availableNetworks.empty()) {
        scanWiFiNetworks();
    }
    
    bool needsRedraw = true;
    bool waitingForPassword = false;
    String password = "";
    
    while (true) {
        M5Cardputer.update();
        
        // Only redraw if something changed
        if (needsRedraw && !waitingForPassword) {
            displayWiFiNetworks();
            needsRedraw = false;
        }
        
        // Handle keyboard input
        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            if (waitingForPassword) {
                // Handle password input
                if (M5Cardputer.Keyboard.isKeyPressed(0x28)) { // Enter key
                    if (password.length() > 0) {
                        WiFiNetwork selectedNetwork = availableNetworks[selectedNetworkIndex];
                        connectToWiFi(selectedNetwork.ssid, password);
                        return;
                    }
                }
                else if (M5Cardputer.Keyboard.isKeyPressed(0x60)) { // ESC key
                    waitingForPassword = false;
                    needsRedraw = true;
                }
                else {
                    // Handle password input
                    handleKeyboardInput(password, true);
                    drawKeyboardInput(password, "Enter password for " + availableNetworks[selectedNetworkIndex].ssid, true);
                }
            }
            else {
                // Handle network selection
                // Up key (0x3B)
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    if (selectedNetworkIndex > 0) {
                        selectedNetworkIndex--;
                        needsRedraw = true;
                    }
                }
                // Down key (0x2E)
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    if (selectedNetworkIndex < availableNetworks.size() - 1) {
                        selectedNetworkIndex++;
                        needsRedraw = true;
                    }
                }
                // Enter key (0x28)
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    WiFiNetwork selectedNetwork = availableNetworks[selectedNetworkIndex];
                    
                    // If network is encrypted, prompt for password
                    if (selectedNetwork.encryptionType != WIFI_AUTH_OPEN) {
                        waitingForPassword = true;
                        password = "";
                        drawKeyboardInput(password, "Enter password for " + selectedNetwork.ssid, true);
                    } else {
                        connectToWiFi(selectedNetwork.ssid, "");
                        return;
                    }
                }
                // ESC key (0x60) - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    return;
                }
            }
        }
        
        delay(50);  // Small delay to prevent CPU hogging
    }
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String getWiFiStatus() {
    if (WiFi.status() == WL_CONNECTED) {
        return "Connected to: " + WiFi.SSID();
    } else {
        return "Not Connected";
    }
} 