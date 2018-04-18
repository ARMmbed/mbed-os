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
#ifndef __MBED_OBJECTS_H__
#define __MBED_OBJECTS_H__

#include "cmsis.h"
#include "PortNames.h"
#include "PeripheralNames.h"
#include "PinNames.h"
#include "platform_devices.h"
#include "gpio_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gpio_s {
    struct arm_gpio_dev_t *gpio_dev;
    struct arm_mps2_io_dev_t *mps2_io_dev;
    void (*arm_mps2_io_write)(struct arm_mps2_io_dev_t* dev,
                            enum arm_mps2_io_access_t access,
                            uint8_t pin_num,
                            uint32_t value);
    uint32_t pin_number;
    PinDirection direction;
} gpio_t;

struct gpio_irq_s {
    struct arm_gpio_dev_t *gpio_dev;
    uint32_t pin_number;             /* Pin number inside the GPIO */
    uint32_t exp_pin_number;         /* Pin number on the expension port */
    IRQn_Type irq_number;            /* IRQ number of the GPIO interrupt of
                                        this pin */
};

struct port_s {
    struct arm_gpio_dev_t *gpio_dev;
};

struct serial_s {
    struct arm_uart_dev_t *uart;
    UARTName index;
    IRQn_Type irq_number;               /* IRQ number of the RX interrupt for
                                            this UART device */
};

struct i2c_s {
    MPS2_I2C_TypeDef *i2c;
    uint32_t freq_us;       /* Stores I2C frequency in microseconds */
};

struct spi_s {
    struct spi_pl022_dev_t *spi;
};

struct analogin_s {
    uint16_t ctrl_register;  /* Control bits with the channel identifier */
};

/* This structure is not used by the HAL implementation. */
struct flash_s {
    uint8_t not_used;
};

/* This TRNG structure is not used by the HAL implementation. */
struct trng_s {
    uint8_t not_used;
};

#ifdef __cplusplus
}
#endif

#endif /* __MBED_OBJECTS_H__ */
