/*
 * Copyright (c) 2014-2017, Arm Limited and affiliates.
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

/*
 * \file mac_mcps_sap.c
 * \brief Add short description about this file!!!
 *
 */
#include "nsconfig.h"
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "string.h"
#include "ns_trace.h"
#include "nsdynmemLIB.h"
#include "ccmLIB.h"
#include "mlme.h"
#include "mac_api.h"
#include "fhss_api.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"

#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_filter.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/rf_driver_storage.h"

#include "sw_mac.h"

#define TRACE_GROUP "mMCp"

typedef struct {
    uint8_t address[8];
    unsigned addr_type:2;
    uint8_t nonce_ptr[8];
    uint32_t frameCounter;
    uint8_t keyId;
}neighbour_security_update_t;

void mcps_sap_pd_req_queue_write(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buffer);
static mac_pre_build_frame_t * mcps_sap_pd_req_queue_read(protocol_interface_rf_mac_setup_s *rf_mac_setup, bool is_bc_queue, bool flush);
static int8_t mcps_pd_data_request(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_build_frame_t *buffer);
static void mcps_data_confirm_handle(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_build_frame_t *buffer);
static void mac_set_active_event(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t event_type);
static void mac_clear_active_event(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t event_type);
static bool mac_read_active_event(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t event_type);

static int8_t mac_tasklet_event_handler = -1;


static void mac_data_poll_radio_disable_check(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if (rf_mac_setup->macCapRxOnIdle || rf_mac_setup->macWaitingData || rf_mac_setup->scan_active) {
        return;
    }

    if (!rf_mac_setup->macRfRadioTxActive ) {
        mac_mlme_mac_radio_disabled(rf_mac_setup);
    }
}

static void mcps_data_confirm_cb(protocol_interface_rf_mac_setup_s *rf_mac_setup, mcps_data_conf_t *confirm)
{

    mac_data_poll_radio_disable_check(rf_mac_setup);

    if( get_sw_mac_api(rf_mac_setup) ) {
        get_sw_mac_api(rf_mac_setup)->data_conf_cb(get_sw_mac_api(rf_mac_setup), confirm);
    }
}

void mcps_sap_data_req_handler(protocol_interface_rf_mac_setup_s *rf_mac_setup , const mcps_data_req_t *data_req ) {
    uint8_t status = MLME_SUCCESS;
    mac_pre_build_frame_t *buffer = NULL;

    if (!rf_mac_setup->mac_security_enabled) {
        if (data_req->Key.SecurityLevel) {
            status = MLME_UNSUPPORTED_SECURITY;
            goto verify_status;
        }
    }

    if (data_req->msduLength > rf_mac_setup->dev_driver->phy_driver->phy_MTU - MAC_DATA_PACKET_MIN_HEADER_LENGTH) {
        tr_debug("packet %u, %u",data_req->msduLength, rf_mac_setup->dev_driver->phy_driver->phy_MTU);
        status = MLME_FRAME_TOO_LONG;
        goto verify_status;
    }
    //protocol_interface_rf_mac_setup_s *rf_mac_setup = (protocol_interface_rf_mac_setup_s*)api;
    buffer = mcps_sap_prebuild_frame_buffer_get(0);
    //tr_debug("Data Req");
    if (!buffer) {
        //Make Confirm Here
        status = MLME_TRANSACTION_OVERFLOW;
        goto verify_status;
    }

    if (!rf_mac_setup->macUpState || rf_mac_setup->scan_active) {
        status = MLME_TRX_OFF;
        tr_debug("Drop MAC tx packet when mac disabled");
        goto verify_status;
    }
    buffer->upper_layer_request = true;
    buffer->fcf_dsn.frametype = FC_DATA_FRAME;
    buffer->fcf_dsn.ackRequested = data_req->TxAckReq;

    buffer->mac_header_length_with_security = 3;
    mac_header_security_parameter_set(&buffer->aux_header, &data_req->Key);
    buffer->security_mic_len = mac_security_mic_length_get(buffer->aux_header.securityLevel);
    if (buffer->aux_header.securityLevel) {
        buffer->fcf_dsn.frameVersion = MAC_FRAME_VERSION_2006;
        buffer->fcf_dsn.securityEnabled = true;
    }

    buffer->mac_header_length_with_security += mac_header_security_aux_header_length(buffer->aux_header.securityLevel, buffer->aux_header.KeyIdMode);

    buffer->msduHandle = data_req->msduHandle;
    buffer->fcf_dsn.DstAddrMode = data_req->DstAddrMode;
    memcpy(buffer->DstAddr, data_req->DstAddr, 8);
    buffer->DstPANId = data_req->DstPANId;
    buffer->SrcPANId = mac_mlme_get_panid(rf_mac_setup);
    buffer->fcf_dsn.SrcAddrMode = data_req->SrcAddrMode;
    buffer->fcf_dsn.framePending = data_req->PendingBit;

    if (buffer->fcf_dsn.SrcAddrMode == MAC_ADDR_MODE_NONE) {
        if (buffer->fcf_dsn.DstAddrMode == MAC_ADDR_MODE_NONE) {
            status = MLME_INVALID_ADDRESS;
            goto verify_status;
        }

        if (rf_mac_setup->shortAdressValid) {
            buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
        } else {
            buffer->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
        }
    }

    mac_frame_src_address_set_from_interface(buffer->fcf_dsn.SrcAddrMode, rf_mac_setup, buffer->SrcAddr);

    if (buffer->DstPANId == buffer->SrcPANId) {
        buffer->fcf_dsn.intraPan = true;
    }

    //Calculate address length
    buffer->mac_header_length_with_security += mac_header_address_length(&buffer->fcf_dsn);
    buffer->mac_payload = data_req->msdu;
    buffer->mac_payload_length = data_req->msduLength;
    //check that header + payload length is not bigger than MAC MTU
    if (data_req->InDirectTx) {
        mac_indirect_queue_write(rf_mac_setup, buffer);
    } else {
        mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);
    }

    verify_status:
    if (status != MLME_SUCCESS){
        mcps_data_conf_t confirm;
        memset(&confirm, 0, sizeof(mcps_data_conf_t));
        confirm.msduHandle = data_req->msduHandle;
        confirm.status = status;
        tr_debug("DATA REQ Fail %u", status);
        mcps_sap_prebuild_frame_buffer_free(buffer);
        mcps_data_confirm_cb(rf_mac_setup, &confirm);
    }
}

static int8_t mac_virtual_data_req_handler(protocol_interface_rf_mac_setup_s *rf_mac_setup , const uint8_t *data_ptr, uint16_t data_length) {

    if (!rf_mac_setup->macUpState || data_length > rf_mac_setup->dev_driver->phy_driver->phy_MTU) {
        return -1;
    }

    //protocol_interface_rf_mac_setup_s *rf_mac_setup = (protocol_interface_rf_mac_setup_s*)api;
    mac_pre_build_frame_t *buffer = mcps_sap_prebuild_frame_buffer_get(data_length);
    //tr_debug("Data Req");
    if (!buffer) {
        //Make Confirm Here
        return -1;
    }

    mac_header_parse_fcf_dsn(&buffer->fcf_dsn, data_ptr);
    // Use MAC sequence as handle
    buffer->msduHandle = data_ptr[2];
    memcpy(buffer->mac_payload,data_ptr,  data_length);
    buffer->mac_payload_length = data_length;

    // Read destination MAC address from MAC header
    mac_header_get_dst_address(&buffer->fcf_dsn, data_ptr, buffer->DstAddr);

    mcps_sap_pd_req_queue_write(rf_mac_setup, buffer);

    if (!buffer->fcf_dsn.ackRequested) {
        phy_device_driver_s *driver = rf_mac_setup->tun_extension_rf_driver->phy_driver;
        driver->phy_tx_done_cb(rf_mac_setup->tun_extension_rf_driver->id, 1, PHY_LINK_TX_SUCCESS, 1, 1);
    }
    return 0;
}

