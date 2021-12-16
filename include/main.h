#if !defined(_MAIN_H_)
#define _MAIN_H_

#include "config.h"
#include <arduino_lmic.h>
#include <hal/hal.h>

extern const lmic_pinmap lmic_pins;

struct tx_pkt_t
{
  uint16_t distance_to_water;
  float air_temperature;
};

#ifdef __cplusplus
extern "C"
{
#endif

  // These functions should not actually be exported
  // but having them defined here, save us from forward-
  // defining them in main.cpp :)

  void scheduleNextMeasurement();
  ostime_t getTransmissionTime(ostime_t req_time);

  osjob_t errorJob;
  void job_error(osjob_t *job);
  osjob_t pingJob;
  void job_pingVersion(osjob_t *job);
  osjob_t performJob;
  void job_performMeasurements(osjob_t *job);
  osjob_t fetchSendJob;
  void job_fetchAndSend(osjob_t *job);

#ifdef __cplusplus
}
#endif

/*
Possible LMIC events
  EV_SCAN_TIMEOUT
  EV_BEACON_FOUND
  EV_BEACON_MISSED
  EV_BEACON_TRACKED
  EV_JOINING
  EV_JOINED
  EV_JOIN_FAILED
  EV_REJOIN_FAILED
  EV_TXCOMPLETE
  EV_LOST_TSYNC
  EV_RESET
  EV_RXCOMPLETE
  EV_LINK_DEAD
  EV_LINK_ALIVE
  EV_TXSTART
  EV_TXCANCELED
  EV_RXSTART
  EV_JOIN_TXCOMPLETE
*/

#endif // _MAIN_H_