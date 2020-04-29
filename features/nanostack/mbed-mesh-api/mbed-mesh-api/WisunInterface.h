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

/** Wi-SUN mesh network interface class
 *
 * Configure Nanostack to use Wi-SUN protocol.
 */
class WisunInterface : public MeshInterfaceNanostack {
public:

    /** Create an uninitialized WisunInterface
     *
     *  Must initialize to initialize the mesh on a phy.
     */
    WisunInterface() { }

    /** Create an initialized WisunInterface
     *
     */
    WisunInterface(NanostackRfPhy *phy) : MeshInterfaceNanostack(phy) { }

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
     * \brief Set Wi-SUN network size.
     *
     * Function stores new parameters to mbed-mesh-api and uses them when connect() is called next time.
     * If device is already connected to the Wi-SUN network then device will restart network discovery after
     * changing the network size.
     *
     * Default value: medium
     * Small network size: less than hundred devices
     * Medium network size: hundreds of devices
     * Large network size: thousands of devices
     * Certificate: used on testing
     *
     * When network size is changed, it will override all or some of the following configuration values:
     * - Timing settings set by set_timing_parameters() of the Wi-SUN interface.
     * - RPL settings set by rpl_parameters_set() of the Border Router interface.
     *
     * When network size is changed, and if timing or RPL values should be other than defaults set by stack for the network size,
     * they need to set again using above function calls.
     *
     * \param network_size Network size in hundreds of devices (e.g. 1200 devices is 12), 0x00 for network size certificate.
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
     * \brief Set own certificate and private key reference to the Wi-SUN network.
     *
     * Function can be called several times if intermediate certificates are used. Then each call to the function
     * adds a certificate reference to own certificate chain. Certificates are in bottom up order i.e. the top certificate is given last.
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
     * Function can be called several times. Certificates are in bottom up order i.e. the top certificate is given last.
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

protected:
    Nanostack::WisunInterface *get_interface() const;
    virtual nsapi_error_t do_initialize();
    virtual nsapi_error_t configure();
};

#endif
