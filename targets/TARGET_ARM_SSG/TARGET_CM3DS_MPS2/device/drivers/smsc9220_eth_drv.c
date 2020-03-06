/*
 * Copyright (c) 2016-2019 Arm Limited
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

#include "cmsis.h"
#include "smsc9220_eth_drv.h"

/** Setter bit manipulation macro */
#define SET_BIT(WORD, BIT_INDEX) ((WORD) |= (1U << (BIT_INDEX)))
/** Clearing bit manipulation macro */
#define CLR_BIT(WORD, BIT_INDEX) ((WORD) &= ~(1U << (BIT_INDEX)))
/** Getter bit manipulation macro */
#define GET_BIT(WORD, BIT_INDEX) (bool)(((WORD) & (1U << (BIT_INDEX))))

/** Setter bit-field manipulation macro */
#define SET_BIT_FIELD(WORD, BIT_MASK, BIT_OFFSET, VALUE) \
            (WORD |= ((VALUE & BIT_MASK) << BIT_OFFSET))

/** Clearing bit-field manipulation macro */
#define CLR_BIT_FIELD(WORD, BIT_MASK, BIT_OFFSET, VALUE) \
            (WORD &= ~((VALUE & BIT_MASK) << BIT_OFFSET))

/** Getter bit-field manipulation macro */
#define GET_BIT_FIELD(WORD, BIT_MASK, BIT_OFFSET) \
            ((WORD >> BIT_OFFSET) & BIT_MASK)

/** Millisec timeout macros */
#define RESET_TIME_OUT_MS        10U
#define REG_WRITE_TIME_OUT_MS    50U
#define PHY_RESET_TIME_OUT_MS    100U
#define INIT_FINISH_DELAY        2000U

struct smsc9220_eth_reg_map_t {
__I  uint32_t rx_data_port;      /**< Receive FIFO Ports (offset 0x0) */
     uint32_t reserved1[0x7];
__O  uint32_t tx_data_port;      /**< Transmit FIFO Ports (offset 0x20) */
     uint32_t reserved2[0x7];

__I  uint32_t rx_status_port;   /**< Receive FIFO status port (offset 0x40) */
__I  uint32_t rx_status_peek;   /**< Receive FIFO status peek (offset 0x44) */
__I  uint32_t tx_status_port;   /**< Transmit FIFO status port (offset 0x48) */
__I  uint32_t tx_status_peek;   /**< Transmit FIFO status peek (offset 0x4C) */

__I  uint32_t id_revision;      /**< Chip ID and Revision (offset 0x50) */
__IO uint32_t irq_cfg;          /**< Main Interrupt Config (offset 0x54) */
__IO uint32_t irq_status;       /**< Interrupt Status (offset 0x58) */
__IO uint32_t irq_enable;       /**< Interrupt Enable Register (offset 0x5C) */
     uint32_t reserved3;        /**< Reserved for future use (offset 0x60) */
__I  uint32_t byte_test;        /**< Byte order test 87654321h (offset 0x64) */
__IO uint32_t fifo_level_irq;   /**< FIFO Level Interrupts (offset 0x68) */
__IO uint32_t rx_cfg;           /**< Receive Configuration (offset 0x6C) */
__IO uint32_t tx_cfg;           /**< Transmit Configuration (offset 0x70) */
__IO uint32_t hw_cfg;           /**< Hardware Configuration (offset 0x74) */
__IO uint32_t rx_datapath_ctrl; /**< RX Datapath Control (offset 0x78) */
__I  uint32_t rx_fifo_inf;      /**< Receive FIFO Information (offset 0x7C) */
__I  uint32_t tx_fifo_inf;      /**< Transmit FIFO Information (offset 0x80) */
__IO uint32_t pmt_ctrl;         /**< Power Management Control (offset 0x84) */
__IO uint32_t gpio_cfg;         /**< GPIO Configuration (offset 0x88) */
__IO uint32_t gptimer_cfg;      /**< GP Timer Configuration (offset 0x8C) */
__I  uint32_t gptimer_count;    /**< GP Timer Count (offset 0x90) */
     uint32_t reserved4;        /**< Reserved for future use (offset 0x94) */
__IO uint32_t word_swap;        /**< WORD SWAP Register (offset 0x98) */
__I  uint32_t free_run_counter; /**< Free Run Counter (offset 0x9C) */
__I  uint32_t rx_dropped_frames;/**< RX Dropped Frames Counter (offset 0xA0) */
__IO uint32_t mac_csr_cmd;      /**< MAC CSR Synchronizer Cmd (offset 0xA4) */
__IO uint32_t mac_csr_data;     /**< MAC CSR Synchronizer Data (offset 0xA8) */
__IO uint32_t afc_cfg;          /**< AutomaticFlow Ctrl Config (offset 0xAC) */
__IO uint32_t eeprom_cmd;       /**< EEPROM Command (offset 0xB0) */
__IO uint32_t eeprom_data;      /**< EEPROM Data (offset 0xB4) */
};

/**
 * \brief TX FIFO Size definitions
 *
 */
#define TX_STATUS_FIFO_SIZE_BYTES       512U /*< fixed allocation in bytes */
#define TX_DATA_FIFO_SIZE_KBYTES_POS    16U
#define TX_DATA_FIFO_SIZE_KBYTES_MASK   0x0FU
#define KBYTES_TO_BYTES_MULTIPLIER      1024U

