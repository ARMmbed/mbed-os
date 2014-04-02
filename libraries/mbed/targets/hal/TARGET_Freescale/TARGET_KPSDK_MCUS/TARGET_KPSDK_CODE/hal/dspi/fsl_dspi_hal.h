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
#if !defined(__FSL_DSPI_HAL_H__)
#define __FSL_DSPI_HAL_H__

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include "fsl_dspi_features.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup dspi_hal
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Defines constant value arrays for the baud rate pre-scalar and scalar divider values.*/
static const uint32_t s_baudratePrescaler[] = { 2, 3, 5, 7 };
static const uint32_t s_baudrateScaler[] = { 2, 4, 6, 8, 16, 32, 64, 128, 256, 512, 1024, 2048,
                                           4096, 8192, 16384, 32768 };

/*! @brief Error codes for the DSPI driver.*/
typedef enum _dspi_status
{
    kStatus_DSPI_Success = 0,
    kStatus_DSPI_SlaveTxUnderrun,        /*!< DSPI Slave Tx Under run error*/
    kStatus_DSPI_SlaveRxOverrun,             /*!< DSPI Slave Rx Overrun error*/
    kStatus_DSPI_Timeout,                    /*!< DSPI transfer timed out*/
    kStatus_DSPI_Busy,                       /*!< DSPI instance is already busy performing a
                                                 transfer.*/
    kStatus_DSPI_NoTransferInProgress,       /*!< Attempt to abort a transfer when no transfer
                                                  was in progress*/
    kStatus_DSPI_InvalidBitCount,             /*!< bits-per-frame value not valid*/
    kStatus_DSPI_InvalidInstanceNumber,      /*!< DSPI instance number does not match current count*/
    kStatus_DSPI_OutOfRange               /*< DSPI out-of-range error used in slave callback */
} dspi_status_t;

/*! @brief DSPI master or slave configuration*/
typedef enum _dspi_master_slave_mode {
    kDspiMaster = 1,     /*!< DSPI peripheral operates in master mode*/
    kDspiSlave = 0       /*!< DSPI peripheral operates in slave mode*/
} dspi_master_slave_mode_t;

/*! @brief DSPI clock polarity configuration for a given CTAR*/
typedef enum _dspi_clock_polarity {
    kDspiClockPolarity_ActiveHigh = 0,   /*!< Active-high DSPI clock (idles low)*/
    kDspiClockPolarity_ActiveLow = 1     /*!< Active-low DSPI clock (idles high)*/
} dspi_clock_polarity_t;

/*! @brief DSPI clock phase configuration for a given CTAR*/
typedef enum _dspi_clock_phase {
    kDspiClockPhase_FirstEdge = 0,       /*!< Data is captured on the leading edge of the SCK and
                                              changed on the following edge.*/
    kDspiClockPhase_SecondEdge = 1       /*!< Data is changed on the leading edge of the SCK and
                                              captured on the following edge.*/
} dspi_clock_phase_t;

/*! @brief DSPI data shifter direction options for a given CTAR*/
typedef enum _dspi_shift_direction {
    kDspiMsbFirst = 0,   /*!< Data transfers start with most significant bit.*/
    kDspiLsbFirst = 1    /*!< Data transfers start with least significant bit.*/
} dspi_shift_direction_t;

/*! @brief DSPI Clock and Transfer Attributes Register (CTAR) selection*/
typedef enum _dspi_ctar_selection {
    kDspiCtar0 = 0,   /*!< CTAR0 selection option for master or slave mode*/
    kDspiCtar1 = 1    /*!< CTAR1 selection option for master mode only*/
} dspi_ctar_selection_t;

/*! @brief DSPI Peripheral Chip Select (PCS) Polarity configuration.*/
typedef enum _dspi_pcs_polarity_config {
    kDspiPcs_ActiveHigh = 0, /*!< PCS Active High (idles low)*/
    kDspiPcs_ActiveLow  = 1 /*!< PCS Active Low (idles high)*/
} dspi_pcs_polarity_config_t;

/*! @brief DSPI Peripheral Chip Select (PCS) configuration (which PCS to configure)*/
typedef enum _dspi_which_pcs_config {
    kDspiPcs0 = 1 << 0, /*!< PCS[0] */
    kDspiPcs1 = 1 << 1, /*!< PCS[1] */
    kDspiPcs2 = 1 << 2, /*!< PCS[2] */
    kDspiPcs3 = 1 << 3, /*!< PCS[3] */
    kDspiPcs4 = 1 << 4, /*!< PCS[4] */
    kDspiPcs5 = 1 << 5  /*!< PCS[5] */
} dspi_which_pcs_config_t;

/*!
 * @brief DSPI Sample Point: Controls when the DSPI master samples SIN in Modified Transfer
 *  Format. This field is valid only when CPHA bit in CTAR register is 0.
 */
