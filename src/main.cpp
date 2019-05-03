#include <Arduino.h>
#include <DallasTemperature.h>
#include <MFMLora.h>

/*
  Instrument configuration
*/
// Length of sensor array
#define SENSOR_COUNT 17
// pins
#define RELAY_PIN 2
#define WARMUP_TIME_MS 100
#define ONEWIRE_PIN 6

OneWire oneWire(ONEWIRE_PIN);
DallasTemperature sensors(&oneWire);

// DS18B20 sensors in order (TOP -> BOTTOM)
// We only need bytes 3 through 8
// Technically we can calculate the CRC (8th) byte
const unsigned char SENSORS_ADDRESSES[SENSOR_COUNT * 6] = {
  0x75, 0x34, 0x2E, 0x16, 0x01, 0xA5,
  0xAF, 0xC0, 0x2C, 0x16, 0x02, 0x94,
  0xDF, 0xB2, 0x2C, 0x16, 0x02, 0x56,
  0x73, 0xC8, 0x2C, 0x16, 0x02, 0xF7,
  0xDE, 0x8E, 0x2E, 0x16, 0x01, 0x6C,
  0x13, 0x3C, 0x2E, 0x16, 0x01, 0x34,
  0x3F, 0x11, 0x2E, 0x16, 0x01, 0x00,
  0xE4, 0x26, 0x2E, 0x16, 0x01, 0xE0,
  0xD6, 0xB4, 0x2C, 0x16, 0x02, 0xAC,
  0xF1, 0x8B, 0x2E, 0x16, 0x01, 0x7A,
  0x12, 0x8F, 0x2E, 0x16, 0x01, 0xE0,
  0x51, 0xC6, 0x2C, 0x16, 0x02, 0x99,
  0x79, 0xC6, 0x2C, 0x16, 0x02, 0x5F,
  0xBD, 0x62, 0x2C, 0x16, 0x02, 0xC5,
  0x78, 0xB2, 0x2C, 0x16, 0x02, 0x34,
  0x00, 0xB5, 0x2C, 0x16, 0x02, 0xE1,
  0x19, 0x61, 0x2C, 0x16, 0x02, 0x61
};

struct Packet
{
  float temperature[SENSOR_COUNT];
};

/**
 * Fired when data is to be collected.
 * 
 * Collected data must be copied to `MFMLora::txData`
 */
void doMeasurements(osjob_t *j)
{
  Packet packet;
  for (unsigned char i = 0; i < SENSOR_COUNT; i++)
  {
    // Prepend address with manufacturer + id
    unsigned char address[8] = {0x28, 0xEE};
    // Copy serial + CRC Byte at a 2 byte offset
    memcpy(&address[i], &SENSORS_ADDRESSES[i * 6] + 0x2, 6);
    // Read and place temperature in packet
    packet.temperature[i] = sensors.getTempC(address);
  }
  // Copy packet struct to txData
  memcpy(MFMLora::txData, (void *)&packet, sizeof(packet));
  MFMLora::txDataLen = sizeof(packet);
}

/**
 * Fired before doing measurements.
 * 
 * You should activate/reset/prepare your instruments here.
 */
void powerUp(osjob_t *j)
{
  digitalWrite(RELAY_PIN, HIGH);
  // Wait for instruments to come online
  delay(WARMUP_TIME_MS);
  // Start DS 1-wire bus
  sensors.begin();
}

/**
 * Fired before sleeping.
 * 
 * You should powerdown instruments here.
 */
void powerDown(osjob_t *j)
{
  digitalWrite(RELAY_PIN, LOW);
}

/**
 * Fired after ATMega reset
 */
void setup()
{
  Serial.begin(9600);
  // Set pinmode
  pinMode(RELAY_PIN, OUTPUT);
  // Enable instruments
  digitalWrite(RELAY_PIN, HIGH);
  // Fire as last
  MFMLora::setup();
}

/**
 * Fired every cycle
 */
void loop()
{
  MFMLora::loop();
}
