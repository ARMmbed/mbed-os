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
 * \file i2c_ip6510_drv.c
 * \brief Driver for IP6510 I2C controller.
 */

#include "i2c_ip6510_drv.h"

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_compiler.h"

/*******************************************************************************
 * Cadence IP6510 I2C device specific definitions based on:
 *      I2C Controller IP User Guide
 *      Part Number:            IP6510
 *      IP Version Number:      r115_f01
 *      User Guide Revision:    1.20
 *
 ******************************************************************************/

/** Setter bit manipulation macro */
#define SET_BIT(WORD, BIT_INDEX) ((WORD) |= (1U << (BIT_INDEX)))
/** Clearing bit manipulation macro */
#define CLR_BIT(WORD, BIT_INDEX) ((WORD) &= ~(1U << (BIT_INDEX)))
/** Getter bit manipulation macro */
#define GET_BIT(WORD, BIT_INDEX) (bool)(((WORD) & (1U << (BIT_INDEX))))
/** Clear-and-Set bit manipulation macro */
#define ASSIGN_BIT(WORD, BIT_INDEX, VALUE) \
            (WORD = ((WORD & ~(1U << (BIT_INDEX))) | (VALUE << (BIT_INDEX))))

/** Getter bit-field manipulation macro */
#define GET_BIT_FIELD(WORD, BIT_MASK, BIT_OFFSET) \
            ((WORD & BIT_MASK) >> BIT_OFFSET)

/** Clear-and-Set bit-field manipulation macro */
#define ASSIGN_BIT_FIELD(WORD, BIT_MASK, BIT_OFFSET, VALUE) \
            (WORD = ((WORD & ~(BIT_MASK)) | ((VALUE << BIT_OFFSET) & BIT_MASK)))

#define BITMASK(width) ((1u<<(width))-1)

/**
 * \brief I2C (IP6510) register map structure
 */
struct i2c_ip6510_reg_map_t {
    volatile uint32_t control_reg;
        /*!< Offset: 0x00 (R/W) Control Register */
    volatile uint32_t status_reg;
        /*!< Offset: 0x04 (R/ ) Status Register */
    volatile uint32_t address_reg;
        /*!< Offset: 0x08 (R/W) I2C Address Register */
    volatile uint32_t data_reg;
        /*!< Offset: 0x0C (R/W) I2C Data Register */
    volatile uint32_t irq_status_reg;
        /*!< Offset: 0x10 (R/ ) Interrupt Status Register */
    volatile uint32_t transfer_size_reg;
        /*!< Offset: 0x14 (R/W) Transfer Size Register */
    volatile uint32_t slave_monitor_pause_reg;
        /*!< Offset: 0x18 (R/W) Slave Monitor Pause Register */
    volatile uint32_t time_out_reg;
        /*!< Offset: 0x1C (R/W) Time Out Register */
    volatile uint32_t irq_mask_reg;
        /*!< Offset: 0x20 (R/ ) Interrupt Mask Register */
    volatile uint32_t irq_enable_reg;
        /*!< Offset: 0x24 ( /W) Interrupt Enable Register */
    volatile uint32_t irq_disable_reg;
        /*!< Offset: 0x28 ( /W) Interrupt Disable Register */
    volatile uint32_t glitch_filter_control_reg;
        /*!< Offset: 0x2C (R/W) Glitch Filter Control Register */
};

#define I2C_IP6510_I2CCR_RW_OFF         (0u)
        /*!< Control Register Read/Write bit field offset */
#define I2C_IP6510_I2CCR_MS_OFF         (1u)
        /*!< Control Register Master/Slave bit field offset */
#define I2C_IP6510_I2CCR_NEA_OFF        (2u)
        /*!< Control Register Normal/Extended address bit field offset */
#define I2C_IP6510_I2CCR_ACKEN_OFF      (3u)
        /*!< Control Register Acknowledge Enable bit field offset */
#define I2C_IP6510_I2CCR_HOLD_OFF       (4u)
        /*!< Control Register Hold mode bit field offset */
#define I2C_IP6510_I2CCR_SLVMON_OFF     (5u)
        /*!< Control Register Slave Monitor mode bit field offset */
#define I2C_IP6510_I2CCR_CLRFIFO_OFF    (6u)
        /*!< Control Register Clear FIFO bit field offset */
#define I2C_IP6510_I2CCR_DIV_B_OFF      (8u)
        /*!< Control Register Divisor B bit field offset */
#define I2C_IP6510_I2CCR_DIV_A_OFF      (14u)
        /*!< Control Register Divisor A bit field offset */

#define I2C_IP6510_I2CSR_RXRW_OFF       (3u)
        /*!< Status Register RX read/write flag bit field offset*/
#define I2C_IP6510_I2CSR_RXDV_OFF       (5u)
        /*!< Status Register Receiver Data Valid bit field offset */
#define I2C_IP6510_I2CSR_TXDV_OFF       (6u)
        /*!< Status Register Transmitter Data Valid bit field offset */
#define I2C_IP6510_I2CSR_BA_OFF         (8u)
        /*!< Status Register Bus Active bit field offset */


