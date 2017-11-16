/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

/*
 * \file mac_data_poll.c
 * \brief Add short description about this file!!!
 *
 */

#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "mac_api.h"
#include "mlme.h"

#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/network_lib.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/mac_data_poll.h"

#define TRACE_GROUP "mPol"

#define MAC_DATA_POLL_DEFAULT_RATE 20

#define MAC_DATA_POLL_REQUEST_EVENT 1

static int8_t mac_data_poll_tasklet = -1;

static void mac_data_poll_cb_run(int8_t interface_id);


static void mac_data_poll_cb(arm_event_s *event)
{
    uint8_t event_type = event->event_type;

    switch (event_type) {
        case MAC_DATA_POLL_REQUEST_EVENT:
            mac_data_poll_cb_run((int8_t)event->event_id);
            break;

        default:

            break;
    }
}

static int8_t mac_data_poll_tasklet_init(void)
{
    if (mac_data_poll_tasklet < 0) {
        mac_data_poll_tasklet = eventOS_event_handler_create(&mac_data_poll_cb, 0);
    }

    return mac_data_poll_tasklet;
}

static int8_t host_link_configuration(bool rx_on_idle, protocol_interface_info_entry_t *cur)
{
    bool backUp_bool = cur->mac_parameters->RxOnWhenIdle;
    if (rx_on_idle) {
        cur->lowpan_info &= ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    } else {
        tr_debug("Enable Poll state by host Link");
        cur->lowpan_info |= INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    }
    mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, rx_on_idle);

    if (thread_info(cur)) {
        if(rx_on_idle != backUp_bool){
            //If mode have been changed, send child update
            thread_bootstrap_child_update_trig(cur);
        }
        return 0;
    } else {
        if (protocol_6lowpan_child_update(cur) == 0) {
            mac_data_poll_init_protocol_poll(cur);
            return 0;
        }
    }

    //Swap back If Send Fail
    if (!backUp_bool) {
        cur->lowpan_info |= INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    } else {
        cur->lowpan_info &= ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    }
    mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, backUp_bool);
    return -1;
}

static void mac_data_poll_protocol_internal(protocol_interface_info_entry_t *cur)
{
    if (cur->rfd_poll_info->protocol_poll == 0) {
        if (cur->rfd_poll_info->host_mode == NET_HOST_SLOW_POLL_MODE) {
            if (!cur->rfd_poll_info->pollActive) {
                mac_poll_timer_trig(1, cur);
            }
        }
    }
    cur->rfd_poll_info->protocol_poll++;
    tr_debug("Protocol Poll: %02x", cur->rfd_poll_info->protocol_poll);
}

static void mac_data_poll_protocol_poll_internal_cancel(struct protocol_interface_info_entry *cur)
{
    nwk_rfd_poll_setups_s *rf_ptr = cur->rfd_poll_info;

    if (rf_ptr->protocol_poll == 0) {
        return;
    }

    if (--rf_ptr->protocol_poll == 0) {
        tr_debug("Disable Protocol Poll");
        if (!rf_ptr->pollActive) {
            if (rf_ptr->nwk_app_poll_time) {
                mac_poll_timer_trig(1, cur);
            } else {
                eventOS_event_timer_cancel(cur->id, mac_data_poll_tasklet);
                tr_debug("Stop Poll");
            }
        }
    }
}

void mac_data_poll_init_protocol_poll(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->rfd_poll_info || !(cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE)) {
        return;
    }

    mac_data_poll_protocol_internal(cur);

}

uint32_t mac_data_poll_host_poll_time_max(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->rfd_poll_info) {
        return 0;
    }

    return cur->rfd_poll_info->slow_poll_rate_seconds;
}

uint32_t mac_data_poll_host_timeout(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->rfd_poll_info) {
        return 0;
    }

    return cur->rfd_poll_info->timeOutInSeconds;
}

uint32_t mac_data_poll_get_max_sleep_period(protocol_interface_info_entry_t *cur)
{
    if (cur && cur->if_stack_buffer_handler && (cur->rfd_poll_info && !cur->rfd_poll_info->pollActive)) { // Verify That TX is not active
        if (cur->rfd_poll_info->protocol_poll) {
            return 300;
        } else {
            return cur->rfd_poll_info->nwk_app_poll_time;
        }
    }
    return 0;
}

