/* ****************************************************************************
 * Copyright(C) 2022 Maxim Integrated Products, Inc., All Rights Reserved.
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
 *************************************************************************** */

#include <stdlib.h>
#include <string.h>

#include "mxc_sys.h"
#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mxc_lock.h"

#include "trng_regs.h"
#include "trng.h"
#include "trng_revb.h"

/***** Global Variables *****/

static mxc_trng_complete_t MXC_TRNG_Callback;

static uint32_t TRNG_count, TRNG_maxLength;
static uint8_t* TRNG_data;

/* ************************************************************************* */
/* Global Control/Configuration functions                                    */
/* ************************************************************************* */

int MXC_TRNG_RevB_Init(void)
{
    return E_NO_ERROR;
}

void MXC_TRNG_RevB_EnableInt(mxc_trng_revb_regs_t* trng)
{
    trng->ctrl |= MXC_F_TRNG_REVB_CTRL_RND_IE;
}

void MXC_TRNG_RevB_DisableInt(mxc_trng_revb_regs_t* trng)
{
    trng->ctrl &= ~MXC_F_TRNG_REVB_CTRL_RND_IE;
}

int MXC_TRNG_RevB_Shutdown(void)
{
    return E_NO_ERROR;
}

void MXC_TRNG_RevB_Handler(mxc_trng_revb_regs_t *trng)
{
    uint32_t temp;
    mxc_trng_complete_t cb;
    
    // if this is last block, disable interrupt before reading trng->data
    if(TRNG_maxLength <= TRNG_count + 4) {
        trng->ctrl &= ~MXC_F_TRNG_REVB_CTRL_RND_IE;
    }
    
    temp = trng->data;
    
    if((TRNG_count + 3) < TRNG_maxLength) {
        memcpy(&(TRNG_data[TRNG_count]), (uint8_t*)(&temp), 4);
        TRNG_count += 4;
    }
    else {
        memcpy(&(TRNG_data[TRNG_count]), (uint8_t*)(&temp), TRNG_maxLength & 0x03);
        TRNG_count += (TRNG_maxLength & 0x03);
    }
    
    if(TRNG_maxLength == TRNG_count) {
        cb = MXC_TRNG_Callback;
        cb(0, 0);
    }
}

/* ************************************************************************* */
/* True Random Number Generator(TRNG) functions                             */
/* ************************************************************************* */

int MXC_TRNG_RevB_RandomInt(mxc_trng_revb_regs_t* trng)
{
    while(!(trng->status & MXC_F_TRNG_REVB_STATUS_RDY));
    
    return (int) trng->data;
}

int MXC_TRNG_RevB_Random(uint8_t* data, uint32_t len)
{
    unsigned int i, temp;
    
    if(data == NULL) {
        return E_NULL_PTR;
    }
    
    for(i = 0; (i + 3) < len; i+=4) {
        temp = MXC_TRNG_RandomInt();
        memcpy(&(data[i]), (uint8_t*)(&temp), 4);
    }
    
    if(len & 0x03) {
        temp = MXC_TRNG_RandomInt();
        memcpy(&(data[i]), (uint8_t*)(&temp), len & 0x03);
    }
    
    return E_NO_ERROR;
}

void MXC_TRNG_RevB_RandomAsync(mxc_trng_revb_regs_t* trng, uint8_t* data, uint32_t len, mxc_trng_complete_t callback)
{
    MXC_ASSERT(data && callback);
    
    if(len == 0) {
        return;
    }
    
    TRNG_data = data;
    TRNG_count = 0;
    TRNG_maxLength = len;
    MXC_TRNG_Callback = callback;
    
    // Enable interrupts
    trng->ctrl |= MXC_F_TRNG_REVB_CTRL_RND_IE;
}

void MXC_TRNG_RevB_GenerateKey(mxc_trng_revb_regs_t* trng)
{
  /*Generate AES Key */
  trng->ctrl |= MXC_F_TRNG_REVB_CTRL_KEYGEN;

  while(trng->ctrl & MXC_F_TRNG_REVB_CTRL_KEYGEN);
}