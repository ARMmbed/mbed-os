/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 */

#include "config.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "string.h"
#include "ns_trace.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "ccmLIB.h"
#include "MAC/IEEE802_15_4/mac_security_interface.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "common_functions.h"
#include "6LoWPAN/Thread/thread.h"
#include "6LoWPAN/Thread/thread_management_internal.h"
#ifndef NO_MLE
#include "MLE/mle.h"
#endif

#define TRACE_GROUP_MAC_SECURITY "mSec"

int8_t mac_security_interface_link_frame_counter_reset(int8_t interface_id)
{
    return -1;
}

int8_t mac_security_interface_link_security_level_set(int8_t interface_id , uint8_t sec_level)
{
    return -1;
}
uint8_t mac_security_interface_link_security_level_get_by_ptr(int8_t interface_id)
{
    return 0;
}

int8_t mac_security_interface_link_frame_conter_read(int8_t interface_id, uint32_t *seq_ptr, uint8_t update_to_next)
{
    return -1;
}

int8_t mac_securityinterface_trig_pending_key(int8_t interface_id)
{
    return -1;
}

uint8_t *mac_security_interface_link_security_key_get_by_id(int8_t interface_id, uint8_t key_id)
{
    return NULL;
}

int8_t mac_security_interface_key_update_trig(int8_t interface_id)
{
    return -1;
}

int8_t mac_security_interface_mac_key_update(int8_t interface_id, uint8_t *key, uint8_t id, bool primary, uint8_t auth_cnt)
{
    return -1;
}

uint8_t mac_security_interface_security_aux_header_len(int8_t interface_id)
{
    return 0;
}

uint8_t mac_security_interface_current_security_key_id_get(int8_t interface_id)
{
    return 0;
}

int8_t mac_security_interface_security_key_reset(int8_t interface_id)
{
    return -1;
}

uint8_t mac_security_interface_aux_security_header_size(uint8_t keyIdMode)
{
    return 0;
}

void mac_security_interface_aux_ccm_nonce_set(uint8_t *noncePtr, uint8_t *mac64, uint32_t securityFrameCounter, uint8_t securityLevel)
{
}

uint8_t *mac_security_interface_aux_security_header_write(uint8_t *ptr, const mac_aux_security_header_t *auxHeader)
{
    return NULL;
}

/* Compare two security levels, as per 802.15.4-2011 7.4.1.1 */
/* Returns true if sec1 is at least as secure as sec2 */
bool mac_security_interface_security_level_compare(uint8_t sec1, uint8_t sec2)
{
    return false;
}

buffer_t *mac_security_interface_aux_header_parse(buffer_t *b, mac_aux_security_header_t *auxSecHeader)
{
    return NULL;
}

uint8_t mac_security_interface_link_security_level_get(int8_t interface_id)
{
    return 0;
}
