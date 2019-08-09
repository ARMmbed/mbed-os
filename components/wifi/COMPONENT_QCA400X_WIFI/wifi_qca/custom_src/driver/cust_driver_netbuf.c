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

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <netbuf.h>
#include "atheros_wifi_api.h"

#define AR6000_DATA_OFFSET 64

uint8_t g_driverState = DRIVER_STATE_INVALID;

#if (!ENABLE_STACK_OFFLOAD)

void default_native_free_fn(PCB_PTR pcb_ptr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)pcb_ptr;
    /* head is guaranteed to always point to the
     * start of the original buffer */
    if (a_netbuf_ptr->head)
        A_FREE(a_netbuf_ptr->head, MALLOC_ID_NETBUFF);

    if (a_netbuf_ptr->native_orig)
        a_netbuf_ptr->native_orig->FREE(a_netbuf_ptr->native_orig);

    A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
}

typedef void (*_pcb_free_fn)(PCB_PTR pcb_ptr);

void a_netbuf_init(void *buffptr, void *freefn, void *priv)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)buffptr;

    a_netbuf_ptr->native.FREE = (_pcb_free_fn)freefn;
    a_netbuf_ptr->native.PRIVATE = priv;
    a_netbuf_ptr->tail = a_netbuf_ptr->data = (pointer)((uint32_t)a_netbuf_ptr->head + AR6000_DATA_OFFSET);
    a_netbuf_ptr->native.FRAG[0].FRAGMENT = a_netbuf_ptr->head;
    a_netbuf_ptr->native.FRAG[0].LENGTH = (uint32_t)a_netbuf_ptr->end - (uint32_t)a_netbuf_ptr->head;
}

void *a_netbuf_alloc(int32_t size)
{
    A_NETBUF *a_netbuf_ptr;

    do
    {
        a_netbuf_ptr = A_MALLOC(sizeof(A_NETBUF), MALLOC_ID_NETBUFF_OBJ);

        if (a_netbuf_ptr == NULL)
        {
            break;
        }
        A_MEMZERO(a_netbuf_ptr, sizeof(A_NETBUF));

        a_netbuf_ptr->native.FREE = default_native_free_fn;

        a_netbuf_ptr->native.FRAG[0].LENGTH = AR6000_DATA_OFFSET + size;
        a_netbuf_ptr->native.FRAG[0].FRAGMENT =
            A_MALLOC((int32_t)(a_netbuf_ptr->native.FRAG[0].LENGTH), MALLOC_ID_NETBUFF);

        if (a_netbuf_ptr->native.FRAG[0].FRAGMENT == NULL)
        {
            A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
            a_netbuf_ptr = NULL;
            break;
        }

        a_netbuf_ptr->head = a_netbuf_ptr->native.FRAG[0].FRAGMENT;
        a_netbuf_ptr->end =
            (pointer)((uint32_t)a_netbuf_ptr->native.FRAG[0].FRAGMENT + a_netbuf_ptr->native.FRAG[0].LENGTH);
        // reserve head room
        a_netbuf_ptr->tail = a_netbuf_ptr->data = (pointer)((uint32_t)a_netbuf_ptr->head + AR6000_DATA_OFFSET);
        A_MEMZERO(&a_netbuf_ptr->trans, sizeof(A_TRANSPORT_OBJ));
    } while (0);

    return ((void *)a_netbuf_ptr);
}

/*
 * Allocate an NETBUF w.o. any encapsulation requirement.
 */
