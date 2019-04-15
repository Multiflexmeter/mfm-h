#include <Arduino.h>
#include <MFMLora.h>

/**
 * Fired when data is to be collected.
 * 
 * Collected data must be copied to `LMIC.frame`
 */
void doMeasurements(osjob_t* j)
{
  int arbitraryValue = 64;
  
  // Temporary data preparation
  memcpy(LMIC.pendTxData, (void *)arbitraryValue, 1);
  LMIC.pendTxLen = 1;
}

/**
 * Fired after ATMega reset
 */
void setup()
{
  Serial.begin(9600);
  MFMLora::setup();
}

/**
 * Fired every cycle
 */
void loop()
{
  MFMLora::loop();
}
