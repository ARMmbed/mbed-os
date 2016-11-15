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

#include "Driver_Storage.h"
#include "cmsis_nvic.h"
#include "MK64F12.h"

#if defined(MCU_MEM_MAP_VERSION) && ((MCU_MEM_MAP_VERSION > 0x0200u) || ((MCU_MEM_MAP_VERSION == 0x0200u) && (MCU_MEM_MAP_VERSION_MINOR >= 0x0008u)))
#define USING_KSDK2 1
#endif

#ifndef USING_KSDK2
#include "device/MK64F12/MK64F12_ftfe.h"
#else
#include "fsl_flash.h"
#endif

#include <string.h>

/* Redefine this macro to a printf equivalent to print trace */
#define tr_debug(...)


#ifdef USING_KSDK2
/*!
 * @name Misc utility defines
 * @{
 */
#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & (uint32_t)(-((int32_t)(a))))
#endif
#ifndef ALIGN_UP
#define ALIGN_UP(x, a) (-((int32_t)((uint32_t)(-((int32_t)(x))) & (uint32_t)(-((int32_t)(a))))))
#endif

#define BYTES_JOIN_TO_WORD_1_3(x, y) ((((uint32_t)(x)&0xFFU) << 24) | ((uint32_t)(y)&0xFFFFFFU))
#define BYTES_JOIN_TO_WORD_2_2(x, y) ((((uint32_t)(x)&0xFFFFU) << 16) | ((uint32_t)(y)&0xFFFFU))
#define BYTES_JOIN_TO_WORD_3_1(x, y) ((((uint32_t)(x)&0xFFFFFFU) << 8) | ((uint32_t)(y)&0xFFU))
#define BYTES_JOIN_TO_WORD_1_1_2(x, y, z) \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFU) << 16) | ((uint32_t)(z)&0xFFFFU))
#define BYTES_JOIN_TO_WORD_1_2_1(x, y, z) \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFFFU) << 8) | ((uint32_t)(z)&0xFFU))
#define BYTES_JOIN_TO_WORD_2_1_1(x, y, z) \
    ((((uint32_t)(x)&0xFFFFU) << 16) | (((uint32_t)(y)&0xFFU) << 8) | ((uint32_t)(z)&0xFFU))
#define BYTES_JOIN_TO_WORD_1_1_1_1(x, y, z, w)                                                      \
    ((((uint32_t)(x)&0xFFU) << 24) | (((uint32_t)(y)&0xFFU) << 16) | (((uint32_t)(z)&0xFFU) << 8) | \
     ((uint32_t)(w)&0xFFU))
/*@}*/

/*!
 * @name Common flash register info defines
 * @{
 */
#if defined(FTFA)
#define FTFx FTFA
#define FTFx_BASE FTFA_BASE
#define FTFx_FSTAT_CCIF_MASK FTFA_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFA_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFA_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFA_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFA_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFA_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFA_FSEC_KEYEN_MASK
#define FTFx_FCNFG_CCIF_MASK FTFA_FCNFG_CCIE_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFA_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFA_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#elif defined(FTFE)
#define FTFx FTFE
#define FTFx_BASE FTFE_BASE
#define FTFx_FSTAT_CCIF_MASK FTFE_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFE_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFE_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFE_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFE_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFE_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFE_FSEC_KEYEN_MASK
#define FTFx_FCNFG_CCIF_MASK FTFE_FCNFG_CCIE_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFE_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFE_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#elif defined(FTFL)
#define FTFx FTFL
#define FTFx_BASE FTFL_BASE
#define FTFx_FSTAT_CCIF_MASK FTFL_FSTAT_CCIF_MASK
#define FTFx_FSTAT_RDCOLERR_MASK FTFL_FSTAT_RDCOLERR_MASK
#define FTFx_FSTAT_ACCERR_MASK FTFL_FSTAT_ACCERR_MASK
#define FTFx_FSTAT_FPVIOL_MASK FTFL_FSTAT_FPVIOL_MASK
#define FTFx_FSTAT_MGSTAT0_MASK FTFL_FSTAT_MGSTAT0_MASK
#define FTFx_FSEC_SEC_MASK FTFL_FSEC_SEC_MASK
#define FTFx_FSEC_KEYEN_MASK FTFL_FSEC_KEYEN_MASK
#define FTFx_FCNFG_CCIF_MASK FTFL_FCNFG_CCIE_MASK
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_RAM) && FSL_FEATURE_FLASH_HAS_FLEX_RAM
#define FTFx_FCNFG_RAMRDY_MASK FTFL_FCNFG_RAMRDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_RAM */
#if defined(FSL_FEATURE_FLASH_HAS_FLEX_NVM) && FSL_FEATURE_FLASH_HAS_FLEX_NVM
#define FTFx_FCNFG_EEERDY_MASK FTFL_FCNFG_EEERDY_MASK
#endif /* FSL_FEATURE_FLASH_HAS_FLEX_NVM */
#else
#error "Unknown flash controller"
#endif
/*@}*/

/*! @brief Access to FTFx->FCCOB */
extern volatile uint32_t *const kFCCOBx;

#endif /* #ifdef USING_KSDK2 */

