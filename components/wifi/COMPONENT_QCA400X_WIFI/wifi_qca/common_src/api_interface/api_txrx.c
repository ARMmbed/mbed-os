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
#include "wifi_common.h"

#include <a_config.h>
#include <a_types.h>
#include <a_osapi.h>
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <aggr_recv_api.h>
#include <hcd_api.h>
#include <wmi_api.h>
#include <atheros_wifi_api.h>
#include <atheros_wifi_internal.h>
#include <atheros_stack_offload.h>
#include <wlan_api.h>

/*****************************************************************************/
/*  query_credit_deficit - Executed by the driver task to satisfy the
 *	 Api_TxGetStatus() request.  It will query the wifi credit counters on
 *	 the Wifi device as needed to get the most up-to-date tx credit status.
 *      void *pCxt - the driver context.
 *****************************************************************************/
static A_STATUS query_credit_deficit(void *pCxt)
{
    uint32_t reg;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        if ((pDCxt->booleans & (SDHD_BOOL_FATAL_ERROR | SDHD_BOOL_DMA_IN_PROG | SDHD_BOOL_DMA_WRITE_WAIT_FOR_BUFFER)))
        {
            break;
        }

        reg = Htc_ReadCreditCounter(pCxt, 1);
        pDCxt->txQueryResult = (reg & 0x80000000) ? 1 : 0;
    } while (0);

    pDCxt->asynchRequest = NULL;
    pDCxt->txQueryInProgress = false;
    DRIVER_WAKE_USER(pCxt);
    return A_OK;
}

/*****************************************************************************/
/*  Api_TxGetStatus - Learns the status regarding any TX requests.  it will
 *	 test the state of the driver first to learn if any TX requests are
 *	 remaining on the host side.  if none then it will test the state of
 *	 the HTC credits to learn if any TX requests are still being processed
 *	 by the WIFI device.  If none then the function will return the idle
 *	 status.
 *      void *pCxt - the driver context.
 *****************************************************************************/
uint16_t Api_TxGetStatus(void *pCxt)
{
    uint16_t status = ATH_TX_STATUS_IDLE;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        if (0 != A_NETBUF_QUEUE_SIZE(&(pDCxt->txQueue)) || pDCxt->driver_state == DRIVER_STATE_TX_PROCESSING)
        {
            status = ATH_TX_STATUS_HOST_PENDING;
            break;
        }
        /* Check the AR410X status */
        status = ATH_TX_STATUS_WIFI_PENDING;
        /* set up driver operation to read credits from device */
        if (pDCxt->asynchRequest == NULL)
        {
            pDCxt->txQueryInProgress = true;
            pDCxt->asynchRequest = query_credit_deficit;
            DRIVER_WAKE_DRIVER(pCxt);
            DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->txQueryInProgress), false, 5000);

            if (pDCxt->txQueryInProgress == false)
            {
                if (pDCxt->txQueryResult == 0)
                {
                    status = ATH_TX_STATUS_IDLE;
                }
            }
        }
    } while (0);

    return status;
}

/*****************************************************************************/
/*  API_TxComplete - Brings a transmit packet to its final conclusion. The
 *   packet is either a data packet in which case it will be freed back to
 *   the caller, or it is a wmi command in which case it will freed.
 *      void *pCxt - the driver context.
 *      void *pReq - the packet.
 *****************************************************************************/
void Api_TxComplete(void *pCxt, void *pReq)
{
    /* some user tasks may have blocked until a transmit complete occurs
     * these lines wake up these tasks. */
    GET_DRIVER_COMMON(pCxt)->tx_complete_pend = false;
    DRIVER_WAKE_USER(pCxt);

    if (ath_custom_init.Api_TxComplete != NULL)
    {
        ath_custom_init.Api_TxComplete(pCxt, pReq);
        return;
    }
    else
    {
        API_TXCOMPLETE(pCxt, pReq);
        A_NETBUF_FREE(pReq);
    }
}

/*****************************************************************************/
/*  API_RxComplete - Brings a received packet to its final conclusion. The
 *   packet is either a data packet in which case it will be processed and
 *   sent to the network layer, or it is a wmi event in which case it will
 *   be delivered to the wmi layer.
 *      void *pCxt - the driver context.
 *      void *pReq - the packet.
 *****************************************************************************/
