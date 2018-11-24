/*
 * Copyright (c) 2016-2018 Arm Limited
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
* \file dualtimer_cmsdk_drv.c
* \brief Generic driver for CMSDK Dualtimers.
*/

#include "dualtimer_cmsdk_drv.h"

/** Dualtimer state definition */
#define DUALTIMER_CMSDK_INITIALIZED    (1u)

/** Dual timer control register bit indexes */
#define CTRL_REG_ONE_SHOT_INDEX        (0u)
#define CTRL_REG_SIZE_INDEX            (1u)
#define CTRL_REG_PRESCALE_INDEX        (3u)
#define CTRL_REG_IRQ_EN_INDEX          (5u)
#define CTRL_REG_MODE_INDEX            (6u)
#define CTRL_REG_TIMER_EN_INDEX        (7u)

/** Dual timer interrupt registers bit indexes */
#define XRIS_REG_RAW_IRQ_INDEX         (0u)
#define XMIS_REG_MASKED_IRQ_INDEX      (0u)
#define XINTCLR_REG_CLEAR_IRQ_INDEX    (0u)

/** Misc definition */
#define TWO_BITS_WIDTH                 (2u)

/** Bit handling macros */
#define ATOMIC_SET_BIT(WORD, BIT_INDEX) (WORD = (1u << BIT_INDEX))
#define ATOMIC_CLR_BIT(WORD, BIT_INDEX) (WORD = ~(1u << BIT_INDEX))
#define SET_BIT(WORD, BIT_INDEX) (WORD |= (1u << BIT_INDEX))
#define CLR_BIT(WORD, BIT_INDEX) (WORD &= ~(1u << BIT_INDEX))
#define GET_BIT(WORD, BIT_INDEX) (bool)((WORD & (1u << BIT_INDEX)))

/** Generate a flexible width left shifted bit-mask */
#define BIT_MASK(BIT_INDEX, WIDTH)\
                 (((1u << (WIDTH)) - 1u) << (BIT_INDEX - WIDTH + 1u))

/** Set N number of bits defined by width at index position in a register */
#define SET_NBITS(WORD, BIT_INDEX, WIDTH, DATA)\
                  (WORD |= (DATA << (BIT_INDEX - WIDTH + 1u))\
                  & BIT_MASK(BIT_INDEX, WIDTH))

/** Get N number of bits defined by width at index position in a register */
#define GET_NBITS(WORD, BIT_INDEX, WIDTH)\
                    ((WORD & BIT_MASK(BIT_INDEX, WIDTH))\
                    >> (BIT_INDEX - WIDTH + 1u))

/** Map the registers for dualtimer device */
#define DUALTIMER_REG_MAP(DEVICE)\
                    ((struct dualtimer_cmsdk_reg_map_t*)((DEVICE)->cfg->base))

/** DualTimer register map structure */
struct dualtimer_cmsdk_reg_map_t {
    volatile uint32_t timer1load;     /** Offset: 0x000 (R/W) Timer 1 Load */
    volatile uint32_t timer1value;    /** Offset: 0x004 (R/ ) Timer 1 Counter
                                       *                     Current Value */
    volatile uint32_t timer1ctrl;     /** Offset: 0x008 (R/W) Timer 1 Control */
    volatile uint32_t timer1intclr;   /** Offset: 0x00C ( /W) Timer 1 Interrupt
                                       *                     Clear */
    volatile uint32_t timer1ris;      /** Offset: 0x010 (R/ ) Timer 1 Raw
                                       *                     Interrupt Status */
    volatile uint32_t timer1mis;      /** Offset: 0x014 (R/ ) Timer 1 Masked
                                       *                     Interrupt Status */
    volatile uint32_t timer1bgload;   /** Offset: 0x018 (R/W) Background Load
                                       *                     Register */
    volatile uint32_t reserved0;
    volatile uint32_t timer2load;     /** Offset: 0x020 (R/W) Timer 2 Load */
    volatile uint32_t timer2value;    /** Offset: 0x024 (R/ ) Timer 2 Counter
                                       *                     Current Value */
    volatile uint32_t timer2ctrl;     /** Offset: 0x028 (R/W) Timer 2 Control */
    volatile uint32_t timer2intclr;   /** Offset: 0x02C ( /W) Timer 2 Interrupt
                                       *                     Clear */
    volatile uint32_t timer2ris;      /** Offset: 0x030 (R/ ) Timer 2 Raw
                                       *                     Interrupt Status */
    volatile uint32_t timer2mis;      /** Offset: 0x034 (R/ ) Timer 2 Masked
                                       *                     Interrupt Status */
    volatile uint32_t timer2bgload;   /** Offset: 0x038 (R/W) Background Load
                                       *                     Register */
    volatile uint32_t reserved1[945];
    volatile uint32_t itcr;           /** Offset: 0xF00 (R/W) Integration Test
                                       *                     Control Register */
    volatile uint32_t itop;           /** Offset: 0xF04 ( /W) Integration Test
                                       *                     Output Set
                                       *                     Register */
};

