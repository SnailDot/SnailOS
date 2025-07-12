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
#include "BootFunctions.h"  // Add the new boot functions header
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

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

// Menu items
const char* menuItems[] = {
    "SnailOS Tools",
    "Connections",
    "Tool Sets",
    "Your Programs",
    "Get Programs",
    "Extra",
    "Settings"
};
const int NUM_MENU_ITEMS = 7;  // Update from 10 to 4 to match new menu items
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
const int NUM_CONTEXT_MENU_ITEMS = 3;  // Reduced from 5 to 3
const int CONTEXT_MENU_VISIBLE_ITEMS = 2;  // Number of items visible at once
int contextMenuViewportStart = 0;  // Index of first visible item

// Display Tools menu items
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
    "Sound",
    "Reset Config"
};

const char* displayFileItems[] = {
    "File Explorer",
    "Backup Device",
    "Clear SD Card"
};


const char* displaySettingsDisplayItems[] = {
    "Edit Boot Text"
};

const char* MainToolsItems[] = {  // Renamed from displayToolsItems
    "File Tools",
    "Dev Tools",
    "Display Tools",
    "Sound Tools",
    "LED Tools"
};

const char* ConnectionItems[] = {  // Renamed from displayToolsItems
    "USB Keyboard",
    "WebGUI"
};

const char* SnailOSItems[] = {  // Renamed from displayToolsItems
    "SnailHub",
   "SnailGotchi",
   "Snail VITA",
   "Pebble Snail"
};

const char* DevToolsItems[] = {  // Renamed from displayToolsItems
    "Input Test",
    "Run Java Script"
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
    "Scan All BlE",
    "Scan Open BLE",
    "Monitor BLE"
};

const char* GetProgramsItems[] = {  // Renamed from displayToolsItems
    "Snail's Store",
    "M5 Store"
};

// Add after the displaySettingsDisplayItems array
const int NUM_DISPLAY_SETTINGS_ITEMS = 1;
const int DISPLAY_SETTINGS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int displaySettingsViewportStart = 0;  // Index of first visible item
int selectedDisplaySetting = 0;  // Currently selected display setting

const int NUM_DISPLAY_TOOLS_ITEMS = 3;
const int DISPLAY_TOOLS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int displayToolsViewportStart = 0;  // Index of first visible item
int selectedDisplayTool = 0;  // Currently selected display tool

// Add after the menuItems array
const int NUM_SETTINGS_ITEMS = 4;
const int SETTINGS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int settingsViewportStart = 0;  // Index of first visible item
int selectedSetting = 0;  // Currently selected setting

// Add after the displaySoundItems array
const int NUM_SOUND_TOOLS_ITEMS = 1;  // Updated to match new menu items
const int SOUND_TOOLS_VISIBLE_ITEMS = 1;  // Number of items visible at once
int soundToolsViewportStart = 0;  // Index of first visible item
int selectedSoundTool = 0;  // Currently selected sound tool

// Add after other menu item constants
const int NUM_FILE_TOOLS_ITEMS = 3;  // Number of items in File Tools menu
const int FILE_TOOLS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int fileToolsViewportStart = 0;  // Index of first visible item
int selectedFileTool = 0;  // Currently selected file tool

// Add after other menu item constants
const int NUM_TOOL_SETS_ITEMS = 5;  // Number of items in Tool Sets menu
const int TOOL_SETS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int toolSetsViewportStart = 0;  // Index of first visible item
int selectedToolSet = 0;  // Currently selected tool set

// Add after other menu item constants
const int PROGRAMS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int programsViewportStart = 0;  // Index of first visible item
int selectedProgram = 0;  // Currently selected program
BinFileEntry binFiles[MAX_BIN_FILES];  // Array to store found .bin files
int binFileCount = 0;  // Number of .bin files found

// Add after other menu item constants
const int NUM_DEV_TOOLS_ITEMS = 2;  // Number of items in Dev Tools menu
const int DEV_TOOLS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int devToolsViewportStart = 0;  // Index of first visible item
int selectedDevTool = 0;  // Currently selected dev tool

// Add after other menu item constants
const int NUM_CONNECTIONS_ITEMS = 2;  // Number of items in Connections menu
const int CONNECTIONS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int connectionsViewportStart = 0;  // Index of first visible item
int selectedConnection = 0;  // Currently selected connection

// Add after other menu item constants
const int NUM_SNAILOS_ITEMS = 4;  // Number of items in SnailOS Tools menu
const int SNAILOS_VISIBLE_ITEMS = 2;  // Number of items visible at once
int snailOSViewportStart = 0;  // Index of first visible item
int selectedSnailOSTool = 0;  // Currently selected SnailOS tool

// Add new variables for predefined sounds selection
int selectedPredefinedSound = 0;
int predefinedSoundsViewportStart = 0;
const int PREDEFINED_SOUNDS_VISIBLE_ITEMS = 2;  // Number of items visible at once

// Add after other menu item constants
const int NUM_GET_PROGRAMS_ITEMS = 2;  // Number of items in Get Programs menu
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
        displayBootScreen();
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

    // Check for any key press
    if (M5Cardputer.Keyboard.isPressed()) {  // Changed from isKeyPressed() to isPressed()
        // Main menu handler
        if (currentState == MAIN_MENU) {
            // ... rest of the menu handlers ...
        }
    }
    
    delay(100);
} 