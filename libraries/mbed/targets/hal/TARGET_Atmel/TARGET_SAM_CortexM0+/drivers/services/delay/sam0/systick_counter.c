#include "delay.h"

/**
 * Value used to calculate ms delay. Default to be used with a 8MHz clock;
 */
static uint32_t cycles_per_ms = 8000000UL / 1000;
static uint32_t cycles_per_us = 8000000UL / 1000000;

/**
 * \brief Initialize the delay driver.
 *
 * This must be called during start up to initialize the delay routine with
 * the current used main clock. It must run any time the main CPU clock is changed.
 */
void delay_init(void)
{
    cycles_per_ms = system_gclk_gen_get_hz(0);
    cycles_per_ms /= 1000;
    cycles_per_us = cycles_per_ms / 1000;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

/**
 * \brief Delay loop to delay at least n number of microseconds
 *
 * \param n  Number of microseconds to wait
 */
void delay_cycles_us(
    uint32_t n)
{
    while (n--) {
        /* Devide up to blocks of 10u */
        delay_cycles(cycles_per_us);
    }
}

/**
 * \brief Delay loop to delay at least n number of milliseconds
 *
 * \param n  Number of milliseconds to wait
 */
void delay_cycles_ms(
    uint32_t n)
{
    while (n--) {
        /* Devide up to blocks of 1ms */
        delay_cycles(cycles_per_ms);
    }
}