typedef enum _dspi_master_sample_point {
    kDspiSckToSin_0Clock = 0,  /*!< 0 system clocks between SCK edge and SIN sample*/
    kDspiSckToSin_1Clock = 1,  /*!< 1 system clock between SCK edge and SIN sample*/
    kDspiSckToSin_2Clock = 2   /*!< 2 system clocks between SCK edge and SIN sample*/
} dspi_master_sample_point_t;

/*! @brief DSPI FIFO selects*/
typedef enum _dspi_fifo {
    kDspiTxFifo = 0,    /*!< DSPI Tx FIFO*/
    kDspiRxFifo = 1     /*!< DSPI Rx FIFO.*/
} dspi_fifo_t;

/*! @brief DSPI status flags and interrupt request enable*/
typedef enum _dspi_status_and_interrupt_request {
    kDspiTxComplete = BP_SPI_RSER_TCF_RE,  /*!< TCF status/interrupt enable */
    kDspiTxAndRxStatus = BP_SPI_SR_TXRXS,  /*!< TXRXS status only, no interrupt*/
    kDspiEndOfQueue = BP_SPI_RSER_EOQF_RE, /*!< EOQF status/interrupt enable*/
    kDspiTxFifoUnderflow = BP_SPI_RSER_TFUF_RE, /*!< TFUF status/interrupt enable*/
    kDspiTxFifoFillRequest = BP_SPI_RSER_TFFF_RE, /*!< TFFF status/interrupt enable*/
    kDspiRxFifoOverflow = BP_SPI_RSER_RFOF_RE, /*!< RFOF status/interrupt enable*/
    kDspiRxFifoDrainRequest = BP_SPI_RSER_RFDF_RE /*!< RFDF status/interrupt enable*/

} dspi_status_and_interrupt_request_t;

/*! @brief DSPI FIFO counter or pointer defines based on bit positions*/
typedef enum _dspi_fifo_counter_pointer {
    kDspiRxFifoPointer = BP_SPI_SR_POPNXTPTR, /*!< Rx FIFO pointer*/
    kDspiRxFifoCounter = BP_SPI_SR_RXCTR,     /*!< Rx FIFO counter*/
    kDspiTxFifoPointer = BP_SPI_SR_TXNXTPTR,  /*!< Tx FIFO pointer*/
    kDspiTxFifoCounter = BP_SPI_SR_TXCTR      /*!< Tx FIFO counter*/
} dspi_fifo_counter_pointer_t;

/*!
 * @brief DSPI data format settings configuration structure
 *
 * This structure contains the data format settings.  These settings apply to a specific
 * CTARn register, which the user must provide in this structure.
 */
typedef struct DspiDataFormatConfig {
    uint32_t bitsPerFrame;        /*!< Bits per frame, minimum 4, maximum 16 (master), 32 (slave) */
    dspi_clock_polarity_t clkPolarity;   /*!< Active high or low clock polarity*/
    dspi_clock_phase_t clkPhase;     /*!< Clock phase setting to change and capture data*/
    dspi_shift_direction_t direction; /*!< MSB or LSB data shift direction
                                           This setting relevant only in master mode and
                                           can be ignored in slave  mode */
} dspi_data_format_config_t;

/*!
 * @brief DSPI hardware configuration settings for master mode
 *
 * Use an instance of this structure with the dspi_hal_master_init() to configure the
 * most common settings of the DSPI peripheral in master mode with a single function call.
 *
 * The @c bitsPerSec member is handled in a special way. If this value is set to 0, then the baud is
 * not set by the dspi_hal_master_init(), and must be set with a separate call to either the
 * dspi_hal_set_baud() or the dspi_hal_set_baud_divisors(). This can be useful when you know the
 * divisors in advance and don't want to spend the time to compute them for the provided rate
 * in bits/sec.
 *
 * This structure also contains another structure template as a member:
 * @c dspi_data_format_config_t @c dataConfig.
 * An example usage for this is assuming declaration @c dspi_master_config_t
 * @c dspiConfig:
   @code
    dspiConfig.dataConfig.bitsPerFrame = 16;
    dspiConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh;
    dspiConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge;
    dspiConfig.dataConfig.direction = kDspiMsbFirst;
   @endcode
 */
