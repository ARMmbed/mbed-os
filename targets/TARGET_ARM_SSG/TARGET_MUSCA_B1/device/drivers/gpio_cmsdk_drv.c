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

#include <stdint.h>
#include <stddef.h>
#include "gpio_cmsdk_drv.h"

/* GPIO register map structure */
struct gpio_cmsdk_reg_map_t {
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
    volatile uint32_t reserved1[997];
    volatile uint32_t pid4;            /* Peripheral ID Register 4 */
    volatile uint32_t pid0;            /* Peripheral ID Register 0 */
    volatile uint32_t pid1;            /* Peripheral ID Register 1 */
    volatile uint32_t pid2;            /* Peripheral ID Register 2 */
    volatile uint32_t pid3;            /* Peripheral ID Register 3 */
    volatile uint32_t cid0;            /* Component ID Register 0 */
    volatile uint32_t cid1;            /* Component ID Register 1 */
    volatile uint32_t cid2;            /* Component ID Register 2 */
    volatile uint32_t cid4;            /* Component ID Register 3 */
};

void gpio_cmsdk_init(struct gpio_cmsdk_dev_t* dev)
{
    /* Nothing to init on the GPIO device */
}

/**
 * \brief Configures port.
 *
 * \param[in] dev             GPIO device to initalize \ref gpio_cmsdk_dev_t
 * \param[in] pin_mask        Pin mask for port access
 * \param[in] direction       Input or output \ref gpio_cmsdk_direction_t
 * \param[in] altfunc_flags   Alternate function \ref gpio_cmsdk_altfunc_t
 *
 */