/**
 * \brief FIFO Info definitions
 *
 */
#define FIFO_USED_SPACE_MASK         0xFFFFU
#define DATA_FIFO_USED_SPACE_POS     0U
#define STATUS_FIFO_USED_SPACE_POS   16U

/**
 * \brief MAC CSR Synchronizer Command bit definitions
 *
 */
enum mac_csr_cmd_bits_t{
    MAC_CSR_CMD_RW_INDEX = 30U,
    MAC_CSR_CMD_BUSY_INDEX = 31U,
};

#define MAC_CSR_CMD_ADDRESS_MASK    0x0FU

/**
 * \brief MAC Control register bit definitions
 *
 */
enum mac_reg_cr_bits_t{
    MAC_REG_CR_RXEN_INDEX = 2U,
    MAC_REG_CR_TXEN_INDEX = 3U
};

/**
 * \brief MII Access register bit definitions
 *
 */
enum mac_reg_mii_acc_bits_t{
    MAC_REG_MII_ACC_BUSY_INDEX = 0U,
    MAC_REG_MII_ACC_WRITE_INDEX = 1U,
    MAC_REG_MII_ACC_PHYADDR_INDEX = 11U
};
#define MAC_REG_MII_ACC_MII_REG_MASK    0x1FU
#define MAC_REG_MII_ACC_MII_REG_OFFSET  6U

/**
 * \brief Hardware config register bit definitions
 *
 */
enum hw_cfg_reg_bits_t{
    HW_CFG_REG_SRST_INDEX = 0U,
    HW_CFG_REG_SRST_TIMEOUT_INDEX = 1U,
    HW_CFG_REG_MUST_BE_ONE_INDEX = 20U,
};
#define HW_CFG_REG_TX_FIFO_SIZE_POS     16U
#define HW_CFG_REG_TX_FIFO_SIZE_MIN     2U  /*< Min Tx fifo size in KB */
#define HW_CFG_REG_TX_FIFO_SIZE_MAX     14U /*< Max Tx fifo size in KB */
#define HW_CFG_REG_TX_FIFO_SIZE         5U  /*< Tx fifo size in KB */

/**
 * \brief EEPROM command register bit definitions
 *
 */
enum eeprom_cmd_reg_bits_t{
    EEPROM_CMD_REG_BUSY_INDEX = 31U,
};

/**
 * \brief PHY Basic Control register bit definitions
 *
 */
enum phy_reg_bctrl_reg_bits_t{
    PHY_REG_BCTRL_RST_AUTO_NEG_INDEX = 9U,
    PHY_REG_BCTRL_AUTO_NEG_EN_INDEX = 12U,
    PHY_REG_BCTRL_RESET_INDEX = 15U
};

/**
 * \brief TX Command A bit definitions
 *
 */
#define TX_CMD_DATA_START_OFFSET_BYTES_POS   16U
#define TX_CMD_DATA_START_OFFSET_BYTES_MASK  0x1FU


enum tx_command_a_bits_t{
    TX_COMMAND_A_LAST_SEGMENT_INDEX = 12U,
    TX_COMMAND_A_FIRST_SEGMENT_INDEX = 13U
};

#define TX_CMD_PKT_LEN_BYTES_MASK  0x7FFU
#define TX_CMD_PKT_TAG_MASK        0xFFFFU
#define TX_CMD_PKT_TAG_POS         16U


/**
 * \brief RX Fifo Status bit definitions
 *
 */
enum rx_fifo_status_bits_t{
    RX_FIFO_STATUS_CRC_ERROR_INDEX       = 1U,
    RX_FIFO_STATUS_DRIBBLING_BIT_INDEX   = 2U,
    RX_FIFO_STATUS_MII_ERROR_INDEX       = 3U,
    RX_FIFO_STATUS_REC_WD_TIMEOUT_INDEX  = 4U,
    RX_FIFO_STATUS_FRAME_TYPE_INDEX      = 5U,
    RX_FIFO_STATUS_COLLISION_SEEN_INDEX  = 6U,
    RX_FIFO_STATUS_FRAME_TOO_LONG_INDEX  = 7U,
    RX_FIFO_STATUS_MULTICAST_INDEX       = 10U,
    RX_FIFO_STATUS_RUNT_FRAME_INDEX      = 11U,
    RX_FIFO_STATUS_LENGTH_ERROR_INDEX    = 12U,
    RX_FIFO_STATUS_BROADCAST_FRAME_INDEX = 13U,
    RX_FIFO_STATUS_ERROR_INDEX           = 15U,
    RX_FIFO_STATUS_FILTERING_FAIL_INDEX  = 30U,
};
#define RX_FIFO_STATUS_PKT_LENGTH_POS    16U
#define RX_FIFO_STATUS_PKT_LENGTH_MASK   0x3FFFU

/**
 * \brief Interrupt Configuration register bit definitions
 *
 */
enum irq_cfg_bits_t{
    IRQ_CFG_IRQ_EN_INDEX = 8U
};

#define IRQ_CFG_INT_DEAS_MASK   0xFFU
#define IRQ_CFG_INT_DEAS_POS    24U
#define IRQ_CFG_INT_DEAS_10US   0x22U

