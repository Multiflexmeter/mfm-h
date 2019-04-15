#include <Arduino.h>
#include <MFMLora.h>

// ======= JOBS ======
osjob_t sendJob;

const int SEND_INTERVAL_SECONDS = 60;

int getMeasurement()
{
  return 64;
}

void sendFunc(osjob_t *j)
{
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
    Serial.println(F("OP_TXRXPEND, not sending"));
    os_setTimedCallback(&sendJob, os_getTime() + sec2osticks(5), sendFunc);
  }
  else
  {
    // Next TX is scheduled after TX_COMPLETE event.
    int measurement = getMeasurement();
    // Copy the measurement 1-on-1 to the Frame
    memcpy(&LMIC.frame, &measurement, sizeof(measurement));
    // Schedule packet sending
    LMIC_setTxData2(1, LMIC.frame, sizeof(measurement), false);
    Serial.println("Packet queued");
  }
}

void setup()
{
  Serial.begin(9600);
  MFMLora::setup();
}

void loop()
{
  os_runloop_once();
}