static void set_port_config(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                            enum gpio_cmsdk_direction_t direction,
                            enum gpio_cmsdk_altfunc_t altfunc_flags)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                            (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(direction == GPIO_CMSDK_INPUT) {
        p_gpio_port->outenableclr = pin_mask;
    } else {
        p_gpio_port->outenableset = pin_mask;
    }

    if (altfunc_flags == GPIO_CMSDK_MAIN_FUNC) {
        p_gpio_port->altfuncclr = pin_mask;
    } else {
        p_gpio_port->altfuncset = pin_mask;
    }

    return;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_pin_config(struct gpio_cmsdk_dev_t* dev, uint32_t pin_num,
                    enum gpio_cmsdk_direction_t direction,
                    enum gpio_cmsdk_altfunc_t altfunc_flags)
{
    uint32_t pin_mask = (1UL << pin_num);

    if(pin_num >= GPIO_CMSDK_MAX_PIN_NUM) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    set_port_config(dev, pin_mask, direction, altfunc_flags);

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_port_config(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                     enum gpio_cmsdk_direction_t direction,
                     enum gpio_cmsdk_altfunc_t altfunc_flags)
{
    if(pin_mask > GPIO_CMSDK_MAX_PORT_MASK) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    set_port_config(dev, pin_mask, direction, altfunc_flags);

    return GPIO_CMSDK_ERR_NONE;
}

void gpio_cmsdk_config_irq(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                         enum gpio_cmsdk_irq_type_t irq_type,
                         enum gpio_cmsdk_irq_polarity_t irq_pol)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    /* Interrupt type: EDGE = 1 - LEVEL = 0 */
    if(irq_type == GPIO_CMSDK_IRQ_EDGE) {
        p_gpio_port->inttypeset = pin_mask;
    } else if(irq_type == GPIO_CMSDK_IRQ_LEVEL) {
        p_gpio_port->inttypeclr = pin_mask;
    }

    /* Interrupt polarity */
    if(irq_pol == GPIO_CMSDK_IRQ_LOW_OR_FALLING_EDGE) {
        p_gpio_port->intpolclr = pin_mask;
    } else if(irq_pol == GPIO_CMSDK_IRQ_HIGH_OR_RISING_EDGE) {
        p_gpio_port->intpolset = pin_mask;
    }
}

enum gpio_cmsdk_error_t gpio_cmsdk_pin_write(struct gpio_cmsdk_dev_t* dev,
                                         uint32_t pin_num,
                                         uint32_t value)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    /* GPIO data output register is a read-modify-write register,
     * so before writing a value on a GPIO pin it is required to disable
     * the interrupts to prevent concurrency problems.
     */
    if(pin_num >= GPIO_CMSDK_MAX_PIN_NUM) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }
    if(value) {
        /* Sets the pin */
        p_gpio_port->dataout |= (1UL << pin_num);
    } else {
        /* Clears the pin */
        p_gpio_port->dataout &= ~(1UL << pin_num);
    }

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t gpio_cmsdk_port_write(struct gpio_cmsdk_dev_t* dev,
                                          uint32_t pin_mask,
                                          uint32_t value)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    /* GPIO data output register is a read-modify-write register,
     * so before writing a value on a GPIO pin it is required to disable
     * the interrupts to prevent concurrency problems.
     */
    if(pin_mask > GPIO_CMSDK_MAX_PORT_MASK) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    /* Clear all bits defined in the mask,
     * and set selected bits from value parameter.
     */
    p_gpio_port->dataout =
                    ((~pin_mask & p_gpio_port->dataout) | (pin_mask  & value));

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_pin_read(struct gpio_cmsdk_dev_t* dev,
                                    uint32_t pin_num,
                                    uint32_t *data)
{
    uint32_t value;
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(pin_num >= GPIO_CMSDK_MAX_PIN_NUM) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    value = p_gpio_port->data;

    *data = (value >> pin_num) & 1UL;

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_port_read(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                   uint32_t *data)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(pin_mask > GPIO_CMSDK_MAX_PORT_MASK) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    *data = p_gpio_port->data & pin_mask;

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_set_pin_irq_cfg(struct gpio_cmsdk_dev_t* dev, uint32_t pin_num,
                         enum gpio_cmsdk_irq_status_t status)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(pin_num >= GPIO_CMSDK_MAX_PIN_NUM) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    if(status == GPIO_CMSDK_IRQ_ENABLE) {
        p_gpio_port->intenset = (1UL << pin_num);
    } else {
        p_gpio_port->intenclr = (1UL << pin_num);
    }

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_set_port_irq_cfg(struct gpio_cmsdk_dev_t* dev, uint32_t pin_mask,
                          enum gpio_cmsdk_irq_status_t status)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(pin_mask > GPIO_CMSDK_MAX_PORT_MASK) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    if(status == GPIO_CMSDK_IRQ_ENABLE) {
        p_gpio_port->intenset = pin_mask;
    } else {
        p_gpio_port->intenclr = pin_mask;
    }

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_get_pin_irq_status(struct gpio_cmsdk_dev_t* dev,
                            uint32_t pin_num, uint32_t* status)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(pin_num >= GPIO_CMSDK_MAX_PIN_NUM) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    *status = ((p_gpio_port->intreg.intstatus >> pin_num) & 1UL);

    return GPIO_CMSDK_ERR_NONE;
}

enum gpio_cmsdk_error_t
gpio_cmsdk_get_port_irq_status(struct gpio_cmsdk_dev_t* dev,
                             uint32_t pin_mask, uint32_t* status)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(pin_mask > GPIO_CMSDK_MAX_PORT_MASK) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    *status = (p_gpio_port->intreg.intstatus & pin_mask);

    return GPIO_CMSDK_ERR_NONE;
}


enum gpio_cmsdk_error_t gpio_cmsdk_clear_irq(struct gpio_cmsdk_dev_t* dev,
                                         uint8_t pin_num)
{
    struct gpio_cmsdk_reg_map_t* p_gpio_port =
                                (struct gpio_cmsdk_reg_map_t*)dev->cfg->base;

    if(pin_num >= GPIO_CMSDK_MAX_PIN_NUM) {
        return GPIO_CMSDK_ERR_INVALID_ARG;
    }

    p_gpio_port->intreg.intclear = (1UL << pin_num);

    return GPIO_CMSDK_ERR_NONE;
}
