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
#include "a_config.h"

// This conditional encapsulates the entire file
#if ENABLE_STACK_OFFLOAD

#include "wifi_common.h"
#include <a_types.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include "wmi_api.h"
#include "a_drv_api.h"
#include "cust_netbuf.h"
#include <wlan_api.h>
#include "atheros_wifi_api.h"

#include "atheros_stack_offload.h"
#include "common_stack_offload.h"
#include "custom_stack_offload.h"

#define MAX_CERT_DATA_LENGTH 1400
#define SSL_MAX_CERT_NAME_LEN 31
extern const uint8_t max_performance_power_param;
ATH_SOCKET_CONTEXT *ath_sock_context[MAX_SOCKETS_SUPPORTED + 1];

/*****************************************************************************/
/*  custom_queue_empty - Checkes whether a socket queue is empty
 * RETURNS: 1 - empty or 0 - not empty
 *****************************************************************************/
uint32_t queue_empty(uint32_t index)
{
    SOCKET_CONTEXT_PTR pcustctxt;
    pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);

    return A_NETBUF_QUEUE_EMPTY(&(pcustctxt->rxqueue));
}

#if ENABLE_SSL
void *sslhandle = NULL; // Driver context ptr needed by the SSL_xxx() API functions

int32_t find_socket_context_from_ssl(SSL *ssl)
{
    int32_t index = 0;

    for (index = 0; index < MAX_SOCKETS_SUPPORTED; index++)
    {
        if (ath_sock_context[index]->ssl == ssl)
            return index;
    }
    /*Index not found*/
    return SOCKET_NOT_FOUND;
}
#endif

/*****************************************************************************/
/*  find_socket_context - Finds socket context based on socket handle. "retrieve"
 *                flag is used to identify if a new context is requested or an
 *                exiting context is retrieved.
 *      uint32_t handle - socket handle.
 * 		uint8_t retrieve - see description
 * Returns- index in context array
 *****************************************************************************/
int32_t find_socket_context(uint32_t handle, uint8_t retrieve)
{
    int32_t index = 0;
    if (retrieve)
    {
        if (handle == 0)
            return SOCKET_NOT_FOUND;
    }

    /*If handle is 0, we want to find first empty context and return the index.
    If handle is non zero, return the index that matches */
    for (index = 0; index < MAX_SOCKETS_SUPPORTED; index++)
    {
        if ((uint32_t)ath_sock_context[index]->handle == handle)
            return index;
    }
    /*Index not found*/
    return SOCKET_NOT_FOUND;
}

/*****************************************************************************/
/*  socket_context_init - Initializes common socket context, called during
 *                 driver init.
 *
 * Returns- A_OK in case of successful init, A_ERROR otherwise
 *****************************************************************************/
A_STATUS socket_context_init(void)
{
    SOCKET_CONTEXT_PTR pcustctxt = NULL;
    uint32_t index = 0;

    for (index = 0; index < MAX_SOCKETS_SUPPORTED + 1; index++)
    {
        /*Allocate socket contexts*/
        if ((ath_sock_context[index] = A_MALLOC(sizeof(ATH_SOCKET_CONTEXT), MALLOC_ID_CONTEXT)) == NULL)
            return A_NO_MEMORY;

        memset(ath_sock_context[index], 0, sizeof(ATH_SOCKET_CONTEXT));
    }

    for (index = 0; index < MAX_SOCKETS_SUPPORTED + 1; index++)
    {
        if ((pcustctxt = A_MALLOC(sizeof(SOCKET_CONTEXT), MALLOC_ID_CONTEXT)) == NULL)
        {
            return A_NO_MEMORY;
        }
        memset(pcustctxt, 0, sizeof(SOCKET_CONTEXT));
        A_EVENT_INIT(&pcustctxt->sockRxWakeEvent, kEventManualClear);
        A_EVENT_INIT(&pcustctxt->sockTxWakeEvent, kEventManualClear);

        A_NETBUF_QUEUE_INIT(&(pcustctxt->rxqueue));

        pcustctxt->blockFlag = 0;
        pcustctxt->respAvailable = false;
        ath_sock_context[index]->sock_context = pcustctxt;
        ath_sock_context[index]->remaining_bytes = 0;
        ath_sock_context[index]->old_netbuf = NULL;

#if NON_BLOCKING_TX
        A_NETBUF_QUEUE_INIT(&(pcustctxt->non_block_queue));
        A_MUTEX_INIT(&pcustctxt->nb_tx_mutex);
#endif
    }

#if ZERO_COPY
    /*Initilize common queue used to store Rx packets for zero copy option*/
    A_NETBUF_QUEUE_INIT(&zero_copy_free_queue);
#endif

    return A_OK;
}

/*****************************************************************************/
/*  socket_context_deinit - De-initializes common socket context, called during
 *                 driver deinit.
 *****************************************************************************/
void socket_context_deinit(void)
{
    uint32_t index = 0;
    SOCKET_CONTEXT_PTR pcustctxt = NULL;

    for (index = 0; index < MAX_SOCKETS_SUPPORTED + 1; index++)
    {
        A_FREE(ath_sock_context[index], MALLOC_ID_CONTEXT);
        ath_sock_context[index] = NULL;
    }
    /*Free the custom context as well*/

    for (index = 0; index < MAX_SOCKETS_SUPPORTED + 1; index++)
    {
        pcustctxt = GET_SOCKET_CONTEXT(ath_sock_context[index]);
#if NON_BLOCKING_TX
        A_MUTEX_DELETE(&pcustctxt->nb_tx_mutex);
#endif
        A_EVENT_DELETE(&pcustctxt->sockRxWakeEvent);
        A_EVENT_DELETE(&pcustctxt->sockTxWakeEvent);
        A_FREE(pcustctxt, MALLOC_ID_CONTEXT);
    }
}

/*****************************************************************************/
/*  Api_SockResponseEventRx - Handler for WMI socket receive events.
 *  void *pCxt  - driver context
 *  uint8_t *datap - pointer to incoming event data
 *  uint32_t len   - length of event data
 *****************************************************************************/
A_STATUS Api_SockResponseEventRx(void *pCxt, uint8_t devId, uint8_t *datap, uint32_t len, void *pReq)
{
    int32_t index = 0;
    uint8_t unblock_flag = 0;
    WMI_SOCK_RESPONSE_EVENT *response = (WMI_SOCK_RESPONSE_EVENT *)datap;
    uint32_t resp_type = A_CPU2LE32(response->resp_type);
    A_STATUS status = A_OK;
    uint8_t freeBuf = 1;

    switch (resp_type)
    {
        case SOCK_OPEN:
            index = find_socket_context(SOCKET_HANDLE_PLACEHOLDER, true);

            if (index < 0 || index > MAX_SOCKETS_SUPPORTED)
            {
                last_driver_error = A_SOCKCXT_NOT_FOUND;
                status = A_ERROR;
                break;
            }
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Store the newly created socket handle*/
                ath_sock_context[index]->handle = A_CPU2LE32(response->sock_handle);
                /* unlock the application thread*/
                unblock_flag = 1;
            }
            break;

        case SOCK_ACCEPT:
            index = find_socket_context(A_CPU2LE32(response->sock_handle), true);

            if (index < 0 || index > MAX_SOCKETS_SUPPORTED)
            {
                last_driver_error = A_SOCKCXT_NOT_FOUND;
                status = A_ERROR;
                break;
            }

            /*Check if a socket is waiting on the response*/
            // if(SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy incoming socket related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->pReq = pReq;

                /*Store the response in socket context result field*/
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
                {
                    unblock_flag = 1;
                }
                freeBuf = 0;
                UNBLOCK_SELECT(pCxt);
            }
            break;
        case SOCK_CLOSE:
            /* A socket close event may be received under two scenarios-
              1. In response to explicit socket close request from host.
              2. An unsolicited socket close to close a socket created earlier for
                 an incoming TCP connection
              In the first scenario, the application thread is waiting for the response,
              so unblock the thread.
              In the second case, just cleanup the socket context*/

            index = find_socket_context(A_CPU2LE32(response->sock_handle), true);

            if (index < 0 || index > MAX_SOCKETS_SUPPORTED)
            {
                last_driver_error = A_SOCKCXT_NOT_FOUND;
                status = A_ERROR;
                break;
            }

            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /* the blocked flag will clear the context. clearing
                 * the context here will erase the mask which would
                 * break up the task synchronization. */
                /*unlock the thread*/
                unblock_flag = 1;
                /*Store the response in socket context result field*/
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
            }
            else
            {
                /* Case 2: clear the context here, no one is waiting */
                if(QUEUE_EMPTY(index)) {
                    clear_socket_context(index);
                }
                else
                {
                    ath_sock_context[index]->TCPCtrFlag = TCP_FIN;
                }
#if T_SELECT_VER1
                // Asynchronous sock_close event.
                // printf("Asynch SOCK Close for socket index %d\n", index);
                // SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_CLOSE);
                UNBLOCK_SELECT(pCxt);
#endif // T_SELECT_VER1
            }
            break;
        case SOCK_CONNECT:
            index = find_socket_context(A_CPU2LE32(response->sock_handle), true);

            if (index < 0 || index > MAX_SOCKETS_SUPPORTED)
            {
                last_driver_error = A_SOCKCXT_NOT_FOUND;
                status = A_ERROR;
                break;
            }
            /*Store the response in socket context result field*/
            ath_sock_context[index]->result = A_CPU2LE32(response->error);
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*unlock the thread*/
                unblock_flag = 1;
            }
            else
            {
                if (A_CPU2LE32(response->error) == TCP_CONNECTION_AVAILABLE)
                {
                    /*Special case: async listen event has been received indicating a new
                    TCP connection is available. The User thread may be blocked with a select
                    call, set the unblock_flag so the user thread can be unlocked*/
                    unblock_flag = 1;
                }
            }
            UNBLOCK_SELECT(pCxt);
            break;

        case SOCK_BIND:
        case SOCK_LISTEN:
        /*Listen Event may be received in two cases:
        Case 1: In response to a listen request from host, that puts TCP socket in
                Listen state.
        Case 2: An unsolicited listen with a result value TCP_CONNECTION_AVAILABLE,
                that informs an application thread that a new TCP connection is
                available. This allows the application to call accept.*/
        case SOCK_ERRNO:
        case SOCK_SETSOCKOPT:
            index = find_socket_context(A_CPU2LE32(response->sock_handle), true);

            if (index < 0 || index > MAX_SOCKETS_SUPPORTED)
            {
                last_driver_error = A_SOCKCXT_NOT_FOUND;
                status = A_ERROR;
                break;
            }
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Store the response in socket context result field*/
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            else
            {
                if (A_CPU2LE32(response->error) == TCP_CONNECTION_AVAILABLE)
                {
                    /*Special case: async listen event has been received indicating a new
                    TCP connection is available. The User thread may be blocked with a select
                    call, set the unblock_flag so the user thread can be unlocked*/
                    ath_sock_context[index]->result = A_CPU2LE32(response->error);
                    unblock_flag = 1;
                }
            }
            break;
        case SOCK_GETSOCKOPT:
            index = find_socket_context(A_CPU2LE32(response->sock_handle), true);

            if (index < 0 || index > MAX_SOCKETS_SUPPORTED)
            {
                last_driver_error = A_SOCKCXT_NOT_FOUND;
                status = A_ERROR;
                break;
            }

            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*Copy select related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->pReq = pReq;
                /* unlock the application thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_IPCONFIG:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->pReq = pReq;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_PING:
        case SOCK_PING6:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }

            break;
        case SOCK_IP6CONFIG:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
#if ENABLE_HTTP_SERVER
        case SOCK_HTTP_SERVER:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            break;
        case SOCK_HTTP_SERVER_CMD:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->pReq = pReq;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*Copy select related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_HTTP_POST_EVENT:
            CUSTOM_API_HTTP_POST_EVENT(pCxt, response->data);
            break;
#endif
#if ENABLE_HTTP_CLIENT
        case SOCK_HTTPC:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->pReq = pReq;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*Copy select related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            // printf("Received http client resp :%d req %x\n", ath_sock_context[index]->result,
            // ath_sock_context[index]->pReq);
            break;
#endif /* ENABLE_HTTP_CLIENT */
        case SOCK_DNC_CMD:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_DNC_ENABLE:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
        case SOCK_DNS_SRVR_CFG_ADDR:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            break;
        case SOCK_IP_HOST_NAME:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            break;
        case SOCK_IP_DNS:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            break;
        case SOCK_IP_SNTP_GET_TIME:
            index = GLOBAL_SOCK_INDEX;
            // printf("resp from sntp \r\n");
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                //("unblock\r\n");
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_IP_SNTP_GET_TIME_OF_DAY:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                //("unblock\r\n");
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_IP_SNTP_QUERY_SNTP_ADDRESS:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                //("unblock\r\n");
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_IPV4_ROUTE:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_IPV6_ROUTE:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
#if ENABLE_SSL
        case SOCK_SSL_SET_FD:
        case SOCK_SSL_ACCEPT:
        case SOCK_SSL_CONNECT:
            index = find_socket_context_from_ssl((SSL *)response->sock_handle); // ssl context
            if (index == SOCKET_NOT_FOUND)
            {
                /* We send the configure command on the "global socket" if the configure is
                * done before the socket is created */
                index = GLOBAL_SOCK_INDEX;
            }
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            break;

        case SOCK_SSL_CONFIGURE:
        case SOCK_SSL_SHUTDOWN:
            index = find_socket_context_from_ssl((SSL *)response->sock_handle); // ssl context
            if (index == SOCKET_NOT_FOUND)
            {
                index = GLOBAL_SOCK_INDEX;
            }
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            break;

        case SOCK_SSL_CTX_NEW:
        case SOCK_SSL_CTX_FREE:
        case SOCK_SSL_NEW:
        case SOCK_SSL_ADD_CERT:
        case SOCK_SSL_STORE_CERT:
        case SOCK_SSL_LOAD_CERT:
            index = GLOBAL_SOCK_INDEX;
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
            }
            break;

        case SOCK_SSL_LIST_CERT:
            index = GLOBAL_SOCK_INDEX;
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                ath_sock_context[index]->pReq = pReq;
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
#endif
        case SOCK_OTA_UPGRADE:
        case SOCK_OTA_READ:
        case SOCK_OTA_DONE:
        case SOCK_OTA_SESSION_START:
        case SOCK_OTA_PARTITION_GET_SIZE:
        case SOCK_OTA_PARTITION_ERASE:
        case SOCK_OTA_PARTITION_VERIFY_CHECKSUM:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            break;
        case SOCK_OTA_PARSE_IMAGE_HDR:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            else
            {
                tOtaParseImageHdrResp *tmp_resp = (tOtaParseImageHdrResp*)response->data;
                Custom_Api_Ota_Resp_Result(pCxt, SOCK_OTA_PARSE_IMAGE_HDR,
                                           tmp_resp->resp_code,
                                           tmp_resp->offset);
            }
            break;
        case SOCK_OTA_PARTITION_WRITE_DATA:
            index = GLOBAL_SOCK_INDEX;
            /*Check if a socket is waiting on the response*/
            if (SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type))
            {
                /*Copy ipconfig related information*/
                ath_sock_context[index]->data = response->data;
                ath_sock_context[index]->result = A_CPU2LE32(response->error);
                ath_sock_context[index]->pReq = pReq;
                /*unlock the thread*/
                unblock_flag = 1;
                freeBuf = 0;
            }
            else
            {
                tOtaPartitionWriteDataResp *tmp_resp = (tOtaPartitionWriteDataResp*)response->data;
                Custom_Api_Ota_Resp_Result(pCxt, SOCK_OTA_PARTITION_WRITE_DATA,
                                           tmp_resp->resp_code,
                                           tmp_resp->size);
            }
            break;
        case SOCK_DHCPS_SUCCESS_CALLBACK:
            Custom_Api_Dhcps_Success_Callback_Event(pCxt, response->data);
            break;
        case SOCK_DHCPC_SUCCESS_CALLBACK:
            Custom_Api_Dhcpc_Success_Callback_Event(pCxt, response->data);
            break;
        default:
            last_driver_error = A_UNKNOWN_CMD;
            status = A_ERROR;
            break;
    }
    /* if a user task is blocked on this event then unblock it. */
    if (unblock_flag /* && SOCK_EV_MASK_TEST(ath_sock_context[index], resp_type)*/)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], resp_type);
        UNBLOCK(ath_sock_context[index], RX_DIRECTION);
    }

    if (freeBuf)
    {
        /*Free the netbuf*/
        A_NETBUF_FREE(pReq);
    }
    return status;
}