/* Bit mask to determine the I2C address mode, if at least one of the upper
 * 3bits (from 10bits) is used (set to 1) than it is a 10bit I2C address */
#define I2C_10BIT_ADDR_MASK                 (0x0380u)

/* Valid range of I2C addresses based on specification */
#define I2C_7BIT_ADDR_LOWEST                (0x8u)
#define I2C_10BIT_ADDR_HIGHEST              (0x3FFu)

/* The default value is based on the Cadence I2C IP User Guide */
#define I2C_TIMEOUT_REG_DEFAULT_VALUE       (0x001Fu)

/* The frequency of the inner clock_enable signal is 22 times faster than the
 * required SCL output frequency */
#define I2C_SPEED_100KHZ_CLOCK_EN_FREQ      (2200000u)
#define I2C_SPEED_400KHZ_CLOCK_EN_FREQ      (8800000u)

/* In case of slave mode with FIFO implemented, to support the minimum
 * SCL period specification for fast-mode (400kHz), the device must be
 * programmed to run at higher frequency. The value is based on the
 * Cadence I2C IP User Guide and the clock divisor calculation method */
#define I2C_SPEED_MAX_SUPP_CLOCK_EN_FREQ    (13200000u)

/* Mask for Divisor A field in the Control Register */
#define I2C_IP6510_I2CCR_DIV_A_WIDTH    (2u)
#define I2C_IP6510_I2CCR_DIV_A_MASK     (BITMASK(I2C_IP6510_I2CCR_DIV_A_WIDTH) \
                                        << I2C_IP6510_I2CCR_DIV_A_OFF)

/* Mask for Divisor B field in the Control Register */
#define I2C_IP6510_I2CCR_DIV_B_WIDTH    (6u)
#define I2C_IP6510_I2CCR_DIV_B_MASK     (BITMASK(I2C_IP6510_I2CCR_DIV_B_WIDTH) \
                                        << I2C_IP6510_I2CCR_DIV_B_OFF)

/* The maximum value of stage A clock divider is 3.*/
#define I2C_IP6510_DIVISOR_A_MAX_VALUE      (3u)

/* The maximum value of stage B clock divider is 63.*/
#define I2C_IP6510_DIVISOR_B_MAX_VALUE      (63u)

/* The maximum number of received bytes in one (not combined) transfer
 * is 0xF (due to Transfer Size Register constraints). */
#define I2C_RECEIVED_BYTE_MAX_NUMBER        (15u)


/**
 * \brief Sets the direction of the I2C transfer.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 * \param[in] transf_dir    Required direction of the I2C transfer.
 *                          \ref i2c_ip6510_transf_dir_t
 */
 static void set_transfer_dir(struct i2c_ip6510_reg_map_t *p_i2c,
                                enum i2c_ip6510_transf_dir_t transf_dir)
{
    if(transf_dir == I2C_IP6510_TRANSMITTER) {
        CLR_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_RW_OFF);

    } else {
        /* I2C_IP6510_RECEIVER */
        SET_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_RW_OFF);
    }
}

/**
 * \brief Determines and sets the I2C addressing mode based on the address.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 * \param[in] addr          I2C device address (7bits or 10bits).
 *
 * \return Returns with the determined addressing mode.
 * \ref i2c_ip6510_addr_mode_t
 */
static enum i2c_ip6510_addr_mode_t set_address_mode(
                                struct i2c_ip6510_reg_map_t *p_i2c,
                                uint16_t addr)
{
    /* If at least one of the upper 3bits (from 10bits) is used (set to 1)
     * than it is a 10bit I2C address */
    if(addr & I2C_10BIT_ADDR_MASK) {
        /* Using I2C Extended Address mode */
        CLR_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_NEA_OFF);
        return I2C_IP6510_EXT_ADDR_MODE;

    } else {
        /* Using I2C Normal Address mode */
        SET_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_NEA_OFF);
        return I2C_IP6510_NOR_ADDR_MODE;
    }
}

/**
 * \brief Enables Slave Monitor mode (in master mode only).
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void enable_slave_monitor(struct i2c_ip6510_reg_map_t *p_i2c)
{
    SET_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_SLVMON_OFF);
}

/**
 * \brief Disables Slave Monitor mode (in master mode only).
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void disable_slave_monitor(struct i2c_ip6510_reg_map_t *p_i2c)
{
    CLR_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_SLVMON_OFF);
}

/**
 * \brief Check if Slave Monitor mode is enabled.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 *
 * \return Returns bool, true if Slave Monitor is enabled, false otherwise
 */
static bool is_slave_monitor_enabled(const struct i2c_ip6510_reg_map_t *p_i2c)
{
    return GET_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_SLVMON_OFF);
}

/**
 * \brief Configures the clock dividers (A and B) with the given values.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 * \param[in] div_a         Value of divisor stage A.
 * \param[in] div_b         Value of divisor stage B.
 */
