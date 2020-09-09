#if !defined(__PRESSURE_SENSOR_H__)
#define __PRESSURE_SENSOR_H__

#include <stdint.h>
#include <Arduino.h>
#include <Adafruit_ADS1015.h>
#include <InterpolationLib.h>

#ifdef __cplusplus
extern "C"
{
#endif

  uint16_t doPressureMeasurement();
  void pressureStop();

#ifdef __cplusplus
}
#endif

#endif