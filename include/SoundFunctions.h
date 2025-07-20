#pragma once

#include <M5Unified.h>
#include <SD.h>
#include <M5Cardputer.h>

// Undefine any existing note definitions to avoid conflicts
#ifdef NOTE_B0
#undef NOTE_B0
#endif
#ifdef NOTE_C1
#undef NOTE_C1
#endif
#ifdef NOTE_CS1
#undef NOTE_CS1
#endif
#ifdef NOTE_D1
#undef NOTE_D1
#endif
#ifdef NOTE_DS1
#undef NOTE_DS1
#endif
#ifdef NOTE_E1
#undef NOTE_E1
#endif
#ifdef NOTE_F1
#undef NOTE_F1
#endif
#ifdef NOTE_FS1
#undef NOTE_FS1
#endif
#ifdef NOTE_G1
#undef NOTE_G1
#endif
#ifdef NOTE_GS1
#undef NOTE_GS1
#endif
#ifdef NOTE_A1
#undef NOTE_A1
#endif
#ifdef NOTE_AS1
#undef NOTE_AS1
#endif
#ifdef NOTE_B1
#undef NOTE_B1
#endif
#ifdef NOTE_C2
#undef NOTE_C2
#endif
#ifdef NOTE_CS2
#undef NOTE_CS2
#endif
#ifdef NOTE_D2
#undef NOTE_D2
#endif
#ifdef NOTE_DS2
#undef NOTE_DS2
#endif
#ifdef NOTE_E2
#undef NOTE_E2
#endif
#ifdef NOTE_F2
#undef NOTE_F2
#endif
#ifdef NOTE_FS2
#undef NOTE_FS2
#endif
#ifdef NOTE_G2
#undef NOTE_G2
#endif
#ifdef NOTE_GS2
#undef NOTE_GS2
#endif
#ifdef NOTE_A2
#undef NOTE_A2
#endif
#ifdef NOTE_AS2
#undef NOTE_AS2
#endif
#ifdef NOTE_B2
#undef NOTE_B2
#endif
#ifdef NOTE_C3
#undef NOTE_C3
#endif
#ifdef NOTE_CS3
#undef NOTE_CS3
#endif
#ifdef NOTE_D3
#undef NOTE_D3
#endif
#ifdef NOTE_DS3
#undef NOTE_DS3
#endif
#ifdef NOTE_E3
#undef NOTE_E3
#endif
#ifdef NOTE_F3
#undef NOTE_F3
#endif
#ifdef NOTE_FS3
#undef NOTE_FS3
#endif
#ifdef NOTE_G3
#undef NOTE_G3
#endif
#ifdef NOTE_GS3
#undef NOTE_GS3
#endif
#ifdef NOTE_A3
#undef NOTE_A3
#endif
#ifdef NOTE_AS3
#undef NOTE_AS3
#endif
#ifdef NOTE_B3
#undef NOTE_B3
#endif
#ifdef NOTE_C4
#undef NOTE_C4
#endif
#ifdef NOTE_CS4
#undef NOTE_CS4
#endif
#ifdef NOTE_D4
#undef NOTE_D4
#endif
#ifdef NOTE_DS4
#undef NOTE_DS4
#endif
#ifdef NOTE_E4
#undef NOTE_E4
#endif
#ifdef NOTE_F4
#undef NOTE_F4
#endif
#ifdef NOTE_FS4
#undef NOTE_FS4
#endif
#ifdef NOTE_G4
#undef NOTE_G4
#endif
#ifdef NOTE_GS4
#undef NOTE_GS4
#endif
#ifdef NOTE_A4
#undef NOTE_A4
#endif
#ifdef NOTE_AS4
#undef NOTE_AS4
#endif
#ifdef NOTE_B4
#undef NOTE_B4
#endif
#ifdef NOTE_C5
#undef NOTE_C5
#endif
#ifdef NOTE_CS5
#undef NOTE_CS5
#endif
#ifdef NOTE_D5
#undef NOTE_D5
#endif
#ifdef NOTE_DS5
#undef NOTE_DS5
#endif
#ifdef NOTE_E5
#undef NOTE_E5
#endif
#ifdef NOTE_F5
#undef NOTE_F5
#endif
#ifdef NOTE_FS5
#undef NOTE_FS5
#endif
#ifdef NOTE_G5
#undef NOTE_G5
#endif
#ifdef NOTE_GS5
#undef NOTE_GS5
#endif
#ifdef NOTE_A5
#undef NOTE_A5
#endif
#ifdef NOTE_AS5
#undef NOTE_AS5
#endif
#ifdef NOTE_B5
#undef NOTE_B5
#endif
#ifdef NOTE_C6
#undef NOTE_C6
#endif
#ifdef NOTE_CS6
#undef NOTE_CS6
#endif
#ifdef NOTE_D6
#undef NOTE_D6
#endif
#ifdef NOTE_DS6
#undef NOTE_DS6
#endif
#ifdef NOTE_E6
#undef NOTE_E6
#endif
#ifdef NOTE_F6
#undef NOTE_F6
#endif
#ifdef NOTE_FS6
#undef NOTE_FS6
#endif
#ifdef NOTE_G6
#undef NOTE_G6
#endif
#ifdef NOTE_GS6
#undef NOTE_GS6
#endif
#ifdef NOTE_A6
#undef NOTE_A6
#endif
#ifdef NOTE_AS6
#undef NOTE_AS6
#endif
#ifdef NOTE_B6
#undef NOTE_B6
#endif
#ifdef NOTE_C7
#undef NOTE_C7
#endif
#ifdef NOTE_CS7
#undef NOTE_CS7
#endif
#ifdef NOTE_D7
#undef NOTE_D7
#endif
#ifdef NOTE_DS7
#undef NOTE_DS7
#endif
#ifdef NOTE_E7
#undef NOTE_E7
#endif
#ifdef NOTE_F7
#undef NOTE_F7
#endif
#ifdef NOTE_FS7
#undef NOTE_FS7
#endif
#ifdef NOTE_G7
#undef NOTE_G7
#endif
#ifdef NOTE_GS7
#undef NOTE_GS7
#endif
#ifdef NOTE_A7
#undef NOTE_A7
#endif
#ifdef NOTE_AS7
#undef NOTE_AS7
#endif
#ifdef NOTE_B7
#undef NOTE_B7
#endif
#ifdef NOTE_C8
#undef NOTE_C8
#endif
#ifdef NOTE_CS8
#undef NOTE_CS8
#endif
#ifdef NOTE_D8
#undef NOTE_D8
#endif
#ifdef NOTE_DS8
#undef NOTE_DS8
#endif
#ifdef REST
#undef REST
#endif

