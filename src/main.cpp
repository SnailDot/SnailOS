#include "SoundFunctions.h"  // Move to top
#include <M5Unified.h>
#include <M5Cardputer.h>
#include <utility/Keyboard.h>
#include <vector>
#include <string>
#include "SDcardFunctions.h"
#include "DisplayTools.h"
#include "MenuStates.h"  // Include the new header
#include <SPIFFS.h>
#include <LittleFS.h>
#include "WiFiFunctions.h"
#include "WebGUIServer.h"  // Add this at the top with other includes
#include "FirmwareStores.h"  // Add after other includes

// Forward declarations
void playTestSound();  // Add this declaration

// Global variables
MenuState currentState = MAIN_MENU;
int selectedMenuItem = 0;
// Make these variables external since they're defined in SDcardFunctions.cpp
extern String currentPath;
extern int fileCount;
extern int selectedFileIndex;
int viewportStart = 0;
int selectedContextMenuItem = 0;
String renameBuffer = "";
String fileToCopy = "";  // Store the path of file to copy
String bootSplashImage = "/SnailOS/Assets/boot.jpg";  // Default splash image path
String bootSplashImageAlt = "/SnailOS/Assets/Boot.jpg";  // Alternative splash image path
String bootText = "SnailOS v0.1";  // Default boot text
String bootTextBuffer = "";  // Buffer for editing boot text

// Add after the boot text variables
const String configPath = "/SnailOS/Data/config.txt";  // Path to config file

// Menu items
const char* menuItems[] = {
    "SnailOS Tools",
    "Connections",
    "Tool Sets",
    "Your Programs",
    "Get Programs",
    "Settings"
};

// Main Menu Setup
const int NUM_MENU_ITEMS = 6;  // Update from 10 to 4 to match new menu items
const int VISIBLE_ITEMS = 2;  // Number of items visible at once
int fileExplorerViewportStart = 0;  // Index of first visible file

// File Explorer UI constants
const int FILE_EXPLORER_VISIBLE_ITEMS = 2;  // Number of files visible at once

// Context menu items
const char* contextMenuItems[] = {
    "Edit",
    "Rename",
    "Delete"
};

// File options setup
const int NUM_CONTEXT_MENU_ITEMS = 3;  // Reduced from 5 to 3
const int CONTEXT_MENU_VISIBLE_ITEMS = 2;  // Number of items visible at once
int contextMenuViewportStart = 0;  // Index of first visible item

const char* displayToolsItems[] = {
    "Display Image",
    "Boot Image",
    "Boot Text"
};

const char* displaySoundItems[] = {
    "Pre Made Sounds"
};

const char* displaySettingsItems[] = {
    "Reboot",
    "Display",
};

const char* displayFileItems[] = {
    "File Explorer",
};


const char* displaySettingsDisplayItems[] = {
    "Edit Boot Text"
};

const char* MainToolsItems[] = {
    "File Tools",
    "Dev Tools",
    "Display Tools",
    "Sound Tools"
};

const char* ConnectionItems[] = {  // Renamed from displayToolsItems
    "WebGUI"
};

const char* SnailOSItems[] = {  // Renamed from displayToolsItems
    "SnailHub"
};

const char* DevToolsItems[] = {  // Renamed from displayToolsItems
    "Input Test"
};

const char* ExtraItems[] = {  // Renamed from displayToolsItems
    "Calculator",
    "HEX Converter",
    "Number Counter"
};

const char* WifiItems[] = {  // Renamed from displayToolsItems
    "Scan All Wifi",
    "Scan Open Wifi",
    "Monitor Wifi"
};


const char* BluetoothItems[] = {  // Renamed from displayToolsItems
    "Placeholder"
};

const char* GetProgramsItems[] = {  // Renamed from displayToolsItems
    "Snail's Store"
};

// Display Settings setup
const int NUM_DISPLAY_SETTINGS_ITEMS = 1;
const int DISPLAY_SETTINGS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int displaySettingsViewportStart = 0;  // Index of first visible item
int selectedDisplaySetting = 0;  // Currently selected display setting

// Display Tools setup
const int NUM_DISPLAY_TOOLS_ITEMS = 3;
const int DISPLAY_TOOLS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int displayToolsViewportStart = 0;  // Index of first visible item
int selectedDisplayTool = 0;  // Currently selected display tool

// settings setup
const int NUM_SETTINGS_ITEMS = 4;
const int SETTINGS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int settingsViewportStart = 0;  // Index of first visible item
int selectedSetting = 0;  // Currently selected setting

// Sound tools setup
const int NUM_SOUND_TOOLS_ITEMS = 1;  // Updated to match new menu items
const int SOUND_TOOLS_VISIBLE_ITEMS = 1;  // Number of items visible at once
int soundToolsViewportStart = 0;  // Index of first visible item
int selectedSoundTool = 0;  // Currently selected sound tool

// File Tools setup
const int NUM_FILE_TOOLS_ITEMS = 1;  // Number of items in File Tools menu
const int FILE_TOOLS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int fileToolsViewportStart = 0;  // Index of first visible item
int selectedFileTool = 0;  // Currently selected file tool

// Tools sets setup
const int NUM_TOOL_SETS_ITEMS = 5;  // Number of items in Tool Sets menu
const int TOOL_SETS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int toolSetsViewportStart = 0;  // Index of first visible item
int selectedToolSet = 0;  // Currently selected tool set

// Your Programs setup
const int PROGRAMS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int programsViewportStart = 0;  // Index of first visible item
int selectedProgram = 0;  // Currently selected program
BinFileEntry binFiles[MAX_BIN_FILES];  // Array to store found .bin files
int binFileCount = 0;  // Number of .bin files found

// Dev tools setup
const int NUM_DEV_TOOLS_ITEMS = 1;  // Number of items in Dev Tools menu
const int DEV_TOOLS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int devToolsViewportStart = 0;  // Index of first visible item
int selectedDevTool = 0;  // Currently selected dev tool

// Connections setup
const int NUM_CONNECTIONS_ITEMS = 1;  // Number of items in Connections menu
const int CONNECTIONS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int connectionsViewportStart = 0;  // Index of first visible item
int selectedConnection = 0;  // Currently selected connection

// SnailOS Tools setup
const int NUM_SNAILOS_ITEMS = 1;  // Number of items in SnailOS Tools menu
const int SNAILOS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int snailOSViewportStart = 0;  // Index of first visible item
int selectedSnailOSTool = 0;  // Currently selected SnailOS tool

// Pre-made sounds setup
int selectedPredefinedSound = 0;
int predefinedSoundsViewportStart = 0;
const int PREDEFINED_SOUNDS_VISIBLE_ITEMS = 2;  // Number of items visible at once

// Get Programs setup
const int NUM_GET_PROGRAMS_ITEMS = 1;  // Number of items in Get Programs menu
const int GET_PROGRAMS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int getProgramsViewportStart = 0;  // Index of first visible item
int selectedGetProgram = 0;  // Currently selected program

// Function declarations
void drawMainMenu();
void drawFileExplorer();
void drawContextMenu();
void drawRenameScreen();
void handleMainMenuInput();
void handleFileExplorerInput();
void handleContextMenuInput();
void handleRenameInput();
void drawDisplayTools();
void updateDisplayToolsViewport();
void drawSettingsMenu();
void updateSettingsViewport();
void drawDisplaySettingsMenu();
void updateDisplaySettingsViewport();
void drawBootTextEditor();
void drawSoundTools();
void updateSoundToolsViewport();
void drawFileTools();
void updateFileToolsViewport();
void drawToolSets();
void updateToolSetsViewport();
void drawProgramsMenu();
void updateProgramsViewport();
void drawDevTools();
void updateDevToolsViewport();
void drawConnections();
void updateConnectionsViewport();
void drawSnailOSTools();
void updateSnailOSViewport();
void drawPredefinedSoundsSelect();
void updatePredefinedSoundsViewport();
void drawGetProgramsMenu();
void updateGetProgramsViewport();

