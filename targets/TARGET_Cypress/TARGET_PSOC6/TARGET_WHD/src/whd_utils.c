/*
 * Copyright 2019 Cypress Semiconductor Corporation
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
 * @file WHD utilities
 *
 * Utilities to help do specialized (not general purpose) WHD specific things
 */
#include "whd_debug.h"
#include "whd_utils.h"
#include "whd_chip_constants.h"
#include "whd_endian.h"
#include "whd_int.h"

#define UNSIGNED_CHAR_TO_CHAR(uch) ( (uch)& 0x7f )

#define RSPEC_KBPS_MASK (0x7f)
#define RSPEC_500KBPS(rate) ( (rate)&  RSPEC_KBPS_MASK )
#define RSPEC_TO_KBPS(rate) (RSPEC_500KBPS( (rate) ) * (unsigned int)500)

#define OTP_WORD_SIZE 16    /* Word size in bits */
#define WPA_OUI_TYPE1                     "\x00\x50\xF2\x01"   /** WPA OUI */

/******************************************************
*             Static Variables
******************************************************/
whd_tlv8_data_t *whd_tlv_find_tlv8(const uint8_t *message, uint32_t message_length, uint8_t type);

whd_tlv8_data_t *whd_tlv_find_tlv8(const uint8_t *message, uint32_t message_length, uint8_t type)
{
    while (message_length != 0)
    {
        uint8_t current_tlv_type   = message[0];
        uint16_t current_tlv_length = (uint16_t)(message[1] + 2);

        /* Check if we've overrun the buffer */
        if (current_tlv_length > message_length)
        {
            return NULL;
        }

        /* Check if we've found the type we are looking for */
        if (current_tlv_type == type)
        {
            return (whd_tlv8_data_t *)message;
        }

        /* Skip current TLV */
        message        += current_tlv_length;
        message_length -= current_tlv_length;
    }
    return 0;
}

inline whd_tlv8_header_t *whd_parse_tlvs(const whd_tlv8_header_t *tlv_buf, uint32_t buflen,
                                         dot11_ie_id_t key)
{
    return (whd_tlv8_header_t *)whd_tlv_find_tlv8( (const uint8_t *)tlv_buf, buflen, key );
}

whd_bool_t whd_is_wpa_ie(vendor_specific_ie_header_t *wpaie, whd_tlv8_header_t **tlvs, uint32_t *tlvs_len)
{
    whd_tlv8_header_t *prev_tlvs = *tlvs;
    whd_tlv8_header_t *new_tlvs = *tlvs;
    vendor_specific_ie_header_t *ie = wpaie;

    /* If the contents match the WPA_OUI and type=1 */
    if ( (ie->tlv_header.length >= (uint8_t)VENDOR_SPECIFIC_IE_MINIMUM_LENGTH) &&
         (memcmp(ie->oui, WPA_OUI_TYPE1, sizeof(ie->oui) ) == 0) )
    {
        /* Found the WPA IE */
        return WHD_TRUE;
    }

    /* calculate the next ie address */
    new_tlvs = (whd_tlv8_header_t *)( ( (uint8_t *)ie ) + ie->tlv_header.length + sizeof(whd_tlv8_header_t) );

    /* check the rest of length of buffer */
    if (*tlvs_len < (uint32_t)( ( (uint8_t *)new_tlvs ) - ( (uint8_t *)prev_tlvs ) ) )
    {
        /* set rest of length to zero to avoid buffer overflow */
        *tlvs_len = 0;
    }
    else
    {
        /* point to the next ie */
        *tlvs = new_tlvs;

        /* tlvs now points to the beginning of next IE pointer, and *ie points to one or more TLV further
         * down from the *prev_tlvs. So the tlvs_len need to be adjusted by prev_tlvs instead of *ie */
        *tlvs_len -= (uint32_t)( ( (uint8_t *)*tlvs ) - ( (uint8_t *)prev_tlvs ) );
    }

    return WHD_FALSE;
}

whd_tlv8_header_t *whd_parse_dot11_tlvs(const whd_tlv8_header_t *tlv_buf, uint32_t buflen, dot11_ie_id_t key)
{
    return (whd_tlv8_header_t *)whd_tlv_find_tlv8( (const uint8_t *)tlv_buf, buflen, key );
}

#ifdef WPRINT_ENABLE_WHD_DEBUG
char *whd_ssid_to_string(uint8_t *value, uint8_t length, char *ssid_buf, uint8_t ssid_buf_len)
{
    memset(ssid_buf, 0, ssid_buf_len);

    if (ssid_buf_len > 0)
    {
        memcpy(ssid_buf, value, ssid_buf_len < length ? ssid_buf_len : length);
    }

    return ssid_buf;
}