void *a_netbuf_alloc_raw(int32_t size)
{
    A_NETBUF *a_netbuf_ptr = NULL;

    do
    {
        a_netbuf_ptr = A_MALLOC(sizeof(A_NETBUF), MALLOC_ID_NETBUFF_OBJ);

        if (a_netbuf_ptr == NULL)
        {
            break;
        }

        A_MEMZERO(a_netbuf_ptr, sizeof(A_NETBUF));

        a_netbuf_ptr->native.FREE = default_native_free_fn;
        a_netbuf_ptr->native.FRAG[0].LENGTH = (uint32_t)size;
        a_netbuf_ptr->native.FRAG[0].FRAGMENT =
            A_MALLOC((int32_t)a_netbuf_ptr->native.FRAG[0].LENGTH, MALLOC_ID_NETBUFF);

        if (a_netbuf_ptr->native.FRAG[0].FRAGMENT == NULL)
        {
            A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
            a_netbuf_ptr = NULL;
            break;
        }

        a_netbuf_ptr->head = a_netbuf_ptr->tail = a_netbuf_ptr->data = a_netbuf_ptr->native.FRAG[0].FRAGMENT;
        a_netbuf_ptr->end =
            (pointer)((uint32_t)a_netbuf_ptr->native.FRAG[0].FRAGMENT + (uint32_t)a_netbuf_ptr->native.FRAG[0].LENGTH);
        A_MEMZERO(&a_netbuf_ptr->trans, sizeof(A_TRANSPORT_OBJ));
    } while (0);

    return ((void *)a_netbuf_ptr);
}

void a_netbuf_free(void *buffptr)
{
    ((A_NATIVE_ORIG *)buffptr)->FREE(((A_NATIVE_ORIG *)buffptr));
}

/* returns the length in bytes of the entire packet.  The packet may
 * be composed of multiple fragments or it may be just a single buffer.
 */
uint32_t a_netbuf_to_len(void *bufPtr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;
    uint32_t len = (uint32_t)a_netbuf_ptr->tail - (uint32_t)a_netbuf_ptr->data;
    uint16_t i = 0;

    if (a_netbuf_ptr->num_frags)
    {
        if (a_netbuf_ptr->native_orig != NULL)
        {
            /* count up fragments */
            for (i = 0; i < a_netbuf_ptr->num_frags; i++)
            {
                len += a_netbuf_ptr->native_orig->FRAG[i].LENGTH;
            }
        }
        else
        {
            /* used for special case native's from ATH_TX_RAW */
            for (i = 1; i <= a_netbuf_ptr->num_frags; i++)
            {
                len += a_netbuf_ptr->native.FRAG[i].LENGTH;
            }
        }
    }

    return len;
}

/* returns a buffer fragment of a packet.  If the packet is not
 * fragmented only index == 0 will return a buffer which will be
 * the whole packet. pLen will hold the length of the buffer in
 * bytes.
 */
void *a_netbuf_get_fragment(void *bufPtr, uint8_t index, int32_t *pLen)
{
    void *pBuf = NULL;
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    if (0 == index)
    {
        pBuf = a_netbuf_to_data(bufPtr);
        *pLen = (int32_t)((uint32_t)a_netbuf_ptr->tail - (uint32_t)a_netbuf_ptr->data);
    }
    else if (a_netbuf_ptr->num_frags >= index)
    {
        if (a_netbuf_ptr->native_orig)
        {
            /* additional fragments are held in native_orig. this is only
             * used for tx packets received from the TCP stack. */
            pBuf = a_netbuf_ptr->native_orig->FRAG[index - 1].FRAGMENT;
            *pLen = (int32_t)a_netbuf_ptr->native_orig->FRAG[index - 1].LENGTH;
        }
        else
        {
            /* used for special case native's from ATH_TX_RAW */
            pBuf = a_netbuf_ptr->native.FRAG[index].FRAGMENT;
            *pLen = (int32_t)a_netbuf_ptr->native.FRAG[index].LENGTH;
        }
    }

    return pBuf;
}

