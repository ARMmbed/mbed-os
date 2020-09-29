/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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
#include "SAMSUNG_S5JS100_RIL_CellularNetwork.h"
#include "CellularContext.h"
#include "CellularLog.h"

namespace mbed {

SAMSUNG_S5JS100_RIL_CellularNetwork::SAMSUNG_S5JS100_RIL_CellularNetwork(RIL_CellularDevice &device)
    : RIL_CellularNetwork(device), _edrx(-1)
{
}

SAMSUNG_S5JS100_RIL_CellularNetwork::~SAMSUNG_S5JS100_RIL_CellularNetwork()
{
}

nsapi_error_t SAMSUNG_S5JS100_RIL_CellularNetwork::set_access_technology_impl(RadioAccessTechnology rat)
{
    _rat = RAT_NB1;
    if (rat != RAT_NB1) {
        // only rat that is supported by this modem
        return NSAPI_ERROR_UNSUPPORTED;
    }

    return NSAPI_ERROR_OK;
}

nsapi_error_t SAMSUNG_S5JS100_RIL_CellularNetwork::set_receive_period(int mode, EDRXAccessTechnology act_type,
                                                                      uint8_t edrx_value)
{
#if defined(TARGET_S5JS100)
    if (mode > 1) {
        tr_warning("Invalid eDRX mode: %d", mode);
        return NSAPI_ERROR_UNSUPPORTED;
    }
    if (act_type != EDRXEUTRAN_NB_S1_mode) {
        return NSAPI_ERROR_UNSUPPORTED;
    }

    _api_mutex.lock();

    _int_data = mode;
    // eDRX enable/disable is set via RIL_REQUEST_PSM_ENABLED
    lock_and_send_request(RIL_REQUEST_PSM_ENABLED, &_int_data, 1, callback(this, &SAMSUNG_S5JS100_RIL_CellularNetwork::request_set_psm_edrx_response));


    // we disabled eDRX or there was an error, no need to set any values
    if (!_int_data || _error) {
        _cond_mutex.unlock();
        _api_mutex.unlock();
        return _error;
    }

    // try to set values only if PSM enable was successful. For now we use default value(0) as we don't
    // have PTW in API.
    if (_error == NSAPI_ERROR_OK) {
        unsigned int *data = new unsigned int[2];
        _edrx = edrx_value;
        data[0] = 0;
        data[1] = _edrx;
        _cond_mutex.unlock();
        ril_token_t *token = lock_and_send_request(RIL_REQUEST_SET_EDRX, (void *)data, sizeof(unsigned int) * 2, callback(this, &SAMSUNG_S5JS100_RIL_CellularNetwork::request_set_psm_edrx_response));
        if (token == NULL) {
            delete [] data;
        }
    }

    _cond_mutex.unlock();
    _api_mutex.unlock();
    return _error;
#else
    return NSAPI_ERROR_UNSUPPORTED;
#endif //#if defined(TARGET_S5JS100)
}

void SAMSUNG_S5JS100_RIL_CellularNetwork::request_set_psm_edrx_response(ril_token_t *token, RIL_Errno err, void *response,
                                                                        size_t response_len)
{
#if defined(TARGET_S5JS100)
    _cond_mutex.lock();
    if (err == RIL_E_SUCCESS) {
        _error = NSAPI_ERROR_OK;
    } else {
        _error = NSAPI_ERROR_DEVICE_ERROR;
    }

    if (token->request_id == RIL_REQUEST_SET_EDRX) {
        unsigned int *req_data = (unsigned int *)token->data;
        delete [] req_data;
    }

    if (err != RIL_E_CANCELLED) {
        _cond_var.notify_one();
    }
    _cond_mutex.unlock();
#endif // #if defined(TARGET_S5JS100)
}

} /* namespace mbed */
