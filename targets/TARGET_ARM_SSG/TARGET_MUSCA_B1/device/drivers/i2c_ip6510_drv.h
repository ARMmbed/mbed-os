/*
 * Copyright (c) 2018-2020 Arm Limited
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

/**
 * \file i2c_ip6510_drv.h
 * \brief Driver for IP6510 I2C controller.
 */

#ifndef __I2C_IP6510_DRV_H__
#define __I2C_IP6510_DRV_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief I2C (IP6510) device state types.
 */
enum i2c_ip6510_state_t {
    I2C_IP6510_UNINITIALIZED    = 0u,
    I2C_IP6510_INITIALIZED      = 1u,
};

/**
 * \brief I2C (IP6510) bus state types.
 */
enum i2c_ip6510_bus_state_t {
    I2C_IP6510_BUS_INACTIVE = 0u,
    I2C_IP6510_BUS_ACTIVE   = 1u,
};

/**
 * \brief Allowed transfer direction options.
 */
enum i2c_ip6510_transf_dir_t {
    I2C_IP6510_TRANSMITTER    = 0u,
    I2C_IP6510_RECEIVER       = 1u,
};

/**
 * \brief Supported I2C (IP6510) device mode options.
 */
enum i2c_ip6510_device_mode_t {
    I2C_IP6510_SLAVE_MODE   = 0u,
    I2C_IP6510_MASTER_MODE  = 1u,
};

/**
 * \brief Supported I2C (IP6510) addressing options.
 */
enum i2c_ip6510_addr_mode_t {
    I2C_IP6510_EXT_ADDR_MODE    = 0u,
    I2C_IP6510_NOR_ADDR_MODE    = 1u,
};

/**
 * \brief Supported I2C (IP6510) bus data rate options.
 */
enum i2c_ip6510_speed_t {
    I2C_IP6510_SPEED_100KHZ         = 0u,
    I2C_IP6510_SPEED_400KHZ         = 1u,
    I2C_IP6510_SPEED_MAX_SUPPORTED  = 2u,
};

#define I2C_IP6510_INTR_COMP_OFF        (0u)
        /*!< Transfer complete interrupt bit field offset */
#define I2C_IP6510_INTR_DATA_OFF        (1u)
        /*!< More data interrupt bit field offset */
#define I2C_IP6510_INTR_NACK_OFF        (2u)
        /*!< Transfer not ACKed interrupt bit field offset */
#define I2C_IP6510_INTR_TO_OFF          (3u)
        /*!< Transfer time out interrupt bit field offset */
#define I2C_IP6510_INTR_SLVRDY_OFF      (4u)
        /*!< Monitored slave ready interrupt bit field offset */
#define I2C_IP6510_INTR_RXOVF_OFF       (5u)
        /*!< Receive overflow interrupt bit field offset */
#define I2C_IP6510_INTR_TXOVF_OFF       (6u)
        /*!< FIFO transmit overflow interrupt bit field offset */
#define I2C_IP6510_INTR_RXUNF_OFF       (7u)
        /*!< FIFO receive underflow interrupt bit field offset */
#define I2C_IP6510_INTR_ARBLOST_OFF     (9u)
        /*!< Arbitration lost interrupt bit field offset */

#define I2C_IP6510_ALL_INTR_MASK        (0x2FFu)

/**
 * \brief I2C (IP6510) interrupt data structure
 */
enum i2c_ip6510_intr_t {
    I2C_IP6510_INTR_COMP_MASK       = (0x1u<<I2C_IP6510_INTR_COMP_OFF),
    I2C_IP6510_INTR_DATA_MASK       = (0x1u<<I2C_IP6510_INTR_DATA_OFF),
    I2C_IP6510_INTR_NACK_MASK       = (0x1u<<I2C_IP6510_INTR_NACK_OFF),
    I2C_IP6510_INTR_TO_MASK         = (0x1u<<I2C_IP6510_INTR_TO_OFF),
    I2C_IP6510_INTR_SLVRDY_MASK     = (0x1u<<I2C_IP6510_INTR_SLVRDY_OFF),
    I2C_IP6510_INTR_RXOVF_MASK      = (0x1u<<I2C_IP6510_INTR_RXOVF_OFF),
    I2C_IP6510_INTR_TXOVF_MASK      = (0x1u<<I2C_IP6510_INTR_TXOVF_OFF),
    I2C_IP6510_INTR_RXUNF_MASK      = (0x1u<<I2C_IP6510_INTR_RXUNF_OFF),
    I2C_IP6510_INTR_ARBLOST_MASK    = (0x1u<<I2C_IP6510_INTR_ARBLOST_OFF),
};

/**
 * \brief I2C (IP6510) error enumeration types.
 */
