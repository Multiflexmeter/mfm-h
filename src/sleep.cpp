#include "sleep.h"

#include <util/atomic.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Arduino.h>
#include <arduino_lmic.h>
#include <hal/hal.h>
#include "debug.h"

uint32_t next_job_time = 0;

uint8_t hal_checkTimer(uint32_t time)
{
  next_job_time = time;
  if ((int32_t)(time - hal_ticks()) <= ms2osticks(5))
  {
    _debugTime();
    _debug("running job for ");
    _debug((uint32_t)osticks2ms(time));
    _debug("\n");
    return 1;
  }
  return 0;
}

void hal_sleep()
{
  uint32_t now = hal_ticks();
  if (LMIC.opmode & (OP_TXRXPEND))
  {
    return;
  }

  int32_t duration_ticks = next_job_time - now;
  if (duration_ticks < ms2osticks(15))
  {
    return;
  }

  _debugTime();
  _debug(F("Next job at: "));
  _debug(next_job_time);
  _debug(F(" ticks; Now: "));
  _debug(now);
  _debug(F(" ticks; Duration: "));
  _debug(duration_ticks);
  _debug("ticks;\n");

  sleep(osticks2ms(duration_ticks));
}

/**
 * 
 */
void sleep(uint32_t duration)
{
  uint32_t start = millis();
#ifdef DEBUG
  Serial.flush();
#endif

  uint32_t elapsed = 0;
  // 15ms is minimum sleep time, so we do not want to
  // overshoot this
  while ((int32_t)(duration - elapsed) > 15)
  {
    _sleep(duration - elapsed);
    elapsed = (millis() - start);
  }

  _debugTime();
  _debug(F("Woke up: "));
  _debug(elapsed);
  _debug("\n");
}

/**
 * @brief Sleep as close to `period` amount of time
 * 
 * Given `period` is an indication, actual slept amount will be returned
 * 
 * Code borrowed from adafruit. Made values unsigned for consistency
 * https://github.com/adafruit/Adafruit_SleepyDog
 * 
 * @param period The requested time to sleep
 * @return uint32_t The time slept
 */
uint32_t _sleep(uint32_t period)
{
  uint32_t start_ms = millis();
  uint32_t wdto, actual_sleep_time;
  get_sleep_period(period, wdto, actual_sleep_time);

  // Build watchdog prescaler register value before timing critical code.
  uint8_t wdps = ((wdto & 0x08 ? 1 : 0) << WDP3) |
                 ((wdto & 0x04 ? 1 : 0) << WDP2) |
                 ((wdto & 0x02 ? 1 : 0) << WDP1) |
                 ((wdto & 0x01 ? 1 : 0) << WDP0);

  // The next section is timing critical so interrupts are disabled.
  cli();
  // First clear any previous watchdog reset.
  MCUSR &= ~(1 << WDRF);
  // Now change the watchdog prescaler and interrupt enable bit so the
  // watchdog reset only triggers the interrupt (and wakes from deep sleep)
  // and not a full device reset.  This is a timing critical section of
  // code that must happen in 4 cycles.
  WDTCSR |= (1 << WDCE) | (1 << WDE); // Set WDCE and WDE to enable changes.
  WDTCSR = wdps;                      // Set the prescaler bit values.
  WDTCSR |= (1 << WDIE);              // Enable only watchdog interrupts.
  // Critical section finished, re-enable interrupts.
  sei();

  // Set full power-down sleep mode and go to sleep.
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();

  // Chip is now asleep!

  // Once awakened by the watchdog execution resumes here.
  // Start by disabling sleep.
  sleep_disable();

  // Fix timing
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    extern volatile unsigned long timer0_overflow_count;
    extern volatile unsigned long timer0_millis;
    // timer0 overflows every 64 * 256 clock cycles
    // https://github.com/arduino/ArduinoCore-avr/blob/6ec80154cd2ca52bce443afbbed8a1ad44d049cb/cores/arduino/wiring.c#L27
    timer0_overflow_count = microsecondsToClockCycles((uint64_t)(start_ms + actual_sleep_time) * 1000) / (64 * 256);
    timer0_millis = start_ms + actual_sleep_time;
    // TCNT0 = 0; // Max deviation from millis will be 2.048 ms
  }

  return actual_sleep_time;
}

/**
 * @brief find the watchdog sleep time nearest to given `period`
 * 
 * @param period The period wanted to sleep
 * @param wdto the watchdog value for the actual sleep period
 * @param actual the actual sleep time in milliseconds
 */
void get_sleep_period(uint32_t period, uint32_t &wdto, uint32_t &actual)
{
  // Note the order of these if statements from highest to lowest  is
  // important so that control flow cascades down to the right value based
  // on its position in the range of discrete timeouts.
  if ((period >= 8000) || (period == 0))
  {
    wdto = WDTO_8S;
    actual = 8000;
  }
  else if (period >= 4000)
  {
    wdto = WDTO_4S;
    actual = 4000;
  }
  else if (period >= 2000)
  {
    wdto = WDTO_2S;
    actual = 2000;
  }
  else if (period >= 1000)
  {
    wdto = WDTO_1S;
    actual = 1000;
  }
  else if (period >= 500)
  {
    wdto = WDTO_500MS;
    actual = 500;
  }
  else if (period >= 250)
  {
    wdto = WDTO_250MS;
    actual = 250;
  }
  else if (period >= 120)
  {
    wdto = WDTO_120MS;
    actual = 120;
  }
  else if (period >= 60)
  {
    wdto = WDTO_60MS;
    actual = 60;
  }
  else if (period >= 30)
  {
    wdto = WDTO_30MS;
    actual = 30;
  }
  else
  {
    wdto = WDTO_15MS;
    actual = 15;
  }
}

// Define watchdog timer interrupt.
ISR(WDT_vect)
{
  // Nothing needs to be done, however interrupt handler must be defined to
  // prevent a reset.
}