/*****************************************************************************/
/*  getIPLength - Calculates total IP header length.
 *  uint8_t version - IP version
 * Returns- IP header length
 *****************************************************************************/
uint32_t getIPLength(uint8_t version)
{
    uint32_t length = 0;
    switch (version)
    {
        case ATH_AF_INET:
            length = IPV4_HEADER_LENGTH;
            break;

        case ATH_AF_INET6:
            length = IPV6_HEADER_LENGTH;
            break;
        default:
            break;
    }

    return length;
}

/*****************************************************************************/
/*  getTransportLength - Calculates Transport layer header length
 *  uint8_t proto - UDP or TCP
 * Returns- Transport layer header length
 *****************************************************************************/
uint32_t getTransportLength(uint8_t proto)
{
    uint32_t length = 0;
    switch (proto)
    {
        case SOCK_STREAM_TYPE:
            length = TCP_HEADER_LENGTH;
            break;

        case SOCK_DGRAM_TYPE:
            length = UDP_HEADER_LENGTH;
            break;
        default:
            break;
    }

    return length;
}

/*****************************************************************************/
/*  Api_socket - API to create new socket
 *  void *pCxt- driver context
 *  uint32_t domain- IPv4, IPv6
 *  uint32_t type- TCP, UDP
 *  uint32_t protocol-
 * Returns- socket handle in case of success, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_socket(void *pCxt, uint32_t domain, uint32_t type, uint32_t protocol)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_OPEN_T sock_open;
    int32_t index = 0;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    osMutexAcquire(pDCxt->apiMutex, osWaitForever);

    do
    {
        /*Create new context*/
        if ((index = find_socket_context(EMPTY_SOCKET_CONTEXT, false)) != SOCKET_NOT_FOUND)
        {
            ath_sock_context[index]->handle = SOCKET_HANDLE_PLACEHOLDER;
            ath_sock_context[index]->domain = domain;
            ath_sock_context[index]->type = type;
        }
        else
        {
            last_driver_error = A_SOCK_UNAVAILABLE;
            result = A_ERROR;
            break;
        }

        /*Create socket open wmi message*/
        sock_open.domain = A_CPU2LE32(domain);
        sock_open.type = A_CPU2LE32(type);
        sock_open.protocol = A_CPU2LE32(protocol);

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OPEN);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OPEN, (void *)(&sock_open), sizeof(SOCK_OPEN_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OPEN);
            /*Command failed, clear socket context and return error*/
            clear_socket_context(index);
            result = A_ERROR;
            break;
        }

        /*Wait for response from target*/
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OPEN));

        /* Generic error reported in handle field */
        if (ath_sock_context[index]->handle == A_ERROR)
        {
            /*Socket not created, return error*/
            clear_socket_context(index);
            result = A_ERROR;
            break;
        }
//        /* 'handle' 0 means (probably) not enough resources */
//        else if (ath_sock_context[index]->handle == 0)
//        {
//            /*Socket not created, return error*/
//            clear_socket_context(index);
//            result = A_RESOURCES;
//            break;
//        }
        /* cosider any other number as valid 'handle' reference */
        else
        {
            result = ath_sock_context[index]->handle;
        }
    } while (0);

    osMutexRelease(pDCxt->apiMutex);

    return result;
}

/*****************************************************************************/
/*  Api_shutdown - Close a previously opened socket
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 * Returns- 0 in case of successful shutdown, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_shutdown(void *pCxt, uint32_t handle)
{
    SOCK_CLOSE_T sock_close;
    int32_t index = 0;
    A_DRIVER_CONTEXT *pDCxt;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_SOCK_INVALID;
            break;
        }

        /*Check if some other thread is wating on this socket*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_CLOSE);

        /*Delete all pending packets in the receive queue*/
        CUSTOM_PURGE_QUEUE(index);

        /*Create a socket close wmi message*/
        sock_close.handle = A_CPU2LE32(handle);
        /* set the sock_st_flags before calling wmi_ to avoid possible race conditions */

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_CLOSE, (void *)(&sock_close), sizeof(SOCK_CLOSE_T)) != A_OK)
        {
            /* clear the flag that would have been cleared by receiving the event */
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_CLOSE);
            result = A_ERROR;
            break;
        }

#if 1
        /* Wait for response from target */
        do {
            if(BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK) {
                A_ASSERT(0);
            }
        } while(SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_CLOSE));
#endif

        result = ath_sock_context[index]->result;
        /* Delete all pending packets */
        CUSTOM_PURGE_QUEUE(index);

        /* clear the socket to make it available for re-use */
        clear_socket_context(index);
    } while (0);

    return result;
}

/*****************************************************************************/
/*  Api_connect - API to connect to a peer
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  void* name- sock addr structure
 *  uint16_t length- sock add length
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_connect(void *pCxt, uint32_t handle, void *name, uint16_t length)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_CONNECT_CMD_T sock_connect;
    int32_t index = 0;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Retrieve context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }

        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        sock_connect.handle = A_CPU2LE32(handle);
        sock_connect.length = A_CPU2LE16(length);

        if (ath_sock_context[index]->domain == ATH_AF_INET)
        {
            sock_connect.addr.name.sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
            sock_connect.addr.name.sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
            sock_connect.addr.name.sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);
        }
        else
        {
            sock_connect.addr.name6.sin6_port = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_port);
            sock_connect.addr.name6.sin6_family = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_family);
            sock_connect.addr.name6.sin6_flowinfo = A_CPU2LE32(((SOCKADDR_6_T *)name)->sin6_flowinfo);
            A_MEMCPY((uint8_t *)&(sock_connect.addr.name6.sin6_addr), (uint8_t *)&((SOCKADDR_6_T *)name)->sin6_addr,
                     sizeof(IP6_ADDR_T));
        }

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_CONNECT);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_CONNECT, (void *)(&sock_connect), sizeof(SOCK_CONNECT_CMD_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_CONNECT);
            result = A_ERROR;
            break;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_CONNECT));
        result = ath_sock_context[index]->result;
        if (A_OK == result)
        {
            ath_sock_context[index]->TCPCtrFlag = TCP_CONNECTED;
        }
    } while (0);

    return result;
}

// Non blocking version of connect
int32_t Api_connect_nb(void *pCxt, uint32_t handle, void *name, uint16_t length)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_CONNECT_CMD_T sock_connect;
    int32_t index = 0;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Retrieve context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }

        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        sock_connect.handle = A_CPU2LE32(handle);
        sock_connect.length = A_CPU2LE16(length);

        if (ath_sock_context[index]->domain == ATH_AF_INET)
        {
            sock_connect.addr.name.sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
            sock_connect.addr.name.sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
            sock_connect.addr.name.sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);
        }
        else
        {
            sock_connect.addr.name6.sin6_port = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_port);
            sock_connect.addr.name6.sin6_family = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_family);
            sock_connect.addr.name6.sin6_flowinfo = A_CPU2LE32(((SOCKADDR_6_T *)name)->sin6_flowinfo);
            A_MEMCPY((uint8_t *)&(sock_connect.addr.name6.sin6_addr), (uint8_t *)&((SOCKADDR_6_T *)name)->sin6_addr,
                     sizeof(IP6_ADDR_T));
        }

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_CONNECT, (void *)(&sock_connect), sizeof(SOCK_CONNECT_CMD_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_CONNECT);
            result = A_ERROR;
            break;
        }

    } while (0);

    return result;
}

/*****************************************************************************/
/*  Api_bind - API to bind to an interface. Works for both IPv4 and v6, name
 *            field must be populated accordingly.
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  void* name- sock addr structure
 *  uint16_t length- sock add length
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_bind(void *pCxt, uint32_t handle, void *name, uint16_t length)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_BIND_CMD_T sock_bind;
    int32_t index = 0;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }

        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }
        sock_bind.handle = A_CPU2LE32(handle);
        sock_bind.length = A_CPU2LE16(length);

        if (ath_sock_context[index]->domain == ATH_AF_INET)
        {
            sock_bind.addr.name.sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
            sock_bind.addr.name.sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
            sock_bind.addr.name.sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);
        }
        else
        {
            sock_bind.addr.name6.sin6_port = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_port);
            sock_bind.addr.name6.sin6_family = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_family);
            sock_bind.addr.name6.sin6_flowinfo = A_CPU2LE32(((SOCKADDR_6_T *)name)->sin6_flowinfo);
            A_MEMCPY((uint8_t *)&(sock_bind.addr.name6.sin6_addr), (uint8_t *)&((SOCKADDR_6_T *)name)->sin6_addr,
                     sizeof(IP6_ADDR_T));
        }

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_BIND);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_BIND, (void *)(&sock_bind), sizeof(SOCK_BIND_CMD_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_BIND);
            result = A_ERROR;
            break;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_BIND));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/*  Api_listen - API to Listen for incoming connections. Only used on stream sockets
 *               Works for both IPv4 and v6.
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  uint32_t backlog- backlog of pending connections. The backlog parameter defines
 *                    the maximum length for the queue of pending connections
 * Returns- 0 in case of success, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_listen(void *pCxt, uint32_t handle, uint32_t backlog)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_LISTEN_T sock_listen;
    int32_t index = 0;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }

        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        sock_listen.handle = A_CPU2LE32(handle);
        sock_listen.backlog = A_CPU2LE16(backlog);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_LISTEN, (void *)(&sock_listen), sizeof(SOCK_LISTEN_T)) != A_OK)
        {
            result = A_ERROR;
            break;
        }

        // result = ath_sock_context[index]->result;
    } while (0);

    if (index >= 0 && result == A_OK)
    {
        ath_sock_context[index]->TCPCtrFlag = TCP_LISTEN;
    }
    else
    {
        // TODO: fixme, how to handle this case !!!
        assert(0);
    }

    return result;
}

/*****************************************************************************/
/*  Api_accept - API to accept incoming inconnections. Works for both IPv4 and v6,
 *            name field must be populated accordingly. Must call select to wait
 *            for incoming connection before calling this API.
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  void* name- sock addr structure
 *  uint16_t length- sock add length
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_accept(void *pCxt, uint32_t handle, void *name, socklen_t length)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_ACCEPT_CMD_T sock_accept;
    int32_t index = 0;
    int32_t accept_index = 0;
    int32_t result = A_OK;
    uint8_t free_buf = 1;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            free_buf = 0;
            break;
        }
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            free_buf = 0;
            break;
        }
        /* prepare wmi accept structure*/
        sock_accept.handle = A_CPU2LE32(handle);
        sock_accept.length = A_CPU2LE16(length);

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_ACCEPT);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_ACCEPT, (void *)(&sock_accept), sizeof(SOCK_ACCEPT_CMD_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_ACCEPT);
            result = A_ERROR;
            free_buf = 0;
            break;
        }

        /*Block until stack provides a response*/
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_ACCEPT));

        if (ath_sock_context[index]->result == TCP_CONNECTION_AVAILABLE)
        {
            /*Special case: Received TCP_CONN_AVAILABLE from target in response to accept, call
            Accept again to create new connection, free the previos netbuf*/
            if (free_buf)
            {
                /*Free the netbuf*/
                A_NETBUF_FREE(ath_sock_context[index]->pReq);
            }
            /*Target stack indicates that a new TCP connection is available, accept it*/
            SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_ACCEPT);

            if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_ACCEPT, (void *)(&sock_accept), sizeof(SOCK_ACCEPT_CMD_T)) != A_OK)
            {
                SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_ACCEPT);
                result = A_ERROR;
                free_buf = 0;
                break;
            }

            /*Block until stack provides a response*/
            do
            {
                if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
                {
                    A_ASSERT(0);
                }
            } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_ACCEPT));
        }

        if (ath_sock_context[index]->result != A_ERROR)
        {
            /*Create new context*/
            if ((accept_index = find_socket_context(EMPTY_SOCKET_CONTEXT, false)) != SOCKET_NOT_FOUND)
            {
                ath_sock_context[accept_index]->handle = ath_sock_context[index]->result;
                ath_sock_context[accept_index]->domain = ath_sock_context[index]->domain;
                ath_sock_context[accept_index]->type = ath_sock_context[index]->type;
            }
            else
            {
                last_driver_error = A_SOCK_UNAVAILABLE;
                result = A_ERROR;
                break;
            }

            if (ath_sock_context[index]->data != NULL)
            {
                /*Based on IPv4 vs IPv6, fill in name fields*/
                if (ath_sock_context[index]->domain == ATH_AF_INET)
                {
                    A_MEMCPY(name, &((SOCK_ACCEPT_RECV_T *)(ath_sock_context[index]->data))->addr.name,
                             sizeof(SOCKADDR_T));
                    ((SOCKADDR_T *)name)->sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
                    ((SOCKADDR_T *)name)->sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
                    ((SOCKADDR_T *)name)->sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);

                    if (SOCK_STREAM_TYPE == ath_sock_context[accept_index]->type)
                    {
                        ath_sock_context[accept_index]->TCPCtrFlag = TCP_CONNECTED;
                    }
                }
                else
                {
                    A_MEMCPY(name, &((SOCK_ACCEPT_RECV_T *)(ath_sock_context[index]->data))->addr.name6,
                             sizeof(SOCKADDR_6_T));
                    ((SOCKADDR_6_T *)name)->sin6_port = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_port);
                    ((SOCKADDR_6_T *)name)->sin6_family = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_family);

                    if (SOCK_STREAM_TYPE == ath_sock_context[accept_index]->type)
                    {
                        ath_sock_context[accept_index]->TCPCtrFlag = TCP_CONNECTED;
                    }
                }
                // A_FREE(ath_sock_context[index]->data, MALLOC_ID_CONTEXT);
                // ath_sock_context[index]->data = NULL;

                ath_sock_context[index]->data = NULL;
            }
        }
        result = ath_sock_context[index]->result;
    } while (0);

    if (free_buf)
    {
        /*Free the netbuf*/
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
    return result;
}

