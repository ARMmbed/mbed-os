/*
 * Copyright (c) 2006-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if DEVICE_STORAGE_SDCARD

#include "SDCardBlock.h"

#ifndef DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_MOSI
    #ifdef TARGET_K64F
    	#define MOSI PTE3
    #else
    	#error "Please define the MOSI GPIO connecting from the MCU to the SDCard"
    #endif
#else
	#define MOSI DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_MOSI
#endif

#ifndef DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_MISO
    #ifdef TARGET_K64F
    	#define MISO PTE1
    #else
    	#error "Please define the MISO GPIO connecting from the MCU to the SDCard"
    #endif
#else
	#define MISO DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_MISO
#endif

#ifndef DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_SCK
    #ifdef TARGET_K64F
    	#define SCK  PTE2
    #else
    	#error "Please define the serial-clock GPIO connecting from the MCU to the SDCard"
    #endif
#else
	#define SCK  DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_SCK
#endif

#ifndef DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_CS
    #ifdef TARGET_K64F
	   #define CS   PTE4
    #else
    	#error "Please define the chip-select GPIO connecting from the MCU to the SDCard"
    #endif
#else
	#define CS   DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_CS
#endif

SDCardBlock _sd(MOSI, MISO, SCK, CS); // MOSI, MISO, SCK, CS
SDCardInterface &sd = _sd;

#endif /* #if DEVICE_STORAGE_SDCARD */