/* When adding new events, update this switch statement to print correct string */
#define CASE_RETURN_STRING(value) case value: \
        return # value;

#define CASE_RETURN(value) case value: \
        break;

const char *whd_event_to_string(whd_event_num_t value)
{
    switch (value)
    {
        CASE_RETURN_STRING(WLC_E_ULP)
        CASE_RETURN(WLC_E_BT_WIFI_HANDOVER_REQ)
        CASE_RETURN(WLC_E_SPW_TXINHIBIT)
        CASE_RETURN(WLC_E_FBT_AUTH_REQ_IND)
        CASE_RETURN(WLC_E_RSSI_LQM)
        CASE_RETURN(WLC_E_PFN_GSCAN_FULL_RESULT)
        CASE_RETURN(WLC_E_PFN_SWC)
        CASE_RETURN(WLC_E_AUTHORIZED)
        CASE_RETURN(WLC_E_PROBREQ_MSG_RX)
        CASE_RETURN(WLC_E_RMC_EVENT)
        CASE_RETURN(WLC_E_DPSTA_INTF_IND)
        CASE_RETURN_STRING(WLC_E_NONE)
        CASE_RETURN_STRING(WLC_E_SET_SSID)
        CASE_RETURN(WLC_E_PFN_BEST_BATCHING)
        CASE_RETURN(WLC_E_JOIN)
        CASE_RETURN(WLC_E_START)
        CASE_RETURN_STRING(WLC_E_AUTH)
        CASE_RETURN(WLC_E_AUTH_IND)
        CASE_RETURN(WLC_E_DEAUTH)
        CASE_RETURN_STRING(WLC_E_DEAUTH_IND)
        CASE_RETURN(WLC_E_ASSOC)
        CASE_RETURN(WLC_E_ASSOC_IND)
        CASE_RETURN(WLC_E_REASSOC)
        CASE_RETURN(WLC_E_REASSOC_IND)
        CASE_RETURN(WLC_E_DISASSOC)
        CASE_RETURN_STRING(WLC_E_DISASSOC_IND)
        CASE_RETURN(WLC_E_ROAM)
        CASE_RETURN(WLC_E_ROAM_PREP)
        CASE_RETURN(WLC_E_ROAM_START)
        CASE_RETURN(WLC_E_QUIET_START)
        CASE_RETURN(WLC_E_QUIET_END)
        CASE_RETURN(WLC_E_BEACON_RX)
        CASE_RETURN_STRING(WLC_E_LINK)
        CASE_RETURN_STRING(WLC_E_RRM)
        CASE_RETURN(WLC_E_MIC_ERROR)
        CASE_RETURN(WLC_E_NDIS_LINK)
        CASE_RETURN(WLC_E_TXFAIL)
        CASE_RETURN(WLC_E_PMKID_CACHE)
        CASE_RETURN(WLC_E_RETROGRADE_TSF)
        CASE_RETURN(WLC_E_PRUNE)
        CASE_RETURN(WLC_E_AUTOAUTH)
        CASE_RETURN(WLC_E_EAPOL_MSG)
        CASE_RETURN(WLC_E_SCAN_COMPLETE)
        CASE_RETURN(WLC_E_ADDTS_IND)
        CASE_RETURN(WLC_E_DELTS_IND)
        CASE_RETURN(WLC_E_BCNSENT_IND)
        CASE_RETURN(WLC_E_BCNRX_MSG)
        CASE_RETURN(WLC_E_BCNLOST_MSG)
        CASE_RETURN_STRING(WLC_E_PFN_NET_FOUND)
        CASE_RETURN(WLC_E_PFN_NET_LOST)
        CASE_RETURN(WLC_E_RESET_COMPLETE)
        CASE_RETURN(WLC_E_JOIN_START)
        CASE_RETURN(WLC_E_ASSOC_START)
        CASE_RETURN(WLC_E_IBSS_ASSOC)
        CASE_RETURN(WLC_E_RADIO)
        CASE_RETURN(WLC_E_PSM_WATCHDOG)
        CASE_RETURN(WLC_E_CCX_ASSOC_START)
        CASE_RETURN(WLC_E_CCX_ASSOC_ABORT)
        CASE_RETURN(WLC_E_PROBREQ_MSG)
        CASE_RETURN(WLC_E_SCAN_CONFIRM_IND)
        CASE_RETURN_STRING(WLC_E_PSK_SUP)
        CASE_RETURN(WLC_E_COUNTRY_CODE_CHANGED)
        CASE_RETURN(WLC_E_EXCEEDED_MEDIUM_TIME)
        CASE_RETURN(WLC_E_ICV_ERROR)
        CASE_RETURN(WLC_E_UNICAST_DECODE_ERROR)
        CASE_RETURN(WLC_E_MULTICAST_DECODE_ERROR)
        CASE_RETURN(WLC_E_TRACE)
        CASE_RETURN(WLC_E_BTA_HCI_EVENT)
        CASE_RETURN(WLC_E_IF)
        CASE_RETURN(WLC_E_P2P_DISC_LISTEN_COMPLETE)
        CASE_RETURN(WLC_E_RSSI)
        CASE_RETURN_STRING(WLC_E_PFN_SCAN_COMPLETE)
        CASE_RETURN(WLC_E_EXTLOG_MSG)
        CASE_RETURN(WLC_E_ACTION_FRAME)
        CASE_RETURN(WLC_E_ACTION_FRAME_COMPLETE)
        CASE_RETURN(WLC_E_PRE_ASSOC_IND)
        CASE_RETURN(WLC_E_PRE_REASSOC_IND)
        CASE_RETURN(WLC_E_CHANNEL_ADOPTED)
        CASE_RETURN(WLC_E_AP_STARTED)
        CASE_RETURN(WLC_E_DFS_AP_STOP)
        CASE_RETURN(WLC_E_DFS_AP_RESUME)
        CASE_RETURN(WLC_E_WAI_STA_EVENT)
        CASE_RETURN(WLC_E_WAI_MSG)
        CASE_RETURN_STRING(WLC_E_ESCAN_RESULT)
        CASE_RETURN(WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE)
        CASE_RETURN(WLC_E_PROBRESP_MSG)
        CASE_RETURN(WLC_E_P2P_PROBREQ_MSG)
        CASE_RETURN(WLC_E_DCS_REQUEST)
        CASE_RETURN(WLC_E_FIFO_CREDIT_MAP)
        CASE_RETURN(WLC_E_ACTION_FRAME_RX)
        CASE_RETURN(WLC_E_WAKE_EVENT)
        CASE_RETURN(WLC_E_RM_COMPLETE)
        CASE_RETURN(WLC_E_HTSFSYNC)
        CASE_RETURN(WLC_E_OVERLAY_REQ)
        CASE_RETURN_STRING(WLC_E_CSA_COMPLETE_IND)
        CASE_RETURN(WLC_E_EXCESS_PM_WAKE_EVENT)
        CASE_RETURN(WLC_E_PFN_SCAN_NONE)
        CASE_RETURN(WLC_E_PFN_SCAN_ALLGONE)
        CASE_RETURN(WLC_E_GTK_PLUMBED)
        CASE_RETURN(WLC_E_ASSOC_IND_NDIS)
        CASE_RETURN(WLC_E_REASSOC_IND_NDIS)
        CASE_RETURN(WLC_E_ASSOC_REQ_IE)
        CASE_RETURN(WLC_E_ASSOC_RESP_IE)
        CASE_RETURN(WLC_E_ASSOC_RECREATED)
        CASE_RETURN(WLC_E_ACTION_FRAME_RX_NDIS)
        CASE_RETURN(WLC_E_AUTH_REQ)
        CASE_RETURN(WLC_E_TDLS_PEER_EVENT)
        CASE_RETURN(WLC_E_SPEEDY_RECREATE_FAIL)
        CASE_RETURN(WLC_E_NATIVE)
        CASE_RETURN(WLC_E_PKTDELAY_IND)
        CASE_RETURN(WLC_E_AWDL_AW)
        CASE_RETURN(WLC_E_AWDL_ROLE)
        CASE_RETURN(WLC_E_AWDL_EVENT)
        CASE_RETURN(WLC_E_NIC_AF_TXS)
        CASE_RETURN(WLC_E_NAN)
        CASE_RETURN(WLC_E_BEACON_FRAME_RX)
        CASE_RETURN(WLC_E_SERVICE_FOUND)
        CASE_RETURN(WLC_E_GAS_FRAGMENT_RX)
        CASE_RETURN(WLC_E_GAS_COMPLETE)
        CASE_RETURN(WLC_E_P2PO_ADD_DEVICE)
        CASE_RETURN(WLC_E_P2PO_DEL_DEVICE)
        CASE_RETURN(WLC_E_WNM_STA_SLEEP)
        CASE_RETURN(WLC_E_TXFAIL_THRESH)
        CASE_RETURN(WLC_E_PROXD)
        CASE_RETURN(WLC_E_IBSS_COALESCE)
        CASE_RETURN(WLC_E_AWDL_RX_PRB_RESP)
        CASE_RETURN(WLC_E_AWDL_RX_ACT_FRAME)
        CASE_RETURN(WLC_E_AWDL_WOWL_NULLPKT)
        CASE_RETURN(WLC_E_AWDL_PHYCAL_STATUS)
        CASE_RETURN(WLC_E_AWDL_OOB_AF_STATUS)
        CASE_RETURN(WLC_E_AWDL_SCAN_STATUS)
        CASE_RETURN(WLC_E_AWDL_AW_START)
        CASE_RETURN(WLC_E_AWDL_AW_END)
        CASE_RETURN(WLC_E_AWDL_AW_EXT)
        CASE_RETURN(WLC_E_AWDL_PEER_CACHE_CONTROL)
        CASE_RETURN(WLC_E_CSA_START_IND)
        CASE_RETURN(WLC_E_CSA_DONE_IND)
        CASE_RETURN(WLC_E_CSA_FAILURE_IND)
        CASE_RETURN(WLC_E_CCA_CHAN_QUAL)
        CASE_RETURN(WLC_E_BSSID)
        CASE_RETURN(WLC_E_TX_STAT_ERROR)
        CASE_RETURN(WLC_E_BCMC_CREDIT_SUPPORT)
        CASE_RETURN(WLC_E_PSTA_PRIMARY_INTF_IND)
        case WLC_E_LAST:
        default:
            return "Unknown";

            break;
    }

    return "Unknown";
}

