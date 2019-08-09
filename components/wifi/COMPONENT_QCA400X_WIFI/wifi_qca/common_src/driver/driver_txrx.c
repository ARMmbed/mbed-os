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
#include <driver_cxt.h>
#include <common_api.h>
#include <hcd_api.h>
#include <custom_wlan_api.h>
#include <spi_hcd_if.h>
#include <hif_internal.h>
#include <htc.h>

/*****************************************************************************/
/*  Driver_RxComplete - For completed RX packet requests this function routes
 *   the request to a handler based on endpoint ID.  Called when the Bus
 *   transfer operation has completed.
 *      void *pCxt - the driver context.
 *      void *pReq - the request object.
 *****************************************************************************/
void Driver_RxComplete(void *pCxt, void *pReq)
{
    uint32_t lookAheads[1];
    uint32_t NumLookAheads = 0;

    if (A_OK != Htc_ProcessRecvHeader(pCxt, pReq, lookAheads, &NumLookAheads))
    {
        A_ASSERT(0);
    }

    if (A_NETBUF_GET_ELEM(pReq, A_REQ_EPID) == ENDPOINT_0)
    {
        Htc_RxComplete(pCxt, pReq);
    }
    else
    {
        Api_RxComplete(pCxt, pReq);
    }
}

/*****************************************************************************/
/*  Driver_TxComplete - For completed TX packet requests this function routes
 *   the request to a handler based on endpoint ID.  Called when the Bus
 *   transfer operation has completed.
 *      void *pCxt - the driver context.
 *      void *pReq - the request object.
 *****************************************************************************/
static void Driver_TxComplete(void *pCxt, void *pReq)
{
    Htc_ProcessTxComplete(pCxt, pReq);

#if 0
    if(A_NETBUF_GET_ELEM(pReq, A_REQ_EPID) == ENDPOINT_0){
        /* this should never happen in practice as the driver
         * does not SEND messages on the HTC endpoint */
        A_ASSERT(0);
        //HTC_TxComplete(pCxt, pReq);
    }else{
        Api_TxComplete(pCxt, pReq);
    }
#else
    Api_TxComplete(pCxt, pReq);
#endif
}

/*****************************************************************************/
/*  Driver_PostProcessRequest - Utility to post process requests that have
 *	 completed Hcd_Request().
 *      void *pCxt - the driver context.
 *      void *pReq - the request object.
 *		A_STATUS reqStatus - the status of the transaction.
 *****************************************************************************/
static void Driver_PostProcessRequest(void *pCxt, void *pReq, A_STATUS reqStatus)
{
    A_DRIVER_CONTEXT *pDCxt;

    A_NETBUF_SET_ELEM(pReq, A_REQ_STATUS, reqStatus);

    if (A_NETBUF_GET_ELEM(pReq, A_REQ_CALLBACK) != NULL)
    {
        pDCxt = GET_DRIVER_COMMON(pCxt);
        pDCxt->spi_hcd.pCurrentRequest = pReq;

        if (reqStatus != A_PENDING)
        {
            pDCxt->booleans |= SDHD_BOOL_DMA_COMPLETE;
        }
    }
}

/*****************************************************************************/
/*  Driver_SubmitTxRequest - Called from a User thread to submit a new
 *      Tx packet to the driver. The function will append the request to the
 *      drivers txqueue and wake up the driver thread.
 *      void *pCxt - the driver context.
 *      void *pReq - the request object.
 *****************************************************************************/
A_STATUS
Driver_SubmitTxRequest(void *pCxt, void *pReq)
{
    A_STATUS status = A_ERROR;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint32_t transferLength, creditsRequired;

    do
    {
        A_NETBUF_SET_ELEM(pReq, A_REQ_CALLBACK, Driver_TxComplete);
        /* reserve space for HTC header to allow for proper calculation
         * of transmit length. The header will be populated when the
         * request is transmitted. */
        if (A_NETBUF_PUSH(pReq, HTC_HDR_LENGTH) != A_OK)
        {
            break;
        }
        /* The length of the request is now complete so
         * calculate and assign the number of credits required by this
         * request. */
        transferLength = DEV_CALC_SEND_PADDED_LEN(pDCxt, A_NETBUF_LEN(pReq));

        if (transferLength <= pDCxt->creditSize)
        {
            creditsRequired = 1;
        }
        else
        {
            /* figure out how many credits this message requires */
            creditsRequired = (uint32_t)(transferLength / pDCxt->creditSize);

            if (creditsRequired * pDCxt->creditSize < transferLength)
            {
                creditsRequired++;
            }
        }

        A_NETBUF_SET_ELEM(pReq, A_REQ_CREDITS, creditsRequired);

        TXQUEUE_ACCESS_ACQUIRE(pCxt);
        {
            A_NETBUF_ENQUEUE(&(pDCxt->txQueue), pReq);
            status = A_OK;
        }
        TXQUEUE_ACCESS_RELEASE(pCxt);

        if (status == A_OK)
        {
            DRIVER_WAKE_DRIVER(pCxt);
        }
    } while (0);

    return status;
}

