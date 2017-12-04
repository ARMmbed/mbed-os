/*
 * Copyright (c) 2013-2017, Arm Limited and affiliates.
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
/**
 *
 * \file mac_mlme.c
 * \brief MLME API for MAC control
 *
 *  MLME API for MAC certification.
 *
 */

#include "nsconfig.h"
#include <string.h>
#include "ns_types.h"
#include "eventOS_event.h"
#include "eventOS_scheduler.h"
#include "eventOS_callback_timer.h"
#include "ns_trace.h"
#include "randLIB.h"
#include "nsdynmemLIB.h"
#include "platform/arm_hal_interrupt.h"
#include "common_functions.h"
#include "sw_mac.h"
#include "mlme.h"
#include "mac_api.h"
#include "fhss_api.h"

#include "MAC/IEEE802_15_4/sw_mac_internal.h"
#include "MAC/IEEE802_15_4/mac_defines.h"
#include "MAC/IEEE802_15_4/mac_header_helper_functions.h"
#include "MAC/IEEE802_15_4/mac_indirect_data.h"
#include "MAC/IEEE802_15_4/mac_security_mib.h"
#include "MAC/IEEE802_15_4/mac_mlme.h"
#include "MAC/IEEE802_15_4/mac_timer.h"
#include "MAC/IEEE802_15_4/mac_pd_sap.h"
#include "MAC/IEEE802_15_4/mac_mcps_sap.h"
#include "MAC/virtual_rf/virtual_rf_defines.h"
#include "MAC/rf_driver_storage.h"

#define TRACE_GROUP "mlme"

#define MAC_ACK_WAIT_DURATION   90

static int8_t mac_mlme_rf_disable(struct protocol_interface_rf_mac_setup *rf_mac_setup);
static int8_t mac_mlme_rf_receiver_enable(struct protocol_interface_rf_mac_setup *rf_mac_setup);

static void mac_mlme_write_mac16(protocol_interface_rf_mac_setup_s *rf_setup, uint8_t *addrPtr);
static void mac_mlme_write_mac64(struct protocol_interface_rf_mac_setup *rf_setup, uint8_t *addrPtr);
static void mac_mlme_timers_disable(protocol_interface_rf_mac_setup_s *rf_ptr);
static void mac_mlme_free_scan_temporary_data(protocol_interface_rf_mac_setup_s *rf_mac_setup);
static int8_t mac_mlme_set_panid(struct protocol_interface_rf_mac_setup *rf_setup, uint16_t pan_id);
static int8_t mac_mlme_set_mac16(struct protocol_interface_rf_mac_setup *rf_setup, uint16_t mac16);
static int8_t mac_mlme_rf_channel_set(struct protocol_interface_rf_mac_setup *rf_setup, uint8_t new_channel);
static void mac_mlme_timer_cb(int8_t timer_id, uint16_t slots);
static void mac_mlme_start_confirm_handler(protocol_interface_rf_mac_setup_s *rf_ptr, const mlme_start_conf_t *conf);
static void mac_mlme_scan_confirm_handler(protocol_interface_rf_mac_setup_s *rf_ptr, const mlme_scan_conf_t *conf);

static void mac_mlme_energy_scan_start(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t channel)
{
    phy_device_driver_s *dev_driver = rf_mac_setup->dev_driver->phy_driver;
    rf_mac_setup->mac_channel = channel;
    if (rf_mac_setup->macRfRadioOn) {
        if (dev_driver->state_control) {
            dev_driver->state_control(PHY_INTERFACE_DOWN, 0);
        }
    }
    if (dev_driver->state_control) {
        dev_driver->state_control(PHY_INTERFACE_RX_ENERGY_STATE, channel);
    }
    rf_mac_setup->macRfRadioOn = true;
    rf_mac_setup->macRfRadioTxActive = false;
}

static uint16_t mlme_scan_analyze_next_channel(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint8_t i, j = 0, k = 1;
    uint32_t mask = 1;
    uint32_t *channel_mask = rf_mac_setup->mac_channel_list.channel_mask;

    if (rf_mac_setup->mac_channel_list.channel_page == CHANNEL_PAGE_9 ||
        rf_mac_setup->mac_channel_list.channel_page == CHANNEL_PAGE_10) {
        k=8;
    }
    for(j=0; j<k; j++)
    {
        for (i=0; i<32; i++)
        {
            if (*channel_mask & mask)
            {
                *channel_mask &= ~mask;
                return (i+j*32);
            }
            mask <<= 1;
        }
        mask = 1;
        channel_mask++;
    }
    return 0xffff;
}

static uint32_t mac_mlme_channel_symbol_rate(uint8_t channel_page, uint8_t channel) {

    uint32_t symbol_rate;

    // Gets symbol rate for channel
    switch (channel_page) {
        case CHANNEL_PAGE_0:
            // 868 Mhz BPSK
            if (channel == 0) {
                symbol_rate = 20000;
            // 915 Mhz BPSK
            } else if (channel >= 1 && channel <= 10) {
                symbol_rate = 40000;
            // 2450 Mhz O-QPSK
            } else {
                symbol_rate = 62500;
            }
            break;
        case CHANNEL_PAGE_1:
            // 868 MHz ASK
            if (channel == 0) {
                symbol_rate = 12500;
            // 915 MHz ASK
            } else {
                symbol_rate = 50000;
            }
            break;
        case CHANNEL_PAGE_2:
            // 868 MHz O-QPSK
            if (channel == 0) {
                symbol_rate = 25000;
            // 915 MHz O-QPSK
            } else {
                symbol_rate = 62500;
            }
            break;
        case CHANNEL_PAGE_3:
            // 2450 CSS
            symbol_rate = 167000;
            break;
        case CHANNEL_PAGE_6:
            // 950 MHz BPSK
            if (channel <= 9) {
                symbol_rate = 20000;
            // 950 MHz GFSK
            } else {
                symbol_rate = 100000;
            }
            break;
        default:
            symbol_rate = 62500;
            break;
    }

    return symbol_rate;
}

static void mac_mlme_calc_scan_duration(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t channel)
{
    rf_mac_setup->mlme_ED_counter = 1;

    if (rf_mac_setup->scan_duration) {
        rf_mac_setup->mlme_ED_counter <<= rf_mac_setup->scan_duration;
    }

    rf_mac_setup->mlme_ED_counter++;

    if (rf_mac_setup->scan_type == MAC_ED_SCAN_TYPE) {
        // Gets symbol rate for channel that is scanned
        uint32_t symbol_rate = mac_mlme_channel_symbol_rate(
             rf_mac_setup->mac_channel_list.channel_page, channel);

        // Energy scan duration is aBaseSuperframeDuration * (2^n + 1)
        // aBaseSuperframeDuration is 960 symbols e.g for 2.4Ghz O-QPSK with 62.5 ksymbols/s -> 15,36ms.
        // ED scan timer timeout is 4.8ms
        uint16_t frame_duration = (uint32_t) 960 * 100000 / symbol_rate;
        rf_mac_setup->mlme_ED_counter = (uint32_t) rf_mac_setup->mlme_ED_counter * frame_duration / 480;
    }
}

static void mac_mlme_start_request(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    mac_pre_build_frame_t *buf;
    platform_enter_critical();

    mac_mlme_rf_disable(rf_mac_setup);
    buf = rf_mac_setup->active_pd_data_request;
    rf_mac_setup->active_pd_data_request = NULL;
    mac_mlme_mac_radio_enable(rf_mac_setup);
    rf_mac_setup->macUpState = true;
    if (buf) {
        // Active packet is pushed back to queue and statistics will be cleared. They need to be updated here.
        sw_mac_stats_update(rf_mac_setup, STAT_MAC_TX_CCA_ATT, rf_mac_setup->mac_tx_status.cca_cnt);
        sw_mac_stats_update(rf_mac_setup, STAT_MAC_TX_RETRY, rf_mac_setup->mac_tx_status.retry);
        mcps_sap_pd_req_queue_write(rf_mac_setup, buf);
    }
    platform_exit_critical();
}