static void set_divisor(struct i2c_ip6510_reg_map_t *p_i2c,
                                uint32_t div_a, uint32_t div_b)
{
    ASSIGN_BIT_FIELD(p_i2c->control_reg,
                     I2C_IP6510_I2CCR_DIV_A_MASK,
                     I2C_IP6510_I2CCR_DIV_A_OFF,
                     div_a);

    ASSIGN_BIT_FIELD(p_i2c->control_reg,
                     I2C_IP6510_I2CCR_DIV_B_MASK,
                     I2C_IP6510_I2CCR_DIV_B_OFF,
                     div_b);
}

/**
 * \brief Enables transmission of ACK.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void ack_enable(struct i2c_ip6510_reg_map_t *p_i2c)
{
    SET_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_ACKEN_OFF);
}

/**
 * \brief Disables transmission of ACK.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void ack_disable(struct i2c_ip6510_reg_map_t *p_i2c)
{
    CLR_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_ACKEN_OFF);
}

/**
 * \brief Enables Hold mode.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void hold_enable(struct i2c_ip6510_reg_map_t *p_i2c)
{
    SET_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_HOLD_OFF);
}

/**
 * \brief Disables Hold mode.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void hold_disable(struct i2c_ip6510_reg_map_t *p_i2c)
{
    CLR_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_HOLD_OFF);
}

/**
 * \brief Clears FIFO (and clears the Transfer Size register).
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void clr_fifo(struct i2c_ip6510_reg_map_t *p_i2c)
{
    __DMB();
    SET_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_CLRFIFO_OFF);
    __DSB();
}

/**
 * \brief Clears the specified I2C interrupts.
 *
 * \param[in] p_i2c     I2C (IP6510) register map structure.
 *                      \ref i2c_ip6510_reg_map_t
 * \param[in] mask      Bit mask for clearing interrupts. Values from
 *                      \ref i2c_ip6510_intr_t could be used to create the mask.
 */
static void clr_irq(struct i2c_ip6510_reg_map_t *p_i2c, uint32_t mask)
{
    p_i2c->irq_status_reg = mask;
}

/**
 * \brief Configures the address register with the given address.
 *
 * \param[in] p_i2c     I2C (IP6510) register map structure.
 *                      \ref i2c_ip6510_reg_map_t
 * \param[in] addr      I2C device address (7bits or 10bits).
 */
static void set_address(struct i2c_ip6510_reg_map_t *p_i2c, uint16_t addr)
{
    __DMB();
    p_i2c->address_reg = (uint32_t)addr;
    __DSB();
}

/**
 * \brief Checks if new and valid data is available to be read.
 *
 * \param[in] p_i2c     I2C (IP6510) register map structure.
 *                      \ref i2c_ip6510_reg_map_t
 *
 * \return Returns bool, true if received data is available, false otherwise.
 */
static bool is_rx_data_ready(const struct i2c_ip6510_reg_map_t *p_i2c)
{
    return GET_BIT(p_i2c->status_reg, I2C_IP6510_I2CSR_RXDV_OFF);
}

/**
 * \brief Checks if there is still data to be transmitted.
 *
 * \param[in] p_i2c     I2C (IP6510) register map structure.
 *                      \ref i2c_ip6510_reg_map_t
 *
 * \return  Returns bool, true if there is still data to be transmitted,
 *          false otherwise.
 */
static bool is_tx_data_waiting(const struct i2c_ip6510_reg_map_t *p_i2c)
{
    return GET_BIT(p_i2c->status_reg, I2C_IP6510_I2CSR_TXDV_OFF);
}

/**
 * \brief Reads a data byte from the FIFO.
 *
 * \param[in] p_i2c     I2C (IP6510) register map structure.
 *                      \ref i2c_ip6510_reg_map_t
 *
 * \return Returns the received data byte.
 */
static uint8_t rx_byte(const struct i2c_ip6510_reg_map_t *p_i2c)
{
    return (uint8_t)(p_i2c->data_reg);
}

/**
 * \brief Writes a data byte into the FIFO to be transmitted.
 *
 * \param[in] p_i2c     I2C (IP6510) register map structure.
 *                      \ref i2c_ip6510_reg_map_t
 * \param[in] data      Data byte to be transmitted.
 */
static void tx_byte(struct i2c_ip6510_reg_map_t *p_i2c, uint8_t data)
{
    __DMB();
    p_i2c->data_reg = (uint32_t)data;
    __DSB();
}

/**
 * \brief Checks if I2C transfer is successful.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 *
 * \return Returns bool, true if transfer is successful, false otherwise.
 */
static bool is_transfer_complete(const struct i2c_ip6510_reg_map_t *p_i2c)
{
    return GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_COMP_OFF);
}

/**
 * \brief Clears the complete interrupt status register.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void clear_irq_status_reg(struct i2c_ip6510_reg_map_t *p_i2c)
{
    p_i2c->irq_status_reg = I2C_IP6510_ALL_INTR_MASK;
}

/**
 * \brief Sets the Transfer Size register with the given value.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 * \param[in] byte_num      New Transfer Size register value.
 */
static void set_transfer_size(struct i2c_ip6510_reg_map_t *p_i2c,
                              uint32_t byte_num)
{
    p_i2c->transfer_size_reg = byte_num;
}

