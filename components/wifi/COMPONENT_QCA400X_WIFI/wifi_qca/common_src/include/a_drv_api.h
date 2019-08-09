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
#ifndef _A_DRV_API_H_
#define _A_DRV_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/****************************************************************************/
/**                                                                        **/
/** WMI related hooks                                                      **/
/**                                                                        **/
/****************************************************************************/
/****************************************************************************/
void Api_ChannelListEvent(void *pCxt, uint8_t devId, int8_t numChan, void *chanList);
void Api_TargetStatsEvent(void *pCxt, uint8_t devId, uint8_t *ptr, uint32_t len);
void Api_ScanCompleteEvent(void *pCxt, uint8_t devId, A_STATUS status);
void Api_RegDomainEvent(void *pCxt, uint8_t devId, uint32_t regCode);
void Api_DisconnectEvent(void *pCxt,
                         uint8_t devId,
                         uint8_t reason,
                         uint8_t *bssid,
                         uint8_t assocRespLen,
                         uint8_t *assocInfo,
                         uint16_t protocolReasonStatus);
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
                      uint8_t *assocInfo);
void Api_BitrateEvent(void *pCxt, uint8_t devId, int32_t rateKbps);
void Api_ReadyEvent(void *pCxt, uint8_t devId, uint8_t *datap, uint8_t phyCap, uint32_t sw_ver, uint32_t abi_ver);
void Api_BssInfoEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len);
void Api_TkipMicErrorEvent(void *pCxt, uint8_t devId, uint8_t keyid, boolean ismcast);
void Api_GetPmkEvent(void *pCxt, uint8_t devId, uint8_t *datap);
void Api_RxDbglogEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len);
void Api_RxGpioDataEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len);
void Api_RxHBChallengeEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len);
void Api_RxPfmDataEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len);
void Api_RxPfmDataDoneEvent(void *wmip, uint8_t devId, uint8_t *datap, int32_t len);

void Api_StoreRecallEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_StoreRecallStartEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_HostDsetStoreEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_HostDsetReadEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_HostDsetCreateEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_HostDsetWriteEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_HostDsetReadbackEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_HostDsetSyncEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_DsetOPEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);

void Api_WpsProfileEvent(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);
void Api_AggrRecvAddbaReqEvent(void *pCxt, uint8_t devId, WMI_ADDBA_REQ_EVENT *evt);    
void Api_AggrRecvDelbaReqEvent(void *pCxt, uint8_t devId, WMI_DELBA_EVENT *evt);
A_STATUS
Api_WmiTxStart(void *pCxt, void *pReq, HTC_ENDPOINT_ID eid);
void Api_RSNASuccessEvent(void *pCxt, uint8_t devId, uint8_t code);
void Api_GetBitRateEvent(void *pCxt, uint8_t devId, void *datap);
A_STATUS
Api_SockResponseEventRx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len, void *pReq);
void Api_ProbeReqEvent(void *pCxt, uint8_t *datap, uint32_t len);

#if MANUFACTURING_SUPPORT
void Api_TestCmdEventRx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);
#endif
void Api_GetTemperatureReply(void *pCxt, uint8_t *datap, uint32_t len);
void Api_GetCountryCodeReply(void *pCxt, uint8_t *datap, uint32_t len);
void Api_GetSetParamReply(void *pCxt, uint8_t *datap);
void Api_GetWpsInitReply(void *pCxt, uint8_t *datap, uint32_t len);
#if ENABLE_P2P_MODE
void p2p_go_neg_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len, WMI_P2P_PROV_INFO *wps_info);

void p2p_invite_sent_result_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void p2p_node_list_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void p2p_list_persistent_network_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void p2p_req_auth_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void get_p2p_ctx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void get_p2p_prov_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void get_p2p_serv_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void p2p_invite_req_rx(void *pCxt, uint8_t devId, uint8_t *datap, uint8_t len);