uint8_t mac_mlme_beacon_req_tx(protocol_interface_rf_mac_setup_s *rf_ptr)
{

    mac_pre_build_frame_t *buf = mcps_sap_prebuild_frame_buffer_get(0);

    if (buf) {
        buf->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_16_BIT;
        buf->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_NONE;
        buf->fcf_dsn.frametype = FC_CMD_FRAME;
        buf->DstPANId = 0xffff;
        buf->DstAddr[0] = 0xff;
        buf->DstAddr[1] = 0xff;
        buf->mac_command_id = MAC_BEACON_REQ;
        buf->mac_header_length_with_security = 7;
        buf->mac_payload = &buf->mac_command_id;
        buf->mac_payload_length = 1;
        buf->priority = MAC_PD_DATA_MEDIUM_PRIORITY;

        tr_debug("BEA REQ tx");
        mcps_sap_pd_req_queue_write(rf_ptr, buf);
        return 1;
    }
    return 0;
}

static void mac_mlme_scan_init(uint8_t channel, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    mac_mlme_calc_scan_duration(rf_mac_setup, channel);

    switch (rf_mac_setup->scan_type) {
        /* ED Scan */
        case MAC_ED_SCAN_TYPE:
            //tr_debug("Energy Scan");
            rf_mac_setup->max_ED = 0;
            mac_mlme_energy_scan_start(rf_mac_setup, channel);
            rf_mac_setup->mac_mlme_event = ARM_NWK_MAC_MLME_ED_ANALYZE;
            eventOS_callback_timer_start(rf_mac_setup->mlme_timer_id, 96);
            break;
        case MAC_PASSIVE_SCAN:
        case MAC_ACTIVE_SCAN:   /*Active*/
            tr_debug("Scan channel %u", channel);
            rf_mac_setup->mac_channel = channel;
            rf_mac_setup->macCapRxOnIdle = true;
            mac_mlme_start_request(rf_mac_setup);
            if (rf_mac_setup->scan_type == MAC_ACTIVE_SCAN) {
                mac_pre_build_frame_t *active_buf = rf_mac_setup->active_pd_data_request;
                /* If there is active data request, it must be Beacon request which were failed to send on previous channel.
                 * Do not push new Beacon request to queue as the MAC will try to send the current active data request anyway.
                 */
                if (!active_buf || (active_buf && active_buf->fcf_dsn.frametype != FC_CMD_FRAME)) {
                    mac_mlme_beacon_req_tx(rf_mac_setup);
                }
            }
            rf_mac_setup->mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
            rf_mac_setup->mlme_tick_count = rf_mac_setup->mlme_ED_counter;
            //tr_debug("mlme_tick_count: %x", rf_mac_setup->mlme_tick_count);
            eventOS_callback_timer_start(rf_mac_setup->mlme_timer_id, 300);
            break;
        case MAC_ORPHAN_SCAN:   /*Orphan*/
            break;
    }
}

static void mac_mlme_scan_start(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint8_t channel;

    channel = (uint8_t) mlme_scan_analyze_next_channel(rf_mac_setup);
    mac_mlme_scan_init(channel, rf_mac_setup);
}

static bool mac_channel_list_validate(const channel_list_s *list, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if (rf_mac_setup->dev_driver->phy_driver->link_type == PHY_LINK_15_4_2_4GHZ_TYPE &&
        list->channel_page == 0) {
        //Accept only Channels channels 11-26
        if (list->channel_mask[0] & 0x7fff800) {
            return true;
        }
        return false;
    }
    else if (rf_mac_setup->dev_driver->phy_driver->link_type == PHY_LINK_15_4_SUBGHZ_TYPE &&
            (list->channel_page == 0 || list->channel_page == 1 || list->channel_page == 2)) {
        if (list->channel_mask[0] & 0x000007ff) {
            return true;
        }

        return false;
    } else {
        for (int i=0; i < 8; i++) {
            if (list->channel_mask[i]) {
                return true;
            }
        }
    }

    return false;
}

static int mac_mlme_generate_scan_confirmation(mlme_scan_conf_t *conf, const mlme_scan_t *msg, uint8_t status)
{
    memset(conf, 0, sizeof(mlme_scan_conf_t));
    conf->status = status;
    if (!msg) {
        return -1;
    }
    conf->ScanType = msg->ScanType;
    conf->ChannelPage = msg->ChannelPage;
    if (msg->ScanType == MAC_ACTIVE_SCAN) {
        /*If the scan is successful, this value will be modified properly in mlme_analyze_next_channel()
        and assigned in mlme_scan_operation()*/
        conf->UnscannedChannels.channel_mask[0] = msg->ScanChannels.channel_mask[0];
    } else if (msg->ScanType == MAC_ED_SCAN_TYPE) {
        conf->ED_values = ns_dyn_mem_temporary_alloc(MLME_MAC_RES_SIZE_MAX);
        if (!conf->ED_values) {
            tr_debug("Could not allocate memory for ED values");
            conf->status = MLME_SUCCESS;
            return -2;
        }
        memset(conf->ED_values, 0, MLME_MAC_RES_SIZE_MAX);
    }
    return 0;
}

void mac_mlme_scan_request(const mlme_scan_t *msg, protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if (rf_mac_setup->mac_mlme_scan_resp){
        mlme_scan_conf_t conf;
        mac_mlme_generate_scan_confirmation(&conf, msg, MLME_SCAN_IN_PROGRESS);
        mac_mlme_scan_confirm_handler(rf_mac_setup, &conf);
        ns_dyn_mem_free(conf.ED_values);
        return;
    }

    mlme_scan_conf_t *resp = ns_dyn_mem_temporary_alloc(sizeof(mlme_scan_conf_t));
    if (!resp) {
        mlme_scan_conf_t conf;
        tr_error("Mac Scan request fail, no memory");
        mac_mlme_generate_scan_confirmation(&conf, NULL, MLME_SUCCESS);
        mac_mlme_scan_confirm_handler(rf_mac_setup, &conf);
        ns_dyn_mem_free(conf.ED_values);
        return;
    }

    rf_mac_setup->mac_mlme_scan_resp = resp;
    //Validate channel list
    tr_debug("chan page %u, mask %"PRIx32, msg->ScanChannels.channel_page, msg->ScanChannels.channel_mask[0]);
    if (!mac_channel_list_validate(&msg->ScanChannels, rf_mac_setup)) {
        tr_debug("Unsupported channel list");
        mac_mlme_generate_scan_confirmation(resp, msg, MLME_INVALID_PARAMETER);
        mac_generic_event_trig(MAC_MLME_SCAN_CONFIRM_HANDLER, rf_mac_setup, false);
        return;
    }

    if (mac_mlme_generate_scan_confirmation(resp, msg, MLME_SUCCESS) != 0) {
        return;
    }

    tr_debug("MAC: Start MLME scan");

    if (mac_mlme_rf_disable(rf_mac_setup) == 0) {
        rf_mac_setup->macCapRxOnIdle = false;
        if (msg->ScanType == MAC_ACTIVE_SCAN) {
            rf_mac_setup->macUpState = true;
        }
    } else {
        tr_error("Failed to disable RF");
        mac_generic_event_trig(MAC_MLME_SCAN_CONFIRM_HANDLER, rf_mac_setup, false);
        return;
    }

    rf_mac_setup->scan_type = msg->ScanType;
    rf_mac_setup->scan_duration = msg->ScanDuration;
    rf_mac_setup->mac_channel_list = msg->ScanChannels;
    rf_mac_setup->mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
    rf_mac_setup->scan_active = true;
    mac_mlme_scan_start(rf_mac_setup);
}

int8_t mac_mlme_start_req(const mlme_start_t *s, struct protocol_interface_rf_mac_setup *rf_mac_setup)
{
    if (!s || !rf_mac_setup || !rf_mac_setup->dev_driver || !rf_mac_setup->dev_driver->phy_driver) {
        return -1;
    }

    tr_debug("MAC: Start network %u channel %x panid", s->LogicalChannel, s->PANId);
    mac_mlme_set_panid(rf_mac_setup, s->PANId);
    // Synchronize FHSS
    if (rf_mac_setup->fhss_api) {
        rf_mac_setup->fhss_api->synch_state_set(rf_mac_setup->fhss_api, FHSS_SYNCHRONIZED, s->PANId);
    }

    if (!rf_mac_setup->fhss_api) {
        rf_mac_setup->mac_channel = s->LogicalChannel;
    }
    mac_mlme_start_request(rf_mac_setup);
    if (s->PANCoordinator) {
        //tr_debug("Cordinator");
        rf_mac_setup->macCapCordinator = true;
        rf_mac_setup->macCapRxOnIdle = true;
    } else {
        rf_mac_setup->macCapCordinator = false;
    }

    if (s->BatteryLifeExtension) {
        rf_mac_setup->macCapBatteryPowered = true;
    } else {
        rf_mac_setup->macCapBatteryPowered = false;
    }
    mlme_start_conf_t conf;
    conf.status = MLME_SUCCESS;
    mac_mlme_start_confirm_handler(rf_mac_setup, &conf);
    return 0;
}

