/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
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

#include "fsl_dspi_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_master_init
 * Description   : Configure the DSPI peripheral in master mode.
 * This function will initialize the module to user defined settings and default settings in master
 * mode.  Here is an example demonstrating how to define the dspi_master_config_t structure and call
 * the dspi_hal_master_init function:
 *    dspi_master_config_t dspiConfig;
 *    dspiConfig.isEnabled = false;
 *    dspiConfig.whichCtar = kDspiCtar0;
 *    dspiConfig.bitsPerSec = 0;
 *    dspiConfig.sourceClockInHz = dspiSourceClock;
 *    dspiConfig.isSckContinuous = false;
 *    dspiConfig.whichPcs = kDspiPcs0;
 *    dspiConfig.pcsPolarity = kDspiPcs_ActiveLow;
 *    dspiConfig.masterInSample = kDspiSckToSin_0Clock;
 *    dspiConfig.isModifiedTimingFormatEnabled = false;
 *    dspiConfig.isTxFifoDisabled = false;
 *    dspiConfig.isRxFifoDisabled = false;
 *    dspiConfig.dataConfig.bitsPerFrame = 16;
 *    dspiConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh;
 *    dspiConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge;
 *    dspiConfig.dataConfig.direction = kDspiMsbFirst;
 *    dspi_hal_master_init(instance, &dspiConfig, calculatedBaudRate);
 *
 *END**************************************************************************/
dspi_status_t dspi_hal_master_init(uint32_t instance, const dspi_master_config_t * config,
                       uint32_t * calculatedBaudRate)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* Enable or disable the module. */
    /* Note, to enable the module, MDIS must be cleared.  However, the member isEnabled*/
    /* must be true (1) to enable module, hence we negate the value of isEnabled to properly*/
    /* configure the MDIS bit*/
    BW_SPI_MCR_MDIS(instance, ~(config->isEnabled == true));

    /* Configure baud rate if a value is provided.*/
    if (config->bitsPerSec != 0U)
    {
        *calculatedBaudRate = dspi_hal_set_baud(instance, config->whichCtar, config->bitsPerSec,
                                               config->sourceClockInHz);
    }
    else
    {
        *calculatedBaudRate = 0;
    }

    /* Set master or slave mode.*/
    dspi_hal_set_master_slave(instance, kDspiMaster);

    /* Configure data format.*/
    if (dspi_hal_configure_data_format(instance, config->whichCtar, &config->dataConfig)
        != kStatus_DSPI_Success)
    {
        return kStatus_DSPI_InvalidBitCount;
    }

    /* Configure for continuous SCK operation*/
    dspi_hal_configure_continuous_sck(instance, config->isSckContinuous);

    /* Configure for peripheral chip select polarity*/
    dspi_hal_configure_pcs_polarity(instance, config->whichPcs,config->pcsPolarity);

    /* Configure sample point for data in, master mode*/
    dspi_hal_set_datain_samplepoint(instance, config->masterInSample);

    /* Configure for modified timing format*/
    dspi_hal_configure_modified_timing_format(instance, config->isModifiedTimingFormatEnabled);

    /* Configure for fifo operation*/
    dspi_hal_configure_fifos(instance, config->isTxFifoDisabled, config->isRxFifoDisabled);

    /* finally, clear the DSPI CONFIGURATION (DCONF), even though this is cleared in some IPs*/
    /* by default and other bit settings are reserved*/
    HW_SPI_MCR_CLR(instance, BM_SPI_MCR_DCONF);

    return kStatus_DSPI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_slave_init
 * Description   : Configure the DSPI peripheral in slave mode.
 * This function initializes the DSPI module for slave mode. Here is an example demonstrating how
 * to define the dspi_slave_config_t structure and call the dspi_hal_slave_init function:
 *    dspi_slave_config_t dspiConfig;
 *    dspiConfig.isEnabled = false;
 *    dspiConfig.isTxFifoDisabled = false;
 *    dspiConfig.isRxFifoDisabled = false;
 *    dspiConfig.dataConfig.bitsPerFrame = 16;
 *    dspiConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh;
 *    dspiConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge;
 *    dspi_hal_slave_init(instance, &dspiConfig);
 *
 *END**************************************************************************/
