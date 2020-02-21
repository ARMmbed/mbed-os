/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* All rights reserved.
* SPDX-License-Identifier: BSD-3-Clause
*
* \file MCR20Drv.c
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


/*****************************************************************************
*                               INCLUDED HEADERS                            *
*---------------------------------------------------------------------------*
* Add to this section all the headers that this module needs to include.    *
*---------------------------------------------------------------------------*
*****************************************************************************/

#include "MCR20Drv.h"
#include "MCR20Reg.h"
#include "XcvrSpi.h"

#if defined(MBED_CONF_NANOSTACK_CONFIGURATION) && DEVICE_SPI && DEVICE_INTERRUPTIN && defined(MBED_CONF_RTOS_PRESENT)

#include "platform/mbed_critical.h"
#include <string.h>

/*****************************************************************************
*                               PRIVATE VARIABLES                           *
*---------------------------------------------------------------------------*
* Add to this section all the variables and constants that have local       *
* (file) scope.                                                             *
* Each of this declarations shall be preceded by the 'static' keyword.      *
* These variables / constants cannot be accessed outside this module.       *
*---------------------------------------------------------------------------*
*****************************************************************************/
uint32_t mPhyIrqDisableCnt = 1;

/*****************************************************************************
*                               PUBLIC VARIABLES                            *
*---------------------------------------------------------------------------*
* Add to this section all the variables and constants that have global      *
* (project) scope.                                                          *
* These variables / constants can be accessed outside this module.          *
* These variables / constants shall be preceded by the 'extern' keyword in  *
* the interface header.                                                     *
*---------------------------------------------------------------------------*
*****************************************************************************/

/*****************************************************************************
*                           PRIVATE FUNCTIONS PROTOTYPES                    *
*---------------------------------------------------------------------------*
* Add to this section all the functions prototypes that have local (file)   *
* scope.                                                                    *
* These functions cannot be accessed outside this module.                   *
* These declarations shall be preceded by the 'static' keyword.             *
*---------------------------------------------------------------------------*
*****************************************************************************/

/*****************************************************************************
*                                PRIVATE FUNCTIONS                          *
*---------------------------------------------------------------------------*
* Add to this section all the functions that have local (file) scope.       *
* These functions cannot be accessed outside this module.                   *
* These definitions shall be preceded by the 'static' keyword.              *
*---------------------------------------------------------------------------*
*****************************************************************************/


/*****************************************************************************
*                             PUBLIC FUNCTIONS                              *
*---------------------------------------------------------------------------*
* Add to this section all the functions that have global (project) scope.   *
* These functions can be accessed outside this module.                      *
* These functions shall have their declarations (prototypes) within the     *
* interface header file and shall be preceded by the 'extern' keyword.      *
*---------------------------------------------------------------------------*
*****************************************************************************/