const char *whd_status_to_string(whd_event_status_t status)
{
    switch (status)
    {
        CASE_RETURN_STRING(WLC_E_STATUS_SUCCESS)
        CASE_RETURN_STRING(WLC_E_STATUS_FAIL)
        CASE_RETURN_STRING(WLC_E_STATUS_TIMEOUT)
        CASE_RETURN_STRING(WLC_E_STATUS_NO_NETWORKS)
        CASE_RETURN_STRING(WLC_E_STATUS_ABORT)
        CASE_RETURN_STRING(WLC_E_STATUS_NO_ACK)
        CASE_RETURN_STRING(WLC_E_STATUS_UNSOLICITED)
        CASE_RETURN_STRING(WLC_E_STATUS_ATTEMPT)
        CASE_RETURN_STRING(WLC_E_STATUS_PARTIAL)
        CASE_RETURN_STRING(WLC_E_STATUS_NEWSCAN)
        CASE_RETURN_STRING(WLC_E_STATUS_NEWASSOC)
        CASE_RETURN_STRING(WLC_E_STATUS_11HQUIET)
        CASE_RETURN_STRING(WLC_E_STATUS_SUPPRESS)
        CASE_RETURN_STRING(WLC_E_STATUS_NOCHANS)
        CASE_RETURN_STRING(WLC_E_STATUS_CCXFASTRM)
        CASE_RETURN_STRING(WLC_E_STATUS_CS_ABORT)
        CASE_RETURN_STRING(WLC_SUP_DISCONNECTED)
        CASE_RETURN_STRING(WLC_SUP_CONNECTING)
        CASE_RETURN_STRING(WLC_SUP_IDREQUIRED)
        CASE_RETURN_STRING(WLC_SUP_AUTHENTICATING)
        CASE_RETURN_STRING(WLC_SUP_AUTHENTICATED)
        CASE_RETURN_STRING(WLC_SUP_KEYXCHANGE)
        CASE_RETURN_STRING(WLC_SUP_KEYED)
        CASE_RETURN_STRING(WLC_SUP_TIMEOUT)
        CASE_RETURN_STRING(WLC_SUP_LAST_BASIC_STATE)
        CASE_RETURN_STRING(WLC_SUP_KEYXCHANGE_PREP_M4)
        CASE_RETURN_STRING(WLC_SUP_KEYXCHANGE_WAIT_G1)
        CASE_RETURN_STRING(WLC_SUP_KEYXCHANGE_PREP_G2)
        CASE_RETURN_STRING(WLC_DOT11_SC_SUCCESS)
        CASE_RETURN_STRING(WLC_DOT11_SC_FAILURE)
        CASE_RETURN_STRING(WLC_DOT11_SC_CAP_MISMATCH)
        CASE_RETURN_STRING(WLC_DOT11_SC_REASSOC_FAIL)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_FAIL)
        CASE_RETURN_STRING(WLC_DOT11_SC_AUTH_MISMATCH)
        CASE_RETURN_STRING(WLC_DOT11_SC_AUTH_SEQ)
        CASE_RETURN_STRING(WLC_DOT11_SC_AUTH_CHALLENGE_FAIL)
        CASE_RETURN_STRING(WLC_DOT11_SC_AUTH_TIMEOUT)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_BUSY_FAIL)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_RATE_MISMATCH)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_SHORT_REQUIRED)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_PBCC_REQUIRED)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_AGILITY_REQUIRED)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_SPECTRUM_REQUIRED)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_BAD_POWER_CAP)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_BAD_SUP_CHANNELS)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_SHORTSLOT_REQUIRED)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_ERPBCC_REQUIRED)
        CASE_RETURN_STRING(WLC_DOT11_SC_ASSOC_DSSOFDM_REQUIRED)
        CASE_RETURN_STRING(WLC_DOT11_SC_DECLINED)
        CASE_RETURN_STRING(WLC_DOT11_SC_INVALID_PARAMS)
        CASE_RETURN_STRING(WLC_DOT11_SC_INVALID_AKMP)
        CASE_RETURN_STRING(WLC_DOT11_SC_INVALID_MDID)
        CASE_RETURN_STRING(WLC_DOT11_SC_INVALID_FTIE)
        case WLC_E_STATUS_FORCE_32_BIT:
        default:
            break;
    }
    return "Unknown";
}

