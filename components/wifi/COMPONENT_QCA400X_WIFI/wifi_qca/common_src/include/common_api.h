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
#ifndef _COMMON_API_H_
#define _COMMON_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <a_config.h>
#include <a_types.h>
#include <htc_api.h>
#include <driver_cxt.h>
#include <atheros_wifi_api.h>

// IGX_UD_CHANGES
typedef struct hcd_intr_callback_t
{
    void *pCxt;
} HCD_INTR_CB, *HCD_INTR_CALLBACK_T;

A_STATUS Driver_CreateThread(void *pCxt);

A_STATUS Driver_DestroyThread(void *pCxt);

void HW_EnableDisableIRQ(void *pCxt, boolean Enable, boolean FromIrq);

A_STATUS HW_ProcessPendingInterrupts(void *pCxt);

A_STATUS wmi_dset_host_cfg_cmd(void *handle);

A_STATUS Driver_Init(void *pCxt);

A_STATUS Driver_DeInit(void *pCxt);

A_STATUS Driver_ContextInit(void *pCxt);

A_STATUS Driver_ContextDeInit(void *pCxt);

uint32_t Htc_ReadCreditCounter(void *pCxt, uint32_t index);

void Htc_GetCreditCounterUpdate(void *pCxt, uint16_t epid);

void Htc_PrepareRecvPacket(void *pCxt, void *pReq);

A_STATUS Htc_SendPacket(void *pCxt, void *pReq);

void Htc_RxComplete(void *pCxt, void *pReq);

A_STATUS Htc_ProcessTxComplete(void *pCxt, void *pReq);

A_STATUS Htc_ProcessRecvHeader(void *pCxt, void *pReq, uint32_t *pNextLookAheads, uint32_t *pNumLookAheads);

A_STATUS HTC_Start(void *pCxt);

A_STATUS HTC_WaitTarget(void *pCxt);

A_STATUS HTC_ConnectService(void *pCxt, HTC_SERVICE_CONNECT_REQ *pConnectReq, HTC_SERVICE_CONNECT_RESP *pConnectResp);

void HTC_ProcessCpuInterrupt(void *pCxt);

void Hcd_Irq(void *pCxt);

A_STATUS Driver_Main(void *pCxt, uint8_t scope, boolean *pCanBlock, uint16_t *pBlock_msec);

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)
void Driver_ReportRxBuffStatus(void *pCxt, boolean status, uint8_t epid);
#else
void Driver_ReportRxBuffStatus(void *pCxt, boolean status);
#endif

A_STATUS Driver_CompleteRequest(void *pCxt, void *pReq);

boolean Driver_TxReady(void *pCxt);

A_STATUS Driver_SubmitTxRequest(void *pCxt, void *pReq);

A_STATUS Driver_SubmitEp0TxRequest(void *pCxt, void *pReq);

boolean Driver_RxReady(void *pCxt);

void Driver_DropTxDataPackets(void *pCxt);

A_STATUS Driver_SendPacket(void *pCxt, void *pReq);

A_STATUS Driver_RecvPacket(void *pCxt, void *pReq);

A_STATUS Driver_GetTargetInfo(void *pCxt);

A_STATUS Driver_BootComm(void *pCxt);

void Driver_RxComplete(void *pCxt, void *pReq);

A_STATUS Driver_ReadRegDiag(void *pCxt, uint32_t *address, uint32_t *data);

A_STATUS Driver_WriteRegDiag(void *pCxt, uint32_t *address, uint32_t *data);

A_STATUS Driver_ReadDataDiag(void *pCxt, uint32_t address, uint8_t *data, uint32_t length);

A_STATUS Driver_WriteDataDiag(void *pCxt, uint32_t address, uint8_t *data, uint32_t length);

A_STATUS Driver_DumpAssertInfo(void *pCxt, uint32_t *pData, uint16_t *pLength);

A_STATUS Driver_StoreRecallFirmwareDownload(void *pCxt);