void p2p_invite_rcvd_result_ev(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

#define A_WMI_P2P_GO_NEG_EVENT(devt, devId, res, len, wps_info) \
    p2p_go_neg_event((devt), (devId), (res), (len), (wps_info))

#define A_WMI_P2P_NODE_LIST_EVENT(devt, devId, res, len) p2p_node_list_event((devt), (devId), (res), (len))

#define A_WMI_P2P_PERSISTENT_LIST_NETWORK_EVENT(devt, devId, res, len) \
    p2p_list_persistent_network_event((devt), (devId), (res), (len))

#define A_WMI_P2P_REQ_AUTH_EVENT(devt, devId, res, len) p2p_req_auth_event((devt), (devId), (res), (len))

#define A_WMI_GET_P2P_CTX(devt, devId, res, len) get_p2p_ctx((devt), (devId), (res), (len))

#define A_WMI_P2P_PROV_DISC_REQ(devt, devId, res, len) get_p2p_prov_disc_req((devt), (devId), (res), (len))
#if 1 // KK
#define A_WMI_P2P_SERV_DISC_REQ(devt, devId, res, len) get_p2p_serv_disc_req((devt), (devId), (res), (len))

#define A_WMI_P2P_INVITE_REQ_RX(devt, devId, datap, len) p2p_invite_req_rx((devt), (devId), (datap), (len))

#define A_WMI_P2P_INVITE_RCVD_RESULT_EVENT(devt, devId, datap, len) \
    p2p_invite_rcvd_result_ev((devt), (devId), (datap), (len))

#define A_WMI_P2P_INVITE_SENT_RESULT_EVENT(devt, devId, res, len) \
    p2p_invite_sent_result_event((devt), (devId), (res), (len))
#endif
#endif

#define A_WMI_CHANNELLIST_RX(devt, devId, numChan, chanList) \
    Api_ChannelListEvent((devt), (devId), (numChan), (chanList))

#define A_WMI_SET_NUMDATAENDPTS(devt, num)

#define A_WMI_CONTROL_TX(devt, osbuf, streamID) Api_WmiTxStart((devt), (osbuf), (streamID))

#define A_WMI_TARGETSTATS_EVENT(devt, devId, pStats, len) Api_TargetStatsEvent((devt), (devId), (pStats), (len))

#define A_WMI_SCANCOMPLETE_EVENT(devt, devId, status) Api_ScanCompleteEvent((devt), (devId), (status))

#define A_WMI_CONNECT_EVENT(devt, devId, channel, bssid, listenInterval, beaconInterval, networkType, beaconIeLen, \
                            assocReqLen, assocRespLen, assocInfo)                                                  \
    Api_ConnectEvent((devt), (devId), (channel), (bssid), (listenInterval), (beaconInterval), (networkType),       \
                     (beaconIeLen), (assocReqLen), (assocRespLen), (assocInfo))

#define A_WMI_REGDOMAIN_EVENT(devt, devId, regCode) Api_RegDomainEvent((devt), (devId), (regCode))

#define A_WMI_DISCONNECT_EVENT(devt, devId, reason, bssid, assocRespLen, assocInfo, protocolReasonStatus) \
    Api_DisconnectEvent((devt), (devId), (reason), (bssid), (assocRespLen), (assocInfo), (protocolReasonStatus))

#define A_WMI_RSNA_SUCCESS_EVENT(devt, devId, code) Api_RSNASuccessEvent((devt), (devId), (code))

#define A_WMI_BITRATE_RX(devt, devId, rateKbps) Api_BitrateEvent((devt), (devId), (rateKbps))

/* A_WMI_TXPWR_RX - not currently supported */
#define A_WMI_TXPWR_RX(devt, devId, txPwr)

#define A_WMI_READY_EVENT(devt, devId, datap, phyCap, sw_ver, abi_ver) \
    Api_ReadyEvent((devt), (devId), (datap), (phyCap), (sw_ver), (abi_ver))

#define A_WMI_GET_BITRATE_EVEVT(devt, devId, datap) Api_GetBitRateEvent((devt), (devId), (datap))

#define A_WMI_SEND_EVENT_TO_APP(ar, eventId, datap, len)
#define A_WMI_SEND_GENERIC_EVENT_TO_APP(ar, eventId, datap, len)

#define A_WMI_BSSINFO_EVENT_RX(ar, devId, datap, len) Api_BssInfoEvent((ar), (devId), (datap), (len))

#define A_WMI_TKIP_MICERR_EVENT(devt, devId, keyid, ismcast) Api_TkipMicErrorEvent((devt), (devId), (keyid), (ismcast))

#define A_WMI_Ac2EndpointID(devht, devId, ac) Driver_AC2EndpointID((devht), (devId), (ac))

#define A_WMI_Endpoint2Ac(devt, devId, ep) Driver_EndpointID2AC((devt), (devId), (ep))

#define A_WMI_GET_PMK_RX(devt, devId, datap) Api_GetPmkEvent((devt), (devId), (datap))