static void dualtimer_cmsdk_defaults(const struct dualtimer_cmsdk_dev_t* dev)
{
    dualtimer_cmsdk_disable_both_timers(dev);
    dualtimer_cmsdk_clear_interrupt_timer1(dev);
    dualtimer_cmsdk_clear_interrupt_timer2(dev);
    dualtimer_cmsdk_set_oneshot_count_both_timers(dev,
                                                  DUALTIMER_CMSDK_WRAPPING);
    dualtimer_cmsdk_set_size_both_timers(dev, DUALTIMER_CMSDK_SIZE_16BIT);
    dualtimer_cmsdk_set_prescale_both_timers(dev,
                                             DUALTIMER_CMSDK_CLOCK_DIV1);
    dualtimer_cmsdk_enable_interrupt_both_timers(dev);
    dualtimer_cmsdk_set_mode_both_timers(dev, DUALTIMER_CMSDK_FREE_RUNNING);
    dualtimer_cmsdk_set_reload_both_timers(dev,
                                           DUALTIMER_CMSDK_DEF_16BIT_RELOAD);
}
/** Init */
void dualtimer_cmsdk_init(const struct dualtimer_cmsdk_dev_t* dev)
{
    if (DUALTIMER_CMSDK_INITIALIZED != dev->data->is_initialized) {
        dualtimer_cmsdk_defaults(dev);
        dev->data->is_initialized = 1;
    }
}

bool dualtimer_cmsdk_is_initialized(const struct dualtimer_cmsdk_dev_t* dev)
{
    return(bool)dev->data->is_initialized;
}

void dualtimer_cmsdk_free(const struct dualtimer_cmsdk_dev_t* dev)
{
    dualtimer_cmsdk_disable_both_timers(dev);
    dev->data->is_initialized = 0;
}

/** Enable - Disable */
void dualtimer_cmsdk_enable_timer1(const struct dualtimer_cmsdk_dev_t* dev)
{
    SET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_TIMER_EN_INDEX);
}

void dualtimer_cmsdk_enable_timer2(const struct dualtimer_cmsdk_dev_t* dev)
{
    SET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_TIMER_EN_INDEX);
}

void dualtimer_cmsdk_enable_both_timers(const struct dualtimer_cmsdk_dev_t* dev)
{
    dualtimer_cmsdk_enable_timer1(dev);
    dualtimer_cmsdk_enable_timer2(dev);
}

void dualtimer_cmsdk_disable_timer1(const struct dualtimer_cmsdk_dev_t* dev)
{
    CLR_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_TIMER_EN_INDEX);
}

void dualtimer_cmsdk_disable_timer2(const struct dualtimer_cmsdk_dev_t* dev)
{
    CLR_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_TIMER_EN_INDEX);
}

void dualtimer_cmsdk_disable_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    dualtimer_cmsdk_disable_timer1(dev);
    dualtimer_cmsdk_disable_timer2(dev);
}

/** Interrupts */
void dualtimer_cmsdk_enable_interrupt_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    SET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_IRQ_EN_INDEX);
}

void dualtimer_cmsdk_enable_interrupt_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    SET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_IRQ_EN_INDEX);
}

void dualtimer_cmsdk_enable_interrupt_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    dualtimer_cmsdk_enable_interrupt_timer1(dev);
    dualtimer_cmsdk_enable_interrupt_timer2(dev);
}

void dualtimer_cmsdk_disable_interrupt_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    CLR_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_IRQ_EN_INDEX);
}

