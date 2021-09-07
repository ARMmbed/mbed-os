/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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
 * \file ws_management_api.h
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
#include "fhss_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Regulatory domain values*/
#define REG_DOMAIN_WW   0x00 /**< World wide */
#define REG_DOMAIN_NA   0x01 /**< North America */
#define REG_DOMAIN_JP   0x02 /**< Japan */
#define REG_DOMAIN_EU   0x03 /**< European Union */
#define REG_DOMAIN_CH   0x04 /**< China */
#define REG_DOMAIN_IN   0x05 /**< India */
#define REG_DOMAIN_MX   0x06 /**< Mexico */
#define REG_DOMAIN_BZ   0x07 /**< Brazil */
#define REG_DOMAIN_AZ   0x08 /**< Australia */
#define REG_DOMAIN_NZ   0x08 /**< New zealand */
#define REG_DOMAIN_KR   0x09 /**< Korea */
#define REG_DOMAIN_PH   0x0A /**< Philippines */
#define REG_DOMAIN_MY   0x0B /**< Malaysia */
#define REG_DOMAIN_HK   0x0C /**< Hong Kong */
#define REG_DOMAIN_SG   0x0D /**< Singapore band 866-869 */
#define REG_DOMAIN_TH   0x0E /**< Thailand */
#define REG_DOMAIN_VN   0x0F /**< Vietnam */
#define REG_DOMAIN_SG_H 0x10 /**< Singapore band 920-925 */

#define OPERATING_MODE_1a 0x1a  /**< 50, 0,5 */
#define OPERATING_MODE_1b 0x1b  /**< 50, 1.0 */
#define OPERATING_MODE_2a 0x2a  /**< 100, 0,5 */
#define OPERATING_MODE_2b 0x2b  /**< 100, 1.0 */
#define OPERATING_MODE_3  0x03  /**< 150, 0.5 */
#define OPERATING_MODE_4a 0x4a  /**< 200, 0.5 */
#define OPERATING_MODE_4b 0x4b  /**< 200, 1.0 */
#define OPERATING_MODE_5  0x05  /**< 300, 0.5 */

#define CHANNEL_FUNCTION_FIXED            0x00  /**< Fixed channel */
#define CHANNEL_FUNCTION_TR51CF           0x01  /**< TR51CF */
#define CHANNEL_FUNCTION_DH1CF            0x02  /**< Direct Hash */
#define CHANNEL_FUNCTION_VENDOR_DEFINED   0x03  /**< vendor given channel hop schedule */

#define CHANNEL_SPACING_200     0x00    /**< 200 khz */
#define CHANNEL_SPACING_400     0x01    /**< 400 khz */
#define CHANNEL_SPACING_600     0x02    /**< 600 khz */
#define CHANNEL_SPACING_100     0x03    /**< 100 khz */
#define CHANNEL_SPACING_250     0x04    /**< 250 khz */
#define CHANNEL_SPACING_800     0x05    /**< 800 khz */
#define CHANNEL_SPACING_1200    0x06    /**< 1200 khz */

/*
 *  Network Size definitions are device amount in hundreds of devices.
 *  These definitions are meant to give some estimates of sizes. Any value can be given as parameter
 */

#define NETWORK_SIZE_CERTIFICATE    0x00    /**< Network configuration used in Wi-SUN certification */
#define NETWORK_SIZE_SMALL          0x01    /**< Small networks */
#define NETWORK_SIZE_MEDIUM         0x08    /**< 100 - 800 device networks are medium sized */
#define NETWORK_SIZE_LARGE          0x0F    /**< 800 - 1500 device networks are large */
#define NETWORK_SIZE_XLARGE         0x19    /**< 2500+ devices */
#define NETWORK_SIZE_AUTOMATIC      0xFF    /**< Automatic network size */

/**
 * \brief Neighbor type to differentiate the Role of the neighbor.
 */

