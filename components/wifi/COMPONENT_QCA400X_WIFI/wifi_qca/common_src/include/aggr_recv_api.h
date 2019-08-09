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

#ifndef __AGGR_RECV_API_H__
#define __AGGR_RECV_API_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*RX_CALLBACK)(void *dev, void *osbuf);

typedef void (*ALLOC_NETBUFS)(A_NETBUF_QUEUE_T *q, uint16_t num);

/*
 * aggr_init:
 * Initialises the data structures, allocates data queues and
 * os buffers. Netbuf allocator is the input param, used by the
 * aggr module for allocation of NETBUFs from driver context.
 * These NETBUFs are used for AMSDU processing.
 * Returns the context for the aggr module.
 */
void *aggr_init(void);

/*
 * aggr_deinit:
 * Frees any allocated resources allocated by aggr_init.
 */
void aggr_deinit(void *cntxt);

/*
 * aggr_recv_addba_req_evt:
 * This event is to initiate/modify the receive side window.
 * Target will send WMI_ADDBA_REQ_EVENTID event to host - to setup
 * recv re-ordering queues. Target will negotiate ADDBA with peer,
 * and indicate via this event after succesfully completing the
 * negotiation. This happens in two situations:
 *  1. Initial setup of aggregation
 *  2. Renegotiation of current recv window.
 * Window size for re-ordering is limited by target buffer
 * space, which is reflected in win_sz.
 * (Re)Start the periodic timer to deliver long standing frames,
 * in hold_q to OS.
 */
void aggr_recv_addba_req_evt(void *cntxt, uint8_t tid, uint16_t seq_no, uint8_t win_sz);

/*
 * aggr_recv_delba_req_evt:
 * Target indicates deletion of a BA window for a tid via the
 * WMI_DELBA_EVENTID. Host would deliver all the frames in the
 * hold_q, reset tid config and disable the periodic timer, if
 * aggr is not enabled on any tid.
 */
void aggr_recv_delba_req_evt(void *cntxt, uint8_t tid);

/*
 * aggr_process_recv_frm:
 * Called only for data frames. When aggr is ON for a tid, the buffer
 * is always consumed, and osbuf would be NULL. For a non-aggr case,
 * osbuf is not modified.
 * AMSDU frames are consumed and are later freed. They are sliced and
 * diced to individual frames and dispatched to stack.
 * After consuming a osbuf(when aggr is ON), a previously registered
 * callback may be called to deliver frames in order.
 */
boolean aggr_process_recv_frm(void *cntxt, uint8_t tid, uint16_t seq_no, boolean is_amsdu, void **osbuf);

/*
 * aggr_module_destroy:
 * Frees up all the queues and frames in them. Releases the cntxt to OS.
 */
void aggr_module_destroy(void *cntxt);

/*
 * aggr_reset_state -- Called when it is deemed necessary to clear the aggregate
 *  hold Q state.  Examples include when a Connect event or disconnect event is
 *  received.
 */
void aggr_reset_state(void *cntxt);

#ifdef __cplusplus
}
#endif

#endif /*__AGGR_RECV_API_H__ */
