#ifndef _MFMLORA_H_
#define _MFMLORA_H_

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include "MFMLoraConfig.h"

extern void doMeasurements(osjob_t *);

// Define MFMLora class
class MFMLora
{
public:
  static void setup(void);
  static void onEvent(ev_t);
  static void loop();

private:
  static void sendData(osjob_t *);
  
  static osjob_t doMeasurementsJob;
  static osjob_t sendDataJob;
};

#endif