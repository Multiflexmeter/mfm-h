#include "pressure_sensor.h"

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

static void writeRegister(uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(0x48);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)(value >> 8));
  Wire.write((uint8_t)(value & 0xFF));
  Wire.endTransmission();
}

uint16_t doPressureMeasurement()
{
  uint16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);
  uint16_t mv = 0.1875F * adc0;

  return mv;
}

void pressureStop()
{
  // Start with default values
  uint16_t config =
      ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      0x0 |                             // 8 samples per second
      ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  writeRegister(ADS1015_REG_POINTER_CONFIG, config);
}
