/**
 * Event.h
 * Klasse für ein jährliches Event
 *
 * @mc       Arduino/UNO
 * @autor    Manuel Bracher / manuel.bracher@gmail.com
 * @version  1.0
 * @created  02.01.15
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */

#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h>
#include "Colors.h"

#define LEN_EVT_STR 30
#define LEN_ANI_STR 15

const char sEvtRep[][4] PROGMEM = { 
    { "5" },   // 00
		{ "15" }, // 01
		{ "30" }, // 02
		{ "60" }, // 03
		};

enum eEvtRep : uint8_t {
	EVT_REP_5,
	EVT_REP_15,
	EVT_REP_30,
	EVT_REP_60,
	EVT_REP_COUNT
};

const uint8_t evtRepRate[] = {
  5,
  15,
  30,
  60
};

typedef struct event_s {
  time_t time;
  char txt[LEN_EVT_STR];
  char animation[LEN_ANI_STR];
  eColor color;
  eEvtRep repRate;
  bool enabled;
} event_t;

class Event {
public:
	Event() {};
};

#endif //EVENT_H