/**
 * \brief Automatic Flow Control register bit definitions
 *
 */
enum afc_bits_t{
    AFC_ANY_INDEX = 0U,
    AFC_ADDR_INDEX = 1U,
    AFC_BROADCAST_INDEX = 2U,
    AFC_MULTICAST_INDEX = 3U
};

#define AFC_BACK_DUR_MASK    0x0FU
#define AFC_BACK_DUR_POS     4U
#define AFC_BACK_DUR         4U  /**< equal to 50us */

#define AFC_LOW_LEVEL_MASK   0xFFU
#define AFC_LOW_LEVEL_POS    8U
#define AFC_LOW_LEVEL        55U  /**< specifies in multiple of 64 bytes */

#define AFC_HIGH_LEVEL_MASK  0xFFU
#define AFC_HIGH_LEVEL_POS   16U
#define AFC_HIGH_LEVEL       110U  /**< specifies in multiple of 64 bytes */

/**
 * \brief Auto-Negotiation Advertisement register bit definitions
 *
 */
enum aneg_bits_t{
    ANEG_10_BASE_T_INDEX             = 5U, /**< 10Mbps able */
    ANEG_10_BASE_T_FULL_DUPL_INDEX   = 6U, /**< 10Mbps with full duplex */
    ANEG_100_BASE_TX_INDEX           = 7U, /**< 100Mbps Tx able */
    ANEG_100_BASE_TX_FULL_DUPL_INDEX = 8U, /**< 100Mbps with full duplex */
    ANEG_SYMM_PAUSE_INDEX            = 10U, /**< Symmetric Pause */
    ANEG_ASYMM_PAUSE_INDEX           = 11U /**< Asymmetric Pause */
};

/**
 * \brief Transmit Configuration register bit definitions
 *
 */
enum tx_cfg_bits_t{
    TX_CFG_STOP_INDEX = 0U,      /*< stop */
    TX_CFG_ON_INDEX = 1U,        /*< on */
    TX_CFG_AO_INDEX = 2U,        /*< allow overrun */
    TX_CFG_TXD_DUMP_INDEX = 14U, /*< Data FIFO dump */
    TX_CFG_TXS_DUMP_INDEX = 15U  /*< Status FIFO dump */
};

/**
 * \brief Chip ID definitions
 *
 */
#define CHIP_ID         0x9220U
#define CHIP_ID_MASK    0xFFFFU
#define CHIP_ID_POS     16U

/**
 * \brief GPIO Configuration register bit definitions
 *
 */
enum gpio_cfg_bits_t{
    GPIO_CFG_GPIO0_PUSHPULL_INDEX = 16U, /*< GPIO0 push/pull or open-drain */
    GPIO_CFG_GPIO1_PUSHPULL_INDEX = 17U, /*< GPIO1 push/pull or open-drain */
    GPIO_CFG_GPIO2_PUSHPULL_INDEX = 18U, /*< GPIO2 push/pull or open-drain */
    GPIO_CFG_GPIO0_LED_INDEX = 28U,      /*< GPIO0 set to LED1 */
    GPIO_CFG_GPIO1_LED_INDEX = 29U,      /*< GPIO1 set to LED2 */
    GPIO_CFG_GPIO2_LED_INDEX = 30U       /*< GPIO2 set to LED3 */
};


static void fill_tx_fifo(const struct smsc9220_eth_dev_t* dev,
                               uint8_t *data, uint32_t size_bytes)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    uint32_t tx_data_port_tmp = 0;
    uint8_t *tx_data_port_tmp_ptr = (uint8_t *)&tx_data_port_tmp;

    /*If the data length is not a multiple of 4, then the beginning of the first
     * DWORD of the TX DATA FIFO gets filled up with zeros and a byte offset is
     * set accordingly to guarantee proper transmission.*/
    uint32_t remainder_bytes = (size_bytes % 4);
    uint32_t filler_bytes = (4 - remainder_bytes);
    for(uint32_t i = 0; i < 4; i++){
        if(i < filler_bytes){
            tx_data_port_tmp_ptr[i] = 0;
        } else {
            tx_data_port_tmp_ptr[i] = data[i-filler_bytes];
        }
    }
    register_map->tx_data_port = tx_data_port_tmp;
    size_bytes -= remainder_bytes;
    data += remainder_bytes;

    while (size_bytes > 0) {
        /* Keep the same endianness in data as in the temp variable */
        tx_data_port_tmp_ptr[0] = data[0];
        tx_data_port_tmp_ptr[1] = data[1];
        tx_data_port_tmp_ptr[2] = data[2];
        tx_data_port_tmp_ptr[3] = data[3];
        register_map->tx_data_port = tx_data_port_tmp;
        data += 4;
        size_bytes -= 4;
    }
}

static void empty_rx_fifo(const struct smsc9220_eth_dev_t* dev,
                               uint8_t *data, uint32_t size_bytes)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    uint32_t rx_data_port_tmp = 0;
    uint8_t *rx_data_port_tmp_ptr = (uint8_t *)&rx_data_port_tmp;

    uint32_t remainder_bytes = (size_bytes % 4);
    size_bytes -= remainder_bytes;

    while (size_bytes > 0) {
        /* Keep the same endianness in data as in the temp variable */
        rx_data_port_tmp = register_map->rx_data_port;
        data[0] = rx_data_port_tmp_ptr[0];
        data[1] = rx_data_port_tmp_ptr[1];
        data[2] = rx_data_port_tmp_ptr[2];
        data[3] = rx_data_port_tmp_ptr[3];
        data += 4;
        size_bytes -= 4;
    }

    rx_data_port_tmp = register_map->rx_data_port;
    for(uint32_t i = 0; i < remainder_bytes; i++) {
        data[i] = rx_data_port_tmp_ptr[i];
    }
}