#ifdef USING_KSDK2
#define ERASE_UNIT                        (FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE)
#define BLOCK1_START_ADDR                 (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE)
#define BLOCK1_SIZE                       (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE)
#define PROGRAM_UNIT                      (FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE)
#define OPTIMAL_PROGRAM_UNIT              (1024UL)
#define PROGRAM_PHRASE_SIZEOF_INLINE_DATA (8)
#define SIZEOF_DOUBLE_PHRASE              (FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT)
#else /* ifdef USING_KSDK2 */
#define ERASE_UNIT                        (4096)
#define BLOCK1_START_ADDR                 (0x80000UL)
#define BLOCK1_SIZE                       (0x80000UL)
#define PROGRAM_UNIT                      (8UL)
#define OPTIMAL_PROGRAM_UNIT              (1024UL)
#define PROGRAM_PHRASE_SIZEOF_INLINE_DATA (8)
#define SIZEOF_DOUBLE_PHRASE              (16)
#endif /* #ifdef USING_KSDK2 */

/* While the K64F flash controller is capable of launching operations asynchronously and
 * allowing program execution to continue while an erase/program is active, it
 * doesn't allow simultaneous read accesses while and erase/program is active on
 * the same block of flash.
 *
 * Read/fetch accesses can originate arbitrarily as a result of program
 * execution. This means that code which operates on flash should not reside in
 * flash; or at least it should not reside in the same bank of flash as it is
 * operating upon. The only way to ensure that application code and flash driver
 * are residing on separate banks of flash is to reserve bank-0 (or BLOCK0) for
 * the application and bank-1 (BLOCK1) for the driver--this also happens to be
 * the default setting.
 *
 * But it is quite likely that this default will be over-ridden by the use of
 * config options depending upon the actual application. If we don't have a
 * clean separation between the application and the space managed by this
 * driver, then we need to enforce the following:
 *
 *   - Force synchronous mode of execution in the storage_driver.
 *   - Disable interrupts during erase/program operations.
 *   - Ensure all code and data structures used in the storage driver execute
 *     out of RAM. Refer to __RAMFUNC (below) which allows for this.
 *
 * It is difficult to determine the application's span of internal-flash at
 * compile time. Therefore we assume that STORAGE_START_ADDR is the
 * boundary between application and this driver. When this boundary is set to
 * lie at BLOCK1_START_ADDR, there is no possibility of read-while-write run-
 * time errors.
 *
 * In the following, caps.asynchronous_ops is defined to be 1 if and only if
 * asynchronous operation mode is requested and there doesn't exist the
 * possibility of concurrent reads.
 */

#if (defined(STORAGE_START_ADDR) && (STORAGE_START_ADDR != BLOCK1_START_ADDR))
#define EXISTS_POSSIBILITY_OF_CONCURRENT_READ 1
#else
#define EXISTS_POSSIBILITY_OF_CONCURRENT_READ 0
#endif

/* Define '__RAMFUNC' as an attribute to mark a function as residing in RAM.
 * Use of __RAMFUNC puts a function in the .data section--i.e. the
 * initialized data section. This will be copied into RAM automatically by the
 * startup sequence. */
#ifndef __RAMFUNC
#if defined(__GNUC__) || defined(__clang__) // GCC and llvm/clang
#define __RAMFUNC __attribute__ ((section (".data#"), noinline)) /* The '#' following ".data" needs a bit of
                        * explanation. Without it, we are liable to get the following warning 'Warning: ignoring
                        * changed section attributes for .data'. This is because __attribute__((section(".data")))
                        * generates the following assembly:
                        *
                        * .section .data,"ax",%progbits
                        *
                        * But .data doesn't need the 'x' (execute) attribute bit. To remove the warning, we specify
                        * the attribute with a '#' at the tail, which emits:
                        *
                        * .section .data#,"ax",%progbits
                        *
                        * Note that '#' (in the above) acts like a comment-start, and masks the additional
                        * attributes which don't apply to '.data'.
                        */
#elif defined (__CC_ARM)
#define __RAMFUNC __attribute__ ((section(".ramfunc"), noinline))
#elif defined ( __ICCARM__ )
#define __RAMFUNC __ramfunc
#else // unknown compiler
    #error "This compiler is not yet supported. If you can contribute support for defining a function to be RAM resident, please provide a definition for __RAMFUNC"
#endif
#endif /* #ifndef __RAMFUNC */

/*
 * forward declarations
 */
static int32_t getBlock(uint64_t addr, ARM_STORAGE_BLOCK *blockP);
static int32_t nextBlock(const ARM_STORAGE_BLOCK* prevP, ARM_STORAGE_BLOCK *nextP);

/*
 * Global state for the driver.
 */
struct mtd_k64f_data {
    ARM_Storage_Callback_t commandCompletionCallback;
    bool                   initialized;
    ARM_POWER_STATE        powerState;

    ARM_STORAGE_OPERATION  currentCommand;
    uint64_t               currentOperatingStorageAddress;
    size_t                 sizeofCurrentOperation;
    size_t                 amountLeftToOperate;
    const uint8_t         *currentOperatingData;
} mtd_k64f_data;

/*
 * Static configuration.
 */
static const ARM_STORAGE_BLOCK blockTable[] = {
    {
        /**< This is the start address of the flash block. */
#ifdef STORAGE_START_ADDR
        .addr       = STORAGE_START_ADDR,
#else
        .addr       = BLOCK1_START_ADDR,
#endif

        /**< This is the size of the flash block, in units of bytes.
         *   Together with addr, it describes a range [addr, addr+size). */
#ifdef STORAGE_SIZE
        .size       = STORAGE_SIZE,
#else
        .size       = BLOCK1_SIZE,
#endif

        .attributes = { /**< Attributes for this block. */
            .erasable        = 1,
            .programmable    = 1,
            .executable      = 1,
            .protectable     = 1,
            .erase_unit      = ERASE_UNIT,
            .protection_unit = BLOCK1_SIZE / 32,
        }
    }
};