// Function to ensure splash image exists
bool ensureSplashImageExists() {
    // Create directories if they don't exist
    if (!SD.exists("/SnailOS")) {
        if (!SD.mkdir("/SnailOS")) {
            Serial.println("Failed to create SnailOS directory");
            return false;
        }
    }
    if (!SD.exists("/SnailOS/Assets")) {
        if (!SD.mkdir("/SnailOS/Assets")) {
            Serial.println("Failed to create Assets directory");
            return false;
        }
    }
    
    // Check if either boot image exists
    return (SD.exists(bootSplashImage) || SD.exists(bootSplashImageAlt));
}

void drawMainMenu() {
    // Clear the entire screen first
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("SnailOS v0.1");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < VISIBLE_ITEMS; i++) {
        int itemIndex = viewportStart + i;
        if (itemIndex >= NUM_MENU_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedMenuItem) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(menuItems[itemIndex]);
        y += 50;
    }
}

void drawError(const String& error) {
    M5.Display.fillScreen(TFT_RED);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Error:");
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, 40);
    M5.Display.println(error);
    M5.Display.println("\nPress ESC to return");
}

void drawFileExplorer() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Check if SD card is mounted
    if (!isSDCardMounted()) {
        drawError("SD Card not mounted");
        return;
    }
    
    // Draw title and current path
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println(currentState == DISPLAY_IMAGE_SELECT ? "Select Image" : "File Explorer");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw current path (only in normal file explorer mode)
    if (currentState != DISPLAY_IMAGE_SELECT) {
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, 50);
    M5.Display.println(currentPath);
    }
    
    // Check if we have any files
    if (fileCount == 0) {
        M5.Display.setTextSize(2);
        M5.Display.setCursor(10, 70);
        M5.Display.println(currentState == DISPLAY_IMAGE_SELECT ? "No images found" : "Empty directory");
        return;
    }
    
    // Draw file list
    int y = currentState == DISPLAY_IMAGE_SELECT ? 60 : 70;  // Adjust y position based on mode
    int visibleCount = 0;
    for (int i = 0; i < fileCount; i++) {
        // Only show visible items
        if (i >= fileExplorerViewportStart && visibleCount < FILE_EXPLORER_VISIBLE_ITEMS) {
            // Draw file entry background
            if (i == selectedFileIndex) {
                M5.Display.fillRect(10, y, M5.Display.width() - 20, 30, TFT_BLACK);
                M5.Display.setTextColor(TFT_GREEN);
            } else {
                M5.Display.fillRect(10, y, M5.Display.width() - 20, 30, TFT_DARKGREEN);
                M5.Display.setTextColor(TFT_BLACK);
            }
            
            // Draw file/folder icon and name
            M5.Display.setTextSize(2);
            M5.Display.setCursor(20, y + 5);
            if (currentState == DISPLAY_IMAGE_SELECT) {
                M5.Display.print("[IMG] ");
            } else if (fileList[i].isDirectory) {
                M5.Display.print("[D] ");
            } else {
                M5.Display.print("[F] ");
            }
            M5.Display.println(fileList[i].name);
            
            y += 35;
            visibleCount++;
        }
    }
    
    // Draw scroll indicator if there are more files
    if (fileCount > FILE_EXPLORER_VISIBLE_ITEMS) {
        M5.Display.setTextSize(1);
        M5.Display.setTextColor(TFT_BLACK);
        M5.Display.setCursor(M5.Display.width() - 20, 10);
        M5.Display.printf("%d/%d", selectedFileIndex + 1, fileCount);
    }
}

void updateFileExplorerViewport() {
    // If selected item is above the viewport, move viewport up
    if (selectedFileIndex < fileExplorerViewportStart) {
        fileExplorerViewportStart = selectedFileIndex;
    }
    // If selected item is below the viewport, move viewport down
    else if (selectedFileIndex >= fileExplorerViewportStart + FILE_EXPLORER_VISIBLE_ITEMS) {
        fileExplorerViewportStart = selectedFileIndex - FILE_EXPLORER_VISIBLE_ITEMS + 1;
    }
    
    // Ensure viewport doesn't go beyond the list
    if (fileExplorerViewportStart < 0) {
        fileExplorerViewportStart = 0;
    }
    
    // Count valid items (directories + jpg files in DISPLAY_IMAGE_SELECT mode)
    int validItems = 0;
    for (int i = 0; i < fileCount; i++) {
        if (currentState != DISPLAY_IMAGE_SELECT || fileList[i].isDirectory || isJpgFile(fileList[i].name)) {
            validItems++;
        }
    }
    
    if (fileExplorerViewportStart > validItems - FILE_EXPLORER_VISIBLE_ITEMS) {
        fileExplorerViewportStart = validItems - FILE_EXPLORER_VISIBLE_ITEMS;
    }
    if (fileExplorerViewportStart < 0) {
        fileExplorerViewportStart = 0;
    }
}

void updateViewport() {
    // Ensure selected item is visible
    if (selectedMenuItem < viewportStart) {
        viewportStart = selectedMenuItem;
    } else if (selectedMenuItem >= viewportStart + VISIBLE_ITEMS) {
        viewportStart = selectedMenuItem - VISIBLE_ITEMS + 1;
    }
}

void updateContextMenuViewport() {
    // If selected item is above the viewport, move viewport up
    if (selectedContextMenuItem < contextMenuViewportStart) {
        contextMenuViewportStart = selectedContextMenuItem;
    }
    // If selected item is below the viewport, move viewport down
    else if (selectedContextMenuItem >= contextMenuViewportStart + CONTEXT_MENU_VISIBLE_ITEMS) {
        contextMenuViewportStart = selectedContextMenuItem - CONTEXT_MENU_VISIBLE_ITEMS + 1;
    }
    
    // Ensure viewport doesn't go beyond the list
    if (contextMenuViewportStart < 0) {
        contextMenuViewportStart = 0;
    }
    if (contextMenuViewportStart > NUM_CONTEXT_MENU_ITEMS - CONTEXT_MENU_VISIBLE_ITEMS) {
        contextMenuViewportStart = NUM_CONTEXT_MENU_ITEMS - CONTEXT_MENU_VISIBLE_ITEMS;
    }
    if (contextMenuViewportStart < 0) {
        contextMenuViewportStart = 0;
    }
}

void drawContextMenu() {
    // Draw semi-transparent background
    M5.Display.fillScreen(TFT_BLACK);
    
    // Draw menu title
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(10, 10);
    M5.Display.println("File Options");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_WHITE);
    
    // Draw menu items
    int y = 60;
    int visibleCount = 0;
    for (int i = 0; i < NUM_CONTEXT_MENU_ITEMS; i++) {
        // Skip Edit option for folders
        if (i == 0 && fileList[selectedFileIndex].isDirectory) {
            continue;
        }
        
        // Only draw visible items
        if (i >= contextMenuViewportStart && visibleCount < CONTEXT_MENU_VISIBLE_ITEMS) {
            // Draw menu item background
            if (i == selectedContextMenuItem) {
                M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_GREEN);
                M5.Display.setTextColor(TFT_BLACK);
            } else {
                M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
                M5.Display.setTextColor(TFT_WHITE);
            }
            
            // Draw menu item text
            M5.Display.setTextSize(2);
            M5.Display.setCursor(20, y + 10);
            M5.Display.println(contextMenuItems[i]);
            y += 50;
            visibleCount++;
        }
    }
    
    // Draw scroll indicator if there are more items
    if (NUM_CONTEXT_MENU_ITEMS > CONTEXT_MENU_VISIBLE_ITEMS) {
        M5.Display.setTextSize(1);
        M5.Display.setTextColor(TFT_WHITE);
        M5.Display.setCursor(M5.Display.width() - 20, 10);
        M5.Display.printf("%d/%d", selectedContextMenuItem + 1, NUM_CONTEXT_MENU_ITEMS);
    }
}

