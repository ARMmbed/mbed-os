/*
 * Copyright (c) 2015-2017, Arm Limited and affiliates.
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

#ifdef HAVE_MPL

#include "ns_types.h"
#include "ns_list.h"
#include "ns_trace.h"
#include "common_functions.h"
#include "nsdynmemLIB.h"
#include "randLIB.h"
#include <string.h>
#include "Core/include/ns_buffer.h"
#include "NWK_INTERFACE/Include/protocol.h"
#include "NWK_INTERFACE/Include/protocol_timer.h"
#include "Common_Protocols/ipv6.h"
#include "Common_Protocols/icmpv6.h"
#include "Service_Libs/Trickle/trickle.h"
#include "6LoWPAN/MAC/mac_helper.h"
#include "6LoWPAN/Thread/thread_common.h"
#include "MPL/mpl.h"

#define TRACE_GROUP "mpl"

#define MPL_OPT_S_MASK      0xC0
#define MPL_OPT_S_SHIFT     6
#define MPL_OPT_M           0x20
#define MPL_OPT_V           0x10

#define MPL_SEED_IPV6_SRC   0
#define MPL_SEED_16_BIT     1
#define MPL_SEED_64_BIT     2
#define MPL_SEED_128_BIT    3

#define MAX_BUFFERED_MESSAGES_SIZE 2048
#define MAX_BUFFERED_MESSAGE_LIFETIME 600 // 1/10 s ticks

static bool mpl_timer_running;
static uint16_t mpl_total_buffered;

const trickle_params_t rfc7731_default_data_message_trickle_params = {
    .Imin = MPL_MS_TO_TICKS(512),   /* RFC 7731 says 10 * expected link latency; ZigBee IP says 512 ms */
    .Imax = MPL_MS_TO_TICKS(512),   /* RFC 7731 says equal to Imin; ZigBee IP says 512 ms */
    .k = 1,                         /* RFC 7731 says 1; ZigBee IP says infinite */
    .TimerExpirations = 3           /* RFC 7731 says 3; ZigBee IP says 2 for routers, 0 for hosts */
};

const trickle_params_t rfc7731_default_control_message_trickle_params = {
    .Imin = MPL_MS_TO_TICKS(512),   /* RFC 7731 says 10 * worst-case link latency */
    .Imax = MPL_MS_TO_TICKS(300000),/* 5 minutes, as per RFC 7731 */
    .k = 1,
    .TimerExpirations = 10
};

/* Note that we don't use a buffer_t, to save a little RAM. We don't need
 * any of the metadata it stores...
 */
typedef struct mpl_data_message
{
    bool running;
    bool colour;
    uint32_t timestamp;
    trickle_t trickle;
    ns_list_link_t link;
    uint16_t mpl_opt_data_offset;   /* offset to option data of MPL option */
    uint8_t message[];
} mpl_buffered_message_t;

typedef struct mpl_seed
{
    ns_list_link_t link;
    bool colour;
    uint16_t lifetime;
    uint8_t min_sequence;
    uint8_t id_len;
    NS_LIST_HEAD(mpl_buffered_message_t, link) messages; /* sequence number order */
    uint8_t id[];
} mpl_seed_t;

/* For simplicity, we assume each MPL domain is on exactly 1 interface */
struct mpl_domain
{
    protocol_interface_info_entry_t *interface;
    uint8_t address[16];
    uint8_t sequence;
    bool colour;
    bool proactive_forwarding;
    uint16_t seed_set_entry_lifetime;
    NS_LIST_HEAD(mpl_seed_t, link) seeds;
    trickle_t trickle;                      // Control timer
    trickle_params_t data_trickle_params;
    trickle_params_t control_trickle_params;
    ns_list_link_t link;
    multicast_mpl_seed_id_mode_e seed_id_mode;
    uint8_t seed_id[];
};

static NS_LIST_DEFINE(mpl_domains, mpl_domain_t, link);

static void mpl_buffer_delete(mpl_seed_t *seed, mpl_buffered_message_t *message);
static void mpl_control_reset_or_start(mpl_domain_t *domain);
static void mpl_schedule_timer(void);
static void mpl_fast_timer(uint16_t ticks);
static buffer_t *mpl_exthdr_provider(buffer_t *buf, ipv6_exthdr_stage_t stage, int16_t *result);
static void mpl_seed_delete(mpl_domain_t *domain, mpl_seed_t *seed);

static bool mpl_initted;

static void mpl_init(void)
{
    if (mpl_initted) {
        return;
    }
    mpl_initted = true;

    ipv6_set_exthdr_provider(ROUTE_MPL, mpl_exthdr_provider);
}

static uint8_t mpl_buffer_sequence(const mpl_buffered_message_t *message)
{
    return message->message[message->mpl_opt_data_offset + 1];
}

static uint16_t mpl_buffer_size(const mpl_buffered_message_t *message)
{
    return IPV6_HDRLEN + common_read_16_bit(message->message + IPV6_HDROFF_PAYLOAD_LENGTH);
}

mpl_domain_t *mpl_domain_lookup(protocol_interface_info_entry_t *cur, const uint8_t address[16])
{
    ns_list_foreach(mpl_domain_t, domain, &mpl_domains) {
        if (domain->interface == cur && addr_ipv6_equal(domain->address, address)) {
            return domain;
        }
    }
    return NULL;
}