typedef enum {
    WS_OTHER = 0,            /**< temporary or soon to be removed neighbor*/
    WS_PRIMARY_PARENT,       /**< Primary parent used for upward packets and used from Border router downwards*/
    WS_SECONDARY_PARENT,     /**< Secondary parent reported to border router and might be used as alternate route*/
    WS_CANDIDATE_PARENT,     /**< Candidate neighbor that is considered as parent if there is problem with active parents*/
    WS_CHILD                 /**< Child with registered address*/
} ws_management_neighbor_type_e;


/** Temporary API change flag. this will be removed when new version of API is implemented on applications
 *
 */
#define WS_MANAGEMENT_API_VER_2 /**< Management API version */

/**
 * Deprecated!
 * Configure PHY mode ID of Wi-SUN stack as defined by Wi-SUN FAN 1.1.
 *
 * \param interface_id Network interface ID.
 * \param phy_mode_id PHY mode ID. Default 255 (not used).
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
#define ws_management_phy_mode_id_set(interface_id, phy_mode_id) ws_management_domain_configuration_set(interface_id, 0, phy_mode_id, 0)

/**
 * Deprecated!
 * Configure Channel plan ID of Wi-SUN stack as defined by Wi-SUN FAN 1.1.
 *
 * \param interface_id Network interface ID.
 * \param channel_plan_id Channel plan ID. Default 255 (not used).
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
#define ws_management_channel_plan_id_set(interface_id, channel_plan_id) ws_management_domain_configuration_set(interface_id, 0, 0, channel_plan_id)

/**
 * \brief Struct ws_statistics defines the Wi-SUN statistics storage structure.
 */
typedef struct ws_statistics {
    /** Asynch TX counter */
    uint32_t asynch_tx_count;
    /** Asynch RX counter */
    uint32_t asynch_rx_count;


    /** Time spent in individual Wi-SUN join state 1 Discovery*/
    uint32_t join_state_1;
    /** Time spent in individual Wi-SUN join state 2 Authentication*/
    uint32_t join_state_2;
    /** Time spent in individual Wi-SUN join state 3 Configuration learn*/
    uint32_t join_state_3;
    /** Time spent in individual Wi-SUN join state 4 RPL parent discovery*/
    uint32_t join_state_4;
    /** Time spent in individual Wi-SUN join state 5 Active state*/
    uint32_t join_state_5;


    /** Amount of Wi-SUN Pan Advertisement Solicit Message sent*/
    uint32_t sent_PAS;
    /** Amount of Wi-SUN Pan Advertisement Message sent*/
    uint32_t sent_PA;
    /** Amount of Wi-SUN Pan Configuration Solicit Message sent*/
    uint32_t sent_PCS;
    /** Amount of Wi-SUN Pan Configuration Message sent*/
    uint32_t sent_PC;

    /** Amount of Wi-SUN Pan Advertisement Solicit Message sent*/
    uint32_t recv_PAS;
    /** Amount of Wi-SUN Pan Advertisement Message sent*/
    uint32_t recv_PA;
    /** Amount of Wi-SUN Pan Configuration Solicit Message sent*/
    uint32_t recv_PCS;
    /** Amount of Wi-SUN Pan Configuration Message sent*/
    uint32_t recv_PC;

    /** New Neighbours found */
    uint32_t Neighbour_add;
    /** New Neighbours Removed */
    uint32_t Neighbour_remove;
    /** New Child added */
    uint32_t Child_add;
    /** Child lost */
    uint32_t child_remove;

} ws_statistics_t;

/**
 * \brief Struct ws_info defines the Wi-SUN stack state.
 */
