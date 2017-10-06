/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#include "objects.h"
#include "pinmap.h"

#if CONFIG_GPIO_EN

#include "gpio_api.h"

// convert Mbed pin mode to HAL Pin Mode
const u8 GPIO_InPinMode[] = {
    DIN_PULL_NONE,      // PullNone
    DIN_PULL_HIGH,      // PullUp
    DIN_PULL_LOW,       // PullDown
    DIN_PULL_NONE       // OpenDrain
};

const u8 GPIO_SWPORT_DR_TBL[] = {
    GPIO_PORTA_DR,
    GPIO_PORTB_DR,
    GPIO_PORTC_DR
};

const u8 GPIO_EXT_PORT_TBL[] = {
    GPIO_EXT_PORTA,
    GPIO_EXT_PORTB,
    GPIO_EXT_PORTC
};

const u8 GPIO_SWPORT_DDR_TBL[] = {
    GPIO_PORTA_DDR,
    GPIO_PORTB_DDR,
    GPIO_PORTC_DDR
};

void gpio_set_hal_pin_mode(gpio_t *obj)
{
    uint32_t mode;

    mode = obj->mode;
    if (obj->direction == PIN_OUTPUT) {
        if (mode == OpenDrain) {
            obj->hal_pin.pin_mode = DOUT_OPEN_DRAIN;
        } else {
            obj->hal_pin.pin_mode = DOUT_PUSH_PULL;
        }
    } else {
        if (mode < 4) {
            obj->hal_pin.pin_mode = GPIO_InPinMode[mode];
        } else {
            obj->hal_pin.pin_mode = DIN_PULL_NONE;
        }
    }
}

uint32_t gpio_set(PinName pin) 
{
    u32 ip_pin;
    
    DBG_ASSERT(pin != (PinName)NC);
    pin_function(pin, 0);
    ip_pin = HAL_GPIO_GetPinName((u32)pin);
    
    return ip_pin;
}

void gpio_init(gpio_t *obj, PinName pin) 
{
    uint32_t pin_name;

    if (pin == (PinName)NC)
        return;

    obj->pin = pin;
    obj->mode = PullNone;
    obj->direction = PIN_INPUT;
    pin_name = gpio_set(pin); // get the IP pin name
    obj->hal_pin.pin_name = pin_name;
    obj->hal_pin.pin_mode = DIN_PULL_NONE;
    obj->hal_port_num = HAL_GPIO_GET_PORT_BY_NAME(pin_name);
    obj->hal_pin_num = HAL_GPIO_GET_PIN_BY_NAME(pin_name);
    HAL_GPIO_Init(&obj->hal_pin);
}

void gpio_mode(gpio_t *obj, PinMode mode) 
{
    obj->mode = mode;
    gpio_set_hal_pin_mode(obj);    
    HAL_GPIO_Init(&obj->hal_pin);
}

// Initial the Pin direction
void gpio_dir(gpio_t *obj, PinDirection direction)
{
    obj->direction = direction;
    gpio_set_hal_pin_mode(obj);
    HAL_GPIO_Init(&obj->hal_pin);
}

// Change the pin direction directly
void gpio_change_dir(gpio_t *obj, PinDirection direction)
{
    uint32_t reg_value;
    uint8_t port_num;
    uint8_t pin_num;

    obj->direction = direction;
    gpio_set_hal_pin_mode(obj);
    port_num = obj->hal_port_num;
    pin_num = obj->hal_pin_num;

    reg_value =  HAL_READ32(GPIO_REG_BASE, GPIO_SWPORT_DDR_TBL[port_num]);
    if (direction) {
        // Out
        reg_value |= (1 << pin_num);
    } else {
        // In
        reg_value &= ~(1 << pin_num);
    }
    HAL_WRITE32(GPIO_REG_BASE, GPIO_SWPORT_DDR_TBL[port_num], reg_value);
}

void gpio_write(gpio_t *obj, int value) 
{
    HAL_GPIO_PIN  *hal_pin=&obj->hal_pin;
    volatile uint32_t reg_value;
    uint8_t port_num;
    uint8_t pin_num;
    
    if (hal_pin->pin_mode != DOUT_OPEN_DRAIN) {
        port_num = obj->hal_port_num;
        pin_num = obj->hal_pin_num;
        
        reg_value =  HAL_READ32(GPIO_REG_BASE, GPIO_SWPORT_DR_TBL[port_num]);
        reg_value &= ~(1 << pin_num);
        reg_value |= ((value&0x01)<< pin_num);
        HAL_WRITE32(GPIO_REG_BASE, GPIO_SWPORT_DR_TBL[port_num], reg_value);            
    } else {
        HAL_GPIO_WritePin(&obj->hal_pin, value);
    }
}

int gpio_read(gpio_t *obj)
{
    volatile uint32_t reg_value;
    uint8_t port_num;
    uint8_t pin_num;
    HAL_GPIO_PIN_MODE pin_mode;

    port_num = obj->hal_port_num;
    pin_num = obj->hal_pin_num;
    pin_mode = obj->hal_pin.pin_mode;

    reg_value =  HAL_READ32(GPIO_REG_BASE, GPIO_EXT_PORT_TBL[port_num]);
    if (pin_mode != DOUT_OPEN_DRAIN) {
        return ((reg_value >> pin_num) & 0x01);
    } else {
        return (!((reg_value >> pin_num) & 0x01));
    }    
}

// This API only works for non-Open-Drain pin
void gpio_direct_write(gpio_t *obj, BOOL value) 
{
    uint8_t port_num;
    uint8_t pin_num;
    uint32_t reg_value;

    port_num = obj->hal_port_num;
    pin_num = obj->hal_pin_num;

    reg_value =  HAL_READ32(GPIO_REG_BASE, GPIO_SWPORT_DR_TBL[port_num]);
    reg_value &= ~(1 << pin_num);
    reg_value |= (value<< pin_num);
    HAL_WRITE32(GPIO_REG_BASE, GPIO_SWPORT_DR_TBL[port_num], reg_value);
}

void gpio_pull_ctrl(gpio_t *obj, PinMode pull_type)
{
    HAL_GPIO_PullCtrl((u32) obj->pin, (u32)pull_type);
}


void gpio_deinit(gpio_t *obj)
{
    HAL_GPIO_DeInit(&obj->hal_pin);
}

#endif