static int8_t mac_virtual_mlme_nap_req_handler(protocol_interface_rf_mac_setup_s *rf_mac_setup , const arm_mlme_req_t *mlme_req)
{
    const uint8_t *ptr = mlme_req->mlme_ptr;
    switch(mlme_req->primitive){
        case MLME_SCAN:{
            if (mlme_req->ptr_length != 47) {
                return -1;
            }

            mlme_scan_t mlme_scan_req;
            mlme_scan_req.ScanType = *ptr++;
            mlme_scan_req.ScanChannels.channel_page = *ptr++;
            memcpy(mlme_scan_req.ScanChannels.channel_mask, ptr, 32);
            ptr += 32;
            mlme_scan_req.ScanDuration = *ptr++;
            mlme_scan_req.ChannelPage = *ptr++;
            mlme_scan_req.Key.SecurityLevel = *ptr++;
            mlme_scan_req.Key.KeyIdMode = *ptr++;
            mlme_scan_req.Key.KeyIndex = *ptr++;
            memcpy(mlme_scan_req.Key.Keysource, ptr, 8);
            mac_mlme_scan_request(&mlme_scan_req, rf_mac_setup);
            return 0;
            break;
        }
        case MLME_SET:{
            if (mlme_req->ptr_length < 3) {
                return -1;
            }
            mlme_set_t mlme_set_req;
            mlme_set_req.attr = *ptr++;
            mlme_set_req.attr_index = *ptr++;
            mlme_set_req.value_pointer = ptr;
            mlme_set_req.value_size = mlme_req->ptr_length - 2;

            return mac_mlme_set_req(rf_mac_setup, &mlme_set_req);
        }
        case MLME_START:{
            mlme_start_t mlme_start_req;
            if (mlme_req->ptr_length != 34) {
                return -1;
            }
            mlme_start_req.PANId = common_read_16_bit(ptr);
            ptr += 2;
            mlme_start_req.LogicalChannel = *ptr++;
            mlme_start_req.StartTime = common_read_32_bit(ptr);
            ptr += 4;
            mlme_start_req.BeaconOrder = *ptr++;
            mlme_start_req.SuperframeOrder = *ptr++;
            mlme_start_req.PANCoordinator = *ptr++;
            mlme_start_req.BatteryLifeExtension = *ptr++;
            mlme_start_req.CoordRealignment = *ptr++;
            mlme_start_req.CoordRealignKey.SecurityLevel = *ptr++;
            mlme_start_req.CoordRealignKey.KeyIdMode = *ptr++;
            mlme_start_req.CoordRealignKey.KeyIndex = *ptr++;
            memcpy(mlme_start_req.CoordRealignKey.Keysource, ptr, 8);
            ptr += 8;
            mlme_start_req.BeaconRealignKey.SecurityLevel = *ptr++;
            mlme_start_req.BeaconRealignKey.KeyIdMode = *ptr++;
            mlme_start_req.BeaconRealignKey.KeyIndex = *ptr++;
            memcpy(mlme_start_req.BeaconRealignKey.Keysource, ptr, 8);
            ptr += 8;
            return mac_mlme_start_req(&mlme_start_req, rf_mac_setup);
        }
        default:
            break;
    }
    return -1;
}


int8_t mac_virtual_sap_data_cb(void *identifier, arm_phy_sap_msg_t *message)
{
    if (!identifier || !message) {
        return -1;
    }

    if (message->id == MACTUN_PD_SAP_NAP_IND) {
        return mac_virtual_data_req_handler(identifier,message->message.generic_data_ind.data_ptr,  message->message.generic_data_ind.data_len);
    }

    if (message->id == MACTUN_MLME_NAP_EXTENSION) {
        return mac_virtual_mlme_nap_req_handler(identifier, &message->message.mlme_request);
    }
    return -1;
}



//static void mac_data_interface_minium_security_level_get(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_security_level_descriptor_t *security_level_compare) {
//    //TODO get securily level table and verify current packet
//    (void)rf_mac_setup;
//    //Accept all true from mac
//    security_level_compare->SecurityMinimum = 0;
//}

static void mac_security_interface_aux_ccm_nonce_set(uint8_t *noncePtr, uint8_t *mac64, uint32_t securityFrameCounter, uint8_t securityLevel)
{
    memcpy(noncePtr, mac64, 8);
    noncePtr += 8;
    noncePtr = common_write_32_bit(securityFrameCounter, noncePtr);
    *noncePtr = securityLevel;
}

/* Compare two security levels, as per 802.15.4-2011 7.4.1.1 */
/* Returns true if sec1 is at least as secure as sec2 */
//static bool mac_security_interface_security_level_compare(uint8_t sec1, uint8_t sec2)
//{
//    /* bit 2 is "encrypted" - must be as encrypted
//    bits 1-0 are "authentication level" - must be at least as high */
//    return (sec1 & 4) >= (sec2 & 4) &&
//           (sec1 & 3) >= (sec2 & 3);
//}


static uint8_t mac_data_interface_decrypt_packet(mac_pre_parsed_frame_t *b, mlme_security_t *security_params)
{
    uint8_t *key;
    mlme_key_descriptor_t *key_description;
    mlme_key_device_descriptor_t *key_device_description = NULL;
    uint8_t device_descriptor_handle;
    uint8_t openPayloadLength = 0;
    protocol_interface_rf_mac_setup_s *rf_mac_setup = (protocol_interface_rf_mac_setup_s*)b->mac_class_ptr;
//    mlme_security_level_descriptor_t security_level_compare;


    if (!rf_mac_setup->mac_security_enabled) {
        if (security_params->SecurityLevel) {
            return MLME_UNSUPPORTED_SECURITY;
        }
        //TODO verify this with Kevin
        return MLME_UNSUPPORTED_LEGACY;
    }

//    security_level_compare.FrameType = b->fcf_dsn.frametype;
    if (b->fcf_dsn.frametype == MAC_FRAME_CMD) {
        openPayloadLength = 1;
//        security_level_compare.CommandFrameIdentifier = mcps_mac_command_frame_id_get(b);
    }

    //TODO do this proper way when security description  is implemeted

//    mac_data_interface_minium_security_level_get(rf_mac_setup, &security_level_compare);
//
//    //Validate Security Level
//    if (!mac_security_interface_security_level_compare(security_params->SecurityLevel, security_level_compare.SecurityMinimum)) {
//        //Drop packet reason rx level was less secured than requested one
//        tr_debug("RX Security level less than minimum level");
//        return MLME_IMPROPER_SECURITY_LEVEL;
//    }

    neighbour_security_update_t neighbour_validation;
    memset(&neighbour_validation, 0, sizeof(neighbour_validation));
    neighbour_validation.frameCounter = mcps_mac_security_frame_counter_read(b);

    if (neighbour_validation.frameCounter == 0xffffffff) {
        tr_debug("Max Framecounter value..Drop");
        return MLME_COUNTER_ERROR;
    }

    //READ SRC Address

    uint16_t SrcPANId = mac_header_get_src_panid(&b->fcf_dsn, mac_header_message_start_pointer(b));
    mac_header_get_src_address(&b->fcf_dsn, mac_header_message_start_pointer(b), neighbour_validation.address);
    neighbour_validation.addr_type = b->fcf_dsn.SrcAddrMode;
    neighbour_validation.keyId = security_params->KeyIndex;

    // Get A Key description
    key_description =  mac_sec_key_description_get(rf_mac_setup, security_params, b->fcf_dsn.SrcAddrMode, neighbour_validation.address, SrcPANId);
    if (!key_description) {
        return MLME_UNAVAILABLE_KEY;
    }

    if (key_description->unique_key_descriptor) {
        //Discover device table by device description handle
        key_device_description = key_description->KeyDeviceList;
        device_descriptor_handle = key_device_description->DeviceDescriptorHandle;
        //Discover device descriptor by handle
        b->neigh_info = mac_sec_mib_device_description_get_attribute_index(rf_mac_setup, device_descriptor_handle);
        if (!b->neigh_info) {
            return MLME_UNSUPPORTED_SECURITY;
        }
    } else {

        if (!b->neigh_info) {
            return MLME_UNSUPPORTED_SECURITY;
        }

        device_descriptor_handle = mac_mib_device_descption_attribute_get_by_descriptor(rf_mac_setup, b->neigh_info);
        key_device_description =  mac_sec_mib_key_device_description_discover_from_list(key_description, device_descriptor_handle);
    }

    if (key_device_description) {
        //validate BlackList status
        if (key_device_description->Blacklisted) {
            tr_debug("Blacklisted key for device %s", trace_array(b->neigh_info->ExtAddress, 8));
            return MLME_UNAVAILABLE_KEY;
        }

        if (neighbour_validation.frameCounter < b->neigh_info->FrameCounter ) {
            tr_debug("MLME_COUNTER_ERROR");
            return MLME_COUNTER_ERROR;
        }
    }

    key = key_description->Key;
    memcpy(neighbour_validation.nonce_ptr, b->neigh_info->ExtAddress, 8);

    ccm_globals_t *ccm_ptr = ccm_sec_init(security_params->SecurityLevel, key, AES_CCM_DECRYPT, 2);

    if (!ccm_ptr) {
        return MLME_UNSUPPORTED_SECURITY;
    }

    mac_security_interface_aux_ccm_nonce_set(ccm_ptr->exp_nonce, neighbour_validation.nonce_ptr, neighbour_validation.frameCounter, security_params->SecurityLevel);

    if (ccm_ptr->mic_len) {
     // this is asuming that there is no headroom for buffers.
     ccm_ptr->adata_len = mcps_mac_header_length_from_received_frame(b) + openPayloadLength;
     //SET MIC PTR
     ccm_ptr->mic = mcps_security_mic_pointer_get(b);
     ccm_ptr->adata_ptr = mac_header_message_start_pointer(b);
    }

    ccm_ptr->data_ptr = (mcps_mac_payload_pointer_get(b) + openPayloadLength);
    ccm_ptr->data_len = mcps_payload_length_from_received_frame(b) - openPayloadLength;
    if (ccm_process_run(ccm_ptr) != 0) {
        tr_warning("MIC Fail adata %s", trace_array(ccm_ptr->adata_ptr, ccm_ptr->adata_len));
        tr_warning("Nonce %s", trace_array(ccm_ptr->exp_nonce, 13));
        if (openPayloadLength) {
            tr_warning("%s", tr_array(ccm_ptr->data_ptr,  ccm_ptr->data_len));
        }
        return MLME_SECURITY_FAIL;
    }

    //Update key device and key description tables

    b->neigh_info->FrameCounter = neighbour_validation.frameCounter + 1;
    if (!key_device_description) {
        // Black list old used keys by this device
        mac_sec_mib_device_description_blacklist(rf_mac_setup, device_descriptor_handle);
        key_device_description =  mac_sec_mib_key_device_description_list_update(key_description);
        if (key_device_description) {
            tr_debug("Set new device user %u for key", device_descriptor_handle);
            key_device_description->DeviceDescriptorHandle = device_descriptor_handle;
        }
    }

    return MLME_SUCCESS;
}