void drawRenameScreen() {
    // Clear screen
    M5.Display.fillScreen(TFT_BLACK);
    
    // Draw title
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Rename File");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_WHITE);
    
    // Draw instructions
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, 50);
    M5.Display.println("Type new name and press Enter");
    M5.Display.println("Backspace/ESC to delete characters");
    
    // Draw input box
    M5.Display.fillRect(10, 80, M5.Display.width() - 20, 40, TFT_DARKGREEN);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(20, 90);
    M5.Display.println(renameBuffer);
    
    // Draw cursor
    int cursorX = 20 + (renameBuffer.length() * 12);  // Approximate width of each character
    M5.Display.drawLine(cursorX, 90, cursorX, 110, TFT_WHITE);
}

void drawDisplayTools() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Display Tools");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < DISPLAY_TOOLS_VISIBLE_ITEMS; i++) {
        int itemIndex = displayToolsViewportStart + i;
        if (itemIndex >= NUM_DISPLAY_TOOLS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedDisplayTool) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(displayToolsItems[itemIndex]);
        y += 50;
    }
}

void updateDisplayToolsViewport() {
    // Ensure selected item is visible
    if (selectedDisplayTool < displayToolsViewportStart) {
        displayToolsViewportStart = selectedDisplayTool;
    } else if (selectedDisplayTool >= displayToolsViewportStart + DISPLAY_TOOLS_VISIBLE_ITEMS) {
        displayToolsViewportStart = selectedDisplayTool - DISPLAY_TOOLS_VISIBLE_ITEMS + 1;
    }
}

void drawSettingsMenu() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Settings");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < SETTINGS_VISIBLE_ITEMS; i++) {
        int itemIndex = settingsViewportStart + i;
        if (itemIndex >= NUM_SETTINGS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedSetting) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(displaySettingsItems[itemIndex]);
        y += 50;
    }
}

void updateSettingsViewport() {
    // Ensure selected item is visible
    if (selectedSetting < settingsViewportStart) {
        settingsViewportStart = selectedSetting;
    } else if (selectedSetting >= settingsViewportStart + SETTINGS_VISIBLE_ITEMS) {
        settingsViewportStart = selectedSetting - SETTINGS_VISIBLE_ITEMS + 1;
    }
}

void drawDisplaySettingsMenu() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Display Settings");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < DISPLAY_SETTINGS_VISIBLE_ITEMS; i++) {
        int itemIndex = displaySettingsViewportStart + i;
        if (itemIndex >= NUM_DISPLAY_SETTINGS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedDisplaySetting) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(displaySettingsDisplayItems[itemIndex]);
        y += 50;
    }
}

void updateDisplaySettingsViewport() {
    // Ensure selected item is visible
    if (selectedDisplaySetting < displaySettingsViewportStart) {
        displaySettingsViewportStart = selectedDisplaySetting;
    } else if (selectedDisplaySetting >= displaySettingsViewportStart + DISPLAY_SETTINGS_VISIBLE_ITEMS) {
        displaySettingsViewportStart = selectedDisplaySetting - DISPLAY_SETTINGS_VISIBLE_ITEMS + 1;
    }
}

void drawBootTextEditor() {
    // Clear screen
    M5.Display.fillScreen(TFT_BLACK);
    
    // Draw title
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Edit Boot Text");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_WHITE);
    
    // Draw instructions
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, 50);
    M5.Display.println("Type new boot text and press Enter");
    M5.Display.println("Backspace/ESC to delete characters");
    
    // Draw input box
    M5.Display.fillRect(10, 80, M5.Display.width() - 20, 40, TFT_DARKGREEN);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(20, 90);
    M5.Display.println(bootTextBuffer);
    
    // Draw cursor
    int cursorX = 20 + (bootTextBuffer.length() * 12);  // Approximate width of each character
    M5.Display.drawLine(cursorX, 90, cursorX, 110, TFT_WHITE);
}

// Add after the boot text variables
bool saveConfig() {
    // Create Data directory if it doesn't exist
    if (!SD.exists("/SnailOS/Data")) {
        if (!SD.mkdir("/SnailOS/Data")) {
            Serial.println("Failed to create Data directory");
            return false;
        }
    }
    
    // Open config file for writing
    File configFile = SD.open(configPath, FILE_WRITE);
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return false;
    }
    
    // Write boot text to config
    configFile.println("bootText=" + bootText);
    
    configFile.close();
    return true;
}

bool loadConfig() {
    // Check if config file exists
    if (!SD.exists(configPath)) {
        Serial.println("No config file found");
        return false;
    }
    
    // Open config file for reading
    File configFile = SD.open(configPath, FILE_READ);
    if (!configFile) {
        Serial.println("Failed to open config file for reading");
        return false;
    }
    
    // Read config file line by line
    while (configFile.available()) {
        String line = configFile.readStringUntil('\n');
        line.trim();  // Remove whitespace
        
        // Check for boot text setting
        if (line.startsWith("bootText=")) {
            bootText = line.substring(9);  // Remove "bootText=" prefix
        }
    }
    
    configFile.close();
    return true;
}

void drawSoundTools() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Sound Tools");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < SOUND_TOOLS_VISIBLE_ITEMS; i++) {
        int itemIndex = soundToolsViewportStart + i;
        if (itemIndex >= NUM_SOUND_TOOLS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedSoundTool) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(displaySoundItems[itemIndex]);
        y += 50;
    }
}

void updateSoundToolsViewport() {
    // Ensure selected item is visible
    if (selectedSoundTool < soundToolsViewportStart) {
        soundToolsViewportStart = selectedSoundTool;
    } else if (selectedSoundTool >= soundToolsViewportStart + SOUND_TOOLS_VISIBLE_ITEMS) {
        soundToolsViewportStart = selectedSoundTool - SOUND_TOOLS_VISIBLE_ITEMS + 1;
    }
}

void drawFileTools() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("File Tools");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < FILE_TOOLS_VISIBLE_ITEMS; i++) {
        int itemIndex = fileToolsViewportStart + i;
        if (itemIndex >= NUM_FILE_TOOLS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedFileTool) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(displayFileItems[itemIndex]);
        y += 50;
    }
}

void updateFileToolsViewport() {
    // Ensure selected item is visible
    if (selectedFileTool < fileToolsViewportStart) {
        fileToolsViewportStart = selectedFileTool;
    } else if (selectedFileTool >= fileToolsViewportStart + FILE_TOOLS_VISIBLE_ITEMS) {
        fileToolsViewportStart = selectedFileTool - FILE_TOOLS_VISIBLE_ITEMS + 1;
    }
}

void drawToolSets() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Tool Sets");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < TOOL_SETS_VISIBLE_ITEMS; i++) {
        int itemIndex = toolSetsViewportStart + i;
        if (itemIndex >= NUM_TOOL_SETS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedToolSet) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(MainToolsItems[itemIndex]);
        y += 50;
    }
}

void updateToolSetsViewport() {
    // Ensure selected item is visible
    if (selectedToolSet < toolSetsViewportStart) {
        toolSetsViewportStart = selectedToolSet;
    } else if (selectedToolSet >= toolSetsViewportStart + TOOL_SETS_VISIBLE_ITEMS) {
        toolSetsViewportStart = selectedToolSet - TOOL_SETS_VISIBLE_ITEMS + 1;
    }
}

void drawProgramsMenu() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Programs");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Check if we have any programs
    if (binFileCount == 0) {
        M5.Display.setTextSize(2);
        M5.Display.setCursor(10, 70);
        M5.Display.println("No .bin files found");
        return;
    }
    
    // Draw program list
    int y = 60;
    for (int i = 0; i < PROGRAMS_VISIBLE_ITEMS; i++) {
        int itemIndex = programsViewportStart + i;
        if (itemIndex >= binFileCount) break;
        
        // Draw button background
        if (itemIndex == selectedProgram) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw program name
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(binFiles[itemIndex].name);
        y += 50;
    }
    
    // Draw scroll indicator if there are more programs
    if (binFileCount > PROGRAMS_VISIBLE_ITEMS) {
        M5.Display.setTextSize(1);
        M5.Display.setTextColor(TFT_BLACK);
        M5.Display.setCursor(M5.Display.width() - 20, 10);
        M5.Display.printf("%d/%d", selectedProgram + 1, binFileCount);
    }
}

