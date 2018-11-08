/*
 * Copyright (c) 2014 Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA
 *      integrated circuit in a product or a software update for such product, must reproduce
 *      the above copyright notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be
 *      used to endorse or promote products derived from this software without specific prior
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse
 *      engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


/**@file
 *
 * @defgroup app_util_platform Utility Functions and Definitions (Platform)
 * @{
 * @ingroup app_common
 *
 * @brief Various types and definitions available to all applications when using SoftDevice.
 */

#ifndef APP_UTIL_PLATFORM_H__
#define APP_UTIL_PLATFORM_H__

#include <stdint.h>
#include "compiler_abstraction.h"
#include "nrf.h"
#ifdef SOFTDEVICE_PRESENT
#include "nrf_soc.h"
#include "nrf_nvic.h"
#endif
#include "nrf_assert.h"
#include "app_error.h"

#if defined(NRF51)
#define _PRIO_SD_HIGH       0
#define _PRIO_APP_HIGH      1
#define _PRIO_APP_MID       1
#define _PRIO_SD_LOW        2
#define _PRIO_APP_LOW       3
#define _PRIO_APP_LOWEST    3
#define _PRIO_THREAD        4
#elif defined(NRF52)
#define _PRIO_SD_HIGH       0
#define _PRIO_SD_MID        1
#define _PRIO_APP_HIGH      2
#define _PRIO_APP_MID       3
#define _PRIO_SD_LOW        4
#define _PRIO_SD_LOWEST     5
#define _PRIO_APP_LOW       6
#define _PRIO_APP_LOWEST    7
#define _PRIO_THREAD        15
#else
#error "No platform defined"
#endif

/**@brief The interrupt priorities available to the application while the SoftDevice is active. */
typedef enum {
#ifdef SOFTDEVICE_PRESENT
    APP_IRQ_PRIORITY_HIGHEST = _PRIO_SD_HIGH,
#else
    APP_IRQ_PRIORITY_HIGHEST = _PRIO_APP_HIGH,
#endif
    APP_IRQ_PRIORITY_HIGH    = _PRIO_APP_HIGH,
#ifndef SOFTDEVICE_PRESENT
    APP_IRQ_PRIORITY_MID     = _PRIO_SD_LOW,
#else
    APP_IRQ_PRIORITY_MID     = _PRIO_APP_MID,
#endif
    APP_IRQ_PRIORITY_LOW     = _PRIO_APP_LOW,
    APP_IRQ_PRIORITY_LOWEST  = _PRIO_APP_LOWEST,
    APP_IRQ_PRIORITY_THREAD  = _PRIO_THREAD     /**< "Interrupt level" when running in Thread Mode. */
} app_irq_priority_t;

/*@brief The privilege levels available to applications in Thread Mode */
typedef enum {
    APP_LEVEL_UNPRIVILEGED,
    APP_LEVEL_PRIVILEGED
} app_level_t;

/**@cond NO_DOXYGEN */
#define EXTERNAL_INT_VECTOR_OFFSET 16
/**@endcond */

void app_util_critical_region_enter(uint8_t *p_nested);
void app_util_critical_region_exit(uint8_t nested);

/**@brief Macro for entering a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
#ifdef SOFTDEVICE_PRESENT
#define CRITICAL_REGION_ENTER()                                                             \
    {                                                                                       \
        uint8_t __CR_NESTED = 0;                                                            \
        app_util_critical_region_enter(&__CR_NESTED);
#else
#define CRITICAL_REGION_ENTER() app_util_critical_region_enter(NULL)
#endif

/**@brief Macro for leaving a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
#ifdef SOFTDEVICE_PRESENT
#define CRITICAL_REGION_EXIT()                                                              \
        app_util_critical_region_exit(__CR_NESTED);                                         \
    }
#else
#define CRITICAL_REGION_EXIT() app_util_critical_region_exit(0)
#endif

/* Workaround for Keil 4 */
#ifndef IPSR_ISR_Msk
#define IPSR_ISR_Msk                       (0x1FFUL /*<< IPSR_ISR_Pos*/)                  /*!< IPSR: ISR Mask */
#endif



/**@brief Macro to enable anonymous unions from a certain point in the code.
 */
#if defined(__CC_ARM)
#define ANON_UNIONS_ENABLE _Pragma("push") \
                               _Pragma("anon_unions")
#elif defined(__ICCARM__)
#define ANON_UNIONS_ENABLE _Pragma("language=extended")
#else
#define ANON_UNIONS_ENABLE
// No action will be taken.
// For GCC anonymous unions are enabled by default.
#endif

/**@brief Macro to disable anonymous unions from a certain point in the code.
 * @note Call only after first calling @ref ANON_UNIONS_ENABLE.
 */
#if defined(__CC_ARM)
#define ANON_UNIONS_DISABLE _Pragma("pop")
#elif defined(__ICCARM__)
#define ANON_UNIONS_DISABLE
// for IAR leave anonymous unions enabled
#else
#define ANON_UNIONS_DISABLE
// No action will be taken.
// For GCC anonymous unions are enabled by default.
#endif


/* Workaround for Keil 4 */
#ifndef CONTROL_nPRIV_Msk
#define CONTROL_nPRIV_Msk                  (1UL /*<< CONTROL_nPRIV_Pos*/)                 /*!< CONTROL: nPRIV Mask */
#endif

/**@brief Function for finding the current interrupt level.
 *
 * @return   Current interrupt level.
 * @retval   APP_IRQ_PRIORITY_HIGH    We are running in Application High interrupt level.
 * @retval   APP_IRQ_PRIORITY_LOW     We are running in Application Low interrupt level.
 * @retval   APP_IRQ_PRIORITY_THREAD  We are running in Thread Mode.
 */
static __INLINE uint8_t current_int_priority_get(void)
{
    uint32_t isr_vector_num = __get_IPSR() & IPSR_ISR_Msk ;
    if (isr_vector_num > 0) {
        int32_t irq_type = ((int32_t)isr_vector_num - EXTERNAL_INT_VECTOR_OFFSET);
        return (NVIC_GetPriority((IRQn_Type)irq_type) & 0xFF);
    } else {
        return APP_IRQ_PRIORITY_THREAD;
    }
}

/**@brief Function for finding out the current privilege level.
 *
 * @return   Current privilege level.
 * @retval   APP_LEVEL_UNPRIVILEGED    We are running in unprivileged level.
 * @retval   APP_LEVEL_PRIVILEGED    We are running in privileged level.
 */
static __INLINE uint8_t privilege_level_get(void)
{
#if defined(NRF51)
    /* the Cortex-M0 has no concept of privilege */
    return APP_LEVEL_PRIVILEGED;
#elif defined(NRF52)
    uint32_t isr_vector_num = __get_IPSR() & IPSR_ISR_Msk ;
    if (0 == isr_vector_num) {
        /* Thread Mode, check nPRIV */
        int32_t control = __get_CONTROL();
        return control & CONTROL_nPRIV_Msk ? APP_LEVEL_UNPRIVILEGED : APP_LEVEL_PRIVILEGED;
    } else {
        /* Handler Mode, always privileged */
        return APP_LEVEL_PRIVILEGED;
    }
#endif
}

#endif // APP_UTIL_PLATFORM_H__

/** @} */
