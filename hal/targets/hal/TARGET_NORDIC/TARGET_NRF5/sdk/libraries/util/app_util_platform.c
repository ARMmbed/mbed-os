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

#include "app_util_platform.h"

static uint32_t m_in_critical_region = 0;

void app_util_disable_irq(void)
{
    __disable_irq();    
    m_in_critical_region++;    
}

void app_util_enable_irq(void)
{
    m_in_critical_region--;    
    if (m_in_critical_region == 0)
    {
        __enable_irq();
    }
}

void app_util_critical_region_enter(uint8_t *p_nested)
{
#ifdef NRF52
    ASSERT(APP_LEVEL_PRIVILEGED == privilege_level_get())
#endif

#if defined(SOFTDEVICE_PRESENT)
    /* return value can be safely ignored */
    (void) sd_nvic_critical_region_enter(p_nested);
#else
    app_util_disable_irq();
#endif
}

void app_util_critical_region_exit(uint8_t nested)
{
#ifdef NRF52
    ASSERT(APP_LEVEL_PRIVILEGED == privilege_level_get())
#endif

#if defined(SOFTDEVICE_PRESENT)
    /* return value can be safely ignored */
    (void) sd_nvic_critical_region_exit(nested);
#else
    app_util_enable_irq();
#endif
}


