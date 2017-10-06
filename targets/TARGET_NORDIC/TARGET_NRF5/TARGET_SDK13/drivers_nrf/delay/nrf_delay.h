#ifndef _NRF_DELAY_H
#define _NRF_DELAY_H

#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCK_FREQ_16MHz (16000000UL)

/**
 * @brief Function for delaying execution for number of microseconds.
 *
 * @note NRF52 has instruction cache and because of that delay is not precise.
 *
 * @param number_of_us
 *
 */
/*lint --e{438, 522, 40, 10, 563} "Variable not used" "Function lacks side-effects" */
__STATIC_INLINE void nrf_delay_us(uint32_t number_of_us);


/**
 * @brief Function for delaying execution for number of miliseconds.
 *
 * @note NRF52 has instruction cache and because of that delay is not precise.
 *
 * @note Function internally calls @ref nrf_delay_us so the maximum delay is the
 * same as in case of @ref nrf_delay_us, approx. 71 minutes.
 *
 * @param number_of_ms
 *
 */

/*lint --e{438, 522, 40, 10, 563} "Variable not used" "Function lacks side-effects" */
__STATIC_INLINE void nrf_delay_ms(uint32_t number_of_ms);

#if defined ( __CC_ARM   )
__STATIC_INLINE void nrf_delay_us(uint32_t number_of_us)
{
    if(!number_of_us)
        return;
__asm
    {
loop:
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    CMP SystemCoreClock, CLOCK_FREQ_16MHz
    BEQ cond
    NOP
#if defined(NRF52) || defined(NRF52840_XXAA) || defined(NRF52832)
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
#endif
cond:
    SUBS number_of_us,number_of_us, #1
    BNE    loop
    }
}

#elif defined ( _WIN32 ) || defined ( __unix ) || defined( __APPLE__ )


#ifndef CUSTOM_NRF_DELAY_US
__STATIC_INLINE void nrf_delay_us(uint32_t number_of_us)
{}
#endif

#elif defined ( __GNUC__ ) || ( __ICCARM__ )

__STATIC_INLINE void nrf_delay_us(uint32_t number_of_us)
{
    const uint32_t clock16MHz = CLOCK_FREQ_16MHz;
    if (number_of_us)
    {
__ASM volatile (
#if ( defined(__GNUC__) && (__CORTEX_M == (0x00U) ) )
        ".syntax unified\n"
#endif
"1:\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " CMP %[SystemCoreClock],%[clock16MHz]\n"
       " BEQ.n 2f\n"
       " NOP\n"
#if defined(NRF52) || defined(NRF52840_XXAA) || defined(NRF52832)
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
       " NOP\n"
#endif
"2:\n"
       " SUBS %0, %0, #1\n"
       " BNE.n 1b\n"
#if __CORTEX_M == (0x00U)
#ifdef __GNUC__
    ".syntax divided\n"
#endif
    :"+l" (number_of_us) :
#else
    :"+r" (number_of_us) :
#endif
                [SystemCoreClock] "r" (SystemCoreClock),
                [clock16MHz] "r" (clock16MHz)
        );
#ifdef __ICCARM__
        __DMB();
#endif
    }
}
#endif

__STATIC_INLINE void nrf_delay_ms(uint32_t number_of_ms)
{
    nrf_delay_us(1000*number_of_ms);
}


#ifdef __cplusplus
}
#endif

#endif
