/*
 * Copyright (c) 2016-2018 ARM Limited
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
 * \file smsc9220_drv.h
 * \brief Generic driver for SMSC9220 Ethernet controller
 */

#ifndef __SMSC9220_ETH_H__
#define __SMSC9220_ETH_H__

#include "stdbool.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/** SMSC9220 device configuration structure */
struct smsc9220_eth_dev_cfg_t {
    const uint32_t base;              /*!< SMSC9220 base address */
};

/** SMSC9220 device data structure */
struct smsc9220_eth_dev_data_t {
    uint32_t state;       /*!< Indicates if the SMSC9220 driver
                               is initialized and enabled */
    void (*wait_ms) (int);/*!< function pointer to system's millisec delay
                               function, will be used for delays */
    uint32_t ongoing_packet_length;/*!< size in bytes of the packet
                                            is being sent */
    uint32_t ongoing_packet_length_sent; /*!< size in bytes of the packet
                                                  has been sent */
    uint32_t current_rx_size_words; /*!< Data length in words,
                                             currently is being read */
};

/** SMSC9220 device structure */
struct smsc9220_eth_dev_t {
    const struct smsc9220_eth_dev_cfg_t* const cfg; /*!< configuration */
    struct smsc9220_eth_dev_data_t* const data;     /*!< data */
};

/**
 * \brief Error code definitions
 *
 */
enum smsc9220_error_t{
    SMSC9220_ERROR_NONE     = 0U, /*!< no error */
    SMSC9220_ERROR_TIMEOUT  = 1U, /*!< timeout */
    SMSC9220_ERROR_BUSY     = 2U, /*!< no error */
    SMSC9220_ERROR_PARAM    = 3U, /*!< invalid parameter */
    SMSC9220_ERROR_INTERNAL = 4U  /*!< internal error */
};

/**
 * \brief Interrupt source definitions
 *
 */
enum smsc9220_interrupt_source {
    SMSC9220_INTERRUPT_GPIO0                  = 0U,
    SMSC9220_INTERRUPT_GPIO1                  = 1U,
    SMSC9220_INTERRUPT_GPIO2                  = 2U,
    SMSC9220_INTERRUPT_RX_STATUS_FIFO_LEVEL   = 3U,
    SMSC9220_INTERRUPT_RX_STATUS_FIFO_FULL    = 4U,
    /* 5 Reserved according to Datasheet */
    SMSC9220_INTERRUPT_RX_DROPPED_FRAME       = 6U,
    SMSC9220_INTERRUPT_TX_STATUS_FIFO_LEVEL   = 7U,
    SMSC9220_INTERRUPT_TX_STATUS_FIFO_FULL    = 8U,
    SMSC9220_INTERRUPT_TX_DATA_FIFO_AVAILABLE = 9U,
    SMSC9220_INTERRUPT_TX_DATA_FIFO_OVERRUN   = 10U,
    /* 11, 12 Reserved according to Datasheet */
    SMSC9220_INTERRUPT_TX_ERROR               = 13U,
    SMSC9220_INTERRUPT_RX_ERROR               = 14U,
    SMSC9220_INTERRUPT_RX_WATCHDOG_TIMEOUT    = 15U,
    SMSC9220_INTERRUPT_TX_STATUS_OVERFLOW     = 16U,
    SMSC9220_INTERRUPT_TX_POWER_MANAGEMENT    = 17U,
    SMSC9220_INTERRUPT_PHY                    = 18U,
    SMSC9220_INTERRUPT_GP_TIMER               = 19U,
    SMSC9220_INTERRUPT_RX_DMA                 = 20U,
    SMSC9220_INTERRUPT_TX_IOC                 = 21U,
    /* 22 Reserved according to Datasheet*/
    SMSC9220_INTERRUPT_RX_DROPPED_FRAME_HALF  = 23U,
    SMSC9220_INTERRUPT_RX_STOPPED             = 24U,
    SMSC9220_INTERRUPT_TX_STOPPED             = 25U,
    /* 26 - 30 Reserved according to Datasheet*/
    SMSC9220_INTERRUPT_SW                     = 31U
};

/**
 * \brief MAC register offset definitions
 *
 */
