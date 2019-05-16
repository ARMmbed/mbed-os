/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
 * SPDX-License-Identifier: Apache-2.0
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
#include "gpio_irq_api.h"
#include "mbed_critical.h"
#include "mbed_error.h"
#include "txz_gpio.h"
#include "PeripheralNames.h"
#include "pinmap.h"

#define CHANNEL_NUM                 (16)
#define DISABLE                     (0)
#define ENABLE                      (1)
#define CLR_INT_FLAG                (0xC0)

const PinMap PinMap_GPIO_IRQ[] = {
    {PK7, GPIO_IRQ_0, PIN_DATA(0, 0)},
    {PL0, GPIO_IRQ_1, PIN_DATA(0, 0)},
    {PA0, GPIO_IRQ_2, PIN_DATA(0, 0)},
    {PA7, GPIO_IRQ_3, PIN_DATA(0, 0)},
    {PB0, GPIO_IRQ_4, PIN_DATA(0, 0)},
    {PB1, GPIO_IRQ_5, PIN_DATA(0, 0)},
    {PB6, GPIO_IRQ_6, PIN_DATA(0, 0)},
    {PB7, GPIO_IRQ_7, PIN_DATA(0, 0)},
    {PG0, GPIO_IRQ_8, PIN_DATA(0, 0)},
    {PG1, GPIO_IRQ_9, PIN_DATA(0, 0)},
    {PK0, GPIO_IRQ_A, PIN_DATA(0, 0)},
    {PK1, GPIO_IRQ_B, PIN_DATA(0, 0)},
    {PC0, GPIO_IRQ_C, PIN_DATA(0, 0)},
    {PC1, GPIO_IRQ_D, PIN_DATA(0, 0)},
    {PC6, GPIO_IRQ_E, PIN_DATA(0, 0)},
    {PC7, GPIO_IRQ_F, PIN_DATA(0, 0)},
    {NC,  NC,         0}
};

extern _gpio_t gpio_port_add;

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler hal_irq_handler[CHANNEL_NUM] = {NULL};
static CG_INTActiveState CurrentState;

static void CG_SetSTBYReleaseINTSrc(CG_INTSrc, CG_INTActiveState, uint8_t);
static void INT_IRQHandler(PinName, uint32_t);

void INT00_IRQHandler(void)
{
    INT_IRQHandler(PK7, 0);
}

void INT01_IRQHandler(void)
{
    INT_IRQHandler(PL0, 1);
}

void INT02_IRQHandler(void)
{
    INT_IRQHandler(PA0, 2);
}

void INT03_IRQHandler(void)
{
    INT_IRQHandler(PA7, 3);
}

void INT04_IRQHandler(void)
{
    INT_IRQHandler(PB0, 4);
}

void INT05_IRQHandler(void)
{
    INT_IRQHandler(PB1, 5);
}

void INT06_IRQHandler(void)
{
    INT_IRQHandler(PB6, 6);
}

void INT07_IRQHandler(void)
{
    INT_IRQHandler(PB7, 7);
}

void INT08_IRQHandler(void)
{
    INT_IRQHandler(PG0, 8);
}

void INT09_IRQHandler(void)
{
    INT_IRQHandler(PG1, 9);
}

void INT10_IRQHandler(void)
{
    INT_IRQHandler(PK0, 10);
}

void INT11_IRQHandler(void)
{
    INT_IRQHandler(PK1, 11);
}

void INT12_IRQHandler(void)
{
    INT_IRQHandler(PC0, 12);
}
void INT13_IRQHandler(void)
{
    INT_IRQHandler(PC1, 13);
}
void INT14_IRQHandler(void)
{
    INT_IRQHandler(PC6, 14);
}
void INT15_IRQHandler(void)
{
    INT_IRQHandler(PC7, 15);
}

int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    // Get gpio interrupt ID
    obj->irq_id = pinmap_peripheral(pin, PinMap_GPIO_IRQ);
    core_util_critical_section_enter();
    // Get GPIO port and pin num
    obj->port = (PortName)PIN_PORT(pin);
    obj->pin_num = PIN_POS(pin);
    // Set pin level as LOW
    gpio_write_bit(&gpio_port_add, obj->port, obj->pin_num, GPIO_Mode_DATA, 0);
    // Enable gpio interrupt function
    pinmap_pinout(pin, PinMap_GPIO_IRQ);
    // Get GPIO irq source
    obj->irq_src = (CG_INTSrc)obj->irq_id;
    // Save irq handler
    hal_irq_handler[obj->irq_src] = handler;
    // Save irq id
    channel_ids[obj->irq_src] = id;
    // Initialize interrupt event as both edges detection
    obj->event = CG_INT_ACTIVE_STATE_BOTH_EDGES;
    // Clear gpio pending interrupt
    NVIC_ClearPendingIRQ((IRQn_Type)obj->irq_id);
    // Set interrupt event and enable INTx clear
    CG_SetSTBYReleaseINTSrc(obj->irq_src, (CG_INTActiveState)obj->event, ENABLE);
    core_util_critical_section_exit();;
    return 0;
}