const char *whd_reason_to_string(whd_event_reason_t reason)
{
    switch (reason)
    {
        CASE_RETURN_STRING(WLC_E_REASON_INITIAL_ASSOC)
        CASE_RETURN_STRING(WLC_E_REASON_LOW_RSSI)
        CASE_RETURN_STRING(WLC_E_REASON_DEAUTH)
        CASE_RETURN_STRING(WLC_E_REASON_DISASSOC)
        CASE_RETURN_STRING(WLC_E_REASON_BCNS_LOST)
        CASE_RETURN_STRING(WLC_E_REASON_FAST_ROAM_FAILED)
        CASE_RETURN_STRING(WLC_E_REASON_DIRECTED_ROAM)
        CASE_RETURN_STRING(WLC_E_REASON_TSPEC_REJECTED)
        CASE_RETURN_STRING(WLC_E_REASON_BETTER_AP)
        CASE_RETURN_STRING(WLC_E_PRUNE_ENCR_MISMATCH)
        CASE_RETURN_STRING(WLC_E_PRUNE_BCAST_BSSID)
        CASE_RETURN_STRING(WLC_E_PRUNE_MAC_DENY)
        CASE_RETURN_STRING(WLC_E_PRUNE_MAC_NA)
        CASE_RETURN_STRING(WLC_E_PRUNE_REG_PASSV)
        CASE_RETURN_STRING(WLC_E_PRUNE_SPCT_MGMT)
        CASE_RETURN_STRING(WLC_E_PRUNE_RADAR)
        CASE_RETURN_STRING(WLC_E_RSN_MISMATCH)
        CASE_RETURN_STRING(WLC_E_PRUNE_NO_COMMON_RATES)
        CASE_RETURN_STRING(WLC_E_PRUNE_BASIC_RATES)
        CASE_RETURN_STRING(WLC_E_PRUNE_CCXFAST_PREVAP)
        CASE_RETURN_STRING(WLC_E_PRUNE_CIPHER_NA)
        CASE_RETURN_STRING(WLC_E_PRUNE_KNOWN_STA)
        CASE_RETURN_STRING(WLC_E_PRUNE_CCXFAST_DROAM)
        CASE_RETURN_STRING(WLC_E_PRUNE_WDS_PEER)
        CASE_RETURN_STRING(WLC_E_PRUNE_QBSS_LOAD)
        CASE_RETURN_STRING(WLC_E_PRUNE_HOME_AP)
        CASE_RETURN_STRING(WLC_E_PRUNE_AP_BLOCKED)
        CASE_RETURN_STRING(WLC_E_PRUNE_NO_DIAG_SUPPORT)
        CASE_RETURN_STRING(WLC_E_SUP_OTHER)
        CASE_RETURN_STRING(WLC_E_SUP_DECRYPT_KEY_DATA)
        CASE_RETURN_STRING(WLC_E_SUP_BAD_UCAST_WEP128)
        CASE_RETURN_STRING(WLC_E_SUP_BAD_UCAST_WEP40)
        CASE_RETURN_STRING(WLC_E_SUP_UNSUP_KEY_LEN)
        CASE_RETURN_STRING(WLC_E_SUP_PW_KEY_CIPHER)
        CASE_RETURN_STRING(WLC_E_SUP_MSG3_TOO_MANY_IE)
        CASE_RETURN_STRING(WLC_E_SUP_MSG3_IE_MISMATCH)
        CASE_RETURN_STRING(WLC_E_SUP_NO_INSTALL_FLAG)
        CASE_RETURN_STRING(WLC_E_SUP_MSG3_NO_GTK)
        CASE_RETURN_STRING(WLC_E_SUP_GRP_KEY_CIPHER)
        CASE_RETURN_STRING(WLC_E_SUP_GRP_MSG1_NO_GTK)
        CASE_RETURN_STRING(WLC_E_SUP_GTK_DECRYPT_FAIL)
        CASE_RETURN_STRING(WLC_E_SUP_SEND_FAIL)
        CASE_RETURN_STRING(WLC_E_SUP_DEAUTH)
        CASE_RETURN_STRING(WLC_E_SUP_WPA_PSK_TMO)
        CASE_RETURN_STRING(DOT11_RC_RESERVED)
        CASE_RETURN_STRING(DOT11_RC_UNSPECIFIED)
        CASE_RETURN_STRING(DOT11_RC_AUTH_INVAL)
        CASE_RETURN_STRING(DOT11_RC_DEAUTH_LEAVING)
        CASE_RETURN_STRING(DOT11_RC_INACTIVITY)
        CASE_RETURN_STRING(DOT11_RC_BUSY)
        CASE_RETURN_STRING(DOT11_RC_INVAL_CLASS_2)
        CASE_RETURN_STRING(DOT11_RC_INVAL_CLASS_3)
        CASE_RETURN_STRING(DOT11_RC_DISASSOC_LEAVING)
        CASE_RETURN_STRING(DOT11_RC_NOT_AUTH)
        CASE_RETURN_STRING(DOT11_RC_BAD_PC)
        CASE_RETURN_STRING(DOT11_RC_BAD_CHANNELS)
        CASE_RETURN_STRING(DOT11_RC_UNSPECIFIED_QOS)
        CASE_RETURN_STRING(DOT11_RC_INSUFFCIENT_BW)
        CASE_RETURN_STRING(DOT11_RC_EXCESSIVE_FRAMES)
        CASE_RETURN_STRING(DOT11_RC_TX_OUTSIDE_TXOP)
        CASE_RETURN_STRING(DOT11_RC_LEAVING_QBSS)
        CASE_RETURN_STRING(DOT11_RC_BAD_MECHANISM)
        CASE_RETURN_STRING(DOT11_RC_SETUP_NEEDED)
        CASE_RETURN_STRING(DOT11_RC_TIMEOUT)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_STATUS_CHG)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_MERGE)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_STOP)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_P2P)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_WINDOW_BEGIN_P2P)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_WINDOW_BEGIN_MESH)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_WINDOW_BEGIN_IBSS)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_WINDOW_BEGIN_RANGING)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_POST_DISC)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_DATA_IF_ADD)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_DATA_PEER_ADD)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_DATA_IND)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_DATA_CONF)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_SDF_RX)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_DATA_END)
        CASE_RETURN_STRING(WLC_E_NAN_EVENT_BCN_RX)
        case DOT11_RC_MAX:
        case WLC_E_REASON_FORCE_32_BIT:
        default:
            break;
    }

    return "Unknown";
}

