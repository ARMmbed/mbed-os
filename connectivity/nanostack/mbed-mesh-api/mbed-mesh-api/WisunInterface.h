/*
 * Copyright (c) 2018-2019 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WISUNINTERFACE_H
#define WISUNINTERFACE_H

#include "MeshInterfaceNanostack.h"

/**
 * \brief Struct ws_rpl_info Wi-SUN router RPL information.
 */
typedef struct ws_rpl_info {
    /** Router dodag id */
    uint8_t rpl_dodag_id[16];
    /** Router instance identifier */
    uint8_t instance_id;
    /** RPL version number */
    uint8_t version;
    /** RPL DODAG node current Rank */
    uint16_t current_rank;
    /** RPL Primary Parent Rank */
    uint16_t primary_parent_rank;
} ws_rpl_info_t;

/**
 * \brief Struct ws_stack_state Wi-SUN stack information.
 */
typedef struct ws_stack_state {
    /** Mesh Interface Global IPv6 Address */
    uint8_t global_addr[16];
    /** Mesh Interface Link Local IPv6 Address */
    uint8_t link_local_addr[16];
    /** Parent link local address */
    uint8_t parent_addr[16];
    /** parent RSSI Out measured RSSI value calculated using EWMA specified by Wi-SUN from range of -174 (0) to +80 (254) dBm.*/
    uint8_t rsl_out;
    /** parent RSSI in measured RSSI value calculated using EWMA specified by Wi-SUN from range of -174 (0) to +80 (254) dBm.*/
    uint8_t rsl_in;
    /** Wi-SUN join state defined by Wi-SUN specification 1-5 */
    uint8_t join_state;
    /** Network PAN ID */
    uint16_t pan_id;
    /** Device RF minimum sensitivity configuration. lowest level of radio signal strength packet heard. Range of -174 (0) to +80 (254) dBm*/
    uint8_t device_min_sens;
} ws_stack_state_t;

/**
 * \brief Struct ws_cca_threshold_table Wi-SUN CCA threshold table information.
 */
typedef struct ws_cca_threshold_table {
    /** Number of channels */
    uint8_t number_of_channels;
    /** CCA threshold table */
    const int8_t *cca_threshold_table;
} ws_cca_threshold_table_t;

typedef enum {
    WISUN_OTHER = 0,            /**< temporary or soon to be removed neighbor*/
    WISUN_PRIMARY_PARENT,       /**< Primary parent used for upward packets and used from Border router downwards*/
    WISUN_SECONDARY_PARENT,     /**< Secondary parent reported to border router and might be used as alternate route*/
    WISUN_CANDIDATE_PARENT,     /**< Candidate neighbor that is considered as parent if there is problem with active parents*/
    WISUN_CHILD                 /**< Child with registered address*/
} ws_nbr_type_e;

/**
 * \brief Struct ws_nbr_info_t Gives the neighbor information.
 */
typedef struct ws_nbr_info {
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
    ws_nbr_type_e type;
} ws_nbr_info_t;


/** Wi-SUN mesh network interface class
 *
 * Configure Nanostack to use Wi-SUN protocol.
 */
class WisunInterface final : public MeshInterfaceNanostack {
public:
    /** Inherit MeshInterfaceNanostack constructors */
    using MeshInterfaceNanostack::MeshInterfaceNanostack;