enum smsc9220_mac_reg_offsets_t{
    SMSC9220_MAC_REG_OFFSET_CR =       0x1U,
    SMSC9220_MAC_REG_OFFSET_ADDRH =    0x2U,
    SMSC9220_MAC_REG_OFFSET_ADDRL =    0x3U,
    SMSC9220_MAC_REG_OFFSET_HASHH =    0x4U,
    SMSC9220_MAC_REG_OFFSET_HASHL =    0x5U,
    SMSC9220_MAC_REG_OFFSET_MII_ACC =  0x6U,
    SMSC9220_MAC_REG_OFFSET_MII_DATA = 0x7U,
    SMSC9220_MAC_REG_OFFSET_FLOW =     0x8U,
    SMSC9220_MAC_REG_OFFSET_VLAN1 =    0x9U,
    SMSC9220_MAC_REG_OFFSET_VLAN2 =    0xAU,
    SMSC9220_MAC_REG_OFFSET_WUFF =     0xBU,
    SMSC9220_MAC_REG_OFFSET_WUCSR =    0xCU,
    SMSC9220_MAC_REG_OFFSET_COE_CR =   0xDU
};

/**
 * \brief PHY register offset definitions
 *
 */
enum phy_reg_offsets_t{
    SMSC9220_PHY_REG_OFFSET_BCTRL =      0U,
    SMSC9220_PHY_REG_OFFSET_BSTATUS =    1U,
    SMSC9220_PHY_REG_OFFSET_ID1 =        2U,
    SMSC9220_PHY_REG_OFFSET_ID2 =        3U,
    SMSC9220_PHY_REG_OFFSET_ANEG_ADV =   4U,
    SMSC9220_PHY_REG_OFFSET_ANEG_LPA =   5U,
    SMSC9220_PHY_REG_OFFSET_ANEG_EXP =   6U,
    SMSC9220_PHY_REG_OFFSET_MCONTROL =   17U,
    SMSC9220_PHY_REG_OFFSET_MSTATUS =    18U,
    SMSC9220_PHY_REG_OFFSET_CSINDICATE = 27U,
    SMSC9220_PHY_REG_OFFSET_INTSRC =     29U,
    SMSC9220_PHY_REG_OFFSET_INTMASK =    30U,
    SMSC9220_PHY_REG_OFFSET_CS =         31U
};

/* Bit definitions for PHY Basic Status Register */
#define PHY_REG_BSTATUS_EXTENDED_CAPABILITIES_INDEX     0U
#define PHY_REG_BSTATUS_JABBER_DETECT_INDEX             1U
#define PHY_REG_BSTATUS_LINK_STATUS_INDEX               2U
#define PHY_REG_BSTATUS_AUTO_NEG_ABILITY_INDEX          3U
#define PHY_REG_BSTATUS_REMOTE_FAULT_INDEX              4U
#define PHY_REG_BSTATUS_AUTO_NEG_COMPLETE_INDEX         5U
#define PHY_REG_BSTATUS_10BASE_T_HALF_DUPLEX_INDEX      11U
#define PHY_REG_BSTATUS_10BASE_T_FULL_DUPLEX_INDEX      12U
#define PHY_REG_BSTATUS_100BASE_TX_HALF_DUPLEX_INDEX    13U
#define PHY_REG_BSTATUS_100BASE_TX_FULL_DUPLEX_INDEX    14U
#define PHY_REG_BSTATUS_100BASE_T4_INDEX                15U

/**
 * \brief FIFO Level Interrupt bit definitions
 *
 */
enum smsc9220_fifo_level_irq_pos_t{
    SMSC9220_FIFO_LEVEL_IRQ_RX_STATUS_POS = 0U,
    SMSC9220_FIFO_LEVEL_IRQ_TX_STATUS_POS = 16U,
    SMSC9220_FIFO_LEVEL_IRQ_TX_DATA_POS = 24U
};

/**
 * \brief FIFO Level Interrupt limits
 *
 */
