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

#ifndef _NETBUF_H_
#define _NETBUF_H_

#include <a_types.h>
#include <a_config.h>
#include <athdefs.h>

/*
 * Network buffer queue support
 */
typedef struct
{
    void *head;
    void *tail;
    uint16_t count;
} A_NETBUF_QUEUE_T;

typedef void (*A_TRANS_CB)(void *, void *);

/* A_TRANSPORT_OBJ - the structure used to encapsulate any/all bus requests */
typedef struct _transport_obj
{
    uint32_t lookahead;      /* expected lookahead value for sanity checking */
    uint16_t transferLength; /* total transfer length including any pad bytes */
    uint16_t address;        /* register address from which the transfer will begin */
    uint16_t cmd;            /* bit field containing command values */
    uint8_t credits;         /* num credits consumed by this request (TX only) */
    uint8_t epid;            /* endpoint ID */
    A_STATUS status;         /* transport status */
    A_TRANS_CB cb;           /* callback to be used upon completion <cannot be NULL> */
} A_TRANSPORT_OBJ;

#define A_NETBUF_QUEUE_INIT(q) a_netbuf_queue_init(q)

#define A_NETBUF_ENQUEUE(q, pReq) a_netbuf_enqueue((q), (pReq))
#define A_NETBUF_PREQUEUE(q, pReq) a_netbuf_prequeue((q), (pReq))
#define A_NETBUF_DEQUEUE(q) (a_netbuf_dequeue((q)))
#define A_NETBUF_PEEK_QUEUE(q) ((A_NETBUF_QUEUE_T *)(q))->head
#define A_NETBUF_QUEUE_SIZE(q) ((A_NETBUF_QUEUE_T *)(q))->count
#define A_NETBUF_QUEUE_EMPTY(q) (!((A_NETBUF_QUEUE_T *)(q))->count)

/*
 *   receiver buffer
 */

#define A_RXBUF_ENQUEUE(q, pReq) a_rxbuf_enqueue((q), (pReq))
#define A_RXBUF_DEQUEUE(q) (a_rxbuf_dequeue((q)))

void a_netbuf_enqueue(A_NETBUF_QUEUE_T *q, void *pReq);
void a_netbuf_prequeue(A_NETBUF_QUEUE_T *q, void *pReq);
void *a_netbuf_dequeue(A_NETBUF_QUEUE_T *q);

void a_rxbuf_enqueue(A_NETBUF_QUEUE_T *q, void *pReq);
void *a_rxbuf_dequeue(A_NETBUF_QUEUE_T *q);

// int32_t a_netbuf_queue_size(A_NETBUF_QUEUE_T *q);
// int32_t a_netbuf_queue_empty(A_NETBUF_QUEUE_T *q);
void a_netbuf_queue_init(A_NETBUF_QUEUE_T *q);
// void *a_netbuf_peek_queue(A_NETBUF_QUEUE_T *q);

#define MAX_NUM_FREE_QUEUES 8
#define AR6000_DATA_OFFSET 64

typedef struct mgmt_credits
{
    uint16_t rxMaxBufferSizeInQueues;
    uint8_t rxMaxBuffersInQueues;
    uint8_t rxQueueMask;
    uint8_t credit_count;
    uint8_t init_credits;
} MGMT_CREDITS;

extern uint8_t send_reverse_credits_flag;
extern MGMT_CREDITS host_credits[MAX_NUM_FREE_QUEUES];

extern A_NETBUF_QUEUE_T rxFreeQueues[MAX_NUM_FREE_QUEUES];

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)
extern uint8_t credits_test;
#endif

// extern uint16_t    rxMaxBuffersInQueues[MAX_NUM_FREE_QUEUES];
// extern uint16_t    rxMaxBufferSizeInQueues[MAX_NUM_FREE_QUEUES];
// extern uint8_t     rxQueueMask[MAX_NUM_FREE_QUEUES];

#define GET_NUM_CREDIT_QUEUES() (MAX_NUM_FREE_QUEUES)

#define GET_MAX_BUFFERS_IN_QUEUE(i) (MAX_BUFFERS_IN_QUEUE_##i)

#define GET_INIT_CREDITS_IN_QUEUES(a)                \
    a[0].init_credits = GET_MAX_BUFFERS_IN_QUEUE(0); \
    a[1].init_credits = GET_MAX_BUFFERS_IN_QUEUE(1); \
    a[2].init_credits = GET_MAX_BUFFERS_IN_QUEUE(2); \
    a[3].init_credits = GET_MAX_BUFFERS_IN_QUEUE(3); \
    a[4].init_credits = GET_MAX_BUFFERS_IN_QUEUE(4); \
    a[5].init_credits = GET_MAX_BUFFERS_IN_QUEUE(5); \
    a[6].init_credits = GET_MAX_BUFFERS_IN_QUEUE(6); \
    a[7].init_credits = GET_MAX_BUFFERS_IN_QUEUE(7);

#define GET_MAX_BUFFERS_IN_QUEUES(a)                         \
    a[0].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(0); \
    a[1].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(1); \
    a[2].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(2); \
    a[3].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(3); \
    a[4].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(4); \
    a[5].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(5); \
    a[6].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(6); \
    a[7].rxMaxBuffersInQueues = GET_MAX_BUFFERS_IN_QUEUE(7);

#define GET_BUFFER_SIZE_IN_QUEUE(i) (BUFFER_SIZE_IN_QUEUE_##i)

#define GET_BUFFER_SIZE_IN_QUEUES(a)                            \
    a[0].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(0); \
    a[1].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(1); \
    a[2].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(2); \
    a[3].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(3); \
    a[4].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(4); \
    a[5].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(5); \
    a[6].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(6); \
    a[7].rxMaxBufferSizeInQueues = GET_BUFFER_SIZE_IN_QUEUE(7);

#define QUEUE_MASK(i) (QUEUE_MASK_##i)

#define GET_MASK_ALL_QUEUES(a)        \
    a[0].rxQueueMask = QUEUE_MASK(0); \
    a[1].rxQueueMask = QUEUE_MASK(1); \
    a[2].rxQueueMask = QUEUE_MASK(2); \
    a[3].rxQueueMask = QUEUE_MASK(3); \
    a[4].rxQueueMask = QUEUE_MASK(4); \
    a[5].rxQueueMask = QUEUE_MASK(5); \
    a[6].rxQueueMask = QUEUE_MASK(6); \
    a[7].rxQueueMask = QUEUE_MASK(7);

#define GET_FREE_QUEUE(i) (rxFreeQueues[i])

#define GET_QUEUE_BUFFER_SIZE(i) (host_credits[i].rxMaxBufferSizeInQueues + AR6000_DATA_OFFSET)
#define GET_QUEUE_DATA_BUFFER_SIZE(i) (host_credits[i].rxMaxBufferSizeInQueues)
#define GET_MAX_NUM_BUFFERS(i) (host_credits[i].rxMaxBuffersInQueues)
#define GET_QUEUE_MASK(i) (host_credits[i].rxQueueMask)
#define CREDIT_INC(i)                       \
    {                                       \
        if (credits_test == 0)              \
            host_credits[i].credit_count++; \
    }
#define CREDIT_ADD(i, c)                   \
    {                                      \
        host_credits[i].credit_count += c; \
    }
#define CREDIT_DEC(i) (host_credits[i].credit_count--)
#define GET_CREDIT(i) (host_credits[i].credit_count)
#define GET_INIT_CREDITS(i) (host_credits[i].init_credits)
#define CLEAR_CREDIT(i) (host_credits[i].credit_count = 0)

#endif /* _NETBUF_H_ */