int8_t mac_mlme_reset(protocol_interface_rf_mac_setup_s *rf_mac_setup, const mlme_reset_t *reset) {
    if (!reset || !rf_mac_setup) {
        return -1;
    }

    // Stop FHSS
    if (rf_mac_setup->fhss_api) {
        rf_mac_setup->fhss_api->synch_state_set(rf_mac_setup->fhss_api, FHSS_UNSYNCHRONIZED, 0);
    }

    mac_mlme_timers_disable(rf_mac_setup);
    mac_mlme_mac_radio_disabled(rf_mac_setup);
    mac_mlme_set_active_state(rf_mac_setup, false);
    mac_mlme_free_scan_temporary_data(rf_mac_setup);
    mac_mcps_buffer_queue_free(rf_mac_setup);
    rf_mac_setup->macProminousMode = false;
    rf_mac_setup->macWaitingData = false;
    rf_mac_setup->macDataPollReq = false;
    rf_mac_setup->macRxDataAtPoll = false;
    //Clean MAC
    if (reset->SetDefaultPIB) {
        tr_debug("RESET MAC PIB");
        rf_mac_setup->mac_short_address = 0xffff;
        rf_mac_setup->pan_id = 0xffff;
        rf_mac_setup->macCapRxOnIdle = true;
        rf_mac_setup->mac_security_enabled = false;
        rf_mac_setup->macCapCordinator = false;
        rf_mac_setup->mac_mlme_retry_max = MAC_DEFAULT_MAX_FRAME_RETRIES;
    }

    return 0;
}


static int8_t mac_mlme_boolean_set(protocol_interface_rf_mac_setup_s *rf_mac_setup,mlme_attr_t attribute ,bool value)
{
    switch (attribute) {
        case macSecurityEnabled:
            rf_mac_setup->mac_security_enabled = value;
            break;

        case macRxOnWhenIdle:
            rf_mac_setup->macCapRxOnIdle = value;
            break;

        case macPromiscuousMode:
            rf_mac_setup->macProminousMode = value;
            break;

        case macGTSPermit:
            rf_mac_setup->macGTSPermit = value;
            break;

        case macAssociationPermit:
            rf_mac_setup->macCapAssocationPermit = value;
            break;
        case macThreadForceLongAddressForBeacon:
            rf_mac_setup->beaconSrcAddressModeLong = value;
            break;

        case macAssociatedPANCoord:

            break;

        case macTimestampSupported:

            break;

        case macBattLifeExt:

            break;

        case macAutoRequest:

            break;

        case macLoadBalancingAcceptAnyBeacon:
            rf_mac_setup->macAcceptAnyBeacon = value;
            if (rf_mac_setup->dev_driver->phy_driver->extension) {
                rf_mac_setup->dev_driver->phy_driver->extension(PHY_EXTENSION_ACCEPT_ANY_BEACON, (uint8_t*)&value);
            }
            break;

        default:
            return -1;
    }
    return 0;
}

static int8_t mac_mlme_16bit_set(protocol_interface_rf_mac_setup_s *rf_mac_setup,mlme_attr_t attribute, uint16_t value)
{
    switch (attribute) {
        case macCoordShortAddress:
            rf_mac_setup->coord_short_address = value;
            break;
        case macPANId:
            mac_mlme_set_panid(rf_mac_setup, value);
            break;

        case macShortAddress:
            mac_mlme_set_mac16(rf_mac_setup, value);
            break;

        case macSyncSymbolOffset:
            //TODO: change this to return real error
            //This is read only
            break;

        case macTransactionPersistenceTime:
            //TODO: check this also
            break;

        default:
            return -1;
    }
    return 0;
}

static int8_t mac_mlme_8bit_set(protocol_interface_rf_mac_setup_s *rf_mac_setup,mlme_attr_t attribute ,uint8_t value)
{
    switch (attribute) {
        case phyCurrentChannel:
            mac_mlme_rf_channel_set(rf_mac_setup, value);
            break;
        case macBeaconPayloadLength:
            if (rf_mac_setup->max_beacon_payload_length < value) {
                return -1;
            }

            rf_mac_setup->mac_beacon_payload_size = value;
            break;
        case macAutoRequestKeyIndex:
            rf_mac_setup->mac_auto_request.KeyIndex = value;
            break;

        case macAutoRequestKeyIdMode:
            rf_mac_setup->mac_auto_request.KeyIdMode = value;
            break;

        case macAutoRequestSecurityLevel:
            rf_mac_setup->mac_auto_request.SecurityLevel = value;
            break;
        case macMaxFrameRetries:
            if (value > 7) {
                return -1;
            }
            rf_mac_setup->mac_mlme_retry_max = value;
            break;

        case macMinBE:
            if (value > rf_mac_setup->macMaxBE) {
                rf_mac_setup->macMinBE = value;
            }
            break;

        case macMaxBE:
            if (value > 8 || value < 3) {
                return -1;
            }
            rf_mac_setup->macMaxBE = value;
            break;

        case macMaxCSMABackoffs:
            if (value > 8 ) {
                return -1;
            }
            rf_mac_setup->macMaxCSMABackoffs = value;
            break;

        default:
            return -1;
    }
    return 0;
}

static int8_t mac_mlme_32bit_set(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_attr_t attribute, uint32_t value)
{
    (void)rf_mac_setup;
    (void) value;
    switch (attribute) {
        case macFrameCounter:
            rf_mac_setup->security_frame_counter = value;
            break;

        default:
            return -1;
    }
    return 0;
}

void mac_extended_mac_set(protocol_interface_rf_mac_setup_s *rf_mac_setup, const uint8_t *mac64)
{
    if( !mac64 || !rf_mac_setup || !rf_mac_setup->dev_driver || !rf_mac_setup->dev_driver->phy_driver ){
        return;
    }
    phy_device_driver_s *dev_driver = rf_mac_setup->dev_driver->phy_driver;
    memcpy(rf_mac_setup->mac64, mac64, 8); //This should be random
    if (dev_driver->address_write) {
        dev_driver->address_write(PHY_MAC_64BIT, rf_mac_setup->mac64);
    }
}

static int8_t mac_mlme_device_description_set(protocol_interface_rf_mac_setup_s *rf_mac_setup,const mlme_set_t *set_req) {

    if (set_req->value_size != sizeof(mlme_device_descriptor_t)) {
        return -1;
    }

    return mac_sec_mib_device_description_set(set_req->attr_index,(mlme_device_descriptor_t *) set_req->value_pointer , rf_mac_setup);
}

static int8_t mac_mlme_key_description_set(protocol_interface_rf_mac_setup_s *rf_mac_setup,const mlme_set_t *set_req) {
    if (set_req->value_size != sizeof(mlme_key_descriptor_entry_t)) {
        return -1;
    }

    return mac_sec_mib_key_description_set(set_req->attr_index,(mlme_key_descriptor_entry_t *) set_req->value_pointer , rf_mac_setup);
}

static int8_t mac_mlme_default_key_source_set(protocol_interface_rf_mac_setup_s *rf_mac_setup,const mlme_set_t *set_req) {
    if (set_req->value_size != 8) {
        return -1;
    }
    if (set_req->attr == macDefaultKeySource) {
        memcpy(rf_mac_setup->mac_default_key_source, (uint8_t *)set_req->value_pointer, 8);
    } else {
        memcpy(rf_mac_setup->mac_auto_request.Keysource, (uint8_t *)set_req->value_pointer, 8);
    }
    return 0;
}

