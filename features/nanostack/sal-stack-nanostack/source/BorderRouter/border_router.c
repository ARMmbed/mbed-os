/*
 * Copyright (c) 2012-2017, Arm Limited and affiliates.
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
#include "ns_types.h"
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan.h"
#include "6LoWPAN/Bootstraps/protocol_6lowpan_bootstrap.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif
#include "mac_api.h"

#include "RPL/rpl_control.h"
#include "6LoWPAN/ND/nd_router_object.h"
#include "Service_Libs/whiteboard/whiteboard.h"
#include "Service_Libs/blacklist/blacklist.h"
#include "Service_Libs/nd_proxy/nd_proxy.h"
#include "shalib.h"

#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "Security/TLS/tls_lib.h"
#include "Security/Common/sec_lib.h"
#include "net_nvm_api.h"
#include "Security/PANA/pana.h"
#include "Security/PANA/pana_internal_api.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Common_Protocols/icmpv6_prefix.h"
#include "Common_Protocols/icmpv6_radv.h"
#include "ipv6_stack/protocol_ipv6.h"
#include "common_functions.h"
#include "net_thread_test.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "6LoWPAN/Thread/thread_routing.h"
#include "BorderRouter/border_router.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/MAC/beacon_handler.h"
#include "6LoWPAN/NVM/nwk_nvm.h"
#include "libNET/src/net_load_balance_internal.h"
#include "6LoWPAN/lowpan_adaptation_interface.h"
#include "6LoWPAN/Fragmentation/cipv6_fragmenter.h"

#ifdef HAVE_6LOWPAN_BORDER_ROUTER

#define TRACE_GROUP_BORDER_ROUTER  "br"

#define TRACE_GROUP  "br"


static int8_t border_router_nd_abro_periodically_update_by_stack(nd_router_setup_t *nd_router_configuration);
static void border_router_free(protocol_interface_info_entry_t *cur);

void arm_nwk_6lowpan_borderrouter_data_free(protocol_interface_info_entry_t *cur)
{
    if (cur->border_router_setup) {
        if (cur->border_router_setup->nd_nwk) {
            icmp_nd_border_router_release(cur->border_router_setup->nd_nwk);
            ns_dyn_mem_free(cur->border_router_setup->nd_nwk);
        }
        if (cur->border_router_setup->nd_border_router_configure) {
            ns_dyn_mem_free(cur->border_router_setup->nd_border_router_configure);
        }
        ns_dyn_mem_free(cur->border_router_setup);
        cur->border_router_setup = 0;
    }
}

void nd_border_router_setup_refresh(nwk_interface_id id, bool fresh_abro)
{
    uint8_t *ptr = 0;
    nd_router_t *nd_router_object;
    nd_router_setup_t *nd_configure;
    protocol_interface_info_entry_t *cur_interface;
    uint8_t nd_options[30];

    cur_interface = protocol_stack_interface_info_get(id);
    if (!cur_interface) {
        return;
    } else if (cur_interface->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        return;
    } else if (cur_interface->border_router_setup == 0) {
        return;
    }
    else if (!cur_interface->border_router_setup->nd_nwk) {
        return;
    } else if (!cur_interface->border_router_setup->nd_border_router_configure) {
        return;
    }

    nd_router_object = cur_interface->border_router_setup->nd_nwk;
    nd_configure = cur_interface->border_router_setup->nd_border_router_configure;
    nd_router_object->life_time = nd_configure->life_time;

    if (!ns_list_is_empty(&nd_router_object->prefix_list)) {
        tr_debug("Release Prefix\n");
        icmpv6_prefix_list_free(&nd_router_object->prefix_list);
    }

    if (!ns_list_is_empty(&nd_router_object->context_list)) {
        tr_info("Release Context");
        lowpan_context_list_free(&nd_router_object->context_list);
    }

    if (!ns_list_is_empty(&nd_configure->context_list)) {
        tr_info("Refresh Contexts");
        ns_list_foreach(lowpan_context_t, cur, &nd_configure->context_list) {
            uint8_t cid_flags = cur->cid | (cur->compression ? LOWPAN_CONTEXT_C : 0);
            uint16_t lifetime_mins = (cur->lifetime + 599) / 600;
            /* Update contexts in our ABRO advertising storage */
            lowpan_context_update(&nd_router_object->context_list, cid_flags, lifetime_mins, cur->prefix, cur->length, true);
            /* And contexts used by the interface itself (we don't hear our own adverts) */
            lowpan_context_update(&cur_interface->lowpan_contexts, cid_flags, lifetime_mins, cur->prefix, cur->length, true);
        }
    }
    /* Set Prefixs */
    if (!ns_list_is_empty(&nd_configure->prefix_list)) {
        tr_info("Refresh Prefixs");
        ns_list_foreach(prefix_entry_t, cur, &nd_configure->prefix_list) {
            ptr = nd_options;
            *ptr++ = cur->prefix_len; //Prefix Len
            *ptr++ = cur->options;   //Autonomous address enabled
            ptr = common_write_32_bit(cur->lifetime, ptr);
            ptr = common_write_32_bit(cur->preftime, ptr);
            ptr = common_write_32_bit(0, ptr); //Reserved

            memcpy(ptr, cur->prefix, 16);
            icmp_nd_router_prefix_update(nd_options, nd_router_object, cur_interface);
        }
    }

    //Update version num

    if (fresh_abro) {
        if (border_router_nd_abro_periodically_update_by_stack(nd_configure) == 0) {
            tr_info("ABRO Update and NVM operation OK");
        }
    }

    nd_router_object->abro_version_num = nd_configure->abro_version_num;
}

