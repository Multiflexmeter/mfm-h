#ifndef _MFMLORA_H_
#define _MFMLORA_H_

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <LowPower.h>
#include "MFMLoraConfig.h"

extern void doMeasurements(osjob_t *);

// Define MFMLora class
class MFMLora
{
public:
  static void setup(void);
  static void onEvent(ev_t);
  static void loop();
  static u1_t txData[MAX_LEN_PAYLOAD];
  static u1_t txDataLen;

private:
  static void sendData(osjob_t *);
  static void sleep(osjob_t *);

  static osjob_t doMeasurementsJob;
  static osjob_t sendDataJob;
  static osjob_t sleepJob;

  const static u16 PROGMEM sleepIterations;
};

#endif