static int8_t mac_mlme_beacon_payload_set(protocol_interface_rf_mac_setup_s *rf_mac_setup, const mlme_set_t *set_req)
{
    if (set_req->value_size > rf_mac_setup->max_beacon_payload_length) {
        return -1;
    }

    memcpy(rf_mac_setup->mac_beacon_payload, set_req->value_pointer, set_req->value_size);
    memset(rf_mac_setup->mac_beacon_payload + set_req->value_size, 0, rf_mac_setup->max_beacon_payload_length - set_req->value_size);

    return 0;
}

static int8_t mac_mlme_handle_set_values(protocol_interface_rf_mac_setup_s *rf_mac_setup,const mlme_set_t *set_req)
{
    if (set_req->value_size == 1) {
        const bool *pbool = set_req->value_pointer;
        //Check first boolean
        if (mac_mlme_boolean_set(rf_mac_setup,set_req->attr, *pbool) == 0) {
            return 0;
        }
        const uint8_t *pu8 = set_req->value_pointer;
        return mac_mlme_8bit_set(rf_mac_setup,set_req->attr, *pu8);

    } else if (set_req->value_size == 2) {
        const uint16_t *pu16 = set_req->value_pointer;
        return mac_mlme_16bit_set(rf_mac_setup,set_req->attr, *pu16);
    } else if (set_req->value_size == 4) {
        const uint32_t *pu32 = set_req->value_pointer;
        return mac_mlme_32bit_set(rf_mac_setup,set_req->attr, *pu32);
    }
    return -1;
}

int8_t mac_mlme_set_req(protocol_interface_rf_mac_setup_s *rf_mac_setup,const mlme_set_t *set_req)
{
    if (!set_req || !rf_mac_setup || !rf_mac_setup->dev_driver || !rf_mac_setup->dev_driver->phy_driver ) {
        return -1;
    }

    switch (set_req->attr) {
        case macDeviceTable:
            return mac_mlme_device_description_set(rf_mac_setup, set_req);
        case macKeyTable:
            return mac_mlme_key_description_set(rf_mac_setup, set_req);
        case macDefaultKeySource:
        case macAutoRequestKeySource:
            return mac_mlme_default_key_source_set(rf_mac_setup, set_req);
        case macBeaconPayload:
            return mac_mlme_beacon_payload_set(rf_mac_setup, set_req);
        case macLoadBalancingBeaconTx:
            return mac_mlme_beacon_tx(rf_mac_setup);
        case macCoordExtendedAddress:
            if( set_req->value_size == 8) {
                memcpy(rf_mac_setup->coord_long_address, set_req->value_pointer, 8);
            }
            return 0;
        default:
            return mac_mlme_handle_set_values(rf_mac_setup, set_req);
    }
}

int8_t mac_mlme_get_req(struct protocol_interface_rf_mac_setup *rf_mac_setup, mlme_get_conf_t *get_req)
{
    if (!get_req || !rf_mac_setup ) {
        return -1;
    }

    switch (get_req->attr) {
        case macDeviceTable:
            get_req->value_pointer = mac_sec_mib_device_description_get_attribute_index(rf_mac_setup, get_req->attr_index);
            if (get_req->value_pointer) {
                get_req->value_size = sizeof(mlme_device_descriptor_t);
            } else {
                get_req->status = MLME_INVALID_INDEX;
            }
            break;

        case macMaxFrameRetries:
            get_req->value_pointer = &rf_mac_setup->mac_mlme_retry_max;
            get_req->value_size = 1;
            break;

        case macFrameCounter:
            get_req->value_pointer = &rf_mac_setup->security_frame_counter;
            get_req->value_size = 4;
            break;

        default:
            get_req->status = MLME_UNSUPPORTED_ATTRIBUTE;
            break;

    }
    return 0;
}

static void mlme_scan_operation(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    uint16_t channel;
    mlme_scan_conf_t *resp = rf_mac_setup->mac_mlme_scan_resp;

    if (rf_mac_setup->scan_type == MAC_ED_SCAN_TYPE) {
        resp->ED_values[resp->ResultListSize] = rf_mac_setup->max_ED;
        resp->ResultListSize++;
    }

    channel = mlme_scan_analyze_next_channel(rf_mac_setup);
    if (channel > 0xff || rf_mac_setup->mac_mlme_scan_resp->ResultListSize == MLME_MAC_RES_SIZE_MAX) {
        resp->status = MLME_SUCCESS;
        tr_debug("Scan Complete..Halt MAC");
        platform_enter_critical();
        mac_mlme_mac_radio_disabled(rf_mac_setup);
        if (resp->ResultListSize == 0 && rf_mac_setup->scan_type == MAC_ACTIVE_SCAN) {
            resp->status = MLME_NO_BEACON;
        }else if( resp->ResultListSize == MLME_MAC_RES_SIZE_MAX ){
            resp->status = MLME_LIMIT_REACHED;
        }
        resp->UnscannedChannels.channel_mask[0] = rf_mac_setup->mac_channel_list.channel_mask[0];
        platform_exit_critical();
        //Scan Confirmation
        mac_generic_event_trig(MAC_MLME_SCAN_CONFIRM_HANDLER, rf_mac_setup, false);
        tr_debug("Trig confirm");
        rf_mac_setup->scan_active = false;
    } else {
        mac_mlme_scan_init((uint8_t) channel, rf_mac_setup);
    }
}

void mac_frame_src_address_set_from_interface(uint8_t SrcAddrMode,protocol_interface_rf_mac_setup_s *rf_ptr, uint8_t *addressPtr)
{
    if( !rf_ptr ){
        return;
    }
    if (SrcAddrMode == MAC_ADDR_MODE_16_BIT) {
        mac_mlme_write_mac16(rf_ptr, addressPtr);
    } else if (SrcAddrMode == MAC_ADDR_MODE_64_BIT) {
        mac_mlme_write_mac64(rf_ptr, addressPtr);
    }
}

void mac_mlme_active_scan_response_timer_start(void *interface)
{
    if (interface) {
        protocol_interface_rf_mac_setup_s *rf_mac_setup = (protocol_interface_rf_mac_setup_s *)interface;
        if (rf_mac_setup) {
            if (rf_mac_setup->mac_mlme_event == ARM_NWK_MAC_MLME_SCAN) {
                eventOS_callback_timer_stop(rf_mac_setup->mlme_timer_id);
                rf_mac_setup->mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
                rf_mac_setup->mlme_tick_count = rf_mac_setup->mlme_ED_counter;

                eventOS_callback_timer_start(rf_mac_setup->mlme_timer_id, 300);
            }
        }
    }
}

static void mac_mlme_timers_disable(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    platform_enter_critical();
    if (rf_ptr->mac_mlme_event != ARM_NWK_MAC_MLME_IDLE) {
        eventOS_callback_timer_stop(rf_ptr->mlme_timer_id);
        rf_ptr->mac_mlme_event = ARM_NWK_MAC_MLME_IDLE;
    }
    timer_mac_stop(rf_ptr);
    platform_exit_critical();
}

void mac_mlme_event_cb(void *mac_ptr)
{
    protocol_interface_rf_mac_setup_s *rf_mac_setup = (protocol_interface_rf_mac_setup_s*) mac_ptr;
    if (!rf_mac_setup) {
        return;
    }
    arm_nwk_mlme_event_type_e event_type;
    event_type = rf_mac_setup->mac_mlme_event;
    rf_mac_setup->mac_mlme_event = ARM_NWK_MAC_MLME_IDLE;
    switch (event_type) {
        case ARM_NWK_MAC_MLME_SCAN:
            mlme_scan_operation(rf_mac_setup);
            break;

        case ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL:
            tr_debug("Data poll data wait TO");
            mac_mlme_poll_process_confirm(rf_mac_setup, MLME_NO_DATA);
            break;

        case ARM_NWK_MAC_MLME_INDIRECT_DATA_POLL_AFTER_DATA:
            mac_mlme_poll_process_confirm(rf_mac_setup, MLME_SUCCESS);
            break;

        default:
            break;
    }
}

static void mac_mcps_timer_cb(int8_t timer_id, uint16_t slots)
{

    protocol_interface_rf_mac_setup_s *rf_ptr = get_sw_mac_ptr_by_timer(timer_id, ARM_MCPS_TIMER);
    if (!rf_ptr || !rf_ptr->dev_driver || !rf_ptr->dev_driver->phy_driver) {
        return;
    }
    rf_ptr->mac_mcps_timer_event.event_data = slots;
    eventOS_event_send(&rf_ptr->mac_mcps_timer_event);

}


