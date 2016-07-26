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

#if STORAGE_RAM

#include "Driver_Storage.h"

#include <string.h>

/*
 * forward declarations
 */
static int32_t getBlock(uint64_t addr, ARM_STORAGE_BLOCK *blockP);
static int32_t nextBlock(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP);

#ifdef STORAGE_CONFIG_HARDWARE_MTD_RAM_ERASE_UNIT
#define ERASE_UNIT           (STORAGE_CONFIG_HARDWARE_MTD_RAM_ERASE_UNIT)
#else
#define ERASE_UNIT           (sizeof(uint32_t))
#endif

#ifdef STORAGE_CONFIG_HARDWARE_MTD_RAM_STORAGE_START_ADDR
#define STORAGE_START_ADDR   (STORAGE_CONFIG_HARDWARE_MTD_RAM_STORAGE_START_ADDR)
#else
#define STORAGE_START_ADDR   (0x00000UL)
#endif

#ifdef STORAGE_CONFIG_HARDWARE_MTD_RAM_STORAGE_SIZE
#define STORAGE_SIZE         (STORAGE_CONFIG_HARDWARE_MTD_RAM_STORAGE_SIZE)
#else
#define STORAGE_SIZE         (0x8000UL)
#endif

#ifdef STORAGE_CONFIG_HARDWARE_MTD_RAM_PROGRAM_UNIT
#define PROGRAM_UNIT         (STORAGE_CONFIG_HARDWARE_MTD_RAM_PROGRAM_UNIT)
#else
#define PROGRAM_UNIT         (1UL)
#endif

#ifdef STORAGE_CONFIG_HARDWARE_MTD_RAM_OPTIMAL_PROGRAM_UNIT
#define OPTIMAL_PROGRAM_UNIT (STORAGE_CONFIG_HARDWARE_MTD_RAM_OPTIMAL_PROGRAM_UNIT)
#else
#define OPTIMAL_PROGRAM_UNIT (sizeof(uint32_t))
#endif

#ifdef STORAGE_CONFIG_HARDWARE_MTD_RAM_BLOCK_COUNT
#define NUM_BLOCKS STORAGE_CONFIG_HARDWARE_MTD_RAM_BLOCK_COUNT
#else
#define NUM_BLOCKS 1
#endif

#if (STORAGE_SIZE % NUM_BLOCKS)
#error "STORAGE_SIZE isn't a multiple of NUM_BLOCKS"
#endif

#define SINGLE_BLOCK_AT_INDEX(N) {                                         \
    addr : (STORAGE_START_ADDR) + ((N) * ((STORAGE_SIZE) / (NUM_BLOCKS))), \
    size : ((STORAGE_SIZE) / (NUM_BLOCKS)),                                \
    attributes : {                                                         \
        erasable        : 1,                                               \
        programmable    : 1,                                               \
        executable      : 1,                                               \
        protectable     : 0,                                               \
        reserved        : 0,                                               \
        erase_unit      : ERASE_UNIT,                                      \
        protection_unit : ((STORAGE_SIZE) / (NUM_BLOCKS)) / 32,            \
    }                                                                      \
}

#define CONCATENATE(A, B) A ## B
#define EXPAND(X) X /* this adds a level of indirection needed to allow macro-expansion following a token-paste operation (see use of CONCATENATE() below). */

#define BLOCK_INITIALIZER_LIST_FOR_1   SINGLE_BLOCK_AT_INDEX(0)
#define BLOCK_INITIALIZER_LIST_FOR_2   BLOCK_INITIALIZER_LIST_FOR_1, SINGLE_BLOCK_AT_INDEX(1)
#define BLOCK_INITIALIZER_LIST_FOR_3   BLOCK_INITIALIZER_LIST_FOR_2, SINGLE_BLOCK_AT_INDEX(2)
#define BLOCK_INITIALIZER_LIST_FOR_4   BLOCK_INITIALIZER_LIST_FOR_3, SINGLE_BLOCK_AT_INDEX(3)
/* ... add more of the above if ever needed */

#define BLOCK_INITIALIZER_LIST(N) EXPAND(CONCATENATE(BLOCK_INITIALIZER_LIST_FOR_, N))

/* The static global memory operated upon by this driver. */
static uint8_t MEMORY[STORAGE_SIZE];

/*
 * Static configuration.
 */