/**
 * \brief Returns the value of the Transfer Size register.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static uint32_t get_transfer_size(struct i2c_ip6510_reg_map_t *p_i2c)
{
    return p_i2c->transfer_size_reg;
}

/**
 * \brief Writes the default values into the registers where it is applicable.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 */
static void reset_regs(struct i2c_ip6510_reg_map_t *p_i2c)
{
    p_i2c->control_reg                  = 0U;
    p_i2c->address_reg                  = 0U;
    p_i2c->transfer_size_reg            = 0U;
    p_i2c->slave_monitor_pause_reg      = 0U;
    p_i2c->time_out_reg                 = I2C_TIMEOUT_REG_DEFAULT_VALUE;
    p_i2c->glitch_filter_control_reg    = 0U;
}

/**
 * \brief Sets the I2C interface mode.
 *
 * \param[in] dev       I2C (IP6510) device structure \ref i2c_ip6510_dev_t
 * \param[in] mode      I2C interface mode \ref i2c_ip6510_device_mode_t
 *
 * \note This function doesn't check if dev is NULL.
 */
static void set_device_mode(struct i2c_ip6510_dev_t *dev,
                                enum i2c_ip6510_device_mode_t mode)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    ASSIGN_BIT(p_i2c->control_reg, I2C_IP6510_I2CCR_MS_OFF, mode);

    dev->data->mode = mode;
}

enum i2c_ip6510_error_t i2c_ip6510_init(struct i2c_ip6510_dev_t *dev,
                                        uint32_t sys_clk)
{
    enum i2c_ip6510_error_t err = I2C_IP6510_ERR_NONE;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(dev->data->state != I2C_IP6510_UNINITIALIZED) {
        return I2C_IP6510_ERR_ALREADY_INIT;
    }

    if(sys_clk == 0U) {
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    i2c_ip6510_disable_irq(dev, I2C_IP6510_ALL_INTR_MASK);

    clear_irq_status_reg(p_i2c);
    clr_fifo(p_i2c);

    /* Updating the system clock frequency */
    dev->data->sys_clk = sys_clk;

    set_device_mode(dev, dev->cfg->default_mode);
    err = i2c_ip6510_set_speed(dev, dev->cfg->default_bus_speed);

    if(err != I2C_IP6510_ERR_NONE) {
        return err;
    }

    dev->data->state = I2C_IP6510_INITIALIZED;

    return I2C_IP6510_ERR_NONE;
}

void i2c_ip6510_uninit(struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    i2c_ip6510_disable_irq(dev, I2C_IP6510_ALL_INTR_MASK);

    clear_irq_status_reg(p_i2c);

    reset_regs(p_i2c);

    clr_fifo(p_i2c);

    dev->data->state = I2C_IP6510_UNINITIALIZED;
}

void i2c_ip6510_hold_enable(const struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                    (struct i2c_ip6510_reg_map_t*)dev->cfg->base;
    hold_enable(p_i2c);
}

void i2c_ip6510_hold_disable(const struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                     (struct i2c_ip6510_reg_map_t*)dev->cfg->base;
    hold_disable(p_i2c);
}

enum i2c_ip6510_state_t i2c_ip6510_get_state(const struct i2c_ip6510_dev_t *dev)
{
    return dev->data->state;
}

enum i2c_ip6510_device_mode_t i2c_ip6510_get_device_mode(
                                const struct i2c_ip6510_dev_t *dev)
{
    return dev->data->mode;
}

enum i2c_ip6510_error_t i2c_ip6510_set_speed(struct i2c_ip6510_dev_t *dev,
                                enum i2c_ip6510_speed_t speed)
{
    uint32_t div_a = 0U;
    uint32_t div_b = 0U;
    uint32_t clock_enable_freq = 0U;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(dev->data->sys_clk == 0U) {
        /* Invalid system clock frequency */
        return I2C_IP6510_ERR;
    }

    /* The required frequency of SCL output is 22 times slower than the
     * frequency of the inner clock_enable signal. */
    switch(speed) {

    case I2C_IP6510_SPEED_100KHZ:
        clock_enable_freq = I2C_SPEED_100KHZ_CLOCK_EN_FREQ;
        dev->data->bus_speed = I2C_IP6510_SPEED_100KHZ;
        break;

    case I2C_IP6510_SPEED_400KHZ:
        clock_enable_freq = I2C_SPEED_400KHZ_CLOCK_EN_FREQ;
        dev->data->bus_speed = I2C_IP6510_SPEED_400KHZ;
        break;

    case I2C_IP6510_SPEED_MAX_SUPPORTED:
        /* In case of slave mode with FIFO implemented, to support the minimum
         * SCL period specification for fast-mode (400kHz), the device must be
         * programmed to run at higher frequency. */
        clock_enable_freq = I2C_SPEED_MAX_SUPP_CLOCK_EN_FREQ;
        dev->data->bus_speed = I2C_IP6510_SPEED_MAX_SUPPORTED;
        break;

    default:
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    /* Calculating the value of stage A and B clock dividers */
    div_a = 0U;
    div_b = (uint32_t)(dev->data->sys_clk / clock_enable_freq);

    if(div_b > I2C_IP6510_DIVISOR_B_MAX_VALUE) {

        /* (div_a+1)x(div_b+1) must be equal to former div_b value */
        div_a = I2C_IP6510_DIVISOR_A_MAX_VALUE;
        div_b = (uint32_t)(div_b+1U) >> I2C_IP6510_I2CCR_DIV_A_WIDTH;
    }

    set_divisor(p_i2c, div_a, div_b);

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_speed_t i2c_ip6510_get_speed(const struct i2c_ip6510_dev_t *dev)
{
    return dev->data->bus_speed;
}

enum i2c_ip6510_bus_state_t i2c_ip6510_get_bus_status(
                                const struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    return (GET_BIT(p_i2c->status_reg, I2C_IP6510_I2CSR_BA_OFF)) ?
            I2C_IP6510_BUS_ACTIVE : I2C_IP6510_BUS_INACTIVE;
}

int i2c_ip6510_get_transfer_size(const struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    return p_i2c->transfer_size_reg;
}

uint32_t i2c_ip6510_get_irq_status(const struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    return p_i2c->irq_status_reg;
}

uint32_t i2c_ip6510_get_irq_mask(const struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    return p_i2c->irq_mask_reg;
}

void i2c_ip6510_clear_irq(struct i2c_ip6510_dev_t *dev, uint32_t mask)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    clr_irq(p_i2c, mask);
}

void i2c_ip6510_enable_irq(struct i2c_ip6510_dev_t *dev, uint32_t mask)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    p_i2c->irq_enable_reg = mask;
}

void i2c_ip6510_disable_irq(struct i2c_ip6510_dev_t *dev, uint32_t mask)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    p_i2c->irq_disable_reg = mask;
}

