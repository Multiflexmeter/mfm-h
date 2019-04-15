// mfm00064_mV.ino
#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {8, 7, 6},
};
// lsb
static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// lsb
static const u1_t PROGMEM DEVEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// as provided
static const u1_t PROGMEM APPKEY[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
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
void os_getArtEui(u1_t *buf) { memcpy_P(buf, APPEUI, 8); }
void os_getDevEui(u1_t *buf) { memcpy_P(buf, DEVEUI, 8); }
void os_getDevKey(u1_t *buf) { memcpy_P(buf, APPKEY, 16); }

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
// This is the event listeners, When the LoRa module (RFM95) is done sending
// it will notify this function by: EV_TXCOMPLETE
// That's when we schedule another sendJob (sendFunc)
void onEvent(ev_t ev)
{
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev)
  {
  case EV_SCAN_TIMEOUT:
    Serial.println(F("EV_SCAN_TIMEOUT"));
    break;
  case EV_BEACON_FOUND:
    Serial.println(F("EV_BEACON_FOUND"));
    break;
  case EV_BEACON_MISSED:
    Serial.println(F("EV_BEACON_MISSED"));
    break;
  case EV_BEACON_TRACKED:
    Serial.println(F("EV_BEACON_TRACKED"));
    break;
  case EV_JOINING:
    Serial.println(F("EV_JOINING"));
    break;
  case EV_JOINED:
    Serial.println(F("EV_JOINED"));
    // Schedule sendjob
    os_setTimedCallback(&sendJob, os_getTime() + sec2osticks(1), sendFunc);
    // Disable link check validation (automatically enabled
    // during join, but not supported by TTN at this time).
    LMIC_setLinkCheckMode(0);
    break;
  case EV_RFU1:
    Serial.println(F("EV_RFU1"));
    break;
  case EV_JOIN_FAILED:
    Serial.println(F("EV_JOIN_FAILED"));
    break;
  case EV_REJOIN_FAILED:
    Serial.println(F("EV_REJOIN_FAILED"));
    break;
    break;
  case EV_TXCOMPLETE:
    Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
    if (LMIC.txrxFlags & TXRX_ACK)
      Serial.println(F("Received ack"));
    if (LMIC.dataLen)
    {
      Serial.println(F("Received "));
      Serial.println(LMIC.dataLen);
      Serial.println(F(" bytes of payload"));
    }
    // Schedule next transmission
    os_setTimedCallback(&sendJob, os_getTime() + sec2osticks(SEND_INTERVAL_SECONDS), sendFunc);
    break;
  case EV_LOST_TSYNC:
    Serial.println(F("EV_LOST_TSYNC"));
    break;
  case EV_RESET:
    Serial.println(F("EV_RESET"));
    break;
  case EV_RXCOMPLETE:
    // data received in ping slot
    Serial.println(F("EV_RXCOMPLETE"));
    break;
  case EV_LINK_DEAD:
    Serial.println(F("EV_LINK_DEAD"));
    break;
  case EV_LINK_ALIVE:
    Serial.println(F("EV_LINK_ALIVE"));
    break;
  default:
    Serial.println(F("Unknown event"));
    break;
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
