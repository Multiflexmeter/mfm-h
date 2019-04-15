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
  memcpy(MFMLora::txData, (void *)arbitraryValue, sizeof(arbitraryValue));
  MFMLora::txDataLen = sizeof(arbitraryValue);
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
