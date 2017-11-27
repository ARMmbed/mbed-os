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
#include "nsconfig.h"
#include "test_mac_filter.h"
#include <string.h>
#include <inttypes.h>
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/rf_driver_storage.h"

#include "nsdynmemLIB_stub.h"
#include "common_functions_stub.h"

int_fast8_t long_value = -1;
int_fast8_t short_value = -1;

int_fast8_t long_cb(int8_t interface_id, uint8_t mac64[8], uint16_t mac16)
{
    memset(&mac64, 0, 8);
    return long_value;
}

int_fast8_t short_cb(int8_t interface_id, uint8_t mac64[8], uint16_t *mac16)
{
    memset(&mac64, 0, 8);
    *mac16 = 0;
    return short_value;
}


bool test_mac_filter_modify_link_quality()
{
    int ret = mac_filter_modify_link_quality(0, NULL);
    if( 0 != ret ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_start(0, 2, -12, 3, -12);

    //Stop is tested last to keep filter enabled during the test

    mac_pre_parsed_frame_t frame;
    frame.fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
    ret = mac_filter_modify_link_quality(0, &frame);
    if( 0 != ret ){
        return false;
    }

    frame.fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    common_functions_stub.uint16_value = 1;
    mac_filter_set_address_mapping(0, &long_cb, &short_cb);
    long_value = 0;
    ret = mac_filter_modify_link_quality(0, &frame);
    if( 1 != ret ){
        return false;
    }
    long_value = -1;

    short_value = 0;
    frame.LQI = 2;
    frame.dbm = 33;
    frame.fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_64_BIT;

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_add_short(0, 0, 30000, 256, 256, 600);
    ret = mac_filter_modify_link_quality(0, &frame);
    if( 0 != ret ){
        return false;
    }

    mac_filter_clear(0);
    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_add_short(0, 0, 0, 2, 256, -66);
    frame.LQI = 254;
    frame.dbm = -94; //== MAC_FILTER_SIGNAL_FLOOR
    ret = mac_filter_modify_link_quality(0, &frame);
    if( 1 != ret ){
        return false;
    }

    //Test enabled == false
    mac_filter_stop(0);
    ret = mac_filter_modify_link_quality(0, &frame);
    if( 0 != ret ){
        return false;
    }

    mac_filter_clear(0);
    mac_filter_set_address_mapping(0, NULL, NULL);

    return true;
}

bool test_mac_filter_start()
{
    int8_t ret = mac_filter_start(0, 2, -12, 3, -12);
    if( -1 != ret ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_filter_start(0, 2, -12, 3, -12);
    if( 0 != ret ){
        return false;
    }

    mac_filter_stop(0);

    return true;
}

bool test_mac_filter_stop()
{
    mac_filter_stop(0);

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_start(0, 2, -12, 3, -12);

    mac_filter_stop(0); //fails if memory leak(s) found
    return true;
}

bool test_mac_filter_clear()
{
    int ret = mac_filter_clear(0);
    if( -1 != ret ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_start(0, 2, -12, 3, -12);

    uint8_t mac[8];
    memset(&mac, 0, 8);
    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_add_long(0, mac, 30000, 256, 256, 600);

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_add_short(0, mac, 30000, 256, 256, 600);

    ret = mac_filter_clear(0);
    if( 0 != ret ){
        return false;
    }
    mac_filter_stop(0);

    return true;
}

bool test_mac_filter_set_address_mapping()
{
    int ret = mac_filter_set_address_mapping(0, &long_cb, &short_cb);
    if( -1 != ret ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_filter_set_address_mapping(0, &long_cb, &short_cb);
    if( 0 != ret ){
        return false;
    }

    mac_filter_stop(0);
    ret = mac_filter_set_address_mapping(0, NULL, NULL);
    if( 0 != ret ){
        return false;
    }

    return true;
}

bool test_mac_filter_delete_short()
{
    int ret = mac_filter_delete_short(0, 0);
    if( -1 != ret ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_start(0, 2, -12, 3, -12);

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_add_short(0, 0, 30000, 256, 256, 600);
    ret = mac_filter_delete_short(0, 0);
    if( 0 != ret ){
        return false;
    }
    mac_filter_stop(0);

    return true;
}

bool test_mac_filter_delete_long()
{
    uint8_t mac[8];
    memset(&mac, 0, 8);
    int ret = mac_filter_delete_long(0, &mac);
    if( -1 != ret ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_start(0, 2, -12, 3, -12);

    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_add_long(0, mac, 30000, 256, 256, 600);
    ret = mac_filter_delete_long(0, &mac);
    if( 0 != ret ){
        return false;
    }
    mac_filter_stop(0);
    return true;
}

bool test_mac_filter_add_short()
{
    int ret = mac_filter_add_short(0, 0, 30000, 256, 256, 600);
    if( ret != -1 ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_start(0, 2, -12, 3, -12);
    ret = mac_filter_add_short(0, 0, 30000, 256, 256, 600);
    if( ret != -2 ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_filter_add_short(0, 0, 30000, 256, 256, 600);
    if( ret != 0 ){
        return false;
    }

    mac_filter_delete_short(0, 0);
    mac_filter_stop(0);
    return true;
}

bool test_mac_filter_add_long()
{
    uint8_t mac[8];
    int8_t ret = mac_filter_add_long(0, mac, 0,0,0,0);
    if( ret != -1 ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    mac_filter_start(0, 2, -12, 3, -12);
    ret = mac_filter_add_long(0, 0, 30000, 256, 256, 600);
    if( ret != -1 ){
        return false;
    }
    ret = mac_filter_add_long(0, mac, 30000, 256, 256, 600);
    if( ret != -2 ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    ret = mac_filter_add_long(0, mac, 30000, 256, 256, 600);
    if( ret != 0 ){
        return false;
    }

    mac_filter_delete_short(0, 0);
    mac_filter_stop(0);
    return true;
}