enum smsc9220_error_t smsc9220_mac_regread(
        const struct smsc9220_eth_dev_t* dev,
        enum smsc9220_mac_reg_offsets_t regoffset,
        uint32_t *data)
{
    volatile uint32_t val;
    uint32_t maccmd = GET_BIT_FIELD(regoffset,
                                        MAC_CSR_CMD_ADDRESS_MASK, 0);
    uint32_t time_out = REG_WRITE_TIME_OUT_MS;

    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    /* Make sure there's no pending operation */
    if(!(GET_BIT(register_map->mac_csr_cmd, MAC_CSR_CMD_BUSY_INDEX))) {
        SET_BIT(maccmd, MAC_CSR_CMD_RW_INDEX);
        SET_BIT(maccmd, MAC_CSR_CMD_BUSY_INDEX);
        register_map->mac_csr_cmd = maccmd;    /* Start operation */

        do {
            val = register_map->byte_test;  /* A no-op read. */
            (void)val;
            if (dev->data->wait_ms) {
                dev->data->wait_ms(1);
            }
            time_out--;
        } while(time_out &&
                GET_BIT(register_map->mac_csr_cmd,MAC_CSR_CMD_BUSY_INDEX));

        if(!time_out) {
            return SMSC9220_ERROR_TIMEOUT;
        }
        else {
            *data = register_map->mac_csr_data;
        }
    } else {
        return SMSC9220_ERROR_BUSY;
    }
    return SMSC9220_ERROR_NONE;
}

enum smsc9220_error_t  smsc9220_mac_regwrite(
        const struct smsc9220_eth_dev_t* dev,
        enum smsc9220_mac_reg_offsets_t regoffset,
        uint32_t data)
{
    volatile uint32_t read = 0;
    uint32_t maccmd = GET_BIT_FIELD(regoffset,
                                        MAC_CSR_CMD_ADDRESS_MASK, 0);
    uint32_t time_out = REG_WRITE_TIME_OUT_MS;

    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    /* Make sure there's no pending operation */
    if(!GET_BIT(register_map->mac_csr_cmd, MAC_CSR_CMD_BUSY_INDEX)) {
        register_map->mac_csr_data = data;      /* Store data. */
        CLR_BIT(maccmd, MAC_CSR_CMD_RW_INDEX);
        SET_BIT(maccmd, MAC_CSR_CMD_BUSY_INDEX);
        register_map->mac_csr_cmd = maccmd;

        do {
            read = register_map->byte_test;     /* A no-op read. */
            (void)read;
            if (dev->data->wait_ms) {
                dev->data->wait_ms(1);
            }
            time_out--;
        } while(time_out &&
                (register_map->mac_csr_cmd &
                 GET_BIT(register_map->mac_csr_cmd, MAC_CSR_CMD_BUSY_INDEX)));

        if(!time_out) {
            return SMSC9220_ERROR_TIMEOUT;
        }
    } else {
       return SMSC9220_ERROR_BUSY;
    }
    return SMSC9220_ERROR_NONE;
}

enum smsc9220_error_t smsc9220_phy_regread(
        const struct smsc9220_eth_dev_t* dev,
        enum phy_reg_offsets_t regoffset,
        uint32_t *data)
{
    uint32_t val = 0;
    uint32_t phycmd = 0;
    uint32_t time_out = REG_WRITE_TIME_OUT_MS;

    if (smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_MII_ACC, &val)) {
        return SMSC9220_ERROR_INTERNAL;
    }

    if(!GET_BIT(val, MAC_REG_MII_ACC_BUSY_INDEX)) {
        phycmd = 0;
        SET_BIT(phycmd, MAC_REG_MII_ACC_PHYADDR_INDEX);
        SET_BIT_FIELD(phycmd, MAC_REG_MII_ACC_MII_REG_MASK,
                      MAC_REG_MII_ACC_MII_REG_OFFSET, regoffset);
        CLR_BIT(phycmd, MAC_REG_MII_ACC_WRITE_INDEX);
        SET_BIT(phycmd, MAC_REG_MII_ACC_BUSY_INDEX);

        if (smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_MII_ACC,
                                  phycmd)) {
            return SMSC9220_ERROR_INTERNAL;
        }

        val = 0;
        do {
            if (dev->data->wait_ms) {
                dev->data->wait_ms(1);
            }
            time_out--;
            if (smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_MII_ACC,
                &val)) {
                return SMSC9220_ERROR_INTERNAL;
            }
        } while(time_out && (GET_BIT(val, MAC_REG_MII_ACC_BUSY_INDEX)));

        if (!time_out) {
            return SMSC9220_ERROR_TIMEOUT;
        } else if (smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_MII_DATA,
                   data)) {
            return SMSC9220_ERROR_INTERNAL;
        }
    } else {
        return SMSC9220_ERROR_BUSY;
    }
    return SMSC9220_ERROR_NONE;
}

