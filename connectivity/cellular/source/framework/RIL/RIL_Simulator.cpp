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
#include "RIL_Simulator.h"

#ifdef RIL_SIMULATOR

#include "mbed_trace.h"
#define TRACE_GROUP "RILSIM"

#include <cstdio>

namespace mbed {

static const char *IPV4_STR = "IP";
static const char *IPV6_STR = "IPV6";
static const char *IPV4V6_STR = "IPV4V6";
static const char *PPP_STR = "PPP";

RIL_Simulator::RIL_Simulator()
    : _queue_thread(osPriorityNormal, 2048, NULL, "ril_sim_queue"),
      _queue(),
      _callback(),
      _overallRadioState(RADIO_STATE_UNAVAILABLE),
      _sim_present(false),
      _reg_state(0),
      _pdp_contexts()
{
    if (_queue_thread.start(callback(&_queue, &events::EventQueue::dispatch_forever)) != osOK) {
        tr_error("Unable to start RIL simulator EQ thread!");
        MBED_ASSERT(0);
    }

    if (RIL_VERSION != 12) {
        tr_error("Simulation supports only RIL v12 !");
    }

    // Mark invalid cids as 0xff because 0 can be a valid context id
    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        _pdp_contexts[i].cid = 0xff;
    }
}

RIL_Simulator::~RIL_Simulator()
{
    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        delete_pdp_context(i);
    }
}

RIL_Simulator *RIL_Simulator::GetInstance()
{
    static RIL_Simulator *modem;

    if (modem == NULL) {
        modem = new RIL_Simulator();
    }
    return modem;
}

bool RIL_Simulator::Init(const struct RIL_Env *env)
{
    _callback = *env;
    _overallRadioState = RADIO_STATE_UNAVAILABLE;

    // send initial indications
    send_radio_ready_ind();
    send_radio_state_changed_ind(RADIO_STATE_OFF);
    send_sim_status_changed_ind();

    return true;
}

RIL_RadioState RIL_Simulator::handle_onStateRequest()
{
    return _overallRadioState;
}

void RIL_Simulator::enqueue_onRequest(int request, void *data, size_t datalen, RIL_Token t)
{
    ril_request req;
    req.request = request;
    req.data = data;
    req.datalen = datalen;
    req.t = t;
    _ril_request_lock.lock();
    _ril_requests.push(req);
    _ril_request_lock.unlock();

    int event_id = _queue.call(this, &RIL_Simulator::handle_onRequest);
    MBED_ASSERT(event_id);
}

void RIL_Simulator::handle_onRequest()
{
    _ril_request_lock.lock();
    if (!_ril_requests.empty()) {
        int request = _ril_requests.front().request;
        RIL_Token token = _ril_requests.front().t;
        void *data = _ril_requests.front().data;
        size_t datalen = _ril_requests.front().datalen;

        tr_debug("handle_onRequest: request = %d ...", request);

        switch (request) {
            case RIL_REQUEST_GET_SIM_STATUS:
                handle_RIL_REQUEST_GET_SIM_STATUS(token);
                break;
            case RIL_REQUEST_DATA_REGISTRATION_STATE:
                handle_RIL_REQUEST_DATA_REGISTRATION_STATE(token);
                break;
            case RIL_REQUEST_RADIO_POWER:
                handle_RIL_REQUEST_RADIO_POWER(token, data, datalen);
                break;
            case RIL_REQUEST_DATA_CALL_LIST:
                handle_RIL_REQUEST_DATA_CALL_LIST(token);
                break;
            case RIL_REQUEST_SETUP_DATA_CALL:
                handle_RIL_REQUEST_SETUP_DATA_CALL(token, data, datalen);
                break;
            case RIL_REQUEST_DEACTIVATE_DATA_CALL:
                handle_RIL_REQUEST_DEACTIVATE_DATA_CALL(token, data, datalen);
                break;
            case RIL_REQUEST_SET_INITIAL_ATTACH_APN:
                handle_RIL_REQUEST_SET_INITIAL_ATTACH_APN(token, data, datalen);
                break;
            default:
                tr_error("Invalid RIL request: %d !", _ril_requests.front().request);
        }

        tr_debug("handle_onRequest: request = %d ... done", request);

        // Remove request from pending list
        _ril_requests.pop();
    } else {
        tr_error("send_OnRequestComplete: No pending RIL requests!");
        MBED_ASSERT(0);
    }
    _ril_request_lock.unlock();
}

