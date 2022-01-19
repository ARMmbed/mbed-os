/****************************************************************************
 * @file     canfd.c
 * @version  V1.00
 * @brief    CAN FD driver source file
 *
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2021 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

#include "NuMicro.h"
#include "string.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Minimum number of time quanta in a bit. */
#define MIN_TIME_QUANTA    9ul
/* Maximum number of time quanta in a bit. */
#define MAX_TIME_QUANTA    20ul
/* Number of receive FIFOs (1 - 2) */
#define CANFD_NUM_RX_FIFOS  2ul

/*CANFD max nominal bit rate*/
#define MAX_NOMINAL_BAUDRATE (1000000UL)

/* Tx Event FIFO Element ESI(Error State Indicator)  */
#define TX_FIFO_E0_EVENT_ESI_Pos   (31)
#define TX_FIFO_E0_EVENT_ESI_Msk   (0x1ul << TX_FIFO_E0_EVENT_ESI_Pos)

/* Tx Event FIFO Element XTD(Extended Identifier)    */
#define TX_FIFO_E0_EVENT_XTD_Pos   (30)
#define TX_FIFO_E0_EVENT_XTD_Msk   (0x1ul << TX_FIFO_E0_EVENT_XTD_Pos)

/* Tx Event FIFO Element RTR(Remote Transmission Request)    */
#define TX_FIFO_E0_EVENT_RTR_Pos   (29)
#define TX_FIFO_E0_EVENT_RTR_Msk   (0x1ul << TX_FIFO_E0_EVENT_RTR_Pos)

/* Tx Event FIFO Element ID(Identifier)    */
#define TX_FIFO_E0_EVENT_ID_Pos    (0)
#define TX_FIFO_E0_EVENT_ID_Msk    (0x1FFFFFFFul << TX_FIFO_E0_EVENT_ID_Pos)

/* Tx Event FIFO Element MM(Message Marker)    */
#define TX_FIFO_E1_EVENT_MM_Pos    (24)
#define TX_FIFO_E1_EVENT_MM_Msk    (0xFFul << TX_FIFO_E1_EVENT_MM_Pos)

/* Tx Event FIFO Element ET(Event Type)    */
#define TX_FIFO_E1_EVENT_ET_Pos    (22)
#define TX_FIFO_E1_EVENT_ET_Msk    (0x3ul << TX_FIFO_E1_EVENT_ET_Pos)

/* Tx Event FIFO Element FDF(FD Format)    */
#define TX_FIFO_E1_EVENT_FDF_Pos    (21)
#define TX_FIFO_E1_EVENT_FDF_Msk    (0x1ul << TX_FIFO_E1_EVENT_FDF_Pos)

/* Tx Event FIFO Element BRS(Bit Rate Switch)    */
#define TX_FIFO_E1_EVENT_BRS_Pos    (20)
#define TX_FIFO_E1_EVENT_BRS_Msk    (0x1ul << TX_FIFO_E1_EVENT_BRS_Pos)

/* Tx Event FIFO Element DLC(Data Length Code)    */
#define TX_FIFO_E1_EVENT_DLC_Pos    (16)
#define TX_FIFO_E1_EVENT_DLC_Msk    (0xFul << TX_FIFO_E1_EVENT_DLC_Pos)

/* Tx Event FIFO Element TXTS(Tx Timestamp)    */
#define TX_FIFO_E1A_EVENT_TXTS_Pos  (0)
#define TX_FIFO_E1A_EVENT_TXTS_Msk  (0xFFFFul << TX_FIFO_E1A_EVENT_TXTS_Pos)

/* Tx Event FIFO Element MM(Message Marker)    */
#define TX_FIFO_E1B_EVENT_MM_Pos    (8)
#define TX_FIFO_E1B_EVENT_MM_Msk    (0xFFul << TX_FIFO_E1B_EVENT_MM_Pos)

/* Tx Event FIFO Element TSC(Timestamp Captured)    */
#define TX_FIFO_E1B_EVENT_TSC_Pos   (4)
#define TX_FIFO_E1B_EVENT_TSC_Msk   (0x1ul << TX_FIFO_E1B_EVENT_TSC_Pos)

/* Tx Event FIFO Element TSC(Timestamp Captured)    */
#define TX_FIFO_E1B_EVENT_TXTS_Pos   (0)
#define TX_FIFO_E1B_EVENT_TXTS_Msk   (0xFul << TX_FIFO_E1B_EVENT_TSC_Pos)

/* Rx Buffer and FIFO Element ESI2(Error State Indicator)    */
#define RX_BUFFER_AND_FIFO_R0_ELEM_ESI_Pos  (31)
#define RX_BUFFER_AND_FIFO_R0_ELEM_ESI_Msk  (0x1ul << RX_BUFFER_AND_FIFO_R0_ELEM_ESI_Pos)

/* Rx Buffer and FIFO Element XTD(Extended Identifier)    */
#define RX_BUFFER_AND_FIFO_R0_ELEM_XTD_Pos  (30)
#define RX_BUFFER_AND_FIFO_R0_ELEM_XTD_Msk  (0x1ul << RX_BUFFER_AND_FIFO_R0_ELEM_XTD_Pos)

/* Rx Buffer and FIFO Element RTR(Remote Transmission Request)    */
#define RX_BUFFER_AND_FIFO_R0_ELEM_RTR_Pos  (29)
#define RX_BUFFER_AND_FIFO_R0_ELEM_RTR_Msk  (0x1ul << RX_BUFFER_AND_FIFO_R0_ELEM_RTR_Pos)

/* Rx Buffer and FIFO Element ID(Identifier)    */
#define RX_BUFFER_AND_FIFO_R0_ELEM_ID_Pos  (0)
#define RX_BUFFER_AND_FIFO_R0_ELEM_ID_Msk  (0x1FFFFFFFul << RX_BUFFER_AND_FIFO_R0_ELEM_ID_Pos)

/* Rx Buffer and FIFO Element ANMF(Accepted Non-matching Frame)    */
#define RX_BUFFER_AND_FIFO_R1_ELEM_ANMF_Pos  (31)
#define RX_BUFFER_AND_FIFO_R1_ELEM_ANMF_Msk  (0x1ul << RX_BUFFER_AND_FIFO_R1_ELEM_ANMF_Pos)

/* Rx Buffer and FIFO Element FIDX(Filter Index)    */
#define RX_BUFFER_AND_FIFO_R1_ELEM_FIDX_Pos  (24)
#define RX_BUFFER_AND_FIFO_R1_ELEM_FIDX_Msk  (0x7Ful << RX_BUFFER_AND_FIFO_R1_ELEM_FIDX_Pos)

/* Rx Buffer and FIFO Element FDF(FD Format)    */
#define RX_BUFFER_AND_FIFO_R1_ELEM_FDF_Pos  (21)
#define RX_BUFFER_AND_FIFO_R1_ELEM_FDF_Msk  (0x1ul << RX_BUFFER_AND_FIFO_R1_ELEM_FDF_Pos)

/* Rx Buffer and FIFO Element BRS(Bit Rate Swit)    */
#define RX_BUFFER_AND_FIFO_R1_ELEM_BSR_Pos  (20)
#define RX_BUFFER_AND_FIFO_R1_ELEM_BSR_Msk  (0x1ul << RX_BUFFER_AND_FIFO_R1_ELEM_BSR_Pos)

/* Rx Buffer and FIFO Element DLC(Bit Rate Swit)    */
#define RX_BUFFER_AND_FIFO_R1_ELEM_DLC_Pos  (16)
#define RX_BUFFER_AND_FIFO_R1_ELEM_DLC_Msk  (0xFul << RX_BUFFER_AND_FIFO_R1_ELEM_DLC_Pos)

/* Rx Buffer and FIFO Element RXTS(Rx Timestamp)    */
#define RX_BUFFER_AND_FIFO_R1_ELEM_RXTS_Pos  (0)
#define RX_BUFFER_AND_FIFO_R1_ELEM_RXTS_Msk  (0xFFFFul << RX_BUFFER_AND_FIFO_R1_ELEM_RXTS_Pos)

/* Tx Buffer Element ESI(Error State Indicator)    */
#define TX_BUFFER_T0_ELEM_ESI_Pos  (31)
#define TX_BUFFER_T0_ELEM_ESI_Msk  (0x1ul << TX_BUFFER_T0_ELEM_ESI_Pos)

/* Tx Buffer Element XTD(Extended Identifier)    */
#define TX_BUFFER_T0_ELEM_XTD_Pos  (30)
#define TX_BUFFER_T0_ELEM_XTD_Msk (0x1ul << TX_BUFFER_T0_ELEM_XTD_Pos)

/* Tx Buffer RTR(Remote Transmission Request)    */
#define TX_BUFFER_T0_ELEM_RTR_Pos  (29)
#define TX_BUFFER_T0_ELEM_RTR_Msk  (0x1ul << TX_BUFFER_T0_ELEM_RTR_Pos)

/* Tx Buffer Element ID(Identifier)    */
#define TX_BUFFER_T0_ELEM_ID_Pos  (0)
#define TX_BUFFER_T0_ELEM_ID_Msk  (0x1FFFFFFFul << TX_BUFFER_T0_ELEM_ID_Pos)

/* Tx Buffer Element MM(Message Marker)    */
#define TX_BUFFER_T1_ELEM_MM1_Pos  (24)
#define TX_BUFFER_T1_ELEM_MM1_Msk  (0xFFul << TX_BUFFER_T1_ELEM_MM1_Pos)

/* Tx Buffer Element EFC(Event FIFO Control)    */
#define TX_BUFFER_T1_ELEM_EFC_Pos  (23)
#define TX_BUFFER_T1_ELEM_EFC_Msk  (0xFFul << TX_BUFFER_T1_ELEM_EFC_Pos)

/* Tx Buffer Element TSCE(Time Stamp Capture Enable for TSU)    */
#define TX_BUFFER_T1_ELEM_TSCE_Pos  (22)
#define TX_BUFFER_T1_ELEM_TSCE_Msk  (0x1ul << TX_BUFFER_T1_ELEM_TSCE_Pos)

/* Tx Buffer Element FDF(FD Format)    */
#define TX_BUFFER_T1_ELEM_FDF_Pos  (21)
#define TX_BUFFER_T1_ELEM_FDF_Msk  (0x1ul << TX_BUFFER_T1_ELEM_FDF_Pos)

/* Tx Buffer Element BRS(Bit Rate Swit)    */
#define TX_BUFFER_T1_ELEM_BSR_Pos  (20)
#define TX_BUFFER_T1_ELEM_BSR_Msk  (0x1ul << TX_BUFFER_T1_ELEM_BSR_Pos)

/* Tx Buffer Element DLC(Bit Rate Swit)    */
#define TX_BUFFER_T1_ELEM_DLC_Pos  (16)
#define TX_BUFFER_T1_ELEM_DLC_Msk  (0xFul << TX_BUFFER_T1_ELEM_DLC_Pos)

/* Tx Buffer Element MM(Message Marker)    */
#define TX_BUFFER_T1_ELEM_MM0_Pos  (8)
#define TX_BUFFER_T1_ELEM_MM0_Msk  (0xFFul << TX_BUFFER_T1_ELEM_MM0_Pos)

#define CANFD_RXFS_RFL CANFD_RXF0S_RF0L_Msk