void mac_data_poll_protocol_poll_mode_decrement(struct protocol_interface_info_entry *cur)
{
    if (!cur || !cur->rfd_poll_info) {
        return;
    }

    if (!(cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE)) {
        return;
    }

    mac_data_poll_protocol_poll_internal_cancel(cur);
}

void mac_data_poll_protocol_poll_mode_disable(struct protocol_interface_info_entry *cur)
{
    if (!cur || !cur->rfd_poll_info) {
        return;
    }

    if (!(cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE)) {
        return;
    }

    mac_data_poll_protocol_poll_internal_cancel(cur);
}

void mac_data_poll_disable(struct protocol_interface_info_entry *cur)
{
    if (!cur || !cur->rfd_poll_info) {
        return;
    }

    eventOS_event_timer_cancel(cur->id, mac_data_poll_tasklet);
    cur->rfd_poll_info->protocol_poll = 0;
    cur->rfd_poll_info->pollActive = false;
}

void mac_data_poll_enable_check(protocol_interface_info_entry_t *cur)
{
    if (!cur || !cur->rfd_poll_info ) {
        return;
    }

    if (!(cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE)) {
        return;
    }

    mac_data_poll_protocol_internal(cur);
}

int8_t mac_data_poll_host_mode_get(struct protocol_interface_info_entry *cur, net_host_mode_t *mode)
{
    if (!cur || !cur->rfd_poll_info) {
        return -1;
    }

    *mode = cur->rfd_poll_info->host_mode;
    return 0;
}

void mac_poll_timer_trig(uint32_t poll_time, protocol_interface_info_entry_t *cur)
{
    if( !cur ){
        return;
    }
    eventOS_event_timer_cancel(cur->id, mac_data_poll_tasklet);

    if (poll_time) {
        if (cur->lowpan_info & INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE) {

            if (poll_time < 20) {
                arm_event_s event = {
                        .receiver = mac_data_poll_tasklet,
                        .sender = mac_data_poll_tasklet,
                        .event_id = cur->id,
                        .data_ptr = NULL,
                        .event_type = MAC_DATA_POLL_REQUEST_EVENT,
                        .priority = ARM_LIB_MED_PRIORITY_EVENT,
                };

                if (eventOS_event_send(&event) != 0) {
                    tr_error("eventOS_event_send() failed");
                }
            } else {
                if (eventOS_event_timer_request(cur->id, MAC_DATA_POLL_REQUEST_EVENT, mac_data_poll_tasklet, poll_time)!= 0) {
                    tr_error("Poll Timer start Fail");
                }
            }

        }
    }
}

void mac_mlme_poll_confirm(protocol_interface_info_entry_t *cur, const mlme_poll_conf_t *confirm)
{
    if( !cur || !confirm ){
        return;
    }

    uint32_t poll_time = 1;
    nwk_rfd_poll_setups_s *rf_ptr = cur->rfd_poll_info;
    if( !rf_ptr ){
        return;
    }

    rf_ptr->pollActive = false;

    switch (confirm->status) {
        case MLME_SUCCESS:
            //tr_debug("Poll Confirm: Data with Data");
            rf_ptr->nwk_parent_poll_fail = 0;
            //Trig new Data Poll immediately
            mle_refresh_entry_timeout(cur->id, rf_ptr->poll_req.CoordAddress, (addrtype_t)rf_ptr->poll_req.CoordAddrMode, true);
            poll_time = 1;
            break;

        case MLME_NO_DATA:
            //Start next case timer
            rf_ptr->nwk_parent_poll_fail = 0;
            mle_refresh_entry_timeout(cur->id, rf_ptr->poll_req.CoordAddress, (addrtype_t)rf_ptr->poll_req.CoordAddrMode, true);
            //tr_debug("Poll Confirm: No Data");

            if (rf_ptr->protocol_poll == 0) {
                poll_time = rf_ptr->nwk_app_poll_time;
            } else {
                poll_time = 300;
            }

            break;
        default:
            tr_debug("Poll Confirm: fail %x", confirm->status);
            rf_ptr->nwk_parent_poll_fail++;
            if (rf_ptr->nwk_parent_poll_fail  > 3) {
                //tr_debug("Parent Poll Fail");
                poll_time = 0;
                rf_ptr->nwk_parent_poll_fail = 0;
                if( rf_ptr->pollFailCb ){
                    rf_ptr->pollFailCb(cur->id);
                }
            } else {
                poll_time = 2000;
            }
            break;
    }

    mac_poll_timer_trig(poll_time, cur);


}


