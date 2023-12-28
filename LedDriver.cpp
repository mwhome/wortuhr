//*****************************************************************************
// LedDriver.cpp
//*****************************************************************************

#include "LedDriver.h"

LedDriver::LedDriver() {
    calculateLedMap();
    strip = new Adafruit_NeoPixel(getLedsCount(), PIN_LEDS_DATA, NEOPIXEL_TYPE);
    strip->begin();
}

LedDriver::~LedDriver() {
}

void LedDriver::clear() {
    strip->clear();
}

void LedDriver::show() {
    strip->show();
}

uint32_t LedDriver::getPixelColor(uint8_t x, uint8_t y) {
  return getPixelColor(x + y * NUMPIXELS_X);
}

uint32_t LedDriver::getPixelColor(uint8_t num) {
  return strip->getPixelColor(ledMapCalc[num]);
}

void LedDriver::setPixel(uint8_t x, uint8_t y, uint8_t color, uint8_t brightness) {
    setPixel(x + y * NUMPIXELS_X, color, brightness);
}

void LedDriver::setPixel(uint8_t num, uint8_t color, uint8_t brightness) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
      
    if (color != MOOD){
      red = brightness * 0.0039 * defaultColors[color].red;
      green = brightness * 0.0039 * defaultColors[color].green;
      blue = brightness * 0.0039 * defaultColors[color].blue;
    } else {
      red = brightness * 0.0039 * getRed(m_wheelColor);
      green = brightness * 0.0039 * getGreen(m_wheelColor);
      blue = brightness * 0.0039 * getBlue(m_wheelColor);
    }

//Serial.println( "red: " + String(red) + " green: " + String(green) + " blue: " + String(blue));

    setPixelRGB(num, red, green, blue);

    return;
}

void LedDriver::setPixelRGB(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue)
{
    setPixelRGB(x + y * NUMPIXELS_X, red, green, blue);
}

void LedDriver::setPixelRGB(uint8_t num, uint8_t red, uint8_t green, uint8_t blue )
{   
#ifdef NEOPIXEL_RGBW
    uint8_t white = 0xFF;
    if (red < white) white = red;
    if (green < white) white = green;
    if (blue < white) white = blue;
    
    setPixelColor(num, Adafruit_NeoPixel::Color(red - white, green - white, blue - white, white));
    
#endif

#ifdef NEOPIXEL_RGB
    setPixelColor(num, Adafruit_NeoPixel::Color(red, green, blue));
#endif
}

void LedDriver::setPixelColor(uint8_t x, uint8_t y, uint32_t color32bit){
  setPixelColor(x + y * NUMPIXELS_X, color32bit);
}

void LedDriver::setPixelColor(uint8_t num, uint32_t color32bit) {
  uint8_t numLedsToWrite;

  if(num < PIXEL_NO_CORNER_1) {
    numLedsToWrite = ledMap.ledsPerLetter;
  } else if (num < PIXEL_NO_ALARM) {
    numLedsToWrite = ledMap.ledsPerCorner;
  }else {
    numLedsToWrite = ledMap.ledsPerAlarm;
  }

  for (uint8_t i = 0; i < numLedsToWrite; i++){
    strip->setPixelColor(ledMapCalc[num] + i, color32bit);
  }
}

void LedDriver::updateColorWheel() {
  m_wheelPos += 2;
  m_wheelColor = wheel(m_wheelPos);
}

uint32_t LedDriver::wheel(uint8_t wheelPos) {
  if (wheelPos < 85) {
    return Adafruit_NeoPixel::Color(wheelPos * 3, 255 - wheelPos * 3, 0);
  } else if (wheelPos < 170) {
    wheelPos -= 85;
    return Adafruit_NeoPixel::Color(255 - wheelPos * 3, 0, wheelPos * 3);
  } else {
    wheelPos -= 170;
    return Adafruit_NeoPixel::Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
}

uint8_t LedDriver::getLedsCount(void){
  uint8_t maxLedNum = 0;
  for(uint8_t i = 0; i < PIXEL_NO_CORNER_1; i++){
    if (ledMapCalc[i] > maxLedNum)
    maxLedNum = ledMapCalc[i] + (ledMap.ledsPerLetter - 1);
  }

  for(uint8_t i = 0; i < NUMPIXELS_CORNERS; i++){
    if (ledMapCalc[PIXEL_NO_CORNER_1+i] > maxLedNum)
      maxLedNum = ledMapCalc[PIXEL_NO_CORNER_1+i] + (ledMap.ledsPerCorner - 1);
  }

  if (ledMapCalc[PIXEL_NO_ALARM] > maxLedNum)
    maxLedNum = ledMapCalc[PIXEL_NO_ALARM] + (ledMap.ledsPerAlarm - 1);

  return maxLedNum + 1;
}

void LedDriver::calculateLedMap(void){
  for (uint8_t i = 0; i < PIXEL_NO_CORNER_1; i++) {
      uint8_t ledNum = 0;
      for (uint8_t j = 0; j < PIXEL_NO_CORNER_1; j++) {
        if (ledMap.matrix[j] < ledMap.matrix[i]) ledNum += ledMap.ledsPerLetter;
      }
      for (uint8_t j = 0; j < NUMPIXELS_CORNERS; j++) {
        if (ledMap.corners[j] < ledMap.matrix[i]) ledNum += ledMap.ledsPerCorner;
      }
      if (ledMap.alarm < ledMap.matrix[i]) ledNum += ledMap.ledsPerAlarm;
      ledMapCalc[i] = ledNum;
    }
    
    for (uint8_t i = 0; i < NUMPIXELS_CORNERS; i++) {
      uint8_t ledNum = 0;
      for (uint8_t j = 0; j < PIXEL_NO_CORNER_1; j++) {
        if (ledMap.matrix[j] < ledMap.corners[i]) ledNum += ledMap.ledsPerLetter;
      }
      for (uint8_t j = 0; j < NUMPIXELS_CORNERS; j++) {
        if (ledMap.corners[j] < ledMap.corners[i]) ledNum += ledMap.ledsPerCorner;
      }
      if (ledMap.alarm < ledMap.corners[i]) ledNum += ledMap.ledsPerAlarm;
      ledMapCalc[PIXEL_NO_CORNER_1 + i] = ledNum;
    }
    
    uint8_t ledNum = 0;
    for (uint8_t j = 0; j < PIXEL_NO_CORNER_1; j++) {
      if (ledMap.matrix[j] < ledMap.alarm) ledNum += ledMap.ledsPerLetter;
    }
    for (uint8_t j = 0; j < NUMPIXELS_CORNERS; j++) {
      if (ledMap.corners[j] < ledMap.alarm) ledNum += ledMap.ledsPerCorner;
    }
    if (ledMap.alarm < ledMap.alarm) ledNum += ledMap.ledsPerAlarm;
    ledMapCalc[PIXEL_NO_ALARM] = ledNum;
}