void dualtimer_cmsdk_disable_interrupt_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    CLR_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_IRQ_EN_INDEX);
}

    void dualtimer_cmsdk_disable_interrupt_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    dualtimer_cmsdk_disable_interrupt_timer1(dev);
    dualtimer_cmsdk_disable_interrupt_timer2(dev);
}

void dualtimer_cmsdk_clear_interrupt_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    ATOMIC_SET_BIT(DUALTIMER_REG_MAP(dev)->timer1intclr,
                   XINTCLR_REG_CLEAR_IRQ_INDEX);
}

void dualtimer_cmsdk_clear_interrupt_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    ATOMIC_SET_BIT(DUALTIMER_REG_MAP(dev)->timer2intclr,
                   XINTCLR_REG_CLEAR_IRQ_INDEX);
}

/** One Shot */
void dualtimer_cmsdk_set_oneshot_count_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        enum dualtimer_cmsdk_oneshot_t mode)
{
    if (DUALTIMER_CMSDK_ONE_SHOT == mode) {
        SET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_ONE_SHOT_INDEX);
    } else {
        CLR_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_ONE_SHOT_INDEX);
    }
}

void dualtimer_cmsdk_set_oneshot_count_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        enum dualtimer_cmsdk_oneshot_t mode)
{
    if (DUALTIMER_CMSDK_ONE_SHOT == mode) {
        SET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_ONE_SHOT_INDEX);
    } else {
        CLR_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_ONE_SHOT_INDEX);
    }
}

void dualtimer_cmsdk_set_oneshot_count_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        enum dualtimer_cmsdk_oneshot_t mode)
{
    dualtimer_cmsdk_set_oneshot_count_timer1(dev, mode);
    dualtimer_cmsdk_set_oneshot_count_timer2(dev, mode);
}

/** Timer Size */
void dualtimer_cmsdk_set_size_timer1(const struct dualtimer_cmsdk_dev_t* dev,
                                     enum dualtimer_cmsdk_size_t bit_size)
{
    if (DUALTIMER_CMSDK_SIZE_32BIT == bit_size) {
        SET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_SIZE_INDEX);
    } else {
        CLR_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_SIZE_INDEX);
    }
}
void dualtimer_cmsdk_set_size_timer2(const struct dualtimer_cmsdk_dev_t* dev,
                                     enum dualtimer_cmsdk_size_t bit_size)
{
    if (DUALTIMER_CMSDK_SIZE_32BIT == bit_size) {
        SET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_SIZE_INDEX);
    } else {
        CLR_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_SIZE_INDEX);
    }
}

void dualtimer_cmsdk_set_size_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        enum dualtimer_cmsdk_size_t bit_size)
{
    dualtimer_cmsdk_set_size_timer1(dev, bit_size);
    dualtimer_cmsdk_set_size_timer2(dev, bit_size);
}

enum dualtimer_cmsdk_size_t dualtimer_cmsdk_get_size_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    if(GET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_SIZE_INDEX)) {
        return DUALTIMER_CMSDK_SIZE_32BIT;
    }
    return DUALTIMER_CMSDK_SIZE_16BIT;
}

enum dualtimer_cmsdk_size_t  dualtimer_cmsdk_get_size_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    if(GET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_SIZE_INDEX)) {
        return DUALTIMER_CMSDK_SIZE_32BIT;
    }
    return DUALTIMER_CMSDK_SIZE_16BIT;
}

/** Timer Prescale */
void dualtimer_cmsdk_set_prescale_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_prescale_t prescale)
{
    SET_NBITS(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_PRESCALE_INDEX,
              TWO_BITS_WIDTH, prescale);
}

void dualtimer_cmsdk_set_prescale_timer2(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_prescale_t prescale)
{
    SET_NBITS(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_PRESCALE_INDEX,
              TWO_BITS_WIDTH, prescale);
}
void dualtimer_cmsdk_set_prescale_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_prescale_t prescale)
{
    dualtimer_cmsdk_set_prescale_timer1(dev, prescale);
    dualtimer_cmsdk_set_prescale_timer2(dev, prescale);
}

enum dualtimer_cmsdk_prescale_t dualtimer_cmsdk_get_prescale_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (enum dualtimer_cmsdk_prescale_t)\
    GET_NBITS(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_PRESCALE_INDEX,
              TWO_BITS_WIDTH);
}

