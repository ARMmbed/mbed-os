/*
 * swtimer.h
 *
 * Created: 18.07.2017 16:37:44
 *  Author: Guenter.Prossliner
 */


#ifndef SWTIMER_H_
#define SWTIMER_H_


#include "us_ticker_api.h"

/** Represents the initial timer value. This is opaque data. */
typedef uint32_t swtimer_t;

/** Initializes the sandwitch-timer to the current µS ticker. */
inline void swtimer_init(swtimer_t *timer)
{
    *timer = us_ticker_read();
}

/** Returns true if the the timer initialized from 'timer' elapsed to the specified 'us' value in µS. */
inline bool swtimer_elapsed(swtimer_t timer, uint32_t us)
{
    // no need to validate the 'us' argument for overflow, because the us_timer has also 32 bit
    // with the current clock of 168MHzm, the max time to be waited is 2^32 / 168 = ~ 25560000 us ~ 25.5 seconds
    uint32_t time_taken = us_ticker_read() - timer;
    return time_taken >= us;
}

/** Busy waits for the sandwitch-timer to elapse */
inline void swtimer_wait_us(swtimer_t timer, uint32_t us)
{
    while (!swtimer_elapsed(timer, us));
}


#endif /* SWTIMER_H_ */