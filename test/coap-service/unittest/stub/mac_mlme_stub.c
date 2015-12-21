/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#include <string.h>
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "MAC/IEEE802_15_4/mac_data_interface.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "platform/arm_hal_interrupt.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_bootstrap.h"
#include "common_functions.h"

int8_t mac_mlme_scan_requet(mac_scan_type_t type, protocol_interface_info_entry_t *interface_ptr)
{
    return -1;
}

void mac_mlme_energy_scan(void (*passed_fptr)(uint8_t *), protocol_interface_info_entry_t *cur)
{
}

int8_t mac_mlme_active_scan_req(struct protocol_interface_info_entry *cur, channel_list_s *scan_list, void (*passed_fptr)(uint8_t *))
{
    return -1;
}

void mac_mlme_active_scan(void (*passed_fptr)(uint8_t *), protocol_interface_info_entry_t *cur)
{
}

void mac_mlme_free_scan_confirm(nwk_scan_params_s *params)
{
}


void mac_mlme_start_req(mlme_start_t *s, protocol_interface_info_entry_t *cur)
{
}

void mac_mlme_cmd_frame_handle(buffer_t *buf)
{
}

void mac_mlme_active_scan_response_timer_start(void *interface)
{
}

void mac_mlme_timers_disbale(protocol_interface_rf_mac_setup_s *rf_ptr)
{
}

void mac_mlme_message_handler(arm_event_s *event)
{
}

void mac_mlme_event_cb(uint8_t event)
{
}

void mac_mlme_timer_cb(int8_t timer_id, uint16_t slots)
{
}

void mac_mlme_free_sacn_temporary_data(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
}

nwk_pan_descriptor_t *mac_mlme_free_pan_descriptions(nwk_pan_descriptor_t *nwk_cur_active)
{
    return NULL;
}

int mac_mlme_data_base_allocate(protocol_interface_info_entry_t *entry)
{
    return -1;
}

int8_t mac_mlme_nwk_id_filter_set(const uint8_t *nw_id, nwk_filter_params_s *filter)
{
    return -1;
}

uint8_t mac_mlme_set_new_sqn(protocol_interface_rf_mac_setup_s *rf_setup)
{
    return 0;
}

int8_t mac_mlme_set_panid(protocol_interface_info_entry_t *cur, uint16_t pan_id)
{
    return -1;
}

int8_t mac_mlme_set_mac16(protocol_interface_info_entry_t *cur, uint16_t mac16)
{
    return -1;
}

int8_t mac_mlme_reset_mac16(protocol_interface_info_entry_t *cur)
{
    return -1;
}

void mac_mlme_write_mac64(protocol_interface_info_entry_t *cur, uint8_t *addrPtr)
{
}

bool mac_mlme_write_mac16(struct protocol_interface_rf_mac_setup *rf_setup, uint8_t *addrPtr)
{
    return false;
}

bool mac_mlme_write_panid(struct protocol_interface_rf_mac_setup *rf_setup, uint8_t *addrPtr)
{
    return false;
}

uint16_t mac_mlme_get_panid(protocol_interface_rf_mac_setup_s *rf_setup)
{
    return 0;
}

uint16_t mac_mlme_get_mac16(protocol_interface_rf_mac_setup_s *rf_setup)
{
    return 0;
}

void mac_mlme_soft_set_mac16(protocol_interface_rf_mac_setup_s *rf_setup, uint16_t shotAddress)
{
}

bool mac_mlme_write_our_addr(struct protocol_interface_info_entry *cur, sockaddr_t *sockaddr)
{
    return false;
}


void mac_mlme_write_mac64_by_interface_type(nwk_interface_id nwk_id, uint8_t *addrPtr)
{
}

int mac_mlme_beacon_notify(nwk_pan_descriptor_t *description, struct nwk_scan_params *scan_params)
{
}

uint8_t *mac_mlme_beacon_payload_allocate(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t len)
{
    return 0;
}

uint8_t *mac_mlme_beacon_palyaod_ptr_get(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    return NULL;
}

nwk_pan_descriptor_t *mac_mlme_select_best_lqi(nwk_pan_descriptor_t *list)
{
    return NULL;
}

void mac_mlme_drop_selected_from_the_scanresult(nwk_scan_params_s *scanParams, nwk_pan_descriptor_t *selected)
{
}

void mac_mlme_scan_confirmation_handle(mlme_scan_conf_t *r, protocol_interface_info_entry_t *cur_interface)
{
}

void mac_mlme_mac_halt(protocol_interface_info_entry_t *cur)
{
}

void mac_mle_mac_radio_enable(protocol_interface_info_entry_t *cur)
{
}

void mac_mlme_mac_wake_up(protocol_interface_info_entry_t *cur)
{
}


int8_t mac_mlme_rf_disable(protocol_interface_info_entry_t *cur)
{
    return -1;
}

void mac_mlme_rf_receiver_enable(protocol_interface_info_entry_t *cur)
{
}

int8_t mac_mlme_rf_channel_set(protocol_interface_info_entry_t *cur, uint8_t new_channel)
{
    return -1;
}

int8_t mac_mlme_set_cordinator_address(protocol_interface_rf_mac_setup_s *rf_mac_setup, addrtype_t adr_type, uint8_t *adr_ptr)
{
    return -1;
}

int8_t mac_mlme_get_cordinator_address(protocol_interface_rf_mac_setup_s *rf_mac_setup, addrtype_t *adr_type, uint8_t *adr_ptr)
{
    return -1;
}

nwk_scan_params_s *mac_mlme_get_scan_params(protocol_interface_info_entry_t *interface)
{
    return NULL;
}