void i2c_ip6510_set_timeout(struct i2c_ip6510_dev_t *dev,
                                uint8_t interval)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    p_i2c->time_out_reg = (uint32_t)interval;
}

void i2c_ip6510_set_slave_monitor_pause_interval(struct i2c_ip6510_dev_t *dev,
                            uint8_t interval)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    p_i2c->slave_monitor_pause_reg = (uint32_t)interval;
}

uint8_t i2c_ip6510_get_slave_monitor_pause_interval(
                            struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    return (uint8_t)(p_i2c->slave_monitor_pause_reg);
}

void i2c_ip6510_set_glitch_filter_length(struct i2c_ip6510_dev_t *dev,
                                uint32_t length)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    p_i2c->glitch_filter_control_reg = length;
}

uint8_t i2c_ip6510_get_glitch_filter_length(const struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    return    (uint8_t)(p_i2c->glitch_filter_control_reg);
}

enum i2c_ip6510_error_t i2c_ip6510_monitor_slave(struct i2c_ip6510_dev_t *dev,
                                uint16_t addr)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
                (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    clear_irq_status_reg(p_i2c);
    set_address_mode(p_i2c, addr);
    set_transfer_dir(p_i2c, I2C_IP6510_TRANSMITTER);
    enable_slave_monitor(p_i2c);

    /* Initiates slave addressing */
    set_address(p_i2c, addr);

    /* Slave Monitor Mode gets disabled if given slave is present */
    while(is_slave_monitor_enabled(p_i2c) ||
            !(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_SLVRDY_OFF))) {

        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
            disable_slave_monitor(p_i2c);
            return I2C_IP6510_ERR_NACK;
        }
    }
    return I2C_IP6510_ERR_NONE;
}

/**
 * \brief Initiates transfer and writes the given number of data bytes to send.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 * \param[in] addr          I2C device address (7 bits or 10 bits).
 * \param[in] tx_data       Data buffer pointer to write.
 * \param[in] tx_length     Data buffer length (number of bytes to write).
 * \param[out] write_cntr   Number of bytes written.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 */
static enum i2c_ip6510_error_t tx_transfer(
                                struct i2c_ip6510_reg_map_t *p_i2c,
                                uint16_t addr,
                                const uint8_t *tx_data,
                                uint32_t *tx_length)
{
    uint32_t write_cntr = *tx_length;

    ack_enable(p_i2c);
    clear_irq_status_reg(p_i2c);
    set_address_mode(p_i2c, addr);
    set_transfer_dir(p_i2c, I2C_IP6510_TRANSMITTER);

    /* Initiates I2C transfer */
    set_address(p_i2c, addr);

    while(write_cntr) {

        tx_byte(p_i2c, *tx_data);

        /* If the sent data byte or slave addressing is not acknowledged */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
            *tx_length -= write_cntr;
            return I2C_IP6510_ERR_NACK;

        } else if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_TXOVF_OFF)) {
            /* In case of FIFO overflow the last written byte is ignored and
             * the Data Register access must be repeated. */
            clr_irq(p_i2c, I2C_IP6510_INTR_TXOVF_MASK);
            continue;
        }

        tx_data++;
        write_cntr--;
    }
    *tx_length -= write_cntr;

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_error_t i2c_ip6510_master_write(struct i2c_ip6510_dev_t *dev,
                                uint16_t addr,
                                const uint8_t *tx_data,
                                bool stop,
                                uint32_t *tx_length)
{
    enum i2c_ip6510_error_t err = I2C_IP6510_ERR_NONE;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(*tx_length == 0U) {
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    clr_fifo(p_i2c);

    /* Prevents the I2C interface from generating a STOP condition */
    hold_enable(p_i2c);

    /* Initiates transfer and transmits the given number of data bytes */
    err = tx_transfer(p_i2c, addr, tx_data, tx_length);

    if(err != I2C_IP6510_ERR_NONE) {
        hold_disable(p_i2c);

        return err;
    }

    while(!is_transfer_complete(p_i2c)) {

        /* If the last transmitted byte did not get an ACK */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
            return I2C_IP6510_ERR_NACK;
        }
    }

    /* If enabled, the host can terminate the transfer with disabled hold mode
     */
    if (stop) {
        hold_disable(p_i2c);
    }

    return I2C_IP6510_ERR_NONE;
}

