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
#include "wifi_common.h"

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <a_drv_api.h>
#include <wmi_api.h>
#include <aggr_recv_api.h>
#include <wmi.h>
#include <atheros_wifi_api.h>      //IGX_UD_CHANGES
#include <atheros_wifi_internal.h> //IGX_UD_CHANGES
#include <wlan_api.h>
#include <common_stack_offload.h>

#if ENABLE_P2P_MODE
#include "p2p.h"
#include "wmi.h"
#include "wmi_api.h"
#include "wmi_host.h"
// WPS_CREDENTIAL persistCredential;
// WMI_WPS_PROFILE_EVENT wpsProfEv;
#endif

#include "dset.h"
#include "dset_api.h"

extern const uint8_t max_performance_power_param;
extern const WMI_SCAN_PARAMS_CMD default_scan_param;

/*****************************************************************************/
/*  Api_TargetStatsEvent - Processes WMI_REPORT_STATISTICS_EVENTID
 *	 from the device.
 * 		void *pCxt - the driver context.
 *		uint8_t *ptr - pointer to the stats buffer.
 *		uint32_t len - length of the ptr buffer.
 *****************************************************************************/
void Api_TargetStatsEvent(void *pCxt, uint8_t devId, uint8_t *ptr, uint32_t len)
{
    UNUSED_ARGUMENT(devId);
    if (ptr != NULL && len != 0)
    {
        GET_DRIVER_COMMON(pCxt)->rssi = ((WMI_TARGET_STATS *)ptr)->cservStats.cs_snr;
    }
    GET_DRIVER_COMMON(pCxt)->rssiFlag = false;
    API_TARGET_STATS_EVENT(pCxt, ptr, len);
}

/*****************************************************************************/
/*  Api_RegDomainEvent - Processes WMI_REGDOMAIN_EVENTID from the device.
 * 		void *pCxt - the driver context.
 *		uint32_t regCode - the encoded regulatory domain value.
 *****************************************************************************/
void Api_RegDomainEvent(void *pCxt, uint8_t devId, uint32_t regCode)
{
    UNUSED_ARGUMENT(devId);
    // A_PRINTF("AR4XXX Reg Code = 0x%x\n", regCode);
    GET_DRIVER_COMMON(pCxt)->regCode = regCode;
    API_REGDOMAIN_EVENT(pCxt, regCode);
}

/*****************************************************************************/
/*  Api_BitrateEvent - Processes WMI_BIT_RATE_REPLY from the device.
 * 		void *pCxt - the driver context.
 *		int32_t rateKbps - the rate in Kbps used for recent transmission.
 *****************************************************************************/
void Api_BitrateEvent(void *pCxt, uint8_t devId, int32_t rateKbps)
{
    UNUSED_ARGUMENT(devId);
    GET_DRIVER_COMMON(pCxt)->bitRate = rateKbps;
    API_BITRATE_EVENT(pCxt, rateKbps);
}

/*****************************************************************************/
/*  Api_GetBitRateEvent - Processes WMI_GET_BITRATE_CMDID from the device.
 * 		void *pCxt - the driver context.
 *		uint8_t *datap - the rate index for recent transmission is in this pointer.
 *****************************************************************************/
void Api_GetBitRateEvent(void *pCxt, uint8_t devId, void *datap)
{
    UNUSED_ARGUMENT(devId);
    CUSTOM_API_BITRATE_EVENT_TX(pCxt, devId, ((WMI_BIT_RATE_REPLY *)datap)->rateIndex);
}

/*****************************************************************************/
/*  Api_ChannelListEvent - Processes a WMI_GET_CHANNEL_LIST_CMD from the device.
 *	 This event will come when the host issues a WMI_GET_CHANNEL_LIST_CMD to
 *	 the device to request a current channel list.
 * 		void *pCxt - the driver context.
 *		int8_t numChan - number of channels in the list.
 *		uint16_t *chanList - array of numChan channels.
 *****************************************************************************/
void Api_ChannelListEvent(void *pCxt, uint8_t devId, int8_t numChan, void *chanList)
{
    uint16_t i;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    UNUSED_ARGUMENT(devId);
    A_MEMCPY(pDCxt->channelList, chanList, numChan * sizeof(uint16_t));
    pDCxt->numChannels = numChan;
    /* convert each channel to proper endianness */
    for (i = 0; i < pDCxt->numChannels; i++)
    {
        pDCxt->channelList[i] = A_LE2CPU16(pDCxt->channelList[i]);
    }

    API_CHANNEL_LIST_EVENT(pCxt, pDCxt->numChannels, pDCxt->channelList);
}

/*****************************************************************************/
/*  Api_ScanCompleteEvent - Processes a WMI_SCAN_COMPLETE_EVENTID from the device.
 *	 These events will occur if the host has previously initiated a scan request.
 *	 This event indicates the final conclusion of the scan operation.
 * 		void *pCxt - the driver context.
 *		A_STATUS status - indicates the status of the scan operation.
 *****************************************************************************/
void Api_ScanCompleteEvent(void *pCxt, uint8_t devId, A_STATUS status)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    UNUSED_ARGUMENT(status);
    UNUSED_ARGUMENT(devId);

    wmi_bssfilter_cmd(pDCxt->pWmiCxt, NONE_BSS_FILTER, 0);
    /* host initiated scan */
    pDCxt->scanDone = true;

    Api_BootProfile(pCxt, BOOT_PROFILE_DONE_SCAN);

    API_SCAN_COMPLETE_EVENT(pCxt, status);
}

/*****************************************************************************/
/*  Api_BssInfoEvent - Processes a WMI_BSS_INFO_EVENT from the device. These
 *	 events will occur if the host has previously initiated a scan request
 * 		void *pCxt - the driver context.
 *		uint8_t *datap - the original event buffer.
 *		int32_t len - the length of datap.
 *****************************************************************************/
void Api_BssInfoEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len)
{
    UNUSED_ARGUMENT(devId);
    CUSTOM_API_BSS_INFO_EVENT(pCxt, datap, len);
}

/*****************************************************************************/
/*  Api_TkipMicErrorEvent - Processes a TKIP error event from the device. This
 *	 event can happen when the device is using TKIP security and it detects a
 *	 decryption error.
 * 		void *pCxt - the driver context.
 *		uint8_t keyid - the Key ID used when the error occurred.
 *		boolean ismcast - 1 if packet was multi-cast 0 otherwise.
 *****************************************************************************/
void Api_TkipMicErrorEvent(void *pCxt, uint8_t devId, uint8_t keyid, boolean ismcast)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    UNUSED_ARGUMENT(ismcast);
    UNUSED_ARGUMENT(keyid);
    UNUSED_ARGUMENT(devId);
    pDCxt->tkipCountermeasures = 1;
    POWER_MODE pwrmode;
    pwrmode.pwr_mode = MAX_PERF_POWER;
    pwrmode.pwr_module = TKIP;

    Api_SetPowerMode(pCxt, &pwrmode);

    /* receiving this event informs the host that the wifi device
     * encountered 2 TKIP MIC errors within the restricted time span.
     * consequently, to fullfill the TKIP countermeasure requirement the
     * host must disconnect from the network and remain disconnected for
     * the specified period.
     */
    wmi_cmd_start(pDCxt->pWmiCxt, NULL, WMI_DISCONNECT_CMDID, 0);
    /* can't use A_MDELAY here as the wmi_disconnect_cmd() will
     * cause a disconnect_event which should get processed. */
    // A_MDELAY(IEEE80211_TKIP_COUNTERMEASURES_TIMEOUT_MSEC);
    API_TKIP_MIC_ERROR_EVENT(pCxt, keyid, ismcast);
}

