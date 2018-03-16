/*
 * Copyright (c) 2018 ARM Limited
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
 * \file spi_pl022_drv.h
 * \brief Generic driver for ARM SPI PL022.
 */

#ifndef __SPI_PL022_DRV_H__
#define __SPI_PL022_DRV_H__

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Frame format */
#define SPI_PL022_CFG_FRF_MOT         0
#define SPI_PL022_CFG_FRF_TI          1
#define SPI_PL022_CFG_FRF_MICROWIRE   2

enum spi_pl022_mode_select_t {
    SPI_PL022_MASTER_SELECT = 0,
    SPI_PL022_SLAVE_SELECT,
};

enum spi_pl022_slave_output_mode_t {
    SPI_PL022_SLAVE_OUTPUT_EN = 0,
    SPI_PL022_SLAVE_OUTPUT_DIS,
};

enum spi_pl022_loopback_select_t {
    SPI_PL022_LOOPBACK_MODE_DIS = 0,
    SPI_PL022_LOOPBACK_MODE_EN,
};

struct spi_pl022_periphid_t {
    uint32_t partNumber;
    uint32_t designerID;
    uint32_t revision;
    uint32_t configuration;
};

struct spi_pl022_primecell_id_t {
    uint8_t cellid0;
    uint8_t cellid1;
    uint8_t cellid2;
    uint8_t cellid3;
};

/* ARM SPI PL022 device control configuration structure */
struct spi_pl022_ctrl_cfg_t {
    enum spi_pl022_mode_select_t spi_mode;  /*!< master-slave */
    uint8_t frame_format;                   /*!< frame format bitmap
                                                 clock phase [7] polarity [6]
                                                 reserved [5:3]
                                                 frame_format [1:0] */
    uint8_t word_size;                      /*!< value 4 to 16 */
    uint8_t reserved[2];                    /*!< to keep 32 bits aligned */
    uint32_t bit_rate;                      /*!< required bit rate */
};

/* ARM SPI PL022 device configuration structure */
struct spi_pl022_dev_cfg_t {
    const uint32_t base;  /*!< SPI PL022 base address */
    const struct spi_pl022_ctrl_cfg_t default_ctrl_cfg; /*!< Default SPI
                                                             configuration */
};

/* ARM SPI PL022 device data structure */
struct spi_pl022_dev_data_t {
    uint32_t state;                        /*!< SPI driver state */
    uint32_t sys_clk;                      /*!< System clock frequency */
    struct spi_pl022_ctrl_cfg_t ctrl_cfg;  /*!< SPI control
                                                configuration data */
};

/* ARM SPI PL022 device structure */
struct spi_pl022_dev_t {
    const struct spi_pl022_dev_cfg_t* const cfg;  /*!< SPI driver
                                                       configuration */
    struct spi_pl022_dev_data_t* const data;      /*!< SPI driver data */
};

enum spi_pl022_error_t {
    SPI_PL022_ERR_NONE = 0,      /*!< No error */
    SPI_PL022_ERR_INVALID_ARGS,  /*!< Invalid input arguments */
    SPI_PL022_ERR_NOT_INIT,      /*!< SPI driver is not initialized */
    SPI_PL022_ERR_NO_TX,         /*!< SPI transm FIFO full */
    SPI_PL022_ERR_NO_RX,         /*!< SPI receive FIFO empty */
    SPI_PL022_ERR_BAD_CONFIG,    /*!< Bad SPI configuration */
};


/* Interrupt mask defines for the interrupt APIs */

/* Receive Overrun Interrupt */
#define SPI_PL022_RX_OR_INTR_POS        0
#define SPI_PL022_RX_OR_INTR_MSK        (0x1ul<<SPI_PL022_RX_OR_INTR_POS)

/* Receive Timeout Interrupt */
#define SPI_PL022_RX_TO_INTR_POS        1
#define SPI_PL022_RX_TO_INTR_MSK        (0x1ul<<SPI_PL022_RX_TO_INTR_POS)

/* Receive FIFO Interrupt */
#define SPI_PL022_RX_FIFO_INTR_POS      2
#define SPI_PL022_RX_FIFO_INTR_MSK      (0x1ul<<SPI_PL022_RX_FIFO_INTR_POS)