#if T_SELECT_VER1

// Non blocking accept
int32_t Api_accept_ver1(void *pCxt, uint32_t handle, void *name, socklen_t length)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_ACCEPT_CMD_T sock_accept;
    int32_t index = 0;
    int32_t accept_index = 0;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        // The accept was issued earlier and the result of accept is avaible here
        if ((ath_sock_context[index]->result != TCP_CONNECTION_AVAILABLE) && (ath_sock_context[index]->data != NULL))
        {
            /*Create new context*/
            if ((accept_index = find_socket_context(EMPTY_SOCKET_CONTEXT, false)) != SOCKET_NOT_FOUND)
            {
                ath_sock_context[accept_index]->handle = ath_sock_context[index]->result;
                ath_sock_context[accept_index]->domain = ath_sock_context[index]->domain;
                ath_sock_context[accept_index]->type = ath_sock_context[index]->type;
            }
            else
            {
                last_driver_error = A_SOCK_UNAVAILABLE;
                result = A_ERROR;
                break;
            }
            if (ath_sock_context[index]->data != NULL)
            {
                /*Based on IPv4 vs IPv6, fill in name fields*/
                if (ath_sock_context[index]->domain == ATH_AF_INET)
                {
                    A_MEMCPY(name, &((SOCK_ACCEPT_RECV_T *)(ath_sock_context[index]->data))->addr.name,
                             sizeof(SOCKADDR_T));
                    ((SOCKADDR_T *)name)->sin_port = A_CPU2LE16(((SOCKADDR_T *)name)->sin_port);
                    ((SOCKADDR_T *)name)->sin_family = A_CPU2LE16(((SOCKADDR_T *)name)->sin_family);
                    ((SOCKADDR_T *)name)->sin_addr = A_CPU2LE32(((SOCKADDR_T *)name)->sin_addr);
                }
                else
                {
                    A_MEMCPY(name, &((SOCK_ACCEPT_RECV_T *)(ath_sock_context[index]->data))->addr.name6,
                             sizeof(SOCKADDR_6_T));
                    ((SOCKADDR_6_T *)name)->sin6_port = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_port);
                    ((SOCKADDR_6_T *)name)->sin6_family = A_CPU2LE16(((SOCKADDR_6_T *)name)->sin6_family);
                }
                // A_FREE(ath_sock_context[index]->data, MALLOC_ID_CONTEXT);
                // ath_sock_context[index]->data = NULL;

                ath_sock_context[index]->data = NULL;
            }
            result = ath_sock_context[index]->result;
            A_NETBUF_FREE(ath_sock_context[index]->pReq);
        }
        else
        {
            /* prepare wmi accept structure*/
            sock_accept.handle = A_CPU2LE32(handle);
            sock_accept.length = A_CPU2LE16(length);

            SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_ACCEPT);

            // This is a non-blocking accept
            if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_ACCEPT, (void *)(&sock_accept), sizeof(SOCK_ACCEPT_CMD_T)) != A_OK)
            {
                SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_ACCEPT);
                result = A_ERROR;
                break;
            }
        }
    } while (0);

    return result;
}
#endif // T_SELECT_VER1

/*****************************************************************************/
/*  Api_errno - API to fetch last error code from target
 *
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 * Returns- error code in case of success, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_errno(void *pCxt, uint32_t handle)
{
    A_DRIVER_CONTEXT *pDCxt;
    int32_t index = 0;
    int32_t result = A_OK;
    SOCK_ERRNO_T sock_errno;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }
        sock_errno.errno = A_CPU2LE32(handle);
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_ERRNO);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_ERRNO, (uint8_t *)&sock_errno, sizeof(SOCK_ERRNO_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_ERRNO);
            result = A_ERROR;
            break;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_ERRNO));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/*  Api_select -
 *
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  uint32_t tv - time to wait in milliseconds
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *  Select can be called to check for incoming connections or for incoming data.
 *  It waits for a specified time period before returning. This call does not
 *  propagate to the target, it is consumed in the host.
 *****************************************************************************/
int32_t Api_select(void *pCxt, uint32_t handle, uint32_t tv)
{
    int32_t index = 0;
    int32_t result = A_OK;

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_SOCK_INVALID;
            break;
        }

        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }
        if ((ath_sock_context[index]->type == SOCK_STREAM_TYPE) && (ath_sock_context[index]->remaining_bytes != 0) &&
            (ath_sock_context[index]->old_netbuf != NULL))
        {
            result = A_OK;
            break;
        }
        /*Packet is available, return*/
        if (!QUEUE_EMPTY(index))
        {
            result = A_OK;
            break;
        }

        if (ath_sock_context[index]->TCPCtrFlag == TCP_FIN)
        {
            if (QUEUE_EMPTY(index))
            {
                clear_socket_context(index);
                result = A_SOCK_INVALID;
                // Why on earth is this here? (MMJ)
                // OS_INT_ENABLE(0);
                break;
            }
        }

        if (ath_sock_context[index]->TCPCtrFlag == TCP_LISTEN)
        {
            /*Wait for specified time*/
            if (BLOCK(pCxt, ath_sock_context[index], tv, RX_DIRECTION) != A_OK)
            {
                /*Check if Peer closed socket while we were waiting*/
                if (ath_sock_context[index]->handle == 0)
                {
                    result = A_SOCK_INVALID;
                    break;
                }
                /*Timeout, no activity detected*/
                result = A_ERROR;
            }
            else
            {
                /*Something is available, it may be a new incoming connection or data*/
                if ((!QUEUE_EMPTY(index)) || (ath_sock_context[index]->result == TCP_CONNECTION_AVAILABLE))
                {
                    ath_sock_context[index]->result = 0;
                    result = A_OK;
                }
                else
                {
                    result = A_ERROR;
                }
            }
        }
        else
        {
            /*Wait for specified time*/
            if (BLOCK_FOR_DATA(pCxt, ath_sock_context[index], tv, RX_DIRECTION) != A_OK)
            {
                /*Check if Peer closed socket while we were waiting*/
                if (ath_sock_context[index]->handle == 0)
                {
                    result = A_SOCK_INVALID;
                    break;
                }
                /*Timeout, no activity detected*/
                result = A_ERROR;
            }
            else
            {
                /*Something is available, it may be a new incoming connection or data*/
                if ((!QUEUE_EMPTY(index)) || (ath_sock_context[index]->result == TCP_CONNECTION_AVAILABLE))
                {
                    ath_sock_context[index]->result = 0;
                    result = A_OK;
                }
                else
                {
                    result = A_ERROR;
                }
            }
        }
    } while (0);
    return result;
}

#if T_SELECT_VER1
/*****************************************************************************/
/*  Api_select -
 *
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  uint32_t tv - time to wait in milliseconds
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *  Select can be called to check for incoming connections or for incoming data.
 *  It waits for a specified time period before returning. This call does not
 *  propagate to the target, it is consumed in the host.
 *****************************************************************************/

int32_t Api_select_ver1(void *pCxt, int32_t num, uint32_t *r_fd, uint32_t *w_fd, uint32_t *e_fd, uint32_t tv)
{
    int32_t index = 0;
    int32_t result = 0;
    int32_t first_time = 1;
    uint32_t r_mask, w_mask;
    HTC_ENDPOINT_ID eid;
    A_ENDPOINT_T *pEp;
    static uint32_t next_txsock = 0;
    uint32_t startsock;
    uint8_t credits;

    w_mask = r_mask = 0;

    // w_fd and r_fd are used to return the socket indexes which have some
    // activity. Take a backup and initialize them to zero
    if (w_fd)
    {
        eid = Util_AC2EndpointID(pCxt, WMM_AC_BE);
        pEp = Util_GetEndpoint(pCxt, eid);
        credits = pEp->credits;

        if (credits > 0) /* Credits Available */
        {
            /* Save the last serviced socket and start from the next one to avoid
             * servicing the first socket alone in case of only one credit available
             */
            startsock = next_txsock;
            for (index = startsock; index < (MAX_SOCKETS_SUPPORTED + startsock); index++)
            {
                index %= MAX_SOCKETS_SUPPORTED;
                if (w_mask & (1 << index))
                {
                    if (ath_sock_context[index]->handle != 0)
                    {
                        *w_fd |= 1 << index;
                        result += 1;

                        next_txsock = index + 1;
                        credits--;
                    }
                    else
                    {
                        // STALE socket. Update the mask for this handle. The next
                        // read will return 0/-1
                        *(w_fd) |= (1 << index);
                        result += 1;
                    }
                }
                /* If there are no credits, break */
                if (!credits)
                    break;
            }
        } /* credits > 0 */
    }     /* w_mask */

    if (r_fd)
    {
        r_mask = *r_fd;
        *r_fd = 0;
    }

    if ((w_mask == 0) && (r_mask == 0))
    {
        // Nothing to do
        return result;
    }

AGAIN:
    // Check if we have enough credits
    if (w_mask)
    {
        eid = Util_AC2EndpointID(pCxt, WMM_AC_BE);
        pEp = Util_GetEndpoint(pCxt, eid);
        if (pEp->credits > 0)
        {
            for (index = 0; index < MAX_SOCKETS_SUPPORTED; index++)
            {
                if (w_mask & (1 << index))
                {
                    *w_fd = 1 << index;
                    result += 1;
                    break;
                }
            }
        }
    }

    // Go through all read handles
    if (r_mask)
    {
        for (index = 0; index < MAX_SOCKETS_SUPPORTED; index++)
        {
            if (r_mask & (1 << index))
            {
                if (ath_sock_context[index]->handle != 0)
                {
#if 0
					if(/*(ath_sock_context[index]->type == SOCK_STREAM_TYPE) && */
					   (ath_sock_context[index]->remaining_bytes != 0) &&
						   (ath_sock_context[index]->old_netbuf != NULL))
					{
						//Update the mask for this handle
						*(r_fd) |= (1<<index);
						result += 1;
						continue;
					}
#endif
                    /*Packet is available*/
                    if (!QUEUE_EMPTY(index) || (ath_sock_context[index]->result == TCP_CONNECTION_AVAILABLE) ||
                        (ath_sock_context[index]->data != NULL))
                    {
                        // Update the mask for this handle
                        *(r_fd) |= (1 << index);
                        result += 1;
                    }

                    // Update the mask for this handle. The next
                    // read will return 0/-1
                    if (ath_sock_context[index]->TCPCtrFlag == TCP_FIN)
                    {
                        // Update the mask for this handle
                        *(r_fd) |= (1 << index);
                        result += 1;
                    }
                }
                else // if( (r_mask & (1<<index)) && (ath_sock_context[index]->handle == 0) )
                {
                    // STALE socket. Update the mask for this handle. The next
                    // read will return 0/-1
                    *(r_fd) |= (1 << index);
                    result += 1;
                }
            }
        }
    }

    // If no pending activity, wait for the time requested and again check for
    // activity
    if ((result == 0) && (first_time))
    {
        first_time = 0;
        // Wait for the specified time
        if (BLOCK_SELECT(pCxt, tv) == A_OK)
        {
            // Some activity happened because of which we were woken up. Figure
            // out the activity and set the mask accordingly
            goto AGAIN;
        }
    }

    return result;
}

#endif

