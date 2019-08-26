/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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
 * \file ws_management_if.h
 * \brief Wi-SUN management interface.
 *
 * This interface is used for configuring Wi-SUN devices.
 * After creating the Wi-SUN interface, you can use this interface to configure the Wi-SUN device
 * behaviour. When you are done with the configurations, you need to call interface up to enable a Wi-SUN node.
 *
 */

#ifndef WS_MANAGEMENT_API_H_
#define WS_MANAGEMENT_API_H_

#include "ns_types.h"
#include "net_interface.h" /* Declaration for channel_list_s. */

#ifdef __cplusplus
extern "C" {
#endif

/* Regulatory domain values*/
#define REG_DOMAIN_WW   0x00 // World wide
#define REG_DOMAIN_NA   0x01 // North America
#define REG_DOMAIN_JP   0x02 // Japan
#define REG_DOMAIN_EU   0x03 // European Union
#define REG_DOMAIN_CH   0x04 // China
#define REG_DOMAIN_IN   0x05 // India
#define REG_DOMAIN_MX   0x06 //
#define REG_DOMAIN_BZ   0x07 // Brazil
#define REG_DOMAIN_AZ   0x08 // Australia
#define REG_DOMAIN_NZ   0x08 // New zealand
#define REG_DOMAIN_KR   0x09 // Korea
#define REG_DOMAIN_PH   0x0A //
#define REG_DOMAIN_MY   0x0B //
#define REG_DOMAIN_HK   0x0C //
#define REG_DOMAIN_SG   0x0D // band 866-869
#define REG_DOMAIN_TH   0x0E //
#define REG_DOMAIN_VN   0x0F //
#define REG_DOMAIN_SG_H 0x10 // band 920-925

#define OPERATING_MODE_1a 0x1a
#define OPERATING_MODE_1b 0x1b
#define OPERATING_MODE_2a 0x2a
#define OPERATING_MODE_2b 0x2b
#define OPERATING_MODE_3  0x03
#define OPERATING_MODE_4a 0x4a
#define OPERATING_MODE_4b 0x4b
#define OPERATING_MODE_5  0x05

#define CHANNEL_FUNCTION_FIXED            0x00 // Fixed channel
#define CHANNEL_FUNCTION_TR51CF           0x01 // TR51CF
#define CHANNEL_FUNCTION_DH1CF            0x02 // Direct Hash
#define CHANNEL_FUNCTION_VENDOR_DEFINED   0x03 // vendor given channel hop schedule

#define CHANNEL_SPACING_200 0x00 // 200 khz
#define CHANNEL_SPACING_400 0x01 // 400 khz
#define CHANNEL_SPACING_600 0x02 // 600 khz
#define CHANNEL_SPACING_100 0x03 // 100 khz
#define CHANNEL_SPACING_250 0x04 // 250 khz

#define NETWORK_SIZE_AUTOMATIC 0x00
#define NETWORK_SIZE_SMALL 0x01
#define NETWORK_SIZE_MEDIUM 0x08
#define NETWORK_SIZE_LARGE 0x10


/** Temporary API change flag. this will be removed when new version of API is implemented on applications
 *
 */
#define WS_MANAGEMENT_API_VER_2

/**
 * \brief Struct ws_statistics defines the Wi-SUN statistics storage structure.
 */
typedef struct ws_statistics {
    /** Asynch TX counter */
    uint32_t asynch_tx_count;
    /** Asynch RX counter */
    uint32_t asynch_rx_count;
} ws_statistics_t;

/**
 * Initialize Wi-SUN stack.
 *
 * Generates the default configuration for Wi-SUN operation
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain Mandatory regulatory domain value of the device.
 * \param network_name_ptr Network name where to join if no configuration found from storage.
 * \param fhss_timer_ptr FHSS functions for timer adaptation to platform.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_node_init(
    int8_t interface_id,
    uint8_t regulatory_domain,
    char *network_name_ptr,
    fhss_timer_t *fhss_timer_ptr);

/**
 * Change the network name
 *
 * Change the network name dynamically at a runtime.
 * If stack is running the network discovery is restarted.
 *
 * \param interface_id Network interface ID.
 * \param network_name_ptr Nul terminated Network name limited to 32 characters.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_network_name_set(
    int8_t interface_id,
    char *network_name_ptr);

/**
 * Configure regulatory domain of Wi-SUN stack.
 *
 * Change the default configuration for Wi-SUN PHY operation.
 *
 * Supported values:
 * Domain: "NA"(0x01), "KR"(0x09)
 * Operating class: (1), (2)
 * operation mode: "1b" (symbol rate 50, modulation index 1)
 *
 * if value of 255 is given then previous value is used.
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain FHSS regulatory domain default to "KR" 0x09.
 * \param operating_class FHSS operating class default to 1.
 * \param operating_mode FHSS phy operating mode default to "1b".
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_regulatory_domain_set(
    int8_t interface_id,
    uint8_t regulatory_domain,
    uint8_t operating_class,
    uint8_t operating_mode);

/**
 * Set timing parameters related to network size.
 *
 * timing parameters follows the specification example from Wi-SUN specification
 *
 * Default value: automatic
 * small network size: hundreds of devices
 * Large network size: thousands of devices
 * automatic: when discovering the network network size is learned
 *            from advertisements and timings adjusted accordingly
 *
 * \param interface_id Network interface ID.
 * \param network_size define from NETWORK_SIZE_*.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_network_size_set(
    int8_t interface_id,
    uint8_t network_size);

/**
 * Set channel mask for FHSS operation.
 *
 * Default value: all channels are allowed.
 *
 * \param interface_id Network interface ID.
 * \param channel_mask set bits matching the channel 1 to allow channel 0 to disallow.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_channel_mask_set(
    int8_t interface_id,
    uint32_t channel_mask[8]);

/**
 * Configure Application defined channel plan.
 *
 * Change the application defined channel plan.
 * This changes our channel plan that is reported to our children.
 * PHY driver must be configured to follow these settings to make the configuration active.
 *
 *
 * \param interface_id Network interface ID.
 * \param channel_plan Channel plan must be 1 application defined if deviating from regulatory domain (0).
 * \param uc_channel_function 0: Fixed channel, 1:TR51CF, 2: Direct Hash, 3: Vendor defined.
 * \param bc_channel_function 0: Fixed channel, 1:TR51CF, 2: Direct Hash, 3: Vendor defined.
 * \param ch0_freq ch0 center frequency.
 * \param channel_spacing Channel spacing value 0:200k, 1:400k, 2:600k, 3:100k.
 * \param number_of_channels FHSS phy operating mode default to "1b".
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_channel_plan_set(
    int8_t interface_id,
    uint8_t channel_plan,
    uint8_t uc_channel_function,
    uint8_t bc_channel_function,
    uint32_t ch0_freq, // Stack can not modify this
    uint8_t channel_spacing,// Stack can not modify this
    uint8_t number_of_channels);// Stack can not modify this

/**
 * Configure timing values for FHSS.
 *
 * Change the default configuration for Wi-SUN FHSS operation.
 *
 * \param interface_id Network interface ID.
 * \param fhss_uc_dwell_interval default to 250 ms.
 * \param fhss_broadcast_interval default to 800 ms.
 * \param fhss_bc_dwell_interval default to 200 ms.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_fhss_timing_configure(
    int8_t interface_id,
    uint8_t fhss_uc_dwell_interval,
    uint32_t fhss_broadcast_interval,
    uint8_t fhss_bc_dwell_interval);

/**
 * Configure unicast channel function.
 *
 * Change the default configuration for Wi-SUN FHSS operation.
 * if application defined is used the behaviour is undefined
 *
 * \param interface_id Network interface ID.
 * \param channel_function Unicast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel. If 0xFFFF, randomly chosen channel is used.
 * \param dwell_interval Used dwell interval when channel function is TR51 or DH1.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_fhss_unicast_channel_function_configure(
    int8_t interface_id,
    uint8_t channel_function,
    uint16_t fixed_channel,
    uint8_t dwell_interval);

/**
 * Configure broadcast channel function.
 *
 * Change the default configuration for Wi-SUN FHSS operation.
 * if application defined is used the behaviour is undefined
 *
 * \param interface_id Network interface ID.
 * \param channel_function Broadcast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel. If 0xFFFF, randomly chosen channel is used.
 * \param dwell_interval Broadcast channel dwell interval.
 * \param broadcast_interval Broadcast interval.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_fhss_broadcast_channel_function_configure(
    int8_t interface_id,
    uint8_t channel_function,
    uint16_t fixed_channel,
    uint8_t dwell_interval,
    uint32_t broadcast_interval);

/**
 * Start collecting Wi-SUN statistics.
 *
 * \param interface_id Network interface ID.
 * \param stats_ptr Pointer to stored statistics.
 *
 * \return 0 Success.
 * \return <0 Failure.
 */
int ws_statistics_start(
    int8_t interface_id,
    ws_statistics_t *stats_ptr);

/**
 * Stop collecting Wi-SUN statistics.
 *
 * \param interface_id Network interface ID.
 *
 * \return 0 Success.
 * \return <0 Failure.
 */
int ws_statistics_stop(
    int8_t interface_id);

#ifdef __cplusplus
}
#endif
#endif /* WS_MANAGEMENT_API_H_ */