/**
 * \brief Initiates transfer and reads the received data bytes.
 *
 * \param[in] p_i2c         I2C (IP6510) register map structure.
 *                          \ref i2c_ip6510_reg_map_t
 * \param[in] addr          I2C device address (7 bits or 10 bits).
 * \param[in] rx_data       Buffer pointer to store the read data.
 * \param[in] rx_length     Buffer length (number of bytes to read).
 * \param[out] read_cntr    Number of bytes read.
 *
 * \return Returns error code as specified in \ref i2c_ip6510_error_t
 */
static enum i2c_ip6510_error_t rx_transfer_combined(
                                struct i2c_ip6510_reg_map_t *p_i2c,
                                uint16_t addr,
                                uint8_t *rx_data,
                                uint32_t *rx_length)
{
    uint32_t read_cntr = *rx_length;
    uint32_t part_rx_length = (read_cntr > I2C_RECEIVED_BYTE_MAX_NUMBER) ?
                                    I2C_RECEIVED_BYTE_MAX_NUMBER : read_cntr;

    /* The I2C controller handles the Acknowledge generation. And Acknowledge
     * has to be enabled. */
    ack_enable(p_i2c);
    clear_irq_status_reg(p_i2c);
    set_address_mode(p_i2c, addr);
    set_transfer_dir(p_i2c, I2C_IP6510_RECEIVER);

    /* Initiates I2C transfer */
    set_address(p_i2c, addr);
    set_transfer_size(p_i2c, part_rx_length);

    while(read_cntr) {

        /* The maximum number of received bytes in one transfer is limited. If
         * the read length is more than the maximum number of bytes that can be
         * received in one transfer, the transfer size register has to be
         * adjusted before reaching 0. If the transfer size register reaches
         * zero, the I2C controller terminates the transfer before completion.
         */
        if (get_transfer_size(p_i2c) == 1U) {
            part_rx_length = (read_cntr > I2C_RECEIVED_BYTE_MAX_NUMBER) ?
                                    I2C_RECEIVED_BYTE_MAX_NUMBER : read_cntr;

            set_transfer_size(p_i2c, part_rx_length);
        }

        /* Waiting for new valid data to read */
        while(!is_rx_data_ready(p_i2c)) {

            /* If the previous slave addressing is not acknowledged */
            if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
                *rx_length -= read_cntr;
                return I2C_IP6510_ERR_NACK;

            } else if(GET_BIT(p_i2c->irq_status_reg,
                      I2C_IP6510_INTR_RXOVF_OFF)) {

                *rx_length -= read_cntr;
                /* In case of Receive Overflow return with error code */
                return I2C_IP6510_ERR_RXOVF;
            }
        }

        *rx_data = rx_byte(p_i2c);

        rx_data++;
        read_cntr--;
    }
    *rx_length -= read_cntr;

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_error_t i2c_ip6510_master_read(struct i2c_ip6510_dev_t *dev,
                                uint16_t addr,
                                uint8_t *rx_data,
                                bool stop,
                                uint32_t *rx_length)
{
    enum i2c_ip6510_error_t err = I2C_IP6510_ERR_NONE;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(*rx_length == 0U) {
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    clr_fifo(p_i2c);

    /* Receive overflow can be avoided and combined read transfer can be
     * realized if Hold Mode is enabled */
    hold_enable(p_i2c);

    /* It reads the given number of received bytes from the FIFO
     * (in a combined transfer if necessary) */
    err = rx_transfer_combined(p_i2c, addr, rx_data, rx_length);

    if(err != I2C_IP6510_ERR_NONE) {
        hold_disable(p_i2c);

        return err;
    }

    /* If enabled, the host can terminate the transfer with disabled hold mode
     */
    if (stop) {
        hold_disable(p_i2c);
    }

    /* Waiting for the whole transfer to be complete */
    while(!is_transfer_complete(p_i2c)) {
        /* If there is more, unexpected data byte received due to
         * any kind of error. */
        if(is_rx_data_ready(p_i2c)) {
            return I2C_IP6510_ERR;
        }
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_RXOVF_OFF)) {
            return I2C_IP6510_ERR_RXOVF;
        }
    }

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_error_t i2c_ip6510_master_write_read(
                                struct i2c_ip6510_dev_t *dev,
                                uint16_t addr,
                                const uint8_t *tx_data,
                                uint8_t *rx_data,
                                uint32_t *tx_length,
                                uint32_t *rx_length)
{
    enum i2c_ip6510_error_t err = I2C_IP6510_ERR_NONE;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if((*tx_length == 0U) || (*rx_length == 0U)) {
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    /* Prevents the I2C interface from generating a STOP condition */
    hold_enable(p_i2c);

    /* Initiates transfer and transmits the given number of data bytes */
    err = tx_transfer(p_i2c, addr, tx_data, tx_length);

    if(err != I2C_IP6510_ERR_NONE) {
        hold_disable(p_i2c);
        return err;
    }

    while(!is_transfer_complete(p_i2c)) {

        /* If the last transmitted byte did not get an ACK */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
            hold_disable(p_i2c);
            return I2C_IP6510_ERR_NACK;
        }
    }

    clr_irq(p_i2c, I2C_IP6510_INTR_COMP_MASK);

    /* Write transaction is complete, continue with read transaction */

    /* The host does not need to ACK only 1 byte,
     * it generates a STOP condition after reception */
    if(*rx_length == 1U) {
        ack_disable(p_i2c);
    }

    /* It reads the given number of received bytes from the FIFO
     * (in a combined transfer if necessary) */
    err = rx_transfer_combined(p_i2c, addr, rx_data, rx_length);

    if(err != I2C_IP6510_ERR_NONE) {
        hold_disable(p_i2c);
        return err;
    }

    /* The host can terminate the transfer with disabled hold mode */
    hold_disable(p_i2c);

    /* Waiting for the whole combined transfer to be complete */
    while(!is_transfer_complete(p_i2c)) {

        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_RXOVF_OFF)) {
            return I2C_IP6510_ERR_RXOVF;
        }
    }

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_error_t i2c_ip6510_master_byte_write(
                                const struct i2c_ip6510_dev_t *dev,
                                uint16_t addr,
                                const uint8_t *tx_data,
                                bool set_addr)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    clear_irq_status_reg(p_i2c);

    if(set_addr == true) {
        clr_fifo(p_i2c);
        set_address_mode(p_i2c, addr);
        set_transfer_dir(p_i2c, I2C_IP6510_TRANSMITTER);
    }
    ack_enable(p_i2c);

    /* Write the data byte to the FIFO */
    tx_byte(p_i2c, *tx_data);

    /* If the first data byte was written, only then set the address */
    if(set_addr == true) {
        /* Initiates I2C transfer */
        set_address(p_i2c, addr);
    }

    /* If the sent data byte or slave addressing is not acknowledged */
    if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
        return I2C_IP6510_ERR_NACK;
    } else if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_TXOVF_OFF)) {
        /* In case of FIFO overflow the last written byte is ignored */
        clr_irq(p_i2c, I2C_IP6510_INTR_TXOVF_MASK);
        return I2C_IP6510_ERR_TXOVF;
    }

    /* Waiting for the written byte to be transmitted */
    while(!is_transfer_complete(p_i2c)) {
        /* If the last transmitted byte did not get an ACK */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
            return I2C_IP6510_ERR_NACK;
        }
    }

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_error_t i2c_ip6510_master_byte_read(
                                const struct i2c_ip6510_dev_t *dev,
                                uint16_t addr,
                                bool last_byte,
                                bool set_addr,
                                uint8_t *rx_data)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    clear_irq_status_reg(p_i2c);

    if(last_byte) {
        /* This is the last byte to receive */
        ack_disable(p_i2c);
        set_transfer_size(p_i2c, 1U);
    } else {
        /* There will be more then one data bytes */
        ack_enable(p_i2c);
        set_transfer_size(p_i2c, I2C_RECEIVED_BYTE_MAX_NUMBER);
    }

    /* If its the first read, the I2C controller has to be configured */
    if (set_addr){
        clr_fifo(p_i2c);
        set_address_mode(p_i2c, addr);
        set_transfer_dir(p_i2c, I2C_IP6510_RECEIVER);
        set_address(p_i2c, addr);
    }

    /* Waiting for new valid data to read */
    while(!is_rx_data_ready(p_i2c)) {

        /* If the previous slave addressing is not acknowledged */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {
            return I2C_IP6510_ERR_NACK;

        } else if(GET_BIT(p_i2c->irq_status_reg,
                  I2C_IP6510_INTR_RXOVF_OFF)) {
            /* In case of Receive Overflow return with error code */
            return I2C_IP6510_ERR_RXOVF;
        }
    }

    /* Read the data byte from the FIFO */
    *rx_data = rx_byte(p_i2c);

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_error_t i2c_ip6510_set_slave_mode(
                                struct i2c_ip6510_dev_t *dev,
                                uint16_t addr)
{
    enum i2c_ip6510_error_t err = I2C_IP6510_ERR_NONE;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    /* Checking whether the address is a valid 7/10bit I2C slave address */
    if((addr < I2C_7BIT_ADDR_LOWEST) || (addr > I2C_10BIT_ADDR_HIGHEST)) {
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    /* I2C speed must be set to the fastest SCL frequency supported */
    err = i2c_ip6510_set_speed(dev, I2C_IP6510_SPEED_MAX_SUPPORTED);

    if(err != I2C_IP6510_ERR_NONE) {
        return err;
    }

    set_device_mode(dev, I2C_IP6510_SLAVE_MODE);
    clear_irq_status_reg(p_i2c);
    ack_enable(p_i2c);
    hold_enable(p_i2c);
    set_address_mode(p_i2c, addr);
    set_address(p_i2c, addr);

    return I2C_IP6510_ERR_NONE;
}

void i2c_ip6510_set_master_mode(struct i2c_ip6510_dev_t *dev)
{
    set_device_mode(dev, I2C_IP6510_MASTER_MODE);
}

enum i2c_ip6510_transf_dir_t i2c_ip6510_get_slave_tranf_dir(
                                struct i2c_ip6510_dev_t *dev)
{
    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    return (GET_BIT(p_i2c->status_reg, I2C_IP6510_I2CSR_RXRW_OFF)) ?
            I2C_IP6510_TRANSMITTER : I2C_IP6510_RECEIVER;
}

enum i2c_ip6510_error_t i2c_ip6510_slave_write(struct i2c_ip6510_dev_t *dev,
                                const uint8_t *tx_data,
                                uint32_t *tx_length)
{
    uint32_t write_cntr = *tx_length;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(write_cntr == 0U) {
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    clr_fifo(p_i2c);
    ack_enable(p_i2c);
    hold_enable(p_i2c);

    /* The amount of data to be transferred is a system known parameter */
    while(write_cntr) {

        /* If the previously sent data byte is not acknowledged */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF)) {

            hold_disable(p_i2c);
            clr_fifo(p_i2c);
            *tx_length -= write_cntr;
            return I2C_IP6510_ERR_NACK;
        }

        tx_byte(p_i2c, *tx_data);

        /* In case of FIFO overflow the last written byte is ignored and
         * the Data Register access must be repeated. */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_TXOVF_OFF)) {

            clr_irq(p_i2c, I2C_IP6510_INTR_TXOVF_MASK);
            continue;
        }

        tx_data++;
        write_cntr--;
    }
    *tx_length -= write_cntr;

    hold_disable(p_i2c);

    /* Waiting for the last byte to be transmitted from the FIFO */
    while(is_tx_data_waiting(p_i2c)) {

        /* If the transfer is terminated by the Master before completion */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_NACK_OFF) &&
                is_tx_data_waiting(p_i2c)) {

            clr_fifo(p_i2c);
            return I2C_IP6510_ERR_NACK;
        }

        /* DATA interrupt flag is set when there are only 2 bytes left in
         * the FIFO, so at this point there is still data in the FIFO to be
         * transmitted and DATA flag should be cleared for further
         * examination. */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_DATA_OFF)) {

            clr_irq(p_i2c, I2C_IP6510_INTR_DATA_MASK);
        }
    }

    clr_fifo(p_i2c);

    while(!is_transfer_complete(p_i2c)) {

        /* If DATA interrupt flag is set while the TXDV flag in the Status
         * Register is cleared the Master wants to continue the transfer. */
        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_DATA_OFF) &&
                !is_tx_data_waiting(p_i2c)) {

            return I2C_IP6510_ERR;
        }
    }

    return I2C_IP6510_ERR_NONE;
}