#define SMSC9220_FIFO_LEVEL_IRQ_MASK             0xFFU
#define SMSC9220_FIFO_LEVEL_IRQ_LEVEL_MIN        0U
#define SMSC9220_FIFO_LEVEL_IRQ_LEVEL_MAX        SMSC9220_FIFO_LEVEL_IRQ_MASK

/**
 * \brief Initializes SMSC9220 Ethernet controller to a known default state:
 *          - device ID is checked
 *          - global interrupt is enabled, but all irq sources are disabled
 *          - all capabilities are advertised
 *              - 10Mbps able
 *              - 10Mbps with full duplex
 *              - 100Mbps Tx able
 *              - 100Mbps with full duplex
 *              - Symmetric Pause
 *              - Asymmetric Pause
 *          - Establish link enabled
 *          - Rx enabled
 *          - Tx enabled
 *        Init should be called prior to any other process and
 *        it's the caller's responsibility to follow proper call order.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] wait_ms_function function pointer to a millisec delay function
 *                             for proper timing of some processes
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_init(const struct smsc9220_eth_dev_t* dev,
                               void(* wait_ms_function)(int));

/**
 * \brief Read MAC register.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] regoffset Register offset
 * \param[in, out] data Pointer to register will be read
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_mac_regread(
        const struct smsc9220_eth_dev_t* dev,
        enum smsc9220_mac_reg_offsets_t regoffset,
        uint32_t *data);

/**
 * \brief Write MAC register.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] regoffset Register offset
 * \param[in] data Register value to write
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_mac_regwrite(
        const struct smsc9220_eth_dev_t* dev,
        enum smsc9220_mac_reg_offsets_t regoffset,
        uint32_t data);

/**
 * \brief Read PHY register.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] regoffset Register offset
 * \param[out] data Register value is read
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_phy_regread(
        const struct smsc9220_eth_dev_t* dev,
        enum phy_reg_offsets_t,
        uint32_t *data);

/**
 * \brief Write PHY register.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] regoffset Register offset
 * \param[in] data Register value to write
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_phy_regwrite(
        const struct smsc9220_eth_dev_t* dev,
        enum phy_reg_offsets_t,
        uint32_t data);

/**
 * \brief Read SMSC9220 ID.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return ID number
 */
