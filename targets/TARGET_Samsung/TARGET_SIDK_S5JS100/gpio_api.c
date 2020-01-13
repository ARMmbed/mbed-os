/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "gpio_api.h"
#include "pinmap.h"
#include "gpio.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
// BIT MASK
#define MASK_1_BIT  (0x00000001)
#define MASK_2_BIT  (0x00000003)
#define MASK_3_BIT  (0x00000007)
#define MASK_4_BIT  (0x0000000F)
#define MASK_5_BIT  (0x0000001F)
#define MASK_6_BIT  (0x0000003F)
#define MASK_7_BIT  (0x0000007F)

#define SetBits(uAddr, uBaseBit, uMaskValue, uSetValue) \
    putreg32((getreg32(uAddr) & ~((uMaskValue)<<(uBaseBit))) | (((uMaskValue)&(uSetValue))<<(uBaseBit)), uAddr)
#define GetBits(uAddr, uBaseBit, uMaskValue) \
    ((getreg32(uAddr)>>(uBaseBit))&(uMaskValue))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
int gpio_pin_mode(PinName pin, PinMode mode);
uint32_t get_tmux_addr(enum gpio_id_ id)
{
    uint32_t i;
    uint32_t offset;
    const struct tmux_range_ tmux_range[] = {
        //   addr  start  end
        {0x03C, 24, 72},
        {0x100, 0, 23},
    };

    offset = 0;

    for (i = 0; i < ARRAY_SIZE(tmux_range); i++) {
        if ((id >= tmux_range[i].start_num) && (id <= tmux_range[i].end_num)) {
            offset = tmux_range[i].addr + ((id - tmux_range[i].start_num) * 4);
            break;
        }
    }

    return (TOP_MUX_BASE + offset);
}

void gpio_set_input_enable(enum gpio_id_ id, enum in_enable_ inen)
{
    uint32_t addr;

    addr = get_tmux_addr(id);
    SetBits(addr, BITPOS_OE, MASK_1_BIT, OUT_DIS);
    SetBits(addr, BITPOS_IE, MASK_1_BIT, inen);
}

void gpio_set_func(enum gpio_id_ id, enum nmux_func_ func)
{
    uint32_t addr;

    addr = get_tmux_addr(id);
    SetBits(addr, BITPOS_FUNC, MASK_3_BIT, func);
}

void gpio_set_pupd(enum gpio_id_ id, enum pull_up_down_ pupd)
{
    uint32_t addr;

    addr = get_tmux_addr(id);
    if (pupd == GPIO_PUPD_DIS) {
        SetBits(addr, BITPOS_PUPD + 1, MASK_1_BIT, pupd);
    } else {
        SetBits(addr, BITPOS_PUPD, MASK_2_BIT, pupd);
    }
}

void gpio_set_ds(enum gpio_id_ id, enum drive_strength_ ds)
{
    uint32_t addr;

    addr = get_tmux_addr(id);
    SetBits(addr, BITPOS_DS, MASK_2_BIT, ds);
}

