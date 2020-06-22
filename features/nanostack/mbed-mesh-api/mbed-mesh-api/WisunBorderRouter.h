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

/** Wi-SUN Border Router class
 *
 * Class can be used to start, stop and configure Wi-SUN Border Router.
 */
class WisunBorderRouter {
public:

    /** Create WisunBorderRouter
     *
     * */
    WisunBorderRouter() { }

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

private:
    int8_t _mesh_if_id = -1;

};

#endif