/*****************************************************************************/
/*  Api_GetPmkEvent - Processes a WMI_GET_PMK_REPLY from the
 *	 device.
 *	 This event will come when the caller has previously sent a WMI_GET_PMC_CMD.
 *	 The function stores the result in wpaPmk for asynchronous access.
 * 		void *pCxt - the driver context.
 *		uint8_t *datap - original event buffer.
 *****************************************************************************/
void Api_GetPmkEvent(void *pCxt, uint8_t devId, uint8_t *datap)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    WMI_GET_PMK_REPLY *pReply = (WMI_GET_PMK_REPLY *)datap;

    A_MEMCPY(pDCxt->conn[devId].wpaPmk, pReply->pmk, WMI_PMK_LEN);
    pDCxt->conn[devId].wpaPmkValid = true;
    API_GET_PMK_EVENT(pCxt, datap);
}

/*****************************************************************************/
/*  Api_ConnectEvent - Processes a WMI_CONNECT_EVENT from the
 *	 device.
 *	 This event will come when the device has connected to a BSS
 *	 network.
 * 		void *pCxt - the driver context.
 *		uint16_t channel - RF channel on which connection exists.
 *		uint8_t *bssid - bssid address of the network.
 *		uint16_t listenInterval - assigned listen interval for sleep mode.
 *		uint16_t beaconInterval - BSS beacon interval.
 *		NETWORK_TYPE networkType - identifies the type of network.
 * 		uint8_t beaconIeLen - length of beacon Information element.
 *		uint8_t assocReqLen - length of assocation request frame.
 *		uint8_t assocRespLen - length of assocation response length.
 *		uint8_t *assocInfo - pointer to assocation frame(s).
 *****************************************************************************/
void Api_ConnectEvent(void *pCxt,
                      uint8_t devId,
                      uint16_t channel,
                      uint8_t *bssid,
                      uint16_t listenInterval,
                      uint16_t beaconInterval,
                      NETWORK_TYPE networkType,
                      uint8_t beaconIeLen,
                      uint8_t assocReqLen,
                      uint8_t assocRespLen,
                      uint8_t *assocInfo)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    A_WEPKEY_T *pKey;
    WMI_ADD_CIPHER_KEY_CMD add_key_param;

    UNUSED_ARGUMENT(listenInterval);
    UNUSED_ARGUMENT(beaconInterval);
    UNUSED_ARGUMENT(beaconIeLen);
    UNUSED_ARGUMENT(assocReqLen);
    UNUSED_ARGUMENT(assocRespLen);
    UNUSED_ARGUMENT(assocInfo);

    /* In SoftAP mode, we will get connect event first time when we start as AP and then whenever a station connects to
     * us.
     * We should not set the channel and bssid, when the later connect event comes.
     * The later connect event has network type as 0.
     */
    if (networkType != 0)
    {
        A_MEMCPY(pDCxt->conn[devId].bssid, bssid, ATH_MAC_LEN);
        pDCxt->conn[devId].bssChannel = channel;
    }

    if ((networkType & ADHOC_NETWORK) && (OPEN_AUTH == pDCxt->conn[devId].dot11AuthMode) &&
        (NONE_AUTH == pDCxt->conn[devId].wpaAuthMode) && (WEP_CRYPT == pDCxt->conn[devId].wpaPairwiseCrypto))
    {
        /* if we are connecting for the first time to an ad-hoc network with
         * WEP. then submit the key via wmi_addKey_cmd */
        if (false == pDCxt->conn[devId].isConnected)
        {
            pKey = &(pDCxt->conn[devId].wepKeyList[pDCxt->conn[devId].wepDefTxKeyIndex]);
            A_MEMZERO(&add_key_param, sizeof(add_key_param));
            add_key_param.keyIndex = pDCxt->conn[devId].wepDefTxKeyIndex;
            add_key_param.keyType = WEP_CRYPT;
            add_key_param.keyUsage = GROUP_USAGE | TX_USAGE;
            add_key_param.keyLength = pKey->keyLen;
            A_MEMCPY(&add_key_param.key, pKey->key, add_key_param.keyLength);
            add_key_param.key_op_ctrl = KEY_OP_INIT_VAL;
            wmi_cmd_start(pDCxt->pWmiCxt, &add_key_param, WMI_ADD_CIPHER_KEY_CMDID, sizeof(WMI_ADD_CIPHER_KEY_CMD));
        }
    }

    /* Update connect & link status atomically */
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        pDCxt->conn[devId].isConnected = true;
        pDCxt->conn[devId].isConnectPending = false;
        if ((channel > 2000) && (channel < 6500))
        {
            pDCxt->conn[devId].channelHint = channel;
        }
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    Api_BootProfile(pCxt, BOOT_PROFILE_DONE_CONNECT);

    pDCxt->rssi = 0;

/* reset the rx aggr state */

#if WLAN_CONFIG_11N_AGGR_SUPPORT
    aggr_reset_state(pDCxt->pAggrCxt);
#endif
    if (pDCxt->wps_in_progress && pDCxt->devId == devId)
    {
        pDCxt->wps_in_progress = 0;
    }

    CUSTOM_API_CONNECT_EVENT(pCxt, devId, channel, bssid, listenInterval, beaconInterval, networkType, beaconIeLen,
                             assocReqLen, assocRespLen, assocInfo);
}

/*****************************************************************************/
/*  Api_DisconnectEvent - Processes a WMI_DISCONNECT_EVENT from the
 *	 device.
 *	 This event will come when the device has disconnected from the BSS
 *	 network.
 * 		void *pCxt - the driver context.
 *		uint8_t reason - encoded reason for the disconnection.
 *		uint8_t *bssid - bssid address of the network.
 *		uint8_t assocRespLen - association response frame length.
 *		uint8_t *assocInfo - association response frame payload.
 *		uint16_t protocolReasonStatus - reason field from assocation response
 *		 frame.
 *****************************************************************************/
void Api_DisconnectEvent(void *pCxt,
                         uint8_t devId,
                         uint8_t reason,
                         uint8_t *bssid,
                         uint8_t assocRespLen,
                         uint8_t *assocInfo,
                         uint16_t protocolReasonStatus)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    POWER_MODE pwrmode;

    UNUSED_ARGUMENT(bssid);
    UNUSED_ARGUMENT(assocRespLen);
    UNUSED_ARGUMENT(assocInfo);
    UNUSED_ARGUMENT(protocolReasonStatus);