static const ARM_STORAGE_BLOCK blockTable[] = {
    BLOCK_INITIALIZER_LIST(NUM_BLOCKS)
};

static const ARM_DRIVER_VERSION version = {
    .api = ARM_STORAGE_API_VERSION,
    .drv = ARM_DRIVER_VERSION_MAJOR_MINOR(1,00)
};

static const ARM_STORAGE_CAPABILITIES caps = {
    asynchronous_ops : 0,
    erase_all        : 1,    /**< Supports EraseChip operation. */
};

static const ARM_STORAGE_INFO info = {
    total_storage        : STORAGE_SIZE, /**< Total available storage, in units of octets. */

    program_unit         : PROGRAM_UNIT,
    optimal_program_unit : OPTIMAL_PROGRAM_UNIT,

    program_cycles       : ARM_STORAGE_PROGRAM_CYCLES_INFINITE, /**< A measure of endurance for reprogramming.
                                                                 *   Use ARM_STOR_PROGRAM_CYCLES_INFINITE for infinite or unknown endurance. */
    erased_value         : 0x0,  /**< Contents of erased memory (0 to indicate erased octets with state 0x00). */
    memory_mapped        : 1,

    programmability      : ARM_STORAGE_PROGRAMMABILITY_ERASABLE, /**< A value of type enum ARM_STOR_PROGRAMMABILITY. */
    retention_level      : ARM_RETENTION_WHILE_DEVICE_ACTIVE,

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

/**
 * This is a helper function which can be used to do arbitrary sanity checking
 * on a range.
 *
 * It applies the 'check' function to every block in a given range. If any of
 * the check() calls return failure (i.e. something other ARM_DRIVER_OK),
 * validation terminates. Otherwise an ARM_DRIVER_OK is returned.
 */
static int32_t checkForEachBlockInRange(uint64_t startAddr, uint32_t size, int32_t (*check)(const ARM_STORAGE_BLOCK *block))
{
    uint64_t addrBeingValidated = startAddr;
    ARM_STORAGE_BLOCK block;
    if (getBlock(addrBeingValidated, &block) != ARM_DRIVER_OK) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    while (addrBeingValidated < (startAddr + size)) {
        int32_t rc;
        if ((rc = check(&block)) != ARM_DRIVER_OK) {
            return rc;
        }

        /* move on to the following block */
        if (nextBlock(&block, &block) != ARM_DRIVER_OK) {
            break;
        }
        addrBeingValidated = block.addr;
    }

    return ARM_DRIVER_OK;
}

static int32_t blockIsProgrammable(const ARM_STORAGE_BLOCK *blockP) {
    if (!blockP->attributes.programmable) {
        return ARM_STORAGE_ERROR_NOT_PROGRAMMABLE;
    }

    return ARM_DRIVER_OK;
}

static int32_t blockIsErasable(const ARM_STORAGE_BLOCK *blockP) {
    if (!blockP->attributes.erasable) {
        return ARM_STORAGE_ERROR_NOT_ERASABLE;
    }

    return ARM_DRIVER_OK;
}

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
    return 1; /* synchronous completion. */
}

static int32_t uninitialize(void)
{
    return 1; /* synchronous completion. */
}

static int32_t powerControl(ARM_POWER_STATE state)
{
    return 1; /* signal synchronous completion. */
}

static int32_t readData(uint64_t addr, void *data, uint32_t size)
{
    /* Argument validation. */
    if ((data == NULL) || (size == 0)) {
        return ARM_DRIVER_ERROR_PARAMETER; /* illegal */
    }
    if ((getBlock(addr, NULL) != ARM_DRIVER_OK) || (getBlock(addr + size - 1, NULL) != ARM_DRIVER_OK)) {
        return ARM_DRIVER_ERROR_PARAMETER; /* illegal address range */
    }

    memcpy(data, &MEMORY[addr], size);
    return size; /* signal synchronous completion. */
}

