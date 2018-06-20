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
#ifndef MBED_FLASH_DATA_H
#define MBED_FLASH_DATA_H

#include <stdint.h>

/** Target flash algorithm structure
 */
typedef struct {
    const uint32_t init; /**< Init function address */
    const uint32_t uninit; /**< Uninit function address */
    const uint32_t erase_sector; /**< Erase sector function address */
    const uint32_t program_page; /**< Program page function address */
    const uint32_t static_base; /**< Static base address */
    uint32_t *algo_blob; /**< Pointer to flash algo binary blob */
} flash_algo_t;

/** Sector information structure
 */
typedef struct {
    const uint32_t start; /**< Sector start address */
    const uint32_t size; /**< Sector size */
} sector_info_t;

/** Flash configuration structure
 */
typedef struct {
    const uint32_t page_size; /**< The minimum program page size that can be written */
    const uint32_t flash_start; /**< Start address of the flash <0, flash_size) */
    const uint32_t flash_size; /**<  Flash size. The size is accumulative sum of all sector sizes */
    const sector_info_t *sectors; /**<  List of sectors - sector can vary in sizes */
    const uint32_t sector_info_count; /**< Number of sectors */
} flash_target_config_t;

/** Target flash configuration
 *  For targets not supporting TrustZone, its flash_set_target_config must define target_config.
 *  For targets supporting TrustZone, it has the following requirements:
 *  -# Flash IAP H/W can only configure to secure. It can access both secure/non-secure flash.
 *  -# Flash IAP port is for secure build only. It exports secure functions for non-secure build.
 *  -# In Flash IAP port, flash_set_target_config must define both target_config/target_config_ns for secure/non-secure flash respectively.
 *  -# Non-secure application can access its non-secure flash only through secure flash IAP functions. It cannot access secure flash.
 */
struct flash_s {
    const flash_target_config_t *target_config; /**< Normal/secure flash configuration structure for targets not supporting/supporting TrustZone */
#if defined(__CORTEX_M23) || defined(__CORTEX_M33)
    const flash_target_config_t *target_config_ns; /**< Non-secure flash configuration structure for targets supporting TrustZone */
#endif
    const flash_algo_t *flash_algo;
};

/** Flash algo argument structure
 *  Contains all registers that should be preserved
 */
typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r9;
    uint32_t pc;
} args_t;

typedef int32_t (*flash_algo_jump_t)(args_t *);

// prototypes for flash algo CMSIS API

typedef int (*CMSIS_Algo_Function_Init)(unsigned long adr, unsigned long clk, unsigned long fnc);
typedef int (*CMSIS_Algo_Function_UnInit)(unsigned long fnc);
typedef int (*CMSIS_Algo_Function_EraseSector)(unsigned long adr);
typedef int (*CMSIS_Algo_Function_EraseChip)(void);
typedef int (*CMSIS_Algo_Function_ProgramPage)(unsigned long adr, unsigned long sz, unsigned char *buf);
typedef unsigned long (*CMSIS_Algo_Function_Verify)(unsigned long adr, unsigned long sz, unsigned char *buf);

#ifdef __cplusplus
extern "C" {
#endif

/* Set target configuration
 */
void flash_set_target_config(flash_t *obj);

#ifdef __cplusplus
};
#endif


#endif
