/*
 * Copyright (c) 2018-2019 Arm Limited
 *
 * SPDX-License-Identifier: Apache-2.0
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

/**
 * \file timer_gp_drv.h
 *
 * \brief Generic driver for general purpose timer.
 *
 * \details
 *        The free running counter is a 32 bit size counter that counts
 *        up to 0xFFFFFFFF.
 *        At this maximum value it wraps around to 0x00000000 and continues
 *        incrementing.
 *        Software can reset the counter to default 1 by calling
 *        \ref timer_gp_counter_reset.
 *        The counter implements two compare interrupts. When the counter
 *        reaches compare value "alarm0" or "alarm1" value it can trigger a
 *        corresponding interrupt.
 */

#ifndef __TIMER_GP_DRV_H__
#define __TIMER_GP_DRV_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIMER_GP_MAX_VALUE        UINT32_MAX /*!< max of 32-bit */
#define TIMER_GP_DEFAULT_RESET    1U
/*!< Counter's reset value will be set at HW level if reset bit is asserted */

/** GP timer device structure */
struct timer_gp_dev_cfg_t {
    const uintptr_t base;  /*!< Timer base address */
};

/** GP timer device data structure */
struct timer_gp_dev_data_t {
    bool is_initialized;
    uint32_t alarm0_init; /*!< Alarm0 value will be set by init */
    uint32_t alarm1_init; /*!< Alarm1 value will be set by init */
};

/** GP timer device structure */
struct timer_gp_dev_t {
    const struct timer_gp_dev_cfg_t* const cfg;   /*!< Timer configuration */
    struct timer_gp_dev_data_t* const data;       /*!< Timer data */
};

/**
 * \brief GP Timer alarm number identifiers
 *
 */
enum timer_gp_alarm_identifier_t{
    TIMER_GP_ALARM_0 = 0,
    TIMER_GP_ALARM_1 = 1
};

/**
 * \brief GP Timer read alarm number identifiers
 *
 */
enum timer_gp_read_alarm_identifier_t{
    TIMER_GP_READ_ALARM_0 = 0,
    TIMER_GP_READ_ALARM_1 = 1,
    TIMER_GP_READ_ALARM_COMBINED = 2
    /*!< Combined is asserted if Alarm1 OR Alarm2 is asserted */
};

/**
 * \brief Initializes timer to a known default state, which is:
 *          - interrupts disabled
 *          - alarm0 and alarm1 set to init value in \ref timer_gp_dev_data_t
 *          - timer reset to default reset value \ref TIMER_GP_DEFAULT_RESET
 *        Init should be called prior to any other process and
 *        it's the caller's responsibility to follow proper call order.
 *        More than one call results fall through.
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 */
void timer_gp_init(const struct timer_gp_dev_t* dev);

/**
 * \brief Resets the timer counter to 1.
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 */
void timer_gp_counter_reset(const struct timer_gp_dev_t* dev);

/**
 * \brief Read the 32bit free runnning counter's current value
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 *
 * \return 32bit counter current value
 */
uint32_t timer_gp_get_counter(const struct timer_gp_dev_t* dev);

/**
 * \brief Enable alarm interrupt of the given source
 *        Note: This function is not interrupt safe.
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 * \param[in] alarm Alarm source of the interrupt
 *            \ref timer_gp_alarm_identifier_t
 *
 */
void timer_gp_interrupt_enable(const struct timer_gp_dev_t* dev,
                               const enum timer_gp_alarm_identifier_t alarm);

/**
 * \brief Disable alarm interrupt of the given source
 *        Note: This function is not interrupt safe.
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 * \param[in] alarm Alarm source of the interrupt
 *            \ref timer_gp_alarm_identifier_t
 *
 */
void timer_gp_interrupt_disable(const struct timer_gp_dev_t* dev,
                                const enum timer_gp_alarm_identifier_t alarm);

/**
 * \brief Get alarm interrupt enabled status of the given source
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 * \param[in] alarm Alarm source of the interrupt
 *            \ref timer_gp_alarm_identifier_t
 *
 * \return true if enabled, false if not
 */
bool timer_gp_interrupt_is_enabled(const struct timer_gp_dev_t* dev,
                                   const enum
                                   timer_gp_alarm_identifier_t alarm);

/**
 * \brief Get alarm interrupt pending status of the given source
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 * \param[in] alarm Alarm source of the interrupt
 *                  \ref timer_gp_read_alarm_identifier_t
 *
 * \return true if active, false if not
 */
bool timer_gp_interrupt_is_active(const struct timer_gp_dev_t* dev,
        const enum timer_gp_read_alarm_identifier_t alarm);

/**
 * \brief Clear alarm interrupt of the given source
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 * \param[in] alarm Alarm source of the interrupt
 *            \ref timer_gp_alarm_identifier_t
 *
 */
void timer_gp_interrupt_clear(const struct timer_gp_dev_t* dev,
                              const enum timer_gp_alarm_identifier_t alarm);

/**
 * \brief Set alarm value of the given source
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 * \param[in] alarm Alarm source \ref timer_gp_alarm_identifier_t
 * \param[in] value When the counter reaches this tick value
 *                  corresponding interrupt status will be asserted.
 */
void timer_gp_set_alarm_value(const struct timer_gp_dev_t* dev,
                              const enum timer_gp_alarm_identifier_t alarm,
                              const uint32_t value);

/**
 * \brief Get alarm value of the given source
 *
 * \param[in] dev Timer device struct \ref timer_gp_dev_t
 * \param[in] alarm Alarm source \ref timer_gp_alarm_identifier_t
 *
 * \return value Counter value when the alarm is asserted.
 */
uint32_t timer_gp_get_alarm_value(const struct timer_gp_dev_t* dev,
        const enum timer_gp_alarm_identifier_t alarm);

#ifdef __cplusplus
}
#endif
#endif /* __TIMER_GP_DRV_H__ */