enum i2c_ip6510_error_t {
    I2C_IP6510_ERR_NONE         = 0u,
    I2C_IP6510_ERR_NOT_INIT     = 1u,
    I2C_IP6510_ERR_ALREADY_INIT = 2u,
    I2C_IP6510_ERR_INVALID_ARG  = 3u,
    I2C_IP6510_ERR_NACK         = 4u,
    I2C_IP6510_ERR_RXOVF        = 5u,
    I2C_IP6510_ERR_TXOVF        = 6u,
    I2C_IP6510_ERR_RXUNF        = 7u,
    I2C_IP6510_ERR_TIMEOUT      = 8u,
    I2C_IP6510_ERR              = 9u,
};

/**
 *  \brief I2C (IP6510) device configuration structure.
 */
struct i2c_ip6510_dev_cfg_t {
    const uint32_t base;
            /*!< I2C device base address */
    const enum i2c_ip6510_speed_t default_bus_speed;
            /*!< I2C bus data rate */
    const enum i2c_ip6510_device_mode_t default_mode;
            /*!< I2C device mode (Master/Slave) */
};

/**
 * \brief I2C (IP6510) device data structure.
 */
struct i2c_ip6510_dev_data_t {
    enum i2c_ip6510_state_t state;      /*!< I2C device state */
    enum i2c_ip6510_device_mode_t mode; /*!< I2C device mode (Master/Slave) */
    enum i2c_ip6510_speed_t bus_speed;  /*!< I2C bus operational data rate */
    uint32_t sys_clk;                   /*!< System clock frequency */
};

/**
 * \brief I2C (IP6510) device structure.
 */
struct i2c_ip6510_dev_t {
    const struct i2c_ip6510_dev_cfg_t* const cfg;
        /*!< I2C (IP6510) configuration structure */
    struct i2c_ip6510_dev_data_t* const data;
        /*!< I2C (IP6510) data structure */
};

/**
 * \brief Initializes I2C (IP6510) device.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] sys_clk   System clock frequency used by the device.
 *
 * It configures the interface as I2C Master with the default data rate
 * and disables every I2C interrupts.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note This API should be called before calling any of the below I2C APIs.
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_init(struct i2c_ip6510_dev_t* dev,
                                uint32_t sys_clk);

/**
 * \brief Uninitializes I2C (IP6510) device and resets registers.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_uninit(struct i2c_ip6510_dev_t* dev);

/**
 * \brief Enables Hold mode.
 *
 * Sets hold bit, so the master does not terminate automatically a read or
 * write transfer.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_hold_enable(const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Disables Hold mode.
 *
 * Resets hold bit, so the master can terminate automatically a read or
 * write transfer.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_hold_disable(const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Returns the I2C (IP6510) device operational state.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Returns the I2C (IP6510) device state \ref i2c_ip6510_state_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_state_t i2c_ip6510_get_state(
                                const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Returns the I2C interface mode.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Returns the I2C interface mode \ref i2c_ip6510_device_mode_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_device_mode_t i2c_ip6510_get_device_mode(
                                const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Sets the I2C data rate.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] speed     New I2C data rate \ref i2c_ip6510_speed_t
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note The I2C device should be in valid state before calling this API.
 *         \ref i2c_ip6510_get_state should return I2C_IP6510_INITIALIZED.
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_set_speed(struct i2c_ip6510_dev_t* dev,
                                enum i2c_ip6510_speed_t speed);

/**
 * \brief Returns the actual I2C data rate.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Returns the actual I2C data rate \ref i2c_ip6510_speed_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_speed_t i2c_ip6510_get_speed(
                                const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Returns the state of the I2C bus.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Returns the I2C bus state \ref i2c_ip6510_bus_state_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_bus_state_t i2c_ip6510_get_bus_status(
                                const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Returns the transfer size registers value.
 *
 * Transfer size registers value contains:
 * Master transmitter mode: number of data bytes still not transmitted
 * minus one.
 * Master receiver mode: number of data bytes that are still expected
 * to be received.
 * Slave transmitter mode: number of bytes remaining in the FIFO after
 * the master terminates the transfer.
 * Slave receiver mode: number of valid data bytes in the FIFO.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Returns the transfer size registers value.
 *
 * \note This function doesn't check if dev is NULL.
 */