void gpio_irq_free(gpio_irq_t *obj)
{
    // Clear gpio_irq
    NVIC_ClearPendingIRQ((IRQn_Type)obj->irq_id);
    // Reset interrupt handler
    hal_irq_handler[obj->irq_src] = NULL;
    // Reset interrupt id
    channel_ids[obj->irq_src] = 0;
}

void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    //Disable GPIO interrupt on obj
    gpio_irq_disable(obj);
    if (enable) {
        // Get gpio interrupt event
        if (event == IRQ_RISE) {
            if ((obj->event == CG_INT_ACTIVE_STATE_FALLING) ||
                    (obj->event == CG_INT_ACTIVE_STATE_BOTH_EDGES)) {
                obj->event = CG_INT_ACTIVE_STATE_BOTH_EDGES;
            } else {
                obj->event = CG_INT_ACTIVE_STATE_RISING;
            }
        } else if (event == IRQ_FALL) {
            if ((obj->event == CG_INT_ACTIVE_STATE_RISING) ||
                    (obj->event == CG_INT_ACTIVE_STATE_BOTH_EDGES)) {
                obj->event = CG_INT_ACTIVE_STATE_BOTH_EDGES;
            } else {
                obj->event = CG_INT_ACTIVE_STATE_FALLING;
            }
        } else {
            error("Not supported event\n");
        }
    } else {
        // Get gpio interrupt event
        if (event == IRQ_RISE) {
            if ((obj->event == CG_INT_ACTIVE_STATE_RISING) ||
                    (obj->event == CG_INT_ACTIVE_STATE_INVALID)) {
                obj->event = CG_INT_ACTIVE_STATE_BOTH_EDGES;
            } else {
                obj->event = CG_INT_ACTIVE_STATE_FALLING;
            }
        } else if (event == IRQ_FALL) {
            if ((obj->event == CG_INT_ACTIVE_STATE_FALLING) ||
                    (obj->event == CG_INT_ACTIVE_STATE_INVALID)) {
                obj->event = CG_INT_ACTIVE_STATE_BOTH_EDGES;
            } else {
                obj->event = CG_INT_ACTIVE_STATE_RISING;
            }
        } else {
            error("Not supported event\n");
        }
    }
    CurrentState = obj->event;
    if (obj->event != CG_INT_ACTIVE_STATE_INVALID ) {
        // Set interrupt event and enable INTx clear
        CG_SetSTBYReleaseINTSrc(obj->irq_src, (CG_INTActiveState)obj->event, ENABLE);
        gpio_write_bit(&gpio_port_add, (gpio_gr_t)obj->port, (gpio_num_t)obj->pin_num, GPIO_Mode_DATA, 0);
    } else {
        gpio_write_bit(&gpio_port_add, (gpio_gr_t)obj->port, (gpio_num_t)obj->pin_num, GPIO_Mode_DATA, 1);
    }
    // Clear interrupt request
    NVIC_ClearPendingIRQ((IRQn_Type)obj->irq_id);
    // Enable GPIO interrupt on obj
    gpio_irq_enable(obj);
}

void gpio_irq_enable(gpio_irq_t *obj)
{
    // Clear and Enable gpio_irq object
    NVIC_ClearPendingIRQ((IRQn_Type)obj->irq_id);
    NVIC_EnableIRQ((IRQn_Type)obj->irq_id);
}

void gpio_irq_disable(gpio_irq_t *obj)
{
    // Disable gpio_irq object
    NVIC_DisableIRQ((IRQn_Type)obj->irq_id);
}

static void INT_IRQHandler(PinName pin, uint32_t index)
{
    PortName port;
    gpio_pinstate_t data = GPIO_PIN_RESET;
    uint8_t pin_num;

    pin_num = PIN_POS(pin);
    port = (PortName)PIN_PORT(pin);
    // Clear interrupt request
    CG_SetSTBYReleaseINTSrc((CG_INTSrc)(CG_INT_SRC_0 + index), CurrentState, DISABLE);
    // Get pin value
    gpio_read_bit(&gpio_port_add, (gpio_gr_t)port, (gpio_num_t)pin_num, GPIO_Mode_DATA, &data);
    switch (data) {
        // Falling edge detection
        case 0:
            hal_irq_handler[index](channel_ids[index], IRQ_FALL);
            break;
        // Rising edge detection
        case 1:
            hal_irq_handler[index](channel_ids[index], IRQ_RISE);
            break;
        default:
            break;
    }
    // Clear gpio pending interrupt
    NVIC_ClearPendingIRQ((IRQn_Type)(CG_INT_SRC_0 + index));
    // Enable interrupt request
    CG_SetSTBYReleaseINTSrc((CG_INTSrc)(CG_INT_SRC_0 + index), CurrentState, ENABLE);
}

static void CG_SetSTBYReleaseINTSrc(CG_INTSrc INTSource, CG_INTActiveState ActiveState, uint8_t NewState)
{
    uint8_t *ptr = ((uint8_t *)(&(TSB_IA->IMC00)) + (INTSource * 2));
    //clear pending falling and rising edge bit
    *ptr = CLR_INT_FLAG;
    *ptr = (ActiveState | NewState);
    {
        uint8_t regval = *ptr;
    }
}
