/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2016, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#if DEVICE_STORAGE

#include "Driver_Storage.h"
#include "cmsis_nvic.h"
#include "storage_driver_info.h"

#include <string.h>

#define FLASH_TIMEOUT_VALUE (50000) /* 50 seconds timeout */

extern FLASH_ProcessTypeDef pFlash;

static const ARM_DRIVER_VERSION version = {
    .api = ARM_STORAGE_API_VERSION,
    .drv = ARM_DRIVER_VERSION_MAJOR_MINOR(1,0)
};

static const ARM_STORAGE_CAPABILITIES capabilities = {
    .asynchronous_ops = 1,
    .erase_all        = 0,
};

static const ARM_STORAGE_INFO informations = {
    .total_storage        = STORAGE_TOTAL_SIZE,
    .program_unit         = 1,
    .optimal_program_unit = 4, /* word (4 bytes) programming */
    .program_cycles       = ARM_STORAGE_PROGRAM_CYCLES_INFINITE,
    .erased_value         = 1,
    .memory_mapped        = 1,
    .programmability      = ARM_STORAGE_PROGRAMMABILITY_ERASABLE,
    .retention_level      = ARM_RETENTION_NVM,
    .security             = {
        .acls                 = 0,
        .rollback_protection  = 0,
        .tamper_proof         = 0,
        .internal_flash       = 1,

        .software_attacks     = 0,
        .board_level_attacks  = 0,
        .chip_level_attacks   = 0,
        .side_channel_attacks = 0,
    }
};

/*
 * Static configuration.
 */
extern size_t block_table_size;
extern ARM_STORAGE_BLOCK block_table[];

/*
 * Global state for the driver.
 */
static struct driver_context_s {
    ARM_Storage_Callback_t callback;
    bool                   initialized;
    ARM_POWER_STATE        power_state;

    ARM_STORAGE_OPERATION  command;
    uint64_t               address;
    size_t                 size;
    size_t                 left;
    const uint8_t          *data;
} driver_context;

static int32_t power_control(ARM_POWER_STATE state);
static int32_t get_next_block(const ARM_STORAGE_BLOCK *prev_block, ARM_STORAGE_BLOCK *next_block);
static int32_t get_block(uint64_t addr, ARM_STORAGE_BLOCK *block);

static void flash_asynch_irq_handler()
{
    struct driver_context_s *context = &driver_context;
    
    /* Cube HAL handler*/
    HAL_FLASH_IRQHandler();
    
    /* Match error codes */
    switch (pFlash.ErrorCode) {
        case HAL_FLASH_ERROR_RD:
        case HAL_FLASH_ERROR_WRP:
        case HAL_FLASH_ERROR_PGS:
        case HAL_FLASH_ERROR_PGP:
        case HAL_FLASH_ERROR_PGA:
            if (context->callback) {
                context->callback(ARM_DRIVER_ERROR_PARAMETER, context->command);
            }
            break;
            
        case HAL_FLASH_ERROR_OPERATION:
            if (context->callback) {
                context->callback(ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE,
                                  context->command);
            }
            break;
            
        default:
            break;
    }
    
    /* End of program/erase */
    switch (context->command) {
        case ARM_STORAGE_OPERATION_PROGRAM_DATA:
            if (context->left == 0) {
                HAL_FLASH_Lock();
                if (context->callback) {    
                    context->callback(context->size, ARM_STORAGE_OPERATION_PROGRAM_DATA);
                }
            } else {
                if (context->left >= 4) {
                    uint32_t offset = context->size - context->left;
                    context->left -= 4;
                    HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_WORD,
                                         (uint32_t)(context->address + offset),
                                         ((const uint32_t *)context->data)[offset / 4]);
                } else {
                    uint32_t offset = context->size - context->left;
                    context->left -= 1;
                    HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_BYTE,
                                         (uint32_t)(context->address + offset),
                                         ((const uint8_t *)context->data)[offset]);
                }    
            }
            break;
            
        case ARM_STORAGE_OPERATION_ERASE:
            if (pFlash.ProcedureOnGoing == FLASH_PROC_NONE) {
                HAL_FLASH_Lock();
                if (context->callback) {
                    context->callback(context->size, ARM_STORAGE_OPERATION_ERASE);
                }
            }
            break;
            
        default:
            if (context->callback) {
                context->callback(ARM_DRIVER_OK, context->command);
            }
            break;
    }
}

