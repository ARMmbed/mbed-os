/**
 * @file flc.h
 * @brief      Flash Controler driver.
 * @details    This driver can be used to operate on the embedded flash memory.
 */
/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *
 *************************************************************************** */

/* **** Includes **** */
#include <string.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "flc.h"
#include "stdlib.h"

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
// Length is number of 32-bit words
int MXC_FLC_Com_VerifyData(uint32_t address, uint32_t length, uint32_t* data)
{
    volatile uint32_t* ptr;
    
    
    for (ptr = (uint32_t*) address; ptr < (((uint32_t*)(address)) + length); ptr++, data++) {
        if (*ptr != *data) {
            return E_BAD_STATE;
        }
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
// make sure to disable ICC with ICC_Disable(); before Running this function
int MXC_FLC_Com_Write(uint32_t address, uint32_t length, uint32_t* buffer)
{
    int err;
    uint32_t bytes_written;

    uint32_t current_data_32;
    uint8_t* current_data = (uint8_t*) &current_data_32;
    uint8_t* buffer8 = (uint8_t*)buffer;

    // Align the address to a word boundary and read/write if we have to
    if (address & 0x3) {
    
        // Figure out how many bytes we have to write to round up the address
        bytes_written = 4 - (address & 0x3);
        
        // Save the data currently in the flash
        memcpy(current_data, (void*)(address & (~0x3)), 4);
        
        // Modify current_data to insert the data from buffer
        memcpy(&current_data[4 - bytes_written], buffer8, bytes_written);
        
        // Write the modified data
        if ((err = MXC_FLC_Write32(address - (address % 4), current_data_32)) != E_NO_ERROR) {
            return err;
        }
        
        address += bytes_written;
        length -= bytes_written;
        buffer8 += bytes_written;
    }
    
    // Align the address to a 4-word (128bit) boundary
    while ((length >= 4) && ((address & 0xF) != 0)) {
        memcpy(current_data, buffer8, 4);
        if ((err = MXC_FLC_Write32(address, current_data_32)) != E_NO_ERROR) {
            return err;
        }
        
        address += 4;
        length -= 4;
        buffer8 += 4;
    }
    
    if (length >= 16) {
        uint32_t buff128[4];
        while (length >= 16) {
            memcpy(buff128, buffer8, 16);
            if ((err = MXC_FLC_Write128(address, buff128)) != E_NO_ERROR) {
                return err;
            }
            
            address += 16;
            length -= 16;
            buffer8 += 16;
        }
        
    }
    
    while (length >= 4) {
        memcpy(current_data, buffer8, 4);
        if ((err = MXC_FLC_Write32(address, current_data_32)) != E_NO_ERROR) {
            return err;
        }
        
        address += 4;
        length -= 4;
        buffer8 += 4;
    }
    
    if (length > 0) {
        // Save the data currently in the flash
        memcpy(current_data, (void*)(address), 4);
        
        // Modify current_data to insert the data from buffer
        memcpy(current_data, buffer8, length);
        
        if ((err = MXC_FLC_Write32(address, current_data_32)) != E_NO_ERROR) {
            return err;
        }
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
void MXC_FLC_Com_Read(int address, void* buffer, int len)
{
    memcpy(buffer, (void*) address, len);
}