int8_t arm_nwk_6lowpan_border_route_nd_default_prefix_timeout_set(int8_t interface_id, uint32_t time)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        uint8_t *nd_options = ns_dyn_mem_temporary_alloc(30);
        if (nd_options) {
            uint8_t *ptr;
            ptr = nd_options;

            ptr = common_write_16_bit(0x4040, ptr); //Prefix Len + Autonomous address enabled
            ptr = common_write_32_bit(time, ptr);
            ptr = common_write_32_bit(time, ptr);
            memcpy(ptr, cur->border_router_setup->border_router_gp_adr, 8);
            ptr += 8;
            memset(ptr, 0, 8);
            ret_val = icmp_nd_router_prefix_proxy_update(nd_options, cur->border_router_setup->nd_border_router_configure);
            ns_dyn_mem_free(nd_options);
        }
    }
    return ret_val;
}

int8_t arm_nwk_6lowpan_border_router_context_update(int8_t interface_id, uint8_t c_id_flags, uint8_t context_len, uint16_t ttl, const uint8_t *context_ptr)
{
    int8_t ret_val = -2;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        ret_val = 0;
        if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
            ret_val = -4;
        } else if (cur->border_router_setup == 0) {
            ret_val = -3;
        } else {
            if (c_id_flags < 0x20 && context_len >= 64) {
                if (cur->border_router_setup->nd_nwk) {
                    nd_router_setup_t *routerSetup = cur->border_router_setup->nd_border_router_configure;

                    if (!lowpan_contex_get_by_id(&routerSetup->context_list, (c_id_flags & LOWPAN_CONTEXT_CID_MASK))) {
                        if (ns_list_count(&routerSetup->context_list) >= ND_MAX_PROXY_CONTEXT_COUNT) {
                            return -1;
                        }
                    }

                    if (lowpan_context_update(&routerSetup->context_list, c_id_flags, ttl, context_ptr, context_len, true) != 0) {
                        ret_val = -2;
                    } else {
                        ret_val = 0;
                    }
                }
            } else {
                ret_val = -3;
            }
        }
    }
    return ret_val;
}

