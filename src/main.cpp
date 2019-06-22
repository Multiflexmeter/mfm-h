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
#define BUZZER_PIN 17
// settings
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

unsigned short getDistance(void)
{
  uint8_t buffer[4];
  unsigned long start;

  // send "Start Reading" command to JSNV2
  JSNV2.write(0x55);
  Serial.flush();
  // Wait until there is data available
  start = millis();
  while (!(JSNV2.available() > 0))
  {
    delay(10);
    if (start + JSNV2_TIMEOUT_MS < millis())
    {
      return -1;
    }
  }
  // There is data availabe, start parsing it.
  // Read four bytes
  JSNV2.readBytes(buffer, 4);
  // Verify checksum
  if (buffer[0] == 0xff && ((buffer[0] + buffer[1] + buffer[2]) & 0x00ff) == buffer[3])
  {
    // Calculate distance
    unsigned short dist = (buffer[1] << 8) | buffer[2]; //distance
    return float(dist) / 10.0f;
  }

  // The packet was invalid.
  return -2;
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
  JSNV2.begin(9600);
  // Wait for the JSN to come online
  delay(WARMUP_TIME_MS);
}

/**
 * Fired before sleeping.
 * 
 * You should powerdown instruments here.
 */
void powerDown(osjob_t *j)
{
  JSNV2.end();
  digitalWrite(RELAY_PIN, LOW);
}

/**
 * Reset beep
 */
void doBuzzerBeep(){
  tone(BUZZER_PIN, 10000);
  delay(180);
  tone(BUZZER_PIN, 5000);
  delay(180);
  tone(BUZZER_PIN, 1000);
  delay(180);
  tone(BUZZER_PIN, 750);
  delay(180);
  tone(BUZZER_PIN, 500);
  delay(180);
  tone(BUZZER_PIN, 100);
  delay(180);
  
  noTone(BUZZER_PIN);
  digitalWrite (BUZZER_PIN, LOW);
}

/**
 * Fired after ATMega reset
 */
void setup()
{
  doBuzzerBeep();
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
