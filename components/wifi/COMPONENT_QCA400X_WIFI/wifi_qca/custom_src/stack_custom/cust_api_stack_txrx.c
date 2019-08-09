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
#include <common_api.h>
#include <custom_wlan_api.h>
#include <htc.h>
#include <atheros_wifi_api.h>
#include "cust_netbuf.h"

#if ENABLE_STACK_OFFLOAD
#include "atheros_stack_offload.h"
#include "common_stack_offload.h"
#include "custom_stack_offload.h"
#include "atheros_wifi.h"

uint32_t Custom_Api_Send(
    QCA_CONTEXT_STRUCT_PTR qca_ptr, PCB_PTR pcb_ptr, uint32_t size, uint32_t frags, uint32_t flags);

uint32_t Custom_Api_Send(QCA_CONTEXT_STRUCT_PTR qca_ptr, PCB_PTR pcb_ptr, uint32_t size, uint32_t frags, uint32_t flags)
{
    return A_OK;
}

uint32_t custom_sock_is_pending(ATH_SOCKET_CONTEXT *sock_context)
{
    uint32_t i, rtn;
    for (rtn = 0, i = 0; i < (SOCK_CMD_LAST / 32 + 1); i++)
    {
        rtn |= sock_context->sock_st_mask[i];
    }

    return rtn;
}

/*****************************************************************************/
/*  custom_receive_tcpip - Called by Custom_DeliverFrameToNetworkStack to
 *   deliver a data frame to the application thread. It unblocks a waiting
 *   application thread and queues the packet in the queue of the corresponding
 *   socket. If socket is not valid, the packet is dropped.
 *      void *pCxt - the driver context.
 *      void *pReq - the packet.
 *****************************************************************************/
A_STATUS custom_receive_tcpip(void *pCxt, void *pReq)
{
    A_NETBUF *a_netbuf_ptr = (A_NETBUF *)pReq;
    int32_t index = 0, handle = 0;
    uint8_t *data;

    SOCKET_CONTEXT_PTR pcustctxt = NULL;

    if (a_netbuf_ptr)
    {
        data = (uint8_t *)A_NETBUF_DATA(a_netbuf_ptr);

        /*Extract socket handle from custom header*/
        handle = A_CPU2LE32(*((uint32_t *)data));

        /*Get index*/
        index = find_socket_context(handle, true);
        if (index < 0 || index > MAX_SOCKETS_SUPPORTED)
        {
            last_driver_error = A_SOCKCXT_NOT_FOUND;
            A_NETBUF_FREE(pReq);
            return A_ERROR;
        }

        if (custom_sock_is_pending(ath_sock_context[index]) != 0)
        {
            /* socket is pending a socket event so it cannot
             * receive data packets */
            last_driver_error = A_SOCK_UNAVAILABLE;
            A_NETBUF_FREE(pReq);
            return A_ERROR;
        }

        UNBLOCK_SELECT(pCxt);
        pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);

        // if(!IS_SOCKET_BLOCKED(ath_sock_context[index]))
        /*To-Do- Remove this once drop ligic is implemented in firmware*/
        if (get_total_pkts_buffered() >= WLAN_CONFIG_NUM_PRE_ALLOC_RX_BUFFERS /*- 1*/)
        {
            QCADRV_PRINTF("%s dropping packets for %x\n", __FUNCTION__, handle);
            A_NETBUF_FREE(pReq);
        }
        else
        {
            A_NETBUF_ENQUEUE(&(pcustctxt->rxqueue), a_netbuf_ptr);
            UNBLOCK(ath_sock_context[index], RX_DIRECTION);
        }
    }
    return A_OK;
}

/*****************************************************************************/
/*  Custom_DeliverFrameToNetworkStack - Called by API_RxComplete to
 *   deliver a data frame to the network stack. This code will perform
 *   platform specific operations.
 *      void *pCxt - the driver context.
 *      void *pReq - the packet.
 *****************************************************************************/
void Custom_DeliverFrameToNetworkStack(void *pCxt, void *pReq)
{
    ATH_PROMISCUOUS_CB prom_cb = (ATH_PROMISCUOUS_CB)(GET_DRIVER_CXT(pCxt)->promiscuous_cb);

    if (GET_DRIVER_COMMON(pCxt)->promiscuous_mode)
    {
        prom_cb(pReq);
    }
    else
    {
        custom_receive_tcpip(pCxt, pReq);
    }
}

/*****************************************************************************/
/*  Custom_Api_Send - Entry point for MQX driver interface to send a packet.
 *	 This is specific to MQX. This function is NOT called from within the
 *	 driver.
 *      QCA_CONTEXT_STRUCT_PTR  qca_ptr - the MQX driver context.
 *		PCB_PTR              pcb_ptr - the MQX packet object.
 *		uint32_t              size - the length in bytes of pcb_ptr.
 *		uint32_t              frags - the number of fragments in pcb_ptr.
 *		uint32_t              flags - optional flags for transmit.
 *****************************************************************************/
A_STATUS custom_send_tcpip(void *pCxt,
                           DRV_BUFFER_PTR db_ptr,
                           /* [IN] the packet to send */
                           uint32_t size,
                           /* [IN] total size of the packet */
                           uint32_t frags,
                           /* [IN] total fragments in the packet */
                           uint8_t *header,
                           uint32_t header_size

                           )
{
    A_STATUS error = A_OK;
    A_NETBUF *a_netbuf_ptr;
    uint32_t total_hdr_size = header_size;
    uint8_t *payloadPtr = db_ptr->bufFragment[0].payload;

    /* create an atheros pcb and continue or fail. */
    do
    {
        /*Get the netbuf pointer from TX packet*/
        a_netbuf_ptr = ((TX_PACKET_PTR)(payloadPtr - TX_PKT_OVERHEAD))->a_netbuf_ptr;

        a_netbuf_ptr->num_frags = (uint8_t)frags;

        A_NETBUF_PUT_DATA(a_netbuf_ptr, (uint8_t *)header, total_hdr_size);
        // ensure there is enough headroom to complete the tx operation
        if (A_NETBUF_HEADROOM(a_netbuf_ptr) < (int32_t)(sizeof(ATH_MAC_HDR) + sizeof(ATH_LLC_SNAP_HDR) +
                                                        sizeof(WMI_DATA_HDR) + HTC_HDR_LENGTH + WMI_MAX_TX_META_SZ))
        {
            error = A_NO_MEMORY;
            A_NETBUF_FREE(a_netbuf_ptr);
            break;
        }
        // carry the original around until completion.
        // it is freed by A_NETBUF_FREE
        a_netbuf_ptr->native_orig = db_ptr;

        if (A_OK != Api_DataTxStart(pCxt, (void *)a_netbuf_ptr))
        {
            error = A_ERROR;
            break;
        }
    } while (0);

    if (error != A_OK)
    {
        /* in the event of api failure, the original pcb should be returned to the caller un-touched
         * and the a_netbuf_ptr should be freed */
        if (error != A_NO_MEMORY)
            A_NETBUF_FREE((void *)a_netbuf_ptr);
    }
    return error;
}

#endif
