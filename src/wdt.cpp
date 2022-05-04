#include <avr/wdt.h>
#include <avr/interrupt.h>

uint8_t do_reset = 0;

ISR(WDT_vect)
{
  // Do not respond to the WDT if we're supposed to reset the MCU
  if (do_reset)
    return;
  // Otherwise reset the watchdog and keep the MCU alive
  wdt_reset();
  wdt_disable();
}