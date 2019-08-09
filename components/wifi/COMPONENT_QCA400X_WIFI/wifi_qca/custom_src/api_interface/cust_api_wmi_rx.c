//------------------------------------------------------------------------------
// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// · Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// · Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// · Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <driver_cxt.h>
#include <wmi_host.h>

//TODO: mixin different layers, need to fix
//      introduced by on_connect events enum
//      need to specify connections between layers
#include "qcom_api.h"

#if ENABLE_P2P_MODE
#include "p2p.h"
#include "wmi.h"
#include "wmi_api.h"
#include "wmi_host.h"
#endif

#include <atheros_wifi_api.h>
#include <atheros_wifi_internal.h>
#if ENABLE_P2P_MODE
extern const WMI_SCAN_PARAMS_CMD default_scan_param;
extern const uint8_t max_performance_power_param;
#endif
static void fill_scan_info(void *pCxt, QCA_SCAN_INFO *pScanInfo, WMI_BSS_INFO_HDR *bih, int32_t len)
{
    A_SCAN_SUMMARY scan_summary;

    if (A_OK == Api_ParseInfoElem(pCxt, bih, len, &scan_summary))
    {
        pScanInfo->channel = scan_summary.channel;
        pScanInfo->rssi = scan_summary.rssi;
        A_MEMCPY(pScanInfo->bssid, scan_summary.bssid, ATH_MAC_LEN);
        pScanInfo->ssid_len = scan_summary.ssid_len;

        A_MEMCPY(pScanInfo->ssid, &(scan_summary.ssid[0]), scan_summary.ssid_len);
        pScanInfo->security_enabled = (uint8_t)((scan_summary.caps & IEEE80211_CAPINFO_PRIVACY) ? 1 : 0);
        pScanInfo->preamble = (uint8_t)((scan_summary.caps & IEEE80211_CAPINFO_SHORT_PREAMBLE) ? 1 : 0);

        if ((scan_summary.caps & (IEEE80211_CAPINFO_ESS | IEEE80211_CAPINFO_IBSS)) == IEEE80211_CAPINFO_ESS)
        {
            pScanInfo->bss_type = QCA_MEDIACTL_MODE_INFRA;
        }
        else if ((scan_summary.caps & (IEEE80211_CAPINFO_ESS | IEEE80211_CAPINFO_IBSS)) == IEEE80211_CAPINFO_IBSS)
        {
            pScanInfo->bss_type = QCA_MEDIACTL_MODE_ADHOC;
        }
        else
        {
            /* error condition report it as ad-hoc in this case*/
            pScanInfo->bss_type = QCA_MEDIACTL_MODE_ADHOC;
        }

        pScanInfo->beacon_period = scan_summary.beacon_period;

        if (scan_summary.rsn_cipher & TKIP_CRYPT)
        {
            pScanInfo->rsn_cipher |= ATH_CIPHER_TYPE_TKIP;
        }

        if (scan_summary.rsn_cipher & AES_CRYPT)
        {
            pScanInfo->rsn_cipher |= ATH_CIPHER_TYPE_CCMP;
        }

        if (scan_summary.rsn_cipher & WEP_CRYPT)
        {
            pScanInfo->rsn_cipher |= ATH_CIPHER_TYPE_WEP;
        }

        if (scan_summary.wpa_cipher & TKIP_CRYPT)
        {
            pScanInfo->wpa_cipher |= ATH_CIPHER_TYPE_TKIP;
        }

        if (scan_summary.wpa_cipher & AES_CRYPT)
        {
            pScanInfo->wpa_cipher |= ATH_CIPHER_TYPE_CCMP;
        }

        if (scan_summary.wpa_cipher & WEP_CRYPT)
        {
            pScanInfo->wpa_cipher |= ATH_CIPHER_TYPE_WEP;
        }

        if (scan_summary.rsn_auth & WPA2_AUTH)
        {
            pScanInfo->rsn_auth |= SECURITY_AUTH_1X;
        }

        if (scan_summary.rsn_auth & WPA2_PSK_AUTH)
        {
            pScanInfo->rsn_auth |= SECURITY_AUTH_PSK;
        }

        if (scan_summary.wpa_auth & WPA_AUTH)
        {
            pScanInfo->wpa_auth |= SECURITY_AUTH_1X;
        }

        if (scan_summary.wpa_auth & WPA_PSK_AUTH)
        {
            pScanInfo->wpa_auth |= SECURITY_AUTH_PSK;
        }
    }
}