void updateProgramsViewport() {
    // Ensure selected item is visible
    if (selectedProgram < programsViewportStart) {
        programsViewportStart = selectedProgram;
    } else if (selectedProgram >= programsViewportStart + PROGRAMS_VISIBLE_ITEMS) {
        programsViewportStart = selectedProgram - PROGRAMS_VISIBLE_ITEMS + 1;
    }
}

void drawDevTools() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Dev Tools");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < DEV_TOOLS_VISIBLE_ITEMS; i++) {
        int itemIndex = devToolsViewportStart + i;
        if (itemIndex >= NUM_DEV_TOOLS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedDevTool) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(DevToolsItems[itemIndex]);
        y += 50;
    }
}

void updateDevToolsViewport() {
    // Ensure selected item is visible
    if (selectedDevTool < devToolsViewportStart) {
        devToolsViewportStart = selectedDevTool;
    } else if (selectedDevTool >= devToolsViewportStart + DEV_TOOLS_VISIBLE_ITEMS) {
        devToolsViewportStart = selectedDevTool - DEV_TOOLS_VISIBLE_ITEMS + 1;
    }
}

void drawConnections() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Connections");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < CONNECTIONS_VISIBLE_ITEMS; i++) {
        int itemIndex = connectionsViewportStart + i;
        if (itemIndex >= NUM_CONNECTIONS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedConnection) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(ConnectionItems[itemIndex]);
        y += 50;
    }
}

void updateConnectionsViewport() {
    // Ensure selected item is visible
    if (selectedConnection < connectionsViewportStart) {
        connectionsViewportStart = selectedConnection;
    } else if (selectedConnection >= connectionsViewportStart + CONNECTIONS_VISIBLE_ITEMS) {
        connectionsViewportStart = selectedConnection - CONNECTIONS_VISIBLE_ITEMS + 1;
    }
}

void drawSnailOSTools() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("SnailOS Tools");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < SNAILOS_VISIBLE_ITEMS; i++) {
        int itemIndex = snailOSViewportStart + i;
        if (itemIndex >= NUM_SNAILOS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedSnailOSTool) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(SnailOSItems[itemIndex]);
        y += 50;
    }
}

void updateSnailOSViewport() {
    // Ensure selected item is visible
    if (selectedSnailOSTool < snailOSViewportStart) {
        snailOSViewportStart = selectedSnailOSTool;
    } else if (selectedSnailOSTool >= snailOSViewportStart + SNAILOS_VISIBLE_ITEMS) {
        snailOSViewportStart = selectedSnailOSTool - SNAILOS_VISIBLE_ITEMS + 1;
    }
}

void drawPredefinedSoundsSelect() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Pre Made Sounds");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < PREDEFINED_SOUNDS_VISIBLE_ITEMS; i++) {
        int itemIndex = predefinedSoundsViewportStart + i;
        if (itemIndex >= NUM_PREDEFINED_SOUNDS) break;
        
        // Draw button background
        if (itemIndex == selectedPredefinedSound) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(PREDEFINED_SOUNDS[itemIndex].name);
        y += 50;
    }
}

void updatePredefinedSoundsViewport() {
    // Ensure selected item is visible
    if (selectedPredefinedSound < predefinedSoundsViewportStart) {
        predefinedSoundsViewportStart = selectedPredefinedSound;
    } else if (selectedPredefinedSound >= predefinedSoundsViewportStart + PREDEFINED_SOUNDS_VISIBLE_ITEMS) {
        predefinedSoundsViewportStart = selectedPredefinedSound - PREDEFINED_SOUNDS_VISIBLE_ITEMS + 1;
    }
}

void drawGetProgramsMenu() {
    // Clear screen
    M5.Display.fillScreen(TFT_GREEN);
    
    // Draw title and border
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLACK);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Get Programs");
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw menu items
    int y = 60;
    for (int i = 0; i < GET_PROGRAMS_VISIBLE_ITEMS; i++) {
        int itemIndex = getProgramsViewportStart + i;
        if (itemIndex >= NUM_GET_PROGRAMS_ITEMS) break;
        
        // Draw button background
        if (itemIndex == selectedGetProgram) {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_BLACK);
            M5.Display.setTextColor(TFT_GREEN);
        } else {
            M5.Display.fillRect(10, y, M5.Display.width() - 20, 40, TFT_DARKGREEN);
            M5.Display.setTextColor(TFT_BLACK);
        }
        
        // Draw menu item
        M5.Display.setTextSize(2);
        M5.Display.setCursor(20, y + 10);
        M5.Display.println(GetProgramsItems[itemIndex]);
        y += 50;
    }
}

void updateGetProgramsViewport() {
    if (selectedGetProgram < getProgramsViewportStart) {
        getProgramsViewportStart = selectedGetProgram;
    } else if (selectedGetProgram >= getProgramsViewportStart + GET_PROGRAMS_VISIBLE_ITEMS) {
        getProgramsViewportStart = selectedGetProgram - GET_PROGRAMS_VISIBLE_ITEMS + 1;
    }
}

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    M5Cardputer.begin(cfg);
    
    // Set screen rotation to 1
    M5.Display.setRotation(1);
    
    // Initialize SD card first
    if (!initSDCard()) {
        drawError("SD Card Error:\nNo card detected or\ncard not accessible");
        delay(2000);  // Show error for 2 seconds
    } else {
        // Create directories if they don't exist
        ensureSplashImageExists();
        
        // Try to load config
        loadConfig();  // If it fails, bootText will keep its default value
        
        // Display splash screen or fallback message
        M5.Display.fillScreen(TFT_BLACK);
        if (SD.exists(bootSplashImage)) {
            drawImage(bootSplashImage);
            delay(4000);  // Show splash for 4 seconds
        } else if (SD.exists(bootSplashImageAlt)) {
            drawImage(bootSplashImageAlt);
            delay(4000);  // Show splash for 4 seconds
        } else {
            // Show boot text message
            M5.Display.setTextColor(TFT_WHITE);
            M5.Display.setTextSize(2);
            M5.Display.setTextDatum(middle_center);
            M5.Display.drawString(bootText, M5.Display.width()/2, M5.Display.height()/2);
            delay(2000);  // Show message for 2 seconds
        }
    }
    
    // Clear screen and set background color to green
    M5.Display.fillScreen(TFT_GREEN);
    
    // Set text color to black for better contrast on green
    M5.Display.setTextColor(TFT_BLACK);
    
    // Set text size and position for the title
    M5.Display.setTextSize(2);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Snail's Tools v0.1");
    
    // Draw a line under the title
    M5.Display.drawLine(10, 40, M5.Display.width() - 10, 40, TFT_BLACK);
    
    // Draw initial menu
    drawMainMenu();
}