void RIL_Simulator::enqueue_OnUnsolicitedResponse(int unsolResponse, const void *data, size_t datalen)
{
    ril_unsolresponse resp;
    resp.unsolResponse = unsolResponse;
    resp.data = data;
    resp.datalen = datalen;
    _ril_unsolresponses.push(resp);

    int event_id = _queue.call(this, &RIL_Simulator::send_OnUnsolicitedResponse);
    MBED_ASSERT(event_id);
}

void RIL_Simulator::send_OnUnsolicitedResponse()
{
    if (!_ril_unsolresponses.empty()) {
        _callback.OnUnsolicitedResponse(_ril_unsolresponses.front().unsolResponse,
                                        _ril_unsolresponses.front().data,
                                        _ril_unsolresponses.front().datalen);
        _ril_unsolresponses.pop();
    } else {
        tr_error("send_OnUnsolicitedResponse: No pending OnUnsolicitedResponse!");
        MBED_ASSERT(0);
    }
}

void RIL_Simulator::send_radio_ready_ind()
{
    // Nothing to do here? RIL does not have a matching response
}

void RIL_Simulator::send_radio_state_changed_ind(RIL_RadioState state)
{
    _overallRadioState = state;
    enqueue_OnUnsolicitedResponse(RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED, NULL, 0);
}

void RIL_Simulator::send_sim_status_changed_ind()
{
    enqueue_OnUnsolicitedResponse(RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED, NULL, 0);
}

void RIL_Simulator::send_network_state_changed_ind(int reg_state)
{
    _reg_state = reg_state;
    enqueue_OnUnsolicitedResponse(RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED, NULL, 0);
}

void RIL_Simulator::send_data_call_list_changed_ind()
{
    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        _pdp_contexts_rearranged[i].cid = 0xff;
    }
    uint32_t active = 0;

    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        if (_pdp_contexts[i].cid != 0xff) {
            memcpy(&_pdp_contexts_rearranged[active], &_pdp_contexts[i], sizeof(RIL_Data_Call_Response_v11));
            active++;
        }
    }

    enqueue_OnUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED,
                                  _pdp_contexts_rearranged,
                                  sizeof(RIL_Data_Call_Response_v11) * active);
}


void RIL_Simulator::handle_RIL_REQUEST_GET_SIM_STATUS(RIL_Token t)
{
    RIL_CardStatus_v6 response = {};

    if (_sim_present) {
        response.card_state = RIL_CARDSTATE_PRESENT;
    } else {
        response.card_state = RIL_CARDSTATE_ABSENT;
    }

    response.universal_pin_state = RIL_PINSTATE_DISABLED;
    response.gsm_umts_subscription_app_index = 0;
    response.cdma_subscription_app_index = response.ims_subscription_app_index = -1;
    response.num_applications = 1;
    RIL_AppStatus &appStatus = response.applications[0];
    appStatus.app_type = RIL_APPTYPE_USIM;
    appStatus.app_state = RIL_APPSTATE_READY;
    appStatus.perso_substate = RIL_PERSOSUBSTATE_UNKNOWN;
    appStatus.aid_ptr = NULL;
    appStatus.app_label_ptr = NULL;
    appStatus.pin1 = RIL_PINSTATE_DISABLED;
    appStatus.pin2 = RIL_PINSTATE_DISABLED;

    _callback.OnRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(response));
}

