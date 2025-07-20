#include "SoundFunctions.h"  // Must be first to get our definitions
#include <M5Unified.h>
#include <M5Cardputer.h>
#include <SD.h>
#include "SDcardFunctions.h"  // Add this include to use isWavFile from SDcardFunctions

// Global variable to track if sound is playing
static bool isPlaying = false;

// Maximum WAV file size (6MB)
const size_t MAX_WAV_SIZE = 6 * 1024 * 1024;  // 6MB in bytes

// Define the list of predefined sounds
const SoundEntry PREDEFINED_SOUNDS[] = {
    {"Test Sound", playTestSound},
    {"Nokia Ringtone", playNokiaRingtone},
    {"Tokyo Drift", playTokyoDrift},
    {"Kerosene", playKerosene},
    {"First Day Out", playFirstDayOut},
    // Add more sounds here as needed
};

// Number of predefined sounds
const int NUM_PREDEFINED_SOUNDS = sizeof(PREDEFINED_SOUNDS) / sizeof(SoundEntry);

// Function to play a predefined sound by index
void playPredefinedSound(int index) {
    if (index >= 0 && index < NUM_PREDEFINED_SOUNDS) {
        // Stop any currently playing sound
        stopSound();
        
        // Play the selected sound
        PREDEFINED_SOUNDS[index].playFunction();
    }
}

bool playWavFile(const String& filepath) {
    // Check if file exists
    if (!SD.exists(filepath)) {
        Serial.println("WAV file not found: " + filepath);
        return false;
    }
    
    // Stop any currently playing sound
    stopSound();
    
    // Open the WAV file
    File wavFile = SD.open(filepath, FILE_READ);
    if (!wavFile) {
        Serial.println("Failed to open WAV file: " + filepath);
        return false;
    }
    
    // Get file size
    size_t fileSize = wavFile.size();
    if (fileSize == 0) {
        Serial.println("WAV file is empty: " + filepath);
        wavFile.close();
        return false;
    }
    
    // Check if file is too large
    if (fileSize > MAX_WAV_SIZE) {
        Serial.printf("WAV file too large (max %dMB): %s\n", MAX_WAV_SIZE / (1024 * 1024), filepath.c_str());
        wavFile.close();
        return false;
    }
    
    // Allocate buffer for WAV data
    uint8_t* wavData = (uint8_t*)malloc(fileSize);
    if (!wavData) {
        Serial.println("Failed to allocate memory for WAV file");
        wavFile.close();
        return false;
    }
    
    // Read entire file into memory
    size_t bytesRead = wavFile.read(wavData, fileSize);
    wavFile.close();
    
    if (bytesRead != fileSize) {
        Serial.println("Failed to read entire WAV file");
        free(wavData);
        return false;
    }
    
    // Initialize speaker if not already done
    if (!M5.Speaker.isEnabled()) {
        if (!M5.Speaker.begin()) {
            Serial.println("Failed to initialize speaker");
            free(wavData);
            return false;
        }
    }
    
    // Set volume to a reasonable level (0-255)
    M5.Speaker.setVolume(200);
    
    // Stop any current playback
    M5.Speaker.stop();
    
    // Play the WAV file from memory
    // Parameters:
    // - wavData: pointer to WAV data including header
    // - fileSize: size of WAV data
    // - repeat: 1 (play once)
    // - channel: -1 (use default channel)
    // - stop_current_sound: true (stop any current playback)
    if (M5.Speaker.playWav(wavData, fileSize, 1, -1, true)) {
        isPlaying = true;
        Serial.println("Playing WAV file: " + filepath);
        // Note: wavData will be freed by the speaker when playback is done
        return true;
    } else {
        Serial.println("Failed to play WAV file: " + filepath);
        free(wavData);
        return false;
    }
}

void stopSound() {
    if (isPlaying) {
        M5.Speaker.stop();
        isPlaying = false;
        Serial.println("Stopped sound playback");
    }
}