char *whd_ether_ntoa(const uint8_t *ea, char *buf, uint8_t buf_len)
{
    const char hex[] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    char *output = buf;
    const uint8_t *octet = ea;

    if (buf_len < WHD_ETHER_ADDR_STR_LEN)
    {
        if (buf_len > 0)
        {
            /* buffer too short */
            buf[0] = '\0';
        }
        return buf;
    }

    for (; octet != &ea[WHD_ETHER_ADDR_LEN]; octet++)
    {
        *output++ = hex[(*octet >> 4) & 0xf];
        *output++ = hex[*octet & 0xf];
        *output++ = ':';
    }

    *(output - 1) = '\0';

    return buf;
}

#endif /* WPRINT_ENABLE_WHD_DEBUG */

void whd_convert_security_type_to_string(whd_security_t security, char *out_str, uint16_t out_str_len)
{
    if (security == WHD_SECURITY_OPEN)
    {
        strncat(out_str, " Open", out_str_len);
    }
    if (security & WEP_ENABLED)
    {
        strncat(out_str, " WEP", out_str_len);
    }
    if (security & WPA3_SECURITY)
    {
        strncat(out_str, " WPA3", out_str_len);
    }
    if (security & WPA2_SECURITY)
    {
        strncat(out_str, " WPA2", out_str_len);
    }
    if (security & WPA_SECURITY)
    {
        strncat(out_str, " WPA", out_str_len);
    }
    if (security & AES_ENABLED)
    {
        strncat(out_str, " AES", out_str_len);
    }
    if (security & TKIP_ENABLED)
    {
        strncat(out_str, " TKIP", out_str_len);
    }
    if (security & SHARED_ENABLED)
    {
        strncat(out_str, " SHARED", out_str_len);
    }
    if (security & ENTERPRISE_ENABLED)
    {
        strncat(out_str, " Enterprise", out_str_len);
    }
    if (security & WPS_ENABLED)
    {
        strncat(out_str, " WPS", out_str_len);
    }
    if (security & FBT_ENABLED)
    {
        strncat(out_str, " FBT", out_str_len);
    }
    if (security & IBSS_ENABLED)
    {
        strncat(out_str, " IBSS", out_str_len);
    }
    if (security == WHD_SECURITY_UNKNOWN)
    {
        strncat(out_str, " Unknown", out_str_len);
    }
    if (!(security & ENTERPRISE_ENABLED) && (security != WHD_SECURITY_OPEN) &&
        (security != WHD_SECURITY_UNKNOWN) )
    {
        strncat(out_str, " PSK", out_str_len);
    }
}

