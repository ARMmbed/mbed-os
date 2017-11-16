/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_virtual_rf_driver.h"
#include <string.h>
#include "ns_virtual_rf_api.h"
#include "platform/arm_hal_phy.h"
#include "rf_driver_storage_stub.h"

int8_t receive(const uint8_t *data_ptr, uint16_t data_len, uint8_t link_quality, int8_t dbm, int8_t driver_id)
{
    return 0;
}

int8_t tx_done(int8_t driver_id, uint8_t tx_handle, phy_link_tx_status_e status, uint8_t cca_retry, uint8_t tx_retry)
{
    return 0;
}

int8_t tx_cb(const virtual_data_req_t *data_req, int8_t driver_id)
{
    return 0;
}

bool phy_rf_mlme_orserver_tx()
{
    rf_driver_storage_stub.list_value = NULL;
    if ( rf_driver_storage_stub.registered_driver ) {
        uint8_t temp_buf[16];
        mlme_set_t set_req;
        set_req.attr = 0;
        set_req.value_pointer = temp_buf;
        set_req.value_size = 16;
        rf_driver_storage_stub.mlme_observer(&set_req);

        set_req.attr = macBeaconPayload;
        rf_driver_storage_stub.mlme_observer(&set_req);
    }
    return true;
}

bool test_phy_rf_extension()
{
    rf_driver_storage_stub.list_value = NULL;
    if ( rf_driver_storage_stub.registered_driver ) {
        uint8_t data[8];
        if ( 0 != rf_driver_storage_stub.registered_driver->extension(PHY_EXTENSION_CTRL_PENDING_BIT, &data) ) {
            return false;
        }

        if ( 0 != rf_driver_storage_stub.registered_driver->extension(PHY_EXTENSION_READ_LAST_ACK_PENDING_STATUS, &data) ) {
            return false;
        }

        if ( 0 != rf_driver_storage_stub.registered_driver->extension(99, &data) ) {
            return false;
        }
    }
    return true;
}

bool test_phy_rf_address_write()
{
    rf_driver_storage_stub.list_value = NULL;
    if( rf_driver_storage_stub.registered_driver ){
        uint8_t addr[8];
        if( 0 != rf_driver_storage_stub.registered_driver->address_write(PHY_MAC_48BIT, &addr) ){
            return false;
        }

        if( 0 != rf_driver_storage_stub.registered_driver->address_write(PHY_MAC_64BIT, &addr) ){
            return false;
        }
    }
    return true;
}

bool test_phy_rf_tx()
{
    rf_driver_storage_stub.list_value = NULL;
    if( rf_driver_storage_stub.registered_driver ){
        rf_driver_storage_stub.registered_driver->arm_net_virtual_tx_cb = NULL;
        if( -1 != rf_driver_storage_stub.registered_driver->tx(NULL, 0, 0, 0) ){
            return false;
        }

        uint8_t data[3];
        if( -1 != rf_driver_storage_stub.registered_driver->tx(&data, 2, 0, 0) ){
            return false;
        }

        rf_driver_storage_stub.registered_driver->arm_net_virtual_tx_cb = &tx_cb;
        if( 0 != rf_driver_storage_stub.registered_driver->tx(&data, 2, 0, 0) ){
            return false;
        }
    }
    return true;
}

bool test_phy_rf_state_control()
{
    rf_driver_storage_stub.list_value = NULL;
    if( rf_driver_storage_stub.registered_driver ){
        if( 0 != rf_driver_storage_stub.registered_driver->state_control( PHY_INTERFACE_RESET, 0) ){
            return false;
        }

        if( 0 != rf_driver_storage_stub.registered_driver->state_control( PHY_INTERFACE_UP, 0) ){
            return false;
        }

        if( 0 != rf_driver_storage_stub.registered_driver->state_control( PHY_INTERFACE_RX_ENERGY_STATE, 0) ){
            return false;
        }
    }
    return true;
}

bool test_phy_rf_virtual_rx()
{
    rf_driver_storage_stub.list_value = NULL;
    if( rf_driver_storage_stub.registered_driver ){
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(NULL, 0, 2) ){
            return false;
        }
        uint8_t buf[6];
        buf[0] = 0;
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 1, 0) ){
            return false;
        }
        arm_device_driver_list_s list;
        rf_driver_storage_stub.list_value = &list;


        rf_driver_storage_stub.registered_driver->phy_rx_cb = &receive;
        rf_driver_storage_stub.registered_driver->phy_tx_done_cb = &tx_done;
        buf[0] = 1;
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 3, 0) ){
            return false;
        }

        if( 0 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 4, 0) ){
            return false;
        }

        buf[0] = 2;

        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 2, 0) ){
            return false;
        }
        if( 0 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 4, 0) ){
            return false;
        }
        buf[0] = 0;
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 4, 0) ){
            return false;
        }
    }
    return true;
}

bool test_virtual_rf_device_register()
{
    rf_driver_storage_stub.int8_value = -1;
    if( -1 != virtual_rf_device_register(PHY_LINK_15_4_SUBGHZ_TYPE, 0) ){
        return false;
    }

    if( -1 != virtual_rf_device_register(PHY_LINK_15_4_2_4GHZ_TYPE, 0) ){
        return false;
    }

    rf_driver_storage_stub.int8_value = 0;
    if( 0 != virtual_rf_device_register(PHY_LINK_TUN, 0) ){
        return false;
    }
    return true;
}