static void fill_ext_scan_info(void *pCxt, ATH_SCAN_EXT *pExtScanInfo, WMI_BSS_INFO_HDR *bih, int32_t len)
{
    A_SCAN_SUMMARY scan_summary;

    if (A_OK == Api_ParseInfoElem(pCxt, bih, len, &scan_summary))
    {
        A_MEMZERO(pExtScanInfo, sizeof(ATH_SCAN_EXT));
        pExtScanInfo->channel = scan_summary.channel;
        pExtScanInfo->rssi = scan_summary.rssi;
        A_MEMCPY(pExtScanInfo->bssid, scan_summary.bssid, ATH_MAC_LEN);
        pExtScanInfo->ssid_len = scan_summary.ssid_len;

        A_MEMCPY(pExtScanInfo->ssid, &(scan_summary.ssid[0]), scan_summary.ssid_len);
        pExtScanInfo->security_enabled = (uint8_t)((scan_summary.caps & IEEE80211_CAPINFO_PRIVACY) ? 1 : 0);
        pExtScanInfo->preamble = (uint8_t)((scan_summary.caps & IEEE80211_CAPINFO_SHORT_PREAMBLE) ? 1 : 0);

        if ((scan_summary.caps & (IEEE80211_CAPINFO_ESS | IEEE80211_CAPINFO_IBSS)) == IEEE80211_CAPINFO_ESS)
        {
            pExtScanInfo->bss_type = QCA_MEDIACTL_MODE_INFRA;
        }
        else if ((scan_summary.caps & (IEEE80211_CAPINFO_ESS | IEEE80211_CAPINFO_IBSS)) == IEEE80211_CAPINFO_IBSS)
        {
            pExtScanInfo->bss_type = QCA_MEDIACTL_MODE_ADHOC;
        }
        else
        {
            /* error condition report it as ad-hoc in this case*/
            pExtScanInfo->bss_type = QCA_MEDIACTL_MODE_ADHOC;
        }

        pExtScanInfo->beacon_period = scan_summary.beacon_period;

        if (scan_summary.rsn_cipher & TKIP_CRYPT)
        {
            pExtScanInfo->rsn_cipher |= ATH_CIPHER_TYPE_TKIP;
        }

        if (scan_summary.rsn_cipher & AES_CRYPT)
        {
            pExtScanInfo->rsn_cipher |= ATH_CIPHER_TYPE_CCMP;
        }

        if (scan_summary.rsn_cipher & WEP_CRYPT)
        {
            pExtScanInfo->rsn_cipher |= ATH_CIPHER_TYPE_WEP;
        }

        if (scan_summary.wpa_cipher & TKIP_CRYPT)
        {
            pExtScanInfo->wpa_cipher |= ATH_CIPHER_TYPE_TKIP;
        }

        if (scan_summary.wpa_cipher & AES_CRYPT)
        {
            pExtScanInfo->wpa_cipher |= ATH_CIPHER_TYPE_CCMP;
        }

        if (scan_summary.wpa_cipher & WEP_CRYPT)
        {
            pExtScanInfo->wpa_cipher |= ATH_CIPHER_TYPE_WEP;
        }

        if (scan_summary.rsn_auth & WPA2_AUTH)
        {
            pExtScanInfo->rsn_auth |= SECURITY_AUTH_1X;
        }

        if (scan_summary.rsn_auth & WPA2_PSK_AUTH)
        {
            pExtScanInfo->rsn_auth |= SECURITY_AUTH_PSK;
        }

        if (scan_summary.wpa_auth & WPA_AUTH)
        {
            pExtScanInfo->wpa_auth |= SECURITY_AUTH_1X;
        }

        if (scan_summary.wpa_auth & WPA_PSK_AUTH)
        {
            pExtScanInfo->wpa_auth |= SECURITY_AUTH_PSK;
        }
    }
}

