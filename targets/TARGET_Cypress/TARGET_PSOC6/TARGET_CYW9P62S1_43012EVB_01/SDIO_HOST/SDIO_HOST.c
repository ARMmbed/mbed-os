/***************************************************************************//**
* \file SDIO_HOST.c
*
* \brief
*  This file provides the source code to the API for the UDB based SDIO driver.
*
********************************************************************************
* \copyright
* Copyright 2016-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
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
*******************************************************************************/

#include "SDIO_HOST.h"
#include "cy_utils.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef CY_RTOS_AWARE

    #include "cyabs_rtos.h"

    #define NEVER_TIMEOUT ( (uint32_t)0xffffffffUL )
    static cy_semaphore_t sdio_transfer_finished_semaphore;
    static bool sema_initialized = false;
#endif

/* Backup struct used to store and restore non retention UDB registers */
typedef struct
{
    uint32_t CY_SDIO_UDB_WRKMULT_CTL_0;
    uint32_t CY_SDIO_UDB_WRKMULT_CTL_1;
    uint32_t CY_SDIO_UDB_WRKMULT_CTL_2;
    uint32_t CY_SDIO_UDB_WRKMULT_CTL_3;
} stc_sdio_backup_regs_t;

/*Globals Needed for DMA */
/*DMA channel structures*/
cy_stc_dma_channel_config_t respChannelConfig;
cy_stc_dma_channel_config_t cmdChannelConfig;
cy_stc_dma_channel_config_t writeChannelConfig;
cy_stc_dma_channel_config_t readChannelConfig;

/*DMA Descriptor structures*/
cy_stc_dma_descriptor_t respDesr;
cy_stc_dma_descriptor_t cmdDesr;
cy_stc_dma_descriptor_t readDesr0;
cy_stc_dma_descriptor_t readDesr1;
cy_stc_dma_descriptor_t writeDesr0;
cy_stc_dma_descriptor_t writeDesr1;

/*Global structure used for data keeping*/
stc_sdio_gInternalData_t gstcInternalData;

/*Global CRC table*/
static uint8_t crcTable[256];

/*Global values used for DMA interrupt*/
static uint32_t yCountRemainder;
static uint32_t yCounts;

/* Global value for card interrupt */
static uint8_t pfnCardInt_count = 0;

/*Global structure to store UDB registers */
static stc_sdio_backup_regs_t regs;

static uint32_t udb_initialized = 0;

cy_stc_syspm_callback_params_t      sdio_pm_callback_params;
cy_stc_syspm_callback_t             sdio_pm_callback_handler;

/* Deep Sleep Mode API Support */
static void SDIO_SaveConfig(void);
static void SDIO_RestoreConfig(void);

/*******************************************************************************
* Function Name: SDIO_DeepSleepCallback
****************************************************************************//**
*
* Callback executed during Deep Sleep entry/exit
*
* \param params
* Pointer to structure that holds callback parameters for this driver.
*
* \param mode
* The state transition mode that is currently happening.
*
* \note
* Saves/Restores SDIO UDB registers
*
* \return
* CY_SYSPM_SUCCESS if the transition was successful, otherwise CY_SYSPM_FAIL
*
*******************************************************************************/
cy_en_syspm_status_t SDIO_DeepSleepCallback(cy_stc_syspm_callback_params_t *params, cy_en_syspm_callback_mode_t mode)
{
    CY_UNUSED_PARAMETER(params);
    cy_en_syspm_status_t status = CY_SYSPM_FAIL;

    switch (mode)
    {
        case CY_SYSPM_CHECK_READY:
        case CY_SYSPM_CHECK_FAIL:
            status = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_BEFORE_TRANSITION:
            SDIO_SaveConfig();
            status = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_AFTER_TRANSITION:
            SDIO_RestoreConfig();
            status = CY_SYSPM_SUCCESS;
            break;

        default:
            break;
    }

    return status;
}

/*******************************************************************************
* Function Name: SDIO_Init
****************************************************************************//**
*
* Initializes the SDIO hardware
*
* \param pfuCb
* Pointer to structure that holds pointers to callback function
* see \ref stc_sdio_irq_cb_t.
*
* \note
* Sets SD Clock Frequency to 400 kHz
*******************************************************************************/
void SDIO_Init(stc_sdio_irq_cb_t* pfuCb)
{
    if ( !udb_initialized )
    {
        udb_initialized = 1;
        SDIO_Host_Config_TriggerMuxes();
        SDIO_Host_Config_UDBs();
    }

    /*Set Number of Blocks to 1 initially, this will be updated later*/
    SDIO_SetNumBlocks(1);

    /*Enable SDIO ISR*/
    NVIC_EnableIRQ((IRQn_Type) SDIO_HOST_sdio_int__INTC_NUMBER);

    /*Enable the Status Reg to generate an interrupt*/
    SDIO_STATUS_AUX_CTL |= (0x10);

    /*Set the priority of DW0, DW1, M4 and M0. DW1 should have highest*/
    /*First clear priority of all*/
    (* (reg32 *)CYREG_PROT_SMPU_MS0_CTL) &= ~0x0300;
    (* (reg32 *)CYREG_PROT_SMPU_MS2_CTL) &= ~0x0300;
    (* (reg32 *)CYREG_PROT_SMPU_MS3_CTL) &= ~0x0300;
    (* (reg32 *)CYREG_PROT_SMPU_MS14_CTL) &= ~0x0300;

    /*Next set priority DW1 =  0, DW0 = 1, M4 = 2, M0 =3*/
    (* (reg32 *)CYREG_PROT_SMPU_MS2_CTL) |= 0x0100;
    (* (reg32 *)CYREG_PROT_SMPU_MS0_CTL) |= 0x0200;
    (* (reg32 *)CYREG_PROT_SMPU_MS14_CTL) |= 0x0200;

    /*Setup callback for card interrupt*/
    gstcInternalData.pstcCallBacks.pfnCardIntCb = pfuCb->pfnCardIntCb;

    /*Setup the DMA channels*/
    SDIO_SetupDMA();

    /*Initialize CRC*/
    SDIO_Crc7Init();

    /*Enable all the bit counters*/
    SDIO_CMD_BIT_CNT_CONTROL_REG    |=  SDIO_ENABLE_CNT;
    SDIO_WRITE_CRC_CNT_CONTROL_REG  |=  SDIO_ENABLE_CNT;
    SDIO_CRC_BIT_CNT_CONTROL_REG    |=  SDIO_ENABLE_CNT;
    SDIO_BYTE_CNT_CONTROL_REG       |=  SDIO_ENABLE_CNT;

    /*Set block byte count to 64, this will be changed later */
    SDIO_SetBlockSize(64);

    /*Set the read and write FIFOs to use the half full status*/
    (*(reg32 *) SDIO_HOST_bSDIO_Write_DP__DP_AUX_CTL_REG) |= 0x0c;
    (*(reg32 *) SDIO_HOST_bSDIO_Read_DP__DP_AUX_CTL_REG)  |= 0x0c;

    /*Set clock to 400k, and enable it*/
    SDIO_SetSdClkFrequency(400000);
    SDIO_EnableIntClock();
    SDIO_EnableSdClk();
}


