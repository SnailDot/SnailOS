#ifndef __FIRMWARE_H
#define __FIRMWARE_H

#include <M5Unified.h>
#include <SD.h>
#include <Update.h>

// Firmware update status codes
enum class UpdateStatus {
    SUCCESS,
    ERROR_NO_SD_CARD,
    ERROR_FILE_NOT_FOUND,
    ERROR_FILE_OPEN,
    ERROR_FILE_READ,
    ERROR_UPDATE_BEGIN,
    ERROR_UPDATE_WRITE,
    ERROR_UPDATE_END,
    ERROR_NOT_ENOUGH_SPACE
};

// Function to perform firmware update from a .bin file on SD card
UpdateStatus performFirmwareUpdate(const String& binPath);

// Function to get a human-readable error message for update status
String getUpdateErrorMessage(UpdateStatus status);

#endif 