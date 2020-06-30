//*****************************************************************************
//
//  am_hal_security.c
//! @file
//!
//! @brief Functions for on-chip security features
//!
//! @addtogroup
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
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
// This is part of revision v2.2.0-7-g63f7c2ba1 of the AmbiqSuite Development Package.
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

// Local defines
// Maximum iterations for hardware CRC to finish
#define MAX_CRC_WAIT        100000

#define AM_HAL_SECURITY_LOCKSTAT_CUSTOMER       0x1
#define AM_HAL_SECURITY_LOCKSTAT_RECOVERY       0x40000000

// Global declaration
// These are declared as ptr variables to avoid an issue with GCC reading from location 0x0.
const volatile uint32_t *g_pFlash0 = (uint32_t*)(AM_HAL_SBL_ADDRESS + 0);
const volatile uint32_t *g_pFlash4 = (uint32_t*)(AM_HAL_SBL_ADDRESS + 4);

//*****************************************************************************
//
//! @brief  Hardcoded function - to Run supplied main program
//!
//! @param  r0 = vtor - address of the vector table
//!
//! @return Returns None
//
//*****************************************************************************
#if (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION <  6000000)
static __asm void
bl_run_main(uint32_t *vtor)
{
    //
    // Store the vector table pointer of the new image into VTOR.
    //
    movw    r3, #0xED08
    movt    r3, #0xE000
    str     r0, [r3, #0]

    //
    // Load the new stack pointer into R1 and the new reset vector into R2.
    //
    ldr     r3, [r0, #0]
    ldr     r2, [r0, #4]

    //
    // Set the stack pointer for the new image.
    //
    mov     sp, r3

    //
    // Jump to the new reset vector.
    //
    bx      r2
}
#elif (defined (__ARMCC_VERSION)) && (__ARMCC_VERSION >= 6000000)
__attribute__((naked))
static void
bl_run_main(uint32_t *vtor)
{
    __asm
    (
        "   movw    r3, #0xED08\n\t"    // Store the vector table pointer of the new image into VTOR.
        "   movt    r3, #0xE000\n\t"
        "   str     r0, [r3, #0]\n\t"
        "   ldr     r3, [r0, #0]\n\t"   // Load the new stack pointer into R1 and the new reset vector into R2.
        "   ldr     r2, [r0, #4]\n\t"
        "   mov     sp, r3\n\t"         // Set the stack pointer for the new image.
        "   bx      r2\n\t"            // Jump to the new reset vector.
    );
}
#elif defined(__GNUC_STDC_INLINE__)
__attribute__((naked))
static void
bl_run_main(uint32_t *vtor)
{
    __asm
    (
        "   movw    r3, #0xED08\n\t"    // Store the vector table pointer of the new image into VTOR.
        "   movt    r3, #0xE000\n\t"
        "   str     r0, [r3, #0]\n\t"
        "   ldr     r3, [r0, #0]\n\t"   // Load the new stack pointer into R1 and the new reset vector into R2.
        "   ldr     r2, [r0, #4]\n\t"
        "   mov     sp, r3\n\t"         // Set the stack pointer for the new image.
        "   bx      r2\n\t"            // Jump to the new reset vector.
    );
}
#elif defined(__IAR_SYSTEMS_ICC__)
__stackless static inline void
bl_run_main(uint32_t *vtor)
{
    __asm volatile (
          "    movw    r3, #0xED08\n"    // Store the vector table pointer of the new image into VTOR.
          "    movt    r3, #0xE000\n"
          "    str     r0, [r3, #0]\n"
          "    ldr     r3, [r0, #0]\n"   // Load the new stack pointer into R1 and the new reset vector into R2.
          "    ldr     r2, [r0, #4]\n"
          "    mov     sp, r3\n"         // Set the stack pointer for the new image.
          "    bx      r2\n"            // Jump to the new reset vector.
          );
}
#else
#error Compiler is unknown, please contact Ambiq support team
#endif

// Pre- SBLv2 known versions that do not support callback
static uint32_t sblPreV2[][4] = {
    // flash0, flash4, sblVersion, sblVersionAddInfo
    {0xA3007860, 0x2E2638FB, 0 , 0},
    {0xA3007E14, 0x5EE4E461, 1 , 0},
    {0xA3008290, 0xB49CECD5, 2 , 0},
};

//*****************************************************************************
//
//! @brief  Get Device Security Info
//!
//! @param  pSecInfo -  Pointer to structure for returned security info
//!
//! This will retrieve the security information for the device
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success
//
//*****************************************************************************
uint32_t am_hal_security_get_info(am_hal_security_info_t *pSecInfo)
{
    uint32_t flash0;
    uint32_t flash4;
    uint32_t i;
    bool     bSbl;
    if (!pSecInfo)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    pSecInfo->info0Version = AM_REGVAL(0x50020040);
    pSecInfo->bInfo0Valid = MCUCTRL->SHADOWVALID_b.INFO0_VALID;
    bSbl = MCUCTRL->BOOTLOADER_b.SECBOOTFEATURE;

    if (bSbl)
    {
        // Check if we're running pre-SBLv2
        flash0 = *g_pFlash0;
        flash4 = *g_pFlash4;
        // Check if SBL is installed
        if ((flash0 >> 24) != AM_IMAGE_MAGIC_SBL)
        {
            return AM_HAL_STATUS_FAIL;
        }
        for ( i = 0; i < sizeof(sblPreV2) / sizeof(sblPreV2[0]); i++ )
        {
            if ((sblPreV2[i][0] == flash0) && (sblPreV2[i][1] == flash4))
            {
                // This is a device prior to SBLv2
                pSecInfo->sblVersion = sblPreV2[i][2];
                pSecInfo->sblVersionAddInfo = sblPreV2[i][3];
                break;
            }
        }

        if ( i == sizeof(sblPreV2) / sizeof(sblPreV2[0]) )
        {
            // SBLv2 or beyond
            // Use SBL jump table function
            uint32_t status;
            uint32_t sblVersion;
            uint32_t (*pFuncVersion)(uint32_t *) = (uint32_t (*)(uint32_t *))(AM_HAL_SBL_ADDRESS + 0x1D1);
            status = pFuncVersion(&sblVersion);
            if (status != AM_HAL_STATUS_SUCCESS)
            {
                return status;
            }
            pSecInfo->sblVersion = sblVersion & 0x7FFF;
            pSecInfo->sblVersionAddInfo = sblVersion >> 15;
        }
    }
    else
    {
        return AM_HAL_STATUS_FAIL;
    }
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Set the key for specified lock
//!
//! @param  lockType - The lock type to be operated upon
//! @param  pKey -  Pointer to 128b key value
//!
//! This will program the lock registers for the specified lock and key
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success
//
//*****************************************************************************
uint32_t am_hal_security_set_key(am_hal_security_locktype_t lockType, am_hal_security_128bkey_t *pKey)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (pKey == NULL)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
    switch (lockType)
    {
        case AM_HAL_SECURITY_LOCKTYPE_CUSTOMER:
        case AM_HAL_SECURITY_LOCKTYPE_RECOVERY:
            break;
        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    SECURITY->LOCKCTRL = lockType;
    SECURITY->KEY0 = pKey->keys.key0;
    SECURITY->KEY1 = pKey->keys.key1;
    SECURITY->KEY2 = pKey->keys.key2;
    SECURITY->KEY3 = pKey->keys.key3;

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Get the current status of the specified lock
//!
//! @param  lockType - The lock type to be operated upon
//! @param  pbUnlockStatus -  Pointer to return variable with lock status
//!
//! This will get the lock status for specified lock - true implies unlocked
//! Note that except for customer lock, other locks are self-locking on status read
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success
//
//*****************************************************************************
uint32_t am_hal_security_get_lock_status(am_hal_security_locktype_t lockType, bool *pbUnlockStatus)
{
    uint32_t unlockMask;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (pbUnlockStatus == NULL)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION
    switch (lockType)
    {
        case AM_HAL_SECURITY_LOCKTYPE_CUSTOMER:
            unlockMask = AM_HAL_SECURITY_LOCKSTAT_CUSTOMER;
            break;
        case AM_HAL_SECURITY_LOCKTYPE_RECOVERY:
            unlockMask = AM_HAL_SECURITY_LOCKSTAT_RECOVERY;
            break;
        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }
    *pbUnlockStatus = SECURITY->LOCKSTAT & unlockMask;
    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//! @brief  Compute CRC32 for a specified payload
//!
//! @param  startAddr - The start address of the payload
//! @param  sizeBytes - The length of payload in bytes
//! @param  pCrc -  Pointer to return computed CRC
//!
//! This will use the hardware engine to compute CRC32 on an arbitrary data payload
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success
//
//*****************************************************************************
uint32_t am_hal_crc32(uint32_t startAddr, uint32_t sizeBytes, uint32_t *pCrc)
{
    uint32_t status;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (pCrc == NULL)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    // Make sure size is multiple of 4 bytes
    if (sizeBytes & 0x3)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
    // TODO - check the address
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Program the CRC engine to compute the crc
    //
    SECURITY->RESULT = 0xFFFFFFFF;
    SECURITY->SRCADDR = startAddr;
    SECURITY->LEN_b.LEN = (sizeBytes >> SECURITY_LEN_LEN_Pos);
    SECURITY->CTRL_b.FUNCTION = SECURITY_CTRL_FUNCTION_CRC32;
    // Start the CRC
    SECURITY->CTRL_b.ENABLE = 1;

    //
    // Wait for CRC to finish
    //
    status = am_hal_flash_delay_status_change(MAX_CRC_WAIT,
        (uint32_t)&SECURITY->CTRL, SECURITY_CTRL_ENABLE_Msk, 0);

    if (status == AM_HAL_STATUS_SUCCESS)
    {
        *pCrc = SECURITY->RESULT;
    }

    return status;
}

//*****************************************************************************
//
//! @brief  Helper function to Perform exit operations for a secondary bootloader
//!
//! @param  pImage - The address of the image to give control to
//!
//! This function does the necessary security operations while exiting from a
//! a secondary bootloader program. If still open, it locks the info0 key region,
//! as well as further updates to the flash protection register.
//! It also checks if it needs to halt to honor a debugger request.
//! If an image address is specified, control is transferred to the same on exit.
//!
//! @return Returns AM_HAL_STATUS_SUCCESS on success, if no image address specified
//! If an image address is provided, a successful execution results in transfer to
//! the image - and this function does not return.
//
//*****************************************************************************
uint32_t am_hal_bootloader_exit(uint32_t *pImage)
{
    uint32_t status = AM_HAL_STATUS_SUCCESS;

    //
    // Lock the assets
    //
    if ( MCUCTRL->SHADOWVALID_b.INFO0_VALID  &&
         MCUCTRL->BOOTLOADER_b.PROTLOCK )
    {
        am_hal_security_128bkey_t keyVal;
        uint32_t *pCustKey = (uint32_t *)0x50021A00;
        bool bLockStatus;

        //
        // PROTLOCK Open
        // This should also mean that Customer key is accessible
        // Now lock the key by writing an incorrect value
        //
        keyVal.keyword[0] = ~pCustKey[0];
        am_hal_security_set_key(AM_HAL_SECURITY_LOCKTYPE_CUSTOMER, &keyVal);

        status = am_hal_security_get_lock_status(AM_HAL_SECURITY_LOCKTYPE_CUSTOMER, &bLockStatus);

        if ((status != AM_HAL_STATUS_SUCCESS) || (bLockStatus))
        {
            return AM_HAL_STATUS_FAIL;
        }

        //
        // Lock the protection register to prevent further region locking
        // CAUTION!!! - Can not do AM_BFW on BOOTLOADER register as all writable bits in this register are Write 1 to clear
        //
        MCUCTRL->BOOTLOADER = _VAL2FLD(MCUCTRL_BOOTLOADER_PROTLOCK, 1);

        //
        // Check if we need to halt (debugger request)
        //
        if (MCUCTRL->SCRATCH0 & 0x1)
        {
            // Debugger wants to halt
            uint32_t dhcsr = AM_REGVAL(0xE000EDF0);
            // Clear the flag in Scratch register
            MCUCTRL->SCRATCH0 &= ~0x1;
            // Halt the core
            dhcsr = ((uint32_t)0xA05F << 16) | (dhcsr & 0xFFFF) | 0x3;
            AM_REGVAL(0xE000EDF0) = dhcsr;
            // Resume from halt
        }
    }

    // Give control to supplied image
    if (pImage)
    {
        bl_run_main(pImage);
        // Does not return
    }

    return status;
}
