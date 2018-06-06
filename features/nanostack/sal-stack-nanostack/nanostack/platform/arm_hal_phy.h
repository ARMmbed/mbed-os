/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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
 */

/**
 * \file arm_hal_phy.h
 * \brief PHY device driver API.
 */

#ifndef ARM_HAL_PHY_H_
#define ARM_HAL_PHY_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Interface states */
typedef enum {
    PHY_INTERFACE_RESET,            /**< Reset PHY driver and set to idle. */
    PHY_INTERFACE_DOWN,             /**< Disable PHY interface driver (RF radio disable). */
    PHY_INTERFACE_UP,               /**< Enable PHY interface driver (RF radio receiver ON). */
    PHY_INTERFACE_RX_ENERGY_STATE,  /**< Enable wireless interface ED scan mode. */
    PHY_INTERFACE_SNIFFER_STATE     /**< Enable sniffer mode. */
} phy_interface_state_e;

/** TX process return codes */
typedef enum {
    PHY_LINK_TX_DONE,           /**< TX process ready and ACK RX. */
    PHY_LINK_TX_DONE_PENDING,   /**< TX process OK with ACK pending flag. */
    PHY_LINK_TX_SUCCESS,        /**< MAC TX complete. MAC will a make decision to enter wait ACK or TX done state. */
    PHY_LINK_TX_FAIL,           /**< Link TX process fail. */
    PHY_LINK_CCA_FAIL,          /**< RF link CCA process fail. */
} phy_link_tx_status_e;

/** Extension types */
typedef enum {
    PHY_EXTENSION_CTRL_PENDING_BIT, /**< Control MAC pending bit for indirect data. */
    PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS, /**< Read status if the last ACK is still pending. */
    PHY_EXTENSION_SET_CHANNEL,  /**< Net library channel set. */
    PHY_EXTENSION_READ_CHANNEL_ENERGY, /**< RF interface ED scan energy read. */
    PHY_EXTENSION_READ_LINK_STATUS, /**< Net library could read link status. */
    PHY_EXTENSION_CONVERT_SIGNAL_INFO, /**< Convert signal info. */
    PHY_EXTENSION_ACCEPT_ANY_BEACON, /**< Set boolean true or false for accept beacon from other Pan-ID than configured. Default value should be false */
} phy_extension_type_e;

/** Address types */
typedef enum {
    PHY_MAC_48BIT, /**< IPv4/IPv6/BLE link layer address for Ethernet. This is optional. */
    PHY_MAC_64BIT, /**< RF/PLC link layer address. */
    PHY_MAC_16BIT, /**< RF interface short address. */
    PHY_MAC_PANID, /**< RF interface 16-Bit PAN-ID. */
} phy_address_type_e;

/** PHY types */
typedef enum phy_link_type_e {
    PHY_LINK_ETHERNET_TYPE,         /**< Standard IEEE 802 Ethernet. */
    PHY_LINK_15_4_2_4GHZ_TYPE,      /**< Standard 802.15.4 2.4GHz radio. */
    PHY_LINK_15_4_SUBGHZ_TYPE,      /**< Standard 802.15.4 subGHz radio 868 /915MHz. */
    PHY_LINK_TUN,                   /**< Tunnel interface for Linux TUN, RF network driver over serial bus or just basic application to application data flow. */
    PHY_LINK_SLIP,                  /**< Generic SLIP driver which just forward SLIP payload */
} phy_link_type_e;

/** Data layers */
typedef enum data_protocol_e {
    LOCAL_SOCKET_DATA = 0,          /**< 6LoWPAN library local socket data. */
    INTERFACE_DATA = 1,             /**< 6LoWPAN library interface internal used protocol. */
    PHY_LAYER_PAYLOAD = 2,          /**< PHY layer data selection or handler. */
    IPV6_DATAGRAM = 3,              /**< IP layer data or TUN driver request data. */
    UNKNOWN_PROTOCOL = 4            /**< Non-supported protocol ID. */
} data_protocol_e;

/** Requested data layer */
typedef enum driver_data_request_e {
    PHY_LAYER_PAYLOAD_DATA_FLOW,    /**< PHY layer data. */
    IPV6_DATAGRAMS_DATA_FLOW,       /**< IP layer data or TUN driver request data. */
} driver_data_request_e;

/** \brief Signal info types.
 *
 * Types of signal quality indication desired by various link protocols. Some are
 * really statistical, but a driver should ideally be able to create an estimate
 * based on its LQI/DBM numbers, for example to bootstrap a statistic calculation.
 */
typedef enum phy_signal_info_type_e {
    PHY_SIGNAL_INFO_ETX,            /**< Expected transmissions, unsigned 16-bit fixed-point ETX*128 [1..512], for example Zigbee IP + RFC 6719. */
    PHY_SIGNAL_INFO_IDR,            /**< Inverse Delivery Ratio, unsigned 16-bit fixed-point IDR*32*256 [1..8], for example MLE draft 06. */
    PHY_SIGNAL_INFO_LINK_MARGIN,    /**< Link margin, unsigned 16-bit fixed-point dB*256, [0..255], for example Thread routing draft. */
} phy_signal_info_type_e;

/** Signal level info */
typedef struct phy_signal_info_s {
    phy_signal_info_type_e type;    /**< Signal info type desired. */
    uint8_t lqi;                    /**< Quality passed to arm_net_phy_rx. */
    int8_t dbm;                     /**< Strength passed to arm_net_phy_rx. */
    uint16_t result;                /**< Resulting signal information. */
} phy_signal_info_s;

/** PHY modulation scheme */
typedef enum phy_modulation_e
{
    M_OFDM,     ///< QFDM
    M_OQPSK,    ///< OQPSK
    M_BPSK,     ///< BPSK
    M_GFSK,     ///< GFSK
    M_UNDEFINED ///< UNDEFINED
} phy_modulation_e;

/** Channel page numbers */
typedef enum
{
    CHANNEL_PAGE_0 = 0,     ///< Page 0
    CHANNEL_PAGE_1 = 1,     ///< Page 1
    CHANNEL_PAGE_2 = 2,     ///< Page 2
    CHANNEL_PAGE_3 = 3,     ///< Page 3
    CHANNEL_PAGE_4 = 4,     ///< Page 4
    CHANNEL_PAGE_5 = 5,     ///< Page 5
    CHANNEL_PAGE_6 = 6,     ///< Page 6
    CHANNEL_PAGE_9 = 9,     ///< Page 9
    CHANNEL_PAGE_10 = 10    ///< Page 10
} channel_page_e;

/** Channel configuration */
typedef struct phy_rf_channel_configuration_s
{
    uint32_t channel_0_center_frequency;    ///< Center frequency
    uint32_t channel_spacing;               ///< Channel spacing
    uint32_t datarate;                      ///< Data rate
    uint16_t number_of_channels;            ///< Number of channels
    phy_modulation_e modulation;            ///< Modulation scheme
} phy_rf_channel_configuration_s;

/** Channel page configuration */
typedef struct phy_device_channel_page_s
{
    channel_page_e channel_page;            ///< Channel page
    const phy_rf_channel_configuration_s *rf_channel_configuration; ///< Pointer to channel configuration
} phy_device_channel_page_s;

/** Virtual data request */
typedef struct virtual_data_req_s {
    uint16_t parameter_length;      /**< Length of user specified header. Can be zero. */
    uint8_t *parameters;            /**< Pointer to user specified header. Optional */
    uint16_t msduLength;            /**< MSDU Length */
    const uint8_t *msdu;            /**< MSDU */
} virtual_data_req_t;

/**
 * @brief arm_net_phy_rx RX callback set by upper layer. Called when data is received
 * @param data_ptr Data received
 * @param data_len Length of the data received
 * @param link_quality Link quality
 * @param dbm Power ratio in decibels
 * @param driver_id ID of driver which received data
 * @return 0 if success, error otherwise
 */
typedef int8_t arm_net_phy_rx_fn(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id);

/**
 * @brief arm_net_phy_tx_done TX done callback set by upper layer. Called when tx sent by upper layer has been handled
 * @param driver_id Id of the driver which handled TX request
 * @param tx_handle Handle of the TX
 * @param status Status code of the TX handling result
 * @param cca_retry Number of CCA retries done during handling
 * @param tx_retry Number of TX retries done during handling
 * @return 0 if success, error otherwise
 */
typedef int8_t arm_net_phy_tx_done_fn(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry);

/**
 * @brief arm_net_virtual_rx RX callback set by user of serial MAC. Called when virtual RF has received data.
 * @param data_ptr Data received
 * @param data_len Length of the data received
 * @param driver_id ID of driver which received data
 * @return 0 if success, error otherwise
 */
typedef int8_t arm_net_virtual_rx_fn(const uint8_t *data_ptr, uint16_t data_len,int8_t driver_id);

/**
 * @brief arm_net_virtual_tx TX callback set by serial MAC. Used to send data.
 * @param data_req Data to be sent
 * @param driver_id Id of the driver to be used.
 * @return 0 if success, error otherwise
 */
typedef int8_t arm_net_virtual_tx_fn(const virtual_data_req_t *data_req,int8_t driver_id);

/**
 * @brief arm_net_virtual_config Configuration receive callback set by upper layer. Used to receive internal configuration parameters.
 * @param driver_id Id of the driver to be used.
 * @param data Pointer to received configuration data.
 * @param length Length of the configuration data.
 * @return 0 if success, error otherwise
 */
typedef int8_t arm_net_virtual_config_rx_fn(int8_t driver_id, const uint8_t *data, uint16_t length);

/**
 * @brief arm_net_virtual_config Configuration send callback set by upper layer. Used to send internal configuration parameters.
 * @param driver_id Id of the driver to be used.
 * @param data Pointer to sent configuration data.
 * @param length Length of the configuration data.
 * @return 0 if success, error otherwise
 */
typedef int8_t arm_net_virtual_config_tx_fn(int8_t driver_id, const uint8_t *data, uint16_t length);

/**
 * @brief arm_net_virtual_confirmation Confirmation receive callback set by upper layer. Used to receive MLME confirmation data.
 * @param driver_id Id of the driver to be used.
 * @param data Pointer to received confirmation data.
 * @param length Length of the confirmation data.
 * @return 0 if success, error otherwise
 */
typedef int8_t arm_net_virtual_confirmation_rx_fn(int8_t driver_id, const uint8_t *data, uint16_t length);

/** Device driver structure */
typedef struct phy_device_driver_s
{
    phy_link_type_e link_type;                                      /**< Define driver types. */
    driver_data_request_e data_request_layer;                       /**< Define interface data OUT protocol. */
    uint8_t *PHY_MAC;                                               /**< Pointer to 64-bit or 48-bit MAC address. */
    uint16_t phy_MTU;                                               /**< Define MAX PHY layer MTU size. */
    char *driver_description;                                       /**< Short driver platform description. Needs to end with zero. */
    uint8_t phy_tail_length;                                        /**< Define PHY driver needed TAIL Length. */
    uint8_t phy_header_length;                                      /**< Define PHY driver needed header length before PDU. */
    int8_t (*state_control)(phy_interface_state_e, uint8_t);        /**< Function pointer for control PHY driver state. */
    int8_t (*tx)(uint8_t *, uint16_t, uint8_t, data_protocol_e);    /**< Function pointer for PHY driver write operation. */
    int8_t (*address_write)(phy_address_type_e , uint8_t *);        /**< Function pointer for PHY driver address write. */
    int8_t (*extension)(phy_extension_type_e, uint8_t *);           /**< Function pointer for PHY driver extension control. */
    const phy_device_channel_page_s *phy_channel_pages;             /**< Pointer to channel page list */

    //Upper layer callbacks, set with arm_net_phy_register();
    arm_net_phy_rx_fn *phy_rx_cb;                                   /**< PHY RX callback. Initialized by \ref arm_net_phy_register(). */
    arm_net_phy_tx_done_fn *phy_tx_done_cb;                         /**< Transmission done callback. Initialized by \ref arm_net_phy_register(). */
    //Virtual upper data rx
    arm_net_virtual_rx_fn *arm_net_virtual_rx_cb;                   /**< Virtual RX callback. Initialized by \ref arm_net_phy_register(). */
    arm_net_virtual_tx_fn *arm_net_virtual_tx_cb;                   /**< Virtual TX callback. Initialized by \ref arm_net_phy_register(). */
    arm_net_virtual_config_rx_fn *virtual_config_rx_cb;             /**< Virtual config receive callback. Initialized by \ref arm_net_phy_register(). */
    arm_net_virtual_config_tx_fn *virtual_config_tx_cb;             /**< Virtual config send callback. Initialized by \ref arm_net_phy_register(). */
    arm_net_virtual_confirmation_rx_fn *virtual_confirmation_rx_cb; /**< Virtual confirmation receive callback. Initialized by \ref arm_net_phy_register(). */
    uint16_t tunnel_type; /**< Tun driver type. */
} phy_device_driver_s;


/**
 * \brief This function registers the device driver to stack.
 *
 * \param phy_driver A pointer to device driver structure.
 *
 * \return >= 0 Device driver ID.
 * \return < 0 Means register fail.
 *
 */
extern int8_t arm_net_phy_register(phy_device_driver_s *phy_driver);


/**
 * \brief Set driver mac64 address.
 *
 * \param MAC A pointer to new mac64 address which is copied to old one.
 * \param  id driver id
 *
 * \return >= 0 SET OK.
 * \return < 0 Means register fail.
 *
 */
extern int8_t arm_net_phy_mac64_set(uint8_t *MAC, int8_t id);

/**
 * \brief Get driver mac64 address.
 *
 * \param  id driver id
 *
 * \return > 0 Return pointer to MAC.
 * \return NULL.
 *
 */
extern uint8_t *arm_net_phy_mac64_get(int8_t id);

/**
 * \brief Get driver link type.
 *
 * \param  id driver id
 *
 * \return driver link type.
 *
 */
extern int arm_net_phy_rf_type(int8_t id);

/**
 * \brief Get driver link type MTU size.
 *
 * \param  id driver id
 *
 * \return size of MTU.
 *
 */
extern uint16_t arm_net_phy_mtu_size(int8_t id);

/**
 * \brief Unregister the driver from storage.
 *
 * \param driver_id driver id
 *
 */
extern void arm_net_phy_unregister(int8_t driver_id);

#ifdef __cplusplus
}
#endif
#endif /* ARM_HAL_PHY_H_ */