typedef struct ws_stack_info {
    /** Parent link local address */
    uint8_t parent[16];
    /** parent RSSI Out measured RSSI value calculated using EWMA specified by Wi-SUN from range of -174 (0) to +80 (254) dBm.*/
    uint8_t rsl_out;
    /** parent RSSI in measured RSSI value calculated using EWMA specified by Wi-SUN from range of -174 (0) to +80 (254) dBm.*/
    uint8_t rsl_in;
    /** Device RF minimum sensitivity configuration. lowest level of radio signal strength packet heard. Range of -174 (0) to +80 (254) dBm*/
    uint8_t device_min_sens;
    /** ETX To border router */
    uint16_t routing_cost;
    /** Network PAN ID */
    uint16_t pan_id;
    /** Wi-SUN join state defined by Wi-SUN specification 1-5*/
    uint8_t join_state;
} ws_stack_info_t;

/**
 * \brief Struct ws_neighbour_info_t Gives the neighbour information.
 */
typedef struct ws_neighbour_info {
    /** Link local address*/
    uint8_t link_local_address[16];
    /** Global address if it is known set to 0 if not available*/
    uint8_t global_address[16];
    /** parent RSSI Out measured RSSI value calculated using EWMA specified by Wi-SUN from range of -174 (0) to +80 (254) dBm.*/
    uint8_t rsl_out;
    /** parent RSSI in measured RSSI value calculated using EWMA specified by Wi-SUN from range of -174 (0) to +80 (254) dBm.*/
    uint8_t rsl_in;
    /** RPL Rank value for parents 0xffff for neighbors RANK is unknown*/
    uint16_t rpl_rank;
    /** Measured ETX value if known set to 0xFFFF if not known or Child*/
    uint16_t etx;
    /** Remaining lifetime Link lifetime for parents and ARO lifetime for children*/
    uint32_t lifetime;
    /** Neighbour type (Primary Parent, Secondary Parent, Candidate parent, child, other(Temporary neighbours))*/
    ws_management_neighbor_type_e type;
} ws_neighbour_info_t;

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
 * Get the network name
 *
 * \param interface_id Network interface ID.
 * \param network_name_ptr Nul terminated Network name limited to 32 characters.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_network_name_get(
    int8_t interface_id,
    char *network_name_ptr);

