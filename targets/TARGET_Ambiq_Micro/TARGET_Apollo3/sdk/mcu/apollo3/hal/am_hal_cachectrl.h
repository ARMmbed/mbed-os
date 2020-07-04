// ****************************************************************************
//
//  am_hal_cachectrl.h
//! @file
//!
//! @brief Functions for accessing and configuring the CACHE controller.
//!
//! @addtogroup cachectrl3 Cache Control (CACHE)
//! @ingroup apollo3hal
//! @{
//
// ****************************************************************************

// ****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
// ****************************************************************************
#ifndef AM_HAL_CACHECTRL_H
#define AM_HAL_CACHECTRL_H

#ifdef __cplusplus
extern "C"
{
#endif

//
// Designate this peripheral.
//
#define AM_APOLLO3_CACHECTRL    1

//
// Cachectrl status.
//
typedef struct
{
    bool     bFlash0SleepMode;
    bool     bFlash1SleepMode;
    bool     bCacheReady;
} am_hal_cachectrl_status_t;

// ****************************************************************************
//
//! @name Cache Config
//! @brief Configuration selection for the cache.
//!
//! These macros may be used in conjunction with the
//!  am_hal_cachectrl_cache_config() function to select the cache type.
//!
//! @{
//
// ****************************************************************************
//
// Cache description type, where:
//  nWay = number of ways (associativity)
//  128B = 128 bits linesize
//  512E = 512 entries, 1024E = 1024 entries
//
typedef enum
{
    AM_HAL_CACHECTRL_DESCR_1WAY_128B_512E  = CACHECTRL_CACHECFG_CONFIG_W1_128B_512E,
    AM_HAL_CACHECTRL_DESCR_2WAY_128B_512E  = CACHECTRL_CACHECFG_CONFIG_W2_128B_512E,
    AM_HAL_CACHECTRL_DESCR_1WAY_128B_1024E = CACHECTRL_CACHECFG_CONFIG_W1_128B_1024E
} am_hal_cachectrl_descr_e;

typedef enum
{
    AM_HAL_CACHECTRL_NCR0 = 0,
    AM_HAL_CACHECTRL_NCR1 = 1
} am_hal_cachectrl_nc_region_e;

// Config struture for AM_HAL_CACHECTRL_CONTROL_NC_CFG
typedef struct
{
    am_hal_cachectrl_nc_region_e    eNCRegion;
    bool                            bEnable;
    uint32_t                        ui32StartAddr;
    uint32_t                        ui32EndAddr;
} am_hal_cachectrl_nc_cfg_t;

//
// Control operations.
//
typedef enum
{
    AM_HAL_CACHECTRL_CONTROL_FLASH_CACHE_INVALIDATE = 1,
    AM_HAL_CACHECTRL_CONTROL_STATISTICS_RESET,
    AM_HAL_CACHECTRL_CONTROL_FLASH_ALL_SLEEP_ENABLE,
    AM_HAL_CACHECTRL_CONTROL_FLASH_ALL_SLEEP_DISABLE,
    AM_HAL_CACHECTRL_CONTROL_FLASH0_SLEEP_ENABLE,
    AM_HAL_CACHECTRL_CONTROL_FLASH0_SLEEP_DISABLE,
    AM_HAL_CACHECTRL_CONTROL_FLASH1_SLEEP_ENABLE,
    AM_HAL_CACHECTRL_CONTROL_FLASH1_SLEEP_DISABLE,
    AM_HAL_CACHECTRL_CONTROL_MONITOR_ENABLE,
    AM_HAL_CACHECTRL_CONTROL_MONITOR_DISABLE,
    AM_HAL_CACHECTRL_CONTROL_LPMMODE_RESET,
    AM_HAL_CACHECTRL_CONTROL_LPMMODE_RECOMMENDED,
    AM_HAL_CACHECTRL_CONTROL_LPMMODE_AGGRESSIVE,
    AM_HAL_CACHECTRL_CONTROL_LPMMODE_SET,
    AM_HAL_CACHECTRL_CONTROL_SEDELAY_SET,
    AM_HAL_CACHECTRL_CONTROL_RDWAIT_SET,
    // Configure up to two non-cacheable regions
    AM_HAL_CACHECTRL_CONTROL_NC_CFG,
} am_hal_cachectrl_control_e;

//
// Cache config values used for ui8Mode.
//
typedef enum
{
    // Note - this enum ordering is critical, do not modify.
    AM_HAL_CACHECTRL_CONFIG_MODE_DISABLE,
    AM_HAL_CACHECTRL_CONFIG_MODE_INSTR,
    AM_HAL_CACHECTRL_CONFIG_MODE_DATA,
    AM_HAL_CACHECTRL_CONFIG_MODE_INSTR_DATA
} am_hal_cachectrl_config_mode_e;

//
// FLASHCFG LPMMODE.
//
typedef enum
{
    AM_HAL_CACHECTRL_FLASHCFG_LPMMODE_NEVER   = CACHECTRL_FLASHCFG_LPMMODE_NEVER,
    AM_HAL_CACHECTRL_FLASHCFG_LPMMODE_STANDBY = CACHECTRL_FLASHCFG_LPMMODE_STANDBY,
    AM_HAL_CACHECTRL_FLASHCFG_LPMMODE_ALWAYS  = CACHECTRL_FLASHCFG_LPMMODE_ALWAYS
} am_hal_cachectrl_flashcfg_lppmode_e;

// ****************************************************************************
//
// Cache configuration structure
//  This structure used for am_hal_cachectrl_config().
//
// ****************************************************************************
typedef struct
{
    //
    //! Set to one of:
    //! AM_HAL_CACHECTRL_DESCR_1WAY_128B_512E
    //!     Direct mapped, 128-bit linesize, 512 entries (4 SRAMs active)
    //! AM_HAL_CACHECTRL_DESCR_2WAY_128B_512E
    //!     Two way set associative, 128-bit linesize, 512 entries (8 SRAMs active)
    //! AM_HAL_CACHECTRL_DESCR_1WAY_128B_1024E
    //!     Direct-mapped set associative, 128-bit linesize, 1024 entries (8 SRAMs active)
    am_hal_cachectrl_descr_e eDescript;

    //
    //! Set to one of the following:
    //! AM_HAL_CACHECTRL_CONFIG_MODE_DISABLE     - Disable both instr and data caching
    //! AM_HAL_CACHECTRL_CONFIG_MODE_INSTR       - Enable instr caching only
    //! AM_HAL_CACHECTRL_CONFIG_MODE_DATA        - Enable data caching only
    //! AM_HAL_CACHECTRL_CONFIG_MODE_INSTR_DATA  - Enable both instr and data caching
    am_hal_cachectrl_config_mode_e eMode;

    //
    //! Set to true to enable the LRU (least recently used) replacement policy.
    //! Set to false to enable the LRR (least recently replaced) replacement policy.
    //! Note - LRR minimizes writes to the TAG SRAM.
    //
    bool bLRU;

} am_hal_cachectrl_config_t;

extern const am_hal_cachectrl_config_t am_hal_cachectrl_defaults;

// ****************************************************************************
//
// Function prototypes
//
// ****************************************************************************
// ****************************************************************************
//
//! @brief Configure the cache using the supplied settings.
//!
//! @param psConfig - pointer to a config structure containing cache settings.
//!
//! This function takes in a structure of cache settings and uses them to
//! configure the cache.  This function will configures all of the settings in
//! the structure as well as recommended settings for various other cache
//! configuration parameters.
//!
//! This function does NOT enable the cache, which is handled in a separate
//! function.  In fact, if the cache is enabled prior to calling this function,
//! it will return from the call disabled.
//!
//! For most applications, the default cache settings will be the most
//! efficient choice. To use the default cache settings with this function, use
//! the address of the global am_hal_cachectrl_defaults structure as the
//! psConfig argument.
//!
//! @return Status.
//
// ****************************************************************************
extern uint32_t am_hal_cachectrl_config(const am_hal_cachectrl_config_t *psConfig);

// ****************************************************************************
//
//! @brief Enable the cache.
//!
//! Enable the cache for operation.
//!
//! @return Status.
//
// ****************************************************************************
extern uint32_t am_hal_cachectrl_enable(void);

// ****************************************************************************
//
//! @brief Disable the cache.
//!
//! Use this function to disable cache.  Other configuration settings are not
//! not required.
//!
//! @return Status.
//
// ****************************************************************************
extern uint32_t am_hal_cachectrl_disable(void);

// ****************************************************************************
//
//! @brief Assert various specific controls on the cache.
//!
//! This function is used to apply various controls on the cache.
//!
//! @param eControl - One of the following:
//!     AM_HAL_CACHECTRL_CONTROL_FLASH_CACHE_INVALIDATE
//!     AM_HAL_CACHECTRL_CONTROL_STATISTICS_RESET
//!     AM_HAL_CACHECTRL_CONTROL_FLASH_ALL_SLEEP_ENABLE,
//!     AM_HAL_CACHECTRL_CONTROL_FLASH_ALL_SLEEP_DISABLE,
//!     AM_HAL_CACHECTRL_CONTROL_FLASH0_SLEEP_ENABLE
//!     AM_HAL_CACHECTRL_CONTROL_FLASH0_SLEEP_DISABLE
//!     AM_HAL_CACHECTRL_CONTROL_FLASH1_SLEEP_ENABLE
//!     AM_HAL_CACHECTRL_CONTROL_FLASH1_SLEEP_DISABLE
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_cachectrl_control(am_hal_cachectrl_control_e eControl,
                                         void *pArgs);

// ****************************************************************************
//
//! @brief Cache controller status function
//!
//! This function returns the current status of the cache.
//!
//! @param psStatus - ptr to a status structure to receive the current statuses.
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_cachectrl_status_get(am_hal_cachectrl_status_t *psStatus);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_CACHECTRL_H
