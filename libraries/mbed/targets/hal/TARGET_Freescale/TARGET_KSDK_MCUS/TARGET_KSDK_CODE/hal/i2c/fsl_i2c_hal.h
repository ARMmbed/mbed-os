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
#if !defined(__FSL_I2C_HAL_H__)
#define __FSL_I2C_HAL_H__

#include <assert.h>
#include <stdbool.h>
#include "fsl_i2c_features.h"
#include "fsl_device_registers.h"

/*!
 * @addtogroup i2c_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief I2C status return codes*/
typedef enum _i2c_status
{
    kStatus_I2C_Success = 0,
    kStatus_I2C_OutOfRange,
    kStatus_I2C_InvalidArgument,
    kStatus_I2C_Fail,
    kStatus_I2C_Busy,                /*!< The master is already performing a transfer.*/
    kStatus_I2C_Timeout,             /*!< The transfer timed out.*/
    kStatus_I2C_ReceivedNak,         /*!< The slave device sent a NAK in response to a byte.*/
    kStatus_I2C_SlaveTxUnderrun,     /*!< I2C Slave TX Underrun error.*/
    kStatus_I2C_SlaveRxOverrun,      /*!< I2C Slave RX Overrun error.*/
    kStatus_I2C_AribtrationLost,     /*!< I2C Arbitration Lost error.*/
} i2c_status_t;

/*! @brief Direction of master and slave transfers*/
typedef enum _i2c_transmit_receive_mode {
    kI2CReceive = 0,    /*!< Master and slave receive.*/
    kI2CTransmit = 1    /*!< Master and slave transmit.*/
} i2c_transmit_receive_mode_t;

/*!
 * @brief I2C module configuration
 *
 * Pass an instance of this structure  to the i2c_hal_init() to configure the entire I2C peripheral
 * in a single function call.
 */
typedef struct I2CConfig {
    bool enableModule;              /*!< Whether the I2C peripheral operation is enabled.*/
    uint32_t baudRate_kbps;         /*!< Requested baud rate in kilobits per second, for example, 100 or 400. Pass zero to not set the baud rate.*/
    bool useIndependentSlaveBaud;   /*!< Enables independent slave mode baud rate at maximum frequency.*/
    bool enableInterrupt;           /*!< Enable for the I2C interrupt.*/
    bool enableDma;                 /*!< Enable DMA transfer signalling.*/
    bool enableHighDrive;           /*!< Enable high drive pin mode.*/
    bool enableWakeup;              /*!< Enable low power wakeup.*/
    uint8_t glitchFilterWidth;      /*!< Specify the glitch filter width in terms of bus clock cycles. Set this value to zero to disable the glitch filter.*/
    uint16_t slaveAddress;          /*!< 7-bit or 10-bit slave address.*/
    uint8_t upperSlaveAddress;      /*!< 7-bit upper slave address, or zero to disable. If 10-bit addresses are enabled, the top 3 bits are provided by the @a slaveAddress field.*/
    bool use10bitSlaveAddress;      /*!< Controls whether 10-bit slave addresses are enabled.*/
    bool enableGeneralCallAddress;  /*!< Enable general call address matching.*/
    bool enableRangeAddressMatch;   /*!< Determines if addresses between @a slaveAddress and @a upperSlaveAddress are matched. Both of those fields must be non-zero.*/

#if FSL_FEATURE_I2C_HAS_STOP_DETECT
    bool enableStopModeHoldoff;     /*!< Option to hold off CPU low power stop mode until the current byte transfer is complete.*/
    bool enableBusStopInterrupt;    /*!< Enable for the bus stop detection interrupt.*/
#endif /* FSL_FEATURE_I2C_HAS_STOP_DETECT*/
} i2c_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! @name Module controls*/
/*@{*/

/*!
 * @brief Initializes and configures the I2C peripheral.
 *
 * @param instance The I2C peripheral instance number
 * @param config Pointer to the configuration settings
 * @param sourceClockInHz I2C source input clock in Hertz.
 */
void i2c_hal_init(uint32_t instance, const i2c_config_t * config, uint32_t sourceClockInHz);

/*!
 * @brief Restores the I2C peripheral to reset state.
 *
 * @param instance The I2C peripheral instance number
 */
void i2c_hal_reset(uint32_t instance);

/*!
 * @brief Enables the I2C module operation.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_enable(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_C1_SET(instance, BM_I2C_C1_IICEN);
}

/*!
 * @brief Disables the I2C module operation.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_disable(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_C1_CLR(instance, BM_I2C_C1_IICEN);
}

/*@}*/