static void mac_mlme_timer_cb(int8_t timer_id, uint16_t slots)
{
    protocol_interface_rf_mac_setup_s *rf_ptr = get_sw_mac_ptr_by_timer(timer_id, ARM_NWK_MLME_TIMER);
    if (!rf_ptr || !rf_ptr->dev_driver || !rf_ptr->dev_driver->phy_driver) {
        return;
    }

    if (rf_ptr->mlme_tick_count == 0) {
        if (rf_ptr->mac_mlme_event != ARM_NWK_MAC_MLME_IDLE) {
            if (rf_ptr->mac_mlme_event == ARM_NWK_MAC_MLME_ED_ANALYZE) {
                if (rf_ptr->mlme_ED_counter > 1) {
                    uint8_t ed = 0;
                    phy_device_driver_s *dev_driver = rf_ptr->dev_driver->phy_driver;
                    rf_ptr->mlme_ED_counter--;
                    if( dev_driver->extension ){
                        dev_driver->extension(PHY_EXTENSION_READ_CHANNEL_ENERGY, &ed);
                    }
                    if (ed > rf_ptr->max_ED) {
                        rf_ptr->max_ED = ed;
                    }

                    /*96 * 50us = 4.8ms*/
                    rf_ptr->mlme_tick_count = 0;
                    rf_ptr->mac_mlme_event = ARM_NWK_MAC_MLME_ED_ANALYZE;
                    eventOS_callback_timer_start(timer_id, slots);
                } else {
                    mac_mlme_rf_disable(rf_ptr);
                    rf_ptr->mac_mlme_event = ARM_NWK_MAC_MLME_SCAN;
                    mac_generic_event_trig(MAC_MLME_EVENT_HANDLER, rf_ptr, false);
                }
            } else {
                mac_generic_event_trig(MAC_MLME_EVENT_HANDLER,rf_ptr, true);
            }
        }
    } else {
        rf_ptr->mlme_tick_count--;
        eventOS_callback_timer_start(timer_id, slots);
    }
}

static void mac_mlme_free_scan_temporary_data(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    rf_mac_setup->scan_active = false;
    if (rf_mac_setup->mac_mlme_scan_resp) {
        mlme_scan_conf_t *r = rf_mac_setup->mac_mlme_scan_resp;
        tr_debug("%02x", r->ResultListSize);
        if( r->ED_values ){
            ns_dyn_mem_free(r->ED_values);
            r->ED_values = NULL;
        }
        uint8_t i = 0;
        while (i < r->ResultListSize) {
            if (r->PAN_values[i]) {
                tr_debug("Free PAN result");
                ns_dyn_mem_free(r->PAN_values[i]);
            }
            i++;
        }
        tr_debug("Free Response");
        ns_dyn_mem_free(rf_mac_setup->mac_mlme_scan_resp);
        rf_mac_setup->mac_mlme_scan_resp = NULL;
    }
}

void mac_mlme_set_active_state(protocol_interface_rf_mac_setup_s *entry, bool new_state)
{
    if( entry ){
        entry->macUpState = new_state;
    }
}

void mac_mlme_data_base_deallocate(struct protocol_interface_rf_mac_setup *rf_mac)
{
    if (rf_mac) {
        if( rf_mac->dev_driver ){
            rf_mac->dev_driver->phy_sap_identifier = NULL;
            rf_mac->dev_driver->phy_sap_upper_cb = NULL;
        }

        eventOS_callback_timer_unregister(rf_mac->mlme_timer_id);
        eventOS_callback_timer_unregister(rf_mac->mac_timer_id);
        eventOS_callback_timer_unregister(rf_mac->mac_mcps_timer);

        ns_dyn_mem_free(rf_mac->dev_driver_tx_buffer.buf);
        ns_dyn_mem_free(rf_mac->mac_beacon_payload);

        mac_sec_mib_deinit(rf_mac);
        ns_dyn_mem_free(rf_mac);
    }
}

static uint8_t mac_backoff_ticks_calc(phy_device_driver_s * phy_driver)
{
    //Calculate 20 symbol time which is typically 10 bytes
    const phy_device_channel_page_s *phy_channel_pages = phy_driver->phy_channel_pages;
    uint32_t datarate = dev_get_phy_datarate(phy_driver, phy_channel_pages->channel_page);
    if (datarate == 0) {
        datarate = 250000;
    }
    //How many 10us ticks backoff period is, assuming 4 bits per symbol (O-QPSK)
    unsigned int ticks = (2000000 / (datarate / 4));
    if (ticks > 255) {
        ticks = 255;
        tr_warn("Backoff period too slow");
    }
    return (uint8_t) ticks;
}

protocol_interface_rf_mac_setup_s * mac_mlme_data_base_allocate(uint8_t *mac64, arm_device_driver_list_s *dev_driver, mac_description_storage_size_t *storage_sizes)
{
    uint16_t total_length = 0;
    //allocate security
    if (!dev_driver || !mac64 || !dev_driver->phy_driver || !storage_sizes) {
        return NULL;
    }

    protocol_interface_rf_mac_setup_s *entry = ns_dyn_mem_alloc(sizeof(protocol_interface_rf_mac_setup_s));
    if (!entry) {
        return NULL;
    }
    //Init everything for 0, NULL or false
    memset(entry, 0, sizeof(protocol_interface_rf_mac_setup_s));
    entry->ifs_timer_id = -1;
    entry->cca_timer_id = -1;
    entry->mlme_timer_id = -1;
    entry->mac_timer_id = -1;
    entry->bc_timer_id = -1;
    entry->mac_interface_id = -1;
    entry->dev_driver = dev_driver;

    if (mac_sec_mib_init(entry, storage_sizes) != 0) {
        mac_mlme_data_base_deallocate(entry);
        return NULL;
    }

    if (!dev_driver->phy_driver->phy_MTU) {
        mac_mlme_data_base_deallocate(entry);
        return NULL;
    }

    //Allocate tx buffer by given MTU + header + tail
    total_length = dev_driver->phy_driver->phy_MTU;
    total_length += (dev_driver->phy_driver->phy_header_length + dev_driver->phy_driver->phy_tail_length);
    entry->dev_driver_tx_buffer.buf = ns_dyn_mem_alloc(total_length);
    if (!entry->dev_driver_tx_buffer.buf) {
        mac_mlme_data_base_deallocate(entry);
        return NULL;
    }

    //allocate Beacon Payload buffer
    entry->max_beacon_payload_length = dev_driver->phy_driver->phy_MTU - MAC_IEEE_802_15_4_MAX_BEACON_OVERHEAD;
    entry->mac_beacon_payload = ns_dyn_mem_alloc(entry->max_beacon_payload_length);
    if (!entry->mac_beacon_payload) {
        mac_mlme_data_base_deallocate(entry);
        return NULL;
    }

    entry->mac_tasklet_id = mac_mcps_sap_tasklet_init();
    if (entry->mac_tasklet_id < 0) {
        mac_mlme_data_base_deallocate(entry);
        return NULL;
    }

    entry->mlme_timer_id = eventOS_callback_timer_register(mac_mlme_timer_cb);
    entry->mac_timer_id = eventOS_callback_timer_register(timer_mac_interrupt);
    entry->mac_mcps_timer = eventOS_callback_timer_register(mac_mcps_timer_cb);
    if (entry->mlme_timer_id == -1 || entry->mac_timer_id == -1 || entry->mac_mcps_timer == -1) {
        mac_mlme_data_base_deallocate(entry);
        return NULL;
    }
    entry->macCapRxOnIdle = true;
    entry->macCapSecrutityCapability = true;
    entry->pan_id = entry->mac_short_address = 0xffff;
    mac_extended_mac_set(entry, mac64);
    entry->mac_ack_wait_duration = MAC_ACK_WAIT_DURATION;
    entry->mac_mlme_retry_max = MAC_DEFAULT_MAX_FRAME_RETRIES;
    memset(entry->mac_default_key_source, 0xff, 8);
    memset(entry->mac_auto_request.Keysource, 0xff, 8);
    memset(entry->mac_beacon_payload, 0, entry->max_beacon_payload_length);
    entry->mac_auto_request.SecurityLevel = 6;
    entry->mac_auto_request.KeyIndex = 0xff;
    mac_pd_sap_rf_low_level_function_set(entry, entry->dev_driver);
    entry->mac_sequence = randLIB_get_8bit();
    entry->mac_bea_sequence = randLIB_get_8bit();
    entry->fhss_api = NULL;
    entry->macMinBE = 3;
    entry->macMaxBE = 5;
    entry->macMaxCSMABackoffs = MAC_CCA_MAX;
    entry->mac_mcps_timer_event.priority = ARM_LIB_LOW_PRIORITY_EVENT;
    entry->mac_mcps_timer_event.event_type = MAC_MCPS_INDIRECT_TIMER_CB;
    entry->mac_mcps_timer_event.data_ptr = entry;
    entry->mac_mcps_timer_event.receiver = entry->mac_tasklet_id;
    entry->mac_mcps_timer_event.sender = 0;
    entry->mac_mcps_timer_event.event_id = 0;
    //How many 10us ticks backoff period is for waiting 20symbols which is typically 10 bytes time
    entry->backoff_period_in_10us = mac_backoff_ticks_calc(dev_driver->phy_driver);
    return entry;
}

