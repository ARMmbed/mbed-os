/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "gpio_irq_api.h"
#include "objects.h"

#if DEVICE_INTERRUPTIN

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t ap3_gpio_enable_interrupts(uint32_t ui32Pin, am_hal_gpio_intdir_e eIntDir);
/** GPIO IRQ HAL structure. gpio_irq_s is declared in the target's HAL
*/
typedef struct gpio_irq_s gpio_irq_t;

typedef void (*gpio_irq_handler)(uint32_t id, gpio_irq_event event);
extern void am_gpio_isr(void);
static ap3_gpio_irq_control_t gpio_irq_control[AP3_GPIO_MAX_PADS];

/**
* \defgroup hal_gpioirq GPIO IRQ HAL functions
*
* # Defined behavior
* * ::gpio_irq_init initializes the GPIO IRQ pin
* * ::gpio_irq_init attaches the interrupt handler
* * ::gpio_irq_free releases the GPIO IRQ pin
* * ::gpio_irq_set enables/disables pin IRQ event
* * ::gpio_irq_enable enables GPIO IRQ
* * ::gpio_irq_disable disables GPIO IRQ
*
* # Undefined behavior
* * Calling other function before ::gpio_irq_init
*
* @{
*/

/** Initialize the GPIO IRQ pin
*
* @param obj     The GPIO object to initialize
* @param pin     The GPIO pin name
* @param handler The handler to be attached to GPIO IRQ
* @param id      The object ID (id != 0, 0 is reserved)
* @return -1 if pin is NC, 0 otherwise
*/
int gpio_irq_init(gpio_irq_t *obj, PinName pin, gpio_irq_handler handler, uint32_t id)
{
    //grab the correct irq control object
    ap3_gpio_irq_control_t *control = &gpio_irq_control[pin];

    //Register locally
    control->pad = pin;
    control->handler = handler;
    control->id = id;
    control->events = IRQ_NONE;

    //Attach to object
    obj->control = control;

    //Make sure the interrupt is set to none to reflect the new events value
    ap3_gpio_enable_interrupts(control->pad, AM_HAL_GPIO_PIN_INTDIR_NONE);

    //Enable GPIO IRQ's in the NVIC
    gpio_irq_enable(obj);
    NVIC_SetVector((IRQn_Type)GPIO_IRQn, (uint32_t)am_gpio_isr);
    NVIC_EnableIRQ((IRQn_Type)GPIO_IRQn);
    return 0;
}

void am_gpio_isr(void)
{
    //call the handler for the interrupt
    uint64_t gpio_int_mask = 0x00;
    am_hal_gpio_interrupt_status_get(true, &gpio_int_mask);
    uint32_t pinNum = 0;
    while (gpio_int_mask) {
        if (gpio_int_mask & 0x0000000000000001) {
            am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(pinNum));
            ap3_gpio_irq_control_t irq_ctrl = gpio_irq_control[pinNum];
            ((gpio_irq_handler)irq_ctrl.handler)(irq_ctrl.id, irq_ctrl.events);
        }
        gpio_int_mask >>= 1;
        pinNum++;
    }
}

/** Release the GPIO IRQ PIN
*
* @param obj The gpio object
*/
void gpio_irq_free(gpio_irq_t *obj)
{
}

/** Enable/disable pin IRQ event
*
* @param obj    The GPIO object
* @param event  The GPIO IRQ event
* @param enable The enable flag
*/
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    //Clear state
    obj->control->events &= (~event);
    if (enable) {
        //Reset if enabled
        obj->control->events |= event;
    }

    // Map enabled events to a value the reflects the ambiq hal/register values
    am_hal_gpio_intdir_e ap3_int_dir = 0x00;
    switch (obj->control->events) {
        case IRQ_NONE:
            ap3_int_dir = AM_HAL_GPIO_PIN_INTDIR_NONE;
            break;
        case IRQ_RISE:
            ap3_int_dir = AM_HAL_GPIO_PIN_INTDIR_LO2HI;
            break;
        case IRQ_FALL:
            ap3_int_dir = AM_HAL_GPIO_PIN_INTDIR_HI2LO;
            break;
        case (IRQ_RISE | IRQ_FALL):
            ap3_int_dir = AM_HAL_GPIO_PIN_INTDIR_BOTH;
            break;
    }

    ap3_gpio_enable_interrupts(obj->control->pad, ap3_int_dir);
}

/** Enable GPIO IRQ
*
* This is target dependent, as it might enable the entire port or just a pin
* @param obj The GPIO object
*/
void gpio_irq_enable(gpio_irq_t *obj)
{
    am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(obj->control->pad));
    am_hal_gpio_interrupt_enable(AM_HAL_GPIO_BIT(obj->control->pad));
}

/** Disable GPIO IRQ
*
* This is target dependent, as it might disable the entire port or just a pin
* @param obj The GPIO object
*/
void gpio_irq_disable(gpio_irq_t *obj)
{
    am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(obj->control->pad));
    am_hal_gpio_interrupt_disable(AM_HAL_GPIO_BIT(obj->control->pad));
}

/**@}*/
uint32_t ap3_gpio_enable_interrupts(uint32_t ui32Pin, am_hal_gpio_intdir_e eIntDir)
{
    uint32_t ui32Padreg, ui32AltPadCfg, ui32GPCfg;
    bool bClearEnable = false;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (ui32Pin >= AM_HAL_GPIO_MAX_PADS) {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Initialize the PADREG accumulator variables.
    //
    ui32GPCfg = ui32Padreg = ui32AltPadCfg = 0;

    //
    // Map the requested interrupt direction settings into the Apollo3
    //  GPIOCFG register field, which is a 4-bit field:
    //  [INTD(1):OUTCFG(2):INCFG(1)].
    // Bit0 of eIntDir maps to GPIOCFG.INTD  (b3).
    // Bit1 of eIntDir maps to GPIOCFG.INCFG (b0).
    //
    ui32GPCfg |= (((eIntDir >> 0) & 0x1) << GPIOCFG_FLD_INTD_S) | (((eIntDir >> 1) & 0x1) << GPIOCFG_FLD_INCFG_S);

    //
    // At this point, the configuration variable, ui32GpioCfg
    // value is set (at bit position 0) and ready to write
    //  to their respective register bitfields.
    //
    uint32_t ui32GPCfgAddr;
    uint32_t ui32GPCfgClearMask;
    uint32_t ui32GPCfgShft;

    ui32GPCfgShft = ((ui32Pin & 0x7) << 2);

    ui32GPCfgAddr = AM_REGADDR(GPIO, CFGA) + ((ui32Pin >> 1) & ~0x3);

    ui32GPCfgClearMask = ~((uint32_t)0xF << ui32GPCfgShft);

    //
    // Get the new values into their rightful bit positions.
    //
    ui32GPCfg <<= ui32GPCfgShft;

    AM_CRITICAL_BEGIN

    if (bClearEnable) {
        //
        // We're configuring a mode that requires clearing the Enable bit.
        //
        am_hal_gpio_output_tristate_disable(ui32Pin);
    }

    GPIO->PADKEY = GPIO_PADKEY_PADKEY_Key;

    // Here's where the magic happens
    AM_REGVAL(ui32GPCfgAddr) = (AM_REGVAL(ui32GPCfgAddr) & ui32GPCfgClearMask) | ui32GPCfg;

    GPIO->PADKEY = 0;

    AM_CRITICAL_END

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_gpio_pinconfig()

#ifdef __cplusplus
}
#endif
/** @}*/
#endif //DEVICE_INTERRUPTIN
