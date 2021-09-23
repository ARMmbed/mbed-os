/*
 * Copyright (c) 2016-2018, Pelion and affiliates.
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


#ifndef MAC_HEADER_HELPER_FUNCTIONS_H_
#define MAC_HEADER_HELPER_FUNCTIONS_H_

struct mac_fcf_sequence_s;
struct mlme_security_s;
struct mac_pre_parsed_frame_s;
struct mac_pre_build_frame;
struct mac_aux_security_header_s;
struct ns_ie_iovec;
struct protocol_interface_rf_mac_setup;

uint8_t mac_security_mic_length_get(uint8_t security_level);
uint8_t mac_header_security_aux_header_length(uint8_t security_level, uint8_t keyIdmode);
bool mac_dst_panid_present(const struct mac_fcf_sequence_s *header);
bool mac_src_panid_present(const struct mac_fcf_sequence_s *header);
uint8_t mac_header_address_length(const struct mac_fcf_sequence_s *fcf);

void mac_header_security_parameter_set(struct mac_aux_security_header_s *header, const struct mlme_security_s *frame_setup);

/**
 * MAC layer FCF and data sequence parse from data
 */
const uint8_t *mac_header_parse_fcf_dsn(struct mac_fcf_sequence_s *header, const uint8_t *ptr);

uint16_t mac_header_off_set_to_aux_header(const struct mac_fcf_sequence_s *fcf);

void mac_header_security_aux_header_parse(const uint8_t *ptr, struct mlme_security_s *security_params);

void mac_header_security_components_read(struct mac_pre_parsed_frame_s *buffer, struct mlme_security_s *security_params);

bool mac_header_information_elements_parse(struct mac_pre_parsed_frame_s *buffer);

bool mac_payload_information_elements_parse(struct mac_pre_parsed_frame_s *buffer);

uint16_t mac_header_information_elements_length(struct mac_pre_build_frame *buffer);

void mac_header_information_elements_preparation(struct mac_pre_build_frame *buffer);

uint16_t mac_buffer_total_payload_length(struct mac_pre_build_frame *buffer);

/**
 *  Next function should only call when address mode is not MAC_ADDR_MODE_NONE and parsed proper header!
 */
uint16_t mac_header_get_src_panid(const struct mac_fcf_sequence_s *header, const uint8_t *ptr, uint16_t configured_pan_id);
uint16_t mac_header_get_dst_panid(const struct mac_fcf_sequence_s *header, const uint8_t *ptr, uint16_t configured_pan_id);
void mac_header_get_src_address(const struct mac_fcf_sequence_s *header, const uint8_t *ptr, uint8_t *address_ptr);
void mac_header_get_dst_address(const struct mac_fcf_sequence_s *header, const uint8_t *ptr, uint8_t *address_ptr);
uint8_t mac_address_length(uint8_t address_mode);
uint8_t mcps_mac_header_length_from_received_frame(const struct mac_pre_parsed_frame_s *buffer);
uint32_t mcps_mac_security_frame_counter_read(const struct mac_pre_parsed_frame_s *buffer);
uint8_t mcps_mac_command_frame_id_get(const struct mac_pre_parsed_frame_s *buffer);
uint8_t *mcps_mac_payload_pointer_get(const struct mac_pre_parsed_frame_s *buffer);
uint8_t *mcps_security_mic_pointer_get(const struct mac_pre_parsed_frame_s *buffer);
/* Write Mac Header and payload */
uint8_t *mac_generic_packet_write(struct protocol_interface_rf_mac_setup *rf_ptr, uint8_t *ptr, const struct mac_pre_build_frame *buffer);


/** get pointer to Mac header start point*/
#define mac_header_message_start_pointer(x)  (&(x)->buf[0])


#endif /* MAC_HEADER_HELPER_FUNCTIONS_H_ */