/*******************************************************************************
* Function Name: SDIO_SendCommand
****************************************************************************//**
*
* Send an SDIO command, don't wait for it to finish.
*
* \param pstcCmdConfig
* Command configuration structure. See \ref stc_sdio_cmd_config_t.
*
*******************************************************************************/
void SDIO_SendCommand(stc_sdio_cmd_config_t *pstcCmdConfig)
{
    /*buffer to hold command data*/
    static uint8_t u8cmdBuf[6];

    /*Populate buffer*/
    /*Element 0 is the Most Significant Byte*/
    u8cmdBuf[0] = SDIO_HOST_DIR | pstcCmdConfig->u8CmdIndex;
    u8cmdBuf[1] = (uint8_t)((pstcCmdConfig->u32Argument & 0xff000000)>>24);
    u8cmdBuf[2] = (uint8_t)((pstcCmdConfig->u32Argument & 0x00ff0000)>>16);
    u8cmdBuf[3] = (uint8_t)((pstcCmdConfig->u32Argument & 0x0000ff00)>>8);
    u8cmdBuf[4] = (uint8_t)((pstcCmdConfig->u32Argument & 0x000000ff));

    /*calculate the CRC of above data*/
    u8cmdBuf[5] = SDIO_CalculateCrc7(u8cmdBuf, 5);
    /*Shift it up by 1 as the CRC takes the upper 7 bits of the last byte of the cmd*/
    u8cmdBuf[5] = u8cmdBuf[5] << 1;
    /*Add on the end bit*/
    u8cmdBuf[5] = u8cmdBuf[5] | SDIO_CMD_END_BIT;

    /*Load the first byte into A0*/
    SDIO_CMD_COMMAND_A0_REG = u8cmdBuf[0];

    /*If a response is expected setup DMA to receive the response*/
    if (pstcCmdConfig->bResponseRequired == true)
    {
        /*Clear the flag in hardware that says skip response*/
        SDIO_CONTROL_REG &= ~SDIO_CTRL_SKIP_RESPONSE;

        /*Set the destination address*/
        respDesr.dst = (uint32_t)(pstcCmdConfig->pu8ResponseBuf);

        /*Initialize the channel with the descriptor*/
        Cy_DMA_Channel_SetDescriptor(SDIO_HOST_Resp_DMA_HW, SDIO_HOST_Resp_DMA_DW_CHANNEL, &respDesr);

        /*Enable the channel*/
        Cy_DMA_Channel_Enable(SDIO_HOST_Resp_DMA_HW, SDIO_HOST_Resp_DMA_DW_CHANNEL);
    }
    else
    {
        /*Set the skip flag*/
        SDIO_CONTROL_REG |= SDIO_CTRL_SKIP_RESPONSE;
    }

    /*Setup the Command DMA*/
    /*Set the source address*/
    cmdDesr.src = (uint32_t)(&u8cmdBuf[1]);

    /*Initialize the channel with the descriptor*/
    Cy_DMA_Channel_SetDescriptor(SDIO_HOST_CMD_DMA_HW, SDIO_HOST_CMD_DMA_DW_CHANNEL , &cmdDesr);

    /*Enable the channel*/
    Cy_DMA_Channel_Enable(SDIO_HOST_CMD_DMA_HW, SDIO_HOST_CMD_DMA_DW_CHANNEL );
}


/*******************************************************************************
* Function Name: SDIO_GetResponse
****************************************************************************//**
*
* Takes a 6 byte response buffer, and extracts the 32 bit response, also checks
* for index errors, CRC errors, and end bit errors.
*
* \param bCmdIndexCheck
* If True check for index errors
*
* \param bCmdCrcCheck
* If True check for CRC errors
*
* \param u8cmdIdx
* Command index, used for checking the index error
*
* \param pu32Response
* location to store 32 bit response
*
* \param pu8ResponseBuf
* buffer that holds the 6 bytes of response data
*
* \return
* \ref en_sdio_result_t
*
*******************************************************************************/
en_sdio_result_t SDIO_GetResponse(uint8_t bCmdIndexCheck, uint8_t bCmdCrcCheck, uint8_t u8cmdIdx, uint32_t* pu32Response, uint8_t *pu8ResponseBuf)
{
    /*Function return*/
    en_sdio_result_t  enRet = Error;
    /*variable to hold temporary CRC*/
    uint8_t u8TmpCrc;
    /*temporary response*/
    uint32_t u32TmpResponse;

    /*Zero out the pu32Response*/
    *pu32Response = 0;

    /*Check if the CRC needs to be checked*/
    if (bCmdCrcCheck)
    {
        /*Calculate the CRC*/
        u8TmpCrc = SDIO_CalculateCrc7(pu8ResponseBuf, 5);

        /*Shift calculated CRC up by one bit to match bit position of CRC*/
        u8TmpCrc = u8TmpCrc << 1;

        /*Compare calculated CRC with received CRC*/
        if ((u8TmpCrc & 0xfe) != (pu8ResponseBuf[5] & 0xfe))
        {
            enRet |= CommandCrcError;
        }
    }

    /*Check if the index needs to be checked*/
    if (bCmdIndexCheck)
    {
        /*The index resides in the lower 6 bits of the 1st byte of the response*/
        if ((u8cmdIdx != (pu8ResponseBuf[0] & 0x3f)))
        {
            enRet |= CommandIdxError;
        }
    }

    /*Check the end bit*/
    if (!(pu8ResponseBuf[5] & 0x01))
    {
        enRet |= CommandEndError;
    }

    if (enRet == Error)
    {
        /*If we get here then there were no errors with the command populate the response*/
        u32TmpResponse = pu8ResponseBuf[1];
        u32TmpResponse = u32TmpResponse << 8;
        u32TmpResponse |= pu8ResponseBuf[2];
        u32TmpResponse = u32TmpResponse << 8;
        u32TmpResponse |= pu8ResponseBuf[3];
        u32TmpResponse = u32TmpResponse << 8;
        u32TmpResponse |= pu8ResponseBuf[4];

        *pu32Response = u32TmpResponse;

        enRet = Ok;
    }

    return enRet;
}


