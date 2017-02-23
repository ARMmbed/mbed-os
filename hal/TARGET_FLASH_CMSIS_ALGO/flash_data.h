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

// Target flash algorithm structure
typedef struct {
    const uint32_t init;
    const uint32_t uninit;
    const uint32_t erase_sector;
    const uint32_t program_page;
    const uint32_t static_base;
    uint32_t *algo_blob;
} flash_algo_t;

typedef struct {
    const uint32_t start;
    const uint32_t size;
} sector_info_t;

typedef struct {
    const uint32_t page_size;
    const uint32_t flash_start;
    const uint32_t flash_size;
    const sector_info_t *sectors;
    const uint32_t sector_info_count;
} flash_target_config_t;

// Target flash configuration
struct flash_s {
    const flash_target_config_t *target_config;
    const flash_algo_t *flash_algo;
};

typedef struct {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r9;
    uint32_t pc;
} args_t;

typedef int32_t (*flash_algo_jump_t)(args_t*);

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
