//*****************************************************************************
//
//  am_hal_ios.c
//! @file
//!
//! @brief Functions for interfacing with the IO Slave module
//!
//! @addtogroup ios3 IO Slave (SPI/I2C)
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

#define AM_HAL_IOS_MAX_SW_FIFO_SIZE 1023
#define AM_HAL_MAGIC_IOS            0x123456
#define AM_HAL_IOS_CHK_HANDLE(h)    ((h) && ((am_hal_handle_prefix_t *)(h))->s.bInit && (((am_hal_handle_prefix_t *)(h))->s.magic == AM_HAL_MAGIC_IOS))

//*****************************************************************************
//
// SRAM Buffer structure
//
//*****************************************************************************
am_hal_ios_buffer_t g_sSRAMBuffer;

//*****************************************************************************
//
// Private Types.
//
//*****************************************************************************
typedef struct
{
    bool        bValid;
    uint32_t    regFIFOCFG;
    uint32_t    regFIFOTHR;
    uint32_t    regCFG;
    uint32_t    regINTEN;
    uint32_t    regACCINTEN;
} am_hal_ios_register_state_t;

typedef struct
{
    am_hal_handle_prefix_t  prefix;
    //
    // Physical module number.
    //
    uint32_t                ui32Module;

    am_hal_ios_register_state_t registerState;

    uint8_t *pui8FIFOBase;
    uint8_t *pui8FIFOEnd;
    uint8_t *pui8FIFOPtr;
    uint8_t ui32HwFifoSize;
    uint32_t ui32FifoBaseOffset;
} am_hal_ios_state_t;

//*****************************************************************************
//
// Forward declarations of static funcitons.
//
//*****************************************************************************
static void am_hal_ios_buffer_init(am_hal_ios_buffer_t *psBuffer,
                                   void *pvArray, uint32_t ui32Bytes);
static void fifo_write(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes);
static uint32_t am_hal_ios_fifo_ptr_set(void *pHandle, uint32_t ui32Offset);
//*****************************************************************************
//
// Function-like macros.
//
//*****************************************************************************
#define am_hal_ios_buffer_empty(psBuffer)                                   \
    ((psBuffer)->ui32Length == 0)

#define am_hal_ios_buffer_full(psBuffer)                                    \
    ((psBuffer)->ui32Length == (psBuffer)->ui32Capacity)

#define am_hal_ios_buffer_data_left(psBuffer)                               \
    ((psBuffer)->ui32Length)

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
volatile uint8_t * const am_hal_ios_pui8LRAM = (uint8_t *)REG_IOSLAVE_BASEADDR;

am_hal_ios_state_t g_IOShandles[AM_REG_IOSLAVE_NUM_MODULES];

