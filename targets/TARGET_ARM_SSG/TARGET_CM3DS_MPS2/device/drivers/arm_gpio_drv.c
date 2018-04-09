/*
 * Copyright (c) 2016-2018 ARM Limited
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

#include "arm_gpio_drv.h"

#include <stddef.h>

/* GPIO state definitions */
#define ARM_GPIO_INITIALIZED  (1 << 0)

#define MAX_PIN_NBR           16

/* GPIO register map structure */
struct arm_gpio_reg_map_t {
    volatile uint32_t  data;           /* Offset: 0x000 (R/W) Data register */
    volatile uint32_t  dataout;        /* Offset: 0x004 (R/W) Data output
                                        *                     latch register */
    volatile uint32_t  reserved0[2];
    volatile uint32_t  outenableset;   /* Offset: 0x010 (R/W) Output enable
                                        *                     set register */
    volatile uint32_t  outenableclr;   /* Offset: 0x014 (R/W) Output enable
                                        *                     clear register */
    volatile uint32_t  altfuncset;     /* Offset: 0x018 (R/W) Alternate function
                                        *                     set register */
    volatile uint32_t  altfuncclr;     /* Offset: 0x01C (R/W) Alternate function
                                        *                     clear register */
    volatile uint32_t  intenset;       /* Offset: 0x020 (R/W) Interrupt enable
                                        *                     set register */
    volatile uint32_t  intenclr;       /* Offset: 0x024 (R/W) Interrupt enable
                                        *                     clear register */
    volatile uint32_t  inttypeset;     /* Offset: 0x028 (R/W) Interrupt type
                                        *                     set register */
    volatile uint32_t  inttypeclr;     /* Offset: 0x02C (R/W) Interrupt type
                                        *                     clear register */
    volatile uint32_t  intpolset;      /* Offset: 0x030 (R/W) Interrupt polarity
                                        *                     set register */
    volatile uint32_t  intpolclr;      /* Offset: 0x034 (R/W) Interrupt polarity
                                        *                     clear register */
    union {
        volatile uint32_t  intstatus;  /* Offset: 0x038 (R/ ) Interrupt status
                                        *                     register */
        volatile uint32_t  intclear;   /* Offset: 0x038 ( /W) Interrupt clear
                                        *                     register */
    }intreg;
    volatile uint32_t reserved1[241];
    volatile uint32_t lb_masked[256];  /* Offset: 0x400 - 0x7FC (R/W)
                                        * Lower byte masked access register */
    volatile uint32_t ub_masked[256];  /* Offset: 0x800 - 0xBFC (R/W)
                                        * Upper byte masked access register */
};

/*
 * \brief  Configures the pin or port.
 *
 * \param[in]  p_gpio_port  GPIO port to configure \ref ahbarm_gpio_reg_map_t
 * \param[in]  mask         Pin bit mask.
 * \param[in]  flags        Pin flags.
 */
static void set_port_config(struct arm_gpio_reg_map_t* p_gpio_port,
                            uint32_t mask,
                            uint32_t flags)
{
    if(flags & ARM_GPIO_PIN_DISABLE) {
        p_gpio_port->altfuncset = mask;
        return;
    }

    if(flags & ARM_GPIO_OUTPUT) {
        p_gpio_port->outenableset = mask;
    } else if(flags & ARM_GPIO_INPUT) {
        p_gpio_port->outenableclr = mask;
    }

    /* Sets interrupt configuration */
    if(flags & ARM_GPIO_IRQ) {
        /* Interrupt type: EDGE = 1 - LEVEL = 0 */
        if(flags & ARM_GPIO_IRQ_EDGE) {
            p_gpio_port->inttypeset = mask;
        } else if(flags & ARM_GPIO_IRQ_LEVEL) {
            p_gpio_port->inttypeclr = mask;
        }

        /* Interrupt polarity */
        if(flags & ARM_GPIO_IRQ_ACTIVE_LOW) {
            p_gpio_port->intpolclr = mask;
        } else if(flags & ARM_GPIO_IRQ_ACTIVE_HIGH) {
            p_gpio_port->intpolset = mask;
        }
    }

    if(flags & ARM_GPIO_PIN_ENABLE) {
        p_gpio_port->altfuncclr = mask;
    }
}

void arm_gpio_init(struct arm_gpio_dev_t* dev)
{
    struct arm_gpio_reg_map_t* p_gpio_port =
                                     (struct arm_gpio_reg_map_t*)dev->cfg->base;

    if(dev->data->state != ARM_GPIO_INITIALIZED) {
        /* Disables all pins in this port */
        set_port_config(p_gpio_port, DEFAULT_PORT_MASK, ARM_GPIO_PIN_DISABLE);

        dev->data->port_mask = DEFAULT_PORT_MASK;
        dev->data->state = ARM_GPIO_INITIALIZED;
    }
}

enum arm_gpio_error_t arm_gpio_config(struct arm_gpio_dev_t* dev,
                                      enum arm_gpio_access_t access,
                                      uint8_t pin_num,
                                      uint32_t flags)
{
    uint32_t pin_mask;
    struct arm_gpio_reg_map_t* p_gpio_port =
                                     (struct arm_gpio_reg_map_t*)dev->cfg->base;

    if(dev->data->state != ARM_GPIO_INITIALIZED) {
        return ARM_GPIO_ERR_PORT_NOT_INIT;
    }

    if(pin_num >= MAX_PIN_NBR) {
        return ARM_GPIO_ERR_INVALID_ARG;
    }

    switch(access) {
    case ARM_GPIO_ACCESS_PIN:
        pin_mask = (1UL << pin_num);
        set_port_config(p_gpio_port, pin_mask, flags);
        break;
    case ARM_GPIO_ACCESS_PORT:
        set_port_config(p_gpio_port, dev->data->port_mask, flags);
        break;
    /* default:  The default is not defined intentionally to force the
     *           compiler to check that all the enumeration values are
     *           covered in the switch.*/
    }

    return ARM_GPIO_ERR_NONE;
}