/* it is necessary to clear the host-side rx aggregation state */
#if WLAN_CONFIG_11N_AGGR_SUPPORT
    aggr_reset_state(pDCxt->pAggrCxt);
#endif

    /*
     * If the event is due to disconnect cmd from the host, only they the target
     * would stop trying to connect. Under any other condition, target would
     * keep trying to connect.
     *
     */
    if (reason == DISCONNECT_CMD)
    {
        pDCxt->conn[devId].isConnectPending = false;
    }
    else
    {
        pDCxt->conn[devId].isConnectPending = true;
    }

    /* Update connect & link status atomically */
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    pDCxt->conn[devId].isConnected = false;
    pDCxt->conn[devId].channelHint = 0;
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);
    pDCxt->rssi = 0;
    A_MEMZERO(pDCxt->conn[devId].bssid, ATH_MAC_LEN);
    pDCxt->conn[devId].bssChannel = 0;

    if (pDCxt->wps_in_progress && pDCxt->devId == devId)
    {
        pDCxt->wps_in_progress = 0;
    }
    CUSTOM_API_DISCONNECT_EVENT(pCxt, devId, reason, bssid, assocRespLen, assocInfo, protocolReasonStatus);
    /* complete TKIP countermeasures operation. TKIP countermeasures
     * requires that the STA avoid any contact with the AP for 60 seconds.
     * this implementation simply blocks for that time via A_MDELAY().
     * Other implementations may choose to behave differently. */
    if (pDCxt->tkipCountermeasures != 0)
    {
        /* FIXME: should instead implement a way to block the driver at driver_main thus
       * allowing the thread to continue running in single-threaded systems.*/
        A_MDELAY(IEEE80211_TKIP_COUNTERMEASURES_TIMEOUT_MSEC);
        pDCxt->tkipCountermeasures = 0;
        Api_ConnectWiFi(pCxt);
        A_MDELAY(200);

        pwrmode.pwr_mode = REC_POWER;
        pwrmode.pwr_module = TKIP;
        Api_SetPowerMode(pCxt, &pwrmode);
    }
}

int dset_length;

void Api_HostDsetStoreEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint8_t *strrclData = pDCxt->tempStorage;
    uint16_t cur_len = pDCxt->strrclCxtLen;

    WMI_HOST_DSET_STORE_EVENT *pDsetEvent = (WMI_HOST_DSET_STORE_EVENT *)datap;
    UNUSED_ARGUMENT(devId);

    dset_length = pDsetEvent->length;

    strrclData = pDCxt->tempStorage + cur_len;

    if (cur_len + dset_length <= pDCxt->tempStorageLength)
    {
        A_MEMCPY(strrclData, pDsetEvent->data, dset_length);
    }
    else
    {
        A_ASSERT(0);
    }
    pDCxt->strrclCxtLen += dset_length;
}

#if 0
A_STATUS
wmi_storerecall_recall_cmd(struct wmi_t *wmip, uint32_t length, void* pData)
{
    void *osbuf;
    WMI_STORERECALL_RECALL_CMD *p;
    uint32_t msgLength = sizeof(WMI_STORERECALL_RECALL_CMD) - sizeof(uint8_t);

    osbuf = A_NETBUF_ALLOC((int32_t)msgLength);
    if (osbuf == NULL) {
        return A_NO_MEMORY;
    }

    A_NETBUF_PUT(osbuf, (int32_t)msgLength);

    p = (WMI_STORERECALL_RECALL_CMD *)(A_NETBUF_DATA(osbuf));

    p->length = A_CPU2LE32(length);
    //A_MEMCPY(&p->data[0], pData, length);
    /* here we append the payload to the msg rather than copy it. this is a
     * memory and CPU optimization as the payload can be very large.
     */
    A_NETBUF_APPEND_FRAGMENT(osbuf, pData, length);

    return (wmi_cmd_send(wmip, osbuf, WMI_STORERECALL_RECALL_CMDID, NO_SYNC_WMIFLAG));
}
#endif

void Api_HostDsetCreateEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    void *osbuf;
    uint32_t msgLength;
    WMI_HOST_DSET_RESP_CREATE_CMD *p;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    HOST_DSET *pHost;

    UNUSED_ARGUMENT(devId);

    WMI_HOST_DSET_CREATE_EVENT *pDsetEvent = (WMI_HOST_DSET_CREATE_EVENT *)datap;
    pHost = dset_get(pDsetEvent->dset_id, pDsetEvent->length);
    QCADRV_PRINTF("dset create  id=%d  length=%d\n", pDsetEvent->dset_id, pDsetEvent->length);

    /*
     *  respose KF dset create request
     */
    msgLength = sizeof(WMI_HOST_DSET_RESP_CREATE_CMD);
    osbuf = A_NETBUF_ALLOC((int32_t)msgLength);

    //NOTE: coverity fix
    A_ASSERT(!(NULL == osbuf));
    if (NULL == osbuf)
    {
        return;
    }

    A_NETBUF_PUT(osbuf, (int32_t)(msgLength));

    p = (WMI_HOST_DSET_RESP_CREATE_CMD *)(A_NETBUF_DATA(osbuf));
    if (pHost != NULL)
        p->flags = 0;
    else
        p->flags = 1;

    wmi_cmd_send(pDCxt->pWmiCxt, osbuf, WMI_HOST_DSET_RESP_CREATE_CMDID, NO_SYNC_WMIFLAG);
}

void Api_HostDsetWriteEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    void *osbuf;
    uint32_t msgLength;
    WMI_HOST_DSET_RESP_WRITE_CMD *p;
    HOST_DSET *pDset = NULL;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    WMI_HOST_DSET_WRITE_EVENT *pDsetEvent = (WMI_HOST_DSET_WRITE_EVENT *)datap;

    if (IS_STRRCL_DSET(pDsetEvent->dset_id))
    {
        pDset = dset_get(pDsetEvent->dset_id, pDsetEvent->length);
    }

    if (pDset == NULL)
        return;

    QCADRV_PRINTF("dset write  id=%d  offset:%d  length=%d\n", pDsetEvent->dset_id, pDsetEvent->offset,
                  pDsetEvent->length);
    dset_write(pDset, pDsetEvent->data, pDsetEvent->offset, pDsetEvent->length);

    /*
     *  respose KF dset write request
     */
    msgLength = sizeof(WMI_HOST_DSET_RESP_WRITE_CMD);
    osbuf = A_NETBUF_ALLOC((int32_t)msgLength);

    //NOTE: coverity fix
    A_ASSERT(!(NULL == osbuf));
    if (NULL == osbuf)
    {
        return;
    }

    A_NETBUF_PUT(osbuf, (int32_t)(msgLength));

    p = (WMI_HOST_DSET_RESP_WRITE_CMD *)(A_NETBUF_DATA(osbuf));
    p->dset_id = pDsetEvent->dset_id;
    p->offset = pDsetEvent->offset;
    p->length = pDsetEvent->length;

    wmi_cmd_send(pDCxt->pWmiCxt, osbuf, WMI_HOST_DSET_RESP_WRITE_CMDID, NO_SYNC_WMIFLAG);
}

