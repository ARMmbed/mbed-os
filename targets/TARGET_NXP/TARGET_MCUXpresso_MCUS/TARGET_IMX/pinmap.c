/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "mbed_assert.h"
#include "PeripheralPins.h"
#include "mbed_error.h"
#include "fsl_clock.h"

/* Array of IOMUX base address. */
static uint32_t iomux_base_addrs[FSL_FEATURE_SOC_IGPIO_COUNT] = { 0x401F80BC, 0x401F813C, 0u, 0x401F8014, 0x400A8000 };

/* Get the IOMUX register address from the GPIO3 pin number */
static uint32_t get_iomux_reg_base(PinName pin)
{
    int32_t gpio_pin = pin & 0xFF;
    uint32_t retval = 0;

    if ((gpio_pin >= 0) && (gpio_pin < 12)) {
        retval = 0x401F81D4 + (gpio_pin * 4);
    } else if ((gpio_pin >= 12) && (gpio_pin < 18)) {
        retval = 0x401F81BC + ((gpio_pin - 12) * 4);
    } else if ((gpio_pin >= 18) && (gpio_pin < 28)) {
        retval = 0x401F8094 + ((gpio_pin - 18) * 4);
    }

    return retval;
}

void pin_function(PinName pin, int function)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t muxregister = iomux_base_addrs[(pin >> GPIO_PORT_SHIFT) - 1];
    uint32_t daisyregister;

    CLOCK_EnableClock(kCLOCK_Iomuxc);

    /* Get mux register address */
    if (muxregister == 0) {
        muxregister = get_iomux_reg_base(pin);
    } else {
        muxregister = muxregister + ((pin & 0xFF) * 4);
    }

    /* Write to the mux register */
    *((volatile uint32_t *)muxregister) = IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(function);

    /* If required write to the input daisy register */
    daisyregister = (function >> DAISY_REG_SHIFT) & 0xFFF;
    if (daisyregister != 0) {
        daisyregister = daisyregister + 0x401F8000;
        *((volatile uint32_t *)daisyregister) = IOMUXC_SELECT_INPUT_DAISY(((function >> DAISY_REG_VALUE_SHIFT) & 0xF));
    }
}

void pin_mode(PinName pin, PinMode mode)
{
    MBED_ASSERT(pin != (PinName)NC);
    uint32_t instance = pin >> GPIO_PORT_SHIFT;
    uint32_t reg;
    uint32_t muxregister = iomux_base_addrs[instance - 1];
    uint32_t configregister;

    if (muxregister == 0) {
        muxregister = get_iomux_reg_base(pin);
    } else {
        muxregister = muxregister + ((pin & 0xFF) * 4);
    }

    /* Get pad register address */
    if (instance == 5) {
        configregister = muxregister + 0x10;
    } else {
        configregister = muxregister + 0x1F0;
    }

    reg = *((volatile uint32_t *)configregister);
    switch (mode) {
        case PullNone:
            /* Write 0 to the PUE bit & 1 to the PKE bit to set the pad to keeper mode */
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
            reg |= (IOMUXC_SW_PAD_CTL_PAD_PKE_MASK);
            break;
        case PullDown:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 0 to PUS bit for 100K pull down */
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= (IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
            break;
        case PullUp_47K:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 1 to PUS bit for 47K pull up*/
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUS(1);
            reg |= (IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
            break;
        case PullUp_100K:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 2 to PUS bit for 100K pull up*/
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUS(2);
            reg |= (IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
            break;
        case PullUp_22K:
            /* Write 1 to PKE & PUE bit to enable the pull configuration and 3 to PUS bit for 22K pull up*/
            reg &= ~(IOMUXC_SW_PAD_CTL_PAD_PUS_MASK);
            reg |= IOMUXC_SW_PAD_CTL_PAD_PUS(3);
            reg |= (IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK);
            break;
        default:
            break;
    }

    /* Below settings for DSE and SPEED fields per test results */
    reg = (reg & ~(IOMUXC_SW_PAD_CTL_PAD_DSE_MASK | IOMUXC_SW_PAD_CTL_PAD_SPEED_MASK)) |
           IOMUXC_SW_PAD_CTL_PAD_DSE(6) | IOMUXC_SW_PAD_CTL_PAD_SPEED(2);

    /* Write value to the pad register */
    *((volatile uint32_t *)configregister) = reg;
}

void pin_mode_opendrain(PinName pin, bool enable)
{
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t instance = pin >> GPIO_PORT_SHIFT;
    uint32_t muxregister = iomux_base_addrs[instance - 1];
    uint32_t configregister;

    if (muxregister == 0) {
        muxregister = get_iomux_reg_base(pin);
    } else {
        muxregister = muxregister + ((pin & 0xFF) * 4);
    }

    /* Get pad register address */
    if (instance == 5) {
        configregister = muxregister + 0x10;
    } else {
        configregister = muxregister + 0x1F0;
    }

    if (enable) {
        *((volatile uint32_t *)configregister) |= IOMUXC_SW_PAD_CTL_PAD_ODE_MASK;
    } else {
        *((volatile uint32_t *)configregister) &= ~IOMUXC_SW_PAD_CTL_PAD_ODE_MASK;
    }
}