enum dualtimer_cmsdk_prescale_t dualtimer_cmsdk_get_prescale_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (enum dualtimer_cmsdk_prescale_t)\
    GET_NBITS(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_PRESCALE_INDEX,
              TWO_BITS_WIDTH);
}

/** Timer Mode */
void  dualtimer_cmsdk_set_mode_timer1(const struct dualtimer_cmsdk_dev_t* dev,
                                      enum dualtimer_cmsdk_mode_t mode)
{
    if (DUALTIMER_CMSDK_PERIODIC == mode) {
        SET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_MODE_INDEX);
    } else {
        CLR_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_MODE_INDEX);
    }
}

void dualtimer_cmsdk_set_mode_timer2(const struct dualtimer_cmsdk_dev_t* dev,
                                     enum dualtimer_cmsdk_mode_t mode)
{
    if (DUALTIMER_CMSDK_PERIODIC == mode) {
        SET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_MODE_INDEX);
    } else {
        CLR_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_MODE_INDEX);
    }
}

void dualtimer_cmsdk_set_mode_both_timers(
                                    const struct dualtimer_cmsdk_dev_t* dev,
                                    enum dualtimer_cmsdk_mode_t mode)
{
    dualtimer_cmsdk_set_mode_timer1(dev, mode);
    dualtimer_cmsdk_set_mode_timer2(dev, mode);
}

enum dualtimer_cmsdk_mode_t dualtimer_cmsdk_get_mode_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (enum dualtimer_cmsdk_mode_t)\
    GET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl, CTRL_REG_MODE_INDEX);
}

enum dualtimer_cmsdk_mode_t dualtimer_cmsdk_get_mode_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (enum dualtimer_cmsdk_mode_t)\
    GET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl, CTRL_REG_MODE_INDEX);
}

/** Reload-Background reload */
enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_reload_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload)
{
    const uint32_t rel_size = dualtimer_cmsdk_get_size_timer1(dev);
    if (DUALTIMER_CMSDK_SIZE_16BIT == rel_size && UINT16_MAX <= reload) {
        return DUALTIMER_CMSDK_ERR_OVERFLOW;
    }
    DUALTIMER_REG_MAP(dev)->timer1load = reload;
    return DUALTIMER_CMSDK_ERR_NONE;
}

enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_reload_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload)
{
    const uint32_t rel_size = dualtimer_cmsdk_get_size_timer2(dev);
    if (DUALTIMER_CMSDK_SIZE_16BIT == rel_size && UINT16_MAX <= reload) {
        return DUALTIMER_CMSDK_ERR_OVERFLOW;
    }
    DUALTIMER_REG_MAP(dev)->timer2load = reload;
    return DUALTIMER_CMSDK_ERR_NONE;
}

enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_reload_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload)
{
    uint32_t ret = 0;
    ret +=(uint32_t)dualtimer_cmsdk_set_reload_timer2(dev, reload);
    ret +=(uint32_t)dualtimer_cmsdk_set_reload_timer2(dev, reload);
    if (ret) {
        return DUALTIMER_CMSDK_ERR_OVERFLOW;
    }
    return DUALTIMER_CMSDK_ERR_NONE;
}

uint32_t dualtimer_cmsdk_get_reload_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return DUALTIMER_REG_MAP(dev)->timer1load;
}

uint32_t dualtimer_cmsdk_get_reload_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return DUALTIMER_REG_MAP(dev)->timer2load;
}

enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_bgreload_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload)
{
    const uint32_t rel_size = dualtimer_cmsdk_get_size_timer1(dev);
    if (DUALTIMER_CMSDK_SIZE_16BIT == rel_size && UINT16_MAX <= reload) {
        return DUALTIMER_CMSDK_ERR_OVERFLOW;
    }
    DUALTIMER_REG_MAP(dev)->timer1bgload = reload;
    return DUALTIMER_CMSDK_ERR_NONE;
}

enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_bgreload_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload)
{
    const uint32_t rel_size = dualtimer_cmsdk_get_size_timer2(dev);
    if (DUALTIMER_CMSDK_SIZE_16BIT == rel_size && UINT16_MAX <= reload) {
        return DUALTIMER_CMSDK_ERR_OVERFLOW;
    }
    DUALTIMER_REG_MAP(dev)->timer2bgload = reload;
    return DUALTIMER_CMSDK_ERR_NONE;
}