/*****************************************************************************/
/*  Driver_TxComplete - For completed TX packet requests this function routes
 *   the request to a handler based on endpoint ID.  Called when the Bus
 *   transfer operation has completed.
 *      void *pCxt - the driver context.
 *      void *pReq - the request object.
 *****************************************************************************/
static void Driver_Ep0TxComplete(void *pCxt, void *pReq)
{
    Htc_ProcessTxComplete(pCxt, pReq);
}

A_STATUS
Driver_SubmitEp0TxRequest(void *pCxt, void *pReq)
{
    A_STATUS status = A_ERROR;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    uint32_t transferLength, creditsRequired;

    do
    {
        A_NETBUF_SET_ELEM(pReq, A_REQ_CALLBACK, Driver_Ep0TxComplete);
        /* reserve space for HTC header to allow for proper calculation
         * of transmit length. The header will be populated when the
         * request is transmitted. */
        if (A_NETBUF_PUSH(pReq, HTC_HDR_LENGTH) != A_OK)
        {
            break;
        }
        /* The length of the request is now complete so
         * calculate and assign the number of credits required by this
         * request. */
        transferLength = DEV_CALC_SEND_PADDED_LEN(pDCxt, A_NETBUF_LEN(pReq));

        if (transferLength <= pDCxt->creditSize)
        {
            creditsRequired = 1;
        }
        else
        {
            /* figure out how many credits this message requires */
            creditsRequired = (uint32_t)(transferLength / pDCxt->creditSize);

            if (creditsRequired * pDCxt->creditSize < transferLength)
            {
                creditsRequired++;
            }
        }

        A_NETBUF_SET_ELEM(pReq, A_REQ_CREDITS, creditsRequired);

        TXQUEUE_ACCESS_ACQUIRE(pCxt);
        {
            A_NETBUF_PREQUEUE(&(pDCxt->txQueue), pReq);
            status = A_OK;
        }
        TXQUEUE_ACCESS_RELEASE(pCxt);

        if (status == A_OK)
        {
            DRIVER_WAKE_DRIVER(pCxt);
        }
    } while (0);

    return status;
}

/*****************************************************************************/
/*  Driver_SendPacket - Entry point to send a packet from the device. Sets
 *   up request params address and other and calls HCD_Request to perform the
 *   work.  If a callback is provided in the request then it will be used to
 *   complete the request.  If a callback is not provided the lower layers
 *	 must complete the request synchronously.
 *      void *pCxt - the driver context.
 *      void *pReq - the request object.
 *****************************************************************************/
A_STATUS Driver_SendPacket(void *pCxt, void *pReq)
{
    A_STATUS status = A_ERROR;
    uint32_t address;
    uint16_t transLength;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        if (A_OK != (status = Htc_SendPacket(pCxt, pReq)))
        {
            Driver_PostProcessRequest(pCxt, pReq, status);
            break;
        }
        /* calc the padding and mbox address */
        /* NOTE: for Transmit the request length may not equal the transfer length due to
         *  padding requirements. */
        transLength = DEV_CALC_RECV_PADDED_LEN(pDCxt, A_NETBUF_LEN(pReq));
        address = HW_GetMboxAddress(pCxt, HIF_ACTIVE_MBOX_INDEX, transLength);
        address &= ATH_TRANS_ADDR_MASK;
        A_NETBUF_SET_ELEM(pReq, A_REQ_ADDRESS, address);
        A_NETBUF_SET_ELEM(pReq, A_REQ_TRANSFER_LEN, transLength);
        /* init the packet read params/cmd incremental vs fixed address etc. */
        A_NETBUF_SET_ELEM(pReq, A_REQ_COMMAND, (ATH_TRANS_WRITE | ATH_TRANS_DMA));
        status = Hcd_Request(pCxt, pReq);
        Driver_PostProcessRequest(pCxt, pReq, status);
    } while (0);

    return status;
}

/*****************************************************************************/
/*  Driver_RecvPacket - Entry point to receive a packet from the device. Sets
 *   up request params address and other and calls HCD_Request to perform the
 *   work.  This path always provides a request callback for asynchronous
 *   completion.  This is because DMA if available will always be used for
 *   packet requests.
 *      void *pCxt - the driver context.
 *      void *pReq - the request object.
 *****************************************************************************/
A_STATUS Driver_RecvPacket(void *pCxt, void *pReq)
{
    // uint16_t validLength, transLength, hdrLength;

    A_STATUS status;
    uint32_t address;

    Htc_PrepareRecvPacket(pCxt, pReq);

    /* init the packet mailbox address - begin with mailbox end address and then subtract the request transfer length */
    address = HW_GetMboxAddress(pCxt, HIF_ACTIVE_MBOX_INDEX, A_NETBUF_LEN(pReq));
    address &= ATH_TRANS_ADDR_MASK;
    A_NETBUF_SET_ELEM(pReq, A_REQ_ADDRESS, address);
    /* init the packet read params/cmd incremental vs fixed address etc. */
    A_NETBUF_SET_ELEM(pReq, A_REQ_COMMAND, (ATH_TRANS_READ | ATH_TRANS_DMA));

    status = Hcd_Request(pCxt, pReq);
    Driver_PostProcessRequest(pCxt, pReq, status);

    return status;
}