enum smsc9220_error_t smsc9220_phy_regwrite(
        const struct smsc9220_eth_dev_t* dev,
        enum phy_reg_offsets_t regoffset,
        uint32_t data)
{
    uint32_t val = 0;
    uint32_t phycmd = 0;
    uint32_t time_out = REG_WRITE_TIME_OUT_MS;

    if (smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_MII_ACC, &val)) {
        return SMSC9220_ERROR_INTERNAL;
    }

    if(!GET_BIT(val, MAC_REG_MII_ACC_BUSY_INDEX)) {
        /* Load the data */
        if (smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_MII_DATA,
                                 (data & 0xFFFF))) {
            return SMSC9220_ERROR_INTERNAL;
        }
        phycmd = 0;
        SET_BIT(phycmd, MAC_REG_MII_ACC_PHYADDR_INDEX);
        SET_BIT_FIELD(phycmd, MAC_REG_MII_ACC_MII_REG_MASK,
                      MAC_REG_MII_ACC_MII_REG_OFFSET, regoffset);
        SET_BIT(phycmd, MAC_REG_MII_ACC_WRITE_INDEX);
        SET_BIT(phycmd, MAC_REG_MII_ACC_BUSY_INDEX);
        /* Start operation */
        if (smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_MII_ACC,
                                  phycmd)) {
            return SMSC9220_ERROR_INTERNAL;
        }

        phycmd = 0;

        do {
            if (dev->data->wait_ms) {
                dev->data->wait_ms(1);
            }
            time_out--;
            if (smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_MII_ACC,
                                     &phycmd)){
                return SMSC9220_ERROR_INTERNAL;
            }
        } while(time_out &&  GET_BIT(phycmd, 0));

        if (!time_out) {
            return SMSC9220_ERROR_TIMEOUT;
        }

    } else {
        return SMSC9220_ERROR_BUSY;
    }
    return SMSC9220_ERROR_NONE;
}

uint32_t smsc9220_read_id(const struct smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    return register_map->id_revision;
}

enum smsc9220_error_t  smsc9220_soft_reset(
        const struct smsc9220_eth_dev_t* dev)
{
    uint32_t time_out = RESET_TIME_OUT_MS;

    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    /* Soft reset */
    SET_BIT(register_map->hw_cfg, HW_CFG_REG_SRST_INDEX);

    do {
        if (dev->data->wait_ms) {
            dev->data->wait_ms(1);
        }
        time_out--;
    } while(time_out &&
            GET_BIT(register_map->hw_cfg, HW_CFG_REG_SRST_TIMEOUT_INDEX));

    if (!time_out) {
        return SMSC9220_ERROR_TIMEOUT;
    }

    return SMSC9220_ERROR_NONE;
}

void smsc9220_set_txfifo(const struct smsc9220_eth_dev_t* dev,
                         uint32_t val)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    if(val >= HW_CFG_REG_TX_FIFO_SIZE_MIN &&
       val <= HW_CFG_REG_TX_FIFO_SIZE_MAX) {
       register_map->hw_cfg = val << HW_CFG_REG_TX_FIFO_SIZE_POS;
    }
}

enum smsc9220_error_t smsc9220_set_fifo_level_irq(
        const struct smsc9220_eth_dev_t* dev,
        enum smsc9220_fifo_level_irq_pos_t irq_level_pos,
        uint32_t level)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    if (level < SMSC9220_FIFO_LEVEL_IRQ_LEVEL_MIN ||
        level > SMSC9220_FIFO_LEVEL_IRQ_LEVEL_MAX) {
        return SMSC9220_ERROR_PARAM;
    }

    CLR_BIT_FIELD(register_map->fifo_level_irq, SMSC9220_FIFO_LEVEL_IRQ_MASK,
                  irq_level_pos, SMSC9220_FIFO_LEVEL_IRQ_MASK);
    SET_BIT_FIELD(register_map->fifo_level_irq, SMSC9220_FIFO_LEVEL_IRQ_MASK,
                  irq_level_pos, level);
    return SMSC9220_ERROR_NONE;
}

enum smsc9220_error_t smsc9220_wait_eeprom(
        const struct smsc9220_eth_dev_t* dev)
{
    uint32_t time_out = REG_WRITE_TIME_OUT_MS;

    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    do {
        if (dev->data->wait_ms) {
            dev->data->wait_ms(1);
        }
        time_out--;
    } while(time_out &&
            GET_BIT(register_map->eeprom_cmd, EEPROM_CMD_REG_BUSY_INDEX));

    if (!time_out) {
        return SMSC9220_ERROR_TIMEOUT;
    }

    return SMSC9220_ERROR_NONE;
}

void smsc9220_init_irqs(const struct smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    smsc9220_disable_all_interrupts(dev);
    smsc9220_clear_all_interrupts(dev);

    /* Set IRQ deassertion interval */
    SET_BIT_FIELD(register_map->irq_cfg, IRQ_CFG_INT_DEAS_MASK,
                  IRQ_CFG_INT_DEAS_POS, IRQ_CFG_INT_DEAS_10US);

    /* enable interrupts */
    SET_BIT(register_map->irq_cfg, IRQ_CFG_IRQ_EN_INDEX);
}

