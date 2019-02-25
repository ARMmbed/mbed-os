/*
 * Copyright (c) 2016-2019, Arm Limited and affiliates.
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

#ifndef SEC_PROT_KEYS_H_
#define SEC_PROT_KEYS_H_

/*
 * Security protocols EAPOL key storage module. This is used by EAPOL protocols to store
 * and update key information. This can be used either as supplicant key storage or as
 * an authenticator key storage for a specific supplicant. Storage can be also used to
 * access global security data (Group Transient Keys and certificate information).
 *
 */

#define PMK_LEN       32
#define PTK_LEN       48
#define GTK_LEN       16
#define GTK_NUM       4

#define KCK_LEN       16
#define KEK_LEN       16

#define KCK_INDEX     0
#define KEK_INDEX     16

#define PMKID_LEN     16
#define PTKID_LEN     16

#define GTK_DEFAULT_LIFETIME    60 * 60 * 24 * 30 // 30 days

typedef struct {
    uint8_t             key[GTK_LEN];                 /**< Group Transient Key (128 bits) */
    uint32_t            lifetime;                     /**< Lifetime is seconds */
    bool                set: 1;                       /**< Group Transient Key set (valid value) */
    bool                live: 1;                      /**< Group Transient Key live (as indicated by authenticator) */
    bool                hash: 1;                      /**< Group Transient Key matches to hash */
} gtk_key_t;

typedef struct {
    gtk_key_t              gtk[GTK_NUM];              /**< 4 Group Transient Keys */
    int8_t                 gtk_set_index;             /**< Group Transient Key to insert */
    bool                   updated;                   /**< Group Transient Keys has been updated */
} sec_prot_gtk_keys_t;

// Security key data
typedef struct {
    uint64_t               pmk_key_replay_cnt;        /**< Pairwise Master Key replay counter */
    uint8_t                pmk[PMK_LEN];              /**< Pairwise Master Key (256 bits) */
    uint8_t                ptk[PTK_LEN];              /**< Pairwise Transient Key (384 bits) */
    uint8_t                *ptk_eui_64;               /**< Pointer to remote EUI-64 used to derive PTK or NULL */
    sec_prot_gtk_keys_t    *gtks;                     /**< Group Transient Keys */
    const sec_prot_certs_t *certs;                    /**< Certificates */
    bool                   pmk_set: 1;                /**< Pairwise Master Key set */
    bool                   ptk_set: 1;                /**< Pairwise Transient Key set */
    bool                   updated: 1;                /**< Keys has been updated */
    bool                   ptk_eui_64_set: 1;         /**< Remote EUI-64 used to derive PTK is set */
} sec_prot_keys_t;

/**
 * sec_prot_keys_create allocates memory for security keys
 *
 * \param gtks GTK keys
 * \param cert_chain certificates
 *
 * \return security keys or NULL
 */
sec_prot_keys_t *sec_prot_keys_create(sec_prot_gtk_keys_t *gtks, const sec_prot_certs_t *certs);

/**
 * sec_prot_keys_init initialises security keys
 *
 * \param sec_keys security keys
 * \param gtks GTK keys
 * \param cert_chain certificates
 *
 */
void sec_prot_keys_init(sec_prot_keys_t *sec_keys, sec_prot_gtk_keys_t *gtks, const sec_prot_certs_t *certs);

/**
 * sec_prot_keys_delete frees security keys memory
 *
 * \param sec_keys security keys
 *
 */
