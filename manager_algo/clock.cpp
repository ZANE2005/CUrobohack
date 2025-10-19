// ============================================================================
// File: clock.cpp
// Impl notes:
// - All waits use millis() via timerDelay(), not delay().
// - alarm() consumes a profile {volume, delayTime, numRings} so you can avoid
//   magic numbers in your FSM and keep behavior consistent.
// ============================================================================

#include "clock.h"
#include <Arduino.h>

// define static constants
const unsigned int Timer::alarmVol   = 10;
const unsigned int Timer::alarmPause = 25;

// define public alarm profiles
const Timer::AlarmProfile Timer::PROFILE_I   = { Timer::alarmVol,   Timer::alarmPause,       1 };
const Timer::AlarmProfile Timer::PROFILE_II  = { Timer::alarmVol,   Timer::alarmPause + 5,   3 };
const Timer::AlarmProfile Timer::PROFILE_III = { Timer::alarmVol,   Timer::alarmPause,       2 };
const Timer::AlarmProfile Timer::PROFILE_IV  = { Timer::alarmVol,   Timer::alarmPause * 4U,  4 };

Timer::Timer(const unsigned int buzzOutPin)
  : buzzerPin(buzzOutPin), timePerLED(defTimePerLed) {}

// plays an alarm according to a profile
void Timer::alarm(const AlarmProfile& profile) {
  for (uint8_t i = 0; i < profile.numRings; i++) {
    // two pulses per ring 
    for (uint8_t j = 0; j < 2; j++) {
      tone(buzzerPin, profile.volume);
      // wait profile.delayTime 
      while (!timerDelay(profile.delayTime)) { /* spin */ }
      noTone(buzzerPin);
      while (!timerDelay(profile.delayTime)) { /* spin */ }
    }
    // reset gap between rings
    unsigned long resetMs = (unsigned long)(profile.delayTime * alarmResetFactor);
    while (!timerDelay(resetMs)) { /* spin */ }
  }
}

bool Timer::timerDelay(unsigned long dt) {
  alarmCur = millis();
  if (alarmCur - alarmPrev >= dt) {
    alarmPrev = alarmCur;
    return true;
  }
  return false;
}

float Timer::potVolControl(float potVal) {
  // returns 0.0 .. 1.0
  if (potVal < 0) potVal = 0;
  if (potVal > maxPot) potVal = maxPot;
  return potVal / maxPot;
}

// method checks to sound alarm based on "countDown" method's state
bool Timer::durationOver() const {
  return state == TRANSITION;
}

unsigned long Timer::getCurTime() const {
  return curTime - prevTime;
}

void Timer::countDown(int LEDsOnParam) {
  LEDsOn = (unsigned int)LEDsOnParam;

  switch (state) {
    // reset
    case RESET:
      prevTime = millis();
      state = INIT_TIMER;
      break;

    // initialize timer
    case INIT_TIMER:
      curTime = millis();
      if (curTime - prevTime >= timePerLED) {
        state = SUB_LED;
      }
      break;

    // decrement LED
    case SUB_LED:
      timeElapsed++;
      if (timeElapsed < LEDsOn) {
        state = RESET;
      } else if (timeElapsed == LEDsOn) {
        state = TRANSITION;
      }
      break;

    // reset after duration runs out
    case TRANSITION:
      timeElapsed = 0;
      state = RESET;
      break;
  }

  /*
  // For debugging purposes:
  Serial.print(state);
  Serial.print(" [] ");
  Serial.print(curTime - prevTime);
  Serial.print(" [] ");
  Serial.print(timeElapsed);
  Serial.print(" [] ");
  Serial.println(LEDsOn);
  */
}
