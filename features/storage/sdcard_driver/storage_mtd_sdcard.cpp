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
#include "Driver_Storage.h"

#include <string.h>

/*
 * Global state for the driver.
 */
extern SDCardInterface &sd;

static bool            initialized = false;
ARM_POWER_STATE        powerState  = ARM_POWER_OFF;

#define SECTOR_SIZE 512UL
static uint8_t buffer[SECTOR_SIZE];

/* Set storage size. If set to 0 the value is taken from the SD card. */
#ifndef DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_SIZE
#define STORAGE_SIZE         (1024UL * 1024UL)
#else
#define STORAGE_SIZE         DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_SIZE
#endif

#ifndef DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_START_ADDR
#define STORAGE_START_ADDR   (0x00000UL)
#else
#defien STORAGE_START_ADDR DEVICE_STORAGE_CONFIG_HARDWARE_MTD_SDCARD_START_ADDR
#endif

#define ERASE_UNIT           (SECTOR_SIZE)
#define PROGRAM_UNIT         (SECTOR_SIZE)
#define OPTIMAL_PROGRAM_UNIT (SECTOR_SIZE)

static const ARM_DRIVER_VERSION version = {
    .api = ARM_STORAGE_API_VERSION,
    .drv = ARM_DRIVER_VERSION_MAJOR_MINOR(1,00)
};

static const ARM_STORAGE_CAPABILITIES caps = {
    /**< Signal Flash Ready event. In other words, can APIs like initialize,
     *   read, erase, program, etc. operate in asynchronous mode?
     *   Setting this bit to 1 means that the driver is capable of launching
     *   asynchronous operations; command completion is signaled by the
     *   generation of an event or the invocation of a completion callback
     *   (depending on how the flash controller has been initialized). If set to
     *   1, drivers may still complete asynchronous operations synchronously as
     *   necessary--in which case they return a positive error code to indicate
     *   synchronous completion. */
    asynchronous_ops : 0,

    /* Enable chip-erase functionality if we own all of block-1. */
    erase_all        : 0,    /**< Supports EraseChip operation. */
    reserved         : 0
};

static ARM_STORAGE_INFO info = {
    total_storage        : STORAGE_SIZE, /**< Total available storage, in units of octets. */

    program_unit         : PROGRAM_UNIT,
    optimal_program_unit : OPTIMAL_PROGRAM_UNIT,

    program_cycles       : ARM_STORAGE_PROGRAM_CYCLES_INFINITE, /**< A measure of endurance for reprogramming.
                                                                 *   Use ARM_STOR_PROGRAM_CYCLES_INFINITE for infinite or unknown endurance. */
    erased_value         : 0x1,  /**< Contents of erased memory (1 to indicate erased octets with state 0xFF). */
    memory_mapped        : 0,

    programmability      : ARM_STORAGE_PROGRAMMABILITY_ERASABLE, /**< A value of type enum ARM_STOR_PROGRAMMABILITY. */
    retention_level      : ARM_RETENTION_NVM,

    reserved             : 0,

    security : {
        acls                 : 0, /**< against internal software attacks using ACLs. */
        rollback_protection  : 0, /**< roll-back protection. */
        tamper_proof         : 0, /**< tamper-proof memory (will be deleted on tamper-attempts using board level or chip level sensors). */
        internal_flash       : 0, /**< Internal flash. */
        reserved1            : 0,

        software_attacks     : 0,
        board_level_attacks  : 0,
        chip_level_attacks   : 0,
        side_channel_attacks : 0,
        reserved2            : 0,
    }
};

static ARM_DRIVER_VERSION getVersion(void)
{
    return version;
}

static ARM_STORAGE_CAPABILITIES getCapabilities(void)
{
    return caps;
}

static int32_t initialize(ARM_Storage_Callback_t callback)
{
    if (!initialized) {
        if (sd.disk_initialize() != 0) {
            return ARM_DRIVER_ERROR;
        }
        initialized = true;

        /* If storage size 0, use full SD card size. */
        if (STORAGE_SIZE == 0) {
            info.total_storage = sd.disk_sectors() * SECTOR_SIZE;
        }
    }

    return 1;
}

static int32_t uninitialize(void)
{
    int32_t result = ARM_DRIVER_ERROR;

    if (initialized) {
        initialized = false;
        result      = 1;
    }

    return result;
}

