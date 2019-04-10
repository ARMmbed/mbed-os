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
#include "hci_api.h"
#include "util/bstream.h"
#include "CyMulti_adv.h"
#include "CyH4TransportDriver.h"
#include "CyVscCmd.h"

#define HCIC_PARAM_SIZE_ENABLE_MULTI_ADV                     3
#define HCIC_PARAM_SIZE_SET_MULTI_ADV_PARAM                  24
#define HCIC_PARAM_SIZE_SET_MULTI_ADV_MIN_SIZE               3
#define HCIC_PARAM_SIZE_SET_MULTI_ADV_MAX_SIZE               34
#define HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA                   31
#define HCI_CY_MULTI_ADV                                     0x0154

#define ARRAY_TO_BSTREAM(p, a, len) {int ijk; for (ijk = 0; ijk < len;        ijk++) *(p)++ = (uint8_t) a[ijk];}
#define INT8_TO_BSTREAM(p, n)    {*(p)++ = (int8_t)(n);}

namespace ble {
namespace vendor {
namespace cypress {

static CY_multi_adv_cb_t *multi_adv_cb;

void cy_multi_adv_internal_cb(uint16_t opcode, uint8_t status, uint16_t data_len, uint8_t *data)
{
    if ( (opcode == HCI_CY_MULTI_ADV) && (multi_adv_cb != NULL))
    {
        (*multi_adv_cb)((HciMultiAdvSubOpcode)*data, (uint8_t)status);
    }
}
void CyMultiAdv::cy_register_multi_adv_cmd_complete_cb(CY_multi_adv_cb_t *cb)
{
    multi_adv_cb = cb;
}

void CyMultiAdv::cy_start_multi_advertisements(uint8_t advertising_enable, uint8_t adv_instance)
{
    uint8_t param[HCIC_PARAM_SIZE_ENABLE_MULTI_ADV] = {0};
    uint8_t *pp = param;

    UINT8_TO_BSTREAM(pp, SET_ADVT_ENABLE);
    UINT8_TO_BSTREAM(pp, advertising_enable);
    UINT8_TO_BSTREAM(pp, adv_instance);

    cy_send_vendor_specific_cmd(HCI_CY_MULTI_ADV, HCIC_PARAM_SIZE_SET_MULTI_ADV_PARAM, param, &cy_multi_adv_internal_cb);
}


ble_error_t multi_adv_set_data( uint8_t * p_data, uint8_t data_len, uint8_t adv_instance, uint8_t scan_rsp_or_adv_opcode )
{
    uint8_t param[HCIC_PARAM_SIZE_SET_MULTI_ADV_MAX_SIZE] = {0};
    uint8_t *pp = param;
    uint8_t total_size = HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA + HCIC_PARAM_SIZE_SET_MULTI_ADV_MIN_SIZE;

    if (data_len > HCI_ADV_DATA_LEN || (p_data == NULL && data_len > 0))
    {
        return BLE_ERROR_INVALID_PARAM;
    }

    UINT8_TO_BSTREAM  (pp, scan_rsp_or_adv_opcode);

    if (p_data != NULL && data_len > 0)
    {
        UINT8_TO_BSTREAM (pp, data_len);

        ARRAY_TO_BSTREAM (pp, p_data, data_len);
    }
    pp += (HCIC_PARAM_SIZE_BLE_WRITE_ADV_DATA - data_len);

    UINT8_TO_BSTREAM  (pp, adv_instance);

    cy_send_vendor_specific_cmd(HCI_CY_MULTI_ADV, total_size, param, &cy_multi_adv_internal_cb);

    return BLE_ERROR_NONE;
}

ble_error_t CyMultiAdv::cy_set_multi_advertisement_scan_response_data( uint8_t * p_data, uint8_t data_len, uint8_t adv_instance )
{
    return multi_adv_set_data( p_data, data_len, adv_instance, SET_SCAN_RESP_DATA);
}

ble_error_t CyMultiAdv::cy_set_multi_advertisement_data( uint8_t * p_data, uint8_t data_len, uint8_t adv_instance )
{
    return multi_adv_set_data( p_data, data_len, adv_instance, SET_ADVT_DATA);
}

ble_error_t CyMultiAdv::cy_set_multi_advertisement_params( uint8_t adv_instance, const CyBleMultiAdvParam p_param )
{
    uint8_t param[HCIC_PARAM_SIZE_SET_MULTI_ADV_PARAM] = {0};
    uint8_t *pp = param;

    // Validate all parameters
    if ( (p_param._adv_int_min.value() < HCI_ADV_MIN_INTERVAL) || (p_param._adv_int_min.value() > HCI_ADV_MAX_INTERVAL) ||
         (p_param._adv_int_max.value() < HCI_ADV_MIN_INTERVAL) || (p_param._adv_int_max.value() > HCI_ADV_MAX_INTERVAL) ||
         (p_param._adv_tx_power < CY_BLE_MULTI_ADV_TX_POWER_MIN) || (p_param._adv_tx_power > CY_BLE_MULTI_ADV_TX_POWER_MAX) ||
         (adv_instance < 1) || (adv_instance > CY_LE_MULTI_ADV_MAX_NUM_INSTANCES) ||
         (p_param._adv_type.value() > ble::advertising_type_t::CONNECTABLE_DIRECTED_LOW_DUTY) ||
         (p_param._channel_map.value() == 0) || (p_param._channel_map.value() > ble::pal::advertising_channel_map_t::ALL_ADVERTISING_CHANNELS) ||
         (p_param._adv_filter_policy > ble::advertising_filter_policy_t::FILTER_SCAN_AND_CONNECTION_REQUESTS) ) {
        return BLE_ERROR_INVALID_PARAM;
    }

    UINT8_TO_BSTREAM  (pp, SET_ADVT_PARAM);
    UINT16_TO_BSTREAM (pp, p_param._adv_int_min.value());
    UINT16_TO_BSTREAM (pp, p_param._adv_int_max.value());
    UINT8_TO_BSTREAM  (pp, (uint8_t)p_param._adv_type.value());
    UINT8_TO_BSTREAM  (pp, (uint8_t)p_param._own_addr_type.value());
    BDA_TO_BSTREAM    (pp, (const uint8_t*)p_param._own_bd_addr.data());
    UINT8_TO_BSTREAM  (pp, (uint8_t)p_param._peer_addr_type.value());
    BDA_TO_BSTREAM    (pp, (const uint8_t*)p_param._peer_bd_addr.data());
    UINT8_TO_BSTREAM  (pp, p_param._channel_map.value());
    UINT8_TO_BSTREAM  (pp, p_param._adv_filter_policy.value());
    UINT8_TO_BSTREAM  (pp, adv_instance);
    INT8_TO_BSTREAM   (pp, p_param._adv_tx_power);

    cy_send_vendor_specific_cmd(HCI_CY_MULTI_ADV, HCIC_PARAM_SIZE_SET_MULTI_ADV_PARAM, param, &cy_multi_adv_internal_cb);

    return BLE_ERROR_NONE;
}

} // namespace cypress
} // namespace vendor
} // namespace ble