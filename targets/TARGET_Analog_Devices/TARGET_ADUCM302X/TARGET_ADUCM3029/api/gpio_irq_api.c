/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "gpio_irq_api.h"
#include "adi_gpio.h"
#include "adi_gpio_def.h"
#include "ADuCM302x_device.h"

#ifdef DEVICE_INTERRUPTIN

#define MAX_GPIO_LINES    16
#define MAX_GPIO_PORTS    ADI_GPIO_NUM_PORTS

typedef struct {
    unsigned int id;
    gpio_irq_event event;
    uint8_t int_enable;
} gpio_chan_info_t;

extern uint8_t gpioMemory[ADI_GPIO_MEMORY_SIZE];
extern uint8_t gpio_initialized;
static gpio_chan_info_t channel_ids[MAX_GPIO_PORTS][MAX_GPIO_LINES];
static gpio_irq_handler irq_handler = NULL;


/** Local interrupt callback routine.
 */
static void gpio_irq_callback(void *pCBParam, uint32_t Event, void *pArg)
{
    uint16_t pin = *(ADI_GPIO_DATA*)pArg;
    int index = 0;

    // determine the index of the pin that caused the interrupt
    while (pin) {
        if (pin & 0x01) {
            // call the user ISR. The argument Event is the port number of the GPIO line.
            if (irq_handler != NULL)
                irq_handler((uint32_t)channel_ids[Event][index].id, channel_ids[Event][index].event);
        }
        index++;
        pin >>= 1;
    }
}


/** Function to get the IENA and IENB register values.
 *  Added here temporarily until these are available in the BSP
 */
static ADI_GPIO_RESULT adi_gpio_GetGroupInterruptPins(const ADI_GPIO_PORT Port, const IRQn_Type eIrq,
        const ADI_GPIO_DATA Pins, uint16_t* const pValue)
{
    ADI_GPIO_TypeDef 	*pReg[MAX_GPIO_PORTS] = {pADI_GPIO0, pADI_GPIO1, pADI_GPIO2};
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */
    uint16_t            Value = 0u;

    pPort = pReg[Port];

    switch (eIrq) {
        case SYS_GPIO_INTA_IRQn:
            Value = pPort->IENA;
            break;
        case SYS_GPIO_INTB_IRQn:
            Value = pPort->IENB;
            break;
        default:
            break; /* This shall never reach */
    }

    *pValue = (Value & Pins);
    return (ADI_GPIO_SUCCESS);
}


/** Function to get the interrupt polarity register content.
 *  Added here temporarily until these are available in the BSP
 */
static ADI_GPIO_RESULT adi_gpio_GetGroupInterruptPolarity(const ADI_GPIO_PORT Port, const ADI_GPIO_DATA Pins,
        uint16_t* const pValue)
{
    ADI_GPIO_TypeDef    *pPort;     /* pointer to port registers */
    ADI_GPIO_TypeDef 	*pReg[MAX_GPIO_PORTS] = {pADI_GPIO0, pADI_GPIO1, pADI_GPIO2};

    pPort = pReg[Port];

    *pValue = (pPort->POL & Pins);

    return (ADI_GPIO_SUCCESS);
}


/** Function to clear the relevant interrupt enable bits in both the IENA and IENB registers
 *  for the given GPIO pin.
 */
static void disable_pin_interrupt(ADI_GPIO_PORT port, uint32_t pin_number)
{
    uint16_t int_reg_val;

    // Read the current content of the IENA register
    adi_gpio_GetGroupInterruptPins(port, SYS_GPIO_INTA_IRQn, 1 << pin_number, &int_reg_val);

    // clear the bit for the pin
    int_reg_val &= ~(1 << pin_number);

    // write the interrupt register
    adi_gpio_SetGroupInterruptPins(port, SYS_GPIO_INTA_IRQn, int_reg_val);

    // Do the same to IENB
    adi_gpio_GetGroupInterruptPins(port, SYS_GPIO_INTB_IRQn, 1 << pin_number, &int_reg_val);

    // clear the bit for the pin
    int_reg_val &= ~(1 << pin_number);

    // write the interrupt register
    adi_gpio_SetGroupInterruptPins(port, SYS_GPIO_INTB_IRQn, int_reg_val);
}


/** Function to set the relevant interrupt enable bits in either the IENA and IENB registers
 *  for the given GPIO pin.
 */
