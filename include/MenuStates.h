#pragma once

// Menu states
enum MenuState {
    MAIN_MENU,
    SNAILGOTCHI,
    CONNECTIONS,  // Add new state for Connections menu
    TOOL_SETS,  // Add new state for Tool Sets submenu
    PROGRAMS,  // Add new state for Programs menu
    GET_PROGRAMS,  // Add new state for Get Programs menu
    FIRMWARE_STORE,  // Add new state for Firmware Store
    DEV_TOOLS,
    DISPLAY_TOOLS,
    SOUND_TOOLS,
    PREDEFINED_SOUNDS_SELECT,  // New state for predefined sounds selection
    FILE_TOOLS,  // Add new state for File Tools submenu
    FILE_EXPLORER,
    FILE_EXPLORER_CONTEXT_MENU,
    FILE_EXPLORER_RENAME,
    DISPLAY_IMAGE,
    DISPLAY_IMAGE_SELECT,
    SOUND_FILE_SELECT,  // State for WAV file selection
    FTP_SERVER,
    WIFI_TOOLS,
    BLE_TOOLS,
    IR_TOOLS,
    SETTINGS,
    DISPLAY_SETTINGS,
    BOOT_TEXT_EDIT
}; 