/* fills the spare bufFragment position for the net buffer */
void a_netbuf_append_fragment(void *bufPtr, uint8_t *frag, int32_t len)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;
    /* the first bufFragment is used to hold the built-in buffer. subsequent
     * entries if any can be used to append additional buffers. hence
     * deduct 1 from MAX_BUF_FRAGS */
    if (a_netbuf_ptr->num_frags < MAX_BUF_FRAGS - 1)
    {
        a_netbuf_ptr->num_frags++;
        a_netbuf_ptr->native.FRAG[a_netbuf_ptr->num_frags].FRAGMENT = frag;
        a_netbuf_ptr->native.FRAG[a_netbuf_ptr->num_frags].LENGTH = len;
    }
    else
    {
        return A_ERROR;
    }
}

#else
#include "common_stack_offload.h"
#include "custom_stack_offload.h"

void a_netbuf_set_tx_pool(void *buffptr);
void a_netbuf_set_rx_pool(void *buffptr);
void a_netbuf_free_tx_pool(void *buffptr);
void a_netbuf_free_rx_pool(void *buffptr);
void Driver_ReportRxBuffStatus(void *pCxt, boolean status);

void default_native_free_fn(void *pcb_ptr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)pcb_ptr;
    /* head is guaranteed to always point to the
     * start of the original buffer */
    if (a_netbuf_ptr->head)
    {
        A_FREE(a_netbuf_ptr->head, MALLOC_ID_NETBUFF);
    }
    if (a_netbuf_ptr->native_orig)
    {
        txpkt_free(a_netbuf_ptr->native_orig);
    }
    A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
}

void *a_netbuf_dequeue_adv(A_NETBUF_QUEUE_T *q, void *pkt)
{
    A_NETBUF *curr, *prev = NULL;

    if (q->head == NULL)
        return (void *)NULL;

    curr = (A_NETBUF *)q->head;

    while (curr != NULL)
    {
        if ((A_NETBUF *)curr->data == pkt)
        {
            /*Match found*/
            if (curr == (A_NETBUF *)q->head)
            {
                /*Match found at head*/
                q->head = curr->queueLink;
                break;
            }
            else
            {
                /*Match found at intermediate node*/
                prev->queueLink = curr->queueLink;
                if (q->tail == curr)
                {
                    /*Last node*/
                    q->tail = prev;
                }
                break;
            }
        }
        prev = curr;
        curr = curr->queueLink;
    }

    if (curr != NULL)
    {
        q->count--;
        curr->queueLink = NULL;
    }
    return (void *)curr;
}

void a_netbuf_purge_queue(uint32_t index)
{
    SOCKET_CONTEXT_PTR pcustctxt;
    A_NETBUF_QUEUE_T *q;
    A_NETBUF *temp;

    pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);

    q = &pcustctxt->rxqueue;

    while (q->count)
    {
        temp = q->head;
        q->head = A_GET_QUEUE_LINK(temp);
        A_CLEAR_QUEUE_LINK(temp);
        A_NETBUF_FREE(temp);
        q->count--;
    }
    q->head = q->tail = NULL;
}

void a_netbuf_init(void *buffptr, void *freefn, void *priv)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)buffptr;

    //	a_netbuf_ptr->native.FREE = (_pcb_free_fn)freefn;
    a_netbuf_ptr->native.context = priv;
    a_netbuf_ptr->tail = a_netbuf_ptr->data = (void *)((uint32_t)a_netbuf_ptr->head + AR6000_DATA_OFFSET);
    a_netbuf_ptr->native.bufFragment[0].payload = a_netbuf_ptr->head;
    a_netbuf_ptr->native.bufFragment[0].length = (uint32_t)a_netbuf_ptr->end - (uint32_t)a_netbuf_ptr->head;
#if DRIVER_CONFIG_IMPLEMENT_RX_FREE_QUEUE
    a_netbuf_ptr->rxPoolID = 1;
#endif
}