void Api_HostDsetReadbackEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    HOST_DSET *pDset;
    uint32_t msgLength;
    uint32_t strrcl_offset;
    uint32_t offset, length;
    //    WMI_STORERECALL_RECALL_DSET *pRecallDset;

    uint16_t snd_size;
    void *osbuf;
    WMI_HOST_DSET_READBACK_CMD *p;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    UNUSED_ARGUMENT(devId);

    WMI_HOST_DSET_READBACK_EVENT *pDsetEvent = (WMI_HOST_DSET_READBACK_EVENT *)datap;

    msgLength = sizeof(WMI_HOST_DSET_READBACK_CMD) - sizeof(uint8_t);
    QCADRV_PRINTF("dset id=%d\n", pDsetEvent->dset_id);

    pDset = dset_find(pDsetEvent->dset_id);

    //NOTE: coverity fix
    A_ASSERT(!(NULL == pDset));
    if (NULL == pDset)
    {
        return;
    }

    if (IS_STRRCL_DSET(pDsetEvent->dset_id))
    {
        //    	pRecallDset = (WMI_STORERECALL_RECALL_DSET *)pDset->data_ptr;
        strrcl_offset = sizeof(WMI_STORERECALL_RECALL_DSET);
    }

    offset = pDsetEvent->offset;
    length = pDsetEvent->length;

    if (pDset->length < offset + length)
        length = pDset->length - offset;

    while (length > 0)
    {
        if (length > MAX_EVENT_SIZE)
            snd_size = MAX_EVENT_SIZE;
        else
            snd_size = length;

        osbuf = A_NETBUF_ALLOC((int32_t)msgLength + snd_size);
        if (osbuf == NULL)
        {
            return;
        }

        A_NETBUF_PUT(osbuf, (int32_t)(msgLength + snd_size));
        p = (WMI_HOST_DSET_READBACK_CMD *)(A_NETBUF_DATA(osbuf));
        p->dset_id = pDsetEvent->dset_id;

        p->offset = offset;
        p->length = snd_size;

        dset_read(pDset, p->data, offset + strrcl_offset, snd_size);
        wmi_cmd_send(pDCxt->pWmiCxt, osbuf, WMI_HOST_DSET_READBACK_CMDID, NO_SYNC_WMIFLAG);

        offset += snd_size;
        length -= snd_size;
    }

    restore_power_state(pDCxt, 1);
    A_MDELAY(50);
}

void Api_HostDsetSyncEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    uint32_t msgLength;
    uint16_t count;
    void *osbuf;
    WMI_HOST_DSET_SYNC_CMD *p;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    UNUSED_ARGUMENT(devId);

    WMI_HOST_DSET_SYNC_EVENT *pDsetEvent = (WMI_HOST_DSET_SYNC_EVENT *)datap;

    if (pDsetEvent->dset_count < MAX_DSET_SIZE)
        count = pDsetEvent->dset_count;
    else
        count = MAX_DSET_SIZE;

    msgLength = sizeof(WMI_HOST_DSET_SYNC_CMD) - sizeof(uint8_t);

    dset_length = sizeof(HOST_DSET_ITEM) * count;
    osbuf = A_NETBUF_ALLOC((int32_t)msgLength + dset_length);
    if (osbuf == NULL)
    {
        return;
    }

    A_NETBUF_PUT(osbuf, (int32_t)(msgLength + dset_length));
    p = (WMI_HOST_DSET_SYNC_CMD *)(A_NETBUF_DATA(osbuf));

    count = dset_fill(&p->data[0], count);
    p->dset_count = count;

    wmi_cmd_send(pDCxt->pWmiCxt, osbuf, WMI_HOST_DSET_SYNC_CMDID, NO_SYNC_WMIFLAG);
}

void Api_HostDsetReadEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint8_t *strrclData = pDCxt->tempStorage;
    void *osbuf;
    WMI_STORERECALL_READ_CMD *p;
    uint32_t msgLength = sizeof(WMI_STORERECALL_READ_CMD) - sizeof(uint8_t);

    UNUSED_ARGUMENT(devId);

    osbuf = A_NETBUF_ALLOC((int32_t)msgLength + dset_length);
    if (osbuf == NULL)
    {
        return;
    }

    A_NETBUF_PUT(osbuf, (int32_t)(msgLength + dset_length));

    p = (WMI_STORERECALL_READ_CMD *)(A_NETBUF_DATA(osbuf));

    p->length = A_CPU2LE32(dset_length);
    A_MEMCPY(&p->data[0], strrclData, dset_length);

    QCADRV_PRINTF("[%02x][%02x][%02x][%02x] [%02x][%02x]\n", strrclData[0], strrclData[1], strrclData[2], strrclData[3],
                  strrclData[4], strrclData[5]);

    /* here we append the payload to the msg rather than copy it. this is a
     * memory and CPU optimization as the payload can be very large.
     */
    //    A_NETBUF_APPEND_FRAGMENT(osbuf, strrclData, dset_length);

    wmi_cmd_send(pDCxt->pWmiCxt, osbuf, WMI_DSET_HOST_READ_CMDID, NO_SYNC_WMIFLAG);
}

void Api_dset_read_event(A_DRIVER_CONTEXT *pDCxt, HOST_DSET_HANDLE *pDsetHandle, WMI_DSET_OP_READ_EVENT *pRespRead)
{
    uint32_t length;
    uint32_t offset, buf_off;

    length = pRespRead->length;
    offset = pRespRead->offset;
    buf_off = offset - pRespRead->offset;

    if (pRespRead->status == 0)
    {
        QCADRV_PRINTF("read off:%d len:%d\n", pRespRead->offset, pRespRead->length);

        if (offset + length > pDsetHandle->offset + pDsetHandle->length)
            length = pDsetHandle->offset + pDsetHandle->length - offset;

        A_MEMCPY(pDsetHandle->data_ptr + buf_off, pRespRead->data, length);

        pDsetHandle->left_len -= length;

        if (pDsetHandle->left_len != 0)
            return;
    }

    QCADRV_PRINTF("read done\n");

    if (pDsetHandle->cb != NULL)
        pDsetHandle->cb(pRespRead->status, pDsetHandle->cb_args);
}

void Api_dset_write_event(A_DRIVER_CONTEXT *pDCxt, HOST_DSET_HANDLE *pDsetHandle, WMI_DSET_OP_READ_EVENT *pRespRead)
{
    uint32_t length;
    uint32_t offset;

    length = pRespRead->length;
    offset = pRespRead->offset;

    QCADRV_PRINTF("read off:%d len:%d\n", pRespRead->offset, pRespRead->length);

    if (offset + length > pDsetHandle->offset + pDsetHandle->length)
        length = pDsetHandle->offset + pDsetHandle->length - offset;

    A_MEMCPY(pDsetHandle->data_ptr + offset, pRespRead->data, length);

    pDsetHandle->left_len -= length;

    if (pDsetHandle->left_len != 0)
        return;

    QCADRV_PRINTF("write done\n");

    if (pDsetHandle->cb != NULL)
        pDsetHandle->cb(pRespRead->status, pDsetHandle->cb_args);
}

