/*
 * Copyright (c) 2016-2017, Arm Limited and affiliates.
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
#include "test_virtual_rf_client.h"
#include <string.h>
#include "ns_virtual_rf_api.h"
#include "MAC/virtual_rf/virtual_rf_defines.h"
#include "mlme.h"

#include "rf_driver_storage_stub.h"
bool rx_test = false;
uint8_t rx_status = 0;
int8_t tx_cb(const virtual_data_req_t *data_req, int8_t driver_id)
{

    if (rx_test) {
        uint8_t *ptr = data_req->parameters;
        if (*ptr++ != NAP_DATA_PHY_RAW_INDICATION) {
            return -1;
        }
    } else {
        uint8_t *ptr = data_req->msdu;
        if (data_req->msduLength != 4)
            return -1;

        if (*ptr++ != NAP_DATA_PHY_RAW_RESPONSE) {
            return -1;
        }
        rx_status = *ptr++;
    }


    return 0;
}

int8_t upper_cb(void *identifier, arm_phy_sap_msg_t *message)
{
    return 0;
}



bool test_phy_rf_virtual_rx()
{
    rf_driver_storage_stub.list_value = NULL;
    rf_driver_storage_stub.int8_value = 0;
    if( rf_driver_storage_stub.registered_driver ){
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(NULL, 0, 2) ){
            return false;
        }
        uint8_t buf[6];

        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 6, 0) ){
            return false;
        }
        arm_device_driver_list_s list;
        list.phy_sap_upper_cb = &upper_cb;
        rf_driver_storage_stub.list_value = &list;

        buf[0] = NAP_DATA_PHY_RAW_REQUEST;
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 5, 0) ){
            return false;
        }

        if( 0 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 6, 0) ){
            return false;
        }
        buf[0] = NAP_DATA_PHY_RAW_INDICATION;
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 5, 0) ){
            return false;
        }

        buf[0] = NAP_MLME_REQUEST;
        if( -1 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 2, 0) ){
            return false;
        }

        if( 0 != rf_driver_storage_stub.registered_driver->arm_net_virtual_rx_cb(&buf, 3, 0) ){
            return false;
        }

    }
    return true;
}

bool test_phy_rf_tx_done()
{
    rf_driver_storage_stub.list_value = NULL;
    rf_driver_storage_stub.int8_value = 0;
    if( rf_driver_storage_stub.registered_driver ){
        rf_driver_storage_stub.registered_driver->arm_net_virtual_tx_cb = NULL;
        if( -1 != rf_driver_storage_stub.registered_driver->phy_tx_done_cb(0,0,0,0,0) ){
            return false;
        }
        rf_driver_storage_stub.registered_driver->arm_net_virtual_tx_cb = &tx_cb;
        rx_test = false;
        rx_status = 0xff;
        if( 0 != rf_driver_storage_stub.registered_driver->phy_tx_done_cb(0,0,MLME_SUCCESS,0,0) ){
            return false;
        }

        if (rx_status != PHY_LINK_TX_DONE) {
            return false;
        }
        rx_status = 0xff;
        if( 0 != rf_driver_storage_stub.registered_driver->phy_tx_done_cb(0,0,MLME_BUSY_CHAN,0,0) ){
            return false;
        }

        if (rx_status != PHY_LINK_CCA_FAIL) {
            return false;
        }

        if( 0 != rf_driver_storage_stub.registered_driver->phy_tx_done_cb(0,0,MLME_TX_NO_ACK,0,0) ){
            return false;
        }

        if (rx_status != PHY_LINK_TX_FAIL) {
            return false;
        }

    }
    return true;
}

bool test_phy_rf_rx()
{
    rf_driver_storage_stub.list_value = NULL;
    rf_driver_storage_stub.int8_value = 0;
    if( rf_driver_storage_stub.registered_driver ){
        rf_driver_storage_stub.registered_driver->arm_net_virtual_tx_cb = NULL;
        if( -1 != rf_driver_storage_stub.registered_driver->phy_rx_cb(NULL, 0, 0, 0, 0) ){
            return false;
        }
        rf_driver_storage_stub.registered_driver->arm_net_virtual_tx_cb = &tx_cb;
        rx_test = true;
        uint8_t buf_temp[5];
        if( 0 != rf_driver_storage_stub.registered_driver->phy_rx_cb(buf_temp, 5, 0, 0, 0) ){
            return false;
        }
    }
    return true;
}

bool test_virtual_rf_client_register()
{
    rf_driver_storage_stub.list_value = NULL;
    rf_driver_storage_stub.int8_value = 0;
    if( 0 != virtual_rf_client_register() ){
        return false;
    }
    return true;
}