void sec_prot_keys_delete(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_gtks_create allocates memory for GTK keys
 *
 * \return GTK keys or NULL
 *
 */
sec_prot_gtk_keys_t *sec_prot_keys_gtks_create(void);

/**
 * sec_prot_keys_gtks_init initialises GTK keys
 *
 * \param gtks GTK keys
 *
 */
void sec_prot_keys_gtks_init(sec_prot_gtk_keys_t *gtks);

/**
 * sec_prot_keys_gtks_delete frees GTK keys memory
 *
 * \param gtks GTK keys
 *
 */
void sec_prot_keys_gtks_delete(sec_prot_gtk_keys_t *gtks);

/**
 * sec_prot_keys_pmk_write writes Pairwise Master Key
 *
 * \param sec_keys security keys
 * \param pmk Pairwise Master Key
 *
 */
void sec_prot_keys_pmk_write(sec_prot_keys_t *sec_keys, uint8_t *pmk);

/**
 * sec_prot_keys_pmk_get gets Pairwise Master Key
 *
 * \param sec_keys security keys
 *
 * \return PMK or NULL
 *
 */
uint8_t *sec_prot_keys_pmk_get(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_pmk_replay_cnt_get gets PMK replay counter value
 *
 * \param sec_keys security keys
 *
 * \return replay counter value
 *
 */
uint64_t sec_prot_keys_pmk_replay_cnt_get(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_pmk_replay_cnt_set sets PMK replay counter value
 *
 * \param sec_keys security keys
 * \param counter new value for replay counter
 *
 */
void sec_prot_keys_pmk_replay_cnt_set(sec_prot_keys_t *sec_keys, uint64_t counter);

/**
 * sec_prot_keys_pmk_replay_cnt_increment increments PMK replay counter value by one
 *
 * \param sec_keys security keys
 *
 */
void sec_prot_keys_pmk_replay_cnt_increment(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_ptk_write writes Pairwise Transient Key
 *
 * \param sec_keys security keys
 * \param ptk Pairwise Transient Key
 *
 */
void sec_prot_keys_ptk_write(sec_prot_keys_t *sec_keys, uint8_t *ptk);

/**
 * sec_prot_keys_ptk_get gets Pairwise Transient Key
 *
 * \param sec_keys security keys
 *
 * \return PTK or NULL
 *
 */
uint8_t *sec_prot_keys_ptk_get(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_ptk_eui_64_set sets PTK EUI-64 storage
 *
 * \param sec_keys security keys
 * \param eui64 EUI-64 storage
 *
 */
void sec_prot_keys_ptk_eui_64_set(sec_prot_keys_t *sec_keys, uint8_t *eui_64);

/**
 * sec_prot_keys_ptk_eui_64_write writes PTK EUI-64
 *
 * \param sec_keys security keys
 * \param eui64 EUI-64
 *
 */
void sec_prot_keys_ptk_eui_64_write(sec_prot_keys_t *sec_keys, uint8_t *eui_64);

/**
 * sec_prot_keys_ptk_eui_64_get gets PTK EUI-64
 *
 * \param sec_keys security keys
 *
 * \return EUI-64 OR NULL
 *
 */
uint8_t *sec_prot_keys_ptk_eui_64_get(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_are_updated returns security keys have been updated flag
 *
 * \param sec_keys security keys
 *
 * \return TRUE keys have been updated, FALSE keys have not been updated
 *
 */
bool sec_prot_keys_are_updated(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_updated_reset resets security keys have been updated flag
 *
 * \param sec_keys security keys
 *
 *
 */
void sec_prot_keys_updated_reset(sec_prot_keys_t *sec_keys);

/**
 * sec_prot_keys_gtkl_get gets Group Transient Key liveness
 *
 * \param gtks GTK keys
 *
 * \return bit field indicating GTK liveness
 *
 */
uint8_t sec_prot_keys_gtkl_get(sec_prot_gtk_keys_t *gtks);

/**
 * sec_prot_keys_gtkl_set sets Group Transient Key liveness
 *
 * \param gtks GTK keys
 * \param gtkl bit field indicating GTK liveness
 *
 */
void sec_prot_keys_gtkl_set(sec_prot_gtk_keys_t *gtks, uint8_t gtkl);

/**
 * sec_prot_keys_gtk_is_live checks if Group Transient Key is live
 *
 * \param gtks GTK keys
 * \param index GTK index
 *
 * \return TRUE GTK is live, FALSE GTK is not live
 *
 */
bool sec_prot_keys_gtk_is_live(sec_prot_gtk_keys_t *gtks, uint8_t index);

/**
 * sec_prot_keys_gtk_insert_index_set sets index of GTK to be inserted
 *
 * \param gtks GTK keys
 * \param index GTK index
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t sec_prot_keys_gtk_insert_index_set(sec_prot_gtk_keys_t *gtks, uint8_t index);

/**
 * sec_prot_keys_gtk_insert_index_get gets index of GTK to be inserted
 *
 * \param gtks GTK keys
 *
 * \return >= 0 GTK index
 * \return < 0 no GTK to be inserted
 *
 */
int8_t sec_prot_keys_gtk_insert_index_get(sec_prot_gtk_keys_t *gtks);

/**
 * sec_prot_keys_gtk_insert_index_clear clears the index of GTK to be inserted
 *
 * \param gtks GTK keys
 *
 */
void sec_prot_keys_gtk_insert_index_clear(sec_prot_gtk_keys_t *gtks);

/**
 * sec_prot_keys_get_gtk_to_insert gets GTK that is marked to be inserted
 *
 * \param gtks GTK keys
 * \param index index of the returned GTK
 *
 * \return GTK or NULL
 *
 */
uint8_t *sec_prot_keys_get_gtk_to_insert(sec_prot_gtk_keys_t *gtks, uint8_t *index);

/**
 * sec_prot_keys_gtk_set sets Group Transient Key
 *
 * \param gtks GTK keys
 * \param index index
 * \param gtk gtk value
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t sec_prot_keys_gtk_set(sec_prot_gtk_keys_t *gtks, uint8_t index, uint8_t *gtk);

/**
 * sec_prot_keys_gtk_is_set checks if Group Transient Key is set
 *
 * \param gtks GTK keys
 * \param index index
 *
 * \return TRUE GTK is set, FALSE GTK is not set
 *
 */
bool sec_prot_keys_gtk_is_set(sec_prot_gtk_keys_t *gtks, uint8_t index);

/**
 * sec_prot_keys_gtk_gets gets Group Transient Key
 *
 * \param gtks GTK keys
 * \param index index
 *
 * \return GTK or NULL
 *
 */
uint8_t *sec_prot_keys_gtk_get(sec_prot_gtk_keys_t *gtks, uint8_t index);

/**
 * sec_prot_keys_gtk_lifetime_get gets GTK lifetime
 *
 * \param gtks GTK keys
 * \param index index
 *
 * \return GTK lifetime
 *
 */
uint32_t sec_prot_keys_gtk_lifetime_get(sec_prot_gtk_keys_t *gtks, uint8_t index);

/**
 * sec_prot_keys_gtk_lifetime_set sets GTK lifetime
 *
 * \param gtks GTK keys
 * \param index index
 * \param lifetime GTK lifetime
 *
 */
void sec_prot_keys_gtk_lifetime_set(sec_prot_gtk_keys_t *gtks, uint8_t index, uint32_t lifetime);

/**
 * sec_prot_keys_gtks_are_updated returns GTKs have been updated flag
 *
 * \param gtks GTK keys
 *
 * \return TRUE GTKs have been updated, FALSE GTKs have not been updated
 *
 */
bool sec_prot_keys_gtks_are_updated(sec_prot_gtk_keys_t *gtks);

/**
 * sec_prot_keys_gtks_updated_set sets GTKs have been updated flag
 *
 * \param gtks GTK keys
 *
 *
 */
void sec_prot_keys_gtks_updated_set(sec_prot_gtk_keys_t *gtks);

/**
 * sec_prot_keys_gtks_updated_set resets GTKs have been updated flag
 *
 * \param gtks GTK keys
 *
 *
 */
void sec_prot_keys_gtks_updated_reset(sec_prot_gtk_keys_t *gtks);

#endif /* SEC_PROT_KEYS_H_ */