void Api_DsetOPEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    struct WMIX_DSET_OP_SET_EVENT
    {
        uint32_t dset_id;
    } * pCmd;
    HOST_DSET_HANDLE *pDsetHandle;

    QCADRV_PRINTF("dset op event\n");

    WMI_DSET_OP_EVENT *pDsetOPEvent = (WMI_DSET_OP_EVENT *)datap;

    pCmd = (struct WMIX_DSET_OP_SET_EVENT *)pDsetOPEvent->data;
    pDsetHandle = dset_find_handle(pCmd->dset_id);

    //NOTE: coverity fix
    A_ASSERT(!(NULL == pDsetHandle));
    if (NULL == pDsetHandle)
    {
        return;
    }

    if (pDsetHandle->cb == NULL && (pDsetOPEvent->hdr.commandId != WMIX_DSET_READ_CMDID))
        pDCxt->dset_op_done = true;

    switch (pDsetOPEvent->hdr.commandId)
    {
        case WMIX_DSET_CREATE_CMDID:
        {
            WMI_DSET_OP_CREATE_EVENT *pCreateEvent;
            pCreateEvent = (WMI_DSET_OP_CREATE_EVENT *)pDsetOPEvent->data;

            QCADRV_PRINTF("create return: %d\n", pCreateEvent->status);
            if (pDsetHandle->cb != NULL)
                pDsetHandle->cb(pCreateEvent->status, pDsetHandle->cb_args);

            pDCxt->setparamStatus = pCreateEvent->status;
            pDCxt->dset_op_done = true;
            break;
        }
        case WMIX_DSET_OPEN_CMDID:
        {
            WMI_DSET_OP_CREATE_EVENT *pCreateEvent;
            pCreateEvent = (WMI_DSET_OP_CREATE_EVENT *)pDsetOPEvent->data;

            QCADRV_PRINTF("open return: %d\n", pCreateEvent->status);
            if (pDsetHandle->cb != NULL)
                pDsetHandle->cb(pCreateEvent->status, pDsetHandle->cb_args);

            pDCxt->setparamStatus = pCreateEvent->status;
            pDCxt->dset_op_done = true;
            break;
        }
        case WMIX_DSET_READ_CMDID:
        {
            WMI_DSET_OP_READ_EVENT *pRespRead;
            pRespRead = (WMI_DSET_OP_READ_EVENT *)pDsetOPEvent->data;

            Api_dset_read_event(pDCxt, pDsetHandle, pRespRead);

            pDCxt->setparamStatus = pRespRead->status;
            pDCxt->dset_op_done = true;
            break;
        }
        case WMIX_DSET_WRITE_CMDID:
        {
            WMI_DSET_OP_CREATE_EVENT *pCreateEvent;
            pCreateEvent = (WMI_DSET_OP_CREATE_EVENT *)pDsetOPEvent->data;

            QCADRV_PRINTF("write return: %d\n", pCreateEvent->status);
            if (pDsetHandle->cb != NULL)
                pDsetHandle->cb(pCreateEvent->status, pDsetHandle->cb_args);

            pDCxt->setparamStatus = pCreateEvent->status;
            pDCxt->dset_op_done = true;
            break;
        }
        case WMIX_DSET_COMMIT_CMDID:
        {
            WMI_DSET_OP_COMMIT_EVENT *pRespCommit;
            pRespCommit = (WMI_DSET_OP_COMMIT_EVENT *)pDsetOPEvent->data;

            QCADRV_PRINTF("commit return: %d\n", pRespCommit->status);
            if (pDsetHandle->cb != NULL)
                pDsetHandle->cb(pRespCommit->status, pDsetHandle->cb_args);

            pDCxt->setparamStatus = pRespCommit->status;
            pDCxt->dset_op_done = true;
            break;
        }
        case WMIX_DSET_CLOSE_CMDID:
        {
            WMI_DSET_OP_CLOSE_EVENT *pRespClose;
            pRespClose = (WMI_DSET_OP_CLOSE_EVENT *)pDsetOPEvent->data;

            QCADRV_PRINTF("close return: %d\n", pRespClose->status);
            if (pDsetHandle->cb != NULL)
                pDsetHandle->cb(pRespClose->status, pDsetHandle->cb_args);

            pDCxt->setparamStatus = pRespClose->status;
            pDCxt->dset_op_done = true;
            break;
        }
        case WMIX_DSET_SIZE_CMDID:
        {
            WMI_DSET_OP_CREATE_EVENT *pCreateEvent;
            pCreateEvent = (WMI_DSET_OP_CREATE_EVENT *)pDsetOPEvent->data;

            QCADRV_PRINTF("size return: %d\n", pCreateEvent->status);
            if (pDsetHandle->cb != NULL)
                pDsetHandle->cb(pCreateEvent->status, pDsetHandle->cb_args);

            pDCxt->setparamStatus = pCreateEvent->status;
            pDCxt->dset_op_done = true;
            break;
        }
        case WMIX_DSET_DELETE_CMDID:
        {
            WMI_DSET_OP_CREATE_EVENT *pCreateEvent;
            pCreateEvent = (WMI_DSET_OP_CREATE_EVENT *)pDsetOPEvent->data;

            QCADRV_PRINTF("delete return: %d\n", pCreateEvent->status);
            if (pDsetHandle->cb != NULL)
                pDsetHandle->cb(pCreateEvent->status, pDsetHandle->cb_args);

            pDCxt->setparamStatus = pCreateEvent->status;
            pDCxt->dset_op_done = true;
            break;
        }
    }
    DRIVER_WAKE_DRIVER(pCxt);
}

/*****************************************************************************/
/*  Api_StoreRecallEvent - Processes a WMI_STORERECALL_STORE_EVENT from the
 *	 device.
 *	 This event will come when the device is ready to be shutdown by the host
 *	 as part of a store-and-recall sequence.
 * 		void *pCxt - the driver context.
 *		uint8_t *datap - original event buffer.
 *		int32_t len - length in bytes of datap
 *		void *pReq - original event rx-request object.
 *****************************************************************************/
void Api_StoreRecallEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    WMI_STORERECALL_STORE_EVENT *pEv;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint8_t *strrclData = pDCxt->tempStorage;

    UNUSED_ARGUMENT(devId);

    switch (pDCxt->strrclState)
    {
        case STRRCL_ST_START:
            A_MEMCPY(strrclData, datap, len);
            pEv = (WMI_STORERECALL_STORE_EVENT *)strrclData;

            if (pEv->msec_sleep == 0)
            {
                /* a 0 msec_sleep indicates a firmware condition which
                 * prevented StoreRecall from completing.  in this event this
                 * driver merely aborts the operation and continues normally.
                 */
                pDCxt->strrclState = STRRCL_ST_INIT;
                pDCxt->strrclBlock = false;
                API_STORE_RECALL_EVENT(pCxt);
                /* FIXME: Notify the application that S&R did not complete
                 * successfully.
                 */
            }
            else
            {
                /* indicate chip shutdown via chip_down. */
                // pDCxt->strrclCxt = datap;
                pDCxt->strrclCxt = strrclData;
                pDCxt->strrclCxtLen = len;
                pDCxt->strrclState = STRRCL_ST_ACTIVE;
                /* this call should shutdown the chip and maintain that state for msec_sleep milliseconds */
                Strrcl_Recall(pCxt, A_LE2CPU32(pEv->msec_sleep));
            }
            break;
        case STRRCL_ST_AWAIT_FW:
            pDCxt->strrclState = STRRCL_ST_INIT;
            pDCxt->strrclBlock = false;
            /* Indicate completion to the custom layer */
            API_STORE_RECALL_EVENT(pCxt);
            break;
        default:
            /* do nothing but this should never happen */
            break;
    }
}

