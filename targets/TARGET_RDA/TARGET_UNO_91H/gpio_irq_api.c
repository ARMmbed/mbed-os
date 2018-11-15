/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include <stddef.h>

#include "gpio_irq_api.h"
#include "mbed_error.h"
#include "cmsis.h"

#if DEVICE_INTERRUPTIN
#if defined(GPIO_IRQ_DEBUG)
#include "mbed_interface.h"
#endif /* GPIO_IRQ_DEBUG */

#define GPIO_PINNUM             28
#define NONE                    (uint32_t)NC
#define GPIO_INT_CTRL_REG       (RDA_GPIO->INTCTRL)
#define GPIO_INT_SEL_REG        (RDA_GPIO->INTSEL)
#define GPIO_DATA_IN_REG        (RDA_GPIO->DIN)

typedef enum {
    GPIO_IRQ_CH0,
    GPIO_IRQ_CH1,
    CHANNEL_NUM
} GPIO_IRQ_IDX_T;

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static uint32_t channel_pinidxs[CHANNEL_NUM] = {0};
static uint8_t  channel_bothedge_flag[CHANNEL_NUM] = {0};
static gpio_irq_handler irq_handler[CHANNEL_NUM] = {NULL};

#if defined(GPIO_IRQ_DEBUG)
static uint32_t exception_cntr;
#endif /* GPIO_IRQ_DEBUG */

static GPIO_IRQ_IDX_T gpio_irq_ava_chidx(void)
{
    GPIO_IRQ_IDX_T ret;
    for (ret = GPIO_IRQ_CH0; ret < CHANNEL_NUM; ret++) {
        if (0 == channel_ids[ret])
            break;
    }
    return ret;
}

static uint32_t gpio_irq_pinidx(PinName pin)
{
    uint8_t idx;
    const uint32_t pinmap_gpio_irq[GPIO_PINNUM] = {
        /* GPIO 0 ~ 13 */
        PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7, PB_8, PB_9, PA_8, PA_9, PC_0, PC_1,
        /* GPIO 14 ~ 21, Not support interrupt */
        NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE,
        /* GPIO 22 ~ 27 */
        PD_0, PD_1, PD_2, PD_3, PA_0, PA_1
    };

    for (idx = 0; idx < GPIO_PINNUM; idx++) {
        if (pinmap_gpio_irq[idx] ==  NONE) {
            continue;
        } else if (pinmap_gpio_irq[idx] == (uint32_t)pin) {
            break;
        }
    }

    if (GPIO_PINNUM == idx) {
        error("The pin cannot generate interrupt");
    }
    return idx;
}

static void handle_interrupt_in(void)
{
    /* Read current interrupt register */
    uint32_t int_ctrl = GPIO_INT_CTRL_REG;
    uint32_t din_val  = GPIO_DATA_IN_REG;
    uint32_t idx;

    if (int_ctrl & (0x01UL << 16)) {
        GPIO_INT_CTRL_REG |= (0x01UL << 16);
        while (GPIO_INT_CTRL_REG & (0x01UL << 16));
    }
    for (idx = GPIO_IRQ_CH0; idx < CHANNEL_NUM; idx++) {
        if (int_ctrl & (0x01UL << (21 + idx))) {
            gpio_irq_event flagRiseFall_1, flagRiseFall_2;
            GPIO_INT_CTRL_REG |= (0x01UL << (17 + idx)); // clear int flag
            flagRiseFall_1 = (int_ctrl & (0x01UL << (2 + idx))) ? IRQ_RISE : IRQ_FALL;
            flagRiseFall_2 = (din_val & (0x01UL << channel_pinidxs[idx])) ? IRQ_RISE : IRQ_FALL;
            if (flagRiseFall_1 == flagRiseFall_2) {
                if (channel_bothedge_flag[idx]) {
                    GPIO_INT_CTRL_REG ^= (0x01UL << (2 + idx));
                }
                irq_handler[idx](channel_ids[idx], flagRiseFall_1);
#if defined(GPIO_IRQ_DEBUG)
                exception_cntr = 0;
#endif /* GPIO_IRQ_DEBUG */
            }
#if defined(GPIO_IRQ_DEBUG)
            else {
                exception_cntr++;
                if (exception_cntr >= 2) {
                    exception_cntr = 0;
                    mbed_error_printf("invalid gpio irq: %d,%d\r\n", (int)flagRiseFall_1, (int)flagRiseFall_2);
                }
            }
#endif /* GPIO_IRQ_DEBUG */
        }
    }
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    uint32_t regval;

    if (pin == NC) return -1;

    obj->ch = (uint16_t)gpio_irq_ava_chidx();
    MBED_ASSERT(CHANNEL_NUM != obj->ch);

    irq_handler[obj->ch] = handler;

    channel_ids[obj->ch] = id;
    channel_pinidxs[obj->ch] = gpio_irq_pinidx(pin);

    regval = RDA_GPIO->INTSEL & ~(0x3FFUL << 10);
    RDA_GPIO->INTSEL = regval |  (0x3FFUL << 10);

    NVIC_SetVector(GPIO_IRQn, (uint32_t)handle_interrupt_in);
    NVIC_SetPriority(GPIO_IRQn, 0x1FUL);
    NVIC_EnableIRQ(GPIO_IRQn);
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    channel_ids[obj->ch] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint32_t reg_val;
    uint16_t intEn;
    MBED_ASSERT(1 >= obj->ch);

    if (IRQ_RISE == event) {
        obj->flagR = (uint8_t)enable;
    } else {
        obj->flagF = (uint8_t)enable;
    }
    if (obj->flagR && obj->flagF)
        channel_bothedge_flag[obj->ch] = 1U;
    else
        channel_bothedge_flag[obj->ch] = 0U;
    if (obj->flagR || obj->flagF)
        intEn = 1;
    else
        intEn = 0;

    if (0 == intEn) {
        GPIO_INT_CTRL_REG &= ~(0x01UL << (6 + obj->ch));
    } else {
        /* Set interrupt select reg */
        reg_val = GPIO_INT_SEL_REG & ~(0x1FUL << (5 * obj->ch));
        GPIO_INT_SEL_REG = reg_val | ((0x1FUL & ((channel_pinidxs[obj->ch] >= 22) ? (channel_pinidxs[obj->ch] - 8) : channel_pinidxs[obj->ch])) << (5 * obj->ch));

        /* Set interrupt control reg */
        reg_val = GPIO_INT_CTRL_REG & ~(0x01UL << (2 + obj->ch));
        GPIO_INT_CTRL_REG = reg_val | (((0U == channel_bothedge_flag[obj->ch]) && (1U == obj->flagR)) ? (0x01UL << (2 + obj->ch)) : (0x00UL))
                                    | (0x01UL << (6 + obj->ch));
    }
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    NVIC_EnableIRQ(GPIO_IRQn);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    NVIC_DisableIRQ(GPIO_IRQn);
}

#endif /* DEVICE_INTERRUPTIN */