mpl_domain_t *mpl_domain_lookup_with_realm_check(protocol_interface_info_entry_t *cur, const uint8_t address[16])
{
    if (!addr_is_ipv6_multicast(address)) {
        return NULL;
    }

    if (addr_ipv6_multicast_scope(address) == IPV6_SCOPE_REALM_LOCAL && cur->mpl_treat_realm_domains_as_one) {
        address = ADDR_ALL_MPL_FORWARDERS;
    }

    return mpl_domain_lookup(cur, address);
}

/* Look up domain by address, ignoring the scop field, so ff22::1 matches ff23::1 */
/* We assume all addresses are multicast, so don't bother checking the first byte */
static mpl_domain_t *mpl_domain_lookup_ignoring_scop(protocol_interface_info_entry_t *cur, const uint8_t address[16])
{
    ns_list_foreach(mpl_domain_t, domain, &mpl_domains) {
       if (domain->interface == cur &&
               memcmp(address + 2, domain->address + 2, 14) == 0 &&
               (address[1] & 0xf0) == (domain->address[1] & 0xf0)) {
           return domain;
       }
    }
    return NULL;
}

static int mpl_domain_count_on_interface(protocol_interface_info_entry_t *cur)
{
    int count = 0;
    ns_list_foreach(mpl_domain_t, domain, &mpl_domains) {
        if (domain->interface == cur) {
            count++;
        }
    }
    return count;
}

mpl_domain_t *mpl_domain_create(protocol_interface_info_entry_t *cur, const uint8_t address[16],
                                const uint8_t *seed_id, multicast_mpl_seed_id_mode_e seed_id_mode,
                                int_fast8_t proactive_forwarding,
                                uint16_t seed_set_entry_lifetime,
                                const trickle_params_t *data_trickle_params,
                                const trickle_params_t *control_trickle_params)
{
    if (!addr_is_ipv6_multicast(address) || addr_ipv6_multicast_scope(address) < IPV6_SCOPE_REALM_LOCAL) {
        return NULL;
    }

    if (addr_ipv6_multicast_scope(address) == IPV6_SCOPE_REALM_LOCAL && cur->mpl_treat_realm_domains_as_one &&
            !addr_ipv6_equal(address, ADDR_ALL_MPL_FORWARDERS)) {
        return NULL;
    }

    mpl_init();

    /* We lock out attempts to join two domains differing only by scop - this
     * is because we couldn't distinguish control messages, which are sent
     * to the link-local version of the same address. Seems to be a
     * specification limitation?
     */
    if (mpl_domain_lookup_ignoring_scop(cur, address)) {
        return NULL;
    }

    if (seed_id_mode == MULTICAST_MPL_SEED_ID_DEFAULT) {
        seed_id_mode = cur->mpl_seed_id_mode;
        seed_id = cur->mpl_seed_id;
    }

    uint8_t seed_id_len;
    if (seed_id_mode > 0) {
        seed_id_len = seed_id_mode;
    } else {
        seed_id_len = 0;
    }

    mpl_domain_t *domain = ns_dyn_mem_alloc(sizeof *domain + seed_id_len);
    if (!domain) {
        return NULL;
    }
    memcpy(domain->address, address, 16);
    domain->interface = cur;
    domain->sequence = randLIB_get_8bit();
    domain->colour = false;
    ns_list_init(&domain->seeds);
    domain->proactive_forwarding = proactive_forwarding >= 0 ? proactive_forwarding
                                                             : cur->mpl_proactive_forwarding;
    domain->seed_set_entry_lifetime = seed_set_entry_lifetime ? seed_set_entry_lifetime
                                                              : cur->mpl_seed_set_entry_lifetime;
    domain->data_trickle_params = data_trickle_params ? *data_trickle_params
                                                      : cur->mpl_data_trickle_params;
    domain->control_trickle_params = control_trickle_params ? *control_trickle_params
                                                            : cur->mpl_control_trickle_params;
    trickle_start(&domain->trickle, &domain->control_trickle_params);
    trickle_stop(&domain->trickle);
    domain->seed_id_mode = seed_id_mode;
    memcpy(domain->seed_id, seed_id, seed_id_len);
    ns_list_add_to_end(&mpl_domains, domain);

    //ipv6_route_add_with_info(address, 128, cur->id, NULL, ROUTE_MPL, domain, 0, 0xffffffff, 0);
    addr_add_group(cur, address);
    if (domain->control_trickle_params.TimerExpirations != 0) {
        uint8_t ll_scope[16];
        memcpy(ll_scope, address, 16);
        ll_scope[1] = (ll_scope[1] & 0xf0) | IPV6_SCOPE_LINK_LOCAL;
        addr_add_group(cur, ll_scope);
    }

    /* If we just created the first domain on an interface, auto-create the all-forwarders domain (this does nothing if we're already a member) */
    if (mpl_domain_count_on_interface(cur) == 1) {
        /* Use default interface parameters */
        mpl_domain_create(cur, ADDR_ALL_MPL_FORWARDERS, NULL, MULTICAST_MPL_SEED_ID_DEFAULT, -1, 0, NULL, NULL);
        cur->mpl_seed = true;
    }

    return domain;
}

bool mpl_domain_delete(protocol_interface_info_entry_t *cur, const uint8_t address[16])
{
    mpl_domain_t *domain = mpl_domain_lookup(cur, address);
    if (!domain) {
        return false;
    }
    int count = mpl_domain_count_on_interface(cur);

    /* Don't let them delete all-mpl-forwarders unless it's the last */
    if (addr_ipv6_equal(address, ADDR_ALL_MPL_FORWARDERS)) {
        if (count != 1) {
            return true;
        }
        cur->mpl_seed = false;
    }

    ns_list_foreach_safe(mpl_seed_t, seed, &domain->seeds) {
        mpl_seed_delete(domain, seed);
    }

    //ipv6_route_delete(address, 128, cur->id, NULL, ROUTE_MPL);
    addr_delete_group(cur, address);
    if (domain->control_trickle_params.TimerExpirations != 0) {
        uint8_t ll_scope[16];
        memcpy(ll_scope, domain->address, 16);
        ll_scope[1] = (ll_scope[1] & 0xf0) | IPV6_SCOPE_LINK_LOCAL;
        addr_delete_group(cur, ll_scope);
    }
    ns_list_remove(&mpl_domains, domain);
    ns_dyn_mem_free(domain);
    return true;
}

