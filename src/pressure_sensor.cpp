#include "pressure_sensor.h"

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */

uint16_t doPressureMeasurement()
{
  uint16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);
  uint16_t mv = 0.1875F * adc0;

  return mv;
}
