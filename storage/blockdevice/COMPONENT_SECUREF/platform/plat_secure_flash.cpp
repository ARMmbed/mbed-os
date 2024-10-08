/*
 * Copyright (c) 2022-2023 Macronix International Co. LTD. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
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
#include <stdio.h>
#include <stdint.h>
#include "../include/plat_secure_flash.h"
#include "FlashIAPBlockDevice.h"

#define PROVISION_INFO_SIZE 0x1000

static inline uint32_t align_up(uint32_t val, uint32_t size)
{
    return (((val - 1) / size) + 1) * size;
}

int32_t plat_store_secure_flash_provision_info(uint8_t *buffer, int size)
{
#if COMPONENT_FLASHIAP
#if (MBED_CONF_FLASHIAP_BLOCK_DEVICE_SIZE == 0) && (MBED_CONF_FLASHIAP_BLOCK_DEVICE_BASE_ADDRESS == 0xFFFFFFFF)
    int status;
    uint32_t actual_size = 0;
    uint32_t bottom_addr, provision_start_addr, last_addr, erase_addr, program_addr;
        
    mbed::FlashIAP flash;
    status = flash.init();
    if (0 != status) {
        return -1;
    }    
    //Find the start of first sector after text area
    bottom_addr = align_up(FLASHIAP_APP_ROM_END_ADDR, flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR));
    last_addr = flash.get_flash_start() + flash.get_flash_size();
    status = flash.deinit();    
    if (0 != status) {
        return -1;
    }    
    FlashIAPBlockDevice iap_bd(bottom_addr, last_addr - bottom_addr);
#else
    FlashIAPBlockDevice iap_bd;    
#endif    
    provision_start_addr = last_addr - 1;
    while (actual_size < PROVISION_INFO_SIZE) {
        provision_start_addr -= flash.get_sector_size(provision_start_addr);
        actual_size += flash.get_sector_size(provision_start_addr);
    }
    if ((provision_start_addr + 1) < bottom_addr) {
        return -1;
    }    
    provision_start_addr = provision_start_addr - bottom_addr + 1;

    status = iap_bd.init();
    if (0 != status) {
        printf("Error : iap init failed.\r\n");
        return -1;
    }    
    erase_addr = provision_start_addr;
    while (erase_addr < (last_addr - bottom_addr)) {
        status = iap_bd.erase(erase_addr, iap_bd.get_erase_size(erase_addr));
        if (0 != status) {
            printf("Error : iap erase failed.\r\n");
            iap_bd.deinit();
            return -1;
        }
        erase_addr += iap_bd.get_erase_size(erase_addr);
    }    
    program_addr = provision_start_addr;
    while (size > 0) {
        status = iap_bd.program(&buffer[program_addr - provision_start_addr], program_addr, iap_bd.get_program_size());
        if (0 != status) {
            printf("Error : iap program failed.\r\n");
            iap_bd.deinit();
            return -1;
        }
        program_addr += iap_bd.get_program_size();
        size -= iap_bd.get_program_size();
    }    
    status = iap_bd.deinit();
    if (0 != status) {
        printf("Error : iap deinit failed.\r\n");
    }    
    return 0;
#endif
}

int32_t plat_get_secure_flash_provision_info(uint8_t *buffer, uint32_t size)
{
#if COMPONENT_FLASHIAP
#if (MBED_CONF_FLASHIAP_BLOCK_DEVICE_SIZE == 0) && (MBED_CONF_FLASHIAP_BLOCK_DEVICE_BASE_ADDRESS == 0xFFFFFFFF)
    int status;
    uint32_t actual_size = 0;
    uint32_t bottom_addr, provision_start_addr, last_addr, read_addr;

    mbed::FlashIAP flash;
    status = flash.init();
    if (0 != status) {
        return -1;
    }

    //Find the start of first sector after text area
    bottom_addr = align_up(FLASHIAP_APP_ROM_END_ADDR, flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR));
    last_addr = flash.get_flash_start() + flash.get_flash_size();
    status = flash.deinit();
    if (0 != status) {
        return -1;
    }
    FlashIAPBlockDevice iap_bd(bottom_addr, last_addr - bottom_addr);
#else
    FlashIAPBlockDevice iap_bd;
#endif

    provision_start_addr = last_addr - 1;
    while (actual_size < PROVISION_INFO_SIZE) {
        provision_start_addr -= flash.get_sector_size(provision_start_addr);
        actual_size += flash.get_sector_size(provision_start_addr);
    }
    if ((provision_start_addr + 1) < bottom_addr) {
        return -1;
    }
    provision_start_addr = provision_start_addr - bottom_addr + 1;

    status = iap_bd.init();
    if (0 != status) {
        printf("Error : iap init failed.\r\n");
        return -1;
    }

    read_addr = provision_start_addr;
    while (size > 0) {
        status = iap_bd.read(&buffer[read_addr - provision_start_addr], read_addr, iap_bd.get_read_size());
        if (0 != status) {
            printf("Error : iap read failed.\r\n");
            iap_bd.deinit();
            return -1;
        }
        read_addr += iap_bd.get_read_size();
        size -= iap_bd.get_read_size();
    }

    status = iap_bd.deinit();
    if (0 != status) {
        printf("Error : iap deinit failed.\r\n");
        return -1;
    }
    return 0;
#endif
}