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
#include "SAMSUNG_S5JS100_RIL.h"
#include "SAMSUNG_S5JS100_RIL_CellularContext.h"
#include "SAMSUNG_S5JS100_RIL_CellularNetwork.h"
#include "SAMSUNG_S5JS100_RIL_CellularInformation.h"
#if defined(TARGET_S5JS100)
#include "SAMSUNG_S5JS100_RIL_IF.h"
#endif // #if defined(TARGET_S5JS100)
#include "CellularLog.h"
using namespace mbed;

typedef enum s5js100_cellular_event_status {
    CellularRILATResponse                   = NSAPI_EVENT_CELLULAR_STATUS_BASE + 11, /* Only available when using RIL layer. This is a (multi-)response to RIL command RIL_REQUEST_SET_FORWARDING_AT_COMMAND. Data field contain AT response (char*) and status_data contains length. */

} s5js100_cellular_connection_status_t;

static const intptr_t cellular_properties[RIL_CellularDevice::PROPERTY_MAX] = {
    1,  // C_EREG
    0,  // C_GREG
    0,  // C_REG
    1,  // PROPERTY_IPV4_STACK
    1,  // PROPERTY_IPV6_STACK
    1,  // PROPERTY_IPV4V6_STACK
    1,  // PROPERTY_NON_IP_PDP_TYPE
};

SAMSUNG_S5JS100_RIL::SAMSUNG_S5JS100_RIL() : RIL_CellularDevice(), _periodic_time(0), _active_time(0)
{
    _property_array = cellular_properties;
    // open information here so we can save unsolicited iccd response
    open_information();
}

SAMSUNG_S5JS100_RIL::~SAMSUNG_S5JS100_RIL()
{
#if defined(TARGET_S5JS100)
    SAMSUNG_S5JS100_RIL_IF::GetInstance()->DeInit();
#endif // #if defined(TARGET_S5JS100)
}

nsapi_error_t SAMSUNG_S5JS100_RIL::hard_power_off()
{
#if defined(TARGET_S5JS100)
    SAMSUNG_S5JS100_RIL_IF::GetInstance()->DeInit();
#endif //#if defined(TARGET_S5JS100)
    return NSAPI_ERROR_OK;
}

RIL_CellularContext *SAMSUNG_S5JS100_RIL::create_context_impl(const char *apn, bool cp_req, bool nonip_req)
{
    return new SAMSUNG_S5JS100_RIL_CellularContext(*this, apn, cp_req, nonip_req);
}

RIL_CellularNetwork *SAMSUNG_S5JS100_RIL::open_network_impl()
{
    return new SAMSUNG_S5JS100_RIL_CellularNetwork(*this);
}

RIL_CellularInformation *SAMSUNG_S5JS100_RIL::open_information_impl()
{
    return new SAMSUNG_S5JS100_RIL_CellularInformation(*this);
}

nsapi_error_t SAMSUNG_S5JS100_RIL::send_at_command(char *data, size_t data_len)
{
#if defined(TARGET_S5JS100)
    if (data == NULL || data_len <= 0) {
        return NSAPI_ERROR_PARAMETER;
    }
    _api_mutex.lock();
    lock_and_send_request(RIL_REQUEST_SET_FORWARDING_AT_COMMAND, data, data_len,
                          callback(this, &SAMSUNG_S5JS100_RIL::common_extension_response));

    _cond_mutex.unlock();
    _api_mutex.unlock();
    return _error;
#else
    return NSAPI_ERROR_UNSUPPORTED;
#endif //#if defined(TARGET_S5JS100
}

nsapi_error_t SAMSUNG_S5JS100_RIL::set_power_save_mode(int periodic_time, int active_time)
{
#if defined(TARGET_S5JS100)
    _api_mutex.lock();

    if (periodic_time == 0 && active_time == 0) {
        _data = 0;
    } else {
        _data = 1;
    }

    lock_and_send_request(RIL_REQUEST_PSM_ENABLED, &_data, 1, callback(this, &SAMSUNG_S5JS100_RIL::common_extension_response));

    // we disabled PSM, no need to set any values
    if (!_data) {
        _cond_mutex.unlock();
        _api_mutex.unlock();
        return _error;
    }

    // try to set values only if PSM enable was successful.
    if (_error == NSAPI_ERROR_OK) {
        unsigned int *data = new unsigned int[2];
        _periodic_time = periodic_time;
        _active_time = active_time;
        data[0] = _periodic_time;
        data[1] = _active_time;
        _cond_mutex.unlock();
        ril_token_t *token = lock_and_send_request(RIL_REQUEST_PSM_TIMER, (void *)data, sizeof(unsigned int) * 2,
                                                   callback(this, &SAMSUNG_S5JS100_RIL::common_extension_response));
        if (token == NULL) {
            delete [] data;
        }
    }

    _cond_mutex.unlock();
    _api_mutex.unlock();
    return _error;
#else
    return NSAPI_ERROR_UNSUPPORTED;
#endif //#if defined(TARGET_S5JS100
}