/*****************************************************************************/
/*  Api_setsockopt - API to set specified socket option
 *            name field must be populated accordingly.
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  uint32_t level- option level
 *  uint32_t optname- option name
 *  uint32_t optlen- option length
 * Returns- 0 in case of success, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_setsockopt(void *pCxt, uint32_t handle, uint32_t level, uint32_t optname, uint8_t *optval, uint32_t optlen)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_OPT_T *sockSetopt;
    //	uint8_t* data = NULL;
    int32_t index = 0, total_length = 0;
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }
        total_length = sizeof(SOCK_OPT_T);

        if (optval != NULL && optlen != 0)
        {
            total_length += optlen - sizeof(uint8_t);
        }

        /*Allocate space for option*/
        if ((sockSetopt = A_MALLOC(total_length, MALLOC_ID_CONTEXT)) == NULL)
        {
            result = A_NO_MEMORY;
            break;
        }

        if (optval != NULL && optlen != 0)
        {
            A_MEMCPY(sockSetopt->optval, optval, optlen);
        }

        sockSetopt->handle = A_CPU2LE32(handle);
        sockSetopt->level = A_CPU2LE32(level);
        sockSetopt->optname = A_CPU2LE32(optname);
        sockSetopt->optlen = A_CPU2LE32(optlen);

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SETSOCKOPT);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SETSOCKOPT, (uint8_t *)sockSetopt, total_length) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SETSOCKOPT);
            A_FREE(sockSetopt, MALLOC_ID_CONTEXT);
            result = A_ERROR;
            break;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SETSOCKOPT));

        A_FREE(sockSetopt, MALLOC_ID_CONTEXT);
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/*  Api_getsockopt - API to fetch specified socket option
 *            name field must be populated accordingly.
 *  void *pCxt- driver context
 *  uint32_t handle- socket handle
 *  uint32_t level- option level
 *  uint32_t optname- option name
 *  uint32_t optlen- option length
 * Returns- 0 in case of success, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_getsockopt(void *pCxt, uint32_t handle, uint32_t level, uint32_t optname, uint8_t *optval, uint32_t optlen)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_OPT_T *sock_getopt = NULL;
    uint32_t index = 0, total_length = 0;
    int32_t result = A_OK;
    uint8_t free_buf = 1;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Find context*/
        if ((int32_t)(index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            free_buf = 0;
            break;
        }

        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            free_buf = 0;
            break;
        }
        /*Cannot call getsockopt with NULL*/
        if (optval == NULL || optlen == 0)
        {
            result = A_ERROR;
            free_buf = 0;
            break;
        }
        /*Total length depends upon the type of option*/
        total_length = sizeof(SOCK_OPT_T) + optlen - sizeof(uint8_t);

        /*Allocate buffer for option*/
        if ((sock_getopt = A_MALLOC(total_length, MALLOC_ID_CONTEXT)) == NULL)
        {
            result = A_NO_MEMORY;
            free_buf = 0;
            break;
        }

        sock_getopt->handle = A_CPU2LE32(handle);
        sock_getopt->level = A_CPU2LE32(level);
        sock_getopt->optname = A_CPU2LE32(optname);
        sock_getopt->optlen = A_CPU2LE32(optlen);

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_GETSOCKOPT);
        /*Send the packet*/
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_GETSOCKOPT, (uint8_t *)sock_getopt, total_length) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_GETSOCKOPT);
            result = A_ERROR;
            free_buf = 0;
            break;
        }

        /*Wait for response from stack*/
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_GETSOCKOPT));

        if (ath_sock_context[index]->data != NULL)
        {
            A_MEMCPY(optval, ((SOCK_OPT_T *)ath_sock_context[index]->data)->optval, optlen);
            // A_FREE(ath_sock_context[index]->data,MALLOC_ID_CONTEXT);
            ath_sock_context[index]->data = NULL; //TODO: check possible leak !!
        }
        result = ath_sock_context[index]->result;
    } while (0);

    A_FREE(sock_getopt, MALLOC_ID_CONTEXT);

    if (free_buf)
    {
        /*Free the netbuf*/
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }

    return result;
}

/*****************************************************************************/
/*  Api_ipconfig - API to obtain IP address information from target
 *  void *pCxt- driver context
 *  uint32_t mode- query vs set
 *  uint32_t* ipv4_addrA_UINT32* subnetMask, uint32_t* gateway4
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipconfig(void *pCxt,
                     uint32_t mode,
                     uint32_t *ipv4_addr,
                     uint32_t *subnetMask,
                     uint32_t *gateway4,
                     IP46ADDR *dnsaddr,
                     char *hostname)
{
    A_DRIVER_CONTEXT *pDCxt;
    IPCONFIG_CMD_T ipcfg = {0};
    IPCONFIG_RECV_T *result;
    uint32_t index = GLOBAL_SOCK_INDEX; // reserved for global commands ToDo- cleanup later
    int32_t res = A_OK;
    uint8_t free_buf = 1;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            res = A_ERROR;
            free_buf = 0;
            break;
        }
        if (IPCFG_STATIC == mode)
        {
            /*This is not a query or dhcp command*/
            ipcfg.mode = A_CPU2LE32(mode);
            ipcfg.ipv4 = A_CPU2LE32(*ipv4_addr);
            ipcfg.subnetMask = A_CPU2LE32(*subnetMask);
            ipcfg.gateway4 = A_CPU2LE32(*gateway4);
        }
        else if (IPCFG_AUTO == mode)
        {
            ipcfg.ipv4 = A_CPU2LE32(*ipv4_addr);
        }
        ipcfg.mode = A_CPU2LE32(mode);
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IPCONFIG);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IPCONFIG, (void *)(&ipcfg), sizeof(IPCONFIG_CMD_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IPCONFIG);
            res = A_ERROR;
            free_buf = 0;
            break;
        }

        do
        {
            if (IPCFG_DHCP == mode)
            {
                if (BLOCK(pCxt, ath_sock_context[index], DHCP_WAIT_TIME, RX_DIRECTION) != A_OK)
                {
                    return (int32_t)A_TIMEOUT;
                }
            }
            else if (IPCFG_AUTO == mode)
            {
                if (BLOCK(pCxt, ath_sock_context[index], DHCP_AUTO_WAIT_TIME, RX_DIRECTION) != A_OK)
                {
                    return (int32_t)A_TIMEOUT;
                }
            }
            else
            {
                if (BLOCK(pCxt, ath_sock_context[index], IPCONFIG_WAIT_TIME, RX_DIRECTION) != A_OK)
                {
                    return (int32_t)A_TIMEOUT;
                }
            }

        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IPCONFIG));

        /*Got a response*/
        if (ath_sock_context[index]->result != -1)
        {
            result = (IPCONFIG_RECV_T *)(ath_sock_context[index]->data);
            if (mode != 1)
            {
                if (ipv4_addr != NULL)
                    *ipv4_addr = A_CPU2LE32(result->ipv4);
                if (subnetMask != NULL)
                    *subnetMask = A_CPU2LE32(result->subnetMask);
                if (gateway4 != NULL)
                    *gateway4 = A_CPU2LE32(result->gateway4);
                if (dnsaddr != NULL)
                    memcpy(dnsaddr, &result->dnsaddr, sizeof(result->dnsaddr));
                if (hostname != NULL)
                    strcpy(hostname, result->hostname);
            }
        }
        res = ath_sock_context[index]->result;
    } while (0);

    if (free_buf)
    {
        /*Free the netbuf*/
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
    return res;
}

/*****************************************************************************/
/*  Api_ip6config - API to obtain IPv6 address information from target
 *  void *pCxt- driver context
 *  uint32_t mode- query vs set
 *
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ip6config(void *pCxt,
                      uint32_t mode,
                      IP6_ADDR_T *v6Global,
                      IP6_ADDR_T *v6Link,
                      IP6_ADDR_T *v6DefGw,
                      IP6_ADDR_T *v6GlobalExtd,
                      int32_t *LinkPrefix,
                      int32_t *GlbPrefix,
                      int32_t *DefgwPrefix,
                      int32_t *GlbPrefixExtd)
{
    A_DRIVER_CONTEXT *pDCxt;
    IPCONFIG_CMD_T ipcfg;
    IPCONFIG_RECV_T *result;
    uint32_t index = GLOBAL_SOCK_INDEX; // reserved for global commands ToDo- cleanup later
    int32_t res = A_OK;
    uint8_t free_buf = 1;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[GLOBAL_SOCK_INDEX]))
        {
            res = A_ERROR;
            free_buf = 0;
            break;
        }

        ipcfg.mode = A_CPU2LE32(mode);

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP6CONFIG);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP6CONFIG, (void *)(&ipcfg), sizeof(IPCONFIG_CMD_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP6CONFIG);
            res = A_ERROR;
            free_buf = 0;
            break;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }

        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IP6CONFIG));

        /*Got a response*/
        if (ath_sock_context[index]->result != -1)
        {
            result = (IPCONFIG_RECV_T *)(ath_sock_context[index]->data);

            A_MEMCPY(v6Global, &result->ipv6GlobalAddr, sizeof(IP6_ADDR_T));
            A_MEMCPY(v6Link, &result->ipv6LinkAddr, sizeof(IP6_ADDR_T));
            A_MEMCPY(v6DefGw, &result->ipv6DefGw, sizeof(IP6_ADDR_T));
            A_MEMCPY(v6GlobalExtd, &result->ipv6LinkAddrExtd, sizeof(IP6_ADDR_T));
            *LinkPrefix = A_CPU2LE32(result->LinkPrefix);
            *GlbPrefix = A_CPU2LE32(result->GlbPrefix);
            *DefgwPrefix = A_CPU2LE32(result->DefGwPrefix);
            *GlbPrefixExtd = A_CPU2LE32(result->GlbPrefixExtd);
        }
        res = ath_sock_context[index]->result;
    } while (0);

    if (free_buf)
    {
        /*Free the netbuf*/
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }

    return res;
}

/*****************************************************************************/
/*  Api_ipconfig_dhcp_pool - API to configure dhcp pool
 *  void *pCxt- driver context
 *  uint32_t *start_ipv4_addr -   Start ip address
 *  uint32_t* end_ipv4_addr int32_t leasetime
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipconfig_dhcp_pool(void *pCxt, uint32_t *start_ipv4_addr, uint32_t *end_ipv4_addr, int32_t leasetime)
{
    A_DRIVER_CONTEXT *pDCxt;
    IPCONFIG_DHCP_POOL_T ipcfg;
    uint32_t index = GLOBAL_SOCK_INDEX; // reserved for global commands ToDo- cleanup later
    pDCxt = GET_DRIVER_COMMON(pCxt);

    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    ipcfg.startaddr = A_CPU2LE32(*start_ipv4_addr);
    ipcfg.endaddr = A_CPU2LE32(*end_ipv4_addr);
    ipcfg.leasetime = A_CPU2LE32(leasetime);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IPCONFIG_DHCP_POOL, (void *)(&ipcfg), sizeof(IPCONFIG_DHCP_POOL_T)) != A_OK)
    {
        return A_ERROR;
    }

    return (A_OK);
}

/*****************************************************************************/
/*  Api_ip6config_router_prefix - API to configure router prefix
 *  void *pCxt- driver context
 *  uint8_t *v6addr -   v6prefix
 *  int32_t  prefixlen int32_t prefix_lifetime ,int32_t valid_lifetime
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ip6config_router_prefix(
    void *pCxt, IP6_ADDR_T *v6addr, int32_t prefixlen, int32_t prefix_lifetime, int32_t valid_lifetime)
{
    A_DRIVER_CONTEXT *pDCxt;
    IP6CONFIG_ROUTER_PREFIX_T ip6cfg;
    uint32_t index = GLOBAL_SOCK_INDEX; // reserved for global commands ToDo- cleanup later
    pDCxt = GET_DRIVER_COMMON(pCxt);

    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMCPY(ip6cfg.v6addr, v6addr, sizeof(IP6_ADDR_T));
    ip6cfg.prefixlen = A_CPU2LE32(prefixlen);
    ip6cfg.prefix_lifetime = A_CPU2LE32(prefix_lifetime);
    ip6cfg.valid_lifetime = A_CPU2LE32(valid_lifetime);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP6CONFIG_ROUTER_PREFIX, (void *)(&ip6cfg),
                       sizeof(IP6CONFIG_ROUTER_PREFIX_T)) != A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

/*****************************************************************************/
/*  Api_ipbridgemode - API to enable bridge mode
 *  void *pCxt- driver context
 *  int32_t status - enable the bridge mode
 * Returns- A_OK in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipbridgemode(void *pCxt, uint16_t status)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_BRIDGEMODE_T sock_ipbridgemode;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[GLOBAL_SOCK_INDEX]))
    {
        return A_ERROR;
    }
    sock_ipbridgemode.bridgemode = A_CPU2LE16(status);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_BRIDGEMODE, (void *)(&sock_ipbridgemode), sizeof(sock_ipbridgemode)) !=
        A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

/*****************************************************************************/
/*  Api_ipconfig_set_tcp_exponential_backoff_retry - API to tcp exponential backoff retry
 *  void *pCxt- driver context
 *  int32_t retry -   No of MAX Retries
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipconfig_set_tcp_exponential_backoff_retry(void *pCxt, int32_t retry)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_BACKOFF_T sock_backoff;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[GLOBAL_SOCK_INDEX]))
    {
        return A_ERROR;
    }
    sock_backoff.max_retry = A_CPU2LE32(retry);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SET_TCP_EXP_BACKOFF_RETRY, (void *)(&sock_backoff),
                       sizeof(SOCK_IP_BACKOFF_T)) != A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

/*****************************************************************************/
/*  Api_ipconfig_set_ip6_status - API to set ip6 status
 *  void *pCxt- driver context
 *  uint16_t status -   enable or diable
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipconfig_set_ip6_status(void *pCxt, uint16_t status)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IPv6_STATUS_T sock_ip6status;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[GLOBAL_SOCK_INDEX]))
    {
        return A_ERROR;
    }
    sock_ip6status.ipv6_status = A_CPU2LE16(status);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SET_IP6_STATUS, (void *)(&sock_ip6status), sizeof(sock_ip6status)) !=
        A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

/*****************************************************************************/
/*  Api_ipconfig_dhcp_release - API to release dhcp ip address
 *  void *pCxt- driver context
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipconfig_dhcp_release(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_DHCP_RELEASE_T release;
    uint16_t ifindx = 0;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[GLOBAL_SOCK_INDEX]))
    {
        return A_ERROR;
    }
    release.ifIndex = A_CPU2LE16(ifindx);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_DHCP_RELEASE, (void *)(&release), sizeof(SOCK_IP_DHCP_RELEASE_T)) !=
        A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

int32_t Api_ipconfig_set_tcp_rx_buffer(void *pCxt, int32_t rxbuf)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_TCP_RX_BUF_T sock_tcp_rx_buf;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[GLOBAL_SOCK_INDEX]))
    {
        return A_ERROR;
    }
    sock_tcp_rx_buf.rxbuf = A_CPU2LE32(rxbuf);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SET_TCP_RX_BUF, (void *)(&sock_tcp_rx_buf),
                       sizeof(SOCK_IP_TCP_RX_BUF_T)) != A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

#if ENABLE_HTTP_SERVER
int32_t Api_ip_http_server(void *pCxt, int32_t command)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_HTTP_SERVER_T sock_http_server;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    sock_http_server.enable = A_CPU2LE32(command);

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_HTTP_SERVER);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_HTTP_SERVER, (void *)(&sock_http_server), sizeof(SOCK_IP_HTTP_SERVER_T)) !=
        A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_HTTP_SERVER);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            A_ASSERT(0);
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_HTTP_SERVER));

    return (ath_sock_context[index]->result);
}

int32_t Api_ip_http_server_method(
    void *pCxt, int32_t command, uint8_t *pagename, uint8_t *objname, int32_t objtype, int32_t objlen, uint8_t *value)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_HTTP_SERVER_CMD_T sock_http_server;
    int index = GLOBAL_SOCK_INDEX;
    uint32_t *length;
    uint8_t *buf;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    sock_http_server.command = A_CPU2LE32(command);
    strcpy((char *)sock_http_server.pagename, (const char *)pagename);
    strcpy((char *)sock_http_server.objname, (const char *)objname);
    sock_http_server.objlen = A_CPU2LE32(objlen);
    if (command == HTTP_POST_METHOD) /* For POST command */
        strcpy((char *)sock_http_server.value, (const char *)value);

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_HTTP_SERVER_CMD);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_HTTP_SERVER_CMD, (void *)(&sock_http_server),
                       sizeof(SOCK_IP_HTTP_SERVER_CMD_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_HTTP_SERVER_CMD);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            A_ASSERT(0);
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_HTTP_SERVER_CMD));

    if ((ath_sock_context[index]->result != -1) && (command == HTTP_GET_METHOD))
    {
        length = (uint32_t *)ath_sock_context[index]->data;
        if (*length > 512)
            A_ASSERT(0);
        buf = (uint8_t *)(length + 1);
        A_MEMCPY(value, buf, *length);
        value[*length] = '\0'; // NULL terminating the string
    }

    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return (ath_sock_context[index]->result);
}
#endif