/* Transmit FIFO Interrupt */
#define SPI_PL022_TX_FIFO_INTR_POS      3
#define SPI_PL022_TX_FIFO_INTR_MSK      (0x1ul<<SPI_PL022_TX_FIFO_INTR_POS)

#define SPI_PL022_ALL_INTR_MSK          \
                                ((0x1ul<<(SPI_PL022_TX_FIFO_INTR_POS+1))-1)

/* Status register bit defines */

/* Transmit FIFO empty */
#define SPI_PL022_SSPSR_TFE_POS         0
#define SPI_PL022_SSPSR_TFE_MSK         (0x1ul<<SPI_PL022_SSPSR_TFE_POS)

/* Transmit FIFO not full */
#define SPI_PL022_SSPSR_TNF_POS         1
#define SPI_PL022_SSPSR_TNF_MSK         (0x1ul<<SPI_PL022_SSPSR_TNF_POS)

/* Receive FIFO not empty */
#define SPI_PL022_SSPSR_RNE_POS         2
#define SPI_PL022_SSPSR_RNE_MSK         (0x1ul<<SPI_PL022_SSPSR_RNE_POS)

/* Receive FIFO full */
#define SPI_PL022_SSPSR_RFF_POS         3
#define SPI_PL022_SSPSR_RFF_MSK         (0x1ul<<SPI_PL022_SSPSR_RFF_POS)

/* Busy either tx/rx or transmit fifo not empty */
#define SPI_PL022_SSPSR_BSY_POS         4
#define SPI_PL022_SSPSR_BSY_MSK         (0x1ul<<SPI_PL022_SSPSR_BSY_POS)

/**
 * \brief Enables PL022 SPI device
 *
 * \param[in] dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void spi_pl022_dev_enable(struct spi_pl022_dev_t* dev);

/**
 * \brief Disables PL022 SPI device
 *
 * \param[in] dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
void spi_pl022_dev_disable(struct spi_pl022_dev_t* dev);

/**
 * \brief Returns SPI status register
 *
 * \param[in] dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL.
 */
uint32_t spi_pl022_get_status(struct spi_pl022_dev_t* dev);

/**
 * \brief Initializes the SPI PL022 device.
 *
 * \param[in] dev      SPI device structure \ref spi_pl022_dev_t
 * \param[in] sys_clk  System clock.
 *
 * \return Error code from \ref spi_pl022_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum spi_pl022_error_t spi_pl022_init(struct spi_pl022_dev_t* dev,
                                      uint32_t sys_clk);

/**
 * \brief Sets the SPI PL022 device configuration.
 *
 * \param[in] dev       SPI device structure \ref spi_pl022_dev_t
 * \param[in] ctrl_cfg  SPI control configuration.
 *
 * \return  Error code from \ref spi_pl022_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum spi_pl022_error_t spi_pl022_set_ctrl_cfg(struct spi_pl022_dev_t* dev,
                                   const struct spi_pl022_ctrl_cfg_t* ctrl_cfg);

/**
 * \brief Gets the SPI PL022 device configuration.
 *
 * \param[in]  dev       SPI device structure \ref spi_pl022_dev_t
 * \param[out] ctrl_cfg  Pointer to fill the SPI control configuration.
 *
 * \return  Error code from \ref spi_pl022_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum spi_pl022_error_t spi_pl022_get_ctrl_cfg(struct spi_pl022_dev_t* dev,
                                         struct spi_pl022_ctrl_cfg_t* ctrl_cfg);


/**
* \brief Selects SPI PL022 device as Master or Slave
*
* \param[in]  dev       SPI device structure \ref spi_pl022_dev_t
* \param[in]  mode      Mode selection \ref spi_pl022_mode_select_t
*
* \note This function doesn't check if dev is NULL.
*/
void spi_pl022_select_mode(struct spi_pl022_dev_t* dev,
                           enum spi_pl022_mode_select_t mode);

/**
* \brief Enables/disables SPI PL022 Slave device output
*
* \param[in]  dev       SPI device structure \ref spi_pl022_dev_t
* \param[in]  mode      Mode selection \ref spi_pl022_slave_output_mode_t
*
* \note This function doesn't check if dev is NULL.
* \note This function doesn't check if dev is Slave or Master
*/
void spi_pl022_set_slave_output(struct spi_pl022_dev_t* dev,
                                enum spi_pl022_slave_output_mode_t mode);

