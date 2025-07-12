#include "DisplayTools.h"
#include <JPEGDecoder.h>

void drawImage(const String& imagePath) {
    // Clear screen
    M5.Display.fillScreen(TFT_BLACK);
    
    // Display loading message
    M5.Display.setTextSize(1);
    M5.Display.setTextColor(TFT_WHITE);
    M5.Display.setCursor(10, 10);
    M5.Display.println("Loading image...");
    M5.Display.setCursor(10, 20);
    M5.Display.println(imagePath);
    
    // Try to open the image file using the provided path
    File jpegFile = SD.open(imagePath);
    if (!jpegFile) {
        M5.Display.setTextColor(TFT_RED);
        M5.Display.setCursor(10, 40);
        M5.Display.println("Failed to open file");
        delay(2000);
        return;
    }
    
    // Display file size
    uint32_t fileSize = jpegFile.size();
    M5.Display.setCursor(10, 30);
    M5.Display.printf("File size: %d bytes", fileSize);
    
    // Clear screen before drawing
    M5.Display.fillScreen(TFT_BLACK);
    
    // Use the simpler drawJpg method
    bool success = M5.Display.drawJpg((Stream*)&jpegFile);
    
    if (!success) {
        M5.Display.setTextColor(TFT_RED);
        M5.Display.setCursor(10, 10);
        M5.Display.println("Failed to draw image");
        delay(2000);
    }
    
    jpegFile.close();
} 