void mpl_domain_change_timing(mpl_domain_t *domain, const struct trickle_params *data_trickle_params, uint16_t seed_set_entry_lifetime)
{
    domain->data_trickle_params = *data_trickle_params;
    domain->seed_set_entry_lifetime = seed_set_entry_lifetime;
}

static void mpl_domain_inconsistent(mpl_domain_t *domain)
{
    trickle_inconsistent_heard(&domain->trickle, &domain->control_trickle_params);
    mpl_schedule_timer();
}

static mpl_seed_t *mpl_seed_lookup(const mpl_domain_t *domain, uint8_t id_len, const uint8_t *seed_id)
{
    ns_list_foreach(mpl_seed_t, seed, &domain->seeds) {
        if (seed->id_len == id_len && memcmp(seed->id, seed_id, id_len) == 0) {
            return seed;
        }
    }

    return NULL;
}

static mpl_seed_t *mpl_seed_create(mpl_domain_t *domain, uint8_t id_len, const uint8_t *seed_id, uint8_t sequence)
{
    mpl_seed_t *seed = ns_dyn_mem_alloc(sizeof(mpl_seed_t) + id_len);
    if (!seed) {
        return NULL;
    }

    seed->min_sequence = sequence;
    seed->lifetime = domain->seed_set_entry_lifetime;
    seed->id_len = id_len;
    seed->colour = domain->colour;
    ns_list_init(&seed->messages);
    memcpy(seed->id, seed_id, id_len);
    ns_list_add_to_end(&domain->seeds, seed);
    return seed;
}

static void mpl_seed_delete(mpl_domain_t *domain, mpl_seed_t *seed)
{
    ns_list_foreach_safe(mpl_buffered_message_t, message, &seed->messages) {
        mpl_buffer_delete(seed, message);
    }
    ns_list_remove(&domain->seeds, seed);
    ns_dyn_mem_free(seed);
}

static void mpl_seed_advance_min_sequence(mpl_seed_t *seed, uint8_t min_sequence)
{
    seed->min_sequence = min_sequence;
    ns_list_foreach_safe(mpl_buffered_message_t, message, &seed->messages) {
        if (common_serial_number_greater_8(min_sequence, mpl_buffer_sequence(message))) {
            mpl_buffer_delete(seed, message);
        }
    }
}

static mpl_buffered_message_t *mpl_buffer_lookup(mpl_seed_t *seed, uint8_t sequence)
{
    ns_list_foreach(mpl_buffered_message_t, message, &seed->messages) {
        if (mpl_buffer_sequence(message) == sequence) {
            return message;
        }
    }
    return NULL;
}

static void mpl_free_space(void)
{
    mpl_seed_t *oldest_seed = NULL;
    mpl_buffered_message_t *oldest_message = NULL;

    /* We'll free one message - earliest sequence number from one seed */
    /* Choose which seed by looking at the timestamp - oldest one first */
    ns_list_foreach(mpl_domain_t, domain, &mpl_domains) {
        ns_list_foreach(mpl_seed_t, seed, &domain->seeds) {
            mpl_buffered_message_t *message = ns_list_get_first(&seed->messages);
            if (!message) {
                continue;
            }
            if (!oldest_message ||
                    protocol_core_monotonic_time - message->timestamp > protocol_core_monotonic_time - oldest_message->timestamp) {
                oldest_message = message;
                oldest_seed = seed;
            }
        }
    }

    if (!oldest_message) {
        return;
    }

    oldest_seed->min_sequence = mpl_buffer_sequence(oldest_message) + 1;
    mpl_buffer_delete(oldest_seed, oldest_message);
}


