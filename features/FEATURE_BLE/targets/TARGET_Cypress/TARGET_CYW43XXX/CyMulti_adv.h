/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
 *
 */
#ifndef CY_MULTI_ADV_H_
#define CY_MULTI_ADV_H_
#include <stdio.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/pal/GapTypes.h"

#define CY_BLE_MULTI_ADV_TX_POWER_MIN                     -21
#define CY_BLE_MULTI_ADV_TX_POWER_MAX                      9
#define CY_LE_MULTI_ADV_MAX_NUM_INSTANCES                  16

#define DEFAULT_ADVERTISING_INTERVAL_MIN 0x400
#define DEFAULT_ADVERTISING_INTERVAL_MAX 0x800

namespace ble {
namespace vendor {
namespace cypress {

typedef enum
{
    SET_ADVT_PARAM      = 0x01,
    SET_ADVT_DATA       = 0x02,
    SET_SCAN_RESP_DATA  = 0x03,
    SET_RANDOM_ADDR     = 0x04,
    SET_ADVT_ENABLE     = 0x05
}HciMultiAdvSubOpcode;

/* Multi adv command complete callback */
typedef void (CY_multi_adv_cb_t)(HciMultiAdvSubOpcode sub_opcode, uint8_t status);

/* LE Multi advertising parameter class */
class CyBleMultiAdvParam
{
public:
    /* Constructure */
    CyBleMultiAdvParam(ble::advertising_type_t advType = ble::advertising_type_t::NON_CONNECTABLE_UNDIRECTED,
        ble::adv_interval_t minInterval = ble::adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MIN),
        ble::adv_interval_t maxInterval = ble::adv_interval_t(DEFAULT_ADVERTISING_INTERVAL_MAX)) :
            _adv_int_min(minInterval),
            _adv_int_max(maxInterval),
            _adv_type(advType),
            _channel_map(ble::pal::advertising_channel_map_t::ALL_ADVERTISING_CHANNELS),
            _adv_filter_policy(ble::advertising_filter_policy_t::NO_FILTER),
            _adv_tx_power(9),
            _peer_bd_addr(),
            _peer_addr_type(ble::peer_address_type_t::PUBLIC),
            _own_bd_addr(),
            _own_addr_type(ble::peer_address_type_t::PUBLIC)
        {
        };

    // Minimum adv interval
    ble::adv_interval_t                  _adv_int_min; 
    // Maximum adv interval
    ble::adv_interval_t                  _adv_int_max; 
    // Adv type
    ble::advertising_type_t              _adv_type;
    // Adv channel map
    ble::pal::advertising_channel_map_t  _channel_map;
    // Advertising filter policy
    ble::advertising_filter_policy_t     _adv_filter_policy;
    // Adv tx power (Max : CY_BLE_MULTI_ADV_TX_POWER_MAX, Min :CY_BLE_MULTI_ADV_TX_POWER_MIN)
    ble::advertising_power_t             _adv_tx_power;
    // Peer Device address 
    ble::address_t                       _peer_bd_addr;
    // Peer LE Address type
    ble::peer_address_type_t             _peer_addr_type;
    // Own LE address
    ble::address_t                       _own_bd_addr;
    // Own LE Address type
    ble::peer_address_type_t             _own_addr_type;
};

class CyMultiAdv{
public:

    /* Constructure */
    CyMultiAdv() {};

    /**
     * Start/Stop Mulit advertisements.
     *
     * cy_start_multi_advertisements gives option to start multiple adverstisment instances
     * Each of the instances can set different #cy_set_multi_advertisement_params and #cy_set_multi_advertisement_data.
     * Hence this feature allows the device to advertise to multiple masters at the same time like a multiple peripheral device,
     * with different advertising data, Random private addresses, tx_power etc.
     *
     * @param advertising_enable 1 - Advertising is enabled, 0 : Advertising is disabled
     * @param adv_instance 1 to CY_LE_MULTI_ADV_MAX_NUM_INSTANCES
     *
     * @return void
     *
     * NOTE : At a time only one instance can advertise with DIRECTED ADV.
     */
    void cy_start_multi_advertisements ( uint8_t advertising_enable, uint8_t adv_instance );

    /**
     * Set multi advertisement data for each adv_instance
     *
     * @param p_data     Advertising Data ( Max length 31 bytess)
     * @param data_len   Advertising Data len ( Max 31 bytes )
     * @param adv_instance 1 to CY_LE_MULTI_ADV_MAX_NUM_INSTANCES
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     */
    ble_error_t cy_set_multi_advertisement_data( uint8_t * p_data, uint8_t data_len, uint8_t adv_instance );

    /**
     * Set multi advertisement data for scan response
     *
     * @param p_data     Advertising Data ( Max length 31 bytess)
     * @param data_len   Advertising Data len ( Max 31 bytes )
     * @param adv_instance 1 to CY_LE_MULTI_ADV_MAX_NUM_INSTANCES
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     */
    ble_error_t cy_set_multi_advertisement_scan_response_data( uint8_t * p_data, uint8_t data_len, uint8_t adv_instance );

    /**
     * Set multi advertisement parameters for each adv_instance
     *
     * All the parameters below refered to as "spec" is BT 4.2 core specification, page 1277 (LE Set Advertising Parameter Command)
     *
     * @param adv_instance 1 to CY_LE_MULTI_ADV_MAX_NUM_INSTANCES
     * @p_param object of the adv parameter class
     *
     * @return BLE_ERROR_NONE if the request has been successfully sent or the
     * appropriate error otherwise.
     *
     */
    ble_error_t cy_set_multi_advertisement_params( uint8_t adv_instance, const CyBleMultiAdvParam p_param );

    /**
     * Register callback for multi adv command status event
     *
     * @param vsc_cb callback pointer
     *
     * @return void
     *
     */
    void cy_register_multi_adv_cmd_complete_cb(CY_multi_adv_cb_t *vsc_cb);
};
} // namespace cypress
} // namespace vendor
} // namespace ble

#endif // CY_MULTI_ADV_H_

