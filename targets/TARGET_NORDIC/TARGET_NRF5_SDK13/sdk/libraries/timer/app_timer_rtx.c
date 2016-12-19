/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
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

#include "sdk_common.h"
#if NRF_MODULE_ENABLED(APP_TIMER)
#include "app_timer.h"
#include <stdlib.h>
#include "nrf.h"
#include "nrf_soc.h"
#include "app_error.h"
#include "cmsis_os.h"
#include "app_util_platform.h"

#define RTC1_IRQ_PRI        APP_IRQ_PRIORITY_LOWEST    /**< Priority of the RTC1 interrupt. */

#define MAX_RTC_COUNTER_VAL 0x00FFFFFF /**< Maximum value of the RTC counter. */

/**@brief This structure keeps information about osTimer.*/
typedef struct
{
    osTimerDef_t timerDef;
    uint32_t     buffer[6];
    osTimerId    id;
}app_timer_info_t;

/**@brief Store an array of timers with configuration. */
typedef struct
{
    uint8_t             max_timers;         /**< The maximum number of timers*/
    uint32_t            prescaler;
    app_timer_info_t *  app_timers;         /**< Pointer to table of timers*/
}app_timer_control_t;
app_timer_control_t     app_timer_control;

/**@brief This structure is defined by RTX. It keeps information about created osTimers. It is used in app_timer_start(). */
typedef struct os_timer_cb_ 
{                   
    struct os_timer_cb_ *   next;               /**< Pointer to next active Timer */
    uint8_t                 state;              /**< Timer State */
    uint8_t                 type;               /**< Timer Type (Periodic/One-shot). */
    uint16_t                reserved;           /**< Reserved. */
    uint32_t                tcnt;               /**< Timer Delay Count. */
    uint32_t                icnt;               /**< Timer Initial Count. */ 
    void *                  arg;                /**< Timer Function Argument. */
    const osTimerDef_t *    timer;              /**< Pointer to Timer definition. */
} os_timer_cb;

/**@brief This functions are defined by RTX.*/
//lint --save -e10 -e19 -e526
extern osStatus svcTimerStop(osTimerId timer_id);                        /**< Used in app_timer_stop(). */
extern osStatus svcTimerStart(osTimerId timer_id, uint32_t millisec);    /**< Used in app_timer_start(). */
// lint --restore
static void * rt_id2obj (void *id)          /**< Used in app_timer_start(). This function gives information if osTimerID is valid */
{
    if ((uint32_t)id & 3U)
    { 
        return NULL; 
    }

#ifdef OS_SECTIONS_LINK_INFO
  
    if ((os_section_id$$Base != 0U) && (os_section_id$$Limit != 0U)) 
    {
        if (id  < (void *)os_section_id$$Base)  
        { 
            return NULL; 
        }
        
        if (id >= (void *)os_section_id$$Limit) 
        { 
            return NULL; 
        }
    }
#endif

    return id;
}



