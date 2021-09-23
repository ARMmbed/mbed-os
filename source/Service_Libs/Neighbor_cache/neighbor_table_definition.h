/*
 * Copyright (c) 2014-2017, Pelion and affiliates.
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
 * \file neighbor_table_definition.h
 * \brief Neighbor Cache for Layer 2
 *
 * Frame Counter filter USAGE:
 * Library keeps track of highest accepted Frame Counter and 15 Frame Counter before that. Functionality is:
 *  -   If received counter is duplicate, packet is silently rejected and Security module return error about that
 *  -   If received counter is bigger than highest accepted counter + 15, then we will ???
 *  -   If received counter is smaller than  highest accepted counter - 15, then packet is silently rejected and security module will return duplicate
 */

#ifndef NEIGHBOR_TABLE_DEFINITION_H_
#define NEIGHBOR_TABLE_DEFINITION_H_

#include "ns_list.h"

#define NEIGH_LQI_DEFAULT_SAMPLE                16

#define NEIGH_LINK_REQUEST_COUNTER              3
/** Flag Bit Definition */
#define NEIGH_DEV_MASK                          0x0200
#define NEIGH_FFD_DEV                           0x0200  /** FFD Router Device */
#define NEIGH_RFD_DEV                           0x0000  /** RFD Router Device */
#define NEIGH_RX_ON_IDLE                        0x0100 /** Radio is ON when mac is at idle */
#define NEIGH_HANDSHAKE_READY                   0x0080 /** MLE association is ready or MAC association complete */
#define NEIGH_SECURITY_HANSHAKE_PENGING_FLAG    0x0040 /** Indicate that Security Handshake is not ready with this neighbour  */
#define NEIGH_PRIORITY_FLAG                     0x0020 /** Indicate priority Neighbor (Cordinator or ND/RPL Router) */
#define NEIGH_RESERVED_FLAG                     0x0010
#define NEIGH_IDR_MASK                          0x000f /** MLE Spesific IDR value */

/** Mode bit definition for MLE */
#define NEIGH_MODE_DEV_MASK 2
#define NEIGH_MODE_FFD_DEV 2
#define NEIGH_MODE_RFD_DEV 0
#define NEIGH_MODE_RX_ON_IDLE 8

#define NEIGH_MAX_KEY_TABLE_SIZE            2

/*!
 * \enum keypair_key_state_e
 * \brief Key Pair key state.
 */
typedef enum keypair_key_state_e {
    KEYPAIR_NOT_VALID = 0,          /*!<  Key info idle state */
    KEYPAIR_FOR_TX_RX,              /*!<  Primary Key */
    KEYPAIR_FOR_RX,                 /*!<  Secondary key only for RX */
    KEYPAIR_WAIT_RX_TO_PRIMARY      /*!<  After key update process key need to be trig by network cordinator */
} keypair_key_state_e;

/*!
 * \enum neighbor_address_type_e
 * \brief Address Type.
 */
typedef enum neighbor_address_type_e {
    NEIGH_64_BIT_ADDRESS = 0,           /*!<  64-bit MAC address */
    NEIGH_16_BIT_ADDRESS,               /*!<  16-bit MAC address */

    NEIGH__TIMED_OUT,                   /*!<  timed-out entry (internal use) */
} neighbor_address_type_e;

/*!
 * \struct neighbor_keypair_info_s
 *
 * \brief Key pair info structure.
 */
typedef struct neighbor_keypair_info_s {
    uint8_t  aes_key[16];                   /*!< Unique key pair key */
    uint8_t  key_id;                        /*!< Unique key pair id */
    uint32_t input_security_frame_counter;  /*!< Accepted highest Frame Counter */
    uint16_t missed_frame_counters;         /*!< Bit mask for detect missed packet counters bit 0 is is always value from  input_security_frame_counter */
    uint32_t output_security_frame_counter; /*!< Output Frame counter */
    keypair_key_state_e  key_state;         /*!< Define, Primary, Secondary and coming key state */
} neighbor_keypair_info_s;

/*!
 * \struct neighbor_generic_sec_info_s
 *
 * \brief Generic Security key components.
 */
typedef struct neighbor_generic_sec_info_s {
    uint8_t key_id;                     /*!< Security Key Id */
    uint32_t security_frame_counter;    /*!< Security frame counter for generic network Key */
    uint16_t missed_frame_counters;     /*!< Bit mask for detect missed packet counters bit 0 is is allways value from  accepted_security_frame_counter */
} neighbor_generic_sec_info_s;

/*!
 * \struct neighbor_sec_key_rx_components_s
 *
 * \brief Security RX key component info structure.
 */
typedef struct neighbor_sec_key_rx_components_s {
    neighbor_generic_sec_info_s component_table[NEIGH_MAX_KEY_TABLE_SIZE]; /*!< Security Key Id component table */
    uint8_t default_ptr;/*!< Indicate primary entry 0 or 1 */
} neighbor_sec_key_rx_components_s;

/*!
 * \struct neighbor_keypair_components_s
 *
 * \brief Security Key pair component table info structure.
 */
typedef struct neighbor_keypair_components_s {
    neighbor_keypair_info_s key_pair_table[NEIGH_MAX_KEY_TABLE_SIZE]; /*!< Security Key pair component table */
    uint8_t default_ptr; /*!< Indicate primary entry 0 or 1 */
} neighbor_keypair_components_s;

/*!
 * \struct neighbor_lqi_info_s
 *
 * \brief Neighbour LQI info structure.
 */
typedef struct neighbor_lqi_info_s {
    uint8_t calculated_avarage_lqi; /*!< Average LQI after  defined Sample Count*/
    uint16_t lqi_sample_sum;        /*!< Sum of LQI's */
    uint8_t sample_counter;         /*!< LQI sample counter */
} neighbor_lqi_info_s;