void Custom_Api_BssInfoEvent(void *pCxt, uint8_t *datap, int32_t len)
{
    WMI_BSS_INFO_HDR *bih = (WMI_BSS_INFO_HDR *)datap;
    QCA_SCAN_INFO_PTR pScanInfo;
    ATH_SCAN_EXT *pExtScanInfo;
    uint16_t i;
    uint8_t worst_snr_idx = 0;
    uint8_t worst_snr = 0xff;
    uint16_t scanCount;

    /* add/replace entry in application scan results */
    if (GET_DRIVER_COMMON(pCxt)->extended_scan)
    {
        pExtScanInfo = (ATH_SCAN_EXT *)(GET_DRIVER_COMMON(pCxt)->pScanOut);
        scanCount = GET_DRIVER_COMMON(pCxt)->scanOutCount;
        // look for previous match
        for (i = 0; i < scanCount; i++)
        {
            if (0 == A_MEMCMP(bih->bssid, pExtScanInfo[i].bssid, ATH_MAC_LEN))
            {
                fill_ext_scan_info(pCxt, &pExtScanInfo[i], bih, len);
                break;
            }
            /* keep worst rssi entry for optional use below */
            if (worst_snr > pExtScanInfo[i].rssi)
            {
                worst_snr = pExtScanInfo[i].rssi;
                worst_snr_idx = i;
            }
        }

        if (i >= scanCount)
        {
            if (GET_DRIVER_COMMON(pCxt)->scanOutSize <= scanCount)
            {
                /* replace other non-matching entry based on rssi */
                if (bih->snr > worst_snr)
                {
                    fill_ext_scan_info(pCxt, &pExtScanInfo[worst_snr_idx], bih, len);
                }
            }
            else
            {
                /* populate new entry */
                fill_ext_scan_info(pCxt, &pExtScanInfo[scanCount], bih, len);
                scanCount++;
                GET_DRIVER_COMMON(pCxt)->scanOutCount = scanCount;
            }
        }
    }
    else
    {
        pScanInfo = (QCA_SCAN_INFO_PTR)(GET_DRIVER_COMMON(pCxt)->pScanOut);
        scanCount = GET_DRIVER_COMMON(pCxt)->scanOutCount;
        // look for previous match
        for (i = 0; i < scanCount; i++)
        {
            if (0 == A_MEMCMP(bih->bssid, pScanInfo[i].bssid, ATH_MAC_LEN))
            {
                fill_scan_info(pCxt, &pScanInfo[i], bih, len);
                break;
            }
            /* keep worst rssi entry for optional use below */
            if (worst_snr > pScanInfo[i].rssi)
            {
                worst_snr = pScanInfo[i].rssi;
                worst_snr_idx = i;
            }
        }

        if (i >= scanCount)
        {
            if (GET_DRIVER_COMMON(pCxt)->scanOutSize <= scanCount)
            {
                /* replace other non-matching entry based on rssi */
                if (bih->snr > worst_snr)
                {
                    fill_scan_info(pCxt, &pScanInfo[worst_snr_idx], bih, len);
                }
            }
            else
            {
                /* populate new entry */
                fill_scan_info(pCxt, &pScanInfo[scanCount], bih, len);
                scanCount++;
                GET_DRIVER_COMMON(pCxt)->scanOutCount = scanCount;
            }
        }
    }
}

void Custom_Api_ConnectEvent(void *pCxt, uint8_t devId, uint16_t channel, uint8_t *bssid)
{
    ATH_CONNECT_CB cb = NULL;
    boolean bssConn = false;

    /* Update connect & link status atomically */
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        if (GET_DRIVER_CXT(pCxt)->connectStateCB != NULL)
        {
            cb = (ATH_CONNECT_CB)GET_DRIVER_CXT(pCxt)->connectStateCB;
            /* call this later from outside the spinlock */
        }
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
    QCA_CONTEXT_STRUCT_PTR qca_ptr = (QCA_CONTEXT_STRUCT_PTR)GET_DRIVER_CXT(pCxt)->pUpperCxt[devId];
    bssConn = !A_MEMCMP(qca_ptr->ADDRESS, bssid, ATH_MAC_LEN);

    /* call the callback function provided by application to
     * indicate connection state */
    if (cb != NULL)
    {
        cb(QCOM_ONCONNECT_EVENT_SUCCESS, devId, bssid, bssConn);
    }
}

void Custom_Api_DisconnectEvent(void *pCxt,
                                uint8_t devId,
                                uint8_t reason,
                                uint8_t *bssid,
                                uint8_t assocRespLen,
                                uint8_t *assocInfo,
                                uint16_t protocolReasonStatus)
{
    ATH_CONNECT_CB cb = NULL;
    boolean bssConn = false;
    uint8_t bssMac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    /* Update connect & link status atomically */
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    if (GET_DRIVER_CXT(pCxt)->connectStateCB != NULL)
    {
        cb = (ATH_CONNECT_CB)GET_DRIVER_CXT(pCxt)->connectStateCB;
        /* call this later from outside the spinlock */
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);
    /* call the callback function provided by application to
        * indicate connection state */
    if (cb != NULL)
    {
        QCA_CONTEXT_STRUCT_PTR qca_ptr = (QCA_CONTEXT_STRUCT_PTR)GET_DRIVER_CXT(pCxt)->pUpperCxt[devId];
        bssConn = !A_MEMCMP(qca_ptr->ADDRESS, bssid, ATH_MAC_LEN);
        bssConn = !A_MEMCMP(bssMac, bssid, ATH_MAC_LEN);

        if (reason == INVALID_PROFILE)
        {
            cb(INVALID_PROFILE, devId, bssid, bssConn);
        }
        else
        {
            cb(QCOM_ONCONNECT_EVENT_DISCONNECT, devId, bssid, bssConn);
        }
    }
}