void *a_netbuf_alloc(int32_t size)
{
    A_NETBUF *a_netbuf_ptr;

    do
    {
        a_netbuf_ptr = A_MALLOC(sizeof(A_NETBUF), MALLOC_ID_NETBUFF_OBJ);

        if (a_netbuf_ptr == NULL)
        {
            break;
        }
        A_MEMZERO(a_netbuf_ptr, sizeof(A_NETBUF));

        a_netbuf_ptr->native.bufFragment[0].length = AR6000_DATA_OFFSET + size;
        a_netbuf_ptr->native.bufFragment[0].payload =
            A_MALLOC((int32_t)(a_netbuf_ptr->native.bufFragment[0].length), MALLOC_ID_NETBUFF);

        if (a_netbuf_ptr->native.bufFragment[0].payload == NULL)
        {
            A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
            a_netbuf_ptr = NULL;
            break;
        }

        a_netbuf_ptr->head = a_netbuf_ptr->native.bufFragment[0].payload;
        a_netbuf_ptr->end = (void *)((uint32_t)a_netbuf_ptr->native.bufFragment[0].payload +
                                     a_netbuf_ptr->native.bufFragment[0].length);
        // reserve head room
        a_netbuf_ptr->tail = a_netbuf_ptr->data = (void *)((uint32_t)a_netbuf_ptr->head + AR6000_DATA_OFFSET);
        A_MEMZERO(&a_netbuf_ptr->trans, sizeof(A_TRANSPORT_OBJ));
        /*Init pool IDs*/
        a_netbuf_ptr->txPoolID = 0;
        a_netbuf_ptr->rxPoolID = 0;
    } while (0);

    return ((void *)a_netbuf_ptr);
}

/*****************************************************************************/
/*  a_netbuf_reinit - called from custom free after TX over SPI is completed.
            It is only called in "Blocking TX" mode where we reuse the same
            netbuf.
            Tail, data pointers within netbuf may have moved during previous TX,
            reinit the netbuf so it can be used again. No allocation is done here.

 * RETURNS: reinitialized netbuf pointer for success and NULL for failure
 *****************************************************************************/
void *a_netbuf_reinit(void *netbuf_ptr, int32_t size)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)(netbuf_ptr);

    do
    {
        a_netbuf_ptr->native.bufFragment[0].length = AR6000_DATA_OFFSET + size;

        a_netbuf_ptr->head = a_netbuf_ptr->native.bufFragment[0].payload;
        a_netbuf_ptr->end = (void *)((uint32_t)a_netbuf_ptr->native.bufFragment[0].payload +
                                     a_netbuf_ptr->native.bufFragment[0].length);
        // reserve head room
        a_netbuf_ptr->tail = a_netbuf_ptr->data = (void *)((uint32_t)a_netbuf_ptr->head + AR6000_DATA_OFFSET);
        A_MEMZERO(&a_netbuf_ptr->trans, sizeof(A_TRANSPORT_OBJ));
        /*Init pool IDs*/
        a_netbuf_ptr->txPoolID = 0;
        a_netbuf_ptr->rxPoolID = 0;
    } while (0);

    return ((void *)a_netbuf_ptr);
}

void a_netbuf_set_rx_pool(void *buffptr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)buffptr;
    a_netbuf_ptr->rxPoolID = 1;
}

void a_netbuf_set_tx_pool(void *buffptr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)buffptr;
    a_netbuf_ptr->txPoolID = 1;
}

/*
 * Allocate an NETBUF w.o. any encapsulation requirement.
 */
