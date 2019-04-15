#ifndef _MFMLORA_CONFIG_H_
#define _MFMLORA_CONFIG_H_

// Define your LoRaWAN keys here

// Define the pin mapping to your LoRa module here
#define _PIN_NSS 10
#define _PIN_RST 9
#define _PIN_DIO_0 8
#define _PIN_DIO_1 7
#define _PIN_DIO_2 6

// Define sleep length (subsequently send interval)
// Divide your wanted time by 8 and round it!
#define _SLEEP_ITERATIONS 2

// Uncomment if an external clock is used
// #define _EXTERNAL_CLOCK

#endif