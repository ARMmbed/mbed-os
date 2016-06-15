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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>


#ifdef ENABLE_DEBUG_LOG_SUPPORT
#include "app_trace.h"
#include "nrf_log.h"

void app_trace_init(void)
{
    (void)NRF_LOG_INIT();
}

void app_trace_dump(uint8_t * p_buffer, uint32_t len)
{
    app_trace_log("\r\n");
    for (uint32_t index = 0; index <  len; index++)
    {
        app_trace_log("0x%02X ", p_buffer[index]);
    }
    app_trace_log("\r\n");
}

#endif // ENABLE_DEBUG_LOG_SUPPORT

/**
 *@}
 **/