/*******************************************************************************
* Function Name: SDIO_InitDataTransfer
****************************************************************************//**
*
* Configure the data channel for a data transfer. For a write this doesn't start
* the write, that must be done separately after the response is received.
*
* \param pstcDataConfig
* Data configuration structure. See \ref stc_sdio_data_config_t
*
*
*******************************************************************************/
void SDIO_InitDataTransfer(stc_sdio_data_config_t *pstcDataConfig)
{
    /*hold size of entire transfer*/
    uint32_t dataSize;

    /*calculate how many bytes are going to be sent*/
    dataSize = pstcDataConfig->u16BlockSize * pstcDataConfig->u16BlockCount;

    /*Set the block size and number of blocks*/
    SDIO_SetBlockSize(pstcDataConfig->u16BlockSize);
    SDIO_SetNumBlocks((pstcDataConfig->u16BlockCount) - 1);

    /*If we are reading data setup the DMA to receive read data*/
    if (pstcDataConfig->bRead == true)
    {
        /*First disable the write channel*/
        Cy_DMA_Channel_Disable(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL );

        /*Clear any pending interrupts in the DMA*/
        Cy_DMA_Channel_ClearInterrupt(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL);

        NVIC_ClearPendingIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);

        /*setup the destination addresses*/
        readDesr0.dst = (uint32_t)(pstcDataConfig->pu8Data);
        readDesr1.dst = (uint32_t)((pstcDataConfig->pu8Data) + 1024);

        /*Setup the X control to transfer two 16 bit elements per transfer for a total of 4 bytes
          Remember X increment is in terms of data element size which is 16, thus why it is 1*/
        readDesr0.xCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 1) |
                                _VAL2FLD(CY_DMA_CTL_DST_INCR, 1);
        readDesr1.xCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 1) |
                                _VAL2FLD(CY_DMA_CTL_DST_INCR, 1);

        /*The X Loop will always transfer 4 bytes. The FIFO will only trigger the
        DMA when it has 4 bytes to send (2 in each F0 and F1). There is a possibility
        that there could be 3,2,or 1 bytes still in the FIFOs. To solve this the DMA
        will be SW triggered when hardware indicates all bytes have been received.
        This leads to an extra 1, 2 or 3 bytes being received. So the RX buffer needs to
        be at least 3 bytes bigger than the data size.

        Since the X loop is setup to 4, the maximum number of Y loop is 256 so one
        descriptor can transfer 1024 bytes. Two descriptors can transfer 2048 bytes.
        Since we don't know the maximum number of bytes to read only two descriptors will
        be used. If more than 2048 bytes need to be read then and interrupt will be enabled
        The descriptor that is not currently running will be updated in the ISR to receive
        more data.

        So there are three conditions to check:
        1) Are we sending less than or equal to 1024 bytes if so use one descriptor
        2) Are we sending greater than 1024, but less than or equal to 2048, use two descriptors
        3) Greater than 2048, use two descriptors and the ISR
        */

        if (dataSize <= 1024)
        {
            /*Setup one descriptor*/
            /*Y Increment is 2 because the X is transfer 2 data elements (which are 16 bits)*/
            readDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (dataSize - 1) / 4) |
                                    _VAL2FLD(CY_DMA_CTL_DST_INCR, 2);

            /*Setup descriptor 0 to point to nothing and disable*/
            readDesr0.nextPtr = 0;
            readDesr0.ctl |= 0x01000000;

            /*Disable Interrupt*/
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);
        }
        else if (dataSize <=2048)
        {
            /*setup the first descriptor for 1024, then setup 2nd descriptor for remainder*/

            readDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 255) |
                                    _VAL2FLD(CY_DMA_CTL_DST_INCR, 2);
            readDesr1.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (dataSize - 1025) / 4) |
                                    _VAL2FLD(CY_DMA_CTL_DST_INCR, 2);


            /*Setup descriptor 0 to point to descriptor 1*/
            readDesr0.nextPtr = (uint32_t)(&readDesr1);
            /*Setup descriptor 1 to point to nothing and disable */
            readDesr1.nextPtr = 0;

            /*Don't disable after first descriptor*/
            readDesr0.ctl &= ~0x01000000;
            /*Disable after second descriptor*/
            readDesr1.ctl |= 0x01000000;

            /*Disable Interrupt*/
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);
        }
        else /*dataSize must be greater than 2048*/
        {
            /*These are for the ISR, Need to figure out how many "descriptors"
            need to run, and the yCount for last descriptor.
            Example: dataSize = 2080
            yCounts = 2, yCountRemainder = 7 (send 8 more set of 4)*/
            yCounts = (dataSize / 1024);

            /*the Ycount register is a +1 register meaning 0 = 1. I However, need to know when there is
            no remainder so I increase the value to make sure there is a remainder and decrement in the ISR*/
            yCountRemainder = (((dataSize - (yCounts * 1024)) + 3 ) / 4);

            /*Setup the Y Ctrl for both descriptors*/
            readDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 255) |
                                    _VAL2FLD(CY_DMA_CTL_DST_INCR, 2);
            readDesr1.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 255) |
                                    _VAL2FLD(CY_DMA_CTL_DST_INCR, 2);

            /*Setup descriptor 0 to point to descriptor 1*/
            readDesr0.nextPtr = (uint32_t)(&readDesr1);
            /*Setup descriptor 1 to point to descriptor 0*/
            readDesr1.nextPtr = (uint32_t)(&readDesr0);

            /*Don't disable the channel on completion of descriptor*/
            readDesr0.ctl &= ~0x01000000;
            readDesr1.ctl &= ~0x01000000;

            /*Decrement yCounts by 2 since we already have 2 descriptors setup*/
            yCounts -= 2;

            /*Enable DMA interrupt*/
            NVIC_EnableIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);
        }

        /*Initialize the channel with the first descriptor*/
        Cy_DMA_Channel_SetDescriptor(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL , &readDesr0);

        /*Enable the channel*/
        Cy_DMA_Channel_Enable(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL );

        /*Set the flag in the control register to enable the read*/
        SDIO_CONTROL_REG |= SDIO_CTRL_ENABLE_READ;
    }

    /*Otherwise it is a write*/
    else
    {
        /*First disable the Read channel*/
        Cy_DMA_Channel_Disable(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL );

        /*Clear any pending interrupts in the DMA*/
        Cy_DMA_Channel_ClearInterrupt(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL);

        NVIC_ClearPendingIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);

        /*setup the SRC addresses*/
        writeDesr0.src = (uint32_t)(pstcDataConfig->pu8Data);
        writeDesr1.src = (uint32_t)((pstcDataConfig->pu8Data) + 1024);


        /*Setup the X control to transfer two 16 bit elements per transfer for a total of 4 bytes
          Remember X increment is in terms of data element size which is 16, thus why it is 1*/
        writeDesr0.xCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 1) |
                                _VAL2FLD(CY_DMA_CTL_SRC_INCR, 1);
        writeDesr1.xCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 1) |
                                _VAL2FLD(CY_DMA_CTL_SRC_INCR, 1);

        if (dataSize <= 1024)
        {
            /*Setup one descriptor*/
            /*Y Increment is 2 because the X is transfer 2 data elements (which are 16 bits)*/
            writeDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (dataSize - 1) / 4) |
                                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, 2);

            /*Setup descriptor 0 to point to nothing and disable*/
            writeDesr0.nextPtr = 0;
            writeDesr0.ctl |= 0x01000000;

            /*Disable Interrupt*/
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);
        }
        else if (dataSize <=2048)
        {
            /*setup the first descriptor for 1024, then setup 2nd descriptor for remainder*/

            writeDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 255) |
                                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, 2);
            writeDesr1.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (dataSize - 1025) / 4) |
                                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, 2);


            /*Setup descriptor 0 to point to descriptor 1*/
            writeDesr0.nextPtr = (uint32_t)(&writeDesr1);
            /*Setup descriptor 1 to point to nothing and disable */
            writeDesr1.nextPtr = 0;

            /*Don't disable after first descriptor*/
            writeDesr0.ctl &= ~0x01000000;
            /*Disable after second descriptor*/
            writeDesr1.ctl |= 0x01000000;

            /*Disable Interrupt*/
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);
        }
        else /*dataSize must be greater than 2048*/
        {
            /*These are for the ISR, Need to figure out how many "descriptors"
            need to run, and the yCount for last descriptor.
            Example: dataSize = 2080
            yCounts = 2, yCountRemainder = 7 (send 8 more set of 4)*/
            yCounts = (dataSize / 1024);

            /*the Ycount register is a +1 register meaning 0 = 1. I However, need to know when there is
            no remainder so I increase the value to make sure there is a remainder and decrement in the ISR*/
            yCountRemainder = (((dataSize - (yCounts * 1024)) + 3 ) / 4);

            /*Setup the Y Ctrl for both descriptors*/
            writeDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 255) |
                                               _VAL2FLD(CY_DMA_CTL_SRC_INCR, 2);
            writeDesr1.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, 255)  |
                                               _VAL2FLD(CY_DMA_CTL_SRC_INCR, 2);

            /*Setup descriptor 0 to point to descriptor 1*/
            writeDesr0.nextPtr = (uint32_t)(&writeDesr1);
            /*Setup descriptor 1 to point to descriptor 0*/
            writeDesr1.nextPtr = (uint32_t)(&writeDesr0);

            /*Don't disable the channel on completion of descriptor*/
            writeDesr0.ctl &= ~0x01000000;
            writeDesr1.ctl &= ~0x01000000;

            /*Decrement yCounts by 2 since we already have 2 descriptors setup*/
            yCounts -= 2;

            /*Enable DMA interrupt*/
            NVIC_EnableIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);
        }

        /*Initialize the channel with the first descriptor*/
        Cy_DMA_Channel_SetDescriptor(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL , &writeDesr0);
    }
}


