/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

#include <inttypes.h>
#include "NWK_INTERFACE/Include/protocol.h"
#include "MAC/IEEE802_15_4/mac_data_poll.h"



nwk_rfd_poll_setups_s *mac_data_poll_get_polling_info(struct protocol_interface_info_entry *cur)
{
    return NULL;
}

void mac_data_poll_polltimer_set(struct protocol_interface_rf_mac_setup *rf_mac_setup, uint32_t slots)
{

}

void mac_data_poll_init_protocol_poll(struct protocol_interface_info_entry *cur, uint8_t set_protocol_poll_active)
{

}

uint32_t mac_data_poll_host_poll_time_max(struct protocol_interface_info_entry *cur)
{
    return 0;
}

void mac_data_poll_protocol_poll_mode_decrement(struct protocol_interface_info_entry *cur)
{

}

void mac_data_poll_protocol_poll_mode_disable(struct protocol_interface_info_entry *cur, uint8_t re_req)
{

}

void mac_data_poll_enable_check(struct protocol_interface_info_entry *cur)
{

}

int8_t mac_data_poll_host_mode_get(struct protocol_interface_info_entry *cur, net_host_mode_t *mode)
{
    return 0;
}

void mac_data_poll_poll_done(void *cur, mac_event_t result, uint8_t *addr, addrtype_t type)
{

}

void mac_data_poll_cb_run(struct protocol_interface_info_entry *cur)
{

}

int8_t mac_data_poll_host_mode_set(struct protocol_interface_info_entry *cur, net_host_mode_t mode, uint32_t poll_time)
{
    return 0;
}

void mac_data_poll_init(struct protocol_interface_info_entry *cur)
{

}

void mac_data_poll_radio_disable_check(struct protocol_interface_info_entry *cur)
{

}

uint32_t mac_data_poll_get_max_sleep_period(struct protocol_interface_info_entry *cur)
{
    return 0;
}

uint32_t mac_data_poll_host_timeout(protocol_interface_info_entry_t *cur)
{
    return 0;
}