#define A_WMIX_DBGLOG_EVENT(wmip, devId, datap, len) Api_RxDbglogEvent((wmip), (devId), (datap), (len))

#define A_WMIX_GPIO_DATA_EVENT(wmip, devId, datap, len) Api_RxGpioDataEvent((wmip), (devId), (datap), (len))

#define WMIX_HB_CHALLENGE_RESP_RX(wmip, devId, datap, len) Api_RxHBChallengeEvent((wmip), (devId), (datap), (len))

#define A_WMIX_PFM_DATA_EVENT(wmip, devId, datap, len) Api_RxPfmDataEvent((wmip), (devId), (datap), (len))

#define A_WMIX_PFM_DATA_DONE_EVENT(wmip, devId, datap, len) Api_RxPfmDataDoneEvent((wmip), (devId), (datap), (len))

#if DRIVER_CONFIG_ENABLE_STORE_RECALL
#define A_WMI_STORERECALL_EVENT_RX(devt, devId, datap, len, osbuf) \
    Api_StoreRecallEvent((devt), (devId), (datap), (len), (osbuf))
#else
#define A_WMI_STORERECALL_EVENT_RX(devt, devId, datap, len, osbuf) A_OK
#endif

#define A_WMI_STORERECALL_START_EVENT_RX(devt, devId, datap, len, osbuf) \
    Api_StoreRecallStartEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_PROBE_REQ_EVENT_RX(devt, datap, len) Api_ProbeReqEvent((devt), (datap), (len))

#define A_WMI_HOST_DSET_EVENT_STORE_RX(devt, devId, datap, len, osbuf) \
    Api_HostDsetStoreEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_HOST_DSET_EVENT_READ_RX(devt, devId, datap, len, osbuf) \
    Api_HostDsetReadEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_HOST_DSET_EVENT_CREATE_RX(devt, devId, datap, len, osbuf) \
    Api_HostDsetCreateEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_HOST_DSET_EVENT_WRITE_RX(devt, devId, datap, len, osbuf) \
    Api_HostDsetWriteEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_HOST_DSET_EVENT_READBACK_RX(devt, devId, datap, len, osbuf) \
    Api_HostDsetReadbackEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_HOST_DSET_EVENT_SYNC_RX(devt, devId, datap, len, osbuf) \
    Api_HostDsetSyncEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_DSET_EVENT_OP_RX(devt, devId, datap, len, osbuf) Api_DsetOPEvent((devt), (devId), (datap), (len), (osbuf))

#define A_WMI_WPS_PROFILE_EVENT_RX(devt, devId, datap, len, osbuf) \
    Api_WpsProfileEvent((devt), (devId), (datap), (len), (osbuf))

#if WLAN_CONFIG_11N_AGGR_SUPPORT
#define A_WMI_AGGR_RECV_ADDBA_REQ_EVT(devt, devId, cmd) Api_AggrRecvAddbaReqEvent((devt), (devId), (cmd))

#define A_WMI_AGGR_RECV_DELBA_REQ_EVT(devt, devId, cmd) Api_AggrRecvDelbaReqEvent((devt), (devId), (cmd))
#endif

#define A_WMI_GET_TEMPERATURE_REPLY(devt, datap, len) Api_GetTemperatureReply((devt), (datap), (len))

#define A_WMI_GET_COUNTRY_CODE_REPLY(devt, datap, len) Api_GetCountryCodeReply((devt), (datap), (len))
#define A_WMI_SET_PARAM_REPLY(devt, datap) Api_GetSetParamReply((devt), (datap))
#define A_WMI_WLAN_WPS_INIT_REPLY_RX(devt, datap, len) Api_GetWpsInitReply((devt), (datap), (len))

#if ENABLE_STACK_OFFLOAD
#define A_WMI_SOCK_RESPONSE_EVENT_RX(devt, devId, datap, len, osbuf) \
    Api_SockResponseEventRx((devt), (devId), (datap), (len), (osbuf))
#else
#define A_WMI_SOCK_RESPONSE_EVENT_RX(devt, devId, datap, len, osbuf) A_OK
#endif
#if MANUFACTURING_SUPPORT
#define A_WMI_TEST_CMD_EVENT_RX(devt, devId, datap, len) Api_TestCmdEventRx((devt), (devId), (datap), (len))
#endif
#ifdef __cplusplus
}
#endif

#endif
