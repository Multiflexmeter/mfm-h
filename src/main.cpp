#include <Arduino.h>
#include <MFMLora.h>

// ======= JOBS ======
osjob_t sendJob;

/**
 * ==================
 * BEGIN EDIT THIS
 * ==================
 */
const int SEND_INTERVAL_SECONDS = 60;
// This value will be sent to Grafana
int getMeasurement()
{
  // int value = analogRead(A0);
  // int mv = (int)((float)value/1024*5000);
  // return mv;
  return 64;
}
/**
 * ==================
 *    	END
 * ==================
 */

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
  // Enable the timing 'framework'
  os_init();
  // Reset the LMIC struct state
  // (pending messages, the frame, settings etc.)
  LMIC_reset();
  LMIC_setClockError(MAX_CLOCK_ERROR * 2 / 100);
  // Disable link check validation
  //LMIC_setLinkCheckMode(0);
  // Spread factor for the receive window
  LMIC.dn2Dr = DR_SF9;
  // Set our datarate and transmit power
  LMIC_setDrTxpow(DR_SF7, 14);
  LMIC_startJoining();
}
void loop()
{
  os_runloop_once();
}
