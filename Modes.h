//******************************************************************************
// Modes.h
//******************************************************************************

#ifndef MODES_H
#define MODES_H

typedef enum eMode : uint8_t
{
    MODE_TIME,				// 0
#ifdef SHOW_MODE_AMPM
    MODE_AMPM,				// 1
#endif
#ifdef SHOW_MODE_SECONDS
    MODE_SECONDS,			// 2 if SHOW_MODE_AMPM is set - 1 if not set and so on...
#endif
#ifdef SHOW_MODE_WEEKDAY
    MODE_WEEKDAY,			// 3 if...
#endif
#ifdef SHOW_MODE_DATE
    MODE_DATE,				// 4
#endif
#if defined(SHOW_MODE_SUNRISE_SUNSET) && defined(APIKEY)
    MODE_SUNRISE, 			// 5
    MODE_SUNSET, 			// 6
#endif
#ifdef SHOW_MODE_MOONPHASE
    MODE_MOONPHASE,			// 7
#endif
#if defined(RTC_BACKUP) || defined(SENSOR_DHT22) || defined(SENSOR_MCP9808) || defined(SENSOR_BME280)
    MODE_TEMP,				// 8
#endif
#if defined(SENSOR_DHT22) || defined(SENSOR_BME280)
    MODE_HUMIDITY,			// 9
#endif
#ifdef APIKEY
    MODE_EXT_TEMP,			// 10
    MODE_EXT_HUMIDITY,		// 11
#endif
#ifdef BUZZER
    MODE_TIMER,				// 12
#endif
#ifdef SHOW_MODE_TEST
    MODE_TEST,				// 13
    MODE_RED,				// 14
    MODE_GREEN,				// 15
    MODE_BLUE,				// 16
    MODE_WHITE,				// 17
#endif
#ifdef SHOW_MODE_SETTINGS
    MODE_SET_1ST,
#ifdef LDR
    MODE_SET_LDR = MODE_SET_1ST,
    MODE_SET_COLOR,
#else
    MODE_SET_COLOR = MODE_SET_1ST,
#endif      
    MODE_SET_COLORCHANGE,
    MODE_SET_TRANSITION,
    MODE_SET_IT_IS,
    MODE_SET_GSI,
    MODE_SET_TIME,
    MODE_SET_YEAR,
    MODE_SET_MONTH,
    MODE_SET_DAY,
    MODE_SET_NIGHTOFF,
    MODE_SET_DAYON,
    MODE_SET_TIMEOUT,
#ifdef BUZZER
    MODE_SET_TIMER,
    MODE_SET_ALARM_1,
    MODE_SET_ALARM_2,
#endif
#endif
    MODE_COUNT,				// 18
    MODE_BLANK,				// 19
    MODE_FEED,				// 20
    MODE_WPS,     // 21
    MODE_SET_BRIGHTNESS, //22
} Mode;

// Overload the ControlType++ operator.
inline Mode& operator++(Mode& eDOW, int)
{
    const uint8_t i = static_cast<uint8_t>(eDOW) + 1;
    eDOW = static_cast<Mode>((i) % MODE_COUNT);
    return eDOW;
}

enum eTransition
{
    TRANSITION_NORMAL, // 0
    TRANSITION_MOVEUP, // 1
    TRANSITION_FADE,   // 2
    TRANSITION_MATRIX,  // 3
    TRANSITION_COUNT = TRANSITION_MATRIX
};

#endif