void *a_netbuf_alloc_raw(int32_t size)
{
    A_NETBUF *a_netbuf_ptr = NULL;

    do
    {
        a_netbuf_ptr = A_MALLOC(sizeof(A_NETBUF), MALLOC_ID_NETBUFF_OBJ);

        if (a_netbuf_ptr == NULL)
        {
            break;
        }

        A_MEMZERO(a_netbuf_ptr, sizeof(A_NETBUF));

        // a_netbuf_ptr->native.FREE = default_native_free_fn;
        a_netbuf_ptr->native.bufFragment[0].length = (uint32_t)size;
        a_netbuf_ptr->native.bufFragment[0].payload =
            A_MALLOC((int32_t)a_netbuf_ptr->native.bufFragment[0].length, MALLOC_ID_NETBUFF);

        if (a_netbuf_ptr->native.bufFragment[0].payload == NULL)
        {
            A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
            a_netbuf_ptr = NULL;
            break;
        }

        a_netbuf_ptr->head = a_netbuf_ptr->tail = a_netbuf_ptr->data = a_netbuf_ptr->native.bufFragment[0].payload;
        a_netbuf_ptr->end = (void *)((uint32_t)a_netbuf_ptr->native.bufFragment[0].payload +
                                     (uint32_t)a_netbuf_ptr->native.bufFragment[0].length);
        A_MEMZERO(&a_netbuf_ptr->trans, sizeof(A_TRANSPORT_OBJ));
    } while (0);

    return ((void *)a_netbuf_ptr);
}

void a_netbuf_free(void *buffptr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)buffptr;
    if (a_netbuf_ptr->rxPoolID != 0)
    {
        a_netbuf_free_rx_pool(buffptr);
        return;
    }
    else if (a_netbuf_ptr->txPoolID != 0)
    {
        a_netbuf_free_tx_pool(buffptr);
        return;
    }
    else
    {
        if (a_netbuf_ptr->native_orig)
        {
            /*This was a TX packet, do not free netbuf here as it may be reused*/
            txpkt_free(a_netbuf_ptr);
        }
        else
        {
            /*For all other packets, free the netbuf*/
            if (a_netbuf_ptr->head)
            {
                A_FREE(a_netbuf_ptr->head, MALLOC_ID_NETBUFF);
            }

            A_FREE(a_netbuf_ptr, MALLOC_ID_NETBUFF_OBJ);
        }
    }
}

void a_netbuf_free_rx_pool(void *buffptr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)buffptr;
    A_CUSTOM_DRIVER_CONTEXT *pCxt = (A_CUSTOM_DRIVER_CONTEXT *)a_netbuf_ptr->native.context;

    RXBUFFER_ACCESS_ACQUIRE((void *)pCxt);
// A_NETBUF_ENQUEUE(&(pCxt->rxFreeQueue), a_netbuf_ptr);
#if DRIVER_CONFIG_IMPLEMENT_RX_FREE_QUEUE
    A_NETBUF_ENQUEUE(&(GET_DRIVER_COMMON(pCxt)->rxFreeQueue), a_netbuf_ptr);
#endif
    Driver_ReportRxBuffStatus((void *)pCxt, true);
    RXBUFFER_ACCESS_RELEASE((void *)pCxt);
}

void a_netbuf_free_tx_pool(void *buffptr)
{
}
/* returns the length in bytes of the entire packet.  The packet may
 * be composed of multiple fragments or it may be just a single buffer.
 */
uint32_t a_netbuf_to_len(void *bufPtr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;
    uint32_t len = (uint32_t)a_netbuf_ptr->tail - (uint32_t)a_netbuf_ptr->data;
    uint16_t i = 0;

    if (a_netbuf_ptr->num_frags)
    {
        if (a_netbuf_ptr->native_orig != NULL)
        {
            /* count up fragments */
            for (i = 0; i < a_netbuf_ptr->num_frags; i++)
            {
                len += a_netbuf_ptr->native_orig->bufFragment[i].length;
            }
        }
        else
        {
            /* used for special case native's from ATH_TX_RAW */
            for (i = 1; i <= a_netbuf_ptr->num_frags; i++)
            {
                len += a_netbuf_ptr->native.bufFragment[i].length;
            }
        }
    }

    return len;
}

/* returns a buffer fragment of a packet.  If the packet is not
 * fragmented only index == 0 will return a buffer which will be
 * the whole packet. pLen will hold the length of the buffer in
 * bytes.
 */