// Add a function to check if sound is currently playing
bool isSoundPlaying() {
    return M5.Speaker.isPlaying();
}

// Add a function to get current volume
uint8_t getVolume() {
    return M5.Speaker.getVolume();
}

// Add a function to set volume
void setVolume(uint8_t volume) {
    M5.Speaker.setVolume(volume);
}

void playTestSound() {
    // Stop any current sound
    stopSound();
    
    // Initialize speaker if not already done
    if (!M5.Speaker.isEnabled()) {
        if (!M5.Speaker.begin()) {
            Serial.println("Failed to initialize speaker");
            return;
        }
    }
    
    // Set volume to a reasonable level (0-255)
    M5.Speaker.setVolume(200);
    
    // Play a sequence of tones
    // Ascending tones
    M5.Speaker.tone(2000, 100);  // 2kHz for 100ms
    delay(150);
    M5.Speaker.tone(3000, 100);  // 3kHz for 100ms
    delay(150);
    M5.Speaker.tone(4000, 100);  // 4kHz for 100ms
    delay(150);
    
    // Descending tones
    M5.Speaker.tone(4000, 100);  // 4kHz for 100ms
    delay(150);
    M5.Speaker.tone(3000, 100);  // 3kHz for 100ms
    delay(150);
    M5.Speaker.tone(2000, 100);  // 2kHz for 100ms
    delay(150);
    
    // Final beep
    M5.Speaker.tone(5000, 200);  // 5kHz for 200ms
    delay(250);
    
    // Stop any remaining tone
    M5.Speaker.stop();
}

// Helper function to check if user wants to stop the sound
bool checkStopSoundPress() {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isPressed()) {
        return true;
    }
    return false;
}

// Add Nokia ringtone implementation
void playNokiaRingtone() {
    // Stop any current sound
    stopSound();
    
    // Initialize speaker if not already done
    if (!M5.Speaker.isEnabled()) {
        if (!M5.Speaker.begin()) {
            Serial.println("Failed to initialize speaker");
            return;
        }
    }
    
    // Set volume to a reasonable level (0-255)
    M5.Speaker.setVolume(200);

    // Nokia ringtone melody and durations
    static const int nokiaMelody[] = {
        SNAIL_NOTE_E5, SNAIL_NOTE_D5, SNAIL_NOTE_FS4, SNAIL_NOTE_GS4, 
        SNAIL_NOTE_CS5, SNAIL_NOTE_B4, SNAIL_NOTE_D4, SNAIL_NOTE_E4, 
        SNAIL_NOTE_B4, SNAIL_NOTE_A4, SNAIL_NOTE_CS4, SNAIL_NOTE_E4,
        SNAIL_NOTE_A4
    };

    static const int nokiaDurations[] = {
        8, 8, 4, 4,
        8, 8, 4, 4,
        8, 8, 4, 4,
        2
    };

    const int nokiaDurationsSize = sizeof(nokiaDurations) / sizeof(int);
    const int tempo = 120;  // Tempo in beats per minute
    const int wholenote = (60000 * 4) / tempo;  // Duration of a whole note in milliseconds

    // Play the melody
    for (int note = 0; note < nokiaDurationsSize; note++) {
        // Calculate note duration based on tempo
        int noteDuration = wholenote / nokiaDurations[note];
        
        // Play the note
        M5.Speaker.tone(nokiaMelody[note], noteDuration);

        // Calculate pause between notes (30% of note duration)
        int pauseBetweenNotes = noteDuration * 0.3;

        // Wait for the note duration plus pause
        delay(noteDuration + pauseBetweenNotes);

        // Check if user wants to stop the sound
        if (checkStopSoundPress()) {
            M5.Speaker.stop();
            return;
        }
    }

    // Stop the speaker after the melody is done
    M5.Speaker.stop();
}

