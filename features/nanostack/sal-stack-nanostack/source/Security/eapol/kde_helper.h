/*
 * Copyright (c) 2018-2019, Pelion and affiliates.
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

#ifndef KDE_HELPER_H_
#define KDE_HELPER_H_

/*
 * EAPOL KDE helper functions
 *
 */

#define KDE_GTK_LEN              6 + 2 + 16
#define KDE_PMKID_LEN            6 + 16
#define KDE_PTKID_LEN            6 + 16
#define KDE_LIFETIME_LEN         6 + 4
#define KDE_GTKL_LEN             6 + 1

/**
 * kde_padded_length_calc calculates padded length for kde (see IEEE 802.11 chapter 11.6.2 EAPOL-Key frames)
 *
 * \param kde_length length without padding
 *
 * \return padded length
 */
uint16_t kde_padded_length_calc(uint16_t kde_length);

/**
 * kde_padding_write writes padded bytes
 *
 * \param start_ptr first byte of the padding
 * \param end_ptr last bytes next byte
 *
 */
void kde_padding_write(uint8_t *start_ptr, uint8_t *end_ptr);

/**
 * kde_gtk_write writes GTK
 *
 * \param ptr pointer where to write
 * \param key_id key identifier (index)
 * \param gtk GTK
 *
 * return incremented write pointer
 *
 */
uint8_t *kde_gtk_write(uint8_t *ptr, uint8_t key_id, const uint8_t *gtk);

/**
 * kde_pmkid_write writes PMKID
 *
 * \param ptr pointer where to write
 * \param pmkid PMKID
 *
 * return incremented write pointer
 *
 */
uint8_t *kde_pmkid_write(uint8_t *ptr, const uint8_t *pmkid);

/**
 * kde_ptkid_write writes PTKID
 *
 * \param ptr pointer where to write
 * \param pmkid PTKID
 *
 * return incremented write pointer
 *
 */
uint8_t *kde_ptkid_write(uint8_t *ptr, const uint8_t *ptkid);

/**
 * kde_lifetime_write writes GTK lifetime
 *
 * \param ptr pointer where to write
 * \param lifetime GTK lifetime
 *
 * return incremented write pointer
 *
 */
uint8_t *kde_lifetime_write(uint8_t *ptr, uint32_t lifetime);

/**
 * kde_gtkl_write writes GTK liveness information
 *
 * \param ptr pointer where to write
 * \param gtkl GTK liveness bit field
 *
 * return incremented write pointer
 *
 */
uint8_t *kde_gtkl_write(uint8_t *ptr, uint8_t gtkl);

/**
 *  kde_gtk_read reads GTK
 *
 * \param ptr pointer where to read
 * \param len length of the remaining data
 * \param key_id key identifier (index)
 * \param gtk GTK
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kde_gtk_read(const uint8_t *ptr, uint16_t len, uint8_t *key_id, uint8_t *gtk);

/**
 *  kde_pmkid_read reads PMKID
 *
 * \param ptr pointer where to read
 * \param len length of the remaining data
 * \param pmkid PMKID
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kde_pmkid_read(const uint8_t *ptr, uint16_t len, uint8_t *pmkid);

/**
 *  kde_ptkid_read reads PTKID
 *
 * \param ptr pointer where to read
 * \param len length of the remaining data
 * \param ptkid PTKID
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kde_ptkid_read(const uint8_t *ptr, uint16_t len, uint8_t *ptkid);

/**
 *  kde_lifetime_read reads GTK lifetime
 *
 * \param ptr pointer where to read
 * \param len length of the remaining data
 * \param lifetime GTK lifetime
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kde_lifetime_read(const uint8_t *ptr, uint16_t len, uint32_t *lifetime);

/**
 *  kde_gtkl_read reads GTK liveness information
 *
 * \param ptr pointer where to read
 * \param len length of the remaining data
 * \param gtkl GTK liveness bit field
 *
 * \return < 0 failure
 * \return >= 0 success
 *
 */
int8_t kde_gtkl_read(const uint8_t *ptr, uint16_t len, uint8_t *gtkl);

#endif /* KDE_HELPER_H_ */