uint32_t app_timer_init(uint32_t                      prescaler,
                        uint8_t                       op_queues_size,
                        void                        * p_buffer,
                        app_timer_evt_schedule_func_t evt_schedule_func)
{
    if (p_buffer == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    app_timer_control.prescaler  = prescaler;
    app_timer_control.app_timers = p_buffer;
    NVIC_SetPriority(RTC1_IRQn, RTC1_IRQ_PRI);

    return NRF_SUCCESS;
}


uint32_t app_timer_create(app_timer_id_t const      * p_timer_id,
                          app_timer_mode_t            mode,
                          app_timer_timeout_handler_t timeout_handler)
{

    if ((timeout_handler == NULL) || (p_timer_id == NULL))
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    app_timer_info_t * p_timer_info = (app_timer_info_t *)*p_timer_id;
    p_timer_info->timerDef.timer = p_timer_info->buffer;
    p_timer_info->timerDef.ptimer = (os_ptimer)timeout_handler;

    p_timer_info->id = osTimerCreate(&(p_timer_info->timerDef), (os_timer_type)mode, NULL);

    if (p_timer_info->id)
        return NRF_SUCCESS;
    else
    {
        return NRF_ERROR_INVALID_PARAM; // This error is unspecified by rtx
    }
}

#define osTimerRunning  2
uint32_t app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context)
{
    if ((timeout_ticks < APP_TIMER_MIN_TIMEOUT_TICKS))
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    uint32_t timeout_ms =
        ((uint32_t)ROUNDED_DIV(timeout_ticks * 1000 * (app_timer_control.prescaler + 1),
                               (uint32_t)APP_TIMER_CLOCK_FREQ));

    app_timer_info_t * p_timer_info = (app_timer_info_t *)timer_id;
    if (rt_id2obj((void *)p_timer_info->id) == NULL)
        return NRF_ERROR_INVALID_PARAM;

    // Pass p_context to timer_timeout_handler
    ((os_timer_cb *)(p_timer_info->id))->arg = p_context;

    if (((os_timer_cb *)(p_timer_info->id))->state == osTimerRunning)
    {
        return NRF_SUCCESS;
    }
    // osTimerStart() returns osErrorISR if it is called in interrupt routine.
    switch (osTimerStart((osTimerId)p_timer_info->id, timeout_ms) )
    {
        case osOK:
            return NRF_SUCCESS;

        case osErrorISR:
            break;

        case osErrorParameter:
            return NRF_ERROR_INVALID_PARAM;

        default:
            return NRF_ERROR_INVALID_PARAM;
    }

    // Start timer without svcCall
    switch (svcTimerStart((osTimerId)p_timer_info->id, timeout_ms))
    {
        case osOK:
            return NRF_SUCCESS;

        case osErrorISR:
            return NRF_ERROR_INVALID_STATE;

        case osErrorParameter:
            return NRF_ERROR_INVALID_PARAM;

        default:
            return NRF_ERROR_INVALID_PARAM;
    }
}

uint32_t app_timer_stop(app_timer_id_t timer_id)
{
    app_timer_info_t * p_timer_info = (app_timer_info_t *)timer_id;
    switch (osTimerStop((osTimerId)p_timer_info->id) )
    {
        case osOK:
            return NRF_SUCCESS;

        case osErrorISR:
            break;

        case osErrorParameter:
            return NRF_ERROR_INVALID_PARAM;

        case osErrorResource:
            return NRF_SUCCESS;

        default:
            return NRF_ERROR_INVALID_PARAM;
    }

    // Stop timer without svcCall
    switch (svcTimerStop((osTimerId)p_timer_info->id))
    {
        case osOK:
            return NRF_SUCCESS;

        case osErrorISR:
            return NRF_ERROR_INVALID_STATE;

        case osErrorParameter:
            return NRF_ERROR_INVALID_PARAM;

        case osErrorResource:
            return NRF_SUCCESS;

        default:
            return NRF_ERROR_INVALID_PARAM;
    }
}


uint32_t app_timer_stop_all(void)
{
    for (int i = 0; i < app_timer_control.max_timers; i++)
    {
        if (app_timer_control.app_timers[i].id)
        {
            (void)app_timer_stop((app_timer_id_t)app_timer_control.app_timers[i].id);
        }
    }
    return 0;
}


extern uint32_t os_tick_val(void);
uint32_t app_timer_cnt_get(void)
{
    return os_tick_val();
}


uint32_t app_timer_cnt_diff_compute(uint32_t   ticks_to,
                                    uint32_t   ticks_from,
                                    uint32_t * p_ticks_diff)
{
    *p_ticks_diff = ((ticks_to - ticks_from) & MAX_RTC_COUNTER_VAL);
    return NRF_SUCCESS;
}
#endif //NRF_MODULE_ENABLED(APP_TIMER)
