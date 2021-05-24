/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
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

#ifndef WISUNBORDERROUTER_H
#define WISUNBORDERROUTER_H

/**
 * \brief Struct br_information Border router dynamic information.
 */
typedef struct ws_br_info {
    /** Address prefix given to devices in network  set to 0 if not available*/
    uint8_t ipv6_prefix[8];
    /** IID of Border router */
    uint8_t ipv6_iid[8];
    /** Border router dodag id */
    uint8_t rpl_dodag_id[16];
    /** Border router instance identifier defined in RPL */
    uint8_t instance_id;
    /** RPL version number */
    uint8_t version;
    /** Timestamp of the the device. Can be used as version number*/
    uint64_t host_timestamp;
    /** Amount of devices in the network. */
    uint16_t device_count;
    /** Gateway Local Address */
    uint8_t gateway_addr[16];
} ws_br_info_t;

/**
 * \brief Struct br_route_info is parent child relation structure.
 */
typedef struct ws_br_route_info {
    /** IID of target device
     * Public IPv6 address can be formed by combining prefix + IID*/
    uint8_t target[8];
    /** IID of parent*/
    uint8_t parent[8];
} ws_br_route_info_t;

/**
 * \brief Struct ws_br_radius_timing_t is RADIUS timing configuration structure.
 */
typedef struct ws_br_radius_timing {
    /** RADIUS retry trickle timer Imin; in 100ms units; range 1-1200; default 20 (2 seconds) */
    uint16_t radius_retry_imin;
    /** RADIUS retry trickle timer Imax; in 100ms units; range 1-1200; default 30 (3 seconds) */
    uint16_t radius_retry_imax;
    /** RADIUS retry trickle count; default 3 */
    uint8_t radius_retry_count;
} ws_br_radius_timing_t;

/** Wi-SUN Border Router class
 *
 * Class can be used to start, stop and configure Wi-SUN Border Router.
 */
class WisunBorderRouter {
public:

    /** Create WisunBorderRouter
     *
     * */
    WisunBorderRouter();

    /**
     * \brief Start Wi-SUN Border Router
     *
     * Starts Wi-SUN Border Router and routing between the mesh and backbone interfaces. Network interfaces
     * must be initialized and connected before calling the start. Backbone interface can be either Ethernet
     * (EMAC) or Cellular.
     *
     * \param mesh_if Wi-SUN mesh network interface
     * \param backbone_if Backbone network interface
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t start(WisunInterface *mesh_if, NetworkInterface *backbone_if);

    /**
     * \brief Start Wi-SUN Border Router
     *
     * Starts Wi-SUN Border Router and routing between the mesh and backbone interfaces. Network interfaces
     * must be initialized and connected before calling the start. Backbone interface can be either Ethernet
     * (EMAC) or Cellular.
     *
     * \param mesh_if Wi-SUN mesh network interface
     * \param backbone_if Backbone network interface
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    MBED_DEPRECATED_SINCE("mbed-os-6.10.0", "Using NetworkInterface type for mesh_if is deprecated, use WisunInterface instead")
    mesh_error_t start(NetworkInterface *mesh_if, NetworkInterface *backbone_if);

    /**
     * \brief Start Wi-SUN Border Router
     *
     * Starts Wi-SUN Border Router and routing between the mesh and backbone interfaces. Mesh network interface
     * must be initialized and connected before calling the start. Backbone OnboardNetworkStack::Interface must
     * be brought up before calling the start. Backbone interface can be either Ethernet (EMAC) or Cellular (PPP).
     *
     * \param mesh_if Wi-SUN mesh network interface
     * \param backbone_if Backbone OnboardNetworkStack::Interface interface
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t start(WisunInterface *mesh_if, OnboardNetworkStack::Interface *backbone_if);

    /**
     * \brief Start Wi-SUN Border Router
     *
     * Starts Wi-SUN Border Router and routing between the mesh and backbone interfaces. Mesh network interface
     * must be initialized and connected before calling the start. Backbone OnboardNetworkStack::Interface must
     * be brought up before calling the start. Backbone interface can be either Ethernet (EMAC) or Cellular (PPP).
     *
     * \param mesh_if Wi-SUN mesh network interface
     * \param backbone_if Backbone OnboardNetworkStack::Interface interface
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    MBED_DEPRECATED_SINCE("mbed-os-6.10.0", "Using NetworkInterface type for mesh_if is deprecated, use WisunInterface instead")
    mesh_error_t start(NetworkInterface *mesh_if, OnboardNetworkStack::Interface *backbone_if);

    /**
     * \brief Stop Wi-SUN Border Router
     *
     * Stops Wi-SUN Border Router.
     *
     * */
    void stop();