/*****************************************************************************/
/*  Api_StoreRecallEvent - Processes a WMI_STORERECALL_STORE_EVENT from the
 *	 device.
 *	 This event will come when the device is ready to be shutdown by the host
 *	 as part of a store-and-recall sequence.
 * 		void *pCxt - the driver context.
 *		uint8_t *datap - original event buffer.
 *		int32_t len - length in bytes of datap
 *		void *pReq - original event rx-request object.
 *****************************************************************************/
void Api_StoreRecallStartEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    WMI_STORERECALL_START_EVENT *pEv;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    //    uint8_t* strrclData = pDCxt->tempStorage;

    UNUSED_ARGUMENT(devId);

    pEv = (WMI_STORERECALL_START_EVENT *)datap;
    if (pEv->msec_sleep == 0)
    {
        /* a 0 msec_sleep indicates a firmware condition which
         * prevented StoreRecall from completing.  in this event this
         * driver merely aborts the operation and continues normally.
         */
        pDCxt->strrclState = STRRCL_ST_INIT;
        pDCxt->strrclBlock = false;
        API_STORE_RECALL_EVENT(pCxt);
        /* FIXME: Notify the application that S&R did not complete
         * successfully.
         */
    }
    else
    {
        /* indicate chip shutdown via chip_down. */
        // pDCxt->strrclCxt = datap;
        // pDCxt->strrclCxt = strrclData;
        // pDCxt->strrclCxtLen = len;

        QCADRV_PRINTF("total = %d   buff len=%d\n", pDCxt->strrclCxtLen, pDCxt->tempStorageLength);

        pDCxt->strrclState = STRRCL_ST_ACTIVE;
        /* this call should shutdown the chip and maintain that state for msec_sleep milliseconds */
        Strrcl_Recall(pCxt, A_LE2CPU32(pEv->msec_sleep));
    }
}

/*****************************************************************************/
/*  Api_WpsProfileEvent - Processes a WMI_WPS_PROFILE_EVENT from the device.
 *	 this event will come when the device has finished a WPS operation. it
 *	 announcess to the host the results of that operation. This function
 *	 stores the information so that it can be processed asynchronously by
 *	 an application thread waiting for the result.
 * 		void *pCxt - the driver context.
 *		uint8_t *datap - original event buffer.
 *		int32_t len - length in bytes of datap
 *		void *pReq - original event rx-request object.
 *****************************************************************************/
void Api_WpsProfileEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq)
{
    WMI_WPS_PROFILE_EVENT *pEv = (WMI_WPS_PROFILE_EVENT *)datap;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    UNUSED_ARGUMENT(len);
    UNUSED_ARGUMENT(devId);
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);

    do
    {
        /* cleanup previous buffer */
        if (pDCxt->wpsBuf != NULL)
        {
            A_NETBUF_FREE(pDCxt->wpsBuf);
            pDCxt->wpsBuf = NULL;
        }

        pDCxt->wpsBuf = pReq;
        pDCxt->wpsEvent = (void *)pEv;
        /* convert event values to host endian format */
        pEv->credential.auth_type = A_LE2CPU16(pEv->credential.auth_type);
        pEv->credential.encr_type = A_LE2CPU16(pEv->credential.encr_type);
        pEv->credential.ap_channel = A_LE2CPU16(pEv->credential.ap_channel);
        /* announce event receive to any waiting threads */
        pDCxt->wpsState = false;
        API_WPS_PROFILE_EVENT(pCxt, datap, len, pReq);

    } while (0);

    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);
}

#if WLAN_CONFIG_11N_AGGR_SUPPORT
/*****************************************************************************/
/*  Api_AggrRecvAddbaReqEvent - processes a WMI_ADDBA_REQ_EVENT from the device.
 *	 These events come when the device reveives a add-block-ACK packet from
 *	 an AP or peer device. This is only relevant for 11n aggregation.
 *      void *pCxt - the driver context.
 *		WMI_ADDBA_REQ_EVENT *evt - the event structure
 *****************************************************************************/
void Api_AggrRecvAddbaReqEvent(void *pCxt, uint8_t devId, WMI_ADDBA_REQ_EVENT *evt)
{
    UNUSED_ARGUMENT(devId);
    evt->st_seq_no = A_LE2CPU16(evt->st_seq_no);

    if (evt->status == 0)
    {
        aggr_recv_addba_req_evt(GET_DRIVER_COMMON(pCxt)->pAggrCxt, evt->tid, evt->st_seq_no, evt->win_sz);
    }
    API_AGGR_RECV_ADDBA_REQ_EVENT(pCxt, evt);
}

/*****************************************************************************/
/*  Api_AggrRecvDelbaReqEvent - processes a WMI_DELBA_EVENT from the device.
 *	 These events come when the device reveives a delete-block-ACK packet from
 *	 an AP or peer device. This is only relevant for 11n aggregation.
 *      void *pCxt - the driver context.
 *		WMI_DELBA_EVENT *evt - the event structure
 *****************************************************************************/
void Api_AggrRecvDelbaReqEvent(void *pCxt, uint8_t devId, WMI_DELBA_EVENT *evt)
{
    UNUSED_ARGUMENT(devId);
    aggr_recv_delba_req_evt(GET_DRIVER_COMMON(pCxt)->pAggrCxt, evt->tid);
    API_AGGR_RECV_DELBA_REQ_EVENT(pCxt, evt);
}
#endif /* WLAN_CONFIG_11N_AGGR_SUPPORT */

/*****************************************************************************/
/*  Api_ReadyEvent - processes a WMI_READY_EVENT from the device. records
 *	 usefull information from the event.
 *      void *pCxt - the driver context.
 *		uint8_t *datap - original event buffer.
 *		uint8_t phyCap - device phy capabilities.
 *		uint32_t sw_ver - device firmware version.
 *		uint32_t abi_ver - device ABI version.
 *****************************************************************************/
void Api_ReadyEvent(void *pCxt, uint8_t devId, uint8_t *datap, uint8_t phyCap, uint32_t sw_ver, uint32_t abi_ver)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    UNUSED_ARGUMENT(devId);
    Api_BootProfile(pCxt, BOOT_PROFILE_WMI_READY);

    pDCxt->wmiReady = true;
    pDCxt->phyCapability = phyCap;
    pDCxt->wlanVersion = sw_ver;
    pDCxt->abiVersion = abi_ver;
    A_MEMCPY(&pDCxt->scan_param, &default_scan_param, sizeof(WMI_SCAN_PARAMS_CMD));

    CUSTOM_API_READY_EVENT(pCxt, datap, phyCap, sw_ver, abi_ver);
}

