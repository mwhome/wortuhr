//*****************************************************************************
// Settings.h
//*****************************************************************************

#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Colors.h"
#include "Configuration.h"
#include "Languages.h"
#include "Modes.h"
#include "Event.h"

#define SETTINGS_MAGIC_NUMBER 0x2A
#define SETTINGS_VERSION 0

#define MOOD_LEVEL_MAX 9

#define LEN_TS_URL 30
#define LEN_OW_API_KEY 33
#define LEN_OW_LOCATION 30

#define NUM_EVTS 8

class Settings {
public:
    Settings();
    ~Settings();

    struct MySettings {
        uint8_t magicNumber;
        uint8_t version;
        boolean useAbc;
        int16_t brightness;
        uint8_t color;
        uint8_t colorChange;
        uint8_t transition;
        uint8_t timeout;
        boolean modeChange;
        boolean purist;
        boolean alarm1;
        time_t  alarm1Time;
        uint8_t alarm1Weekdays;
        boolean alarm2;
        time_t  alarm2Time;
        uint8_t alarm2Weekdays;
        time_t  nightOffTime;
        time_t  dayOnTime;
        boolean hourBeep;
        uint8_t moodRate;
        event_t events[NUM_EVTS];
        char timeServer[LEN_TS_URL];
        char owApiKey[LEN_OW_API_KEY];
        char owLocation[LEN_OW_LOCATION];
        eFrontCover frontCover;
        bool chGsi;
        uint8_t ldrPosX;
        uint8_t ldrPosY;
    } mySettings;

    void saveToEEPROM();
    void resetToDefault();
    
private:
    void loadFromEEPROM();
};

#endif