static void mcps_comm_status_indication_generate(uint8_t status, mac_pre_parsed_frame_t *buf, mac_api_t * mac)
{
    mlme_comm_status_t comm_status;
    memset(&comm_status,0 ,sizeof(mlme_comm_status_t) );
    comm_status.status = status;
    //Call com status
    comm_status.PANId = mac_header_get_dst_panid(&buf->fcf_dsn, mac_header_message_start_pointer(buf));
    comm_status.DstAddrMode = buf->fcf_dsn.DstAddrMode;;
    mac_header_get_dst_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), comm_status.DstAddr);
    comm_status.SrcAddrMode = buf->fcf_dsn.SrcAddrMode;
    mac_header_get_src_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), comm_status.SrcAddr);
    mac_header_security_components_read(buf, &comm_status.Key);
    mac->mlme_ind_cb(mac,MLME_COMM_STATUS , &comm_status);
}



static int8_t mac_data_interface_host_accept_data(mcps_data_ind_t * data_ind, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if ((data_ind->DstAddrMode == MAC_ADDR_MODE_16_BIT) && (data_ind->DstAddr[0] == 0xff && data_ind->DstAddr[1] == 0xff)) {
        tr_debug("Drop Multicast packet");
        return -1;
    }


    if (data_ind->msduLength) {
        if (!rf_mac_setup->macRxDataAtPoll && rf_mac_setup->macDataPollReq) {
            eventOS_callback_timer_stop(rf_mac_setup->mlme_timer_id);
            rf_mac_setup->macRxDataAtPoll = true;
            eventOS_callback_timer_start(rf_mac_setup->mlme_timer_id, 400); //20ms
            rf_mac_setup->mac_mlme_event = ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL_AFTER_DATA;
            rf_mac_setup->mlme_tick_count = 0;
        }
        return 0;
    } else {
        eventOS_callback_timer_stop(rf_mac_setup->mlme_timer_id);
        mac_mlme_poll_process_confirm(rf_mac_setup, MLME_NO_DATA);
        return -1;
    }

}

static int8_t mac_data_sap_rx_handler(mac_pre_parsed_frame_t *buf, protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_api_t * mac)
{
    int8_t retval = -1;
    uint8_t status;
    //allocate Data ind primitiv and parse packet to that
    mcps_data_ind_t * data_ind = ns_dyn_mem_temporary_alloc(sizeof(mcps_data_ind_t));

    if (!data_ind) {
        goto DROP_PACKET;
    }
    memset(data_ind, 0, sizeof(mcps_data_ind_t));
    //Parse data
    data_ind->DSN = buf->fcf_dsn.DSN;
    data_ind->DstAddrMode = buf->fcf_dsn.DstAddrMode;
    data_ind->DstPANId = mac_header_get_dst_panid(&buf->fcf_dsn, mac_header_message_start_pointer(buf));
    mac_header_get_dst_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), data_ind->DstAddr);
    data_ind->SrcAddrMode = buf->fcf_dsn.SrcAddrMode;
    data_ind->SrcPANId = mac_header_get_src_panid(&buf->fcf_dsn, mac_header_message_start_pointer(buf));
    mac_header_get_src_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), data_ind->SrcAddr);

    data_ind->mpduLinkQuality = buf->LQI;
    data_ind->signal_dbm = buf->dbm;
    /* Parse security part */
    mac_header_security_components_read(buf, &data_ind->Key);
    data_ind->msduLength = buf->mac_payload_length;
    data_ind->msdu_ptr = mcps_mac_payload_pointer_get(buf);



    buf->neigh_info = mac_sec_mib_device_description_get(rf_mac_setup, data_ind->SrcAddr, data_ind->SrcAddrMode);
    if (buf->fcf_dsn.securityEnabled) {
        status = mac_data_interface_decrypt_packet(buf, &data_ind->Key);
        if (status != MLME_SUCCESS) {
            mcps_comm_status_indication_generate(status, buf, mac);
            goto DROP_PACKET;
        }
    }

    /* Validate Polling device */
    if (!rf_mac_setup->macCapRxOnIdle) {
        if (mac_data_interface_host_accept_data(data_ind, rf_mac_setup) != 0) {
            //tr_debug("Drop by not Accept");
            goto DROP_PACKET;
        }
    }

    if (mac) {
        mac->data_ind_cb(mac, data_ind);
        retval = 0;
    }

    DROP_PACKET:
    ns_dyn_mem_free(data_ind);
    mcps_sap_pre_parsed_frame_buffer_free(buf);
    return retval;
}

static void mac_lib_res_no_data_to_req(mac_pre_parsed_frame_t *buffer, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    mac_pre_build_frame_t *buf = mcps_sap_prebuild_frame_buffer_get(0);
    if (!buf) {
        return;
    }

    buf->fcf_dsn.SrcAddrMode = buffer->fcf_dsn.DstAddrMode;
    buf->fcf_dsn.DstAddrMode = buffer->fcf_dsn.SrcAddrMode;
    //SET PANID
    buf->SrcPANId = mac_header_get_dst_panid(&buffer->fcf_dsn, mac_header_message_start_pointer(buffer));
    buf->DstPANId = buf->SrcPANId;

    mac_header_get_dst_address(&buffer->fcf_dsn, mac_header_message_start_pointer(buffer), buf->SrcAddr);
    mac_header_get_src_address(&buffer->fcf_dsn, mac_header_message_start_pointer(buffer), buf->DstAddr);

    buf->fcf_dsn.securityEnabled = buffer->fcf_dsn.securityEnabled;
    buf->fcf_dsn.intraPan = true;
    buf->fcf_dsn.ackRequested = true;
    buf->mac_header_length_with_security = 3;
    if (buffer->fcf_dsn.securityEnabled) {
        buf->fcf_dsn.frameVersion = MAC_FRAME_VERSION_2006;
        buf->aux_header.securityLevel = rf_mac_setup->mac_auto_request.SecurityLevel;
        buf->aux_header.KeyIdMode = rf_mac_setup->mac_auto_request.KeyIdMode;
        buf->aux_header.KeyIndex = rf_mac_setup->mac_auto_request.KeyIndex;
        memcpy(buf->aux_header.Keysource, rf_mac_setup->mac_auto_request.Keysource, 8);
    }
    buf->fcf_dsn.frametype = FC_DATA_FRAME;
    buf->priority = MAC_PD_DATA_MEDIUM_PRIORITY;
    buf->mac_payload = NULL;
    buf->mac_payload_length = 0;
    buf->security_mic_len = mac_security_mic_length_get(buf->aux_header.securityLevel);
    buf->mac_header_length_with_security += mac_header_security_aux_header_length(buf->aux_header.securityLevel, buf->aux_header.KeyIdMode);
    buf->mac_header_length_with_security += mac_header_address_length(&buf->fcf_dsn);
    mcps_sap_pd_req_queue_write(rf_mac_setup, buf);
}