enum arm_gpio_error_t arm_gpio_write(struct arm_gpio_dev_t* dev,
                                     enum arm_gpio_access_t access,
                                     uint8_t pin_num,
                                     uint32_t value)
{
    struct arm_gpio_reg_map_t* p_gpio_port =
                                     (struct arm_gpio_reg_map_t*)dev->cfg->base;

    if(pin_num >= MAX_PIN_NBR) {
        return ARM_GPIO_ERR_INVALID_ARG;
    }

    /* As ARM is a read-modify-write architecture, before set a
     * value on a GPIO register it is required to disable the
     * interrupts to prevent problems in a multitasking
     * environment */
    switch(access) {
    case ARM_GPIO_ACCESS_PIN:
        if(value) {
            /* Sets the pin */
            p_gpio_port->dataout |= (1UL << pin_num);
        } else {
            /* Clears the pin */
            p_gpio_port->dataout &= ~(1UL << pin_num);
        }
        break;
    case ARM_GPIO_ACCESS_PORT:
        if(value) {
            /* Sets masked pins */
            p_gpio_port->dataout |= dev->data->port_mask;
        } else {
            /* Clears masked pins */
            p_gpio_port->dataout &= ~(dev->data->port_mask);
        }
        break;
    /* default: The default is not defined intentionally to force the
     *          compiler to check that all the enumeration values are
     *          covered in the switch. */
    }

    return ARM_GPIO_ERR_NONE;
}

int32_t arm_gpio_read(struct arm_gpio_dev_t* dev, enum arm_gpio_access_t access,
                      uint8_t pin_num)
{
    uint32_t value;
    struct arm_gpio_reg_map_t* p_gpio_port =
                                     (struct arm_gpio_reg_map_t*)dev->cfg->base;

    value = p_gpio_port->data;

    if(access == ARM_GPIO_ACCESS_PIN) {
        if(pin_num >= MAX_PIN_NBR) {
            return -1;
        }
        value = ((value >> pin_num) & 1UL);
    } else {
        value &= dev->data->port_mask;
    }

    return (int32_t)value;
}

enum arm_gpio_error_t arm_gpio_set_interrupt(struct arm_gpio_dev_t* dev,
                                             enum arm_gpio_access_t access,
                                             uint8_t pin_num,
                                             enum arm_gpio_irq_status_t status)
{
    uint32_t mask = 0;
    struct arm_gpio_reg_map_t* p_gpio_port =
                                     (struct arm_gpio_reg_map_t*)dev->cfg->base;

    if(dev->data->state != ARM_GPIO_INITIALIZED) {
        return ARM_GPIO_ERR_PORT_NOT_INIT;
    }

    if(pin_num >= MAX_PIN_NBR) {
        return ARM_GPIO_ERR_INVALID_ARG;
    }

    switch(access) {
    case ARM_GPIO_ACCESS_PIN:
        mask = (1UL << pin_num);
        break;
    case ARM_GPIO_ACCESS_PORT:
        mask = dev->data->port_mask;
        break;
    /* default: The default is not defined intentionally to force the
     *          compiler to check that all the enumeration values are
     *          covered in the switch.*/
    }

    if(status == ARM_GPIO_IRQ_ENABLE) {
        p_gpio_port->intenset = mask;
    } else {
        p_gpio_port->intenclr = mask;
    }

    return ARM_GPIO_ERR_NONE;
}

enum arm_gpio_error_t arm_gpio_get_irq_status(struct arm_gpio_dev_t* dev,
                                              enum arm_gpio_access_t access,
                                              uint8_t pin_num,
                                              uint32_t* status)
{
    struct arm_gpio_reg_map_t* p_gpio_port =
                                    (struct arm_gpio_reg_map_t*)dev->cfg->base;

    if(dev->data->state != ARM_GPIO_INITIALIZED) {
        return ARM_GPIO_ERR_PORT_NOT_INIT;
    }

    if(pin_num >= MAX_PIN_NBR) {
        return ARM_GPIO_ERR_INVALID_ARG;
    }

    *status = p_gpio_port->intreg.intstatus;

    if(access == ARM_GPIO_ACCESS_PIN) {
        *status = ((*status >> pin_num) & 1UL);
    } else {
        *status &= dev->data->port_mask;
    }

    return ARM_GPIO_ERR_NONE;
}

enum arm_gpio_error_t arm_gpio_clear_interrupt(struct arm_gpio_dev_t* dev,
                                               uint8_t pin_num)
{
    struct arm_gpio_reg_map_t* p_gpio_port =
                                     (struct arm_gpio_reg_map_t*)dev->cfg->base;

    if(dev->data->state != ARM_GPIO_INITIALIZED) {
        return ARM_GPIO_ERR_PORT_NOT_INIT;
    }

    if(pin_num >= MAX_PIN_NBR) {
        return ARM_GPIO_ERR_INVALID_ARG;
    }

    p_gpio_port->intreg.intstatus = (1UL << pin_num);

    return ARM_GPIO_ERR_NONE;
}

void arm_gpio_set_port_mask(struct arm_gpio_dev_t* dev, uint32_t port_mask)
{
    dev->data->port_mask = (port_mask & DEFAULT_PORT_MASK);
}

uint32_t arm_gpio_get_port_mask(struct arm_gpio_dev_t* dev)
{
    if(dev->data->state != ARM_GPIO_INITIALIZED) {
        return 0;
    }

    return (dev->data->port_mask & DEFAULT_PORT_MASK);
}