/*! @name DMA*/
/*@{*/

/*!
 * @brief Enables or disables the DMA support.
 *
 * @param instance The I2C peripheral instance number
 * @param enable Pass true to enable DMA transfer signalling
 */
static inline void i2c_hal_set_dma_enable(uint32_t instance, bool enable)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_C1_DMAEN(instance, (uint8_t)enable);
}

/*@}*/

/*! @name Pin functions*/
/*@{*/

/*!
 * @brief Controls the drive capability of the I2C pads.
 *
 * @param instance The I2C peripheral instance number
 * @param enable Passing true will enable high drive mode of the I2C pads. False sets normal
 *     drive mode.
 */
static inline void i2c_hal_set_high_drive(uint32_t instance, bool enable)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_C2_HDRS(instance, (uint8_t)enable);
}

/*!
 * @brief Controls the width of the programmable glitch filter.
 *
 * Controls the width of the glitch, in terms of bus clock cycles, that the filter must absorb.
 * The filter does not allow any glitch whose size is less than or equal to this width setting, 
 * to pass.
 *
 * @param instance The I2C peripheral instance number
 * @param glitchWidth Maximum width in bus clock cycles of the glitches that is filtered.
 *     Pass zero to disable the glitch filter.
 */
static inline void i2c_hal_set_glitch_filter(uint32_t instance, uint8_t glitchWidth)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_FLT_FLT(instance, glitchWidth);
}

/*@}*/

/*! @name Low power*/
/*@{*/

/*!
 * @brief Controls the I2C wakeup enable.
 *
 * The I2C module can wake the MCU from low power mode with no peripheral bus running when
 * slave address matching occurs. 
 *
 * @param instance The I2C peripheral instance number.
 * @param enable true - Enables the wakeup function in low power mode.<br>
 *     false - Normal operation. No interrupt is  generated when address matching in
 *     low power mode.
 */
static inline void i2c_hal_set_wakeup_enable(uint32_t instance, bool enable)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_C1_WUEN(instance, (uint8_t)enable);
}

#if FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF
/*!
 * @brief Controls the stop mode hold off.
 *
 * This function lets you enable the hold off entry to low power stop mode when any data transmission
 * or reception is occurring.
 *
 * @param instance The I2C peripheral instance number
 * @param enable false - Stop hold off is disabled. The MCU's entry to stop mode is not gated.<br>
 *     true - Stop hold off is enabled.
 */

static inline void i2c_hal_set_stop_holdoff(uint32_t instance, bool enable)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_FLT_SHEN(instance, (uint8_t)enable);
}
#endif /* FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF*/

/*@}*/

/*! @name Baud rate*/
/*@{*/

/*!
 * brief Returns the maximum supported baud rate in kilohertz.
 *
 * @param   instance        The I2C peripheral instance number
 * @param   sourceClockInHz I2C source input clock in Hertz
 * @return The maximum baud rate in kilohertz
 */
uint32_t i2c_hal_get_max_baud(uint32_t instance, uint32_t sourceClockInHz);

/*!
 * @brief Sets the I2C bus frequency for master transactions.
 *
 * @param instance The I2C peripheral instance number
 * @param sourceClockInHz I2C source input clock in Hertz
 * @param kbps Requested bus frequency in kilohertz. Common values are either 100 or 400.
 * @param absoluteError_Hz If this parameter is not NULL, it is filled in with the
 *     difference in Hertz between the requested bus frequency and the closest frequency
 *     possible given available divider values.
 *
 * @retval kStatus_Success The baud rate was changed successfully. However, there is no
 *      guarantee on the minimum error. If you want to ensure that the baud was set to within
 *      a certain error, then use the @a absoluteError_Hz parameter.
 * @retval kStatus_OutOfRange The requested baud rate was not within the range of rates
 *      supported by the peripheral.
 */
i2c_status_t i2c_hal_set_baud(uint32_t instance, uint32_t sourceClockInHz, uint32_t kbps,
                                  uint32_t * absoluteError_Hz);

/*!
 * @brief Sets the I2C baud rate multiplier and table entry.
 *
 * Use this function to set the I2C bus frequency register values directly, if they are
 * known in advance.
 *
 * @param instance The I2C peripheral instance number
 * @param mult Value of the MULT bitfield, ranging from 0-2. 
 * @param icr The ICR bitfield value, which is the index into an internal table in the I2C
 *     hardware that selects the baud rate divisor and SCL hold time.
 */
