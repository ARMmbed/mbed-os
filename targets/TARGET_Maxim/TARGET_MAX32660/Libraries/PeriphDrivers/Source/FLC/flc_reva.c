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
#include "flc_reva.h"
#include "flc.h"

/**
 * @ingroup flc
 * @{
 */

/* **** Definitions **** */

/* **** Globals **** */

/* **** Functions **** */

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
static int MXC_busy_flc(mxc_flc_reva_regs_t* flc)
{
    return (flc->ctrl & (MXC_F_FLC_REVA_CTRL_WR | MXC_F_FLC_REVA_CTRL_ME | MXC_F_FLC_REVA_CTRL_PGE));
}

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
static int MXC_prepare_flc(mxc_flc_reva_regs_t* flc)
{
    /* Check if the flash controller is busy */
    if (MXC_busy_flc(flc)) {
        return E_BUSY;
    }
    
    // Set flash clock divider to generate a 1MHz clock from the APB clock
    flc->clkdiv = SystemCoreClock / 1000000;
    
    /* Clear stale errors */
    if (flc->intr & MXC_F_FLC_REVA_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_REVA_INTR_AF;
    }
    
    /* Unlock flash */
    flc->ctrl = (flc->ctrl & ~MXC_F_FLC_REVA_CTRL_UNLOCK) | MXC_S_FLC_REVA_CTRL_UNLOCK_UNLOCKED;
    
    return E_NO_ERROR;
}

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
int MXC_FLC_RevA_Busy(void)
{
    uint32_t flc_cn = 0;
    int i;
    mxc_flc_reva_regs_t *flc;
    
    for (i = 0; i < MXC_FLC_INSTANCES; i++) {
        flc = (mxc_flc_reva_regs_t*) MXC_FLC_GET_FLC (i);
        flc_cn = MXC_busy_flc (flc);
        
        if (flc_cn != 0) {
            break;
        }
    }
    
    return flc_cn;
}
//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
int MXC_FLC_RevA_MassErase (mxc_flc_reva_regs_t *flc)
{
    int err;
    
    if ((err = MXC_prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }
    
    /* Write mass erase code */
    flc->ctrl = (flc->ctrl & ~MXC_F_FLC_REVA_CTRL_ERASE_CODE) | MXC_S_FLC_REVA_CTRL_ERASE_CODE_ERASEALL;
    
    /* Issue mass erase command */
    flc->ctrl |= MXC_F_FLC_REVA_CTRL_ME;
    
    /* Wait until flash operation is complete */
    while (MXC_busy_flc(flc));
    
    /* Lock flash */
    flc->ctrl &= ~MXC_F_FLC_REVA_CTRL_UNLOCK;
    
    /* Check access violations */
    if (flc->intr & MXC_F_FLC_REVA_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_REVA_INTR_AF;
        return E_BAD_STATE;
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
int MXC_FLC_RevA_PageErase (mxc_flc_reva_regs_t *flc, uint32_t addr)
{
    int err;

    if ((err = MXC_prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }

    /* Write page erase code */
    flc->ctrl = (flc->ctrl & ~MXC_F_FLC_REVA_CTRL_ERASE_CODE) | MXC_S_FLC_REVA_CTRL_ERASE_CODE_ERASEPAGE;
    /* Issue page erase command */
    flc->addr = addr;
    flc->ctrl |= MXC_F_FLC_REVA_CTRL_PGE;
    
    /* Wait until flash operation is complete */
    while (MXC_FLC_Busy());
    
    /* Lock flash */
    flc->ctrl &= ~MXC_F_FLC_REVA_CTRL_UNLOCK;
    
    /* Check access violations */
    if (flc->intr & MXC_F_FLC_REVA_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_REVA_INTR_AF;
        return E_BAD_STATE;
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
int MXC_FLC_RevA_Write32 (mxc_flc_reva_regs_t* flc, uint32_t logicAddr, uint32_t data, uint32_t physicalAddr)
{
    int err, i = 0;
    uint32_t byte;
    volatile uint32_t* ptr;
    uint32_t current_data[4] = {0, 0, 0, 0};
    
    // Address checked if it is byte addressable
    if (logicAddr & 0x3) {
        return E_BAD_PARAM;
    }
    
    // Check if the location trying to be written has 1's in to be written to 0's
    if ((* (uint32_t*) logicAddr & data) != data) {
        return E_BAD_STATE;
    }
    
    // Get byte idx within 128-bit word
    byte = (logicAddr & 0xf);
    // Align address to 128-bit word
    logicAddr = logicAddr & 0xfffffff0;
    
    if ((err = MXC_prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }
    
    // Get current data stored in flash
    for (ptr = (uint32_t*) logicAddr; ptr < (uint32_t*)(logicAddr + 16); ptr++, i++) {
        current_data[i] = *ptr;
    }
    
    // write the data
    flc->addr = physicalAddr;
    
    if (byte < 4) {
        current_data[0] = data;
    }
    else if (byte < 8) {
        current_data[1] = data;
    }
    else if (byte < 12) {
        current_data[2] = data;
    }
    else {
        current_data[3] = data;
    }
    
    return MXC_FLC_Write128(logicAddr, current_data);
}

//******************************************************************************
#if IAR_PRAGMAS
#pragma section=".flashprog"
#else
__attribute__((section(".flashprog")))
#endif
// make sure to disable ICC with ICC_Disable(); before Running this function
int MXC_FLC_RevA_Write128 (mxc_flc_reva_regs_t *flc, uint32_t addr, uint32_t *data)
{
    int err;
    
    // Address checked if it is 128-bit aligned
    if (addr & 0xF) {
        return E_BAD_PARAM;
    }
    
    if ((err = MXC_prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }
    
    // write 128-bits
    flc->ctrl &= ~MXC_F_FLC_REVA_CTRL_WDTH;
    
    // write the data
    flc->addr = addr;
    flc->data[0] = data[0];
    flc->data[1] = data[1];
    flc->data[2] = data[2];
    flc->data[3] = data[3];
    flc->ctrl |= MXC_F_FLC_REVA_CTRL_WR;

    /* Wait until flash operation is complete */
    while ((flc->ctrl & MXC_F_FLC_REVA_CTRL_PEND)!=0){}
    while (MXC_busy_flc (flc)){}
    
    /* Lock flash */
    flc->ctrl &= ~MXC_F_FLC_REVA_CTRL_UNLOCK;
    
    /* Check access violations */
    if (flc->intr & MXC_F_FLC_REVA_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_REVA_INTR_AF;
        return E_BAD_STATE;
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
int MXC_FLC_RevA_EnableInt(uint32_t mask)
{
    mask &= (MXC_F_FLC_REVA_INTR_DONEIE |  MXC_F_FLC_REVA_INTR_AFIE);
    
    if (!mask) {
        /* No bits set? Wasn't something we can enable. */
        return E_BAD_PARAM;
    }
    
    /* Apply enables and write back, preserving the flags */
    ((mxc_flc_reva_regs_t*) MXC_FLC_GET_FLC(0))->intr |= mask;
    
    return E_NO_ERROR;
}

//******************************************************************************
int MXC_FLC_RevA_DisableInt(uint32_t mask)
{
    mask &= (MXC_F_FLC_REVA_INTR_DONEIE |  MXC_F_FLC_REVA_INTR_AFIE);
    
    if (!mask) {
        /* No bits set? Wasn't something we can disable. */
        return E_BAD_PARAM;
    }
    
    /* Apply disables and write back, preserving the flags */
    ((mxc_flc_reva_regs_t*) MXC_FLC_GET_FLC(0))->intr &= ~mask;
    
    return E_NO_ERROR;
}

//******************************************************************************
int MXC_FLC_RevA_GetFlags(void)
{
    return (((mxc_flc_reva_regs_t*) MXC_FLC_GET_FLC(0))->intr & (MXC_F_FLC_REVA_INTR_DONE | MXC_F_FLC_REVA_INTR_AF));
}

//******************************************************************************
int MXC_FLC_RevA_ClearFlags(uint32_t mask)
{
    mask &= (MXC_F_FLC_REVA_INTR_DONE | MXC_F_FLC_REVA_INTR_AF);
    
    if (!mask) {
        /* No bits set? Wasn't something we can clear. */
        return E_BAD_PARAM;
    }
    
    /* Both flags are write zero clear */
    ((mxc_flc_reva_regs_t*) MXC_FLC_GET_FLC(0))->intr ^= mask;
    
    return E_NO_ERROR;
}

//******************************************************************************
int MXC_FLC_RevA_UnlockInfoBlock (mxc_flc_reva_regs_t *flc, uint32_t address)
{
    if ((address < MXC_INFO_MEM_BASE) || (address >= (MXC_INFO_MEM_BASE + (MXC_INFO_MEM_SIZE * 2)))) {
        return E_BAD_PARAM;
    }

    /* Make sure the info block is locked */
    flc->actrl = 0x1234;
    
    /* Write the unlock sequence */
    flc->actrl = 0x3a7f5ca3;
    flc->actrl = 0xa1e34f20;
    flc->actrl = 0x9608b2c1;
    
    return E_NO_ERROR;
}

//******************************************************************************
int MXC_FLC_RevA_LockInfoBlock (mxc_flc_reva_regs_t *flc, uint32_t address)
{
    if ((address < MXC_INFO_MEM_BASE) || (address >= (MXC_INFO_MEM_BASE + (MXC_INFO_MEM_SIZE * 2)))) {
        return E_BAD_PARAM;
    }
    
    flc->actrl = 0xDEADBEEF;
    return E_NO_ERROR;
}
/**@} end of group flc */