static void enable_pin_interrupt(ADI_GPIO_PORT port, uint32_t pin_number, IRQn_Type eIrq)
{
    uint16_t int_reg_val;

    // Read the current interrupt enable register content
    adi_gpio_GetGroupInterruptPins(port, eIrq, 1 << pin_number, &int_reg_val);

    // set the bit for the pin
    int_reg_val |= (1 << pin_number);

    // write the interrupt register
    adi_gpio_SetGroupInterruptPins(port, eIrq, int_reg_val);
}


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
    uint32_t port = pin >> GPIO_PORT_SHIFT;
    uint32_t pin_num = pin & 0xFF;

    // check for valid pin and ID
    if ((pin == NC) || (id == 0)) {
        return -1;
    }

    // make sure gpio driver has been initialized
    if (!gpio_initialized) {
        adi_gpio_Init(gpioMemory,ADI_GPIO_MEMORY_SIZE);
        gpio_initialized = 1;
    }

    // save the handler
    if (handler) {
        irq_handler = handler;
    }

    // disable the interrupt for the given pin
    disable_pin_interrupt((ADI_GPIO_PORT)port, pin_num);

    // set the port pin as input
    adi_gpio_InputEnable(port, 1 << pin_num, true);

    // save the ID for future reference
    channel_ids[port][pin_num].id = (uint32_t)id;
    channel_ids[port][pin_num].event = IRQ_NONE;
    channel_ids[port][pin_num].int_enable = 0;
    obj->id = id;
    obj->pinname = pin;

    return 0;
}

/** Release the GPIO IRQ PIN
 *
 * @param obj The gpio object
 */
void gpio_irq_free(gpio_irq_t *obj)
{
    uint32_t port = obj->pinname >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pinname & 0xFF;

    // disable interrupt for the given pin
    gpio_irq_disable(obj);

    // clear the status table
    channel_ids[port][pin_num].id = (uint32_t)0;
    channel_ids[port][pin_num].event = IRQ_NONE;
    channel_ids[port][pin_num].int_enable = 0;
}

/** Enable/disable pin IRQ event
 *
 * @param obj    The GPIO object
 * @param event  The GPIO IRQ event
 * @param enable The enable flag
 */
void gpio_irq_set(gpio_irq_t *obj, gpio_irq_event event, uint32_t enable)
{
    uint16_t int_polarity_reg;
    uint32_t port = obj->pinname >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pinname & 0xFF;

    if (event == IRQ_NONE) {
        return;
    }

    // read the current polarity register
    adi_gpio_GetGroupInterruptPolarity((ADI_GPIO_PORT)port,	1 << pin_num, &int_polarity_reg);

    if (event == IRQ_RISE) {
        int_polarity_reg |= (1 << pin_num);
    } else {
        int_polarity_reg &= ~(1 << pin_num);
    }

    // set the polarity register
    adi_gpio_SetGroupInterruptPolarity((ADI_GPIO_PORT)port, int_polarity_reg);

    channel_ids[port][pin_num].event = event;

    // enable interrupt for this pin if enable flag is set
    if (enable) {
        gpio_irq_enable(obj);
    } else {
        gpio_irq_disable(obj);
    }
}

/** Enable GPIO IRQ
 *
 * This is target dependent, as it might enable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_enable(gpio_irq_t *obj)
{
    uint32_t port = obj->pinname >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pinname & 0xFF;

    if (channel_ids[port][pin_num].event == IRQ_NONE) {
        return;
    }

    // Group all RISE interrupts in INTA and FALL interrupts in INTB
    if (channel_ids[port][pin_num].event == IRQ_RISE) {
        // set the callback routine
        adi_gpio_RegisterCallback(SYS_GPIO_INTA_IRQn, gpio_irq_callback, obj);
        enable_pin_interrupt((ADI_GPIO_PORT)port, pin_num, SYS_GPIO_INTA_IRQn);
    } else if (channel_ids[port][pin_num].event == IRQ_FALL) {
        // set the callback routine
        adi_gpio_RegisterCallback(SYS_GPIO_INTB_IRQn, gpio_irq_callback, obj);
        enable_pin_interrupt((ADI_GPIO_PORT)port, pin_num, SYS_GPIO_INTB_IRQn);
    }

    channel_ids[port][pin_num].int_enable = 1;
}

/** Disable GPIO IRQ
 *
 * This is target dependent, as it might disable the entire port or just a pin
 * @param obj The GPIO object
 */
void gpio_irq_disable(gpio_irq_t *obj)
{
    uint32_t port = obj->pinname >> GPIO_PORT_SHIFT;
    uint32_t pin_num = obj->pinname & 0xFF;

    if (channel_ids[port][pin_num].event == IRQ_NONE) {
        return;
    }

    // Group all RISE interrupts in INTA and FALL interrupts in INTB
    if (channel_ids[port][pin_num].event == IRQ_RISE) {
        disable_pin_interrupt((ADI_GPIO_PORT)port, pin_num);
    }
    else if (channel_ids[port][pin_num].event == IRQ_FALL) {
        disable_pin_interrupt((ADI_GPIO_PORT)port, pin_num);
    }

    channel_ids[port][pin_num].int_enable = 0;
}

#endif 	// #ifdef DEVICE_INTERRUPTIN
