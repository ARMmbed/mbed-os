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

#include "arm_uart_drv.h"

#include <stddef.h>

/* UART register map structure */
struct _arm_uart_reg_map_t {
    volatile uint32_t data;   /* Offset: 0x000 (R/W) data register    */
    volatile uint32_t state;  /* Offset: 0x004 (R/W) status register  */
    volatile uint32_t ctrl;   /* Offset: 0x008 (R/W) control register */
    union {
        volatile uint32_t intrstatus;  /* Offset: 0x00c (R/ ) interrupt status
                                        *                     register */
        volatile uint32_t intrclear;   /* Offset: 0x00c ( /W) interrupt clear
                                        *                     register  */
    }intr_reg;
    volatile uint32_t bauddiv;        /* Offset: 0x010 (R/W) Baudrate divider
                                       *                     register */
};

/* CTRL Register */
#define ARM_UART_TX_EN       (1ul << 0)
#define ARM_UART_RX_EN       (1ul << 1)
#define ARM_UART_TX_INTR_EN  (1ul << 2)
#define ARM_UART_RX_INTR_EN  (1ul << 3)

/* STATE Register */
#define ARM_UART_TX_BF  (1ul << 0)
#define ARM_UART_RX_BF  (1ul << 1)

/* INTSTATUS Register */
#define ARM_UART_TX_INTR  (1ul << 0)
#define ARM_UART_RX_INTR  (1ul << 1)

/* UART state definitions */
#define ARM_UART_INITIALIZED  (1ul << 0)

enum arm_uart_error_t arm_uart_init(struct arm_uart_dev_t* dev,
                                    uint32_t system_clk)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;
    if(system_clk == 0) {
        return ARM_UART_ERR_INVALID_ARG;
    }

    /* Sets baudrate and system clock */
    dev->data->system_clk = system_clk;
    dev->data->baudrate = dev->cfg->default_baudrate;

    /* Sets baudrate */
    p_uart->bauddiv = (dev->data->system_clk / dev->cfg->default_baudrate);

    /* Enables receiver and transmitter */
    p_uart->ctrl = ARM_UART_RX_EN | ARM_UART_TX_EN;

    dev->data->state = ARM_UART_INITIALIZED;

    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_set_baudrate(struct arm_uart_dev_t* dev,
                                            uint32_t baudrate)
{
    uint32_t bauddiv;
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(baudrate == 0) {
        return ARM_UART_ERR_INVALID_BAUD;
    }

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return ARM_UART_ERR_NOT_INIT;
    }

    /* Sets baudrate */
    bauddiv = (dev->data->system_clk / baudrate);
    dev->data->baudrate = baudrate;

    /* Minimum bauddiv value */
    if(bauddiv < 16) {
        return ARM_UART_ERR_INVALID_BAUD;
    }

    p_uart->bauddiv = bauddiv;

    return ARM_UART_ERR_NONE;
}

uint32_t arm_uart_get_baudrate(struct arm_uart_dev_t* dev)
{
    return dev->data->baudrate;
}

enum arm_uart_error_t arm_uart_set_clock(struct arm_uart_dev_t* dev,
                                         uint32_t system_clk)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(system_clk == 0) {
        return ARM_UART_ERR_INVALID_ARG;
    }

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return ARM_UART_ERR_NOT_INIT;
    }

    /* Sets system clock */
    dev->data->system_clk = system_clk;

    /* Updates baudrate divider */
    p_uart->bauddiv = (dev->data->system_clk / dev->data->baudrate);

    /* Enables receiver and transmitter */
    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_read(struct arm_uart_dev_t* dev, uint8_t* byte)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(p_uart->state & ARM_UART_RX_BF)) {
        return ARM_UART_ERR_NOT_READY;
    }

    /* Reads data */
    *byte = (uint8_t)p_uart->data;

    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_write(struct arm_uart_dev_t* dev, uint8_t byte)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(p_uart->state & ARM_UART_TX_BF) {
        return ARM_UART_ERR_NOT_READY;
    }

    /* Sends data */
    p_uart->data = byte;

    return ARM_UART_ERR_NONE;
}

enum arm_uart_error_t arm_uart_irq_tx_enable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return ARM_UART_ERR_NOT_INIT;
    }

    p_uart->ctrl |= ARM_UART_TX_INTR_EN;

    return ARM_UART_ERR_NONE;
}

void arm_uart_irq_tx_disable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(dev->data->state & ARM_UART_INITIALIZED ) {
        p_uart->ctrl &= ~ARM_UART_TX_INTR_EN;
    }
}

uint32_t arm_uart_tx_ready(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return 0;
    }

    return !(p_uart->state & ARM_UART_TX_BF);
}

enum arm_uart_error_t arm_uart_irq_rx_enable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return ARM_UART_ERR_NOT_INIT;
    }

    p_uart->ctrl |= ARM_UART_RX_INTR_EN;

    return ARM_UART_ERR_NONE;
}

void arm_uart_irq_rx_disable(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(dev->data->state & ARM_UART_INITIALIZED) {
        p_uart->ctrl &= ~ARM_UART_RX_INTR_EN;
    }
}

uint32_t arm_uart_rx_ready(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(!(dev->data->state & ARM_UART_INITIALIZED)) {
        return 0;
    }

    return (p_uart->state & ARM_UART_RX_BF);
}

void arm_uart_clear_interrupt(struct arm_uart_dev_t* dev,
                              enum arm_uart_irq_t irq)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;

    if(dev->data->state & ARM_UART_INITIALIZED) {
        /* Clears pending interrupts */
        switch(irq) {
        case ARM_UART_IRQ_RX:
            p_uart->intr_reg.intrclear = ARM_UART_RX_INTR;
            break;
        case ARM_UART_IRQ_TX:
            p_uart->intr_reg.intrclear = ARM_UART_TX_INTR;
            break;
        case ARM_UART_IRQ_COMBINED:
            p_uart->intr_reg.intrclear = (ARM_UART_RX_INTR | ARM_UART_TX_INTR);
            break;
        case ARM_UART_IRQ_NONE:
            break;
        /* default: not defined to force all cases to be handled */
        }
    }
}

enum arm_uart_irq_t arm_uart_get_interrupt_status(struct arm_uart_dev_t* dev)
{
    struct _arm_uart_reg_map_t* p_uart =
                                    (struct _arm_uart_reg_map_t*)dev->cfg->base;
    enum arm_uart_irq_t intrstatus = ARM_UART_IRQ_NONE;


    if(dev->data->state & ARM_UART_INITIALIZED) {
        switch(p_uart->intr_reg.intrstatus) {
        case ARM_UART_TX_INTR:
            intrstatus = ARM_UART_IRQ_TX;
            break;
        case ARM_UART_RX_INTR:
            intrstatus = ARM_UART_IRQ_RX;
            break;
        case ARM_UART_TX_INTR | ARM_UART_RX_INTR:
            intrstatus = ARM_UART_IRQ_COMBINED;
            break;
        default:
            break;
        }
    }

    return intrstatus;
}