int8_t arm_nwk_6lowpan_border_router_nd_context_load(int8_t interface_id, uint8_t *contex_data)
{
    int8_t ret_val = -2;
    protocol_interface_info_entry_t *cur = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        ret_val = 0;
        if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
            ret_val = -4;
        } else if (cur->border_router_setup == 0) {
            ret_val = -3;
        } else {
            uint8_t c_id;
            uint16_t lifetime;
            nd_router_setup_t *nd_router_setup;
            uint8_t con_len = *contex_data++;

            nd_router_setup = cur->border_router_setup->nd_border_router_configure;

            c_id = *contex_data++ & 0x1f; // ignore reserved fields
            lifetime = common_read_16_bit(contex_data);
            contex_data += 2;
            //Now Pointer Indicate to prefix
            //Check first is current ID at list
            if (!lowpan_contex_get_by_id(&nd_router_setup->context_list, (c_id & LOWPAN_CONTEXT_CID_MASK))) {
                if (ns_list_count(&nd_router_setup->context_list) >= ND_MAX_PROXY_CONTEXT_COUNT) {
                    tr_debug("All Contexts are allocated");
                    return -1;
                }
            }
            return lowpan_context_update(&nd_router_setup->context_list, c_id, lifetime, contex_data, con_len, true);

        }
    }
    return ret_val;
}


//int8_t border_router_nd_configure_update(void)
int8_t arm_nwk_6lowpan_border_router_configure_push(int8_t interface_id)
{
    int8_t ret_val = -1;
    protocol_interface_info_entry_t *cur_interface;
    cur_interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur_interface) {
        ret_val = 0;
        if (cur_interface->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
            ret_val = -4;
        } else if (cur_interface->border_router_setup == 0) {
            ret_val = -3;
        } else if ((cur_interface->lowpan_info & INTERFACE_NWK_ACTIVE) == 0) {
            ret_val = -2;
        } else {
            if (thread_info(cur_interface) == NULL) {
                cur_interface->border_router_setup->nd_nwk->nd_timer = 1;
                cur_interface->border_router_setup->nd_nwk->nd_re_validate = 1;
                cur_interface->border_router_setup->nd_nwk->abro_version_num++;
                ret_val = 0;
            }
        }
    }
    return ret_val;
}

int8_t arm_nwk_6lowpan_border_router_context_remove_by_id(int8_t interface_id, uint8_t c_id)
{
    lowpan_context_t *entry;
    protocol_interface_info_entry_t *cur_interface = 0;
    nd_router_setup_t *nd_router_configuration = 0;
    cur_interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur_interface) {
        return -1;
    }

    if (cur_interface->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        return -4;
    }

    if (cur_interface->border_router_setup == 0) {
        return -3;
    }

    nd_router_configuration = cur_interface->border_router_setup->nd_border_router_configure;

    entry = lowpan_contex_get_by_id(&nd_router_configuration->context_list, c_id);
    if (entry) {
        ns_list_remove(&nd_router_configuration->context_list, entry);
        ns_dyn_mem_free(entry);
    }
    return 0;
}

int8_t arm_nwk_6lowpan_border_router_context_parameter_update(int8_t interface_id, uint8_t c_id, uint8_t compress_mode, uint16_t ttl)
{
    protocol_interface_info_entry_t *cur_interface;
    nd_router_setup_t *nd_router_configuration;
    lowpan_context_t *entry;
    cur_interface = protocol_stack_interface_info_get_by_id(interface_id);
    if (!cur_interface) {
        return -1;
    }

    if (cur_interface->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
        return -4;
    }

    if (cur_interface->border_router_setup == 0) {
        return -3;
    }

    nd_router_configuration = cur_interface->border_router_setup->nd_border_router_configure;

    entry = lowpan_contex_get_by_id(&nd_router_configuration->context_list, c_id);
    if (entry) {
        uint8_t cid_flag = entry->cid;
        entry->compression = compress_mode;
        entry->lifetime = ttl;
        cid_flag |= (entry->compression ? LOWPAN_CONTEXT_C : 0);
        return 0;
    }
    return -1;
}