static void mac_data_poll_cb_run(int8_t interface_id)
{
    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur) {
        return;
    }
    nwk_rfd_poll_setups_s *rf_ptr = cur->rfd_poll_info;

    if (!rf_ptr) {
        return;
    }

    rf_ptr->poll_req.CoordAddrMode = (unsigned)mac_helper_coordinator_address_get(cur, rf_ptr->poll_req.CoordAddress);

    if (rf_ptr->poll_req.CoordAddrMode == MAC_ADDR_MODE_NONE) {
        return;
    }

    memset(&rf_ptr->poll_req.Key, 0, sizeof(mlme_security_t));
    rf_ptr->poll_req.CoordPANId = mac_helper_panid_get(cur);

    rf_ptr->poll_req.Key.SecurityLevel = mac_helper_default_security_level_get(cur);
    if (rf_ptr->poll_req.Key.SecurityLevel) {

        rf_ptr->poll_req.Key.KeyIndex = mac_helper_default_key_index_get(cur);
        rf_ptr->poll_req.Key.KeyIdMode = mac_helper_default_security_key_id_mode_get(cur);
    }

    if (cur->mac_api && cur->mac_api->mlme_req) {
        rf_ptr->pollActive = true;
        cur->mac_api->mlme_req(cur->mac_api, MLME_POLL, (void*) &rf_ptr->poll_req);
    } else {
        tr_error("MAC not registered to interface");
    }
}

int8_t mac_data_poll_host_mode_set(struct protocol_interface_info_entry *cur, net_host_mode_t mode, uint32_t poll_time)
{
#ifndef NO_MLE
    if( !cur){
        return -1;
    }
    int8_t ret_val = 0;
    nwk_rfd_poll_setups_s *rf_ptr = cur->rfd_poll_info;
    //Check IF Bootsrap Ready and type is Host
    if (!rf_ptr ) {
        return -1;
    }

    if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
        return -3;
    }


    switch (mode) {
        case NET_HOST_SLOW_POLL_MODE:
            //slow mode
            //Check Host current sleep state
            if (rf_ptr->host_mode == NET_HOST_FAST_POLL_MODE || rf_ptr->host_mode == NET_HOST_SLOW_POLL_MODE || rf_ptr->host_mode == NET_HOST_RX_ON_IDLE) {
                uint32_t new_poll_time;
                uint32_t new_Timeout;

                if (poll_time == 0 || poll_time > 864001) {
                    return -2;
                }

                //Calculate
                //Timeout from Poll Time
                new_Timeout = poll_time * 4;

                if (new_Timeout < 32) { //Keep Timeout always to 32 seconds min
                    new_Timeout = 32;
                }

                if (new_Timeout != rf_ptr->timeOutInSeconds) {
                    rf_ptr->timeOutInSeconds = new_Timeout;
                }

                rf_ptr->slow_poll_rate_seconds = poll_time;

                new_poll_time = (poll_time * 1000);
                if (rf_ptr->host_mode == NET_HOST_RX_ON_IDLE) {
                    tr_debug("Init Poll timer and period");
                    mle_class_mode_set(cur->id, MLE_CLASS_SLEEPY_END_DEVICE);
                }

                rf_ptr->nwk_app_poll_time = new_poll_time;
                tr_debug("Enable Poll state slow poll");
                if (host_link_configuration(false, cur) != 0) {
                    return -3;
                }
                mac_poll_timer_trig(1, cur);
                rf_ptr->host_mode = NET_HOST_SLOW_POLL_MODE;
            }
            break;
        case NET_HOST_FAST_POLL_MODE:
            //fast mode
            //Check Host current sleep state
            if (rf_ptr->host_mode == NET_HOST_FAST_POLL_MODE || rf_ptr->host_mode == NET_HOST_SLOW_POLL_MODE || rf_ptr->host_mode == NET_HOST_RX_ON_IDLE) {
                if (rf_ptr->host_mode != NET_HOST_FAST_POLL_MODE) {
                    tr_debug("Enable Fast poll mode");
                    if (rf_ptr->host_mode == NET_HOST_RX_ON_IDLE) {
                        tr_debug("Init Poll timer and period");
                        if (host_link_configuration(false, cur) != 0) {
                            return  -3;
                        }
                    }
                }
                tr_debug("Enable Poll By APP");
                mle_class_mode_set(cur->id, MLE_CLASS_SLEEPY_END_DEVICE);
                mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, false);
                mac_poll_timer_trig(1, cur);
                rf_ptr->nwk_app_poll_time = 300;
                rf_ptr->host_mode = NET_HOST_FAST_POLL_MODE;

            }
            break;

        case NET_HOST_RX_ON_IDLE:
            // Non-sleep mode
            if (rf_ptr->host_mode == NET_HOST_FAST_POLL_MODE || rf_ptr->host_mode == NET_HOST_SLOW_POLL_MODE) {
                if (host_link_configuration(true, cur) == 0) {
                    rf_ptr->host_mode = NET_HOST_RX_ON_IDLE;
                    rf_ptr->nwk_app_poll_time = 0;
                } else {
                    ret_val = -3;
                }
            }
            break;

        default:
            ret_val = -1;
            break;
    }

    return ret_val;