int i2c_ip6510_get_transfer_size(const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Returns the I2C interrupt status.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return The returned status value can be checked against values from
 *         the \ref i2c_ip6510_intr_t type.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t i2c_ip6510_get_irq_status(const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Returns the I2C interrupt mask status.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return The returned mask value can be checked against values from
 *         the \ref i2c_ip6510_intr_t type.
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t i2c_ip6510_get_irq_mask(const struct i2c_ip6510_dev_t* dev);

/**
 * \brief Clears the specified I2C interrupts.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] mask      Bit mask for clearing interrupts. Values from
 *                      \ref i2c_ip6510_intr_t could be used to create the mask.
 *
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_clear_irq(struct i2c_ip6510_dev_t* dev, uint32_t mask);

/**
 * \brief Enables the specified I2C interrupts.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] mask      Bit mask for enabling interrupts. Values from
 *                      \ref i2c_ip6510_intr_t could be used to create the mask.
 *
 * \note User is responsible to configure the interrupt vector and
 *       the interrupt controller.
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_enable_irq(struct i2c_ip6510_dev_t* dev, uint32_t mask);

/**
 * \brief Disables I2C interrupts.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] mask      Bit mask for disabling interrupts. Values from
 *                      \ref i2c_ip6510_intr_t could be used to create the mask.
 *
 * \note User is responsible to configure the interrupt vector and
 *       the interrupt controller.
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_disable_irq(struct i2c_ip6510_dev_t* dev, uint32_t mask);

/**
 * \brief Sets the time out interval.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] interval  New time out interval in number of scl cycles minus one.
 *
 * \note The default time out interval is 0x20.
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_set_timeout(struct i2c_ip6510_dev_t* dev,
                                uint8_t interval);

/**
 * \brief Sets the pause interval in slave monitor mode.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] interval  New pause interval in number of scl cycles minus one.
 *
 * \note The default pause interval is 0 (continuous monitoring).
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_set_slave_monitor_pause_interval(struct i2c_ip6510_dev_t* dev,
                            uint8_t interval);

/**
 * \brief Returns the pause interval of slave monitor mode.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Returns the pause interval of slave monitor mode
 *         in number of scl cycles minus one.
 *
 * \note The default pause interval is 0 (continuous monitoring).
 * \note This function doesn't check if dev is NULL.
 */
uint8_t i2c_ip6510_get_slave_monitor_pause_interval(
                            struct i2c_ip6510_dev_t* dev);

/**
 * \brief Sets the length of the glitch filter.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] length    New length of the glitch filter [0(min) ... 15(max)].
 *
 * \note If length of glitch filter is set to 0 then the glitch filter is
 *       bypassed. The default glitch filter length is 0.
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_set_glitch_filter_length(struct i2c_ip6510_dev_t* dev,
                                uint32_t length);

/**
 * \brief Returns the length of the glitch filter.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Returns the actual length of the glitch filter.
 *
 * \note The default glitch filter length is 0.
 * \note This function doesn't check if dev is NULL.
 */
uint8_t i2c_ip6510_get_glitch_filter_length(const struct i2c_ip6510_dev_t* dev);

/**
 * \brief The I2C device monitors if addressed I2C slave is present on the bus.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] addr      Address of the monitored I2C device (7 bits or 10 bits).
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *         Return value is I2C_IP6510_ERR_NONE if the addressed I2C slave is
 *         present on the bus.
 *
 * \note The I2C interface should be in Master mode before calling this API,
 *       for better performance the function doesn't check this,
 *       \ref i2c_ip6510_get_device_mode should return I2C_IP6510_MASTER_MODE.
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_monitor_slave(struct i2c_ip6510_dev_t* dev,
                                uint16_t addr);

/**
 * \brief Writes data to I2C device.
 *
 * \param[in] dev           I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] addr          I2C device address (7 bits or 10 bits).
 * \param[in] tx_data       Data buffer pointer to write.
 * \param[in] stop          If false, master does not generate STOP symbol.
 * \param[in,out] tx_length Data buffer length (number of bytes to write).
 *                          On return: stores the number of bytes written.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note The I2C interface should be in Master mode before calling this API,
 *       for better performance the function doesn't check this,
 *       \ref i2c_ip6510_get_device_mode should return I2C_IP6510_MASTER_MODE.
 * \note This function doesn't check if the pointers are NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_master_write(struct i2c_ip6510_dev_t* dev,
                                uint16_t addr,
                                const uint8_t* tx_data,
                                bool stop,
                                uint32_t* tx_length);

/**
 * \brief Reads data from I2C device.
 *
 * \param[in] dev           I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] addr          I2C device address (7 bits or 10 bits).
 * \param[in] rx_data       Buffer pointer to store the read data.
 * \param[in] stop          If false, master does not generate STOP symbol.
 * \param[in,out] rx_length Buffer length (number of bytes to read).
 *                          On return: stores the number of read bytes.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note The I2C interface should be in Master mode before calling this API,
 *       for better performance the function doesn't check this,
 *       \ref i2c_ip6510_get_device_mode should return I2C_IP6510_MASTER_MODE.
 * \note This function doesn't check if the pointers are NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_master_read(struct i2c_ip6510_dev_t* dev,
                                uint16_t addr,
                                uint8_t* rx_data,
                                bool stop,
                                uint32_t* rx_length);

/**
 * \brief Writes to and reads from I2C device (combined transfer).
 *
 * \param[in] dev           I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] addr          I2C device address (7 bits or 10 bits).
 * \param[in] tx_data       Data buffer pointer to write.
 * \param[in] rx_data       Buffer pointer to store the read data.
 * \param[in,out] tx_length Data buffer length (number of bytes to write).
 *                          On return: stores the number of bytes written.
 * \param[in,out] rx_length Buffer length (number of bytes to read).
 *                          On return: stores the number of read bytes.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note The I2C interface should be in Master mode before calling this API,
 *       for better performance the function doesn't check this,
 *       \ref i2c_ip6510_get_device_mode should return I2C_IP6510_MASTER_MODE.
 * \note This function doesn't check if the pointers are NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_master_write_read(
                                struct i2c_ip6510_dev_t* dev,
                                uint16_t addr,
                                const uint8_t* tx_data,
                                uint8_t* rx_data,
                                uint32_t* tx_length,
                                uint32_t* rx_length);

/*
 * \brief Writes one byte to I2C device
 *
 * \param[in] dev            I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] addr           Address to write to, accepts both 7 and 10 bit
 *                           addresses
 * \param[in] tx_data        Data pointer to write.
 * \param[in] set_addr       Set true for the first data byte. If true,
 *                           overwrites address register, starting a new
 *                           transaction. Set false for all following bytes in
 *                           the same transaction.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_master_byte_write(
                                const struct i2c_ip6510_dev_t* dev,
                                uint16_t addr,
                                const uint8_t* tx_data,
                                bool set_addr);

/*
 * \brief Reads one byte from I2C device
 *
 * \param[in]  dev          I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in]  addr         Address to read from, accepts both 7 and 10 bit
 *                          addresses
 * \param[in]  last_byte    If true ends transmission on the next byte received
 * \param[in]  set_addr     Set true for the first data byte. If true,
 *                          overwrites address register, starting a new
 *                          transaction. Set false for all following bytes in
 *                          the same transaction.
 * \param[out] rx_data      Data pointer to read to.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_master_byte_read(
                                const struct i2c_ip6510_dev_t* dev,
                                uint16_t addr,
                                bool last_byte,
                                bool set_addr,
                                uint8_t* rx_data);

/**
 * \brief Sets the I2C interface to be in slave mode with the given address.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] addr      Unique identifying address of the I2C interface.
 *
 * It sets the I2C interface to be in slave mode with an address which
 * identifies the interface on the I2C bus and sets the appropriate I2C
 * data rate for the slave mode.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_set_slave_mode(
                                struct i2c_ip6510_dev_t* dev,
                                uint16_t addr);

/**
 * \brief Sets the I2C interface to be in master mode
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * It only sets the I2C interface to be in master mode thus it will not answer
 * to further transfer requests.
 *
 * \note This function doesn't check if dev is NULL.
 */
void i2c_ip6510_set_master_mode(struct i2c_ip6510_dev_t* dev);

/**
 * \brief Returns the direction of the transmission received from the Master.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 *
 * \return Received direction of the transmission \ref i2c_ip6510_transf_dir_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum i2c_ip6510_transf_dir_t i2c_ip6510_get_slave_tranf_dir(
                                struct i2c_ip6510_dev_t* dev);

/**
 * \brief Sends data over the I2C bus to the Master.
 *
 * \param[in] dev           I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] tx_data       Data buffer pointer to write.
 * \param[in,out] tx_length Data buffer length (number of bytes to write).
 *                          On return: stores the number of bytes written.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note This API should be called only if transmission request has been
 *       received from the Master.
 * \note The I2C interface should be in Slave mode before calling this API,
 *       for better performance the function doesn't check this,
 *       \ref i2c_ip6510_get_device_mode should return I2C_IP6510_SLAVE_MODE.
 * \note This function doesn't check if the pointers are NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_slave_write(struct i2c_ip6510_dev_t* dev,
                                const uint8_t* tx_data,
                                uint32_t* tx_length);

/**
 * \brief Receives data from I2C Master.
 *
 * \param[in] dev           I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] rx_data       Buffer pointer to store the read data.
 * \param[in,out] rx_length Buffer length (number of bytes to read), a system
 *                          known parameter or must be communicated in advance.
 *                          On return: stores the number of bytes read.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 *
 * \note This API should be called only if transmission request has been
 *       received from the Master.
 * \note The I2C interface should be in Slave mode before calling this API,
 *       for better performance the function doesn't check this,
 *       \ref i2c_ip6510_get_device_mode should return I2C_IP6510_SLAVE_MODE.
 * \note This function doesn't check if the pointers are NULL.
 */
enum i2c_ip6510_error_t i2c_ip6510_slave_read(struct i2c_ip6510_dev_t* dev,
                                uint8_t* rx_data,
                                uint32_t* rx_length);

#ifdef __cplusplus
}
#endif
#endif /* __I2C_IP6510_DRV_H__ */