    /**
     * \brief Set Wi-SUN network name.
     *
     * Function stores new network name to mbed-mesh-api and uses it when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the network name.
     *
     * Function overwrites network name defined by Mbed OS configuration.
     *
     * \param network_name Network name as NUL terminated string. Can't exceed 32 characters and can't be NULL.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_network_name(char *network_name);

    /**
     * \brief Get Wi-SUN network name.
     *
     * Function reads network name from mbed-mesh-api.
     *
     * \param network_name Network name as NUL terminated string. Must have space for 33 characters (string and null terminator).
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_network_name(char *network_name);

    /**
     * \brief Validate Wi-SUN network name.
     *
     * Function validates network name. Function can be used to test that values that will be used on set function are valid.
     *
     * \param network_name Network name as NUL terminated string. Can't exceed 32 characters and can't be NULL.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_network_name(char *network_name);

    /**
     * \brief Set Wi-SUN network regulatory domain, operating class and operating mode.
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the regulatory_domain, operating_class or operating_mode.
     *
     * Function overwrites parameters defined by Mbed OS configuration.
     *
     * \param regulatory_domain Values defined in Wi-SUN PHY-specification. Use 0xff to use leave parameter unchanged.
     * \param operating_class Values defined in Wi-SUN PHY-specification.  Use 0xff to use leave parameter unchanged.
     * \param operating_mode Values defined in Wi-SUN PHY-specification. Use 0xff to use leave parameter unchanged.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_network_regulatory_domain(uint8_t regulatory_domain = 0xff, uint8_t operating_class = 0xff, uint8_t operating_mode = 0xff);

    /**
     * \brief Get Wi-SUN network regulatory domain, operating class and operating mode.
     *
     * Function reads regulatory_domain, operating_class and operating_mode from mbed-mesh-api.
     *
     * \param regulatory_domain Values defined in Wi-SUN PHY-specification.
     * \param operating_class Values defined in Wi-SUN PHY-specification.
     * \param operating_mode Values defined in Wi-SUN PHY-specification.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_network_regulatory_domain(uint8_t *regulatory_domain, uint8_t *operating_class, uint8_t *operating_mode);

    /**
     * \brief Validate Wi-SUN network regulatory domain, operating class and operating mode.
     *
     * Function validates regulatory_domain, operating_class and operating_mode. Function can be used to test that values that will
     * be used on set function are valid.
     *
     * \param regulatory_domain Values defined in Wi-SUN PHY-specification.
     * \param operating_class Values defined in Wi-SUN PHY-specification.
     * \param operating_mode Values defined in Wi-SUN PHY-specification.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_network_regulatory_domain(uint8_t regulatory_domain, uint8_t operating_class, uint8_t operating_mode);

    /**
     * \brief Set Wi-SUN network regulatory domain, PHY mode ID and channel plan ID.
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the regulatory_domain, phy_mode_id or channel_plan_id.
     *
     * \param regulatory_domain Values defined in Wi-SUN PHY-specification. Use 0 to leave parameter unchanged or 0xff to use default value.
     * \param phy_mode_id Values defined in Wi-SUN PHY-specification. Use 0 to leave parameter unchanged or 0xff to use default value.
     * \param channel_plan_id Values defined in Wi-SUN PHY-specification. Use 0 to leave parameter unchanged or 0xff to use default value.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_network_domain_configuration(uint8_t regulatory_domain, uint8_t phy_mode_id, uint8_t channel_plan_id);

    /**
     * \brief Get Wi-SUN network regulatory domain, PHY mode ID and channel plan ID.
     *
     * Function reads regulatory_domain, phy_mode_id and channel_plan_id from mbed-mesh-api.
     *
     * \param regulatory_domain Values defined in Wi-SUN PHY-specification.
     * \param phy_mode_id Values defined in Wi-SUN PHY-specification.
     * \param channel_plan_id Values defined in Wi-SUN PHY-specification.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_network_domain_configuration(uint8_t *regulatory_domain, uint8_t *phy_mode_id, uint8_t *channel_plan_id);

    /**
     * \brief Validate Wi-SUN network regulatory domain, PHY mode ID and channel plan ID.
     *
     * Function validates regulatory_domain, phy_mode_id and channel_plan_id. Function can be used to test that values that will
     * be used on set function are valid.
     *
     * \param regulatory_domain Values defined in Wi-SUN PHY-specification.
     * \param phy_mode_id Values defined in Wi-SUN PHY-specification.
     * \param channel_plan_id Values defined in Wi-SUN PHY-specification.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_network_domain_configuration(uint8_t regulatory_domain, uint8_t phy_mode_id, uint8_t channel_plan_id);

    /**
     * \brief Set Wi-SUN network size.
     *
     * Function stores network size parameter to the mbed-mesh-api and uses it when connect() is called for the next
     * time. If a device is already connected to the Wi-SUN network, then the device will restart network discovery
     * after changing the network size.
     *
     * It is recommended to set the correct network size because some Wi-SUN network configuration parameters are
     * adjusted based on the selected network size. A network configured for a small amount of devices may not work
     * optimally for large number of devices. This is because the network bandwidth is divided with all the devices in
     * the network. Enough bandwidth must be reserved for application data usage as well as the Wi-SUN network
     * operations. In addition, the application should adapt to the network characteristics by using the InternetSocket
     * methods get_stagger_estimate_to_address() and get_rtt_estimate_to_address().
     *
     * The network size is measured as hundreds of devices that are expected to join to the network. For example,
     * for a 400-device network set network size to 4.
     *
     * The Wi-SUN stack will automatically adjust timing and RPL configuration values based on the selected network
     * size and data rate. If a customized timing or RPL values are needed, the APIs below should be invoked after
     * changing the network size:
     * - set_timing_parameters() to set timing settings to the Wi-SUN interface.
     * - rpl_parameters_set() to set RPL settings to the Border Router interface.
     *
     * By default the Wi-SUN stack is configured to use a few hundreds of devices.
     *
     * The network size should be set to 0 when running certification tests.
     *
     * \param network_size Network size in hundreds of devices (e.g. 12 for 1200 devices), 0 for certificate testing.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_network_size(uint8_t network_size);

    /**
     * \brief Get Wi-SUN network size.
     *
     * Function reads network size from mbed-mesh-api.
     *
     * \param network_size Network size in hundreds of devices, 0x00 for network size certificate.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_network_size(uint8_t *network_size);

    /**
     * \brief Validate Wi-SUN network size.
     *
     * Function validates network size from mbed-mesh-api. Function can be used to test that values that will
     * be used on set function are valid.
     *
     * \param network_size Network size in hundreds of devices, 0x00 for network size certificate.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_network_size(uint8_t network_size);

    /**
     * \brief Set Wi-SUN FHSS channel mask
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then settings take effect right away.
     *
     * \param channel_mask Values defined in Wi-SUN management API. Channel mask bit field.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_channel_mask(uint32_t channel_mask[8]);

    /**
     * \brief Get Wi-SUN FHSS channel mask
     *
     * Function reads FHSS channel mask from mbed-mesh-api.
     *
     * \param channel_mask Values defined in Wi-SUN management API. Channel mask bit field.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_channel_mask(uint32_t *channel_mask);

    /**
     * \brief Validate Wi-SUN FHSS channel mask
     *
     * Function validates FHSS channel mask. Function can be used to test that values that will
     * be used on set function are valid.
     *
     * \param channel_mask Values defined in Wi-SUN management API. Channel mask bit field.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_channel_mask(uint32_t channel_mask[8]);

    /**
     * \brief Set Wi-SUN FHSS unicast channel function parameters
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the channel function, fixed channel or dwell interval.
     *
     * Function overwrites parameters defined by Mbed OS configuration.
     *
     * \param channel_function Channel function. Fixed, TR51CF, DH1CF or vendor defined.
     * \param fixed_channel Used channel when channel function is fixed channel. Use 0xffff when fixed channel function not on use.
     * \param dwell_interval Used dwell interval when channel function is TR51 or DH1. Use 0x00 to use leave parameter unchanged.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_unicast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel = 0xffff, uint8_t dwell_interval = 0x00);

    /**
     * \brief Get Wi-SUN FHSS unicast channel function parameters
     *
     * Function reads FHSS unicast channel function parameters from mbed-mesh-api.
     *
     * \param channel_function Channel function. Fixed, TR51CF, DH1CF or vendor defined.
     * \param fixed_channel Used channel when channel function is fixed channel.
     * \param dwell_interval Used dwell interval when channel function is TR51 or DH1.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_unicast_channel_function(mesh_channel_function_t *channel_function, uint16_t *fixed_channel, uint8_t *dwell_interval);

    /**
     * \brief Validate Wi-SUN FHSS unicast channel function parameters
     *
     * Function validates FHSS unicast channel function parameters. Function can be used to test that values that will
     * be used on set function are valid.
     *
     * \param channel_function Channel function. Fixed, TR51CF, DH1CF or vendor defined.
     * \param fixed_channel Used channel when channel function is fixed channel.
     * \param dwell_interval Used dwell interval when channel function is TR51 or DH1.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_unicast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel, uint8_t dwell_interval);

    /**
     * \brief Set Wi-SUN FHSS broadcast channel function parameters
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the channel function, fixed channel, dwell interval or broadcast_interval.
     *
     * Function overwrites parameters defined by Mbed OS configuration.
     *
     * \param channel_function Channel function. Fixed, TR51CF, DH1CF or vendor defined.
     * \param fixed_channel Used channel when channel function is fixed channel. Use 0xffff when fixed channel function not on use.
     * \param dwell_interval Used dwell interval when channel function is TR51 or DH1. Use 0x00 to use leave parameter unchanged.
     * \param broadcast_interval Used broadcast interval. Use 0x00 to use leave parameter unchanged.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_broadcast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel = 0xffff, uint8_t dwell_interval = 0x00, uint32_t broadcast_interval = 0x00);

    /**
     * \brief Get Wi-SUN FHSS broadcast channel function parameters
     *
     * Function reads FHSS broadcast channel function parameters from mbed-mesh-api.
     *
     * \param channel_function Channel function. Fixed, TR51CF, DH1CF or vendor defined.
     * \param fixed_channel Used channel when channel function is fixed channel.
     * \param dwell_interval Used dwell interval when channel function is TR51 or DH1.
     * \param broadcast_interval Used broadcast interval.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_broadcast_channel_function(mesh_channel_function_t *channel_function, uint16_t *fixed_channel, uint8_t *dwell_interval, uint32_t *broadcast_interval);

    /**
     * \brief Validate Wi-SUN FHSS broadcast channel function parameters
     *
     * Function validates FHSS broadcast channel function parameters from mbed-mesh-api. Function can be used to test that values that will
     * be used on set function are valid.
     *
     * \param channel_function Channel function. Fixed, TR51CF, DH1CF or vendor defined.
     * \param fixed_channel Used channel when channel function is fixed channel.
     * \param dwell_interval Used dwell interval when channel function is TR51 or DH1.
     * \param broadcast_interval Used broadcast interval.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_broadcast_channel_function(mesh_channel_function_t channel_function, uint16_t fixed_channel, uint8_t dwell_interval, uint32_t broadcast_interval);

    /**
     * \brief Set Wi-SUN timing parameters
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then settings take effect right away.
     *
     * \param disc_trickle_imin Discovery trickle Imin. Range 1-255 seconds. Use 0x00 to use leave parameter unchanged.
     * \param disc_trickle_imax Discovery trickle Imax. Range (2-2^8)*Imin. Use 0x00 to use leave parameter unchanged.
     * \param disc_trickle_k Discovery trickle k. Use 0x00 to use leave parameter unchanged.
     * \param pan_timeout PAN timeout; seconds; Range 60-15300 seconds. Use 0x00 to use leave parameter unchanged.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_timing_parameters(uint16_t disc_trickle_imin = 0x00, uint16_t disc_trickle_imax = 0x00, uint8_t disc_trickle_k = 0x00, uint16_t pan_timeout = 0x00);

    /**
     * \brief Get Wi-SUN timing parameters
     *
     * Function reads timing parameters from mbed-mesh-api.
     *
     * \param disc_trickle_imin Discovery trickle Imin. Range 1-255 seconds.
     * \param disc_trickle_imax Discovery trickle Imax. Range (2-2^8)*Imin.
     * \param disc_trickle_k Discovery trickle k.
     * \param pan_timeout PAN timeout; seconds; Range 60-15300 seconds.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_timing_parameters(uint16_t *disc_trickle_imin, uint16_t *disc_trickle_imax, uint8_t *disc_trickle_k, uint16_t *pan_timeout);

    /**
     * \brief Validates Wi-SUN timing parameters
     *
     * Function validates timing parameters. Function can be used to test that values that will be used on set
     * function are valid.
     *
     * \param disc_trickle_imin Discovery trickle Imin. Range 1-255 seconds.
     * \param disc_trickle_imax Discovery trickle Imax. Range (2-2^8)*Imin.
     * \param disc_trickle_k Discovery trickle k.
     * \param pan_timeout PAN timeout; seconds; Range 60-15300 seconds.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_timing_parameters(uint16_t disc_trickle_imin, uint16_t disc_trickle_imax, uint8_t disc_trickle_k, uint16_t pan_timeout);

    /**
     * \brief Set Wi-SUN device minimum sensitivity
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then settings take effect right away.
     *
     * \param device_min_sens Device minimum sensitivity. Range 0(-174 dB) to 254(+80 dB).
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_device_min_sens(uint8_t device_min_sens);

    /**
     * \brief Get Wi-SUN device minimum sensitivity.
     *
     * Function reads device minimum sensitivity from mbed-mesh-api.
     *
     * \param device_min_sens Device minimum sensitivity. Range 0-254.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_device_min_sens(uint8_t *device_min_sens);

    /**
     * \brief Validates Device minimum sensitivity.
     *
     * Function validates device minimum sensitivity. Function can be used to test that values that will be used on set
     * function are valid.
     *
     * \param device_min_sens Device minimum sensitivity. Range 0-254.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_device_min_sens(uint8_t device_min_sens);

    /**
     * \brief Set own certificate and private key reference to the Wi-SUN network.
     *
     * Function can be called several times if intermediate certificates are used. Then each call to the function
     * adds a certificate reference to own certificate chain. Certificates are in bottom up order i.e. own certificate
     * is given first and the top certificate is given last.
     *
     * PEM formatted certificates must use either "\n" or "\r\n" as line separator. PEM formatted certificates
     * must be NUL terminated and the NUL terminator is counted to certificate length.
     *
     * It is possible to add multiple PEM certificates concatenated together in one call set_own_certificate(). In that
     * case certificates are in bottom up order i.e. own certificate is given first and the top certificate is given
     * last. NUL terminator is added after the last concatenated certificate and the NUL terminator is counted to
     * total concatenated certificate length.
     *
     * Function must be called before connecting the device i.e before call to connect() method.
     * Function will not copy certificate or key, therefore addresses must remain valid.
     *
     * \param cert Certificate address.
     * \param cert_len Certificate length in bytes.
     * \param cert_key Certificate key address.
     * \param cert_key_len Certificate key length in bytes.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_STATE if method is called after calling connect().
     * \return MESH_ERROR_MEMORY in case of memory allocation failure.
     * */
    mesh_error_t set_own_certificate(uint8_t *cert, uint16_t cert_len, uint8_t *cert_key = NULL, uint16_t cert_key_len = 0);