static ARM_DRIVER_VERSION get_version(void)
{
    return version;
}

static ARM_STORAGE_CAPABILITIES get_capabilities(void)
{
  return capabilities;
}

static int32_t initialize(ARM_Storage_Callback_t callback)
{
    struct driver_context_s *context = &driver_context;
    
    if (context->initialized) {
        context->callback = callback;
        return 1;
    }
    memset(context, 0, sizeof(struct driver_context_s));
    context->callback = callback;
    if (pFlash.ProcedureOnGoing != FLASH_PROC_NONE) {
        return (int32_t)ARM_DRIVER_ERROR_BUSY;
    }
    
    /* Enable interrupt */
    power_control(ARM_POWER_FULL);
    context->initialized = 1;
    return 1;
}

static int32_t uninitialize(void)
{
    struct driver_context_s *context = &driver_context;

    /* Disable interrupt */
    power_control(ARM_POWER_OFF);
    if (!context->initialized) {
        return ARM_DRIVER_ERROR;
    }
    context->callback = NULL;
    context->initialized = false;
    return 1;
}

static int32_t power_control(ARM_POWER_STATE state)
{
    struct driver_context_s *context = &driver_context;
    
    if (context->power_state == state) {
        return 1;
    }
    context->power_state = state;
    if (capabilities.asynchronous_ops == 1) {
        switch (state) {
            case ARM_POWER_FULL:
                NVIC_SetVector(FLASH_IRQn, (uint32_t)flash_asynch_irq_handler);
                NVIC_ClearPendingIRQ(FLASH_IRQn);
                NVIC_EnableIRQ(FLASH_IRQn);
                break;
                
            case ARM_POWER_LOW:
                return ARM_DRIVER_ERROR_UNSUPPORTED;
                break;
                
            case ARM_POWER_OFF:
                __HAL_FLASH_DISABLE_IT(FLASH_IT_EOP);
                __HAL_FLASH_DISABLE_IT(FLASH_IT_ERR);
                NVIC_DisableIRQ(FLASH_IRQn);
                NVIC_ClearPendingIRQ(FLASH_IRQn);
                break;
                
            default:
                return ARM_DRIVER_ERROR_UNSUPPORTED;
                break;
        }
    }
    return 1;
}