dspi_status_t dspi_hal_slave_init(uint32_t instance, const dspi_slave_config_t * config)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* Enable or disable the module.
     * Note, to enable the module, MDIS must be cleared.  However, the member isEnabled
     * must be true (1) to enable module, hence we negate the value of isEnabled to properly
     * configure the MDIS bit
     */
    BW_SPI_MCR_MDIS(instance, ~(config->isEnabled == true));

    /* Set master or slave moe. */
    dspi_hal_set_master_slave(instance, kDspiSlave);

    /* Configure data format. For slave mode, only CTAR0 is available for use */
    if (dspi_hal_configure_data_format(instance, kDspiCtar0, &config->dataConfig)
        != kStatus_DSPI_Success)
    {
        return kStatus_DSPI_InvalidBitCount;
    }

    /* Configure for fifo operation */
    dspi_hal_configure_fifos(instance, config->isTxFifoDisabled, config->isRxFifoDisabled);

    /* finally, clear the DSPI CONFIGURATION (DCONF), even though this is cleared in some IPs
     * by default and other bit settings are reserved
     */
    HW_SPI_MCR_CLR(instance, BM_SPI_MCR_DCONF);

    return kStatus_DSPI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_reset
 * Description   : Restore DSPI to reset configuration.
 * This function basically resets all of the DSPI registers to their default setting including
 * disabling the module.
 *
 *END**************************************************************************/