/*******************************************************************************
* Function Name: SDIO_SendCommandAndWait
****************************************************************************//**
*
* This function sends a command on the command channel and waits for that
* command to finish before returning. If a Command 53 is issued this function
* will handle all of the data transfer and wait to return until it is done.
*
* \param pstcCmd
* Pointer command configuration structure see \ref stc_sdio_cmd_t.
*
* \return
* \ref en_sdio_result_t
*
*******************************************************************************/
en_sdio_result_t SDIO_SendCommandAndWait(stc_sdio_cmd_t *pstcCmd)
{
    /* Store the command and data configurations*/
    stc_sdio_cmd_config_t   stcCmdConfig;
    stc_sdio_data_config_t  stcDataConfig;

    uint32_t u32CmdTimeout = 0;

    /*Returns from various function calls*/
    en_sdio_result_t enRet = Error;
    en_sdio_result_t enRetTmp = Ok;

    /* Hold value of if these checks are needed */
    uint8_t             bCmdIndexCheck;
    uint8_t             bCmdCrcCheck;
    static uint8_t      u8responseBuf[6];

    /* Clear statuses */
    gstcInternalData.stcEvents.u8CmdComplete = 0;
    gstcInternalData.stcEvents.u8TransComplete = 0;
    gstcInternalData.stcEvents.u8CRCError = 0;

    /* Setup the command configuration */
    stcCmdConfig.u8CmdIndex = (uint8_t)pstcCmd->u32CmdIdx;
    stcCmdConfig.u32Argument =  pstcCmd->u32Arg;

#ifdef CY_RTOS_AWARE

    cy_rslt_t result;

    /* Initialize the semaphore. This is not done in init because init is called
    * in interrupt thread. cy_rtos_init_semaphore call is prohibited in
    * interrupt thread.
    */
    if(!sema_initialized)
    {
        cy_rtos_init_semaphore( &sdio_transfer_finished_semaphore, 1, 0 );
        sema_initialized = true;
    }
#else

    /* Variable used for holding timeout value */
    uint32_t    u32Timeout = 0;
#endif

    /*Determine the type of response and if we need to do any checks*/
    /*Command 0 and 8 have no response, so don't wait for one*/
    if (pstcCmd->u32CmdIdx == 0 || pstcCmd->u32CmdIdx == 8)
    {
        bCmdIndexCheck        = false;
        bCmdCrcCheck          = false;
        stcCmdConfig.bResponseRequired     = false;
        stcCmdConfig.pu8ResponseBuf = NULL;
    }

    /*Command 5's response doesn't have a CRC or index, so don't check*/
    else if (pstcCmd->u32CmdIdx == 5)
    {
        bCmdIndexCheck        = false;
        bCmdCrcCheck          = false;
        stcCmdConfig.bResponseRequired     = true;
        stcCmdConfig.pu8ResponseBuf = u8responseBuf;
    }
    /*Otherwise check everything*/
    else
    {
        bCmdIndexCheck        = true;
        bCmdCrcCheck          = true;
        stcCmdConfig.bResponseRequired     = true;
        stcCmdConfig.pu8ResponseBuf = u8responseBuf;
    }

    /*Check if the command is 53, if it is then setup the data transfer*/
    if (pstcCmd->u32CmdIdx == 53)
    {
        /*Set the number of blocks in the global struct*/
        stcDataConfig.u16BlockCount = (uint16_t)pstcCmd->u16BlockCnt;
        /*Set the size of the data transfer*/
        stcDataConfig.u16BlockSize = (uint16_t)pstcCmd->u16BlockSize;
        /*Set the direction are we reading or writing*/
        stcDataConfig.bRead = pstcCmd->bRead;
        /*Set the pointer for the data*/
        stcDataConfig.pu8Data = pstcCmd->pu8Data;

        /*Get the data Transfer Ready*/
        SDIO_InitDataTransfer(&stcDataConfig);

        /*Set bit saying this was a CMD_53*/
        SDIO_CONTROL_REG |= SDIO_CTRL_ENABLE_INT;
    }

    /*Send the command*/
    SDIO_SendCommand(&stcCmdConfig);

    /*Wait for the command to finish*/
    do
    {
        u32CmdTimeout++;
        enRetTmp = SDIO_CheckForEvent(SdCmdEventCmdDone);

    } while ((enRetTmp != Ok) && (u32CmdTimeout < SDIO_CMD_TIMEOUT));


    if (u32CmdTimeout == SDIO_CMD_TIMEOUT)
    {
        enRet |= CMDTimeout;
    }
    else /*CMD Passed*/
    {
        /*If a response is expected check it*/
        if (stcCmdConfig.bResponseRequired == true)
        {
            enRetTmp = SDIO_GetResponse(bCmdCrcCheck, bCmdIndexCheck, (uint8_t)pstcCmd->u32CmdIdx, pstcCmd->pu32Response, u8responseBuf);
            if (enRetTmp != Ok)
            {
                enRet |= enRetTmp;
            }
            else  /*Response good*/
            {
                /*if it was command 53, check the response to ensure there was no error*/
                if ((pstcCmd->u32CmdIdx) == 53)
                {
                    /*Make sure none of the error bits are set*/
                    if (*(pstcCmd->pu32Response) & 0x0000cf00)
                    {
                        enRet |= ResponseFlagError;
                    }
                    else /*CMD53 Response good*/
                    {
                        /*If it was command 53 and it was a write enable the write*/
                        if (pstcCmd->bRead == false && enRet == Error)
                        {
                            Cy_DMA_Channel_Disable(SDIO_HOST_Resp_DMA_HW, SDIO_HOST_Resp_DMA_DW_CHANNEL );
                            Cy_DMA_Channel_Disable(SDIO_HOST_CMD_DMA_HW, SDIO_HOST_CMD_DMA_DW_CHANNEL );
                            Cy_DMA_Channel_Disable(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL );

                            /*Set the flag in the control register to enable the write*/
                            Cy_DMA_Channel_Enable(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL);
                            /*Enable the channel*/
                            Cy_SysLib_DelayCycles(35);
                            SDIO_CONTROL_REG |= SDIO_CTRL_ENABLE_WRITE;
                        }

                    #ifdef CY_RTOS_AWARE
                        /* Wait for the transfer to finish.
                        *  Acquire semaphore and wait until it will be released
                        *  in SDIO_IRQ:
                        *  1. sdio_transfer_finished_semaphore count is equal to
                        *     zero. cy_rtos_get_semaphore waits until semaphore
                        *     count is increased by cy_rtos_set_semaphore() in
                        *     SDIO_IRQ.
                        *  2. The cy_rtos_set_semaphore() increases
                        *     sdio_transfer_finished_semaphore count.
                        *  3. The cy_rtos_get_semaphore() function decreases
                        *     sdio_transfer_finished_semaphore back to zero
                        *     and exit. Or timeout occurs
                        */
                        result = cy_rtos_get_semaphore( &sdio_transfer_finished_semaphore, 10, false );

                        enRetTmp = SDIO_CheckForEvent(SdCmdEventTransferDone);

                        if (result != CY_RSLT_SUCCESS)
                    #else
                         /* Wait for the transfer to finish */
                        do
                        {
                            u32Timeout++;
                            enRetTmp = SDIO_CheckForEvent(SdCmdEventTransferDone);

                        } while (!((enRetTmp == Ok) || (enRetTmp == DataCrcError) || (u32Timeout >= SDIO_DAT_TIMEOUT)));

                        if (u32Timeout == SDIO_DAT_TIMEOUT)
                    #endif
                        {
                            enRet |= DataTimeout;
                        }

                        /* if it was a read it is possible there is still extra data hanging out, trigger the
                           DMA again. This can result in extra data being transfered so the read buffer should be
                           3 bytes bigger than needed*/
                        if (pstcCmd->bRead == true)
                        {
                            Cy_TrigMux_SwTrigger((uint32_t)SDIO_HOST_Read_DMA_DW__TR_IN, 2);
                        }

                        if (enRetTmp == DataCrcError)
                        {
                            enRet |= DataCrcError;
                        }
                    }/*CMD53 response good*/
                }/*Not a CMD53*/
            } /*Response Good*/
        } /*No Response Required, thus no CMD53*/
    } /*CMD Passed*/

#ifndef CY_RTOS_AWARE
    u32Timeout = 0;
#endif

    /*If there were no errors then indicate transfer was okay*/
    if (enRet == Error)
    {
        enRet = Ok;
    }

    /*reset CmdTimeout value*/
    u32CmdTimeout = 0;

    /*Always Reset on exit to clean up*/
    Cy_DMA_Channel_Disable(SDIO_HOST_Resp_DMA_HW, SDIO_HOST_Resp_DMA_DW_CHANNEL );
    Cy_DMA_Channel_Disable(SDIO_HOST_CMD_DMA_HW, SDIO_HOST_CMD_DMA_DW_CHANNEL );
    Cy_DMA_Channel_Disable(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL );
    Cy_DMA_Channel_Disable(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL );
    /*No longer a CMD_53*/
    SDIO_CONTROL_REG &= ~(SDIO_CTRL_ENABLE_INT | SDIO_CTRL_ENABLE_WRITE | SDIO_CTRL_ENABLE_READ);
    SDIO_Reset();

    return enRet;
}