#else /* MLE_DRAFT */
    return -1;
#endif /* MLE_DRAFT */
}

void mac_data_poll_init(struct protocol_interface_info_entry *cur)
{
    if (!cur) {
        return;
    }

    if (cur->lowpan_info & INTERFACE_NWK_ROUTER_DEVICE) {
        if (cur->rfd_poll_info) {
            ns_dyn_mem_free(cur->rfd_poll_info);
            cur->rfd_poll_info = NULL;
        }
        mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, true);
        return;
    }

    //Allocate and Init
    nwk_rfd_poll_setups_s *rfd_ptr = cur->rfd_poll_info;

    if (!rfd_ptr) {
        if (mac_data_poll_tasklet_init() < 0 ) {
            tr_error("Mac data poll tasklet init fail");
        } else {
            rfd_ptr = ns_dyn_mem_alloc(sizeof(nwk_rfd_poll_setups_s));
            if (rfd_ptr) {
                rfd_ptr->timeOutInSeconds = 0;
                rfd_ptr->nwk_parent_poll_fail = false;
                rfd_ptr->protocol_poll = 0;
                rfd_ptr->slow_poll_rate_seconds = 0;
                rfd_ptr->pollActive = false;
                cur->rfd_poll_info = rfd_ptr;
            }
        }
    }

    if (!rfd_ptr) {
        return;
    }

    //Set MAc Data poll Fail Callback
    rfd_ptr->pollFailCb = nwk_parent_poll_fail_cb;

    if (cur->mac_parameters->RxOnWhenIdle) {
        tr_debug("Set Non-Sleepy HOST");
        rfd_ptr->host_mode = NET_HOST_RX_ON_IDLE;
        mle_class_mode_set(cur->id, MLE_CLASS_END_DEVICE);
    } else {

        rfd_ptr->protocol_poll = 1;
        mac_poll_timer_trig(200, cur);
        tr_debug("Set Sleepy HOST configure");
        rfd_ptr->host_mode = NET_HOST_FAST_POLL_MODE;
        mle_class_mode_set(cur->id, MLE_CLASS_SLEEPY_END_DEVICE);
        rfd_ptr->slow_poll_rate_seconds = 3;
        rfd_ptr->timeOutInSeconds = 32;
        rfd_ptr->nwk_app_poll_time = 300;
        rfd_ptr->nwk_parent_poll_fail = 0;
    }
}
