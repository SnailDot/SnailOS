#ifndef __WIFI_FUNCTIONS_H
#define __WIFI_FUNCTIONS_H

#include <WiFi.h>
#include <vector>
#include <string>

// Structure to hold WiFi network information
struct WiFiNetwork {
    String ssid;
    int32_t rssi;
    uint8_t encryptionType;
};

// Function declarations
void scanWiFiNetworks();
void displayWiFiNetworks();
void connectToWiFi(const String& ssid, const String& password);
void handleWiFiConnection();
bool isWiFiConnected();
String getWiFiStatus();

// Global variables
extern std::vector<WiFiNetwork> availableNetworks;
extern int selectedNetworkIndex;
extern bool isScanning;

#endif // __WIFI_FUNCTIONS_H 