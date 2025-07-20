#ifndef __INPUT_FUNCTIONS_H
#define __INPUT_FUNCTIONS_H

#include <M5Cardputer.h>
#include <string>

// Function declarations
String getPasswordInput(const String& prompt);
void handleKeyboardInput(String& input, bool isPassword = false);
void drawKeyboardInput(const String& input, const String& prompt, bool isPassword = false);

#endif // __INPUT_FUNCTIONS_H 