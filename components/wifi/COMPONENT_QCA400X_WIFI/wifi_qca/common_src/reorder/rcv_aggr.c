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
#include <aggr_recv_api.h>
#include "wmi.h"
#include "atheros_wifi_api.h"

#if WLAN_CONFIG_11N_AGGR_SUPPORT

#define NUM_OF_TIDS 8
#define AGGR_SZ_DEFAULT 8

#define IEEE80211_MAX_SEQ_NO 0xFFF

typedef struct
{
    uint32_t aggr_enabled_tid_mask; /* config value of aggregation size */
    uint16_t seq_last[NUM_OF_TIDS];
} AGGR_INFO;

static AGGR_INFO aggr_context;

void aggr_deinit(void *cntxt)
{
    UNUSED_ARGUMENT(cntxt);
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : aggr_init
*  Returned Value :
*  Comments       :
*  		Initializes aggregation data structures.
*
*END*-----------------------------------------------------------------*/
void *aggr_init(void)
{
    /* some implementations malloc a context but this version simply uses
     * a global context */

    aggr_reset_state((void *)&aggr_context);

    return &aggr_context;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : aggr_recv_addba_req_evt
*  Returned Value :
*  Comments       :
*  		Firmware has indicated that aggregation is enabled
*		for this TID.
*
*END*-----------------------------------------------------------------*/
void aggr_recv_addba_req_evt(void *cntxt, uint8_t tidaid, uint16_t seq_no, uint8_t win_sz)
{
    AGGR_INFO *p_aggr = (AGGR_INFO *)cntxt;
    uint8_t tid = tidaid & 0xF;
    A_ASSERT(p_aggr);
    A_ASSERT(tid < NUM_OF_TIDS);
    UNUSED_ARGUMENT(win_sz);
    p_aggr->aggr_enabled_tid_mask |= (1 << tid);
    p_aggr->seq_last[tid] = seq_no;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : aggr_recv_delba_req_evt
*  Returned Value :
*  Comments       :
*  		Firmware has indicated that aggregation is no longer enabled
*		for this TID.
*
*END*-----------------------------------------------------------------*/
void aggr_recv_delba_req_evt(void *cntxt, uint8_t tidaid)
{
    AGGR_INFO *p_aggr = (AGGR_INFO *)cntxt;
    uint8_t tid = tidaid & 0xF;
    A_ASSERT(p_aggr);
    A_ASSERT(tid < NUM_OF_TIDS);

    p_aggr->aggr_enabled_tid_mask &= ~(1 << tid);
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : aggr_reset_state
*  Returned Value :
*  Comments       :
*  		Called when it is deemed necessary to clear the aggregate
*  		hold Q state.  Examples include when a Connect event or
*  		disconnect event is received.
*
*END*-----------------------------------------------------------------*/
void aggr_reset_state(void *cntxt)
{
    AGGR_INFO *p_aggr = (AGGR_INFO *)cntxt;

    A_ASSERT(p_aggr);

    p_aggr->aggr_enabled_tid_mask = 0;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : aggr_process_recv_frm
*  Returned Value : true - forward frame it is in-order
*					false - drop frame as its out-of-order
*  Comments       :
*        Processes a received frame to determine whether it should be
*		 dropped.  Aggregation allows for frames to be sent out-of-order
*		 which some systems/tcp stacks fail to handle.  One option is
*		 to queue packets in the driver and re-order them so that the
*		 driver always delivers packets to the next layer in-order.
*		 That however requires significant buffer space and so this
* 		 implementation instead will drop any out-of-order packets.
*		 it does this by returning true for in-order packets and
*		 false for out-of-order packets.  If aggregation
*		 is not enabled then there is no need to monitor the tid
*		 and the return value will always be true.
*
*END*-----------------------------------------------------------------*/
boolean aggr_process_recv_frm(void *cntxt, uint8_t tid, uint16_t seq_no, boolean is_amsdu, void **osbuf)
{
    AGGR_INFO *p_aggr = (AGGR_INFO *)cntxt;
    boolean result = false;
    A_ASSERT(p_aggr);
    A_ASSERT(tid < NUM_OF_TIDS);
    UNUSED_ARGUMENT(osbuf);
    UNUSED_ARGUMENT(is_amsdu);

    do
    {
        if ((!p_aggr->aggr_enabled_tid_mask) & (1 << tid))
        {
            result = true;
            break;
        }

        if (p_aggr->seq_last[tid] > seq_no)
        {
            if (p_aggr->seq_last[tid] - seq_no <= (IEEE80211_MAX_SEQ_NO >> 1))
            {
                /* drop this out-of-order packet */
                break;
            }
        }
        else
        {
            if (seq_no - p_aggr->seq_last[tid] > (IEEE80211_MAX_SEQ_NO >> 1))
            {
                /* drop this out-of-order packet */
                break;
            }
        }
        /* adopt the new seq_no and allow the packet */
        result = true;
        p_aggr->seq_last[tid] = seq_no;
    } while (0);

    return result;
}
#endif
