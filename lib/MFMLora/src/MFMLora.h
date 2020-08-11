#ifndef _MFMLORA_H_
#define _MFMLORA_H_

#include <Arduino.h>
#include <avr/wdt.h>
#include <EEPROM.h>
#include <lmic.h>
#include <hal/hal.h>
#include <LowPower.h>

#include "MFMLoraConfig.h"

extern uint8_t doMeasurements(uint8_t *data);
extern void powerDown();
extern void powerUp();

// Define MFMLora class
class MFMLora
{
public:
  static void setup(void);
  static void onEvent(ev_t);
  static void loop();
  static void scheduleCycle();

  static bool PowerOnReset;

private:
  static void sendData(osjob_t *);
  static void sleep(osjob_t *);
  static void doMeasurementsWrapper(osjob_t *);
  static void powerUpWrapper(osjob_t *);
  static void powerDownWrapper(osjob_t *);
  static void setupLMIC(void);
  static bool loadLMIC(void);
  static void saveLMIC(void);

  static osjob_t doMeasurementsJob;
  static osjob_t sleepJob;
  static osjob_t powerDownJob;
  static osjob_t powerUpJob;

  static u16 sleepIterations;
};

#endif