void Api_RxComplete(void *pCxt, void *pReq)
{
    A_STATUS status = A_OK;
    uint16_t minHdrLen;
    uint16_t packetLen;
    uint8_t containsDot11Hdr = 0;
    WMI_DATA_HDR *dhdr;
#if WLAN_CONFIG_11N_AGGR_SUPPORT
#if !ENABLE_STACK_OFFLOAD
    ATH_MAC_HDR *mac_hdr;
#endif
#endif
    uint8_t is_amsdu, is_acl_data_frame;
#if !ENABLE_STACK_OFFLOAD
    uint16_t seq_no;
    uint8_t tid;
#endif
    uint8_t devId;
    uint8_t meta_type;
    HTC_ENDPOINT_ID ept = (HTC_ENDPOINT_ID)A_NETBUF_GET_ELEM(pReq, A_REQ_EPID);
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    /* take lock to protect buffer counts
     * and adaptive power throughput state */

    if (ath_custom_init.Api_RxComplete != NULL)
    {
        ath_custom_init.Api_RxComplete(pCxt, pReq);
        return;
    }
    else
    {
        if (pDCxt->wmiEnabled == true)
        {
            if (ept == ENDPOINT_1)
            {
                /*
                 * this is a wmi control packet.
                 */
                wmi_control_rx(pDCxt->pWmiCxt, pReq);
            }
            else if (pDCxt->promiscuous_mode)
            {
                wmi_data_hdr_remove(pDCxt->pWmiCxt, pReq);
                CUSTOM_DELIVER_FRAME(pCxt, pReq);
                status = A_OK;
            }
            else
            {
                /*
                 * this is a data packet.
                 */
                dhdr = (WMI_DATA_HDR *)A_NETBUF_DATA(pReq);
                packetLen = A_NETBUF_LEN(pReq);
                dhdr->info2 = A_LE2CPU16(dhdr->info2);
                dhdr->info3 = A_LE2CPU16(dhdr->info3);
                is_acl_data_frame = (uint8_t)(WMI_DATA_HDR_GET_DATA_TYPE(dhdr) == WMI_DATA_HDR_DATA_TYPE_ACL);

                devId = WMI_DATA_HDR_GET_DEVID(dhdr);

                minHdrLen = MIN_HDR_LEN;

                /* In the case of AP mode we may receive NULL data frames
                 * that do not have LLC hdr. They are 16 bytes in size.
                 * Allow these frames in the AP mode.
                 * ACL data frames don't follow ethernet frame bounds for
                 * min length
                 */
                do
                {
                    if ((pDCxt->conn[devId].networkType != AP_NETWORK && (packetLen < minHdrLen)) ||
                        (packetLen > AR4100_MAX_RX_MESSAGE_SIZE))
                    {
                        /*
                        * packet is too short or too long
                        */
                        status = A_ERROR;
                        break;
                    }
                    else
                    {
                        is_amsdu = (uint8_t)WMI_DATA_HDR_IS_AMSDU(dhdr);
#if !ENABLE_STACK_OFFLOAD
                        tid = (uint8_t)WMI_DATA_HDR_GET_UP(dhdr);
                        seq_no = (uint16_t)(WMI_DATA_HDR_GET_SEQNO(dhdr));
#endif
                        meta_type = (uint8_t)WMI_DATA_HDR_GET_META(dhdr);
                        containsDot11Hdr = (uint8_t)WMI_DATA_HDR_GET_DOT11(dhdr);
                        /* NOTE: rssi is not currently managed on a per peer basis.
                         * 	therefore, if multiple peers are sending packets to
                         *	this radio the rssi will be averaged across all
                         * 	indiscriminately.
                         */
                        /* simple averaging algorithm.  7/8 of original value is combined
                         * with 1/8 of new value unless original value is 0 in which case
                         * just adopt the new value as its probably the first in the
                         * connection.
                         */
                        pDCxt->rssi = (int8_t)((pDCxt->rssi) ? (pDCxt->rssi * 7 + dhdr->rssi) >> 3 : dhdr->rssi);
                        wmi_data_hdr_remove(pDCxt->pWmiCxt, pReq);

                        if (meta_type)
                        {
                            /* this code does not accept per frame
                             * meta data */
                            A_ASSERT(0);
                        }
                        /* NWF: print the 802.11 hdr bytes */
                        if (containsDot11Hdr)
                        {
                            /* this code does not accept dot11 headers */
                            A_ASSERT(0);
                        }
                        else if (!is_amsdu && !is_acl_data_frame)
                        {
                            status = WMI_DOT3_2_DIX(pReq);
                        }
                        else
                        {
                            /* this code does not accept amsdu or acl data */
                            A_ASSERT(0);
                        }

                        if (status != A_OK)
                        {
                            /* Drop frames that could not be processed (lack of memory, etc.) */
                            break;
                        }

#if WLAN_CONFIG_11N_AGGR_SUPPORT
#if !ENABLE_STACK_OFFLOAD
                        mac_hdr = A_NETBUF_DATA(pReq);
                        /*Check if packet is multicast, if so, do not check for aggregation*/
                        if ((mac_hdr->dstMac[0] & 0x01) == 0)
                        {
                            if (false == aggr_process_recv_frm(pDCxt->pAggrCxt, tid, seq_no, is_amsdu, (void **)&pReq))
                            {
                                status = A_ERROR;
                                break;
                            }
                        }
#endif /* !ENABLE_STACK_OFFLOAD*/
#endif /* WLAN_CONFIG_11N_AGGR_SUPPORT */
                        CUSTOM_DELIVER_FRAME(pCxt, pReq);
                        status = A_OK;
                    }
                } while (0);

                if (A_OK != status)
                {
                    A_NETBUF_FREE(pReq);
                }
            }
        }
        else
        {
            A_NETBUF_FREE(pReq);
        }

        return;
    }
}