typedef struct DspiMasterConfig {
    bool isEnabled;                         /*!< Set to true to enable the DSPI peripheral.*/
    dspi_ctar_selection_t whichCtar;  /*!< Desired Clock and Transfer Attributes Register (CTAR)*/
    uint32_t bitsPerSec;                   /*!< Baud rate in bits per second*/
    uint32_t sourceClockInHz;               /*!< Module source clock  */
    dspi_data_format_config_t dataConfig;    /*!< Data format configuration structure*/
    bool isSckContinuous;                    /*!< Disable(0) or Enable(1) continuous SCK operation*/
    dspi_which_pcs_config_t whichPcs;        /*!< Desired Peripheral Chip Select (PCS) */
    dspi_pcs_polarity_config_t pcsPolarity;  /*!< Peripheral Chip Select (PCS) polarity setting.*/
    dspi_master_sample_point_t masterInSample; /*!< Master data-in (SIN) sample point setting.*/
    bool isModifiedTimingFormatEnabled;      /*!< Disable(0) or Enable(1) modified timing format.*/
    bool isTxFifoDisabled;                    /*!< Disable(1) or Enable(0) Tx FIFO  */
    bool isRxFifoDisabled;                    /*!< Disable(1) or Enable(0) Rx FIFO */
} dspi_master_config_t;

/*!
 * @brief DSPI hardware configuration settings for slave mode.
 *
 * Use an instance of this structure with the dspi_hal_slave_init() to configure the
 * most common settings of the DSPI peripheral in slave mode with a single function call.
 */
typedef struct DspiSlaveConfig {
    bool isEnabled;                         /*!< Set to true to enable the DSPI peripheral. */
    dspi_data_format_config_t dataConfig;    /*!< Data format configuration structure */
    bool isTxFifoDisabled;                    /*!< Disable(1) or Enable(0) Tx FIFO */
    bool isRxFifoDisabled;                    /*!< Disable(1) or Enable(0) Rx FIFO */
} dspi_slave_config_t;

/*!
 * @brief DSPI baud rate divisors settings configuration structure.
 *
 * Note: These settings are relevant only in master mode.
 * This structure contains the baud rate divisor settings, which provides the user with the option to
 * explicitly set these baud rate divisors. In addition, the user must also set the
 * CTARn register  with the divisor settings.
 */
typedef struct DspiBaudRateDivisors {
    bool doubleBaudRate;          /*!< Double Baud rate parameter setting */
    uint32_t prescaleDivisor;     /*!< Baud Rate Pre-scalar parameter setting*/
    uint32_t baudRateDivisor;     /*!< Baud Rate scaler parameter setting */
} dspi_baud_rate_divisors_t;

/*!
 * @brief DSPI delay settings configuration structure
 *
 * Note: These settings are relevant only in master mode.
 * This structure contains the various delay settings.  These settings apply to a specific
 * CTARn register, which the user must provide in this structure.
 */
typedef struct DspiDelaySettingsConfig {
    uint32_t pcsToSckPre;         /*!< PCS to SCK delay pre-scalar (PCSSCK) */
    uint32_t pcsToSck;            /*!< PCS to SCK Delay scalar (CSSCK) */
    uint32_t afterSckPre;         /*!< After SCK delay pre-scalar (PASC)*/
    uint32_t afterSck;            /*!< After SCK delay scalar (ASC)*/
    uint32_t afterTransferPre;    /*!< Delay after transfer pre-scalar (PDT)*/
    uint32_t afterTransfer;       /*!< Delay after transfer scalar (DT) */
} dspi_delay_settings_config_t;

/*!
 * @brief DSPI command and data configuration structure
 *
 * Note: This structure is used  with the PUSHR register, which
 * provides the means to write to the Tx FIFO. Data written to this register is
 * transferred to the Tx FIFO. Eight or sixteen-bit write accesses to the PUSHR transfer all
 * 32 register bits to the Tx FIFO. The register structure is different in master and slave
 * modes. In master mode, the register provides 16-bit command and 16-bit data to the Tx
 * FIFO. In slave mode all 32 register bits can be used as data, supporting up to 32-bit SPI
 * frame operation.
 */
