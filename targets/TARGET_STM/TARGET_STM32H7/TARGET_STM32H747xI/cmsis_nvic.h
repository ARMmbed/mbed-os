/*
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018-2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
*/

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

// CORE: 16 vectors = 64 bytes from 0x00 to 0x3F
// MCU Peripherals: 150 vectors = 600 bytes from 0x40 to 0x297
// Total: 166 vectors = 664 bytes (0x298) to be reserved in RAM
#define NVIC_NUM_VECTORS      166

#ifdef CORE_CM7
#define NVIC_RAM_VECTOR_ADDRESS 0x20000000    // Vectors positioned at start of DTCM RAM
#else
#define NVIC_RAM_VECTOR_ADDRESS 0x10000000    // Vectors positioned at start of D2 RAM (CM4)
#endif

#endif