/*****************************************************************************/
/*  API_WmiTxStart - Function used by WMI via A_WMI_CONTROL_TX() to send
 *   a WMI command.
 *      void *pCxt - the driver context.
 *      void *pReq - the packet.
 *****************************************************************************/
A_STATUS
Api_WmiTxStart(void *pCxt, void *pReq, HTC_ENDPOINT_ID eid)
{
    A_STATUS status = A_ERROR;

    do
    {
        A_NETBUF_SET_ELEM(pReq, A_REQ_EPID, eid);

        if (A_OK != Driver_SubmitTxRequest(pCxt, pReq))
        {
            break;
        }

        status = A_OK;
    } while (0);

    return status;
}

/*****************************************************************************/
/*  Api_DataTxStart - Starts the process of transmitting a data packet (pReq).
 *	 Various checks are performed to ensure the driver and device are ready
 *	 to open the data path.  A dot.3 header is pre-pended if necessary and a
 *	 WMI_DATA_HDR is pre-pended. In a multi-threaded system this function is
 * 	 expected to be executed on a non-driver thread.
 *      void *pCxt - the driver context.
 *		void *pReq - the Transmit packet.
 *****************************************************************************/
A_STATUS
Api_DataTxStart(void *pCxt, void *pReq)
{
#define AC_NOT_MAPPED 99
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint8_t ac = AC_NOT_MAPPED;
    HTC_ENDPOINT_ID eid = ENDPOINT_UNUSED;
    boolean bMoreData = false;
    uint8_t dot11Hdr = WLAN_CONFIG_TX_DOT11_HDR;
    A_STATUS status = A_ERROR;
    WMI_TX_META_V5 ip_meta;

    do
    {
        /* If target is not associated */
        if ((false == pDCxt->conn[0].isConnected && false == pDCxt->conn[0].isConnectPending)
#if (WLAN_NUM_OF_DEVICES > 1)
            && (false == pDCxt->conn[1].isConnected && false == pDCxt->conn[1].isConnectPending)
#endif
                )
        {
            break;
        }
        else if ((false == pDCxt->conn[0].isConnected)
#if (WLAN_NUM_OF_DEVICES > 1)
                 && (false == pDCxt->conn[1].isConnected)
#endif
                     )
        {
            // block until connection completes.
            DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->conn[0].isConnected), true, 5000);

            if ((false == pDCxt->conn[0].isConnected)
#if (WLAN_NUM_OF_DEVICES > 1)
                && (false == pDCxt->conn[1].isConnected)
#endif
                    )
            {
                break;
            }
        }

        if (A_OK != Api_DriverAccessCheck(pCxt, 1, ACCESS_REQUEST_TX))
        {
            break;
        }

        if (pDCxt->wmiReady == false)
        {
            break;
        }

        if (false == pDCxt->wmiEnabled)
        {
            break;
        }

/* add wmi_data_hdr to front of frame */
#if ENABLE_STACK_OFFLOAD
        if (wmi_data_hdr_add(pDCxt->pWmiCxt, pReq, DATA_MSGTYPE, bMoreData,
                             (dot11Hdr) ? WMI_DATA_HDR_DATA_TYPE_802_11 : WMI_DATA_HDR_DATA_TYPE_802_3,
                             WMI_META_VERSION_5, (void *)&ip_meta) != A_OK)
        {
            break;
        }
#else
        /* convert frame header */
        if (wmi_dix_2_dot3(pDCxt->pWmiCxt, pReq) != A_OK)
        {
            break;
        }

        if (wmi_data_hdr_add(pDCxt->pWmiCxt, pReq, DATA_MSGTYPE, bMoreData,
                             (dot11Hdr) ? WMI_DATA_HDR_DATA_TYPE_802_11 : WMI_DATA_HDR_DATA_TYPE_802_3, 0,
                             NULL) != A_OK)
        {
            break;
        }
#endif
        /* get the stream mapping */
        ac = wmi_implicit_create_pstream(pDCxt->pWmiCxt, pReq, 0, pDCxt->wmmEnabled);
        eid = Util_AC2EndpointID(pCxt, ac);

        if (eid && eid != ENDPOINT_UNUSED)
        {
            A_NETBUF_SET_ELEM(pReq, A_REQ_EPID, eid);
            /* Submit for transmission to next layer */
            if (A_OK != Driver_SubmitTxRequest(pCxt, pReq))
            {
                break;
            }
        }
        else
        {
            break;
        }

        status = A_OK;
    } while (0);

    return status;
}