static int8_t mac_beacon_request_handler(mac_pre_parsed_frame_t *buffer, protocol_interface_rf_mac_setup_s *rf_mac_setup) {

    if (buffer->fcf_dsn.SrcAddrMode != MAC_ADDR_MODE_NONE || buffer->fcf_dsn.DstAddrMode != MAC_ADDR_MODE_16_BIT) {
        return -1;
    }
    // Note FHSS from received beacon request
    if (rf_mac_setup->fhss_api) {
        rf_mac_setup->fhss_api->receive_frame(rf_mac_setup->fhss_api, 0, NULL, 0, NULL, FHSS_SYNCH_REQUEST_FRAME);
    }
    // mac_data_interface_build_beacon() uses the same buffer to build the response
    // and it is then feed to protocol buffer. Buffer should be freed only if it returns zero.
    return mac_mlme_beacon_tx(rf_mac_setup);

}

static int8_t mac_command_sap_rx_handler(mac_pre_parsed_frame_t *buf, protocol_interface_rf_mac_setup_s *rf_mac_setup,  mac_api_t * mac)
{
    int8_t retval = -1;
    mlme_security_t security_params;
    uint8_t mac_command;
    uint8_t status;
    uint8_t temp_src_address[8];


    //Read address and pan-id
    mac_header_get_src_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), temp_src_address);
    uint8_t address_mode = buf->fcf_dsn.SrcAddrMode;
    buf->neigh_info = mac_sec_mib_device_description_get(rf_mac_setup, temp_src_address, address_mode);
    //Decrypt Packet if secured
    if (buf->fcf_dsn.securityEnabled) {
        mac_header_security_components_read(buf, &security_params);

        status = mac_data_interface_decrypt_packet(buf, &security_params);
        if (status != MLME_SUCCESS) {

            mcps_comm_status_indication_generate(status, buf, mac);
            goto DROP_PACKET;
        }
    }

    mac_command = mcps_mac_command_frame_id_get(buf);

    switch (mac_command) {
        case MAC_DATA_REQ:
            //Here 2 check
            if (mac_indirect_data_req_handle(buf, rf_mac_setup) == 0) {
                tr_debug("Trig Dummy packet");
                mac_lib_res_no_data_to_req(buf, rf_mac_setup);
            }
            retval = 0;
            break;
        case MAC_BEACON_REQ:
            retval = mac_beacon_request_handler(buf, rf_mac_setup);
            break;

        default:
            break;
    }

    DROP_PACKET:
    mcps_sap_pre_parsed_frame_buffer_free(buf);
    return retval;
}

static void mac_nap_tun_data_handler(mac_pre_parsed_frame_t *buf, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    phy_device_driver_s *driver = rf_mac_setup->tun_extension_rf_driver->phy_driver;
    if (driver->phy_rx_cb) {
        driver->phy_rx_cb(buf->buf, buf->frameLength, buf->LQI, buf->dbm, rf_mac_setup->tun_extension_rf_driver->id);
    }
    mcps_sap_pre_parsed_frame_buffer_free(buf);
}

static void mac_data_interface_parse_beacon(mac_pre_parsed_frame_t *buf, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    mlme_beacon_ind_t ind_data;

    uint16_t len;
    uint8_t *ptr;
    mlme_beacon_gts_spec_t gts_spec;
//    uint8_t *gts_info = NULL;
    uint8_t *pending_address_list = NULL;
    uint8_t SuperframeSpec[2];

    uint16_t src_pan_id = mac_header_get_src_panid(&buf->fcf_dsn, mac_header_message_start_pointer(buf));

    //validate beacon pan-id and filter other network out
    if (rf_mac_setup->pan_id < 0xffff && (rf_mac_setup->pan_id != src_pan_id && !rf_mac_setup->macAcceptAnyBeacon)) {
        tr_debug("Drop Beacon by unknow panid");
        return;
    }

    /*Add received bytes in statistics*/
    tr_debug("mac_parse_beacon");

    ptr = mcps_mac_payload_pointer_get(buf);
    len = mcps_payload_length_from_received_frame(buf);
    SuperframeSpec[0] = *ptr++;
    SuperframeSpec[1] = *ptr++;
    gts_spec.description_count = (*ptr & 7);
    gts_spec.gts_permit = ((*ptr++ & 0x80) >> 7);
    len -= 3;
    if (gts_spec.description_count) {
        tr_error("GTS info count not zero");
        //calucalate Length
        uint8_t gts_field_length = ((gts_spec.description_count) * 3);
        if (len < gts_field_length ) {
            return;
        }
//        gts_info = ptr;
        len -= gts_field_length;
        ptr += gts_field_length;
    }
    //Pendinlist
    ind_data.PendAddrSpec.short_address_count = (*ptr & 7);
    ind_data.PendAddrSpec.extended_address_count = ((*ptr++ & 0x70) >> 4);
    len -= 1;
    if (ind_data.PendAddrSpec.short_address_count || ind_data.PendAddrSpec.extended_address_count) {
        if ((ind_data.PendAddrSpec.extended_address_count + ind_data.PendAddrSpec.short_address_count)  > 7) {
            //over 7 address
            return;
        }
        uint8_t pending_address_list_size = (ind_data.PendAddrSpec.short_address_count * 2);
        pending_address_list_size += (ind_data.PendAddrSpec.extended_address_count *8);
        if (len < pending_address_list_size) {
            return;
        }
        pending_address_list = ptr;
        ptr += pending_address_list_size;
        len -= pending_address_list_size;
    }

    memset(&ind_data.PANDescriptor, 0, sizeof(mlme_pan_descriptor_t));

    if (rf_mac_setup->fhss_api) {
        ind_data.PANDescriptor.LogicalChannel = 0; //Force Allways same channel
    } else {
        ind_data.PANDescriptor.LogicalChannel = rf_mac_setup->mac_channel;
    }
    ind_data.PANDescriptor.ChannelPage = 0;
    ind_data.PANDescriptor.CoordAddrMode = buf->fcf_dsn.SrcAddrMode;
    mac_header_get_src_address(&buf->fcf_dsn, mac_header_message_start_pointer(buf), ind_data.PANDescriptor.CoordAddress);
    ind_data.PANDescriptor.CoordPANId = src_pan_id;
    ind_data.PANDescriptor.LinkQuality = buf->LQI;
    ind_data.PANDescriptor.GTSPermit = gts_spec.gts_permit;
    ind_data.PANDescriptor.Timestamp = buf->timestamp;
    mac_header_security_components_read(buf, &ind_data.PANDescriptor.Key);
    ind_data.PANDescriptor.SecurityFailure = 0;
    ind_data.PANDescriptor.SuperframeSpec[0] = SuperframeSpec[0];
    ind_data.PANDescriptor.SuperframeSpec[1] = SuperframeSpec[1];

    ind_data.BSN = buf->fcf_dsn.DSN;
    ind_data.AddrList = pending_address_list;
    ind_data.beacon_data_length = len;
    ind_data.beacon_data = ptr;

    mac_mlme_beacon_notify(rf_mac_setup, &ind_data);

}

static void mac_data_interface_frame_handler(mac_pre_parsed_frame_t *buf)
{
    protocol_interface_rf_mac_setup_s *rf_mac_setup = buf->mac_class_ptr;
    if (!rf_mac_setup) {
        tr_debug("Drop by no mac class");
        mcps_sap_pre_parsed_frame_buffer_free(buf);
        return;
    }

    if ( mac_filter_modify_link_quality(rf_mac_setup->mac_interface_id,buf) == 1) {
        mcps_sap_pre_parsed_frame_buffer_free(buf);
        return;
    }
    //Sniffer Should push here data to stack!!!!
    mac_api_t * mac = get_sw_mac_api(rf_mac_setup);
    if( !mac || (rf_mac_setup->mac_mlme_scan_resp && buf->fcf_dsn.frametype != MAC_FRAME_BEACON)) {
        mcps_sap_pre_parsed_frame_buffer_free(buf);
        return;
    }
    if (buf->fcf_dsn.ackRequested == false) {
        sw_mac_stats_update(rf_mac_setup, STAT_MAC_BC_RX_COUNT, 0);
    }
    sw_mac_stats_update(rf_mac_setup, STAT_MAC_RX_COUNT, 0);
    switch (buf->fcf_dsn.frametype) {
        case MAC_FRAME_BEACON:
            sw_mac_stats_update(rf_mac_setup, STAT_MAC_BEA_RX_COUNT, 0);
            mac_data_interface_parse_beacon(buf, rf_mac_setup);
            mcps_sap_pre_parsed_frame_buffer_free(buf);
            break;
        case MAC_FRAME_DATA:
            if (rf_mac_setup->tun_extension_rf_driver) {
                mac_nap_tun_data_handler(buf, rf_mac_setup);
                return;
            }
            mac_data_sap_rx_handler(buf, rf_mac_setup, mac);
            break;
        case MAC_FRAME_CMD:
            if (rf_mac_setup->tun_extension_rf_driver) {
                if (mcps_mac_command_frame_id_get(buf) != MAC_BEACON_REQ) {
                    mac_nap_tun_data_handler(buf, rf_mac_setup);
                    return;
                }
            }

            //Handle Command Frame
            mac_command_sap_rx_handler(buf, rf_mac_setup, mac);
            break;

        default:
            mcps_sap_pre_parsed_frame_buffer_free(buf);
    }

}

