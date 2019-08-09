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
#ifndef _CUSTOM_API_H_
#define _CUSTOM_API_H_

#include <wmi.h>

void Custom_HW_PowerUpDown(void *pCxt, uint32_t powerUp);
/*1A_STATUS
Custom_HW_Init(void *pCxt);
A_STATUS
Custom_HW_DeInit(void *pCxt);*/

void custom_os_usec_delay(uint32_t uSeconds);

A_STATUS
Custom_Driver_CreateThread(void *pCxt);
A_STATUS
Custom_Driver_DestroyThread(void *pCxt);
A_STATUS
Custom_Driver_ContextInit(void *pCxt);
void Custom_Driver_ContextDeInit(void *pCxt);
void *Custom_GetRxRequest(void *pCxt, uint16_t length);
void Custom_DeliverFrameToNetworkStack(void *pCxt, void *pReq);
void Custom_Api_BssInfoEvent(void *pCxt, uint8_t *datap, int32_t len);
void Custom_Api_ConnectEvent(void *pCxt, uint8_t devId, uint16_t channel, uint8_t *bssid);
void Custom_Api_DisconnectEvent(void *pCxt,
                                uint8_t devId,
                                uint8_t reason,
                                uint8_t *bssid,
                                uint8_t assocRespLen,
                                uint8_t *assocInfo,
                                uint16_t protocolReasonStatus);
void Custom_Api_RSNASuccessEvent(void *pCxt, uint8_t devid, uint8_t code);
void Custom_Api_ReadyEvent(void *pCxt, uint8_t *datap, uint8_t phyCap, uint32_t sw_ver, uint32_t abi_ver);
void Custom_Api_BitRateEvent_tx(void *pCxt, uint8_t devId, int8_t rateIndex);
void Custom_Api_ProbeReq_Event(void *pCxt, uint8_t *datap, uint32_t len);
void Custom_Api_Wps_Init_Event(void *pCxt, uint8_t devid, uint8_t code);

A_STATUS Custom_HW_Init(void *pCxt);
A_STATUS Custom_HW_DeInit(void *pCxt);

void *Custom_Api_GetDriverCxt(uint8_t device_id);

void Custom_Api_PfmDataEvent(void *wmip, uint8_t *datap, int32_t len);
void Custom_Api_PfmDataDoneEvent(void *wmip, uint8_t *datap, int32_t len);
void Custom_Api_Http_Post_Event(void *pCxt, uint8_t *datap);

#define CUSTOM_API_WMIX_PFM_DATA_EVENT(wmip, datap, len) Custom_Api_PfmDataEvent((wmip), (datap), (len))
#define CUSTOM_API_WMIX_PFM_DATA_DONE_EVENT(wmip, datap, len) Custom_Api_PfmDataDoneEvent((wmip), (datap), (len))

#if ENABLE_P2P_MODE
// void
// Custom_Api_wps_profile_event_rx(void *pCxt, uint8_t devId, uint8_t *datap, int32_t len, void *pReq);

void Api_GpioDataEvent(void *wmip, uint8_t *datap, int32_t len); // IGX_UD_CHANGES

#endif

/* COMMON -> CUSTOM APIs :: These macros are used by common code to call into custom code.
 * 	For a given system many of these will resolve to function calls however for some it
 * 	may be appropriate to simply define them as A_OK. */
#define CUSTOM_DELIVER_FRAME(pCxt, pReq) Custom_DeliverFrameToNetworkStack((pCxt), (pReq))

void Custom_Api_GpioDataEvent(void *wmip, uint8_t *datap, int32_t len);
#define CUSTOM_API_WMIX_GPIO_DATA_EVENT(wmip, datap, len) Custom_Api_GpioDataEvent((wmip), (datap), (len))

#define CUSTOM_API_BSS_INFO_EVENT(pCxt, datap, len) Custom_Api_BssInfoEvent((pCxt), (datap), (len))

#define CUSTOM_API_PROBEREQ_EVENT(pCxt, datap, len) Custom_Api_ProbeReq_Event((pCxt), (datap), (len))

#define CUSTOM_API_BITRATE_EVENT_TX(pCxt, devId, rateIndex) Custom_Api_BitRateEvent_tx((pCxt), (devId), (rateIndex))
#define CUSTOM_API_CONNECT_EVENT(pCxt, devId, channel, bssid, c, d, e, f, g, h, i) \
    Custom_Api_ConnectEvent((pCxt), (devId), (channel), (bssid))
#define CUSTOM_API_DISCONNECT_EVENT(pCxt, devId, a, b, c, d, e) \
    Custom_Api_DisconnectEvent((pCxt), (devId), (a), (b), (c), (d), (e))
#define CUSTOM_API_RSNA_SUCCESS_EVENT(pCxt, devId, code) Custom_Api_RSNASuccessEvent((pCxt), (devId), (code))

#define CUSTOM_API_READY_EVENT(pCxt, datap, phyCap, sw_ver, abi_ver) \
    Custom_Api_ReadyEvent((pCxt), (datap), (phyCap), (sw_ver), (abi_ver))

//#define CUSTOM_HW_POWER_UP_DOWN(pCxt, powerup) Custom_HW_PowerUpDown((pCxt), (powerup))
//#define CUSTOM_HW_INIT(pCxt) Custom_HW_Init((pCxt))
//#define CUSTOM_HW_DEINIT(pCxt) Custom_HW_DeInit((pCxt))

#define CUSTOM_DRIVER_CXT_INIT(pCxt) Custom_Driver_ContextInit((pCxt))
#define CUSTOM_DRIVER_CXT_DEINIT(pCxt) Custom_Driver_ContextDeInit((pCxt))
#define CUSTOM_DRIVER_GET_RX_REQ(pCxt, l) Custom_GetRxRequest((pCxt), (l))
#define CUSTOM_API_HTTP_POST_EVENT(pCxt, datap) Custom_Api_Http_Post_Event((pCxt), (datap))
void Custom_Api_Ota_Resp_Result(void *pCxt, uint32_t cmd, uint32_t resp_code, uint32_t result);
int32_t Custom_Api_Dhcps_Success_Callback_Event(void *pCxt, uint8_t *datap);
int32_t Custom_Api_Dhcpc_Success_Callback_Event(void *pCxt, uint8_t *datap);

#if ENABLE_P2P_MODE
//#define CUSTOM_API_WPS_PROFILE_EVENT_RX(pCxt, datap, len, pReq) Custom_Api_wps_profile_event_rx((pCxt), (datap),
//(len), (pReq))
//#define CUSTOM_API_P2P_SAVE_WPS_CRED(pCxt)    Custom_Api_p2p_save_wps_credentials((pCxt))
#endif

typedef void (*CUSTOM_HW_INTR_CB)(void *);

void Custom_HW_InterruptHandler(CUSTOM_HW_INTR_CB pHcdCallback, void *pHcdContext);

#endif /* _CUSTOM_API_H_ */
