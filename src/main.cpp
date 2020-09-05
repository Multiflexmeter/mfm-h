#include <main.h>

Adafruit_BMP280 bmp; // I2C

struct Packet
{
    uint16_t a;
    float b;
    float c;
};

/**
 * Fired when data is to be collected.
 * 
 * Collected data must be copied to `LMIC.frame`
 */
uint8_t doMeasurements(uint8_t *data)
{

    // Setup packet
    Packet packet{
        doPressureMeasurement(),
        bmp.readTemperature(),
        bmp.readPressure(),
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
    // Wait for the JSN to come online
    delay(WARMUP_TIME_MS);
}

/**
 * Fired before sleeping.
 * 
 * You should powerdown instruments here.
 */
void powerDown()
{
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(ONEWIRE_PIN, LOW);
}

/**
 * Fired after ATMega reset
 */
void setup()
{
    // doBuzzerBeep();
    Serial.begin(9600);
    // Set pinmode
    pinMode(RELAY_PIN, OUTPUT);
    // Enable instruments
    digitalWrite(RELAY_PIN, HIGH);

    delay(10);

    // Initialize BMP280
    if (!bmp.begin())
    {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
        while (1)
            ;
    }

    /* Default settings from datasheet. */
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

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