uint32_t smsc9220_read_id(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Initiates a soft reset, returns failure or success.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_soft_reset(
        const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Set maximum transition unit by Tx fifo size.
 *        Note: The MTU will be smaller by 512 bytes,
 *        because the status uses this fixed space.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] val Size of the fifo in kbytes
 *                \ref HW_CFG_REG_TX_FIFO_SIZE_MIN
 *                \ref HW_CFG_REG_TX_FIFO_SIZE_MAX
 */
void smsc9220_set_txfifo(const struct smsc9220_eth_dev_t* dev,
                         uint32_t val);

/**
 * \brief Set FIFO level interrupt for a given source
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] irq_level_pos Bit position of the FIFO to set
 *            \ref smsc9220_fifo_level_irq_pos_t
 * \param[in] level Level of the FIFO, when the FIFO used space is greater
 *             than this value, corresponding interrupt will be generated.
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_set_fifo_level_irq(
        const struct smsc9220_eth_dev_t* dev,
        enum smsc9220_fifo_level_irq_pos_t irq_level_pos,
        uint32_t level);

/**
 * \brief Wait for EEPROM to be ready to use.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_wait_eeprom(
        const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Initialise irqs by clearing and disabling all interrupt sources
 *        and enable interrupts. Since all interrupt sources are disabled,
 *        interrupt won't be triggered, until interrupt sources won't be
 *        enabled by \ref smsc9220_enable_interrupt
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_init_irqs(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Check PHY ID registers.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_check_phy(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Reset PHY
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_reset_phy(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Advertise all speeds and pause capabilities
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_advertise_cap(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Enable transmission
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_enable_xmit(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Disable transmission
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_disable_xmit(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Enable MAC transmitter
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_enable_mac_xmit(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Disable MAC transmitter
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_disable_mac_xmit(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Enable receive
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_enable_mac_recv(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Disable receive
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_disable_mac_recv(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Enable the given interrupt source.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] source Enum of the interrupt source.
 */
void smsc9220_enable_interrupt(const struct smsc9220_eth_dev_t* dev,
                               enum smsc9220_interrupt_source source);

/**
 * \brief Disable the given interrupt source.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] source Enum of the interrupt source.
 */
void smsc9220_disable_interrupt(const struct smsc9220_eth_dev_t* dev,
                                enum smsc9220_interrupt_source source);

/**
 * \brief Disable all of the interrupt sources.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_disable_all_interrupts(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Clear the given interrupt source.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] source Enum of the interrupt source.
 */
void smsc9220_clear_interrupt(const struct smsc9220_eth_dev_t* dev,
                              enum smsc9220_interrupt_source source);

/**
 * \brief Clear all of the interrupt sources.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_clear_all_interrupts(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Get the status of the given interrupt source.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] source Enum of the interrupt source.
 *
 * \return non-zero if the given interrupt source is triggered, zero otherwise
 */
int smsc9220_get_interrupt(const struct smsc9220_eth_dev_t* dev,
                           enum smsc9220_interrupt_source source);

/**
 * \brief Establish link
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 */
void smsc9220_establish_link(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Read MAC address from EEPROM.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in,out] mac array will include the read MAC address in
 *                6 bytes hexadecimal format.
 *                It should be allocated by the caller to 6 bytes.
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_read_mac_address(
        const struct smsc9220_eth_dev_t* dev, char *mac);

/**
 * \brief Check device ID.
 *
 * \return error code /ref smsc9220_error_t
 */
int smsc9220_check_id(const struct smsc9220_eth_dev_t* dev);

/**
 * \brief Get the data size of the Tx buffer, aka Maximum Transition Unit
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return Fifo data size in bytes
 */
uint32_t smsc9220_get_tx_data_fifo_size(const struct
                                            smsc9220_eth_dev_t* dev);

/**
 * \brief Send Ethernet packet from buffer chain.
 *        The full packet length should be known in the beginning
 *        of a new packet.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in] total_payload_length Length of the ethernet payload.
 *            Should be equal to the sum of passed buffers within a packet.
 * \param[in] is_new_packet Should be set to true if the input buffer has to
 *            be sent as the start of a new packet or as a full packet.
 * \param[in] data Pointer to the data should be sent.
 * \param[in] current_size Size of the data in bytes.
 *
 * \return error code /ref smsc9220_error_t
 */
enum smsc9220_error_t smsc9220_send_by_chunks(
        const struct smsc9220_eth_dev_t* dev,
        uint32_t total_payload_length,
        bool is_new_packet,
        const char *data, uint32_t current_size);

/**
 * \brief Receive Ethernet packet from Rx FIFO to the passed buffer.
 *        Stops reading at packet border.
 *        If the passed buffer is larger than the current packet,
 *        the whole packet will be read into the buffer.
 *        If the current packet is larger than the passed buffer,
 *        the buffer will be filled with data and the next call
 *        will continue the read from that point.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 * \param[in,out] data Pointer where the data will be read to.
 *                     The caller is responsible to allocate it.
 * \param[in] dlen Length of the allocated data in bytes.
 *
 * \return Remaining bytes left in the fifo of the current packet.
 */
uint32_t smsc9220_receive_by_chunks(const struct smsc9220_eth_dev_t* dev,
                                        char *data, uint32_t dlen);

/**
 * \brief Get the used space of Rx fifo in bytes.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return Data received and waiting for read in bytes
 */
uint32_t smsc9220_get_rxfifo_data_used_space(const struct
                                                 smsc9220_eth_dev_t* dev);

/**
 * \brief Get the size of next unread packet in Rx buffer, using the peak
 *        register, which is not destructive so can be read asynchronously.
 *        Warning: In case of heavy receiving load, it's possible this register
 *        is not perfectly in sync.
 *
 * \param[in] dev Ethernet device structure \ref smsc9220_eth_dev_t
 *
 * \return Size in bytes of the next packet can be read from Rx fifo, according
 *         to the peek register.
 */
uint32_t smsc9220_peek_next_packet_size(const struct
                                            smsc9220_eth_dev_t* dev);

#ifdef __cplusplus
}
#endif

#endif /* __SMSC9220_ETH_H__ */