/**
 * Validate the network name
 *
 * \param interface_id Network interface ID.
 * \param network_name_ptr Nul terminated Network name limited to 32 characters.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_network_name_validate(
    int8_t interface_id,
    char *network_name_ptr);

/**
 * Set domain configuration of Wi-SUN stack.
 *
 * Change the default configuration for Wi-SUN PHY operation.
 *
 * Supported values:
 *     Regulatory domain: "NA"(0x01), "KR"(0x09), "EU"(0x03), "IN"(0x05), "BZ"(0x07), "JP"(0x09), "WW"(0x00)
 *
 *     PHY mode ID:
 *         FSK without FEC:
 *         PHY mode ID | Symbol Rate (kbps) | Modulation Index
 *                   1                   50                0.5
 *                   2                   50                1.0
 *                   3                  100                0.5
 *                   4                  100                1.0
 *                   5                  150                0.5
 *                   6                  200                0.5
 *                   7                  200                1.0
 *                   8                  300                0.5
 *
 *         FSK with FEC:
 *         PHY mode ID | Symbol Rate (kbps) | Modulation Index
 *                  17                   50                0.5
 *                  18                   50                1.0
 *                  19                  100                0.5
 *                  20                  100                1.0
 *                  21                  150                0.5
 *                  22                  200                0.5
 *                  23                  200                1.0
 *                  24                  300                0.5
 *
 *         OFDM:
 *         PHY mode ID | Option | MCS | Data rate (kbps)
 *                  34        1     2                400
 *                  35        1     3                800
 *                  36        1     4               1200
 *                  37        1     5               1600
 *                  38        1     6               2400
 *                  51        2     3                400
 *                  52        2     4                600
 *                  53        2     5                800
 *                  54        2     6               1200
 *                  68        3     4                300
 *                  69        3     5                400
 *                  70        3     6                600
 *                  84        4     4                150
 *                  85        4     5                200
 *                  86        4     6                300
 *
 *     Channel plan ID:
 *         North America (NA):  (1), (2), (5)
 *         Brazil (BZ):         (1), (2), (5)
 *
 * If value of 0 is given then previous value is used.
 * If value of 255 is given then default value is used.
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain Regulatory domain.
 * \param phy_mode_id PHY mode ID.
 * \param channel_plan_id Channel plan ID.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_domain_configuration_set(
    int8_t interface_id,
    uint8_t regulatory_domain,
    uint8_t phy_mode_id,
    uint8_t channel_plan_id);

/**
 * Get domain configuration of Wi-SUN stack.
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain Regulatory domain.
 * \param phy_mode_id PHY mode ID.
 * \param channel_plan_id Channel plan ID.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_domain_configuration_get(
    int8_t interface_id,
    uint8_t *regulatory_domain,
    uint8_t *phy_mode_id,
    uint8_t *channel_plan_id);

/**
 * Validate domain configuration of Wi-SUN stack.
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain Regulatory domain.
 * \param phy_mode_id PHY mode ID.
 * \param channel_plan_id Channel plan ID.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_domain_configuration_validate(
    int8_t interface_id,
    uint8_t regulatory_domain,
    uint8_t phy_mode_id,
    uint8_t channel_plan_id);

/**
 * Configure regulatory domain of Wi-SUN stack.
 *
 * Change the default configuration for Wi-SUN PHY operation.
 *
 * Supported values:
 * Domain: "NA"(0x01), "KR"(0x09), "EU"(0x03), "IN"(0x05), "KR"(0x09), "JP"(0x09), "WW"(0x00)
 * Operating class: (1), (2), (3), (4)
 * Operation mode: "1a" (symbol rate 50, modulation index 0.5)
 *                 "1b" (symbol rate 50, modulation index 1.0)
 *                 "2a" (symbol rate 100, modulation index 0.5)
 *                 "2b" (symbol rate 100, modulation index 1.0)
 *                 "3"  (symbol rate 150, modulation index 0.5)
 *                 "4a" (symbol rate 200, modulation index 0.5)
 *                 "4b" (symbol rate 200, modulation index 1.0)
 *                 "5"  (symbol rate 300, modulation index 0.5)
 *
 * if value of 255 is given then previous value is used.
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain FHSS regulatory domain. Default to "EU" 0x03.
 * \param operating_class FHSS operating class. Default to 2.
 * \param operating_mode FHSS phy operating mode. Default to "3".
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
 * Get regulatory domain of Wi-SUN stack.
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain FHSS regulatory domain.
 * \param operating_class FHSS operating class.
 * \param operating_mode FHSS phy operating mode.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_regulatory_domain_get(
    int8_t interface_id,
    uint8_t *regulatory_domain,
    uint8_t *operating_class,
    uint8_t *operating_mode);

/**
 * Validate regulatory domain of Wi-SUN stack.
 *
 * \param interface_id Network interface ID.
 * \param regulatory_domain FHSS regulatory domain.
 * \param operating_class FHSS operating class.
 * \param operating_mode FHSS phy operating mode.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_regulatory_domain_validate(
    int8_t interface_id,
    uint8_t regulatory_domain,
    uint8_t operating_class,
    uint8_t operating_mode);

/**
 * Set timing parameters related to network size.
 *
 * timing parameters follows the specification example from Wi-SUN specification
 *
 * Default value: medium 100 - 800 device
 * small network size: less than 100 devices
 * Large network size: 800 - 1500 devices
 * automatic: when discovering the network network size is learned
 *            from advertisements and timings adjusted accordingly
 *
 * When network size is changed, it will override following configuration values:
 * - Timing settings set by ws_management_timing_parameters_set()
 * - RPL settings set by ws_bbr_rpl_parameters_set()
 *
 * If values should be other than defaults set by stack, they need to set using
 * above function calls after network size change.
 *
 * \param interface_id Network interface ID.
 * \param network_size Network size in hundreds of devices, certificate or automatic.
 *                     See NETWORK_SIZE_ definition.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_network_size_set(
    int8_t interface_id,
    uint8_t network_size);

/**
 * Get timing parameters related to network size.
 *
 * \param interface_id Network interface ID.
 * \param network_size Network size in hundreds of devices, certificate or automatic.
 *                     See NETWORK_SIZE_ definition.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_network_size_get(
    int8_t interface_id,
    uint8_t *network_size);

/**
 * Validate timing parameters related to network size.
 *
 * \param interface_id Network interface ID.
 * \param network_size Network size in hundreds of devices, certificate or automatic.
 *                     See NETWORK_SIZE_ definition.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_network_size_validate(
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
 * Get channel mask for FHSS operation.
 *
 * \param interface_id Network interface ID.
 * \param channel_mask set bits matching the channel 1 to allow channel 0 to disallow.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_channel_mask_get(
    int8_t interface_id,
    uint32_t *channel_mask);

/**
 * Validate channel mask for FHSS operation.
 *
 * \param interface_id Network interface ID.
 * \param channel_mask set bits matching the channel 1 to allow channel 0 to disallow.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_channel_mask_validate(
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
 * Calling with fhss_uc_dwell_interval = 0, fhss_broadcast_interval = 0xffffffff,
 * fhss_bc_dwell_interval = 0 restores stack defaults
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
 * Calling with dwell_interval = 0, channel_function = 0xff, fixed_channel = 0xffff restores stack defaults
 *
 * \param interface_id Network interface ID.
 * \param channel_function Unicast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel.
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
 * Get unicast channel function.
 *
 * \param interface_id Network interface ID.
 * \param channel_function Unicast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel.
 * \param dwell_interval Used dwell interval when channel function is TR51 or DH1.
 *
 * \return 0, OK.
 * \return <0 fail.
 */