enum smsc9220_error_t smsc9220_check_phy(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t phyid1 = 0;
    uint32_t phyid2 = 0;

    if (smsc9220_phy_regread(dev, SMSC9220_PHY_REG_OFFSET_ID1,&phyid1)) {
        return SMSC9220_ERROR_INTERNAL;
    }
    if (smsc9220_phy_regread(dev, SMSC9220_PHY_REG_OFFSET_ID2,&phyid2)) {
        return SMSC9220_ERROR_INTERNAL;
    }
    if ((phyid1 == 0xFFFF && phyid2 == 0xFFFF) ||
            (phyid1 == 0x0 && phyid2 == 0x0)) {
        return SMSC9220_ERROR_INTERNAL;
    }
    return SMSC9220_ERROR_NONE;
}

enum smsc9220_error_t smsc9220_reset_phy(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t read = 0;

    if(smsc9220_phy_regread(dev, SMSC9220_PHY_REG_OFFSET_BCTRL, &read)) {
        return SMSC9220_ERROR_INTERNAL;
    }

    SET_BIT(read, PHY_REG_BCTRL_RESET_INDEX);
    if(smsc9220_phy_regwrite(dev, SMSC9220_PHY_REG_OFFSET_BCTRL, read)) {
        return SMSC9220_ERROR_INTERNAL;
    }
    return SMSC9220_ERROR_NONE;
}

void smsc9220_advertise_cap(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t aneg_adv = 0;
    smsc9220_phy_regread(dev, SMSC9220_PHY_REG_OFFSET_ANEG_ADV, &aneg_adv);

    SET_BIT(aneg_adv, ANEG_10_BASE_T_INDEX);
    SET_BIT(aneg_adv, ANEG_10_BASE_T_FULL_DUPL_INDEX);
    SET_BIT(aneg_adv, ANEG_100_BASE_TX_INDEX);
    SET_BIT(aneg_adv, ANEG_100_BASE_TX_FULL_DUPL_INDEX);
    SET_BIT(aneg_adv, ANEG_SYMM_PAUSE_INDEX);
    SET_BIT(aneg_adv, ANEG_ASYMM_PAUSE_INDEX);

    smsc9220_phy_regwrite(dev, SMSC9220_PHY_REG_OFFSET_ANEG_ADV, aneg_adv);
}

void smsc9220_enable_xmit(const struct smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    SET_BIT(register_map->tx_cfg, TX_CFG_ON_INDEX);
}

void smsc9220_disable_xmit(const struct smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    CLR_BIT(register_map->tx_cfg, TX_CFG_ON_INDEX);
}

void smsc9220_enable_mac_xmit(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t mac_cr = 0;
    smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_CR, &mac_cr);

    SET_BIT(mac_cr, MAC_REG_CR_TXEN_INDEX);

    smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_CR, mac_cr);
}

void smsc9220_disable_mac_xmit(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t mac_cr = 0;
    smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_CR, &mac_cr);

    CLR_BIT(mac_cr, MAC_REG_CR_TXEN_INDEX);

    smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_CR, mac_cr);
}

void smsc9220_enable_mac_recv(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t mac_cr = 0;
    smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_CR, &mac_cr);

    SET_BIT(mac_cr, MAC_REG_CR_RXEN_INDEX);

    smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_CR, mac_cr);
}

void smsc9220_disable_mac_recv(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t mac_cr = 0;
    smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_CR, &mac_cr);

    CLR_BIT(mac_cr, MAC_REG_CR_RXEN_INDEX);

    smsc9220_mac_regwrite(dev, SMSC9220_MAC_REG_OFFSET_CR, mac_cr);
}

int smsc9220_check_id(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t id = smsc9220_read_id(dev);

    return ((GET_BIT_FIELD(id, CHIP_ID_MASK, CHIP_ID_POS) == CHIP_ID) ? 0 : 1);
}

void smsc9220_enable_interrupt(const struct smsc9220_eth_dev_t* dev,
                               enum smsc9220_interrupt_source source)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    SET_BIT(register_map->irq_enable, source);
}

void smsc9220_disable_interrupt(const struct smsc9220_eth_dev_t* dev,
                                enum smsc9220_interrupt_source source)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    CLR_BIT(register_map->irq_enable, source);
}

void smsc9220_disable_all_interrupts(const struct smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    register_map->irq_enable = 0;
}

void smsc9220_clear_interrupt(const struct smsc9220_eth_dev_t* dev,
                              enum smsc9220_interrupt_source source)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    SET_BIT(register_map->irq_status, source);
}

void smsc9220_clear_all_interrupts(const struct smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    register_map->irq_status = UINT32_MAX;
}

int smsc9220_get_interrupt(const struct smsc9220_eth_dev_t* dev,
                           enum smsc9220_interrupt_source source)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    return GET_BIT(register_map->irq_status, source);
}

void smsc9220_establish_link(const struct smsc9220_eth_dev_t* dev)
{
    uint32_t bcr = 0;
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    smsc9220_phy_regread(dev, SMSC9220_PHY_REG_OFFSET_BCTRL, &bcr);
    SET_BIT(bcr, PHY_REG_BCTRL_AUTO_NEG_EN_INDEX);
    SET_BIT(bcr, PHY_REG_BCTRL_RST_AUTO_NEG_INDEX);
    smsc9220_phy_regwrite(dev, SMSC9220_PHY_REG_OFFSET_BCTRL, bcr);

    SET_BIT(register_map->hw_cfg, HW_CFG_REG_MUST_BE_ONE_INDEX);
}

