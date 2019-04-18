#ifndef _MFMLORA_CONFIG_H_
#define _MFMLORA_CONFIG_H_

// Get these keys from your LoRaWAN Network provider

#ifndef DEV_EUI
#define DEV_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#endif

#ifndef APP_EUI
#define APP_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#endif

#ifndef APP_KEY
#define APP_KEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#endif

// 1 Sleep iteration is 8 seconds.
#ifndef SLEEP_ITERATIONS
#define SLEEP_ITERATIONS 2
#endif

// Comment to disable printing the build date-time
#define PRINT_BUILD_DATE_TIME

// Comment if using external clock
#define INTERNAL_CLOCK

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

#endif