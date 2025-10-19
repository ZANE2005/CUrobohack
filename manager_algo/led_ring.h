// ============================================================================
// File: led_ring.h
// Purpose: Minimal LED ring abstraction (e.g., NeoPixel) used by your FSM.
// Why: Centralize LED behaviors (default color, RGB selection, subtract steps)
//      so the state machine stays clean and hardware-agnostic.
// ===========================================================================

#ifndef LED_DIAL_ANIMATION_H
#define LED_DIAL_ANIMATION_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LEDring {

public:

  //define constructors
  LEDring(const unsigned int ledOutputPin, const unsigned int potInputPin);

  LEDring(const unsigned int ledOutputPin, const unsigned int potInputPin, int LEDs);

  void initalizePot(int numLEDs);

  //method to map Pot to neopixel values
  int neoPixelMap();

  //set the default color
  void setDefaultColor(int brightness);

  //checks to subtract LED
  void subtractLitLED(int time_elapsed);

  //methods to allow for rgb selection 
  void selectRGB( unsigned int index);

  void ResetRGB();


private:

  //define function to show NeoPixel 
  void showNeoPixel();

  void updateRGB(uint8_t R, uint8_t G, uint8_t B);
  
  uint8_t modRGBByIndex(uint8_t a, uint8_t x, unsigned int aIndex, unsigned int xIndex);

  //define private variables and vaules
  const unsigned int ledPin, potPin;

  unsigned int potMap, numLEDs, maxPot = 1023, maxRGB = 255;

  uint8_t defColor, colorMap;

  Adafruit_NeoPixel *ring;

  uint8_t RGB[3]  = {defColor,0,0};
  
};


#endif