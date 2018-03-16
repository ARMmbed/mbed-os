/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef __MBED_GPIO_OBJECTS_H__
#define __MBED_GPIO_OBJECTS_H__

/*
 * GPIO device number, there are 16 pins per GPIO
 * equivalent: pin / 16
 */
#define GPIO_DEV_NUMBER(pin)  ((pin) >> 4)
/*
 * Pin number of this pin inside its GPIO
 * equivalent: pin % 16
 */
#define GPIO_PIN_NUMBER(pin)  ((pin) & 0xF)

/* Number of the GPIO device */
#define GPIO0_NUMBER          0
#define GPIO1_NUMBER          1
#define GPIO2_NUMBER          2
#define GPIO3_NUMBER          3

/* Base EXP pin number for the corresponding GPIO */
#define EXP_PIN_BASE0         EXP0
#define EXP_PIN_BASE1         EXP16
#define EXP_PIN_BASE2         EXP32
#define EXP_PIN_BASE3         EXP48

#define GPIO_DEVICES          4
#define PINS_PER_GPIO         16
/* Pins 4 to 15 of GPIO3 are not used */
#define PINS_NOT_USED         12
#define PINS_NUMBER           (GPIO_DEVICES * PINS_PER_GPIO - PINS_NOT_USED)

/* GPIO3 port only uses first 4 pins */
#define GPIO3_PIN_NUMBER  4

/* When doing a port access, the pin number argument is useless */
#define ARG_NOT_USED          0

#endif /* __MBED_GPIO_OBJECTS_H__ */