#if ENABLE_HTTP_CLIENT
/*****************************************************************************/
/*  Api_httpc_method - API to use http client methods
 *                      It is a non blocking call. Just sends the HTTP command and return
 *  void *pCxt- driver context
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/

int32_t Api_httpc_method(void *pCxt, uint32_t command, uint8_t *url, uint8_t *data, uint8_t **output)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_HTTPC_T httpc;
    int32_t index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }

    memset(&httpc, 0, sizeof(httpc));

    httpc.command = A_CPU2LE32(command);
    if (url)
        A_MEMCPY(httpc.url, url, strlen((const char *)url));
    if (data)
        A_MEMCPY(httpc.data, data, strlen((const char *)data));

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_HTTPC);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_HTTPC, (void *)(&httpc), sizeof(SOCK_HTTPC_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_HTTPC);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            A_ASSERT(0);
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_HTTPC));

    if ((ath_sock_context[index]->result != A_ERROR) && (output != NULL))
    {
        *output = ath_sock_context[index]->data;
        // printf("Adding Q %p %p\n", ath_sock_context[index]->pReq, ath_sock_context[index]->data);
        A_NETBUF_ENQUEUE(&zero_copy_free_queue, ath_sock_context[index]->pReq);
    }
    else
        A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return ath_sock_context[index]->result;
}

#endif /* ENABLE_HTTP_CLIENT */

#if ENABLE_DNS_CLIENT
extern uint32_t dns_block_time;

int32_t Api_ip_resolve_host_name(void *pCxt, DNC_CFG_CMD *DncCfg, DNC_RESP_INFO *DncRespInfo)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    int index = GLOBAL_SOCK_INDEX;
    DNC_RESP_INFO *result = NULL;
    int32_t res = A_OK;
    uint8_t free_buf = 1;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        free_buf = 0;
        return A_ERROR;
    }
    DncCfg->mode = A_CPU2LE32(DncCfg->mode);
    DncCfg->domain = A_CPU2LE32(DncCfg->domain);
    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_DNC_CMD);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_DNC_CMD, (void *)DncCfg, sizeof(DNC_CFG_CMD)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_DNC_CMD);
        free_buf = 0;
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], dns_block_time, RX_DIRECTION) != A_OK)
        {
            // printf("dnsclient timed out \r\n");
            return -1;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_DNC_CMD));

    if (ath_sock_context[index]->result != -1)
    {
        result = (DNC_RESP_INFO *)(ath_sock_context[index]->data);
        A_MEMCPY(DncRespInfo, result, sizeof(DNC_RESP_INFO));
    }
    res = ath_sock_context[index]->result;
    /*Free the netbuf*/
    if (free_buf)
    {
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
    return res;
}

int32_t Api_ip_dns_client(void *pCxt, int32_t command)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_DNS_CLIENT_T sock_dns_client;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    sock_dns_client.command = A_CPU2LE32(command);

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_DNC_ENABLE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_DNC_ENABLE, (void *)(&sock_dns_client), sizeof(SOCK_IP_DNS_CLIENT_T)) !=
        A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_DNC_ENABLE);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            A_ASSERT(0);
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_DNC_ENABLE));

    return (ath_sock_context[index]->result);
}

int32_t Api_ip_dns_server_addr(void *pCxt, IP46ADDR *addr)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_CFG_DNS_SRVR_ADDR sock_dns_srvr_addr;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }

    if (addr->type == ATH_AF_INET)
    {
        sock_dns_srvr_addr.addr.type = ATH_AF_INET;
        sock_dns_srvr_addr.addr.addr4 = addr->addr4;
        sock_dns_srvr_addr.addr.au1Rsvd[0] = addr->au1Rsvd[0];
    }
    if (addr->type == ATH_AF_INET6)
    {
        sock_dns_srvr_addr.addr.type = ATH_AF_INET6;
        sock_dns_srvr_addr.addr.au1Rsvd[0] = addr->au1Rsvd[0];
        memcpy(&sock_dns_srvr_addr.addr.addr6, &addr->addr6, sizeof(IP6_ADDR_T));
    }

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_DNS_SRVR_CFG_ADDR);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_DNS_SRVR_CFG_ADDR, (void *)(&sock_dns_srvr_addr),
                       sizeof(SOCK_IP_CFG_DNS_SRVR_ADDR)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_DNS_SRVR_CFG_ADDR);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            A_ASSERT(0);
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_DNS_SRVR_CFG_ADDR));

    return (ath_sock_context[index]->result);
}
#endif /* ENABLE_DNS_CLIENT */

int32_t Api_ip_hostname(void *pCxt, char *domain_name)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_CFG_HOST_NAME sock_ip_host_name;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    strcpy(sock_ip_host_name.domain_name, domain_name);
    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP_HOST_NAME);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_HOST_NAME, (void *)(&sock_ip_host_name),
                       sizeof(SOCK_IP_CFG_HOST_NAME)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP_HOST_NAME);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            A_ASSERT(0);
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IP_HOST_NAME));

    return (ath_sock_context[index]->result);
}

#if ENABLE_DNS_SERVER

int32_t Api_ip_dns_local_domain(void *pCxt, char *domain_name)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_CFG_DNS_LOCAL_DOMAIN sock_dns_local_domain;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    strcpy(sock_dns_local_domain.domain_name, domain_name);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_DNS_LOCAL_DOMAIN, (void *)(&sock_dns_local_domain),
                       sizeof(SOCK_IP_CFG_DNS_LOCAL_DOMAIN)) != A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

int32_t Api_ipdns(void *pCxt, int32_t command, char *domain_name, IP46ADDR *dnsaddr)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_DNS_T sock_ip_dns;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    memset(&sock_ip_dns, 0, sizeof(SOCK_IP_DNS_T));
    strcpy(sock_ip_dns.domain_name, domain_name);
    sock_ip_dns.command = A_CPU2LE32(command);
    // printf("api_dns addr type %d \r\n",dnsaddr->type);
    if (dnsaddr->type == ATH_AF_INET)
    {
        sock_ip_dns.addr.type = ATH_AF_INET;
        sock_ip_dns.addr.addr4 = dnsaddr->addr4;
        sock_ip_dns.addr.au1Rsvd[0] = dnsaddr->au1Rsvd[0];
    }
    if (dnsaddr->type == ATH_AF_INET6)
    {
        sock_ip_dns.addr.type = ATH_AF_INET6;
        sock_ip_dns.addr.au1Rsvd[0] = dnsaddr->au1Rsvd[0];
        memcpy(&sock_ip_dns.addr.addr6, &dnsaddr->addr6, sizeof(IP6_ADDR_T));
    }
    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP_DNS);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_DNS, (void *)(&sock_ip_dns), sizeof(SOCK_IP_DNS_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP_DNS);
        return A_ERROR;
    }
    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            A_ASSERT(0);
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IP_DNS));

    return (ath_sock_context[index]->result);
}

int32_t Api_ip_dns_server(void *pCxt, int32_t command)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;

    SOCK_IP_DNS_SERVER_STATUS_T sock_dns_server;
    int index = GLOBAL_SOCK_INDEX;
    pDCxt = GET_DRIVER_COMMON(pCxt);

    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    memset(&sock_dns_server, 0, sizeof(SOCK_IP_DNS_SERVER_STATUS_T));
    sock_dns_server.enable = A_CPU2LE32(command);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_DNS_SERVER_STATUS, (void *)(&sock_dns_server),
                       sizeof(SOCK_IP_DNS_SERVER_STATUS_T)) != A_OK)
    {
        return A_ERROR;
    }

    return (A_OK);
}

#endif /* ENABLE_DNS_SERVER */

#if ENABLE_SNTP_CLIENT
int32_t Api_ip_sntp_srvr_addr(void *pCxt, int32_t command, char *sntp_srvr_addr)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_CFG_SNTP_SRVR_ADDR sock_ip_sntp_srvr_addr;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    strcpy(sock_ip_sntp_srvr_addr.addr, sntp_srvr_addr);
    sock_ip_sntp_srvr_addr.command = A_CPU2LE32(command);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SNTP_SRVR_ADDR, (void *)(&sock_ip_sntp_srvr_addr),
                       sizeof(SOCK_IP_CFG_SNTP_SRVR_ADDR)) != A_OK)
    {
        return A_ERROR;
    }
    return (A_OK);
}

int32_t Api_ip_sntp_get_time(void *pCxt, tSntpTime *SntpTime)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;
    uint8_t free_buf = 1;
    tSntpTime *result = NULL;
    uint8_t buffer[sizeof(WMI_SOCKET_CMD) + 4];
    WMI_SOCKET_CMD *sock_cmd = (WMI_SOCKET_CMD *)&buffer[0];

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        free_buf = 0;
        return A_ERROR;
    }
    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP_SNTP_GET_TIME);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SNTP_GET_TIME, (void *)&sock_cmd, (sizeof(WMI_SOCKET_CMD) + 4)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP_SNTP_GET_TIME);
        free_buf = 0;
        return A_ERROR;
    }
    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], (DHCP_WAIT_TIME * 2), RX_DIRECTION) != A_OK)
        {
            return -1;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IP_SNTP_GET_TIME));

    if (ath_sock_context[index]->result != -1)
    {
        result = (tSntpTime *)(ath_sock_context[index]->data);
        A_MEMCPY(SntpTime, result, sizeof(tSntpTime));
    }
    res = ath_sock_context[index]->result;
    /*Free the netbuf*/
    if (free_buf)
    {
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
    return res;
}

int32_t Api_ip_sntp_get_time_of_day(void *pCxt, tSntpTM *SntpTm)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;
    uint8_t free_buf = 1;
    tSntpTM *result = NULL;
    uint8_t buffer[sizeof(WMI_SOCKET_CMD) + 4];
    WMI_SOCKET_CMD *sock_cmd = (WMI_SOCKET_CMD *)&buffer[0];

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        free_buf = 0;
        return A_ERROR;
    }

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP_SNTP_GET_TIME_OF_DAY);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SNTP_GET_TIME_OF_DAY, (void *)&sock_cmd, (sizeof(WMI_SOCKET_CMD) + 4)) !=
        A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP_SNTP_GET_TIME_OF_DAY);
        free_buf = 0;
        return A_ERROR;
    }
    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], (DHCP_WAIT_TIME * 2), RX_DIRECTION) != A_OK)
        {
            return -1;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IP_SNTP_GET_TIME_OF_DAY));

    if (ath_sock_context[index]->result != -1)
    {
        result = (tSntpTM *)(ath_sock_context[index]->data);
        A_MEMCPY(SntpTm, result, sizeof(tSntpTM));
    }
    res = ath_sock_context[index]->result;
    /*Free the netbuf*/
    if (free_buf)
    {
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
    return res;
}

int32_t Api_ip_sntp_modify_zone_dse(void *pCxt, uint8_t hr, uint8_t mn, uint8_t zone_cal, uint8_t dse_en_dis)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;
    SOCK_SNTP_MODIFY_TIMEZONE sock_ip_sntp_time_zone;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    sock_ip_sntp_time_zone.hour = hr;
    sock_ip_sntp_time_zone.min = mn;
    sock_ip_sntp_time_zone.add_sub = zone_cal;
    sock_ip_sntp_time_zone.dse = dse_en_dis;

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP_SNTP_CONFIG_TIMEZONE_DSE);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SNTP_CONFIG_TIMEZONE_DSE, (void *)(&sock_ip_sntp_time_zone),
                       sizeof(SOCK_SNTP_MODIFY_TIMEZONE)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP_SNTP_CONFIG_TIMEZONE_DSE);
        return A_ERROR;
    }

    return res;
}

int32_t Api_ip_sntp_query_srvr_address(void *pCxt, tSntpDnsAddr SntpDnsAddr[MAX_SNTP_SERVERS])
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;
    uint8_t free_buf = 1;
    tSntpDnsAddr *result = NULL;
    SOCK_IP_QUERY_SNTP_CONFIG sock_ip_query_sntp_config;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        free_buf = 0;
        return A_ERROR;
    }

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP_SNTP_QUERY_SNTP_ADDRESS);
    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SNTP_QUERY_SNTP_ADDRESS, (void *)(&sock_ip_query_sntp_config),
                       sizeof(SOCK_IP_QUERY_SNTP_CONFIG)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP_SNTP_QUERY_SNTP_ADDRESS);
        free_buf = 0;
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], (DHCP_WAIT_TIME * 2), RX_DIRECTION) != A_OK)
        {
            return -1;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IP_SNTP_QUERY_SNTP_ADDRESS));

    if (ath_sock_context[index]->result != -1)
    {
        result = (tSntpDnsAddr *)(ath_sock_context[index]->data);
        A_MEMCPY(SntpDnsAddr, result, (sizeof(tSntpDnsAddr) * MAX_SNTP_SERVERS));
    }
    res = ath_sock_context[index]->result;
    /*Free the netbuf*/
    if (free_buf)
    {
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
    return res;
}

int32_t Api_ip_sntp_client(void *pCxt, int32_t command)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IP_SNTP_CLIENT_T sock_sntp_client;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    sock_sntp_client.command = A_CPU2LE32(command);

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IP_SNTP_CLIENT_ENABLE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IP_SNTP_CLIENT_ENABLE, (void *)(&sock_sntp_client),
                       sizeof(SOCK_IP_SNTP_CLIENT_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IP_SNTP_CLIENT_ENABLE);
        return A_ERROR;
    }

    return (res);
}

