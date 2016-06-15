/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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

#include "softdevice_handler_appsh.h"
#include "app_scheduler.h"
#include <string.h>

void softdevice_evt_get(void * p_event_data, uint16_t event_size)
{
    APP_ERROR_CHECK_BOOL(event_size == 0);
    intern_softdevice_events_execute();
}

uint32_t softdevice_evt_schedule(void)
{
    return app_sched_event_put(NULL, 0, softdevice_evt_get);
}
