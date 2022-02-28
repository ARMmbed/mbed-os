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
 *************************************************************************** */

#include "mxc_device.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mxc_sys.h"

#include "crc.h"
#include "crc_reva.h"


/* ************************************************************************* */
/* Global Control/Configuration functions                                    */
/* ************************************************************************* */

int MXC_CRC_Init(void)
{
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_CRC);

    MXC_CRC_RevA_Init((mxc_crc_reva_regs_t*) MXC_CRC);

    return E_NO_ERROR;
}

int MXC_CRC_Shutdown(void)
{
    int error = MXC_CRC_RevA_Shutdown((mxc_crc_reva_regs_t*) MXC_CRC);
    
    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_CRC);
    
    return error;
}

void MXC_CRC_Handler (int ch, int error)
{
    MXC_CRC_RevA_Handler(ch, error);
}

void MXC_CRC_SetDirection(mxc_crc_bitorder_t bitOrder)
{
    MXC_CRC_RevA_SetDirection((mxc_crc_reva_regs_t*) MXC_CRC, (mxc_crc_reva_bitorder_t) bitOrder);
}

mxc_crc_bitorder_t MXC_CRC_GetDirection(void)
{
    return MXC_CRC_RevA_GetDirection((mxc_crc_reva_regs_t*) MXC_CRC);
}

void MXC_CRC_SwapDataIn(mxc_crc_bitorder_t bitOrder)
{
    MXC_CRC_RevA_SwapDataIn((mxc_crc_reva_regs_t*) MXC_CRC, (mxc_crc_reva_bitorder_t) bitOrder);
}

void MXC_CRC_SwapDataOut(mxc_crc_bitorder_t bitOrder)
{
    MXC_CRC_RevA_SwapDataOut((mxc_crc_reva_regs_t*) MXC_CRC, (mxc_crc_reva_bitorder_t) bitOrder);
}

void MXC_CRC_SetPoly(uint32_t poly)
{
    MXC_CRC_RevA_SetPoly((mxc_crc_reva_regs_t*) MXC_CRC, poly);
}

uint32_t MXC_CRC_GetPoly(void)
{
    return MXC_CRC_RevA_GetPoly((mxc_crc_reva_regs_t*) MXC_CRC);
}

uint32_t MXC_CRC_GetResult(void)
{
    return MXC_CRC_RevA_GetResult((mxc_crc_reva_regs_t*) MXC_CRC);
}

int MXC_CRC_Compute(mxc_crc_req_t* req)
{
    return MXC_CRC_RevA_Compute((mxc_crc_reva_regs_t*) MXC_CRC, (mxc_crc_reva_req_t*) req);
}

int MXC_CRC_ComputeAsync(mxc_crc_req_t* req)
{
    return MXC_CRC_RevA_ComputeAsync((mxc_crc_reva_regs_t*) MXC_CRC, (mxc_crc_reva_req_t*) req);
}