static mpl_buffered_message_t *mpl_buffer_create(buffer_t *buf, mpl_domain_t *domain, mpl_seed_t *seed, uint8_t sequence, uint8_t hop_limit)
{
    /* IP layer ensures buffer length == IP length */
    uint16_t ip_len = buffer_data_length(buf);

    while (mpl_total_buffered + ip_len > MAX_BUFFERED_MESSAGES_SIZE) {
        mpl_free_space();
    }

    /* As we came in, message sequence was >= min_sequence, but mpl_free_space
     * could end up pushing min_sequence forward. We must take care and
     * re-check min_sequence.
     *
     * For example, let's say min_sequence=1, we're holding 1,3,5, and we receive 2.
     * a) If mpl_free_space doesn't touch this seed, we're fine.
     * b) If it frees 1, it will advance min_sequence to 2, and we're fine.
     * c) If it frees 1 and 3, it will advance min_sequence to 4, and we cannot
     *    accept this message. (If we forced min_sequence to 2, we'd end up processing
     *    message 3 again).
     */
    if (common_serial_number_greater_8(seed->min_sequence, sequence)) {
        tr_debug("Can no longer accept %"PRIu8" < %"PRIu8, sequence, seed->min_sequence);
        return NULL;
    }

    mpl_buffered_message_t *message = ns_dyn_mem_alloc(sizeof(mpl_buffered_message_t) + ip_len);
    if (!message) {
        return NULL;
    }
    memcpy(message->message, buffer_data_pointer(buf), ip_len);
    message->message[IPV6_HDROFF_HOP_LIMIT] = hop_limit;
    message->mpl_opt_data_offset = buf->mpl_option_data_offset;
    message->colour = seed->colour;
    message->timestamp = protocol_core_monotonic_time;
    /* Make sure trickle structure is initialised */
    trickle_start(&message->trickle, &domain->data_trickle_params);
    if (domain->proactive_forwarding) {
        mpl_schedule_timer();
    } else {
        /* Then stop it if not proactive */
        trickle_stop(&message->trickle);
    }

    /* Messages held ordered - eg for benefit of mpl_seed_bm_len() */
    bool inserted = false;
    ns_list_foreach_reverse(mpl_buffered_message_t, m, &seed->messages) {
        if (common_serial_number_greater_8(sequence, mpl_buffer_sequence(m))) {
            ns_list_add_after(&seed->messages, m, message);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        ns_list_add_to_start(&seed->messages, message);
    }
    mpl_total_buffered += ip_len;

    /* Does MPL spec intend this distinction between start and reset? */
    mpl_control_reset_or_start(domain);

    return message;
}

static void mpl_buffer_delete(mpl_seed_t *seed, mpl_buffered_message_t *message)
{
    mpl_total_buffered -= mpl_buffer_size(message);
    ns_list_remove(&seed->messages, message);
    ns_dyn_mem_free(message);
}

static void mpl_buffer_transmit(mpl_domain_t *domain, mpl_buffered_message_t *message, bool newest)
{
    uint16_t ip_len = mpl_buffer_size(message);
    buffer_t *buf = buffer_get(ip_len);
    if (!buf) {
        return;
    }

    buffer_data_add(buf, message->message, ip_len);

    /* Modify the M flag [Thread says it must be clear] */
    uint8_t *flag = buffer_data_pointer(buf) + message->mpl_opt_data_offset;
    if (newest && !thread_info(domain->interface)) {
        *flag |= MPL_OPT_M;
    } else {
        *flag &= ~MPL_OPT_M;
    }

    // Make sure ip_routed_up is set, even on locally-seeded packets, to
    // distinguishes the "forwarded" copies from the original seed.
    // Used to suppress extra copies to sleepy children.
    buf->ip_routed_up = true;
    buf->dst_sa.addr_type = ADDR_IPV6;
    buf->src_sa.addr_type = ADDR_IPV6;
    memcpy(buf->dst_sa.address, message->message + IPV6_HDROFF_DST_ADDR, 16);
    memcpy(buf->src_sa.address, message->message + IPV6_HDROFF_SRC_ADDR, 16);

    ipv6_transmit_multicast_on_interface(buf, domain->interface);
}

static void mpl_buffer_inconsistent(const mpl_domain_t *domain, mpl_buffered_message_t *message)
{
    trickle_inconsistent_heard(&message->trickle, &domain->data_trickle_params);
    mpl_schedule_timer();
}

static uint8_t mpl_seed_bm_len(const mpl_seed_t *seed)
{
    mpl_buffered_message_t *last = ns_list_get_last(&seed->messages);
    if (last) {
        return ((uint8_t) (mpl_buffer_sequence(last) - seed->min_sequence)) / 8 + 1;
    } else {
        return 0;
    }
}

/* Attempt to optimise by saying ID is source IPv6 */
static uint16_t mpl_seed_info_size(const mpl_seed_t *seed, const uint8_t *src)
{
    uint8_t id_len = seed->id_len;
    if (id_len == 16 && src && addr_ipv6_equal(src, seed->id)) {
        id_len = 0;
    }
    return 2 + id_len + mpl_seed_bm_len(seed);
}

static uint8_t *mpl_write_seed_info(uint8_t *ptr, const mpl_seed_t *seed, const uint8_t *src)
{
    uint8_t bm_len = mpl_seed_bm_len(seed);
    ptr[0] = seed->min_sequence;
    ptr[1] = bm_len << 2;
    uint8_t id_len = seed->id_len;
    if (id_len == 16 && src && addr_ipv6_equal(src, seed->id)) {
        id_len = 0;
    }
    switch (id_len) {
        case  0: ptr[1] |= MPL_SEED_IPV6_SRC; break;
        case  2: ptr[1] |= MPL_SEED_16_BIT; break;
        case  8: ptr[1] |= MPL_SEED_64_BIT; break;
        case 16: ptr[1] |= MPL_SEED_128_BIT; break;
        default: return ptr;
    }
    ptr += 2;
    memcpy(ptr, seed->id, id_len);
    ptr += id_len;
    memset(ptr, 0, bm_len);
    ns_list_foreach(mpl_buffered_message_t, buffer, &seed->messages) {
        uint8_t i = mpl_buffer_sequence(buffer) - seed->min_sequence;
        bit_set(ptr, i);
    }
    ptr += bm_len;
    return ptr;
}

/* Does MPL spec really intend this distinction between start and reset? */
/* (Reset sets interval to Imin, Start puts it somewhere random between Imin and Imax) */
static void mpl_control_reset_or_start(mpl_domain_t *domain)
{
    if (trickle_running(&domain->trickle, &domain->control_trickle_params)) {
        trickle_inconsistent_heard(&domain->trickle, &domain->control_trickle_params);
    } else {
        trickle_start(&domain->trickle, &domain->control_trickle_params);
    }
    mpl_schedule_timer();
}

static uint8_t mpl_seed_id_len(uint8_t seed_id_type)
{
    static const uint8_t len[] = {
        [MPL_SEED_IPV6_SRC] = 0,
        [MPL_SEED_16_BIT] = 2,
        [MPL_SEED_64_BIT] = 8,
        [MPL_SEED_128_BIT] = 16
    };
    return len[seed_id_type];
}

static uint8_t mpl_seed_id_type(uint8_t seed_id_len)
{
    switch (seed_id_len) {
        default: return MPL_SEED_IPV6_SRC;
        case 2: return MPL_SEED_16_BIT;
        case 8: return MPL_SEED_64_BIT;
        case 16: return MPL_SEED_128_BIT;
    }
}

/*
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |   min-seqno   |  bm-len   | S |   seed-id (0/2/8/16 octets)   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  .            buffered-mpl-messages (variable length)            .
 *  .                                                               .
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
static void mpl_send_control(mpl_domain_t *domain)
{
    uint16_t size = 0;
    const uint8_t *src = NULL;

    ns_list_foreach(mpl_seed_t, seed, &domain->seeds) {
        /* If not chosen yet, pick source to match a seed id, to save 16 bytes */
        if (!src && seed->id_len == 16 && addr_is_assigned_to_interface(domain->interface, seed->id)) {
            src = seed->id;
        }
        size += mpl_seed_info_size(seed, src);
    }
    buffer_t *buf = buffer_get(size);
    if (!buf) {
        return;
    }
    uint8_t *ptr = buffer_data_pointer(buf);
    ns_list_foreach(mpl_seed_t, seed, &domain->seeds) {
        ptr = mpl_write_seed_info(ptr, seed, src);
    }
    buffer_data_end_set(buf, ptr);
    buf->options.type = ICMPV6_TYPE_INFO_MPL_CONTROL;
    buf->options.code = 0;
    buf->options.hop_limit = 255;
    memcpy(buf->dst_sa.address, domain->address, 16);
    buf->dst_sa.address[1] = (buf->dst_sa.address[1] & 0xf0) | IPV6_SCOPE_LINK_LOCAL;
    buf->dst_sa.addr_type = ADDR_IPV6;
    if (src) {
        buf->src_sa.addr_type = ADDR_IPV6;
        memcpy(buf->src_sa.address, src, 16);
    }
    buf->info = (buffer_info_t)(B_FROM_ICMP | B_TO_ICMP | B_DIR_DOWN);
    buf->interface = domain->interface;
    protocol_push(buf);
}

