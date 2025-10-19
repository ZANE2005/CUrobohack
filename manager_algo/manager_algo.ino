// ============================================================================
// File: manager_algo.ino
// Purpose: Top-level sketch. Owns setup()/loop() and routes into the FSM.
// Why: Keep .ino minimal so architecture is reusable across projects.
//
// FSM OVERVIEW
//   START        : Initialize LED mapping; scan button
//   PRIMED       : Wait for short/long press to branch
//   COLOR_SELECT : Cycle through RGB palette on short press; exit on wrap
//   COUNT_DOWN   : Timer-driven LED decrease
//   ALARM        : Play alarm profile
//   RESET        : Return to START
//
// Notes:
// - Non-blocking design: buttons are polled every tick; timer uses millis().
// - Alarm uses Timer profiles to avoid magic numbers.
// ======================================================== ====================


#include "led_ring.h"
#include "clock.h"
#include "button_debounce.h"

// Define pins

//changed from const -> constexpr type modifyer 
constexpr uint8_t RING_PIN   = 3;
constexpr uint8_t POT_PIN    = 14;
constexpr uint8_t BUZZER_PIN = 5;
constexpr uint8_t BUTTON_PIN = 4;

// Define instances of hardware components
LEDring ring(RING_PIN, POT_PIN);
Button  button(BUTTON_PIN);
Timer   timer(BUZZER_PIN);


//struct to store main data
typedef struct {
  int litLEDs;
  const int debounceDelay = 5, longPress = 2000;
  unsigned int RGBindex = 0;
} massData;

//create global instance of struct (perhaps change to make a local instance when nessasary )
massData data;

//define the states as an enumeration
typedef enum {
  START,
  PRIMED,
  COLOR_SELECT,
  COUNT_DOWN,
  ALARM,
  RESET,
  NUM_STATES

} MachineState;

//state functions for FSM
void start();
void primed();
void colorSelect();
void countDown();
void alarm();
void reset();

//create a struct to map each state to its corresponding function.
typedef struct {
  MachineState stateName;
  void (*stateFunc)(void);

} StateNode;

//define an array of StateNode
StateNode stateMachine[NUM_STATES] = {
  { START, start },
  { PRIMED, primed },
  { COLOR_SELECT, colorSelect },
  { COUNT_DOWN, countDown },
  { ALARM, alarm },
  { RESET, reset }

};


//declare default state
MachineState currentState = START;

//define manager algo
void masterAlgo() {
  stateMachine[currentState].stateFunc();
}

void setup() {
  // Initialize Serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  //run the masterAlgo FSM
  masterAlgo();
}

//define function for each state in MachineState

void start() {
  data.litLEDs = ring.neoPixelMap();
  button.ButtonDebounce(data.debounceDelay, data.longPress);
  
  if (button.isShortPressed()) {
    timer.alarm(Timer::PROFILE_I);
    currentState = PRIMED;
  }
}

void primed() {
  button.ButtonDebounce(data.debounceDelay, data.longPress);
  if (button.isLongPressed()) {
    timer.alarm(Timer::PROFILE_II);
    currentState = COLOR_SELECT;
  }
  
  else if (button.isShortPressed()) {
      timer.alarm(Timer::PROFILE_III);
      currentState = COUNT_DOWN;
  }
}
void colorSelect() {
  
  if (data.RGBindex == 0) {
    ring.ResetRGB();
  }
  button.ButtonDebounce(data.debounceDelay, data.longPress);
  ring.selectRGB(data.RGBindex);

  if (button.isShortPressed() && data.RGBindex <= 2) {
    timer.alarm(Timer::PROFILE_I);
    ++data.RGBindex;
    currentState = COLOR_SELECT;
    } 
  else if (data.RGBindex > 2) {
    timer.alarm(Timer::PROFILE_II);
    data.RGBindex = 0;
    currentState = START;
  }

}


void countDown() {

  timer.countDown(data.litLEDs);
  ring.subtractLitLED(timer.timeElapsed);

  if (timer.durationOver()) {
    currentState = ALARM;
  }
}
void alarm() {
  timer.alarm(Timer::PROFILE_IV);
  currentState = RESET;
}
void reset() {
  currentState = START;
  
}
