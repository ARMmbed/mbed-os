/* mbed Microcontroller Library
 * (C)Copyright TOSHIBA ELECTRONIC DEVICES & STORAGE CORPORATION 2018 All rights reserved
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
#include "gpio_irq_api.h"
#include "mbed_error.h"
#include "PeripheralNames.h"
#include "pinmap.h"
#include "gpio_include.h"
#include "mbed_critical.h"

const PinMap PinMap_GPIO_IRQ[] = {
    {PC0, GPIO_IRQ_0, PIN_DATA(0, 0)},
    {PC1, GPIO_IRQ_1, PIN_DATA(0, 0)},
    {PC2, GPIO_IRQ_2, PIN_DATA(0, 0)},
    {PB1, GPIO_IRQ_3, PIN_DATA(0, 0)},
    {PJ4, GPIO_IRQ_4, PIN_DATA(0, 0)},
    {PK1, GPIO_IRQ_5, PIN_DATA(0, 0)},
    {PH3, GPIO_IRQ_6, PIN_DATA(0, 0)},
    {PA6, GPIO_IRQ_7, PIN_DATA(0, 0)},
    {PL3, GPIO_IRQ_8, PIN_DATA(0, 0)},
    {PM2, GPIO_IRQ_9, PIN_DATA(0, 0)},
    {PN3, GPIO_IRQ_A, PIN_DATA(0, 0)},
    {PA7, GPIO_IRQ_B, PIN_DATA(0, 0)},
    {PL4, GPIO_IRQ_C, PIN_DATA(0, 0)},
    {PK7, GPIO_IRQ_D, PIN_DATA(0, 0)},
    {PP3, GPIO_IRQ_E, PIN_DATA(0, 0)},
    {PM6, GPIO_IRQ_F, PIN_DATA(0, 0)},
    {NC,  NC,         0}
};

static uint32_t channel_ids[CHANNEL_NUM] = {0};
static gpio_irq_handler hal_irq_handler[CHANNEL_NUM] = {NULL};
static void SetSTBYReleaseINTSrc(cg_intsrc, cg_intactivestate, FunctionalState);
static cg_intactivestate CurrentState;
static void INT_IRQHandler(PinName pin, uint32_t index);

// Initialize gpio IRQ pin
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    uint8_t  bit       = 0;
    uint32_t port_base = 0;

    // Get gpio interrupt ID
    obj->irq_id = pinmap_peripheral(pin, PinMap_GPIO_IRQ);

    // Disable interrupt by CPU
    core_util_critical_section_enter();

    // Calculate port and pin position
    obj->port = (PortName)PIN_PORT(pin);
    obj->pin  = pin;
    bit       = PIN_POS(pin);

    port_base = BITBAND_PORT_BASE(obj->port);
    port_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_DATA);
    BITBAND_PORT_CLR(port_base, bit);
    // Enable gpio interrupt function
    pinmap_pinout(pin, PinMap_GPIO_IRQ);

    // Get GPIO irq source
    switch (obj->irq_id) {
        case GPIO_IRQ_0:
            obj->irq_src = cg_int_src_0;
            break;
        case GPIO_IRQ_1:
            obj->irq_src = cg_int_src_1;
            break;
        case GPIO_IRQ_2:
            obj->irq_src = cg_int_src_2;
            break;
        case GPIO_IRQ_3:
            obj->irq_src = cg_int_src_3;
            break;
        case GPIO_IRQ_4:
            obj->irq_src = cg_int_src_4;
            break;
        case GPIO_IRQ_5:
            obj->irq_src = cg_int_src_5;
            break;
        case GPIO_IRQ_6:
            obj->irq_src = cg_int_src_6;
            break;
        case GPIO_IRQ_7:
            obj->irq_src = cg_int_src_7;
            break;
        case GPIO_IRQ_8:
            obj->irq_src = cg_int_src_8;
            break;
        case GPIO_IRQ_9:
            obj->irq_src = cg_int_src_9;
            break;
        case GPIO_IRQ_A:
            obj->irq_src = cg_int_src_a;
            break;
        case GPIO_IRQ_B:
            obj->irq_src = cg_int_src_b;
            break;
        case GPIO_IRQ_C:
            obj->irq_src = cg_int_src_c;
            break;
        case GPIO_IRQ_D:
            obj->irq_src = cg_int_src_d;
            break;
        case GPIO_IRQ_E:
            obj->irq_src = cg_int_src_e;
            break;
        case GPIO_IRQ_F:
            obj->irq_src = cg_int_src_f;
            break;
        default:
            break;
    }

    // Save irq handler
    hal_irq_handler[obj->irq_src] = handler;

    // Save irq id
    channel_ids[obj->irq_src] = id;

    // Initialize interrupt event as both edges detection
    obj->event = cg_int_active_state_both_edges;
    CurrentState = cg_int_active_state_both_edges;
    // Set interrupt event and enable INTx clear
    SetSTBYReleaseINTSrc(obj->irq_src, (cg_intactivestate)obj->event, ENABLE);

    // Clear gpio pending interrupt
    NVIC_ClearPendingIRQ((IRQn_Type) obj->irq_id);

    core_util_critical_section_exit();

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

// Set interrupt event of gpio_irq object
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint8_t  bit       = 0;
    uint32_t port_base = 0;

    //Disable GPIO interrupt on obj
    gpio_irq_disable(obj);
    if (enable) {
        // Get gpio interrupt event
        if (event == IRQ_RISE) {
            if ((obj->event == cg_int_active_state_falling) || (obj->event == cg_int_active_state_both_edges)) {
                obj->event = cg_int_active_state_both_edges;
            } else {
                obj->event = cg_int_active_state_rising;
            }
        } else if (event == IRQ_FALL) {
            if ((obj->event == cg_int_active_state_rising) || (obj->event == cg_int_active_state_both_edges)) {
                obj->event = cg_int_active_state_both_edges;
            } else {
                obj->event = cg_int_active_state_falling;
            }
        } else {
            error("Not supported event\n");
        }
    } else {
        // Get gpio interrupt event
        if (event == IRQ_RISE) {
            if ((obj->event == cg_int_active_state_rising) || (obj->event == cg_int_active_state_invalid)) {
                obj->event = cg_int_active_state_invalid;
            } else {
                obj->event = cg_int_active_state_falling;
            }
        } else if (event == IRQ_FALL) {
            if ((obj->event == cg_int_active_state_falling) || (obj->event == cg_int_active_state_invalid)) {
                obj->event = cg_int_active_state_invalid;
            } else {
                obj->event = cg_int_active_state_rising;
            }
        } else {
            error("Not supported event\n");
        }
    }
    CurrentState = obj->event;
    // Calculate port and pin position
    bit  = PIN_POS(obj->pin);

    port_base = BITBAND_PORT_BASE(obj->port);
    port_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_DATA);

    if (obj->event != cg_int_active_state_invalid ) {
        // Set interrupt event and enable INTx clear
        SetSTBYReleaseINTSrc(obj->irq_src, (cg_intactivestate) obj->event, ENABLE);
        BITBAND_PORT_CLR(port_base, bit);
    } else {
        BITBAND_PORT_SET(port_base, bit);
    }
    //Enable GPIO interrupt on obj
    gpio_irq_enable(obj);
}

// Enable gpio_irq object
void gpio_irq_enable(gpio_irq_t *obj)
{
    // Clear and Enable gpio_irq object
    NVIC_ClearPendingIRQ((IRQn_Type)obj->irq_id);
    NVIC_EnableIRQ((IRQn_Type)obj->irq_id);
}

// Disable gpio_irq object
void gpio_irq_disable(gpio_irq_t *obj)
{
    // Disable gpio_irq object
    NVIC_DisableIRQ((IRQn_Type)obj->irq_id);
}

static void INT_IRQHandler(PinName pin, uint32_t index)
{
    int      port      = 0;
    uint8_t  bit       = 0;
    uint32_t data      = 0;
    uint32_t port_base = 0;

    // Calculate port and pin position
    port = PIN_PORT(pin);
    bit  = PIN_POS(pin);

    // Clear interrupt request
    SetSTBYReleaseINTSrc((cg_intsrc)(cg_int_src_0 + index), CurrentState, DISABLE);

    port_base = BITBAND_PORT_BASE(port);
    port_base = BITBAND_PORT_MODE_BASE(port_base, GPIO_Mode_DATA);
    BITBAND_PORT_READ(data, port_base, bit);

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
    NVIC_ClearPendingIRQ((IRQn_Type)(cg_int_src_0 + index));

    // Enable interrupt request
    SetSTBYReleaseINTSrc((cg_intsrc)(cg_int_src_0 + index), CurrentState, ENABLE);
}

void INT00_IRQHandler(void)
{
    INT_IRQHandler(PC0, 0);
}

void INT01_IRQHandler(void)
{
    INT_IRQHandler(PC1, 1);
}

void INT02_IRQHandler(void)
{
    INT_IRQHandler(PC2, 2);
}

void INT03_IRQHandler(void)
{
    INT_IRQHandler(PB1, 3);
}

void INT04_IRQHandler(void)
{
    INT_IRQHandler(PJ4, 4);
}

void INT05_IRQHandler(void)
{
    INT_IRQHandler(PK1, 5);
}

void INT06_IRQHandler(void)
{
    INT_IRQHandler(PH3, 6);
}

void INT07_IRQHandler(void)
{
    INT_IRQHandler(PA6, 7);
}

void INT08_IRQHandler(void)
{
    INT_IRQHandler(PL3, 8);
}

void INT09_IRQHandler(void)
{
    INT_IRQHandler(PM2, 9);
}

void INT10_IRQHandler(void)
{
    INT_IRQHandler(PN3, 10);
}

void INT11_IRQHandler(void)
{
    INT_IRQHandler(PA7, 11);
}

void INT12_IRQHandler(void)
{
    INT_IRQHandler(PL4, 12);
}

void INT13_IRQHandler(void)
{
    INT_IRQHandler(PK7, 13);
}

void INT14_IRQHandler(void)
{
    INT_IRQHandler(PP3, 14);
}

void INT15_IRQHandler(void)
{
    INT_IRQHandler(PM6, 15);
}

static void SetSTBYReleaseINTSrc(cg_intsrc intsource, cg_intactivestate ActiveState, FunctionalState NewState)
{
    __IO uint8_t *p_imc;

    if (intsource < 3U) {
        p_imc  = (__IO uint8_t *)(&TSB_IA->IMC00 + (intsource));
        *p_imc = (uint8_t)(0xC0 | ActiveState | NewState);
    } else {
        intsource -= 3;
        p_imc  = (__IO uint8_t *)(&TSB_IB->IMC033 + (intsource));
        *p_imc = (uint8_t)(0xC0 | ActiveState | NewState);
    }
    // Dummy read is need
    {
        __IO uint8_t imc = *p_imc;
    }
}