void SAMSUNG_S5JS100_RIL::modem_debug_on(bool on)
{
#if defined(TARGET_S5JS100)
    _api_mutex.lock();
	if(on == false)
		_data = 0;
	else
		_data = 1;


    lock_and_send_request(RIL_REQUEST_SET_DEBUG_TRACE, &_data, 1, callback(this, &SAMSUNG_S5JS100_RIL::common_extension_response));


    _cond_mutex.unlock();
    _api_mutex.unlock();
    return;
#else
    return;
#endif //#if defined(TARGET_S5JS100
}

void SAMSUNG_S5JS100_RIL::pms_timer_changed(const void *data, size_t data_len)
{
    const int *dt = (const int *)data;
    if (data && data_len > 0) {
        int status = dt[0];
        tr_debug("PSM timer was %s", status ? "accepted" : "rejected");
        if (status) {
            // accepted, check the values
            tr_debug("PSM T3412 timer: %d, T3324 timer: %d", dt[1], dt[2]);
        }
    }
    // TODO: send notification to application. need to specify new cellular_event_status
}

void SAMSUNG_S5JS100_RIL::pms_status_changed(const void *data, size_t data_len)
{
}

void SAMSUNG_S5JS100_RIL::update_localtime(const void *data, size_t data_len)
{
    struct tm l_time = {0};
    char *temp = NULL;
    const char *sep = "/,:+";
    int idx = 0;

    if (data)
    {
        temp = strtok((char *)data,sep);
        while(temp)
        {
            switch (idx) {
                case 0 :
                    l_time.tm_year = atoi(temp)+2000-1900;
                    break;
                case 1 :
                    l_time.tm_mon = atoi(temp)-1;
                    if (l_time.tm_mon >= 12 || l_time.tm_mon < 0)
                    {
                        return;
                    }
                    break;
                case 2 :
                    l_time.tm_mday = atoi(temp);
                    if (l_time.tm_mday > 31 || l_time.tm_mday < 1)
                    {
                        return;
                    }
                    break;
                case 3 :
                    l_time.tm_hour = atoi(temp);
                    if (l_time.tm_hour >= 24 || l_time.tm_hour < 0)
                    {
                        return;
                    }
                    break;
                case 4 :
                    l_time.tm_min = atoi(temp);
                    if (l_time.tm_min >= 60 || l_time.tm_min < 0)
                    {
                        return;
                    }
                    break;
                case 5 :
                    l_time.tm_sec = atoi(temp);
                    if (l_time.tm_sec >= 60 || l_time.tm_sec < 0)
                    {
                        return;
                    }
                    break;
                default :
                    break;
            }
            temp = strtok(NULL, sep);
            idx++;
        }
    }
    if (idx <= 7)
    {
        return;
    }
    set_time(mktime(&l_time));
}

void SAMSUNG_S5JS100_RIL::at_response(const void *data, size_t data_len)
{
    cell_callback_data_t dt;
    dt.data = data;
    dt.status_data = data_len;
    if (_status_cb) {
        _status_cb((nsapi_event_t)CellularRILATResponse, (intptr_t)&dt);
    }
}

void SAMSUNG_S5JS100_RIL::unsolicited_response(int response_id, const void *data, size_t data_len)
{
#if defined(TARGET_S5JS100)
    switch (response_id) {
        case RIL_UNSOL_PSM_TIMER_CHANGED:
            pms_timer_changed(data, data_len);
            break;
        case RIL_UNSOL_PSM_STATUS_CHANGED:
            pms_status_changed(data, data_len);
            break;
        case RIL_UNSOL_FORWARDING_AT_COMMAND_IND:
            at_response(data, data_len);
            break;
        case RIL_UNSOL_NITZ_TIME_RECEIVED:
            update_localtime(data, data_len);
            break;
        default:
            break;
    }
#endif // #if defined(TARGET_S5JS100)
    RIL_CellularDevice::unsolicited_response(response_id, data, data_len);
}

void SAMSUNG_S5JS100_RIL::common_extension_response(ril_token_t *token, RIL_Errno err, void *response, size_t response_len)
{
#if defined(TARGET_S5JS100)
    _cond_mutex.lock();
    if (err == RIL_E_SUCCESS) {
        _error = NSAPI_ERROR_OK;
    } else {
        _error = NSAPI_ERROR_DEVICE_ERROR;
    }

    if (token->request_id == RIL_REQUEST_PSM_TIMER) {
        unsigned int *req_data = (unsigned int *)token->data;
        delete [] req_data;
    }

    if (err != RIL_E_CANCELLED) {
        _cond_var.notify_one();
    }
    _cond_mutex.unlock();
#endif // #if defined(TARGET_S5JS100)
}