void HW_EnableDisableSPIIRQ(void *pCxt, boolean Enable, boolean FromIrq);

void HW_PowerUpDown(void *pCxt, uint8_t powerUp);

A_STATUS Bus_InOutToken(void *pCxt, uint32_t OutToken, uint8_t DataSize, uint32_t *pInToken);

void Bus_TransferComplete(void *pCxt, A_STATUS status);

void HW_InterruptHandler(void *pCxt);

void Strrcl_Recall(void *pCxt, uint32_t msec_sleep);

A_STATUS Hcd_ProgramWriteBufferWaterMark(void *pCxt, uint32_t length);

void Hcd_RefreshWriteBufferSpace(void *pCxt);

// Todo
A_STATUS Hcd_DoPioInternalAccess(void *pCxt, uint16_t addr, uint32_t *pValue, boolean isRead);

/**********TBD**********/
uint16_t Api_TxGetStatus(void *pCxt);

void Api_TxComplete(void *pCxt, void *pReq);

void Api_RxComplete(void *pCxt, void *pReq);

A_STATUS Api_WmiTxStart(void *pCxt, void *pReq, HTC_ENDPOINT_ID eid);

A_STATUS Api_DataTxStart(void *pCxt, void *pReq);

void Api_BootProfile(void *pCxt, uint8_t val);

A_STATUS Api_InitStart(void *pCxt);

A_STATUS Api_InitFinish(void *pCxt);

void Api_WMIInitFinish(void *pCxt);

A_STATUS Api_DeInitStart(void *pCxt);

A_STATUS Api_DeInitFinish(void *pCxt);

A_STATUS Api_DisconnectWiFi(void *pCxt);

A_STATUS Api_ConnectWiFi(void *pCxt);

A_STATUS Api_ParseInfoElem(void *pCxt, WMI_BSS_INFO_HDR *bih, int32_t len, A_SCAN_SUMMARY *pSummary);

A_STATUS Api_DriverAccessCheck(void *pCxt, uint8_t block_allowed, uint8_t request_reason);

A_STATUS Api_ProgramCountryCode(void *pCxt, uint8_t *country_code, uint16_t length, uint8_t *pResult);

A_STATUS Api_ProgramMacAddress(void *pCxt, uint8_t *addr, uint16_t length, uint8_t *pResult);

A_STATUS Api_SetPowerMode(void *pCxt, POWER_MODE *pwrmode);

uint32_t HW_GetMboxAddress(void *pCxt, uint16_t mbox, uint32_t length);

A_ENDPOINT_T *Util_GetEndpoint(void *pCxt, uint16_t id);

uint16_t Util_Ieee2freq(int32_t chan);

uint32_t Util_Freq2ieee(uint16_t freq);

HTC_ENDPOINT_ID Util_AC2EndpointID(void *pCxt, uint8_t ac);

uint8_t Util_EndpointID2AC(void *pCxt, HTC_ENDPOINT_ID ep);

uint8_t Util_Ascii2Hex(char val);

A_STATUS scan_setup(void *pCxt, void *pWmi, WMI_START_SCAN_CMD *start_scan);

A_STATUS wait_scan_done(void *pCxt, void *pWmi);

A_STATUS move_power_state_to_maxperf(void *pCxt, int32_t module);

A_STATUS TxRawPacket(void *pCxt, void *pReq, ATH_MAC_TX_PARAMS *pParams);

#if MANUFACTURING_SUPPORT
void Api_Test_Cmd_Event(void *pCxt, uint8_t *datap, uint32_t len);
#endif

#if MANUFACTURING_SUPPORT
#define API_TEST_RESP_EVENT(pCxt, datap, len) Api_Test_Cmd_Event((pCxt), (datap), (len))
#endif

void Driver_WakeDriver(void *pCxt);

void Driver_WakeUser(void *pCxt);


#define DRIVER_WAKE_DRIVER(pCxt) Driver_WakeDriver((pCxt))
#define DRIVER_WAKE_USER(pCxt) Driver_WakeUser((pCxt))