void dspi_hal_reset(uint32_t instance)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* first, make sure the module is enabled to allow writes to certain registers*/
    dspi_hal_enable(instance);

    /* Halt all transfers*/
    HW_SPI_MCR_WR(instance, BM_SPI_MCR_HALT);

    /* flush the fifos*/
    dspi_hal_flush_fifos(instance, true, true);

    /* set the registers to their default states*/
    /* clear the status bits (write-1-to-clear)*/
    HW_SPI_SR_WR(instance, BM_SPI_SR_TCF | BM_SPI_SR_EOQF | BM_SPI_SR_TFUF | BM_SPI_SR_TFFF |
                           BM_SPI_SR_RFOF | BM_SPI_SR_RFDF);
    HW_SPI_TCR_WR(instance, 0);
    HW_SPI_CTARn_WR(instance, 0, 0); /* CTAR0*/
    HW_SPI_CTARn_WR(instance, 1, 0); /* CTAR1*/
    HW_SPI_RSER_WR(instance, 0);
    /* disable the module*/
    HW_SPI_MCR_WR(instance, BM_SPI_MCR_MDIS | BM_SPI_MCR_HALT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_set_baud
 * Description   : Set the DSPI baud rate in bits per second.
 * This function will take in the desired bitsPerSec (baud rate) and will calculate the nearest
 * possible baud rate without exceeding the desired baud rate, and will return the calculated
 * baud rate in bits-per-second. It requires that the caller also provide the frequency of the
 * module source clock (in Hz).
 *
 *END**************************************************************************/
uint32_t dspi_hal_set_baud(uint32_t instance, dspi_ctar_selection_t whichCtar, uint32_t bitsPerSec,
                        uint32_t sourceClockInHz)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* for master mode configuration, if slave mode detected, return 0*/
    if (HW_SPI_MCR(instance).B.MSTR != 1)
    {
        return 0;
    }

    uint32_t prescaler, bestPrescaler;
    uint32_t scaler, bestScaler;
    uint32_t dbr, bestDbr;
    uint32_t realBaudrate, bestBaudrate;
    uint32_t diff, min_diff;
    uint32_t baudrate = bitsPerSec;

    /* find combination of prescaler and scaler resulting in baudrate closest to the */
    /* requested value */
    min_diff = 0xFFFFFFFFU;
    bestPrescaler = 0;
    bestScaler = 0;
    bestDbr = 1;
    bestBaudrate = 0; /* required to avoid compilation warning */

    /* In all for loops, if min_diff = 0, the exit for loop*/
    for (prescaler = 0; (prescaler < 4) && min_diff; prescaler++)
    {
        for (scaler = 0; (scaler < 16) && min_diff; scaler++)
        {
            for (dbr = 1; (dbr < 3) && min_diff; dbr++)
            {
                realBaudrate = ((sourceClockInHz * dbr) /
                                (s_baudratePrescaler[prescaler] * (s_baudrateScaler[scaler])));

                /* calculate the baud rate difference based on the conditional statement*/
                /* that states that the calculated baud rate must not exceed the desired baud rate*/
                if (baudrate >= realBaudrate)
                {
                    diff = baudrate-realBaudrate;
                    if (min_diff > diff)
                    {
                        /* a better match found */
                        min_diff = diff;
                        bestPrescaler = prescaler;
                        bestScaler = scaler;
                        bestBaudrate = realBaudrate;
                        bestDbr = dbr;
                    }
                }
            }
        }
    }

    uint32_t temp;
    /* write the best dbr, prescalar, and baud rate scalar to the CTAR*/
    temp = HW_SPI_CTARn_RD(instance, whichCtar); /* save register contents*/
    temp &= ~(BM_SPI_CTARn_DBR| BM_SPI_CTARn_PBR | BM_SPI_CTARn_BR);
    temp |= BF_SPI_CTARn_DBR(bestDbr - 1) |
            BF_SPI_CTARn_PBR(bestPrescaler) |
            BF_SPI_CTARn_BR(bestScaler);
    HW_SPI_CTARn_WR(instance, whichCtar, temp);

    /* return the actual calculated baud rate*/
    return bestBaudrate;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_set_baud_divisors
 * Description   : Configure the baud rate divisors manually.
 * This function allows the caller to manually set the baud rate divisors in the event that
 * these dividers are known and the caller does not wish to call the dspi_hal_set_baud function.
 *
 *END**************************************************************************/
void dspi_hal_set_baud_divisors(uint32_t instance,
                                dspi_ctar_selection_t whichCtar,
                                const dspi_baud_rate_divisors_t * divisors)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    uint32_t temp;

    /* these settings are only relevant in master mode*/
    if (HW_SPI_MCR(instance).B.MSTR == 1)
    {
        temp = HW_SPI_CTARn_RD(instance, whichCtar); /* save register contents*/
        temp &= ~(BM_SPI_CTARn_DBR | BM_SPI_CTARn_PBR | BM_SPI_CTARn_BR); /* clear dividers*/
        temp |= BF_SPI_CTARn_DBR(divisors->doubleBaudRate) |
                BF_SPI_CTARn_PBR(divisors->prescaleDivisor) |
                BF_SPI_CTARn_BR(divisors->baudRateDivisor);
        HW_SPI_CTARn_WR(instance, whichCtar, temp);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_configure_pcs_polarity
 * Description   : Configure DSPI peripheral chip select polarity.
 * This function will take in the desired peripheral chip select (PCS) and it's
 * corresponding desired polarity and will configure the PCS signal to operate with the
 * desired characteristic.
 *
 *END**************************************************************************/
void dspi_hal_configure_pcs_polarity(uint32_t instance, dspi_which_pcs_config_t pcs,
                                     dspi_pcs_polarity_config_t activeLowOrHigh)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    uint32_t temp;

    temp = BR_SPI_MCR_PCSIS(instance);

    if (activeLowOrHigh == kDspiPcs_ActiveLow)
    {
        temp |= pcs;
    }
    else  /* kDspiPcsPolarity_ActiveHigh*/
    {
        temp &= ~(unsigned)pcs;
    }

    BW_SPI_MCR_PCSIS(instance, temp);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_configure_fifos
 * Description   : Configure DSPI fifos.
 * This function with allow the caller to disable/enable the TX and RX FIFOs (independently).
 * Note that to disable, the caller must pass in a logic 1 (true) for the particular FIFO
 * configuration.  To enable, the caller must pass in a logic 0 (false).  For example, to enable
 * both the TX and RX FIFOs, the caller will make this function call (where instance is the
 *
 *END**************************************************************************/
void dspi_hal_configure_fifos(uint32_t instance, bool disableTxFifo, bool disableRxFifo)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* first see if MDIS is set or cleared */
    uint32_t isMdisSet = HW_SPI_MCR(instance).B.MDIS;

    if (isMdisSet)
    {
        /* clear the MDIS bit to allow us to write to the fifo disables */
        HW_SPI_MCR_CLR(instance, BM_SPI_MCR_MDIS);
    }

    BW_SPI_MCR_DIS_TXF(instance, (disableTxFifo == true));
    BW_SPI_MCR_DIS_RXF(instance, (disableRxFifo == true));

    /* set MDIS if it was set to begin with */
    if (isMdisSet)
    {
        HW_SPI_MCR_SET(instance, BM_SPI_MCR_MDIS);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_flush_fifos
 * Description   : Flush DSPI fifos.
 *
 *END**************************************************************************/
void dspi_hal_flush_fifos(uint32_t instance, bool enableFlushTxFifo, bool enableFlushRxFifo)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    BW_SPI_MCR_CLR_TXF(instance, (enableFlushTxFifo == true));
    BW_SPI_MCR_CLR_RXF(instance, (enableFlushRxFifo == true));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_configure_data_format
 * Description   : Configure the data format for a particular CTAR.
 * This function configures the bits-per-frame, polarity, phase, and shift direction for a
 * particular CTAR. An example use case is as follows:
 *    dspi_data_format_config_t dataFormat;
 *    dataFormat.bitsPerFrame = 16;
 *    dataFormat.clkPolarity = kDspiClockPolarity_ActiveLow;
 *    dataFormat.clkPhase = kDspiClockPhase_FirstEdge;
 *    dataFormat.direction = kDspiMsbFirst;
 *    dspi_hal_configure_data_format(instance, kDspiCtar0, &dataFormat);
 *
 *END**************************************************************************/
dspi_status_t dspi_hal_configure_data_format(uint32_t instance,
                                        dspi_ctar_selection_t whichCtar,
                                        const dspi_data_format_config_t * config)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    /* check bits-per-frame value to make sure it it within the proper range*/
    /* in either master or slave mode*/
    if ((config->bitsPerFrame < 4) ||
        ((config->bitsPerFrame > 16) && (HW_SPI_MCR(instance).B.MSTR == 1)) ||
        ((config->bitsPerFrame > 32) && (HW_SPI_MCR(instance).B.MSTR == 0)))
    {
        return kStatus_DSPI_InvalidBitCount;
    }

    uint32_t temp;

    /* for master mode configuration*/
    if (HW_SPI_MCR(instance).B.MSTR == 1)
    {
        temp = HW_SPI_CTARn_RD(instance, whichCtar); /* save register contents*/
        temp &= ~(BM_SPI_CTARn_FMSZ| BM_SPI_CTARn_CPOL | BM_SPI_CTARn_CPHA | BM_SPI_CTARn_LSBFE);
        temp |= BF_SPI_CTARn_FMSZ(config->bitsPerFrame - 1) |
                BF_SPI_CTARn_CPOL(config->clkPolarity) |
                BF_SPI_CTARn_CPHA(config->clkPhase) |
                BF_SPI_CTARn_LSBFE(config->direction);
        HW_SPI_CTARn_WR(instance, whichCtar, temp);
    }
    else /* for slave mode configuration*/
    {
        temp = HW_SPI_CTARn_SLAVE_RD(instance, whichCtar); /* save register contents*/
        temp &= ~(BM_SPI_CTARn_SLAVE_FMSZ| BM_SPI_CTARn_SLAVE_CPOL | BM_SPI_CTARn_SLAVE_CPHA);
        temp |= BF_SPI_CTARn_SLAVE_FMSZ(config->bitsPerFrame - 1) |
                BF_SPI_CTARn_SLAVE_CPOL(config->clkPolarity) |
                BF_SPI_CTARn_SLAVE_CPHA(config->clkPhase);
        HW_SPI_CTARn_SLAVE_WR(instance, whichCtar, temp);
    }
    return kStatus_DSPI_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_configure_delays
 * Description   : Configure the delays for a particular CTAR, master mode only.
 * This function configures the PCS to SCK delay prescalar (PCSSCK),
 * the PCS to SCK Delay scalar (CSSCK),
 * the After SCK delay prescalar (PASC),
 * the After SCK delay scalar (ASC),
 * the Delay after transfer prescalar (PDT),
 * and the Delay after transfer scalar (DT).
 * The following is an example use case of this function:
 *    dspi_delay_settings_config_t delayConfig;
 *    delayConfig.pcsToSckPre = 0x3;
 *    delayConfig.pcsToSck = 0xF;
 *    delayConfig.afterSckPre = 0x2;
 *    delayConfig.afterSck = 0xA;
 *    delayConfig.afterTransferPre = 0x1;
 *    delayConfig.afterTransfer = 0x5;
 *    dspi_hal_configure_delays(instance, kDspiCtar0, &delayConfig);
 *
 *END**************************************************************************/
void dspi_hal_configure_delays(uint32_t instance,
                               dspi_ctar_selection_t whichCtar,
                               const dspi_delay_settings_config_t * config)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    uint32_t temp;

    /* these settings are only relevant in master mode*/
    if (HW_SPI_MCR(instance).B.MSTR == 1)
    {
        temp = HW_SPI_CTARn_RD(instance, whichCtar); /* save register contents*/
        temp &= ~(BM_SPI_CTARn_PCSSCK | BM_SPI_CTARn_PASC | BM_SPI_CTARn_PDT |
                  BM_SPI_CTARn_CSSCK| BM_SPI_CTARn_ASC | BM_SPI_CTARn_DT);
        temp |= BF_SPI_CTARn_PCSSCK(config->pcsToSckPre) |
                BF_SPI_CTARn_PASC(config->afterSckPre) |
                BF_SPI_CTARn_PDT(config->afterTransferPre) |
                BF_SPI_CTARn_CSSCK(config->pcsToSck) |
                BF_SPI_CTARn_ASC(config->afterSck) |
                BF_SPI_CTARn_DT(config->afterTransfer);
        HW_SPI_CTARn_WR(instance, whichCtar, temp);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_configure_dma
 * Description   : Configure transmit and receive DMA requests.
 * This function configures the FIFOs to generate a DMA or interrupt request. Note that the
 * corresponding request enable must also be set.  For the Transmit FIFO Fill, in order
 * to generate a DMA request, the Transmit FIFO Fill Request Enable (TFFF_RE) must also be set.
 * Similarly for the Receive FIFO Drain Request, to generate a DMA request, the Receive FIFO Drain
 * Request Enable (RFDF_RE) must also be set.  These request enables can be configured via
 * the function dspi_hal_configure_interrupt().  So basically to enable DMA operation, first enable
 * the desired request enable using the dspi_hal_configure_interrupt() function and then use
 * the dspi_hal_configure_dma() to configure the request to generate a DMA reuqest.
 *
 *END**************************************************************************/
void dspi_hal_configure_dma(uint32_t instance, bool enableTransmit, bool enableReceive)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    BW_SPI_RSER_TFFF_DIRS(instance, enableTransmit);
    BW_SPI_RSER_RFDF_DIRS(instance, enableReceive);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_configure_interrupt
 * Description   : Configure DSPI interrupts.
 * This function will configure the various interrupt sources of the DSPI.  The parameters to pass
 * in are instance, interrupt source, and enable/disable setting.
 * The interrupt source will be of a typedef enum whose value will be the bit position of the
 * interrupt source setting within the RSER register.  In the DSPI, all of the interrupt
 * configuration settings reside within the one register.  The typedef enum will equate each
 * interrupt source to the bit position defined in the device header file.
 * The function will use these bit positions in its algorithm to enable/disable the
 * interrupt source, where interrupt source is of type dspi_status_and_interrupt_request_t.
 *    temp = (HW_SPI_RSER_RD(instance) & ~interruptSrc) | (enable << interruptSrc);
 *    HW_SPI_RSER_WR(instance, temp);
 *
 *    dspi_hal_configure_interrupt(instance, kDspiTxComplete, true); <- example use-case
 *
 *END**************************************************************************/
void dspi_hal_configure_interrupt(uint32_t instance,
                                  dspi_status_and_interrupt_request_t interruptSrc,
                                  bool enable)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    uint32_t temp;

    temp = (HW_SPI_RSER_RD(instance) & ~(0x1U << interruptSrc)) | ((uint32_t)enable << interruptSrc);
    HW_SPI_RSER_WR(instance, temp);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_get_fifo_data
 * Description   : Read fifo registers for debug purposes.
 *
 *END**************************************************************************/
uint32_t dspi_hal_get_fifo_data(uint32_t instance, dspi_fifo_t whichFifo, uint32_t whichFifoEntry)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    if (whichFifo == kDspiTxFifo)
    {
        return HW_SPI_TXFRn_RD(instance, whichFifoEntry);
    }
    else
    {
        return HW_SPI_RXFRn_RD(instance, whichFifoEntry);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : dspi_hal_write_data_master_mode
 * Description   : Write data into the data buffer, master mode.
 * In master mode, the 16-bit data is appended with the 16-bit command info. The command portion
 * provides characteristics of the data being sent such as: optional continuous chip select
 * operation between transfers, the desired Clock and Transfer Attributes register to use for the
 * associated SPI frame, the desired PCS signal to use for the data transfer, whether the current
 * transfer is the last in the queue, and whether to clear the transfer count (normally needed when
 * sending the first frame of a data packet). An example use case is as follows:
 *    dspi_command_config_t commandConfig;
 *    commandConfig.isChipSelectContinuous = true;
 *    commandConfig.whichCtar = kDspiCtar0;
 *    commandConfig.whichPcs = kDspiPcs1;
 *    commandConfig.clearTransferCount = false;
 *    commandConfig.isEndOfQueue = false;
 *    dspi_hal_write_data_master_mode(instance, &commandConfig, dataWord);
 *
 *END**************************************************************************/
void dspi_hal_write_data_master_mode(uint32_t instance,
                                     dspi_command_config_t * command,
                                     uint16_t data)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);

    uint32_t temp;

    temp = BF_SPI_PUSHR_CONT(command->isChipSelectContinuous) |
           BF_SPI_PUSHR_CTAS(command->whichCtar) |
           BF_SPI_PUSHR_PCS(command->whichPcs) |
           BF_SPI_PUSHR_EOQ(command->isEndOfQueue) |
           BF_SPI_PUSHR_CTCNT(command->clearTransferCount) |
           BF_SPI_PUSHR_TXDATA(data);

    HW_SPI_PUSHR_WR(instance, temp);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

