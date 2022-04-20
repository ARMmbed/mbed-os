/* ****************************************************************************
 * Copyright (C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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

/**
 * @file mxc_sys.c
 * @brief      System layer driver.
 * @details    This driver is used to control the system layer of the device.
 */

/* **** Includes **** */
#include <stddef.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "mxc_aes.h"
#include "flc.h"
#include "gcr_regs.h"
#include "fcr_regs.h"
#include "mcr_regs.h"

/**
 * @ingroup mxc_sys
 * @{
 */

/* **** Definitions **** */
#define MXC_SYS_CLOCK_TIMEOUT       MSEC(1)
#define MXC_SYS_ERFO_TIMEOUT        MSEC(100)

/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************** */
int MXC_SYS_GetUSN(uint8_t *usn, uint8_t *checksum)
{
    uint32_t *infoblock = (uint32_t*)MXC_INFO0_MEM_BASE;

    /* Read the USN from the info block */
    MXC_FLC_UnlockInfoBlock(MXC_INFO0_MEM_BASE);

    memset(usn, 0, MXC_SYS_USN_CHECKSUM_LEN);

    usn[0]  = (infoblock[0] & 0x007F8000) >> 15;
    usn[1]  = (infoblock[0] & 0x7F800000) >> 23;
    usn[2]  = (infoblock[1] & 0x0000007F) << 1;
    usn[2] |= (infoblock[0] & 0x80000000) >> 31;
    usn[3]  = (infoblock[1] & 0x00007F80) >> 7;
    usn[4]  = (infoblock[1] & 0x007F8000) >> 15;
    usn[5]  = (infoblock[1] & 0x7F800000) >> 23;
    usn[6]  = (infoblock[2] & 0x007F8000) >> 15;
    usn[7]  = (infoblock[2] & 0x7F800000) >> 23;
    usn[8]  = (infoblock[3] & 0x0000007F) << 1;
    usn[8] |= (infoblock[2] & 0x80000000) >> 31;
    usn[9]  = (infoblock[3] & 0x00007F80) >> 7;
    usn[10] = (infoblock[3] & 0x007F8000) >> 15;

    // Compute the checksum
    if(checksum != NULL) {
        uint8_t info_checksum[2];
        uint8_t key[MXC_SYS_USN_CHECKSUM_LEN];

        /* Initialize the remainder of the USN and key */
        memset(key, 0, MXC_SYS_USN_CHECKSUM_LEN);
        memset(checksum, 0, MXC_SYS_USN_CHECKSUM_LEN);

        /* Read the checksum from the info block */
        info_checksum[0] = ((infoblock[3] & 0x7F800000) >> 23);
        info_checksum[1] = ((infoblock[4] & 0x007F8000) >> 15);

        /* Setup the encryption parameters */
        MXC_AES_Init();

        mxc_aes_req_t aesReq;
        aesReq.length = MXC_SYS_USN_CHECKSUM_LEN/4;
        aesReq.inputData = (uint32_t*)usn;
        aesReq.resultData = (uint32_t*)checksum;
        aesReq.keySize = MXC_AES_128BITS;
        aesReq.encryption = MXC_AES_ENCRYPT_EXT_KEY;

        MXC_AES_SetExtKey(key, MXC_AES_128BITS);
        MXC_AES_Encrypt(&aesReq);
        MXC_AES_Shutdown();

        /* Verify the checksum */
        if((checksum[1] != info_checksum[0]) ||
            (checksum[0] != info_checksum[1])) {

            MXC_FLC_LockInfoBlock(MXC_INFO0_MEM_BASE);
            return E_UNKNOWN;
        }
    }

    /* Add the info block checksum to the USN */
    usn[11] = ((infoblock[3] & 0x7F800000) >> 23);
    usn[12] = ((infoblock[4] & 0x007F8000) >> 15);

    MXC_FLC_LockInfoBlock(MXC_INFO0_MEM_BASE);

    return E_NO_ERROR;
}

/* ************************************************************************** */
int MXC_SYS_IsClockEnabled(mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 and 64 for the perckcn1 register. */
    if (clock > 63) {
        clock -= 64;
        return !(MXC_MCR->clkdis & (0x1 << clock));
    }
    else if (clock > 31) {
        clock -= 32;
        return !(MXC_GCR->pclkdis1 & (0x1 << clock));
    }
    else {
        return !(MXC_GCR->pclkdis0 & (0x1 << clock));
    }
}