int8_t arm_nwk_6lowpan_border_router_init(int8_t interface_id, const border_router_setup_s *border_router_setup_ptr)
{
    int8_t ret_val = -3;
    protocol_interface_info_entry_t *cur = 0;
    nd_router_setup_t *nd_router_configuration = 0;
    cur = protocol_stack_interface_info_get_by_id(interface_id);
    if (cur) {
        ret_val = 0;
        if (cur->bootsrap_mode != ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER) {
            ret_val = -4;
        } else if (cur->border_router_setup == 0) {
            ret_val = -3;
        } else if (thread_info(cur)) {
            ret_val = -5;
        } else if ((cur->lowpan_info & INTERFACE_NWK_ACTIVE) ==  0) {
            uint8_t *ptr = cur->border_router_setup->border_router_gp_adr;
            memcpy(ptr, border_router_setup_ptr->lowpan_nd_prefix, 8);
            ptr += 8;
            cur->border_router_setup->mac_short_adr = border_router_setup_ptr->mac_short_adr;
            if (border_router_setup_ptr->mac_short_adr < 0xfffe) {
                cur->lowpan_address_mode = NET_6LOWPAN_GP16_ADDRESS;
                mac_helper_mac16_address_set(cur, border_router_setup_ptr->mac_short_adr);
                memcpy(ptr, ADDR_SHORT_ADR_SUFFIC, 6);
                ptr += 6;
                ptr = common_write_16_bit(cur->border_router_setup->mac_short_adr, ptr);
            } else {
                memcpy(ptr, cur->iid_eui64, 8);
                cur->lowpan_address_mode = NET_6LOWPAN_GP64_ADDRESS;
            }

            nd_router_configuration = cur->border_router_setup->nd_border_router_configure;

            nd_router_configuration->life_time =  border_router_setup_ptr->ra_life_time;
            nd_router_configuration->abro_version_num = border_router_setup_ptr->abro_version_num;
            cur->configure_flags |= INTERFACE_ND_BORDER_ROUTER_DEFINED;
        } else {
            ret_val = -1;
        }

        if (ret_val == 0) {
            uint8_t *nd_options  = 0;
            uint8_t *beaon_payload = 0;

            //Check here FHSS Pointer Hox Hoxs
            beaon_payload = mac_helper_beacon_payload_reallocate(cur, 18);

            cur->border_router_setup->mac_panid = border_router_setup_ptr->mac_panid;

            if (beaon_payload == NULL) {
                ret_val = -2;
            } else {
                *beaon_payload++ = border_router_setup_ptr->beacon_protocol_id;
                *beaon_payload++ = 7; //Accept Join / Host & Router
                memcpy(beaon_payload, border_router_setup_ptr->network_id, 16);

                if (mac_helper_beacon_payload_register(cur) != 0) {
                    tr_error("Beacon payload register Fail");
                }

                nd_options  = ns_dyn_mem_temporary_alloc(30);
                if (nd_options) {
                    uint8_t *ptr = cur->border_router_setup->nd_nwk->border_router;
                    nd_router_base_init(cur->border_router_setup->nd_nwk);


                    cur->border_router_setup->nd_nwk->nd_state = ND_BR_READY;
                    cur->border_router_setup->nd_nwk->nwk_id = IF_6LoWPAN;
                    cur->border_router_setup->nd_nwk->life_time = border_router_setup_ptr->ra_life_time;
                    memcpy(cur->border_router_setup->nd_nwk->border_router, cur->border_router_setup->border_router_gp_adr, 16);
                    if (border_router_setup_ptr->mac_short_adr < 0xfffe) {
                        cur->border_router_setup->mac_short_adr = border_router_setup_ptr->mac_short_adr;
                    } else {
                        cur->border_router_setup->mac_short_adr = 0xffff;
                    }

                    //Create GP64
                    if (ret_val == 0) {
                        ptr = nd_options;
                        *ptr++ = 64;//Prefix Len
                        *ptr++ = PIO_A; //Autonomous address enabled
                        memset(ptr, 0xff, 8);/* Valid Life time & Valid Preferred Life time  0xffffff */
                        ptr += 8;
                        memcpy(ptr, border_router_setup_ptr->lowpan_nd_prefix, 8);
                        ptr += 8;
                        memset(ptr, 0, 8);
                        ret_val = icmp_nd_router_prefix_proxy_update(nd_options, nd_router_configuration);

                        //Default Prefix TO ND
                    }
                    if (ret_val == 0) {
                        //Context
                        if (lowpan_context_update(&nd_router_configuration->context_list, (LOWPAN_CONTEXT_C | 0), (24 * 60), border_router_setup_ptr->lowpan_nd_prefix, 64, true) != 0) {
                            ret_val = -2;
                        }
                    }

                    ns_dyn_mem_free(nd_options);
                    if (ret_val == 0) {
                        tr_info("BR nwk base ready for start");
                    }
                } else {
                    ret_val = -2;
                }
            }

            if (ret_val == -2) {
                if (cur) {
                    border_router_free(cur);
                }
            }
        }
    }
    return ret_val;
}