/*!
 ******************************************************************************
 * Prints partial details of a scan result on a single line
 *
 * @param[in] record  A pointer to the whd_scan_result_t record
 *
 */

void whd_print_scan_result(whd_scan_result_t *record)
{
    const char *str = NULL;
    char sec_type_string[40] = { 0 };

    switch (record->bss_type)
    {
        case WHD_BSS_TYPE_ADHOC:
            str = "Adhoc";
            break;

        case WHD_BSS_TYPE_INFRASTRUCTURE:
            str = "Infra";
            break;

        case WHD_BSS_TYPE_ANY:
            str = "Any";
            break;

        case WHD_BSS_TYPE_MESH:
        case WHD_BSS_TYPE_UNKNOWN:
            str = "Unknown";
            break;

        default:
            str = "?";
            break;
    }

    UNUSED_PARAMETER(str);
    WPRINT_MACRO( ("%5s ", str) );
    WPRINT_MACRO( ("%02X:%02X:%02X:%02X:%02X:%02X ", record->BSSID.octet[0], record->BSSID.octet[1],
                   record->BSSID.octet[2], record->BSSID.octet[3], record->BSSID.octet[4],
                   record->BSSID.octet[5]) );

    if (record->flags & WHD_SCAN_RESULT_FLAG_RSSI_OFF_CHANNEL)
    {
        WPRINT_MACRO( ("OFF ") );
    }
    else
    {
        WPRINT_MACRO( ("%d ", record->signal_strength) );
    }

    if (record->max_data_rate < 100000)
    {
        WPRINT_MACRO( (" %.1f ", (double)(record->max_data_rate / 1000.0) ) );
    }
    else
    {
        WPRINT_MACRO( ("%.1f ", (double)(record->max_data_rate / 1000.0) ) );
    }
    WPRINT_MACRO( (" %3d  ", record->channel) );

    whd_convert_security_type_to_string(record->security, sec_type_string, (sizeof(sec_type_string) - 1) );

    WPRINT_MACRO( ("%-20s ", sec_type_string) );
    WPRINT_MACRO( (" %-32s ", record->SSID.value) );

    if (record->ccode[0] != '\0')
    {
        WPRINT_MACRO( ("%c%c    ", record->ccode[0], record->ccode[1]) );
    }
    else
    {
        WPRINT_MACRO( ("      ") );
    }

    if (record->flags & WHD_SCAN_RESULT_FLAG_BEACON)
    {
        WPRINT_MACRO( (" %-15s", " BEACON") );
    }
    else
    {
        WPRINT_MACRO( (" %-15s", " PROBE ") );
    }
    WPRINT_MACRO( ("\n") );
}