// Note definitions (renamed to avoid conflicts)
#define SNAIL_NOTE_B0  31
#define SNAIL_NOTE_C1  33
#define SNAIL_NOTE_CS1 35
#define SNAIL_NOTE_D1  37
#define SNAIL_NOTE_DS1 39
#define SNAIL_NOTE_E1  41
#define SNAIL_NOTE_F1  44
#define SNAIL_NOTE_FS1 46
#define SNAIL_NOTE_G1  49
#define SNAIL_NOTE_GS1 52
#define SNAIL_NOTE_A1  55
#define SNAIL_NOTE_AS1 58
#define SNAIL_NOTE_B1  62
#define SNAIL_NOTE_C2  65
#define SNAIL_NOTE_CS2 69
#define SNAIL_NOTE_D2  73
#define SNAIL_NOTE_DS2 78
#define SNAIL_NOTE_E2  82
#define SNAIL_NOTE_F2  87
#define SNAIL_NOTE_FS2 93
#define SNAIL_NOTE_G2  98
#define SNAIL_NOTE_GS2 104
#define SNAIL_NOTE_A2  110
#define SNAIL_NOTE_AS2 117
#define SNAIL_NOTE_B2  123
#define SNAIL_NOTE_C3  131
#define SNAIL_NOTE_CS3 139
#define SNAIL_NOTE_D3  147
#define SNAIL_NOTE_DS3 156
#define SNAIL_NOTE_E3  165
#define SNAIL_NOTE_F3  175
#define SNAIL_NOTE_FS3 185
#define SNAIL_NOTE_G3  196
#define SNAIL_NOTE_GS3 208
#define SNAIL_NOTE_A3  220
#define SNAIL_NOTE_AS3 233
#define SNAIL_NOTE_B3  247
#define SNAIL_NOTE_C4  262
#define SNAIL_NOTE_CS4 277
#define SNAIL_NOTE_D4  294
#define SNAIL_NOTE_DS4 311
#define SNAIL_NOTE_E4  330
#define SNAIL_NOTE_F4  349
#define SNAIL_NOTE_FS4 370
#define SNAIL_NOTE_G4  392
#define SNAIL_NOTE_GS4 415
#define SNAIL_NOTE_A4  440
#define SNAIL_NOTE_AS4 466
#define SNAIL_NOTE_B4  494
#define SNAIL_NOTE_C5  523
#define SNAIL_NOTE_CS5 554
#define SNAIL_NOTE_D5  587
#define SNAIL_NOTE_DS5 622
#define SNAIL_NOTE_E5  659
#define SNAIL_NOTE_F5  698
#define SNAIL_NOTE_FS5 740
#define SNAIL_NOTE_G5  784
#define SNAIL_NOTE_GS5 831
#define SNAIL_NOTE_A5  880
#define SNAIL_NOTE_AS5 932
#define SNAIL_NOTE_B5  988
#define SNAIL_NOTE_C6  1047
#define SNAIL_NOTE_CS6 1109
#define SNAIL_NOTE_D6  1175
#define SNAIL_NOTE_DS6 1245
#define SNAIL_NOTE_E6  1319
#define SNAIL_NOTE_F6  1397
#define SNAIL_NOTE_FS6 1480
#define SNAIL_NOTE_G6  1568
#define SNAIL_NOTE_GS6 1661
#define SNAIL_NOTE_A6  1760
#define SNAIL_NOTE_AS6 1865
#define SNAIL_NOTE_B6  1976
#define SNAIL_NOTE_C7  2093
#define SNAIL_NOTE_CS7 2217
#define SNAIL_NOTE_D7  2349
#define SNAIL_NOTE_DS7 2489
#define SNAIL_NOTE_E7  2637
#define SNAIL_NOTE_F7  2794
#define SNAIL_NOTE_FS7 2960
#define SNAIL_NOTE_G7  3136
#define SNAIL_NOTE_GS7 3322
#define SNAIL_NOTE_A7  3520
#define SNAIL_NOTE_AS7 3729
#define SNAIL_NOTE_B7  3951
#define SNAIL_NOTE_C8  4186
#define SNAIL_NOTE_CS8 4435
#define SNAIL_NOTE_D8  4699
#define SNAIL_NOTE_DS8 4978
#define SNAIL_REST 0

// Structure to hold information about a predefined sound
struct SoundEntry {
    const char* name;        // Display name of the sound
    void (*playFunction)();  // Function pointer to play the sound
};

// Predefined sounds list
extern const SoundEntry PREDEFINED_SOUNDS[];
extern const int NUM_PREDEFINED_SOUNDS;

// Function declarations
extern bool isWavFile(const String& filename);
extern bool playWavFile(const String& filepath);
extern void stopSound();
extern bool isSoundPlaying();
extern uint8_t getVolume();
extern void setVolume(uint8_t volume);
extern void playTestSound();
extern void playNokiaRingtone();
extern void playTokyoDrift();
extern void playKerosene();
extern void playFirstDayOut();
extern void playWavFile(const char* filename);
extern bool checkStopSoundPress();

// Add new function to play a predefined sound by index
void playPredefinedSound(int index); 