uint8_t mac_mlme_set_new_sqn(protocol_interface_rf_mac_setup_s *rf_setup)
{
    uint8_t ret_val = 0;

    if (rf_setup) {
        rf_setup->mac_sequence++;
        ret_val = rf_setup->mac_sequence;
    }
    return ret_val;
}

uint8_t mac_mlme_set_new_beacon_sqn(protocol_interface_rf_mac_setup_s *rf_setup)
{
    uint8_t ret_val = 0;

    if (rf_setup) {
        rf_setup->mac_bea_sequence++;
        ret_val = rf_setup->mac_bea_sequence;
    }
    return ret_val;
}

static int8_t mac_mlme_set_panid(struct protocol_interface_rf_mac_setup *rf_setup, uint16_t pan_id)
{
    phy_device_driver_s *dev_driver = rf_setup->dev_driver->phy_driver;
    if (!dev_driver->address_write) {
        if (dev_driver->link_type == PHY_LINK_TUN) {
            rf_setup->pan_id = pan_id;
            return 0;
        }
        return -1;
    }

    uint8_t temp_8[2];
    rf_setup->pan_id = pan_id;
    common_write_16_bit(pan_id, temp_8);

    return dev_driver->address_write(PHY_MAC_PANID, temp_8);
}

static void mac_mle_write_mac16_to_phy(phy_device_driver_s *dev_driver, uint16_t mac16)
{
    uint8_t temp[2];
    common_write_16_bit(mac16, temp);
    if (dev_driver->address_write) {
        dev_driver->address_write(PHY_MAC_16BIT, temp);
    }
}

static int8_t mac_mlme_set_mac16(struct protocol_interface_rf_mac_setup *rf_setup, uint16_t mac16)
{
    int8_t ret_val = -1;
    if (rf_setup) {

        rf_setup->mac_short_address = mac16;
        //SET RF 16-bit
        if (mac16 > 0xfffd) {
            rf_setup->shortAdressValid = false;
        } else {
            rf_setup->shortAdressValid = true;
        }
        mac_mle_write_mac16_to_phy(rf_setup->dev_driver->phy_driver, mac16);
        ret_val = 0;
    }
    return ret_val;
}

static void mac_mlme_write_mac64(protocol_interface_rf_mac_setup_s *rf_setup, uint8_t *addrPtr)
{
    memcpy(addrPtr, rf_setup->mac64, 8);
}

static void mac_mlme_write_mac16(protocol_interface_rf_mac_setup_s *rf_setup, uint8_t *addrPtr)
{
    common_write_16_bit(rf_setup->mac_short_address, addrPtr);
}

uint16_t mac_mlme_get_panid(protocol_interface_rf_mac_setup_s *rf_setup)
{
    uint16_t panId = 0xffff;
    if (rf_setup) {
        panId = rf_setup->pan_id;
    }
    return panId;
}

static bool add_or_update_beacon(mlme_scan_conf_t* conf, mlme_beacon_ind_t *data, fhss_api_t *fhss_api)
{
    bool found = false;
    bool update_beacon = false;
    for( int i=0; i < conf->ResultListSize; i++){
        mlme_pan_descriptor_t *cur_desc = conf->PAN_values[i];
        if( !cur_desc ){ //Not an active or passive scan
            break;
        }
        /* When FHSS is enabled, logical channel check is not valid
         * as the Beacon can be sent on any enabled channel */
        if (fhss_api || (cur_desc->LogicalChannel == data->PANDescriptor.LogicalChannel)) {
            if (cur_desc->CoordPANId == data->PANDescriptor.CoordPANId) {
                if (cur_desc->LinkQuality < data->PANDescriptor.LinkQuality) {
                    cur_desc->CoordAddrMode = data->PANDescriptor.CoordAddrMode;
                    memcpy(cur_desc->CoordAddress, data->PANDescriptor.CoordAddress, 8);
                    cur_desc->LinkQuality = data->PANDescriptor.LinkQuality;
                    update_beacon = true;
                }
                found = true;
            }
        }
    }
    if(!found && conf->ResultListSize != MLME_MAC_RES_SIZE_MAX) {
        mlme_pan_descriptor_t *desc = ns_dyn_mem_temporary_alloc(sizeof(mlme_pan_descriptor_t));
        if( !desc ){
            return false;
        }
        memset(desc, 0, sizeof(mlme_pan_descriptor_t));
        mlme_pan_descriptor_t *dat = &data->PANDescriptor;

        desc->CoordAddrMode = dat->CoordAddrMode;
        desc->CoordPANId = dat->CoordPANId;
        memcpy(desc->CoordAddress, dat->CoordAddress, 8);
        desc->LogicalChannel = dat->LogicalChannel;
        desc->ChannelPage = dat->ChannelPage;
        memcpy(desc->SuperframeSpec, dat->SuperframeSpec, 2);
        desc->GTSPermit = dat->GTSPermit;
        desc->LinkQuality = dat->LinkQuality;
        desc->Timestamp = dat->Timestamp;
        desc->SecurityFailure = dat->SecurityFailure;

        desc->Key.SecurityLevel = dat->Key.SecurityLevel;
        desc->Key.KeyIdMode = dat->Key.KeyIdMode;
        desc->Key.KeyIndex = dat->Key.KeyIndex;
        memcpy(desc->Key.Keysource, dat->Key.Keysource, 8);

        conf->PAN_values[conf->ResultListSize] = desc;
        conf->ResultListSize++;
        update_beacon = true;
    }
    return update_beacon;
}

int mac_mlme_beacon_notify(protocol_interface_rf_mac_setup_s *rf_mac_setup, mlme_beacon_ind_t *data)
{
    bool update_beacon = true;
    bool contains_fhss_synch_info = false;
    if( !rf_mac_setup || !data){
        return -1;
    }

    /* Cut FHSS synchronization info from Beacon payload length.
     * Synchronization info is stored later in this function but
     * should not be delivered to upper layer as a part of Beacon payload.
     */
    if (rf_mac_setup->fhss_api) {
        if (data->beacon_data_length > FHSS_SYNCH_INFO_START) {
            data->beacon_data_length -= FHSS_SYNCH_INFO_LENGTH;
            contains_fhss_synch_info = true;
        } else {
            // Received single channel Beacon when FHSS enabled.
            return 0;
        }
    }

    mac_api_t *mac = get_sw_mac_api(rf_mac_setup);
    if (mac && mac->mlme_ind_cb) {
        mac->mlme_ind_cb(mac, MLME_BEACON_NOTIFY, data);
    }

    tr_debug("Beacon Notify: %s", trace_array(data->beacon_data, data->beacon_data_length));

    if (rf_mac_setup->mac_mlme_scan_resp) {
        mlme_scan_conf_t* conf = rf_mac_setup->mac_mlme_scan_resp;
        update_beacon = add_or_update_beacon(conf, data, rf_mac_setup->fhss_api);
    }
    if (rf_mac_setup->fhss_api && (update_beacon == true)) {
        if (contains_fhss_synch_info == true) {
            // Assume synchronization info is found from the end of the Beacon payload
            uint8_t *synch_info_start = data->beacon_data + data->beacon_data_length;
            rf_mac_setup->fhss_api->receive_frame(rf_mac_setup->fhss_api, data->PANDescriptor.CoordPANId, data->PANDescriptor.CoordAddress,
                    data->PANDescriptor.Timestamp, synch_info_start, FHSS_SYNCH_FRAME);
        }
    }

    return 0;
}

