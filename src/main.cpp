#include <main.h>

// Adafruit_BMP280 bmp; // I2C
BMP280_DEV bmp;

struct Packet
{
  uint16_t a;
  float b;
  float c;
};

void twi_disable()
{
  TWCR &= ~(1 << TWEN);
  power_twi_disable();

  // Disable PC5(SCL) PC4(SDA)
  DDRC |= (1 << PC4) | (1 << PC5);
  PORTC |= (1 << PC4) | (1 << PC5);
}

void twi_enable()
{
  power_twi_enable();
  TWCR |= (1 << TWEN);
}

/**
 * Fired when data is to be collected.
 * 
 * Collected data must be copied to `LMIC.frame`
 */
uint8_t doMeasurements(uint8_t *data)
{
  float temp, pres;
  bmp.startForcedConversion();
  uint8_t retries = 0;
  while (!bmp.getTempPres(temp, pres) && retries++ < 3)
  {
    delay(10);
  };
  if (retries >= 3)
  {
    Serial.println(F("Failed to get TempPres"));
  }

  // Setup packet
  Packet packet{
      doPressureMeasurement(),
      temp,
      pres,
  };

  // Return measurement packet
  memcpy(data, (void *)&packet, sizeof(packet));
  return sizeof(packet);
}

/**
 * Fired before doing measurements.
 * 
 * You should activate/reset/prepare your instruments here.
 */
void powerUp()
{
  digitalWrite(RELAY_PIN, HIGH);
  twi_enable();
  // Wait for the JSN to come online
  delay(WARMUP_TIME_MS);

  uint8_t success = bmp.begin(
      Mode::FORCED_MODE,
      Oversampling::OVERSAMPLING_X2,
      Oversampling::OVERSAMPLING_X2,
      IIRFilter::IIR_FILTER_16,
      TimeStandby::TIME_STANDBY_250MS);

  if (!success)
  {
    Serial.println(F("BMP280 begin failed"));
  }
}

/**
 * Fired before sleeping.
 * 
 * You should powerdown instruments here.
 */
void powerDown()
{
  bmp.stopConversion();
  pressureStop();
  twi_disable();
  // Pull all low
  DDRB = 1;
  DDRC = 1;
  DDRD = 1;
  PORTB = 0;
  PORTC = 0;
  PORTD = 0;
}

/**
 * Fired after ATMega reset
 */
void setup()
{
  pinMode(ONEWIRE_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(9600);
  // Enable instruments
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(ONEWIRE_PIN, LOW);

  delay(10);

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