// Add Tokyo Drift implementation
void playTokyoDrift() {
    // Stop any current sound
    stopSound();
    
    // Initialize speaker if not already done
    if (!M5.Speaker.isEnabled()) {
        if (!M5.Speaker.begin()) {
            Serial.println("Failed to initialize speaker");
            return;
        }
    }
    
    // Set volume to a reasonable level (0-255)
    M5.Speaker.setVolume(200);

    // Tokyo Drift melody and durations
    static const int tokyoDriftMelody[] = {
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_NOTE_AS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS5,
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_NOTE_AS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS5,
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_NOTE_AS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS5,
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_NOTE_AS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS5,
        SNAIL_NOTE_F5, SNAIL_REST, SNAIL_NOTE_F5, SNAIL_REST,
        SNAIL_NOTE_GS5, SNAIL_NOTE_FS5, SNAIL_NOTE_F5,
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_NOTE_GS5, SNAIL_NOTE_FS5, SNAIL_NOTE_F5,
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_NOTE_AS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS5,
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_NOTE_AS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS5,
        SNAIL_NOTE_AS4, SNAIL_REST, SNAIL_NOTE_AS4, SNAIL_REST,
        SNAIL_REST
    };

    static const int tokyoDriftDurations[] = {
        4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        3, 3, 4,
        4, 4, 4, 4,
        1
    };

    const int tokyoDriftDurationsSize = sizeof(tokyoDriftDurations) / sizeof(int);
    const int tempo = 120;  // Tempo in beats per minute
    const int wholenote = (60000 * 4) / tempo;  // Duration of a whole note in milliseconds

    // Play the melody
    for (int note = 0; note < tokyoDriftDurationsSize; note++) {
        // Calculate note duration based on tempo
        int noteDuration = wholenote / tokyoDriftDurations[note];
        
        // Play the note (skip if it's a rest)
        if (tokyoDriftMelody[note] != SNAIL_REST) {
            M5.Speaker.tone(tokyoDriftMelody[note], noteDuration);
        }

        // Calculate pause between notes (30% of note duration)
        int pauseBetweenNotes = noteDuration * 0.3;

        // Wait for the note duration plus pause
        delay(noteDuration + pauseBetweenNotes);

        // Check if user wants to stop the sound
        if (checkStopSoundPress()) {
            M5.Speaker.stop();
            return;
        }
    }

    // Stop the speaker after the melody is done
    M5.Speaker.stop();
}