typedef struct neigh_cache_entry_s {
    uint16_t                            ttl;                /*!< Entry timeout tick value is interface specific MLE configured interface use 4 Second tick value */
    uint8_t                             link_req_counter;   /*!< This value is used when TTL goes to zero*/
    uint16_t                            timeout_rx;         /*!< Configured Timeout start value MLE define or MAC */
    uint8_t                             mac64[8];           /*!< EUI-64 for Node */
    uint16_t                            mac16;              /*!< Neighbor 16-bit Address, 0xffff if not configured */
    uint16_t                            flags;              /*!< Indicate Priority, Handshake Ready, Security active,Device Type & Receiver state at MAC idle state  */
    neighbor_lqi_info_s                 lqi_info;           /*!< Neighbor LQI info */
    uint8_t                             route_external_advertise;   /*!< Route external advertisement counter */
    neighbor_sec_key_rx_components_s    neighbor_sec_key_rx_components; /* Security RX key frame filters */
    neighbor_keypair_components_s       *neighbor_keypair_info; /* Optionally when node to node have unique key pairs */
    ns_list_link_t                      link;               /*!< List link entry */
} neigh_cache_entry_s;

#define NEIGH_ENTRY_PTR_ERR ((neigh_cache_entry_s *) -1)

/*!
 * \struct neigh_cache_s
 *
 * \brief Top-level neighbour cache descriptor.
 *
 * This passed by reference to all APIs. Users should not manipulate contents.
 */
typedef struct neigh_cache_s {
    NS_LIST_HEAD(neigh_cache_entry_s, link) head;
} neigh_cache_s;

/**
 * \brief A function to initialize a neighbor cache before use
 *
 * \param neigh_cache pointer to neighbor cache.
 */
extern void neighbor_cache_init(neigh_cache_s *neigh_cache);

/**
 * \brief A function to flush an entire neighbour cache
 *
 * \param neigh_cache pointer to neighbor cache.
 *
 * \return <n> The number of entries removed
 */
extern int neighbor_cache_flush(neigh_cache_s *neigh_cache);

/**
 * \brief Helper macro to get the first cache entry, for iteration.
 *
 * \param neigh_cache pointer to neighbor cache.
 *
 * \return pointer to first cache entry
 * \return NULL if cache empty
 */
#define neighbor_cache_first(neigh_cache) ns_list_get_first(&(neigh_cache)->head)

/**
 * \brief Helper macro to get the next cache entry, for iteration.
 *
 * Not safe for use if you're deleting entries - use
 * neighbor_cache_delete_by_pointer in that case.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param entry pointer to current neighbor cache entry.
 *
 * \return pointer to next cache entry
 * \return NULL if no more entries
 */
#define neighbor_cache_entry_next(cache, entry) ns_list_get_next(&(cache)->head, entry)

/**
 * \brief A function to create or return an existing neighbor cache entry.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param address_ptr pointer to EUI-64 address (64-bit)
 *
 * \return pointer to cache entry, possibly newly created.
 * \return NULL if entry not found or an unable to allocate memory for new entry
 */
extern neigh_cache_entry_s *neighbor_cache_entry_create(neigh_cache_s *neigh_cache, const uint8_t address_ptr[8]);

/**
 * \brief A function to locate an existing entry by address
 *
 * Note that this can re-order the cache, so could upset iteration using macros.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param address_type type for 16-bit or 64-bit address
 * \param address_ptr pointer to address of specified type
 *
 * \return pointer to cache entry
 * \return NULL if not found
 */
extern neigh_cache_entry_s *neighbor_cache_entry_get(neigh_cache_s *neigh_cache, neighbor_address_type_e address_type, const void *address_ptr);

/**
 * \brief A function to delete an entry by address.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param address_type type for 16-bit or 64-bit address
 * \param address_ptr pointer to address of specified type
 *
 * \return 0 Removed OK
 * \return -1 Entry not found
 */
extern int8_t neighbor_cache_entry_delete(neigh_cache_s *neigh_cache, neighbor_address_type_e address_type, const void *address_ptr);

/**
 * \brief A function to delete an entry by entry pointer.
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param entry pointer to entry
 *
 * \return pointer to the next entry, to allow deletion during iteration
 * \return NULL if no more entries
 * \return NEIGH_ENTRY_PTR_ERR if entry pointer not found (no longer checked)
 */
extern neigh_cache_entry_s *neighbor_cache_entry_delete_by_entry_pointer(neigh_cache_s *neigh_cache, neigh_cache_entry_s *entry);


/**
 * \brief A function to get a timed-out neighbor entry.
 *
 * Returns an entry whose TTL field is set to zero.
 *
 * \param neigh_cache pointer to neighbor cache.
 *
 * \return pointer to a timed-out entry
 * \return NULL if no timed-out entries
 */
extern neigh_cache_entry_s *neighbor_cache_entry_get_timed_out(neigh_cache_s *neigh_cache);

/**
 * \brief A function to update Neighbor cache Time-To-Live values.
 *
 * This decrements the TTL for all entries in the cache. TTL values are
 * are clamped to not wrap past zero. When an entry's TTL value becomes zero,
 * link_req_counter is set to NEIGH_LINK_REQUEST_COUNTER. (Note that
 * newly-created entries have ttl and link_req_counter both zero - they will
 * need initialising before use).
 *
 * \param neigh_cache pointer to neighbor cache.
 * \param tick amount to decrement TTL
 *
 * \return total number of entries in the cache whose TTL is 0 after the update
 */
extern int neighbor_cache_ttl_update(neigh_cache_s *neigh_cache, uint16_t ticks);

#endif /* NEIGHBOR_TABLE_DEFINITION_H_ */