/*
 * There is an edge case in control handling when the hop limit runs out. This
 * is handled as follows:
 *
 *          Hop Limit 2                Hop Limit 1                [Won't Forward]
 *    Seed ---------------> Forwarder -------------> Final Node  -------X------> Neighbour Node
 * In Message Set        In Message Set           MinSequence advanced        Not In Message Set
 *
 * The Final Node does NOT add the message to its buffered message set, and it
 * advances MinSequence so that doesn't have to report about the message either
 * positively or negatively in control messages.
 *
 * If it reported "present" in control messages, the Neighbour Node would see a "missing"
 * message and reset its control timer. If it reported "absent", the Forwarder would
 * notice the inconsistency and resend. So we sidestep the issue by advancing MinSequence.
 * This also saves RAM - we'd never retransmit the message anyway, so why buffer it?
 *
 * This means we drop out-of-order packets at the edge of a hop limit boundary,
 * but this isn't a huge deal.
 */
buffer_t *mpl_control_handler(buffer_t *buf, protocol_interface_info_entry_t *cur)
{
    if (!addr_is_ipv6_multicast(buf->dst_sa.address) || addr_ipv6_multicast_scope(buf->dst_sa.address) != IPV6_SCOPE_LINK_LOCAL || buf->options.hop_limit != 255) {
        tr_warn("Invalid control");
        return buffer_free(buf);
    }

    /* Um, how do we distinguish between multiple domains with different scop?
     * Control messages just have the domain address with scop 2. Currently
     * deal with that by not letting users join two domains differing only in
     * scop.
     */

    mpl_domain_t *domain = mpl_domain_lookup_ignoring_scop(cur, buf->dst_sa.address);
    if (!domain) {
        return buffer_free(buf);
    }

    bool they_have_new_data = false;
    bool we_have_new_data = false;
    const uint8_t *ptr = buffer_data_pointer(buf);
    const uint8_t *end = buffer_data_end(buf);

    // All objects will currently have the same colour. The scan
    // of the control message will flip the colour of every mentioned seed
    // and data message. Then the omission of anything we have will be detected
    // by its colour not being flipped.
    // This is equivalent to having a "mentioned" flag, except we don't have
    // to have a separate "reset" loop.
    bool new_colour = --domain->colour; // smart-alec binary flip

    while (ptr < end) {
        if (end - ptr < 2) {
            tr_err("MPL control error");
            break;
        }
        uint8_t min_seqno = ptr[0];
        uint8_t bm_len = ptr[1] >> 2;
        uint8_t seed_id_type = ptr[1] & 3;
        uint8_t seed_id_len = mpl_seed_id_len(seed_id_type);
        ptr += 2;
        /* Sequence number is 8-bit, so bitmask should never be bigger than 32 bytes */
        if (bm_len > 32 || end - ptr < seed_id_len + bm_len) {
            tr_err("MPL control error");
            break;
        }
        const uint8_t *seed_id;
        if (seed_id_type == MPL_SEED_IPV6_SRC) {
            seed_id = buf->src_sa.address;
            seed_id_len = 16;
            /* Thread spec says, or at least implies, that ML16/RLOC address is
             * matched against corresponding 16-bit seed id (although
             * Thread doesn't use control messages...) */
            if (thread_addr_is_mesh_local_16(seed_id, cur)) {
                seed_id += 14;
                seed_id_len = 2;
            }
        } else {
            seed_id = ptr;
            ptr += seed_id_len;
        }
        mpl_seed_t *seed = mpl_seed_lookup(domain, seed_id_len, seed_id);
        if (!seed) {
            they_have_new_data = true;
            ptr += bm_len;
            continue;
        }

        seed->colour = new_colour;
        /* They are assumed to not be interested in messages lower than their min_seqno */
        ns_list_foreach(mpl_buffered_message_t, message, &seed->messages) {
            if (common_serial_number_greater_8(min_seqno, mpl_buffer_sequence(message))) {
                message->colour = new_colour;
            }
        }
        for (uint8_t i = 0; i / 8 < bm_len; i++) {
            if (bit_test(ptr, i)) {
                mpl_buffered_message_t *message = mpl_buffer_lookup(seed, min_seqno + i);

                if (!message && common_serial_number_greater_8(min_seqno + i, seed->min_sequence)) {
                    they_have_new_data = true;
                } else if (message) {
                    message->colour = new_colour;
                }
            }
        }
        ptr += bm_len;
    }

    /* Search for seeds or messages they haven't mentioned */
    ns_list_foreach(mpl_seed_t, seed, &domain->seeds) {
        if (seed->colour != new_colour) {
            seed->colour = new_colour;
            we_have_new_data = true;
        }
        ns_list_foreach(mpl_buffered_message_t, message, &seed->messages) {
            if (message->colour != new_colour) {
                message->colour = new_colour;
                mpl_buffer_inconsistent(domain, message);
                we_have_new_data = true;
            }
        }
    }

    if (they_have_new_data || we_have_new_data) {
        if (they_have_new_data) {
            tr_info("%s has new MPL data", trace_ipv6(buf->src_sa.address));
        }
        if (we_have_new_data) {
            tr_info("We have new MPL data for %s", trace_ipv6(buf->src_sa.address));
        }
        mpl_domain_inconsistent(domain);
    } else {
        trickle_consistent_heard(&domain->trickle);
    }


    return buffer_free(buf);
}