    /**
     * \brief Set Wi-SUN RPL DIO trickle parameters.
     *
     * Function stores new parameters to Border Router and uses them when mesh interface connect() is called
     * next time. If device is already connected to the Wi-SUN network then device will restart Wi-SUN network after
     * changing the RPL DIO trickle parameters. Mesh interface must be initialized before calling this
     * function.
     *
     * \param dio_interval_min DIO trickle timer Imin parameter. Use 0x00 to use leave parameter unchanged.
     * \param dio_interval_doublings DIO trickle timer Imax parameter as doublings of Imin. Use 0x00 to use leave parameter unchanged.
     * \param dio_redundancy_constant DIO trickle timer redundancy constant. Use 0xff to use leave parameter unchanged.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_rpl_parameters(uint8_t dio_interval_min, uint8_t dio_interval_doublings, uint8_t dio_redundancy_constant);

    /**
     * \brief Get Wi-SUN RPL DIO trickle parameters.
     *
     * Function reads DIO trickle timer Imin, DIO trickle timer Imax and DIO trickle timer redundancy
     * constant from Border Router. Mesh interface must be initialized before calling this function.
     *
     * \param dio_interval_min DIO trickle timer Imin parameter.
     * \param dio_interval_doublings DIO trickle timer Imax parameter as doublings of Imin.
     * \param dio_redundancy_constant DIO trickle timer redundancy constant.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_rpl_parameters(uint8_t *dio_interval_min, uint8_t *dio_interval_doublings, uint8_t *dio_redundancy_constant);

    /**
     * \brief Validate Wi-SUN RPL DIO trickle parameters.
     *
     * Function validates DIO trickle timer Imin, DIO trickle timer Imax and DIO trickle timer redundancy
     * constant. Function can be used to test that values that will be used on set function are valid.
     * Mesh interface must be initialized before the calling this function.
     *
     * \param dio_interval_min DIO trickle timer Imin parameter.
     * \param dio_interval_doublings DIO trickle timer Imax parameter as doublings of Imin.
     * \param dio_redundancy_constant DIO trickle timer redundancy constant.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_rpl_parameters(uint8_t dio_interval_min, uint8_t dio_interval_doublings, uint8_t dio_redundancy_constant);

    /**
     * \brief Set Wi-SUN PAN configuration parameters.
     *
     * Function stores new parameters to Border Router and uses them when mesh interface connect() is called
     * next time. If device is already connected to the Wi-SUN network then device will restart Wi-SUN network after
     * changing the PAN configuration parameters. Mesh interface must be initialized before calling this
     * function.
     *
     * \param pan_id PAN ID. 0xffff will generate the PAN ID on the mesh interface connect() call if not already generated.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_pan_configuration(uint16_t pan_id);

    /**
     * \brief Get Wi-SUN PAN configuration parameters.
     *
     * Function reads PAN ID from Border Router. Mesh interface must be initialized before calling this function.
     *
     * \param pan_id PAN ID.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t get_pan_configuration(uint16_t *pan_id);

    /**
     * \brief Validate Wi-SUN PAN configuration parameters.
     *
     * Function validates PAN ID. Function can be used to test that values that will be used on set function are valid.
     * Mesh interface must be initialized before calling this function.
     *
     * \param pan_id PAN ID.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t validate_pan_configuration(uint16_t pan_id);

    /**
     * \brief Get Wi-SUN Border Router information.
     *
     * Function reads RPL information from Border Router.
     * Mesh interface must be initialized before calling this function.
     *
     * \param info_ptr Structure given to stack where information will be stored
     *
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t info_get(ws_br_info_t *info_ptr);

    /**
     * \brief Get Wi-SUN neighbor table information.
     *
     * Function reads Routing Table information from Border Router.
     * Table is Parent child relation using the Global address IID of the devices.
     * To get the full IPv6 address of the device, IPv6 =  Global Prefix + IID.
     * Mesh interface must be initialized before calling this function.
     *
     * \param table_ptr Application allocated memory block where routing table is written.
     * \param table_len Length of the table allocated by application given as amount of entries.
     *
     * \returns 0 - x on success indicates number of entries written to the table_ptr.
     * \return <0 in case of errors.
     * */
    int routing_table_get(ws_br_route_info_t *table_ptr, uint16_t table_len);

    /**
     * \brief Set Wi-SUN RADIUS server IPv6 address.
     *
     * Function sets external RADIUS server IPv6 address to Border Router. Setting the address enables
     * external RADIUS server interface on Border Router. To disable external RADIUS server interface,
     * call the function with address set to NULL. The RADIUS shared secret must be set before address
     * is set using set_radius_shared_secret() call.
     *
     * \param address Pointer to IPv6 address string or NULL to disable RADIUS. Address string format is e.g. 2001:1234::1 and it is NUL terminated.
     * \return MESH_ERROR_NONE on success.
     * \return MESH_ERROR_UNKNOWN in case of failure.
     * */
    mesh_error_t set_radius_server_ipv6_address(const char *address);