/**
* \brief Enables SPI PL022 device in loopback mode
*
* \param[in]  dev       SPI device structure \ref spi_pl022_dev_t
* \param[in]  mode      Mode selection \ref spi_pl022_loopback_select_t
*
* \note This function doesn't check if dev is NULL.
*/
void spi_pl022_set_loopback_mode(struct spi_pl022_dev_t* dev,
                                 enum spi_pl022_loopback_select_t mode);

/**
* \brief Clears interrupt mask of SPI PL022
*
* \param[in]  dev           SPI device structure \ref spi_pl022_dev_t
* \param[in]  irq_mask      Selection of interrupts to enable
*
* \note This function doesn't check if dev is NULL.
*/
void spi_pl022_enable_interrupt(struct spi_pl022_dev_t* dev,
                                uint32_t irq_mask);

/**
* \brief Sets interrupt mask of SPI PL022
*
* \param[in]  dev           SPI device structure \ref spi_pl022_dev_t
* \param[in]  irq_mask      Selection of interrupts to disable
*
* \note This function doesn't check if dev is NULL.
*/
void spi_pl022_disable_interrupt(struct spi_pl022_dev_t* dev,
                                 uint32_t irq_mask);

/**
* \brief Gets raw interrupt status of SPI PL022
*
* \param[in]  dev           SPI device structure \ref spi_pl022_dev_t
*
* \return  Returns raw interrupt status value
*
* \note This function doesn't check if dev is NULL.
*/
uint32_t spi_pl022_get_raw_irq_status(struct spi_pl022_dev_t* dev);

/**
* \brief Gets masked interrupt status of SPI PL022
*
* \param[in]  dev           SPI device structure \ref spi_pl022_dev_t
*
* \return  Returns masked interrupt status value
*
* \note This function doesn't check if dev is NULL.
*/
uint32_t spi_pl022_get_masked_irq_status(struct spi_pl022_dev_t* dev);

/**
* \brief Sets interrupt mask of SPI PL022
*
* \param[in]  dev           SPI device structure \ref spi_pl022_dev_t
* \param[in]  irq_mask      Selection of interrupts to disable
*
* \note This function doesn't check if dev is NULL.
*/
void spi_pl022_clear_interrupt(struct spi_pl022_dev_t* dev,
                               uint32_t irq_mask);

/**
* \brief Enables transmit or receive DMA
*
* \param[in]  dev           SPI device structure \ref spi_pl022_dev_t
* \param[in]  dma           Selects the DMA to be enabled
*                           - bit position 0 - Receive DMA
*                           - bit position 1 - Transmit DMA
*
* \note This function doesn't check if dev is NULL.
*/
void spi_pl022_dma_mode_enable(struct spi_pl022_dev_t* dev,
                               uint32_t dma);

/**
* \brief Disables transmit or receive DMA
*
* \param[in]  dev           SPI device structure \ref spi_pl022_dev_t
* \param[in]  dma           Selects the DMA to be disabled
*                           - bit position 0 - Receive DMA
*                           - bit position 1 - Transmit DMA
*
* \note This function doesn't check if dev is NULL.
*/
void spi_pl022_dma_mode_disable(struct spi_pl022_dev_t* dev,
                                uint32_t dma);

/**
 * \brief Gets peripheral identification of the device
 *
 * \param[in]  dev        SPI device structure \ref spi_pl022_dev_t
 * \param[out] periphid   Pointer to fill peripheral ids
 *
 * \note This function doesn't check if dev is NULL.
 */
void spi_pl022_get_periphID(struct spi_pl022_dev_t* dev,
                            struct spi_pl022_periphid_t* periphid);

/**
 * \brief Gets PrimeCell identification of the device
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 * \param[out] cellid   Pointer to fill PrimeCell ids
 *
 * \note This function doesn't check if dev is NULL.
 */
void spi_pl022_get_PrimeCell_ID(struct spi_pl022_dev_t* dev,
                                struct spi_pl022_primecell_id_t* cellid);

