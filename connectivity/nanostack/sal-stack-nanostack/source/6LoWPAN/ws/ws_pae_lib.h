/*
 * Copyright (c) 2018-2021, Pelion and affiliates.
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

#ifndef WS_PAE_LIB_H_
#define WS_PAE_LIB_H_

/*
 * Port access entity library functions.
 *
 */

typedef struct {
    kmp_api_t *kmp;                    /**< KMP API */
    bool timer_running;                /**< Timer running inside KMP */
    ns_list_link_t link;               /**< Link */
} kmp_entry_t;

typedef NS_LIST_HEAD(kmp_entry_t, link) kmp_list_t;

typedef struct supp_entry_s {
    kmp_list_t kmp_list;               /**< Ongoing KMP negotiations */
    kmp_addr_t addr;                   /**< EUI-64 (Relay IP address, Relay port) */
    sec_prot_keys_t sec_keys;          /**< Security keys */
    uint32_t ticks;                    /**< Ticks */
    uint16_t waiting_ticks;            /**< Waiting ticks */
    uint16_t store_ticks;              /**< NVM store ticks */
    bool active : 1;                   /**< Is active */
    bool access_revoked : 1;           /**< Nodes access is revoked */
    ns_list_link_t link;               /**< Link */
} supp_entry_t;

typedef NS_LIST_HEAD(supp_entry_t, link) supp_list_t;

typedef struct {
    kmp_shared_comp_t *data;           /**< KMP shared component data */
    ns_list_link_t link;               /**< Link */
} shared_comp_entry_t;

typedef NS_LIST_HEAD(shared_comp_entry_t, link) shared_comp_list_t;

/**
 * ws_pae_lib_kmp_list_init initializes KMP list
 *
 * \param kmp_list KMP list
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
void ws_pae_lib_kmp_list_init(kmp_list_t *kmp_list);

/**
 * ws_pae_lib_kmp_list_add adds KMP to KMP list
 *
 * \param kmp_list KMP list
 * \param kmp KMP
 *
 * \return KMP list entry on success
 * \return NULL on failure
 *
 */
kmp_entry_t *ws_pae_lib_kmp_list_add(kmp_list_t *kmp_list, kmp_api_t *kmp);

/**
 * ws_pae_lib_kmp_list_delete deletes KMP from KMP list
 *
 * \param kmp_list KMP list
 * \param kmp KMP
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_lib_kmp_list_delete(kmp_list_t *kmp_list, kmp_api_t *kmp);

/**
 * ws_pae_lib_kmp_list_free frees KMP list
 *
 * \param kmp_list KMP list
 *
 */
void ws_pae_lib_kmp_list_free(kmp_list_t *kmp_list);

/**
 * ws_pae_lib_kmp_list_type_get gets KMP from KMP list based on KMP type
 *
 * \param kmp_list KMP list
 * \param type type
 *
 * \return KMP on success
 * \return NULL on failure
 *
 */
kmp_api_t *ws_pae_lib_kmp_list_type_get(kmp_list_t *kmp_list, kmp_type_e type);

/**
 * ws_pae_lib_kmp_list_instance_id_get gets KMP from KMP list based on instance identifier
 *
 * \param kmp_list KMP list
 * \param instance_id instance identifier
 *
 * \return KMP on success
 * \return NULL on failure
 *
 */
kmp_api_t *ws_pae_lib_kmp_list_instance_id_get(kmp_list_t *kmp_list, uint8_t instance_id);

/**
 * ws_pae_lib_kmp_list_entry_get gets KMP entry from KMP list based on KMP
 *
 * \param kmp_list KMP list
 * \param kmp KMP
 *
 * \return KMP list entry on success
 * \return NULL on failure
 *
 */
kmp_entry_t *ws_pae_lib_kmp_list_entry_get(kmp_list_t *kmp_list, kmp_api_t *kmp);

/**
 * ws_pae_lib_kmp_list_empty checks whether KMP list is empty
 *
 * \param kmp_list KMP list
 *
 * \return true list is empty
 * \return false list is not empty
 *
 */
bool ws_pae_lib_kmp_list_empty(kmp_list_t *kmp_list);

/**
 * ws_pae_lib_kmp_list_count counts entries on KMP list
 *
 * \param kmp_list KMP list
 *
 * \return count of entries on the list
 *
 */
uint8_t ws_pae_lib_kmp_list_count(kmp_list_t *kmp_list);

/**
 * ws_pae_lib_kmp_timer_start starts KMP timer
 *
 * \param kmp_list KMP list
 * \param entry KMP list entry
 *
 */
void ws_pae_lib_kmp_timer_start(kmp_list_t *kmp_list, kmp_entry_t *entry);

