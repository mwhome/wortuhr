//*****************************************************************************
// Settings.cpp
//*****************************************************************************

#include "Settings.h"

Settings::Settings() {
    loadFromEEPROM();
}

Settings::~Settings() {
}

// Set all defaults
void Settings::resetToDefault() {
    mySettings.magicNumber = SETTINGS_MAGIC_NUMBER;
    mySettings.version = SETTINGS_VERSION;
    mySettings.useAbc = false;
    mySettings.brightness = 50;
    mySettings.color = WHITE;
    mySettings.colorChange = COLORCHANGE_NO;
    mySettings.transition = TRANSITION_FADE;
    mySettings.timeout = 10;
    mySettings.modeChange = false;
    mySettings.purist = false;
    mySettings.alarm1 = false;
    mySettings.alarm1Time = 0;
    mySettings.alarm1Weekdays = 0b11111110;
    mySettings.alarm2 = false;
    mySettings.alarm2Time = 0;
    mySettings.alarm2Weekdays = 0b11111110;
    mySettings.nightOffTime = 3600;
    mySettings.dayOnTime = 18000;
    mySettings.hourBeep = false;
    mySettings.moodRate = 0;

    for (uint8_t i = 0; i < NUM_EVTS ; i++) {
      mySettings.events[i].enabled = false;
      memset(mySettings.events[i].txt, 0, sizeof(mySettings.events[i].txt));
      memset(mySettings.events[i].animation, 0, sizeof(mySettings.events[i].animation));
      mySettings.events[i].color = WHITE;
      mySettings.events[i].repRate = EVT_REP_60;
      mySettings.events[i].time = 0;
    }

    String(NTP_DEFAULT_SERVER).toCharArray(mySettings.timeServer, sizeof(mySettings.timeServer), 0);
    memset(mySettings.owApiKey, 0, sizeof(mySettings.owApiKey));
    String(DEFAULT_LOCATION).toCharArray(mySettings.owLocation, sizeof(mySettings.owLocation), 0);
    mySettings.frontCover = FRONTCOVER_CH_BE;
    mySettings.chGsi = true;
    mySettings.ldrPosX = LDR_X_DEFAULT;
    mySettings.ldrPosY = LDR_Y_DEFAULT;
    
    saveToEEPROM();
}

// Load settings from EEPROM
void Settings::loadFromEEPROM() {
    EEPROM.begin(1024);
    EEPROM.get(0, mySettings);
    if ((mySettings.magicNumber != SETTINGS_MAGIC_NUMBER) || (mySettings.version != SETTINGS_VERSION))
        resetToDefault();
    EEPROM.end();
}

// Write settings to EEPROM
void Settings::saveToEEPROM() {
    Serial.println("Settings (" + String(sizeof(mySettings)) + "bytes) saved to EEPROM.");
    EEPROM.begin(1024);
    EEPROM.put(0, mySettings);
    //EEPROM.commit();
    EEPROM.end();
}
