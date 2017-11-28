/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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

#define HAVE_RPL
#include <string.h>
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"

#include "Core/include/ns_buffer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_stats.h"
#include "Common_Protocols/ipv6_constants.h"
#include "Common_Protocols/icmpv6.h"

#include "Service_Libs/etx/etx.h" /* slight ick */

#include "net_rpl.h"
#include "RPL/rpl_protocol.h"
#include "RPL/rpl_upward.h"
#include "RPL/rpl_downward.h"
#include "RPL/rpl_policy.h"
#include "RPL/rpl_control.h"

void rpl_control_set_memory_limits(size_t soft_limit, size_t hard_limit)
{

}

void *rpl_realloc(void *p, uint16_t old_size, uint16_t new_size)
{
    return NULL;
}

void *rpl_alloc(uint16_t size)
{
    return NULL;
}

void rpl_free(void *p, uint16_t size)
{
}



void rpl_control_transmit_multicast_dio(struct rpl_domain *domain, struct rpl_instance *instance, uint8_t instance_id, uint8_t dodag_version, uint16_t rank, uint8_t g_mop_prf, uint8_t dtsn, const uint8_t dodagid[16], const struct rpl_dodag_conf *conf)
{

}

void rpl_control_event(struct rpl_domain *domain, rpl_event_t event)
{
}

void rpl_control_publish_host_address(rpl_domain_t *domain, const uint8_t addr[16], uint32_t lifetime)
{

}

void rpl_control_unpublish_address(rpl_domain_t *domain, const uint8_t addr[16])
{

}

rpl_domain_t *rpl_control_create_domain(void)
{
    return NULL;
}

void rpl_control_delete_domain(rpl_domain_t *domain)
{

}

void rpl_control_set_domain_on_interface(protocol_interface_info_entry_t *cur, rpl_domain_t *domain, bool downstream)
{

}

void rpl_control_remove_domain_from_interface(protocol_interface_info_entry_t *cur)
{

}

void rpl_control_set_callback(rpl_domain_t *domain, rpl_domain_callback_t callback, void *cb_handle)
{

}

void rpl_control_disable_ra_routes(struct rpl_domain *domain)
{

}

bool rpl_control_have_dodag(rpl_domain_t *domain)
{
    return false;
}

rpl_dodag_t *rpl_control_create_dodag_root(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid, const rpl_dodag_conf_t *conf, uint16_t rank, uint8_t g_mop_prf)
{
    return NULL;
}

void rpl_control_delete_dodag_root(rpl_domain_t *domain, rpl_dodag_t *dodag)
{

}

void rpl_control_update_dodag_route(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, bool age)
{

}

void rpl_control_update_dodag_prefix(rpl_dodag_t *dodag, const uint8_t *prefix, uint8_t prefix_len, uint8_t flags, uint32_t lifetime, uint32_t preftime, bool age)
{

}

void rpl_control_increment_dtsn(rpl_dodag_t *dodag)
{

}

void rpl_control_increment_dodag_version(rpl_dodag_t *dodag)
{

}

void rpl_control_set_dodag_pref(rpl_dodag_t *dodag, uint8_t pref)
{

}

void rpl_control_poison(rpl_domain_t *domain, uint8_t count)
{

}

void rpl_control_force_leaf(rpl_domain_t *domain, bool leaf)
{

}

void rpl_control_transmit(struct rpl_domain *domain, struct protocol_interface_info_entry *cur, uint8_t code, struct buffer *buf, const uint8_t *dst)
{

}

void rpl_control_transmit_dio(struct rpl_domain *domain, struct protocol_interface_info_entry *cur, uint8_t instance_id, uint8_t dodag_version, uint16_t rank, uint8_t g_mop_prf, uint8_t dtsn, struct rpl_dodag *dodag, const uint8_t dodagid[16], const struct rpl_dodag_conf *conf, const uint8_t *dst)
{

}

void rpl_control_transmit_dis(rpl_domain_t *domain, protocol_interface_info_entry_t *cur, uint8_t pred, uint8_t instance_id, const uint8_t *dodagid, const uint8_t version, const uint8_t *dst)
{

}

bool rpl_control_transmit_dao(struct rpl_domain *domain, struct protocol_interface_info_entry *cur, struct rpl_instance *instance, uint8_t instance_id, uint8_t dao_sequence, const uint8_t dodagid[16], const uint8_t *opts, uint16_t opts_size, const uint8_t *dst)
{
    return false;
}

buffer_t *rpl_control_handler(buffer_t *buf)
{
    return NULL;
}

buffer_t *rpl_control_source_route_error_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    return NULL;
}

void rpl_control_fast_timer(uint16_t ticks)
{

}

void rpl_control_slow_timer(uint16_t seconds)
{

}

rpl_instance_t *rpl_control_enumerate_instances(rpl_domain_t *domain, rpl_instance_t *instance)
{
    return NULL;
}

rpl_instance_t *rpl_control_lookup_instance(rpl_domain_t *domain, uint8_t instance_id, const uint8_t *dodagid)
{
    return NULL;
}

bool rpl_control_read_dodag_info(const rpl_instance_t *instance, rpl_dodag_info_t *dodag_info)
{
    return false;
}

const uint8_t *rpl_control_preferred_parent_addr(const rpl_instance_t *instance, bool global)
{
    return NULL;
}

void rpl_control_print(route_print_fn_t *print_fn)
{

}