enum smsc9220_error_t smsc9220_read_mac_address(
        const struct smsc9220_eth_dev_t* dev, char *mac)
{
    uint32_t mac_low = 0;
    uint32_t mac_high = 0;

    if(!mac) {
        return SMSC9220_ERROR_PARAM;
    }

    if (smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_ADDRH, &mac_high)) {
        return SMSC9220_ERROR_INTERNAL;
    }
    if (smsc9220_mac_regread(dev, SMSC9220_MAC_REG_OFFSET_ADDRL, &mac_low)) {
        return SMSC9220_ERROR_INTERNAL;
    }
    mac[0] = mac_low & 0xFF;
    mac[1] = (mac_low >> 8) & 0xFF;
    mac[2] = (mac_low >> 16) & 0xFF;
    mac[3] = (mac_low >> 24) & 0xFF;
    mac[4] = mac_high & 0xFF;
    mac[5] = (mac_high >> 8) & 0xFF;

    return SMSC9220_ERROR_NONE;
}

uint32_t smsc9220_get_tx_data_fifo_size(
        const struct smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    uint32_t tx_fifo_size =
            GET_BIT_FIELD(register_map->hw_cfg,
                    TX_DATA_FIFO_SIZE_KBYTES_MASK,
                    TX_DATA_FIFO_SIZE_KBYTES_POS) * KBYTES_TO_BYTES_MULTIPLIER;

    return (tx_fifo_size - TX_STATUS_FIFO_SIZE_BYTES);
}

enum smsc9220_error_t smsc9220_init(
        const struct smsc9220_eth_dev_t* dev,
        void(* wait_ms_function)(uint32_t))
{
    uint32_t phyreset = 0;
    enum smsc9220_error_t error = SMSC9220_ERROR_NONE;
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    if (!wait_ms_function) {
        return SMSC9220_ERROR_PARAM;
    }
    dev->data->wait_ms = wait_ms_function;

    error = smsc9220_check_id(dev);
    if(error != SMSC9220_ERROR_NONE) {
        return error;
    }

    error = smsc9220_soft_reset(dev);
    if(error != SMSC9220_ERROR_NONE) {
        return error;
    }

    smsc9220_set_txfifo(dev, HW_CFG_REG_TX_FIFO_SIZE);

    SET_BIT_FIELD(register_map->afc_cfg, AFC_BACK_DUR_MASK,
                  AFC_BACK_DUR_POS, AFC_BACK_DUR);
    SET_BIT_FIELD(register_map->afc_cfg, AFC_LOW_LEVEL_MASK,
                  AFC_LOW_LEVEL_POS, AFC_LOW_LEVEL);
    SET_BIT_FIELD(register_map->afc_cfg, AFC_HIGH_LEVEL_MASK,
                  AFC_HIGH_LEVEL_POS, AFC_HIGH_LEVEL);

    error = smsc9220_wait_eeprom(dev);
    if(error != SMSC9220_ERROR_NONE) {
        return error;
    }

    /* Configure GPIOs as LED outputs. */
    register_map->gpio_cfg = 0;
    SET_BIT(register_map->gpio_cfg, GPIO_CFG_GPIO0_PUSHPULL_INDEX);
    SET_BIT(register_map->gpio_cfg, GPIO_CFG_GPIO1_PUSHPULL_INDEX);
    SET_BIT(register_map->gpio_cfg, GPIO_CFG_GPIO2_PUSHPULL_INDEX);
    SET_BIT(register_map->gpio_cfg, GPIO_CFG_GPIO0_LED_INDEX);
    SET_BIT(register_map->gpio_cfg, GPIO_CFG_GPIO1_LED_INDEX);
    SET_BIT(register_map->gpio_cfg, GPIO_CFG_GPIO2_LED_INDEX);

    smsc9220_init_irqs(dev);

    /* Configure MAC addresses here if needed. */

    error = smsc9220_check_phy(dev);
    if(error != SMSC9220_ERROR_NONE) {
        return error;
    }

    error = smsc9220_reset_phy(dev);
    if(error != SMSC9220_ERROR_NONE) {
        return error;
    }

    if (dev->data->wait_ms) {
        dev->data->wait_ms(PHY_RESET_TIME_OUT_MS);
    }
    /* Checking whether phy reset completed successfully.*/
    error = smsc9220_phy_regread(dev, SMSC9220_PHY_REG_OFFSET_BCTRL,
                                &phyreset);
    if(error != SMSC9220_ERROR_NONE) {
        return error;
    }

    if(GET_BIT(phyreset, PHY_REG_BCTRL_RESET_INDEX)) {
        return SMSC9220_ERROR_INTERNAL;
    }

    smsc9220_advertise_cap(dev);
    smsc9220_establish_link(dev);

    smsc9220_enable_mac_xmit(dev);
    smsc9220_enable_xmit(dev);
    smsc9220_enable_mac_recv(dev);

    /* This sleep is compulsory otherwise txmit/receive will fail. */
    if (dev->data->wait_ms) {
        dev->data->wait_ms(INIT_FINISH_DELAY);
    }
    dev->data->state = 1;

    return SMSC9220_ERROR_NONE;
}