void loop() {
    M5.update();
    M5Cardputer.update();

    // Handle keyboard input
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            // Handle menu navigation
            if (currentState == MAIN_MENU) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedMenuItem = (selectedMenuItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
                    updateViewport();
                    drawMainMenu();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedMenuItem = (selectedMenuItem + 1) % NUM_MENU_ITEMS;
                    updateViewport();
                    drawMainMenu();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedMenuItem) {
                        case 0:  // SnailOS Tools
                            currentState = SNAILGOTCHI;
                            selectedSnailOSTool = 0;
                            snailOSViewportStart = 0;
                            drawSnailOSTools();
                            break;
                        case 1:  // Connections
                            currentState = CONNECTIONS;
                            selectedConnection = 0;
                            connectionsViewportStart = 0;
                            drawConnections();
                            break;
                        case 2:  // Tool Sets
                            currentState = TOOL_SETS;
                            selectedToolSet = 0;
                            toolSetsViewportStart = 0;
                            drawToolSets();
                            break;
                        case 3:  // Your Programs
                            currentState = PROGRAMS;
                            // Scan for .bin files
                            binFileCount = scanBinFiles(binFiles, MAX_BIN_FILES);
                            selectedProgram = 0;
                            programsViewportStart = 0;
                            drawProgramsMenu();
                            break;
                        case 4:  // Get Programs
                            currentState = GET_PROGRAMS;
                            selectedGetProgram = 0;
                            getProgramsViewportStart = 0;
                            drawGetProgramsMenu();
                            break;
                        case 5:  // Settings
                            currentState = SETTINGS;
                            selectedSetting = 0;
                            settingsViewportStart = 0;
                            drawSettingsMenu();
                            break;
                    }
                }
            }
            else if (currentState == FILE_EXPLORER || currentState == DISPLAY_IMAGE_SELECT || currentState == SOUND_FILE_SELECT) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    if (fileCount > 0) {
                        selectedFileIndex = (selectedFileIndex - 1 + fileCount) % fileCount;
                        updateFileExplorerViewport();
                        drawFileExplorer();
                    }
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    if (fileCount > 0) {
                        selectedFileIndex = (selectedFileIndex + 1) % fileCount;
                        updateFileExplorerViewport();
                        drawFileExplorer();
                    }
                }
                // Left or Right key - open context menu (only in normal file explorer mode)
                else if ((M5Cardputer.Keyboard.isKeyPressed(0x2F) || M5Cardputer.Keyboard.isKeyPressed(0x2C)) && 
                         currentState == FILE_EXPLORER) {
                    if (fileCount > 0) {
                        selectedContextMenuItem = 0;
                        // Skip Edit option for folders
                        if (fileList[selectedFileIndex].isDirectory && selectedContextMenuItem == 0) {
                            selectedContextMenuItem = 1;
                        }
                        currentState = FILE_EXPLORER_CONTEXT_MENU;
                        drawContextMenu();
                    }
                }
                // Enter key - navigate into directory or select file
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    if (fileCount > 0 && selectedFileIndex < fileCount) {
                        if (fileList[selectedFileIndex].isDirectory) {
                            String newPath = fileList[selectedFileIndex].path;
                            int tempCount = 0;
                            FileEntry tempEntries[MAX_FILES];
                            readDirectory(newPath, tempEntries, tempCount);
                            
                            if (tempCount >= 0) {
                                currentPath = newPath;
                                selectedFileIndex = 0;
                                fileExplorerViewportStart = 0;
                                readDirectory(currentPath, fileList, fileCount);
                                sortFileList(fileList, fileCount);
                                drawFileExplorer();
                            } else {
                                drawError("Cannot access directory");
                                delay(1000);
                                drawFileExplorer();
                            }
                        } else if (currentState == DISPLAY_IMAGE_SELECT && isJpgFile(fileList[selectedFileIndex].name)) {
                            // Display the selected image
                            currentState = DISPLAY_IMAGE;
                            drawImage(fileList[selectedFileIndex].path);
                        } else if (currentState == SOUND_FILE_SELECT && isWavFile(fileList[selectedFileIndex].name)) {
                            // Show playing message first
                                M5.Display.fillScreen(TFT_BLACK);
                                M5.Display.setTextColor(TFT_WHITE);
                                M5.Display.setTextSize(2);
                                M5.Display.setTextDatum(middle_center);
                                M5.Display.drawString("Playing WAV File", M5.Display.width()/2, M5.Display.height()/2 - 20);
                                M5.Display.setTextSize(1);
                                M5.Display.drawString(fileList[selectedFileIndex].name, M5.Display.width()/2, M5.Display.height()/2 + 10);
                                M5.Display.drawString("Press any key to stop", M5.Display.width()/2, M5.Display.height()/2 + 30);
                                
                            // Small delay to ensure the screen is updated
                            delay(50);
                            
                            // Now play the WAV file
                            if (playWavFile(fileList[selectedFileIndex].path)) {
                                // Wait for any key press to stop
                                while (true) {
                                    M5.update();
                                    M5Cardputer.update();
                                    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                                        stopSound();
                                        break;
                                    }
                                    delay(50);
                                }
                                
                                // Return to sound tools menu
                                currentState = SOUND_TOOLS;
                                drawSoundTools();
                            } else {
                                drawError("Failed to play WAV file");
                                delay(1000);
                                drawFileExplorer();
                            }
                        }
                    }
                }
                // Backspace key - go up one directory
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A)) {
                    if (currentPath != "/") {
                        String newPath = currentPath.substring(0, currentPath.lastIndexOf('/'));
                        if (newPath.isEmpty()) newPath = "/";
                        
                        int tempCount = 0;
                        FileEntry tempEntries[MAX_FILES];
                        readDirectory(newPath, tempEntries, tempCount);
                        
                        if (tempCount >= 0) {
                            currentPath = newPath;
                            selectedFileIndex = 0;
                            fileExplorerViewportStart = 0;
                            readDirectory(currentPath, fileList, fileCount);
                            sortFileList(fileList, fileCount);
                            drawFileExplorer();
                        } else {
                            drawError("Cannot access parent directory");
                            delay(1000);
                            drawFileExplorer();
                        }
                    }
                }
                // ESC key - return to appropriate menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    if (currentState == DISPLAY_IMAGE_SELECT) {
                        currentState = DISPLAY_TOOLS;
                        drawDisplayTools();
                    } else {
                        currentState = MAIN_MENU;
                        drawMainMenu();
                    }
                }
            }
            else if (currentState == FILE_EXPLORER_CONTEXT_MENU) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedContextMenuItem = (selectedContextMenuItem - 1 + NUM_CONTEXT_MENU_ITEMS) % NUM_CONTEXT_MENU_ITEMS;
                    // Skip Edit option for folders
                    if (selectedContextMenuItem == 0 && fileList[selectedFileIndex].isDirectory) {
                        selectedContextMenuItem = (selectedContextMenuItem - 1 + NUM_CONTEXT_MENU_ITEMS) % NUM_CONTEXT_MENU_ITEMS;
                    }
                    updateContextMenuViewport();
                    drawContextMenu();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedContextMenuItem = (selectedContextMenuItem + 1) % NUM_CONTEXT_MENU_ITEMS;
                    // Skip Edit option for folders
                    if (selectedContextMenuItem == 0 && fileList[selectedFileIndex].isDirectory) {
                        selectedContextMenuItem = (selectedContextMenuItem + 1) % NUM_CONTEXT_MENU_ITEMS;
                    }
                    updateContextMenuViewport();
                    drawContextMenu();
                }
                // Enter key - select option
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    // Handle menu actions
                    if (selectedContextMenuItem == 2) { // Delete option (now index 2 instead of 4)
                        String path = fileList[selectedFileIndex].path;
                        if (deleteFileOrFolder(path)) {
                            // Refresh the file list
                            readDirectory(currentPath, fileList, fileCount);
                            sortFileList(fileList, fileCount);
                            // Reset selection to avoid out of bounds
                            if (selectedFileIndex >= fileCount) {
                                selectedFileIndex = fileCount - 1;
                            }
                            if (selectedFileIndex < 0) {
                                selectedFileIndex = 0;
                            }
                            currentState = FILE_EXPLORER;
                            drawFileExplorer();
                        } else {
                            drawError("Failed to delete file/folder");
                            delay(1000);
                            currentState = FILE_EXPLORER;
                            drawFileExplorer();
                        }
                    }
                    else if (selectedContextMenuItem == 1) { // Rename option
                        // Initialize rename buffer with current filename
                        String currentPath = fileList[selectedFileIndex].path;
                        int lastSlash = currentPath.lastIndexOf('/');
                        renameBuffer = currentPath.substring(lastSlash + 1);
                        currentState = FILE_EXPLORER_RENAME;
                        drawRenameScreen();
                    }
                    else {
                        // TODO: Implement other menu actions
                        currentState = FILE_EXPLORER;
                        drawFileExplorer();
                    }
                }
                // Backspace key - return to file explorer
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A)) {
                    currentState = FILE_EXPLORER;
                    drawFileExplorer();
                }
                // ESC key - return to file explorer
                else if (M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = FILE_EXPLORER;
                    drawFileExplorer();
                }
            }
            else if (currentState == FILE_EXPLORER_RENAME) {
                // Handle special keys
                if (M5Cardputer.Keyboard.isKeyPressed(0x28)) { // Enter key
                    // Rename the file
                    String currentPath = fileList[selectedFileIndex].path;
                    if (renameFileOrFolder(currentPath, renameBuffer)) {
                        // Refresh the file list
                        readDirectory(currentPath.substring(0, currentPath.lastIndexOf('/')), fileList, fileCount);
                        sortFileList(fileList, fileCount);
                        currentState = FILE_EXPLORER;
                        drawFileExplorer();
                    } else {
                        drawError("Failed to rename file/folder");
                        delay(1000);
                        currentState = FILE_EXPLORER;
                        drawFileExplorer();
                    }
                }
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) { // Backspace or ESC
                    if (renameBuffer.length() > 0) {
                        renameBuffer = renameBuffer.substring(0, renameBuffer.length() - 1);
                        drawRenameScreen();
                    }
                }
                // Handle regular keys
                else {
                    // Get the key that was pressed
                    for (int i = 0; i < 128; i++) {
                        if (M5Cardputer.Keyboard.isKeyPressed(i)) {
                            // Only accept printable ASCII characters
                            if (i >= 32 && i <= 126) {
                                if (renameBuffer.length() < 32) { // Limit filename length
                                    renameBuffer += (char)i;
                                    drawRenameScreen();
                                }
                            }
                            break;
                        }
                    }
                }
            }
            else if (currentState == DISPLAY_TOOLS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedDisplayTool = (selectedDisplayTool - 1 + NUM_DISPLAY_TOOLS_ITEMS) % NUM_DISPLAY_TOOLS_ITEMS;
                    updateDisplayToolsViewport();
                    drawDisplayTools();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedDisplayTool = (selectedDisplayTool + 1) % NUM_DISPLAY_TOOLS_ITEMS;
                    updateDisplayToolsViewport();
                    drawDisplayTools();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedDisplayTool) {
                        case 0:  // Display Image
                            currentState = DISPLAY_IMAGE_SELECT;
                            // Initialize file explorer for image selection
                            currentPath = "/";
                            selectedFileIndex = 0;
                            fileExplorerViewportStart = 0;
                            // Use the new scanImageFiles function instead of readDirectory
                            fileCount = scanImageFiles(fileList, MAX_FILES);
                            sortFileList(fileList, fileCount);
                            drawFileExplorer();
                            break;
                        case 1:  // Boot Image
                            if (SD.exists(bootSplashImage)) {
                                currentState = DISPLAY_IMAGE;
                                drawImage(bootSplashImage);
                            } else if (SD.exists(bootSplashImageAlt)) {
                                currentState = DISPLAY_IMAGE;
                                drawImage(bootSplashImageAlt);
                            } else {
                                // Show error message
                                M5.Display.fillScreen(TFT_BLACK);
                                M5.Display.setTextColor(TFT_WHITE);
                                M5.Display.setTextSize(2);
                                M5.Display.setTextDatum(middle_center);
                                M5.Display.drawString("No Boot Image Found", M5.Display.width()/2, M5.Display.height()/2 - 20);
                                M5.Display.setTextSize(1);
                                M5.Display.drawString("Place boot.jpg or Boot.jpg in", M5.Display.width()/2, M5.Display.height()/2 + 10);
                                M5.Display.drawString("/SnailOS/Assets/", M5.Display.width()/2, M5.Display.height()/2 + 30);
                                delay(2000);  // Show message for 2 seconds
                                drawDisplayTools();  // Return to display tools menu
                            }
                            break;
                        case 2:  // Boot Text
                            // Show boot text preview
                            M5.Display.fillScreen(TFT_BLACK);
                            M5.Display.setTextColor(TFT_WHITE);
                            M5.Display.setTextSize(2);
                            M5.Display.setTextDatum(middle_center);
                            M5.Display.drawString(bootText, M5.Display.width()/2, M5.Display.height()/2);
                            
                            // Wait for any key press
                            while (true) {
                                M5.update();
                                M5Cardputer.update();
                                if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                                    break;
                                }
                                delay(50);
                            }
                            
                            // Reset text settings to defaults before returning
                            M5.Display.setTextDatum(top_left);  // Reset text alignment
                            M5.Display.setTextColor(TFT_BLACK);  // Reset text color
                            
                            // Return to display tools menu
                            drawDisplayTools();
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            else if (currentState == DISPLAY_IMAGE) {
                // Backspace or ESC key - return to display tools menu
                if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = DISPLAY_TOOLS;
                    drawDisplayTools();
                }
            }
            else if (currentState == SETTINGS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedSetting = (selectedSetting - 1 + NUM_SETTINGS_ITEMS) % NUM_SETTINGS_ITEMS;
                    updateSettingsViewport();
                    drawSettingsMenu();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedSetting = (selectedSetting + 1) % NUM_SETTINGS_ITEMS;
                    updateSettingsViewport();
                    drawSettingsMenu();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedSetting) {
                        case 0:  // Reboot
                            ESP.restart();
                            break;
                        case 1:  // Display
                            currentState = DISPLAY_SETTINGS;
                            selectedDisplaySetting = 0;  // Reset selection to first item
                            displaySettingsViewportStart = 0;  // Reset viewport
                            drawDisplaySettingsMenu();  // Draw the menu immediately
                            break;
                        case 2:  // Sound
                            // TODO: Implement sound settings
                            break;
                        case 3:  // Reset Config
                            // TODO: Implement config reset
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for display settings menu
            else if (currentState == DISPLAY_SETTINGS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedDisplaySetting = (selectedDisplaySetting - 1 + NUM_DISPLAY_SETTINGS_ITEMS) % NUM_DISPLAY_SETTINGS_ITEMS;
                    updateDisplaySettingsViewport();
                    drawDisplaySettingsMenu();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedDisplaySetting = (selectedDisplaySetting + 1) % NUM_DISPLAY_SETTINGS_ITEMS;
                    updateDisplaySettingsViewport();
                    drawDisplaySettingsMenu();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedDisplaySetting) {
                        case 0:  // Edit Boot Text
                            currentState = BOOT_TEXT_EDIT;
                            bootTextBuffer = bootText;  // Initialize buffer with current boot text
                            drawBootTextEditor();
                            break;
                    }
                }
                // Backspace or ESC key - return to settings menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = SETTINGS;
                    drawSettingsMenu();
                }
            }
            // Add new handler for boot text editing
            else if (currentState == BOOT_TEXT_EDIT) {
                // Handle special keys
                if (M5Cardputer.Keyboard.isKeyPressed(0x28)) { // Enter key
                    // Save the new boot text
                    bootText = bootTextBuffer;
                    // Save to config file
                    if (!saveConfig()) {
                        drawError("Failed to save config");
                        delay(1000);
                    }
                    // Return to display settings menu
                    currentState = DISPLAY_SETTINGS;
                    drawDisplaySettingsMenu();
                }
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) { // Backspace or ESC
                    if (bootTextBuffer.length() > 0) {
                        bootTextBuffer = bootTextBuffer.substring(0, bootTextBuffer.length() - 1);
                        drawBootTextEditor();
                    } else {
                        // If buffer is empty and ESC is pressed, return to display settings
                        currentState = DISPLAY_SETTINGS;
                        drawDisplaySettingsMenu();
                    }
                }
                // Handle regular keys
                else {
                    // Get the key that was pressed
                    for (int i = 0; i < 128; i++) {
                        if (M5Cardputer.Keyboard.isKeyPressed(i)) {
                            // Only accept printable ASCII characters
                            if (i >= 32 && i <= 126) {
                                if (bootTextBuffer.length() < 32) { // Limit text length
                                    bootTextBuffer += (char)i;
                                    drawBootTextEditor();
                                }
                            }
                            break;
                        }
                    }
                }
            }
            else if (currentState == SOUND_TOOLS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedSoundTool = (selectedSoundTool - 1 + NUM_SOUND_TOOLS_ITEMS) % NUM_SOUND_TOOLS_ITEMS;
                    updateSoundToolsViewport();
                    drawSoundTools();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedSoundTool = (selectedSoundTool + 1) % NUM_SOUND_TOOLS_ITEMS;
                    updateSoundToolsViewport();
                    drawSoundTools();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedSoundTool) {
                        case 0:  // Pre Made Sounds
                            currentState = PREDEFINED_SOUNDS_SELECT;
                            selectedPredefinedSound = 0;
                            predefinedSoundsViewportStart = 0;
                            drawPredefinedSoundsSelect();
                            break;
                        case 1:  // Play WAV File
                            currentState = SOUND_FILE_SELECT;
                            // Initialize file explorer for WAV selection
                            currentPath = "/";
                            selectedFileIndex = 0;
                            fileExplorerViewportStart = 0;
                            readDirectory(currentPath, fileList, fileCount, isWavFile);  // Use WAV filter
                            sortFileList(fileList, fileCount);
                            drawFileExplorer();
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for WAV file selection
            else if (currentState == SOUND_FILE_SELECT) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    if (fileCount > 0) {
                        // Find previous valid file/folder
                        int newIndex = selectedFileIndex;
                        do {
                            newIndex = (newIndex - 1 + fileCount) % fileCount;
                        } while (!fileList[newIndex].isDirectory && !isWavFile(fileList[newIndex].name));
                        
                        selectedFileIndex = newIndex;
                        updateFileExplorerViewport();
                        drawFileExplorer();
                    }
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    if (fileCount > 0) {
                        // Find next valid file/folder
                        int newIndex = selectedFileIndex;
                        do {
                            newIndex = (newIndex + 1) % fileCount;
                        } while (!fileList[newIndex].isDirectory && !isWavFile(fileList[newIndex].name));
                        
                        selectedFileIndex = newIndex;
                        updateFileExplorerViewport();
                        drawFileExplorer();
                    }
                }
                // Enter key - navigate into directory or play WAV file
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    if (fileCount > 0 && selectedFileIndex < fileCount) {
                        if (fileList[selectedFileIndex].isDirectory) {
                            String newPath = fileList[selectedFileIndex].path;
                            int tempCount = 0;
                            FileEntry tempEntries[MAX_FILES];
                            readDirectory(newPath, tempEntries, tempCount);
                            
                            if (tempCount >= 0) {
                                currentPath = newPath;
                                selectedFileIndex = 0;
                                fileExplorerViewportStart = 0;
                                readDirectory(currentPath, fileList, fileCount);
                                sortFileList(fileList, fileCount);
                                drawFileExplorer();
                            } else {
                                drawError("Cannot access directory");
                                delay(1000);
                                drawFileExplorer();
                            }
                        } else if (isWavFile(fileList[selectedFileIndex].name)) {
                            // Show playing message first
                                M5.Display.fillScreen(TFT_BLACK);
                                M5.Display.setTextColor(TFT_WHITE);
                                M5.Display.setTextSize(2);
                                M5.Display.setTextDatum(middle_center);
                                M5.Display.drawString("Playing WAV File", M5.Display.width()/2, M5.Display.height()/2 - 20);
                                M5.Display.setTextSize(1);
                                M5.Display.drawString(fileList[selectedFileIndex].name, M5.Display.width()/2, M5.Display.height()/2 + 10);
                                M5.Display.drawString("Press any key to stop", M5.Display.width()/2, M5.Display.height()/2 + 30);
                                
                            // Small delay to ensure the screen is updated
                            delay(50);
                            
                            // Now play the WAV file
                            if (playWavFile(fileList[selectedFileIndex].path)) {
                                // Wait for any key press to stop
                                while (true) {
                                    M5.update();
                                    M5Cardputer.update();
                                    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                                        stopSound();
                                        break;
                                    }
                                    delay(50);
                                }
                                
                                // Return to sound tools menu
                                currentState = SOUND_TOOLS;
                                drawSoundTools();
                            } else {
                                drawError("Failed to play WAV file");
                                delay(1000);
                                drawFileExplorer();
                            }
                        }
                    }
                }
                // Backspace key - go up one directory
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A)) {
                    if (currentPath != "/") {
                        String newPath = currentPath.substring(0, currentPath.lastIndexOf('/'));
                        if (newPath.isEmpty()) newPath = "/";
                        
                        int tempCount = 0;
                        FileEntry tempEntries[MAX_FILES];
                        readDirectory(newPath, tempEntries, tempCount);
                        
                        if (tempCount >= 0) {
                            currentPath = newPath;
                            selectedFileIndex = 0;
                            fileExplorerViewportStart = 0;
                            readDirectory(currentPath, fileList, fileCount);
                            sortFileList(fileList, fileCount);
                            drawFileExplorer();
                        } else {
                            drawError("Cannot access parent directory");
                            delay(1000);
                            drawFileExplorer();
                        }
                    }
                }
                // ESC key - return to sound tools menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = SOUND_TOOLS;
                    drawSoundTools();
                }
            }
            // Add new handler for File Tools menu
            else if (currentState == FILE_TOOLS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedFileTool = (selectedFileTool - 1 + NUM_FILE_TOOLS_ITEMS) % NUM_FILE_TOOLS_ITEMS;
                    updateFileToolsViewport();
                    drawFileTools();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedFileTool = (selectedFileTool + 1) % NUM_FILE_TOOLS_ITEMS;
                    updateFileToolsViewport();
                    drawFileTools();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedFileTool) {
                        case 0:  // File Explorer
                            currentState = FILE_EXPLORER;
                            // Initialize file explorer
                            currentPath = "/";
                            selectedFileIndex = 0;
                            fileExplorerViewportStart = 0;
                            readDirectory(currentPath, fileList, fileCount);
                            sortFileList(fileList, fileCount);
                            drawFileExplorer();
                            break;
                        case 1:  // FTP File Transfer
                            // TODO: Implement FTP transfer
                            break;
                        case 2:  // USB File Transfer
                            // TODO: Implement USB transfer
                            break;
                        case 3:  // Reset Config
                            // TODO: Implement config reset
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for Tool Sets menu
            else if (currentState == TOOL_SETS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedToolSet = (selectedToolSet - 1 + NUM_TOOL_SETS_ITEMS) % NUM_TOOL_SETS_ITEMS;
                    updateToolSetsViewport();
                    drawToolSets();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedToolSet = (selectedToolSet + 1) % NUM_TOOL_SETS_ITEMS;
                    updateToolSetsViewport();
                    drawToolSets();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedToolSet) {
                        case 0:  // File Tools
                            currentState = FILE_TOOLS;
                            selectedFileTool = 0;
                            fileToolsViewportStart = 0;
                            drawFileTools();
                            break;
                        case 1:  // Dev Tools
                            currentState = DEV_TOOLS;
                            selectedDevTool = 0;
                            devToolsViewportStart = 0;
                            drawDevTools();
                            break;
                        case 2:  // Display Tools
                            currentState = DISPLAY_TOOLS;
                            selectedDisplayTool = 0;
                            displayToolsViewportStart = 0;
                            drawDisplayTools();
                            break;
                        case 3:  // Sound Tools
                            currentState = SOUND_TOOLS;
                            selectedSoundTool = 0;
                            soundToolsViewportStart = 0;
                            drawSoundTools();
                            break;
                        case 4:  // WIFI Tools
                            // Removed WiFi connection from here as it's now in Connections menu
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for Programs menu
            else if (currentState == PROGRAMS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    if (binFileCount > 0) {
                        selectedProgram = (selectedProgram - 1 + binFileCount) % binFileCount;
                        updateProgramsViewport();
                        drawProgramsMenu();
                    }
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    if (binFileCount > 0) {
                        selectedProgram = (selectedProgram + 1) % binFileCount;
                        updateProgramsViewport();
                        drawProgramsMenu();
                    }
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    if (binFileCount > 0 && selectedProgram < binFileCount) {
                        // Show message that firmware flashing is disabled
                        M5.Display.fillScreen(TFT_BLACK);
                        M5.Display.setTextColor(TFT_WHITE);
                        M5.Display.setTextSize(2);
                        M5.Display.setTextDatum(middle_center);
                        M5.Display.drawString("Firmware Flashing", M5.Display.width()/2, M5.Display.height()/2 - 30);
                        M5.Display.setTextSize(1);
                        M5.Display.drawString("Feature Disabled", M5.Display.width()/2, M5.Display.height()/2 - 10);
                        M5.Display.drawString(binFiles[selectedProgram].name, M5.Display.width()/2, M5.Display.height()/2 + 10);
                        M5.Display.drawString("Press any key to continue", M5.Display.width()/2, M5.Display.height()/2 + 30);
                        
                        // Wait for any key press
                        while (true) {
                            M5.update();
                            M5Cardputer.update();
                            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                                drawProgramsMenu();
                                break;
                            }
                            delay(50);
                        }
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for Dev Tools menu
            else if (currentState == DEV_TOOLS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedDevTool = (selectedDevTool - 1 + NUM_DEV_TOOLS_ITEMS) % NUM_DEV_TOOLS_ITEMS;
                    updateDevToolsViewport();
                    drawDevTools();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedDevTool = (selectedDevTool + 1) % NUM_DEV_TOOLS_ITEMS;
                    updateDevToolsViewport();
                    drawDevTools();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedDevTool) {
                        case 0:  // Run C++ Script
                            // TODO: Implement C++ script running
                            break;
                        case 1:  // Run Java Script
                            // TODO: Implement JavaScript running
                            break;
                        case 2:  // Run MP Script
                            // TODO: Implement MicroPython script running
                            break;
                    }
                }
                // Backspace or ESC key - return to tool sets menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = TOOL_SETS;
                    drawToolSets();
                }
            }
            // Add new handler for Connections menu
            else if (currentState == CONNECTIONS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedConnection = (selectedConnection - 1 + NUM_CONNECTIONS_ITEMS) % NUM_CONNECTIONS_ITEMS;
                    updateConnectionsViewport();
                    drawConnections();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedConnection = (selectedConnection + 1) % NUM_CONNECTIONS_ITEMS;
                    updateConnectionsViewport();
                    drawConnections();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedConnection) {
                        case 0:  // WebGUI
                            handleWiFiConnection();  // Call the WiFi connection handler
                            if (isWiFiConnected()) {
                                startWebGUIServer();  // Start the WebGUI server
                                
                                // Keep server running until ESC is pressed
                                while (true) {
                                    M5Cardputer.update();
                                    updateWebGUIServer();  // Handle any server updates
                                    
                                    if (M5Cardputer.Keyboard.isKeyPressed(0x60)) {  // ESC key
                                        stopWebGUIServer();
                                        break;
                                    }
                                    delay(50);
                                }
                            }
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for SnailOS Tools menu (after the Connections handler)
            else if (currentState == SNAILGOTCHI) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedSnailOSTool = (selectedSnailOSTool - 1 + NUM_SNAILOS_ITEMS) % NUM_SNAILOS_ITEMS;
                    updateSnailOSViewport();
                    drawSnailOSTools();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedSnailOSTool = (selectedSnailOSTool + 1) % NUM_SNAILOS_ITEMS;
                    updateSnailOSViewport();
                    drawSnailOSTools();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedSnailOSTool) {
                        case 0:  // SnailHub
                            // TODO: Implement SnailHub
                            break;
                        case 1:  // SnailGPT
                            // TODO: Implement SnailGPT
                            break;
                        case 2:  // SnailGotchi
                            // TODO: Implement SnailGotchi
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for predefined sounds selection
            else if (currentState == PREDEFINED_SOUNDS_SELECT) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedPredefinedSound = (selectedPredefinedSound - 1 + NUM_PREDEFINED_SOUNDS) % NUM_PREDEFINED_SOUNDS;
                    updatePredefinedSoundsViewport();
                    drawPredefinedSoundsSelect();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedPredefinedSound = (selectedPredefinedSound + 1) % NUM_PREDEFINED_SOUNDS;
                    updatePredefinedSoundsViewport();
                    drawPredefinedSoundsSelect();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    // Show playing message first
                    M5.Display.fillScreen(TFT_BLACK);
                    M5.Display.setTextColor(TFT_WHITE);
                    M5.Display.setTextSize(2);
                    M5.Display.setTextDatum(middle_center);
                    M5.Display.drawString("Playing Sound", M5.Display.width()/2, M5.Display.height()/2 - 20);
                    M5.Display.setTextSize(1);
                    M5.Display.drawString(PREDEFINED_SOUNDS[selectedPredefinedSound].name, M5.Display.width()/2, M5.Display.height()/2 + 10);
                    M5.Display.drawString("Press any key to stop", M5.Display.width()/2, M5.Display.height()/2 + 30);
                    
                    // Small delay to ensure the screen is updated
                    delay(50);
                    
                    // Now play the selected sound
                    playPredefinedSound(selectedPredefinedSound);
                    
                    // Wait for any key press to stop
                    while (true) {
                        M5.update();
                        M5Cardputer.update();
                        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                            stopSound();
                            break;
                        }
                        delay(50);
                    }
                    
                    // Return to predefined sounds menu
                    drawPredefinedSoundsSelect();
                }
                // Backspace or ESC key - return to sound tools menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = SOUND_TOOLS;
                    drawSoundTools();
                }
            }
            // Add new handler for Get Programs menu
            else if (currentState == GET_PROGRAMS) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedGetProgram = (selectedGetProgram - 1 + NUM_GET_PROGRAMS_ITEMS) % NUM_GET_PROGRAMS_ITEMS;
                    updateGetProgramsViewport();
                    drawGetProgramsMenu();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedGetProgram = (selectedGetProgram + 1) % NUM_GET_PROGRAMS_ITEMS;
                    updateGetProgramsViewport();
                    drawGetProgramsMenu();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    switch (selectedGetProgram) {
                        case 0:  // Snail's Store
                            currentState = FIRMWARE_STORE;
                            initFirmwareStore();  // Initialize the firmware store
                            drawFirmwareStore();  // Draw the firmware store menu
                            break;
                    }
                }
                // Backspace or ESC key - return to main menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = MAIN_MENU;
                    drawMainMenu();
                }
            }
            // Add new handler for Firmware Store menu (after the GET_PROGRAMS handler)
            else if (currentState == FIRMWARE_STORE) {
                // Up key
                if (M5Cardputer.Keyboard.isKeyPressed(0x3B)) {
                    selectedFirmware = (selectedFirmware - 1 + firmwareList.size()) % firmwareList.size();
                    updateFirmwareStoreViewport();
                    drawFirmwareStore();
                }
                // Down key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2E)) {
                    selectedFirmware = (selectedFirmware + 1) % firmwareList.size();
                    updateFirmwareStoreViewport();
                    drawFirmwareStore();
                }
                // Enter key
                else if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
                    handleFirmwareStoreInput();
                }
                // Backspace or ESC key - return to Get Programs menu
                else if (M5Cardputer.Keyboard.isKeyPressed(0x2A) || M5Cardputer.Keyboard.isKeyPressed(0x60)) {
                    currentState = GET_PROGRAMS;
                    drawGetProgramsMenu();
                }
            }
        }
    }

    delay(100);
} 