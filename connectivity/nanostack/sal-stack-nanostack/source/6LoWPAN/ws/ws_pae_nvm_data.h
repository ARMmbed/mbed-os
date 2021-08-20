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

#ifndef WS_PAE_NVM_DATA_H_
#define WS_PAE_NVM_DATA_H_

/*
 * Port access entity non-volatile memory (NVM) data module. Module is used
 * to create and parse PAE NVM data TLVs.
 *
 */

// file names
#define NW_INFO_FILE_NAME               "pae_nw_info"
#define KEYS_FILE_NAME                  "pae_keys"
#define FRAME_COUNTER_FILE_NAME         "pae_frame_counter"

#define PAE_NVM_NW_INFO_TAG              1
#define PAE_NVM_KEYS_TAG                 2
#define PAE_NVM_FRAME_COUNTER_TAG        3
#define PAE_NVM_KEY_STORAGE_INDEX_TAG    4
#define PAE_NVM_KEY_STORAGE_TAG          5

// pan_id (2) +  network name (33) + GTK EUI-64 (own EUI-64) (8) + (GTK set (1) + GTK expiry timestamp (8) + status (1) + install order (1) + GTK (16)) * 4
#define PAE_NVM_NW_INFO_LEN              2 + 33 + 8 + (1 + 8 + 1 + 1 + GTK_LEN) * GTK_NUM

// PTK EUI-64 set (1) + PTK EUI-64 (8) + PMK set (1) + PMK lifetime (4) + PMK (32) + PMK replay counter (8) + PTK set (1) + PTK lifetime (4) + PTK (48)
#define PAE_NVM_KEYS_LEN                 1 + 8 + 1 + 4 + PMK_LEN + 8 + 1 + 4 + PTK_LEN

// restart counter + stored time + pan version + (frame counter set (1) + GTK (16) + frame counter (4)) * 4
#define PAE_NVM_FRAME_COUNTER_LEN        4 + 8 + 2 + (1 + GTK_LEN + 4) * GTK_NUM

#define PAE_NVM_DEFAULT_BUFFER_SIZE      sizeof(nvm_tlv_t) + PAE_NVM_NW_INFO_LEN

// key storage index bitfield (8)
#define PAE_NVM_KEY_STORAGE_INDEX_LEN    8

typedef struct nw_info_nvm_tlv {
    uint16_t tag;                             /**< Unique tag */
    uint16_t len;                             /**< Number of the bytes after the length field */
    uint8_t data[PAE_NVM_NW_INFO_LEN];        /**< Data */
} nw_info_nvm_tlv_t;

typedef struct keys_nvm_tlv {
    uint16_t tag;                             /**< Unique tag */
    uint16_t len;                             /**< Number of the bytes after the length field */
    uint8_t data[PAE_NVM_KEYS_LEN];           /**< Data */
} keys_nvm_tlv_t;

typedef struct frame_cnt_nvm_tlv {
    uint16_t tag;                             /**< Unique tag */
    uint16_t len;                             /**< Number of the bytes after the length field */
    uint8_t data[PAE_NVM_FRAME_COUNTER_LEN];  /**< Data */
} frame_cnt_nvm_tlv_t;

/**
 * ws_pae_nvm_store_generic_tlv_create create NVM generic storage TLV
 *
 * \param tlv_entry TLV entry
 * \param tag tag
 * \param length length of the (whole) entry
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
void ws_pae_nvm_store_generic_tlv_create(nvm_tlv_t *tlv_entry, uint16_t tag, uint16_t length);

nvm_tlv_t *ws_pae_nvm_store_generic_tlv_allocate_and_create(uint16_t tag, uint16_t length);

void ws_pae_nvm_store_generic_tlv_free(nvm_tlv_t *tlv_entry);

/**
 * ws_pae_nvm_store_nw_info_tlv_create create NVM network info TLV
 *
 * \param tlv_entry TLV
 * \param pan_id PAN ID
 * \param nw_name network name
 * \param gtks GTK keys
 *
 * \return TLV entry or NULL
 *
 */
void ws_pae_nvm_store_nw_info_tlv_create(nw_info_nvm_tlv_t *tlv_entry, uint16_t pan_id, char *nw_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks);

/**
 * ws_pae_nvm_store_nw_info_tlv_read read from NVM network info TLV
 *
 * \param tlv_entry TLV
 * \param pan_id PAN ID
 * \param nw_name network name
 * \param gtks GTK keys
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_nw_info_tlv_read(nw_info_nvm_tlv_t *tlv_entry, uint16_t *pan_id, char *nw_name, uint8_t *gtk_eui64, sec_prot_gtk_keys_t *gtks);

/**
 * ws_pae_nvm_store_keys_tlv_create create NVM keys TLV
 *
 * \param tlv_entry TLV
 * \param sec_keys security keys
 *
 */
void ws_pae_nvm_store_keys_tlv_create(keys_nvm_tlv_t *tlv_entry, sec_prot_keys_t *sec_keys);

/**
 * ws_pae_nvm_store_nw_info_tlv_read read from NVM keys TLV
 *
 * \param tlv_entry TLV
 * \param sec_keys security keys
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_keys_tlv_read(keys_nvm_tlv_t *tlv_entry, sec_prot_keys_t *sec_keys);

/**
 * ws_pae_nvm_store_frame_counter_tlv_create create NVM frame counter TLV
 *
 * \param tlv_entry TLV buffer pointer
 * \param restart_cnt re-start counter
 * \param pan_version PAN version
 * \param counters frame counters
 *
 */
void ws_pae_nvm_store_frame_counter_tlv_create(frame_cnt_nvm_tlv_t *tlv_entry, uint32_t restart_cnt, uint16_t pan_version, frame_counters_t *counters);

/**
 * ws_pae_nvm_store_frame_counter_tlv_read read from NVM frame counter TLV
 *
 * \param tlv_entry TLV
 * \param restart_cnt re-start counter
 * \param stored_time stored timestampt
 * \param pan_version PAN version
 * \param counters frame counters
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_frame_counter_tlv_read(frame_cnt_nvm_tlv_t *tlv_entry, uint32_t *restart_cnt, uint64_t *stored_time, uint16_t *pan_version, frame_counters_t *counters);

/**
 * ws_pae_nvm_store_key_storage_index_tlv_create create NVM key storage index TLV
 *
 * \param tlv_entry TLV entry
 * \param bitfield index of filenames
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
void ws_pae_nvm_store_key_storage_index_tlv_create(nvm_tlv_t *tlv_entry, uint64_t bitfield);

/**
 * ws_pae_nvm_store_key_storage_index_tlv_read read NVM key storage index TLV
 *
 * \param tlv_entry TLV entry
 * \param bitfield index of filenames
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_key_storage_index_tlv_read(nvm_tlv_t *tlv_entry, uint64_t *bitfield);

/**
 * ws_pae_nvm_store_key_storage_tlv_create create NVM key storage TLV
 *
 * \param tlv_entry TLV entry
 * \param length length of the (whole) entry
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
void ws_pae_nvm_store_key_storage_tlv_create(nvm_tlv_t *tlv_entry, uint16_t length);

/**
 * ws_pae_nvm_store_key_storage_tlv_read read NVM key storage TLV
 *
 * \param tlv_entry TLV entry
 * \param length length of the (whole) entry
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t ws_pae_nvm_store_key_storage_tlv_read(nvm_tlv_t *tlv_entry, uint16_t length);

nvm_tlv_t *ws_pae_buffer_allocate(void);

#endif /* WS_PAE_NVM_DATA_H_ */