//*****************************************************************************
//
// IOS power control function
//
//*****************************************************************************
uint32_t am_hal_ios_power_ctrl(void *pHandle,
                               am_hal_sysctrl_power_state_e ePowerState,
                               bool bRetainState)
{
    am_hal_ios_state_t *pIOSState = (am_hal_ios_state_t*)pHandle;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !AM_HAL_IOS_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    //
    // Decode the requested power state and update IOS operation accordingly.
    //
    switch (ePowerState)
    {
        case AM_HAL_SYSCTRL_WAKE:
            if (bRetainState && !pIOSState->registerState.bValid)
            {
                return AM_HAL_STATUS_INVALID_OPERATION;
            }

            //
            // Enable power control.
            //
            am_hal_pwrctrl_periph_enable((am_hal_pwrctrl_periph_e)(AM_HAL_PWRCTRL_PERIPH_IOS + pIOSState->ui32Module));

            if (bRetainState)
            {
                //
                // Restore IOS registers
                IOSLAVEn(pIOSState->ui32Module)->FIFOCFG     = pIOSState->registerState.regFIFOCFG;
                IOSLAVEn(pIOSState->ui32Module)->FIFOTHR     = pIOSState->registerState.regFIFOTHR;
                IOSLAVEn(pIOSState->ui32Module)->CFG         = pIOSState->registerState.regCFG;
                IOSLAVEn(pIOSState->ui32Module)->INTEN       = pIOSState->registerState.regINTEN;
                IOSLAVEn(pIOSState->ui32Module)->REGACCINTEN = pIOSState->registerState.regACCINTEN;

                pIOSState->registerState.bValid = false;
            }
            break;

        case AM_HAL_SYSCTRL_NORMALSLEEP:
        case AM_HAL_SYSCTRL_DEEPSLEEP:
            if (bRetainState)
            {
                // Save IOS Registers
                pIOSState->registerState.regFIFOCFG    = IOSLAVEn(pIOSState->ui32Module)->FIFOCFG;
                pIOSState->registerState.regFIFOTHR    = IOSLAVEn(pIOSState->ui32Module)->FIFOTHR;
                pIOSState->registerState.regCFG        = IOSLAVEn(pIOSState->ui32Module)->CFG;
                pIOSState->registerState.regINTEN      = IOSLAVEn(pIOSState->ui32Module)->INTEN;
                pIOSState->registerState.regACCINTEN   = IOSLAVEn(pIOSState->ui32Module)->REGACCINTEN;
                pIOSState->registerState.bValid = true;
            }

            //
            // Disable power control.
            //
            am_hal_pwrctrl_periph_disable((am_hal_pwrctrl_periph_e)(AM_HAL_PWRCTRL_PERIPH_IOS + pIOSState->ui32Module));
            break;

        default:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    //
    // Return the status.
    //
    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_power_ctrl()

//*****************************************************************************
//
// IOS uninitialize function
//
//*****************************************************************************
uint32_t am_hal_ios_uninitialize(void *pHandle)
{
    am_hal_ios_state_t *pIOSState = (am_hal_ios_state_t*)pHandle;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if (pIOSState->prefix.s.bEnable)
    {
        am_hal_ios_disable(pHandle);
    }

    pIOSState->prefix.s.bInit = false;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_uninitialize()


//*****************************************************************************
//
// IOS initialization function
//
//*****************************************************************************
uint32_t am_hal_ios_initialize(uint32_t ui32Module, void **ppHandle)
{
#ifndef AM_HAL_DISABLE_API_VALIDATION
    //
    // Validate the module number
    //
    if ( ui32Module >= AM_REG_IOSLAVE_NUM_MODULES )
    {
        return AM_HAL_STATUS_OUT_OF_RANGE;
    }

    if (ppHandle == NULL)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }

    if (g_IOShandles[ui32Module].prefix.s.bInit)
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    g_IOShandles[ui32Module].prefix.s.bInit = true;
    g_IOShandles[ui32Module].prefix.s.bEnable = false;
    g_IOShandles[ui32Module].prefix.s.magic = AM_HAL_MAGIC_IOS;

    //
    // Initialize the handle.
    //
    g_IOShandles[ui32Module].ui32Module = ui32Module;

    //
    // Return the handle.
    //
    *ppHandle = (void *)&g_IOShandles[ui32Module];

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_initialize()

//*****************************************************************************
//
// IOS enable function
//
//*****************************************************************************
uint32_t am_hal_ios_enable(void *pHandle)
{
    am_hal_ios_state_t *pIOSState = (am_hal_ios_state_t*)pHandle;
#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    if (pIOSState->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_SUCCESS;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    IOSLAVEn(pIOSState->ui32Module)->CFG |= _VAL2FLD(IOSLAVE_CFG_IFCEN, 1);

    pIOSState->prefix.s.bEnable = true;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_enable()

//*****************************************************************************
//
// IOS disable function
//
//*****************************************************************************
uint32_t am_hal_ios_disable(void *pHandle)
{
    am_hal_ios_state_t *pIOSState = (am_hal_ios_state_t*)pHandle;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    if (!pIOSState->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_SUCCESS;
    }

    IOSLAVEn(pIOSState->ui32Module)->CFG &= ~(_VAL2FLD(IOSLAVE_CFG_IFCEN, 1));

    pIOSState->prefix.s.bEnable = false;

    return AM_HAL_STATUS_SUCCESS;

} // am_hal_ios_disable()

//*****************************************************************************
//
// IOS configuration function.
//
//*****************************************************************************
uint32_t am_hal_ios_configure(void *pHandle, am_hal_ios_config_t *psConfig)
{
    uint32_t ui32LRAMConfig = 0;
    am_hal_ios_state_t *pIOSState = (am_hal_ios_state_t*)pHandle;
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Validate the parameters
    //
    if ( (psConfig == NULL) ||
         (pIOSState->ui32Module >= AM_REG_IOSLAVE_NUM_MODULES) )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
    // Configure not allowed in Enabled state
    if (pIOSState->prefix.s.bEnable)
    {
        return AM_HAL_STATUS_INVALID_OPERATION;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = pIOSState->ui32Module;

    am_hal_pwrctrl_periph_enable(AM_HAL_PWRCTRL_PERIPH_IOS);

    //
    // Record the FIFO parameters for later use.
    //
    pIOSState->pui8FIFOBase = (uint8_t *)(REG_IOSLAVE_BASEADDR + psConfig->ui32FIFOBase);
    pIOSState->pui8FIFOEnd = (uint8_t *)(REG_IOSLAVE_BASEADDR + psConfig->ui32RAMBase);
    pIOSState->ui32HwFifoSize = pIOSState->pui8FIFOEnd - pIOSState->pui8FIFOBase;
    pIOSState->ui32FifoBaseOffset = psConfig->ui32FIFOBase;

    //
    // Initialize the global SRAM buffer
    // Total size, which is SRAM Buffer plus the hardware FIFO needs to be
    // limited to 1023
    //
    if ( psConfig->ui32SRAMBufferCap > (AM_HAL_IOS_MAX_SW_FIFO_SIZE - pIOSState->ui32HwFifoSize + 1) )
    {
        psConfig->ui32SRAMBufferCap = (AM_HAL_IOS_MAX_SW_FIFO_SIZE - pIOSState->ui32HwFifoSize + 1);
    }
    am_hal_ios_buffer_init(&g_sSRAMBuffer, psConfig->pui8SRAMBuffer, psConfig->ui32SRAMBufferCap);

    //
    // Calculate the value for the IO Slave FIFO configuration register.
    //
    ui32LRAMConfig  = _VAL2FLD(IOSLAVE_FIFOCFG_ROBASE,   psConfig->ui32ROBase >> 3);
    ui32LRAMConfig |= _VAL2FLD(IOSLAVE_FIFOCFG_FIFOBASE, psConfig->ui32FIFOBase >> 3);
    ui32LRAMConfig |= _VAL2FLD(IOSLAVE_FIFOCFG_FIFOMAX,  psConfig->ui32RAMBase >> 3);

    //
    // Just in case, disable the IOS
    //
    am_hal_ios_disable(pHandle);

    //
    // Write the configuration register with the user's selected interface
    // characteristics.
    //
    IOSLAVEn(ui32Module)->CFG = psConfig->ui32InterfaceSelect;

    //
    // Write the FIFO configuration register to set the memory map for the LRAM.
    //
    IOSLAVEn(ui32Module)->FIFOCFG = ui32LRAMConfig;

    //
    // Clear the FIFO State
    //
    IOSLAVEn(pIOSState->ui32Module)->FIFOCTR_b.FIFOCTR = 0x0;
    IOSLAVEn(pIOSState->ui32Module)->FIFOPTR_b.FIFOSIZ = 0x0;
    am_hal_ios_fifo_ptr_set(pHandle, pIOSState->ui32FifoBaseOffset);

    //
    // Enable the IOS. The following configuration options can't be set while
    // the IOS is disabled.
    //
    am_hal_ios_enable(pHandle);

    //
    // Initialize the FIFO pointer to the beginning of the FIFO section.
    //
    am_hal_ios_fifo_ptr_set(pHandle, psConfig->ui32FIFOBase);

    //
    // Write the FIFO threshold register.
    //
    IOSLAVEn(ui32Module)->FIFOTHR = psConfig->ui32FIFOThreshold;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_config()

//*****************************************************************************
//
// IOS enable interrupts function
//
//*****************************************************************************
uint32_t am_hal_ios_interrupt_enable(void *pHandle, uint32_t ui32IntMask)
{
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // OR the desired interrupt into the enable register.
    //
    IOSLAVEn(ui32Module)->INTEN |= ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_int_enable()

//*****************************************************************************
//
// IOS disable interrupts function
//
//*****************************************************************************
uint32_t am_hal_ios_interrupt_disable(void *pHandle, uint32_t ui32IntMask)
{
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // Clear the desired bit from the interrupt enable register.
    //
    IOSLAVEn(ui32Module)->INTEN &= ~(ui32IntMask);

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_int_disable()

//*****************************************************************************
//
// IOS interrupt clear
//
//*****************************************************************************
uint32_t am_hal_ios_interrupt_clear(void *pHandle, uint32_t ui32IntMask)
{
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // Use the interrupt clear register to deactivate the chosen interrupt.
    //
    IOSLAVEn(ui32Module)->INTCLR = ui32IntMask;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_int_clear()

//*****************************************************************************
//
// IOS get interrupt status
//
//*****************************************************************************
uint32_t am_hal_ios_interrupt_status_get(void *pHandle, bool bEnabledOnly,
                                         uint32_t *pui32IntStatus)
{
    uint32_t ui32IntStatus = 0;
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if ( !AM_HAL_IOS_CHK_HANDLE(pHandle) )
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    if ( !pui32IntStatus )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    ui32IntStatus = IOSLAVEn(ui32Module)->INTSTAT;

    if ( bEnabledOnly )
    {
        ui32IntStatus &= IOSLAVEn(ui32Module)->INTEN;
    }

    *pui32IntStatus = ui32IntStatus;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_int_status_get()

//*****************************************************************************
//
//! @brief Check the amount of space used in the FIFO
//!
//! @param pui32UsedSpace is bytes used in the Overall FIFO.
//!
//! This function returns the available data in the overall FIFO yet to be
//! read by the host. This takes into account the SRAM buffer and hardware FIFO
//!
//! @return success or error code
//
//*****************************************************************************
uint32_t am_hal_ios_fifo_space_used(void *pHandle, uint32_t *pui32UsedSpace)
{
    uint32_t ui32Module;
    uint32_t ui32Val = 0;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if ( !pui32UsedSpace )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // Start a critical section for thread safety.
    //
    AM_CRITICAL_BEGIN

    ui32Val = g_sSRAMBuffer.ui32Length;
    ui32Val += IOSLAVEn(ui32Module)->FIFOPTR_b.FIFOSIZ;

    //
    // End the critical section
    //
    AM_CRITICAL_END

    *pui32UsedSpace = ui32Val;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_fifo_space_used()

//*****************************************************************************
//
//! @brief Check the amount of space left in the FIFO
//!
//! @param pui32LeftSpace is bytes left in the Overall FIFO.
//!
//! This function returns the available space in the overall FIFO to accept
//! new data. This takes into account the SRAM buffer and hardware FIFO
//!
//! @return success or error code
//
//*****************************************************************************
uint32_t am_hal_ios_fifo_space_left(void *pHandle, uint32_t *pui32LeftSpace)
{
    uint32_t ui32Module;
    uint32_t ui32Val = 0;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if ( !pui32LeftSpace )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // Start a critical section for thread safety.
    //
    AM_CRITICAL_BEGIN

    //
    // We waste one byte in HW FIFO
    //
    ui32Val = g_sSRAMBuffer.ui32Capacity + ((am_hal_ios_state_t*)pHandle)->ui32HwFifoSize - 1;
    ui32Val -= g_sSRAMBuffer.ui32Length;
    ui32Val -= IOSLAVEn(ui32Module)->FIFOPTR_b.FIFOSIZ;

    //
    // End the critical section
    //
    AM_CRITICAL_END

    *pui32LeftSpace = ui32Val;

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_fifo_space_left()

//*****************************************************************************
//
//! @brief Check the amount of space left in the hardware FIFO
//!
//! @param pui32LeftSpace is bytes left in the IOS FIFO.
//!
//! This function reads the IOSLAVE FIFOPTR register and determines the amount
//! of space left in the IOS LRAM FIFO.
//!
//! @return success or error code
//
//*****************************************************************************
static uint32_t fifo_space_left(void *pHandle, uint32_t *pui32LeftSpace)
{
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if ( !pui32LeftSpace )
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // We waste one byte in HW FIFO
    //
    *pui32LeftSpace = ((uint32_t)((am_hal_ios_state_t*)pHandle)->ui32HwFifoSize - IOSLAVEn(ui32Module)->FIFOPTR_b.FIFOSIZ - 1);

    return AM_HAL_STATUS_SUCCESS;
} // fifo_space_left()

//*****************************************************************************
//
// Helper function for managing IOS FIFO writes.
//
//*****************************************************************************
static void fifo_write(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes)
{
    am_hal_ios_state_t *pIOSState = (am_hal_ios_state_t*)pHandle;
    uint8_t *pFifoPtr = pIOSState->pui8FIFOPtr;
    uint8_t *pFifoBase = pIOSState->pui8FIFOBase;
    uint8_t *pFifoEnd = pIOSState->pui8FIFOEnd;

    while ( ui32NumBytes )
    {
        //
        // Write the data to the FIFO
        //
        *pFifoPtr++ = *pui8Data++;
        ui32NumBytes--;

        //
        // Make sure to wrap the FIFO pointer if necessary.
        //
        if ( pFifoPtr == pFifoEnd )
        {
            pFifoPtr = pFifoBase;
        }
    }
    pIOSState->pui8FIFOPtr = pFifoPtr;
} // fifo_write()

//*****************************************************************************
//
// IOS interrupt service routine
//
//*****************************************************************************
uint32_t am_hal_ios_interrupt_service(void *pHandle, uint32_t ui32IntMask)
{
    uint32_t thresh;
    uint32_t freeSpace, usedSpace, chunk1, chunk2, ui32WriteIndex;
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // Check for FIFO size interrupts.
    //
    if ( ui32IntMask & AM_HAL_IOS_INT_FSIZE )
    {
        thresh = IOSLAVEn(ui32Module)->FIFOTHR_b.FIFOTHR;

        //
        // While the FIFO is at or below threshold Add more data
        // If Fifo level is above threshold, we're guaranteed an FSIZ interrupt
        //
        while ( g_sSRAMBuffer.ui32Length &&
                ((usedSpace = IOSLAVEn(ui32Module)->FIFOPTR_b.FIFOSIZ) <= thresh) )
        {
            //
            // So, we do have some data in SRAM which needs to be moved to FIFO.
            // A chunk of data is a continguous set of bytes in SRAM that can be
            //  written to FIFO. Determine the chunks of data from SRAM that can
            //  be written. Up to two chunks possible
            //
            ui32WriteIndex = g_sSRAMBuffer.ui32WriteIndex;
            chunk1 = ((ui32WriteIndex > (uint32_t)g_sSRAMBuffer.ui32ReadIndex) ?   \
                        (ui32WriteIndex - (uint32_t)g_sSRAMBuffer.ui32ReadIndex) : \
                        (g_sSRAMBuffer.ui32Capacity - (uint32_t)g_sSRAMBuffer.ui32ReadIndex));
            chunk2 = g_sSRAMBuffer.ui32Length - chunk1;
            // We waste one byte in HW FIFO
            freeSpace = ((am_hal_ios_state_t*)pHandle)->ui32HwFifoSize - usedSpace - 1;
            // Write data in chunks
            // Determine the chunks of data from SRAM that can be written
            if ( chunk1 > freeSpace )
            {
                fifo_write(pHandle, (uint8_t *)(g_sSRAMBuffer.pui8Data + g_sSRAMBuffer.ui32ReadIndex), freeSpace);
                //
                // Advance the read index, wrapping if needed.
                //
                g_sSRAMBuffer.ui32ReadIndex += freeSpace;
                // No need to check for wrap as we wrote less than chunk1
                //
                // Adjust the length value to reflect the change.
                //
                g_sSRAMBuffer.ui32Length -= freeSpace;
            }
            else
            {
                fifo_write(pHandle, (uint8_t *)(g_sSRAMBuffer.pui8Data + g_sSRAMBuffer.ui32ReadIndex), chunk1);

                //
                // Update the read index - wrapping as needed
                //
                g_sSRAMBuffer.ui32ReadIndex += chunk1;
                g_sSRAMBuffer.ui32ReadIndex %= g_sSRAMBuffer.ui32Capacity;
                //
                // Adjust the length value to reflect the change.
                //
                g_sSRAMBuffer.ui32Length -= chunk1;
                freeSpace -= chunk1;

                if ( freeSpace && chunk2 )
                {
                    if ( chunk2 > freeSpace )
                    {
                        fifo_write(pHandle, (uint8_t *)(g_sSRAMBuffer.pui8Data + g_sSRAMBuffer.ui32ReadIndex), freeSpace);

                        //
                        // Advance the read index, wrapping if needed.
                        //
                        g_sSRAMBuffer.ui32ReadIndex += freeSpace;

                        // No need to check for wrap in chunk2
                        //
                        // Adjust the length value to reflect the change.
                        //
                        g_sSRAMBuffer.ui32Length -= freeSpace;
                    }
                    else
                    {
                        fifo_write(pHandle, (uint8_t *)(g_sSRAMBuffer.pui8Data + g_sSRAMBuffer.ui32ReadIndex), chunk2);
                        //
                        // Advance the read index, wrapping if needed.
                        //
                        g_sSRAMBuffer.ui32ReadIndex += chunk2;

                        // No need to check for wrap in chunk2
                        //
                        // Adjust the length value to reflect the change.
                        //
                        g_sSRAMBuffer.ui32Length -= chunk2;
                    }
                }
            }

            //
            // Need to retake the FIFO space, after Threshold interrupt has been reenabled
            // Clear any spurious FSIZE interrupt that might have got raised
            //
            IOSLAVEn(ui32Module)->INTCLR_b.FSIZE = 1;
        }
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_fifo_service()

//*****************************************************************************
//
//! @brief Writes the specified number of bytes to the IOS fifo.
//!
//! @param pui8Data is a pointer to the data to be written to the fifo.
//! @param ui32NumBytes is the number of bytes to send.
//! @param pui32WrittenBytes is number of bytes written (could be less than ui32NumBytes, if not enough space)
//!
//! This function will write data from the caller-provided array to the IOS
//! LRAM FIFO. If there is no space in the LRAM FIFO, the data will be copied
//! to a temporary SRAM buffer instead.
//!
//! The maximum message size for the IO Slave is 1023 bytes.
//!
//! @note In order for SRAM copy operations in the function to work correctly,
//! the \e am_hal_ios_buffer_service() function must be called in the ISR for
//! the ioslave module.
//!
//! @return success or error code
//
//*****************************************************************************
uint32_t am_hal_ios_fifo_write(void *pHandle, uint8_t *pui8Data, uint32_t ui32NumBytes, uint32_t *pui32WrittenBytes)
{
    uint32_t ui32FIFOSpace = 0;
    uint32_t ui32SRAMSpace;
    uint32_t ui32SRAMLength;
    uint32_t totalBytes = ui32NumBytes;
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
    if ( !pui8Data || !pui32WrittenBytes)
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // This operation will only work properly if an SRAM buffer has been
    // allocated. Make sure that am_hal_ios_fifo_buffer_init() has been called,
    // and the buffer pointer looks valid.
    //
    am_hal_debug_assert(g_sSRAMBuffer.pui8Data != 0);

    if ( ui32NumBytes == 0 )
    {
        *pui32WrittenBytes = 0;
    }
    else
    {
        //
        // Start a critical section for thread safety.
        //
        AM_CRITICAL_BEGIN

        ui32SRAMLength = g_sSRAMBuffer.ui32Length;

        //
        // End the critical section
        //
        AM_CRITICAL_END

        //
        // If the SRAM buffer is empty, we should just write directly to the FIFO.
        //
        if ( ui32SRAMLength == 0 )
        {
            fifo_space_left(pHandle, &ui32FIFOSpace);

            //
            // If the whole message fits, send it now.
            //
            if ( ui32NumBytes <= ui32FIFOSpace )
            {
                fifo_write(pHandle, pui8Data, ui32NumBytes);
                ui32NumBytes = 0;
            }
            else
            {
                fifo_write(pHandle, pui8Data, ui32FIFOSpace);
                ui32NumBytes -= ui32FIFOSpace;
                pui8Data += ui32FIFOSpace;
            }
        }

        //
        // If there's still data, write it to the SRAM buffer.
        //
        if ( ui32NumBytes )
        {
            uint32_t idx, writeIdx, capacity, fifoSize;
            ui32SRAMSpace = g_sSRAMBuffer.ui32Capacity - ui32SRAMLength;

            writeIdx = g_sSRAMBuffer.ui32WriteIndex;
            capacity = g_sSRAMBuffer.ui32Capacity;

            //
            // Make sure that the data will fit inside the SRAM buffer.
            //
            if ( ui32SRAMSpace > ui32NumBytes )
            {
                ui32SRAMSpace = ui32NumBytes;
            }

            //
            // If the data will fit, write it to the SRAM buffer.
            //
            for ( idx = 0; idx < ui32SRAMSpace; idx++ )
            {
                g_sSRAMBuffer.pui8Data[(idx + writeIdx) % capacity] = pui8Data[idx];
            }

            ui32NumBytes -= idx;

            //
            // Start a critical section for thread safety before updating length & wrIdx.
            //
            AM_CRITICAL_BEGIN

            //
            // Advance the write index, making sure to wrap if necessary.
            //
            g_sSRAMBuffer.ui32WriteIndex = (idx + writeIdx) % capacity;

            //
            // Update the length value appropriately.
            //
            g_sSRAMBuffer.ui32Length += idx;

            //
            // End the critical section
            //
            AM_CRITICAL_END

            // It is possible that there is a race condition that the FIFO level has
            // gone below the threshold by the time we set the wrIdx above, and hence
            // we may never get the threshold interrupt to serve the SRAM data we
            // just wrote

            // If that is the case, explicitly generate the FSIZE interrupt from here
            fifoSize = IOSLAVEn(ui32Module)->FIFOPTR_b.FIFOSIZ;

            if ( fifoSize <= IOSLAVEn(ui32Module)->FIFOTHR_b.FIFOTHR )
            {
                IOSLAVEn(ui32Module)->INTSET_b.FSIZE = 1;
            }
        }

        *pui32WrittenBytes = totalBytes - ui32NumBytes;
    }

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_fifo_write()

//*****************************************************************************
//
//! @brief Sets the IOS FIFO pointer to the specified LRAM offset.
//!
//! @param ui32Offset is LRAM offset to set the FIFO pointer to.
//!
//! @return success or error code
//
//*****************************************************************************
static uint32_t am_hal_ios_fifo_ptr_set(void *pHandle, uint32_t ui32Offset)
{
    uint32_t ui32Module;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    ui32Module = ((am_hal_ios_state_t*)pHandle)->ui32Module;

    //
    // Start a critical section for thread safety.
    //
    AM_CRITICAL_BEGIN

    //
    // Set the FIFO Update bit.
    //
    IOSLAVEn(ui32Module)->FUPD = 0x1;

    //
    // Change the FIFO offset.
    //
    IOSLAVEn(ui32Module)->FIFOPTR = ui32Offset;

    //
    // Clear the FIFO update bit.
    //
    IOSLAVEn(ui32Module)->FUPD = 0x0;

    //
    // Set the global FIFO-pointer tracking variable.
    //
    ((am_hal_ios_state_t*)pHandle)->pui8FIFOPtr = (uint8_t *) (REG_IOSLAVE_BASEADDR + ui32Offset);

    //
    // End the critical section.
    //
    AM_CRITICAL_END

    return AM_HAL_STATUS_SUCCESS;
} // am_hal_ios_fifo_ptr_set()

//*****************************************************************************
//
// Initialize an SRAM buffer for use with the IO Slave.
//
//*****************************************************************************
static void am_hal_ios_buffer_init(am_hal_ios_buffer_t *psBuffer, void *pvArray,
                                   uint32_t ui32Bytes)
{
    psBuffer->ui32WriteIndex = 0;
    psBuffer->ui32ReadIndex = 0;
    psBuffer->ui32Length = 0;
    psBuffer->ui32Capacity = ui32Bytes;
    psBuffer->pui8Data = (uint8_t *)pvArray;
} // am_hal_ios_buffer_init()

//*****************************************************************************
//
//! @brief IOS control function
//!
//! @param handle       - handle for the IOS.
//! @param eReq         - device specific special request code.
//! @param pArgs        - pointer to the request specific arguments.
//!
//! This function allows advanced settings
//!
//! @return success or error code
//
//*****************************************************************************
uint32_t am_hal_ios_control(void *pHandle, am_hal_ios_request_e eReq, void *pArgs)
{
    am_hal_ios_state_t *pIOSState = (am_hal_ios_state_t*)pHandle;
    uint32_t ui32Val = 0;

#ifndef AM_HAL_DISABLE_API_VALIDATION
    if (!AM_HAL_IOS_CHK_HANDLE(pHandle))
    {
        return AM_HAL_STATUS_INVALID_HANDLE;
    }

    //
    // Validate the parameters
    //
    if ((eReq < AM_HAL_IOS_REQ_ARG_MAX) && (NULL == pArgs))
    {
        return AM_HAL_STATUS_INVALID_ARG;
    }
#endif // AM_HAL_DISABLE_API_VALIDATION

    switch (eReq)
    {
        case AM_HAL_IOS_REQ_HOST_INTSET:
            IOSLAVEn(pIOSState->ui32Module)->IOINTCTL = _VAL2FLD(IOSLAVE_IOINTCTL_IOINTSET, *((uint32_t *)pArgs));
            break;
        case AM_HAL_IOS_REQ_HOST_INTCLR:
            IOSLAVEn(pIOSState->ui32Module)->IOINTCTL = _VAL2FLD(IOSLAVE_IOINTCTL_IOINTCLR, *((uint32_t *)pArgs));
            break;
        case AM_HAL_IOS_REQ_HOST_INTGET:
            *((uint32_t*)pArgs) = IOSLAVEn(pIOSState->ui32Module)->IOINTCTL_b.IOINT;
            break;
        case AM_HAL_IOS_REQ_HOST_INTEN_GET:
            *((uint32_t*)pArgs) = IOSLAVEn(pIOSState->ui32Module)->IOINTCTL_b.IOINTEN;
            break;
        case AM_HAL_IOS_REQ_READ_GADATA:
            *((uint32_t*)pArgs) = IOSLAVEn(pIOSState->ui32Module)->GENADD_b.GADATA;
            break;
        case AM_HAL_IOS_REQ_READ_POLL:
            while ( IOSLAVEn(pIOSState->ui32Module)->FUPD & IOSLAVE_FUPD_IOREAD_Msk );
            break;
        case AM_HAL_IOS_REQ_FIFO_UPDATE_CTR:
            am_hal_ios_fifo_space_used(pHandle, &ui32Val);
            IOSLAVEn(pIOSState->ui32Module)->FIFOCTR_b.FIFOCTR = ui32Val;
            break;
        case AM_HAL_IOS_REQ_FIFO_BUF_CLR:
            am_hal_ios_buffer_init(&g_sSRAMBuffer, NULL, 0);
            //
            // Clear the FIFO State
            //
            IOSLAVEn(pIOSState->ui32Module)->FIFOCTR_b.FIFOCTR = 0x0;
            IOSLAVEn(pIOSState->ui32Module)->FIFOPTR_b.FIFOSIZ = 0x0;
            break;
        case AM_HAL_IOS_REQ_MAX:
            return AM_HAL_STATUS_INVALID_ARG;
    }

    return AM_HAL_STATUS_SUCCESS;
}

//*****************************************************************************
//
//  End the doxygen group
//! @}
//
//*****************************************************************************