static void border_router_free(protocol_interface_info_entry_t *cur)
{
    if (cur->border_router_setup) {
        if (cur->border_router_setup->nd_nwk) {
            icmp_nd_router_object_reset(cur->border_router_setup->nd_nwk);
            ns_dyn_mem_free(cur->border_router_setup->nd_nwk);
        }


        if (cur->border_router_setup->nd_border_router_configure) {
            icmpv6_prefix_list_free(&cur->border_router_setup->nd_border_router_configure->prefix_list);
            lowpan_context_list_free(&cur->border_router_setup->nd_border_router_configure->context_list);
        }
        ns_dyn_mem_free(cur->border_router_setup);
        cur->border_router_setup = 0;
    }
}

static int8_t border_router_nd_abro_periodically_update_by_stack(nd_router_setup_t *nd_router_configuration)
{
    int8_t ret_val = -1;
    if (nd_router_configuration) {
        nd_router_configuration->abro_version_num++;
        ret_val = 0;
    }
    return ret_val;
}

void border_router_start(protocol_interface_info_entry_t *cur, bool warm_link_restart)
{
    nd_router_t *nd_nwk = cur->border_router_setup->nd_nwk;
    mlme_start_t start_req;
    memset(&start_req, 0, sizeof(mlme_start_t));
    start_req.PANId = cur->border_router_setup->mac_panid;
    start_req.LogicalChannel = cur->mac_parameters->mac_channel;
    start_req.ChannelPage = 0;
    start_req.BeaconOrder = 0x0f;
    start_req.SuperframeOrder = 0x0f;
    start_req.PANCoordinator = 1;
    if( cur->mac_api ){
        protocol_timer_start(PROTOCOL_TIMER_BOOTSTRAP_TIM, bootstrap_timer_handle, BOOTSTRAP_START_TIMEOUT);
        cur->mac_api->mlme_req(cur->mac_api, MLME_START, (void*)&start_req);
    }
    if (warm_link_restart) {
        return;
    }

#ifndef NO_MLE
    mle_class_list_clean(cur->id);
    blacklist_clear();
#endif

    cur->border_router_setup->initActive = true;

    cur->lowpan_info |= INTERFACE_NWK_BOOTSRAP_ADDRESS_REGISTER_READY;
    addr_interface_set_ll64(cur, NULL);
    cur->interface_mode = INTERFACE_UP;
    cur->nwk_mode = ARM_NWK_GP_IP_MODE;
    if (nd_nwk) {
        if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
            protocol_6lowpan_router_synch_to_new_router(cur, NULL, 0, true);
            cur->bootsrap_state_machine_cnt = 30;
        } else {
            nd_nwk->mle_advert_timer = 0;
            cur->bootsrap_state_machine_cnt = 1;
        }
        cur->nwk_bootstrap_state = ER_SCAN;
        whiteboard_interface_register(nd_nwk->border_router, cur->id);
        cur->nwk_nd_re_scan_count = 2;

    } else {
        thread_interface_up(cur);
        rpl_control_remove_domain_from_interface(cur);
        nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, cur);
    }
}