/* ************************************************************************** */
void MXC_SYS_ClockDisable(mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 and 64 for the perckcn1 register. */
    if (clock > 63) {
        clock -= 64;
        MXC_MCR->clkdis |= (0x1 << clock);
    }
    else if (clock > 31) {
        clock -= 32;
        MXC_GCR->pclkdis1 |= (0x1 << clock);
    }
    else {
        MXC_GCR->pclkdis0 |= (0x1 << clock);
    }
}

/* ************************************************************************** */
void MXC_SYS_ClockEnable(mxc_sys_periph_clock_t clock)
{
    /* The mxc_sys_periph_clock_t enum uses enum values that are the offset by 32 and 64 for the perckcn1 register. */
    if (clock > 63) {
        clock -= 64;
        MXC_MCR->clkdis &= ~(0x1 << clock);
    }
    else if (clock > 31) {
        clock -= 32;
        MXC_GCR->pclkdis1 &= ~(0x1 << clock);
    }
    else {
        MXC_GCR->pclkdis0 &= ~(0x1 << clock);
    }
}
/* ************************************************************************** */
void MXC_SYS_RTCClockEnable()
{
    MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_ERTCO_EN;
}

/* ************************************************************************** */
int MXC_SYS_RTCClockDisable(void)
{
    /* Check that the RTC is not the system clock source */
    if ((MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_SYSCLK_SEL) != MXC_S_GCR_CLKCTRL_SYSCLK_SEL_ERTCO) {
        MXC_GCR->clkctrl &= ~MXC_F_GCR_CLKCTRL_ERTCO_EN;
        return E_NO_ERROR;
    }
    else {
        return E_BAD_STATE;
    }
}

/******************************************************************************/
int MXC_SYS_ClockSourceEnable(mxc_sys_system_clock_t clock)
{
    switch (clock) {
    case MXC_SYS_CLOCK_IPO:
        MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_IPO_EN;
        return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_IPO_RDY);
        break;
        
    case MXC_SYS_CLOCK_IBRO:
        MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_IBRO_EN;
        return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_IBRO_RDY);
        break;
        
    case MXC_SYS_CLOCK_EXTCLK:
        // MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_EXTCLK_EN;
        // return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_EXTCLK_RDY);
        return E_NOT_SUPPORTED;
        break;
        
    case MXC_SYS_CLOCK_INRO:
        // The 80k clock is always enabled
        return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_INRO_RDY);
        break;
        
    case MXC_SYS_CLOCK_ERFO:
        MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_ERFO_EN;
        return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_ERFO_RDY);
        break;
        
    case MXC_SYS_CLOCK_ERTCO:
        MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_ERTCO_EN;
        return MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_ERTCO_RDY);
        break;
        
    default:
        return E_BAD_PARAM;
        break;
    }
}