static int32_t read_data(uint64_t addr, void *data, uint32_t size)
{
    struct driver_context_s *context = &driver_context;
    
    if (!context->initialized) {
        return ARM_DRIVER_ERROR;
    }
    /* hardware is busy */
    if (pFlash.ProcedureOnGoing != FLASH_PROC_NONE) {
        return ARM_DRIVER_ERROR_BUSY;
    }
    /* argument validation */
    if ((data == NULL) || (size == 0)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* range check */
    if ((get_block(addr, NULL) != ARM_DRIVER_OK) || (get_block(addr + size - 1, NULL) != ARM_DRIVER_OK)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    context->command = ARM_STORAGE_OPERATION_READ_DATA;
    memcpy(data, (const void *)(uintptr_t)addr, size);
    return size;
}

static int32_t program_data(uint64_t addr, const void *data, uint32_t size)
{
    ARM_STORAGE_BLOCK block;
    struct driver_context_s *context = &driver_context;

    if (!context->initialized) {
        return ARM_DRIVER_ERROR;
    }
    /* hardware is busy */
    if (pFlash.ProcedureOnGoing != FLASH_PROC_NONE) {
        return ARM_DRIVER_ERROR_BUSY;
    }
    /* addr alignment */
    if ((addr % informations.program_unit) != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* size alignment */
    if ((size % informations.program_unit) != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }   
    /* argument validation */
    if ((data == NULL) || (size == 0)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* range check */
    if ((get_block(addr, NULL) != ARM_DRIVER_OK) || (get_block(addr + size - 1, NULL) != ARM_DRIVER_OK)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* minimum size check */
    get_next_block(NULL, &block);
    if (addr < block.addr) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* programmability */
    uint64_t tmp_addr = addr;
    if (get_block(tmp_addr, &block) != ARM_DRIVER_OK) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    while (tmp_addr < (addr + size)) {
        if (!block.attributes.programmable) {
            return ARM_STORAGE_ERROR_NOT_PROGRAMMABLE;
        }

        /* move on to the following block */
        if (get_next_block(&block, &block) != ARM_DRIVER_OK) {
            break;
        }
        tmp_addr = block.addr;
    }
    context->command = ARM_STORAGE_OPERATION_PROGRAM_DATA;
    if (capabilities.asynchronous_ops == 0) {
        /* Synchronous programming */
        HAL_FLASH_Unlock();
        uint64_t i = 0;
        for ( ; i < size; i += 4) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(i + addr),
                    ((const uint32_t *)data)[i / 4]) != HAL_OK) {
                HAL_FLASH_Lock();
                return ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE;
            }
        }
        for ( ; i < size; i += 1) {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, (uint32_t)(i + addr),
                    ((const uint8_t *)data)[i]) != HAL_OK) {
                HAL_FLASH_Lock();
                return ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE;
            }
        }
        HAL_FLASH_Lock();
        return size;
    } else {
        /* Asynchronous programming */
        context->size    = size;
        context->left    = size;
        context->data    = data;
        context->address = addr;
        if (context->left >= 4) {
            uint32_t offset = context->size - context->left;
            context->left -= 4;
            HAL_FLASH_Unlock();
            HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_WORD,
                                 (uint32_t)(context->address + offset),
                                 ((const uint32_t *)context->data)[offset / 4]);
        } else {
            uint32_t offset = context->size - context->left;
            context->left -= 1;
            HAL_FLASH_Unlock();
            HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_BYTE,
                                 (uint32_t)(context->address + offset),
                                 ((const uint8_t *)context->data)[offset]);
        }
        return ARM_DRIVER_OK;
    }
}

