#include "MFMLora.h"

/*
  Define MFMLora jobs
*/
osjob_t MFMLora::doMeasurementsJob;
osjob_t MFMLora::sendDataJob;

/**
 * Setup the LoRaWAN framework and join the network
 * with OTAA joining
 */
void MFMLora::setup(void)
{
  os_init();
  LMIC_reset();
  // If there is no external clock, set a 2% clock_error
  // this relaxes the RX window timing.
#ifndef _EXTERNAL_CLOCK
  LMIC_setClockError(MAX_CLOCK_ERROR * 2 / 100);
#endif
  // RX Window at SF 9
  LMIC.dn2Dr = DR_SF9;
  // Start transmitting at SF 7
  LMIC_setDrTxpow(DR_SF7, 14);
  LMIC_startJoining();
}

/**
 * Fires when an LoRaWAN Event occurs.
 * 
 * Delegated from LMIC its `onEvent`.
 */
void MFMLora::onEvent(ev_t ev)
{
  switch (ev)
  {
  case EV_JOINED:
    Serial.println(F("JOINED"));
    // Trigger measurements and sending
    os_setCallback(&MFMLora::doMeasurementsJob, doMeasurements);
    os_setCallback(&MFMLora::sendDataJob, sendData);
    break;

  case EV_TXCOMPLETE:
    Serial.println(F("TX_COMPLETE"));
    break;

  default:
    break;
  }
}

/**
 * Loops the LoRaWAN timing system.
 * 
 * WARNING: You should never use the loop
 * for anything else then this!
 */
void MFMLora::loop()
{
  os_runloop_once();
}

void MFMLora::sendData(osjob_t* j)
{
  Serial.println(F("Sending..."));
}

/*
  Delegation from config files to LMIC its framework
*/

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = _PIN_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = _PIN_RST,
    .dio = {
        _PIN_DIO_0,
        _PIN_DIO_1,
        _PIN_DIO_2},
};

// Store APPEUI DEVEUI and APPKEY in Flash ROM
const u1_t PROGMEM APPEUI[8] = _APP_EUI;
const u1_t PROGMEM DEVEUI[8] = _DEV_EUI;
const u1_t PROGMEM APPKEY[16] = _APP_KEY;

// Provide getters for the LMIC Library
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

// delegate the LMIC its `onEvent` to the MFMLora class
void onEvent(ev_t ev)
{
  MFMLora::onEvent(ev);
}