void mac_mcps_trig_buffer_from_queue(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if (!rf_mac_setup) {
        return;
    }
    while (!rf_mac_setup->active_pd_data_request) {
        bool is_bc_queue = false;
        mac_pre_build_frame_t *buffer;
        // With FHSS, poll broadcast queue on broadcast channel first
        if (rf_mac_setup->fhss_api) {
            if (rf_mac_setup->fhss_api->is_broadcast_channel(rf_mac_setup->fhss_api) == true) {
                if (rf_mac_setup->pd_data_request_bc_queue_to_go) {
                    is_bc_queue = true;
                }
            }
        }
        buffer = mcps_sap_pd_req_queue_read(rf_mac_setup, is_bc_queue, false);

        if (buffer) {
            rf_mac_setup->active_pd_data_request = buffer;
            if (mcps_pd_data_request(rf_mac_setup, buffer) != 0) {
                rf_mac_setup->active_pd_data_request = NULL;
                mcps_data_confirm_handle(rf_mac_setup, buffer);
            } else {
                return;
            }
        } else {
            return;
        }
    }
}

static void mac_pd_data_confirm_handle(protocol_interface_rf_mac_setup_s *rf_mac_setup) {

    if (!rf_mac_setup->active_pd_data_request) {
        tr_debug("No pending TX process anymore");
    } else {
        mac_pre_build_frame_t *buffer = rf_mac_setup->active_pd_data_request;
        rf_mac_setup->active_pd_data_request = NULL;
        mcps_data_confirm_handle(rf_mac_setup, buffer);
    }

    mac_mcps_trig_buffer_from_queue(rf_mac_setup);
}

static void mac_mcps_sap_data_tasklet(arm_event_s *event)
{
    uint8_t event_type = event->event_type;

    switch (event_type) {
        case MCPS_SAP_DATA_IND_EVENT:
            if (event->data_ptr) {
                mac_data_interface_frame_handler((mac_pre_parsed_frame_t*)event->data_ptr);
            }

            break;

        case MCPS_SAP_DATA_CNF_EVENT:
            //mac_data_interface_tx_done(event->data_ptr);
            mac_pd_data_confirm_handle((protocol_interface_rf_mac_setup_s*)event->data_ptr);
            break;

        case MAC_MLME_EVENT_HANDLER:
            mac_mlme_event_cb(event->data_ptr);
            break;
        case MAC_MCPS_INDIRECT_TIMER_CB:
            mac_indirect_data_ttl_handle((protocol_interface_rf_mac_setup_s*)event->data_ptr, (uint16_t)event->event_data);
            break;

        case MAC_MLME_SCAN_CONFIRM_HANDLER:
            mac_mlme_scan_confirmation_handle((protocol_interface_rf_mac_setup_s *) event->data_ptr);
            break;
        case MAC_SAP_TRIG_TX:
            mac_clear_active_event((protocol_interface_rf_mac_setup_s *) event->data_ptr, MAC_SAP_TRIG_TX);
            mac_mcps_trig_buffer_from_queue((protocol_interface_rf_mac_setup_s *) event->data_ptr);
            //No break necessary
        default:
            break;
    }
}

int8_t mac_mcps_sap_tasklet_init(void)
{
    if (mac_tasklet_event_handler < 0) {
        mac_tasklet_event_handler = eventOS_event_handler_create(&mac_mcps_sap_data_tasklet, 0);
    }

    return mac_tasklet_event_handler;
}

mac_pre_build_frame_t * mcps_sap_prebuild_frame_buffer_get(uint16_t payload_size)
{
    mac_pre_build_frame_t * buffer = ns_dyn_mem_temporary_alloc(sizeof(mac_pre_build_frame_t));
    if (!buffer) {
        return NULL;
    }
    memset(buffer, 0, sizeof(mac_pre_build_frame_t));
    if (payload_size) {
        //Mac interlnal payload allocate
        buffer->mac_payload = ns_dyn_mem_temporary_alloc(payload_size);
        if (!buffer->mac_payload) {
            ns_dyn_mem_free(buffer);
            return NULL;
        }
        buffer->mac_allocated_payload_ptr = true;
        buffer->mac_payload_length = payload_size;
    } else {
        buffer->mac_allocated_payload_ptr = false;
    }
    return buffer;
}


void mcps_sap_prebuild_frame_buffer_free(mac_pre_build_frame_t *buffer)
{
    if (!buffer) {
        return;
    }

    if (buffer->mac_allocated_payload_ptr) {
        ns_dyn_mem_free(buffer->mac_payload);
    }
    //Free Buffer frame
    ns_dyn_mem_free(buffer);

}

static ccm_globals_t * mac_frame_security_parameters_init(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_build_frame_t *buffer)
{
    /* Encrypt the packet payload if AES encyption bit is set */
    mlme_security_t key_source;
    key_source.KeyIdMode = buffer->aux_header.KeyIdMode;
    key_source.KeyIndex = buffer->aux_header.KeyIndex;
    key_source.SecurityLevel = buffer->aux_header.securityLevel;
    memcpy(key_source.Keysource, buffer->aux_header.Keysource, 8);
    mlme_key_descriptor_t *key_description =  mac_sec_key_description_get(rf_ptr, &key_source, buffer->fcf_dsn.DstAddrMode, buffer->DstAddr, buffer->DstPANId);

    if (!key_description ) {
        buffer->status = MLME_UNAVAILABLE_KEY;
        return NULL;

    }
    mlme_device_descriptor_t *device_description;
    uint8_t *nonce_ext_64_ptr;

    if (key_description->unique_key_descriptor) {
        device_description = mac_sec_mib_device_description_get_attribute_index(rf_ptr, key_description->KeyDeviceList->DeviceDescriptorHandle);
        if (!device_description) {

            buffer->status = MLME_UNAVAILABLE_KEY;
            return NULL;
        }
        nonce_ext_64_ptr = device_description->ExtAddress;
    } else {
        //Discover device descriptor only unicast packet which need ack
        if (buffer->fcf_dsn.DstAddrMode && buffer->fcf_dsn.ackRequested) {
            device_description =  mac_sec_mib_device_description_get(rf_ptr, buffer->DstAddr, buffer->fcf_dsn.DstAddrMode);
            if (!device_description) {

                buffer->status = MLME_UNAVAILABLE_KEY;
                return NULL;
            }
        }
        nonce_ext_64_ptr = rf_ptr->mac64;
    }

    uint8_t *key_ptr = key_description->Key;

    //Remember to update security counter here!
    buffer->aux_header.frameCounter = rf_ptr->security_frame_counter;

    //Check If frame counter overflow is coming
    if (buffer->aux_header.frameCounter == 0xffffffff) {
        buffer->status = MLME_COUNTER_ERROR;
        return NULL;
    }

    ccm_globals_t *ccm_ptr = ccm_sec_init(buffer->aux_header.securityLevel, key_ptr, AES_CCM_ENCRYPT, 2);
    if (!ccm_ptr) {
        buffer->status = MLME_SECURITY_FAIL;
        return NULL;
    }

    mac_security_interface_aux_ccm_nonce_set(ccm_ptr->exp_nonce, nonce_ext_64_ptr, buffer->aux_header.frameCounter,
                                             buffer->aux_header.securityLevel);
    //Increment security counter
    rf_ptr->security_frame_counter++;
    return ccm_ptr;

}