void Custom_Api_ReadyEvent(void *pCxt, uint8_t *datap, uint8_t phyCap, uint32_t sw_ver, uint32_t abi_ver)
{
    QCA_CONTEXT_STRUCT_PTR qca_ptr = (QCA_CONTEXT_STRUCT_PTR)GET_DRIVER_CXT(pCxt)->pUpperCxt[0];
    /* this custom implementation sets an event after setting CXT_WMI_READY
     * so as to allow the blocked user thread to wake up. */
    A_MEMCPY(qca_ptr->ADDRESS, datap, ATH_MAC_LEN);
    DRIVER_WAKE_USER(pCxt);
    UNUSED_ARGUMENT(phyCap);
    UNUSED_ARGUMENT(sw_ver);
    UNUSED_ARGUMENT(abi_ver);
}

void Custom_Api_GpioDataEvent(void *wmip, uint8_t *datap, int32_t len)
{
    if (ath_custom_init.Api_GpioDataEventRx != NULL)
    {
        ath_custom_init.Api_GpioDataEventRx(datap, len);
    }
}

void Custom_Api_PfmDataEvent(void *wmip, uint8_t *datap, int32_t len)
{
    if (ath_custom_init.Custom_Api_PfmDataEventRx != NULL)
    {
        ath_custom_init.Custom_Api_PfmDataEventRx(datap, len);
    }
}

void Custom_Api_PfmDataDoneEvent(void *wmip, uint8_t *datap, int32_t len)
{
    if (ath_custom_init.Custom_Api_PfmDataDoneEventRx != NULL)
    {
        ath_custom_init.Custom_Api_PfmDataDoneEventRx(((struct wmi_t *)wmip)->wmi_devt, datap, len);
    }
}

void Custom_Api_RSNASuccessEvent(void *pCxt, uint8_t devId, uint8_t code)
{
    ATH_CONNECT_CB cb = NULL;
    /* this is the event that the customer has to use to send a callback
     * to the application so that it will print the success event */

    /* get the callback handler from the device context */
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    if (GET_DRIVER_CXT(pCxt)->connectStateCB != NULL)
    {
        cb = (ATH_CONNECT_CB)GET_DRIVER_CXT(pCxt)->connectStateCB;
        /* call this later from outside the spinlock */
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);
    /* call the callback function provided by application to
        * indicate 4 way handshake status */
    if (cb != NULL)
    {
        cb(code, devId, NULL, false);
    }
}

void Custom_Api_BitRateEvent_tx(void *pCxt, uint8_t devId, int8_t rateIndex)
{
    ATH_CONNECT_CB cb = NULL;
    /* the driver will get the index of the last tx rate from chip
     * based on this index we get the rate tx from the array */

    /* get the callback handler from the device context */
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    if (GET_DRIVER_CXT(pCxt)->connectStateCB != NULL)
    {
        cb = (ATH_CONNECT_CB)GET_DRIVER_CXT(pCxt)->connectStateCB;
        /* call this later from outside the spinlock */
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);
    /* call the callback function provided by application to
        * indicate last transmitted rate */

    if (cb != NULL)
    {
        cb(wmi_rateTable[rateIndex][0], devId, NULL, false);
    }
}

void Custom_Api_ProbeReq_Event(void *pCxt, uint8_t *datap, uint32_t len)
{
    ATH_PROBEREQ_CB cb = NULL;
    WMI_P2P_RX_PROBE_REQ_EVENT_STRUCT *buf = (WMI_P2P_RX_PROBE_REQ_EVENT_STRUCT *)datap;

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        if (GET_DRIVER_CXT(pCxt)->probeReqCB != NULL)
        {
            cb = (ATH_PROBEREQ_CB)GET_DRIVER_CXT(pCxt)->probeReqCB;
            /* call this later from outside the spinlock */
        }
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    /* call the callback function provided by application to
     * indicate Probe request message to the application */
    if (cb != NULL)
    {
        cb(buf->data, buf->len, buf->freq);
    }
}

/* EOF */
