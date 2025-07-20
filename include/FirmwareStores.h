#pragma once

#include <M5Unified.h>
#include <SD.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <vector>
#include <String.h>

// Structure to hold firmware info
struct FirmwareInfo {
    String number;
    String name;
    String description;
    String downloadUrl;
};

// Function declarations
void initFirmwareStore();
void drawFirmwareStore();
void updateFirmwareStoreViewport();
void handleFirmwareStoreInput();
bool loadFirmwareList();
void downloadFirmware(String fileAddr, String fileName);
String replaceChars(String input);

// Global variables
extern int firmwareStoreViewportStart;
extern int selectedFirmware;
extern std::vector<FirmwareInfo> firmwareList;

// Constants
extern const int FIRMWARE_STORE_VISIBLE_ITEMS;
extern const FirmwareInfo FIRMWARE_ITEMS[10];  // Specify the size of the array (10 items)
extern const int NUM_FIRMWARE_ITEMS;  // Add a constant for the number of items 