static void mac_common_data_confirmation_handle (protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buf)
{
    mac_event_t m_event;
    /* Raed MAC TX state */
    m_event = (mac_event_t) rf_mac_setup->mac_tx_result;
    rf_mac_setup->mac_tx_result = MAC_STATE_IDLE;

    /* Discard Tx timeout timer */
    timer_mac_stop(rf_mac_setup);
    if (m_event == MAC_CCA_FAIL) {
        sw_mac_stats_update(rf_mac_setup, STAT_MAC_TX_CCA_FAIL, 0);
        /* CCA fail */
        //rf_mac_setup->cca_failure++;
        buf->status = MLME_BUSY_CHAN;
    } else {
        sw_mac_stats_update(rf_mac_setup, STAT_MAC_TX_COUNT, buf->mac_payload_length);
        if (m_event == MAC_TX_FAIL) {
            sw_mac_stats_update(rf_mac_setup, STAT_MAC_TX_FAIL, 0);
            tr_error("MAC tx fail");
            buf->status = MLME_TX_NO_ACK;
        } else if (m_event == MAC_TX_DONE) {
            if (mac_is_ack_request_set(buf) == false) {
                sw_mac_stats_update(rf_mac_setup, STAT_MAC_BC_TX_COUNT, 0);
            }
            if (buf->fcf_dsn.frametype == FC_CMD_FRAME && buf->mac_command_id == MAC_DATA_REQ) {
                buf->status = MLME_NO_DATA;
            } else {
                buf->status = MLME_SUCCESS;
            }

        } else if (m_event == MAC_TX_DONE_PENDING) {
            buf->status = MLME_SUCCESS;
        } else if (m_event == MAC_TX_TIMEOUT) {
            /* Make MAC Soft Reset */;
            tr_debug("Driver TO event");
            //Disable allways
            mac_mlme_mac_radio_disabled(rf_mac_setup);
            //Enable Radio
            mac_mlme_mac_radio_enable(rf_mac_setup);
            buf->status = MLME_TRANSACTION_EXPIRED;
        }
    }
}

void mac_data_wait_timer_start(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    eventOS_callback_timer_stop(rf_mac_setup->mlme_timer_id);
    eventOS_callback_timer_start(rf_mac_setup->mlme_timer_id, 200); //10ms
    rf_mac_setup->mac_mlme_event = ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL;
    rf_mac_setup->mlme_tick_count = 30; //300ms
}

static void mac_data_interface_internal_tx_confirm_handle(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buf)
{
    //GET Interface

    switch (buf->mac_command_id) {
        case MAC_DATA_REQ:

            if (buf->status == MLME_SUCCESS) {
                if (!rf_mac_setup->macRxDataAtPoll) {
                    //Start Timer
                    mac_data_wait_timer_start(rf_mac_setup);
                    //Set Buffer back to
                }
                rf_mac_setup->active_pd_data_request = buf;
                return;

            } else {
                //Disable Radio
                if (!rf_mac_setup->macCapRxOnIdle) {
                    mac_mlme_mac_radio_disabled(rf_mac_setup);
                }
                rf_mac_setup->macDataPollReq = false;

                mac_api_t *mac_api = get_sw_mac_api(rf_mac_setup);

                if( mac_api ) {
                    mlme_poll_conf_t confirm;
                    confirm.status = buf->status;
                    mac_api->mlme_conf_cb(mac_api,MLME_POLL, &confirm);
                }

            }
            break;

        case MAC_BEACON_REQ:
            mac_mlme_active_scan_response_timer_start(rf_mac_setup);
            break;

        default:
            if (rf_mac_setup->tun_extension_rf_driver) {
                if (buf->fcf_dsn.ackRequested) {
                    phy_device_driver_s *driver = rf_mac_setup->tun_extension_rf_driver->phy_driver;
                    driver->phy_tx_done_cb(rf_mac_setup->tun_extension_rf_driver->id, 1, (phy_link_tx_status_e)buf->status, rf_mac_setup->mac_tx_status.cca_cnt, rf_mac_setup->mac_tx_status.retry);
                }
            }
            break;
    }

    mcps_sap_prebuild_frame_buffer_free(buf);

}

static void mcps_data_confirm_handle(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_build_frame_t *buffer) {

    sw_mac_stats_update(rf_ptr, STAT_MAC_TX_CCA_ATT, rf_ptr->mac_tx_status.cca_cnt);
    sw_mac_stats_update(rf_ptr, STAT_MAC_TX_RETRY, rf_ptr->mac_tx_status.retry);
    mcps_data_conf_t confirm;
    if (rf_ptr->fhss_api) {
        // FHSS checks if this failed buffer needs to be pushed back to TX queue and retransmitted
        if ((rf_ptr->mac_tx_result == MAC_TX_FAIL) || (rf_ptr->mac_tx_result == MAC_CCA_FAIL)) {
            if (rf_ptr->fhss_api->data_tx_fail(rf_ptr->fhss_api, buffer->msduHandle, mac_convert_frame_type_to_fhss(buffer->fcf_dsn.frametype)) == true) {
                mcps_sap_pd_req_queue_write(rf_ptr, buffer);
                tr_debug("Channel retry, TX result: %u, handle: %u", rf_ptr->mac_tx_result, buffer->msduHandle);
                return;
            }
        }
    }
    confirm.cca_retries = rf_ptr->mac_tx_status.cca_cnt;
    confirm.tx_retries = rf_ptr->mac_tx_status.retry;
    mac_common_data_confirmation_handle(rf_ptr, buffer);
    confirm.msduHandle = buffer->msduHandle;
    confirm.status = buffer->status;
    confirm.timestamp = 0;

    if (buffer->upper_layer_request) {
        //Check tunnel
        mcps_sap_prebuild_frame_buffer_free(buffer);
        mcps_data_confirm_cb(rf_ptr, &confirm);
    } else {
        mac_data_interface_internal_tx_confirm_handle(rf_ptr, buffer);
    }
}

static void mac_security_data_params_set(ccm_globals_t *ccm_ptr, uint8_t *data_ptr, uint16_t msduLength)
{
    ccm_ptr->data_len = msduLength;
    ccm_ptr->data_ptr = data_ptr;
}


static void mac_security_authentication_data_params_set(ccm_globals_t *ccm_ptr, uint8_t *a_data_ptr,
        uint8_t a_data_length)
{
    if (ccm_ptr->mic_len) {

        ccm_ptr->adata_len = a_data_length;
        ccm_ptr->adata_ptr = a_data_ptr;
        ccm_ptr->mic = ccm_ptr->data_ptr;
        ccm_ptr->mic += ccm_ptr->data_len;
    }
}

static uint8_t *mac_security_interface_aux_security_header_write(uint8_t *ptr, const mac_aux_security_header_t *auxHeader)
{
    uint8_t auxBaseHeader;
    auxBaseHeader = auxHeader->securityLevel;
    auxBaseHeader |= (auxHeader->KeyIdMode << 3);
    *ptr++ = auxBaseHeader;
    ptr = common_write_32_bit_inverse(auxHeader->frameCounter, ptr);

    switch (auxHeader->KeyIdMode) {
        case MAC_KEY_ID_MODE_SRC8_IDX:
            memcpy(ptr, auxHeader->Keysource, 8);
            ptr += 8;
            *ptr++ = auxHeader->KeyIndex;
            break;
        case MAC_KEY_ID_MODE_SRC4_IDX:
            memcpy(ptr, auxHeader->Keysource, 4);
            ptr += 4;
            *ptr++ = auxHeader->KeyIndex;
            break;
        case MAC_KEY_ID_MODE_IDX:
            *ptr++ = auxHeader->KeyIndex;
            break;
        default:
            break;
    }
    return ptr;
}