static int32_t powerControl(ARM_POWER_STATE state)
{
    powerState = state;

    return 1;
}

static int32_t readData(uint64_t addr, void *data, uint32_t size)
{
    int32_t result = ARM_DRIVER_ERROR;

    if (!initialized) {
        result = ARM_DRIVER_ERROR; /* illegal */
    }
    /* Argument validation. */
    else if ((data == NULL) || (size == 0)) {
        result = ARM_DRIVER_ERROR_PARAMETER; /* illegal */
    } else {
        /* Set return value to be "success". */
        result = size;

        /* Start sector. This is incremented in each loop. */
        uint32_t sector_start = addr / SECTOR_SIZE;

        /* Start offset. This is only non-zero in the first loop. */
        uint32_t sector_alignment = addr % SECTOR_SIZE;

        /* Remaining data size. Decremented with SECTOR_SIZE in each loop. */
        uint32_t read_remaining = size;

        /* Index pointing to the current write location in the output buffer. */
        uint32_t read_index = 0;

        /* Loop until all bytes have been read. */
        while (read_remaining > 0) {
            /* Read full sector into temporary buffer. */
            int result = sd.disk_read(buffer, sector_start);
            if (result == 0) {
                /* Data size from offset to end of sector. */
                uint32_t sector_remaining = SECTOR_SIZE - sector_alignment;

                /* Ensure we don't read more than what is remaining. */
                if (read_remaining < sector_remaining) {
                    sector_remaining = read_remaining;
                }

                /* Copy data from temporary buffer to output buffer. */
                memcpy(&((uint8_t *)data)[read_index], &buffer[sector_alignment], sector_remaining);

                /* Update loop variables. */
                sector_start++;
                sector_alignment = 0;
                read_remaining  -= sector_remaining;
                read_index      += sector_remaining;
            } else {
                result = ARM_DRIVER_ERROR;
                break;
            }
        }
    }

    return result;
}

static int32_t programData(uint64_t addr, const void *data, uint32_t size)
{
    int32_t result = ARM_DRIVER_ERROR;

    if (!initialized) {
        result = (int32_t) ARM_DRIVER_ERROR; /* illegal */
    }
    /* argument validation */
    else if ((data == NULL) || (size == 0)) {
        result = ARM_DRIVER_ERROR_PARAMETER; /* illegal */
    } else if (((addr % PROGRAM_UNIT) != 0) || ((size % PROGRAM_UNIT) != 0))   {
        result = ARM_DRIVER_ERROR_PARAMETER;
    } else {
        /* Set return value to be "success". */
        result = size;

        /* Start sector. This is incremented in each loop. */
        uint32_t sector_start = addr / SECTOR_SIZE;

        /* Remaining data size. Decremented with SECTOR_SIZE in each loop. */
        uint32_t write_remaining = size;

        /* Index pointing to the current read location in the input buffer. */
        uint32_t write_index = 0;

        /* Loop until all bytes have been read. */
        while (write_remaining > 0) {
            int result = sd.disk_write(&((uint8_t *)data)[write_index], sector_start);
            if (result == 0) {
                /* Update loop variables. */
                sector_start++;
                write_index     += SECTOR_SIZE;
                write_remaining -= SECTOR_SIZE;
            } else {
                result = ARM_DRIVER_ERROR;
                break;
            }
        }
    }

    return result;
}

static int32_t erase(uint64_t addr, uint32_t size)
{
    int32_t result = ARM_DRIVER_ERROR;

    if (!initialized) {
        result = (int32_t) ARM_DRIVER_ERROR; /* illegal */
    }
    /* argument validation */
    else if (size == 0) {
        result = ARM_DRIVER_ERROR_PARAMETER;
    }
    /* alignment */
    else if (((addr % ERASE_UNIT) != 0) || ((size % ERASE_UNIT) != 0)) {
        result = ARM_DRIVER_ERROR_PARAMETER;
    } else {
        /* Set return value to be "success". */
        result = size;

        /* Start sector. This is incremented in each loop. */
        uint32_t sector_start = addr / ERASE_UNIT;

        /* Remaining erase size. Set to at least ERASE_UNIT. */
        uint32_t erase_remaining = (size < ERASE_UNIT) ? ERASE_UNIT : size;

        /* Fill temporary buffer with erase value. */
        memset(buffer, info.erased_value * 0xFF, ERASE_UNIT);

        /* Write temporary buffer to emulate erasing. */
        while (erase_remaining > 0) {
            int result = sd.disk_write(buffer, sector_start);
            if (result == 0) {
                /* Update loop variables. */
                sector_start++;
                erase_remaining -= ERASE_UNIT;
            } else {
                result = ARM_DRIVER_ERROR;
                break;
            }
        }
    }

    return result;
}

