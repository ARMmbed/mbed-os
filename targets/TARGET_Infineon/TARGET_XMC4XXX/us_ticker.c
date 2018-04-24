/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/***************************************************************** Includes **/
#include "us_ticker_api.h"
#include "xmc_scu.h"
#include "xmc_ccu4.h"

/****************************************************************** Defines **/
#define TIMER_SLICE_0_PTR           CCU43_CC40
#define TIMER_SLICE_1_PTR           CCU43_CC41
#define TIMER_SLICE_2_PTR           CCU43_CC42
#define TIMER_MODULE_PTR            CCU43
#define SLICE_0_NUMBER              (0U)
#define SLICE_1_NUMBER              (1U)
#define SLICE_2_NUMBER              (2U)
#define TIMER_IRQn                  CCU43_0_IRQn
#define TIMER_IRQ_HANDLER           CCU43_0_IRQHandler
#define CCU_TRIGGER                 XMC_SCU_CCU_TRIGGER_CCU43

/****************************************************************** Globals **/
const ticker_info_t tick_info = {
        .frequency = 1000000,
        .bits = 32
};
static int us_ticker_inited = 0;

/******************************************************* Internal Functions **/

/**
 * @internal Initialize Timer for us_ticker
 *
 * Three 16 bit timers are concatenated to a 64 bit timer.
 * Timer 0 is responsable to generate 1us.
 * Timer 1 bits 0-15 of 32 bit timer
 * Timer 2 bit 16-31 of 32 bit timer
 */
static void timer_init(void) {

    XMC_CCU4_SLICE_COMPARE_CONFIG_t slice_config;

    XMC_CCU4_SLICE_EVENT_CONFIG_t event_config =
    {
            .mapped_input = CCU43_IN0_SCU_GSC43,
            .edge = XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_RISING_EDGE
    };

    /* Init CCU4x module */
    XMC_CCU4_Init(TIMER_MODULE_PTR, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);

    XMC_STRUCT_INIT(slice_config);

    slice_config.monoshot = XMC_CCU4_SLICE_TIMER_REPEAT_MODE_REPEAT;
    XMC_CCU4_SLICE_CompareInit(TIMER_SLICE_0_PTR, &slice_config);

    slice_config.timer_concatenation = true;
    XMC_CCU4_SLICE_CompareInit(TIMER_SLICE_1_PTR, &slice_config);

    slice_config.timer_concatenation = true;
    XMC_CCU4_SLICE_CompareInit(TIMER_SLICE_2_PTR, &slice_config);

    XMC_CCU4_SLICE_SetTimerPeriodMatch(TIMER_SLICE_0_PTR, 0x0078U);
    XMC_CCU4_SLICE_SetTimerPeriodMatch(TIMER_SLICE_1_PTR, 0xffffU);
    XMC_CCU4_SLICE_SetTimerPeriodMatch(TIMER_SLICE_2_PTR, 0xffffU);

    /* Configure event to start synchronously the three slices */
    XMC_CCU4_SLICE_ConfigureEvent(TIMER_SLICE_0_PTR, XMC_CCU4_SLICE_EVENT_0, &event_config);
    XMC_CCU4_SLICE_ConfigureEvent(TIMER_SLICE_1_PTR, XMC_CCU4_SLICE_EVENT_0, &event_config);
    XMC_CCU4_SLICE_ConfigureEvent(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_EVENT_0, &event_config);

    XMC_CCU4_SLICE_StartConfig(TIMER_SLICE_0_PTR, XMC_CCU4_SLICE_EVENT_0, XMC_CCU4_SLICE_START_MODE_TIMER_START);
    XMC_CCU4_SLICE_StartConfig(TIMER_SLICE_1_PTR, XMC_CCU4_SLICE_EVENT_0, XMC_CCU4_SLICE_START_MODE_TIMER_START);
    XMC_CCU4_SLICE_StartConfig(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_EVENT_0, XMC_CCU4_SLICE_START_MODE_TIMER_START);

    /* Configure event to stop synchronously the three slices */
    event_config.edge = XMC_CCU4_SLICE_EVENT_EDGE_SENSITIVITY_FALLING_EDGE;
    XMC_CCU4_SLICE_ConfigureEvent(TIMER_SLICE_0_PTR, XMC_CCU4_SLICE_EVENT_1, &event_config);
    XMC_CCU4_SLICE_ConfigureEvent(TIMER_SLICE_1_PTR, XMC_CCU4_SLICE_EVENT_1, &event_config);
    XMC_CCU4_SLICE_ConfigureEvent(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_EVENT_1, &event_config);

    XMC_CCU4_SLICE_StopConfig(TIMER_SLICE_0_PTR, XMC_CCU4_SLICE_EVENT_1, XMC_CCU4_SLICE_END_MODE_TIMER_STOP);
    XMC_CCU4_SLICE_StopConfig(TIMER_SLICE_1_PTR, XMC_CCU4_SLICE_EVENT_1, XMC_CCU4_SLICE_END_MODE_TIMER_STOP);
    XMC_CCU4_SLICE_StopConfig(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_EVENT_1, XMC_CCU4_SLICE_END_MODE_TIMER_STOP);

    /* Assign interrupt node and set priority */
    XMC_CCU4_SLICE_SetInterruptNode(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP, XMC_CCU4_SLICE_SR_ID_0);

    /* Enable clock for all slices */
    XMC_CCU4_EnableClock(TIMER_MODULE_PTR, (uint8_t)SLICE_0_NUMBER);
    XMC_CCU4_EnableClock(TIMER_MODULE_PTR, (uint8_t)SLICE_1_NUMBER);
    XMC_CCU4_EnableClock(TIMER_MODULE_PTR, (uint8_t)SLICE_2_NUMBER);

    XMC_CCU4_EnableShadowTransfer(TIMER_MODULE_PTR, XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_SLICE_1 | XMC_CCU4_SHADOW_TRANSFER_SLICE_2);

    /* Synchronous start of CCU4x timer slices */
    XMC_SCU_SetCcuTriggerHigh(CCU_TRIGGER);
}

