// ============================================================================
// File: button_debounce.h
// Purpose: Debounced button input with short/long press edge detection.
// Why: Raw digitalRead() chatters; this class filters noise and gives
//      clean "events" (isShortPressed/isLongPressed) to drive FSM.
// Notes:
//   - isShortPressed()/isLongPressed() return true exactly once per event.
// ============================================================================


#ifndef BUTTON
#define BUTTON

#include<Arduino.h>
#include<string.h>

class Button{
  
  public:
  // constructor 
  Button(const unsigned int inputPin);

  //consider adding copy constructors for additional buttons
  void ButtonDebounce(const unsigned long bounceDelay, const unsigned long longPress)  ;

  bool isLongPressed()const ;

  bool isShortPressed()const ;

  private:

  //define state enum
  typedef enum {
    RESET,
    START,
    GO,
    WAIT,
    DRAWN ,
    SHORT_PRESS,
    HOLD,
    LONG_PRESS,
    RELEASE_WAIT
  } ButtonStates;

  //define button pin
  const unsigned int buttonPin;

  //define private variables
  const unsigned  int defBounceDelay = 5;

  unsigned int  prevState, buttonReading, state = 0;
  
  unsigned long curTime, prevTime;

  //function to check if button is pressed (LOW)
  bool buttonPressed()const ;

  //funciton to check if button is not pressed (HIGH)
  bool buttonReleased()const ;


};
#endif 