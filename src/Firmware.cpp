#include "Firmware.h"
#include "SDcardFunctions.h"

UpdateStatus performFirmwareUpdate(const String& binPath) {
    // Check if SD card is mounted
    if (!isSDCardMounted()) {
        return UpdateStatus::ERROR_NO_SD_CARD;
    }

    // Check if file exists
    if (!SD.exists(binPath)) {
        return UpdateStatus::ERROR_FILE_NOT_FOUND;
    }

    // Open the update file
    File updateBin = SD.open(binPath);
    if (!updateBin) {
        return UpdateStatus::ERROR_FILE_OPEN;
    }

    if (updateBin.isDirectory()) {
        updateBin.close();
        return UpdateStatus::ERROR_FILE_NOT_FOUND;
    }

    size_t updateSize = updateBin.size();
    if (updateSize == 0) {
        updateBin.close();
        return UpdateStatus::ERROR_FILE_READ;
    }

    // Check if there's enough space
    if (updateSize > UPDATE_SIZE_UNKNOWN) {
        updateBin.close();
        return UpdateStatus::ERROR_NOT_ENOUGH_SPACE;
    }

    // Begin update
    if (!Update.begin(updateSize)) {
        updateBin.close();
        return UpdateStatus::ERROR_UPDATE_BEGIN;
    }

    // Write update data
    size_t written = Update.writeStream(updateBin);
    if (written != updateSize) {
        updateBin.close();
        return UpdateStatus::ERROR_UPDATE_WRITE;
    }

    // End update
    if (!Update.end()) {
        updateBin.close();
        return UpdateStatus::ERROR_UPDATE_END;
    }

    updateBin.close();
    return UpdateStatus::SUCCESS;
}

String getUpdateErrorMessage(UpdateStatus status) {
    switch (status) {
        case UpdateStatus::SUCCESS:
            return "Update successful";
        case UpdateStatus::ERROR_NO_SD_CARD:
            return "SD card not mounted";
        case UpdateStatus::ERROR_FILE_NOT_FOUND:
            return "Update file not found";
        case UpdateStatus::ERROR_FILE_OPEN:
            return "Failed to open update file";
        case UpdateStatus::ERROR_FILE_READ:
            return "Failed to read update file";
        case UpdateStatus::ERROR_UPDATE_BEGIN:
            return "Failed to begin update";
        case UpdateStatus::ERROR_UPDATE_WRITE:
            return "Failed to write update";
        case UpdateStatus::ERROR_UPDATE_END:
            return "Failed to finalize update";
        case UpdateStatus::ERROR_NOT_ENOUGH_SPACE:
            return "Not enough space for update";
        default:
            return "Unknown error";
    }
} 