static int arm_mac_channel_list_analyze(protocol_interface_info_entry_t *cur)
{
    int number_of_channels = 0;
    for (int i=0; i<8; i++) {
        for (int j=0; j<4; j++) {
            number_of_channels += common_count_bits((uint8_t)(cur->mac_parameters->mac_channel_list.channel_mask[i] >> (j * 8)));
        }
    }
    return number_of_channels;
}

static int8_t arm_border_router_interface_up(protocol_interface_info_entry_t *cur)
{
    bool warm_restart = false;
    if (cur->interface_mode == INTERFACE_UP || cur->lowpan_info & INTERFACE_NWK_ACTIVE) {
        //Disable interface like starting normal scan request
        if (cur->nwk_bootstrap_state != ER_BOOTSRAP_DONE) {
            return -4;
        }
        warm_restart = true;
        tr_debug("Do warm link start");
    } else {
        nd_router_t *nd_nwk = cur->border_router_setup->nd_nwk;
        //SET SHort Address
        if (cur->border_router_setup->mac_short_adr < 0xfffe) {
            mac_helper_mac16_address_set(cur, cur->border_router_setup->mac_short_adr);
            cur->lowpan_desired_short_address = cur->border_router_setup->mac_short_adr;
            protocol_6lowpan_set_ll16(cur, cur->border_router_setup->mac_short_adr);
        }
        mac_helper_panid_set(cur, cur->border_router_setup->mac_panid);

        if (cur->nwk_wpan_nvm_api) {
            wpan_nvm_params_t *params = cur->nwk_wpan_nvm_api->nvm_params_get_cb(cur->nwk_wpan_nvm_api, cur->border_router_setup->mac_panid);
            cur->if_lowpan_security_params->mle_security_frame_counter = params->mle_securit_counter;
            //SET MAC and MLE security frame counters
            mle_service_security_set_frame_counter(cur->id, params->mle_securit_counter);
            mac_helper_link_frame_counter_set(cur->id, params->mac_security_frame_counter);
        }

        // cur->mac_channel = cur->border_router_setup->channel;
        if (nd_nwk) {
            nd_border_router_setup_refresh(nd_nwk->nwk_id, false);
        }
        cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER;
        icmpv6_recv_ra_routes(cur, false);
        protocol_6lowpan_interface_common_init(cur);

        mac_helper_pib_boolean_set(cur, macRxOnWhenIdle, true);
        mac_helper_pib_boolean_set(cur, macAssociationPermit, true);

        arm_6lowpan_security_init_ifup(cur);

        mac_helper_default_security_level_set(cur, cur->mac_parameters->mac_configured_sec_level);

        //set 6lowpan default
        mac_helper_mac_mlme_max_retry_set(cur->id, LOWPAN_MAX_FRAME_RETRIES);

        if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_PANA_AUTHENTICATION) {
            if (pana_server_key_material_load(cur->id) != 0) {
                tr_warn("Border router security load fail");
                return -3;
            }
        }
    }

    int number_of_channels = arm_mac_channel_list_analyze(cur);
    cur->lowpan_info |= INTERFACE_NWK_ACTIVE;

    /* Start network scan when at least 2 of 256 channels are enabled,
     * otherwise MAC is started immediately.
     */
    if (number_of_channels > 1) {
        if (warm_restart) {
            bootsrap_next_state_kick(ER_WARM_ACTIVE_SCAN, cur);
        } else {
            bootsrap_next_state_kick(ER_ACTIVE_SCAN, cur);
        }
    } else {
        border_router_start(cur, warm_restart);
    }
    return 0;
}

static int arm_border_router_proxy_validate(int8_t interface_id, uint8_t *address)
{

    /* Could also check route type, but I don't think it really matters */
    ipv6_route_t *route;
    route = ipv6_route_choose_next_hop(address, interface_id, NULL);
    if (!route || route->prefix_len < 128) {
        return -1;
    }

    return 0;
}

