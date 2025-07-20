#ifndef JPEGDECODER_WRAPPER_H
#define JPEGDECODER_WRAPPER_H

// Include necessary filesystem headers first
#include <FS.h>
#include <LittleFS.h>

// Define SPIFFS as LittleFS before including JPEGDecoder
#define SPIFFS LittleFS

// Now include the JPEGDecoder
#include <JPEGDecoder.h>

// Undefine SPIFFS to avoid conflicts
#undef SPIFFS

#endif // JPEGDECODER_WRAPPER_H 