bool mpl_hbh_len_check(const uint8_t *opt_data, uint8_t opt_data_len)
{
    if (opt_data_len < 2) {
        return false;
    }
    if (opt_data[0] & MPL_OPT_V) {
        return true; /* No length complaint - we let "process" drop */
    }

    uint8_t seed_id_type = (opt_data[0] & MPL_OPT_S_MASK) >> MPL_OPT_S_SHIFT;
    /* Note that option is allowed to be longer - spec allows for extension
     * beyond seed-id.
     */
    if (opt_data_len < 2 + mpl_seed_id_len(seed_id_type)) {
        return false;
    }
    return true;
}

/*      0                   1                   2                   3
 *      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *                                     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                                     |  Option Type  |  Opt Data Len |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     | S |M|V|  rsv  |   sequence    |      seed-id (optional)       |
 *     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

bool mpl_process_hbh(buffer_t *buf, protocol_interface_info_entry_t *cur, uint8_t *opt_data)
{
    if ((buf->options.ip_extflags & IPEXT_HBH_MPL) || buf->options.ll_security_bypass_rx) {
        tr_warn("Bad MPL");
        return false;
    }

    /* mpl_hbh_len_check has already returned true, so know length is okay */

    /* V flag indicates incompatible new version - packets MUST be dropped */
    if (opt_data[0] & MPL_OPT_V) {
        tr_warn("MPL V!");
        return false;
    }

    mpl_domain_t *domain = mpl_domain_lookup_with_realm_check(cur, buf->dst_sa.address);
    if (!domain) {
        tr_debug("No MPL domain");
        return false;
    }

    buf->options.ip_extflags |= IPEXT_HBH_MPL;
    buf->mpl_option_data_offset = opt_data - buffer_data_pointer(buf);

    return true;
   // return mpl_forwarder_process_message(buf, domain, opt_data);
}

