//*****************************************************************************
//
//  am_hal_secure_ota.c
//! @file
//!
//! @brief Functions for secure over-the-air.
//!
//! @addtogroup
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
#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

// Local defines
#define FLASH_INVALID               0xFFFFFFFF

// Internal OTA state information
typedef struct
{
    uint32_t flashSize;
    uint32_t otaDescAddr;
    uint32_t numOta;
} am_hal_secure_ota_state_t;

static am_hal_secure_ota_state_t gSOtaState;

// Erase a flash page
static void
erase_flash_page(uint32_t ui32ProgamKey, uint32_t ui32Addr)
{
    uint32_t ui32CurrentPage, ui32CurrentBlock;

    //
    // Figure out what page and block we're working on.
    //
    ui32CurrentPage =  AM_HAL_FLASH_ADDR2PAGE(ui32Addr);
    ui32CurrentBlock = AM_HAL_FLASH_ADDR2INST(ui32Addr);

    //
    // Start a critical section.
    //
    AM_CRITICAL_BEGIN
    am_hal_flash_page_erase(ui32ProgamKey,
                            ui32CurrentBlock, ui32CurrentPage);
    //
    // Exit the critical section.
    //
    AM_CRITICAL_END
}


//*****************************************************************************
//
//! @brief  Initialize OTA state
//!
//! Initializes the OTA state. This should be called before doing any other operation
//!
//! @param  ui32ProgamKey - The Flash programming key
//! @param  pOtaDesc should be start of a flash page designated for OTA Descriptor
//!
//! This call will erase the flash page, which will then be incrementally
//! populated as OTA's are added.  It will also initialize the OTAPOINTER to point
//! to this descriptor, marking it as invalid at the same time
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success
//
//*****************************************************************************
uint32_t am_hal_ota_init(uint32_t ui32ProgamKey, uint32_t *pOtaDesc)
{
    am_hal_mcuctrl_device_t sDevice;
    uint32_t otaDescAddr = (uint32_t)pOtaDesc;

    //
    // Get chip specific info
    //
    am_hal_mcuctrl_info_get(AM_HAL_MCUCTRL_INFO_DEVICEID, &sDevice);
    gSOtaState.flashSize = sDevice.ui32FlashSize;

    // Validate the flash page
    if ((otaDescAddr >= gSOtaState.flashSize) ||
        (otaDescAddr & (AM_HAL_FLASH_PAGE_SIZE - 1)))
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
    // TODO - check against protected pages
    // Erase the page
    erase_flash_page(ui32ProgamKey, otaDescAddr);
    // Initialize the OTA Pointer
    MCUCTRL->OTAPOINTER = otaDescAddr;
    gSOtaState.numOta = 0;
    gSOtaState.otaDescAddr = otaDescAddr;

    return AM_HAL_STATUS_SUCCESS;
}

// Add a new OTA to descriptor
//*****************************************************************************
//
//! @brief  Add a new image for OTA
//!
//! Adds a new image to the OTA Descriptor.
//!
//! @param  ui32ProgamKey - The Flash programming key
//! @param  imageMagic image magic# identifying type of image being added to OTA descr
//! @param  pImage should point to the start of new image to be added to descr
//!
//! This will program the next available entry in OTA descriptor. It will also set
//! appropriate state in the OTA pointer register
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success
//
//*****************************************************************************
uint32_t am_hal_ota_add(uint32_t ui32ProgamKey, uint8_t imageMagic, uint32_t *pImage)
{
    uint32_t imageAddr = (uint32_t)pImage;
    // Validate the Image Pointer
    if ((imageAddr >= gSOtaState.flashSize) ||
        (imageAddr & (AM_HAL_FLASH_PAGE_SIZE - 1)))
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
    if (gSOtaState.numOta == AM_HAL_SECURE_OTA_MAX_OTA)
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    imageAddr |= AM_HAL_OTA_STATUS_PENDING;
    // Program the OTA Descriptor word
    am_hal_flash_program_main(ui32ProgamKey,
        &imageAddr,
        ((uint32_t *)gSOtaState.otaDescAddr + gSOtaState.numOta++),
        1);

    // Set appropriate OTA Pointer bits
    MCUCTRL->OTAPOINTER_b.OTAVALID = 1;
    if (imageMagic == AM_IMAGE_MAGIC_SBL)
    {
        MCUCTRL->OTAPOINTER_b.OTASBLUPDATE = 1;
    }

    return AM_HAL_STATUS_SUCCESS;
}

// Get OTA Status
// Can be called anytime (generally after coming back from reset to check the status of OTA
// Will be also used by sbl_main to identify list of OTA's left for it (would show up as PENDING)
//*****************************************************************************
//
//! @brief  Get Current OTA Descriptor state
//!
//! @param  pOtaDesc should be start of a flash page designated for OTA Descriptor
//! @param  maxOta Determines the size of the following buffer
//! @param  pStatus - Return Parameter - populated by this function indicating the OTA
//! status of various OTA's
//!
//! This will retrieve the current OTA status of various images added to the OTA descr
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success
//
//*****************************************************************************
uint32_t am_hal_get_ota_status(uint32_t *pOtaDesc, uint32_t maxOta, am_hal_ota_status_t *pStatus)
{
    uint32_t numOta = 0;
    // Fill up the return structure
    while (maxOta--)
    {
        if (pOtaDesc[numOta] == FLASH_INVALID)
        {
            pStatus[numOta].pImage = (uint32_t *)pOtaDesc[numOta];
            break;
        }
        else
        {
            pStatus[numOta].pImage = (uint32_t *)(pOtaDesc[numOta] & ~0x3);
            pStatus[numOta].status = (am_hal_ota_status_e)(pOtaDesc[numOta] & 0x3);
        }
        numOta++;
    }
    return AM_HAL_STATUS_SUCCESS;
}