/*******************************************************************************
* Function Name: SDIO_CheckForEvent
****************************************************************************//**
*
* Checks to see if a specific event has occurred such a command complete or
* transfer complete.
*
* \param enEventType
* The type of event to check for. See \ref en_sdio_event_t.
*
* \return
* \ref en_sdio_result_t
*
*******************************************************************************/
en_sdio_result_t SDIO_CheckForEvent(en_sdio_event_t enEventType)
{
    en_sdio_result_t  enRet      = Error;

    /*Disable Interrupts while modifying the global*/
    NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_sdio_int__INTC_NUMBER);

    /*Switch the event to check*/
    switch ( enEventType )
    {
        /*If the command is done clear the flag*/
        case SdCmdEventCmdDone:
            if (gstcInternalData.stcEvents.u8CmdComplete > 0)
            {
                gstcInternalData.stcEvents.u8CmdComplete = 0;
                enRet = Ok;
            }
            break;

        /*If the transfer is done check for CRC Error and clear the flag*/
        case SdCmdEventTransferDone:
            if (gstcInternalData.stcEvents.u8TransComplete > 0)
            {
                gstcInternalData.stcEvents.u8TransComplete = 0;
                enRet = Ok;
            }
            /*Check for CRC error and set flags*/
            if (gstcInternalData.stcEvents.u8CRCError > 0)
            {
                enRet = DataCrcError;
                gstcInternalData.stcEvents.u8CRCError = 0;
            }
            break;
    }

    /*Re-enable Interrupts*/
    NVIC_EnableIRQ((IRQn_Type) SDIO_HOST_sdio_int__INTC_NUMBER);
    return enRet;
}