#endif /* ENABLE_SNTP_CLIENT */

#if ENABLE_ROUTING_CMDS
/*****************************************************************************/
/*  Api_ipv4_route - API to add, del and show IPv4 routes
 *  void *pCxt- driver context
 *  uint32_t* ipv4_addr
 *  uint32_t* subnetMask
 *  uint32_t* gateway
 *  uint32_t* ifIndex
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipv4_route(void *pCxt,
                       uint32_t command,
                       IP_ADDR_T *ipv4_addr,
                       IP_ADDR_T *subnetMask,
                       IP_ADDR_T *gateway,
                       uint32_t *ifIndex,
                       IPV4_ROUTE_LIST_T *routelist)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IPV4_ROUTE_T sock_ipv4_route;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    sock_ipv4_route.command = A_CPU2LE32(command);
    sock_ipv4_route.address = A_CPU2LE32(ipv4_addr->s_addr);
    sock_ipv4_route.mask = A_CPU2LE32(subnetMask->s_addr);
    sock_ipv4_route.gateway = A_CPU2LE32(gateway->s_addr);
    sock_ipv4_route.ifIndex = A_CPU2LE32(*ifIndex);

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IPV4_ROUTE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IPV4_ROUTE, (void *)(&sock_ipv4_route), sizeof(SOCK_IPV4_ROUTE_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IPV4_ROUTE);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IPV4_ROUTE));

    if (command == 2)
    {
        A_MEMCPY(routelist, ath_sock_context[index]->data, sizeof(IPV4_ROUTE_LIST_T));
    }
    A_NETBUF_FREE(ath_sock_context[index]->pReq);
    return (res);
}

/*****************************************************************************/
/*  Api_ipv6_route - API to add, del and show IPv6 routes
 *  void *pCxt- driver context
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ipv6_route(void *pCxt,
                       uint32_t command,
                       IP6_ADDR_T *ip6addr,
                       uint32_t *prefixLen,
                       IP6_ADDR_T *gateway,
                       uint32_t *ifIndex,
                       IPV6_ROUTE_LIST_T *routelist)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_IPV6_ROUTE_T sock_ipv6_route;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    /*Check if socket is blocked for a previous command*/
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    sock_ipv6_route.command = A_CPU2LE32(command);
    A_MEMCPY(sock_ipv6_route.address, ip6addr->s6_addr, sizeof(sock_ipv6_route.address));
    sock_ipv6_route.prefixLen = A_CPU2LE32(*prefixLen);
    A_MEMCPY(sock_ipv6_route.nexthop, gateway, sizeof(sock_ipv6_route.nexthop));
    sock_ipv6_route.ifIndex = A_CPU2LE32(*ifIndex);

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_IPV6_ROUTE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_IPV6_ROUTE, (void *)(&sock_ipv6_route), sizeof(SOCK_IPV6_ROUTE_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_IPV6_ROUTE);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_IPV6_ROUTE));

    if (command == 2)
    {
        A_MEMCPY(routelist, ath_sock_context[index]->data, sizeof(IPV6_ROUTE_LIST_T));
    }
    A_NETBUF_FREE(ath_sock_context[index]->pReq);
    return (res);
}
#endif /* ENABLE_ROUTING_CMDS */
       /*****************************************************************************/
       /*  Api_tcp_connection_timeout - API to set TCp Connection Timeout
        *  void *pCxt- driver context
        * Returns- 0 on Success , A_ERROR otherwise
        *****************************************************************************/
int32_t Api_tcp_connection_timeout(void *pCxt, uint32_t timeout_val)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_TCP_CONN_TIMEOUT_T sock_tcp_conn_timeout;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&sock_tcp_conn_timeout, sizeof(SOCK_TCP_CONN_TIMEOUT_T));
    sock_tcp_conn_timeout.timeout_val = timeout_val;

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_TCP_CONN_TIMEOUT);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_TCP_CONN_TIMEOUT, (void *)(&sock_tcp_conn_timeout),
                       sizeof(SOCK_TCP_CONN_TIMEOUT_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_TCP_CONN_TIMEOUT);
        return A_ERROR;
    }
    return A_OK;
}
/*****************************************************************************/
/*  Api_ota_upgrade - API to intialize OTA upgrade
 *  void *pCxt- driver context
 * Returns- 0 in case of successful OTA, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_upgrade(void *pCxt,
                        uint32_t addr,
                        char *filename,
                        uint8_t mode,
                        uint8_t preserve_last,
                        uint8_t protocol,
                        uint32_t *resp_code,
                        uint32_t *length)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_OTA_UPGRADE_T sock_ota_upgrade;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&sock_ota_upgrade, sizeof(SOCK_OTA_UPGRADE_T));
    sock_ota_upgrade.ipaddress = addr;
    A_MEMCPY(sock_ota_upgrade.filename, filename, strlen(filename));
    sock_ota_upgrade.mode = mode;
    sock_ota_upgrade.preserve_last = preserve_last;
    sock_ota_upgrade.protocol = protocol;

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_UPGRADE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_UPGRADE, (void *)(&sock_ota_upgrade), sizeof(SOCK_OTA_UPGRADE_T)) !=
        A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_UPGRADE);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_UPGRADE));

    *resp_code = ((tOtaUpgradeResp *)(ath_sock_context[index]->data))->resp_code;
    *length = ((tOtaUpgradeResp *)(ath_sock_context[index]->data))->size;
    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return A_OK;
}
/*****************************************************************************/
/*  Api_ota_read - API to read OTA upgrade area
 *  void *pCxt- driver context
 * Returns- 0 in case of successful OTA READ, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_read_area(void *pCxt, uint32_t offset, uint32_t size, uint8_t *buffer, uint32_t *ret_len)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_OTA_READ_OTA_AREA_T sock_ota_read;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&sock_ota_read, sizeof(SOCK_OTA_READ_OTA_AREA_T));
    sock_ota_read.offset = offset;
    sock_ota_read.size = size;

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_READ);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_READ, (void *)(&sock_ota_read), sizeof(SOCK_OTA_READ_OTA_AREA_T)) !=
        A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_READ);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_READ));

    // resp->resp_code=((tOtaReadResp*)(ath_sock_context[index]->data))->resp_code;
    *ret_len = ((tOtaReadResp *)(ath_sock_context[index]->data))->size;

    A_MEMCPY(buffer, (void*)&((tOtaReadResp *)(ath_sock_context[index]->data))->data, *ret_len);

    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return A_OK;
}
/*****************************************************************************/
/*  Api_ota_done - API to send OTA Complete event to target
 *  void *pCxt- driver context
 * Returns- 0 in case of successful OTA , A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_done(void *pCxt, boolean good_image)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_OTA_DONE_T sock_ota_done;
    int index = GLOBAL_SOCK_INDEX;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&sock_ota_done, sizeof(SOCK_OTA_DONE_T));
    sock_ota_done.data = good_image;

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_DONE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_DONE, (void *)(&sock_ota_done), sizeof(SOCK_OTA_DONE_T)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_DONE);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_DONE));

    // A_MEMCPY(resp, ath_sock_context[index]->data, sizeof(tOtaDoneResp));

    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return A_OK;
}

/*****************************************************************************/
/*  Api_ota_session_start - API to send OTA Session Start command to target
 *  A_VOID *pCxt- driver context
 * Returns- 0 in case of successful OTA , A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_session_start(void *pCxt, uint32_t flags, uint32_t partition_index)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_OTA_SESSION_START_T ota_cmd;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = A_OK;
    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&ota_cmd, sizeof(ota_cmd));
    ota_cmd.flags = flags;
    ota_cmd.partition_index = partition_index;

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_SESSION_START);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_SESSION_START, (void *)(&ota_cmd), sizeof(ota_cmd)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_SESSION_START);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_SESSION_START));
    res = ((tOtaResp *)(ath_sock_context[index]->data))->resp_code;
    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return res;
}

/*****************************************************************************/
/*  Api_ota_partition_get_size - API to send OTA Partition Get Size command to target
 *  A_VOID *pCxt- driver context
 * Returns- partition size in case of successful OTA , 0 otherwise
 *****************************************************************************/
uint32_t Api_ota_partition_get_size(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    struct sock_ota_cmd_s
    {
        WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
        uint8_t data[1];
    } POSTPACK ota_cmd;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return 0;
    }
    A_MEMZERO(&ota_cmd, sizeof(ota_cmd));

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_PARTITION_GET_SIZE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_PARTITION_GET_SIZE, (void *)(&ota_cmd), sizeof(ota_cmd)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_PARTITION_GET_SIZE);
        return 0;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return 0;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_PARTITION_GET_SIZE));

    res = ((tOtaPartitionGetSizeResp *)(ath_sock_context[index]->data))->size;
    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return res;
}

/*****************************************************************************/
/*  Api_ota_partition_erase - API to send OTA Partition Erase command to target
 *  A_VOID *pCxt- driver context
 * Returns- 0 in case of successful OTA , A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_partition_erase(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    struct sock_ota_cmd_s
    {
        WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
        uint8_t data[1];
    } POSTPACK ota_cmd;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&ota_cmd, sizeof(ota_cmd));

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_PARTITION_ERASE);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_PARTITION_ERASE, (void *)(&ota_cmd), sizeof(ota_cmd)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_PARTITION_ERASE);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_PARTITION_ERASE));

    res = ((tOtaResp *)(ath_sock_context[index]->data))->resp_code;
    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return res;
}

/*****************************************************************************/
/*  Api_ota_partition_verify_checksum - API to send OTA Partition Verify Checksum command to target
 *  A_VOID *pCxt- driver context
 * Returns- 0 in case of successful OTA , A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_partition_verify_checksum(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    struct sock_ota_cmd_s
    {
        WMI_SOCKET_CMD wmi_cmd FIELD_PACKED;
        uint8_t data[1];
    } POSTPACK ota_cmd;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&ota_cmd, sizeof(ota_cmd));

    SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_PARTITION_VERIFY_CHECKSUM);

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_PARTITION_VERIFY_CHECKSUM, (void *)(&ota_cmd), sizeof(ota_cmd)) != A_OK)
    {
        SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_PARTITION_VERIFY_CHECKSUM);
        return A_ERROR;
    }

    do
    {
        if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
        {
            return A_ERROR;
        }
    } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_PARTITION_VERIFY_CHECKSUM));

    res = ((tOtaResp *)(ath_sock_context[index]->data))->resp_code;
    A_NETBUF_FREE(ath_sock_context[index]->pReq);

    return res;
}

/*****************************************************************************/
/*  Api_ota_parse_image_hdr - API to send OTA Parse Image Header command to target
 *  A_VOID *pCxt- driver context
 * Returns- 0 in case of successful OTA , A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_parse_image_hdr(void *pCxt, uint8_t *header, uint32_t *offset)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_OTA_PARSE_IMAGE_HDR_T ota_cmd;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        return A_ERROR;
    }
    A_MEMZERO(&ota_cmd, sizeof(ota_cmd));
    A_MEMCPY(ota_cmd.header, header, 24); // copy ota image header

    if (GET_DRIVER_CXT(pCxt)->otaCB != NULL) // non blocking */
    {
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_PARSE_IMAGE_HDR, (void *)(&ota_cmd), sizeof(ota_cmd)) != A_OK)
        {
            return A_ERROR;
        }
        else
        {
            return A_PENDING;
        }
    }
    else
    {
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_PARSE_IMAGE_HDR);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_PARSE_IMAGE_HDR, (void *)(&ota_cmd), sizeof(ota_cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_PARSE_IMAGE_HDR);
            return A_ERROR;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                return A_ERROR;
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_PARSE_IMAGE_HDR));

        *offset = ((tOtaParseImageHdrResp *)(ath_sock_context[index]->data))->offset;
        res = ((tOtaParseImageHdrResp *)(ath_sock_context[index]->data))->resp_code;
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
    return res;
}

/*****************************************************************************/
/*  Api_ota_partition_write_data - API to send OTA Partition Write Data command to target
 *  A_VOID *pCxt- driver context
 * Returns- 0 in case of successful OTA , A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ota_partition_write_data(void *pCxt, uint32_t offset, uint8_t *buf, uint32_t buf_size, uint32_t *ret_size)
{
    A_DRIVER_CONTEXT *pDCxt = NULL;
    SOCK_OTA_PARTITON_WRITE_DATA_T *ota_cmd = NULL;
    int index = GLOBAL_SOCK_INDEX;
    int32_t res = QCOM_OTA_ERR_WRITE_DATA_ERROR;
    uint32_t cmd_len;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    // Check if socket is blocked for a previous command
    if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
    {
        goto ota_error;
    }

    *ret_size = 0;
    if (buf_size == 0)
        return QCOM_OTA_OK;
    if ((buf == NULL) || (buf_size > MAX_OTA_AREA_READ_SIZE))
        return QCOM_OTA_ERR_WRITE_DATA_ERROR;

    cmd_len = sizeof(SOCK_OTA_PARTITON_WRITE_DATA_T) + buf_size - 1;

    ota_cmd = A_MALLOC(cmd_len, MALLOC_ID_CONTEXT);
    if (ota_cmd == NULL)
    {
        return A_NO_MEMORY;
    }

    A_MEMZERO(ota_cmd, cmd_len);
    A_MEMCPY(ota_cmd->data, buf, buf_size);
    ota_cmd->offset = offset;
    ota_cmd->size = buf_size;

    if (GET_DRIVER_CXT(pCxt)->otaCB != NULL) // non blocking */
    {
        if ((res = wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_PARTITION_WRITE_DATA, (void *)(ota_cmd), cmd_len)) != A_OK)
        {
            goto ota_error;
        }
        else
        {
            res = A_PENDING;
        }
    }
    else
    {
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_OTA_PARTITION_WRITE_DATA);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_OTA_PARTITION_WRITE_DATA, (void *)(ota_cmd), cmd_len) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_OTA_PARTITION_WRITE_DATA);
            goto ota_error;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                goto ota_error;
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_OTA_PARTITION_WRITE_DATA));

        *ret_size = ((tOtaPartitionWriteDataResp *)(ath_sock_context[index]->data))->size;
        res = ((tOtaPartitionWriteDataResp *)(ath_sock_context[index]->data))->resp_code;
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    }
ota_error:
    if (ota_cmd != NULL)
        A_FREE(ota_cmd, MALLOC_ID_CONTEXT);
    return res;
}