// Add Kerosene implementation
void playKerosene() {
    // Stop any current sound
    stopSound();
    
    // Initialize speaker if not already done
    if (!M5.Speaker.isEnabled()) {
        if (!M5.Speaker.begin()) {
            Serial.println("Failed to initialize speaker");
            return;
        }
    }
    
    // Set volume to a reasonable level (0-255)
    M5.Speaker.setVolume(200);

    // Kerosene melody and durations
    static const int keroseneMelody[] = {
        SNAIL_NOTE_G4, SNAIL_NOTE_AS4, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_D5, SNAIL_NOTE_C5, SNAIL_NOTE_DS5, SNAIL_NOTE_D5,
        SNAIL_NOTE_D5, SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_AS4, SNAIL_NOTE_A4, SNAIL_NOTE_AS4, SNAIL_NOTE_AS4,
        SNAIL_NOTE_AS4, SNAIL_NOTE_C5, SNAIL_NOTE_D5,
        
        SNAIL_NOTE_D5, SNAIL_NOTE_D5, SNAIL_NOTE_DS5, SNAIL_NOTE_D5, SNAIL_NOTE_DS5,
        SNAIL_NOTE_DS5, SNAIL_NOTE_DS5, SNAIL_NOTE_C5, SNAIL_NOTE_DS5, SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_D5, SNAIL_NOTE_AS4, SNAIL_NOTE_D5,
        SNAIL_NOTE_AS4, SNAIL_NOTE_AS4, SNAIL_NOTE_A4, SNAIL_NOTE_AS4, SNAIL_NOTE_A4, SNAIL_NOTE_A4, SNAIL_NOTE_AS4, SNAIL_NOTE_A4, SNAIL_NOTE_AS4, SNAIL_NOTE_AS4, SNAIL_NOTE_G4, SNAIL_NOTE_AS4,
        SNAIL_NOTE_G4, SNAIL_NOTE_G4, SNAIL_NOTE_F4, SNAIL_NOTE_G4, SNAIL_NOTE_F4, SNAIL_NOTE_F4,

        SNAIL_NOTE_C5,
        SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_D5,
        SNAIL_NOTE_F5, SNAIL_NOTE_D5, SNAIL_NOTE_F5, SNAIL_NOTE_F5, SNAIL_NOTE_D5, SNAIL_NOTE_F5, SNAIL_NOTE_D5, SNAIL_NOTE_D5, SNAIL_NOTE_C5, SNAIL_NOTE_D5,
        SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_AS4, SNAIL_NOTE_C5, SNAIL_NOTE_AS4, SNAIL_NOTE_AS4, SNAIL_NOTE_D5, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_D5,

        SNAIL_NOTE_DS5,
        SNAIL_NOTE_DS5, SNAIL_NOTE_DS5, SNAIL_NOTE_C5, SNAIL_NOTE_DS5, SNAIL_NOTE_C5, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_C5, SNAIL_NOTE_D5, SNAIL_NOTE_D5,
        SNAIL_NOTE_AS4, SNAIL_NOTE_D5, SNAIL_NOTE_AS4, SNAIL_NOTE_AS4, SNAIL_NOTE_A4, SNAIL_NOTE_AS4, SNAIL_NOTE_A4, SNAIL_NOTE_A4, SNAIL_NOTE_AS4, SNAIL_NOTE_A4,
        SNAIL_NOTE_AS4, SNAIL_NOTE_AS4, SNAIL_NOTE_G4, SNAIL_NOTE_AS4, SNAIL_NOTE_G4, SNAIL_NOTE_G4, SNAIL_NOTE_F4, SNAIL_NOTE_G4, SNAIL_NOTE_F4, SNAIL_NOTE_F4,

        SNAIL_REST
    };

    static const int keroseneDurations[] = {
        4, 3, 4, 4, 2, 3, 4, 1,
        3, 4, 2, 4, 3, 4, 1,
        3, 4, 1,

        4, 3, 4, 4, 4,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8,

        4,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

        4,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

        1
    };

    const int keroseneDurationsSize = sizeof(keroseneDurations) / sizeof(int);
    const int tempo = 120;  // Tempo in beats per minute
    const int wholenote = (60000 * 4) / tempo;  // Duration of a whole note in milliseconds

    // Play the melody
    for (int note = 0; note < keroseneDurationsSize; note++) {
        // Calculate note duration based on tempo
        int noteDuration = wholenote / keroseneDurations[note];
        
        // Play the note (skip if it's a rest)
        if (keroseneMelody[note] != SNAIL_REST) {
            M5.Speaker.tone(keroseneMelody[note], noteDuration);
        }

        // Calculate pause between notes (30% of note duration)
        int pauseBetweenNotes = noteDuration * 0.3;

        // Wait for the note duration plus pause
        delay(noteDuration + pauseBetweenNotes);

        // Check if user wants to stop the sound
        if (checkStopSoundPress()) {
            M5.Speaker.stop();
            return;
        }
    }

    // Stop the speaker after the melody is done
    M5.Speaker.stop();
}