/*******************************************************************************
* Function Name: SDIO_CalculateCrc7
****************************************************************************//**
*
* Calculate the 7 bit CRC for the command channel
*
* \param pu8Data
* Data to calculate CRC on
*
* \param u8Size
* Number of bytes to calculate CRC on
*
* \return
* CRC
*
* \note
* This code was copied from
* http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
*
*******************************************************************************/
uint8_t SDIO_CalculateCrc7(uint8_t* pu8Data, uint8_t u8Size)
{
    uint8_t data;
    uint8_t remainder = 0;
    uint32_t byte;

    for(byte = 0; byte < u8Size; ++byte)
    {
        data = pu8Data[byte] ^ remainder;
        remainder = crcTable[data] ^ (remainder << 8);
    }

    return (remainder>>1);
}


/*******************************************************************************
* Function Name: SDIO_Crc7Init
****************************************************************************//**
*
* Initialize 7-bit CRC Table
*
* \note
* This code was copied from
* http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
*
*******************************************************************************/
void SDIO_Crc7Init(void)
{
    uint8_t remainder;
    uint8_t bit;
    uint32_t dividend;

    for(dividend = 0; dividend < 256; ++dividend)
    {
        remainder = dividend;

        for(bit = 8; bit > 0; --bit)
        {
            if (remainder & SDIO_CRC_UPPER_BIT)
            {
                remainder = (remainder << 1) ^ SDIO_CRC7_POLY;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }

        crcTable[dividend] = (remainder);
    }
}


/*******************************************************************************
* Function Name: SDIO_SetBlockSize
****************************************************************************//**
*
* Sets the size of each block
*
* \param u8ByteCount
* Size of the block
*
*******************************************************************************/
void SDIO_SetBlockSize(uint8_t u8ByteCount)
{
    SDIO_BYTE_COUNT_REG = u8ByteCount;
}


/*******************************************************************************
* Function Name: SDIO_SetNumBlocks
****************************************************************************//**
*
* Sets the number of blocks to send
*
* \param u8BlockCount
* Size of the block
*
*******************************************************************************/
void SDIO_SetNumBlocks(uint8_t u8BlockCount)
{
    SDIO_DATA_BLOCK_COUNTER_A0_REG = u8BlockCount;
    SDIO_DATA_BLOCK_COUNTER_D0_REG = u8BlockCount;
    /*The one is used so that we can do 256 bytes*/
    SDIO_DATA_BLOCK_COUNTER_A1_REG = 1;
    SDIO_DATA_BLOCK_COUNTER_D1_REG = 1;
}


/*******************************************************************************
* Function Name: SDIO_EnableIntClock
****************************************************************************//**
*
* Enable Internal clock for the block
*
*******************************************************************************/
void SDIO_EnableIntClock(void)
{
    SDIO_CONTROL_REG |= SDIO_CTRL_INT_CLK;
    Cy_SysClk_PeriphEnableDivider(SDIO_HOST_Internal_Clock_DIV_TYPE, SDIO_HOST_Internal_Clock_DIV_NUM);
}


/*******************************************************************************
* Function Name: SDIO_DisableIntClock
****************************************************************************//**
*
* Enable Disable clock for the block
*
*******************************************************************************/
void SDIO_DisableIntClock(void)
{
    SDIO_CONTROL_REG &= ~SDIO_CTRL_INT_CLK;
    Cy_SysClk_PeriphDisableDivider(SDIO_HOST_Internal_Clock_DIV_TYPE, SDIO_HOST_Internal_Clock_DIV_NUM);
}


/*******************************************************************************
* Function Name: SDIO_EnableSdClk
****************************************************************************//**
*
* Enable SD Clock out to pin
*
*******************************************************************************/
void SDIO_EnableSdClk(void)
{
    SDIO_CONTROL_REG |= SDIO_CTRL_SD_CLK;
}


/*******************************************************************************
* Function Name: SDIO_DisableSdClk
****************************************************************************//**
*
* Disable SD Clock out to the pin
*
*******************************************************************************/
void SDIO_DisableSdClk(void)
{
    SDIO_CONTROL_REG &= ~SDIO_CTRL_SD_CLK;
}


/*******************************************************************************
* Function Name: SDIO_SetSdClkFrequency
****************************************************************************//**
*
* Sets the frequency of the SD Clock
*
* \param u32SdClkFreqHz
* Frequency of SD Clock in Hz.
*
* \note
* Only an integer divider is used, so the desired frequency may not be meet
*******************************************************************************/
void SDIO_SetSdClkFrequency(uint32_t u32SdClkFreqHz)
{
    uint16_t u16Div;
    /*
     * The UDB SDIO implemenation has a extra divider internally that divides the input clock to the UDB
     * by 2. The desired clock frequency is hence intentionally multiplied by 2 in order to get the required
     * SDIO operating frequency.
     */
    u16Div = Cy_SysClk_ClkPeriGetFrequency() / (2 * u32SdClkFreqHz);
    Cy_SysClk_PeriphSetDivider(SDIO_HOST_Internal_Clock_DIV_TYPE, SDIO_HOST_Internal_Clock_DIV_NUM, (u16Div-1));
}


/*******************************************************************************
* Function Name: SDIO_SetupDMA
****************************************************************************//**
*
* Configures the DMA for the SDIO block
*
*******************************************************************************/
void SDIO_SetupDMA(void)
{
    /*Set the number of bytes to send*/
    SDIO_HOST_CMD_DMA_CMD_DMA_Desc_config.xCount = (SDIO_NUM_RESP_BYTES - 1);
    /*Set the destination address*/
    SDIO_HOST_CMD_DMA_CMD_DMA_Desc_config.dstAddress = (void*)SDIO_CMD_COMMAND_PTR;

    /*Initialize descriptor for cmd channel*/
    Cy_DMA_Descriptor_Init(&cmdDesr, &SDIO_HOST_CMD_DMA_CMD_DMA_Desc_config);

    /*Set flag to disable descriptor when done*/
    cmdDesr.ctl |= 0x01000000;

    /*Configure channel*/
    /*CMD channel can be preempted, and has lower priority*/
    cmdChannelConfig.descriptor = &cmdDesr;
    cmdChannelConfig.preemptable = 1;
    cmdChannelConfig.priority    = 1;
    cmdChannelConfig.enable      = 0u;

    /*Configure Channel with initial Settings*/
    Cy_DMA_Channel_Init(SDIO_HOST_CMD_DMA_HW, SDIO_HOST_CMD_DMA_DW_CHANNEL, &cmdChannelConfig);

    /*Enable DMA block*/
    Cy_DMA_Enable(SDIO_HOST_CMD_DMA_HW);

    /*Set the number of bytes to receive*/
    SDIO_HOST_Resp_DMA_Resp_DMA_Desc_config.xCount = SDIO_NUM_RESP_BYTES;
    /*Set the source address*/
    SDIO_HOST_Resp_DMA_Resp_DMA_Desc_config.srcAddress = (void*)SDIO_CMD_RESPONSE_PTR;

    /*Initialize descriptor for response channel*/
    Cy_DMA_Descriptor_Init(&respDesr, &SDIO_HOST_Resp_DMA_Resp_DMA_Desc_config);

    /*Set flag to disable descriptor when done*/
    respDesr.ctl |= 0x01000000;

    /*Configure channel*/
    /*response channel can be preempted, and has lower priority*/
    respChannelConfig.descriptor = &respDesr;
    respChannelConfig.preemptable = 1;
    respChannelConfig.priority    = 1;
    respChannelConfig.enable      = 0u;

    /*Configure Channel with initial Settings*/
    Cy_DMA_Channel_Init(SDIO_HOST_Resp_DMA_HW, SDIO_HOST_Resp_DMA_DW_CHANNEL, &respChannelConfig);
    /*Enable DMA block*/
    Cy_DMA_Enable(SDIO_HOST_Resp_DMA_HW);

    /*Set the destination address*/
    SDIO_HOST_Write_DMA_Write_DMA_Desc_config.dstAddress = (void*)SDIO_DAT_WRITE_PTR;

    /*Initialize descriptor for write channel*/
    Cy_DMA_Descriptor_Init(&writeDesr0, &SDIO_HOST_Write_DMA_Write_DMA_Desc_config);
    Cy_DMA_Descriptor_Init(&writeDesr1, &SDIO_HOST_Write_DMA_Write_DMA_Desc_config);

    /*Configure channel*/
    /*write channel cannot be preempted, and has highest priority*/
    writeChannelConfig.descriptor = &writeDesr0;
    writeChannelConfig.preemptable = 0;
    writeChannelConfig.priority    = 0;
    writeChannelConfig.enable      = 0u;

    /*Configure Channel with initial Settings*/
    Cy_DMA_Channel_Init(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL, &writeChannelConfig);

    /*Enable the interrupt*/
    Cy_DMA_Channel_SetInterruptMask(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL,CY_DMA_INTR_MASK);

    /*Enable DMA block*/
    Cy_DMA_Enable(SDIO_HOST_Write_DMA_HW);

    /*Set the source address*/
    SDIO_HOST_Read_DMA_Read_DMA_Desc_config.srcAddress = (void*)SDIO_DAT_READ_PTR;
    /*Initialize descriptor for read channel*/
    Cy_DMA_Descriptor_Init(&readDesr0, &SDIO_HOST_Read_DMA_Read_DMA_Desc_config);
    Cy_DMA_Descriptor_Init(&readDesr1, &SDIO_HOST_Read_DMA_Read_DMA_Desc_config);

    /*Configure channel*/
    /*read channel cannot be preempted, and has highest priority*/
    readChannelConfig.descriptor = &readDesr0;
    readChannelConfig.preemptable = 0;
    readChannelConfig.priority    = 0;
    readChannelConfig.enable      = 0u;

    /*Configure Channel with initial Settings*/
    Cy_DMA_Channel_Init(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL, &readChannelConfig);

    /*Enable the interrupt*/
    Cy_DMA_Channel_SetInterruptMask(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL,CY_DMA_INTR_MASK);

    /*Enable DMA block*/
    Cy_DMA_Enable(SDIO_HOST_Read_DMA_HW);
}


/*******************************************************************************
* Function Name: SDIO_Reset
****************************************************************************//**
*
* Reset the SDIO interface
*
*******************************************************************************/
void SDIO_Reset(void)
{
    /*Control register is in pulse mode, so this just pulses the reset*/
    SDIO_CONTROL_REG |= (SDIO_CTRL_RESET_DP);
}


/*******************************************************************************
* Function Name: SDIO_EnableChipInt
****************************************************************************//**
*
* Enables the SDIO Chip Int by setting the mask bit
*
*******************************************************************************/
void SDIO_EnableChipInt(void)
{
    SDIO_STATUS_INT_MSK |= SDIO_STS_CARD_INT;
}


/*******************************************************************************
* Function Name: SDIO_DisableChipInt
****************************************************************************//**
*
* Enables the SDIO Chip Int by setting the mask bit
*
*******************************************************************************/
void SDIO_DisableChipInt(void)
{
    SDIO_STATUS_INT_MSK &= ~SDIO_STS_CARD_INT;
}


/*******************************************************************************
* Function Name: SDIO_IRQ
****************************************************************************//**
*
* SDIO interrupt, checks for events, and calls callbacks
*
*******************************************************************************/
void SDIO_IRQ(void)
{
    uint8_t u8Status;

    /* First read the status register */
    u8Status = SDIO_STATUS_REG;

    /* Check card interrupt */
    if (u8Status & SDIO_STS_CARD_INT )
    {
        pfnCardInt_count++;
    }

    /* Execute card interrupt callback if neccesary */
    if (0 != pfnCardInt_count)
    {
        if (NULL != gstcInternalData.pstcCallBacks.pfnCardIntCb)
        {
            gstcInternalData.pstcCallBacks.pfnCardIntCb();
        }
        pfnCardInt_count--;
    }

    /* If the command is complete set the flag */
    if (u8Status & SDIO_STS_CMD_DONE)
    {
        gstcInternalData.stcEvents.u8CmdComplete++;
    }

    /* Check if a write is complete */
    if (u8Status & SDIO_STS_WRITE_DONE )
    {

        /* Clear the Write flag and CMD53 flag */
        SDIO_CONTROL_REG &= ~(SDIO_CTRL_ENABLE_WRITE | SDIO_CTRL_ENABLE_INT);

        /* Check if the CRC status return was bad */
        if (u8Status & SDIO_STS_CRC_ERR)
        {
            /* CRC was bad, set the flag */
            gstcInternalData.stcEvents.u8CRCError++;
        }

        /* Set the done flag */

    #ifdef CY_RTOS_AWARE
        cy_rtos_set_semaphore( &sdio_transfer_finished_semaphore, true );
    #else
        gstcInternalData.stcEvents.u8TransComplete++;
    #endif
    }

    /* Check if a read is complete */
    if (u8Status & SDIO_STS_READ_DONE)
    {
        /* Clear the read flag */
        SDIO_CONTROL_REG &= ~(SDIO_CTRL_ENABLE_READ| SDIO_CTRL_ENABLE_INT);

        /* Check the CRC */
        if (u8Status & SDIO_STS_CRC_ERR)
        {
            /* CRC was bad, set the flag */
            gstcInternalData.stcEvents.u8CRCError++;
        }
        /* Okay we're done so set the done flag */
    #ifdef CY_RTOS_AWARE
        cy_rtos_set_semaphore( &sdio_transfer_finished_semaphore, true );
    #else
        gstcInternalData.stcEvents.u8TransComplete++;
    #endif
    }

    NVIC_ClearPendingIRQ((IRQn_Type) SDIO_HOST_sdio_int__INTC_NUMBER);
}


/*******************************************************************************
* Function Name: SDIO_READ_DMA_IRQ
****************************************************************************//**
*
* SDIO DMA Read interrupt, checks counts and toggles to other descriptor if
* needed
*
*******************************************************************************/
void SDIO_READ_DMA_IRQ(void)
{
    /*Shouldn't have to change anything unless it is the last descriptor*/

    /*If the current descriptor is 0, then change descriptor 1*/
    if (Cy_DMA_Channel_GetCurrentDescriptor(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL) == &readDesr0)
    {
        /*We need to increment the destination address every time*/
        readDesr1.dst += 2048;

        /*If this is the last descriptor*/
        if ((yCounts == 1) && (yCountRemainder == 0))
        {
            /* In this case all we need to change is the next descriptor and disable*/
            readDesr1.nextPtr = 0;
            readDesr1.ctl |= 0x01000000;
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);
        }
        else if (yCounts == 0 && (yCountRemainder > 0))
        {
            /*change next descriptor, and disable*/
            readDesr1.nextPtr = 0;
            readDesr1.ctl |= 0x01000000;
            /*Also change the yCount*/
            readDesr1.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (yCountRemainder-1)) |
                                    _VAL2FLD(CY_DMA_CTL_DST_INCR, 2);
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);
        }
    }

    /*If the current descriptor is 1, then change descriptor 0*/
    if (Cy_DMA_Channel_GetCurrentDescriptor(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL) == &readDesr1)
    {
        /*We need to increment the destination address everytime*/
        readDesr0.dst += 2048;

        /*If this is the last descriptor*/
        if ((yCounts == 1) && (yCountRemainder == 0))
        {
            /* In this case all we need to change is the next descriptor and disable*/
            readDesr0.nextPtr = 0;
            readDesr0.ctl |= 0x01000000;
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);
        }
        else if (yCounts == 0 && (yCountRemainder > 0))
        {
            /*change next descriptor, and disable*/
            readDesr0.nextPtr = 0;
            readDesr0.ctl |= 0x01000000;
            /*Also change the yCount*/
            readDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (yCountRemainder-1)) |
                                    _VAL2FLD(CY_DMA_CTL_DST_INCR, 2);
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Read_Int_INTC_NUMBER);
        }
    }

    /*Clear the interrupt*/
    Cy_DMA_Channel_ClearInterrupt(SDIO_HOST_Read_DMA_HW, SDIO_HOST_Read_DMA_DW_CHANNEL);
    /*decrement y counts*/
    yCounts--;
}