    /**
     * \brief Remove own certificates from the Wi-SUN network.
     *
     * Function must be called before connecting the device i.e before call to connect() method.
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_STATE if method is called after calling connect().
     * */
    mesh_error_t remove_own_certificates(void);

    /**
     * \brief Set trusted certificate reference to the Wi-SUN network.
     *
     * Function can be called several times. Each call to the function adds a trusted certificate to Wi-SUN.
     *
     * PEM formatted certificates must use either "\n" or "\r\n" as line separator. PEM formatted certificates
     * must be NUL terminated and the NUL terminator is counted to certificate length.
     *
     * It is possible to add multiple PEM certificates concatenated together in one call set_trusted_certificate().
     * NUL terminator is added after the last concatenated certificate and the NUL terminator is counted to
     * total concatenated certificate length.
     *
     * Function must be called before connecting the device i.e before call to connect() method.
     * Function will not copy certificate, therefore addresses must remain valid.
     *
     * \param cert Certificate address.
     * \param cert_len Certificate length in bytes.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_STATE if method is called after calling connect().
     * \return MESH_ERROR_MEMORY in case of memory allocation failure.
     * */
    mesh_error_t set_trusted_certificate(uint8_t *cert, uint16_t cert_len);

    /**
     * \brief Remove trusted certificates from the Wi-SUN network.
     *
     * Function must be called before connecting the device i.e before call to connect() method.
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_STATE if method is called after calling connect().
     * */
    mesh_error_t remove_trusted_certificates(void);