/* seeding is true if this is processing an outgoing message */
bool mpl_forwarder_process_message(buffer_t *buf, mpl_domain_t *domain, bool seeding)
{
    const uint8_t *opt_data = buffer_data_pointer(buf) + buf->mpl_option_data_offset;
    uint8_t sequence = opt_data[1];
    uint8_t seed_id_type = (opt_data[0] & MPL_OPT_S_MASK) >> MPL_OPT_S_SHIFT;
    const uint8_t *seed_id = opt_data + 2;
    uint8_t seed_id_len = mpl_seed_id_len(seed_id_type);

    /* Special handling - just ignore the MPL option if receiving loopback copy.
     * (MPL gets to process the outgoing message, and with seeding true - when
     * looping back, we want to accept it without MPL getting in the way).
     */
    if (!seeding && buf->options.multicast_loop) {
        return true;
    }

    if (!domain) {
        domain = mpl_domain_lookup_with_realm_check(buf->interface, buf->dst_sa.address);
        if (!domain) {
            tr_debug("No domain %s  %s", tr_ipv6(domain->address), tr_array(seed_id, seed_id_len));
            return false;
        }
    }

    if (seed_id_type == MPL_SEED_IPV6_SRC) {
        seed_id = buf->src_sa.address;
        seed_id_len = 16;
        /* Thread spec says, or at least implies, that ML16/RLOC address is
         * matched against corresponding 16-bit seed id */
        if (thread_addr_is_mesh_local_16(seed_id, buf->interface)) {
            seed_id += 14;
            seed_id_len = 2;
        }
    }

    tr_debug("seed %s seq %"PRIu8, tr_array(seed_id, seed_id_len), sequence);
    mpl_seed_t *seed = mpl_seed_lookup(domain, seed_id_len, seed_id);
    if (!seed) {
        seed = mpl_seed_create(domain, seed_id_len, seed_id, sequence);
        if (!seed) {
            tr_debug("No seed %s  %s", tr_ipv6(domain->address), tr_array(seed_id, seed_id_len));
            return false;
        }
    }

    /* If the M flag is set, we report an inconsistency against any messages with higher sequences */
    if ((opt_data[0] & MPL_OPT_M) && !thread_info(buf->interface)) {
        ns_list_foreach(mpl_buffered_message_t, message, &seed->messages) {
            if (common_serial_number_greater_8(mpl_buffer_sequence(message), sequence)) {
                mpl_buffer_inconsistent(domain, message);
            }
        }
    }

    /* Drop old messages (sequence < MinSequence) */
    if (common_serial_number_greater_8(seed->min_sequence, sequence)) {
        tr_debug("Old MPL message %"PRIu8" < %"PRIu8, sequence, seed->min_sequence);
        return false;
    }

    mpl_buffered_message_t *message = mpl_buffer_lookup(seed, sequence);
    if (message) {
        tr_debug("Repeated MPL message %"PRIu8, sequence);
        trickle_consistent_heard(&message->trickle);
        return false;
    }

    seed->lifetime = domain->seed_set_entry_lifetime;

    uint8_t hop_limit = buffer_data_pointer(buf)[IPV6_HDROFF_HOP_LIMIT];
    if (!seeding && hop_limit != 0) {
        hop_limit--;
    }

    if (domain->data_trickle_params.TimerExpirations == 0 || hop_limit == 0 ||
        (thread_info(domain->interface) && !thread_i_am_router(domain->interface))) {
        /* As a non-forwarder, just accept the packet and advance the
         * min_sequence - means we will drop anything arriving out-of-order, but
         * old implementation always did this in all cases anyway (even if
         * being a forwarder).
         *
         * We also do this if hop limit is 0, so we are not going to forward.
         * This avoids the edge case discussed in the comment above mpl_control_handler.
         *
         * And finally, also treat Thread non-routers like this, to avoid
         * need to dynamically changing TimerExpirations.
         */
        mpl_seed_advance_min_sequence(seed, sequence + 1);
        return true;
    }

    message = mpl_buffer_create(buf, domain, seed, sequence, hop_limit);

    return true;
}


static void mpl_schedule_timer(void)
{
    if (!mpl_timer_running) {
        mpl_timer_running = true;
        protocol_timer_start(PROTOCOL_TIMER_MULTICAST_TIM, mpl_fast_timer, MPL_TICK_MS);
    }
}

static void mpl_fast_timer(uint16_t ticks)
{
    bool need_timer = false;
    mpl_timer_running = false;

    ns_list_foreach(mpl_domain_t, domain, &mpl_domains) {
        if (trickle_timer(&domain->trickle, &domain->control_trickle_params, ticks)) {
            mpl_send_control(domain);
        }
        ns_list_foreach(mpl_seed_t, seed, &domain->seeds) {
            ns_list_foreach(mpl_buffered_message_t, message, &seed->messages) {
                if (trickle_timer(&message->trickle, &domain->data_trickle_params, ticks)) {
                    mpl_buffer_transmit(domain, message, ns_list_get_next(&seed->messages, message) == NULL);
                }
                need_timer = need_timer || trickle_running(&message->trickle, &domain->data_trickle_params);
            }
        }
        need_timer = need_timer || trickle_running(&domain->trickle, &domain->control_trickle_params);
    }

    if (need_timer) {
        mpl_schedule_timer();
    }
}

void mpl_slow_timer(uint16_t seconds)
{
    ns_list_foreach(mpl_domain_t, domain, &mpl_domains) {
        uint32_t message_age_limit = (domain->seed_set_entry_lifetime * UINT32_C(10)) / 4;
        if (message_age_limit > MAX_BUFFERED_MESSAGE_LIFETIME) {
            message_age_limit = MAX_BUFFERED_MESSAGE_LIFETIME;
        }
        ns_list_foreach_safe(mpl_seed_t, seed, &domain->seeds) {
            /* Count down seed lifetime, and expire immediately when hit */
            if (seed->lifetime > seconds) {
                seed->lifetime -= seconds;
            } else {
                mpl_seed_delete(domain, seed);
                continue;
            }
            /* Once data trickle timer has stopped, we MAY delete a message by
             * advancing MinSequence. We use timestamp to control this, so we
             * can hold beyond just the initial data transmission, permitting
             * it to be restarted by control messages.
             */
            ns_list_foreach_safe(mpl_buffered_message_t, message, &seed->messages) {
                if (!trickle_running(&message->trickle, &domain->data_trickle_params) &&
                        protocol_core_monotonic_time - message->timestamp >= message_age_limit) {
                    seed->min_sequence = mpl_buffer_sequence(message) + 1;
                    mpl_buffer_delete(seed, message);
                } else {
                    break;
                }
            }
        }
    }
}

void mpl_clear_realm_scope_seeds(protocol_interface_info_entry_t *cur)
{
    ns_list_foreach(mpl_domain_t, domain, &mpl_domains) {
        if (domain->interface == cur && addr_ipv6_multicast_scope(domain->address) <= IPV6_SCOPE_REALM_LOCAL) {
            ns_list_foreach_safe(mpl_seed_t, seed, &domain->seeds) {
                mpl_seed_delete(domain, seed);
            }
        }
    }
}

