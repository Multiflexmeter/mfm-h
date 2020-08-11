#include "lmic.h"

#ifndef _MFMLORA_CONFIG_H_
#define _MFMLORA_CONFIG_H_

// Get these keys from your LoRaWAN Network provider

#ifndef DEV_EUI
#define DEV_EUI                                    \
  {                                                \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
  }
#endif

#ifndef APP_EUI
#define APP_EUI                                    \
  {                                                \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
  }
#endif

#ifndef APP_KEY
#define APP_KEY                                                                                    \
  {                                                                                                \
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
  }
#endif

#ifndef MFMLORA_DEBUG
#define MFMLORA_DEBUG 0
#endif

// 1 Sleep iteration is 8 seconds.
#ifndef SLEEP_ITERATIONS
#define SLEEP_ITERATIONS (112 - 6) // Adjust for incorrect timing
#endif

// How long to sleep before rejoining if a JOIN fails
#ifndef REJOIN_SLEEP_ITERATIONS
#define REJOIN_SLEEP_ITERATIONS (2700 - 145) // Adjust for incorrect timing
#endif

// Comment to disable printing the build date-time
#define PRINT_BUILD_DATE_TIME

// Comment if using external clock
#define INTERNAL_CLOCK

// This will trigger a rejoin if an ACK request is not answered
// for x amount of uplinks
#define LMIC_LINKCHECK 0

// #define USE_KPN
#define USE_TTN

/*
  Below are the more advanced settings.
  Don't just change them without knowing what you are doing!
*/

#ifndef PIN_NSS
#define PIN_NSS 10
#endif

#ifndef PIN_RST
#define PIN_RST 9
#endif

#ifndef PIN_DIO_0
#define PIN_DIO_0 8
#endif

#ifndef PIN_DIO_1
#define PIN_DIO_1 7
#endif

#ifndef PIN_DIO_2
#define PIN_DIO_2 6
#endif

/*
  Below are derived settings, you should not have to change them.
*/

#if defined(USE_KPN) && defined(USE_TTN)
#error "Either USE_KPN or USE_TTN has to be defined, not both!"
#endif

#ifdef USE_TTN
#define DN2DR DR_SF9
#define DN2Freq EU868_F6
#endif
#ifdef USE_KPN
#define DN2DR DR_SF12
#define DN2Freq EU868_F6
#endif

#endif