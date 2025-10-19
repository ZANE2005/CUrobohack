// ============================================================================
// File: clock.h
// Purpose: Timer class providing non-blocking timing, countdown logic, and
//          customizable alarm playback via simple "profiles" (volume/delay/rings).
// Why: Replaces delay() with millis()-based scheduling so the rest of 
//      system (button reads, LEDs, sensors) stays responsive.
// ============================================================================

#ifndef CLOCK
#define CLOCK

#include <Arduino.h>

class Timer {

public:
  // initialize public variables
  unsigned int timeElapsed = 0;

  // default constructor
  Timer(const unsigned int buzzerOutPin);

  // method plays a customizable alarm (via external profile)
  struct AlarmProfile {
    uint8_t  volume;
    uint16_t delayTime;
    uint8_t  numRings;
  };

  // declare public, pre-defined profiles (definitions in .cpp)
  static const AlarmProfile PROFILE_I;
  static const AlarmProfile PROFILE_II;
  static const AlarmProfile PROFILE_III;
  static const AlarmProfile PROFILE_IV;

  void alarm(const AlarmProfile& profile);

  // method sets a timer and counts down based on how many LEDs are on
  void countDown(int LEDsOn);

  // checks if LED should be subtracted
  bool durationOver() const;

  // get the current time delta
  unsigned long getCurTime() const;

  // non-blocking delay helper used by alarm()
  bool timerDelay(unsigned long dt);

  // map potentiometer reading to volume (0.0–1.0)
  float potVolControl(float potVal);

private:
  // state variables
  unsigned int state = 0, prev_state = 0;

  // timer states
  typedef enum {
    RESET,
    INIT_TIMER,
    SUB_LED,
    TRANSITION,
  } TimerStates;

  // buzzer pin
  const unsigned int buzzerPin;

  // maybe change this if not needed
  unsigned long timePerLED;

  // time constants
  const unsigned long defTimePerLed = 2000, min = 60000;

  // time bookkeeping
  unsigned long prevTime = 0, curTime = 0;

  // alarm timing variables
  unsigned long alarmCur = 0, alarmPrev = 0;

  unsigned int LEDsOn = 0;

  float alarmResetFactor = 7.5f;

  // maximum pot value
  static constexpr float maxPot = 1023.0f;

  // standard values for alarm (declared here, defined in .cpp)
  static const unsigned int alarmVol;
  static const unsigned int alarmPause;
};

#endif