static int32_t programData(uint64_t addr, const void *data, uint32_t size)
{
    /* argument validation */
    if ((data == NULL) || (size == 0)) {
        return ARM_DRIVER_ERROR_PARAMETER; /* illegal */
    }
    /* range check */
    if ((getBlock(addr, NULL) != ARM_DRIVER_OK) || (getBlock(addr + size - 1, NULL) != ARM_DRIVER_OK)) {
        return ARM_DRIVER_ERROR_PARAMETER; /* illegal address range */
    }
    /* alignment */
    if (((addr % PROGRAM_UNIT) != 0) || ((size % PROGRAM_UNIT) != 0)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* programmability */
    if (checkForEachBlockInRange(addr, size, blockIsProgrammable) != ARM_DRIVER_OK) {
        return ARM_STORAGE_ERROR_NOT_PROGRAMMABLE;
    }

    memcpy(&MEMORY[addr], data, size);
    return size;
}

static int32_t erase(uint64_t addr, uint32_t size)
{
    /* argument validation */
    if (size == 0) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* range check */
    if ((getBlock(addr, NULL) != ARM_DRIVER_OK) || (getBlock(addr + size - 1, NULL) != ARM_DRIVER_OK)) {
        return ARM_DRIVER_ERROR_PARAMETER; /* illegal address range */
    }
    /* alignment */
    if (((addr % ERASE_UNIT) != 0) || ((size % ERASE_UNIT) != 0)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }
    /* erasability */
    if (checkForEachBlockInRange(addr, size, blockIsErasable) != ARM_DRIVER_OK) {
        return ARM_STORAGE_ERROR_NOT_ERASABLE;
    }

    memset(&MEMORY[addr], 0x0, size);
    return size;
}

static int32_t eraseAll(void)
{
    /* unless we are managing all of storage, we shouldn't allow chip-erase. */
    if (!caps.erase_all) {
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    memset(&MEMORY[STORAGE_START_ADDR], 0x0, STORAGE_SIZE);
    return 1;
}

static ARM_STORAGE_STATUS getStatus(void)
{
    ARM_STORAGE_STATUS status = {
        .busy  = 0,
        .error = 0,
    };

    return status;
}

static int32_t getInfo(ARM_STORAGE_INFO *infoP)
{
    memcpy(infoP, &info, sizeof(ARM_STORAGE_INFO));

    return ARM_DRIVER_OK;
}

static uint32_t resolveAddress(uint64_t addr) {
    return (uint32_t)&MEMORY[addr];
}

int32_t nextBlock(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP)
{
    if (prevP == NULL) {
        /* fetching the first block (instead of next) */
        if (nextP) {
            memcpy(nextP, &blockTable[0], sizeof(ARM_STORAGE_BLOCK));
        }
        return ARM_DRIVER_OK;
    }

    static const size_t NUM_SEGMENTS = sizeof(blockTable) / sizeof(ARM_STORAGE_BLOCK);
    for (size_t index = 0; index < (NUM_SEGMENTS - 1); index++) {
        if ((blockTable[index].addr == prevP->addr) && (blockTable[index].size == prevP->size)) {
            if (nextP) {
                memcpy(nextP, &blockTable[index + 1], sizeof(ARM_STORAGE_BLOCK));
            }
            return ARM_DRIVER_OK;
        }
    }

    if (nextP) {
        nextP->addr = ARM_STORAGE_INVALID_OFFSET;
        nextP->size = 0;
    }
    return ARM_DRIVER_ERROR;
}

int32_t getBlock(uint64_t addr, ARM_STORAGE_BLOCK *blockP)
{
    static const size_t NUM_SEGMENTS = sizeof(blockTable) / sizeof(ARM_STORAGE_BLOCK);

    const ARM_STORAGE_BLOCK *iter = &blockTable[0];
    for (size_t index = 0; index < NUM_SEGMENTS; ++index, ++iter) {
        if ((addr >= iter->addr) && (addr < (iter->addr + iter->size))) {
            if (blockP) {
                memcpy(blockP, iter, sizeof(ARM_STORAGE_BLOCK));
            }
            return ARM_DRIVER_OK;
        }
    }

    if (blockP) {
        blockP->addr = ARM_STORAGE_INVALID_OFFSET;
        blockP->size = 0;
    }
    return ARM_DRIVER_ERROR;
}

ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_RAM = {
    .GetVersion         = getVersion,
    .GetCapabilities    = getCapabilities,
    .Initialize         = initialize,
    .Uninitialize       = uninitialize,
    .PowerControl       = powerControl,
    .ReadData           = readData,
    .ProgramData        = programData,
    .Erase              = erase,
    .EraseAll           = eraseAll,
    .GetStatus          = getStatus,
    .GetInfo            = getInfo,
    .ResolveAddress     = resolveAddress,
    .GetNextBlock       = nextBlock,
    .GetBlock           = getBlock
};

#endif /* #if STORAGE_RAM */