void whd_hexdump(uint8_t *data, uint32_t data_len)
{
    uint32_t i;
    uint8_t buff[17] = {0};

    UNUSED_PARAMETER(buff);
    for (i = 0; i < data_len; i++)
    {
        if ( (i % 16) == 0 )
        {
            if (i != 0)
            {
                WPRINT_MACRO( ("  %s\n", buff) );
            }
            WPRINT_MACRO( ("%04" PRIx32 " ", i) );
        }
        WPRINT_MACRO( (" %02x", data[i]) );

        if ( (data[i] < 0x20) || (data[i] > 0x7e) )
        {
            buff[i % 16] = '.';
        }
        else
        {
            buff[i % 16] = data[i];
        }
        buff[(i % 16) + 1] = '\0';
    }
    while ( (i % 16) != 0 )
    {
        WPRINT_MACRO( ("   ") );
        i++;
    }
    WPRINT_MACRO( ("  %s\n", buff) );
}

void whd_ioctl_to_string(uint32_t cmd, char *ioctl_str, uint16_t ioctl_str_len)
{
    if (cmd == 2)
    {
        strncpy(ioctl_str, "WLC_UP", ioctl_str_len);
    }
    else if (cmd == 20)
    {
        strncpy(ioctl_str, "WLC_SET_INFRA", ioctl_str_len);
    }
    else if (cmd == 22)
    {
        strncpy(ioctl_str, "WLC_SET_AUTH", ioctl_str_len);
    }
    else if (cmd == 26)
    {
        strncpy(ioctl_str, "WLC_SET_SSID", ioctl_str_len);
    }
    else if (cmd == 52)
    {
        strncpy(ioctl_str, "WLC_DISASSOC", ioctl_str_len);
    }
    else if (cmd == 55)
    {
        strncpy(ioctl_str, "WLC_SET_ROAM_TRIGGER", ioctl_str_len);
    }
    else if (cmd == 57)
    {
        strncpy(ioctl_str, "WLC_SET_ROAM_DELTA", ioctl_str_len);
    }
    else if (cmd == 59)
    {
        strncpy(ioctl_str, "WLC_SET_ROAM_SCAN_PERIOD", ioctl_str_len);
    }
    else if (cmd == 110)
    {
        strncpy(ioctl_str, "WLC_SET_GMODE", ioctl_str_len);
    }
    else if (cmd == 116)
    {
        strncpy(ioctl_str, "WLC_SET_SCANSUPPRESS", ioctl_str_len);
    }
    else if (cmd == 134)
    {
        strncpy(ioctl_str, "WLC_SET_WSEC", ioctl_str_len);
    }
    else if (cmd == 165)
    {
        strncpy(ioctl_str, "WLC_SET_WPA_AUTH", ioctl_str_len);
    }
    else if (cmd == 268)
    {
        strncpy(ioctl_str, "WLC_SET_WSEC_PMK", ioctl_str_len);
    }
}