/*****************************************************************************/
/*  Api_RSNASuccessEvent - processes a RSNA SUCCESS Message from the device.
 *      void *pCxt - the driver context.
 *		uint8_t code - success code.
 *****************************************************************************/
void Api_RSNASuccessEvent(void *pCxt, uint8_t devId, uint8_t code)
{
    UNUSED_ARGUMENT(devId);
    /* this will call the custom event which will call
     * the WifiCallback which will indicate the status
     */
    CUSTOM_API_RSNA_SUCCESS_EVENT(pCxt, devId, code);
}

/*****************************************************************************/
/* GetTemperatureReply - process the temperature reply from the device
 *      void *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *      uint8_t len    - buffer length
 *****************************************************************************/
void Api_GetTemperatureReply(void *pCxt, uint8_t *datap, uint32_t len)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    WMI_GET_TEMPERATURE_REPLY *pReply = (WMI_GET_TEMPERATURE_REPLY *)datap;
    pDCxt->temperatureValid = true;
    pDCxt->raw_temperature = pReply->tempRegVal;
    pDCxt->tempDegree = pReply->tempDegree;
    API_GET_TEMPERATURE_REPLY(pCxt, datap, len);
}

/*****************************************************************************/
/* Api_GetWpsInitReply - Init WPS event back
 *      void *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *      uint8_t code   - init return code
 *****************************************************************************/
void Api_GetWpsInitReply(void *pCxt, uint8_t *datap, uint32_t len)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
   /* this will call the custom event which will call
    * the WifiCallback which will indicate the status
    */
   WMI_WLAN_WPS_INIT_KEY_REPLY *pReply = ( WMI_WLAN_WPS_INIT_KEY_REPLY *)datap;
   pDCxt->wps_init_key = pReply->status;
   API_WPS_INIT_REPLY(pCxt,datap, len);
}

/*****************************************************************************/
/* GetCountryCodeReply - process the countrycode reply from the device
 *      void *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *      uint8_t len    - buffer length
 *****************************************************************************/
void Api_GetCountryCodeReply(void *pCxt, uint8_t *datap, uint32_t len)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    WMI_GET_COUNTRY_CODE_REPLY *pReply = (WMI_GET_COUNTRY_CODE_REPLY *)datap;
    pDCxt->countryCodeValid = true;
    A_MEMCPY(pDCxt->raw_countryCode, pReply->country_code, 3);
    QCADRV_PRINTF("code is %c , %c.\n", pDCxt->raw_countryCode[0], pDCxt->raw_countryCode[1]);
    API_GET_COUNTRY_CODE_REPLY(pCxt, datap, len);
}
/* GetSetParamReply - recv param set ack from the device
 *      A_VOID *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *****************************************************************************/
void Api_GetSetParamReply(void *pCxt, uint8_t *datap)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    WMI_PARAM_SET_REPLY *reply = (WMI_PARAM_SET_REPLY *)datap;

    pDCxt->setparamValid = true;
    pDCxt->setparamStatus = reply->status;
    API_SET_PARAM_REPLY_EVENT(pCxt);
}

#if ENABLE_P2P_MODE
/*****************************************************************************/
/*  p2p_go_neg_event   - processes a P2P GO Negotiation event from the device.
 *      void *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *      uint8_t len    - buffer length
 *****************************************************************************/
void p2p_go_neg_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len, WMI_P2P_PROV_INFO *wps_info)
{
    API_P2P_GO_NEG_EVENT(pCxt, devId, datap, len, wps_info);
}

void p2p_invite_sent_result_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_P2P_INVITE_SEND_RESULT(pCxt, devId, datap, len);
}
/*****************************************************************************/
/*  p2p_node_list_event- event which gives the discovered nodes.
 *      void *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *      uint8_t len    - buffer length
 *****************************************************************************/
void p2p_node_list_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_P2P_NODE_LIST_EVENT(pCxt, devId, datap, len);
}

/*****************************************************************************/
/*  p2p_list_persistent_network_event - event which gives the discovered nodes.
 *      void *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *      uint8_t len    - buffer length
 *****************************************************************************/
void p2p_list_persistent_network_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_P2P_LIST_PERSISTENT_NETWORK_EVENT(pCxt, devId, datap, len);
}

/*****************************************************************************/
/*  p2p_req_auth_event - processes a response event for authentication request.
 *      void *pCxt   - the driver context.
 *      uint8_t *datap - original event buffer.
 *      uint8_t len    - buffer length
 *****************************************************************************/
void p2p_req_auth_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_P2P_REQ_AUTH_EVENT(pCxt, devId, datap, len);
}

void get_p2p_ctx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_GET_P2P_CTX(pCxt, devId, datap, len);
}

void get_p2p_prov_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_P2P_PROV_DISC_REQ(pCxt, devId, datap, len);
}
#if 1 // KK
void get_p2p_serv_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_P2P_SERV_DISC_REQ(pCxt, devId, datap, len);
}

void p2p_invite_req_rx(void *pCxt, uint8_t devId, uint8_t *datap, uint8_t len)
{
    API_P2P_INVITE_REQ(pCxt, devId, datap, len);
}

void p2p_invite_rcvd_result_ev(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    API_P2P_INVITE_RCVD_RESULT(pCxt, devId, datap, len);
}
#endif
#endif

#if MANUFACTURING_SUPPORT
void Api_TestCmdEventRx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    UNUSED_ARGUMENT(devId);
    API_TEST_RESP_EVENT(pCxt, datap, len);

    pDCxt->testResp = true;
    DRIVER_WAKE_USER(pCxt);
}
#endif

/*****************************************************************************/
/*  Api_RxDbglogEvent - Processes a WMIX_DBGLOG_EVENT from the device.
 *****************************************************************************/
void Api_RxDbglogEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len)
{
    UNUSED_ARGUMENT(devId);
    API_WMIX_DBGLOG_EVENT(wmip, datap, len);
}

/*****************************************************************************/
/*  Api_RxGpioDataEvent - Processes a WMIX_GPIO_DATA_EVENT from the device.
 *****************************************************************************/
void Api_RxGpioDataEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len)
{
    UNUSED_ARGUMENT(devId);
    CUSTOM_API_WMIX_GPIO_DATA_EVENT(wmip, datap, len);
}

/*****************************************************************************/
/*  Api_RxHBChallengeEvent - Processes a WMIX_GPIO_DATA_EVENT from the device.
 *****************************************************************************/
void Api_RxHBChallengeEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len)
{
    //pDCxt = GET_DRIVER_COMMON(pCxt);
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);  
    WMIX_HB_CHALLENGE_RESP_EVENT *pEv = (WMIX_HB_CHALLENGE_RESP_EVENT *)datap;

    if(pEv->cookie == pDCxt->hb_sequence)
    pDCxt->hb_challenge_done = true;

    DRIVER_WAKE_USER(pCxt);
}

#if ENABLE_KF_PERFORMANCE

