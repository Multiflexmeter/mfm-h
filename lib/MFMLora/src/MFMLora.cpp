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
const u16 PROGMEM MFMLora::sleepIterations = SLEEP_ITERATIONS;

lmic_t EEMEM eeprom_lmic_state;

/**
 * Setup the LoRaWAN framework and join the network
 * with OTAA joining
 */
void MFMLora::setup(void)
{
#ifdef PRINT_BUILD_DATE_TIME
  Serial.print(F("Build at: "));
  Serial.print(F(__DATE__));
  Serial.print(" ");
  Serial.println(F(__TIME__));
#endif

  // If an LMIC State exists
  if (MFMLora::loadLMIC()) {
    MFMLora::scheduleCycle(false);
  } else {
    MFMLora::setupLMIC();
  }
}

/**
 * Loads LMIC State from EEPROM
 */
bool MFMLora::loadLMIC(void) {
  eeprom_read_block(&LMIC, &eeprom_lmic_state, sizeof(lmic_t));

  // If debug, print (part-of) loaded LMIC state
#if MFMLORA_DEBUG > 0
  Serial.print(F("LMIC DevAddr: "));
  Serial.println(LMIC.devaddr, HEX);
  Serial.print(F("LMIC seqnoUp: "));
  Serial.println(LMIC.seqnoUp);
#endif

  // If state was not joined to the network then load is unsuccesful
  if (LMIC.devaddr == 0) {
    return false;
  }
  // If there is a saved state initialize the system
#if MFMLORA_DEBUG > 0
  Serial.print(F("Recovering LMIC state"));
#endif
  os_init();
  os_radio(RADIO_RST);
  LMIC.opmode = OP_NONE;
  return true;
}

/**
 * Save LMIC State to EEPROM
 */
void MFMLora::saveLMIC(void) {
  eeprom_write_block(&LMIC, &eeprom_lmic_state, sizeof(lmic_t));
}

/**
 * Sets up the LMIC framework
 */
void MFMLora::setupLMIC(void) {
  os_init();
  LMIC_reset();

  // If there is no external clock, set a 2% clock_error
  // this relaxes the RX window timing.
#ifdef INTERNAL_CLOCK
  LMIC_setClockError(MAX_CLOCK_ERROR * 2 / 100);
#endif
  // RX Window at SF 9
  LMIC.dn2Dr = DR_SF9;
  // Start transmitting at SF 7
  LMIC_setDrTxpow(DR_SF7, 14);
  // Enable ADR
  LMIC_setAdrMode(0);
  LMIC_startJoining();
}

/**
 * Fires when an LoRaWAN Event occurs.
 * 
 * Delegated from LMIC its `onEvent`.
 */
void MFMLora::onEvent(ev_t ev)
{
  MFMLora::saveLMIC();

  Serial.print(F("Ev: "));
  Serial.println(ev);
  switch (ev)
  {
  case EV_JOINED:
    Serial.println(F("JOINED"));
    // Schedule cycle without sleeping
    MFMLora::scheduleCycle(false);
    break;

  case EV_TXCOMPLETE:
    Serial.println(F("TX_COMPLETE"));
    // Enable linkchecking
    LMIC_setLinkCheckMode(1);
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
  if (sleep)
  {
    // Scheduled asap
    os_setCallback(&MFMLora::powerDownJob, powerDown);
    os_setCallback(&MFMLora::sleepJob, MFMLora::sleep);
    // Scheduled after wakeup (Cheesy, sleepjob takes long)
    // FIX:
    os_setCallback(&MFMLora::powerUpJob, powerUp);
    os_setCallback(&MFMLora::doMeasurementsJob, doMeasurements);
    os_setCallback(&MFMLora::sendDataJob, sendData);
  }
  else
  {
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
    .nss = PIN_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = PIN_RST,
    .dio = {
        PIN_DIO_0,
        PIN_DIO_1,
        PIN_DIO_2},
};

// Store APPEUI DEVEUI and APPKEY in Flash ROM
const u1_t PROGMEM APPEUI[8] = APP_EUI;
const u1_t PROGMEM DEVEUI[8] = DEV_EUI;
const u1_t PROGMEM APPKEY[16] = APP_KEY;

// Provide getters for the LMIC Library
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

// delegate the LMIC its `onEvent` to the MFMLora class
void onEvent(ev_t ev)
{
  MFMLora::onEvent(ev);
}