int ws_management_fhss_unicast_channel_function_get(
    int8_t interface_id,
    uint8_t *channel_function,
    uint16_t *fixed_channel,
    uint8_t *dwell_interval);

/**
 * Validate unicast channel function.
 *
 * \param interface_id Network interface ID.
 * \param channel_function Unicast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel.
 * \param dwell_interval Used dwell interval when channel function is TR51 or DH1.
 *
 * \return 0, OK.
 * \return <0 fail.
 */
int ws_management_fhss_unicast_channel_function_validate(
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
 * Calling with dwell_interval = 0, channel_function = 0xff,
 * broadcast_interval = 0xffffffff, fixed_channel = 0xffff restores stack defaults
 *
 * \param interface_id Network interface ID.
 * \param channel_function Broadcast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel.
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
 * Get broadcast channel function.
 *
 * \param interface_id Network interface ID.
 * \param channel_function Broadcast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel.
 * \param dwell_interval Broadcast channel dwell interval.
 * \param broadcast_interval Broadcast interval.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_fhss_broadcast_channel_function_get(
    int8_t interface_id,
    uint8_t *channel_function,
    uint16_t *fixed_channel,
    uint8_t *dwell_interval,
    uint32_t *broadcast_interval);

/**
 * Validate broadcast channel function.
 *
 * \param interface_id Network interface ID.
 * \param channel_function Broadcast channel function.
 * \param fixed_channel Used channel when channel function is fixed channel.
 * \param dwell_interval Broadcast channel dwell interval.
 * \param broadcast_interval Broadcast interval.
 *
 * \return 0, OK.
 * \return <0 Fail.
 */
int ws_management_fhss_broadcast_channel_function_validate(
    int8_t interface_id,
    uint8_t channel_function,
    uint16_t fixed_channel,
    uint8_t dwell_interval,
    uint32_t broadcast_interval);

/**
 * Set timing parameters
 *
 * Sets the parameters for the PAN discovery trickle timer and PAN timeout.
 *
 * When network size is changed using ws_management_network_size_set(),
 * it will override the configuration values set by this function.
 *
 * \param interface_id Network interface ID.
 * \param disc_trickle_imin Discovery trickle Imin
 * \param disc_trickle_imax Discovery trickle Imax.
 * \param disc_trickle_k Discovery trickle k.
 * \param pan_timeout PAN timeout.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_timing_parameters_set(
    int8_t interface_id,
    uint16_t disc_trickle_imin,
    uint16_t disc_trickle_imax,
    uint8_t disc_trickle_k,
    uint16_t pan_timeout);

/**
 * Get timing parameters
 *
 * Gets the parameters for the PAN discovery trickle timer and PAN timeout.
 *
 * \param interface_id Network interface ID.
 * \param disc_trickle_imin Discovery trickle Imin
 * \param disc_trickle_imax Discovery trickle Imax.
 * \param disc_trickle_k Discovery trickle k.
 * \param pan_timeout PAN timeout.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_timing_parameters_get(
    int8_t interface_id,
    uint16_t *disc_trickle_imin,
    uint16_t *disc_trickle_imax,
    uint8_t *disc_trickle_k,
    uint16_t *pan_timeout);

/**
 * Validate timing parameters
 *
 * Validates the parameters for the PAN discovery trickle timer and PAN timeout.
 *
 * \param interface_id Network interface ID.
 * \param disc_trickle_imin Discovery trickle Imin
 * \param disc_trickle_imax Discovery trickle Imax.
 * \param disc_trickle_k Discovery trickle k.
 * \param pan_timeout PAN timeout.
 *
 * \return 0, Init OK.
 * \return <0 Init fail.
 */
int ws_management_timing_parameters_validate(
    int8_t interface_id,
    uint16_t disc_trickle_imin,
    uint16_t disc_trickle_imax,
    uint8_t disc_trickle_k,
    uint16_t pan_timeout);

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

/**
 * Get information from the stack state.
 * Parent information and link qualities with stack state info
 *
 * \param interface_id Network interface ID.
 * \param info_ptr Pointer to stored stack state.
 *
 * \return 0 Success.
 * \return <0 Failure.
 */
int ws_stack_info_get(
    int8_t interface_id,
    ws_stack_info_t *info_ptr);

/**
 * Get Neighbor table information from stack.
 *
 * To allocate correct amount of memory first use the API with NULL to get current amount
 * of neighbors. Then Allocate the memory and call the function to fill the table.
 *
 * \param interface_id Network interface ID.
 * \param neighbor_ptr Pointer to memory where Neighbor table entries can be written.
 * \param count amount of neighbor table entries allocated to memory.
 *
 * \return >=0 Success with amount of entries written in table.
 * \return >=0 if neighbor_ptr is NULL returns the amount of neighbors currently.
 * \return <0 Failure.
 */
int ws_neighbor_info_get(
    int8_t interface_id,
    ws_neighbour_info_t *neighbor_ptr,
    uint16_t count);

/**
 * Set minimum RF sensitivity acceptable for the parent selection
 *
 * Set radio signal minimum sensitivity level acceptable for parent selection.
 * Range of -174 (0) to +80 (254) dBm.
 *
 * If device_min_sens is set to 0 then automatic adjustment is done by the stack.
 *
 * Setting a value that is not suitable for Radio might prevent the device joining to the network.
 *
 * This configuration limits the EAPOL parents accepted for Authentication and device must hear signal
 * level higher than device_min_sens + CAND_PARENT_THRESHOLD + CAND_PARENT_HYSTERESIS
 * to start authentication.
 *
 * ETX Calculation gives a maximum ETX if two way EWMA RSL is less than
 * device_min_sens + CAND_PARENT_THRESHOLD + CAND_PARENT_HYSTERESIS to
 * prevent selecting parents with poor signal quality
 *
 * \param interface_id Network interface ID.
 * \param device_min_sens value used in the parent selections.
 *
 * \return 0 Success.
 * \return <0 Failure.
 */
int ws_device_min_sens_set(
    int8_t interface_id,
    uint8_t device_min_sens);

#ifdef __cplusplus
}
#endif
#endif /* WS_MANAGEMENT_API_H_ */
