/*******************************************************************************
 * Copyright (c) 2010-2017 Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   - Modified versions of the software must be conspicuously marked as such.
 *   - This software is licensed solely and exclusively for use with processors
 *     manufactured by or for Analog Devices, Inc.
 *   - This software may not be combined or merged with other code in any manner
 *     that would cause the software to become subject to terms and conditions
 *     which differ from those listed here.
 *   - Neither the name of Analog Devices, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *   - The use of this software may or may not infringe the patent rights of one
 *     or more patent holders.  This license does not release you from the
 *     requirement that you obtain separate licenses from these patent holders
 *     to use this software.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
 * INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
 * CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include <math.h>
#include "mbed_assert.h"

#include <adi_types.h>

#include "spi_api.h"

#if DEVICE_SPI

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "drivers/spi/adi_spi.h"



#if defined(BUILD_SPI_MI_DYNAMIC)
#if defined(ADI_DEBUG)
#warning "BUILD_SPI_MI_DYNAMIC is defined.  Memory allocation for SPI will be dynamic"
int adi_spi_memtype = 0;
#endif
#else
/*******************************************************************************
   ADI_SPI_DEV_DATA_TYPE Instance memory containing memory pointer should
   guarantee 4 byte alignmnet.
 *******************************************************************************/
ADI_SPI_HANDLE      spi_Handle0;
uint32_t            spi_Mem0[(ADI_SPI_MEMORY_SIZE + 3)/4];
ADI_SPI_HANDLE      spi_Handle1;
uint32_t            spi_Mem1[(ADI_SPI_MEMORY_SIZE + 3)/4];
ADI_SPI_HANDLE      spi_Handle2;
uint32_t            spi_Mem2[(ADI_SPI_MEMORY_SIZE + 3)/4];
#if defined(ADI_DEBUG)
#warning "BUILD_SPI_MI_DYNAMIC is NOT defined.  Memory allocation for SPI will be static"
int adi_spi_memtype = 1;
#endif
#endif



/** Initialize the SPI peripheral
 *
 * Configures the pins used by SPI, sets a default format and frequency, and enables the peripheral
 * @param[out] obj  The SPI object to initialize
 * @param[in]  mosi The pin to use for MOSI
 * @param[in]  miso The pin to use for MISO
 * @param[in]  sclk The pin to use for SCLK
 * @param[in]  ssel The pin to use for SSEL
 */
void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel);
    ADI_SPI_HANDLE      *pSPI_Handle;
    uint32_t            *SPI_Mem;
    ADI_SPI_RESULT      SPI_Return = ADI_SPI_SUCCESS;
    uint32_t            nDeviceNum = 0;
    ADI_SPI_CHIP_SELECT spi_cs = ADI_SPI_CS_NONE;


#if defined(BUILD_SPI_MI_DYNAMIC)
    if (mosi == SPI0_MOSI) {
        nDeviceNum = SPI_0;
    } else if (mosi == SPI1_MOSI) {
        nDeviceNum = SPI_1;
    } else if (mosi == SPI2_MOSI) {
        nDeviceNum = SPI_2;
    }
    pSPI_Handle = &obj->SPI_Handle;
    obj->pSPI_Handle = pSPI_Handle;
    SPI_Mem = obj->SPI_Mem;
#else
    if (mosi == SPI0_MOSI) {
        nDeviceNum = SPI_0;
        pSPI_Handle = &spi_Handle0;
        SPI_Mem = &spi_Mem0[0];
    } else if (mosi == SPI1_MOSI) {
        nDeviceNum = SPI_1;
        pSPI_Handle = &spi_Handle1;
        SPI_Mem = &spi_Mem1[0];
    } else if (mosi == SPI2_MOSI) {
        nDeviceNum = SPI_2;
        pSPI_Handle = &spi_Handle2;
        SPI_Mem = &spi_Mem2[0];
    }
    obj->pSPI_Handle    = pSPI_Handle;
#endif


    obj->instance = pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->instance != NC);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    SystemCoreClockUpdate();
    SPI_Return = adi_spi_Open(nDeviceNum, SPI_Mem, ADI_SPI_MEMORY_SIZE, pSPI_Handle);
    if (SPI_Return) {
        obj->error = SPI_EVENT_ERROR;
        return;
    }

    if (ssel != NC) {
        if ( (ssel == SPI0_CS0) || (ssel == SPI1_CS0) || (ssel == SPI2_CS0)) {
            spi_cs = ADI_SPI_CS0;
        } else if ( (ssel == SPI0_CS1) || (ssel == SPI1_CS1) || (ssel == SPI2_CS1)) {
            spi_cs = ADI_SPI_CS1;
        } else if ( (ssel == SPI0_CS2) || (ssel == SPI1_CS2) || (ssel == SPI2_CS2)) {
            spi_cs = ADI_SPI_CS2;
        } else if ( (ssel == SPI0_CS3) || (ssel == SPI1_CS3) || (ssel == SPI2_CS3)) {
            spi_cs = ADI_SPI_CS3;
        }

        SPI_Return = adi_spi_SetChipSelect(*pSPI_Handle, spi_cs);
        if (SPI_Return) {
            obj->error = SPI_EVENT_ERROR;
            return;
        }
    }
}


/** Release a SPI object
 *
 * TODO: spi_free is currently unimplemented
 * This will require reference counting at the C++ level to be safe
 *
 * Return the pins owned by the SPI object to their reset state
 * Disable the SPI peripheral
 * Disable the SPI clock
 * @param[in] obj The SPI object to deinitialize
 */
