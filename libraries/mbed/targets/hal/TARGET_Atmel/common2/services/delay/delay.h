#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup group_common_services_delay Busy-Wait Delay Routines
 *
 * This module provides simple loop-based delay routines for those
 * applications requiring a brief wait during execution. Common for
 * API ver. 2.
 *
 * @{
 */
#include <compiler.h>
#include <gclk.h>

// TEMP: Added by V
#include "sam0/systick_counter.h"
#ifdef SYSTICK_MODE
#include "sam0/systick_counter.h"
#endif
#ifdef CYCLE_MODE
#include "sam0/cycle_counter.h"
#endif

void delay_init(void);

/**
 * \def delay_s
 * \brief Delay in at least specified number of seconds.
 * \param delay Delay in seconds
 */
#define delay_s(delay)          cpu_delay_s(delay)

/**
 * \def delay_ms
 * \brief Delay in at least specified number of milliseconds.
 * \param delay Delay in milliseconds
 */
#define delay_ms(delay)         cpu_delay_ms(delay)

/**
 * \def delay_us
 * \brief Delay in at least specified number of microseconds.
 * \param delay Delay in microseconds
 */
#define delay_us(delay)         cpu_delay_us(delay)

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* DELAY_H_INCLUDED */
