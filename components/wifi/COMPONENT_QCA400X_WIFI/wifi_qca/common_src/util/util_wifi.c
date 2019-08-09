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

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <common_api.h>
#include <driver_cxt.h>

A_ENDPOINT_T *Util_GetEndpoint(void *pCxt, uint16_t id)
{
    /* this translation allows multiple virtual endpoints to be mapped to
     * a common endpoint object. */
    if (id >= ENDPOINT_MANAGED_MAX)
    {
        id = ENDPOINT_MANAGED_MAX - 1;
    }
    return &(GET_DRIVER_COMMON(pCxt)->ep[id]);
}

/*
 * converts ieee channel number to frequency
 */
uint16_t Util_Ieee2freq(int32_t chan)
{
    if (chan == 14)
    {
        return (uint16_t)2484;
    }
    if (chan < 14)
    { /* 0-13 */
        return (uint16_t)(2407 + (chan * 5));
    }
    if (chan < 27)
    { /* 15-26 */
        return (uint16_t)(2512 + ((chan - 15) * 20));
    }
    return (uint16_t)(5000 + (chan * 5));
}

/*
 * Converts MHz frequency to IEEE channel number.
 */
uint32_t Util_Freq2ieee(uint16_t freq)
{
    if (freq == 2484)
        return (uint32_t)14;
    if (freq < 2484)
        return (uint32_t)((freq - 2407) / 5);
    if (freq < 5000)
        return (uint32_t)(15 + ((freq - 2512) / 20));
    return (uint32_t)((freq - 5000) / 5);
}

HTC_ENDPOINT_ID
Util_AC2EndpointID(void *pCxt, uint8_t ac)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    return pDCxt->ac2EpMapping[ac];
}

uint8_t Util_EndpointID2AC(void *pCxt, HTC_ENDPOINT_ID ep)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    return pDCxt->ep2AcMapping[ep];
}
