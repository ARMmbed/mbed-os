/*
 * Copyright (c) 2013-2015, 2017, Pelion and affiliates.
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

#ifndef PANA_RELAY_TABLE_H_
#define PANA_RELAY_TABLE_H_

typedef struct relay_table_entry_t {
    uint8_t                         relay_address[16];                  /*!< destination address */
    uint8_t                         parent_address[16];                 /*!< destination address */
    uint16_t                        parent_relay_port;
    uint8_t                         ttl;                                /*!< destination TTL */
    struct relay_table_entry_t    *next_info;                           /*!< Pointer to Next Route Information If is 0xff then this entry last one */
    struct relay_table_entry_t     *prev_info;                          /*!< Pointer to Next Route InformationIf this is 0xff then this entry is first one */
} relay_table_entry_t;

extern void pana_relay_table_init(void);
extern relay_table_entry_t *pana_relay_table_update(uint8_t *address);
extern relay_table_entry_t *pana_relay_detect(uint8_t *address);
extern void pana_relay_ttl_update(uint16_t ttl_time);

#endif /* PANA_RELAY_TABLE_H_ */