void *a_netbuf_get_fragment(void *bufPtr, uint8_t index, int32_t *pLen)
{
    void *pBuf = NULL;
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    if (0 == index)
    {
        pBuf = a_netbuf_to_data(bufPtr);
        *pLen = (int32_t)((uint32_t)a_netbuf_ptr->tail - (uint32_t)a_netbuf_ptr->data);
    }
    else if (a_netbuf_ptr->num_frags >= index)
    {
        if (a_netbuf_ptr->native_orig)
        {
            /* additional fragments are held in native_orig. this is only
             * used for tx packets received from the TCP stack. */
            pBuf = a_netbuf_ptr->native_orig->bufFragment[index - 1].payload;
            *pLen = (int32_t)a_netbuf_ptr->native_orig->bufFragment[index - 1].length;
        }
        else
        {
            /* used for special case native's from ATH_TX_RAW */
            pBuf = a_netbuf_ptr->native.bufFragment[index].payload;
            *pLen = (int32_t)a_netbuf_ptr->native.bufFragment[index].length;
        }
    }

    return pBuf;
}

/* fills the spare bufFragment position for the net buffer */
int a_netbuf_append_fragment(void *bufPtr, uint8_t *frag, int32_t len)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;
    /* the first bufFragment is used to hold the built-in buffer. subsequent
     * entries if any can be used to append additional buffers. hence
     * deduct 1 from MAX_BUF_FRAGS */
    if (a_netbuf_ptr->num_frags < MAX_BUF_FRAGS - 1)
    {
        a_netbuf_ptr->num_frags++;
        a_netbuf_ptr->native.bufFragment[a_netbuf_ptr->num_frags].payload = frag;
        a_netbuf_ptr->native.bufFragment[a_netbuf_ptr->num_frags].length = len;
    }
    else
    {
        return A_ERROR;
    }

    return A_OK;
}

#endif /*!ENABLE_STACK_OFFLOAD*/

void a_netbuf_enqueue(A_NETBUF_QUEUE_T *q, void *pReq)
{
    //    uint8_t   i;

    OSA_EnterCritical(kCriticalDisableInt);
    if (q->head == NULL)
    {
        q->head = pReq;
    }
    else
    {
        A_ASSIGN_QUEUE_LINK(q->tail, pReq);
        //((A_NETBUF*)q->tail)->queueLink = (A_NETBUF*)pReq;
    }

    q->tail = pReq;
    A_CLEAR_QUEUE_LINK(pReq);
    //((A_NETBUF*)pkt)->queueLink = NULL;
    q->count++;
    OSA_ExitCritical(kCriticalDisableInt);
}

void a_netbuf_prequeue(A_NETBUF_QUEUE_T *q, void *pReq)
{
    if (q->head == NULL)
    {
        q->tail = pReq;
    }
    A_ASSIGN_QUEUE_LINK(pReq, q->head);
    //((A_NETBUF*)pkt)->queueLink = q->head;
    q->head = pReq;
    q->count++;
}

void *a_netbuf_dequeue(A_NETBUF_QUEUE_T *q)
{
    void *pReq;

    if (q->head == NULL)
        return (void *)NULL;
    OSA_EnterCritical(kCriticalDisableInt);
    pReq = q->head;

    if (q->tail == q->head)
    {
        q->tail = q->head = NULL;
    }
    else
    {
        q->head = A_GET_QUEUE_LINK(pReq);
        // q->head = (void*)(curr->queueLink);
    }

    q->count--;
    A_CLEAR_QUEUE_LINK(pReq);
    // curr->queueLink = NULL;
    OSA_ExitCritical(kCriticalDisableInt);
    return (void *)pReq;
}

void a_netbuf_queue_init(A_NETBUF_QUEUE_T *q)
{
    q->head = q->tail = NULL;
    q->count = 0;
}

