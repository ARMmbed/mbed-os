/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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
#include "cmsis.h"
#include "mbed_toolchain.h"

/* RAM vector_table needs to be aligned with the size of the vector table */
/* TODO: Use MXC_IRQ_COUNT to automatically set this alignment per DUI0553A 4.3.4 */
/*       Vector Table Offset which requires the next-power-of-two alignment. This */
/*       can be calculated by 4*pow(2,ceil(log2(MXC_IRQ_COUNT))) */
MBED_ALIGN(512)
void (*ramVectorTable[MXC_IRQ_COUNT])(void);