    /**
     * \brief Get Wi-SUN RADIUS server IPv6 address.
     *
     * Function gets external RADIUS server IPv6 address from Border Router.
     *
     * \param address Pointer to buffer where to write IPv6 address string. Must have space at least for 39 characters and NUL terminator.
     * \return MESH_ERROR_NONE on success.
     * \return error value in case of failure, e.g. if address has not been set to Border Router.
     * */
    mesh_error_t get_radius_server_ipv6_address(char *address);

    /**
     * \brief Set Wi-SUN RADIUS shared secret.
     *
     * Function sets RADIUS shared secret to Border Router. Shared secret may be an ASCII string. Check
     * the format and length constraints for the shared secret from the documentation of RADIUS server you
     * are connecting to.
     *
     * \param shared_secret_len The length of the shared secret in bytes.
     * \param shared_secret Pointer to shared secret. Can be 8-bit ASCII string or byte array. Is not NUL terminated.
     * \return MESH_ERROR_NONE on success.
     * \return error value in case of failure.
     * */
    mesh_error_t set_radius_shared_secret(uint16_t shared_secret_len, const uint8_t *shared_secret);

    /**
     * \brief Get Wi-SUN RADIUS shared secret.
     *
     * Function gets RADIUS shared secret from Border Router.
     *
     * \param shared_secret_len On function call, is the size of the shared secret write buffer in bytes, on return is the shared secret length in bytes.
     * \param shared_secret Pointer to buffer where to write shared secret or NULL. At maximum, bytes set by the length parameter are written. If NULL only buffer length is returned.
     * \return MESH_ERROR_NONE on success.
     * \return error value in case of failure.
     * */
    mesh_error_t get_radius_shared_secret(uint16_t *shared_secret_len, uint8_t *shared_secret);

    /**
     * \brief Set Wi-SUN RADIUS timing parameters.
     *
     * Function sets RADIUS timing parameters to Border Router. For RADIUS retry trickle timer default
     * settings are that the first retry is done between 1 to 3 seconds after the initial attempt and
     * all retries are done in maximum in 9 seconds.
     *
     * \param timing Timing parameters.
     * \return MESH_ERROR_NONE on success.
     * \return error value in case of failure.
     * */
    mesh_error_t set_radius_timing(ws_br_radius_timing_t *timing);

    /**
     * \brief Get Wi-SUN RADIUS timing parameters.
     *
     * Function gets RADIUS timing parameters from Border Router.
     *
     * \param timing Timing parameters.
     * \return MESH_ERROR_NONE on success.
     * \return error value in case of failure.
     * */
    mesh_error_t get_radius_timing(ws_br_radius_timing_t *timing);

    /**
     * \brief Validate Wi-SUN RADIUS timing parameters.
     *
     * Function validates RADIUS timing parameters on Border Router.
     *
     * \param timing Timing parameters.
     * \return MESH_ERROR_NONE on success.
     * \return error value in case of failure.
     * */
    mesh_error_t validate_radius_timing(ws_br_radius_timing_t *timing);

    /**
     * \brief Set DNS query result to Nanostack cache.
     *
     * Function sets DNS query result to Nanostack cache to get distributed to the devices in the Wi-SUN network.
     * Function must be called for a running Wi-SUN Border Router instance.
     *
     * \param address resolved address of domain_name.
     * \param domain_name name of the domain. Must be non-NULL.
     * \return MESH_ERROR_NONE on success.
     * \return error value in case of failure.
     * */
    mesh_error_t set_dns_query_result(SocketAddress *address, char *domain_name);

private:
    mesh_error_t configure();
    mesh_error_t apply_configuration(int8_t mesh_if_id);
    mesh_error_t set_bbr_radius_address(void);
    mesh_error_t set_bbr_radius_shared_secret(void);
    mesh_error_t set_bbr_radius_timing(void);
    char _radius_ipv6_addr[40];
    ws_br_radius_timing_t _radius_timing = {
        .radius_retry_imin = 0,
        .radius_retry_imax = 0,
        .radius_retry_count = 0
    };
    char *_shared_secret = NULL;
    uint16_t _shared_secret_len = 0;
    int8_t _mesh_if_id = -1;
    bool _radius_ipv6_addr_set = false;
    bool _configured = false;
    bool _radius_timing_set = false;
};

#endif