static const ARM_DRIVER_VERSION version = {
    .api = ARM_STORAGE_API_VERSION,
    .drv = ARM_DRIVER_VERSION_MAJOR_MINOR(1,00)
};


#if ((!defined(STORAGE_CONFIG_HARDWARE_MTD_K64F_ASYNC_OPS) || STORAGE_CONFIG_HARDWARE_MTD_K64F_ASYNC_OPS) && \
     !EXISTS_POSSIBILITY_OF_CONCURRENT_READ)
#define ASYNC_OPS 1
#else
#define ASYNC_OPS 0
#endif

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
    .asynchronous_ops = ASYNC_OPS,

    /* Enable chip-erase functionality if we own all of block-1. */
    #if ((!defined (STORAGE_START_ADDR) || (STORAGE_START_ADDR == BLOCK1_START_ADDR)) && \
         (!defined (STORAGE_SIZE)       || (STORAGE_SIZE == BLOCK1_SIZE)))
    .erase_all        = 1,    /**< Supports EraseChip operation. */
    #else
    .erase_all        = 0,    /**< Supports EraseChip operation. */
    #endif
};

static const ARM_STORAGE_INFO info = {
#ifdef STORAGE_SIZE
    .total_storage        = STORAGE_SIZE, /**< Total available storage, in units of octets. */
#else
    .total_storage        = BLOCK1_SIZE, /**< Total available storage, in units of octets. By default, BLOCK0 is reserved to hold program code. */
#endif

    .program_unit         = PROGRAM_UNIT,
    .optimal_program_unit = OPTIMAL_PROGRAM_UNIT,

    .program_cycles       = ARM_STORAGE_PROGRAM_CYCLES_INFINITE, /**< A measure of endurance for reprogramming.
                                                                  *   Use ARM_STOR_PROGRAM_CYCLES_INFINITE for infinite or unknown endurance. */

    .erased_value         = 0x1,  /**< Contents of erased memory (1 to indicate erased octets with state 0xFF). */
    .memory_mapped        = 1,

    .programmability      = ARM_STORAGE_PROGRAMMABILITY_ERASABLE, /**< A value of type enum ARM_STOR_PROGRAMMABILITY. */
    .retention_level      = ARM_RETENTION_NVM,
    .security             = {
        .acls                 = 0, /**< against internal software attacks using ACLs. */
        .rollback_protection  = 0, /**< roll-back protection. */
        .tamper_proof         = 0, /**< tamper-proof memory (will be deleted on tamper-attempts using board level or chip level sensors). */
        .internal_flash       = 1, /**< Internal flash. */

        .software_attacks     = 0,
        .board_level_attacks  = 0,
        .chip_level_attacks   = 0,
        .side_channel_attacks = 0,
    }
};

/**
 * This is the command code written into the first FCCOB register, FCCOB0.
 */
enum FlashCommandOps {
    RD1SEC = (uint8_t)0x01, /* read 1s section */
    PGM8   = (uint8_t)0x07, /* program phrase */
    ERSBLK = (uint8_t)0x08, /* erase block */
    ERSSCR = (uint8_t)0x09, /* erase flash sector */
    PGMSEC = (uint8_t)0x0B, /* program section */
    SETRAM = (uint8_t)0x81, /* Set FlexRAM. (unused for now) */
};

/**
 * Read out the CCIF (Command Complete Interrupt Flag) to ensure all previous
 * operations have completed.
 */
static inline bool controllerCurrentlyBusy(void)
{
#ifdef USING_KSDK2
    return ((FTFx->FSTAT & FTFx_FSTAT_CCIF_MASK) == 0);
#else
    return (BR_FTFE_FSTAT_CCIF(FTFE) == 0);
#endif
}

static inline bool failedWithAccessError(void)
{
#ifdef USING_KSDK2
    /* Get flash status register value */
    uint8_t registerValue = FTFx->FSTAT;

    /* checking access error */
    return registerValue & FTFx_FSTAT_ACCERR_MASK;
#else /* ifdef USING_KSDK2 */
    return BR_FTFE_FSTAT_ACCERR(FTFE);
#endif /* ifdef USING_KSDK2 */
}

static inline bool failedWithProtectionError()
{
#ifdef USING_KSDK2
    /* Get flash status register value */
    uint8_t registerValue = FTFx->FSTAT;

    /* checking protection error */
    return registerValue & FTFx_FSTAT_FPVIOL_MASK;
#else /* ifdef USING_KSDK2 */
    return BR_FTFE_FSTAT_FPVIOL(FTFE);
#endif /* ifdef USING_KSDK2 */
}

static inline bool failedWithRunTimeError()
{
#ifdef USING_KSDK2
    /* Get flash status register value */
    uint8_t registerValue = FTFx->FSTAT;

    /* checking MGSTAT0 non-correctable error */
    return registerValue & FTFx_FSTAT_MGSTAT0_MASK;
#else /* ifdef USING_KSDK2 */
    return BR_FTFE_FSTAT_MGSTAT0(FTFE);
#endif /* ifdef USING_KSDK2 */
}

static inline void clearAccessError(void)
{
#ifdef USING_KSDK2
    FTFx->FSTAT |= FTFx_FSTAT_ACCERR_MASK;
#else
    BW_FTFE_FSTAT_ACCERR(FTFE, 1);
#endif
}

