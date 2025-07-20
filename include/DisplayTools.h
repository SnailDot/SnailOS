#pragma once

#include <M5Unified.h>
#include <M5Cardputer.h>
#include <utility/Keyboard.h>
#include <vector>
#include <string>
#include "SDcardFunctions.h"
#include "MenuStates.h"  // Include the new header with MenuState enum
#include "JPEGDecoderWrapper.h"  // Use our wrapper instead of JPEGDecoder directly
#include <SD.h>

// Forward declarations
void drawError(const String& error);

// Display Tools menu items
extern const char* displayToolsItems[];
extern const int NUM_DISPLAY_TOOLS_ITEMS;
extern const int DISPLAY_TOOLS_VISIBLE_ITEMS;

// Display Tools state variables
extern int displayToolsViewportStart;
extern int selectedDisplayTool;
extern MenuState currentState;  // Reference to the state from main.cpp

// Function declarations
void drawDisplayTools();
void updateDisplayToolsViewport();
void drawImage(const String& imagePath);
void drawMainMenu();
void drawFileExplorer();
void drawContextMenu();
void drawRenameScreen();
void drawSettingsMenu();
void updateSettingsViewport();
void drawDisplaySettingsMenu();
void updateDisplaySettingsViewport();
void drawBootTextEditor();
void drawSoundTools();
void updateSoundToolsViewport(); 