static int8_t mcps_generic_packet_build(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_build_frame_t *buffer) {
    phy_device_driver_s *dev_driver = rf_ptr->dev_driver->phy_driver;
    dev_driver_tx_buffer_s *tx_buf = &rf_ptr->dev_driver_tx_buffer;
    uint8_t open_payload = 0;
    uint8_t *ptr;
    ccm_globals_t *ccm_ptr = NULL;

    if (buffer->mac_header_length_with_security == 0) {
        rf_ptr->mac_tx_status.length = buffer->mac_payload_length;
        ptr = tx_buf->buf;
        if (dev_driver->phy_header_length) {
            ptr += dev_driver->phy_header_length;
        }
        tx_buf->len = buffer->mac_payload_length;

        memcpy(ptr, buffer->mac_payload, buffer->mac_payload_length );
        return 0;
    }

    if (buffer->fcf_dsn.frametype != FC_DATA_FRAME) {
        open_payload = 1;
    }

    if (buffer->fcf_dsn.frametype == MAC_FRAME_BEACON) {
        buffer->fcf_dsn.DSN = mac_mlme_set_new_beacon_sqn(rf_ptr);
    } else {
        buffer->fcf_dsn.DSN = mac_mlme_set_new_sqn(rf_ptr);
    }

    if (buffer->fcf_dsn.securityEnabled) {
        ccm_ptr = mac_frame_security_parameters_init(rf_ptr, buffer);
        if ( !ccm_ptr) {
            return -2;
        }
    }

    if (buffer->mac_payload_length > MAC_IEEE_802_15_4_MAX_MAC_SAFE_PAYLOAD_SIZE &&
            dev_driver->phy_MTU == MAC_IEEE_802_15_4_MAX_PHY_PACKET_SIZE) {
        /* IEEE 802.15.4-2003 only allowed unsecured payloads up to 102 bytes
        * (always leaving room for maximum MAC overhead).
        * IEEE 802.15.4-2006 allows bigger if MAC header is small enough, but
        * we have to set the version field.
        */
        buffer->fcf_dsn.frameVersion = MAC_FRAME_VERSION_2006;
    }
    //Frame length
    uint16_t frame_length = buffer->mac_header_length_with_security + buffer->mac_payload_length + buffer->security_mic_len;
    if ((frame_length) > dev_driver->phy_MTU - 2) {
        tr_debug("Too Long %u, %u pa %u header %u mic",frame_length, buffer->mac_payload_length, buffer->mac_header_length_with_security,  buffer->security_mic_len);
        buffer->status = MLME_FRAME_TOO_LONG;
        return -1;
    }

    rf_ptr->mac_tx_status.length = frame_length;
    ptr = tx_buf->buf;
    if (dev_driver->phy_header_length) {
        ptr += dev_driver->phy_header_length;
    }
    tx_buf->len = frame_length;

    ptr = mcps_generic_header_write(ptr, buffer);

    if (buffer->fcf_dsn.securityEnabled) {
        ptr = mac_security_interface_aux_security_header_write(ptr, &buffer->aux_header);
        ccm_ptr->data_len = (buffer->mac_payload_length - open_payload);
            ccm_ptr->data_ptr = ptr + open_payload;
        mac_security_data_params_set(ccm_ptr, (ptr + open_payload), (buffer->mac_payload_length - open_payload));
        mac_security_authentication_data_params_set(ccm_ptr, (ptr  - buffer->mac_header_length_with_security), (buffer->mac_header_length_with_security +open_payload));
        //Copy Payload
        memcpy(ptr, buffer->mac_payload, buffer->mac_payload_length);
        ccm_process_run(ccm_ptr);
    } else {
        //Copy Payload
        memcpy(ptr, buffer->mac_payload, buffer->mac_payload_length );
    }
    return 0;
}

static int8_t mcps_pd_data_request(protocol_interface_rf_mac_setup_s *rf_ptr, mac_pre_build_frame_t *buffer)
{
    rf_ptr->macTxRequestAck = false;
    memset(&(rf_ptr->mac_tx_status), 0, sizeof(mac_tx_status_t));
    if (mcps_generic_packet_build(rf_ptr, buffer) != 0) {
        return -1;
    }
    rf_ptr->macTxRequestAck = buffer->fcf_dsn.ackRequested;

    mac_mlme_mac_radio_enable(rf_ptr);
    /* Trig MAC TX */
    return mac_pd_sap_req(rf_ptr);
}

bool mac_is_ack_request_set(mac_pre_build_frame_t *buffer)
{
    return buffer->fcf_dsn.ackRequested;
}

int mac_convert_frame_type_to_fhss(uint8_t frame_type)
{
    if (FC_BEACON_FRAME == frame_type) {
        return FHSS_SYNCH_FRAME;
    }
    if (FC_CMD_FRAME == frame_type) {
        return FHSS_SYNCH_REQUEST_FRAME;
    }
    return FHSS_DATA_FRAME;
}