void spi_free(spi_t *obj)
{
    ADI_SPI_HANDLE  SPI_Handle;
    ADI_SPI_RESULT  SPI_Return = ADI_SPI_SUCCESS;

    SPI_Handle = *obj->pSPI_Handle;
    SPI_Return = adi_spi_Close(SPI_Handle);
    if (SPI_Return) {
        obj->error = SPI_EVENT_ERROR;
        return;
    }
}


/** Configure the SPI format
 *
 * Set the number of bits per frame, configure clock polarity and phase, shift order and master/slave mode.
 * The default bit order is MSB.
 * @param[in,out] obj   The SPI object to configure
 * @param[in]     bits  The number of bits per frame
 * @param[in]     mode  The SPI mode (clock polarity, phase, and shift direction)
 * @param[in]     slave Zero for master mode or non-zero for slave mode
 *
 ** Configure the data transmission format
 *
 *  @param bits Number of bits per SPI frame (4 - 16)
 *  @param mode Clock polarity and phase mode (0 - 3)
 *
 * @code
 * mode | POL PHA
 * -----+--------
 *   0  |  0   0
 *   1  |  0   1
 *   2  |  1   0
 *   3  |  1   1
 * @endcode

    bool          phase;
        true  : trailing-edge
        false : leading-edge

    bool          polarity;
        true  : CPOL=1 (idle high) polarity
        false : CPOL=0 (idle-low) polarity
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    ADI_SPI_HANDLE  SPI_Handle;
    ADI_SPI_RESULT  SPI_Return = ADI_SPI_SUCCESS;
    bool            master;

    master = !((bool_t)slave);
    SPI_Handle = *obj->pSPI_Handle;

    SPI_Return = adi_spi_SetMasterMode(SPI_Handle, master);
    if (SPI_Return) {
        obj->error = SPI_EVENT_ERROR;
        return;
    }
}


/** Set the SPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and bus clock
 * Configures the SPI peripheral's baud rate
 * @param[in,out] obj The SPI object to configure
 * @param[in]     hz  The baud rate in Hz
 */
void spi_frequency(spi_t *obj, int hz)
{
    ADI_SPI_HANDLE  SPI_Handle;
    ADI_SPI_RESULT  SPI_Return = ADI_SPI_SUCCESS;

    SPI_Handle = *obj->pSPI_Handle;
    SPI_Return = adi_spi_SetBitrate(SPI_Handle, (uint32_t) hz);
    if (SPI_Return) {
        obj->error = SPI_EVENT_ERROR;
        return;
    }
}


/** Write a byte out in master mode and receive a value
 *
 * @param[in] obj   The SPI peripheral to use for sending
 * @param[in] value The value to send
 * @return Returns the value received during send
 */
int spi_master_write(spi_t *obj, int value)
{
    ADI_SPI_TRANSCEIVER transceive;
    uint8_t         TxBuf;
    uint8_t         RxBuf;
    ADI_SPI_HANDLE  SPI_Handle;
    ADI_SPI_RESULT  SPI_Return = ADI_SPI_SUCCESS;

    TxBuf = (uint8_t)value;

    transceive.pReceiver        = &RxBuf;
    transceive.ReceiverBytes    = 1;        /* link transceive data size to the remaining count */
    transceive.nRxIncrement     = 1;        /* auto increment buffer */
    transceive.pTransmitter     = &TxBuf;   /* initialize data attributes */
    transceive.TransmitterBytes = 1;        /* link transceive data size to the remaining count */
    transceive.nTxIncrement     = 1;        /* auto increment buffer */

    transceive.bDMA = false;
    transceive.bRD_CTL = false;
    SPI_Handle = *obj->pSPI_Handle;
    SPI_Return = adi_spi_MasterReadWrite(SPI_Handle, &transceive);
    if (SPI_Return) {
        obj->error = SPI_EVENT_ERROR;
        return 1;
    }

    return((int)RxBuf);
}


/** Write a block out in master mode and receive a value
 *
 *  The total number of bytes sent and recieved will be the maximum of
 *  tx_length and rx_length. The bytes written will be padded with the
 *  value 0xff.
 *
 * @param[in] obj        The SPI peripheral to use for sending
 * @param[in] tx_buffer  Pointer to the byte-array of data to write to the device
 * @param[in] tx_length  Number of bytes to write, may be zero
 * @param[in] rx_buffer  Pointer to the byte-array of data to read from the device
 * @param[in] rx_length  Number of bytes to read, may be zero
 * @param[in] write_fill Default data transmitted while performing a read
 * @returns
 *      The number of bytes written and read from the device. This is
 *      maximum of tx_length and rx_length.
 */
int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    ADI_SPI_TRANSCEIVER transceive;
    ADI_SPI_HANDLE  SPI_Handle;
    ADI_SPI_RESULT  SPI_Return = ADI_SPI_SUCCESS;

    transceive.pReceiver        = (uint8_t*)rx_buffer;
    transceive.ReceiverBytes    = rx_length;            /* link transceive data size to the remaining count */
    transceive.nRxIncrement     = 1;                    /* auto increment buffer */
    transceive.pTransmitter     = (uint8_t*)tx_buffer;  /* initialize data attributes */
    transceive.TransmitterBytes = tx_length;            /* link transceive data size to the remaining count */
    transceive.nTxIncrement     = 1;                    /* auto increment buffer */

    transceive.bDMA = false;
    transceive.bRD_CTL = false;
    SPI_Handle = *obj->pSPI_Handle;
    SPI_Return = adi_spi_MasterReadWrite(SPI_Handle, &transceive);
    if (SPI_Return) {
        obj->error = SPI_EVENT_ERROR;
        return -1;
    }
    else {
        return((int)tx_length);
    }
}

#endif