static inline void clearProtectionError(void)
{
#ifdef USING_KSDK2
    FTFx->FSTAT |= FTFx_FSTAT_FPVIOL_MASK;
#else
    BW_FTFE_FSTAT_FPVIOL(FTFE, 1);
#endif
}

/**
 * @brief Clear the error bits before launching a command.
 *
 * The ACCERR error bit indicates an illegal access has occurred to an FTFE
 * resource caused by a violation of the command write sequence or issuing an
 * illegal FTFE command. While ACCERR is set, the CCIF flag cannot be cleared to
 * launch a command. The ACCERR bit is cleared by writing a 1 to it.
 *
 * The FPVIOL error bit indicates an attempt was made to program or erase an
 * address in a protected area of program flash or data flash memory during a
 * command write sequence or a write was attempted to a protected area of the
 * FlexRAM while enabled for EEPROM. While FPVIOL is set, the CCIF flag cannot
 * be cleared to launch a command. The FPVIOL bit is cleared by writing a 1 to
 * it.
 */
static inline void clearErrorStatusBits()
{
    if (failedWithAccessError()) {
        clearAccessError();
    }
    if (failedWithProtectionError()) {
        clearProtectionError();
    }
}

/* The following functions are only needed if using interrupt-driven operation. */
#if ASYNC_OPS
static inline void enableCommandCompletionInterrupt(void)
{
#ifdef USING_KSDK2
    FTFx->FCNFG |= FTFE_FCNFG_CCIE_MASK;
#else
    BW_FTFE_FCNFG_CCIE((uintptr_t)FTFE, 1); /* enable interrupt to detect CCIE being set. */
#endif
}

static inline void disbleCommandCompletionInterrupt(void)
{
#ifdef USING_KSDK2
    FTFx->FCNFG &= ~FTFE_FCNFG_CCIE_MASK;
#else
    BW_FTFE_FCNFG_CCIE((uintptr_t)FTFE, 0); /* disable command completion interrupt. */
#endif
}

static inline bool commandCompletionInterruptEnabled(void)
{
#ifdef USING_KSDK2
    return ((FTFx->FCNFG & FTFE_FCNFG_CCIE_MASK) != 0);
#else
    return (BR_FTFE_FCNFG_CCIE((uintptr_t)FTFE) != 0);
#endif
}

/**
 * Once all relevant command parameters have been loaded, the user launches the
 * command by clearing the FSTAT[CCIF] bit by writing a '1' to it. The CCIF flag
 * remains zero until the FTFE command completes.
 */
static inline void launchCommand(void)
{
#ifdef USING_KSDK2
    FTFx->FSTAT = FTFx_FSTAT_CCIF_MASK;
#else
    BW_FTFE_FSTAT_CCIF(FTFE, 1);
#endif
}

#else /* #if !ASYNC_OPS */

#if EXISTS_POSSIBILITY_OF_CONCURRENT_READ
/* This function needs to execute from RAM to avoid read-while-write errors. */
__RAMFUNC
#endif
static void launchCommandAndWaitForCompletion()
{
    // It contains the inlined equivalent of the following code snippet:
    //     launchCommand();
    //     while (controllerCurrentlyBusy()) {
    //         /* Spin waiting for the command execution to complete. */
    //     }

#ifdef USING_KSDK2
    FTFx->FSTAT = FTFx_FSTAT_CCIF_MASK; /* launchcommand() */
    while ((FTFx->FSTAT & FTFx_FSTAT_CCIF_MASK) == 0) {
        /* Spin waiting for the command execution to complete. */
    }
#else
    BW_FTFE_FSTAT_CCIF(FTFE, 1); /* launchCommand() */
    while (BR_FTFE_FSTAT_CCIF(FTFE) == 0) {
        /* Spin waiting for the command execution to complete. */
    }
#endif
}
#endif /* #if !ASYNC_OPS */

#ifndef USING_KSDK2
static inline void setupAddressInCCOB123(uint64_t addr)
{
    BW_FTFE_FCCOB1_CCOBn((uintptr_t)FTFE, (addr >> 16) & 0xFFUL); /* bits [23:16] of the address. */
    BW_FTFE_FCCOB2_CCOBn((uintptr_t)FTFE, (addr >>  8) & 0xFFUL); /* bits [15:8]  of the address. */
    BW_FTFE_FCCOB3_CCOBn((uintptr_t)FTFE, (addr >>  0) & 0xFFUL); /* bits [7:0]   of the address. */
}
#endif /* ifndef USING_KSDK2 */

static inline void setupRead1sSection(uint64_t addr, size_t cnt)
{
#ifdef USING_KSDK2
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(RD1SEC, addr);
    kFCCOBx[1] = BYTES_JOIN_TO_WORD_2_1_1(cnt >> 4, 0 /* normal read level */, 0);
#else
    BW_FTFE_FCCOB0_CCOBn((uintptr_t)FTFE, RD1SEC);
    setupAddressInCCOB123(addr);
    BW_FTFE_FCCOB4_CCOBn((uintptr_t)FTFE, ((cnt >> 4) >> 8) & 0xFFUL); /* bits [15:8] of cnt in units of 128-bits. */
    BW_FTFE_FCCOB5_CCOBn((uintptr_t)FTFE, ((cnt >> 4) >> 0) & 0xFFUL); /* bits  [7:0] of cnt in units of 128-bits. */
    BW_FTFE_FCCOB6_CCOBn((uintptr_t)FTFE, 0); /* use normal read level for 1s. */
#endif
}

