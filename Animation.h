//******************************************************************************
// Animation.h
//******************************************************************************

#ifndef ANIMATION_H
#define ANIMATION_H
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "Configuration.h"
#include <LittleFS.h>
#include <Arduino_JSON.h>
#include "Colors.h"

#define MAXFRAMES 25
#define MAXANIMATION 30
#define ANIMATIONSLISTE "/animationsliste.json"

typedef struct
  {
    color_s color[11][10];
    uint16_t delay;
  } s_frame;

typedef struct
  {
    char name[20];
    uint8_t loops;
    uint8_t laufmode;
    s_frame frame[MAXFRAMES+1];
  } s_myanimation;


bool loadAnimation(String aniname );
void saveAnimationsListe();
bool saveAnimation(String aniname );
void getAnimationList();
uint32_t string_to_num(String in_color);
String num_to_string(uint32_t in_color);
String color_to_string(color_s in_color);
color_s num_to_color(uint32_t in_color);
color_s string_to_color(String in_color);
bool loadjsonarry( int8_t frame, uint8_t zeile, String &jsonBuffer);


#endif