int arm_border_router_proxy_state_update(int8_t caller_interface_id, int8_t handler_interface_id, bool status)
{
    (void)caller_interface_id;

    protocol_interface_info_entry_t *cur = protocol_stack_interface_info_get_by_id(handler_interface_id);
    if (!cur) {
        tr_error("No Interface");
        return -1;
    }

    if (status) {
        tr_debug("Border router Backhaul link ready");
    } else {
        tr_debug("Border router Backhaul link down");
    }
    return 0;
}

void arm_border_router_ready(protocol_interface_info_entry_t *cur)
{
    if (cur->border_router_setup) {
        nd_router_t *nd_nwk = cur->border_router_setup->nd_nwk;
        if (nd_nwk) {
            addr_add_router_groups(cur);
            addr_add_group(cur, ADDR_REALM_LOCAL_ALL_ROUTERS);
            icmpv6_radv_enable(cur);

            if (cur->lowpan_info & INTERFACE_NWK_BOOTSRAP_MLE) {
                nd_nwk->mle_advert_timer = 50;
            }

            nd_nwk->nd_re_validate = nd_nwk->life_time;
            nd_nwk->nd_re_validate /= 4;
            nd_nwk->nd_re_validate *= 3;
            nd_nwk->nd_timer = 10;
            nd_set_br(nd_nwk);
            icmpv6_restart_router_advertisements(cur, nd_nwk->border_router);
            rpl_control_set_domain_on_interface(cur, protocol_6lowpan_rpl_domain, true);
            cur->border_router_setup->initActive = false;

            //SET Interface Default address
            cur->global_address_available = true;
            ipv6_prefix_on_link_update(nd_nwk->border_router);

        }
        //Updates beacon contents
        beacon_join_priority_update(cur->id);

        cur->bootsrap_state_machine_cnt = 0;
        cur->nwk_bootstrap_state = ER_BOOTSRAP_DONE;
        net_load_balance_internal_state_activate(cur, true);
        nwk_bootsrap_state_update(ARM_NWK_BOOTSTRAP_READY, cur);
        nd_proxy_downstream_interface_register(cur->id, arm_border_router_proxy_validate, arm_border_router_proxy_state_update);
    }
}

static int8_t arm_border_router_interface_down(protocol_interface_info_entry_t *cur)
{
    int8_t ret_val = 0;
    nd_router_t *nd_nwk = cur->border_router_setup->nd_nwk;
    /* Change Active -> Idle */
    /* Disable Protocols Timers */
    tr_info("BR-->IDLE");
    neighbor_cache_flush(&cur->neigh_cache);

    mlme_reset_t reset;
    reset.SetDefaultPIB = true;
    cur->mac_api->mlme_req(cur->mac_api, MLME_RESET, &reset);

    net_load_balance_internal_state_activate(cur, false);
    whiteboard_interface_unregister_all_address(cur->id);
    if (cur->nwk_wpan_nvm_api) {
        cur->nwk_wpan_nvm_api->nvm_params_update_cb(cur->nwk_wpan_nvm_api, true);
    }
    cur->if_lowpan_security_params->mle_security_frame_counter = mle_service_security_get_frame_counter(cur->id);
#ifndef NO_MLE
    mle_class_list_clean(cur->id);
    blacklist_clear();
#endif
    if (nd_nwk) {
        ipv6_prefix_on_link_remove(nd_nwk->border_router);
        icmp_nd_border_router_release(nd_nwk);
    }

    //MAC
    mac_helper_link_frame_counter_set(cur->id, 0);
    mac_helper_panid_set(cur, 0xffff);
    mac_helper_mac16_address_set(cur, 0xffff);
    /* Init RPL Timers */

    cur->lowpan_info &= ~INTERFACE_NWK_ACTIVE;
    mac_helper_default_security_level_set(cur, SEC_NONE);
    //Reset WhiteBoard
    whiteboard_init(cur->id);
    lowpan_adaptation_interface_reset(cur->id);
    reassembly_interface_reset(cur->id);
    protocol_core_interface_info_reset(cur);
    cur->bootsrap_state_machine_cnt = 0;
    cur->interface_mode = INTERFACE_IDLE;
    nd_proxy_downstream_interface_unregister(cur->id);
    return ret_val;
}