/*****************************************************************************/
/*  Api_RxPfmDataEvent - Processes a WMIX_PFM_DATA_EVENT from the device.
 *****************************************************************************/
void Api_RxPfmDataEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len)
{
    UNUSED_ARGUMENT(devId);
    CUSTOM_API_WMIX_PFM_DATA_EVENT(wmip, datap, len);
}

/*****************************************************************************/
/*  Api_RxPfmDataDoneEvent - Processes a WMIX_PFM_DATA_DONE_EVENT from the device.
 *****************************************************************************/

void Api_RxPfmDataDoneEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len)
{
    UNUSED_ARGUMENT(devId);
    CUSTOM_API_WMIX_PFM_DATA_DONE_EVENT(wmip, datap, len);
}
#endif

#if 0
/*****************************************************************************/
/*  Api_PFMReportEvent - Processes a WMI_PFM_REPORT_EVENTID from the device. 
 *****************************************************************************/
void add_one_profile(uint8_t event_id, uint16_t  timestamp);

void 
Api_PFMReportEvent(void *pCxt, uint8_t devId, WMI_PFM_REPORT_EVENT *pEv)
{
	uint8_t   i;    
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    UNUSED_ARGUMENT(devId);
	for (i=0; i < MAX_TIMESTAMP; i++)
    {
	    if (pEv->eventID[i] == 0)
		    break;

        add_one_profile(pEv->eventID[i] + 15, (uint16_t)pEv->timestamp[i] / 32);
    }
	pDCxt->pfmDone = true;

	DRIVER_WAKE_USER(pCxt);
}

/*****************************************************************************/
/*  Api_PFMReportEvent - Processes a WMI_PFM_REPORT_EVENTID from the device. 
 *****************************************************************************/
void 
Api_DiagReportEvent(void *pCxt, WMI_DIAG_EVENT *pEv)
{
	if(ath_custom_init.Boot_Profile == NULL)
		return;

/*	if (pEv->commandId == WMID_FSM_AUTH_EVENTID)
    {
		ath_custom_init.Boot_Profile(BOOT_PROFILE_AUTH_EVENT);
    }

	else if (pEv->commandId == WMID_FSM_ASSOC_EVENTID)
    {
		ath_custom_init.Boot_Profile(BOOT_PROFILE_ASSOC_EVENT);
    }
	else if (pEv->commandId == WMID_START_SCAN_EVENTID)
	{
		ath_custom_init.Boot_Profile(BOOT_PROFILE_START_SCAN);
	}
*/
}
#endif

#if ENABLE_P2P_MODE
void Api_p2p_go_neg_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len, WMI_P2P_PROV_INFO *wps_info)
{
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_GO_NEG_RESULT_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap, sizeof(WMI_P2P_GO_NEG_RESULT_EVENT));

        if (GET_DRIVER_COMMON(pCxt)->p2pEvtState == true)
        {
            GET_DRIVER_COMMON(pCxt)->p2pEvtState = false;
        }
        else
        {
            GET_DRIVER_COMMON(pCxt)->p2pevtflag = true;
        }
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_node_list_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    uint8_t *tmpBuf;
    evt_id = WMI_P2P_NODE_LIST_EVENTID;
    WMI_P2P_NODE_LIST_EVENT *handleP2PDev = (WMI_P2P_NODE_LIST_EVENT *)datap;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    tmpBuf = GET_DRIVER_COMMON(pCxt)->pScanOut;
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);
        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));
        tmpBuf += sizeof(uint32_t);
        *tmpBuf = handleP2PDev->num_p2p_dev;
        tmpBuf++;

        A_MEMCPY((uint8_t *)tmpBuf, ((uint8_t *)(handleP2PDev->data)),
                 (sizeof(P2P_DEVICE_LITE) * (handleP2PDev->num_p2p_dev)));

        GET_DRIVER_COMMON(pCxt)->p2pEvtState = false;
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_req_auth_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_REQ_TO_AUTH_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap, sizeof(WMI_P2P_REQ_TO_AUTH_EVENT));

        GET_DRIVER_COMMON(pCxt)->p2pevtflag = true;
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_list_persistent_network_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_LIST_PERSISTENT_NETWORK_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    WMI_P2P_PERSISTENT_LIST_NETWORK_EVENT *ev = (WMI_P2P_PERSISTENT_LIST_NETWORK_EVENT *)datap;

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), ev->data,
                 (MAX_LIST_COUNT * sizeof(WMI_PERSISTENT_MAC_LIST)));

        GET_DRIVER_COMMON(pCxt)->p2pEvtState = false;
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_get_p2p_ctx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_PROV_DISC_RESP_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap, sizeof(WMI_P2P_PROV_DISC_RESP_EVENT));

        GET_DRIVER_COMMON(pCxt)->p2pEvtState = false;
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_prov_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_PROV_DISC_REQ_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap, sizeof(WMI_P2P_PROV_DISC_REQ_EVENT));

        GET_DRIVER_COMMON(pCxt)->p2pevtflag = true;
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_serv_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_SDPD_RX_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap, sizeof(WMI_P2P_SDPD_RX_EVENT));

        GET_DRIVER_COMMON(pCxt)->p2pevtflag = true;
    }

    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_invite_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    // async event for peer invite
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_INVITE_REQ_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap, sizeof(WMI_P2P_FW_INVITE_REQ_EVENT));

        GET_DRIVER_COMMON(pCxt)->p2pevtflag = true;
    }

    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_invite_rcvd_result(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    evt_id = WMI_P2P_INVITE_RCVD_RESULT_EVENTID;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap,
                 sizeof(WMI_P2P_INVITE_RCVD_RESULT_EVENT));

        GET_DRIVER_COMMON(pCxt)->p2pevtflag = true;
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

void Api_p2p_invite_send_result(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len)
{
    uint32_t evt_id = 0;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    evt_id = WMI_P2P_INVITE_SENT_RESULT_EVENTID;

    DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt);
    {
        A_MEMZERO(GET_DRIVER_COMMON(pCxt)->pScanOut, pDCxt->tempStorageLength);

        A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, &evt_id, sizeof(uint32_t));

        A_MEMCPY((GET_DRIVER_COMMON(pCxt)->pScanOut) + sizeof(uint32_t), datap,
                 sizeof(WMI_P2P_INVITE_SENT_RESULT_EVENT));

        GET_DRIVER_COMMON(pCxt)->p2pevtflag = true;
    }
    DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt);

    DRIVER_WAKE_USER(pCxt);
}

#endif
#if MANUFACTURING_SUPPORT
void Api_Test_Cmd_Event(void *pCxt, uint8_t *datap, uint32_t len)
{
    UNUSED_ARGUMENT(len);
    A_MEMCPY(GET_DRIVER_COMMON(pCxt)->pScanOut, datap, len);
    GET_DRIVER_COMMON(pCxt)->testCmdRespBufLen = len;
}
#endif

void Api_ProbeReqEvent(void *pCxt, uint8_t *datap, uint32_t len)
{
    CUSTOM_API_PROBEREQ_EVENT(pCxt, datap, len);
}
