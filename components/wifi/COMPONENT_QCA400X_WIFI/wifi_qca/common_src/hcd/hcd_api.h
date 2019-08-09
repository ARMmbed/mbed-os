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
#ifndef _HCD_API_H_
#define _HCD_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <htc_api.h>
#include <driver_cxt.h>

A_STATUS
Hcd_Request(void *pCxt, void *pReq);
boolean Hcd_ReadCPUInterrupt(void *pCxt, uint8_t *cpuIntrCause);
void Hcd_ClearCPUInterrupt(void *pCxt);
A_STATUS
Hcd_UnmaskInterrupts(void *pCxt, uint16_t Mask); // IGX_UD_CHANGES
A_STATUS
Hcd_UnmaskInterrupts(void *pCxt, uint16_t Mask);
A_STATUS
Hcd_GetLookAhead(void *pCxt);
boolean Hcd_BusInterrupt(void *pCxt);
A_STATUS
Hcd_ReinitTarget(void *pCxt);
void Hcd_Deinitialize(void *pCxt);
A_STATUS
Hcd_Init(void *pCxt);
void Hcd_EnableCPUInterrupt(void *pCxt);
A_STATUS
Hcd_DoPioExternalAccess(void *pCxt, void *pReq);
void Hcd_MaskInterrupts(void *pCxt, uint16_t Mask);
A_STATUS
Hcd_ReinitTarget(void *pCxt);
A_STATUS
Hcd_UnmaskInterrupts(void *pCxt, uint16_t Mask);
A_STATUS
Bus_InOutDescriptorSet(void *pCxt, void *pReq);

#if 0
/*
* The below 3-functions are need to implement and  
* HCD_SUSPEND_CB and HCD_RESUME_CB structures are not avialable with 
* current driver source
*/

void Hcd_AckInterrupt(void *pCxt, uint16_t Mask);
void Hcd_RegisterSuspendHandler(HCD_SUSPEND_CB *suspendCallback, void *pSuspContext);
void Hcd_RegisterResumeHandler(HCD_RESUME_CB *resumeCallback, void *pSuspContext);
void 
Hcd_RegisterInterruptHandler(HCD_INTR_CALLBACK_T *callback, void *pContext);
#endif //#if 0

#endif //_HCD_API_H_