void whd_event_info_to_string(uint32_t cmd, uint16_t flag, uint32_t reason, char *ioctl_str, uint16_t ioctl_str_len)
{
    if (cmd == 0)
    {
        strncpy(ioctl_str, "WLC_E_SET_SSID", ioctl_str_len);
    }
    else if (cmd == 3)
    {
        strncpy(ioctl_str, "WLC_E_AUTH    ", ioctl_str_len);
    }
    else if (cmd == 16)
    {
        strncpy(ioctl_str, "WLC_E_LINK    ", ioctl_str_len);
    }
    else if (cmd == 46)
    {
        strncpy(ioctl_str, "WLC_E_PSK_SUP ", ioctl_str_len);
    }
    else if (cmd == 54)
    {
        strncpy(ioctl_str, "WLC_E_IF      ", ioctl_str_len);
    }
    else if (cmd == 69)
    {
        strncpy(ioctl_str, "WLC_E_ESCAN_RESULT", ioctl_str_len);
    }

    if (flag == 0)
    {
        strncat(ioctl_str, "  WLC_E_STATUS_SUCCESS",  ioctl_str_len);
    }
    if (flag == 8)
    {
        strncat(ioctl_str, "  WLC_E_STATUS_PARTIAL",  ioctl_str_len);
    }
    else if (flag == 262)
    {
        strncat(ioctl_str, "  WLC_SUP_KEYED       ",  ioctl_str_len);
    }

    if (reason == 0)
    {
        strncat(ioctl_str, "    WLC_E_REASON_INITIAL_ASSOC",  ioctl_str_len);
    }
    else if (reason == 512)
    {
        strncat(ioctl_str, "    WLC_E_SUP_OTHER",  ioctl_str_len);
    }
}

