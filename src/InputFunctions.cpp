#include "InputFunctions.h"

String getPasswordInput(const String& prompt) {
    String input = "";
    bool done = false;
    
    while (!done) {
        M5Cardputer.update();
        
        // Handle keyboard input
        handleKeyboardInput(input, false);
        
        // Draw the input screen
        drawKeyboardInput(input, prompt, false);
        
        // Check for enter key to finish (0x28 is the key code for Enter)
        if (M5Cardputer.Keyboard.isKeyPressed(0x28)) {
            done = true;
        }
        
        // Check for escape key to cancel (0x60 is the key code for ESC)
        if (M5Cardputer.Keyboard.isKeyPressed(0x60)) {
            return "";
        }
        
        delay(50);  // Small delay to prevent CPU hogging
    }
    
    return input;
}

void handleKeyboardInput(String& input, bool isPassword) {
    // Handle backspace (0x2A is the key code for Backspace)
    if (M5Cardputer.Keyboard.isKeyPressed(0x2A)) {
        if (input.length() > 0) {
            input = input.substring(0, input.length() - 1);
        }
        delay(200); // Prevent multiple backspaces
    }
    
    // Handle regular key input
    for (int i = 0; i < 128; i++) {
        if (M5Cardputer.Keyboard.isKeyPressed(i)) {
            // Only accept printable ASCII characters
            if (i >= 32 && i <= 126) {
                if (input.length() < 32) { // Limit password length
                    input += (char)i;
                }
            }
            break;
        }
    }
}

void drawKeyboardInput(const String& input, const String& prompt, bool isPassword) {
    M5.Display.fillScreen(TFT_BLACK);  // Changed from M5Cardputer.Lcd to M5.Display
    M5.Display.setTextColor(TFT_WHITE);  // Changed from WHITE to TFT_WHITE
    M5.Display.setTextSize(2);
    
    // Draw prompt
    M5.Display.setCursor(10, 10);
    M5.Display.println(prompt);
    
    // Draw input box
    M5.Display.drawRect(10, 50, M5.Display.width() - 20, 40, TFT_WHITE);  // Changed from WHITE to TFT_WHITE
    
    // Draw input text (masked if password)
    M5.Display.setCursor(20, 60);
    if (isPassword) {
        for (int i = 0; i < input.length(); i++) {
            M5.Display.print("*");
        }
    } else {
        M5.Display.print(input);
    }
    
    // Draw cursor
    if (millis() % 1000 < 500) {
        int cursorX = 20 + (isPassword ? input.length() : input.length()) * 12;
        M5.Display.drawLine(cursorX, 60, cursorX, 80, TFT_WHITE);  // Changed from WHITE to TFT_WHITE
    }
    
    // Draw help text
    M5.Display.setTextSize(1);
    M5.Display.setCursor(10, M5.Display.height() - 30);
    M5.Display.println("Enter: Confirm  Esc: Cancel");
} 