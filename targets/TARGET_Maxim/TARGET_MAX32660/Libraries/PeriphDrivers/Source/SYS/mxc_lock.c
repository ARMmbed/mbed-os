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
#include "mxc_lock.h"

#if USE_LOCK_IN_DRIVERS

#ifndef __riscv
/* ************************************************************************** */
int MXC_GetLock(uint32_t* lock, uint32_t value)
{
    do {
    
        // Return if the lock is taken by a different thread
        if (__LDREXW((volatile unsigned long*) lock) != 0) {
            return E_BUSY;
        }
        
        // Attempt to take the lock
    }
    while (__STREXW(value, (volatile unsigned long*) lock) != 0);
    
    // Do not start any other memory access until memory barrier is complete
    __DMB();
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
void MXC_FreeLock(uint32_t* lock)
{
    // Ensure memory operations complete before releasing lock
    __DMB();
    *lock = 0;
}
#else // __riscv
/* ************************************************************************** */
int MXC_GetLock(uint32_t* lock, uint32_t value)
{   
    #warning "Unimplemented for RISCV"
    return E_NO_ERROR;
}

/* ************************************************************************** */
void MXC_FreeLock(uint32_t* lock)
{
    #warning "Unimplemented for RISCV"
}
#endif

#endif // USE_LOCK_IN_DRIVERS
