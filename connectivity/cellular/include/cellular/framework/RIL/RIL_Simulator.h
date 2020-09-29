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

#ifndef RIL_SIMULATOR_H_
#define RIL_SIMULATOR_H_

#ifdef RIL_SIMULATOR

#include "Thread.h"
#include "Mutex.h"
#include "nsapi_types.h"
#include "EventQueue.h"
#include "ril.h"

#include <queue>

const int32_t MAX_SIM_PDP_CONTEXTS = 4;

namespace mbed {

class RIL_Simulator {
public:
    ~RIL_Simulator();

    static RIL_Simulator *GetInstance();
    bool Init(const struct RIL_Env *env);

    RIL_RadioState handle_onStateRequest();

    void enqueue_onRequest(int request, void *data, size_t datalen, RIL_Token t);

private:
    RIL_Simulator();

    void handle_onRequest();
    void enqueue_OnUnsolicitedResponse(int unsolResponse, const void *data, size_t datalen);
    void send_OnUnsolicitedResponse();

    void send_radio_ready_ind();
    void send_radio_state_changed_ind(RIL_RadioState state);
    void send_sim_status_changed_ind();
    void send_network_state_changed_ind(int reg_state);
    void send_data_call_list_changed_ind();

    void handle_RIL_REQUEST_GET_SIM_STATUS(RIL_Token t);
    void handle_RIL_REQUEST_DATA_REGISTRATION_STATE(RIL_Token t);
    void handle_RIL_REQUEST_RADIO_POWER(RIL_Token t, void *data, size_t datalen);
    void handle_RIL_REQUEST_DATA_CALL_LIST(RIL_Token t);
    void handle_RIL_REQUEST_SETUP_DATA_CALL(RIL_Token t, void *data, size_t datalen);
    void handle_RIL_REQUEST_DEACTIVATE_DATA_CALL(RIL_Token t, void *data, size_t datalen);
    void handle_RIL_REQUEST_SET_INITIAL_ATTACH_APN(RIL_Token t, void *data, size_t datalen);

    RIL_Data_Call_Response_v11 *add_pdp_context(const char *pdp_type);
    void delete_pdp_context(int cid);

private:
    rtos::Thread            _queue_thread;
    events::EventQueue      _queue;
    struct RIL_Env          _callback;

    RIL_RadioState          _overallRadioState;
    bool                    _sim_present;
    int                     _reg_state; // registration state 0-5 from TS 27.007 10.1.20 AT+CGREG

    struct ril_request {
        int         request;
        void        *data;
        size_t      datalen;
        RIL_Token   t;
    };

    struct ril_unsolresponse {
        int         unsolResponse;
        const void  *data;
        size_t      datalen;
    };

    rtos::Mutex                     _ril_request_lock; // _ril_requests must be protected as those it can be accessed from separate thread (application)
    std::queue<ril_request>         _ril_requests;
    std::queue<ril_unsolresponse>   _ril_unsolresponses;

    RIL_Data_Call_Response_v11      _pdp_contexts[MAX_SIM_PDP_CONTEXTS];
    RIL_Data_Call_Response_v11      _pdp_contexts_rearranged[MAX_SIM_PDP_CONTEXTS]; // Rearranged list
};

} /* namespace mbed */


#endif // RIL_SIMULATOR
#endif // RIL_SIMULATOR_H_