static inline bool currentCommandByteIsRD1SEC(void)
{
#ifdef USING_KSDK2
    return ((kFCCOBx[0] >> 24) == RD1SEC);
#else
    return (BR_FTFE_FCCOB0_CCOBn((uintptr_t)FTFE) == RD1SEC);
#endif
}

static inline bool currentCommandSetupIsAnEraseCheck(const struct mtd_k64f_data *context)
{
    return ((context->currentCommand == ARM_STORAGE_OPERATION_ERASE) && currentCommandByteIsRD1SEC());
}

static inline void setupEraseSector(uint64_t addr)
{
#ifdef USING_KSDK2
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(ERSSCR, addr);
#else
    BW_FTFE_FCCOB0_CCOBn((uintptr_t)FTFE, ERSSCR);
    setupAddressInCCOB123(addr);
#endif
}

static inline void setupEraseBlock(uint64_t addr)
{
#ifdef USING_KSDK2
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(ERSBLK, addr);
#else
    BW_FTFE_FCCOB0_CCOBn((uintptr_t)FTFE, ERSBLK);
    setupAddressInCCOB123(addr);
#endif
}

static inline void setup8ByteWrite(uint64_t addr, const void *data)
{
    /* Program FCCOB to load the required command parameters. */
#ifdef USING_KSDK2
    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(PGM8, addr);

    /* Program 8 bytes of data into FCCOB(4..11)_CCOBn */
    kFCCOBx[1] = ((const uint32_t *)data)[0];
    kFCCOBx[2] = ((const uint32_t *)data)[1];
#else /* ifdef USING_KSDK2 */
    BW_FTFE_FCCOB0_CCOBn((uintptr_t)FTFE, PGM8);
    setupAddressInCCOB123(addr);

    BW_FTFE_FCCOB4_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[3]); /* byte 3 of program value. */
    BW_FTFE_FCCOB5_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[2]); /* byte 2 of program value. */
    BW_FTFE_FCCOB6_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[1]); /* byte 1 of program value. */
    BW_FTFE_FCCOB7_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[0]); /* byte 0 of program value. */
    BW_FTFE_FCCOB8_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[7]); /* byte 7 of program value. */
    BW_FTFE_FCCOB9_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[6]); /* byte 6 of program value. */
    BW_FTFE_FCCOBA_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[5]); /* byte 5 of program value. */
    BW_FTFE_FCCOBB_CCOBn((uintptr_t)FTFE, ((const uint8_t *)data)[4]); /* byte 4 of program value. */
#endif /* ifdef USING_KSDK2 */
}

static inline void setupProgramSection(uint64_t addr, const void *data, size_t cnt)
{
#ifdef USING_KSDK2
    static const uintptr_t FlexRAMBase = FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS;
    memcpy((void *)FlexRAMBase, (const uint8_t *)data, cnt);

    kFCCOBx[0] = BYTES_JOIN_TO_WORD_1_3(PGMSEC, addr);
    kFCCOBx[1] = BYTES_JOIN_TO_WORD_2_2(cnt >> 4, 0xFFFFU);
#else /* ifdef USING_KSDK2 */
    static const uintptr_t FlexRAMBase = 0x14000000;
    memcpy((void *)FlexRAMBase, (const uint8_t *)data, cnt);

    BW_FTFE_FCCOB0_CCOBn((uintptr_t)FTFE, PGMSEC);
    setupAddressInCCOB123(addr);

    BW_FTFE_FCCOB4_CCOBn((uintptr_t)FTFE, ((((uint32_t)(cnt >> 4)) & (0x0000FF00)) >> 8)); /* number of 128-bits to program [15:8] */
    BW_FTFE_FCCOB5_CCOBn((uintptr_t)FTFE,  (((uint32_t)(cnt >> 4)) & (0x000000FF)));       /* number of 128-bits to program  [7:0] */
#endif /* ifdef USING_KSDK2 */
}

/**
 * Compute the size of the largest 'program-section' operation which is
 * possible for the range [addr, addr+size) starting from addr. It is assumed
 * that 'addr' is aligned to a double-phrase boundary (see \ref
 * SIZEOF_DOUBLE_PHRASE)--if not, then only a single phrase (8-bytes) write is possible.
 */
static inline size_t sizeofLargestProgramSection(uint64_t addr, size_t size)
{
    /* ensure 'size' is aligned to a double-phrase boundary */
    if ((size % SIZEOF_DOUBLE_PHRASE) == PROGRAM_PHRASE_SIZEOF_INLINE_DATA) {
        size -= PROGRAM_PHRASE_SIZEOF_INLINE_DATA;
    }

    /* ensure 'size' isn't larger than OPTIMAL_PROGRAM_UNIT */
    if (size > OPTIMAL_PROGRAM_UNIT) {
        size = OPTIMAL_PROGRAM_UNIT;
    }

    /* ensure that the operation doesn't cross an erase boundary */
    size_t amountLeftInEraseUnit = ERASE_UNIT - (size_t)(addr % ERASE_UNIT);
    if (size > amountLeftInEraseUnit) {
        size = amountLeftInEraseUnit;
    }

    return size;
}

/**
 * Advance the state machine for program-data. This function is called only if
 * amountLeftToOperate is non-zero.
 */