int8_t mac_mlme_virtual_confirmation_handle(int8_t driver_id, const uint8_t *data_ptr, uint16_t length)
{
    (void) length;
    protocol_interface_rf_mac_setup_s *mac_setup = get_sw_mac_ptr_by_driver_id(driver_id);
    if (!mac_setup) {
        return -1;
    }
    mlme_primitive primitive = *data_ptr;
    if (primitive == MLME_SCAN) {
        mlme_scan_conf_t *resp = ns_dyn_mem_temporary_alloc(sizeof(mlme_scan_conf_t));
        if (!resp) {
            return -1;
        }
        memset(resp, 0, sizeof(mlme_scan_conf_t));
        resp->ScanType = MAC_ACTIVE_SCAN;
        mac_setup->mac_mlme_scan_resp = resp;
        mac_mlme_scan_confirmation_handle(mac_setup);
    }
    return 0;
}

static void mac_mlme_start_confirm_handler(protocol_interface_rf_mac_setup_s *rf_ptr, const mlme_start_conf_t *conf)
{
    if (rf_ptr->tun_extension_rf_driver) {
        virtual_data_req_t start_conf;
        uint8_t buf_temp[2];
        uint8_t payload_buf[2];
        // Add some random data as empty payload is not allowed
        payload_buf[0] = 0;
        payload_buf[1] = 0;

        memset(&start_conf, 0, sizeof(virtual_data_req_t));
        buf_temp[0] = NAP_MLME_CONFIRM;
        buf_temp[1] = MLME_START;
        start_conf.parameters = buf_temp;
        start_conf.parameter_length = sizeof(buf_temp);
        start_conf.msdu = payload_buf;
        start_conf.msduLength = sizeof(payload_buf);

        rf_ptr->tun_extension_rf_driver->phy_driver->arm_net_virtual_tx_cb(&start_conf, rf_ptr->tun_extension_rf_driver->id);
    } else if (get_sw_mac_api(rf_ptr)) {
        if (get_sw_mac_api(rf_ptr)->mlme_conf_cb) {
            get_sw_mac_api(rf_ptr)->mlme_conf_cb(get_sw_mac_api(rf_ptr), MLME_START, conf);
        }
    }
}

static void mac_mlme_scan_confirm_handler(protocol_interface_rf_mac_setup_s *rf_ptr, const mlme_scan_conf_t *conf)
{
    if (conf->ScanType == MAC_ACTIVE_SCAN) {
        tr_debug("Active Scan Result");
    } else if (conf->ScanType == MAC_ED_SCAN_TYPE) {
        tr_debug("ED Scan Result");
    }
    if (rf_ptr->tun_extension_rf_driver) {
        virtual_data_req_t scan_conf;
        uint8_t buf_temp[2];
        uint8_t payload_buf[2];
        // Add some random data as empty payload is not allowed
        payload_buf[0] = 0;
        payload_buf[1] = 0;

        memset(&scan_conf, 0, sizeof(virtual_data_req_t));
        buf_temp[0] = NAP_MLME_CONFIRM;
        buf_temp[1] = MLME_SCAN;
        scan_conf.parameters = buf_temp;
        scan_conf.parameter_length = sizeof(buf_temp);
        scan_conf.msdu = payload_buf;
        scan_conf.msduLength = sizeof(payload_buf);

        rf_ptr->tun_extension_rf_driver->phy_driver->arm_net_virtual_tx_cb(&scan_conf, rf_ptr->tun_extension_rf_driver->id);
    } else if (get_sw_mac_api(rf_ptr)) {
        if (get_sw_mac_api(rf_ptr)->mlme_conf_cb) {
            get_sw_mac_api(rf_ptr)->mlme_conf_cb(get_sw_mac_api(rf_ptr), MLME_SCAN, conf);
        }
    }
}

void mac_mlme_scan_confirmation_handle(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    if( !rf_ptr ){
        return;
    }
    mlme_scan_conf_t *r = rf_ptr->mac_mlme_scan_resp;

    if (r) {
        mac_mlme_scan_confirm_handler(rf_ptr, r);
    }
    mac_mlme_free_scan_temporary_data(rf_ptr);
}

void mac_mlme_mac_radio_disabled(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if( !rf_mac_setup || !rf_mac_setup->dev_driver || !rf_mac_setup->dev_driver->phy_driver ){
        return;
    }
    platform_enter_critical();
    timer_mac_stop(rf_mac_setup);
    mac_mlme_rf_disable(rf_mac_setup);
    platform_exit_critical();
}

void mac_mlme_mac_radio_enable(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    if( !rf_mac_setup || !rf_mac_setup->dev_driver || !rf_mac_setup->dev_driver->phy_driver){
        return;
    }
    platform_enter_critical();
    mac_mlme_rf_receiver_enable(rf_mac_setup);
    platform_exit_critical();
}

static int8_t mac_mlme_rf_disable(protocol_interface_rf_mac_setup_s *rf_mac_setup)
{
    int8_t ret_val = 0;
    phy_device_driver_s *dev_driver = rf_mac_setup->dev_driver->phy_driver;
    if (!dev_driver->state_control) {
        if (dev_driver->link_type == PHY_LINK_TUN) {
            rf_mac_setup->macRfRadioOn = false;
            rf_mac_setup->macRfRadioTxActive = false;
            return 0;
        }
        return -1;
    }
    if (rf_mac_setup->macRfRadioOn) {
        ret_val = dev_driver->state_control(PHY_INTERFACE_DOWN, 0);
        rf_mac_setup->macRfRadioOn = false;
        rf_mac_setup->macRfRadioTxActive = false;
    }

    return ret_val;
}

static int8_t mac_mlme_rf_receiver_enable(struct protocol_interface_rf_mac_setup *rf_mac_setup)
{
    int8_t retval;

    phy_device_driver_s *dev_driver = rf_mac_setup->dev_driver->phy_driver;
    if (!dev_driver->state_control) {
        if (dev_driver->link_type == PHY_LINK_TUN) {
            rf_mac_setup->macRfRadioOn = true;
            return 0;
        }
        return -1;
    }

    if (rf_mac_setup->macRfRadioOn) {
        return 0;
    }

    if (rf_mac_setup->macProminousMode) {
        tr_debug("Sniffer mode");
        retval = dev_driver->state_control(PHY_INTERFACE_SNIFFER_STATE, rf_mac_setup->mac_channel);
    } else {
        retval = dev_driver->state_control(PHY_INTERFACE_UP, rf_mac_setup->mac_channel);
    }
    rf_mac_setup->macRfRadioOn = true;
    //tr_debug("Enable radio with channel %u", rf_mac_setup->mac_channel);
    return retval;
}

/**
 * Initialize MAC channel selection sequence
 *
 * TODO: initialize channel select sequence
 *       in coordinator mode
 *
 * \param new_channel channel to set
 *
 * \return 0 success
 * \return -1 HW error
 */
static int8_t mac_mlme_rf_channel_set(struct protocol_interface_rf_mac_setup *rf_setup, uint8_t new_channel)
{
        if (new_channel == rf_setup->mac_channel) {
            return 0;
        }
        mac_pre_build_frame_t *buf;

        //Disable allways
        mac_mlme_mac_radio_disabled(rf_setup);
        buf = rf_setup->active_pd_data_request;
        rf_setup->active_pd_data_request = NULL;
        //Set Channel
        rf_setup->mac_channel = new_channel;
        //Enable Radio
        mac_mlme_mac_radio_enable(rf_setup);
        if (buf) {
            mcps_sap_pd_req_queue_write(rf_setup, buf);
        }

    return 0;
}

/**
 * MAC channel change
 *
 * \param new_channel channel to set
 *
 * \return 0 success
 * \return -1 error
 */