enum smsc9220_error_t smsc9220_send_by_chunks(
                            const struct smsc9220_eth_dev_t* dev,
                            uint32_t total_payload_length,
                            bool is_new_packet,
                            const char *data, uint32_t current_size)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;
    bool is_first_segment = false;
    bool is_last_segment = false;
    uint32_t txcmd_a, txcmd_b = 0;
    uint32_t tx_buffer_free_space = 0;
    volatile uint32_t xmit_stat = 0;

    if (!data) {
        return SMSC9220_ERROR_PARAM;
    }

    if (is_new_packet) {
        is_first_segment = true;
        dev->data->ongoing_packet_length = total_payload_length;
        dev->data->ongoing_packet_length_sent = 0;
    } else if (dev->data->ongoing_packet_length != total_payload_length ||
             dev->data->ongoing_packet_length_sent >= total_payload_length) {
        return SMSC9220_ERROR_PARAM;
    }

    /* Would next chunk fit into buffer? */
    tx_buffer_free_space = GET_BIT_FIELD(register_map->tx_fifo_inf,
                                         FIFO_USED_SPACE_MASK,
                                         DATA_FIFO_USED_SPACE_POS);
    if (current_size > tx_buffer_free_space) {
        return SMSC9220_ERROR_INTERNAL; /* Not enough space in FIFO */
    }
    if ((dev->data->ongoing_packet_length_sent + current_size) ==
         total_payload_length) {
        is_last_segment = true;
    }

    txcmd_a = 0;
    txcmd_b = 0;

    if (is_last_segment) {
        SET_BIT(txcmd_a, TX_COMMAND_A_LAST_SEGMENT_INDEX);
    }
    if (is_first_segment) {
        SET_BIT(txcmd_a, TX_COMMAND_A_FIRST_SEGMENT_INDEX);
    }

    uint32_t data_start_offset_bytes = (4 - (current_size % 4));

    SET_BIT_FIELD(txcmd_a, TX_CMD_PKT_LEN_BYTES_MASK, 0, current_size);
    SET_BIT_FIELD(txcmd_a, TX_CMD_DATA_START_OFFSET_BYTES_MASK,
                           TX_CMD_DATA_START_OFFSET_BYTES_POS,
                           data_start_offset_bytes);

    SET_BIT_FIELD(txcmd_b, TX_CMD_PKT_LEN_BYTES_MASK, 0, current_size);
    SET_BIT_FIELD(txcmd_b, TX_CMD_PKT_TAG_MASK, TX_CMD_PKT_TAG_POS,
                  current_size);

    register_map->tx_data_port = txcmd_a;
    register_map->tx_data_port = txcmd_b;

    fill_tx_fifo(dev, (uint8_t *)data, current_size);

    if (is_last_segment) {
        /* Pop status port for error check */
        xmit_stat = register_map->tx_status_port;
        (void)xmit_stat;
    }
    dev->data->ongoing_packet_length_sent += current_size;
    return SMSC9220_ERROR_NONE;
}

uint32_t smsc9220_get_rxfifo_data_used_space(const struct
                                                 smsc9220_eth_dev_t* dev)
{
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    return GET_BIT_FIELD(register_map->rx_fifo_inf, FIFO_USED_SPACE_MASK,
                         DATA_FIFO_USED_SPACE_POS);
}

uint32_t smsc9220_receive_by_chunks(const struct smsc9220_eth_dev_t* dev,
                                        char *data, uint32_t dlen)
{
    uint32_t rxfifo_inf = 0;
    uint32_t rxfifo_stat = 0;
    uint32_t packet_length_byte = 0;
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    if (!data) {
        return 0; /* Invalid input parameter, cannot read */
    }
    rxfifo_inf = register_map->rx_fifo_inf;

    if(rxfifo_inf & 0xFFFF) { /* If there's data */
        rxfifo_stat = register_map->rx_status_port;
        if(rxfifo_stat != 0) {   /* Fetch status of this packet */
                /* Ethernet controller is padding to 32bit aligned data */
                packet_length_byte = GET_BIT_FIELD(rxfifo_stat,
                                        RX_FIFO_STATUS_PKT_LENGTH_MASK,
                                        RX_FIFO_STATUS_PKT_LENGTH_POS);
                packet_length_byte -= 4;
                dev->data->current_rx_size_words = packet_length_byte;
        }
    }

    empty_rx_fifo(dev, (uint8_t *)data, packet_length_byte);
    dev->data->current_rx_size_words = 0;
    return packet_length_byte;
}

uint32_t smsc9220_peek_next_packet_size(const struct
                                            smsc9220_eth_dev_t* dev)
{
    uint32_t packet_size = 0;
    struct smsc9220_eth_reg_map_t* register_map =
            (struct smsc9220_eth_reg_map_t*)dev->cfg->base;

    if(smsc9220_get_rxfifo_data_used_space(dev)) {
        packet_size = GET_BIT_FIELD(register_map->rx_status_peek,
                                    RX_FIFO_STATUS_PKT_LENGTH_MASK,
                                    RX_FIFO_STATUS_PKT_LENGTH_POS);
    }
    return packet_size;
}