/**
 * \brief Sets system clock.
 *
 * \param[in] dev      SPI device structure \ref spi_pl022_dev_t
 * \param[in] sys_clk  System clock.
 *
 * \return  Error code from \ref spi_pl022_error_t
 *
 * \note This function doesn't check if dev is NULL.
 */
enum spi_pl022_error_t spi_pl022_set_sys_clk(struct spi_pl022_dev_t* dev,
                                             uint32_t sys_clk);

/**
 * \brief Reads single data from SPI. Non blocking.
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 * \param[out] rx_ptr   Buffer pointer to be filled
 *                      must be enough for configured word size
 *
 * \return  Error code from \ref spi_pl022_error_t
 *
 * \note This function doesn't check if dev is NULL and
 * if the driver is initialized to reduce the number of checks and
 * make the function execution faster.
 */
enum spi_pl022_error_t spi_pl022_read(struct spi_pl022_dev_t* dev,
                                      void *rx_ptr);

/**
 * \brief Reads single data from slave SPI. Non blocking.
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \return  Returns data value from the device
 *
 * \note This function doesn't check if dev is NULL and
 * does not validate whether there is any data in the RX buffer
 */
uint32_t spi_pl022_slave_read(struct spi_pl022_dev_t* dev);

/**
 * \brief  Writes single data to SPI. Non blocking.
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 * \param[in]  mode     Master or slave \ref spi_pl022_mode_select_t
 * \param[out] tx_ptr   Pointer to the data to be sent
 *
 * \return  Error code from \ref spi_pl022_error_t
 *
 * \note This function doesn't check if dev is NULL and
 * if the driver is initialized to reduce the number of checks and
 * make the function execution faster.
 */
enum spi_pl022_error_t spi_pl022_write(struct spi_pl022_dev_t* dev,
                                       const enum spi_pl022_mode_select_t mode,
                                       const void *tx_ptr);

/**
 * \brief  Transmit and Receive data on SPI in a blocking call
 *
 * \param[in]     dev         SPI device structure \ref spi_pl022_dev_t
 * \param[in]     tx_ptr      Buffer pointer to be filled
 * \param[in/out] tx_len_ptr  Num values to transfer (updated on error)
 *                            need to be multiples of transfer word length
 * \param[out]    rx_ptr      Buffer pointer to be filled
 * \param[in/out] rx_len_ptr  Num values to receive (updated on error)
 *                            need to be multiples of transfer word length
 *
 * \return  Error code from \ref spi_pl022_error_t
 *
 * \note This function doesn't check if dev is NULL and
 * if the driver is initialized to reduce the number of checks and
 * make the function execution faster.
 */
enum spi_pl022_error_t spi_pl022_txrx_blocking(struct spi_pl022_dev_t* dev,
                                               const void *tx_ptr,
                                               uint32_t *tx_len_ptr,
                                               void *rx_ptr,
                                               uint32_t *rx_len_ptr);


/************************** TEST APIs ****************************/

/**
 * \brief  Enables Test FIFO mode
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL
 */
void spi_pl022_test_fifo_enable(struct spi_pl022_dev_t* dev);

/**
 * \brief  Disables Test FIFO mode
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL
 */
void spi_pl022_test_fifo_disable(struct spi_pl022_dev_t* dev);

/**
 * \brief  Enables Integration Test mode
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL
 */
void spi_pl022_integration_test_enable(struct spi_pl022_dev_t* dev);

/**
 * \brief  Disables Integration Test mode
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL
 */
void spi_pl022_integration_test_disable(struct spi_pl022_dev_t* dev);


/**
 * \brief  Writes data to Test data register
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL and
 *       whether Test FIFO mode is enabled
 */
void spi_pl022_write_test_data(struct spi_pl022_dev_t* dev, void *tx_ptr);

/**
 * \brief  Reads integration test output register
 *
 * \param[in]  dev      SPI device structure \ref spi_pl022_dev_t
 *
 * \note This function doesn't check if dev is NULL
 */
uint32_t spi_pl022_read_test_output_reg(struct spi_pl022_dev_t* dev);


#ifdef __cplusplus
}
#endif
#endif /* __SPI_PL022_DRV_H__ */
