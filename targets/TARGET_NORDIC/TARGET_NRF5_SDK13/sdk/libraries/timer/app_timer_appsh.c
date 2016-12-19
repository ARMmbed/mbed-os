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

#include "app_timer_appsh.h"
#include "app_scheduler.h"

static void app_timer_evt_get(void * p_event_data, uint16_t event_size)
{
    app_timer_event_t * p_timer_event = (app_timer_event_t *)p_event_data;

    APP_ERROR_CHECK_BOOL(event_size == sizeof(app_timer_event_t));
    p_timer_event->timeout_handler(p_timer_event->p_context);
}

uint32_t app_timer_evt_schedule(app_timer_timeout_handler_t timeout_handler,
                                void *                      p_context)
{
    app_timer_event_t timer_event;

    timer_event.timeout_handler = timeout_handler;
    timer_event.p_context       = p_context;

    return app_sched_event_put(&timer_event, sizeof(timer_event), app_timer_evt_get);
}

