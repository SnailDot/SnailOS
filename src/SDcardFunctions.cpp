#include "SDcardFunctions.h"
#include "Firmware.h"

// Global variables
String currentPath = "/";
FileEntry* fileList = new FileEntry[MAX_FILES];
int fileCount = 0;
int selectedFileIndex = 0;
bool isFilePickerMode = false;
bool sdCardMounted = false;

bool initSDCard() {
    // Initialize SPI for SD card
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
    
    // Try to mount the SD card
    if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
        sdCardMounted = false;
        return false;
    }
    
    // Check if the card is actually mounted
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        sdCardMounted = false;
        return false;
    }
    
    sdCardMounted = true;
    return true;
}

void closeSDCard() {
    SD.end();
    sdCardMounted = false;
}

bool isSDCardMounted() {
    return sdCardMounted;
}

bool deleteFileOrFolder(const String& path) {
    if (!sdCardMounted) return false;
    
    File file = SD.open(path);
    if (!file) return false;

    if (file.isDirectory()) {
        file.close();
        File dir = SD.open(path);
        File entry = dir.openNextFile();
        
        while (entry) {
            String entryPath = path + "/" + entry.name();
            if (entry.isDirectory()) {
                deleteFileOrFolder(entryPath);
            } else {
                SD.remove(entryPath);
            }
            entry = dir.openNextFile();
        }
        dir.close();
        return SD.rmdir(path);
    } else {
        file.close();
        return SD.remove(path);
    }
}

bool renameFileOrFolder(const String& path, const String& newName) {
    if (!sdCardMounted) return false;
    
    String newPath = path.substring(0, path.lastIndexOf('/') + 1) + newName;
    return SD.rename(path, newPath);
}

bool copyFile(const String& sourcePath, const String& destPath) {
    if (!sdCardMounted) return false;
    
    File sourceFile = SD.open(sourcePath, FILE_READ);
    if (!sourceFile) return false;

    File destFile = SD.open(destPath, FILE_WRITE);
    if (!destFile) {
        sourceFile.close();
        return false;
    }

    uint8_t buffer[BUFFER_SIZE];
    size_t bytesRead;
    size_t totalBytes = 0;
    size_t fileSize = sourceFile.size();

    while ((bytesRead = sourceFile.read(buffer, BUFFER_SIZE)) > 0) {
        if (destFile.write(buffer, bytesRead) != bytesRead) {
            sourceFile.close();
            destFile.close();
            return false;
        }
        totalBytes += bytesRead;
    }

    sourceFile.close();
    destFile.close();
    return true;
}

bool createFolder(const String& path) {
    if (!sdCardMounted) return false;
    return SD.mkdir(path);
}

// File type filtering functions
bool isJpgFile(const String& filename) {
    String lowerName = filename;
    lowerName.toLowerCase();
    return lowerName.endsWith(".jpg") || lowerName.endsWith(".jpeg");
}

bool isWavFile(const String& filename) {
    String lowerName = filename;
    lowerName.toLowerCase();
    return lowerName.endsWith(".wav");
}

void filterFilesByType(FileEntry* entries, int& count, bool (*filterFunc)(const String&)) {
    int writeIndex = 0;
    
    // Move all matching files to the front of the array
    for (int readIndex = 0; readIndex < count; readIndex++) {
        if (entries[readIndex].isDirectory || filterFunc(entries[readIndex].name)) {
            if (writeIndex != readIndex) {
                entries[writeIndex] = entries[readIndex];
            }
            writeIndex++;
        }
    }
    
    // Update the count to reflect the filtered list
    count = writeIndex;
}

// Update readDirectory to use filtering
void readDirectory(const String& path, FileEntry* entries, int& count, bool (*filterFunc)(const String&)) {
    if (!sdCardMounted) {
        count = 0;
        return;
    }

    File dir = SD.open(path);
    if (!dir || !dir.isDirectory()) {
        count = 0;
        return;
    }

    count = 0;
    File entry = dir.openNextFile();
    
    while (entry && count < MAX_FILES) {
        entries[count].name = entry.name();
        entries[count].path = path + "/" + entry.name();
        entries[count].isDirectory = entry.isDirectory();
        count++;
        entry = dir.openNextFile();
    }
    
    dir.close();

    // Apply filter if one was provided
    if (filterFunc != nullptr) {
        filterFilesByType(entries, count, filterFunc);
    }
}

void sortFileList(FileEntry* entries, int count) {
    // Simple bubble sort - directories first, then alphabetical
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            bool swap = false;
            
            // Directories come first
            if (!entries[j].isDirectory && entries[j + 1].isDirectory) {
                swap = true;
            }
            // If both are same type, sort alphabetically
            else if (entries[j].isDirectory == entries[j + 1].isDirectory) {
                if (entries[j].name.compareTo(entries[j + 1].name) > 0) {
                    swap = true;
                }
            }
            
            if (swap) {
                FileEntry temp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = temp;
            }
        }
    }
}

// Add function to scan for .bin files
int scanBinFiles(BinFileEntry binFiles[], int maxFiles) {
    int count = 0;
    if (!isSDCardMounted()) return 0;
    
    // Start from root directory
    scanDirectoryForBin("/", binFiles, count, maxFiles);
    return count;
}

// Helper function to recursively scan directories
void scanDirectoryForBin(const String& dirPath, BinFileEntry binFiles[], int& count, int maxFiles) {
    File dir = SD.open(dirPath);
    if (!dir) return;
    
    if (!dir.isDirectory()) {
        dir.close();
        return;
    }
    
    File entry = dir.openNextFile();
    while (entry && count < maxFiles) {
        String entryPath = entry.path();
        
        if (entry.isDirectory()) {
            // Recursively scan subdirectories
            scanDirectoryForBin(entryPath, binFiles, count, maxFiles);
        } else if (entryPath.endsWith(".bin") || entryPath.endsWith(".BIN")) {
            // Found a .bin file
            binFiles[count].name = entry.name();
            binFiles[count].path = entryPath;
            count++;
        }
        
        entry = dir.openNextFile();
    }
    
    dir.close();
}

// Update the updateFirmware function to use the new Firmware module
bool updateFirmware(const String& binPath) {
    UpdateStatus status = performFirmwareUpdate(binPath);
    if (status == UpdateStatus::SUCCESS) {
        return true;
    } else {
        Serial.println("Firmware update failed: " + getUpdateErrorMessage(status));
        return false;
    }
}

// Add after scanDirectoryForBin function
void scanDirectoryForImages(const String& dirPath, FileEntry entries[], int& count, int maxFiles) {
    File dir = SD.open(dirPath);
    if (!dir) return;
    
    if (!dir.isDirectory()) {
        dir.close();
        return;
    }
    
    File entry = dir.openNextFile();
    while (entry && count < maxFiles) {
        String entryPath = entry.path();
        
        if (entry.isDirectory()) {
            // Recursively scan subdirectories
            scanDirectoryForImages(entryPath, entries, count, maxFiles);
        } else if (isJpgFile(entry.name())) {
            // Found a JPG file
            entries[count].name = entry.name();
            entries[count].path = entryPath;
            entries[count].isDirectory = false;
            count++;
        }
        
        entry = dir.openNextFile();
    }
    
    dir.close();
}

// Add new function to scan for all JPG files
int scanImageFiles(FileEntry entries[], int maxFiles) {
    int count = 0;
    if (!isSDCardMounted()) return 0;
    
    // Start from root directory
    scanDirectoryForImages("/", entries, count, maxFiles);
    return count;
} 