/**
 * ws_pae_lib_kmp_timer_stop stops KMP timer
 *
 * \param kmp_list KMP list
 * \param entry KMP list entry
 *
 */
void ws_pae_lib_kmp_timer_stop(kmp_list_t *kmp_list, kmp_entry_t *entry);

/**
 * ws_pae_lib_kmp_timer_timeout KMP timer timeout callback
 *
 * \param kmp KMP
 * \param ticks timer ticks
 *
 */
typedef void ws_pae_lib_kmp_timer_timeout(kmp_api_t *kmp, uint16_t ticks);

/**
 *  ws_pae_lib_kmp_timer_update updates KMP timers on KMP list
 *
 * \param kmp_list KMP list
 * \param ticks timer ticks
 * \param timeout callback to call on timeout
 *
 * \return true KMP list has KMPs
 * \return false no KMPs on KMP list
 *
 */
bool ws_pae_lib_kmp_timer_update(kmp_list_t *kmp_list, uint16_t ticks, ws_pae_lib_kmp_timer_timeout timeout);

/**
 *  ws_pae_lib_supp_list_init initiates supplicant list
 *
 * \param supp_list supplicant list
 *
 */
void ws_pae_lib_supp_list_init(supp_list_t *supp_list);

/**
 *  ws_pae_lib_supp_list_add adds entry to supplicant list
 *
 * \param supp_list supplicant list
 * \param addr address
 *
 * \return supplicant list entry on success
 * \return NULL on failure
 */
supp_entry_t *ws_pae_lib_supp_list_add(supp_list_t *supp_list, const kmp_addr_t *addr);

/**
 * ws_pae_lib_supp_deleted supplicant delete callback
 *
 * \param instance Instance
 *
 */
typedef void ws_pae_lib_supp_deleted(void *instance);

/**
 *  ws_pae_lib_supp_list_add removes entry from supplicant list
 *
 * \param instance Instance
 * \param supp_list supplicant list
 * \param entry entry
 * \param supp_deleted callback to call on supplicant delete
 *
 * \return < 0 failure
 * \return >= 0 success
 */
int8_t ws_pae_lib_supp_list_remove(void *instance, supp_list_t *supp_list, supp_entry_t *supp, ws_pae_lib_supp_deleted supp_deleted);

/**
 *  ws_pae_lib_supp_list_entry_eui_64_get gets entry from supplicant list based on EUI-64
 *
 * \param supp_list supplicant list
 * \param eui_64 EUI-64
 *
 * \return supplicant list entry on success
 * \return NULL on failure
 */
supp_entry_t *ws_pae_lib_supp_list_entry_eui_64_get(const supp_list_t *supp_list, const uint8_t *eui_64);

/**
 *  ws_pae_lib_supp_list_delete deletes supplicant list
 *
 * \param supp_list supplicant list
 *
 */
void ws_pae_lib_supp_list_delete(supp_list_t *supp_list);

/**
 *  ws_pae_lib_supp_list_timer_update updates timers on supplicant list
 *
 * \param instance Instance
 * \param active_supp_list list of active supplicants
 * \param inactive_supp_list list of inactive supplicants
 * \param ticks timer ticks
 * \param timeout callback to call on timeout
 * \param supp_deleted callback to call on supplicant delete
 *
 * \return true timer needs still to be running
 * \return false timer can be stopped
 */
bool ws_pae_lib_supp_list_timer_update(void *instance, supp_list_t *active_supp_list, uint16_t ticks, ws_pae_lib_kmp_timer_timeout timeout, ws_pae_lib_supp_deleted supp_deleted);

/**
 *  ws_pae_lib_supp_list_slow_timer_update updates slow timer on supplicant list
 *
 * \param supp_list list of supplicants
 * \param seconds seconds
 *
 */
void ws_pae_lib_supp_list_slow_timer_update(supp_list_t *supp_list, uint16_t seconds);

/**
 *  ws_pae_lib_supp_list_timer_update updates supplicant timers
 *
 * \param entry supplicant entry
 * \param ticks timer ticks
 * \param timeout callback to call on timeout
 *
 * \return true timer needs still to be running
 * \return false timer can be stopped
 */
bool ws_pae_lib_supp_timer_update(void *instance, supp_entry_t *entry, uint16_t ticks, ws_pae_lib_kmp_timer_timeout timeout);

/**
 *  ws_pae_lib_supp_init initiates supplicant entry
 *
 * \param entry supplicant entry
 *
 */
void ws_pae_lib_supp_init(supp_entry_t *entry);

/**
 *  ws_pae_lib_supp_delete deletes supplicant entry
 *
 * \param entry supplicant entry
 *
 */
void ws_pae_lib_supp_delete(supp_entry_t *entry);