/*****************************************************************************/
/*  Api_ping - API to obtain IP address information from target
 *  void *pCxt- driver context
 *  uint32_t* ipv4_addr
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ping(void *pCxt, uint32_t ipv4_addr, uint32_t size, uint32_t ms_interval)
{
    A_DRIVER_CONTEXT *pDCxt;
    PING_T ping;
    uint32_t index = GLOBAL_SOCK_INDEX; // reserved for global commands ToDo- cleanup later
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }
        ping.ip_addr = A_CPU2LE32(ipv4_addr);
        ping.size = A_CPU2LE32(size);

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_PING);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_PING, (void *)(&ping), sizeof(PING_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_PING);
            result = A_ERROR;
            break;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], ms_interval, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }

        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_PING));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/*  Api_ping6 - API to obtain IP address information from target
 *  void *pCxt- driver context
 *  uint32_t* ipv4_addr
 * Returns- 0 in case of successful connect, A_ERROR otherwise
 *****************************************************************************/
int32_t Api_ping6(void *pCxt, uint8_t *ip6addr, uint32_t size, uint32_t ms_interval)
{
    A_DRIVER_CONTEXT *pDCxt;
    PING_6_T ping6;
    uint32_t index = GLOBAL_SOCK_INDEX; // reserved for global commands ToDo- cleanup later
    int32_t result = A_OK;

    pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }
        A_MEMCPY(ping6.ip6addr, ip6addr, sizeof(ping6.ip6addr));
        ping6.size = A_CPU2LE32(size);

        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_PING6);

        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_PING6, (void *)(&ping6), sizeof(PING_6_T)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_PING6);
            result = A_ERROR;
            break;
        }

        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_PING6));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/*  clear_socket_context - clears all elements of socket context
 *  int32_t - index to socket context
 * Returns- None
 *****************************************************************************/
void clear_socket_context(int32_t index)
{
    ath_sock_context[index]->handle = 0;
    ath_sock_context[index]->domain = 0;
    ath_sock_context[index]->type = 0;
    ath_sock_context[index]->TCPCtrFlag = 0;
    A_MEMZERO(&ath_sock_context[index]->sock_st_mask[0], sizeof(ath_sock_context[index]->sock_st_mask));

    /* Free remaining data when socket is closed */
    if (ath_sock_context[index]->old_netbuf != NULL)
    {
        A_NETBUF_FREE(ath_sock_context[index]->old_netbuf);
        ath_sock_context[index]->old_netbuf = NULL;
    }
    ath_sock_context[index]->remaining_bytes = 0;

    /*If for some reason, data field is not freed, free it here
      This may happen when a command response comes later than Timeout period*/
    if (ath_sock_context[index]->data)
    {
        QCADRV_PRINTF("%s index:%d data:%p\n", __func__, index, ath_sock_context[index]->data);
        A_FREE(ath_sock_context[index]->data, MALLOC_ID_CONTEXT);
        ath_sock_context[index]->data = NULL;
    }
}

/*****************************************************************************/
/*  send_stack_init - Sends Stack initialization information.
 * RETURNS: A_OK on success A_ERROR otherwise
 *****************************************************************************/
A_STATUS send_stack_init(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt;
    uint8_t buffer[sizeof(WMI_SOCKET_CMD) + 4];
    WMI_SOCKET_CMD *sock_cmd = (WMI_SOCKET_CMD *)&buffer[0];
    pDCxt = GET_DRIVER_COMMON(pCxt);

    buffer[8] = 1;
    buffer[9] = 2;
    buffer[10] = MAX_SOCKETS_SUPPORTED;

    if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_STACK_INIT, (void *)(sock_cmd), sizeof(WMI_SOCKET_CMD) + 4) != A_OK)
    {
        return A_ERROR;
    }

#if ENABLE_SSL
    sslhandle = (GET_DRIVER_CXT(pCxt)->pUpperCxt[0]);
#endif

    return A_OK;
}

enum
{
    /* Least priority since we dont allow any operation during store-recall */
    POWER_STATE_MOVED_FOR_STRRCL = 1,
    /* IPDHCP will happen only after the network connection usually */
    POWER_STATE_MOVED_FOR_IPDHCP,
    POWER_STATE_MOVED_FOR_TCPRX
};

int32_t power_state_for_module = 0;
/*****************************************************************************/
/*  move_to_maxperf - Changes the power mode to max perf in firmware. There are
 *                    some commands which need to run in MAX_PERF mode like store_recall etc
 *                    This function changes the power modeto MAX_PERF and will not
 *                    intimate the application. Later after the command succeeds,
 *                    use restore_power_state to change to original power state
 *                    Note: Application should not directly use this function
 *  int32_t - module which request the change
 *  Returns- A_OK on success, A_ERROR otherwise
 *****************************************************************************/
A_STATUS move_power_state_to_maxperf(void *pCxt, int32_t module)
{
    A_DRIVER_CONTEXT *pDCxt = (A_DRIVER_CONTEXT *)pCxt;

    /* Already some HIGH module changes state */
    if (power_state_for_module >= module)
    {
        return A_OK;
    }

    /* Change the power mode only when the current power mode is REC_POWER */
    if (pDCxt->userPwrMode == REC_POWER)
    {
        if (A_OK != wmi_cmd_start(pDCxt->pWmiCxt, &max_performance_power_param, WMI_SET_POWER_MODE_CMDID,
                                  sizeof(WMI_POWER_MODE_CMD)))
        {
            return A_ERROR;
        }
    }
    power_state_for_module = module;
    return A_OK;
}
/*****************************************************************************/
/*  restore_power_state - Restores the original power state, which is changed by
 *                        "move_power_state_to_maxperf"
 *  int32_t - module which request the change
 * RETURNS: A_OK on success A_ERROR otherwise
 *****************************************************************************/
A_STATUS restore_power_state(void *pCxt, int32_t module)
{
    A_DRIVER_CONTEXT *pDCxt = (A_DRIVER_CONTEXT *)pCxt;

    /* Already some HIGH priority module changes state,
     * so don't change the state now */
    if (!power_state_for_module || (power_state_for_module > module))
    {
        return A_OK;
    }
    if (pDCxt->userPwrMode != max_performance_power_param)
    {
        if (A_OK !=
            wmi_cmd_start(pDCxt->pWmiCxt, &pDCxt->userPwrMode, WMI_SET_POWER_MODE_CMDID, sizeof(WMI_POWER_MODE_CMD)))
        {
            return A_ERROR;
        }
    }

    power_state_for_module = 0;
    return A_OK;
}
#if ENABLE_SSL
/*****************************************************************************/
/* SSL_ctx_new - Create new SSL context. This function must be called before
 *               using any other SSL functions. It needs to be called as either
 *               server or client
 * Sslrole role - 1 = server, 2 = client
 * int32_t inbufSize - initial inBuf size: Can grow
 * int32_t outbufSize - outBuf size: Fixed
 * int32_t reserved - currently not used (must be zero)
 * Returns - SSL context handle on success or NULL on error (out of memory)
 *****************************************************************************/
SSL_CTX *Api_SSL_ctx_new(void *pCxt, SSL_ROLE_T role, int32_t inbufSize, int32_t outbufSize, int32_t reserved)
{
    SOCK_SSL_CTX_NEW_T cmd;
    SSL_CTX *result = NULL;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    int32_t index = GLOBAL_SOCK_INDEX;

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            break;
        }

        /* Send command and wait for response */
        cmd.role = role;
        cmd.inbufSize = inbufSize;
        cmd.outbufSize = outbufSize;
        cmd.reserved = reserved;
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_CTX_NEW);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_CTX_NEW, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_CTX_NEW);
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_CTX_NEW));
        result = (SSL_CTX *)ath_sock_context[GLOBAL_SOCK_INDEX]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/* SSL_ctx_free - Free the SSL context
 * SSL_CTX *ctx - sslContext
 *****************************************************************************/
int32_t Api_SSL_ctx_free(void *pCxt, SSL_CTX *ctx)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    SOCK_SSL_CTX_FREE_T cmd;
    int32_t result = A_OK;
    int32_t index = GLOBAL_SOCK_INDEX;

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        /* Send command and wait for response */
        cmd.ctx = (uint32_t)ctx;
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_CTX_FREE);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_CTX_FREE, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_CTX_FREE);
            result = A_ERROR;
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_CTX_FREE));
        result = ath_sock_context[GLOBAL_SOCK_INDEX]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/* SSL_new - Create SSL connection object. When SSL transaction is done, close
 *           it with ssl_shutdown().
 *           Note that the socket should subsequently also be closed.
 * SSL_CTX *ctx - sslContext
 * Return - SSL object handle on success or NULL on error (out of memory)
 *****************************************************************************/
SSL *Api_SSL_new(void *pCxt, SSL_CTX *ctx)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    SOCK_SSL_NEW_T cmd;
    SSL *result = NULL;
    int32_t index = GLOBAL_SOCK_INDEX;

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            break;
        }

        /* Send command and wait for response */
        cmd.ctx = (uint32_t)ctx;
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_NEW);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_NEW, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_NEW);
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_NEW));
        result = (SSL *)ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/* SSL_set_fd - Attach given socket descriptor to the SSL connection
 * SSL *ssl - SSL connection
 * uint32_t fd - Socket descriptor
 * Return - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t Api_SSL_set_fd(void *pCxt, SSL *ssl, uint32_t fd)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    SOCK_SSL_SET_FD_T cmd;
    int32_t result = A_OK;
    int32_t index;

    do
    {
        /* Find the socket */
        index = find_socket_context(fd, true);
        if (index == SOCKET_NOT_FOUND)
        {
            return A_ERROR;
        }

        /* Check if socket is blocked for a previous command */
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            return A_ERROR;
        }

        /* Store the ssl object pointer in the socket struct */
        ath_sock_context[index]->ssl = ssl;

        /* Send command and wait for response */
        cmd.ssl = (uint32_t)ssl;
        cmd.fd = fd;
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_SET_FD);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_SET_FD, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_SET_FD);
            result = A_ERROR;
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_SET_FD));
        result = ath_sock_context[index]->result;
    } while (0);

    if (result < 0)
    {
        /* remove the ssl pointer again if the cmd failed */
        ath_sock_context[index]->ssl = NULL;
    }

    return result;
}

/*****************************************************************************/
/* SSL_accept - Initiate SSL handshake.
 * SSL *ssl - SSL connection
 * Returns - 1 on success, ESSL_HSDONE if handshake has already been performed.
 *           Negative error code otherwise.
 *****************************************************************************/
int32_t Api_SSL_accept(void *pCxt, SSL *ssl)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    SOCK_SSL_ACCEPT_T cmd;
    int32_t result = A_OK;
    int32_t index;

    do
    {
        /* Find the socket used with this SSL connection */
        index = find_socket_context_from_ssl(ssl);
        if (index == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }

        /* Check if socket is blocked for a previous command */
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        /* Send command and wait for response */
        cmd.ssl = (uint32_t)ssl;
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_ACCEPT);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_ACCEPT, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_ACCEPT);
            result = A_ERROR;
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_ACCEPT));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/* SSL_connect - Initiate SSL handshake.
 * SSL *ssl - SSL connection
 * Returns - 1 on success, ESSL_HSDONE if handshake has already been performed.
 *           Negative error code otherwise.
 *****************************************************************************/
int32_t Api_SSL_connect(void *pCxt, SSL *ssl)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_SSL_ACCEPT_T cmd;
    int32_t result = A_OK;
    int32_t index;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    do
    {
        index = find_socket_context_from_ssl(ssl);
        if (index == SOCKET_NOT_FOUND)
        {
            result = A_ERROR;
            break;
        }

        /* Check if socket is blocked for a previous command */
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        /* Send command and wait for response */
        cmd.ssl = (uint32_t)ssl;
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_CONNECT);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_CONNECT, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_CONNECT);
            result = A_ERROR;
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_CONNECT));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/* SSL_shutdown - Close SSL connection.
 *                The socket must be closed by other means.
 * SSL *ssl - SSL connection
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t Api_SSL_shutdown(void *pCxt, SSL *ssl)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    ;
    SOCK_SSL_SHUTDOWN_T cmd;
    int32_t result = A_OK;
    int32_t index = find_socket_context_from_ssl(ssl);

    if (index == SOCKET_NOT_FOUND)
    {
        /* We send the shutdown command on the "global socket" if the socket used for
           the SSL is disconnected/closed before the SSL connection is shutdown. */
        index = GLOBAL_SOCK_INDEX;
    }
    do
    {
        /* Check if socket is blocked for a previous command */
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        /* Send command and wait for response */
        cmd.ssl = (uint32_t)ssl;
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_SHUTDOWN);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_SHUTDOWN, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_SHUTDOWN);
            result = A_ERROR;
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_SHUTDOWN));
        result = ath_sock_context[index]->result;

        /* clear the SSL object ptr in the socket struct */
        ath_sock_context[index]->ssl = NULL;
    } while (0);

    return result;
}

int32_t Api_SSL_configure(void *pCxt, SSL *ssl, SSL_CONFIG *cfg)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    ;
    SOCK_SSL_CONFIGURE_T cmd;
    int32_t result = A_OK;
    int32_t index = find_socket_context_from_ssl(ssl);

    if (index == SOCKET_NOT_FOUND)
    {
        /* We send the configure command on the "global socket" if the configure is
         * done before the socket is created */
        index = GLOBAL_SOCK_INDEX;
    }
    do
    {
        /* Check if socket is blocked for a previous command */
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        /* Send command and wait for response */
        cmd.ssl = (uint32_t)ssl;
        cmd.size = sizeof(SSL_CONFIG);
        memcpy(cmd.data, cfg, sizeof(SSL_CONFIG));
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_CONFIGURE);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_CONFIGURE, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_CONFIGURE);
            result = A_ERROR;
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_CONFIGURE));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

/*****************************************************************************/
/* SSL_addCert - Add a certificate or Certificate Authority (CA) list to the
 *               SharkSsl object.
 *               A SharkSsl object in server mode is required to have at least
 *               one certificate.
 *               You can only set one CA list, thus the CA list must include
 *               all root certificates required for the session.
 * SSL *ssl - SSL connection
 * SslCert cert -address of array of binary data
 * uint32_t size - size of array
 * SSL_CERT_TYPE_T type - certificate or CA list
 * Returns - 1 on success or negative error code on error (see SslErrors)
 *****************************************************************************/
