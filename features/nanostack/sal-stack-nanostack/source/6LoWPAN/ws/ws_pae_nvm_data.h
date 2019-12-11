/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#ifndef WS_PAE_NVM_DATA_H_
#define WS_PAE_NVM_DATA_H_

/*
 * Port access entity non-volatile memory (NVM) data module. Module is used
 * to create and parse PAE NVM data TLVs.
 *
 */

/**
 * ws_pae_nvm_store_nw_info_tlv_create create NVM network info TLV
 *
 * \param tlv_entry TLV entry pointer
 * \param pan_id PAN ID
 * \param nw_name network name
 * \param gtks GTK keys
 *
 * \return TLV entry or NULL
 *
 */
void ws_pae_nvm_store_nw_info_tlv_create(nvm_tlv_entry_t *tlv_entry, uint16_t pan_id, char *nw_name, sec_prot_gtk_keys_t *gtks);

/**
 * ws_pae_nvm_store_nw_info_tlv_read read from NVM network info TLV
 *
 * \param tlv_entry TLV entry
 * \param pan_id PAN ID
 * \param nw_name network name
 * \param gtks GTK keys
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_nw_info_tlv_read(nvm_tlv_entry_t *tlv_entry, uint16_t *pan_id, char *nw_name, sec_prot_gtk_keys_t *gtks);

/**
 * ws_pae_nvm_store_keys_tlv_create create NVM keys TLV
 *
 * \param tlv_entry TLV entry buffer pointer
 * \param sec_keys security keys
 *
 */
void ws_pae_nvm_store_keys_tlv_create(nvm_tlv_entry_t *tlv_entry, sec_prot_keys_t *sec_keys);

/**
 * ws_pae_nvm_store_nw_info_tlv_read read from NVM keys TLV
 *
 * \param tlv_entry TLV entry
 * \param sec_keys security keys
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_keys_tlv_read(nvm_tlv_entry_t *tlv_entry, sec_prot_keys_t *sec_keys);

/**
 * ws_pae_nvm_store_frame_counter_tlv_create create NVM frame counter TLV
 *
 * \param tlv_entry TLV entry buffer pointer
 * \param counters frame counters
 *
 */
void ws_pae_nvm_store_frame_counter_tlv_create(nvm_tlv_entry_t *tlv_entry, frame_counters_t *counters);

/**
 * ws_pae_nvm_store_frame_counter_tlv_read read from NVM frame counter TLV
 *
 * \param tlv_entry TLV entry
 * \param counters frame counters
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_frame_counter_tlv_read(nvm_tlv_entry_t *tlv_entry, frame_counters_t *counters);

nvm_tlv_entry_t *ws_pae_buffer_allocate(void);

#endif /* WS_PAE_NVM_DATA_H_ */
