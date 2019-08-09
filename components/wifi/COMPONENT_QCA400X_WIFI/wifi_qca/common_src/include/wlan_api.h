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
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#ifndef _HOST_WLAN_API_H_
#define _HOST_WLAN_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <a_osapi.h>
#include "platform/mbed_wait_api.h"

#define DRIVER_INSERT(pCxt) A_OK
#define DRIVER_REMOVE(pCxt)

A_STATUS Driver_WaitForCondition(void *pCxt, volatile boolean *pCond, boolean value, uint32_t msec);
#define DRIVER_WAIT_FOR_CONDITION(pCxt, pC, v, ms) Driver_WaitForCondition((pCxt), (pC), (v), (ms))
#define WAIT_FOR_WMI_RESPONSE(pCxt)

#define API_TXCOMPLETE(pCxt, pReq) \
    UNUSED_ARGUMENT((pCxt));       \
    UNUSED_ARGUMENT((pReq));

#define API_TARGET_STATS_EVENT(pCxt, ptr, len) Driver_WakeUser((pCxt))

#define API_REGDOMAIN_EVENT(pCxt, regCode)

#define API_BITRATE_EVENT(pCxt, rateKbps) Driver_WakeUser((pCxt))
#define API_CHANNEL_LIST_EVENT(pCxt, numChan, chanList) Driver_WakeUser((pCxt))
#define API_SCAN_COMPLETE_EVENT(pCxt, status) Driver_WakeUser((pCxt))
#define API_GET_PMK_EVENT(pCxt, datap) Driver_WakeUser((pCxt))
#define API_STORE_RECALL_EVENT(pCxt) Driver_WakeUser((pCxt))
#define API_WPS_PROFILE_EVENT(pCxt, datap, len, pReq) Driver_WakeUser((pCxt))
#define API_GET_TEMPERATURE_REPLY(pCxt, datap, len) Driver_WakeUser((pCxt))
#define API_SET_PARAM_REPLY_EVENT(pCxt) Driver_WakeUser((pCxt))
#define API_GET_COUNTRY_CODE_REPLY(pCxt, datap, len) Driver_WakeUser((pCxt))
#define API_WPS_INIT_REPLY(pCxt, datap, len) Driver_WakeUser((pCxt))

#define API_TKIP_MIC_ERROR_EVENT(pCxt, keyid, ismcast)
#define API_AGGR_RECV_ADDBA_REQ_EVENT(pCxt, evt)
#define API_AGGR_RECV_DELBA_REQ_EVENT(pCxt, evt)
#define API_WMIX_DBGLOG_EVENT(wmip, datap, len)

// The best resolution we have is 1 millisecond so there is no microsecond delay.
// To avoid OSA_TimeDelay() delaying for 0ms, we add 1 millisecond
/* TODO: move to env layer !!*/
#define HW_USEC_DELAY(pCxt, usec) wait_us(usec)

void HW_SetClock(void *pCxt, uint32_t *pClockRate);
#define HW_SET_CLOCK(pCxt, pClockRate) HW_SetClock((pCxt), (pClockRate))

#define A_WMI_EVENT(evt, pCxt, datap, len, osbuf) A_ERROR

uint16_t wlan_ieee2freq(int chan);
uint32_t wlan_freq2ieee(uint16_t freq);

#ifdef __cplusplus
}
#endif

#endif /* _HOST_WLAN_API_H_ */
