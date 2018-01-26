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
#include "port_api.h"
#include "pinmap.h"
#include "gpio_api.h"
#include "PinNames.h"

#if CONFIG_GPIO_EN

#if DEVICE_PORTIN || DEVICE_PORTOUT

#define GPIO_PORT_NUM       3
#define GPIO_PORT_WIDTH     32
#define GPIO_PORT_WIDTH_MAX     32

const u8 Default_Port_PinDef[GPIO_PORT_NUM][GPIO_PORT_WIDTH+1] = {
    // Port 0 has these pin:
    {PA_0, PA_1, PB_3, PB_4,
     PB_6, PB_7, PC_1, PC_3,
     PC_4, PC_5, PC_6, PC_7,
     PC_8, PC_9, PD_1, PD_3,
     PD_4, PD_5, PD_6, PD_7,
     PD_9, PE_1, PE_2, PE_3,
     PE_5, PE_6, PE_7, PE_8,
     PG_3, PH_1, PH_3, PH_5,
     0xFF},

    // Port 1
    {PA_2, PA_3, PA_4, PA_5,
     PA_6, PA_7, PB_0, PB_1,
     PB_2, PB_5, PC_0, PC_2,
     PD_0, PD_2, PD_8, PE_0,
     PE_4, PE_9, PE_A, PF_0,
     PF_1, PF_2, PF_3, PF_4,
     PF_5, PG_0, PG_1, PG_2,
     PG_4, PG_5, PG_6, PG_7,
     0xFF},

    // Port 2
    {PH_0, PH_2, PH_4, PH_6,
     PH_7, PI_0, PI_1, PI_2,
     PI_3, PI_4, PI_5, PI_6,
     PI_7, PJ_0, PJ_1, PJ_2,
     PJ_3, PJ_4, PJ_5, PJ_6,
     PK_0, PK_1, PK_2, PK_3,
     PK_4, PK_5, PK_6, 
     0xFF}
     
};

extern const u8 GPIO_SWPORT_DR_TBL[];
extern const u8 GPIO_EXT_PORT_TBL[];

extern  VOID HAL_GPIO_Init(HAL_GPIO_PIN  *GPIO_Pin);
extern u32 HAL_GPIO_GetPinName(u32 chip_pin);

// high nibble = port number (0=A, 1=B, 2=C, 3=D, 4=E, 5=F, ...)
// low nibble  = pin number
PinName port_pin(PortName port, int pin_n)
{
    return (PinName)(pin_n + (port << 4));
}

void port_init(port_t *obj, PortName port, int mask, PinDirection dir) 
{
    u32 i;

    if (port >= GPIO_PORT_NUM) {
        DBG_GPIO_ERR("port_init: Invalid port num(%d), max port num is %d\r\n", \
            port, (GPIO_PORT_NUM-1));
    }
    
    // Fill PORT object structure for future use
    obj->port      = port;
    obj->mask      = mask;
    obj->direction = dir;

    if (obj->pin_def == NULL) {
        DBG_GPIO_ERR("Port Define Table isn't assigned\n");
        obj->pin_def = (uint8_t*)&Default_Port_PinDef[port][0];
    }

    i=0;
    while (obj->pin_def[i] != 0xff) {        
        i++;
        if (i == GPIO_PORT_WIDTH_MAX) {
            break;
        }
    }

    obj->mask &= ((1<<i) - 1);
    port_dir(obj, dir);
}

void port_dir(port_t *obj, PinDirection dir) 
{
    uint32_t i;
    HAL_GPIO_PIN GPIO_Pin;
    
    obj->direction = dir;
    for (i = 0; i < GPIO_PORT_WIDTH_MAX; i++) { // Process all pins
        if (obj->pin_def[i] == 0xff) {
            // end of table
            break;
        }
        if (obj->mask & (1 << i)) { // If the pin is used

            GPIO_Pin.pin_name = HAL_GPIO_GetPinName(obj->pin_def[i]); // get the IP pin name
            
            if (dir == PIN_OUTPUT) {
                GPIO_Pin.pin_mode = DOUT_PUSH_PULL;
            } else { // PIN_INPUT
                GPIO_Pin.pin_mode = DIN_PULL_NONE;
            }
            HAL_GPIO_Init(&GPIO_Pin);
        }
    }
}

void port_mode(port_t *obj, PinMode mode) 
{
    uint32_t i;
    
    for (i = 0; i < GPIO_PORT_WIDTH_MAX; i++) { // Process all pins
        if (obj->pin_def[i] == 0xff) {
            // end of table
            break;
        }
        if (obj->mask & (1 << i)) { // If the pin is used
            pin_mode(obj->pin_def[i], mode);
        }
    }
}

void port_write(port_t *obj, int value) 
{
    uint32_t i;
    uint32_t pin_name;
    uint8_t port_num;
    uint8_t pin_num;
    uint32_t hal_port[3];
    uint8_t port_changed[3];

    for (i=0;i<3;i++) {
        hal_port[i] =  HAL_READ32(GPIO_REG_BASE, GPIO_SWPORT_DR_TBL[i]);
        port_changed[i] = 0;
    }
    
    for (i = 0; i < GPIO_PORT_WIDTH_MAX; i++) { // Process all pins
        if (obj->pin_def[i] == 0xff) {
            // end of table
            break;
        }
        if (obj->mask & (1 << i)) { // If the pin is used
            pin_name = HAL_GPIO_GetPinName(obj->pin_def[i]); // get the IP pin name
            port_num = HAL_GPIO_GET_PORT_BY_NAME(pin_name);
            pin_num = HAL_GPIO_GET_PIN_BY_NAME(pin_name);
            hal_port[port_num] &= ~(1 << pin_num);
            hal_port[port_num] |= (((value>>i) & 0x01)<< pin_num);
            port_changed[port_num] = 1;
        }
    }

    for (i=0;i<3;i++) {
        if (port_changed[i]) {
            HAL_WRITE32(GPIO_REG_BASE, GPIO_SWPORT_DR_TBL[i], hal_port[i]);
        }
    }
    
}

int port_read(port_t *obj) 
{
    int value=0;
    u32 i;
    uint32_t pin_name;
    uint8_t port_num;
    uint8_t pin_num;
    uint32_t hal_port[3];

    for (i=0;i<3;i++) {
        hal_port[i] =  HAL_READ32(GPIO_REG_BASE, GPIO_EXT_PORT_TBL[i]);
    }

    for (i = 0; i < GPIO_PORT_WIDTH_MAX; i++) { // Process all pins
        if (obj->pin_def[i] == 0xff) {
            // end of table
            break;
        }
        if (obj->mask & (1 << i)) { // If the pin is used
            pin_name = HAL_GPIO_GetPinName(obj->pin_def[i]); // get the IP pin name
            port_num = HAL_GPIO_GET_PORT_BY_NAME(pin_name);
            pin_num = HAL_GPIO_GET_PIN_BY_NAME(pin_name);
            if (hal_port[port_num] & (1<<pin_num)) {
                value |= (1<<i);
            }
        }
    }

    return value;
}

#endif  //#if DEVICE_PORTIN || DEVICE_PORTOUT
#endif  //#if CONFIG_GPIO_EN
