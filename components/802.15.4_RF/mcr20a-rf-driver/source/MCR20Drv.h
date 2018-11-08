/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
*
* \file MCR20Drv.h
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __MCR20_DRV_H__
#define __MCR20_DRV_H__

#include <stdint.h>

/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 * Note that it is not a good practice to include header files into header   *
 * files, so use this section only if there is no other better solution.     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*****************************************************************************
 *                             PRIVATE MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/* Disable XCVR clock output by default, to reduce power consumption */
#ifndef gMCR20_ClkOutFreq_d
#define gMCR20_ClkOutFreq_d gCLK_OUT_FREQ_DISABLE
#endif

/*****************************************************************************
 *                            PUBLIC FUNCTIONS                               *
 *---------------------------------------------------------------------------*
 * Add to this section all the global functions prototype preceded (as a     *
 * good practice) by the keyword 'extern'                                    *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_Init
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
extern void MCR20Drv_Init
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_SPI_DMA_Init
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_SPI_DMA_Init
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_Start_PB_DMA_SPI_Write
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_Start_PB_DMA_SPI_Write
(
    uint8_t *srcAddress,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_Start_PB_DMA_SPI_Read
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_Start_PB_DMA_SPI_Read
(
    uint8_t *dstAddress,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_DirectAccessSPIWrite
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_DirectAccessSPIWrite
(
    uint8_t address,
    uint8_t value
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_DirectAccessSPIMultiByteWrite
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_DirectAccessSPIMultiByteWrite
(
    uint8_t startAddress,
    uint8_t *byteArray,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_PB_SPIBurstWrite
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_PB_SPIBurstWrite
(
    uint8_t *byteArray,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_DirectAccessSPIRead
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint8_t MCR20Drv_DirectAccessSPIRead
(
    uint8_t address
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_DirectAccessSPIMultyByteRead
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/

uint8_t MCR20Drv_DirectAccessSPIMultiByteRead
(
    uint8_t startAddress,
    uint8_t *byteArray,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_PB_SPIByteWrite
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_PB_SPIByteWrite
(
    uint8_t address,
    uint8_t value
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_PB_SPIBurstRead
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint8_t MCR20Drv_PB_SPIBurstRead
(
    uint8_t *byteArray,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_IndirectAccessSPIWrite
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_IndirectAccessSPIWrite
(
    uint8_t address,
    uint8_t value
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_IndirectAccessSPIMultiByteWrite
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_IndirectAccessSPIMultiByteWrite
(
    uint8_t startAddress,
    uint8_t *byteArray,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_IndirectAccessSPIRead
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint8_t MCR20Drv_IndirectAccessSPIRead
(
    uint8_t address
);
/*---------------------------------------------------------------------------
 * Name: MCR20Drv_IndirectAccessSPIMultiByteRead
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_IndirectAccessSPIMultiByteRead
(
    uint8_t startAddress,
    uint8_t *byteArray,
    uint8_t numOfBytes
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_IsIrqPending
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
uint32_t MCR20Drv_IsIrqPending
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_IRQ_Disable
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_IRQ_Disable
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_IRQ_Enable
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_IRQ_Enable
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_RST_PortConfig
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_RST_B_PortConfig
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_RST_Assert
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_RST_B_Assert
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_RST_Deassert
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_RST_B_Deassert
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_SoftRST_Assert
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_SoftRST_Assert
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_SoftRST_Deassert
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_SoftRST_Deassert
(
    void
);


/*---------------------------------------------------------------------------
 * Name: MCR20Drv_RESET
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_RESET
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_Soft_RESET
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_Soft_RESET
(
    void
);

/*---------------------------------------------------------------------------
 * Name: MCR20Drv_Set_CLK_OUT_Freq
 * Description: -
 * Parameters: -
 * Return: -
 *---------------------------------------------------------------------------*/
void MCR20Drv_Set_CLK_OUT_Freq
(
    uint8_t freqDiv
);

#define ProtectFromMCR20Interrupt()   MCR20Drv_IRQ_Disable()
#define UnprotectFromMCR20Interrupt() MCR20Drv_IRQ_Enable()

#endif /* __MCR20_DRV_H__ */