enum i2c_ip6510_error_t i2c_ip6510_slave_read(struct i2c_ip6510_dev_t *dev,
                                uint8_t *rx_data,
                                uint32_t *rx_length)
{
    uint32_t read_cntr = *rx_length;

    struct i2c_ip6510_reg_map_t *p_i2c =
            (struct i2c_ip6510_reg_map_t*)dev->cfg->base;

    if(read_cntr == 0U) {
        return I2C_IP6510_ERR_INVALID_ARG;
    }

    if(i2c_ip6510_get_state(dev) != I2C_IP6510_INITIALIZED) {
        return I2C_IP6510_ERR_NOT_INIT;
    }

    ack_enable(p_i2c);

    /* Receive overflow can be avoided if Hold Mode is enabled */
    hold_enable(p_i2c);

    while(read_cntr) {

        while(!is_rx_data_ready(p_i2c)) {

            /* With HOLD enabled the overflow conditions are avoided */
            if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_RXOVF_OFF)) {
                hold_disable(p_i2c);
                *rx_length -= read_cntr;
                return I2C_IP6510_ERR_RXOVF;
            }
        }

        *rx_data = rx_byte(p_i2c);

        rx_data++;
        read_cntr--;
    }
    *rx_length -= read_cntr;

    hold_disable(p_i2c);

    /* Waiting for the master to terminate the transfer */
    while(!is_transfer_complete(p_i2c)) {

        if(GET_BIT(p_i2c->irq_status_reg, I2C_IP6510_INTR_RXOVF_OFF)) {
            return I2C_IP6510_ERR_RXOVF;

        } else if(is_rx_data_ready(p_i2c)) {
            /* If there are more, unexpected data bytes received from the Master
             */
            return I2C_IP6510_ERR;
        }
    }

    return I2C_IP6510_ERR_NONE;
}
