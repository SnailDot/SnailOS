#ifndef __SDCARD_FUNCTIONS_H
#define __SDCARD_FUNCTIONS_H

#include <M5Unified.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>

// SD Card pins for M5StampS3
#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12

// Constants
const int MAX_FILES = 100;  // Maximum number of files to display
const size_t BUFFER_SIZE = 4096;  // Buffer size for file operations

// File entry structure
struct FileEntry {
    String name;
    String path;
    bool isDirectory;
};

// SD Card Functions
bool initSDCard();
void closeSDCard();
bool isSDCardMounted();

// File Operations
bool deleteFileOrFolder(const String& path);
bool renameFileOrFolder(const String& path, const String& newName);
bool copyFile(const String& sourcePath, const String& destPath);
bool createFolder(const String& path);

// Directory Navigation
void readDirectory(const String& path, FileEntry* entries, int& count, bool (*filterFunc)(const String&) = nullptr);
void sortFileList(FileEntry* entries, int count);

// File Browser State
extern String currentPath;
extern FileEntry* fileList;
extern int fileCount;
extern int selectedFileIndex;
extern bool isFilePickerMode;

// Add new declarations for bin file handling
const int MAX_BIN_FILES = 20;
struct BinFileEntry {
    String name;
    String path;
};

// Add function declarations for bin file handling
int scanBinFiles(BinFileEntry binFiles[], int maxFiles);
void scanDirectoryForBin(const String& dirPath, BinFileEntry binFiles[], int& count, int maxFiles);
bool updateFirmware(const String& binPath);

// File type filtering functions
bool isJpgFile(const String& filename);
bool isWavFile(const String& filename);
void filterFilesByType(FileEntry* entries, int& count, bool (*filterFunc)(const String&));

// Add new declarations for image file scanning
void scanDirectoryForImages(const String& dirPath, FileEntry entries[], int& count, int maxFiles);
int scanImageFiles(FileEntry entries[], int maxFiles);

#endif 