/*******************************************************************************
* Function Name: SDIO_WRITE_DMA_IRQ
****************************************************************************//**
*
* SDIO DMA Write interrupt, checks counts and toggles to other descriptor if
* needed
*
*******************************************************************************/
void SDIO_WRITE_DMA_IRQ(void)
{
    /*We shouldn't have to change anything unless it is the last descriptor*/

    /*If the current descriptor is 0, then change descriptor 1*/
    if (Cy_DMA_Channel_GetCurrentDescriptor(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL) == &writeDesr0)
    {
        /*We also need to increment the destination address every-time*/
        writeDesr1.src += 2048;

        /*If this is the last descriptor*/
        if ((yCounts == 1) && (yCountRemainder == 0))
        {
            /* In this case all we need to change is the next descriptor and disable*/
            writeDesr1.nextPtr = 0;
            writeDesr1.ctl |= 0x01000000;
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);

        }
        else if (yCounts == 0 && (yCountRemainder > 0))
        {
            /*change next descriptor, and disable*/
            writeDesr1.nextPtr = 0;
            writeDesr1.ctl |= 0x01000000;
            /*Also change the yCount*/
            writeDesr1.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (yCountRemainder -1)) |
                                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, 2);
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);
        }
    }

    /*If the current descriptor is 1, then change descriptor 0*/
    if (Cy_DMA_Channel_GetCurrentDescriptor(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL) == &writeDesr1)
    {
        /*We also need to increment the destination address*/
        writeDesr0.src += 2048;
        /*If this is the last descriptor*/
        if ((yCounts == 1) && (yCountRemainder == 0))
        {
            /* In this case all we need to change is the next descriptor and disable*/
            writeDesr0.nextPtr = 0;
            writeDesr0.ctl |= 0x01000000;
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);
        }
        else if (yCounts == 0 && (yCountRemainder > 0))
        {
            /*change next descriptor, and disable*/
            writeDesr0.nextPtr = 0;
            writeDesr0.ctl |= 0x01000000;
            /*Also change the yCount*/
            writeDesr0.yCtl = _VAL2FLD(CY_DMA_CTL_COUNT, (yCountRemainder -1)) |
                                    _VAL2FLD(CY_DMA_CTL_SRC_INCR, 2);
            NVIC_DisableIRQ((IRQn_Type) SDIO_HOST_Write_Int_INTC_NUMBER);
        }
    }

    /*Clear the interrupt*/
    Cy_DMA_Channel_ClearInterrupt(SDIO_HOST_Write_DMA_HW, SDIO_HOST_Write_DMA_DW_CHANNEL);
    yCounts--;
}

