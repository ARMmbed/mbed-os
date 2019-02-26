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
 * \file timer_gp_drv.c
 *
 * \brief Generic driver for general purpose timer.
 */

#include "timer_gp_drv.h"

/** Setter bit manipulation macro */
#define SET_BIT(WORD, BIT_INDEX) ((WORD) |= (1U << (BIT_INDEX)))
/** Clearing bit manipulation macro */
#define CLR_BIT(WORD, BIT_INDEX) ((WORD) &= ~(1U << (BIT_INDEX)))
/** Getter bit manipulation macro */
#define GET_BIT(WORD, BIT_INDEX) (bool)(((WORD) & (1U << (BIT_INDEX))))

/**
 * \brief Timer register map structure
 *
 */
struct timer_gp_reg_map_t {
    volatile uint32_t reset;      /*!< Offset: 0x000 (R/W) Control Reset */
    volatile uint32_t irq_mask;   /*!< Offset: 0x004 (R/W) Masked Interrupt */
    volatile uint32_t irq_clear;  /*!< Offset: 0x008 (R/W) Interrupt Clear */
    volatile uint32_t reserved;   /*!< Offset: 0x00C Reserved */
    volatile uint32_t alarm0;     /*!< Offset: 0x010 (R/W) Alarm0 data value */
    volatile uint32_t alarm1;     /*!< Offset: 0x014 (R/W) Alarm1 data value */
    volatile uint32_t irq_status; /*!< Offset: 0x018 (R) Raw Interrupt status */
    volatile uint32_t counter;    /*!< Offset: 0x01C (R) Counter data value */
};

/**
 * \brief Reset control register bit identifiers
 *
 */
enum reset_reg_bits_t{
    RESET_REG_INDEX = 0
    /* 1-31: Reserved. Read as zero. Do No Modify (DNM). */
};

void timer_gp_init(const struct timer_gp_dev_t* dev)
{
    if (dev->data->is_initialized == false) {
        timer_gp_interrupt_disable(dev, TIMER_GP_ALARM_0);
        timer_gp_interrupt_clear(dev, TIMER_GP_ALARM_0);
        timer_gp_interrupt_disable(dev, TIMER_GP_ALARM_1);
        timer_gp_interrupt_clear(dev, TIMER_GP_ALARM_1);
        timer_gp_set_alarm_value(dev, TIMER_GP_ALARM_0, dev->data->alarm0_init);
        timer_gp_set_alarm_value(dev, TIMER_GP_ALARM_1, dev->data->alarm1_init);
        timer_gp_counter_reset(dev);
        dev->data->is_initialized = true;
    }
}

void timer_gp_counter_reset(const struct timer_gp_dev_t* dev)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;

    SET_BIT(register_map->reset, RESET_REG_INDEX);
    /* Reset bit is not self-clearing and some pulse width is required
     * for successful reset, so we have to check whether the
     * timer counter is set to reset value. Until this bit is asserted
     * the timer won't be started.
     * The timer is running only if the reset bit is cleared.*/
    while (timer_gp_get_counter(dev) != TIMER_GP_DEFAULT_RESET)
        ;
    CLR_BIT(register_map->reset, RESET_REG_INDEX);
}

uint32_t timer_gp_get_counter(const struct timer_gp_dev_t* dev)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    return register_map->counter;
}

void timer_gp_interrupt_enable(const struct timer_gp_dev_t* dev,
                               const enum timer_gp_alarm_identifier_t alarm)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    SET_BIT(register_map->irq_mask, alarm);
}

void timer_gp_interrupt_disable(const struct timer_gp_dev_t* dev,
                                const enum timer_gp_alarm_identifier_t alarm)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    CLR_BIT(register_map->irq_mask, alarm);
}

bool timer_gp_interrupt_is_enabled(const struct timer_gp_dev_t* dev,
        const enum timer_gp_alarm_identifier_t alarm)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    return GET_BIT(register_map->irq_mask, alarm);
}

bool timer_gp_interrupt_is_active(const struct timer_gp_dev_t* dev,
        const enum timer_gp_read_alarm_identifier_t alarm)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    return GET_BIT(register_map->irq_status, alarm);
}

void timer_gp_interrupt_clear(const struct timer_gp_dev_t* dev,
                              const enum timer_gp_alarm_identifier_t alarm)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    enum timer_gp_read_alarm_identifier_t read_alarm =
                    ((alarm == TIMER_GP_ALARM_0) ?
                    (TIMER_GP_READ_ALARM_0) :
                    (TIMER_GP_READ_ALARM_1));

    SET_BIT(register_map->irq_clear, alarm);

    /* Clear bit is not self-clearing and some pulse width is required
     * for successful interrupt clear, so we have to check whether the
     * interrupt is cleared. */
    while(timer_gp_interrupt_is_active(dev, read_alarm))
        ;
    CLR_BIT(register_map->irq_clear, alarm);
}

void timer_gp_set_alarm_value(const struct timer_gp_dev_t* dev,
                              const enum timer_gp_alarm_identifier_t alarm,
                              const uint32_t value)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    if (alarm == TIMER_GP_ALARM_0) {
        register_map->alarm0 = value;
    } else {
        register_map->alarm1 = value;
    }
}

uint32_t timer_gp_get_alarm_value(const struct timer_gp_dev_t* dev,
                                  const enum timer_gp_alarm_identifier_t alarm)
{
    struct timer_gp_reg_map_t* const register_map =
            (struct timer_gp_reg_map_t*)dev->cfg->base;
    if (alarm == TIMER_GP_ALARM_0) {
        return register_map->alarm0;
    } else {
        return register_map->alarm1;
    }
}