typedef struct DspiCommandDataConfig {
    bool isChipSelectContinuous;  /*!< Option to enable the continuous assertion of chip select
                                       between transfers*/
    dspi_ctar_selection_t whichCtar; /*!< The desired Clock and Transfer Attributes
                                          Register (CTAR) to use for CTAS*/
    dspi_which_pcs_config_t whichPcs;   /*!< The desired PCS signal to use for the data transfer*/
    bool isEndOfQueue;            /*!< Signals that the current transfer is the last in the queue*/
    bool clearTransferCount;      /*!< Clears SPI_TCNT field; cleared before transmission starts*/
} dspi_command_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Configuration
 * @{
 */

/*!
 * @brief Configure the DSPI peripheral in master mode
 *
 * This function  initializes the module to the user defined settings and default settings in master
 * mode.  This is an example demonstrating how to define the dspi_master_config_t structure and call
 * the dspi_hal_master_init function:
   @code
    dspi_master_config_t dspiConfig;
    dspiConfig.isEnabled = false;
    dspiConfig.whichCtar = kDspiCtar0;
    dspiConfig.bitsPerSec = 0;
    dspiConfig.sourceClockInHz = dspiSourceClock;
    dspiConfig.isSckContinuous = false;
    dspiConfig.whichPcs = kDspiPcs0;
    dspiConfig.pcsPolarity = kDspiPcs_ActiveLow;
    dspiConfig.masterInSample = kDspiSckToSin_0Clock;
    dspiConfig.isModifiedTimingFormatEnabled = false;
    dspiConfig.isTxFifoDisabled = false;
    dspiConfig.isRxFifoDisabled = false;
    dspiConfig.dataConfig.bitsPerFrame = 16;
    dspiConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh;
    dspiConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge;
    dspiConfig.dataConfig.direction = kDspiMsbFirst;
    dspi_hal_master_init(instance, &dspiConfig, calculatedBaudRate);
   @endcode
 *
 * @param instance Module instance number
 * @param config   Pointer to the master mode configuration data structure
 * @param calculatedBaudRate The calculated baud rate passed back to the user for them to determine
 *                           if the calculated baud rate is close enough to meet their needs.
 * @return  An error code or kStatus_DSPI_Success.
 */
dspi_status_t dspi_hal_master_init(uint32_t instance, const dspi_master_config_t * config,
                       uint32_t * calculatedBaudRate);

/*!
 * @brief Configures the DSPI peripheral in slave mode.
 *
 * This function initializes the DSPI module for slave mode. This is an example demonstrating how
 * to define the dspi_slave_config_t structure and call the dspi_hal_slave_init function:
   @code
    dspi_slave_config_t dspiConfig;
    dspiConfig.isEnabled = false;
    dspiConfig.isTxFifoDisabled = false;
    dspiConfig.isRxFifoDisabled = false;
    dspiConfig.dataConfig.bitsPerFrame = 16;
    dspiConfig.dataConfig.clkPolarity = kDspiClockPolarity_ActiveHigh;
    dspiConfig.dataConfig.clkPhase = kDspiClockPhase_FirstEdge;
    dspi_hal_slave_init(instance, &dspiConfig);
   @endcode
 *
 * @param instance Module instance number
 * @param config   Pointer to the slave mode configuration data structure
 * @return  An error code or kStatus_DSPI_Success.
 */
dspi_status_t dspi_hal_slave_init(uint32_t instance, const dspi_slave_config_t * config);

/*!
 * @brief Restores the DSPI to reset the configuration.
 *
 * This function basically resets all of the DSPI registers to their default setting including
 * disabling the module.
 *
 * @param instance Module instance number
 */
void dspi_hal_reset(uint32_t instance);

/*!
 * @brief Enable the DSPI peripheral, set MCR MDIS to 0.
 *
 * @param instance Module instance number
 */
static inline void dspi_hal_enable(uint32_t instance)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    HW_SPI_MCR_CLR(instance, BM_SPI_MCR_MDIS);
}

/*!
 * @brief Disables the DSPI peripheral, sets MCR MDIS to 1.
 *
 * @param instance Module instance number
 */
static inline void dspi_hal_disable(uint32_t instance)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    HW_SPI_MCR_SET(instance, BM_SPI_MCR_MDIS);
}

/*!
 * @brief Sets the DSPI baud rate in bits per second.
 *
 * This function  takes in the desired bitsPerSec (baud rate) and  calculates the nearest
 * possible baud rate without exceeding the desired baud rate, and  returns the calculated
 * baud rate in bits-per-second. It requires that the caller also provide the frequency of the
 * module source clock (in Hertz).
 *
 * @param instance Module instance number
 * @param whichCtar The desired Clock and Transfer Attributes Register (CTAR) of the type
 *                  dspi_ctar_selection_t
 * @param bitsPerSec The desired baud rate in bits per second
 * @param sourceClockInHz Module source input clock in Hertz
 * @return  The actual calculated baud rate
 */
uint32_t dspi_hal_set_baud(uint32_t instance, dspi_ctar_selection_t whichCtar, uint32_t bitsPerSec,
                        uint32_t sourceClockInHz);

/*!
 * @brief Configures the baud rate divisors manually.
 *
 * This function allows the caller to manually set the baud rate divisors in the event that
 * these dividers are known and the caller does not wish to call the dspi_hal_set_baud function.
 *
 * @param instance Module instance number
 * @param whichCtar The desired Clock and Transfer Attributes Register (CTAR) of type
 *                  dspi_ctar_selection_t
 * @param divisors Pointer to a structure containing the user defined baud rate divisor settings
 */
void dspi_hal_set_baud_divisors(uint32_t instance,
                                dspi_ctar_selection_t whichCtar,
                                const dspi_baud_rate_divisors_t * divisors);

