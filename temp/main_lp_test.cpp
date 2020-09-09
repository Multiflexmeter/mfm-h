#include <Arduino.h>
#include <BMP280_DEV.h>
#include <Adafruit_ADS1015.h>
#include <LowPower.h>
#include <Wire.h>
#include <RH_RF95.h>

BMP280_DEV bmp;
Adafruit_ADS1115 ads2;
TwoWire wire;
RH_RF95 rfm(10, 8);

static void writeRegister(uint8_t reg, uint16_t value)
{
  Wire.beginTransmission(0x48);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)(value >> 8));
  Wire.write((uint8_t)(value & 0xFF));
  Wire.endTransmission();
}

void setup(void)
{
  delay(100);
  bmp.begin(
      Mode::SLEEP_MODE,
      Oversampling::OVERSAMPLING_X2,
      Oversampling::OVERSAMPLING_X2,
      IIRFilter::IIR_FILTER_16,
      TimeStandby::TIME_STANDBY_250MS);
  ads2.begin();

  // Start with default values
  uint16_t config =
      ADS1015_REG_CONFIG_CQUE_NONE |    // Disable the comparator (default val)
      ADS1015_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1015_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      0x0 |                             // 8 samples per second
      ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  delay(10);

  writeRegister(ADS1015_REG_POINTER_CONFIG, config);

  rfm.init();
  rfm.sleep();

  DDRB = 0;
  DDRC = 0;
  DDRD = 0;
  PORTB = 1;
  PORTC = 1;
  PORTD = 1;
  DDRB = 0;
  DDRC = 0;
  DDRD = 0;
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
  PRR = 1;
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}

void loop(void)
{
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}