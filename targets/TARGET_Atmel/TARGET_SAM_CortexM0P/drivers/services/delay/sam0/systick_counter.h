#ifndef CYCLE_COUNTER_H_INCLUDED
#define CYCLE_COUNTER_H_INCLUDED

#include <compiler.h>
#include <clock.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name Convenience functions for busy-wait delay loops
 *
 * @{
 */

/**
 * \brief Delay loop to delay n number of cycles
 * Delay program execution for at least the specified number of CPU cycles.
 *
 * \param n  Number of cycles to delay
 */
static inline void delay_cycles(
    const uint32_t n)
{
    if (n > 0) {
        SysTick->LOAD = n;
        SysTick->VAL = 0;

        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
        };
    }
}

void delay_cycles_us(uint32_t n);

void delay_cycles_ms(uint32_t n);

/**
 * \brief Delay program execution for at least the specified number of microseconds.
 *
 * \param delay  number of microseconds to wait
 */
#define cpu_delay_us(delay)      delay_cycles_us(delay)

/**
 * \brief Delay program execution for at least the specified number of milliseconds.
 *
 * \param delay  number of milliseconds to wait
 */
#define cpu_delay_ms(delay)      delay_cycles_ms(delay)

/**
 * \brief Delay program execution for at least the specified number of seconds.
 *
 * \param delay  number of seconds to wait
 */
#define cpu_delay_s(delay)       delay_cycles_ms(1000 * delay)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* CYCLE_COUNTER_H_INCLUDED */
