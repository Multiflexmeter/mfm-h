#include "sensors.h"

#include <Arduino.h>
#include <OneWire.h>
#include "DallasTemperature.h"
#include "debug.h"
#include "config.h"

OneWire wire(17);
DallasTemperature ds(&wire);

#define address_count 3
uint8_t addresses[address_count * 8] = {
  0x28,0x1B,0x69,0x19,0x0D,0x00,0x00,0xE4, // Device address 1 // Deepest in the water
  0x28,0x04,0x7E,0x1A,0x0D,0x00,0x00,0x3E, // Device address 2
  0x28,0x63,0x71,0x19,0x0D,0x00,0x00,0x9F, // Device address 3 // Closes to the surface
};

void smbus_init(void)
{
  ds.begin();
}

void smbus_doMeasurements(void)
{
  ds.requestTemperatures();
}

uint8_t smbus_getMeasurement(uint8_t *buf)
{
  uint16_t temperature = 0;
  for (uint8_t ix = 0; ix < address_count; ix++)
  {
    temperature = (uint16_t)(ds.getTempC(&addresses[ix * 8]) * 10);
    buf[ix * 2] = temperature & 0xff;
    buf[ix * 2 + 1] = (temperature >> 8) & 0xff;
  }

  return address_count * 2;
}