enum dualtimer_cmsdk_error_t dualtimer_cmsdk_set_bgreload_both_timers(
                                        const struct dualtimer_cmsdk_dev_t* dev,
                                        uint32_t reload)
{
    uint32_t ret = 0;
    ret +=(uint32_t)dualtimer_cmsdk_set_bgreload_timer1(dev, reload);
    ret +=(uint32_t)dualtimer_cmsdk_set_bgreload_timer2(dev, reload);
    if (ret) {
        return DUALTIMER_CMSDK_ERR_OVERFLOW;
    }
    return DUALTIMER_CMSDK_ERR_NONE;
}

uint32_t dualtimer_cmsdk_get_bgreload_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return DUALTIMER_REG_MAP(dev)->timer1bgload;
}

uint32_t dualtimer_cmsdk_get_bgreload_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return DUALTIMER_REG_MAP(dev)->timer2bgload;
}

/** Current timer value */
uint32_t dualtimer_cmsdk_get_current_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return DUALTIMER_REG_MAP(dev)->timer1value;
}

uint32_t dualtimer_cmsdk_get_current_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return DUALTIMER_REG_MAP(dev)->timer2value;
}

/** Status check methods */
bool dualtimer_cmsdk_is_enabled_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl,
                         CTRL_REG_TIMER_EN_INDEX);
}

bool dualtimer_cmsdk_is_enabled_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl,
                         CTRL_REG_TIMER_EN_INDEX);
}

bool dualtimer_cmsdk_is_interrupt_enabled_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer1ctrl,
                         CTRL_REG_IRQ_EN_INDEX);
}

bool dualtimer_cmsdk_is_interrupt_enabled_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer2ctrl,
                         CTRL_REG_IRQ_EN_INDEX);
}

bool dualtimer_cmsdk_is_interrupt_active_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer1mis,
                         XMIS_REG_MASKED_IRQ_INDEX);
}

bool dualtimer_cmsdk_is_interrupt_active_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer2mis,
                         XMIS_REG_MASKED_IRQ_INDEX);
}

bool dualtimer_cmsdk_is_raw_interrupt_active_timer1(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer1ris,
                         XRIS_REG_RAW_IRQ_INDEX);
}

bool dualtimer_cmsdk_is_raw_interrupt_active_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    return (bool)GET_BIT(DUALTIMER_REG_MAP(dev)->timer2ris,
                         XRIS_REG_RAW_IRQ_INDEX);
}

uint32_t dualtimer_cmsdk_get_elapsed_value_timer1(
                                    const struct dualtimer_cmsdk_dev_t* dev)
{
    struct dualtimer_cmsdk_reg_map_t* register_map =
            (struct dualtimer_cmsdk_reg_map_t*)dev->cfg->base;
    return register_map->timer1load - register_map->timer1value;
}

uint32_t dualtimer_cmsdk_get_elapsed_value_timer2(
                                        const struct dualtimer_cmsdk_dev_t* dev)
{
    struct dualtimer_cmsdk_reg_map_t* register_map =
            (struct dualtimer_cmsdk_reg_map_t*)dev->cfg->base;
    return register_map->timer2load - register_map->timer2value;
}

void dualtimer_cmsdk_reset_timer1(const struct dualtimer_cmsdk_dev_t* dev)
{
    struct dualtimer_cmsdk_reg_map_t* register_map =
            (struct dualtimer_cmsdk_reg_map_t*)dev->cfg->base;
    register_map->timer1value = register_map->timer1load;
}

void dualtimer_cmsdk_reset_timer2(const struct dualtimer_cmsdk_dev_t* dev)
{
    struct dualtimer_cmsdk_reg_map_t* register_map =
            (struct dualtimer_cmsdk_reg_map_t*)dev->cfg->base;
    register_map->timer2value = register_map->timer2load;
}

void dualtimer_cmsdk_reset_both_timers(const struct dualtimer_cmsdk_dev_t* dev)
{
    struct dualtimer_cmsdk_reg_map_t* register_map =
            (struct dualtimer_cmsdk_reg_map_t*)dev->cfg->base;
    register_map->timer1value = register_map->timer1load;
    register_map->timer2value = register_map->timer2load;
}