    /**
     * \brief Get router IP address
     *
     * \param address
     * \param len
     * */
    bool getRouterIpAddress(char *address, int8_t len);

    /**
     * \brief Enable Wi-SUN statistics
     *
     * After enabling statistics those can be read using the network, physical layer,
     * MAC and FHSS and Wi-SUN statistics read functions.
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN on error
     * */
    mesh_error_t enable_statistics(void);

    /**
     * \brief Reset Wi-SUN statistics
     *
     * Resets MAC statistics and Wi-SUN statistics.
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN on error
     * */
    mesh_error_t reset_statistics(void);

    /**
     * \brief Reads Wi-SUN network statistics
     *
     * Reads network statistics.
     *
     * \param statistics Network statistics.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN on error
     * */
    mesh_error_t read_nw_statistics(mesh_nw_statistics_t *statistics);

    /**
     * \brief Reads Wi-SUN MAC statistics
     *
     * Reads MAC statistics.
     *
     * \param statistics MAC statistics.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN on error
     * */
    mesh_error_t read_mac_statistics(mesh_mac_statistics_t *statistics);

    /**
     * \brief Get Wi-SUN Router information.
     *
     * Function reads RPL information from nanostack.
     * Mesh interface must be initialized before calling this function.
     *
     * \param info_ptr Structure given to stack where information will be stored
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t info_get(ws_rpl_info_t *info_ptr);

    /**
     * \brief Get Wi-SUN Stack information.
     *
     * Function reads Stack information from nanostack.
     * Mesh interface must be initialized before calling this function.
     *
     * \param stack_info_ptr Structure given to stack where information will be stored
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t stack_info_get(ws_stack_state_t *stack_info_ptr);

    /**
     * \brief Get Wi-SUN CCA threshold table information.
     *
     * Function reads CCA threshold table from nanostack.
     *
     ** \param ws_cca_threshold_table_t Structure given to stack where information will be stored
     **
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t cca_threshold_table_get(ws_cca_threshold_table_t *table);

    /**
     * \brief Get Wi-SUN Neighbor table information.
     *
     * To allocate correct amount of memory first use the API with nbr_ptr = NULL to get current amount
     * of neighbors in count pointer. Then Allocate the memory and call the function to fill the table.
     *
     * \param nbr_ptr Pointer to memory where Neighbor table entries can be written.
     * \param count amount of neighbor table entries allocated to memory.
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t nbr_info_get(ws_nbr_info_t *nbr_ptr, uint16_t *count);

protected:
    Nanostack::WisunInterface *get_interface() const;
    nsapi_error_t do_initialize() override;
    virtual nsapi_error_t configure();
};

#endif
