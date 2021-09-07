/*
 * Copyright (c) 2018-2020, Pelion and affiliates.
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

#ifndef WS_IE_LIB_H_
#define WS_IE_LIB_H_

struct ws_pan_information_s;
struct ws_utt_ie;
struct ws_bt_ie;
struct ws_us_ie;
struct ws_hopping_schedule_s;
struct ws_fc_ie;

/**
 * @brief ws_wp_network_name_t WS nested payload network name
 */
typedef struct ws_wp_network_name {
    uint8_t network_name_length;
    uint8_t *network_name;
} ws_wp_network_name_t;

/* WS_WH HEADER IE */
uint8_t *ws_wh_utt_write(uint8_t *ptr, uint8_t message_type);
uint8_t *ws_wh_bt_write(uint8_t *ptr);
uint8_t *ws_wh_fc_write(uint8_t *ptr, struct ws_fc_ie *fc_ie);
uint8_t *ws_wh_rsl_write(uint8_t *ptr, uint8_t rsl);
uint8_t *ws_wh_vh_write(uint8_t *ptr, uint8_t *vendor_header, uint8_t vendor_header_length);
uint8_t *ws_wh_ea_write(uint8_t *ptr, uint8_t *eui64);

bool ws_wh_utt_read(uint8_t *data, uint16_t length, struct ws_utt_ie *utt_ie);
bool ws_wh_bt_read(uint8_t *data, uint16_t length, struct ws_bt_ie *bt_ie);
bool ws_wh_fc_read(uint8_t *data, uint16_t length, struct ws_fc_ie *fc_ie);
bool ws_wh_rsl_read(uint8_t *data, uint16_t length, int8_t *rsl);
bool ws_wh_ea_read(uint8_t *data, uint16_t length, uint8_t *eui64);

/* WS_WP_NESTED PAYLOD IE */
uint8_t *ws_wp_base_write(uint8_t *ptr, uint16_t length);
uint8_t *ws_wp_nested_hopping_schedule_write(uint8_t *ptr, struct ws_hopping_schedule_s *hopping_schedule, bool unicast_schedule);
uint8_t *ws_wp_nested_vp_write(uint8_t *ptr, uint8_t *vendor_payload, uint16_t vendor_payload_length);
uint8_t *ws_wp_nested_pan_info_write(uint8_t *ptr, struct ws_pan_information_s *pan_congiguration);
uint8_t *ws_wp_nested_netname_write(uint8_t *ptr, uint8_t *network_name, uint8_t network_name_length);
uint8_t *ws_wp_nested_pan_ver_write(uint8_t *ptr, struct ws_pan_information_s *pan_congiguration);
uint8_t *ws_wp_nested_gtkhash_write(uint8_t *ptr, uint8_t *gtkhash, uint8_t gtkhash_length);
uint16_t ws_wp_nested_hopping_schedule_length(struct ws_hopping_schedule_s *hopping_schedule, bool unicast_schedule);

bool ws_wp_nested_us_read(uint8_t *data, uint16_t length, struct ws_us_ie *us_ie);
bool ws_wp_nested_bs_read(uint8_t *data, uint16_t length, struct ws_bs_ie *bs_ie);
bool ws_wp_nested_pan_read(uint8_t *data, uint16_t length, struct ws_pan_information_s *pan_congiguration);
bool ws_wp_nested_pan_version_read(uint8_t *data, uint16_t length, uint16_t *pan_version);
bool ws_wp_nested_network_name_read(uint8_t *data, uint16_t length, ws_wp_network_name_t *network_name);
uint8_t *ws_wp_nested_gtkhash_read(uint8_t *data, uint16_t length);


#endif /* WS_IE_LIB_H_ */
