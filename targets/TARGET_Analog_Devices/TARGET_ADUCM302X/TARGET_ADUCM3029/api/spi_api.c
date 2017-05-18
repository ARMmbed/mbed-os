/* mbed Microcontroller Library
 * Copyright (c) 2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <math.h>
#include "mbed_assert.h"

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
ADI_SPI_HANDLE      spi_Handle0;
uint8_t             spi_Mem0[ADI_SPI_MEMORY_SIZE];
ADI_SPI_HANDLE      spi_Handle1;
uint8_t             spi_Mem1[ADI_SPI_MEMORY_SIZE];
ADI_SPI_HANDLE      spi_Handle2;
uint8_t             spi_Mem2[ADI_SPI_MEMORY_SIZE];
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
    uint8_t             *SPI_Mem;
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
 */
void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    ADI_SPI_HANDLE  SPI_Handle;
    ADI_SPI_RESULT  SPI_Return = ADI_SPI_SUCCESS;
    bool            master;

    master = !((bool)slave);
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
    uint8_t                 TxBuf;
    uint8_t                 RxBuf;
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


/** Check if a value is available to read
 *
 * @param[in] obj The SPI peripheral to check
 * @return non-zero if a value is available
 */
int spi_slave_receive(spi_t *obj)
{
    return 0;
}


/** Get a received value out of the SPI receive buffer in slave mode
 *
 * Blocks until a value is available
 * @param[in] obj The SPI peripheral to read
 * @return The value received
 */
int spi_slave_read(spi_t *obj)
{
    return 0;
}


/** Write a value to the SPI peripheral in slave mode
 *
 * Blocks until the SPI peripheral can be written to
 * @param[in] obj   The SPI peripheral to write
 * @param[in] value The value to write
 */
void spi_slave_write(spi_t *obj, int value)
{
    return;
}



#endif
