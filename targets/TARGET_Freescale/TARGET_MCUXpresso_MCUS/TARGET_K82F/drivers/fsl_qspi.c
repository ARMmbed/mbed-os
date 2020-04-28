/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_qspi.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.qspi"
#endif


/*******************************************************************************
 * Definitations
 ******************************************************************************/
enum _qspi_transfer_state
{
    kQSPI_TxBusy = 0x0U, /*!< QSPI is busy */
    kQSPI_TxIdle,        /*!< Transfer is done. */
    kQSPI_TxError        /*!< Transfer error occured. */
};

#define QSPI_AHB_BUFFER_REG(base, index) (((volatile uint32_t *)&((base)->BUF0CR))[(index)])

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Base pointer array */
static QuadSPI_Type *const s_qspiBases[] = QuadSPI_BASE_PTRS;
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/* Clock name array */
static const clock_ip_name_t s_qspiClock[] = QSPI_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t QSPI_GetInstance(QuadSPI_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_qspiBases); instance++)
    {
        if (s_qspiBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_qspiBases));

    return instance;
}

void QSPI_Init(QuadSPI_Type *base, qspi_config_t *config, uint32_t srcClock_Hz)
{
    uint32_t i = 0;
    uint32_t val = 0;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable QSPI clock */
    CLOCK_EnableClock(s_qspiClock[QSPI_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Do software reset to QSPI module */
    QSPI_SoftwareReset(base);

    /* Clear the FIFO region */
    QSPI_ClearFifo(base, kQSPI_AllFifo);

    /* Configure QSPI */
    QSPI_Enable(base, false);

#if !defined (FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL) || (!FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL)
    /* Set qspi clock source */
    base->SOCCR = config->clockSource;

    /* Set the divider of QSPI clock */
    base->MCR &= ~QuadSPI_MCR_SCLKCFG_MASK;

    if (srcClock_Hz % config->baudRate) {
        /* In case we cannot get the exact baudrate, get the closest lower value */
        base->MCR |= QuadSPI_MCR_SCLKCFG(srcClock_Hz / config->baudRate);
    } else {
        base->MCR |= QuadSPI_MCR_SCLKCFG(srcClock_Hz / config->baudRate - 1U);
    }

#endif /* FSL_FEATURE_QSPI_CLOCK_CONTROL_EXTERNAL */

    /* Set AHB buffer size and buffer master */
    for (i = 0; i < FSL_FEATURE_QSPI_AHB_BUFFER_COUNT; i++)
    {
        val = QuadSPI_BUF0CR_MSTRID(config->AHBbufferMaster[i]) | QuadSPI_BUF0CR_ADATSZ(config->AHBbufferSize[i] / 8U);
        QSPI_AHB_BUFFER_REG(base, i) = val;
    }
    if (config->enableAHBbuffer3AllMaster)
    {
        base->BUF3CR |= QuadSPI_BUF3CR_ALLMST_MASK;
    }
    else
    {
        base->BUF3CR &= ~QuadSPI_BUF3CR_ALLMST_MASK;
    }

    /* Set watermark */
    base->RBCT &= ~QuadSPI_RBCT_WMRK_MASK;
    base->RBCT |= QuadSPI_RBCT_WMRK(config->rxWatermark - 1);

#if !defined (FSL_FEATURE_QSPI_HAS_NO_TXDMA) || (!FSL_FEATURE_QSPI_HAS_NO_TXDMA)
    base->TBCT &= ~QuadSPI_TBCT_WMRK_MASK;
    base->TBCT |= QuadSPI_TBCT_WMRK(config->txWatermark - 1);
#endif /* FSL_FEATURE_QSPI_HAS_NO_TXDMA */

    /* Enable QSPI module */
    if (config->enableQspi)
    {
        QSPI_Enable(base, true);
    }
}

void QSPI_GetDefaultQspiConfig(qspi_config_t *config)
{
    config->clockSource = 2U;
    config->baudRate = 24000000U;
    config->AHBbufferMaster[0] = 0xE;
    config->AHBbufferMaster[1] = 0xE;
    config->AHBbufferMaster[2] = 0xE;
    config->enableAHBbuffer3AllMaster = true;
    config->txWatermark = 8;
    config->rxWatermark = 8;
    config->enableQspi = true;
}

void QSPI_Deinit(QuadSPI_Type *base)
{
    QSPI_Enable(base, false);
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    CLOCK_DisableClock(s_qspiClock[QSPI_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

void QSPI_SetFlashConfig(QuadSPI_Type *base, qspi_flash_config_t *config)
{
    uint32_t address = FSL_FEATURE_QSPI_AMBA_BASE + config->flashA1Size;
    uint32_t val = 0;
    uint32_t i = 0;

    /* Disable module */
    QSPI_Enable(base, false);

    /* Config the serial flash size */
    base->SFA1AD = address;
    address += config->flashA2Size;
    base->SFA2AD = address;
#if defined(FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE) && (FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE)
    address += config->flashB1Size;
    base->SFB1AD = address;
    address += config->flashB2Size;
    base->SFB2AD = address;
#endif /* FSL_FEATURE_QSPI_SUPPORT_PARALLEL_MODE */

#if !defined (FSL_FEATURE_QSPI_HAS_NO_SFACR) || (!FSL_FEATURE_QSPI_HAS_NO_SFACR)
    /* Set Word Addressable feature */
    val = QuadSPI_SFACR_WA(config->enableWordAddress) | QuadSPI_SFACR_CAS(config->cloumnspace);
    base->SFACR = val;
#endif /* FSL_FEATURE_QSPI_HAS_NO_SFACR */

    /* Config look up table */
    base->LUTKEY = 0x5AF05AF0U;
    base->LCKCR = 0x2U;
    for (i = 0; i < FSL_FEATURE_QSPI_LUT_DEPTH; i++)
    {
        base->LUT[i] = config->lookuptable[i];
    }
    base->LUTKEY = 0x5AF05AF0U;
    base->LCKCR = 0x1U;

#if !defined (FSL_FEATURE_QSPI_HAS_NO_TDH) || (!FSL_FEATURE_QSPI_HAS_NO_TDH)
    /* Config flash timing */
    val = QuadSPI_FLSHCR_TCSS(config->CSHoldTime) | QuadSPI_FLSHCR_TDH(config->dataHoldTime) |
          QuadSPI_FLSHCR_TCSH(config->CSSetupTime);
#else
    val = QuadSPI_FLSHCR_TCSS(config->CSHoldTime) | QuadSPI_FLSHCR_TCSH(config->CSSetupTime);
#endif /* FSL_FEATURE_QSPI_HAS_NO_TDH */
    base->FLSHCR = val;

    /* Set flash endianness */
    base->MCR &= ~QuadSPI_MCR_END_CFG_MASK;
    base->MCR |= QuadSPI_MCR_END_CFG(config->endian);

    /* Enable QSPI again */
    QSPI_Enable(base, true);
}

void QSPI_SoftwareReset(QuadSPI_Type *base)
{
    uint32_t i = 0;

    /* Reset AHB domain and buffer domian */
    base->MCR |= (QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK);

    /* Wait several time for the reset to finish, this method came from IC team */
    for (i = 0; i < 100; i++)
    {
        __ASM("nop");
    }

    /* Disable QSPI module */
    QSPI_Enable(base, false);

    /* Clear the reset flags */
    base->MCR &= ~(QuadSPI_MCR_SWRSTHD_MASK | QuadSPI_MCR_SWRSTSD_MASK);

    /* Enable QSPI module */
    QSPI_Enable(base, true);
}

uint32_t QSPI_GetRxDataRegisterAddress(QuadSPI_Type *base)
{
    /* From RDBR */
    if (base->RBCT & QuadSPI_RBCT_RXBRD_MASK)
    {
        return (uint32_t)(&(base->RBDR[0]));
    }
    else
    {
        /* From ARDB */
        return FSL_FEATURE_QSPI_ARDB_BASE;
    }
}

void QSPI_ExecuteIPCommand(QuadSPI_Type *base, uint32_t index)
{
    while (QSPI_GetStatusFlags(base) & (kQSPI_Busy | kQSPI_IPAccess))
    {
    }
    QSPI_ClearCommandSequence(base, kQSPI_IPSeq);

    /* Write the seqid bit */
    base->IPCR = ((base->IPCR & (~QuadSPI_IPCR_SEQID_MASK)) | QuadSPI_IPCR_SEQID(index / 4U));
}

void QSPI_ExecuteAHBCommand(QuadSPI_Type *base, uint32_t index)
{
    while (QSPI_GetStatusFlags(base) & (kQSPI_Busy | kQSPI_AHBAccess))
    {
    }
    QSPI_ClearCommandSequence(base, kQSPI_BufferSeq);
    base->BFGENCR = ((base->BFGENCR & (~QuadSPI_BFGENCR_SEQID_MASK)) | QuadSPI_BFGENCR_SEQID(index / 4U));
}

void QSPI_UpdateLUT(QuadSPI_Type *base, uint32_t index, uint32_t *cmd)
{
    uint8_t i = 0;

    /* Unlock the LUT */
    base->LUTKEY = 0x5AF05AF0U;
    base->LCKCR = 0x2U;

    /* Write data into LUT */
    for (i = 0; i < 4; i++)
    {
        base->LUT[index + i] = *cmd;
        cmd++;
    }

    /* Lcok LUT again */
    base->LUTKEY = 0x5AF05AF0U;
    base->LCKCR = 0x1U;
}

void QSPI_SetReadDataArea(QuadSPI_Type *base, qspi_read_area_t area)
{
    base->RBCT &= ~QuadSPI_RBCT_RXBRD_MASK;
    base->RBCT |= QuadSPI_RBCT_RXBRD(area);
}

uint32_t QSPI_ReadData(QuadSPI_Type *base)
{
    if (base->RBCT & QuadSPI_RBCT_RXBRD_MASK)
    {
        return base->RBDR[0];
    }
    else
    {
        /* Data from ARDB. */
        return *((uint32_t *)FSL_FEATURE_QSPI_ARDB_BASE);
    }
}

void QSPI_WriteBlocking(QuadSPI_Type *base, uint32_t *buffer, size_t size)
{
    assert(size >= 16U);

    uint32_t i = 0;

    for (i = 0; i < size / 4U; i++)
    {
        /* Check if the buffer is full */
        while (QSPI_GetStatusFlags(base) & kQSPI_TxBufferFull)
        {
        }
        QSPI_WriteData(base, *buffer);
        buffer++;
    }
}

void QSPI_ReadBlocking(QuadSPI_Type *base, uint32_t *buffer, size_t size)
{
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t temp = 0;
    uint32_t level = (base->RBCT & QuadSPI_RBCT_WMRK_MASK) + 1U;

    while (i < size / 4)
    {
        /* Check if there is data */
        if ((size / 4 - i) < level)
        {
            do
            {
                temp = (base->RBSR & QuadSPI_RBSR_RDBFL_MASK) >> QuadSPI_RBSR_RDBFL_SHIFT;
            } while (!temp);
        }
        else
        {
            while ((QSPI_GetStatusFlags(base) & kQSPI_RxWatermark) == 0U)
            {
            }
        }

        level = (level < (size / 4 - i)) ? level : (size / 4 - i);

        /* Data from RBDR */
        if (base->RBCT & QuadSPI_RBCT_RXBRD_MASK)
        {
            for (j = 0; j < level; j++)
            {
                buffer[i + j] = base->RBDR[j];
            }
        }
        else
        {
            /* Data from ARDB. */
            for (j = 0; j < level; j++)
            {
                buffer[i + j] = ((uint32_t *)FSL_FEATURE_QSPI_ARDB_BASE)[j];
            }
        }
        i += level;

        /* Clear the Buffer */
        QSPI_ClearErrorFlag(base, kQSPI_RxBufferDrain);
    }
}
