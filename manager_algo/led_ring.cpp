  // ============================================================================
// File: led_ring.cpp
// Impl notes:
// - Keep rendering logic self-contained. The FSM shouldn't care how you draw,
//   only that it can set colors, select palette entries, and reduce lit count.
// - Replace placeholders with your actual LED library calls.
// ============================================================================
  

  
  #include "led_ring.h"
  #include <Arduino.h>

  // primary constructor
  LEDring::LEDring(const unsigned int ledOutputPin, const unsigned int potInputPin): ledPin(ledOutputPin), potPin(potInputPin), numLEDs(16) {
    ring = new Adafruit_NeoPixel(numLEDs, ledPin, NEO_RGBW + NEO_KHZ800);
    ring->begin();
  }

  // secondary constructor
  LEDring::LEDring(const unsigned int ledOutputPin, const unsigned int potInputPin, int LEDs): numLEDs(LEDs), ledPin(ledOutputPin), potPin(potInputPin) {
    ring = new Adafruit_NeoPixel(numLEDs, ledPin, NEO_RGBW + NEO_KHZ800);
    ring->begin();
  }
  //show neoPixel LEDs
  void LEDring::showNeoPixel() {
    ring->show();
  }

  // Initialize POT
  void LEDring::initalizePot(int numLEDs) {

    int potVal = analogRead(potPin);
    potMap = map(potVal, 0, maxPot, 0, numLEDs +1);
    colorMap = map(potVal, 0, maxPot, 0, maxRGB);
  }

  //subtract litLEDs
  void LEDring::subtractLitLED(int timeElapsed) {
    if (timeElapsed <= potMap) {

      //turn off a LED after certain amount of time
      int LEDsRemaining = potMap - timeElapsed;  //time elapsed is a the amount of time elapsed per pixel

      for (int i = numLEDs; LEDsRemaining <= i; i--) {
        ring->setPixelColor(i, defColor, defColor, defColor);
      }
      showNeoPixel();
    }
  }
  // NeoPixel mapping
  int LEDring::neoPixelMap() {

    // Read the potentiometer value again
    initalizePot(numLEDs);

    // For turning off LEDs greater than the potMap value
    for (int i = potMap; i < numLEDs; i++) {
      //                       (R,        G,        B)
      ring->setPixelColor(i, defColor, defColor, defColor);
    }
    for (int i = 0; i < potMap; i++) {
      //                       (R,     G,      B)
      ring->setPixelColor(i, RGB[0], RGB[1], RGB[2]);  // Set to default color
    }
    showNeoPixel();

    //  potMap will return the number of lit pixels
    return potMap;
  }
  void LEDring::ResetRGB(){
    RGB[0]={0};
    RGB[1]={0};
    RGB[2]={0};
  }

  void LEDring::updateRGB(uint8_t R, uint8_t G, uint8_t B){
    for (int i =0; i<=numLEDs; ++i){
      ring->setPixelColor(i,R,G,B);

    }
    showNeoPixel();
  }

  uint8_t LEDring::modRGBByIndex(uint8_t a, uint8_t x, unsigned int aIndex, unsigned int xIndex) {
      // Prevent divide-by-zero error
      // if (RGB[xIndex] == 0) return RGB[xIndex];
      if (xIndex == aIndex) return RGB[aIndex];
      else if (xIndex != aIndex) return RGB[xIndex];
      
  }
  //define method to select the color on state "defcolor"
void LEDring::selectRGB( unsigned int index) {
    initalizePot(numLEDs);
    RGB[index] = colorMap;
    uint8_t r = modRGBByIndex(RGB[index], RGB[0], index, 0);
    uint8_t g = modRGBByIndex(RGB[index], RGB[1], index, 1);
    uint8_t b = modRGBByIndex(RGB[index], RGB[2], index, 2);
    updateRGB(r, g, b);
}



    


      
    