static inline void i2c_hal_set_baud_icr(uint32_t instance, uint8_t mult, uint8_t icr)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_F_WR(instance, BF_I2C_F_MULT(mult) | BF_I2C_F_ICR(icr));
}

/*!
 * @brief Slave baud rate control
 *
 * Enables an independent slave mode baud rate at the maximum frequency. This forces clock stretching
 * on the SCL in very fast I2C modes.
 *
 * @param instance The I2C peripheral instance number
 * @param enable true - Slave baud rate is independent of the master baud rate;<br>
 *     false - The slave baud rate follows the master baud rate and clock stretching may occur.
 */
static inline void i2c_hal_set_independent_slave_baud(uint32_t instance, bool enable)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_C2_SBRC(instance, (uint8_t)enable);
}

/*@}*/

/*! @name Bus operations*/
/*@{*/

/*!
 * @brief Sends a START or a Repeated START signal on the I2C bus.
 *
 * This function is used to initiate a new master mode transfer by sending the START signal. It
 * is also used to send a Repeated START signal when a transfer is already in progress.
 *
 * @param instance The I2C peripheral instance number
 */
void i2c_hal_send_start(uint32_t instance);

/*!
 * @brief Sends a STOP signal on the I2C bus.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_send_stop(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    assert(HW_I2C_C1(instance).B.MST == 1);
    HW_I2C_C1_CLR(instance, BM_I2C_C1_MST | BM_I2C_C1_TX);
}

/*!
 * @brief Selects either transmit or receive modes.
 *
 * @param instance The I2C peripheral instance number
 * @param mode Specifies either transmit mode or receive mode. The valid values are:
 *     - #kI2CTransmit
 *     - #kI2CReceive
 */
static inline void i2c_hal_set_direction(uint32_t instance, i2c_transmit_receive_mode_t mode)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_C1_TX(instance, (uint8_t)mode);
}

/*!
 * @brief Returns the currently selected transmit or receive mode.
 *
 * @param instance The I2C peripheral instance number
 * @retval #kI2CTransmit I2C is configured for master or slave transmit mode.
 * @retval #kI2CReceive I2C is configured for master or slave receive mode.
 */
static inline i2c_transmit_receive_mode_t i2c_hal_get_direction(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return HW_I2C_C1(instance).B.TX ? kI2CTransmit : kI2CReceive;
}

/*!
 * @brief Causes an ACK to be sent on the bus.
 *
 * This function specifies that an ACK signal is sent in response to the next received byte.
 *
 * Note that the behavior of this function is changed when the I2C peripheral is placed in
 * Fast ACK mode. In this case, this function causes an ACK signal to be sent in
 * response to the current byte, rather than the next received byte.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_send_ack(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_C1_CLR(instance, BM_I2C_C1_TXAK);
}

/*!
 * @brief Causes a NAK to be sent on the bus.
 *
 * This function specifies that a NAK signal is sent in response to the next received byte.
 *
 * Note that the behavior of this function is changed when the I2C peripheral is placed in the
 * Fast ACK mode. In this case, this function causes an NAK signal to be sent in
 * response to the current byte, rather than the next received byte.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_send_nak(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_C1_SET(instance, BM_I2C_C1_TXAK);
}

/*@}*/

/*! @name Data transfer*/
/*@{*/

/*!
 * @brief Returns the last byte of data read from the bus and initiate another read.
 *
 * In a master receive mode, calling this function initiates receiving  the next byte of data.
 *
 * @param instance The I2C peripheral instance number
 * @return This function returns the last byte received while the I2C module is configured in master
 *     receive or slave receive mode.
 */
static inline uint8_t i2c_hal_read(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return HW_I2C_D_RD(instance);
}

/*!
 * @brief Writes one byte of data to the I2C bus.
 *
 * When this function is called in the master transmit mode, a data transfer is initiated. In slave
 * mode, the same function is available after an address match occurs.
 *
 * In a master transmit mode, the first byte of data written following the start bit or repeated
 * start bit is used for the address transfer and must consist of the slave address (in bits 7-1)
 * concatenated with the required R/\#W bit (in position bit 0).
 *
 * @param instance The I2C peripheral instance number
 * @param data The byte of data to transmit
 */
static inline void i2c_hal_write(uint32_t instance, uint8_t data)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_D_WR(instance, data);
}

/*@}*/