/*---------------------------------------------------------------------------
* Name: MCR20Drv_Init
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_Init
(
    void
)
{
    xcvr_spi_init(gXcvrSpiInstance_c);
    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 8000000);

    gXcvrDeassertCS_d();
#if !defined(TARGET_KW24D)
    MCR20Drv_RST_B_Deassert();
#endif
    RF_IRQ_Init();
    RF_IRQ_Disable();
    mPhyIrqDisableCnt = 1;
}

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
)
{
    uint16_t txData;

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 16000000);

    gXcvrAssertCS_d();

    txData = (address & TransceiverSPI_DirectRegisterAddressMask);
    txData |= value << 8;

    xcvr_spi_transfer(gXcvrSpiInstance_c, (uint8_t *)&txData, 0, sizeof(txData));

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();
}

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
)
{
    uint8_t txData;

    if ((numOfBytes == 0) || (byteArray == NULL)) {
        return;
    }

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 16000000);

    gXcvrAssertCS_d();

    txData = (startAddress & TransceiverSPI_DirectRegisterAddressMask);

    xcvr_spi_transfer(gXcvrSpiInstance_c, &txData, 0, sizeof(txData));
    xcvr_spi_transfer(gXcvrSpiInstance_c, byteArray, 0, numOfBytes);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();
}

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
)
{
    uint32_t txData;

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 16000000);

    gXcvrAssertCS_d();

    txData  = TransceiverSPI_WriteSelect            |
              TransceiverSPI_PacketBuffAccessSelect |
              TransceiverSPI_PacketBuffByteModeSelect;
    txData |= (address) << 8;
    txData |= (value)   << 16;

    xcvr_spi_transfer(gXcvrSpiInstance_c, (uint8_t *)&txData, 0, 3);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();
}

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
)
{
    uint8_t txData;

    if ((numOfBytes == 0) || (byteArray == NULL)) {
        return;
    }

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 16000000);

    gXcvrAssertCS_d();

    txData = TransceiverSPI_WriteSelect            |
             TransceiverSPI_PacketBuffAccessSelect |
             TransceiverSPI_PacketBuffBurstModeSelect;

    xcvr_spi_transfer(gXcvrSpiInstance_c, &txData, 0, 1);
    xcvr_spi_transfer(gXcvrSpiInstance_c, byteArray, 0, numOfBytes);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_DirectAccessSPIRead
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/

uint8_t MCR20Drv_DirectAccessSPIRead
(
    uint8_t address
)
{
    uint8_t txData;
    uint8_t rxData;

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 8000000);

    gXcvrAssertCS_d();

    txData = (address & TransceiverSPI_DirectRegisterAddressMask) |
             TransceiverSPI_ReadSelect;

    xcvr_spi_transfer(gXcvrSpiInstance_c, &txData, 0, sizeof(txData));
    xcvr_spi_transfer(gXcvrSpiInstance_c, 0, &rxData, sizeof(rxData));

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();

    return rxData;

}

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
)
{
    uint8_t  txData;
    uint8_t  phyIRQSTS1;

    if ((numOfBytes == 0) || (byteArray == NULL)) {
        return 0;
    }

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 8000000);

    gXcvrAssertCS_d();

    txData = (startAddress & TransceiverSPI_DirectRegisterAddressMask) |
             TransceiverSPI_ReadSelect;

    xcvr_spi_transfer(gXcvrSpiInstance_c, &txData, &phyIRQSTS1, sizeof(txData));
    xcvr_spi_transfer(gXcvrSpiInstance_c, 0, byteArray, numOfBytes);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();

    return phyIRQSTS1;
}

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
)
{
    uint8_t  txData;
    uint8_t  phyIRQSTS1;

    if ((numOfBytes == 0) || (byteArray == NULL)) {
        return 0;
    }

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 8000000);

    gXcvrAssertCS_d();

    txData = TransceiverSPI_ReadSelect |
             TransceiverSPI_PacketBuffAccessSelect |
             TransceiverSPI_PacketBuffBurstModeSelect;

    xcvr_spi_transfer(gXcvrSpiInstance_c, &txData, &phyIRQSTS1, sizeof(txData));
    xcvr_spi_transfer(gXcvrSpiInstance_c, 0, byteArray, numOfBytes);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();

    return phyIRQSTS1;
}

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
)
{
    uint32_t  txData;

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 16000000);

    gXcvrAssertCS_d();

    txData = TransceiverSPI_IARIndexReg;
    txData |= (address) << 8;
    txData |= (value)   << 16;

    xcvr_spi_transfer(gXcvrSpiInstance_c, (uint8_t *)&txData, 0, 3);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();
}

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
)
{
    uint16_t  txData;

    if ((numOfBytes == 0) || (byteArray == NULL)) {
        return;
    }

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 16000000);

    gXcvrAssertCS_d();

    txData = TransceiverSPI_IARIndexReg;
    txData |= (startAddress)  << 8;

    xcvr_spi_transfer(gXcvrSpiInstance_c, (uint8_t *)&txData, 0, sizeof(txData));
    xcvr_spi_transfer(gXcvrSpiInstance_c, (uint8_t *)byteArray, 0, numOfBytes);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_IndirectAccessSPIRead
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
uint8_t MCR20Drv_IndirectAccessSPIRead
(
    uint8_t address
)
{
    uint16_t  txData;
    uint8_t   rxData;

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 8000000);

    gXcvrAssertCS_d();

    txData = TransceiverSPI_IARIndexReg | TransceiverSPI_ReadSelect;
    txData |= (address) << 8;

    xcvr_spi_transfer(gXcvrSpiInstance_c, (uint8_t *)&txData, 0, sizeof(txData));
    xcvr_spi_transfer(gXcvrSpiInstance_c, 0, &rxData, sizeof(rxData));

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();

    return rxData;
}

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
)
{
    uint16_t  txData;

    if ((numOfBytes == 0) || (byteArray == NULL)) {
        return;
    }

    ProtectFromMCR20Interrupt();

    xcvr_spi_configure_speed(gXcvrSpiInstance_c, 8000000);

    gXcvrAssertCS_d();

    txData = (TransceiverSPI_IARIndexReg | TransceiverSPI_ReadSelect);
    txData |= (startAddress) << 8;

    xcvr_spi_transfer(gXcvrSpiInstance_c, (uint8_t *)&txData, 0, sizeof(txData));
    xcvr_spi_transfer(gXcvrSpiInstance_c, 0, byteArray, numOfBytes);

    gXcvrDeassertCS_d();
    UnprotectFromMCR20Interrupt();
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_IsIrqPending
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
uint32_t  MCR20Drv_IsIrqPending
(
    void
)
{
    return RF_isIRQ_Pending();
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_IRQ_Disable
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_IRQ_Disable
(
    void
)
{
    core_util_critical_section_enter();

    if (mPhyIrqDisableCnt == 0) {
        RF_IRQ_Disable();
    }

    mPhyIrqDisableCnt++;

    core_util_critical_section_exit();
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_IRQ_Enable
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_IRQ_Enable
(
    void
)
{
    core_util_critical_section_enter();

    if (mPhyIrqDisableCnt) {
        mPhyIrqDisableCnt--;

        if (mPhyIrqDisableCnt == 0) {
            RF_IRQ_Enable();
        }
    }

    core_util_critical_section_exit();
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_RST_Assert
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_RST_B_Assert
(
    void
)
{
    RF_RST_Set(0);
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_RST_Deassert
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_RST_B_Deassert
(
    void
)
{
    RF_RST_Set(1);
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_SoftRST_Assert
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_SoftRST_Assert
(
    void
)
{
    MCR20Drv_IndirectAccessSPIWrite(SOFT_RESET, (0x80));
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_SoftRST_Deassert
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_SoftRST_Deassert
(
    void
)
{
    MCR20Drv_IndirectAccessSPIWrite(SOFT_RESET, (0x00));
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_Soft_RESET
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_Soft_RESET
(
    void
)
{
    //assert SOG_RST
    MCR20Drv_IndirectAccessSPIWrite(SOFT_RESET, (0x80));

    //deassert SOG_RST
    MCR20Drv_IndirectAccessSPIWrite(SOFT_RESET, (0x00));
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_RESET
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_RESET
(
    void
)
{
#if !defined(TARGET_KW24D)
    volatile uint32_t delay = 1000;
    //assert RST_B
    MCR20Drv_RST_B_Assert();

    while (delay--);

    //deassert RST_B
    MCR20Drv_RST_B_Deassert();
#endif
}

/*---------------------------------------------------------------------------
* Name: MCR20Drv_Set_CLK_OUT_Freq
* Description: -
* Parameters: -
* Return: -
*---------------------------------------------------------------------------*/
void MCR20Drv_Set_CLK_OUT_Freq
(
    uint8_t freqDiv
)
{
    uint8_t clkOutCtrlReg = (freqDiv & cCLK_OUT_DIV_Mask) | cCLK_OUT_EN | cCLK_OUT_EXTEND;

    if (freqDiv == gCLK_OUT_FREQ_DISABLE) {
        clkOutCtrlReg = (cCLK_OUT_EXTEND | gCLK_OUT_FREQ_4_MHz); //reset value with clock out disabled
    }

    MCR20Drv_DirectAccessSPIWrite((uint8_t) CLK_OUT_CTRL, clkOutCtrlReg);
}

#endif /* MBED_CONF_NANOSTACK_CONFIGURATION */