int8_t mac_mlme_rf_channel_change(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t new_channel)
{
    if (!rf_mac_setup || !rf_mac_setup->dev_driver || !rf_mac_setup->dev_driver->phy_driver) {
        return -1;
    }

    if (!rf_mac_setup->dev_driver->phy_driver->extension) {
        if (rf_mac_setup->dev_driver->phy_driver->link_type == PHY_LINK_TUN) {
            rf_mac_setup->mac_channel = new_channel;
            return 0;
        }
        return -1;
    }
    if (new_channel == rf_mac_setup->mac_channel) {
        return 0;
    }
    platform_enter_critical();
    rf_mac_setup->mac_channel = new_channel;
    rf_mac_setup->dev_driver->phy_driver->extension(PHY_EXTENSION_SET_CHANNEL, &rf_mac_setup->mac_channel);
    platform_exit_critical();
    return 0;
}

void mac_mlme_poll_process_confirm(protocol_interface_rf_mac_setup_s *rf_mac_setup, uint8_t status)
{
    if (!rf_mac_setup || !rf_mac_setup->dev_driver || !rf_mac_setup->dev_driver->phy_driver) {
        return;
    }

    //Free active Data buffer
    if (rf_mac_setup->active_pd_data_request) {
        mcps_sap_prebuild_frame_buffer_free(rf_mac_setup->active_pd_data_request);
        rf_mac_setup->active_pd_data_request = NULL;
    }
    //Disable timer
    rf_mac_setup->macWaitingData = false;
    rf_mac_setup->macDataPollReq = false;
    rf_mac_setup->macRxDataAtPoll = false;

    if (!rf_mac_setup->macCapRxOnIdle) {
        //Disable Radio If we are RX off at idle device
        //tr_debug("disbale by aceptance data");
        if (!rf_mac_setup->macRfRadioTxActive ) {
            //Disable radio if no active TX and radio is enabled
            //tr_debug("RF disable");
            mac_mlme_mac_radio_disabled(rf_mac_setup);
        }
    }

    mac_api_t *mac_api = get_sw_mac_api(rf_mac_setup);
    if( mac_api ) {
        mlme_poll_conf_t confirm;
        confirm.status = status;
        mac_api->mlme_conf_cb(mac_api,MLME_POLL, &confirm);
    }
    //Trig Packet from queue
    mac_mcps_trig_buffer_from_queue(rf_mac_setup);

}

void mac_mlme_poll_req(protocol_interface_rf_mac_setup_s *cur, const mlme_poll_t *poll_req)
{
    if( !cur || !poll_req ){
        return;
    }
    if (cur->macDataPollReq ) {
        tr_debug("Poll Active do not start new");
        return;
    }
    mac_pre_build_frame_t *buf = mcps_sap_prebuild_frame_buffer_get(0);
    if (!buf) {
        tr_debug("No mem for data Req");
        //Confirmation call here
        return;
    }

    buf->fcf_dsn.frametype = FC_CMD_FRAME;
    buf->fcf_dsn.ackRequested = true;
    buf->fcf_dsn.intraPan = true;

    buf->DstPANId = poll_req->CoordPANId;
    buf->SrcPANId = poll_req->CoordPANId;
    buf->fcf_dsn.DstAddrMode = poll_req->CoordAddrMode;
    memcpy(buf->DstAddr, poll_req->CoordAddress, 8);

    buf->mac_command_id = MAC_DATA_REQ;
    buf->mac_payload = &buf->mac_command_id;
    buf->mac_payload_length = 1;
    buf->mac_header_length_with_security = 3;

    mac_header_security_parameter_set(&buf->aux_header, &poll_req->Key);

    if (buf->aux_header.securityLevel) {
        buf->fcf_dsn.securityEnabled = true;
        buf->fcf_dsn.frameVersion = MAC_FRAME_VERSION_2006;

    }
    cur->macDataPollReq = true;
    cur->macWaitingData = true;
    cur->macRxDataAtPoll = false;

    buf->security_mic_len = mac_security_mic_length_get(buf->aux_header.securityLevel);
    buf->mac_header_length_with_security += mac_header_security_aux_header_length(buf->aux_header.securityLevel, buf->aux_header.KeyIdMode);

    if ( cur->mac_short_address > 0xfffd ) {
        buf->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
    } else {
        buf->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
    }
    mac_frame_src_address_set_from_interface(buf->fcf_dsn.SrcAddrMode, cur, buf->SrcAddr);
    buf->mac_header_length_with_security += mac_header_address_length(&buf->fcf_dsn);
    buf->priority = MAC_PD_DATA_MEDIUM_PRIORITY;
    mcps_sap_pd_req_queue_write(cur, buf);
}

static bool mac_mlme_beacon_at_queue(protocol_interface_rf_mac_setup_s *rf_ptr) {
    mac_pre_build_frame_t *buf = rf_ptr->active_pd_data_request;
    if (buf && buf->fcf_dsn.frametype == FC_BEACON_FRAME ) {
        return true;
    }

    buf = rf_ptr->pd_data_request_queue_to_go;

    while(buf) {
        if (buf->fcf_dsn.frametype == FC_BEACON_FRAME ) {
            return true;
        }
        buf = buf->next;
    }
    return false;
}

int8_t mac_mlme_beacon_tx(protocol_interface_rf_mac_setup_s *rf_ptr)
{
    if (!rf_ptr || !rf_ptr->macCapCordinator) {
        return -1;
    }

    //Verify if beacon is already generated to queue
    if (mac_mlme_beacon_at_queue(rf_ptr)) {
        return -2;
    }

    uint16_t length = 4;
    length += rf_ptr->mac_beacon_payload_size;
    // Add more space for FHSS synchronization info
    if (rf_ptr->fhss_api) {
        length += FHSS_SYNCH_INFO_LENGTH;
    }
   /* if (rf_ptr->beacon_join_priority_tx_cb_ptr) {
        length += 2;
    }*/

    mac_pre_build_frame_t *buf = mcps_sap_prebuild_frame_buffer_get(length);
    if(!buf) {
        return -1;
    }
    uint8_t sf_2 = 0x0f;

    buf->fcf_dsn.frametype = FC_BEACON_FRAME;
    buf->fcf_dsn.DstAddrMode = MAC_ADDR_MODE_NONE;

    if (rf_ptr->beaconSrcAddressModeLong) {
        buf->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
        buf->mac_header_length_with_security = 13;
    } else {
        if (rf_ptr->shortAdressValid) {
            buf->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_16_BIT;
            buf->mac_header_length_with_security = 7;
        } else {
            buf->fcf_dsn.SrcAddrMode = MAC_ADDR_MODE_64_BIT;
            buf->mac_header_length_with_security = 13;
        }
    }
    buf->SrcPANId = mac_mlme_get_panid(rf_ptr);
    mac_frame_src_address_set_from_interface(buf->fcf_dsn.SrcAddrMode, rf_ptr, buf->SrcAddr);
    uint8_t *ptr = buf->mac_payload;

    *ptr++ = 0xff;//Superframe disabled

    if (rf_ptr->macCapCordinator) {
        sf_2 |= 0x40; //Cord
    }

    if (rf_ptr->macCapBatteryPowered) {
        sf_2 |= 0x10; //Battery
    }

    if (rf_ptr->macCapAssocationPermit) {
        sf_2 |= 0x80; //Permit
    }
    *ptr++ = sf_2; //Superframe desc MSB
    ptr = common_write_16_bit(0, ptr); //NO GTS Support

    if (rf_ptr->mac_beacon_payload_size) {
        memcpy(ptr, rf_ptr->mac_beacon_payload, rf_ptr->mac_beacon_payload_size);

        /*if (rf_ptr->beacon_join_priority_tx_cb_ptr) {
            uint8_t beacon_join_priority = rf_ptr->beacon_join_priority_tx_cb_ptr(rf_ptr->mac_interface_id);
            ptr[PLAIN_BEACON_PAYLOAD_SIZE] = BEACON_OPTION_JOIN_PRIORITY_TYPE_LEN;
            ptr[PLAIN_BEACON_PAYLOAD_SIZE + 1] = beacon_join_priority;
            ptr += BEACON_OPTION_JOIN_PRIORITY_LEN;
        }*/
    }
    buf->priority = MAC_PD_DATA_HIGH_PRIOTITY;

    tr_debug("BEA tx");
    mcps_sap_pd_req_queue_write(rf_ptr, buf);
    sw_mac_stats_update(rf_ptr, STAT_MAC_BEA_TX_COUNT, 0);
    return 0;
}