/*! @name Slave address*/
/*@{*/

/*!
 * @brief Sets the primary 7-bit slave address.
 *
 * @param instance The I2C peripheral instance number
 * @param address The slave address in the upper 7 bits. Bit 0 of this value must be 0.
 */
void i2c_hal_set_slave_address_7bit(uint32_t instance, uint8_t address);

/*!
 * @brief Sets the primary slave address and enables 10-bit address mode.
 *
 * @param instance The I2C peripheral instance number
 * @param address The 10-bit slave address, in bits [10:1] of the value. Bit 0 must be 0.
 */
void i2c_hal_set_slave_address_10bit(uint32_t instance, uint16_t address);

/*!
 * @brief Controls whether the general call address is recognized.
 *
 * @param instance The I2C peripheral instance number
 * @param enable Whether to enable the general call address.
 */
static inline void i2c_hal_set_general_call_enable(uint32_t instance, bool enable)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_C2_GCAEN(instance, (uint8_t)enable);
}

/*!
 * @brief Enables or disables the slave address range matching.
 *
 * @param instance The I2C peripheral instance number
 * @param enable Pass true to enable the range address matching. You must also call the
 *     i2c_hal_set_upper_slave_address_7bit() to set the upper address.
 */
static inline void i2c_hal_set_slave_range_address_enable(uint32_t instance, bool enable)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    BW_I2C_C2_RMEN(instance, (uint8_t)enable);
}

/*!
 * @brief Sets the upper slave address.
 *
 * This slave address is used as a secondary slave address. If range address
 * matching is enabled, this slave address acts as the upper bound on the slave address
 * range.
 *
 * This function sets only a 7-bit slave address. If 10-bit addressing was enabled by calling the
 * i2c_hal_set_slave_address_10bit(), then the top 3 bits set with that function are also used
 * with the address set with this function to form a 10-bit address.
 *
 * Passing 0 for the @a address parameter  disables  matching the upper slave address.
 *
 * @param instance The I2C peripheral instance number
 * @param address The upper slave address in the upper 7 bits. Bit 0 of this value must be 0.
 *     This address must be greater than the primary slave address that is set by
 *     calling the i2c_hal_set_slave_address_7bit().
 */
static inline void i2c_hal_set_upper_slave_address_7bit(uint32_t instance, uint8_t address)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    assert((address & 1) == 0);
    assert((address == 0) || (address > HW_I2C_A1_RD(instance)));
    HW_I2C_RA_WR(instance, address);
}

/*@}*/

/*! @name Status*/
/*@{*/

/*!
 * @brief Returns whether the I2C module is in master mode.
 *
 * @param instance The I2C peripheral instance number
 * @retval true The module is in master mode, which implies it is also performing a transfer.
 * @retval false The module is in slave mode.
 */
static inline bool i2c_hal_is_master(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_C1(instance).B.MST;
}

/*!
 * @brief Gets the transfer complete flag.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Transfer is complete.
 * @retval false Transfer is in progress.
 */
static inline bool i2c_hal_is_transfer_complete(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_S(instance).B.TCF;
}

/*!
 * @brief Returns whether the I2C slave was addressed.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Addressed as slave.
 * @retval false Not addressed.
 */
static inline bool i2c_hal_is_addressed_as_slave(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_S(instance).B.IAAS;
}

/*!
 * @brief Determines whether the I2C bus is busy.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Bus is busy.
 * @retval false Bus is idle.
 */
static inline bool i2c_hal_is_bus_busy(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_S(instance).B.BUSY;
}

/*!
 * @brief Returns whether the arbitration procedure was lost.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Loss of arbitration
 * @retval false Standard bus operation
 */
static inline bool i2c_hal_was_arbitration_lost(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_S(instance).B.ARBL;
}

/*!
 * @brief Clears the arbitration lost flag.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_clear_arbitration_lost(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_S_WR(instance, BM_I2C_S_ARBL);
}

/*!
 * @brief Get the range address match flag.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Addressed as slave.
 * @retval false Not addressed.
 */
static inline bool i2c_hal_is_range_address_match(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_S(instance).B.RAM;
}

/*!
 * @brief Returns whether the I2C slave was addressed in read or write mode.
 *
 * @param instance The I2C peripheral instance number
 * @retval #kI2CReceive Slave receive, master writing to slave
 * @retval #kI2CTransmit Slave transmit, master reading from slave
 */
