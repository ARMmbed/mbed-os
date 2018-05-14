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

/*
 * Code implementation file for the fpga functions.
 */

#include "SMM_MPS2.h"                   // MPS2 common header

// Function to delay n*ticks (25MHz = 40nS per tick)
// Used for I2C drivers
void i2c_delay(unsigned int tick);

/* Sleep function to delay n*mS
 * Uses FPGA counter.
 */
void Sleepms(unsigned int msec);

/* Sleep function to delay n*uS
 */
void Sleepus(unsigned int usec);