/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup CANFD_Driver CAN_FD Driver
  @{
*/

int32_t g_CANFD_i32ErrCode = 0;       /*!< CANFD global error code */

/** @addtogroup CANFD_EXPORTED_FUNCTIONS CAN_FD Exported Functions
  @{
*/

static uint32_t _GetCanfdSramBaseAddr(CANFD_T * psCanfd)
{
	if (psCanfd == CANFD0)
		return (CANFD0_SRAM_BASE_ADDR);
	else if (psCanfd == CANFD1)
		return (CANFD1_SRAM_BASE_ADDR);
	else if (psCanfd == CANFD2)
		return (CANFD2_SRAM_BASE_ADDR);
	else if (psCanfd == CANFD3)
		return (CANFD3_SRAM_BASE_ADDR);
	
	return 0;
}

/**
 * @brief       Calculates the CAN FD RAM buffer address.
 *
 * @param[in]   psConfigAddr  CAN FD element star address structure.
 * @param[in]   psConfigSize  CAN FD element size structure.
 *
 * @return      None.
 *
 * @details     Calculates the CAN FD RAM buffer address.
 */
static void CANFD_CalculateRamAddress(CANFD_RAM_PART_T *psConfigAddr, CANFD_ELEM_SIZE_T *psConfigSize)
{
    uint32_t u32RamAddrOffset = 0;

    /* Get the Standard Message ID Filter element address */
    if (psConfigSize->u32SIDFC > 0)
    {
        psConfigAddr->u32SIDFC_FLSSA = 0;
        u32RamAddrOffset += psConfigSize->u32SIDFC * sizeof(CANFD_STD_FILTER_T);
    }

    /* Get the Standard Message ID Filter element address */
    if (psConfigSize->u32XIDFC > 0)
    {
        psConfigAddr->u32XIDFC_FLESA = u32RamAddrOffset;
        u32RamAddrOffset += psConfigSize->u32XIDFC * sizeof(CANFD_EXT_FILTER_T);
    }

    /* Get the Tx Buffer element address */
    if (psConfigSize->u32TxBuf > 0)
    {
        psConfigAddr->u32TXBC_TBSA = u32RamAddrOffset;
        u32RamAddrOffset += psConfigSize->u32TxBuf * sizeof(CANFD_BUF_T);
    }

    /* Get the Rx Buffer element address */
    if (psConfigSize->u32RxBuf > 0)
    {
        psConfigAddr->u32RXBC_RBSA = u32RamAddrOffset;
        u32RamAddrOffset += psConfigSize->u32RxBuf * sizeof(CANFD_BUF_T);
    }

    /* Get the Rx FIFO0 element address */
    if (psConfigSize->u32RxFifo0 > 0)
    {
        psConfigAddr->u32RXF0C_F0SA = u32RamAddrOffset;
        u32RamAddrOffset += psConfigSize->u32RxFifo0 * sizeof(CANFD_BUF_T);
    }

    /* Get the Rx FIFO1 element address */
    if (psConfigSize->u32RxFifo1 > 0)
    {
        psConfigAddr->u32RXF1C_F1SA = u32RamAddrOffset;
        u32RamAddrOffset += psConfigSize->u32RxFifo1 * sizeof(CANFD_BUF_T);
    }

    /* Get the Rx FIFO1 element address */
    if (psConfigSize->u32TxEventFifo > 0)
    {
        psConfigAddr->u32TXEFC_EFSA = u32RamAddrOffset;
        u32RamAddrOffset += psConfigSize->u32TxEventFifo *  sizeof(CANFD_EXT_FILTER_T);
    }
}


/**
 * @brief       Get the default configuration structure.
 *
 * @param[in]   psConfig       Pointer to CAN FD configuration structure.
 * @param[in]   u8OpMode       Setting the CAN FD Operating mode.
 *
 * @return      None.
 *
 * @details     This function initializes the CAN FD configure structure to default value.
 *              The default value are:
 *              sNormBitRate.u32BitRate   = 500000bps;
 *              u32DataBaudRate     = 0(CAN mode) or 1000000(CAN FD mode) ;
 *              u32MRamSize         = 0x20010000;
 *              bEnableLoopBack     = FALSE;
 *              bBitRateSwitch      = FALSE(CAN Mode) or TRUE(CAN FD Mode);
 *              bFDEn               = FALSE(CAN Mode) or TRUE(CAN FD Mode);
 *              CAN FD Standard ID elements = 12
 *              CAN FD Extended ID elements = 10
 *              CAN FD TX Buffer elements = 3
 *              CAN FD RX Buffer elements = 3
 *              CAN FD RX FIFO0 elements = 3
 *              CAN FD RX FIFO1 elements = 3
 *              CAN FD TX Event FOFI elements = 3
*/
void CANFD_GetDefaultConfig(CANFD_FD_T *psConfig, uint8_t u8OpMode)
{
    memset(psConfig, 0, sizeof(CANFD_FD_T));

    psConfig->sBtConfig.sNormBitRate.u32BitRate = 500000;

    if (u8OpMode == CANFD_OP_CAN_MODE)
    {
        psConfig->sBtConfig.sDataBitRate.u32BitRate = 0;
        psConfig->sBtConfig.bFDEn = FALSE;
        psConfig->sBtConfig.bBitRateSwitch = FALSE;
    }
    else
    {
        psConfig->sBtConfig.sDataBitRate.u32BitRate = 10000000;
        psConfig->sBtConfig.bFDEn = TRUE;
        psConfig->sBtConfig.bBitRateSwitch = TRUE;
    }

    /*Disable the Internal Loopback mode */
    psConfig->sBtConfig.bEnableLoopBack = FALSE;
    /*Get the CAN FD memory address*/
    psConfig->u32MRamSize  = CANFD_SRAM_SIZE;

    /* CAN FD Standard message ID elements as 12 elements    */
    psConfig->sElemSize.u32SIDFC = 12;
    /* CAN FD Extended message ID elements as 10 elements    */
    psConfig->sElemSize.u32XIDFC = 10;
    /* CAN FD TX Buffer elements as 3 elements    */
    psConfig->sElemSize.u32TxBuf = 3;
    /* CAN FD RX Buffer elements as 3 elements    */
    psConfig->sElemSize.u32RxBuf = 3;
    /* CAN FD RX FIFO0 elements as 3 elements    */
    psConfig->sElemSize.u32RxFifo0 = 3;
    /* CAN FD RX FIFO1 elements as 3 elements    */
    psConfig->sElemSize.u32RxFifo1 = 3;
    /* CAN FD TX Event FOFI elements as 3 elements    */
    psConfig->sElemSize.u32TxEventFifo = 3;
    /*Calculates the CAN FD RAM buffer address*/
    CANFD_CalculateRamAddress(&psConfig->sMRamStartAddr, &psConfig->sElemSize);
}


/**
 * @brief       Encode the Data Length Code.
 *
 * @param[in]   u8NumberOfBytes  Number of bytes in a message.
 *
 * @return      Data Length Code.
 *
 * @details     Converts number of bytes in a message into a Data Length Code.
 */
static uint8_t CANFD_EncodeDLC(uint8_t u8NumberOfBytes)
{
    if (u8NumberOfBytes <= 8) return u8NumberOfBytes;
    else if (u8NumberOfBytes <= 12) return 9;
    else if (u8NumberOfBytes <= 16) return 10;
    else if (u8NumberOfBytes <= 20) return 11;
    else if (u8NumberOfBytes <= 24) return 12;
    else if (u8NumberOfBytes <= 32) return 13;
    else if (u8NumberOfBytes <= 48) return 14;
    else return 15;
}


/**
 * @brief       Decode the Data Length Code.
 *
 * @param[in]   u8Dlc   Data Length Code.
 *
 * @return      Number of bytes in a message.
 *
 * @details     Converts a Data Length Code into a number of message bytes.
 */
static uint8_t CANFD_DecodeDLC(uint8_t u8Dlc)
{
    if (u8Dlc <= 8) return u8Dlc;
    else if (u8Dlc == 9) return 12;
    else if (u8Dlc == 10) return 16;
    else if (u8Dlc == 11) return 20;
    else if (u8Dlc == 12) return 24;
    else if (u8Dlc == 13) return 32;
    else if (u8Dlc == 14) return 48;
    else return 64;
}


/**
 * @brief       Sets the CAN FD protocol timing characteristic.
 *
 * @param[in]   psCanfd     The pointer of the specified CANFD module.
 * @param[in]   psConfig    Pointer to the timing configuration structure.
 *
 * @return      None.
 *
 * @details     This function gives user settings to CAN bus timing characteristic.
 *              The function is for an experienced user. For less experienced users, call
 *              the CANFD_Open() and fill the baud rate field with a desired value.
 *              This provides the default timing characteristics to the module.
 */
static void CANFD_SetTimingConfig(CANFD_T *psCanfd, const CANFD_TIMEING_CONFIG_T *psConfig)
{
    uint32_t *pu32DBTP;

    /* configuration change enable */
    psCanfd->CCCR |= CANFD_CCCR_CCE_Msk;

    if (psCanfd == (CANFD_T *)CANFD0)
    {
        /* Get CANF D0 clock divider number */
        CLK->CLKDIV5 = (CLK->CLKDIV5 & ~CLK_CLKDIV5_CANFD0DIV_Msk) | CLK_CLKDIV5_CANFD0(psConfig->u8PreDivider) ;
    }

    /* nominal bit rate */
    psCanfd->NBTP = (((psConfig->u8NominalRJumpwidth & 0x7F) - 1) << 25) +
                    (((psConfig->u16NominalPrescaler & 0x1FF) - 1) << 16) +
                    ((((psConfig->u8NominalPhaseSeg1 + psConfig->u8NominalPropSeg) & 0xFF) - 1) << 8) +
                    (((psConfig->u8NominalPhaseSeg2 & 0x7F) - 1) << 0);


    /* canfd->DBTP */
    if (psCanfd->CCCR & CANFD_CCCR_FDOE_Msk)
    {
        pu32DBTP = (((uint32_t *)psCanfd) + 0x03);
        *pu32DBTP = (((psConfig->u8DataPrescaler & 0x1F) - 1) << 16) +
                    ((((psConfig->u8DataPhaseSeg1 + psConfig->u8DataPropSeg) & 0x1F) - 1) << 8) +
                    (((psConfig->u8DataPhaseSeg2 & 0xF) - 1) << 4) +
                    (((psConfig->u8DataRJumpwidth & 0xF) - 1) << 0);
    }
}


/**
 * @brief       Get the segment values.
 *
 * @param[in]   u32NominalBaudRate  The nominal speed in bps.
 * @param[in]   u32DataBaudRate     The data speed in bps.
 * @param[in]   u32Ntq              Number of nominal time quanta per bit.
 * @param[in]   u32Dtq              Number of data time quanta per bit.
 * @param[in]   psConfig            Passed is a configuration structure, on return the configuration is stored in the structure
 *
 * @return      None.
 *
 * @details     Calculates the segment values for a single bit time for nominal and data baudrates.
 */
static void CANFD_GetSegments(uint32_t u32NominalBaudRate, uint32_t u32DataBaudRate, uint32_t u32Ntq, uint32_t u32Dtq, CANFD_TIMEING_CONFIG_T *psConfig)
{
    float ideal_sp;
    int int32P1;

    /* get ideal sample point */
    if (u32NominalBaudRate >= 1000000)     ideal_sp = 0.750;
    else if (u32NominalBaudRate >= 800000) ideal_sp = 0.800;
    else                                   ideal_sp = 0.875;

    /* distribute time quanta */
    int32P1 = (int)(u32Ntq * ideal_sp);
    /* can controller doesn't separate prop seg and phase seg 1 */
    psConfig->u8NominalPropSeg = 0;
    /* subtract one TQ for sync seg */
    psConfig->u8NominalPhaseSeg1 = int32P1 - 1;
    psConfig->u8NominalPhaseSeg2 = u32Ntq - int32P1;
    /* sjw is 20% of total TQ, rounded to nearest int */
    psConfig->u8NominalRJumpwidth = (u32Ntq + (5 - 1)) / 5;


    /* if using baud rate switching then distribute time quanta for data rate */
    if (u32Dtq > 0)
    {
        /* get ideal sample point */
        if (u32DataBaudRate >= 1000000)     ideal_sp = 0.750;
        else if (u32DataBaudRate >= 800000) ideal_sp = 0.800;
        else                             ideal_sp = 0.875;

        /* distribute time quanta */
        int32P1 = (int)(u32Dtq * ideal_sp);
        /* can controller doesn't separate prop seg and phase seg 1 */
        psConfig->u8DataPropSeg = 0;
        /* subtract one TQ for sync seg */
        psConfig->u8DataPhaseSeg1 = int32P1 - 1;
        psConfig->u8DataPhaseSeg2 = u32Dtq - int32P1;
        /* sjw is 20% of total TQ, rounded to nearest int */
        psConfig->u8DataRJumpwidth = (u32Dtq + (5 - 1)) / 5;
    }
    else
    {
        psConfig->u8DataPropSeg = 0;
        psConfig->u8DataPhaseSeg1 = 0;
        psConfig->u8DataPhaseSeg2 = 0;
        psConfig->u8DataRJumpwidth = 0;
    }
}


/**
 * @brief       Calculates the CAN controller timing values for specific baudrates.
 *
 * @param[in]   u32NominalBaudRate  The nominal speed in bps.
 * @param[in]   u32DataBaudRate     The data speed in bps. Zero to disable baudrate switching.
 * @param[in]   u32SourceClock_Hz   CAN FD Protocol Engine clock source frequency in Hz.
 * @param[in]   psConfig            Passed is a configuration structure, on return the configuration is stored in the structure
 *
 * @return      true if timing configuration found, false if failed to find configuration.
 *
 * @details     Calculates the CAN controller timing values for specific baudrates.
 */
static uint32_t CANFD_CalculateTimingValues(uint32_t u32NominalBaudRate, uint32_t u32DataBaudRate, uint32_t u32SourceClock_Hz, CANFD_TIMEING_CONFIG_T *psConfig)
{
    int i32Nclk;
    int i32Nclk2;
    int i32Ntq;
    int i32Dclk;
    int i32Dclk2;
    int i32Dtq;

    /* observe baud rate maximums */
    if (u32NominalBaudRate > MAX_NOMINAL_BAUDRATE) u32NominalBaudRate = MAX_NOMINAL_BAUDRATE;

    for (i32Ntq = MAX_TIME_QUANTA; i32Ntq >= MIN_TIME_QUANTA; i32Ntq--)
    {
        i32Nclk = u32NominalBaudRate * i32Ntq;

        for (psConfig->u16NominalPrescaler = 0x001; psConfig->u16NominalPrescaler <= 0x400; (psConfig->u16NominalPrescaler)++)
        {
            i32Nclk2 = i32Nclk * psConfig->u16NominalPrescaler;

            if (((u32SourceClock_Hz / i32Nclk2) <= 5) && ((float)(u32SourceClock_Hz) / i32Nclk2) == (u32SourceClock_Hz / i32Nclk2))
            {
                psConfig->u8PreDivider = u32SourceClock_Hz / i32Nclk2;


                /* if not using baudrate switch then we are done */
                if (!u32DataBaudRate)
                {
                    i32Dtq = 0;
                    psConfig->u8DataPrescaler = 0;
                    CANFD_GetSegments(u32NominalBaudRate, u32DataBaudRate, i32Ntq, i32Dtq, psConfig);
                    return TRUE;
                }

                /* if baudrates are the same and the solution for nominal will work for
                data, then use the nominal settings for both */
                if ((u32DataBaudRate == u32NominalBaudRate) && psConfig->u16NominalPrescaler <= 0x20)
                {
                    i32Dtq = i32Ntq;
                    psConfig->u8DataPrescaler = (uint8_t)psConfig->u16NominalPrescaler;
                    CANFD_GetSegments(u32NominalBaudRate, u32DataBaudRate, i32Ntq, i32Dtq, psConfig);
                    return TRUE;
                }

                /* calculate data settings */
                for (i32Dtq = MAX_TIME_QUANTA; i32Dtq >= MIN_TIME_QUANTA; i32Dtq--)
                {
                    i32Dclk = u32DataBaudRate * i32Dtq;

                    for (psConfig->u8DataPrescaler = 0x01; psConfig->u8DataPrescaler <= 0x20; (psConfig->u8DataPrescaler)++)
                    {
                        i32Dclk2 = i32Dclk * psConfig->u8DataPrescaler;

                        if ((float)(u32SourceClock_Hz) / i32Dclk2 == psConfig->u8PreDivider)
                        {
                            CANFD_GetSegments(u32NominalBaudRate, u32DataBaudRate, i32Ntq, i32Dtq, psConfig);
                            return TRUE;
                        }
                    }
                }

                if (u32DataBaudRate == 0)
                {
                    CANFD_GetSegments(u32NominalBaudRate, 0, i32Ntq, 0, psConfig);
                    return TRUE;
                }
            }
        }
    }

    /* failed to find solution */
    return 0;
}


/**
 * @brief       Config message ram and Set bit-time.
 *
 * @param[in]   psCanfd     The pointer to CAN FD module base address.
 * @param[in]   psCanfdStr  message ram setting and bit-time setting
 *
 * @return      None.
 *
 * @details     Converts a Data Length Code into a number of message bytes.
 */
void CANFD_Open(CANFD_T *psCanfd, CANFD_FD_T *psCanfdStr)
{
    if (psCanfd == (CANFD_T *)CANFD0)
    {
        //CLK_EnableModuleClock(CANFD0_MODULE);
		CLK->AHBCLK1 |= BIT0;
        //SYS_ResetModule(CANFD0_RST);
        NVIC_EnableIRQ(CANFD00_IRQn);
        NVIC_EnableIRQ(CANFD01_IRQn);
    }
	else if (psCanfd == (CANFD_T *)CANFD1)
    {
        //CLK_EnableModuleClock(CANFD1_MODULE);
		CLK->AHBCLK1 |= BIT1;
        //SYS_ResetModule(CANFD1_RST);
        NVIC_EnableIRQ(CANFD10_IRQn);
        NVIC_EnableIRQ(CANFD11_IRQn);
    }
	else if (psCanfd == (CANFD_T *)CANFD2)
    {
        //CLK_EnableModuleClock(CANFD2_MODULE);
		CLK->AHBCLK1 |= BIT2;
        //SYS_ResetModule(CANFD2_RST);
        NVIC_EnableIRQ(CANFD20_IRQn);
        NVIC_EnableIRQ(CANFD21_IRQn);
    }
	else if (psCanfd == (CANFD_T *)CANFD3)
    {
        //CLK_EnableModuleClock(CANFD3_MODULE);
		CLK->AHBCLK1 |= BIT3;
        //SYS_ResetModule(CANFD3_RST);
        NVIC_EnableIRQ(CANFD30_IRQn);
        NVIC_EnableIRQ(CANFD31_IRQn);
    }


    /* configuration change enable */
    psCanfd->CCCR |= CANFD_CCCR_CCE_Msk;

    if (psCanfdStr->sBtConfig.bBitRateSwitch)
    {
        /* enable FD and baud-rate switching */
        psCanfd->CCCR |= CANFD_CCCR_BRSE_Msk;
    }

    if (psCanfdStr->sBtConfig.bFDEn)
    {
        /*FD Operation enabled*/
        psCanfd->CCCR |= CANFD_CCCR_FDOE_Msk;
    }

    /*Clear the Rx Fifo0 element setting */
    psCanfd->RXF0C = 0;
    /*Clear the Rx Fifo1 element setting */
    psCanfd->RXF1C = 0;

    /* calculate and apply timing */
    if (CANFD_CalculateTimingValues(psCanfdStr->sBtConfig.sNormBitRate.u32BitRate, psCanfdStr->sBtConfig.sDataBitRate.u32BitRate,
                                    SystemCoreClock, &psCanfdStr->sBtConfig.sConfigBitTing))
    {
        CANFD_SetTimingConfig(psCanfd, &psCanfdStr->sBtConfig.sConfigBitTing);
    }

    /* Configures the Standard ID Filter element */
    if (psCanfdStr->sElemSize.u32SIDFC != 0)
        CANFD_ConfigSIDFC(psCanfd, &psCanfdStr->sMRamStartAddr, &psCanfdStr->sElemSize);

    /*Configures the Extended ID Filter element */
    if (psCanfdStr->sElemSize.u32XIDFC != 0)
        CANFD_ConfigXIDFC(psCanfd, &psCanfdStr->sMRamStartAddr, &psCanfdStr->sElemSize);

    /*Configures the Tx Buffer element */
    if (psCanfdStr->sElemSize.u32RxBuf != 0)
        CANFD_InitTxDBuf(psCanfd, &psCanfdStr->sMRamStartAddr, &psCanfdStr->sElemSize, eCANFD_BYTE64);

    /*Configures the Rx Buffer element */
    if (psCanfdStr->sElemSize.u32RxBuf != 0)
        CANFD_InitRxDBuf(psCanfd, &psCanfdStr->sMRamStartAddr, &psCanfdStr->sElemSize, eCANFD_BYTE64);

    /*Configures the Rx Fifo0 element */
    if (psCanfdStr->sElemSize.u32RxFifo0 != 0)
        CANFD_InitRxFifo(psCanfd, 0, &psCanfdStr->sMRamStartAddr, &psCanfdStr->sElemSize, 0, eCANFD_BYTE64);

    /*Configures the Rx Fifo1 element */
    if (psCanfdStr->sElemSize.u32RxFifo1 != 0)
        CANFD_InitRxFifo(psCanfd, 1, &psCanfdStr->sMRamStartAddr, &psCanfdStr->sElemSize, 0, eCANFD_BYTE64);

    /*Configures the Tx Event FIFO element */
    if (psCanfdStr->sElemSize.u32TxEventFifo != 0)
        CANFD_InitTxEvntFifo(psCanfd, &psCanfdStr->sMRamStartAddr, &psCanfdStr->sElemSize, 0);

    /*Reject all Non-matching Frames Extended ID and Frames Standard ID,Reject all remote frames with 11-bit standard IDs and 29-bit extended IDs */
    CANFD_SetGFC(psCanfd, eCANFD_REJ_NON_MATCH_FRM, eCANFD_REJ_NON_MATCH_FRM, 1, 1);

    if (psCanfdStr->sBtConfig.bEnableLoopBack)
    {
        psCanfd->CCCR |= CANFD_CCCR_TEST_Msk;
        psCanfd->TEST |= CANFD_TEST_LBCK_Msk;
    }
}


/**
 * @brief       Close the CAN FD Bus.
 *
 * @param[in]   psCanfd   The pointer to CANFD module base address.
 *
 * @return      None.
 *
 * @details     Disable the CAN FD clock and Interrupt.
 */
void CANFD_Close(CANFD_T *psCanfd)
{
    if (psCanfd == (CANFD_T *)CANFD0)
    {
        //CLK_DisableModuleClock(CANFD0_MODULE);
		CLK->AHBCLK1 |= BIT20;
        NVIC_DisableIRQ(CANFD00_IRQn);
        NVIC_DisableIRQ(CANFD01_IRQn);
    }
	else if (psCanfd == (CANFD_T *)CANFD1)
    {
        //CLK_DisableModuleClock(CANFD1_MODULE);
		CLK->AHBCLK1 |= BIT21;
        NVIC_DisableIRQ(CANFD10_IRQn);
        NVIC_DisableIRQ(CANFD11_IRQn);
    }
	else if (psCanfd == (CANFD_T *)CANFD2)
    {
        //CLK_DisableModuleClock(CANFD2_MODULE);
		CLK->AHBCLK1 |= BIT22;
        NVIC_DisableIRQ(CANFD20_IRQn);
        NVIC_DisableIRQ(CANFD21_IRQn);
    }
	else if (psCanfd == (CANFD_T *)CANFD3)
    {
        //CLK_DisableModuleClock(CANFD3_MODULE);
		CLK->AHBCLK1 |= BIT23;
        NVIC_DisableIRQ(CANFD30_IRQn);
        NVIC_DisableIRQ(CANFD31_IRQn);
    }
}


/**
 * @brief       Get the element's address when read transmit buffer.
 *
 * @param[in]   psCanfd      The pointer of the specified CAN FD module.
 * @param[in]   u32Idx       The number of the transmit buffer element
 *
 * @return      Address of the element in transmit buffer.
 *
 * @details     The function is used to get the element's address when read transmit buffer.
 */
static uint32_t CANFD_GetTxBufferElementAddress(CANFD_T *psCanfd, uint32_t u32Idx)
{
    uint32_t u32Size = 0;
    u32Size = (psCanfd->TXESC & CANFD_TXESC_TBDS_Msk) >> CANFD_TXESC_TBDS_Pos;

    if (u32Size < 5U)
    {
        u32Size += 4U;
    }
    else
    {
        u32Size = u32Size * 4U - 10U;
    }

    return (psCanfd->TXBC & CANFD_TXBC_TBSA_Msk) + u32Idx * u32Size * 4U;
}


/**
 * @brief       Enables CAN FD interrupts according to provided mask .
 *
 * @param[in]   psCanfd          The pointer of the specified CAN FD module.
 * @param[in]   u32IntLine0      The Interrupt Line 0 type select.
 * @param[in]   u32IntLine1      The Interrupt Line 1 type select.
 *                              - \ref CANFD_IE_ARAE_Msk     : Access to Reserved Address Interrupt
 *                              - \ref CANFD_IE_PEDE_Msk     : Protocol Error in Data Phase Interrupt
 *                              - \ref CANFD_IE_PEAE_Msk     : Protocol Error in Arbitration Phase Interrupt
 *                              - \ref CANFD_IE_WDIE_Msk     : Watchdog Interrupt
 *                              - \ref CANFD_IE_BOE_Msk      : Bus_Off Status Interrupt
 *                              - \ref CANFD_IE_EWE_Msk      : Warning Status Interrupt
 *                              - \ref CANFD_IE_EPE_Msk      : Error Passive Interrupt
 *                              - \ref CANFD_IE_ELOE_Msk     : Error Logging Overflow Interrupt
 *                              - \ref CANFD_IE_BEUE_Msk     : Bit Error Uncorrected Interrupt
 *                              - \ref CANFD_IE_BECE_Msk     : Bit Error Corrected Interrupt
 *                              - \ref CANFD_IE_DRXE_Msk     : Message stored to Dedicated Rx Buffer Interrupt
 *                              - \ref CANFD_IE_TOOE_Msk     : Timeout Occurred Interrupt
 *                              - \ref CANFD_IE_MRAFE_Msk    : Message RAM Access Failure Interrupt
 *                              - \ref CANFD_IE_TSWE_Msk     : Timestamp Wraparound Interrupt
 *                              - \ref CANFD_IE_TEFLE_Msk    : Tx Event FIFO Event Lost Interrupt
 *                              - \ref CANFD_IE_TEFFE_Msk    : Tx Event FIFO Full Interrupt
 *                              - \ref CANFD_IE_TEFWE_Msk    : Tx Event FIFO Watermark Reached Interrupt
 *                              - \ref CANFD_IE_TEFNE_Msk    : Tx Event FIFO New Entry Interrupt
 *                              - \ref CANFD_IE_TFEE_Msk     : Tx FIFO Empty Interrupt
 *                              - \ref CANFD_IE_TCFE_Msk     : Transmission Cancellation Finished Interrupt
 *                              - \ref CANFD_IE_TCE_Msk      : Transmission Completed Interrupt
 *                              - \ref CANFD_IE_HPME_Msk     : High Priority Message Interrupt
 *                              - \ref CANFD_IE_RF1LE_Msk    : Rx FIFO 1 Message Lost Interrupt
 *                              - \ref CANFD_IE_RF1FE_Msk    : Rx FIFO 1 Full Interrupt
 *                              - \ref CANFD_IE_RF1WE_Msk    : Rx FIFO 1 Watermark Reached Interrupt
 *                              - \ref CANFD_IE_RF1NE_Msk    : Rx FIFO 1 New Message Interrupt
 *                              - \ref CANFD_IE_RF0LE_Msk    : Rx FIFO 0 Message Lost Interrupt
 *                              - \ref CANFD_IE_RF0FE_Msk    : Rx FIFO 0 Full Interrupt
 *                              - \ref CANFD_IE_RF0WE_Msk    : Rx FIFO 0 Watermark Reached Interrupt
 *                              - \ref CANFD_IE_RF0NE_Msk    : Rx FIFO 0 New Message Interrupt
 *
 * @param[in]   u32TXBTIE        Enable Tx Buffer Transmission 0-31 Interrupt.
 * @param[in]   u32TXBCIE        Enable Tx Buffer Cancellation Finished 0-31 Interrupt.
 * @return      None.
 *
 * @details     This macro enable specified CAN FD interrupt.
 */
void CANFD_EnableInt(CANFD_T *psCanfd, uint32_t u32IntLine0, uint32_t u32IntLine1, uint32_t u32TXBTIE, uint32_t u32TXBCIE)
{

    if (u32IntLine0 != 0)
    {
        /*Setting the CANFD0_IRQ0 Interrupt*/
        psCanfd->IE |= u32IntLine0;
        /* Enable CAN FD specified interrupt */
        psCanfd->ILE |= ((uint32_t)1U << 0);
    }

    if (u32IntLine1 != 0)
    {
        /*Setting the CANFD0_IRQ1 Interrupt*/
        psCanfd->ILS |= u32IntLine1;
        /* Enable CAN FD specified interrupt */
        psCanfd->ILE |= ((uint32_t)1U << 1);
    }

    /*Setting the Tx Buffer Transmission Interrupt Enable*/
    psCanfd->TXBTIE |= u32TXBTIE;

    /*Tx Buffer Cancellation Finished Interrupt Enable*/
    psCanfd->TXBCIE |= u32TXBCIE;
}


/**
 * @brief       Disables CAN FD interrupts according to provided mask .
 *
 * @param[in]   psCanfd          The pointer of the specified CAN FD module.
 * @param[in]   u32IntLine0      The Interrupt Line 0 type select.
 * @param[in]   u32IntLine1      The Interrupt Line 1 type select.
 *                              - \ref CANFD_IE_ARAE_Msk     : Access to Reserved Address Interrupt
 *                              - \ref CANFD_IE_PEDE_Msk     : Protocol Error in Data Phase Interrupt
 *                              - \ref CANFD_IE_PEAE_Msk     : Protocol Error in Arbitration Phase Interrupt
 *                              - \ref CANFD_IE_WDIE_Msk     : Watchdog Interrupt
 *                              - \ref CANFD_IE_BOE_Msk      : Bus_Off Status Interrupt
 *                              - \ref CANFD_IE_EWE_Msk      : Warning Status Interrupt
 *                              - \ref CANFD_IE_EPE_Msk      : Error Passive Interrupt
 *                              - \ref CANFD_IE_ELOE_Msk     : Error Logging Overflow Interrupt
 *                              - \ref CANFD_IE_BEUE_Msk     : Bit Error Uncorrected Interrupt
 *                              - \ref CANFD_IE_BECE_Msk     : Bit Error Corrected Interrupt
 *                              - \ref CANFD_IE_DRXE_Msk     : Message stored to Dedicated Rx Buffer Interrupt
 *                              - \ref CANFD_IE_TOOE_Msk     : Timeout Occurred Interrupt
 *                              - \ref CANFD_IE_MRAFE_Msk    : Message RAM Access Failure Interrupt
 *                              - \ref CANFD_IE_TSWE_Msk     : Timestamp Wraparound Interrupt
 *                              - \ref CANFD_IE_TEFLE_Msk    : Tx Event FIFO Event Lost Interrupt
 *                              - \ref CANFD_IE_TEFFE_Msk    : Tx Event FIFO Full Interrupt
 *                              - \ref CANFD_IE_TEFWE_Msk    : Tx Event FIFO Watermark Reached Interrupt
 *                              - \ref CANFD_IE_TEFNE_Msk    : Tx Event FIFO New Entry Interrupt
 *                              - \ref CANFD_IE_TFEE_Msk     : Tx FIFO Empty Interrupt
 *                              - \ref CANFD_IE_TCFE_Msk     : Transmission Cancellation Finished Interrupt
 *                              - \ref CANFD_IE_TCE_Msk      : Transmission Completed Interrupt
 *                              - \ref CANFD_IE_HPME_Msk     : High Priority Message Interrupt
 *                              - \ref CANFD_IE_RF1LE_Msk    : Rx FIFO 1 Message Lost Interrupt
 *                              - \ref CANFD_IE_RF1FE_Msk    : Rx FIFO 1 Full Interrupt
 *                              - \ref CANFD_IE_RF1WE_Msk    : Rx FIFO 1 Watermark Reached Interrupt
 *                              - \ref CANFD_IE_RF1NE_Msk    : Rx FIFO 1 New Message Interrupt
 *                              - \ref CANFD_IE_RF0LE_Msk    : Rx FIFO 0 Message Lost Interrupt
 *                              - \ref CANFD_IE_RF0FE_Msk    : Rx FIFO 0 Full Interrupt
 *                              - \ref CANFD_IE_RF0WE_Msk    : Rx FIFO 0 Watermark Reached Interrupt
 *                              - \ref CANFD_IE_RF0NE_Msk    : Rx FIFO 0 New Message Interrupt
 *
 * @param[in]   u32TXBTIE        Disable Tx Buffer Transmission 0-31 Interrupt.
 * @param[in]   u32TXBCIE        Disable Tx Buffer Cancellation Finished 0-31 Interrupt.
 * @return      None.
 *
 * @details     This macro disable specified CAN FD interrupt.
 */
void CANFD_DisableInt(CANFD_T *psCanfd, uint32_t u32IntLine0, uint32_t u32IntLine1, uint32_t u32TXBTIE, uint32_t u32TXBCIE)
{
    if (u32IntLine0 != 0)
    {
        /*Clear the CANFD0_IRQ0 Interrupt*/
        psCanfd->IE &= ~u32IntLine0;
        /* Disable CAN FD specified interrupt */
        psCanfd->ILE &= ~((uint32_t)1U << 0);
    }

    if (u32IntLine1 != 0)
    {
        /*Clear the CANFD0_IRQ1 Interrupt*/
        psCanfd->ILS &= ~u32IntLine1;
        /* Disable CAN FD specified interrupt */
        psCanfd->ILE &= ~((uint32_t)1U << 1);
    }

    /*Setting the Tx Buffer Transmission Interrupt Disable*/
    psCanfd->TXBTIE &= ~u32TXBTIE;

    /*Tx Buffer Cancellation Finished Interrupt Disable*/
    psCanfd->TXBCIE &= ~u32TXBCIE;
}


/**
 * @brief       Copy Tx Message to  TX buffer and Request transmission.
 *
 * @param[in]   psCanfd         The pointer to CAN FD module base address.
 * @param[in]   u32TxBufIdx     The Message Buffer index.
 * @param[in]   psTxMsg         Message to be copied.
 *
 * @return      number of tx requests set: 0= Tx Message Buffer is currently in use.
 *                                         1= Write Tx Message Buffer Successfully.
 *
 * @details     Copy Tx Message to FIFO/Queue TX buffer and Request transmission.
 */
uint32_t CANFD_TransmitTxMsg(CANFD_T *psCanfd, uint32_t u32TxBufIdx, CANFD_FD_MSG_T *psTxMsg)
{
    uint32_t u32Success = 0;
    uint32_t u32TimeOutCount = CANFD_TIMEOUT;

    /* write the message to the message buffer */
    u32Success = CANFD_TransmitDMsg(psCanfd, u32TxBufIdx, psTxMsg);

    if (u32Success == 1)
    {
        /* wait for completion */
        while (!(psCanfd->TXBRP & (1UL << u32TxBufIdx)))
        {
            if(u32TimeOutCount-- == 0)
            {
                u32Success = 0;
                break;
            }

        }
    }

    return u32Success;
}


/**
 * @brief       Writes a Tx Message to Transmit Message Buffer.
 *
 * @param[in]   psCanfd        The pointer of the specified CAN FD module.
 * @param[in]   u32TxBufIdx    The Message Buffer index.
 * @param[in]   psTxMsg        Pointer to CAN FD message frame to be sent.
 *
 * @return      1  Write Tx Message Buffer Successfully.
 *              0  Tx Message Buffer is currently in use.
 *
 * @details     This function writes a CANFD Message to the specified Transmit Message Buffer
 *              and changes the Message Buffer state to start CANFD Message transmit. After
 *              that the function returns immediately.
 */
uint32_t CANFD_TransmitDMsg(CANFD_T *psCanfd, uint32_t u32TxBufIdx, CANFD_FD_MSG_T *psTxMsg)
{
    CANFD_BUF_T *psTxBuffer;
    uint32_t u32Idx = 0, u32Success = 1;
	uint32_t u32SramBaseAddr;

    if (u32TxBufIdx >= CANFD_MAX_TX_BUF_ELEMS) return 0;

    /* transmission is pending in this message buffer */
    if (psCanfd->TXBRP & (1UL << u32TxBufIdx)) return 0;

    //psTxBuffer = (CANFD_BUF_T *)(CANFD_SRAM_BASE_ADDR + (psCanfd->TXBC & 0xFFFF) + (u32TxBufIdx * sizeof(CANFD_BUF_T)));
	u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
	psTxBuffer = (CANFD_BUF_T *)(u32SramBaseAddr + (psCanfd->TXBC & 0xFFFF) + (u32TxBufIdx * sizeof(CANFD_BUF_T)));

    if (psTxMsg->eIdType == eCANFD_XID)
    {
        psTxBuffer->u32Id = TX_BUFFER_T0_ELEM_XTD_Msk | (psTxMsg->u32Id & 0x1FFFFFFF);
    }
    else
    {
        psTxBuffer->u32Id = (psTxMsg->u32Id & 0x7FF) << 18;
    }

    if (psTxMsg->eFrmType == eCANFD_REMOTE_FRM) psTxBuffer->u32Id |= TX_BUFFER_T0_ELEM_RTR_Msk;

    psTxBuffer->u32Config = (CANFD_EncodeDLC(psTxMsg->u32DLC) << 16);

    if (psTxMsg->bFDFormat) psTxBuffer->u32Config |= TX_BUFFER_T1_ELEM_FDF_Msk;

    if (psTxMsg->bBitRateSwitch) psTxBuffer->u32Config |= TX_BUFFER_T1_ELEM_BSR_Msk;


    for (u32Idx = 0; u32Idx < (psTxMsg->u32DLC + (4 - 1)) / 4; u32Idx++)
    {
        psTxBuffer->au32Data[u32Idx] = psTxMsg->au32Data[u32Idx];
    }

    psCanfd->TXBAR = (1 << u32TxBufIdx);

    return u32Success;
}


/**
 * @brief       Global Filter Configuration (GFC).
 *
 * @param[in]   psCanfd          The pointer to CAN FD module base address.
 * @param[in]   eNMStdFrm        Accept/Reject Non-Matching Standard(11-bits) Frames.
 * @param[in]   eEMExtFrm        Accept/Reject Non-Matching Extended(29-bits) Frames.
 * @param[in]   u32RejRmtStdFrm  Reject/Filter Remote Standard Frames.
 * @param[in]   u32RejRmtExtFrm  Reject/Filter Remote Extended Frames.
 *
 * @return      None.
 *
 * @details     Global Filter Configuration.
 */
void CANFD_SetGFC(CANFD_T *psCanfd, E_CANFD_ACC_NON_MATCH_FRM eNMStdFrm, E_CANFD_ACC_NON_MATCH_FRM eEMExtFrm, uint32_t u32RejRmtStdFrm, uint32_t u32RejRmtExtFrm)
{
    psCanfd->GFC &= (CANFD_GFC_RRFS_Msk | CANFD_GFC_RRFE_Msk);
    psCanfd->GFC = (eNMStdFrm << CANFD_GFC_ANFS_Pos) | (eEMExtFrm << CANFD_GFC_ANFE_Pos)
                   | (u32RejRmtStdFrm << CANFD_GFC_RRFS_Pos) | (u32RejRmtExtFrm << CANFD_GFC_RRFE_Pos);
}


/**
 * @brief       Rx FIFO Configuration for RX_FIFO_0 and RX_FIFO_1.
 *
 * @param[in]   psCanfd          The pointer to CAN FD module base address.
 * @param[in]   u32RxFifoNum     0: RX FIFO_0, 1: RX_FIFO_1.
 * @param[in]   psRamConfig      Rx FIFO Size in number of configuration ram address.
 * @param[in]   psElemSize       Rx FIFO Size in number of Rx FIFO elements (element number (max. = 64)).
 * @param[in]   u32FifoWM        Watermark in number of Rx FIFO elements
 * @param[in]   eFifoSize        Maximum data field size that should be stored in this Rx FIFO
 *                               (configure BYTE64 if you are unsure, as this is the largest data field allowed in CAN FD)
 *
 * @return      None.
 *
 * @details     Rx FIFO Configuration for RX_FIFO_0 and RX_FIFO_1.
 */
void CANFD_InitRxFifo(CANFD_T *psCanfd, uint32_t u32RxFifoNum, CANFD_RAM_PART_T *psRamConfig, CANFD_ELEM_SIZE_T *psElemSize, uint32_t u32FifoWM, E_CANFD_DATA_FIELD_SIZE eFifoSize)
{
    uint32_t u32Address;
    uint32_t u32Size;
	uint32_t u32SramBaseAddr;

    /* ignore if index is too high */
    if (u32RxFifoNum > CANFD_NUM_RX_FIFOS)return;

    /* ignore if index is too high */
    if (psElemSize-> u32RxFifo0 > CANFD_MAX_RX_FIFO0_ELEMS) return;

    /* ignore if index is too high */
    if (psElemSize-> u32RxFifo1 > CANFD_MAX_RX_FIFO1_ELEMS) return;

    switch (u32RxFifoNum)
    {
        case 0:
            if (psElemSize-> u32RxFifo0)
            {
                /* set size of Rx FIFO 0, set offset, blocking mode */
                psCanfd->RXF0C = (psRamConfig->u32RXF0C_F0SA) | (psElemSize->u32RxFifo0 << CANFD_RXF0C_F0S_Pos)
                                 | (u32FifoWM << CANFD_RXF0C_F0WM_Pos);
                psCanfd->RXESC = (psCanfd->RXESC & (~CANFD_RXESC_F0DS_Msk)) | (eFifoSize << CANFD_RXESC_F0DS_Pos);
                /*Get the RX FIFO 0 Start Address in the RAM*/
                //u32Address = CANFD_SRAM_BASE_ADDR + (psRamConfig->u32RXF0C_F0SA & CANFD_RXF0C_F0SA_Msk);
				u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
				u32Address = u32SramBaseAddr + (psRamConfig->u32RXF0C_F0SA & CANFD_RXF0C_F0SA_Msk);
                u32Size = eFifoSize;

                if (u32Size < 5U)
                {
                    u32Size += 4U;
                }
                else
                {
                    u32Size = u32Size * 4U - 10U;
                }

                /*Clear the RX FIFO 0 Memory*/
                memset((uint32_t *)(u32Address), 0x00, (u32Size * 4 * psElemSize->u32RxFifo0));
            }
            else
            {
                psCanfd->RXF0C = 0;
            }

            break;

        case 1:
            if (psElemSize-> u32RxFifo1)
            {

                /* set size of Rx FIFO 1, set offset, blocking mode */
                psCanfd->RXF1C = (psRamConfig->u32RXF1C_F1SA) | (psElemSize->u32RxFifo1 << CANFD_RXF1C_F1S_Pos)
                                 | (u32FifoWM << CANFD_RXF1C_F1WM_Pos);
                psCanfd->RXESC = (psCanfd->RXESC & (~CANFD_RXESC_F1DS_Msk)) | (eFifoSize << CANFD_RXESC_F1DS_Pos);
                /*Get the RX FIFO 1 Start Address in the RAM*/
                //u32Address = CANFD_SRAM_BASE_ADDR + (psRamConfig->u32RXF1C_F1SA & CANFD_RXF1C_F1SA_Msk);
				u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
				u32Address = u32SramBaseAddr + (psRamConfig->u32RXF1C_F1SA & CANFD_RXF1C_F1SA_Msk);

                u32Size = eFifoSize;

                if (u32Size < 5U)
                {
                    u32Size += 4U;
                }
                else
                {
                    u32Size = u32Size * 4U - 10U;
                }

                /*Clear the RX FIFO 0 Memory*/
                memset((uint32_t *)(u32Address), 0x00, (u32Size * 4 * psElemSize->u32RxFifo1));
            }
            else
            {
                psCanfd->RXF1C = 0;
            }

            break;
    }
}


/**
 * @brief       Function configures the data structures used by a dedicated Rx Buffer.
 *
 * @param[in]   psCanfd          The pointer to CAN FD module base address.
 * @param[in]   psRamConfig      Tx buffer configuration ram address.
 * @param[in]   psElemSize       Tx buffer configuration element size.
 * @param[in]   eTxBufSize       Maximum data field size that should be stored in a dedicated Tx Buffer
 *                              (configure BYTE64 if you are unsure, as this is the largest data field allowed in CAN FD)largest data field allowed in CAN FD)
 *
 * @return      None.
 *
 * @details     Function configures the data structures used by a dedicated Rx Buffer.
 */
void CANFD_InitTxDBuf(CANFD_T *psCanfd, CANFD_RAM_PART_T *psRamConfig, CANFD_ELEM_SIZE_T *psElemSize, E_CANFD_DATA_FIELD_SIZE eTxBufSize)
{
    uint32_t u32Address;
    uint32_t u32Size;
	uint32_t u32SramBaseAddr;
    /*Setting the Tx Buffer Start Address*/
    psCanfd->TXBC = ((psElemSize->u32TxBuf & 0x3F) << CANFD_TXBC_NDTB_Pos) | (psRamConfig->u32TXBC_TBSA & CANFD_TXBC_TBSA_Msk);
    /*Get the TX Buffer Start Address in the RAM*/
    //u32Address = CANFD_SRAM_BASE_ADDR + (psRamConfig->u32TXBC_TBSA & CANFD_TXBC_TBSA_Msk);
	u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
	u32Address = u32SramBaseAddr + (psRamConfig->u32TXBC_TBSA & CANFD_TXBC_TBSA_Msk);
    /*Setting the Tx Buffer Data Field Size*/
    psCanfd->TXESC = (psCanfd->TXESC & (~CANFD_TXESC_TBDS_Msk)) | (eTxBufSize <<  CANFD_TXESC_TBDS_Pos);
    /*Get the Buffer Data Field Size*/
    u32Size = eTxBufSize;

    if (u32Size < 5U)
    {
        u32Size += 4U;
    }
    else
    {
        u32Size = u32Size * 4U - 10U;
    }

    /*Clear the TX Buffer Memory*/
    memset((uint32_t *)(u32Address), 0x00, (u32Size * 4 * psElemSize->u32TxBuf));
}


/**
 * @brief       Function configures the data structures used by a dedicated Rx Buffer.
 *
 * @param[in]   psCanfd          The pointer to CAN FD module base address.
 * @param[in]   psRamConfig      Rx buffer configuration ram address.
 * @param[in]   psElemSize       Rx buffer configuration element size.
 * @param[in]   eRxBufSize       Maximum data field size that should be stored in a dedicated Rx Buffer
 *                              (configure BYTE64 if you are unsure, as this is the largest data field allowed in CAN FD)largest data field allowed in CAN FD)
 *
 * @return      None.
 *
 * @details     Function configures the data structures used by a dedicated Rx Buffer.
 */
void CANFD_InitRxDBuf(CANFD_T *psCanfd, CANFD_RAM_PART_T *psRamConfig, CANFD_ELEM_SIZE_T *psElemSize, E_CANFD_DATA_FIELD_SIZE eRxBufSize)
{
    uint32_t u32Address;
    uint32_t u32Size;
	uint32_t u32SramBaseAddr;
    /*Setting the Rx Buffer Start Address*/
    psCanfd->RXBC = (psRamConfig->u32RXBC_RBSA & CANFD_RXBC_RBSA_Msk);
    /*Get the RX Buffer Start Address in the RAM*/
    //u32Address = CANFD_SRAM_BASE_ADDR + (psRamConfig->u32RXBC_RBSA & CANFD_RXBC_RBSA_Msk);
	u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
	u32Address = u32SramBaseAddr + (psRamConfig->u32RXBC_RBSA & CANFD_RXBC_RBSA_Msk);
    /*Setting the Rx Buffer Data Field Size*/
    psCanfd->RXESC = (psCanfd->RXESC & (~CANFD_RXESC_RBDS_Msk)) | (eRxBufSize <<  CANFD_RXESC_RBDS_Pos);
    /*Get the Buffer Data Field Size*/
    u32Size = eRxBufSize;

    if (u32Size < 5U)
    {
        u32Size += 4U;
    }
    else
    {
        u32Size = u32Size * 4U - 10U;
    }

    /*Clear the RX Buffer Memory*/
    memset((uint32_t *)(u32Address), 0x00, (u32Size * 4 * psElemSize->u32RxBuf));
}


/**
 * @brief       Configures the register SIDFC for the 11-bit Standard Message ID Filter elements.
 *
 * @param[in]   psCanfd           The pointer to CAN FD module base address.
 * @param[in]   psRamConfig       Standard ID filter configuration ram address
 * @param[in]   psElemSize        Standard ID filter configuration element size
 *
 * @return      None.
 *
 * @details     Function configures the data structures used by a dedicated Rx Buffer.
 */
void CANFD_ConfigSIDFC(CANFD_T *psCanfd, CANFD_RAM_PART_T *psRamConfig, CANFD_ELEM_SIZE_T *psElemSize)
{
    uint32_t u32Address;
	uint32_t u32SramBaseAddr;
    /*Setting the Filter List Standard Start Address and List Size  */
    psCanfd->SIDFC = ((psElemSize->u32SIDFC & 0xFF) << CANFD_SIDFC_LSS_Pos) | (psRamConfig->u32SIDFC_FLSSA & CANFD_SIDFC_FLSSA_Msk);
    /*Get the Filter List Standard Start Address in the RAM*/
    //u32Address = CANFD_SRAM_BASE_ADDR + (psRamConfig->u32SIDFC_FLSSA & CANFD_SIDFC_FLSSA_Msk);
	u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
	u32Address = u32SramBaseAddr + (psRamConfig->u32SIDFC_FLSSA & CANFD_SIDFC_FLSSA_Msk);
    /*Clear the Filter List Memory*/
    memset((uint32_t *)(u32Address), 0x00, (psElemSize->u32SIDFC * 4));
}


/**
 * @brief       Configures the register XIDFC for the 29-bit Extended Message ID Filter elements.
 *
 * @param[in]   psCanfd           The pointer to CAN FD module base address.
 * @param[in]   psRamConfig       Extended ID filter configuration ram address
 * @param[in]   psElemSize        Extended ID filter configuration element size
 *
 * @return      None.
 *
 * @details     Configures the register XIDFC for the 29-bit Extended Message ID Filter elements.
 */
void CANFD_ConfigXIDFC(CANFD_T *psCanfd, CANFD_RAM_PART_T *psRamConfig, CANFD_ELEM_SIZE_T *psElemSize)
{
    uint32_t u32Address;
	uint32_t u32SramBaseAddr;
    /*Setting the Filter List Extended Start Address and List Size  */
    psCanfd->XIDFC = ((psElemSize->u32XIDFC & 0xFF) << CANFD_XIDFC_LSE_Pos) | (psRamConfig->u32XIDFC_FLESA & CANFD_XIDFC_FLESA_Msk);
    /*Get the Filter List Standard Start Address in the RAM*/
    //u32Address = CANFD_SRAM_BASE_ADDR + (psRamConfig->u32XIDFC_FLESA & CANFD_XIDFC_FLESA_Msk);
	u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
	u32Address = u32SramBaseAddr + (psRamConfig->u32XIDFC_FLESA & CANFD_XIDFC_FLESA_Msk);
    /*Clear the Filter List Memory*/
    memset((uint32_t *)(u32Address), 0x00, (psElemSize->u32XIDFC * 8));
}


/**
 * @brief       Writes a 11-bit Standard ID filter element in the Message RAM.
 *
 * @param[in]   psCanfd          The pointer to CAN FD module base address.
 * @param[in]   u32FltrIdx       Index at which the filter element should be written in the '11-bit Filter' section of Message RAM
 * @param[in]   u32Filter        Rx Individual filter value.
 *
 * @return      None.
 *
 * @details     Writes a 11-bit Standard ID filter element in the Message RAM.
 */
void CANFD_SetSIDFltr(CANFD_T *psCanfd, uint32_t u32FltrIdx, uint32_t u32Filter)
{
    CANFD_STD_FILTER_T *psFilter;
	uint32_t u32SramBaseAddr;

    /* ignore if index is too high */
    if (u32FltrIdx >= CANFD_MAX_11_BIT_FTR_ELEMS) return;

    /*Get the Filter List Configuration Address in the RAM*/
    //psFilter = (CANFD_STD_FILTER_T *)(CANFD_SRAM_BASE_ADDR + (psCanfd->SIDFC & CANFD_SIDFC_FLSSA_Msk) + (u32FltrIdx * sizeof(CANFD_STD_FILTER_T)));
	u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
	psFilter = (CANFD_STD_FILTER_T *)(u32SramBaseAddr + (psCanfd->SIDFC & CANFD_SIDFC_FLSSA_Msk) + (u32FltrIdx * sizeof(CANFD_STD_FILTER_T)));
    /*Wirted the Standard ID filter element to RAM */
    psFilter->VALUE = u32Filter;
}


/**
 * @brief       Writes a 29-bit extended id filter element in the Message RAM.
 *              Size of an Extended Id filter element is 2 words. So 2 words are written into the Message RAM for each filter element
 *
 * @param[in]   psCanfd         The pointer to CAN FD module base address.
 * @param[in]   u32FltrIdx      Index at which the filter element should be written in the '29-bit Filter' section of Message RAM.
 * @param[in]   u32FilterLow    Rx Individual filter low value.
 * @param[in]   u32FilterHigh   Rx Individual filter high value.
 *
 * @return      None.
 *
 * @details     Writes a 29-bit extended id filter element in the Message RAM.
 */
void CANFD_SetXIDFltr(CANFD_T *psCanfd, uint32_t u32FltrIdx, uint32_t u32FilterLow, uint32_t u32FilterHigh)
{
    CANFD_EXT_FILTER_T *psFilter;
	uint32_t u32SramBaseAddr;

    /* ignore if index is too high */
    if (u32FltrIdx >= CANFD_MAX_29_BIT_FTR_ELEMS) return;

    /*Get the Filter List Configuration Address on RAM*/
    //psFilter = (CANFD_EXT_FILTER_T *)(CANFD_SRAM_BASE_ADDR + (psCanfd->XIDFC & CANFD_XIDFC_FLESA_Msk) + (u32FltrIdx * sizeof(CANFD_EXT_FILTER_T)));
	u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
	psFilter = (CANFD_EXT_FILTER_T *)(u32SramBaseAddr + (psCanfd->XIDFC & CANFD_XIDFC_FLESA_Msk) + (u32FltrIdx * sizeof(CANFD_EXT_FILTER_T)));
    /*Wirted the Extended ID filter element to RAM */
    psFilter->LOWVALUE = u32FilterLow;
    psFilter->HIGHVALUE = u32FilterHigh;
}


/**
 * @brief       Reads a CAN FD Message from Receive Message Buffer.
 *
 * @param[in]   psCanfd     The pointer of the specified CAN FD module.
 * @param[in]   u8MbIdx     The CANFD Message Buffer index.
 * @param[in]   psMsgBuf    Pointer to CAN FD message frame structure for reception.
 *
 * @return       1:Rx Message Buffer is full and has been read successfully.
 *               0:Rx Message Buffer is empty.
 *
 * @details     This function reads a CAN message from a specified Receive Message Buffer.
 *              The function fills a receive CAN message frame structure with just received data
 *              and activates the Message Buffer again.The function returns immediately.
*/
uint32_t CANFD_ReadRxBufMsg(CANFD_T *psCanfd, uint8_t u8MbIdx, CANFD_FD_MSG_T *psMsgBuf)
{
    CANFD_BUF_T *psRxBuffer;
    uint32_t u32Success = 0;
    uint32_t newData = 0;
	uint32_t u32SramBaseAddr;

    if (u8MbIdx < CANFD_MAX_RX_BUF_ELEMS)
    {
        if (u8MbIdx < 32)
            newData = (psCanfd->NDAT1 >> u8MbIdx) & 1;
        else
            newData = (psCanfd->NDAT2 >> (u8MbIdx - 32)) & 1;

        /* new message is waiting to be read */
        if (newData)
        {
            /* get memory location of rx buffer */
            //psRxBuffer = (CANFD_BUF_T *)(CANFD_SRAM_BASE_ADDR + (psCanfd->RXBC & 0xFFFF) + (u8MbIdx * sizeof(CANFD_BUF_T)));
			u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
			psRxBuffer = (CANFD_BUF_T *)(u32SramBaseAddr + (psCanfd->RXBC & 0xFFFF) + (u8MbIdx * sizeof(CANFD_BUF_T)));

            /* read the message */
            CANFD_CopyDBufToMsgBuf(psRxBuffer, psMsgBuf);

            /* clear 'new data' flag */
            if (u8MbIdx < 32)
                psCanfd->NDAT1 |= (1UL << u8MbIdx);
            else
                psCanfd->NDAT2 |= (1UL << (u8MbIdx - 32));

            u32Success = 1;
        }
    }

    return u32Success;
}


/**
 * @brief       Reads a CAN FD Message from Rx FIFO.
 *
 * @param[in]   psCanfd     The pointer of the specified CANFD module.
 * @param[in]   u8FifoIdx   Number of the FIFO, 0 or 1.
 * @param[in]   psMsgBuf    Pointer to CANFD message frame structure for reception.
 *
 * @return      1           Read Message from Rx FIFO successfully.
 *              2           Rx FIFO is already overflowed and has been read successfully
 *              0           Rx FIFO is not enabled.
 *
 * @details     This function reads a CAN message from the CANFD build-in Rx FIFO.
 */
uint32_t CANFD_ReadRxFifoMsg(CANFD_T *psCanfd, uint8_t u8FifoIdx, CANFD_FD_MSG_T *psMsgBuf)
{
    CANFD_BUF_T *pRxBuffer;
    uint8_t GetIndex;
    uint32_t u32Success = 0;
    __I  uint32_t *pRXFS;
    __IO uint32_t *pRXFC, *pRXFA;
    uint8_t msgLostBit;
	uint32_t u32SramBaseAddr;

    /* check for valid FIFO number */
    if (u8FifoIdx < CANFD_NUM_RX_FIFOS)
    {
        if (u8FifoIdx == 0)
        {
            pRXFS = &(psCanfd->RXF0S);
            pRXFC = &(psCanfd->RXF0C);
            pRXFA = &(psCanfd->RXF0A);
            msgLostBit = 3;
        }
        else
        {
            pRXFS = &(psCanfd->RXF1S);
            pRXFC = &(psCanfd->RXF1C);
            pRXFA = &(psCanfd->RXF1A);
            msgLostBit = 7;
        }

        /* if FIFO is not empty */
        if ((*pRXFS & 0x7F) > 0)
        {
            GetIndex = (uint8_t)((*pRXFS >> 8) & 0x3F);
            //pRxBuffer = (CANFD_BUF_T *)(CANFD_SRAM_BASE_ADDR + (*pRXFC & 0xFFFF) + (GetIndex * sizeof(CANFD_BUF_T)));
			u32SramBaseAddr = _GetCanfdSramBaseAddr(psCanfd);
			pRxBuffer = (CANFD_BUF_T *)(u32SramBaseAddr + (*pRXFC & 0xFFFF) + (GetIndex * sizeof(CANFD_BUF_T)));

            CANFD_CopyRxFifoToMsgBuf(pRxBuffer, psMsgBuf);
            /* we got the message */
            *pRXFA = GetIndex;

            /* check for overflow */
            if (*pRXFS & CANFD_RXFS_RFL)
            {
                /* clear overflow flag */
                psCanfd->IR = (1UL << msgLostBit);
                u32Success = 2;
            }
            else
            {
                u32Success = 1;
            }
        }
    }

    return u32Success;
}


/**
 * @brief       Copies a message from a dedicated Rx buffer into a message buffer.
 *
 * @param[in]   psRxBuf         Buffer to read from.
 * @param[in]   psMsgBuf        Location to store read message.
 *
 * @return      None.
 *
 * @details     Copies a message from a dedicated Rx buffer into a message buffer.
 */
void CANFD_CopyDBufToMsgBuf(CANFD_BUF_T *psRxBuf, CANFD_FD_MSG_T *psMsgBuf)
{
    uint32_t u32Idx;

    if (psRxBuf->u32Id & RX_BUFFER_AND_FIFO_R0_ELEM_ESI_Msk)
        psMsgBuf->bErrStaInd = TRUE;
    else
        psMsgBuf->bErrStaInd = FALSE;

    /* if 29-bit ID */
    if (psRxBuf->u32Id & RX_BUFFER_AND_FIFO_R0_ELEM_XTD_Msk)
    {
        psMsgBuf->u32Id = (psRxBuf->u32Id & RX_BUFFER_AND_FIFO_R0_ELEM_ID_Msk);
        psMsgBuf->eIdType = eCANFD_XID;
    }
    /* if 11-bit ID */
    else
    {
        psMsgBuf->u32Id = (psRxBuf->u32Id  >> 18) & 0x7FF;
        psMsgBuf->eIdType = eCANFD_SID;
    }

    if (psRxBuf->u32Id  & RX_BUFFER_AND_FIFO_R0_ELEM_RTR_Msk)
        psMsgBuf->eFrmType = eCANFD_REMOTE_FRM;
    else
        psMsgBuf->eFrmType = eCANFD_DATA_FRM;


    if (psRxBuf->u32Config &  RX_BUFFER_AND_FIFO_R1_ELEM_FDF_Msk)
        psMsgBuf->bFDFormat = TRUE;
    else
        psMsgBuf->bFDFormat = FALSE;

    if (psRxBuf->u32Config &  RX_BUFFER_AND_FIFO_R1_ELEM_BSR_Msk)
        psMsgBuf->bBitRateSwitch = TRUE;
    else
        psMsgBuf->bBitRateSwitch = FALSE;

    psMsgBuf->u32DLC = CANFD_DecodeDLC((psRxBuf->u32Config & RX_BUFFER_AND_FIFO_R1_ELEM_DLC_Msk) >> RX_BUFFER_AND_FIFO_R1_ELEM_DLC_Pos);

    for (u32Idx = 0 ; u32Idx < psMsgBuf->u32DLC ; u32Idx++)
    {
        psMsgBuf->au8Data[u32Idx] = psRxBuf->au8Data[u32Idx];
    }
}


/**
 * @brief       Get Rx FIFO water level.
 *
 * @param[in]   psCanfd         The pointer to CANFD module base address.
 * @param[in]   u32RxFifoNum    0: RX FIFO_0, 1: RX_FIFO_1
 *
 * @return      Rx FIFO water level.
 *
 * @details     Get Rx FIFO water level.
 */
uint32_t CANFD_GetRxFifoWaterLvl(CANFD_T *psCanfd, uint32_t u32RxFifoNum)
{
    uint32_t u32WaterLevel = 0;

    if (u32RxFifoNum == 0)
        u32WaterLevel = ((psCanfd->RXF0C & CANFD_RXF0C_F0WM_Msk) >> CANFD_RXF0C_F0WM_Pos);
    else
        u32WaterLevel = ((psCanfd->RXF1C & CANFD_RXF1C_F1WM_Msk) >> CANFD_RXF1C_F1WM_Pos);

    return u32WaterLevel;
}


/**
 * @brief       Copies messages from FIFO into a message buffert.
 *
 * @param[in]   psRxBuf         Buffer to read from.
 * @param[in]   psMsgBuf        Location to store read message.
 *
 * @return      None.
 *
 * @details      Copies messages from FIFO into a message buffert.
 */
void CANFD_CopyRxFifoToMsgBuf(CANFD_BUF_T *psRxBuf, CANFD_FD_MSG_T *psMsgBuf)
{
    /*Copies a message from a dedicated Rx FIFO into a message buffer*/
    CANFD_CopyDBufToMsgBuf(psRxBuf, psMsgBuf);
}


/**
 * @brief       Cancel a Tx buffer transmission request.
 *
 * @param[in]   psCanfd         The pointer to CANFD module base address.
 * @param[in]   u32TxBufIdx     Tx buffer index number
 *
 * @return      None.
 *
 * @details     Cancel a Tx buffer transmission request.
 */
void CANFD_TxBufCancelReq(CANFD_T *psCanfd, uint32_t u32TxBufIdx)
{
    psCanfd->TXBCR |= (0x1ul << u32TxBufIdx);
}


/**
 * @brief       Checks if a Tx buffer cancellation request has been finished or not.
 *
 * @param[in]   psCanfd         The pointer to CAN FD module base address.
 * @param[in]   u32TxBufIdx     Tx buffer index number
 *
 * @return      0: cancellation finished.
 *              1: cancellation fail
 *
 * @details     Checks if a Tx buffer cancellation request has been finished or not.
 */
uint32_t CANFD_IsTxBufCancelFin(CANFD_T *psCanfd, uint32_t u32TxBufIdx)
{
    /* wait for completion */
    return ((psCanfd->TXBCR & (0x1ul << u32TxBufIdx)) >> u32TxBufIdx);
}


/**
 * @brief       Checks if a Tx buffer transmission has occurred or not.
 *
 * @param[in]   psCanfd         The pointer to CAN FD module base address.
 * @param[in]   u32TxBufIdx     Tx buffer index number
 *
 * @return     0: No transmission occurred.
 *             1: Transmission occurred
 *
 * @details     Checks if a Tx buffer transmission has occurred or not.
 */
uint32_t CANFD_IsTxBufTransmitOccur(CANFD_T *psCanfd, uint32_t u32TxBufIdx)
{
    return ((psCanfd->TXBTO & (0x1ul << u32TxBufIdx)) >> u32TxBufIdx);
}


/**
 * @brief       Init Tx event fifo
 *
 * @param[in]   psCanfd          The pointer to CAN FD module base address.
 * @param[in]   psRamConfig      Tx Event Fifo configuration ram address.
 * @param[in]   psElemSize       Tx Event Fifo configuration element size
 * @param[in]   u32FifoWaterLvl  FIFO water level
 *
 * @return      None.
 *
 * @details     Init Tx event fifo.
 */
void CANFD_InitTxEvntFifo(CANFD_T *psCanfd, CANFD_RAM_PART_T *psRamConfig, CANFD_ELEM_SIZE_T *psElemSize, uint32_t u32FifoWaterLvl)
{
    /* Set TX Event FIFO element size,watermark,start address. */
    psCanfd->TXEFC = (u32FifoWaterLvl << CANFD_TXEFC_EFWN_Pos) | (psElemSize->u32TxEventFifo << CANFD_TXEFC_EFS_Pos)
                     | (psRamConfig->u32TXEFC_EFSA & CANFD_TXEFC_EFSA_Msk);
}


/**
 * @brief       Get Tx event fifo water level
 *
 * @param[in]   psCanfd       The pointer to CANFD module base address.
 *
 * @return      Tx event fifo water level.
 *
 * @details     Get Tx event fifo water level.
 */
uint32_t CANFD_GetTxEvntFifoWaterLvl(CANFD_T *psCanfd)
{
    return ((psCanfd->TXEFC & CANFD_TXEFC_EFWN_Msk) >> CANFD_TXEFC_EFWN_Pos);
}


/**
 * @brief        Copy Event Elements from TX Event FIFO to user buffer
 *
 * @param[in]   psCanfd          The pointer to CAN FD module base address.
 * @param[in]   u32TxEvntNum     Tx Event FIFO number
 * @param[in]   psTxEvntElem     Tx Event Message struct
 *
 * @return      None.
 *
 * @details     Copy all Event Elements from TX Event FIFO to the Software Event List .
 */
void CANFD_CopyTxEvntFifoToUsrBuf(CANFD_T *psCanfd, uint32_t u32TxEvntNum, CANFD_TX_EVNT_ELEM_T *psTxEvntElem)
{
    uint32_t *pu32TxEvnt;
    /*Get the Tx Event FIFO Address*/
    pu32TxEvnt = (uint32_t *)CANFD_GetTxBufferElementAddress(psCanfd, u32TxEvntNum);

    /*Get the Error State Indicator*/
    if ((pu32TxEvnt[0] & TX_FIFO_E0_EVENT_ESI_Msk) > 0)
        psTxEvntElem->bErrStaInd = TRUE; //Transmitting node is error passive
    else
        psTxEvntElem->bErrStaInd = FALSE;//Transmitting node is error active

    /*Get the Tx FIFO Identifier type and Identifier*/

    if ((pu32TxEvnt[0] & TX_FIFO_E0_EVENT_XTD_Msk) > 0)
    {
        psTxEvntElem-> eIdType = eCANFD_XID;
        psTxEvntElem->u32Id = (pu32TxEvnt[0] & TX_FIFO_E0_EVENT_ID_Msk);// Extended ID
    }
    else
    {
        psTxEvntElem-> eIdType = eCANFD_SID;
        psTxEvntElem->u32Id = (pu32TxEvnt[0] & TX_FIFO_E0_EVENT_ID_Msk) >> 18;// Standard ID
    }

    /*Get the Frame type*/
    if ((pu32TxEvnt[0] & TX_FIFO_E0_EVENT_RTR_Msk) > 0)
        psTxEvntElem->bRemote = TRUE; //Remote frame
    else
        psTxEvntElem->bRemote = FALSE; //Data frame

    /*Get the FD Format type*/
    if ((pu32TxEvnt[0] & TX_FIFO_E1_EVENT_FDF_Msk) > 0)
        psTxEvntElem->bFDFormat = TRUE; //CAN FD frame format
    else
        psTxEvntElem->bFDFormat = FALSE; //Classical CAN frame format

    /*Get the Bit Rate Switch type*/
    if ((pu32TxEvnt[0] & TX_FIFO_E1_EVENT_BRS_Msk) > 0)
        psTxEvntElem->bBitRateSwitch = TRUE; //Frame transmitted with bit rate switching
    else
        psTxEvntElem->bBitRateSwitch = FALSE; //Frame transmitted without bit rate switching

    /*Get the Tx FIFO Data Length  */
    psTxEvntElem->u32DLC = CANFD_DecodeDLC((uint8_t)((pu32TxEvnt[1] & TX_FIFO_E1_EVENT_DLC_Msk) >> TX_FIFO_E1_EVENT_DLC_Pos));

    /*Get the Tx FIFO Timestamp  */
    psTxEvntElem->u32TxTs = (((pu32TxEvnt[1] & TX_FIFO_E1A_EVENT_TXTS_Msk) >> TX_FIFO_E1A_EVENT_TXTS_Pos));
    /*Get the Tx FIFO Message marker  */
    psTxEvntElem->u32MsgMarker = (((pu32TxEvnt[1] & TX_FIFO_E1_EVENT_MM_Msk) >> TX_FIFO_E1_EVENT_MM_Pos));
}


/**
 * @brief       Get CAN FD interrupts status.
 *
 * @param[in]   psCanfd         The pointer of the specified CAN FD module.
 * @param[in]   u32IntTypeFlag  Interrupt Type Flag, should be
 *                              - \ref CANFD_IR_ARA_Msk     : Access to Reserved Address interrupt Indicator
 *                              - \ref CANFD_IR_PED_Msk     : Protocol Error in Data Phase interrupt Indicator
 *                              - \ref CANFD_IR_PEA_Msk     : Protocol Error in Arbitration Phase interrupt Indicator
 *                              - \ref CANFD_IR_WDI_Msk     : Watchdog interrupt Indicator
 *                              - \ref CANFD_IR_BO_Msk      : Bus_Off Status interrupt Indicator
 *                              - \ref CANFD_IR_EW_Msk      : Warning Status interrupt Indicator
 *                              - \ref CANFD_IR_EP_Msk      : Error Passive interrupt Indicator
 *                              - \ref CANFD_IR_ELO_Msk     : Error Logging Overflow interrupt Indicator
 *                              - \ref CANFD_IR_DRX_Msk     : Message stored to Dedicated Rx Buffer interrupt Indicator
 *                              - \ref CANFD_IR_TOO_Msk     : Timeout Occurred interrupt Indicator
 *                              - \ref CANFD_IR_MRAF_Msk    : Message RAM Access Failure interrupt Indicator
 *                              - \ref CANFD_IR_TSW_Msk     : Timestamp Wraparound interrupt Indicator
 *                              - \ref CANFD_IR_TEFL_Msk    : Tx Event FIFO Event Lost interrupt Indicator
 *                              - \ref CANFD_IR_TEFF_Msk    : Tx Event FIFO Full Indicator
 *                              - \ref CANFD_IR_TEFW_Msk    : Tx Event FIFO Watermark Reached Interrupt Indicator
 *                              - \ref CANFD_IR_TEFN_Msk    : Tx Event FIFO New Entry Interrupt Indicator
 *                              - \ref CANFD_IR_TFE_Msk     : Tx FIFO Empty Interrupt Indicator
 *                              - \ref CANFD_IR_TCF_Msk     : Transmission Cancellation Finished Interrupt Indicator
 *                              - \ref CANFD_IR_TC_Msk      : Transmission Completed interrupt Indicator
 *                              - \ref CANFD_IR_HPM_Msk     : High Priority Message Interrupt Indicator
 *                              - \ref CANFD_IR_RF1L_Msk    : Rx FIFO 1 Message Lost Interrupt Indicator
 *                              - \ref CANFD_IR_RF1F_Msk    : Rx FIFO 1 Full Interrupt Indicator
 *                              - \ref CANFD_IR_RF1W_Msk    : Rx FIFO 1 Watermark Reached Interrupt Indicator
 *                              - \ref CANFD_IR_RF1N_Msk    : Rx FIFO 1 New Message Interrupt Indicator
 *                              - \ref CANFD_IR_RF0L_Msk    : Rx FIFO 0 Message Lost Interrupt Indicator
 *                              - \ref CANFD_IR_RF0F_Msk    : Rx FIFO 0 Full Interrupt Indicator
 *                              - \ref CANFD_IR_RF0W_Msk    : Rx FIFO 0 Watermark Reached Interrupt Indicator
 *                              - \ref CANFD_IR_RF0N_Msk    : Rx FIFO 0 New Message Interrupt Indicator
 *
 * @return      None.
 *
 * @details     This function gets all CAN FD interrupt status flags.
 */
uint32_t CANFD_GetStatusFlag(CANFD_T *psCanfd, uint32_t u32IntTypeFlag)
{
    return (psCanfd->IR & u32IntTypeFlag);
}


/**
 * @brief       Clears the CAN FD module interrupt flags
 *
 * @param[in]   psCanfd           The pointer of the specified CANFD module.
 * @param[in]   u32InterruptFlag  The specified interrupt of CAN FD module
 *                               - \ref CANFD_IR_ARA_Msk     : Access to Reserved Address interrupt Indicator
 *                               - \ref CANFD_IR_PED_Msk     : Protocol Error in Data Phase interrupt Indicator
 *                               - \ref CANFD_IR_PEA_Msk     : Protocol Error in Arbitration Phase interrupt Indicator
 *                               - \ref CANFD_IR_WDI_Msk     : Watchdog interrupt Indicator
 *                               - \ref CANFD_IR_BO_Msk      : Bus_Off Status interrupt Indicator
 *                               - \ref CANFD_IR_EW_Msk      : Warning Status interrupt Indicator
 *                               - \ref CANFD_IR_EP_Msk      : Error Passive interrupt Indicator
 *                               - \ref CANFD_IR_ELO_Msk     : Error Logging Overflow interrupt Indicator
 *                               - \ref CANFD_IR_DRX_Msk     : Message stored to Dedicated Rx Buffer interrupt Indicator
 *                               - \ref CANFD_IR_TOO_Msk     : Timeout Occurred interrupt Indicator
 *                               - \ref CANFD_IR_MRAF_Msk    : Message RAM Access Failure interrupt Indicator
 *                               - \ref CANFD_IR_TSW_Msk     : Timestamp Wraparound interrupt Indicator
 *                               - \ref CANFD_IR_TEFL_Msk    : Tx Event FIFO Event Lost interrupt Indicator
 *                               - \ref CANFD_IR_TEFF_Msk    : Tx Event FIFO Full Indicator
 *                               - \ref CANFD_IR_TEFW_Msk    : Tx Event FIFO Watermark Reached Interrupt Indicator
 *                               - \ref CANFD_IR_TEFN_Msk    : Tx Event FIFO New Entry Interrupt Indicator
 *                               - \ref CANFD_IR_TFE_Msk     : Tx FIFO Empty Interrupt Indicator
 *                               - \ref CANFD_IR_TCF_Msk     : Transmission Cancellation Finished Interrupt Indicator
 *                               - \ref CANFD_IR_TC_Msk      : Transmission Completed interrupt Indicator
 *                               - \ref CANFD_IR_HPM_Msk     : High Priority Message Interrupt Indicator
 *                               - \ref CANFD_IR_RF1L_Msk    : Rx FIFO 1 Message Lost Interrupt Indicator
 *                               - \ref CANFD_IR_RF1F_Msk    : Rx FIFO 1 Full Interrupt Indicator
 *                               - \ref CANFD_IR_RF1W_Msk    : Rx FIFO 1 Watermark Reached Interrupt Indicator
 *                               - \ref CANFD_IR_RF1N_Msk    : Rx FIFO 1 New Message Interrupt Indicator
 *                               - \ref CANFD_IR_RF0L_Msk    : Rx FIFO 0 Message Lost Interrupt Indicator
 *                               - \ref CANFD_IR_RF0F_Msk    : Rx FIFO 0 Full Interrupt Indicator
 *                               - \ref CANFD_IR_RF0W_Msk    : Rx FIFO 0 Watermark Reached Interrupt Indicator
 *                               - \ref CANFD_IR_RF0N_Msk    : Rx FIFO 0 New Message Interrupt Indicator
 *
 * @return      None.
 *
 * @details     This function clears CAN FD interrupt status flags.
 */
void CANFD_ClearStatusFlag(CANFD_T *psCanfd, uint32_t u32InterruptFlag)
{
    /* Write 1 to clear status flag. */
    psCanfd->IR |= u32InterruptFlag;
}


/**
 * @brief       Gets the CAN FD Bus Error Counter value.
 *
 * @param[in]   psCanfd        The pointer of the specified CAN FD module.
 * @param[in]   pu8TxErrBuf    TxErrBuf Buffer to store Tx Error Counter value.
 * @param[in]   pu8RxErrBuf    RxErrBuf Buffer to store Rx Error Counter value.
 *
 * @return      None.
 *
 * @details     This function gets the CAN FD Bus Error Counter value for both Tx and Rx direction.
 *              These values may be needed in the upper layer error handling.
 */
void CANFD_GetBusErrCount(CANFD_T *psCanfd, uint8_t *pu8TxErrBuf, uint8_t *pu8RxErrBuf)
{
    if (pu8TxErrBuf)
    {
        *pu8TxErrBuf = (uint8_t)((psCanfd->ECR >> CANFD_ECR_TEC_Pos) & CANFD_ECR_TEC_Msk);
    }

    if (pu8RxErrBuf)
    {
        *pu8RxErrBuf = (uint8_t)((psCanfd->ECR >> CANFD_ECR_REC_Pos) & CANFD_ECR_REC_Msk);
    }
}


/**
 * @brief       CAN FD Run to the Normal Operation.
 *
 * @param[in]   psCanfd        The pointer of the specified CAN FD module.
 * @param[in]   u8Enable       TxErrBuf Buffer to store Tx Error Counter value.
 *
 * @return      None.
 *
 * @details     This function gets the CAN FD Bus Error Counter value for both Tx and Rx direction.
 *              These values may be needed in the upper layer error handling.
 *
 * @note       This function sets g_CANFD_i32ErrCode to CANFD_TIMEOUT_ERR if waiting CANFD time-out.
 */
void CANFD_RunToNormal(CANFD_T *psCanfd, uint8_t u8Enable)
{
    uint32_t u32TimeOutCount = CANFD_TIMEOUT;

    g_CANFD_i32ErrCode = 0;

    if (u8Enable)
    {
        /* start operation */
        psCanfd->CCCR &= ~(CANFD_CCCR_CCE_Msk | CANFD_CCCR_INIT_Msk);

        while (psCanfd->CCCR & CANFD_CCCR_INIT_Msk)
        {
            if(u32TimeOutCount-- == 0)
            {
                g_CANFD_i32ErrCode = CANFD_TIMEOUT_ERR;
                break;
            }
        }
    }
    else
    {
        /* init mode */
        psCanfd->CCCR |= CANFD_CCCR_INIT_Msk;

        while (!(psCanfd->CCCR & CANFD_CCCR_INIT_Msk))
        {
            if(u32TimeOutCount-- == 0)
            {
                g_CANFD_i32ErrCode = CANFD_TIMEOUT_ERR;
                break;
            }
        }
    }
}



/*@}*/ /* end of group CANFD_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group CANFD_Driver */

/*@}*/ /* end of group Standard_Driver */
