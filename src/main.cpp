/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP280 Breakout
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

//BMP280_CHIPID       = 0x58
//BME280_CHIPID       = 0x60
//BMP280_ADDRESS      = 0x77
//BMP280_ADDRESS_ALT  = 0x76

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_ADS1015.h>
#include <InterpolationLib.h>
#include <DallasTemperature.h>
#include <LowPower.h>
#include <avr/power.h>

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

const int numValues = 2;
double xValues[2] = {PRESSURE_LOW_mV, PRESSURE_HIGH_mV};
double yValues[2] = {PRESSURE_LOW_cm, PRESSURE_HIGH_cm};

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */
OneWire oneWire(ONEWIRE_PIN);
DallasTemperature sensors(&oneWire);
Adafruit_BMP280 bmp; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

float getTemperature(void)
{
    float temp = 0;
    sensors.requestTemperatures();
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    temp = sensors.getTempCByIndex(0);
    Serial.print("Watertemperature: ");
    Serial.print(temp);
    Serial.println(" Celcius");
    return temp;
}

void printValues()
{
    bmp.readTemperature();
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    bmp.readPressure();
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure() / 100);
    Serial.println(" hPa");

    bmp.readAltitude();
    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA)); /* Adjusted to local forecast! */
    Serial.println(" m");
}

void ReadPressureSensor()
{
    int16_t adc0;
    adc0 = ads.readADC_SingleEnded(0);
    int16_t depth = 0.1875F * adc0; //see table in setup for which value to multiply with, in this case with 1875mV

    double depth_cm = Interpolation::Linear(xValues, yValues, numValues, depth, true);

    Serial.print("depth: ");
    Serial.print(depth);
    Serial.println("mV");
    Serial.print("depth cm: ");
    Serial.print(depth_cm);
    Serial.println("cm");
}

void WakeupInstruments()
{
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BMP_PIN, HIGH);
    power_twi_enable(); //Enable TWI (I2C)
    Wire.begin();

    delay(100);

    if (!bmp.begin())
    {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    ads.begin();     //start external ADC
    sensors.begin(); //start DS18B20
}

void Sleep(uint16_t sleeptime)
{
    Serial.println(F("Entering sleep"));
    Serial.flush();

    // bmp.setSampling(Adafruit_BMP280::MODE_SLEEP,      /* Operating Mode. */
    //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    //                 Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    //power_twi_disable(); // Disable TWI (I2C)
    //TWCR &= ~(bit(TWEN) | bit(TWIE) | bit(TWEA)); // turn off I2C

    // Disable digital input buffers on all ADC0-ADC5 pins
    DIDR0 = 0x3F;
    //  Disable I2C interface so we can control the SDA and SCL pins directly
    TWCR &= ~(_BV(TWEN));
    //  disable I2C module this allow us to control SCA/SCL pins and reinitialize the I2C bus at wake up
    TWCR = 0;
    // set I2C pin as input no pull up this prevent current draw on I2C pins that completly destroy our low power mode
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);

    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BMP_PIN, LOW);

    delay(100);

    uint8_t i = 0;
    for (i = 0; i < sleeptime; i++)
    {
        LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    }

    WakeupInstruments();
}

void setup()
{
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BMP_PIN, OUTPUT);
    // digitalWrite(RELAY_PIN, HIGH);
    //digitalWrite(BMP_PIN, HIGH);

    delay(500);
    Serial.begin(9600);
    while (!Serial)
        ; // time to get serial running
    Serial.println(F("BMP280 test"));

    /* unsigned status;
    
    // default settings
    status = bme.begin(0x76, &Wire);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
*/

    WakeupInstruments();

    /* Default settings from datasheet. */
    //   bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    //                   Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    //                   Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    //                   Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    //                   Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    // The ADC input range (or gain) can be changed via the following
    // functions, but be careful never to exceed VDD +0.3V max, or to
    // exceed the upper and lower limits if you adjust the input range!
    // Setting these values incorrectly may destroy your ADC!
    //                                                                ADS1015  ADS1115
    //                                                                -------  -------
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

    Serial.println("-- Default Test --");
    Serial.println("ADC Range: +/- 6.144V (1 bit = 0.1875mV for ADS1115)");

    Serial.println();
}

void loop()
{
    printValues();
    ReadPressureSensor();
    getTemperature();
    getTemperature();
    Serial.println();
    // delay(4000);
    Sleep(2);
}