/**
 *  ws_pae_lib_supp_timer_ticks_set sets supplicant timer ticks
 *
 * \param entry supplicant entry
 * \param ticks ticks
 *
 */
void ws_pae_lib_supp_timer_ticks_set(supp_entry_t *entry, uint32_t ticks);

/**
 *  ws_pae_lib_supp_timer_ticks_add adds supplicant timer ticks
 *
 * \param entry supplicant entry
 * \param ticks ticks
 *
 */
void ws_pae_lib_supp_timer_ticks_add(supp_entry_t *entry, uint32_t ticks);

/**
 *  ws_pae_lib_supp_timer_is_running checks whether supplicant timer is running
 *
 * \param entry supplicant entry
 *
 * \return TRUE timer is running, FALSE timer is not running
 *
 */
bool ws_pae_lib_supp_timer_is_running(supp_entry_t *entry);

/**
 *  ws_pae_lib_supp_list_to_active move supplicant to active supplicants list
 *
 * \param active_supp_list list of active supplicants
 * \param inactive_supp_list list of inactive supplicants
 * \param entry supplicant entry
 *
 */
void ws_pae_lib_supp_list_to_active(supp_list_t *active_supp_list, supp_list_t *inactive_supp_list, supp_entry_t *entry);

/**
 *  ws_pae_lib_supp_list_to_inactive move supplicant to inactive supplicants list
 *
 * \param instance Instance
 * \param active_supp_list list of active supplicants
 * \param entry supplicant entry
 * \param supp_deleted callback to call on supplicant delete
 *
 */
void ws_pae_lib_supp_list_to_inactive(void *instance, supp_list_t *active_supp_list, supp_entry_t *entry, ws_pae_lib_supp_deleted supp_deleted);

/**
 *  ws_pae_lib_supp_list_purge purge inactive supplicants list
 *
 * \param instance Instance
 * \param active_supp_list list of active supplicants
 * \param max_number maximum number of supplicant entries, can be set to 0 in combination with max_purge
 *                   to free list entries even when maximum number supplicant entries has not been reached
 * \param max_purge maximum number of supplicants to purge in one call, 0 means not limited
 * \param supp_deleted callback to call on supplicant delete
 *
 */
void ws_pae_lib_supp_list_purge(void *instance, supp_list_t *active_supp_list, uint16_t max_number, uint8_t max_purge, ws_pae_lib_supp_deleted supp_deleted);

/**
 *  ws_pae_lib_supp_list_kmp_count counts the number of KMPs of a certain type in a list of supplicants
 *
 * \param supp_list list of supplicants
 * \param type KMP type
 *
 * \return number of KMPs in the supplicant list
 *
 */
uint16_t ws_pae_lib_supp_list_kmp_count(supp_list_t *supp_list, kmp_type_e type);

/**
 *  ws_pae_lib_supp_list_entry_is_in_list checks if the entry is in the list
 *
 * \param supp_list list of supplicants
 * \param searched_entry entry that is searched
 *
 * \return TRUE entry is in list, FALSE otherwise
 *
 */
bool ws_pae_lib_supp_list_entry_is_in_list(supp_list_t *supp_list, supp_entry_t *searched_entry);

/**
 *  ws_pae_lib_supp_list_kmp_receive_check check if received message is for this KMP in a list of supplicants
 *
 * \param supp_list list of supplicants
 * \param pdu pdu
 * \param size pdu size
 *
 * \return KMP api for the received message
 *
 */
kmp_api_t *ws_pae_lib_supp_list_kmp_receive_check(supp_list_t *supp_list, const void *pdu, uint16_t size);

/**
 *  ws_pae_lib_shared_comp_list_init init shared component list
 *
 * \param comp_list component list
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_lib_shared_comp_list_init(shared_comp_list_t *comp_list);

/**
 *  ws_pae_lib_shared_comp_list_free free shared component list
 *
 * \param comp_list component list
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_lib_shared_comp_list_free(shared_comp_list_t *comp_list);

/**
 *  ws_pae_lib_shared_comp_list_add add to shared component list
 *
 * \param comp_list component list
 * \param data shared component
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_lib_shared_comp_list_add(shared_comp_list_t *comp_list, kmp_shared_comp_t *data);

/**
 *  ws_pae_lib_shared_comp_list_remove remove from shared component list
 *
 * \param comp_list component list
 * \param data shared component
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_lib_shared_comp_list_remove(shared_comp_list_t *comp_list, kmp_shared_comp_t *data);

/**
 *  ws_pae_lib_shared_comp_list_timeout timeout to shared component list
 *
 * \param comp_list component list
 * \param ticks elapsed time in seconds
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_lib_shared_comp_list_timeout(shared_comp_list_t *comp_list, uint16_t ticks);

#endif /* WS_PAE_AUTH_H_ */
