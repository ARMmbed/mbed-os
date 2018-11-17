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
#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#define NVIC_FLASH_VECTOR_ADDRESS 	0x00000000
#define NVIC_USER_IRQ_NUMBER 		166
#define NVIC_NUM_VECTORS            (16 + 166)   // CORE + MCU Peripherals
#define NVIC_RAM_VECTOR_ADDRESS     0x20000000    // Location of vectors in RAM

#endif
