/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#ifndef MBED_CMSIS_H
#define MBED_CMSIS_H

#include <stdint.h>

void NVIC_SystemReset ();

void __enable_irq ();

void __disable_irq ();

int __get_PRIMASK ();

void __CLREX ();

uint8_t __LDREXB (volatile uint8_t *ptr);

uint16_t __LDREXH (volatile uint16_t *ptr);

uint32_t __LDREXW (volatile uint32_t *ptr);

uint32_t __get_IPSR	(void);

uint32_t __STREXB (uint8_t value, volatile uint8_t *addr);

uint32_t __STREXH (uint16_t value, volatile uint16_t *addr);

uint32_t __STREXW (uint32_t value, volatile uint32_t *addr);

#endif
