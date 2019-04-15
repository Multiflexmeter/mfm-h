#ifndef _MFMLORA_H_
#define _MFMLORA_H_

#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include "MFMLoraConfig.h"

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = _PIN_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = _PIN_RST,
    .dio = {
      _PIN_DIO_0,
      _PIN_DIO_1,
      _PIN_DIO_2
    },
};

// Store APPEUI DEVEUI and APPKEY in Flash ROM
const u1_t PROGMEM APPEUI[8] = _APP_EUI;
const u1_t PROGMEM DEVEUI[8] = _DEV_EUI;
const u1_t PROGMEM APPKEY[16] = _APP_KEY;

// Provide getters for the LMIC Library
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }


class MFMLora
{
public:
  static void setup(void);

private:
};

#endif