/**
 * @internal Set compare value of Timer
 *
 */
static void timer_set_compare_value(uint32_t ticks) {

    /* Set new compare value */
    XMC_CCU4_SLICE_SetTimerCompareMatch(TIMER_SLICE_1_PTR, ticks & 0xffffU);
    XMC_CCU4_SLICE_SetTimerCompareMatch(TIMER_SLICE_2_PTR, ticks >> 16U);

    /* Stop Timer */
    XMC_SCU_SetCcuTriggerLow(CCU_TRIGGER);

    /* Enable Shadow Transfer */
    XMC_CCU4_EnableShadowTransfer(TIMER_MODULE_PTR, XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_SLICE_1 | XMC_CCU4_SHADOW_TRANSFER_SLICE_2);

    /* Start Timer */
    XMC_SCU_SetCcuTriggerHigh(CCU_TRIGGER);
}

/**
 * @internal Get Timer value
 *
 * @return actual Timer value
 */
static uint32_t timer_get_time(void) {

    uint32_t time_h = XMC_CCU4_SLICE_GetTimerValue(TIMER_SLICE_2_PTR);
    uint32_t time_l = XMC_CCU4_SLICE_GetTimerValue(TIMER_SLICE_1_PTR);

    while (XMC_CCU4_SLICE_GetTimerValue(TIMER_SLICE_2_PTR) != time_h) {
        time_h = XMC_CCU4_SLICE_GetTimerValue(TIMER_SLICE_2_PTR);
        time_l = XMC_CCU4_SLICE_GetTimerValue(TIMER_SLICE_1_PTR);
    }

    return ((time_h << 16) | time_l);
}

/************************************************************** ISR Handler **/

/**
 * @brief Timer ISR Handler
 *
 */
void TIMER_IRQ_HANDLER(void) {

    us_ticker_irq_handler();
}

/**************************************************************** Functions **/

/**
 * @brief Initialize the ticker
 *
 */
void us_ticker_init(void) {

    if(us_ticker_inited) {
        return;
    }
    us_ticker_inited = 1;

    /* Initialize Hardware Timer(s) */
    timer_init();
}

/**
 * @brief Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t us_ticker_read() {

    if (!us_ticker_inited)
        us_ticker_init();

    return timer_get_time();
}

/**
 * @brief Set interrupt for specified timestamp
 *
 * @param Set interrupt for specified timestamp
 */
void us_ticker_set_interrupt(timestamp_t timestamp) {


    NVIC_DisableIRQ(TIMER_IRQn);

    /* Set Compare Value */
    timer_set_compare_value((uint32_t)timestamp);
    XMC_CCU4_SLICE_EnableEvent(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP);

    NVIC_EnableIRQ(TIMER_IRQn);
}

/**
 * @brief Disable us ticker interrupt
 *
 */
void us_ticker_disable_interrupt(void) {

    /* Disable Event */
    XMC_CCU4_SLICE_DisableEvent(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP);
    NVIC_DisableIRQ(TIMER_IRQn);
}

/**
 * @brief Clear us ticker interrupt
 *
 */
void us_ticker_clear_interrupt(void) {

    /* Clear Event */
    XMC_CCU4_SLICE_ClearEvent(TIMER_SLICE_2_PTR, XMC_CCU4_SLICE_IRQ_ID_COMPARE_MATCH_UP);
    NVIC_ClearPendingIRQ(TIMER_IRQn);
}

/** Set pending interrupt that should be fired right away.
 *
 * The ticker should be initialized prior calling this function.
 */
void us_ticker_fire_interrupt(void){

    if(!us_ticker_inited) {
        return;
    }
    NVIC_SetPendingIRQ(TIMER_IRQn);
}

/** Get frequency and counter bits of this ticker.
 *
 */
const ticker_info_t* us_ticker_get_info(void){

    return &tick_info;
}

/*EOF*/