#if ENABLE_STACK_OFFLOAD

#define SOCKET_CONTEXT_INIT socket_context_init()
#define STACK_INIT(pCxt) send_stack_init((pCxt))
#define SOCKET_CONTEXT_DEINIT() socket_context_deinit()
#define MIN_HDR_LEN sizeof(WMI_DATA_HDR)
#define WMI_DOT3_2_DIX(pReq) A_OK

#else

#define STACK_INIT(pCxt) A_OK
#define SOCKET_CONTEXT_INIT A_OK
#define SOCKET_CONTEXT_DEINIT() A_OK
#define MIN_HDR_LEN sizeof(WMI_DATA_HDR) + sizeof(ATH_MAC_HDR) + sizeof(ATH_LLC_SNAP_HDR)
#define WMI_DOT3_2_DIX(pReq) wmi_dot3_2_dix((pReq))

#endif

enum BOOT_PROFILE
{
    BOOT_PROFILE_WMI_READY = 0,
    BOOT_PROFILE_READ_REFCLK,
    BOOT_PROFILE_DONE_CONFIG,
    BOOT_PROFILE_START_CONNECT,
    BOOT_PROFILE_DONE_CONNECT,
    BOOT_PROFILE_DRIVE_READY,
    BOOT_PROFILE_START_SCAN,
    BOOT_PROFILE_DONE_SCAN,
    BOOT_PROFILE_POWER_UP,
    BOOT_PROFILE_BOOT_PARAMETER,
};

#if ENABLE_P2P_MODE

#define API_P2P_GO_NEG_EVENT(devt, devId, res, len, wps_info) \
    Api_p2p_go_neg_event((devt), (devId), (res), (len), (wps_info))

#define API_P2P_INVITE_SEND_RESULT(pCxt, devId, datap, len) Api_p2p_invite_send_result((pCxt), (devId), (datap), (len))

#define API_P2P_NODE_LIST_EVENT(devt, devId, res, len) Api_p2p_node_list_event((devt), (devId), (res), (len))

#define API_P2P_LIST_PERSISTENT_NETWORK_EVENT(devt, devId, res, len) \
    Api_p2p_list_persistent_network_event((devt), (devId), (res), (len))

#define API_GET_P2P_CTX(devt, devId, res, len) Api_get_p2p_ctx((devt), (devId), (res), (len))

#define API_P2P_PROV_DISC_REQ(devt, devId, res, len) Api_p2p_prov_disc_req((devt), (devId), (res), (len))

#define API_P2P_SERV_DISC_REQ(devt, devId, res, len) Api_p2p_serv_disc_req((devt), (devId), (res), (len))

#define API_P2P_INVITE_REQ(pCxt, devId, datap, len) Api_p2p_invite_req((pCxt), (devId), (datap), (len))

#define API_P2P_INVITE_RCVD_RESULT(pCxt, devId, datap, len) Api_p2p_invite_rcvd_result((pCxt), (devId), (datap), (len))

#define API_P2P_REQ_AUTH_EVENT(devt, devId, res, len) Api_p2p_req_auth_event((devt), (devId), (res), (len))

void Api_p2p_go_neg_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len, WMI_P2P_PROV_INFO *wps_info);

void Api_p2p_invite_send_result(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_p2p_node_list_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_p2p_list_persistent_network_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_get_p2p_ctx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_p2p_prov_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_p2p_serv_disc_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_p2p_invite_req(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_p2p_req_auth_event(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);

void Api_p2p_invite_rcvd_result(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len);
#endif // ENABLE_P2P_MODE

#if MANUFACTURING_SUPPORT
#define API_TEST_RESP_EVENT(pCxt, datap, len) Api_Test_Cmd_Event((pCxt), (datap), (len))

void Api_Test_Cmd_Event(void *pCxt, uint8_t *datap, uint32_t len);

#endif

#ifdef __cplusplus
}
#endif

#endif /* _COMMON_API_H_ */
