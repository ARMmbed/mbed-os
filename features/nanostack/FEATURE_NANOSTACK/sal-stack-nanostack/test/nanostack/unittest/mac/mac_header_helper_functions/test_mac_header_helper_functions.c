/*
 * Copyright (c) 2016, Arm Limited and affiliates.
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
#include "test_mac_header_helper_functions.h"
#include <string.h>
#include "inttypes.h"
#include "mac_common_defines.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/rf_driver_storage.h"
#include "nsdynmemLIB.h"
#include "nsdynmemLIB_stub.h"

#include "common_functions_stub.h"

bool test_mac_security_mic_length_get()
{
    if( 0 != mac_security_mic_length_get(0)){
        return false;
    }
    if( 4 != mac_security_mic_length_get(1)){
        return false;
    }
    if( 8 != mac_security_mic_length_get(2)){
        return false;
    }
    if( 16 != mac_security_mic_length_get(3)){
        return false;
    }
    if( 0 != mac_security_mic_length_get(4)){
        return false;
    }
    if( 4 != mac_security_mic_length_get(5)){
        return false;
    }
    if( 8 != mac_security_mic_length_get(6)){
        return false;
    }
    if( 16 != mac_security_mic_length_get(7)){
        return false;
    }
    return true;
}

bool test_mac_header_security_aux_header_length()
{
    if( 0 != mac_header_security_aux_header_length(0, 0)){
        return false;
    }
    if( 14 != mac_header_security_aux_header_length(1, MAC_KEY_ID_MODE_SRC8_IDX)){
        return false;
    }
    if( 10 != mac_header_security_aux_header_length(1, MAC_KEY_ID_MODE_SRC4_IDX)){
        return false;
    }
    if( 6 != mac_header_security_aux_header_length(1, MAC_KEY_ID_MODE_IDX)){
        return false;
    }
    if( 5 != mac_header_security_aux_header_length(1, MAC_KEY_ID_MODE_IMPLICIT)){
        return false;
    }

    return true;
}

bool test_mac_header_address_length()
{
    mac_fcf_sequence_t seq;
    if( 0 != mac_header_address_length(NULL)){
        return false;
    }
    seq.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    seq.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    seq.intraPan = 0;
    if( 8 != mac_header_address_length(&seq)){
        return false;
    }

    seq.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    seq.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    if( 20 != mac_header_address_length(&seq)){
        return false;
    }

    seq.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    seq.SrcAddrMode = MAC_ADDR_MODE_NONE;
    if( 4 != mac_header_address_length(&seq)){
        return false;
    }

    seq.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    seq.SrcAddrMode = MAC_ADDR_MODE_NONE;
    if( 10 != mac_header_address_length(&seq)){
        return false;
    }

    seq.DstAddrMode = MAC_ADDR_MODE_NONE;
    seq.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    if( 4 != mac_header_address_length(&seq)){
        return false;
    }

    seq.DstAddrMode = MAC_ADDR_MODE_NONE;
    seq.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    if( 10 != mac_header_address_length(&seq)){
        return false;
    }

    return true;
}

bool test_mac_header_security_parameter_set()
{
    mac_header_security_parameter_set(NULL, NULL);
    mac_aux_security_header_t header;
    memset(&header, 0, sizeof(mac_aux_security_header_t));
    mlme_security_t sec;
    sec.SecurityLevel = 1;
    sec.KeyIdMode = MAC_KEY_ID_MODE_IMPLICIT;
    mac_header_security_parameter_set(&header, &sec);
    if( header.securityLevel != 1 ){
        return false;
    }

    memset(&sec.Keysource, 5, 8);
    sec.KeyIdMode = MAC_KEY_ID_MODE_IDX;
    mac_header_security_parameter_set(&header, &sec);
    if( header.Keysource[0] != 0 ){
        return false;
    }

    memset(&sec.Keysource, 6, 8);
    sec.KeyIdMode = MAC_KEY_ID_MODE_SRC4_IDX;
    mac_header_security_parameter_set(&header, &sec);
    if( header.Keysource[0] != 6 || header.Keysource[6] != 0){
        return false;
    }

    memset(&sec.Keysource, 7, 8);
    sec.KeyIdMode = MAC_KEY_ID_MODE_SRC8_IDX;
    mac_header_security_parameter_set(&header, &sec);
    if( header.Keysource[7] != 7 ){
        return false;
    }

    return true;
}

bool test_mac_header_parse_fcf_dsn()
{
    mac_header_parse_fcf_dsn(NULL, NULL);

    mac_fcf_sequence_t header;
    uint8_t ptr[3];
    ptr[2] = 7;
    common_functions_stub.uint16_value = 0x987F;
    mac_header_parse_fcf_dsn(&header, &ptr);

    if( header.DSN != 7 ){
        return false;
    }
    if( header.frametype != 7 ){
        return false;
    }
    if( !header.securityEnabled ){
        return false;
    }
    if( !header.framePending ){
        return false;
    }
    if( !header.ackRequested ){
        return false;
    }
    if( !header.intraPan ){
        return false;
    }
    if( header.DstAddrMode != 2 ){
        return false;
    }
    if( header.frameVersion != 1 ){
        return false;
    }
    if( header.SrcAddrMode != 2 ){
        return false;
    }
    return true;
}

bool test_mac_header_security_components_read()
{
    mac_header_security_components_read(NULL, NULL);
    mac_pre_parsed_frame_t frame;
    memset(&frame, 0, sizeof(mac_pre_parsed_frame_t));
    mlme_security_t sec;
    mac_header_security_components_read(&frame, &sec);

    frame.fcf_dsn.securityEnabled = true;
    mac_header_security_components_read(&frame, &sec);

    nsdynmemlib_stub.returnCounter = 1;
    mac_pre_parsed_frame_t *frame2 = ns_dyn_mem_alloc(sizeof(mac_pre_parsed_frame_t)+13);
    memset(frame2, 0, sizeof(mac_pre_parsed_frame_t)+13);
    frame2->buf[0] = MAC_KEY_ID_MODE_IMPLICIT << 3 + 1;
    frame2->fcf_dsn.securityEnabled = true;
    mac_header_security_components_read(frame2, &sec);
    if( sec.KeyIdMode != MAC_KEY_ID_MODE_IMPLICIT ) {
        return false;
    }

    frame2->buf[0] = (MAC_KEY_ID_MODE_IDX << 3) + 1;
    frame2->buf[5] = 5;
    mac_header_security_components_read(frame2, &sec);
    if( sec.KeyIndex != 5 ) {
        return false;
    }

    frame2->buf[0] = (MAC_KEY_ID_MODE_SRC4_IDX << 3) + 1;
    frame2->buf[5] = 6;
    mac_header_security_components_read(frame2, &sec);
    if( sec.Keysource[0] != 6 ) {
        return false;
    }

    frame2->buf[5] = 7;
    frame2->buf[9] = 9;
    frame2->buf[0] = (MAC_KEY_ID_MODE_SRC8_IDX << 3) + 1;
    mac_header_security_components_read(frame2, &sec);
    if( sec.Keysource[0] != 7 || sec.Keysource[4] != 9 ) {
        return false;
    }

    ns_dyn_mem_free(frame2);
    return true;
}

bool test_mac_header_get_src_panid()
{
    if( 0 != mac_header_get_src_panid(NULL, NULL) ){
        return false;
    }
    mac_fcf_sequence_t seq;
    uint8_t ptr[15];
    memset(&ptr, 0, 15);
    ptr[3] = 3;
    ptr[4] = 0;
    seq.intraPan = false;
    seq.DstAddrMode = MAC_ADDR_MODE_NONE;
    if( 3 != mac_header_get_src_panid(&seq, &ptr) ){
        return false;
    }

    ptr[7] = 4;
    ptr[8] = 0;
    seq.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    if( 4 != mac_header_get_src_panid(&seq, &ptr) ){
        return false;
    }

    ptr[13] = 5;
    ptr[14] = 0;
    seq.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    if( 5 != mac_header_get_src_panid(&seq, &ptr) ){
        return false;
    }

    return true;
}

bool test_mac_header_get_dst_panid()
{
    if( 0 != mac_header_get_dst_panid(NULL, NULL) ){
        return false;
    }
    mac_fcf_sequence_t seq;
    uint8_t ptr[5];
    memset(&ptr, 0, 5);
    ptr[3] = 3;
    ptr[4] = 0;
    seq.DstAddrMode = MAC_ADDR_MODE_NONE;
    uint16_t ret = mac_header_get_dst_panid(&seq, &ptr);
    if( 65535 != ret ){
        return false;
    }

    seq.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    if( 3 != mac_header_get_dst_panid(&seq, &ptr) ){
        return false;
    }

    return true;
}

bool test_mac_header_get_src_address()
{
    mac_header_get_src_address(NULL, NULL, NULL);

    mac_fcf_sequence_t seq;
    uint8_t ptr[25];
    memset(&ptr, 0, 25);
    uint8_t addr[15];
    memset(&addr, 0, 15);
    seq.intraPan = false;
    seq.DstAddrMode = MAC_ADDR_MODE_NONE;
    seq.SrcAddrMode = MAC_ADDR_MODE_NONE;
    mac_header_get_src_address(&seq, &ptr, &addr);

    seq.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    seq.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    ptr[9] = 5;
    mac_header_get_src_address(&seq, &ptr, &addr);
    if( 5 != addr[1] ){
        return false;
    }

    ptr[15] = 15;
    seq.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    seq.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    mac_header_get_src_address(&seq, &ptr, &addr);
    if( 15 != addr[7] ){
        return false;
    }

    return true;
}

bool test_mac_header_get_dst_address()
{
    mac_header_get_dst_address(NULL, NULL, NULL);
    mac_fcf_sequence_t seq;
    uint8_t ptr[25];
    memset(&ptr, 0, 25);
    uint8_t addr[15];
    memset(&addr, 0, 15);
    seq.DstAddrMode = MAC_ADDR_MODE_NONE;
    mac_header_get_dst_address(&seq, &ptr, &addr);

    ptr[5] = 5;
    seq.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    mac_header_get_dst_address(&seq, &ptr, &addr);
    if( 5 != addr[1] ){
        return false;
    }

    ptr[5] = 15;
    seq.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    mac_header_get_dst_address(&seq, &ptr, &addr);
    if( 15 != addr[7] ){
        return false;
    }

    return true;
}

bool test_mcps_payload_length_from_received_frame()
{
    if( 0 != mcps_payload_length_from_received_frame(NULL) ){
        return false;
    }
    mac_pre_parsed_frame_t frame;
    frame.mac_payload_length = 2;
    if( 2 != mcps_payload_length_from_received_frame(&frame) ){
        return false;
    }

    return true;
}

bool test_mcps_mac_header_length_from_received_frame()
{
    if( 0 != mcps_mac_header_length_from_received_frame(NULL) ){
        return false;
    }
    mac_pre_parsed_frame_t frame;
    frame.mac_header_length = 1;
    frame.security_aux_header_length = 1;
    if( 2 != mcps_mac_header_length_from_received_frame(&frame) ){
        return false;
    }
    return true;
}

bool test_mcps_mac_security_frame_counter_read()
{
    uint32_t ret = mcps_mac_security_frame_counter_read(NULL);
    if( 0xffffffff != ret ){
        return false;
    }
    mac_pre_parsed_frame_t frame;
    frame.mac_header_length = 0;
    frame.fcf_dsn.securityEnabled = true;
    common_functions_stub.uint32_value = 0x5454;
    ret = mcps_mac_security_frame_counter_read(&frame);
    if( 0x5454 != ret ){
        return false;
    }

    return true;
}

bool test_mcps_mac_command_frame_id_get()
{
    if( NULL != mcps_mac_command_frame_id_get(NULL) ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    mac_pre_parsed_frame_t *frame2 = ns_dyn_mem_alloc(sizeof(mac_pre_parsed_frame_t)+13);
    memset(frame2, 0, sizeof(mac_pre_parsed_frame_t)+13);
    frame2->mac_header_length = 0;
    frame2->security_aux_header_length = 0;
    frame2->buf[0] = 2;
    if( NULL == mcps_mac_command_frame_id_get(frame2) ){
        return false;
    }
    ns_dyn_mem_free(frame2);
    return true;
}

bool test_mcps_mac_payload_pointer_get()
{
    if( NULL != mcps_mac_payload_pointer_get(NULL) ){
        return false;
    }

    nsdynmemlib_stub.returnCounter = 1;
    mac_pre_parsed_frame_t *frame2 = ns_dyn_mem_alloc(sizeof(mac_pre_parsed_frame_t)+13);
    memset(frame2, 0, sizeof(mac_pre_parsed_frame_t)+13);
    frame2->mac_header_length = 0;
    frame2->security_aux_header_length = 0;
    frame2->buf[0] = 2;
    if( NULL == mcps_mac_payload_pointer_get(frame2) ){
        return false;
    }
    ns_dyn_mem_free(frame2);

    return true;
}

bool test_mcps_security_mic_pointer_get()
{
    if( NULL != mcps_security_mic_pointer_get(NULL) ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    mac_pre_parsed_frame_t *frame2 = ns_dyn_mem_alloc(sizeof(mac_pre_parsed_frame_t)+13);
    memset(frame2, 0, sizeof(mac_pre_parsed_frame_t)+13);
    frame2->mac_payload_length = 0;
    frame2->buf[0] = 2;
    if( NULL == mcps_security_mic_pointer_get(frame2) ){
        return false;
    }
    ns_dyn_mem_free(frame2);
    return true;
}

bool test_mcps_mac_security_aux_header_start_pointer_get()
{
    if( NULL != mcps_mac_security_aux_header_start_pointer_get(NULL) ){
        return false;
    }
    nsdynmemlib_stub.returnCounter = 1;
    mac_pre_parsed_frame_t *frame2 = ns_dyn_mem_alloc(sizeof(mac_pre_parsed_frame_t)+13);
    memset(frame2, 0, sizeof(mac_pre_parsed_frame_t)+13);
    frame2->mac_header_length = 0;
    frame2->fcf_dsn.securityEnabled = true;
    frame2->buf[0] = 2;
    if( NULL == mcps_mac_security_aux_header_start_pointer_get(frame2) ){
        return false;
    }

    ns_dyn_mem_free(frame2);
    return true;
}

static int test_pointer_move_length(uint8_t *new, uint8_t *old)
{
    return (int)(new - old);
}

bool test_mcps_generic_header_write()
{
    uint8_t buffer_ptr[30];
    uint8_t *ptr;
    nsdynmemlib_stub.returnCounter = 1;
    mac_pre_build_frame_t *frame2 = ns_dyn_mem_alloc(sizeof(mac_pre_build_frame_t));
    memset(frame2, 0, sizeof(mac_pre_build_frame_t));

    frame2->fcf_dsn.DSN = 1;
    frame2->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
    frame2->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    frame2->fcf_dsn.intraPan = true;
    ptr = mcps_generic_header_write(buffer_ptr, frame2);
    //3+4+2
    if (test_pointer_move_length(ptr, buffer_ptr) != 9) {
        return false;
    }

    frame2->fcf_dsn.intraPan = false;
    ptr = mcps_generic_header_write(buffer_ptr, frame2);
    //3+4+4
    if (test_pointer_move_length(ptr, buffer_ptr) != 11) {
        return false;
    }
    frame2->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_64_BIT;
    ptr = mcps_generic_header_write(buffer_ptr, frame2);
    //3+10+4
    if (test_pointer_move_length(ptr, buffer_ptr) != 17) {
        return false;
    }

    frame2->fcf_dsn.intraPan = true;
    ptr = mcps_generic_header_write(buffer_ptr, frame2);
    //3+10+2
    if (test_pointer_move_length(ptr, buffer_ptr) != 15) {
        return false;
    }
    frame2->fcf_dsn.intraPan = false;
    frame2->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;
    ptr = mcps_generic_header_write(buffer_ptr, frame2);
    //3+4
    if (test_pointer_move_length(ptr, buffer_ptr) != 7) {
        return false;
    }
    frame2->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    ptr = mcps_generic_header_write(buffer_ptr, frame2);
    //3+10
    if (test_pointer_move_length(ptr, buffer_ptr) != 13) {
        return false;
    }

    ns_dyn_mem_free(frame2);
    return true;

}