/*******************************************************************************
* Function Name: SDIO_Free
****************************************************************************//**
*
* Frees any system resources that were allocated by the SDIO driver.
*
*******************************************************************************/
void SDIO_Free(void)
{
#ifdef CY_RTOS_AWARE
    cy_rtos_deinit_semaphore(&sdio_transfer_finished_semaphore);
#endif
}

/*******************************************************************************
* Function Name: SDIO_SaveConfig
********************************************************************************
*
* Saves the user configuration of the SDIO UDB non-retention registers. Call the
* SDIO_SaveConfig() function before the Cy_SysPm_CpuEnterDeepSleep() function.
*
*******************************************************************************/
static void SDIO_SaveConfig(void)
{
    regs.CY_SDIO_UDB_WRKMULT_CTL_0 = UDB->WRKMULT.CTL[0];
    regs.CY_SDIO_UDB_WRKMULT_CTL_1 = UDB->WRKMULT.CTL[1];
    regs.CY_SDIO_UDB_WRKMULT_CTL_2 = UDB->WRKMULT.CTL[2];
    regs.CY_SDIO_UDB_WRKMULT_CTL_3 = UDB->WRKMULT.CTL[3];
}


/*******************************************************************************
* Function Name: SDIO_RestoreConfig
********************************************************************************
*
* Restores the user configuration of the SDIO UDB non-retention registers. Call
* the SDIO_Wakeup() function after the Cy_SysPm_CpuEnterDeepSleep() function.
*
*******************************************************************************/
static void SDIO_RestoreConfig(void)
{
    UDB->WRKMULT.CTL[0] = regs.CY_SDIO_UDB_WRKMULT_CTL_0;
    UDB->WRKMULT.CTL[1] = regs.CY_SDIO_UDB_WRKMULT_CTL_1;
    UDB->WRKMULT.CTL[2] = regs.CY_SDIO_UDB_WRKMULT_CTL_2;
    UDB->WRKMULT.CTL[3] = regs.CY_SDIO_UDB_WRKMULT_CTL_3;
}

#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */
