/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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
#include "nsconfig.h"
#include <stdio.h>
#include <string.h>

#include "ns_types.h"
#include "ns_trace.h"
#include "platform/arm_hal_phy.h"
#include "mlme.h"
#include "net_interface.h"
#include "serial_mac_api.h"
#include "virtual_rf_defines.h"
#include "MAC/rf_driver_storage.h"
#include "mac_api.h"



#define TRACE_GROUP "vrf"

static phy_device_driver_s device_driver;
static uint8_t rf_mac_address[8];
static int8_t rf_driver_id = (-1);
static bool data_request_pending_flag = false;

/** XXX: dummy values copied from Atmel RF driver */
static const phy_rf_channel_configuration_s phy_2_4ghz = {.channel_0_center_frequency = 2405000000, .channel_spacing = 5000000, .datarate = 250000, .number_of_channels = 16, .modulation = M_OQPSK};
static const phy_rf_channel_configuration_s phy_subghz = {.channel_0_center_frequency = 868300000, .channel_spacing = 2000000, .datarate = 250000, .number_of_channels = 11, .modulation = M_OQPSK};

static const phy_rf_channel_configuration_s phy_subghz_8_ch = {.channel_0_center_frequency = 868300000, .channel_spacing = 2000000, .datarate = 250000, .number_of_channels = 8, .modulation = M_OQPSK};
static const phy_rf_channel_configuration_s phy_subghz_11_ch = {.channel_0_center_frequency = 868300000, .channel_spacing = 2000000, .datarate = 250000, .number_of_channels = 11, .modulation = M_OQPSK};
static const phy_rf_channel_configuration_s phy_subghz_16_ch = {.channel_0_center_frequency = 868300000, .channel_spacing = 2000000, .datarate = 250000, .number_of_channels = 16, .modulation = M_OQPSK};
static const phy_rf_channel_configuration_s phy_2_4ghz_14_ch = {.channel_0_center_frequency = 2405000000, .channel_spacing = 1000000, .datarate = 250000, .number_of_channels = 14, .modulation = M_OQPSK};
static const phy_rf_channel_configuration_s phy_2_4ghz_5_ch = {.channel_0_center_frequency = 2405000000, .channel_spacing = 1000000, .datarate = 250000, .number_of_channels = 5, .modulation = M_OQPSK}; //For FHSS testing only
static const phy_rf_channel_configuration_s phy_2_4ghz_256_ch = {.channel_0_center_frequency = 2405000000, .channel_spacing = 1000000, .datarate = 250000, .number_of_channels = 256, .modulation = M_OQPSK}; //For FHSS testing only

static phy_device_channel_page_s phy_channel_pages[] = {
    {CHANNEL_PAGE_0, &phy_2_4ghz}, // this will be modified to contain 11 or 16 channels, depending on radio type
    {CHANNEL_PAGE_1, &phy_subghz_11_ch}, // channel 0&ASK or 1..10&ASK
    {CHANNEL_PAGE_2, &phy_subghz_11_ch}, // 0&O-QPSK, or 1-10&O-QPSK
    // following are based on 202.15.2009 additional channel pages:
    {CHANNEL_PAGE_3, &phy_2_4ghz_14_ch}, // 0.13&CSSS
    {CHANNEL_PAGE_4, &phy_subghz_16_ch}, // 0&UWB, 1-4&UWB or 5-15&UWB
    {CHANNEL_PAGE_5, &phy_subghz_8_ch}, // this would need to be either 0..3&O-QPSK or 4-7&MPSK
    {CHANNEL_PAGE_6, &phy_subghz_11_ch}, // this would need to be either 0..9&BPSK or 1-10&GFSK
    // just for testing fhss
    {CHANNEL_PAGE_9, &phy_2_4ghz_256_ch},
    {CHANNEL_PAGE_10, &phy_2_4ghz_5_ch}, // 5 channels, just as in cmd_network.c
    {CHANNEL_PAGE_0, NULL}
};

static int8_t phy_rf_state_control(phy_interface_state_e new_state, uint8_t channel);
static int8_t phy_rf_tx(uint8_t *data_ptr, uint16_t data_len, uint8_t tx_handle, data_protocol_e protocol);
static int8_t phy_rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr);
static int8_t phy_rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr);


static int8_t phy_rf_virtual_rx(const uint8_t *data_ptr, uint16_t data_len, int8_t driver_id)
{
    if (rf_driver_id != driver_id || !data_ptr) {
        return -1;
    }

    uint8_t data_type = *data_ptr++;

    switch (data_type) {
        case NAP_DATA_PHY_RAW_INDICATION: {
            if (data_len < 4 || !device_driver.phy_rx_cb) {
                return -1;
            }
            int8_t dbm;
            uint8_t link_quality = *data_ptr++;
            dbm = *data_ptr++;
            return device_driver.phy_rx_cb(data_ptr, data_len - 3, link_quality, dbm, driver_id);
        }
        case NAP_DATA_PHY_RAW_RESPONSE: {
            if (data_len != 4 || !device_driver.phy_tx_done_cb) {
                return -1;
            }
            uint8_t tx_retry, cca_retry;
            phy_link_tx_status_e status;
            status = (phy_link_tx_status_e) * data_ptr++;
            cca_retry = *data_ptr++;
            tx_retry = *data_ptr;
            return  device_driver.phy_tx_done_cb(driver_id, 1, status, cca_retry, tx_retry);
        }
        case NAP_CONFIG_INTERNAL: {
            if (!device_driver.virtual_config_rx_cb) {
                return -1;
            }
            return  device_driver.virtual_config_rx_cb(driver_id, data_ptr, data_len - 1);
        }
        case NAP_MLME_CONFIRM: {
            if (!device_driver.virtual_confirmation_rx_cb) {
                return -1;
            }
            return device_driver.virtual_confirmation_rx_cb(driver_id, data_ptr, data_len - 1);
        }
        default:
            break;
    }
    return -1;
}

/**
 * \brief This function is used by the network stack library to set the interface state:
 *
 * \param new_state An interface state: PHY_INTERFACE_RESET, PHY_INTERFACE_DOWN,
 *                  PHY_INTERFACE_UP or PHY_INTERFACE_RX_ENERGY_STATE.
 *
 * \param channel An RF channel that the command applies to.
 *
 * \return 0 State update is OK.
 * \return -1 An unsupported state or a general failure.
 */
int8_t phy_rf_state_control(phy_interface_state_e new_state, uint8_t channel)
{
    (void)new_state;
    (void)channel;
    return 0;
}

/**
 * \brief This function is used give driver data to transfer.
 *
 * \param data_ptr A pointer to TX data. The platform driver can use the same pointer, but the
 *                 network stack will free the memory when the device driver implementation
 *                 notifies the stack (using the unique tx_handle) that it is allowed to do so.
 *
 * \param data_len The length of data behind a pointer.
 *
 * \param tx_handle A unique TX handle defined by the network stack.
 *
 * \return 0 TX process start is OK. The library must wait for the TX Done callback
 *           before pushing a new packet.
 * \return 1 TX process is OK at the Ethernet side (fast TX phase).
 *
 * \return -1 PHY is busy.
 *
 */
static int8_t phy_rf_tx(uint8_t *data_ptr, uint16_t data_len, uint8_t tx_handle, data_protocol_e protocol)
{
    if (!data_ptr) {
        return -1;
    }
    virtual_data_req_t data_req;
    (void)protocol;
    (void)tx_handle;

    //Push TO LMAC
    data_req.parameter_length = 0;
    data_req.parameters = NULL;

    data_req.msdu = data_ptr;
    data_req.msduLength = data_len + 1;

    //SET PHY Header and increment length
    *data_ptr = NAP_DATA_PHY_RAW_REQUEST;

    //Push To LMAC
    if (!device_driver.arm_net_virtual_tx_cb) {
        tr_debug("Virtual Init not configured");
        return -1;
    }
    return device_driver.arm_net_virtual_tx_cb(&data_req, rf_driver_id);

}

static void phy_rf_mlme_orserver_tx(const mlme_set_t *set_req)
{
    switch (set_req->attr) {

        case macBeaconPayload:
        case macLoadBalancingBeaconTx:
            break;
        default:
            return;

    }

    virtual_data_req_t data_req;
    uint8_t msg_aram[4];
    uint8_t temp = 0;
    msg_aram[0] = NAP_MLME_REQUEST;
    msg_aram[1] = MLME_SET;
    msg_aram[2] = set_req->attr;
    msg_aram[3] = set_req->attr_index;
    //Push TO LMAC
    data_req.parameter_length = 4;
    data_req.parameters = msg_aram;
    if (set_req->value_pointer) {
        data_req.msdu = (uint8_t *) set_req->value_pointer;
        data_req.msduLength = set_req->value_size;
    } else {
        data_req.msdu = &temp;
        data_req.msduLength = 1;
    }

    //Push To LMAC
    if (!device_driver.arm_net_virtual_tx_cb) {
        tr_debug("Virtual Init not configured");
        return;
    }
    device_driver.arm_net_virtual_tx_cb(&data_req, rf_driver_id);

}

/**
 * \brief This is the default PHY interface address write API for all interface types.
 *
 * \param address_type Defines the PHY address type: PHY_MAC_64BIT, PHY_MAC_48BIT,
 *                     PHY_MAC_PANID or PHY_MAC_16BIT.
 *
 * \param address_ptr A pointer to an address.
 *
 * \return 0 Write is OK.
 * \return -1 PHY is busy.
 */
int8_t phy_rf_address_write(phy_address_type_e address_type, uint8_t *address_ptr)
{
    if (address_ptr) {
        switch (address_type) {
            case PHY_MAC_64BIT: {
                memcpy(rf_mac_address, address_ptr, 8);
                break;
            }
            default:
                break;
        }
    }
    return 0;
}

/**
 * \brief This is the default PHY interface address write API for all interface types.
 *
 * \param extension_type Supported extension types: PHY_EXTENSION_CTRL_PENDING_BIT,
 *                       PHY_EXTENSION_SET_CHANNEL, PHY_EXTENSION_READ_CHANNEL_ENERGY
 *                       or PHY_EXTENSION_READ_LINK_STATUS.
 *
 * \param data_ptr A pointer to an 8-bit data storage for read or write purpose,
 *                 based on the extension command types.
 *
 * \return 0 State update is OK.
 * \return -1 An unsupported state or a general failure.
 */
static int8_t phy_rf_extension(phy_extension_type_e extension_type, uint8_t *data_ptr)
{
    if (data_ptr) {
        switch (extension_type) {
            /*Control MAC pending bit for Indirect data transmission*/
            case PHY_EXTENSION_CTRL_PENDING_BIT: {
                data_request_pending_flag = *data_ptr;
                break;
            }
            /*Return frame pending status*/
            case PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS: {
                *data_ptr = data_request_pending_flag;
                break;
            }
            default:
                break;
        }
    }

    return 0;
}

// XXX: the phy_channel_pages needs to match the config at cmd_network.c, or the RF init fails
int8_t virtual_rf_device_register(phy_link_type_e link_type, uint16_t mtu_size)
{
    if (rf_driver_id < 0) {
        memset(&device_driver, 0, sizeof(phy_device_driver_s));
        /*Set pointer to MAC address*/
        device_driver.PHY_MAC = rf_mac_address;
        device_driver.arm_net_virtual_rx_cb = &phy_rf_virtual_rx;
        device_driver.driver_description = "VSND";

        device_driver.link_type = link_type;

        if (link_type == PHY_LINK_15_4_SUBGHZ_TYPE) {
            /*Type of RF PHY is SubGHz*/
            phy_channel_pages[0].rf_channel_configuration = &phy_subghz;
            device_driver.phy_channel_pages = phy_channel_pages;
        } else  if (link_type == PHY_LINK_15_4_2_4GHZ_TYPE) {
            /*Type of RF PHY is 2.4GHz*/
            phy_channel_pages[0].rf_channel_configuration = &phy_2_4ghz;
            device_driver.phy_channel_pages = phy_channel_pages;
        } else {
            device_driver.phy_channel_pages = NULL;
        }

        device_driver.phy_MTU = mtu_size;
        /*Set 1 byte header in PHY*/
        device_driver.phy_header_length = 1;
        /* Register handler functions */
        device_driver.state_control = &phy_rf_state_control;
        device_driver.tx = &phy_rf_tx;
        device_driver.address_write = phy_rf_address_write;
        device_driver.extension = &phy_rf_extension;

        rf_driver_id = arm_net_phy_register(&device_driver);

        arm_net_observer_cb_set(rf_driver_id, phy_rf_mlme_orserver_tx);
    }

    return rf_driver_id;
}