// Add First Day Out implementation
void playFirstDayOut() {
    // Stop any current sound
    stopSound();
    
    // Initialize speaker if not already done
    if (!M5.Speaker.isEnabled()) {
        if (!M5.Speaker.begin()) {
            Serial.println("Failed to initialize speaker");
            return;
        }
    }
    
    // Set volume to a reasonable level (0-255)
    M5.Speaker.setVolume(200);

    // First Day Out melody and durations
    static const int firstDayOutMelody[] = {
        SNAIL_REST,

        SNAIL_NOTE_E4, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4,
        SNAIL_NOTE_E4, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4, SNAIL_NOTE_GS4,

        SNAIL_NOTE_E4, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4,
        SNAIL_NOTE_E4, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4, SNAIL_NOTE_B4,

        SNAIL_NOTE_E4, SNAIL_NOTE_CS4, SNAIL_NOTE_E4, SNAIL_NOTE_GS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS4, SNAIL_NOTE_E4,
        SNAIL_NOTE_CS4, SNAIL_NOTE_B5, SNAIL_NOTE_A5, SNAIL_NOTE_GS4, SNAIL_REST,

        SNAIL_NOTE_E4, SNAIL_NOTE_CS4, SNAIL_NOTE_E4, SNAIL_NOTE_GS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS4, SNAIL_NOTE_E4,
        SNAIL_NOTE_CS4, SNAIL_NOTE_B5, SNAIL_NOTE_A5, SNAIL_NOTE_CS5, SNAIL_REST,

        SNAIL_NOTE_E4, SNAIL_NOTE_CS4, SNAIL_NOTE_E4, SNAIL_NOTE_GS4, SNAIL_NOTE_B4, SNAIL_NOTE_DS4, SNAIL_NOTE_E4,
        SNAIL_NOTE_CS4, SNAIL_NOTE_B5, SNAIL_NOTE_A5, SNAIL_NOTE_CS5, SNAIL_REST,

        SNAIL_NOTE_A5, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4, SNAIL_NOTE_GS4,
        SNAIL_NOTE_A5, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4, SNAIL_NOTE_GS4,
        SNAIL_NOTE_A5, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4, SNAIL_NOTE_GS4,
        SNAIL_NOTE_A5, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4, SNAIL_NOTE_GS4,
        SNAIL_NOTE_A5, SNAIL_NOTE_GS4, SNAIL_NOTE_CS4, SNAIL_NOTE_GS4,

        SNAIL_REST
    };

    static const int firstDayOutDurations[] = {
        4,

        2, 3, 1,
        2, 3, 1, 3,

        2, 3, 1,
        2, 3, 1, 3,

        3, 6, 6, 6, 6, 6, 6,
        4, 3, 3, 3, 2,

        3, 6, 6, 6, 6, 6, 6,
        4, 3, 3, 3, 2,

        3, 6, 6, 6, 6, 6, 6,
        4, 3, 3, 3, 2,

        4, 8, 8, 8,
        4, 8, 8, 8,
        4, 8, 8, 8,
        4, 8, 8, 8,
        4, 8, 8, 8,

        1
    };

    const int firstDayOutDurationsSize = sizeof(firstDayOutDurations) / sizeof(int);
    const int tempo = 120;  // Tempo in beats per minute
    const int wholenote = (60000 * 4) / tempo;  // Duration of a whole note in milliseconds

    // Play the melody
    for (int note = 0; note < firstDayOutDurationsSize; note++) {
        // Calculate note duration based on tempo
        int noteDuration = wholenote / firstDayOutDurations[note];
        
        // Play the note (skip if it's a rest)
        if (firstDayOutMelody[note] != SNAIL_REST) {
            M5.Speaker.tone(firstDayOutMelody[note], noteDuration);
        }

        // Calculate pause between notes (30% of note duration)
        int pauseBetweenNotes = noteDuration * 0.3;

        // Wait for the note duration plus pause
        delay(noteDuration + pauseBetweenNotes);

        // Check if user wants to stop the sound
        if (checkStopSoundPress()) {
            M5.Speaker.stop();
            return;
        }
    }

    // Stop the speaker after the melody is done
    M5.Speaker.stop();
} 