static buffer_t *mpl_exthdr_provider(buffer_t *buf, ipv6_exthdr_stage_t stage, int16_t *result)
{
    mpl_domain_t *domain = mpl_domain_lookup_with_realm_check(buf->interface, buf->dst_sa.address);
    if (!domain) {
        // We need to tunnel

        if (stage != IPV6_EXTHDR_MODIFY) {
            *result = 0;
            return buf;
        }

        *result = IPV6_EXTHDR_MODIFY_TUNNEL;
        memcpy(buf->dst_sa.address, ADDR_ALL_MPL_FORWARDERS, 16);
        buf->src_sa.addr_type = ADDR_NONE; // force auto-selection
        return buf;
    }

    const uint8_t *seed_id;
    uint8_t seed_id_len;
    uint8_t seed_id_buf[16];
    if (domain->seed_id_mode > 0) {
        seed_id_len = domain->seed_id_mode;
        seed_id = domain->seed_id;
    } else switch (domain->seed_id_mode) {
        case MULTICAST_MPL_SEED_ID_MAC_SHORT: {
            uint16_t addr = mac_helper_mac16_address_get(buf->interface);
            if (addr < 0xfffe) {
                common_write_16_bit(addr, seed_id_buf);
                seed_id = seed_id_buf;
                seed_id_len = 2;
                break;
            }
            // Otherwise fall through to extended
        case MULTICAST_MPL_SEED_ID_MAC:
            seed_id = buf->interface->mac;
            seed_id_len = 8;
            break;

        case MULTICAST_MPL_SEED_ID_IID_EUI64:
            seed_id = buf->interface->iid_eui64;
            seed_id_len = 8;
            break;

        case MULTICAST_MPL_SEED_ID_IID_SLAAC:
            seed_id = buf->interface->iid_slaac;
            seed_id_len = 8;
            break;
        }

        default:
        case MULTICAST_MPL_SEED_ID_IPV6_SRC_FOR_DOMAIN:
            seed_id = addr_select_source(buf->interface, domain->address, 0);
            seed_id_len = 16;
            break;
    }

    if (!seed_id) {
        tr_err("No MPL Seed ID");
        return buffer_free(buf);
    }

    /* "Compress" seed ID if it's the IPv6 source address */
    /* (For Thread, also compress if source is the 16-bit address) */
    if (seed_id_len == 16 && addr_ipv6_equal(seed_id, buf->src_sa.address)) {
        seed_id_len = 0;
    } else if (seed_id_len == 2 && thread_addr_is_mesh_local_16(buf->src_sa.address, buf->interface) &&
            seed_id[0] == buf->src_sa.address[14] && seed_id[1] == buf->src_sa.address[15]) {
        seed_id_len = 0;
    }

    switch (stage) {
        case IPV6_EXTHDR_SIZE:
            *result = 4 + seed_id_len;
            return buf;
        case IPV6_EXTHDR_INSERT: {
            /* Only have 4 possible lengths/padding patterns to consider:
              * HbH 2 + Option header 4 + Seed 0 + Padding 2 = 8
              * HbH 2 + Option header 4 + Seed 2 + Padding 0 = 8
              * HbH 2 + Option header 4 + Seed 8 + Padding 2 = 16
              * HbH 2 + Option header 4 + Seed 16 + Padding 2 = 24
              */
            uint8_t extlen = (6 + seed_id_len + 7) &~ 7;
            buf = buffer_headroom(buf, extlen);
            if (!buf) {
                return NULL;
            }
            uint8_t *ext = buffer_data_reserve_header(buf, extlen);
            ext[0] = buf->options.type;
            buf->options.type = IPV6_NH_HOP_BY_HOP;
            ext[1] = (extlen / 8) - 1;
            ext[2] = IPV6_OPTION_MPL;
            ext[3] = 2 + seed_id_len;
            ext[4] = (mpl_seed_id_type(seed_id_len) << MPL_OPT_S_SHIFT);
            ext[5] = 0; // sequence placeholder
            memcpy(ext + 6, seed_id, seed_id_len);
            if (seed_id_len != 2) {
                ext[extlen - 2] = IPV6_OPTION_PADN;
                ext[extlen - 1] = 0;
            }

            *result = 0;
            buf->options.ip_extflags |= IPEXT_HBH_MPL | IPEXT_HBH_MPL_UNFILLED;
            return buf;
        }
        case IPV6_EXTHDR_MODIFY:
            if (buf->options.ip_extflags & IPEXT_HBH_MPL_UNFILLED) {
                /* We assume we created this, therefore our option is in place
                 * in the expected place. Sequence is set now, AFTER
                 * fragmentation.
                 */
                uint8_t *iphdr = buffer_data_pointer(buf);
                uint8_t *ext = iphdr + IPV6_HDRLEN;
                if (iphdr[IPV6_HDROFF_NH] != IPV6_NH_HOP_BY_HOP || ext[2] != IPV6_OPTION_MPL) {
                    tr_err("modify");
                    return buffer_free(buf);
                }
                /* We don't bother setting the M flag on these initial packets. Setting to 0 is always acceptable. */
                ext[5] = domain->sequence++;
                buf->options.ip_extflags &=~ IPEXT_HBH_MPL_UNFILLED;
                buf->mpl_option_data_offset = IPV6_HDRLEN + 4;
                mpl_forwarder_process_message(buf, domain, true);
            }
            *result = 0;
            return buf;
        default:
            return buffer_free(buf);
    }
}

#endif /* HAVE_MPL */