/*!
 * @brief Configures the DSPI for master or slave.
 *
 * @param instance Module instance number
 * @param mode Mode setting (master or slave) of type dspi_master_slave_mode_t
 */
static inline void dspi_hal_set_master_slave(uint32_t instance, dspi_master_slave_mode_t mode)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_MSTR(instance, (uint32_t)mode);
}

/*!
 * @brief Configures the DSPI for the continuous SCK operation.
 *
 * @param instance Module instance number
 * @param enable Enables (true) or disables(false) continuous SCK operation.
 */
static inline void dspi_hal_configure_continuous_sck(uint32_t instance, bool enable)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_CONT_SCKE(instance, (enable == true));
}

/*!
 * @brief Configures the DSPI to enable modified timing format.
 *
 * @param instance Module instance number
 * @param enable Enables (true) or disables(false) modified timing format.
 */
static inline void dspi_hal_configure_modified_timing_format(uint32_t instance, bool enable)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_MTFE(instance, (enable == true));
}

/*!
 * @brief Configures the DSPI peripheral chip select strobe enable. Configures the PCS[5] to be the
 *        active-low PCS Strobe output.
 *
 * PCS[5] is a special case that can be configured as an active low PCS strobe or as a Peripheral
 * Chip Select in master mode. When configured as a strobe, it provides a signal to an external
 * demultiplexer to decode PCS[0] to PCS[4] signals into as many as 128 glitch-free PCS signals.
 *
 * @param instance Module instance number
 * @param enable Enable (true) PCS[5] to operate as the peripheral chip select (PCS) strobe
 *               If disable (false), PCS[5] operates as a peripheral chip select
 */
static inline void dspi_hal_configure_pcs_strobe(uint32_t instance, bool enable)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_PCSSE(instance, (enable == true));
}

/*!
 * @brief Configures the DSPI received FIFO overflow overwrite enable.
 *
 * When enabled, this function allows incoming receive data to overwrite the existing data in the
 * receive shift register when the Rx FIFO is full.  Otherwise when disabled, the incoming data
 * is ignored when the RX FIFO is full.
 *
 * @param instance Module instance number.
 * @param enable If enabled (true), allows incoming data to overwrite Rx FIFO contents when full,
 *               else incoming data is ignored.
 */
static inline void dspi_hal_configure_rx_fifo_overwrite(uint32_t instance, bool enable)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_ROOE(instance, (enable == true));
}

/*!
 * @brief Configures the DSPI peripheral chip select polarity.
 *
 * This function  takes in the desired peripheral chip select (PCS) and it's
 * corresponding desired polarity and  configures the PCS signal to operate with the
 * desired characteristic.
 *
 * @param instance Module instance number
 * @param pcs The particular peripheral chip select (parameter value is of type
 *            dspi_which_pcs_config_t) for which we wish to apply the active high or active
 *            low characteristic.
 * @param activeLowOrHigh The setting for either "active high, inactive low (0)"  or
 *                        "active low, inactive high(1)" of type dspi_pcs_polarity_config_t.
 */
void dspi_hal_configure_pcs_polarity(uint32_t instance, dspi_which_pcs_config_t pcs,
                                     dspi_pcs_polarity_config_t activeLowOrHigh);

/*!
 * @brief Configures the DSPI FIFOs.
 *
 * This function  allows the caller to disable/enable the Tx and Rx FIFOs (independently).
 * Note that to disable, the caller must pass in a logic 1 (true) for the particular FIFO
 * configuration.  To enable, the caller must pass in a logic 0 (false).  For example, to enable
 * both the Tx and Rx FIFOs, the caller  makes this function call (where instance is the
 * desired module instance number):
   @code
    dspi_hal_configure_fifos(instance, false, false);
   @endcode
 *
 * @param instance      Module instance number
 * @param disableTxFifo Disables (false) the TX FIFO, else enables (true) the TX FIFO
 * @param disableRxFifo Disables (false) the RX FIFO, else enables (true) the RX FIFO
 */
void dspi_hal_configure_fifos(uint32_t instance, bool disableTxFifo, bool disableRxFifo);

/*!
 * @brief Flushes the DSPI FIFOs.
 *
 * @param instance Module instance number
 * @param enableFlushTxFifo Flushes (true) the Tx FIFO, else do not flush (false) the Tx FIFO
 * @param enableFlushRxFifo Flushes (true) the Rx FIFO, else do not flush (false) the Rx FIFO
 */
void dspi_hal_flush_fifos(uint32_t instance, bool enableFlushTxFifo, bool enableFlushRxFifo);


/*!
 * @brief Configures when the DSPI master samples SIN in  the Modified Transfer Format
 *
 * This function controls when the DSPI master samples SIN (data in) in the Modified Transfer
 * Format.  Note that this is valid only when the CPHA bit in the CTAR register is 0.
 *
 * @param instance Module instance number
 * @param samplePnt selects when the data in (SIN) is sampled, of type dspi_master_sample_point_t.
 *                  This value selects either 0, 1, or 2 system clocks between the SCK edge
 *                  and the SIN (data in) sample.
 */
