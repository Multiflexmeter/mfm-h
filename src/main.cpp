#include <Arduino.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <MFMLora.h>

/*
  Instrument configuration
*/
// pins
#define RELAY_PIN 2
#define JSNV2_TX 4
#define JSNV2_RX 3
#define ONEWIRE_PIN 5
// settings
#define MAX_DISTANCE 850
#define WARMUP_TIME_MS 200
#define JSNV2_TIMEOUT_MS 100

OneWire oneWire(ONEWIRE_PIN);
DallasTemperature sensors(&oneWire);
SoftwareSerial JSNV2(JSNV2_TX, JSNV2_RX);

struct Packet
{
  unsigned short distance;
  float temperature;
};

unsigned short
getDistance(void)
{
  byte buforek[4];
  unsigned long start;

  JSNV2.write(0x55); //request distance measurement
  JSNV2.flush();
  delay(5);
  start = millis();
  while (!(JSNV2.available() > 0))
  {
    delay(10);
    if ((start + JSNV2_TIMEOUT_MS) < millis())
    { //but dont wait too long
      return -1;
    }
  }
  for (int i = 0; i < 4; i++)
  { //read 4 bytes which contain distance data
    buforek[i] = (byte)JSNV2.read();
    delay(2);
  }

  if (buforek[0] == 0xff && ((buforek[0] + buforek[1] + buforek[2]) & 0x00ff) == buforek[3]) //SUM check
  {
    return ((uint16_t)buforek[1] << 8) + buforek[2]; //Two's complement make 16 bit int
  }
  return 0;
}

float getTemperature(void)
{
  sensors.begin();
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

/**
 * Fired when data is to be collected.
 * 
 * Collected data must be copied to `LMIC.frame`
 */
void doMeasurements(osjob_t *j)
{
  Packet packet{
      getDistance(), getTemperature()};

  // Temporary data preparation
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
  delay(WARMUP_TIME_MS);

  JSNV2.begin(9600);
}

/**
 * Fired before sleeping.
 * 
 * You should powerdown instruments here.
 */
void powerDown(osjob_t *j)
{
  JSNV2.end();
  digitalWrite(JSNV2_TX, LOW);
  digitalWrite(JSNV2_RX, LOW);
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