void mcps_sap_pd_req_queue_write(protocol_interface_rf_mac_setup_s *rf_mac_setup, mac_pre_build_frame_t *buffer)
{
    if( !rf_mac_setup || !buffer){
        return;
    }
    if (!rf_mac_setup->active_pd_data_request) {
        // Push broadcast buffers to queue when broadcast disabled flag is set
        if ((rf_mac_setup->macBroadcastDisabled == true) && !mac_is_ack_request_set(buffer)) {
            goto push_to_queue;
        }
        if (rf_mac_setup->fhss_api) {
            if (rf_mac_setup->fhss_api->check_tx_conditions(rf_mac_setup->fhss_api, !mac_is_ack_request_set(buffer),
            	buffer->msduHandle, mac_convert_frame_type_to_fhss(buffer->fcf_dsn.frametype), buffer->mac_payload_length,
            	rf_mac_setup->dev_driver->phy_driver->phy_header_length, rf_mac_setup->dev_driver->phy_driver->phy_tail_length) == false) {
                goto push_to_queue;
            }
        }
        //Start TX process immediately
        rf_mac_setup->active_pd_data_request = buffer;
        if (mcps_pd_data_request(rf_mac_setup, buffer) != 0) {
            rf_mac_setup->active_pd_data_request = NULL;
            mcps_data_conf_t confirm;
            memset(&confirm, 0, sizeof(mcps_data_conf_t));
            confirm.msduHandle = buffer->msduHandle;
            confirm.status = buffer->status;
            bool requested_from_up = buffer->upper_layer_request;
            mcps_sap_prebuild_frame_buffer_free(buffer);
            if (requested_from_up) {
                mcps_data_confirm_cb(rf_mac_setup, &confirm);
            }
            //Call
        }

        return;
    }
push_to_queue:
    rf_mac_setup->direct_queue_bytes += buffer->mac_payload_length;
    mac_pre_build_frame_t *prev = NULL;
    mac_pre_build_frame_t *cur = rf_mac_setup->pd_data_request_queue_to_go;
    bool use_bc_queue = false;

    // When FHSS is enabled, broadcast buffers are pushed to own queue
    if (rf_mac_setup->fhss_api) {
        if (rf_mac_setup->fhss_api->use_broadcast_queue(rf_mac_setup->fhss_api, !mac_is_ack_request_set(buffer),
                mac_convert_frame_type_to_fhss(buffer->fcf_dsn.frametype)) == true) {
            cur = rf_mac_setup->pd_data_request_bc_queue_to_go;
            use_bc_queue = true;
            rf_mac_setup->broadcast_queue_size++;
        }
    }
    if (use_bc_queue == false) {
        rf_mac_setup->unicast_queue_size++;
    }
    sw_mac_stats_update(rf_mac_setup, STAT_MAC_TX_QUEUE, rf_mac_setup->unicast_queue_size + rf_mac_setup->broadcast_queue_size);

    //Push to queue
    if (!cur) {
        if (rf_mac_setup->fhss_api && (use_bc_queue == true)) {
            rf_mac_setup->pd_data_request_bc_queue_to_go = buffer;
            return;
        } else {
            rf_mac_setup->pd_data_request_queue_to_go = buffer;
            return;
        }
    }

    while(cur) {
        if (cur->priority < buffer->priority) {
            //Set before cur
            if (prev) {
                prev->next = buffer;
                buffer->next = cur;
            } else {
                buffer->next = cur;
                if (rf_mac_setup->fhss_api && (use_bc_queue == true)) {
                    rf_mac_setup->pd_data_request_bc_queue_to_go = buffer;
                } else {
                    rf_mac_setup->pd_data_request_queue_to_go = buffer;
                }
            }
            cur = NULL;

        } else if( cur->next == NULL) {
            cur->next = buffer;
            cur = NULL;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
}

static mac_pre_build_frame_t * mcps_sap_pd_req_queue_read(protocol_interface_rf_mac_setup_s *rf_mac_setup, bool is_bc_queue, bool flush)
{
    mac_pre_build_frame_t *queue = rf_mac_setup->pd_data_request_queue_to_go;
    if (is_bc_queue == true) {
        queue = rf_mac_setup->pd_data_request_bc_queue_to_go;
    }

    if (!queue) {
        return NULL;
    }

    mac_pre_build_frame_t *buffer = queue;
    mac_pre_build_frame_t *prev = NULL;
    // With FHSS, check TX conditions
    if (rf_mac_setup->fhss_api) {
        while (buffer) {
            if ((flush == true) || (rf_mac_setup->fhss_api->check_tx_conditions(rf_mac_setup->fhss_api, !mac_is_ack_request_set(buffer),
                    buffer->msduHandle, mac_convert_frame_type_to_fhss(buffer->fcf_dsn.frametype), buffer->mac_payload_length,
                    rf_mac_setup->dev_driver->phy_driver->phy_header_length, rf_mac_setup->dev_driver->phy_driver->phy_tail_length) == true)) {
                break;
            }
            prev = buffer;
            buffer = buffer->next;
        }
    }
    // This check is here to prevent (Linux) border router from pushing broadcast frames to RF interface on unicast channel.
    while (buffer) {
        /* Allow returning buffer when:
         * - Flush is enabled
         * - Broadcast not disabled
         * - Broadcast is disabled and buffer has unicast destination
         */
        if ((flush == true) || (rf_mac_setup->macBroadcastDisabled == false) || ((rf_mac_setup->macBroadcastDisabled == true) && mac_is_ack_request_set(buffer))) {
            break;
        }
        prev = buffer;
        buffer = buffer->next;
    }
    if (!buffer) {
        return NULL;
    }
    // When other than first buffer is read out, link next buffer to previous buffer
    if (prev) {
        prev->next = buffer->next;
    }

    // When the first buffer is read out, set next buffer as the new first buffer
    if (is_bc_queue == false) {
        if (!prev) {
            rf_mac_setup->pd_data_request_queue_to_go = buffer->next;
        }
        rf_mac_setup->unicast_queue_size--;
    } else {
        if (!prev) {
            rf_mac_setup->pd_data_request_bc_queue_to_go = buffer->next;
        }
        rf_mac_setup->broadcast_queue_size--;
    }
    buffer->next = NULL;
    rf_mac_setup->direct_queue_bytes -= buffer->mac_payload_length;

    sw_mac_stats_update(rf_mac_setup, STAT_MAC_TX_QUEUE, rf_mac_setup->unicast_queue_size + rf_mac_setup->broadcast_queue_size);
    return buffer;
}

void mcps_sap_pre_parsed_frame_buffer_free(mac_pre_parsed_frame_t *buf)
{
    ns_dyn_mem_free(buf);
}

mac_pre_parsed_frame_t * mcps_sap_pre_parsed_frame_buffer_get(const uint8_t *data_ptr, uint16_t frame_length)
{
    mac_pre_parsed_frame_t *buffer = ns_dyn_mem_temporary_alloc(sizeof(mac_pre_parsed_frame_t) + frame_length);

    if (buffer) {
        memset(buffer, 0, sizeof(mac_pre_parsed_frame_t) + frame_length);
        buffer->frameLength = frame_length;
        memcpy(mac_header_message_start_pointer(buffer), data_ptr, frame_length);
    }
    return buffer;
}

static void mac_set_active_event(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t event_type)
{
    rf_mac_setup->active_mac_events |= (1 << event_type);
}

static void mac_clear_active_event(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t event_type)
{
    rf_mac_setup->active_mac_events &= ~(1 << event_type);
}

static bool mac_read_active_event(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t event_type)
{
    if (rf_mac_setup->active_mac_events & (1 << event_type)) {
        return true;
    }
    return false;
}

int8_t mcps_sap_pd_ind(mac_pre_parsed_frame_t *buffer)
{
    if (mac_tasklet_event_handler < 0 || !buffer) {
        return -1;
    }

    arm_event_s event = {
            .receiver = mac_tasklet_event_handler,
            .sender = 0,
            .event_id = 0,
            .data_ptr = buffer,
            .event_type = MCPS_SAP_DATA_IND_EVENT,
            .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    return eventOS_event_send(&event);
}

void mcps_sap_pd_confirm(void *mac_ptr)
{
    if (mac_tasklet_event_handler < 0  || !mac_ptr) {
        return;
    }
    arm_event_s event = {
        .receiver = mac_tasklet_event_handler,
        .sender = 0,
        .event_id = 0,
        .data_ptr = mac_ptr,
        .event_type = MCPS_SAP_DATA_CNF_EVENT,
        .priority = ARM_LIB_HIGH_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) != 0) {
        tr_error("mcps_sap_pd_confirm(): event send failed");
    }
}

void mcps_sap_trig_tx(void *mac_ptr)
{
    if (mac_tasklet_event_handler < 0  || !mac_ptr) {
        return;
    }
    if (mac_read_active_event(mac_ptr, MAC_SAP_TRIG_TX) == true) {
        return;
    }
    arm_event_s event = {
        .receiver = mac_tasklet_event_handler,
        .sender = 0,
        .event_id = 0,
        .data_ptr = mac_ptr,
        .event_type = MAC_SAP_TRIG_TX,
        .priority = ARM_LIB_MED_PRIORITY_EVENT,
    };

    if (eventOS_event_send(&event) != 0) {
        tr_error("mcps_sap_trig_tx(): event send failed");
    } else {
        mac_set_active_event(mac_ptr, MAC_SAP_TRIG_TX);
    }
}


void mac_generic_event_trig(uint8_t event_type, void *mac_ptr, bool low_latency)
{
    arm_library_event_priority_e priority;
    if (low_latency) {
        priority = ARM_LIB_LOW_PRIORITY_EVENT;
    } else {
        priority = ARM_LIB_HIGH_PRIORITY_EVENT;
    }
    arm_event_s event = {
        .receiver = mac_tasklet_event_handler,
        .sender = 0,
        .event_id = 0,
        .data_ptr = mac_ptr,
        .event_type = event_type,
        .priority = priority,
    };

    if (eventOS_event_send(&event) != 0) {
        tr_error("mac_generic_event_trig(): event send failed");
    }
}

void mac_mcps_buffer_queue_free(protocol_interface_rf_mac_setup_s *rf_mac_setup) {

    if (rf_mac_setup->active_pd_data_request) {
        mcps_sap_prebuild_frame_buffer_free(rf_mac_setup->active_pd_data_request);
        rf_mac_setup->active_pd_data_request = NULL;
    }

    while (rf_mac_setup->pd_data_request_queue_to_go) {
        mac_pre_build_frame_t *buffer = mcps_sap_pd_req_queue_read(rf_mac_setup, false, true);
        if (buffer) {
            mcps_sap_prebuild_frame_buffer_free(buffer);
        }
    }

    while (rf_mac_setup->pd_data_request_bc_queue_to_go) {
        mac_pre_build_frame_t *buffer = mcps_sap_pd_req_queue_read(rf_mac_setup, true, true);
        if (buffer) {
            mcps_sap_prebuild_frame_buffer_free(buffer);
        }
    }

    while (rf_mac_setup->indirect_pd_data_request_queue) {
        mac_pre_build_frame_t *buffer = rf_mac_setup->indirect_pd_data_request_queue;
        if (buffer) {
            rf_mac_setup->indirect_pd_data_request_queue = buffer->next;
            mcps_sap_prebuild_frame_buffer_free(buffer);
        }
    }
}
/**
 * Function return list start pointer
 */
static mac_pre_build_frame_t * mcps_sap_purge_from_list(mac_pre_build_frame_t *list_ptr_original, uint8_t msduhandle, uint8_t *status)
{
    mac_pre_build_frame_t *list_prev = NULL;
    mac_pre_build_frame_t *list_ptr = list_ptr_original;
    while (list_ptr) {
        if (list_ptr->fcf_dsn.frametype == MAC_FRAME_DATA && list_ptr->msduHandle == msduhandle) {

            if (list_prev) {
                list_prev->next = list_ptr->next;
            } else {
                list_ptr_original = list_ptr->next;
            }
            list_ptr->next = NULL;

            //Free data and buffer
            mcps_sap_prebuild_frame_buffer_free(list_ptr);
            list_ptr = NULL;
            *status = true;
        } else {
            list_prev = list_ptr;
            list_ptr = list_ptr->next;
        }
    }
    return list_ptr_original;
}


static bool mcps_sap_purge_req_from_queue(protocol_interface_rf_mac_setup_s *rf_mac_setup , uint8_t msduhandle)
{
    //Discover from TX queue data packets with given
    uint8_t status = false;
    rf_mac_setup->pd_data_request_queue_to_go = mcps_sap_purge_from_list(rf_mac_setup->pd_data_request_queue_to_go, msduhandle, &status);

    if (status) {
        return true;
    }

    rf_mac_setup->indirect_pd_data_request_queue = mcps_sap_purge_from_list(rf_mac_setup->indirect_pd_data_request_queue, msduhandle, &status);

    return status;

}

void mcps_sap_purge_reg_handler(protocol_interface_rf_mac_setup_s *rf_mac_setup, const mcps_purge_t *purge_req)
{
    mcps_purge_conf_t confirmation;
    confirmation.msduHandle = purge_req->msduHandle;

    if (mcps_sap_purge_req_from_queue(rf_mac_setup , confirmation.msduHandle)) {
        confirmation.status = MLME_SUCCESS;
    } else {
        confirmation.status = MLME_INVALID_HANDLE;
    }

    if( get_sw_mac_api(rf_mac_setup) ) {
        get_sw_mac_api(rf_mac_setup)->purge_conf_cb(get_sw_mac_api(rf_mac_setup), &confirmation);
    }
}
