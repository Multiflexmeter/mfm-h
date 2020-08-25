#include <main.h>

struct Packet
{
};

unsigned short medianUS(unsigned short (*func)(), int count, int interval)
{
    MedianFilter median(count, 0);
    for (int i = 0; i < count; i++)
    {
        median.in(func());
        delay(interval);
    }
    return (unsigned short)median.out();
}

/**
 * Fired when data is to be collected.
 * 
 * Collected data must be copied to `LMIC.frame`
 */
uint8_t doMeasurements(uint8_t *data)
{
    // DS18B20 First message after power loss is always -127, thus read twice
    if (MFMLora::PowerOnReset)
    {
        // getTemperature();
    }

    // Setup packet
    Packet packet{};

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