static inline void dspi_hal_set_datain_samplepoint(uint32_t instance,
                                                   dspi_master_sample_point_t samplePnt)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_SMPL_PT(instance, samplePnt);
}


/*!
 * @brief Starts the DSPI transfers, clears HALT bit in MCR.
 *
 * This function call called whenever the module is ready to begin data transfers in either master
 * or slave mode.
 *
 * @param instance Module instance number
 */
static inline void dspi_hal_start_transfer(uint32_t instance)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    HW_SPI_MCR_CLR(instance, BM_SPI_MCR_HALT);
}

/*!
 * @brief Stops (halts) DSPI transfers, sets HALT bit in MCR.
 *
 * This function call  stops data transfers in either master or slave mode.
 *
 * @param instance Module instance number
 */
static inline void dspi_hal_stop_transfer(uint32_t instance)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    HW_SPI_MCR_SET(instance, BM_SPI_MCR_HALT);
}

/*!
 * @brief Configures the data format for a particular CTAR.
 *
 * This function configures the bits-per-frame, polarity, phase, and shift direction for a
 * particular CTAR. An example use case is as follows:
   @code
    dspi_data_format_config_t dataFormat;
    dataFormat.bitsPerFrame = 16;
    dataFormat.clkPolarity = kDspiClockPolarity_ActiveLow;
    dataFormat.clkPhase = kDspiClockPhase_FirstEdge;
    dataFormat.direction = kDspiMsbFirst;
    dspi_hal_configure_data_format(instance, kDspiCtar0, &dataFormat);
   @endcode
 *
 * @param instance Module instance number
 * @param whichCtar The desired Clock and Transfer Attributes Register (CTAR) of type
 *                  dspi_ctar_selection_t.
 * @param config Pointer to a structure containing the user defined data format configuration settings.
 * @return  An error code or kStatus_DSPI_Success
 */
dspi_status_t dspi_hal_configure_data_format(uint32_t instance,
                                        dspi_ctar_selection_t whichCtar,
                                        const dspi_data_format_config_t * config);

/*!
 * @brief Configures the delays for a particular CTAR, master mode only.
 *
 * This function configures the PCS to SCK delay pre-scalar (PCSSCK),
 * the PCS to SCK Delay scalar (CSSCK),
 * the After SCK delay pre-scalar (PASC),
 * the After SCK delay scalar (ASC),
 * the Delay after transfer pre-scalar (PDT),
 * and the Delay after transfer scalar (DT).
 * The following is an example use case of this function:
 * @code
    dspi_delay_settings_config_t delayConfig;
    delayConfig.pcsToSckPre = 0x3;
    delayConfig.pcsToSck = 0xF;
    delayConfig.afterSckPre = 0x2;
    delayConfig.afterSck = 0xA;
    delayConfig.afterTransferPre = 0x1;
    delayConfig.afterTransfer = 0x5;
    dspi_hal_configure_delays(instance, kDspiCtar0, &delayConfig);
 * @endcode
 *
 * @param instance Module instance number
 * @param whichCtar The desired Clock and Transfer Attributes Register (CTAR) of type
 *                  dspi_ctar_selection_t.
 * @param config Pointer to a structure containing the user defined delay configuration settings.
 */
void dspi_hal_configure_delays(uint32_t instance,
                               dspi_ctar_selection_t whichCtar,
                               const dspi_delay_settings_config_t * config);

/*@}*/

/*!
 * @name DMA
 * @{
 */

/*!
 * @brief Configures transmit and receive DMA requests.
 *
 * This function configures the FIFOs to generate a DMA or an interrupt request. Note that the
 * corresponding request enable must also be set.  For the Transmit FIFO Fill, in order
 * to generate a DMA request, the Transmit FIFO Fill Request Enable (TFFF_RE) must also be set.
 * Similarly for the Receive FIFO Drain Request, to generate a DMA request, the Receive FIFO Drain
 * Request Enable (RFDF_RE) must also be set.  These requests can be configured with
 * the function dspi_hal_configure_interrupt().  To enable DMA operation, first enable
 * the desired request enable by using the dspi_hal_configure_interrupt() function and then use
 * the dspi_hal_configure_dma() to configure the request and generate a DMA request.
 *
 * @param enableTransmit Configures Tx FIFO fill request to generate a DMA or interrupt request
 * @param enableReceive Configures Rx FIFO fill request to generate a DMA or interrupt request
 */
void dspi_hal_configure_dma(uint32_t instance, bool enableTransmit, bool enableReceive);

