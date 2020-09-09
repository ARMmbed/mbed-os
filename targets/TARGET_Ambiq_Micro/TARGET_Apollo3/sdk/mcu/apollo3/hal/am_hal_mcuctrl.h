//*****************************************************************************
//
//  am_hal_mcuctrl.h
//! @file
//!
//! @brief Functions for accessing and configuring the MCUCTRL.
//!
//! @addtogroup mcuctrl3 MCU Control (MCUCTRL)
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
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
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#ifndef AM_HAL_MCUCTRL_H
#define AM_HAL_MCUCTRL_H

#ifdef __cplusplus
extern "C"
{
#endif

//
// Designate this peripheral.
//
#define AM_APOLLO3_MCUCTRL  1

//*****************************************************************************
//
// Chip Revision IDentification.
//
//*****************************************************************************
#define APOLLO3_B0                                                          \
        ((MCUCTRL->CHIPREV  &                                               \
           (MCUCTRL_CHIPREV_REVMAJ_Msk | MCUCTRL_CHIPREV_REVMIN_Msk)) ==    \
           (_VAL2FLD(MCUCTRL_CHIPREV_REVMAJ, MCUCTRL_CHIPREV_REVMAJ_B) |    \
            _VAL2FLD(MCUCTRL_CHIPREV_REVMIN, MCUCTRL_CHIPREV_REVMIN_REV0)))

#define APOLLO3_A1                                                          \
        ((MCUCTRL->CHIPREV  &                                               \
           (MCUCTRL_CHIPREV_REVMAJ_Msk | MCUCTRL_CHIPREV_REVMIN_Msk)) ==    \
           (_VAL2FLD(MCUCTRL_CHIPREV_REVMAJ, MCUCTRL_CHIPREV_REVMAJ_A) |    \
            _VAL2FLD(MCUCTRL_CHIPREV_REVMIN, MCUCTRL_CHIPREV_REVMIN_REV1)))

#define APOLLO3_A0                                                          \
        ((MCUCTRL->CHIPREV  &                                               \
           (MCUCTRL_CHIPREV_REVMAJ_Msk | MCUCTRL_CHIPREV_REVMIN_Msk)) ==    \
           (_VAL2FLD(MCUCTRL_CHIPREV_REVMAJ, MCUCTRL_CHIPREV_REVMAJ_A) |    \
            _VAL2FLD(MCUCTRL_CHIPREV_REVMIN, MCUCTRL_CHIPREV_REVMIN_REV0)))

//
// Determine if >= a given revision level.
//
#define APOLLO3_GE_B0                                                       \
        ((MCUCTRL->CHIPREV  &                                               \
           (MCUCTRL_CHIPREV_REVMAJ_Msk | MCUCTRL_CHIPREV_REVMIN_Msk)) >=    \
           (_VAL2FLD(MCUCTRL_CHIPREV_REVMAJ, MCUCTRL_CHIPREV_REVMAJ_B) |    \
            _VAL2FLD(MCUCTRL_CHIPREV_REVMIN, MCUCTRL_CHIPREV_REVMIN_REV0)))

#define APOLLO3_GE_A1                                                       \
        ((MCUCTRL->CHIPREV  &                                               \
           (MCUCTRL_CHIPREV_REVMAJ_Msk | MCUCTRL_CHIPREV_REVMIN_Msk)) >=    \
           (_VAL2FLD(MCUCTRL_CHIPREV_REVMAJ, MCUCTRL_CHIPREV_REVMAJ_A) |    \
            _VAL2FLD(MCUCTRL_CHIPREV_REVMIN, MCUCTRL_CHIPREV_REVMIN_REV1)))


//*****************************************************************************
//
// MCUCTRL specific definitions.
//
//*****************************************************************************
#define AM_HAL_MCUCTRL_CHIPPN_FLASH_SIZE_N  ((MCUCTRL_CHIPPN_PARTNUM_FLASHSIZE_M >> MCUCTRL_CHIPPN_PARTNUM_FLASHSIZE_S) + 1)
#define AM_HAL_MCUCTRL_CHIPPN_SRAM_SIZE_N   ((MCUCTRL_CHIPPN_PARTNUM_SRAMSIZE_M  >> MCUCTRL_CHIPPN_PARTNUM_SRAMSIZE_S) + 1)

//*****************************************************************************
//
// MCUCTRL enumerations
//
//*****************************************************************************
//**************************************
//! MCUCTRL control operations
//**************************************
typedef enum
{
    AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_ENABLE,
    AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_DISABLE,
    AM_HAL_MCUCTRL_CONTROL_EXTCLK32K_ENABLE,
    AM_HAL_MCUCTRL_CONTROL_EXTCLK32K_DISABLE,
    AM_HAL_MCUCTRL_CONTROL_SRAM_PREFETCH
} am_hal_mcuctrl_control_e;

//**************************************
//! MCUCTRL info get
//**************************************
typedef enum
{
    AM_HAL_MCUCTRL_INFO_FEATURES_AVAIL,
    AM_HAL_MCUCTRL_INFO_DEVICEID,
    AM_HAL_MCUCTRL_INFO_FAULT_STATUS
} am_hal_mcuctrl_infoget_e;

//**************************************
//! MCUCTRL SRAM prefetch settings
//!
//! Prefetch settings are made via a call to:
//! am_hal_mcuctrl_control(AM_HAL_MCUCTRL_CONTROL_SRAM_PREFETCH,
//!                        &ui32PrefetchSetting);
//!
//! The settings may be logically ORed together to obtain the desired settings.
//!
//! Notes:
//! - NOPREFETCH settings override PREFETCH settings if both are provided.
//!   For example, calling with both PREFETCH_INSTR and NOPREFETCH_INSTR
//!   will result in instruction prefetch being disabled.
//! - When executing from SRAM, it is recommended that the PREFETCH_INSTR and
//!   PREFETCH_INSTRCACHE bits be set.
//! - It is generally okay to have PREFETCH_INSTR & PREFETCH_INSTRCACHE enabled
//!   even if no SRAM execution is expected.
//! - It is generally not recommended that data prefetch be enabled unless the
//!   work flow has a large number of sequential accesses.
//! - Setting PREFETCH_INSTRCACHE requires PREFETCH_INSTR.  This is enforced by
//!   the function and an error is returned if both are not being set or if
//!   PREFETCH_INSTR is not already set in the register.
//! - Setting PREFETCH_DATACACHE requires PREFETCH_DATA.  This is enforced by
//!   the function.  An error is returned if both are not being set or if
//!   PREFETCH_DATA is not already set in the register.
//**************************************
#define SRAM_NOPREFETCH_Pos     16
#define AM_HAL_MCUCTRL_SRAM_PREFETCH_INSTR        (MCUCTRL_SRAMMODE_IPREFETCH_Msk       << 0)
#define AM_HAL_MCUCTRL_SRAM_PREFETCH_INSTRCACHE   (MCUCTRL_SRAMMODE_IPREFETCH_CACHE_Msk << 0)
#define AM_HAL_MCUCTRL_SRAM_PREFETCH_DATA         (MCUCTRL_SRAMMODE_DPREFETCH_Msk       << 0)
#define AM_HAL_MCUCTRL_SRAM_PREFETCH_DATACACHE    (MCUCTRL_SRAMMODE_DPREFETCH_CACHE_Msk << 0)
#define AM_HAL_MCUCTRL_SRAM_NOPREFETCH_INSTR      (MCUCTRL_SRAMMODE_IPREFETCH_Msk       << SRAM_NOPREFETCH_Pos)
#define AM_HAL_MCUCTRL_SRAM_NOPREFETCH_INSTRCACHE (MCUCTRL_SRAMMODE_IPREFETCH_CACHE_Msk << SRAM_NOPREFETCH_Pos)
#define AM_HAL_MCUCTRL_SRAM_NOPREFETCH_DATA       (MCUCTRL_SRAMMODE_DPREFETCH_Msk       << SRAM_NOPREFETCH_Pos)
#define AM_HAL_MCUCTRL_SRAM_NOPREFETCH_DATACACHE  (MCUCTRL_SRAMMODE_DPREFETCH_CACHE_Msk << SRAM_NOPREFETCH_Pos)

//*****************************************************************************
//
// MCUCTRL data structures
//
//*****************************************************************************
//**************************************
//! MCUCTRL device structure
//**************************************
typedef struct
{
    //
    //! Device part number. (BCD format)
    //
    uint32_t ui32ChipPN;

    //
    //! Unique Chip ID 0.
    //
    uint32_t ui32ChipID0;

    //
    //! Unique Chip ID 1.
    //
    uint32_t ui32ChipID1;

    //
    //! Chip Revision.
    //
    uint32_t ui32ChipRev;

    //
    //! Vendor ID.
    //
    uint32_t ui32VendorID;

    //
    //! SKU (Apollo3).
    //
    uint32_t ui32SKU;

    //
    //! Qualified chip.
    //
    uint32_t ui32Qualified;

    //
    //! Flash Size.
    //
    uint32_t ui32FlashSize;

    //
    //! SRAM Size.
    //
    uint32_t ui32SRAMSize;

    //
    // JEDEC chip info
    //
    uint32_t ui32JedecPN;
    uint32_t ui32JedecJEPID;
    uint32_t ui32JedecCHIPREV;
    uint32_t ui32JedecCID;
}
am_hal_mcuctrl_device_t;

//**************************************
//! MCUCTRL fault structure
//**************************************
typedef struct
{
    //
    //! ICODE bus fault occurred.
    //
    bool bICODE;

    //
    //! ICODE bus fault address.
    //
    uint32_t ui32ICODE;

    //
    //! DCODE bus fault occurred.
    //
    bool bDCODE;

    //
    //! DCODE bus fault address.
    //
    uint32_t ui32DCODE;

    //
    //! SYS bus fault occurred.
    //
    bool bSYS;

    //
    //! SYS bus fault address.
    //
    uint32_t ui32SYS;
}
am_hal_mcuctrl_fault_t;

//**************************************
//! MCUCTRL status structure
//**************************************
typedef struct
{
    bool        bBurstAck;          // FEATUREENABLE
    bool        bBLEAck;            // "
    bool        bDebuggerLockout;   // DEBUGGER
    bool        bADCcalibrated;     // ADCCAL
    bool        bBattLoadEnabled;   // ADCBATTLOAD
    uint8_t     bSecBootOnWarmRst;  // BOOTLOADER
    uint8_t     bSecBootOnColdRst;  //  "
} am_hal_mcuctrl_status_t;

//**************************************
//! MCUCTRL features available structure
//**************************************
typedef struct
{
    bool        bBurstAvail;        // FEATUREENABLE
    bool        bBLEavail;          //  "
    bool        bBLEFeature;        // SKU
    bool        bBurstFeature;      //  "
    uint8_t     ui8SecBootFeature;  // BOOTLOADER
} am_hal_mcuctrl_feature_t;


// ****************************************************************************
//
//! @brief Apply various specific commands/controls on the MCUCTRL module.
//!
//! This function is used to apply various controls to MCUCTRL.
//!
//! @param eControl - One of the following:
//!     AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_ENABLE
//!     AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_DISABLE
//!     AM_HAL_MCUCTRL_CONTROL_EXTCLK32K_ENABLE
//!     AM_HAL_MCUCTRL_CONTROL_EXTCLK32K_DISABLE
//!     AM_HAL_MCUCTRL_CONTROL_SRAM_PREFETCH
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_mcuctrl_control(am_hal_mcuctrl_control_e eControl,
                                       void *pArgs);

// ****************************************************************************
//
//! @brief MCUCTRL status function
//!
//! This function returns current status of the MCUCTRL as obtained from
//! various registers of the MCUCTRL block.
//!
//! @param psStatus - ptr to a status structure to receive the current statuses.
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_mcuctrl_status_get(am_hal_mcuctrl_status_t *psStatus);

// ****************************************************************************
//
//! @brief Get information of the given MCUCTRL item.
//!
//! This function returns a data structure of information regarding the given
//! MCUCTRL parameter.
//!
//! @param eInfoGet - One of the following:         Return structure type:
//!     AM_HAL_MCUCTRL_INFO_DEVICEID,               psDevice
//!     AM_HAL_MCUCTRL_INFO_FAULT_STATUS            psFault
//!
//! @param pInfo - A pointer to a structure to receive the return data,
//! the type of which is dependent on the eInfo parameter.
//!
//! @return status      - generic or interface specific status.
//
// ****************************************************************************
extern uint32_t am_hal_mcuctrl_info_get(am_hal_mcuctrl_infoget_e eInfoGet,
                                        void *pInfo);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_MCUCTRL_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