void a_netbuf_configure(void *buffptr, void *buffer, uint16_t headroom, uint16_t length, uint16_t size)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)buffptr;

    A_MEMZERO(a_netbuf_ptr, sizeof(A_NETBUF));

    if (buffer != NULL)
    {
        a_netbuf_ptr->head = buffer;
        a_netbuf_ptr->data = &(((uint8_t *)buffer)[headroom]);
        a_netbuf_ptr->tail = &(((uint8_t *)buffer)[headroom + length]);
        a_netbuf_ptr->end = &(((uint8_t *)buffer)[size]);
    }
}

void *a_netbuf_to_data(void *bufPtr)
{
    return (((A_NETBUF *)bufPtr)->data);
}

/*
 * Add len # of bytes to the beginning of the network buffer
 * pointed to by bufPtr
 */
A_STATUS
a_netbuf_push(void *bufPtr, int32_t len)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    if ((uint32_t)a_netbuf_ptr->data - (uint32_t)a_netbuf_ptr->head < (uint32_t)len)
    {
        return A_ERROR;
    }

    a_netbuf_ptr->data = (void *)(((uint32_t)a_netbuf_ptr->data) - len);

    return A_OK;
}

/*
 * Add len # of bytes to the beginning of the network buffer
 * pointed to by bufPtr and also fill with data
 */
A_STATUS
a_netbuf_push_data(void *bufPtr, uint8_t *srcPtr, int32_t len)
{
    a_netbuf_push(bufPtr, len);
    A_MEMCPY(((A_NETBUF *)bufPtr)->data, srcPtr, (uint32_t)len);

    return A_OK;
}

/*
 * Add len # of bytes to the end of the network buffer
 * pointed to by bufPtr
 */
A_STATUS
a_netbuf_put(void *bufPtr, int32_t len)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    if ((uint32_t)a_netbuf_ptr->end - (uint32_t)a_netbuf_ptr->tail < (uint32_t)len)
    {
        return A_ERROR;
    }

    a_netbuf_ptr->tail = (void *)(((uint32_t)a_netbuf_ptr->tail) + len);

    return A_OK;
}

/*
 * Add len # of bytes to the end of the network buffer
 * pointed to by bufPtr and also fill with data
 */
A_STATUS
a_netbuf_put_data(void *bufPtr, uint8_t *srcPtr, int32_t len)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;
    void *start = a_netbuf_ptr->tail;

    a_netbuf_put(bufPtr, len);
    A_MEMCPY(start, srcPtr, (uint32_t)len);

    return A_OK;
}

/*
 * Returns the number of bytes available to a a_netbuf_push()
 */
int32_t a_netbuf_headroom(void *bufPtr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    return (int32_t)((uint32_t)a_netbuf_ptr->data - (uint32_t)a_netbuf_ptr->head);
}

int32_t a_netbuf_tailroom(void *bufPtr)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    return (int32_t)((uint32_t)a_netbuf_ptr->end - (uint32_t)a_netbuf_ptr->tail);
}

/*
 * Removes specified number of bytes from the beginning of the buffer
 */
A_STATUS
a_netbuf_pull(void *bufPtr, int32_t len)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    if ((uint32_t)a_netbuf_ptr->tail - (uint32_t)a_netbuf_ptr->data < (uint32_t)len)
    {
        return A_ERROR;
    }

    a_netbuf_ptr->data = (void *)((uint32_t)a_netbuf_ptr->data + len);

    return A_OK;
}

/*
 * Removes specified number of bytes from the end of the buffer
 */
A_STATUS
a_netbuf_trim(void *bufPtr, int32_t len)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)bufPtr;

    if ((uint32_t)a_netbuf_ptr->tail - (uint32_t)a_netbuf_ptr->data < (uint32_t)len)
    {
        return A_ERROR;
    }

    a_netbuf_ptr->tail = (void *)((uint32_t)a_netbuf_ptr->tail - len);

    return A_OK;
}