void RIL_Simulator::handle_RIL_REQUEST_DATA_REGISTRATION_STATE(RIL_Token t)
{
    char szRegState[5] = {0, };
    char szRat[5] = {0, };
    char szReasonDataDenied[10] = {0, };
    char szMaxDataCalls[2] = {0, };
    char szTac[6] = {0, };
    char szPcid[4] = {0, };
    char szCid[11] = {0, };

    char *response[11] = {0, };

    int rat = RADIO_TECH_LTE;
    int reg_state = _reg_state;//1; // Registered to home network
    int rej_cause = 0;
    int lte_tac = 123456;
    int lte_pcid = 4444;
    int lte_eci = 1122334455;

    snprintf(szRat, sizeof(szRat) - 1, "%d", rat);
    snprintf(szRegState, sizeof(szRegState) - 1, "%d", reg_state);
    snprintf(szReasonDataDenied, sizeof(szReasonDataDenied) - 1, "%d", rej_cause);
    snprintf(szMaxDataCalls, sizeof(szMaxDataCalls) - 1, "%" PRIu32, MAX_SIM_PDP_CONTEXTS);
    snprintf(szTac, sizeof(szTac) - 1, "%d", lte_tac);
    snprintf(szPcid, sizeof(szPcid) - 1, "%d", lte_pcid);
    snprintf(szCid, sizeof(szCid) - 1, "%d", lte_eci);
    response[0] = szRegState;
    response[3] = szRat;
    response[4] = szReasonDataDenied;
    response[5] = szMaxDataCalls;
    response[6] = szTac;
    response[7] = szPcid;
    response[8] = szCid;

    _callback.OnRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(response));
}

void RIL_Simulator::handle_RIL_REQUEST_RADIO_POWER(RIL_Token t, void *data, size_t datalen)
{
    MBED_ASSERT(data);
    MBED_ASSERT(datalen > 0);
    _callback.OnRequestComplete(t, RIL_E_SUCCESS, NULL, 0);

    if (((int *)data)[0] > 0) {
        send_radio_state_changed_ind(RADIO_STATE_ON);
    } else {
        // Delete contexts
        for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
            delete_pdp_context(i);
        }

        send_radio_state_changed_ind(RADIO_STATE_OFF);
        send_network_state_changed_ind(0);
    }

    _sim_present = true;
    send_sim_status_changed_ind();
}


RIL_Data_Call_Response_v11 *RIL_Simulator::add_pdp_context(const char *pdp_type)
{
    RIL_Data_Call_Response_v11 resp = {0};

    RIL_Errno error = RIL_E_SUCCESS;

    resp.status             = PDP_FAIL_NONE;
    resp.suggestedRetryTime = 0;
    // cid will be assigned below
    resp.active             = 2;

    if (strcmp(pdp_type, IPV4_STR) == 0) {
        resp.type           = (char *)IPV4_STR;
    } else if (strcmp(pdp_type, IPV6_STR) == 0) {
        resp.type           = (char *)IPV6_STR;
    } else if (strcmp(pdp_type, IPV4V6_STR) == 0) {
        resp.type           = (char *)IPV4V6_STR;
    } else if (strcmp(pdp_type, PPP_STR) == 0) {
        resp.type           = (char *)PPP_STR;
    } else {
        resp.status         = RIL_E_GENERIC_FAILURE;
    }

    char *ifname = new char[32];
    if (ifname) {
        snprintf(ifname, 31, "ifname%d", resp.cid);
    }
    resp.ifname             = ifname;
    resp.addresses          = (char *)"10.0.0.1";
    resp.dnses              = (char *)"8.8.8.8";
    resp.gateways           = (char *)"10.0.0.99";
    resp.pcscf              = (char *)"";
    resp.mtu                = 1500;

    // Copy to active array
    int free_idx = 0xff;
    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        if (_pdp_contexts[i].cid == 0xff) {
            free_idx = i;
            break;
        }
    }

    if (free_idx != 0xff) {
        resp.cid = free_idx;
        memcpy(&_pdp_contexts[free_idx], &resp, sizeof(resp));
    } else {
        error = RIL_E_GENERIC_FAILURE;
    }

    // If successful, return pointer to the created response
    return error == RIL_E_SUCCESS ? &_pdp_contexts[free_idx] : NULL;
}

void RIL_Simulator::delete_pdp_context(int cid)
{
    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        _pdp_contexts[i].cid = i;

        delete[] _pdp_contexts[i].ifname;
        _pdp_contexts[i].ifname = NULL;
        break;
    }
}


void RIL_Simulator::handle_RIL_REQUEST_DATA_CALL_LIST(RIL_Token t)
{
    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        _pdp_contexts_rearranged[i].cid = 0xff;
    }
    uint32_t active = 0;

    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        if (_pdp_contexts[i].cid != 0xff) {
            memcpy(&_pdp_contexts_rearranged[active], &_pdp_contexts[i], sizeof(RIL_Data_Call_Response_v11));
            active++;
        }
    }

    _callback.OnRequestComplete(t, RIL_E_SUCCESS, _pdp_contexts_rearranged, sizeof(RIL_Data_Call_Response_v11) * active);
}

