#ifndef _MFMLORA_CONFIG_H_
#define _MFMLORA_CONFIG_H_

// Get these keys from your LoRaWAN Network provider

#ifndef DEV_EUI
#define DEV_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#endif

#ifdef APP_EUI
#define APP_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#endif

#ifdef APP_KEY
#define APP_KEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#endif

// 1 Sleep iteration is 8 seconds.
#ifdef SLEEP_ITERATIONS
#define SLEEP_ITERATIONS 2
#endif

/*
  Below are the more advanced settings.
  Don't just change them without knowing what you are doing!
*/

#ifdef PIN_NSS
#define PIN_NSS 10
#endif

#ifdef PIN_RST
#define PIN_RST 9
#endif

#ifdef PIN_DIO_0
#define PIN_DIO_0 8
#endif

#ifdef PIN_DIO_1
#define PIN_DIO_1 7
#endif

#ifdef PIN_DIO_2
#define PIN_DIO_2 6
#endif

// Uncomment if using an external clock
//#define EXTERNAL_CLOCK

// Defined dynamically by PIO environment
#ifdef BUILD_TIME
#define BUILD_TIME "MISSING"
#endif

#endif