/*
 * Copyright (c) 2020, Pelion and affiliates.
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

#ifndef WS_PAE_KEY_STORAGE_H_
#define WS_PAE_KEY_STORAGE_H_

/*
 * Port access entity key storage functions.
 *
 */

// Number of storages i.e. records in NVM
#define DEFAULT_NUMBER_OF_STORAGES                 50

// Number of entries stored in a storage i.e. one record in NVM
#define DEFAULT_NUMBER_OF_ENTRIES_IN_ONE_STORAGE   100

// Interval to check if storage has been modified and needs to be updated to NVM
#define DEFAULT_STORING_INTERVAL                   3600

struct supp_entry_s;
struct sec_prot_gtk_keys_s;
struct sec_prot_certs_s;

/**
 * ws_pae_key_storage_memory_set sets memory used for key storages
 *
 * This functions can be used to set memory used by key storage. When memory areas
 * are set, module does not allocate memory internally from heap.
 *
 * \param key_storages_number number of memory areas
 * \param key_storage_size array of memory area sizes
 * \param key_storages array of memory area start pointers
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_key_storage_memory_set(uint8_t key_storages_number, const uint16_t *key_storage_size, void **key_storages);

/**
 * ws_pae_key_storage_memory_set sets key storage settings
 *
 * Allocation max number and allocation size sets the settings that are used when key storage
 * memory is allocated dynamically from heap. These settings must be set before (first) interface
 * up and shall not be set if key storage memory is set by ws_pae_key_storage_memory_set() call.
 *
 * \param alloc_max_number maximum number of allocation made to dynamic memory
 * \param alloc_size size of each allocation
 * \param storing_interval interval in which the check to store to NVM is made
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_key_storage_settings_set(uint8_t alloc_max_number, uint16_t alloc_size, uint16_t storing_interval);

/**
 * ws_pae_key_storage_init init key storage
 *
 */
void ws_pae_key_storage_init(void);

/**
 * ws_pae_key_storage_init delete key storage
 *
 */
void ws_pae_key_storage_delete(void);

/**
 * ws_pae_key_storage_store store to NVM
 *
 * Checks whether key storage data has been updated and stores to NVM.
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_key_storage_store(void);

/**
 * ws_pae_key_storage_read read from NVM
 *
 * Reads key storage data from NVM.
 *
 */
void ws_pae_key_storage_read(uint32_t restart_cnt);

/**
 * ws_pae_key_storage_remove remove storage from NVM
 *
 * Removes key storage data from NVM.
 *
 */
void ws_pae_key_storage_remove(void);

/**
 * ws_pae_key_storage_supp_write writes supplicant entry to key storage
 *
 * \param instance instance
 * \param pae_supp supplicant entry
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_key_storage_supp_write(const void *instance, struct supp_entry_s *pae_supp);

/**
 * ws_pae_key_storage_supp_read reads supplicant entry from key storage
 *
 * \param instance instance
 * \param eui_64 EUI-64 of the supplicant
 * \param gtks GTK keys
 * \param cert_chain certificates
 *
 * \return supplicant entry or NULL if supplicant entry does not exits
 *
 */
struct supp_entry_s *ws_pae_key_storage_supp_read(const void *instance, const uint8_t *eui_64, struct sec_prot_gtk_keys_s *gtks, const struct sec_prot_certs_s *certs);

/**
 * ws_pae_key_storage_supp_delete delete supplicant entry from key storage
 *
 * \param instance instance
 * \param eui_64 EUI-64 of the supplicant
 *
 * \return true entry was deleted
 * \return false entry was not deleted
 *
 */
bool ws_pae_key_storage_supp_delete(const void *instance, const uint8_t *eui64);

/**
 * ws_pae_key_storage_timer key storage timers
 *
 * \param seconds Seconds passed
 *
 */
void ws_pae_key_storage_timer(uint16_t seconds);

/**
 * ws_pae_key_storage_fast_timer key storage fast timers
 *
 * \param ticks Ticks passed
 *
 */
void ws_pae_key_storage_fast_timer(uint16_t ticks);

/**
 * ws_pae_key_storage_storing_interval_get gets key storage storing interval
 *
 * \return storing interval in seconds
 *
 */
uint16_t ws_pae_key_storage_storing_interval_get(void);

#endif /* WS_PAE_KEY_STORAGE_H_ */