int8_t arm_nwk_6lowpan_borderrouter_init(protocol_interface_info_entry_t *cur)
{
    /* Border Router Allocate entry */
    if (cur->border_router_setup == 0) {
        cur->border_router_setup = ns_dyn_mem_alloc(sizeof(br_info_t));
        if (!cur->border_router_setup) {
            return -1;

        }
        cur->border_router_setup->nd_nwk = ns_dyn_mem_alloc(sizeof(nd_router_t));
        cur->border_router_setup->nd_border_router_configure = ns_dyn_mem_alloc(sizeof(nd_router_setup_t));

        if (cur->border_router_setup->nd_nwk == 0 || cur->border_router_setup->nd_border_router_configure == 0) {
            arm_nwk_6lowpan_borderrouter_data_free(cur);
        }
        if (cur->border_router_setup) {
            cur->border_router_setup->mac_short_adr = 0xffff;
            cur->border_router_setup->mac_panid = 0xffff;
            if (cur->border_router_setup->nd_border_router_configure) {
                ns_list_init(&cur->border_router_setup->nd_border_router_configure->prefix_list);
                ns_list_init(&cur->border_router_setup->nd_border_router_configure->context_list);
            }
        }

    } else {
        if (cur->border_router_setup->nd_nwk) {
            //Clear Setup
            nd_router_t *router_object = cur->border_router_setup->nd_nwk;
            /**
             * Reset Current ND Setup
             */
            icmp_nd_router_object_reset(router_object);
        }
    }
    whiteboard_init(cur->id);
    cur->bootsrap_mode = ARM_NWK_BOOTSRAP_MODE_6LoWPAN_BORDER_ROUTER;
    cur->lowpan_info |= INTERFACE_NWK_ROUTER_DEVICE;
    cur->lowpan_info &= ~INTERFACE_NWK_CONF_MAC_RX_OFF_IDLE;
    cur->if_up = arm_border_router_interface_up;
    cur->if_down = arm_border_router_interface_down;
    return 0;
}

#else

int8_t arm_nwk_6lowpan_border_router_init(int8_t interface_id, const border_router_setup_s *border_router_setup_ptr) {
    (void) interface_id;
    (void) border_router_setup_ptr;
    return -3;
}

int8_t arm_nwk_6lowpan_border_router_context_parameter_update(int8_t interface_id, uint8_t c_id,
        uint8_t compress_mode, uint16_t ttl)
{
    (void) interface_id;
    (void) c_id;
    (void) compress_mode;
    (void) ttl;
    return -1;
}

int8_t arm_nwk_6lowpan_border_router_context_remove_by_id(int8_t interface_id, uint8_t c_id)
{
    (void) interface_id;
    (void) c_id;
    return -1;
}

int8_t arm_nwk_6lowpan_border_router_configure_push(int8_t interface_id)
{
    (void) interface_id;
    return -1;
}

int8_t arm_nwk_6lowpan_border_router_nd_context_load(int8_t interface_id, uint8_t *contex_data) {
    (void) interface_id;
    (void) contex_data;
    return -1;
}

int8_t arm_nwk_6lowpan_border_router_context_update(int8_t interface_id, uint8_t c_id_flags, uint8_t context_len, uint16_t ttl, const uint8_t *context_ptr) {
    (void) interface_id;
    (void) c_id_flags;
    (void) context_len;
    (void) ttl;
    (void) context_ptr;
    return -1;
}

int8_t arm_nwk_6lowpan_border_route_nd_default_prefix_timeout_set(int8_t interface_id, uint32_t time) {
    (void) interface_id;
    (void) time;
    return -1;
}

#endif
