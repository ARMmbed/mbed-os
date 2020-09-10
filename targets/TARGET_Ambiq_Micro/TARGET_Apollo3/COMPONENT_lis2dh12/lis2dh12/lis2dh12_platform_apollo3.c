/* 
 * Copyright (c) 2019-2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "lis2dh12_platform_apollo3.h"

/*
 * @brief  Write generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to write
 * @param  bufp      pointer to data to write in register reg
 * @param  len       number of consecutive register to write
 *
 */
int32_t lis2dh12_write_platform_apollo3(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    uint32_t retVal32 = 0;
    lis2dh12_platform_apollo3_if_t* pif = (lis2dh12_platform_apollo3_if_t*)handle;
    am_hal_iom_transfer_t iomTransfer = {0};

    if( bufp == NULL )          { return AM_HAL_STATUS_FAIL; }
    if( pif == NULL )           { return AM_HAL_STATUS_FAIL; }
    if( pif->iomHandle == NULL) { return AM_HAL_STATUS_FAIL; }

    // Set up transfer
    iomTransfer.uPeerInfo.ui32I2CDevAddr    = pif->addCS;
    iomTransfer.ui32InstrLen                = 1;
    iomTransfer.ui32Instr                   = (reg | 0x80);
    iomTransfer.ui32NumBytes                = len;
    iomTransfer.eDirection                  = AM_HAL_IOM_TX;
    iomTransfer.pui32TxBuffer               = (uint32_t*)bufp;
    iomTransfer.pui32RxBuffer               = NULL;
    iomTransfer.bContinue                   = false;

    if( pif->useSPI ){
        // ToDo: Support SPI w/ CS assertion
    }

    // Send the transfer
    retVal32 = am_hal_iom_blocking_transfer(pif->iomHandle, &iomTransfer);

    if( pif->useSPI ){
        // ToDo: Support SPI / CS de-assertion
    }

    if( retVal32 != AM_HAL_STATUS_SUCCESS ){ return retVal32; }

    return 0;
}

/*
 * @brief  Read generic device register (platform dependent)
 *
 * @param  handle    customizable argument. In this examples is used in
 *                   order to select the correct sensor bus handler.
 * @param  reg       register to read
 * @param  bufp      pointer to buffer that store the data read
 * @param  len       number of consecutive register to read
 *
 */
int32_t lis2dh12_read_platform_apollo3(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    uint32_t retVal32 = 0;
    lis2dh12_platform_apollo3_if_t* pif = (lis2dh12_platform_apollo3_if_t*)handle;
    am_hal_iom_transfer_t iomTransfer = {0};

    if( bufp == NULL )          { return AM_HAL_STATUS_FAIL; }
    if( pif == NULL )           { return AM_HAL_STATUS_FAIL; }
    if( pif->iomHandle == NULL) { return AM_HAL_STATUS_FAIL; }

    // Set up first transfer
    iomTransfer.uPeerInfo.ui32I2CDevAddr    = pif->addCS;
    iomTransfer.ui32InstrLen                = 1;
    iomTransfer.ui32Instr                   = (reg | 0x80);
    iomTransfer.ui32NumBytes                = 0;
    iomTransfer.eDirection                  = AM_HAL_IOM_TX;
    iomTransfer.bContinue                   = true;

    if( pif->useSPI ){
        // ToDo: Support SPI w/ CS assertion
    }

    // Send the first transfer
    retVal32 = am_hal_iom_blocking_transfer(pif->iomHandle, &iomTransfer);
    if( retVal32 != AM_HAL_STATUS_SUCCESS ){ return retVal32; }

    // Change direction, and add the rx buffer
    iomTransfer.eDirection                  = AM_HAL_IOM_RX;
    iomTransfer.pui32RxBuffer               = (uint32_t*)bufp;
    iomTransfer.ui32NumBytes                = len;
    iomTransfer.bContinue                   = false;

    // Send the second transfer
    retVal32 = am_hal_iom_blocking_transfer(pif->iomHandle, &iomTransfer);

    if( retVal32 != AM_HAL_STATUS_SUCCESS ){ return retVal32; }
    
    return 0;
}
