// ============================================================================
// File: button_debounce.cpp
// Impl notes:
// - Active-LOW button expected (INPUT_PULLUP). Adjust if  wiring differs.
// - Debounce uses a simple "settle time" window before accepting a change.
// - Long press edge fires once when threshold reached (even if still held).
// ============================================================================

#include"button_debounce.h"
#include<Arduino.h>


//default constructor
Button::Button(const unsigned int inputPin):state(0),prevState(0), buttonReading(0), curTime(0), prevTime(0),  buttonPin(inputPin){

  pinMode(buttonPin, INPUT);

}

//check if button is pressed
bool Button::buttonPressed()const {
  return digitalRead(buttonPin)==HIGH;
}

//check if button isn't pressed
bool Button::buttonReleased()const {
  return digitalRead(buttonPin)==LOW;
}

//check state in buttonDebounce to determine if shortPress
bool Button::isShortPressed()const {
  return state ==SHORT_PRESS;
}
//check state in buttonDebounce to determine if long press
bool Button::isLongPressed()const {
  return state==LONG_PRESS ;
}


//method gives hold down capabilities to button debounce
void Button::ButtonDebounce(const unsigned long bounceDelay,const unsigned long longPress)  {

  prevState = state;

  switch(state) {
    
    case RESET:
      // isTriggered = false;
      curTime = prevTime = 0;
      state = START;
      break;

    case START:
      if (buttonPressed()) {
        state = GO;
      }
      break;

    case GO:
      prevTime = millis();
      state = WAIT;
      break;

    case WAIT:
      curTime = millis();
      //_____________________________________________________________________
      if (curTime - prevTime >=bounceDelay) {
        state = DRAWN;
      }

      if (buttonReleased()) {
        state = RESET;
      }
      break;

    // DRAWN
    case DRAWN:
      curTime = millis();
      if(buttonReleased()){
        state = SHORT_PRESS;
      }
      else if(curTime-prevTime > longPress ){
        state = HOLD;
      }
      break;

    case SHORT_PRESS:
      Serial.println("SHORT PRESS");
      state = RESET;
      break;

    //HOLD
    case HOLD:
      state = LONG_PRESS;
    break;

    //RELEASE WAIT
    case LONG_PRESS:
      //Perhaps this needs fixing
      Serial.println("LONG PRESS");
      state = RESET;
    break;

  }

  
  // FOR TESTING PURPOSES:
  
  // Serial.print( state);
  // Serial.print(" [] ");
  // Serial.print( curTime-prevTime);
  // Serial.println(" [] ");
    



  

}