/******************************************************************************/
int MXC_SYS_ClockSourceDisable(mxc_sys_system_clock_t clock)
{
    uint32_t current_clock;
    
    current_clock = MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_SYSCLK_SEL;
    
    // Don't turn off the clock we're running on
    if (clock == current_clock) {
        return E_BAD_PARAM;
    }
    
    switch (clock) {
    case MXC_SYS_CLOCK_IPO:
        MXC_GCR->clkctrl &= ~MXC_F_GCR_CLKCTRL_IPO_EN;
        break;
        
    case MXC_SYS_CLOCK_IBRO:
        MXC_GCR->clkctrl &= ~MXC_F_GCR_CLKCTRL_IBRO_EN;
        break;
        
    case MXC_SYS_CLOCK_EXTCLK:
        // MXC_GCR->clkctrl &= ~MXC_F_GCR_CLKCTRL_EXTCLK_EN;
        break;
        
    case MXC_SYS_CLOCK_INRO:
        // The 80k clock is always enabled
        break;
        
    case MXC_SYS_CLOCK_ERFO:
        MXC_GCR->clkctrl &= ~MXC_F_GCR_CLKCTRL_ERFO_EN;
        break;
        
    case MXC_SYS_CLOCK_ERTCO:
        MXC_GCR->clkctrl &= ~MXC_F_GCR_CLKCTRL_ERTCO_EN;
        break;
        
    default:
        return E_BAD_PARAM;
    }
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
int MXC_SYS_Clock_Timeout(uint32_t ready)
{
    // Start timeout, wait for ready
    if(ready == MXC_F_GCR_CLKCTRL_ERFO_RDY) {
        MXC_DelayAsync(MXC_SYS_ERFO_TIMEOUT, NULL);
    } else {
        MXC_DelayAsync(MXC_SYS_CLOCK_TIMEOUT, NULL);
    }

    do {
        if (MXC_GCR->clkctrl & ready) {
            MXC_DelayAbort();
            return E_NO_ERROR;
        }
    }
    while (MXC_DelayCheck() == E_BUSY);
    
    return E_TIME_OUT;
}

/* ************************************************************************** */
int MXC_SYS_Clock_Select(mxc_sys_system_clock_t clock)
{
    uint32_t current_clock;
    
    // Save the current system clock
    current_clock = MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_SYSCLK_SEL;
    
    switch (clock) {
    case MXC_SYS_CLOCK_IPO:
    
        // Enable IPO clock
        if (!(MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_IPO_EN)) {
        
            MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_IPO_EN;
            
            // Check if IPO clock is ready
            if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_IPO_RDY) != E_NO_ERROR) {
                return E_TIME_OUT;
            }
        }
        
        // Set IPO clock as System Clock
        MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, MXC_S_GCR_CLKCTRL_SYSCLK_SEL_IPO);
        
        break;
        
    case MXC_SYS_CLOCK_IBRO:
    
        // Enable IBRO clock
        if (!(MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_IBRO_EN)) {
            MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_IBRO_EN;
            
            // Check if IBRO clock is ready
            if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_IBRO_RDY) != E_NO_ERROR) {
                return E_TIME_OUT;
            }
        }
        
        // Set IBRO clock as System Clock
        MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, MXC_S_GCR_CLKCTRL_SYSCLK_SEL_IBRO);
        
        break;
        
    case MXC_SYS_CLOCK_EXTCLK:
        // Enable HIRC clock
        // if(!(MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_EXTCLK_EN)) {
        //     MXC_GCR->clkctrl |=MXC_F_GCR_CLKCTRL_EXTCLK_EN;
        
        //     // Check if HIRC clock is ready
        //     if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_EXTCLK_RDY) != E_NO_ERROR) {
        //         return E_TIME_OUT;
        //     }
        // }
        
        // Set HIRC clock as System Clock
        // MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, MXC_S_GCR_CLKCTRL_SYSCLK_SEL_EXTCLK);
        
        break;
        
    case MXC_SYS_CLOCK_ERFO:
    
        // Enable ERFO clock
        if (!(MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_ERFO_EN)) {
            MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_ERFO_EN;
            
            // Check if ERFO clock is ready
            if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_ERFO_RDY) != E_NO_ERROR) {
                return E_TIME_OUT;
            }
        }
        
        // Set ERFO clock as System Clock
        MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, MXC_S_GCR_CLKCTRL_SYSCLK_SEL_ERFO);
        
        break;
        
    case MXC_SYS_CLOCK_INRO:
        // Set INRO clock as System Clock
        MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, MXC_S_GCR_CLKCTRL_SYSCLK_SEL_INRO);
        
        break;
        
    case MXC_SYS_CLOCK_ERTCO:
    
        // Enable ERTCO clock
        if (!(MXC_GCR->clkctrl & MXC_F_GCR_CLKCTRL_ERTCO_EN)) {
            MXC_GCR->clkctrl |= MXC_F_GCR_CLKCTRL_ERTCO_EN;
            
            // Check if ERTCO clock is ready
            if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_ERTCO_RDY) != E_NO_ERROR) {
                return E_TIME_OUT;
            }
        }
        
        // Set ERTCO clock as System Clock
        MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, MXC_S_GCR_CLKCTRL_SYSCLK_SEL_ERTCO);
        
        break;
        
    default:
        return E_BAD_PARAM;
    }
    
    // Wait for system clock to be ready
    if (MXC_SYS_Clock_Timeout(MXC_F_GCR_CLKCTRL_SYSCLK_RDY) != E_NO_ERROR) {
    
        // Restore the old system clock if timeout
        MXC_SETFIELD(MXC_GCR->clkctrl, MXC_F_GCR_CLKCTRL_SYSCLK_SEL, current_clock);
        
        return E_TIME_OUT;
    }
    
    // Update the system core clock
    SystemCoreClockUpdate();
    
    return E_NO_ERROR;
}

/* ************************************************************************** */
void MXC_SYS_Reset_Periph(mxc_sys_reset_t reset)
{
    /* The mxc_sys_reset_t enum uses enum values that are the offset by 32 and 64 for the rst register. */
    if (reset > 63) {
        reset -= 64;
        MXC_MCR->rst = (0x1 << reset);
		while (MXC_MCR->rst & (0x1 << reset));
    }
    else if (reset > 31) {
        reset -= 32;
        MXC_GCR->rst1 = (0x1 << reset);
		while (MXC_GCR->rst1 & (0x1 << reset));
    }
    else {
        MXC_GCR->rst0 = (0x1 << reset);
		while (MXC_GCR->rst0 & (0x1 << reset));
    }
}
/**@} end of mxc_sys */