static inline void setupNextProgramData(struct mtd_k64f_data *context)
{
    if ((context->amountLeftToOperate == PROGRAM_PHRASE_SIZEOF_INLINE_DATA) ||
        ((context->currentOperatingStorageAddress % SIZEOF_DOUBLE_PHRASE) == PROGRAM_PHRASE_SIZEOF_INLINE_DATA)) {
        setup8ByteWrite(context->currentOperatingStorageAddress, context->currentOperatingData);
        tr_debug("setupNextProgramData: W8, [%lu]", (uint32_t)context->currentOperatingStorageAddress);

        context->amountLeftToOperate            -= PROGRAM_PHRASE_SIZEOF_INLINE_DATA;
        context->currentOperatingStorageAddress += PROGRAM_PHRASE_SIZEOF_INLINE_DATA;
        context->currentOperatingData           += PROGRAM_PHRASE_SIZEOF_INLINE_DATA;
    } else {
        size_t amount = sizeofLargestProgramSection(context->currentOperatingStorageAddress, context->amountLeftToOperate);
        setupProgramSection(context->currentOperatingStorageAddress, context->currentOperatingData, amount);
        tr_debug("setupNextProgramData: W%u, [%lu]", amount, (uint32_t)context->currentOperatingStorageAddress);

        context->amountLeftToOperate            -= amount;
        context->currentOperatingStorageAddress += amount;
        context->currentOperatingData           += amount;
    }
}

/* Setup a command to determine if erase is needed for the range
 * [context->currentOperatingStorageAddress, context->currentOperatingStorageAddress + ERASE_UNIT).
 * If any byte within the range isn't 0xFF, this command results in a run-time
 * error.
 *
 * Upon launch, if any byte within the given range isn't 0xFF then this command
 * will terminate in a run-time error.
 */
static inline void setupNextEraseCheck(const struct mtd_k64f_data *context)
{
    setupRead1sSection(context->currentOperatingStorageAddress, ERASE_UNIT); /* setup check for erased */
}

static inline void progressEraseContextByEraseUnit(struct mtd_k64f_data *context)
{
    context->amountLeftToOperate            -= ERASE_UNIT;
    context->currentOperatingStorageAddress += ERASE_UNIT;
}

/**
 * Advance the state machine for erase. This function is called only if
 * amountLeftToOperate is non-zero.
 */
static inline void setupNextErase(struct mtd_k64f_data *context)
{
    setupEraseSector(context->currentOperatingStorageAddress); /* Program FCCOB to load the required command parameters. */
    progressEraseContextByEraseUnit(context);
}