int32_t Api_SSL_addCert(void *pCxt, SSL_CTX *ctx, SSL_CERT_TYPE_T type, uint8_t *cert, uint32_t size)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_SSL_ADD_CERT_T *pCmd;
    int32_t result = A_OK;
    uint32_t n, buffSize, offset = 0;
    void *pBuff;
    int index = GLOBAL_SOCK_INDEX;
    uint8_t *data = (uint8_t *)cert;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    do
    {
        /* Check if socket is blocked for a previous command */
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        do
        {
            n = size - offset;
            if (n > MAX_CERT_DATA_LENGTH)
            {
                n = MAX_CERT_DATA_LENGTH;
            }
            buffSize = sizeof(SOCK_SSL_ADD_CERT_T) + n - 1;
            pBuff = A_NETBUF_ALLOC(buffSize);
            if (pBuff)
            {
                /* reserve the necessary space in the buffer */
                A_NETBUF_PUT(pBuff, buffSize);

                pCmd = A_NETBUF_DATA(pBuff);
                pCmd->wmi_cmd.cmd_type = SOCK_SSL_ADD_CERT;
                pCmd->wmi_cmd.length = buffSize - sizeof(WMI_SOCKET_CMD);
                pCmd->ctx = (uint32_t)ctx;
                pCmd->type = type;
                pCmd->total = size;
                pCmd->offset = offset;
                pCmd->size = n;
                A_MEMCPY(pCmd->data, &data[offset], n);
                offset += n;

                /* send the buffer to the wifi device */
                SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_ADD_CERT);
                result = wmi_cmd_send(pDCxt->pWmiCxt, pBuff, WMI_SOCKET_CMDID, NO_SYNC_WMIFLAG);
                if (result != A_OK)
                {
                    SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_ADD_CERT);
                    return A_ERROR;
                }
                do
                {
                    if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
                    {
                        A_ASSERT(0);
                    }
                } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_ADD_CERT));
                result = ath_sock_context[index]->result;
            }
        } while (offset < size && result >= A_OK);
        if (offset != size)
        {
            result = A_ERROR;
        }
    } while (0);

    return result;
}

int32_t Api_SSL_storeCert(void *pCxt, char *name, uint8_t *cert, uint32_t size)
{
    A_DRIVER_CONTEXT *pDCxt;
    SOCK_SSL_STORE_CERT_T *pCmd;
    int32_t result = A_OK;
    uint32_t n, buffSize, offset = 0;
    void *pBuff;
    int index = GLOBAL_SOCK_INDEX;
    uint8_t *data = (uint8_t *)cert;

    pDCxt = GET_DRIVER_COMMON(pCxt);
    do
    {
        /* Check if socket is blocked for a previous command */
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            result = A_ERROR;
            break;
        }

        do
        {
            n = size - offset;
            if (n > MAX_CERT_DATA_LENGTH)
            {
                n = MAX_CERT_DATA_LENGTH;
            }
            buffSize = sizeof(SOCK_SSL_STORE_CERT_T) + n - 1;
            pBuff = A_NETBUF_ALLOC(buffSize);
            if (pBuff)
            {
                /* reserve the necessary space in the buffer */
                A_NETBUF_PUT(pBuff, buffSize);

                pCmd = A_NETBUF_DATA(pBuff);
                pCmd->wmi_cmd.cmd_type = SOCK_SSL_STORE_CERT;
                pCmd->wmi_cmd.length = buffSize - sizeof(WMI_SOCKET_CMD);
                strncpy((char *)pCmd->name, name, SSL_MAX_CERT_NAME_LEN);
                pCmd->name[SSL_MAX_CERT_NAME_LEN] = '\0';
                pCmd->total = size;
                pCmd->offset = offset;
                pCmd->size = n;
                A_MEMCPY(pCmd->data, &data[offset], n);
                offset += n;

                /* send the buffer to the wifi device */
                SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_STORE_CERT);
                result = wmi_cmd_send(pDCxt->pWmiCxt, pBuff, WMI_SOCKET_CMDID, NO_SYNC_WMIFLAG);
                if (result != A_OK)
                {
                    SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_STORE_CERT);
                    return A_ERROR;
                }
                do
                {
                    if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
                    {
                        A_ASSERT(0);
                    }
                } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_STORE_CERT));
                result = ath_sock_context[index]->result;
            }
        } while (offset < size && result >= A_OK);
        if (offset != size)
        {
            result = A_ERROR;
        }
    } while (0);

    return result;
}

int32_t Api_SSL_loadCert(void *pCxt, SSL_CTX *ctx, SSL_CERT_TYPE_T type, char *name)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    SOCK_SSL_LOAD_CERT_T cmd;
    int32_t result = A_ERROR;
    int32_t index = GLOBAL_SOCK_INDEX;

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            break;
        }

        /* Send command and wait for response */
        cmd.ctx = (uint32_t)ctx;
        cmd.type = type;
        strncpy((char *)cmd.name, name, SSL_MAX_CERT_NAME_LEN);
        cmd.name[SSL_MAX_CERT_NAME_LEN] = '\0';
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_LOAD_CERT);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_LOAD_CERT, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_LOAD_CERT);
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_LOAD_CERT));
        result = ath_sock_context[index]->result;
    } while (0);

    return result;
}

int32_t Api_SSL_listCert(void *pCxt, SSL_FILE_NAME_LIST *fileNames)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    SOCK_SSL_LIST_CERT_T cmd;
    int32_t result = A_ERROR;
    int32_t index = GLOBAL_SOCK_INDEX;

    do
    {
        /*Check if socket is blocked for a previous command*/
        if (IS_SOCKET_BLOCKED(ath_sock_context[index]))
        {
            break;
        }

        /* Send command and wait for response */
        SOCK_EV_MASK_SET(ath_sock_context[index], SOCK_SSL_LIST_CERT);
        if (wmi_socket_cmd(pDCxt->pWmiCxt, SOCK_SSL_LIST_CERT, (void *)(&cmd), sizeof(cmd)) != A_OK)
        {
            SOCK_EV_MASK_CLEAR(ath_sock_context[index], SOCK_SSL_LIST_CERT);
            break;
        }
        do
        {
            if (BLOCK(pCxt, ath_sock_context[index], COMMAND_BLOCK_TIMEOUT, RX_DIRECTION) != A_OK)
            {
                A_ASSERT(0);
            }
        } while (SOCK_EV_MASK_TEST(ath_sock_context[index], SOCK_SSL_LIST_CERT));
        result = ath_sock_context[index]->result;
        memcpy(fileNames, ath_sock_context[index]->data, sizeof(SSL_FILE_NAME_LIST));
        A_NETBUF_FREE(ath_sock_context[index]->pReq);
    } while (0);

    return result;
}

#endif // ENABLE_SSL

A_STATUS blockForDataRx(void *pCxt, void *ctxt, uint32_t msec, uint8_t direction)
{
    SOCKET_CONTEXT_PTR pcustctxt = GET_SOCKET_CONTEXT(ctxt);
    pcustctxt->respAvailable = false;
    return (blockForResponse(pCxt, ctxt, msec, direction));
}
/*****************************************************************************/
/*  blockForResponse - blocks a thread, either indefinitely or for a specified
 *                     time period in milliseconds
 * RETURNS: A_OK on success or error if timeout occurs.
 *****************************************************************************/
A_STATUS blockForResponse(void *pCxt, void *ctxt, uint32_t msec, uint8_t direction)
{
    A_STATUS result = A_OK;
    SOCKET_CONTEXT_PTR pcustctxt = GET_SOCKET_CONTEXT(ctxt);
    uint32_t setFlags;

    pcustctxt->blockFlag = 1;
    if (msec == 0)
    {
        msec = 1;
    }

    if (direction == RX_DIRECTION)
    {
        if (pcustctxt->respAvailable == false)
        {
            pcustctxt->rxBlock = true;
            event_t *pEvent = &pcustctxt->sockRxWakeEvent;
            A_STATUS res = A_EVENT_WAIT(pEvent, 0x01, false, msec, &setFlags);
            if (A_OK != res)
            {
                result = A_ERROR;
            }
            else
            {
                A_EVENT_CLEAR(pEvent, 0x01);
            }
            pcustctxt->rxBlock = false;
            pcustctxt->respAvailable = false;
        }
        else if (pcustctxt->respAvailable == true)
        {
            /*Response is available, reset the flag for future use*/
            result = A_OK;
            pcustctxt->respAvailable = false;
        }
        else
        {
            // undefined state
            assert(0);
        }
    }
    else if (direction == TX_DIRECTION)
    {
        if (pcustctxt->txUnblocked == false)
        {
            pcustctxt->txBlock = true;
            event_t *pEvent = &pcustctxt->sockTxWakeEvent;
            if (A_OK != A_EVENT_WAIT(pEvent, 0x01, false, msec, &setFlags))
            {
                result = A_ERROR;
            }
            else
            {
                A_EVENT_CLEAR(pEvent, 0x01);
            }
            pcustctxt->txBlock = false;
            pcustctxt->txUnblocked = false;
        }

        if (pcustctxt->txUnblocked == true)
        {
            /*Response is available, reset the flag for future use*/
            result = A_OK;
            pcustctxt->txUnblocked = false;
        }
        /* Propagate error, thread unsafe */
        result = (A_STATUS)socket_get_driver_error(ctxt);
        /* Clear error, thread unsafe*/
        socket_set_driver_error(ctxt, A_OK);
    }
    else
    {
        last_driver_error = A_EINVAL;
    }

    pcustctxt->blockFlag = 0;
    return result;
}

#if T_SELECT_VER1
A_STATUS blockSelect(void *pCxt, uint32_t msec)
{
    A_STATUS result = A_OK;
    if (msec == 0)
    {
        msec = 1;
    }

    event_t *pEvent = &GET_DRIVER_CXT(pCxt)->sockSelectWakeEvent;
    uint32_t setFlags;
    if (A_OK != A_EVENT_WAIT(pEvent, 0x01, true, msec, &setFlags))
    {
        result = A_ERROR;
    }
    else
    {
        A_EVENT_CLEAR(pEvent, 0x01);
    }
    return result;
}
#endif // T_SELECT_VER1

/*****************************************************************************/
/*  isSocketBlocked - Checks if a thread is blocked on a given socket
 * RETURNS: value of block flag
 *****************************************************************************/
uint32_t isSocketBlocked(void *ctxt)
{
    SOCKET_CONTEXT_PTR pcustctxt = GET_SOCKET_CONTEXT(ctxt);
    return pcustctxt->blockFlag;
}

/*****************************************************************************/
/*  unblock - Unblocks a thread if it is blocked.
 * RETURNS: A_OK if unblock was successful, A_ERROR if thread was not blocked
 *****************************************************************************/
A_STATUS unblock(void *ctxt, uint8_t direction)
{
    A_STATUS result = A_OK;
    SOCKET_CONTEXT_PTR pcustctxt = GET_SOCKET_CONTEXT(ctxt);

    /*Unblock task if it is blocked*/
    if (direction == RX_DIRECTION)
    {
        pcustctxt->respAvailable = true;
        /*Unblock task if it is blocked*/
        if (pcustctxt->rxBlock == true)
        {
            A_EVENT_SET(&pcustctxt->sockRxWakeEvent, 0x01);
        }
    }
    else if (direction == TX_DIRECTION)
    {
        pcustctxt->txUnblocked = true;
        if (pcustctxt->txBlock == true)
        {
            A_EVENT_SET(&pcustctxt->sockTxWakeEvent, 0x01);
        }
    }
    return result;
}

/*****************************************************************************/
/*  Set socket 'driver_error' field
 *****************************************************************************/
void socket_set_driver_error(void *ctxt, int32_t error)
{
    assert(NULL != ctxt);
    SOCKET_CONTEXT_PTR socket = GET_SOCKET_CONTEXT(ctxt);
    assert(NULL != socket);
    socket->driver_error = error;
}

/*****************************************************************************/
/*  Get socket 'driver_error' field
 *****************************************************************************/
int32_t socket_get_driver_error(void *ctxt)
{
    assert(NULL != ctxt);
    SOCKET_CONTEXT_PTR socket = GET_SOCKET_CONTEXT(ctxt);
    assert(NULL != socket);
    return socket->driver_error;
}

#if T_SELECT_VER1
A_STATUS unblockSelect(void *pCxt)
{
    A_STATUS result = A_OK;
    A_EVENT_SET(&((GET_DRIVER_CXT(pCxt))->sockSelectWakeEvent), 0x01);
    return result;
}
#endif

#if T_SELECT_VER1

#define __FDMASK(d) (((uint32_t)1) << (d))
#define __FD_SET(d, set) ((*set) |= __FDMASK(d))
#define __FD_CLR(d, set) ((*set) &= ~__FDMASK(d))
#define __FD_ISSET(d, set) (((set)&__FDMASK(d)) != 0)
#define __FD_ZERO(set) ((*set) = 0)

int32_t FD_IsSet(uint32_t handle, uint32_t mask)
{
    int32_t index = 0;
    if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
    {
        return 1;
    }
    return (__FD_ISSET(index, mask));
}

int32_t FD_Set(uint32_t handle, uint32_t *mask)
{
    int32_t index = 0;
    if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
    {
        return 0;
    }
    __FD_SET(index, (mask));
    return (A_OK);
}

int32_t FD_Clr(uint32_t handle, uint32_t *mask)
{
    int32_t index = 0;
    if ((index = find_socket_context(handle, true)) == SOCKET_NOT_FOUND)
    {
        // special case, we are trying to clear the mask for a non-existent socket.
        // This is a workaround now. Need to come up with an elegant solution.
        // Go through the all the existing sockets and remove the mask elements for which no valid sockets exists.

        *mask &= ((1 << (MAX_SOCKETS_SUPPORTED + 1)) - 1);
        for (index = 0; index < MAX_SOCKETS_SUPPORTED; index++)
        {
            if ((*mask & (1 << index)) && (ath_sock_context[index]->handle == 0))
            {
                __FD_CLR(index, (mask));
            }
        }
        return A_OK;
    }
    __FD_CLR(index, (mask));
    return (A_OK);
}

int32_t FD_Zero(uint32_t *mask)
{
    *mask = 0;
    return (A_OK);
}

#endif // T_SELECT_VER1

#endif // ENABLE_STACK_OFFLOAD