static int32_t erase(uint64_t addr, uint32_t size)
{
    struct driver_context_s *context = &driver_context;

    if (!context->initialized) {
        return ARM_DRIVER_ERROR;
    }
    /* hardware is busy */
    if (pFlash.ProcedureOnGoing != FLASH_PROC_NONE) {
        return ARM_DRIVER_ERROR_BUSY;
    }
    /* argument validation */
    if ((size == 0)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* range check */
    if ((get_block(addr, NULL) != ARM_DRIVER_OK) || (get_block(addr + size - 1, NULL) != ARM_DRIVER_OK)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    ARM_STORAGE_BLOCK block;
    uint64_t tmp_addr = addr;
    /* size alignment */
    get_block(addr + size - 1, &block);
    if (((addr + size) % block.attributes.erase_unit) != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* address alignment */
    if (get_block(tmp_addr, &block) != ARM_DRIVER_OK) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    if ((addr % block.attributes.erase_unit) != 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* erasability */
    while (tmp_addr < (addr + size)) {
        if (!block.attributes.erasable) {
            return ARM_STORAGE_ERROR_NOT_ERASABLE;
        }

        /* move on to the following block */
        if (get_next_block(&block, &block) != ARM_DRIVER_OK) {
            break;
        }
        tmp_addr = block.addr;
    }
    context->command = ARM_STORAGE_OPERATION_ERASE;
    
    /* look for sector to erase */
    int8_t base_sector = -1;
    uint8_t nb_sectors = 0;
    uint8_t sector = 0;
    memcpy(&block, &block_table[0], sizeof(ARM_STORAGE_BLOCK));
    tmp_addr = addr;
    while (tmp_addr < (addr + size)) {
        for (uint8_t i = 0; i < block.size / block.attributes.erase_unit; i++) {
            uint32_t sector_addr = i * block.attributes.erase_unit + block.addr;
            if (sector_addr >= addr && sector_addr + block.attributes.erase_unit <= addr + size) {
                if (base_sector == -1) {
                    base_sector = sector;
                }
                nb_sectors += 1;
            }
            sector += 1;
        }
        /* move on to the following block */
        if (get_next_block(&block, &block) != ARM_DRIVER_OK) {
            break;
        }
        tmp_addr = block.addr;
    }
        
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t sector_fail = 0;
    
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector        = base_sector;
    EraseInitStruct.NbSectors     = nb_sectors;
    
    if (capabilities.asynchronous_ops == 0) {
        /* Synchronous erasing */
        HAL_FLASH_Unlock();
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &sector_fail) != HAL_OK) {
            HAL_FLASH_Lock();
            return ARM_STORAGE_ERROR_NOT_ERASABLE;
        }
        HAL_FLASH_Lock();
        return size;
    } else {
        /* Asynchronous erasing */
        context->size = size;
        HAL_FLASH_Unlock();
        HAL_FLASHEx_Erase_IT(&EraseInitStruct);
        return ARM_DRIVER_OK;
    }
}

static int32_t erase_all(void)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_STORAGE_STATUS get_status(void)
{
    struct driver_context_s *context = &driver_context;
    ARM_STORAGE_STATUS status = {
        .busy  = 0,
        .error = 0,
    };
    if (!context->initialized) {
        status.error = 1;
        return status;
    }
    if (pFlash.ProcedureOnGoing != FLASH_PROC_NONE) {
        status.busy = 1;
    } else if (pFlash.ErrorCode != HAL_FLASH_ERROR_NONE) {
        status.error = 1;
    }
    return status;
}

static int32_t get_info(ARM_STORAGE_INFO *info)
{
    memcpy(info, &informations, sizeof(ARM_STORAGE_INFO));
    return ARM_DRIVER_OK;
}

static uint32_t resolve_address(uint64_t addr)
{
    return (uint32_t)addr;
}

static int32_t get_next_block(const ARM_STORAGE_BLOCK *prev_block, ARM_STORAGE_BLOCK *next_block)
{  
    if (prev_block == NULL) {
        /* fetching the first block (instead of next) */
        if (next_block) {
            const ARM_STORAGE_BLOCK *iter = &block_table[0];
            uint8_t sector = 0;
            for (size_t index = 0; index < block_table_size; ++index, ++iter) {
                if (sector < STORAGE_START_SECTOR) {
                    sector += iter->size / iter->attributes.erase_unit;
                    continue;
                }
                memcpy(next_block, &block_table[index], sizeof(ARM_STORAGE_BLOCK));
                return ARM_DRIVER_OK;
            }
        }
    }
    for (size_t index = 0; (block_table_size > 1) && (index < (block_table_size - 1)); index++) {
        if ((block_table[index].addr == prev_block->addr) && (block_table[index].size == prev_block->size)) {
            if (next_block) {
                memcpy(next_block, &block_table[index + 1], sizeof(ARM_STORAGE_BLOCK));
            }
            return ARM_DRIVER_OK;
        }
    }
    if (next_block) {
        next_block->addr = ARM_STORAGE_INVALID_OFFSET;
        next_block->size = 0;
    }
    return ARM_DRIVER_ERROR;
}

static int32_t get_block(uint64_t addr, ARM_STORAGE_BLOCK *block)
{
    const ARM_STORAGE_BLOCK *iter = &block_table[0];
    for (size_t index = 0; index < block_table_size; ++index, ++iter) {
        if ((addr >= iter->addr) && (addr < (iter->addr + iter->size))) {
            if (block) {
                memcpy(block, iter, sizeof(ARM_STORAGE_BLOCK));
            }
            return ARM_DRIVER_OK;
        }
    }
    if (block) {
        block->addr = ARM_STORAGE_INVALID_OFFSET;
        block->size = 0;
    }
    return ARM_DRIVER_ERROR;
}

ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_STM32 = {
    .GetVersion      = get_version,
    .GetCapabilities = get_capabilities,
    .Initialize      = initialize,
    .Uninitialize    = uninitialize,
    .PowerControl    = power_control,
    .ReadData        = read_data,
    .ProgramData     = program_data,
    .Erase           = erase,
    .EraseAll        = erase_all,
    .GetStatus       = get_status,
    .GetInfo         = get_info,
    .ResolveAddress  = resolve_address,
    .GetNextBlock    = get_next_block,
    .GetBlock        = get_block
};

#endif /* #if DEVICE_STORAGE */