static int32_t eraseAll(void)
{
    int32_t result = ARM_DRIVER_ERROR;

    if (!initialized) {
        result = (int32_t) ARM_DRIVER_ERROR; /* illegal */
    } else {
        /* Emulate eraseAll by calling normal erase with the whole range. */
        result = erase(STORAGE_START_ADDR, info.total_storage);
        if (result > 0) {
            result = 1; // return value 1 means success
        } else {
            result = ARM_DRIVER_ERROR;
        }
    }

    return result;
}

static ARM_STORAGE_STATUS getStatus(void)
{
    ARM_STORAGE_STATUS status = {
        .busy  = 0,
        .error = 0,
    };

    if (!initialized) {
        status.error = 1;
    }

    return status;
}

static int32_t getInfo(ARM_STORAGE_INFO *infoP)
{
    int32_t result = ARM_DRIVER_ERROR;

    if (!initialized) {
        initialize(NULL);
    }

    if (infoP) {
        memcpy(infoP, &info, sizeof(ARM_STORAGE_INFO));
        result = ARM_DRIVER_OK;
    }

    return result;
}

static uint32_t resolveAddress(uint64_t addr)
{
    (void) addr;

    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

int32_t nextBlock(const ARM_STORAGE_BLOCK *prevP, ARM_STORAGE_BLOCK *nextP)
{
    int32_t result = ARM_DRIVER_ERROR;

    if (prevP == NULL) {
        /* fetching the first block (instead of next) */
        if (nextP) {
            nextP->addr                       = STORAGE_START_ADDR;
            nextP->size                       = info.total_storage;
            nextP->attributes.erasable        = 1;
            nextP->attributes.programmable    = 1;
            nextP->attributes.executable      = 0;
            nextP->attributes.protectable     = 0;
            nextP->attributes.reserved        = 0;
            nextP->attributes.erase_unit      = ERASE_UNIT;
            nextP->attributes.protection_unit = 0;

            result = ARM_DRIVER_OK;
        }
    } else {
        if (nextP) {
            nextP->addr = ARM_STORAGE_INVALID_OFFSET;
            nextP->size = 0;

            result = ARM_DRIVER_OK;
        }
    }

    return result;
}

int32_t getBlock(uint64_t addr, ARM_STORAGE_BLOCK *blockP)
{
    int32_t result = ARM_DRIVER_ERROR;

    if ((STORAGE_START_ADDR <= addr) && (addr < (STORAGE_START_ADDR + info.total_storage))) {
        /* fetching the first block (instead of next) */
        if (blockP) {
            blockP->addr                       = STORAGE_START_ADDR;
            blockP->size                       = info.total_storage;
            blockP->attributes.erasable        = 1;
            blockP->attributes.programmable    = 1;
            blockP->attributes.executable      = 0;
            blockP->attributes.protectable     = 0;
            blockP->attributes.reserved        = 0;
            blockP->attributes.erase_unit      = ERASE_UNIT;
            blockP->attributes.protection_unit = 0;

            result = ARM_DRIVER_OK;
        }
    } else {
        if (blockP) {
            blockP->addr = ARM_STORAGE_INVALID_OFFSET;
            blockP->size = 0;

            result = ARM_DRIVER_OK;
        }
    }

    return result;
}

extern "C" ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_SPI_SDCard = {
    .GetVersion      = getVersion,
    .GetCapabilities = getCapabilities,
    .Initialize      = initialize,
    .Uninitialize    = uninitialize,
    .PowerControl    = powerControl,
    .ReadData        = readData,
    .ProgramData     = programData,
    .Erase           = erase,
    .EraseAll        = eraseAll,
    .GetStatus       = getStatus,
    .GetInfo         = getInfo,
    .ResolveAddress  = resolveAddress,
    .GetNextBlock    = nextBlock,
    .GetBlock        = getBlock
};

#endif /* #if DEVICE_STORAGE_SDCARD */