/*@}*/

/*!
 * @name Low power
 * @{
 */

/*!
 * @brief Configures the DSPI operation during doze mode.
 *
 * This function provides support for an externally controlled doze mode, power-saving, mechanism.
 * When disabled, the doze mode has no effect on the DSPI, and when enabled, the Doze mode
 * disables the DSPI.
 *
 * @param instance Module instance number
 * @param enable If disabled (false), the doze mode has no effect on the DSPI, if enabled (true), the doze mode
 *                 disables the DSPI.
 */
static inline void dspi_hal_configure_doze_mode(uint32_t instance, bool enable)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_DOZE(instance, (enable == true));
}

/*@}*/

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Configures the DSPI interrupts.
 *
 * This function  configures the various interrupt sources of the DSPI.  The parameters are instance, interrupt source, and enable/disable setting.
 * The interrupt source is a typedef enum whose value is the bit position of the
 * interrupt source setting within the RSER register.  In the DSPI, all interrupt
 * configuration settings are in  one register.  The typedef enum  equates each
 * interrupt source to the bit position defined in the device header file.
 * The function  uses these bit positions in its algorithm to enable/disable the
 * interrupt source, where interrupt source is the dspi_status_and_interrupt_request_t type.
 * @code
    temp = (HW_SPI_RSER_RD(instance) & ~interruptSrc) | (enable << interruptSrc);
    HW_SPI_RSER_WR(instance, temp);

    dspi_hal_configure_interrupt(instance, kDspiTxComplete, true); <- example use-case
 * @endcode
 *
 * @param instance Module instance number
 * @param interruptSrc The interrupt source, of type dspi_status_and_interrupt_request_t
 * @param enable Enable (true) or disable (false) the interrupt source to generate requests
 */
void dspi_hal_configure_interrupt(uint32_t instance,
                              dspi_status_and_interrupt_request_t interruptSrc,
                              bool enable);


/*!
 * @brief Gets the DSPI interrupt configuration, returns if interrupt request is enabled or disabled.
 *
 * This function  returns the requested interrupt source setting (enabled or disabled, of
 * type bool).  The parameters to pass in are instance and interrupt source.  It  utilizes the
 * same enum definitions for the interrupt sources as described in the "interrupt configuration"
 * function. The function  uses these bit positions in its algorithm to obtain the desired
 * interrupt source setting.
 * @code
    return ((HW_SPI_RSER_RD(instance) & interruptSrc) >> interruptSrc);

   getInterruptSetting = dspi_hal_get_interrupt_config(instance, kDspiTxComplete);
 * @endcode
 *
 * @param instance Module instance number
 * @param interruptSrc The interrupt source, of type dspi_status_and_interrupt_request_t
 * @return Configuration of interrupt request: enable (true) or disable (false).
 */
static inline bool dspi_hal_get_interrupt_config(uint32_t instance,
                                             dspi_status_and_interrupt_request_t interruptSrc)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    return ((HW_SPI_RSER_RD(instance) >> interruptSrc) & 0x1);
}

/*@}*/

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the DSPI status flag state.
 *
 * The status flag  is defined in the same enum as the interrupt source enable because the bit
 * position of the interrupt source and corresponding status flag are the same in the RSER and
 * SR registers.  The function  uses these bit positions in its algorithm to obtain the desired
 * flag state, similar to the dspi_get_interrupt_config function.
 * @code
    return ((HW_SPI_SR_RD(instance) & statusFlag) >> statusFlag);

    getStatus = dspi_hal_get_status_flag(instance, kDspiTxComplete);
 * @endcode
 *
 * @param instance   Module instance number
 * @param statusFlag The status flag, of type dspi_status_and_interrupt_request_t
 * @return State of the status flag: asserted (true) or not-asserted (false)
 */
static inline bool dspi_hal_get_status_flag(uint32_t instance,
                                        dspi_status_and_interrupt_request_t statusFlag)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    return ((HW_SPI_SR_RD(instance) >> statusFlag) & 0x1);
}

/*!
 * @brief Clears the DSPI status flag.
 *
 * This function  clears the desired status bit by using a write-1-to-clear.  The user passes in
 * the instance and the desired status bit to clear.  The list of status bits is defined in the
 * dspi_status_and_interrupt_request_t.  The function  uses these bit positions in its algorithm
 * to clear the desired flag state. It uses  this macro:
 * @code
    HW_SPI_SR_WR(instance, statusFlag);

    dspi_hal_clear_status_flag(instance, kDspiTxComplete);
 * @endcode
 *
 * @param instance   Module instance number
 * @param statusFlag The status flag, of type dspi_status_and_interrupt_request_t
 */
static inline void dspi_hal_clear_status_flag(uint32_t instance,
                                              dspi_status_and_interrupt_request_t statusFlag)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    HW_SPI_SR_SET(instance, (0x1U << statusFlag));
}