static int32_t executeCommand(struct mtd_k64f_data *context)
{
    tr_debug("executeCommand: top");

#if ASYNC_OPS
    /* Asynchronous operation */
    (void)context; /* avoid compiler warning about un-used variables */
    launchCommand();

    /* At this point, The FTFE reads the command code and performs a series of
     * parameter checks and protection checks, if applicable, which are unique
     * to each command. */
    /* Spin waiting for the command execution to begin. */
    while (!controllerCurrentlyBusy() && !failedWithAccessError() && !failedWithProtectionError());
    if (failedWithAccessError() || failedWithProtectionError()) {
        clearErrorStatusBits();
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    enableCommandCompletionInterrupt();

    tr_debug("executeCommand: async. return");
    return ARM_DRIVER_OK; /* signal asynchronous completion. An interrupt will signal completion later. */
#else /* #if ASYNC_OPS */
    /* Synchronous operation. This is the common case. */

    while (1) {
        tr_debug("executeCommand: synchronous iteration");

        #if EXISTS_POSSIBILITY_OF_CONCURRENT_READ
        __disable_irq();
        #endif
        launchCommandAndWaitForCompletion();
        #if EXISTS_POSSIBILITY_OF_CONCURRENT_READ
        __enable_irq();
        #endif

        /* Execution may result in failure. Check for errors */
        if (failedWithAccessError() || failedWithProtectionError()) {
            clearErrorStatusBits();
            return ARM_DRIVER_ERROR_PARAMETER;
        }
        if (failedWithRunTimeError()) {
            /* filter away run-time errors which may legitimately arise from an erase-check operation. */
            if (!currentCommandSetupIsAnEraseCheck(context)) {
                return ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE;
            }
        }

        /* signal synchronous completion. */
        switch (context->currentCommand) {
            case ARM_STORAGE_OPERATION_PROGRAM_DATA:
                if (context->amountLeftToOperate == 0) {
                    return context->sizeofCurrentOperation;
                }

                /* start the successive program operation */
                setupNextProgramData(context);
                /* continue on to the next iteration of the parent loop */
                break;

            case ARM_STORAGE_OPERATION_ERASE:
                if (currentCommandSetupIsAnEraseCheck(context)) {
                    if (failedWithRunTimeError()) {
                        /* a run-time failure from an erase-check indicates at an erase operation is necessary */
                        setupNextErase(context);
                        /* continue on to the next iteration of the parent loop */
                        break;
                    } else {
                        /* erase can be skipped since this sector is already erased. */
                        tr_debug("fast forward erase");
                        progressEraseContextByEraseUnit(context);
                    }
                }
                if (context->amountLeftToOperate == 0) {
                    return context->sizeofCurrentOperation;
                }

                setupNextEraseCheck(context); /* start the erase check on the successive erase sector */
                /* continue on to the next iteration of the parent loop */
                break;

            default:
                return 1;
        }
    }
#endif /* #ifdef ASYNC_OPS */
}

#if ASYNC_OPS
static inline void launchCommandFromIRQ(const struct mtd_k64f_data *context)
{
    launchCommand();

    while (!controllerCurrentlyBusy() && !failedWithAccessError() && !failedWithProtectionError());
    if (failedWithAccessError() || failedWithProtectionError()) {
        clearErrorStatusBits();
        if (context->commandCompletionCallback) {
            tr_debug("irq: invoking callback with error");
            context->commandCompletionCallback(ARM_DRIVER_ERROR_PARAMETER, context->currentCommand);
        }
        return;
    }

    enableCommandCompletionInterrupt();
}

static void ftfe_ccie_irq_handler(void)
{
    disbleCommandCompletionInterrupt();

    struct mtd_k64f_data *context = &mtd_k64f_data;
    /* check for errors */
    if (failedWithAccessError() || failedWithProtectionError()) {
        clearErrorStatusBits();
        if (context->commandCompletionCallback) {
            context->commandCompletionCallback(ARM_DRIVER_ERROR_PARAMETER, context->currentCommand);
        }
        return;
    }
    if (failedWithRunTimeError()) {
        /* filter away run-time errors which may legitimately arise from an erase-check operation. */
        if (!currentCommandSetupIsAnEraseCheck(context)) {
            if (context->commandCompletionCallback) {
                context->commandCompletionCallback(ARM_STORAGE_ERROR_RUNTIME_OR_INTEGRITY_FAILURE, context->currentCommand);
            }
            return;
        }
    }

    switch (context->currentCommand) {
        case ARM_STORAGE_OPERATION_PROGRAM_DATA:
            if (context->amountLeftToOperate == 0) {
                if (context->commandCompletionCallback) {
                    tr_debug("irq: [PROGRAM] invoking callback");
                    context->commandCompletionCallback(context->sizeofCurrentOperation, ARM_STORAGE_OPERATION_PROGRAM_DATA);
                }
                return;
            }

            /* start the successive program operation */
            setupNextProgramData(context);
            launchCommandFromIRQ(context);
            break;

        case ARM_STORAGE_OPERATION_ERASE:
            if (currentCommandSetupIsAnEraseCheck(context)) {
                if (failedWithRunTimeError()) {
                    /* a run-time failure from an erase-check indicates at an erase operation is necessary */
                    setupNextErase(context);
                    launchCommandFromIRQ(context);
                    break;
                } else {
                    /* erase can be skipped since this sector is already erased. */
                    tr_debug("fast forward erase");
                    progressEraseContextByEraseUnit(context);
                }
            }
            if (context->amountLeftToOperate == 0) {
                if (context->commandCompletionCallback) {
                    tr_debug("irq: [ERASE] invoking callback");
                    context->commandCompletionCallback(context->sizeofCurrentOperation, ARM_STORAGE_OPERATION_ERASE);
                }
                return;
            }

            setupNextEraseCheck(context); /* start the erase check on the successive erase sector */
            launchCommandFromIRQ(context);
            break;

        default:
            if (context->commandCompletionCallback) {
                tr_debug("irq: [default] invoking callback");
                context->commandCompletionCallback(ARM_DRIVER_OK, context->currentCommand);
            }
            break;
    }
}
#endif /* #if ASYNC_OPS */

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

static int32_t blockIsProgrammable(const ARM_STORAGE_BLOCK *blockP)
{
    if (!blockP->attributes.programmable) {
        return ARM_STORAGE_ERROR_NOT_PROGRAMMABLE;
    }

    return ARM_DRIVER_OK;
}

static int32_t blockIsErasable(const ARM_STORAGE_BLOCK *blockP)
{
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
    tr_debug("called initialize(%p)", callback);

    struct mtd_k64f_data *context = &mtd_k64f_data;
    memset(context, 0, sizeof(mtd_k64f_data));
    context->currentCommand = ARM_STORAGE_OPERATION_INITIALIZE;

    if (context->initialized) {
        context->commandCompletionCallback = callback;

        return 1; /* synchronous completion. */
    }

    if (controllerCurrentlyBusy()) {
        /* The user cannot initiate any further FTFE commands until notified that the
         * current command has completed.*/
        return (int32_t)ARM_DRIVER_ERROR_BUSY;
    }

    clearErrorStatusBits();

    context->commandCompletionCallback = callback;

    /* Enable the command-completion interrupt. */
#if ASYNC_OPS
    NVIC_SetVector(FTFE_IRQn, (uint32_t)ftfe_ccie_irq_handler);
    NVIC_ClearPendingIRQ(FTFE_IRQn);
    NVIC_EnableIRQ(FTFE_IRQn);
#endif

    context->initialized = true;

    return 1; /* synchronous completion. */
}

static int32_t uninitialize(void)
{
    tr_debug("called uninitialize");

    struct mtd_k64f_data *context = &mtd_k64f_data;
    context->currentCommand = ARM_STORAGE_OPERATION_UNINITIALIZE;

    if (!context->initialized) {
        return ARM_DRIVER_ERROR;
    }

    /* Disable the command-completion interrupt. */
#if ASYNC_OPS
    if (commandCompletionInterruptEnabled()) {
        disbleCommandCompletionInterrupt();
        NVIC_DisableIRQ(FTFE_IRQn);
        NVIC_ClearPendingIRQ(FTFE_IRQn);
    }
#endif

    context->commandCompletionCallback = NULL;
    context->initialized               = false;
    return 1; /* synchronous completion. */
}

static int32_t powerControl(ARM_POWER_STATE state)
{
    tr_debug("called powerControl(%u)", state);

    struct mtd_k64f_data *context = &mtd_k64f_data;
    context->currentCommand = ARM_STORAGE_OPERATION_POWER_CONTROL;

    context->powerState = state;
    return 1; /* signal synchronous completion. */
}

static int32_t readData(uint64_t addr, void *data, uint32_t size)
{
    tr_debug("called ReadData(%lu, %lu)", (uint32_t)addr, size);

    struct mtd_k64f_data *context = &mtd_k64f_data;
    context->currentCommand = ARM_STORAGE_OPERATION_READ_DATA;

    if (!context->initialized) {
        return ARM_DRIVER_ERROR; /* illegal */
    }

    /* Argument validation. */
    if ((data == NULL) || (size == 0)) {
        return ARM_DRIVER_ERROR_PARAMETER; /* illegal */
    }
    if ((getBlock(addr, NULL) != ARM_DRIVER_OK) || (getBlock(addr + size - 1, NULL) != ARM_DRIVER_OK)) {
        return ARM_DRIVER_ERROR_PARAMETER; /* illegal address range */
    }

    context->currentCommand = ARM_STORAGE_OPERATION_READ_DATA;
    memcpy(data, (const void *)(uintptr_t)addr, size);
    return size; /* signal synchronous completion. */
}

static int32_t programData(uint64_t addr, const void *data, uint32_t size)
{
    tr_debug("called ProgramData(%lu, %lu)", (uint32_t)addr, size);

    struct mtd_k64f_data *context = &mtd_k64f_data;
    if (!context->initialized) {
        return (int32_t)ARM_DRIVER_ERROR; /* illegal */
    }

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

    if (controllerCurrentlyBusy()) {
        /* The user cannot initiate any further FTFE commands until notified that the
         * current command has completed.*/
        return ARM_DRIVER_ERROR_BUSY;
    }

    context->currentCommand                 = ARM_STORAGE_OPERATION_PROGRAM_DATA;
    context->sizeofCurrentOperation         = size;
    context->amountLeftToOperate            = size;
    context->currentOperatingData           = data;
    context->currentOperatingStorageAddress = addr;

    clearErrorStatusBits();
    setupNextProgramData(context);
    return executeCommand(context);
}

static int32_t erase(uint64_t addr, uint32_t size)
{
    tr_debug("called erase(%lu, %lu)", (uint32_t)addr, size);

    struct mtd_k64f_data *context = &mtd_k64f_data;

    if (!context->initialized) {
        return (int32_t)ARM_DRIVER_ERROR; /* illegal */
    }
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

    if (controllerCurrentlyBusy()) {
        /* The user cannot initiate any further FTFE commands until notified that the
         * current command has completed.*/
        return (int32_t)ARM_DRIVER_ERROR_BUSY;
    }

    context->currentCommand                 = ARM_STORAGE_OPERATION_ERASE;
    context->currentOperatingStorageAddress = addr;
    context->sizeofCurrentOperation         = size;
    context->amountLeftToOperate            = size;

    clearErrorStatusBits();
    setupNextEraseCheck(context);
    return executeCommand(context);
}

static int32_t eraseAll(void)
{
    tr_debug("called eraseAll");

    struct mtd_k64f_data *context = &mtd_k64f_data;

    if (!context->initialized) {
        return (int32_t)ARM_DRIVER_ERROR; /* illegal */
    }

    /* unless we are managing all of block 1, we shouldn't allow chip-erase. */
    if ((caps.erase_all != 1) ||
        ((sizeof(blockTable) / sizeof(ARM_STORAGE_BLOCK)) != 1) || /* there are more than one flash blocks */
        (blockTable[0].addr != BLOCK1_START_ADDR)               ||
        (blockTable[0].size != BLOCK1_SIZE)) {
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    if (controllerCurrentlyBusy()) {
        /* The user cannot initiate any further FTFE commands until notified that the
         * current command has completed.*/
        return (int32_t)ARM_DRIVER_ERROR_BUSY;
    }

    context->currentCommand = ARM_STORAGE_OPERATION_ERASE_ALL;

    clearErrorStatusBits();

    /* Program FCCOB to load the required command parameters. */
    setupEraseBlock(BLOCK1_START_ADDR);
    return executeCommand(context);
}

static ARM_STORAGE_STATUS getStatus(void)
{
    struct mtd_k64f_data *context = &mtd_k64f_data;

    ARM_STORAGE_STATUS status = {
        .busy  = 0,
        .error = 0,
    };

    if (!context->initialized) {
        status.error = 1;
        return status;
    }

    if (controllerCurrentlyBusy()) {
        status.busy = 1;
    } else if (failedWithAccessError() || failedWithProtectionError() || failedWithRunTimeError()) {
        status.error = 1;
    }
    return status;
}

static int32_t getInfo(ARM_STORAGE_INFO *infoP)
{
    memcpy(infoP, &info, sizeof(ARM_STORAGE_INFO));

    return ARM_DRIVER_OK;
}

static uint32_t resolveAddress(uint64_t addr)
{
    return (uint32_t)addr;
}

int32_t nextBlock(const ARM_STORAGE_BLOCK *prevP, ARM_STORAGE_BLOCK *nextP)
{
    if (prevP == NULL) {
        /* fetching the first block (instead of next) */
        if (nextP) {
            memcpy(nextP, &blockTable[0], sizeof(ARM_STORAGE_BLOCK));
        }
        return ARM_DRIVER_OK;
    }

    static const size_t NUM_SEGMENTS = sizeof(blockTable) / sizeof(ARM_STORAGE_BLOCK);
    for (size_t index = 0; (NUM_SEGMENTS > 1) && (index < (NUM_SEGMENTS - 1)); index++) {
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

ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F = {
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