void RIL_Simulator::handle_RIL_REQUEST_SETUP_DATA_CALL(RIL_Token t, void *data, size_t datalen)
{
    RIL_Data_Call_Response_v11 fail_resp = {0};
    RIL_Data_Call_Response_v11 *new_pdp = NULL;

    MBED_ASSERT(data);
    MBED_ASSERT(datalen > 0);

    char **req = (char **)data;
    char *rat_str = req[0];
    int rat = atoi(rat_str);
    char *pdp_type = req[6];

    if (datalen != sizeof(char *) * 8 || !req) {
        fail_resp.status = RIL_E_GENERIC_FAILURE;
    } else if (_reg_state == 0) {
        fail_resp.status = RIL_E_OP_NOT_ALLOWED_BEFORE_REG_TO_NW;
    } else if (_overallRadioState != RADIO_STATE_ON) {
        fail_resp.status = RIL_E_RADIO_NOT_AVAILABLE;
    } else if (rat != 16) { // Only NB-IOT is supported
        fail_resp.status = RIL_E_REQUEST_NOT_SUPPORTED;
    } else {
        new_pdp = add_pdp_context(pdp_type);
        if (!new_pdp) {
            fail_resp.status = RIL_E_GENERIC_FAILURE;
        }
    }

    if (!new_pdp) {
        _callback.OnRequestComplete(t, RIL_E_SUCCESS, &fail_resp, sizeof(RIL_Data_Call_Response_v11));
    } else {
        _callback.OnRequestComplete(t, RIL_E_SUCCESS, new_pdp, sizeof(RIL_Data_Call_Response_v11));
        send_data_call_list_changed_ind();
    }
}

void RIL_Simulator::handle_RIL_REQUEST_DEACTIVATE_DATA_CALL(RIL_Token t, void *data, size_t datalen)
{
    MBED_ASSERT(data);
    MBED_ASSERT(datalen > 0);

    char **req = (char **)data;
    char *cid_str = req[0];
    int cid = atoi(cid_str);
//    char* reason_str = req[1];
//    int reason = atoi(reason_str);

    RIL_Errno error = RIL_E_GENERIC_FAILURE;

    for (int i = 0; i < MAX_SIM_PDP_CONTEXTS; i++) {
        if (_pdp_contexts[i].cid == cid) {
            _pdp_contexts[i].cid = 0xff; // Mark as invalid
            error = RIL_E_SUCCESS;
            break;
        }
    }

    _callback.OnRequestComplete(t, error, NULL, 0);
}

void RIL_Simulator::handle_RIL_REQUEST_SET_INITIAL_ATTACH_APN(RIL_Token t, void *data, size_t datalen)
{
    _callback.OnRequestComplete(t, RIL_E_SUCCESS, NULL, 0);

    if (_overallRadioState == RADIO_STATE_ON) {
        send_network_state_changed_ind(1); // Home
    }
}

#ifdef __cplusplus
extern "C" {
#endif

static void onRequest(int request, void *data, size_t datalen, RIL_Token t)
{
    mbed::RIL_Simulator::GetInstance()->enqueue_onRequest(request, data, datalen, t);
}

static RIL_RadioState onStateRequest()
{
    return mbed::RIL_Simulator::GetInstance()->handle_onStateRequest();
}

static const char *getVersion(void)
{
    return "RIL Simulator v12";
}

/**
 * RIL callbacks exposed to RILD daemon
 */
static const RIL_RadioFunctions s_callbacks = {
    12,
    onRequest,
    onStateRequest,
    NULL,
    NULL,
    getVersion
};

const RIL_RadioFunctions *RIL_Init(const struct RIL_Env *env, int argc, char **argv)
{
    mbed::RIL_Simulator *inst = mbed::RIL_Simulator::GetInstance();
    if (!inst->Init(env)) {
        return NULL;
    }
    return &s_callbacks;
}

#ifdef __cplusplus
}
#endif

} /* namespace mbed */

#endif // RIL_SIMULATOR