/*!
 * @brief Gets the DSPI FIFO counter or pointer.
 *
 * This function  returns the number of entries or the next pointer in the Tx or Rx FIFO.
 * The parameters to pass in are the instance and either the Tx or Rx FIFO counter or a
 * pointer.  The latter  is an enum type defined as the bitmask of
 * those particular bit fields found in the device header file. For example:
 * @code
    return ((HW_SPI_SR_RD(instance) >> desiredParamter) & 0xF);

    dspi_hal_get_fifo_counter_or_pointer(instance, kDspiRxFifoCounter);
 * @endcode
 *
 * @param instance         Module instance number
 * @param desiredParameter Desired parameter to obtain, of type dspi_fifo_counter_pointer_t
 */
static inline uint32_t dspi_hal_get_fifo_counter_or_pointer(uint32_t instance,
                                              dspi_fifo_counter_pointer_t desiredParameter)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    return ((HW_SPI_SR_RD(instance) >> desiredParameter) & 0xFU);
}


/*@}*/

/*!
 * @name Data transfer
 * @{
 */

/*!
 * @brief Reads data from the data buffer.
 *
 * @param instance Module instance number
 */
static inline uint32_t dspi_hal_read_data(uint32_t instance)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    return HW_SPI_POPR_RD(instance);
}

/*!
 * @brief Writes data into the data buffer, slave mode.
 *
 * In slave mode, up to 32-bit words may be written.
 *
 * @param instance Module instance number
 * @param data The data to send
 */
static inline void dspi_hal_write_data_slave_mode(uint32_t instance, uint32_t data)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    HW_SPI_PUSHR_SLAVE_WR(instance, data);
}

/*!
 * @brief Writes data into the data buffer, master mode.
 *
 * In master mode, the 16-bit data is appended to the 16-bit command info. The command portion
 * provides characteristics of the data such as: optional continuous chip select
 * operation between transfers, the desired Clock and Transfer Attributes register to use for the
 * associated SPI frame, the desired PCS signal to use for the data transfer, whether the current
 * transfer is the last in the queue, and whether to clear the transfer count (normally needed when
 * sending the first frame of a data packet). This is an example:
 * @code
    dspi_command_config_t commandConfig;
    commandConfig.isChipSelectContinuous = true;
    commandConfig.whichCtar = kDspiCtar0;
    commandConfig.whichPcs = kDspiPcs1;
    commandConfig.clearTransferCount = false;
    commandConfig.isEndOfQueue = false;
    dspi_hal_write_data_master_mode(instance, &commandConfig, dataWord);
 * endcode
 *
 * @param instance Module instance number
 * @param command Pointer to command structure
 * @param data The data word to be sent
 */
void dspi_hal_write_data_master_mode(uint32_t instance,
                                     dspi_command_config_t * command,
                                     uint16_t data);

/*!
 * @brief Gets the transfer count.
 *
 * This function returns the current value of the DSPI Transfer Count Register.
 *
 * @param instance Module instance number
 * @return The current transfer count
 */
static inline uint32_t dspi_hal_get_transfer_count(uint32_t instance)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    return BR_SPI_TCR_SPI_TCNT(instance);
}

/*!
 * @brief Pre-sets the transfer count.
 *
 * This function allows the caller to pre-set the DSI Transfer Count Register to a desired value up
 * to 65535; Incrementing past this resets the counter back to 0.
 *
 * @param instance Module instance number
 * @param presetValue The desired pre-set value for the transfer counter
 */
static inline void dspi_hal_preset_transfer_count(uint32_t instance, uint16_t presetValue)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_TCR_SPI_TCNT(instance, presetValue);
}

/*@}*/

/*!
 * @name Debug
 * @{
 */

/*!
 * @brief Read FIFO registers for debug purposes.
 *
 * @param instance Module instance number
 * @param whichFifo Selects Tx or Rx FIFO, of type dspi_fifo_t.
 * @param whichFifoEntry Selects which FIFO entry to read: 0, 1, 2, or 3.
 * @retrun The desired FIFO register contents
 */
uint32_t dspi_hal_get_fifo_data(uint32_t instance, dspi_fifo_t whichFifo, uint32_t whichFifoEntry);

/*!
 * @brief Configures the DSPI to halt during debug mode.
 *
 * @param instance Module instance number
 * @param enable Enables (true) debug mode to halt transfers, else disable to not halt transfer
 *               in debug mode.
 */
static inline void dspi_hal_configure_halt_in_debug_mode(uint32_t instance, bool enable)
{
    assert(instance < HW_SPI_INSTANCE_COUNT);
    BW_SPI_MCR_FRZ(instance, (enable == true));
}

/* @}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_DSPI_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

