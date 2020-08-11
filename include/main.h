#if !defined(_MAIN_H_)
#define _MAIN_H_

#include <Arduino.h>
#include <MFMLora.h>
#include <MedianFilter.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_ADS1015.h>
#include <InterpolationLib.h>
#include <DallasTemperature.h>
#include <LowPower.h>
#include <avr/power.h>

#define WARMUP_TIME_MS 10

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define RELAY_PIN 2
#define ONEWIRE_PIN 17 //or A3
#define BMP_PIN 14     //or A0 (DIO0)

#define SEALEVELPRESSURE_HPA (1018.26)
#define PRESSURE_LOW_mV 500
#define PRESSURE_HIGH_mV 4500
#define PRESSURE_LOW_cm 0
#define PRESSURE_HIGH_cm 1000

#endif