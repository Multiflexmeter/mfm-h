#include "MFMLora.h"

/*
  Define MFMLora jobs
*/
osjob_t MFMLora::doMeasurementsJob;
osjob_t MFMLora::sendDataJob;
osjob_t MFMLora::sleepJob;
osjob_t MFMLora::powerDownJob;
osjob_t MFMLora::powerUpJob;

/*
  Define MFMLora properties
*/
u1_t MFMLora::txData[MAX_LEN_PAYLOAD];
u1_t MFMLora::txDataLen;
const u16 PROGMEM MFMLora::sleepIterations = _SLEEP_ITERATIONS;

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
    // Schedule cycle without sleeping
    MFMLora::scheduleCycle(false);
    break;

  case EV_TXCOMPLETE:
    Serial.println(F("TX_COMPLETE"));
    // Schedule cycle
    MFMLora::scheduleCycle(true);
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

/**
 * Schedules a cycle optionally with sleeping.
 */
void MFMLora::scheduleCycle(bool sleep)
{
  if (sleep) {
    // Scheduled asap
    os_setCallback(&MFMLora::powerDownJob, powerDown);
    os_setCallback(&MFMLora::sleepJob, MFMLora::sleep);
    // Scheduled after wakeup
    os_setTimedCallback(&MFMLora::powerUpJob, os_getTime() + sec2osticks(MFMLora::sleepIterations * 8), powerUp);
    os_setTimedCallback(&MFMLora::doMeasurementsJob, os_getTime() + sec2osticks(MFMLora::sleepIterations * 8), doMeasurements);
    os_setTimedCallback(&MFMLora::sendDataJob, os_getTime() + sec2osticks(MFMLora::sleepIterations * 8), sendData);
  } else {
    // Scheduled asap
    os_setCallback(&MFMLora::powerUpJob, powerUp);
    os_setCallback(&MFMLora::doMeasurementsJob, doMeasurements);
    os_setCallback(&MFMLora::sendDataJob, sendData);
  }
}

/**
 * Sends MFMLora::txData to uplink
 * 
 * Fired after measurements have been copied to
 * txData by doMeasurements.
 */
void MFMLora::sendData(osjob_t *j)
{
  // Send tx data
  LMIC_setTxData2(1, MFMLora::txData, MFMLora::txDataLen, 0);
}

void MFMLora::sleep(osjob_t *j)
{
  Serial.flush();
  noInterrupts();
  for (u16 i = 0; i < MFMLora::sleepIterations; i++)
  {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  interrupts();
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