void s5js100_setdrv(uint32_t cfgset)
{
    uint32_t ds;
    uint32_t pin;
    /* CAVEAT: GPIO_FAST|SLOWXXX is compatible with GPIO_DRV_XXX */
    ds = (cfgset & GPIO_DRVSTR_MASK) >> GPIO_DRVSTR_SHIFT;
    pin = (cfgset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
    gpio_set_ds(pin, ds);
}

void s5js100_pullup(uint32_t cfgset)
{
    uint32_t pin;
    uint32_t pupd;

    /* CAVEAT: GPIO_PUPD_XXX is compatible with GPIO_PUD_XXX */
    pupd = (cfgset & GPIO_PUPD_MASK) >> GPIO_PUPD_SHIFT;
    pin = (cfgset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
    gpio_set_pupd(pin, pupd);
}

void s5js100_setintedge(unsigned int pin, int edge)
{
    uint32_t group;
    uint32_t addr;
    uint32_t bitpos;

    group = pin >> 5;           //group = 0, 1,
    addr = group << 2;          // addr = 0x0, 0x4,
    bitpos = pin & 0x1F;

    switch (edge) {
        case LEVEL_LOW:
            putreg32(getreg32(GPIO_INT_METHOD_REG + addr) | (1 << bitpos), GPIO_INT_METHOD_REG + addr);
            putreg32(getreg32(GPIO_INT_POL_REG + addr) & ~(1 << bitpos), GPIO_INT_POL_REG + addr);
            break;
        case LEVEL_HIGH:
            putreg32(getreg32(GPIO_INT_METHOD_REG + addr) | (1 << bitpos), GPIO_INT_METHOD_REG + addr);
            putreg32(getreg32(GPIO_INT_POL_REG + addr) | (1 << bitpos), GPIO_INT_POL_REG + addr);
            break;
        case NEG_EDGE:
            putreg32(getreg32(GPIO_INT_METHOD_REG + addr) & ~(1 << bitpos), GPIO_INT_METHOD_REG + addr);
            putreg32(getreg32(GPIO_INT_BOTH_REG + addr) & ~(1 << bitpos), GPIO_INT_BOTH_REG + addr);
            putreg32(getreg32(GPIO_INT_POL_REG + addr) & ~(1 << bitpos), GPIO_INT_POL_REG + addr);
            //gpio_set_pupd(pin, GPIO_PULL_UP);
            break;
        case POS_EDGE:
            putreg32(getreg32(GPIO_INT_METHOD_REG + addr) & ~(1 << bitpos), GPIO_INT_METHOD_REG + addr);
            putreg32(getreg32(GPIO_INT_BOTH_REG + addr) & ~(1 << bitpos), GPIO_INT_BOTH_REG + addr);
            putreg32(getreg32(GPIO_INT_POL_REG + addr) | (1 << bitpos), GPIO_INT_POL_REG + addr);
            //gpio_set_pupd(pin, GPIO_PULL_DOWN);
            break;
        case BOTH_EDGE:
            putreg32(getreg32(GPIO_INT_METHOD_REG + addr) & ~(1 << bitpos), GPIO_INT_METHOD_REG + addr);
            putreg32(getreg32(GPIO_INT_BOTH_REG + addr) | (1 << bitpos), GPIO_INT_BOTH_REG + addr);
            break;
        default:
            break;
    }
}

void s5js100_setintmask(unsigned int pin, int mask)
{
    uint32_t group;
    uint32_t addr;
    uint32_t bitpos;
    group = pin >> 5;           //group = 0, 1,
    addr = group << 2;          // addr = 0x0, 0x4,
    bitpos = pin & 0x1F;

    if (mask) {
        putreg32(getreg32(GPIO_INT_MASK_REG + addr) | (0 << bitpos), GPIO_INT_MASK_REG + addr); // int disable
    } else {
        putreg32(getreg32(GPIO_INT_MASK_REG + addr) | (1 << bitpos), GPIO_INT_MASK_REG + addr); // int enable
    }
    /* workaround, eint first is set pended if counter part has GPIO output */
    putreg32(getreg32(GPIO_INT_PEND_CLR_REG + addr) | (1 << bitpos), GPIO_INT_PEND_CLR_REG + addr); // pending clear
}

uint32_t s5js100_get_intpin(unsigned int ch)
{
    uint32_t i;
    uint32_t pend = getreg32(GPIO_INT_PEND_CLR_REG + (ch * 0x4));
    for (i = 0; i < 32; i++) {
        if (pend & (0x1 << i)) {
            return i + ch * 32;
        }
    }

    return -1;
}


int s5js100_configinput(uint32_t cfgset)
{
    uint32_t pin;
    uint32_t addr, bitpos;

    pin = (cfgset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;

    gpio_set_func(pin, GPIO_GPIO);

    /* Set as input */
    addr = GPIO_BASE + ((pin >> 5) << 2);
    bitpos = pin & 0x1F;
    putreg32(getreg32(addr) | (1 << bitpos), addr);
    gpio_set_input_enable(pin, IN_EN);

    /* Set pull-up/down */
    s5js100_pullup(cfgset);

    return 0;
}

int s5js100_configalt(uint32_t cfgset)
{
    uint32_t pin;
    uint32_t func;

    pin = (cfgset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
    func = (cfgset & GPIO_FUNC_MASK) >> GPIO_FUNC_SHIFT;
    gpio_set_func(pin, func);

    /* Set pull-up mode */
    s5js100_pullup(cfgset);

    /* Set drive strength */
    s5js100_setdrv(cfgset);

    return 0;
}

int s5js100_configinterrupt(uint32_t cfgset, uint32_t nirq)
{
    uint32_t pin;

    pin = (cfgset & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;

    if (cfgset & GPIO_EINT) {
        /*
         * First, configure the port as a generic input so that we have
         * a known starting point and consistent behavior during the
         * re-configuration.
         */
        s5js100_configinput(cfgset);

        /* Then, just remember the rising/falling edge interrupt enabled */
        s5js100_setintedge(pin, (cfgset & GPIO_EINT_MASK) >> GPIO_EINT_SHIFT);
        s5js100_setintmask(pin, 0);

        /* workaround gpio eint clear int in NVIC side */
        ///SetBits(0xE000E280, nirq - S5JS100_IRQ_FIRST, MASK_1_BIT, 0x1);
    } else {
        s5js100_setintmask(pin, 1);
    }

    return 0;
}


int s5js100_configgpio(uint32_t cfgset)
{
    int ret = -1;

    ret = s5js100_configalt(cfgset);

    s5js100_configinterrupt(cfgset, (cfgset & GPIO_IRQ_MASK) >> GPIO_IRQ_SHIFT);

    return ret;
}

void s5js100_gpio_clear_pending(uint32_t pincfg)
{
    uint32_t pin;
    uint32_t group;
    uint32_t addr;
    uint32_t bitpos;

    pin = (pincfg & GPIO_PIN_MASK) >> GPIO_PIN_SHIFT;
    group = pin >> 5;           //group = 0, 1,
    addr = group << 2;          // addr = 0x0, 0x4,
    bitpos = pin & 0x1F;

    putreg32(getreg32(GPIO_INT_PEND_CLR_REG + addr) | (1 << bitpos), GPIO_INT_PEND_CLR_REG + addr); // pending clear

}

uint32_t gpio_set(PinName pin)
{
    int bit;

    bit = pin % 32;
    return (1 << bit);
}

void gpio_init(gpio_t *obj, PinName pin)
{
    int idx;
    int bit;

    if (pin == NC || pin > P72) {
        obj->reg_out = NULL;
        obj->pin = NC;
        return;
    }

    idx = pin / 32;
    bit = pin % 32;

    obj->pin = pin;
    obj->mask = (1 << bit);
    obj->reg_out = (unsigned int *)(GPIO_OUT_REG + (idx * 4));
    obj->reg_dir = (unsigned int *)(GPIO_CON_REG + (idx * 4));
    obj->reg_in = (unsigned int *)(GPIO_IN_REG + (idx * 4));
    if (pin >= P00 && pin <= P72) {
        // Set GPIO pin set as GENERAL GPIO
        s5js100_configgpio(GPIO_DEFAULT_CONFIG | pin << GPIO_PIN_SHIFT);
    }

}

void gpio_mode(gpio_t *obj, PinMode mode)
{
    enum gpio_id_ id = (enum gpio_id_)obj->pin;
    if (obj->reg_out == NULL) {
        return;
    }
    //pin_mode(obj->pin, mode);
    if (mode == PullUp) {
        gpio_pin_mode(obj->pin, mode);
        gpio_set_pupd(id, GPIO_PULL_UP);
    }
    if (mode == PullDown) {
        gpio_pin_mode(obj->pin, mode);
        gpio_set_pupd(id, GPIO_PULL_DOWN);
    }
}

void gpio_dir(gpio_t *obj, PinDirection direction)
{
    if (obj->reg_out == NULL) {
        return;
    }
    if (direction == PIN_INPUT) {
        *(obj->reg_dir) |= obj->mask;
    } else if (direction == PIN_OUTPUT) {
        *(obj->reg_dir) &= ~(obj->mask);
    }
}

int gpio_is_connected(const gpio_t *obj)
{
    if (obj->pin != (PinName)NC) {
        return 1;
    } else {
        return 0;
    }
}
