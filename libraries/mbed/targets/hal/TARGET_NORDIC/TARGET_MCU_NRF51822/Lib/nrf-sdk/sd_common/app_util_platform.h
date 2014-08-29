/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
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
#include "nrf51.h"
#include "app_error.h"

/**@brief The interrupt priorities available to the application while the SoftDevice is active. */
typedef enum
{
    APP_IRQ_PRIORITY_HIGH = 1,
    APP_IRQ_PRIORITY_LOW  = 3
} app_irq_priority_t;

#define NRF_APP_PRIORITY_THREAD    4                    /**< "Interrupt level" when running in Thread Mode. */

/**@cond NO_DOXYGEN */
#define EXTERNAL_INT_VECTOR_OFFSET 16
/**@endcond */

#define PACKED(TYPE) __packed TYPE

/**@brief Macro for entering a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
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

/**@brief Macro for leaving a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       CRITICAL_REGION_EXIT() for each call to CRITICAL_REGION_ENTER(), and they must be located
 *       in the same scope.
 */
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
