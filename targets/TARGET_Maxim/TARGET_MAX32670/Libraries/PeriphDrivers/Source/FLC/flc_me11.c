/**
 * @file flc.h
 * @brief      Flash Controler driver.
 * @details    This driver can be used to operate on the embedded flash memory.
 */
/* ****************************************************************************
 * Copyright(C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files(the "Software"), 
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
#include "flc_reva.h"
#include "flc_common.h"

//******************************************************************************
void MXC_FLC_ME11_Flash_Operation(void)
{
    /* Flush all instruction caches */
    MXC_GCR->scon |= MXC_F_GCR_SCON_ICC0_FLUSH;
    
    /* Wait for flush to complete */
    while(MXC_GCR->scon & MXC_F_GCR_SCON_ICC0_FLUSH) {
    }
}

//******************************************************************************
int MXC_FLC_ME11_GetByAddress(mxc_flc_regs_t **flc, uint32_t addr)
{

    // flash base start from 0x00000000
    if ( addr < MXC_FLASH_MEM_SIZE ) { 
        *flc = MXC_FLC;
    }
    else if((addr >= MXC_INFO_MEM_BASE) && (addr <(MXC_INFO_MEM_BASE + MXC_INFO_MEM_SIZE))) {
        *flc = MXC_FLC;
    }
    else {
        return E_BAD_PARAM;
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
int MXC_FLC_ME11_GetPhysicalAddress(uint32_t addr, uint32_t *result)
{
    // flash base start from 0x00000000
    if ( addr < MXC_FLASH_MEM_SIZE ) {
        *result = addr & (MXC_FLASH_MEM_SIZE-1);
    }
    else if((addr >= MXC_INFO_MEM_BASE) && (addr <(MXC_INFO_MEM_BASE + MXC_INFO_MEM_SIZE))) {
        *result = (addr & (MXC_INFO_MEM_SIZE-1)) + MXC_FLASH_MEM_SIZE;
    }
    else {
        return E_BAD_PARAM;
    }
    
    return E_NO_ERROR;
}

//******************************************************************************

int MXC_FLC_Init()
{
    return E_NO_ERROR;
}

#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
int MXC_FLC_Busy(void)
{
    return MXC_FLC_RevA_Busy();
}

#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
int MXC_FLC_ME11_PageErase(uint32_t address)
{
    int err;
    uint32_t addr;
    mxc_flc_regs_t *flc = NULL;
    
    // Get FLC Instance
    if((err = MXC_FLC_ME11_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    
    if((err = MXC_FLC_ME11_GetPhysicalAddress(address, &addr)) < E_NO_ERROR) {
        return err;
    }
    
    err = MXC_FLC_RevA_PageErase((mxc_flc_reva_regs_t*) flc, addr);
    // Flush the cache
    MXC_FLC_ME11_Flash_Operation();
    
    return err;
}

#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
// make sure to disable ICC with ICC_Disable(); before Running this function
int MXC_FLC_ME11_Write128(uint32_t address, uint32_t *data)
{
    int err;
    mxc_flc_regs_t *flc = NULL;
    uint32_t addr;
    
    // Address checked if it is 128-bit aligned
    if(address & 0xF) {
        return E_BAD_PARAM;
    }
    
    // Get FLC Instance
    if((err = MXC_FLC_ME11_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    
    if((err = MXC_FLC_ME11_GetPhysicalAddress(address, &addr)) < E_NO_ERROR) {
        return err;
    }
    
    if((err = MXC_FLC_RevA_Write128((mxc_flc_reva_regs_t*) flc, addr, data)) != E_NO_ERROR) {
        return err;
    }
    
    // Flush the cache
    MXC_FLC_ME11_Flash_Operation();
    
    if((err= MXC_FLC_Com_VerifyData(address, 4, data)) !=E_NO_ERROR) {
        return err;
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
int MXC_FLC_ME11_Write32(uint32_t address, uint32_t data)
{
    uint32_t addr, aligned;
    int err;
    mxc_flc_regs_t *flc = NULL;
    
    // Address checked if it is byte addressable
    if(address & 0x3) {
        return E_BAD_PARAM;
    }
    
    // Align address to 128-bit word
    aligned = address & 0xfffffff0;
    
    // Get FLC Instance
    if((err = MXC_FLC_ME11_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    
    if((err = MXC_FLC_ME11_GetPhysicalAddress(aligned, &addr)) < E_NO_ERROR) {
        return err;
    }
    
    return MXC_FLC_RevA_Write32((mxc_flc_reva_regs_t*) flc, address, data, addr);
    
}

int MXC_FLC_ME11_MassErase(void)
{
    int err;
    mxc_flc_regs_t *flc;
    
    flc = MXC_FLC;
    err = MXC_FLC_RevA_MassErase((mxc_flc_reva_regs_t*) flc);

    if(err != E_NO_ERROR) {
        return err;
    }
        
    MXC_FLC_ME11_Flash_Operation();
    
    return E_NO_ERROR;
    
    
}
int MXC_FLC_ME11_UnlockInfoBlock(uint32_t address)
{
    int err;
    mxc_flc_regs_t *flc;
    
    if((err = MXC_FLC_ME11_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    
    return MXC_FLC_RevA_UnlockInfoBlock((mxc_flc_reva_regs_t*) flc, address);
}
int MXC_FLC_ME11_LockInfoBlock(uint32_t address)
{
    int err;
    mxc_flc_regs_t *flc;
    
    if((err = MXC_FLC_ME11_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    
    return MXC_FLC_RevA_LockInfoBlock((mxc_flc_reva_regs_t*) flc, address);
}
/* ****************************************************************************** */
int MXC_FLC_MassErase(void)
{
    return MXC_FLC_ME11_MassErase();
}

int MXC_FLC_PageErase(uint32_t address)
{
    return MXC_FLC_ME11_PageErase(address);
}

int MXC_FLC_Write32(uint32_t address, uint32_t data)
{
    return MXC_FLC_ME11_Write32(address, data);
}

int MXC_FLC_Write128(uint32_t address, uint32_t *data)
{
    return MXC_FLC_ME11_Write128(address, data);
}

int MXC_FLC_Write(uint32_t address, uint32_t length, uint32_t *buffer)
{
    return MXC_FLC_Com_Write(address, length, buffer);
}
void MXC_FLC_Read(int address, void* buffer, int len)
{
    MXC_FLC_Com_Read(address, buffer, len);
}

int MXC_FLC_EnableInt(uint32_t flags)
{
    return MXC_FLC_RevA_EnableInt(flags);
}

int MXC_FLC_DisableInt(uint32_t flags)
{
    return MXC_FLC_RevA_DisableInt(flags);
}

int MXC_FLC_GetFlags(void)
{
    return MXC_FLC_RevA_GetFlags();
}

int MXC_FLC_ClearFlags(uint32_t flags)
{
    return MXC_FLC_RevA_ClearFlags(flags);
}

int MXC_FLC_UnlockInfoBlock(uint32_t address)
{
    return MXC_FLC_ME11_UnlockInfoBlock(address);
}

int MXC_FLC_LockInfoBlock(uint32_t address)
{
    return MXC_FLC_ME11_LockInfoBlock(address);
}
