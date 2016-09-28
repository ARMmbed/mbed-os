/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
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
#include "app_error.h"
#endif
/**@brief The interrupt priorities available to the application while the SoftDevice is active. */
typedef enum
{
#ifndef SOFTDEVICE_PRESENT
    APP_IRQ_PRIORITY_HIGHEST = 0,
#endif
    APP_IRQ_PRIORITY_HIGH    = 1,
#ifndef SOFTDEVICE_PRESENT
    APP_IRQ_PRIORITY_MID     = 2,
#endif
    APP_IRQ_PRIORITY_LOW     = 3
} app_irq_priority_t;

#define NRF_APP_PRIORITY_THREAD    4                    /**< "Interrupt level" when running in Thread Mode. */

/**@cond NO_DOXYGEN */
#define EXTERNAL_INT_VECTOR_OFFSET 16
/**@endcond */

void critical_region_enter (void);
void critical_region_exit (void);

/**@brief Macro for entering a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
#ifdef SOFTDEVICE_PRESENT
#define CRITICAL_REGION_ENTER()                                                             \
    {                                                                                       \
        uint8_t IS_NESTED_CRITICAL_REGION = 0;                                              \
        uint32_t CURRENT_INT_PRI = current_int_priority_get();                              \
        if (CURRENT_INT_PRI != APP_IRQ_PRIORITY_HIGH)                                       \
        {                                                                                   \
            uint32_t ERR_CODE = sd_nvic_critical_region_enter(&IS_NESTED_CRITICAL_REGION);  \
            if (ERR_CODE == NRF_ERROR_SOFTDEVICE_NOT_ENABLED)                               \
            {                                                                               \
                __disable_irq();                                                            \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                APP_ERROR_CHECK(ERR_CODE);                                                  \
            }                                                                               \
        }        
#else
#define CRITICAL_REGION_ENTER() critical_region_enter()
#endif

/**@brief Macro for leaving a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
#ifdef SOFTDEVICE_PRESENT
#define CRITICAL_REGION_EXIT()                                                              \
        if (CURRENT_INT_PRI != APP_IRQ_PRIORITY_HIGH)                                       \
        {                                                                                   \
            uint32_t ERR_CODE;                                                              \
            __enable_irq();                                                                 \
            ERR_CODE = sd_nvic_critical_region_exit(IS_NESTED_CRITICAL_REGION);             \
            if (ERR_CODE != NRF_ERROR_SOFTDEVICE_NOT_ENABLED)                               \
            {                                                                               \
                APP_ERROR_CHECK(ERR_CODE);                                                  \
            }                                                                               \
        }                                                                                   \
    }
#else
#define CRITICAL_REGION_EXIT() critical_region_exit()
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
    uint32_t isr_vector_num = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
    if (isr_vector_num > 0)
    {
        int32_t irq_type = ((int32_t)isr_vector_num - EXTERNAL_INT_VECTOR_OFFSET);
        return (NVIC_GetPriority((IRQn_Type)irq_type) & 0xFF);
    }
    else
    {
        return NRF_APP_PRIORITY_THREAD;
    }
}

#endif // APP_UTIL_PLATFORM_H__

/** @} */