static inline i2c_transmit_receive_mode_t i2c_hal_get_slave_direction(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return HW_I2C_S(instance).B.SRW ? kI2CTransmit : kI2CReceive;
}

/*!
 * @brief Returns whether an ACK was received after the last byte was transmitted.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Acknowledges that the signal was received after the completion of one byte of data
 *     transmission on the bus.
 * @retval false No acknowledgement of the signal is detected.
 */
static inline bool i2c_hal_get_receive_ack(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)!(HW_I2C_S(instance).B.RXAK);
}

/*@}*/

/*! @name Interrupt*/
/*@{*/

/*!
 * @brief Enables the I2C interrupt requests.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_enable_interrupt(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_C1_SET(instance, BM_I2C_C1_IICIE);
}

/*!
 * @brief Disables the I2C interrupt requests.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_disable_interrupt(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_C1_CLR(instance, BM_I2C_C1_IICIE);
}

/*!
 * @brief Returns whether the I2C interrupts are enabled.
 *
 * @param instance The I2C peripheral instance number
 * @retval true I2C interrupts are enabled.
 * @retval false I2C interrupts are disabled.
 */
static inline bool i2c_hal_is_interrupt_enabled(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_C1(instance).B.IICIE;
}

/*!
 * @brief Returns the current I2C interrupt flag.
 *
 * @param instance The I2C peripheral instance number
 * @retval true An interrupt is pending.
 * @retval false No interrupt is pending.
 */
static inline bool i2c_hal_get_interrupt_status(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_S(instance).B.IICIF;
}

/*!
 * @brief Clears the I2C interrupt if set.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_clear_interrupt(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_S_SET(instance, BM_I2C_S_IICIF);
}

/*@}*/

#if FSL_FEATURE_I2C_HAS_STOP_DETECT

/*! @name Bus stop detection status*/
/*@{*/

/*!
 * @brief Gets the flag indicating a STOP signal was detected on the I2C bus.
 *
 * @param instance The I2C peripheral instance number
 * @retval true STOP signal detected on bus.
 * @retval false No STOP signal was detected on the bus.
 */
static inline bool i2c_hal_get_stop_detect(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_FLT(instance).B.STOPF;
}

/*!
 * @brief Clears the bus STOP signal detected flag.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_clear_stop_detect(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_FLT_SET(instance, BM_I2C_FLT_STOPF);
}

/*@}*/
#if FSL_FEATURE_I2C_HAS_START_DETECT

/*! @name Bus stop detection interrupt*/
/*@{*/

/*!
 * @brief Enables the I2C bus stop detection interrupt.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_enable_bus_stop_interrupt(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_FLT_SET(instance, BM_I2C_FLT_SSIE);
}

/*!
 * @brief Disables the I2C bus stop detection interrupt.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_disable_bus_stop_interrupt(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_FLT_CLR(instance, BM_I2C_FLT_SSIE);
}

/*!
 * @brief Returns whether  the I2C bus stop detection interrupts are enabled.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Stop detect interrupts are enabled.
 * @retval false Stop detect interrupts are disabled.
 */
static inline bool i2c_hal_is_bus_stop_interrupt_enabled(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_FLT(instance).B.SSIE;
}

#else

/*! @name Bus stop detection interrupt*/
/*@{*/

/*!
 * @brief Enables the I2C bus stop detection interrupt.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_enable_bus_stop_interrupt(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_FLT_SET(instance, BM_I2C_FLT_STOPIE);
}

/*!
 * @brief Disables the I2C bus stop detection interrupt.
 *
 * @param instance The I2C peripheral instance number
 */
static inline void i2c_hal_disable_bus_stop_interrupt(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    HW_I2C_FLT_CLR(instance, BM_I2C_FLT_STOPIE);
}

/*!
 * @brief Returns whether the I2C bus stop detection interrupts are enabled.
 *
 * @param instance The I2C peripheral instance number
 * @retval true Stop detect interrupts are enabled.
 * @retval false Stop detect interrupts are disabled.
 */
static inline bool i2c_hal_is_bus_stop_interrupt_enabled(uint32_t instance)
{
    assert(instance < HW_I2C_INSTANCE_COUNT);
    return (bool)HW_I2C_FLT(instance).B.STOPIE;
}

#endif  /* FSL_FEATURE_I2C_HAS_START_DETECT*/

/*@}*/
#endif /* FSL_FEATURE_I2C_HAS_STOP_DETECT*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_I2C_HAL_H__*/
/*******************************************************************************
 * EOF
 ******************************************************************************/

