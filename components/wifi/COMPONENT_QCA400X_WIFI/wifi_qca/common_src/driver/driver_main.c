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
#include <driver_cxt.h>
#include <common_api.h>
#include <custom_wlan_api.h>
#include <wmi_api.h>
#include <netbuf.h>
#include <htc.h>
#include <spi_hcd_if.h>
#include <hcd_api.h>
#include "mbed_rtos_storage.h"

#include "atheros_wifi_api.h"
#include "atheros_wifi_internal.h"

/*****************************************************************************/
/********** IMPLEMENTATION **********/
/*****************************************************************************/

osThreadId_t atheros_wifi_task_id = NULL;
mbed_rtos_storage_thread_t thread_cb;

uint32_t g_totAlloc = 0;
uint32_t g_poolid = 0xffffffff;

/*****************************************************************************/
/*  Driver_Main - This is the top level entry point for the Atheros wifi driver
 *   This should be called from either a dedicated thread running in a loop or,
 *   in the case of single threaded systems it should be called periodically
 *   from the main loop.  Also, if in a single threaded system a API call
 *   requires synchronous completion then it may be necessary to call this
 *   function from within the API call.
 *      void *pCxt - the driver context.
 *      uint8_t scope - Limits/allows what type of activity may be processed.
 *          Options are any combination of DRIVER_SCOPE_TX + DRIVER_SCOPE_RX.
 *      boolean *pCanBlock - used to report to caller whether the driver is
 *          in a state where it is safe to block until further notice.
 *		uint16_t *pBlock_msec - used to report to caller how long the driver
 *			may block for. if zero then driver can block indefinitely.
 *****************************************************************************/
// uint32_t g_drv_progress = 0;
A_STATUS
Driver_Main(void *pCxt, uint8_t scope, boolean *pCanBlock, uint16_t *pBlock_msec)
{
    void *pReq;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    HW_ProcessPendingInterrupts(pCxt); // IGX_UD_CHANGES

    /* This is where packets are received from the
     * wifi device. Because system buffers must be
     * available to receive a packet we only call into
     * this function if we know that buffers are
     * available to satisfy the operation.
     */
    if ((scope & DRIVER_SCOPE_RX) && true == Driver_RxReady(pCxt))
    {
        // g_drv_progress = 0;
        pDCxt->driver_state = DRIVER_STATE_RX_PROCESSING;
        pReq = pDCxt->pRxReq;
        pDCxt->pRxReq = NULL;
        if (A_OK != Driver_RecvPacket(pCxt, pReq))
        {
            /* if this happens it is a critical error */
            A_ASSERT(0);
        }
        /* FIXME: as an optimization the next lookahead may have
         * been acquired from the trailer of the previous rx packet.
         * in that case we should pre-load the lookahead so as to
         * avoid reading it from the registers. */
        /* reset the lookahead for the next read operation */
        pDCxt->lookAhead = 0;
        /* check to see if a deferred bus request has completed. if so
      * process it. */
        if (pDCxt->booleans & SDHD_BOOL_DMA_COMPLETE)
        {
            /* get the request */
            if (A_OK == Driver_CompleteRequest(pCxt, pDCxt->spi_hcd.pCurrentRequest))
            {
                /* clear the pending request and the boolean */
                pDCxt->spi_hcd.pCurrentRequest = NULL;
                pDCxt->booleans &= ~SDHD_BOOL_DMA_COMPLETE;
            }
        }
    }

    pDCxt->driver_state = DRIVER_STATE_PENDING_CONDITION_D;

    if ((scope & DRIVER_SCOPE_TX) && true == Driver_TxReady(pCxt))
    {
        // g_drv_progress = 0;
        pDCxt->driver_state = DRIVER_STATE_TX_PROCESSING;
        /* after processing any outstanding device interrupts
         * see if there is a packet that requires transmitting
         */
        if (pDCxt->txQueue.count)
        {
            /* accesslock here to sync with threads calling
             * submit TX
             */
            TXQUEUE_ACCESS_ACQUIRE(pCxt);
            {
                pReq = A_NETBUF_DEQUEUE(&(pDCxt->txQueue));
            }
            TXQUEUE_ACCESS_RELEASE(pCxt);

            if (pReq != NULL)
            {
                Driver_SendPacket(pCxt, pReq);
            }
        }
    }

    pDCxt->driver_state = DRIVER_STATE_PENDING_CONDITION_E;

    /* execute any asynchronous/special request when possible */
    if (0 == (pDCxt->booleans & SDHD_BOOL_DMA_IN_PROG) && pDCxt->asynchRequest)
    {
        // g_drv_progress = 0;
        pDCxt->driver_state = DRIVER_STATE_ASYNC_PROCESSING;
        pDCxt->asynchRequest(pCxt);
        pDCxt->driver_state = DRIVER_STATE_PENDING_CONDITION_F;
    }

    pDCxt->driver_state = DRIVER_STATE_PENDING_CONDITION_G;
    /* allow caller to block if all necessary conditions are satisfied */
    if (pCanBlock)
    {
        if ((pDCxt->spi_hcd.PendingIrqAcks == 0 || pDCxt->rxBufferStatus == false) &&
            (pDCxt->spi_hcd.IrqDetected == false) && (pDCxt->txQueue.count == 0 || Driver_TxReady(pCxt) == false) &&
            (0 == (pDCxt->booleans & SDHD_BOOL_DMA_COMPLETE)))
        {
            *pCanBlock = true;

            if (pDCxt->creditDeadlock == true)
            {
                if (pBlock_msec)
                {
                    *pBlock_msec = DEADLOCK_BLOCK_MSEC;
                }

                pDCxt->creditDeadlockCounter++;
            }

            pDCxt->driver_state = DRIVER_STATE_IDLE;
        }
        else
        {
            *pCanBlock = false;
        }
    }

    // Used for testing purpose
    // A_MDELAY(1);
    // g_drv_progress++;
    // if (g_drv_progress == 20)
    // {
        // extern A_STATUS DoPioWriteInternal(void *pCxt, uint16_t Addr, uint32_t Value);
        // DoPioWriteInternal(pCxt, ATH_SPI_INTR_ENABLE_REG, 0x1f);
        // g_drv_progress = 0;
    // }

    return A_OK;
}

/*****************************************************************************/
/*  Driver_ReportRxBuffStatus - Tracks availability of Rx Buffers for those
 *	 systems that have a limited pool. The driver quearies this status before
 *	 initiating a RX packet transaction.
 *      void *pCxt - the driver context.
 *      boolean status - new status true - RX buffers are available,
 *			false - RX buffers are not available.
 *****************************************************************************/

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)
uint8_t GetQueueIndexByEPID(uint8_t epid)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
        if (GET_QUEUE_MASK(i) & (1 << epid))
            break;
    if (i >= 1 && i < 8)
        i = 7;
    return i;
}

uint8_t GetQueueCtrlIndexByEPID(uint8_t epid)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
        if (GET_QUEUE_MASK(i) & (1 << epid))
            break;
    return i;
}

void Driver_ReportRxBuffStatus(void *pCxt, boolean status, uint8_t epid)
{
    boolean oldStatus; // CHECKME
    uint8_t bufNdx;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    bufNdx = GetQueueIndexByEPID(epid);
    /* NOTE: Dont acquire the lock here instead the
     * caller should acquire this lock if necessary prior to calling this function */
    // RXBUFFER_ACCESS_ACQUIRE(pCxt);

    oldStatus = pDCxt->rxBufferStatus;

    if (status)
    {
        pDCxt->rxMultiBufferStatus |= 1 << bufNdx;
    }
    else
    {
        pDCxt->rxMultiBufferStatus &= ~(1 << bufNdx);
    }
    pDCxt->rxBufferStatus = (pDCxt->rxMultiBufferStatus != 0);

    // RXBUFFER_ACCESS_RELEASE(pCxt);
    /* the driver thread may have blocked on this
     * status so conditionally wake up the thread
     * via QueueWork */
    if ((oldStatus == false) && (status == true))
    {
        if (pDCxt->hcd.PendingIrqAcks)
        {
            DRIVER_WAKE_DRIVER(pCxt);
        }
    }
}
#else
void Driver_ReportRxBuffStatus(void *pCxt, boolean status)
{
    boolean oldStatus;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    /* NOTE: Dont acquire the lock here instead the
     * caller should acquire this lock if necessary prior to calling this function */
    // RXBUFFER_ACCESS_ACQUIRE(pCxt);
    {
        oldStatus = pDCxt->rxBufferStatus;
        pDCxt->rxBufferStatus = status;
    }
    // RXBUFFER_ACCESS_RELEASE(pCxt);
    /* the driver thread may have blocked on this
     * status so conditionally wake up the thread
     * via QueueWork */
    if (oldStatus == false && status == true)
    {
        if (pDCxt->spi_hcd.PendingIrqAcks)
        {
            DRIVER_WAKE_DRIVER(pCxt);
        }
    }
}
#endif

/*****************************************************************************/
/*  Driver_CompleteRequest - Completes a deferred request. One that finished
 *      asynchronously. Such a request must have a non-null callback that will
 *      be called by this function to fullfill the operation.
 *      void *pCxt - the driver context.
 *      void *pReq - the bus request object.
 *****************************************************************************/
A_STATUS
Driver_CompleteRequest(void *pCxt, void *pReq)
{
    A_STATUS status = A_ERROR;
    void (*cb)(void *, void *);
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    /* A hardware WAR exists whereby interrupts from the device are disabled
     * during an ongoing bus transaction.  This function is called when
     * the bus transaction completes.  if irqMask is set then it means that
     * interrupts should be re-enabled */
    if (pDCxt->spi_hcd.irqMask)
    {
        Hcd_UnmaskInterrupts(pCxt, pDCxt->spi_hcd.irqMask);
        pDCxt->spi_hcd.irqMask = 0;
    }

    if (pReq != NULL && NULL != (cb = A_NETBUF_GET_ELEM(pReq, A_REQ_CALLBACK)))
    {
        cb(pCxt, pReq);
        status = A_OK;
    }
    else
    {
        A_ASSERT(0);
    }

    return status;
}

/*****************************************************************************/
/*  Driver_TxReady - Determines whether it is safe to start a TX operation. If
 *     a TX operation can be started this function returns true else false.
 *      void *pCxt - the driver context.
 *****************************************************************************/
boolean Driver_TxReady(void *pCxt)
{
    boolean res = false;
    void *pReq;
    A_ENDPOINT_T *pEp;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    pDCxt->creditDeadlock = false;

    do
    {
        /* there can be no errors */
        if (pDCxt->booleans & SDHD_BOOL_FATAL_ERROR)
        {
            break;
        }
        /* there can be no in-complete requests */
        if ((pDCxt->booleans & SDHD_BOOL_DMA_IN_PROG) || (pDCxt->booleans & SDHD_BOOL_DMA_COMPLETE) ||
            /* the write buffer watermark interrupt must not be configured */
            (pDCxt->booleans & SDHD_BOOL_DMA_WRITE_WAIT_FOR_BUFFER))
        {
            break;
        }

        /* there must be enough credits for the target endpoint */
        if (NULL == (pReq = A_NETBUF_PEEK_QUEUE(&(pDCxt->txQueue))))
        {
            break;
        }

        if (NULL == (pEp = Util_GetEndpoint(pCxt, A_NETBUF_GET_ELEM(pReq, A_REQ_EPID))))
        {
            break;
        }
        /* ensure as sanity check that the request length is less than a credit length */
        if (pDCxt->creditSize < DEV_CALC_SEND_PADDED_LEN(pDCxt, A_NETBUF_LEN(pReq)))
        {
            // THIS IS AN ERROR AS REQUESTS SHOULD NEVER EXCEED THE CREDIT SIZE
            A_ASSERT(0);
#if DRIVER_CONFIG_DISABLE_ASSERT
            break;
#endif
        }
#if RESERVE_ONE_CREDIT_FOR_CONTROL
        if ((pEp->epIdx == 0) || (pEp->epIdx == 1))
        {
            /* confirm there are enough credits for this transfer */
            if (0 == pEp->credits)
            {
                // if(pDCxt->rxBufferStatus == false)
                {
                    /* need to use alternate mode to acquire credits */
                    Htc_GetCreditCounterUpdate(pCxt, A_NETBUF_GET_ELEM(pReq, A_REQ_EPID));

                    if (0 == pEp->credits)
                    { /* test again in case no new credits were acquired */
                        if (pDCxt->rxBufferStatus == false)
                        {
                            /* with no rx buffers to receive a credit report and no interrupt
                             * credits a condition exists where the driver may become deadlocked.
                             * Hence the creditDeadlock boolean is set to prevent the driver from
                             * sleeping.  this will cause the driver to poll for credits until
                             * the condition is passed.
                             */
                            pDCxt->creditDeadlock = true;
#if 0
                            assert(0);
                            if(pDCxt->creditDeadlockCounter >= MAX_CREDIT_DEADLOCK_ATTEMPTS){
                                /* This is a serious condition that can be brought about by
                                 * a flood of RX packets which generate TX responses and do not
                                 * return the RX buffer to the driver until the TX response
                                 * completes. To mitigate this situation purge the tx queue
                                 * of any packets on data endpoints.
                                 */
                                Driver_DropTxDataPackets(pCxt);
                                pDCxt->creditDeadlockCounter = 0;
                            }
#endif
                        }

                        break;
                    }
                }
                // else{
                //	break;/* wait for credit report from device */
                //}
            }
        }

        else
        {
            /* confirm there are enough credits for this transfer */
            if (pEp->credits <= 1)
            {
                // if(pDCxt->rxBufferStatus == false)
                {
                    /* need to use alternate mode to acquire credits */
                    Htc_GetCreditCounterUpdate(pCxt, A_NETBUF_GET_ELEM(pReq, A_REQ_EPID));

                    if (pEp->credits <= 1)
                    { /* test again in case no new credits were acquired */
                        if (pDCxt->rxBufferStatus == false)
                        {
                            /* with no rx buffers to receive a credit report and no interrupt
                             * credits a condition exists where the driver may become deadlocked.
                             * Hence the creditDeadlock boolean is set to prevent the driver from
                             * sleeping.  this will cause the driver to poll for credits until
                             * the condition is passed.
                             */
                            pDCxt->creditDeadlock = true;
#if 0
        				if(pDCxt->creditDeadlockCounter >= MAX_CREDIT_DEADLOCK_ATTEMPTS){
        					/* This is a serious condition that can be brought about by
        					 * a flood of RX packets which generate TX responses and do not
        					 * return the RX buffer to the driver until the TX response
        					 * completes. To mitigate this situation purge the tx queue
        					 * of any packets on data endpoints.
        					 */
        					Driver_DropTxDataPackets(pCxt);
        					pDCxt->creditDeadlockCounter = 0;
        				}
#endif
                        }

                        break;
                    }
                }
                // else{
                //	break;/* wait for credit report from device */
                //}
            }

        } /*(pEp->epIdx == 0) || (pEp->epIdx == 1)*/
#else
        if (0 == pEp->credits)
        {
            // if(pDCxt->rxBufferStatus == false)
            {
                /* need to use alternate mode to acquire credits */
                Htc_GetCreditCounterUpdate(pCxt, A_NETBUF_GET_ELEM(pReq, A_REQ_EPID));

                if (0 == pEp->credits)
                { /* test again in case no new credits were acquired */
                    if (pDCxt->rxBufferStatus == false)
                    {
                        /* with no rx buffers to receive a credit report and no interrupt
                         * credits a condition exists where the driver may become deadlocked.
                         * Hence the creditDeadlock boolean is set to prevent the driver from
                         * sleeping.  this will cause the driver to poll for credits until
                         * the condition is passed.
                         */
                        pDCxt->creditDeadlock = true;
#if 1
                        if(pDCxt->creditDeadlockCounter >= MAX_CREDIT_DEADLOCK_ATTEMPTS){
                            /* This is a serious condition that can be brought about by
                             * a flood of RX packets which generate TX responses and do not
                             * return the RX buffer to the driver until the TX response
                             * completes. To mitigate this situation purge the tx queue
                             * of any packets on data endpoints.
                             */
                            Driver_DropTxDataPackets(pCxt);
                            pDCxt->creditDeadlockCounter = 0;
                        }
#endif
                    }

                    break;
                }
            }
            // else{
            //	break;/* wait for credit report from device */
            //}
        }
#endif

        pDCxt->creditDeadlockCounter = 0;

        /* there must be enough write buffer space in the target fifo */
        if (pDCxt->spi_hcd.WriteBufferSpace <
            DEV_CALC_SEND_PADDED_LEN(pDCxt, A_NETBUF_LEN(pReq)) + ATH_SPI_WRBUF_RSVD_BYTES)
        {
            /* this will read the internal register to get the latest value for write buffer space */
            Hcd_RefreshWriteBufferSpace(pCxt);

            if (pDCxt->spi_hcd.WriteBufferSpace <
                DEV_CALC_SEND_PADDED_LEN(pDCxt, A_NETBUF_LEN(pReq)) + ATH_SPI_WRBUF_RSVD_BYTES)
            {
                /* there currently isn't enough space in the target fifo to accept this packet.
                 * Hence setup the write buffer watermark interrupt for future notification and exit. */
                Hcd_ProgramWriteBufferWaterMark(pCxt, DEV_CALC_SEND_PADDED_LEN(pDCxt, A_NETBUF_LEN(pReq)));
                /* wait for interrupt to indicate space available */
                break;
            }
        }
        /* all conditions are met to transmit a packet */
        res = true;
    } while (0);

    return res;
}

/*****************************************************************************/
/*  Driver_RxReady - Determines whether it is safe to start a RX operation. If
 *     a RX operation can be started this function returns true else false.
 *      void *pCxt - the driver context.
 *****************************************************************************/
boolean Driver_RxReady(void *pCxt)
{
    boolean res = false;
    HTC_FRAME_HDR *pHTCHdr;
    int32_t fullLength;
    uint32_t interrupts;
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);

    do
    {
        /* there can be no errors */
        if (pDCxt->booleans & SDHD_BOOL_FATAL_ERROR)
        {
            break;
        }
        /* there can be no in-complete requests */
        if ((pDCxt->booleans & SDHD_BOOL_DMA_IN_PROG) || (pDCxt->booleans & SDHD_BOOL_DMA_COMPLETE))
        {
            break;
        }
        /* the wifi device must have indicated that a packet is ready */
        if (0 == pDCxt->spi_hcd.PendingIrqAcks)
        {
            break;
        }
        /* there must be rx buffers to process the request */
        if (false == pDCxt->rxBufferStatus && NULL == pDCxt->pRxReq)
        {
            break;
        }
        /* clear the pkt interrupt if its set. We do this here rather than in the
         * interrupt handler because we will continue to process packets until we
         * read a lookahead==0. only then will we unmask the interrupt */
        if (A_OK != Hcd_DoPioInternalAccess(pCxt, ATH_SPI_INTR_CAUSE_REG, &interrupts, true))
        {
            break;
        }

        if (interrupts & ATH_SPI_INTR_PKT_AVAIL)
        {
            interrupts = ATH_SPI_INTR_PKT_AVAIL;

            if (A_OK != Hcd_DoPioInternalAccess(pCxt, ATH_SPI_INTR_CAUSE_REG, &interrupts, false))
            {
                break;
            }
        }
        /* RX packet interrupt processing must be enabled */
        if (0 == (pDCxt->enabledSpiInts & ATH_SPI_INTR_PKT_AVAIL))
        {
            break;
        }
        /* As a last step read the lookahead and the Receive buffer register in an
         * effort to determine that a complete packet is ready for transfer. Do this
         * operation last as it does require several BUS transactions to complete. */
        if (0 == pDCxt->lookAhead)
        {
            /* need to refresh the lookahead */
            if (A_OK != Hcd_GetLookAhead(pCxt))
            {
                /* this is a major error */
                A_ASSERT(0);
            }
        }
        /* if lookahead is 0 then no need to proceed */
        if (0 == pDCxt->lookAhead)
        {
            if (0 != pDCxt->spi_hcd.PendingIrqAcks)
            {
                /* this should always be true here */
                pDCxt->spi_hcd.PendingIrqAcks = 0;
                Hcd_UnmaskInterrupts(pCxt, ATH_SPI_INTR_PKT_AVAIL);
            }

            break;
        }
        /* compare lookahead and recv buf ready value */
        pHTCHdr = (HTC_FRAME_HDR *)&(pDCxt->lookAhead);
        if (pDCxt->spi_hcd.ReadBufferSpace < sizeof(HTC_FRAME_HDR) ||
            pDCxt->spi_hcd.ReadBufferSpace < A_LE2CPU16(pHTCHdr->PayloadLen))
        {
            /* force a call to BUS_GetLookAhead on the next pass */
            pDCxt->lookAhead = 0;
            break;
        }
        /* there must be resources available to receive a packet */
        if (NULL == pDCxt->pRxReq)
        {
            fullLength =
                (int32_t)DEV_CALC_RECV_PADDED_LEN(pDCxt, A_LE2CPU16(pHTCHdr->PayloadLen) + sizeof(HTC_FRAME_HDR));

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)
            {
                uint32_t flags, bufNdx;

                bufNdx = GetQueueIndexByEPID(pHTCHdr->EndpointID);
                flags = 1 << bufNdx;

                if (pDCxt->rxMultiBufferStatus & flags)
                {
                    /* try to get an RX buffer */
                    pDCxt->pRxReq = CUSTOM_DRIVER_GET_RX_REQ(pCxt, fullLength, pHTCHdr->EndpointID);
                }
            }
#else

            if (pDCxt->rxBufferStatus)
            {
                /* try to get an RX buffer */
                pDCxt->pRxReq = CUSTOM_DRIVER_GET_RX_REQ(pCxt, fullLength);
            }
#endif

            if (pDCxt->pRxReq == NULL)
            {
                break;
            }
            /* init the packet callback */
            A_NETBUF_SET_ELEM(pDCxt->pRxReq, A_REQ_CALLBACK, Driver_RxComplete);
        }
        /* all conditions are met to receive a packet */
        res = true;
    } while (0);

    return res;
}


extern void socket_set_driver_error(void *ctxt, int32_t error);
void Driver_DropTxDataPackets(void *pCxt)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(pCxt);
    void *pReq;
    uint16_t i, count;

    if (pDCxt->txQueue.count)
    {
        count = pDCxt->txQueue.count;
        /* accesslock here to sync with threads calling
         * submit TX
         */
        for (i = 0; i < count; i++)
        {
            TXQUEUE_ACCESS_ACQUIRE(pCxt);
            {
                pReq = A_NETBUF_DEQUEUE(&(pDCxt->txQueue));
            }
            TXQUEUE_ACCESS_RELEASE(pCxt);

            if (pReq != NULL)
            {
                if (A_NETBUF_GET_ELEM(pReq, A_REQ_EPID) > ENDPOINT_1)
                {
                    /* Get netbuffer socket context and set it into error state. */
                    /* This field is used/readed in unblock function. */
                    A_NETBUF_PTR a_netbuf_ptr = (A_NETBUF *)pReq;
                    DRV_BUFFER_PTR db_ptr = a_netbuf_ptr->native_orig;
                    socket_set_driver_error(db_ptr->context, A_DEADLOCK);
                    Driver_CompleteRequest(pCxt, pReq);
                }
                else
                {
                    TXQUEUE_ACCESS_ACQUIRE(pCxt);
                    /* re-queue the packet as it is not one we can purge */
                    A_NETBUF_ENQUEUE(&(pDCxt->txQueue), pReq);
                    TXQUEUE_ACCESS_RELEASE(pCxt);
                }
            }
        }
    }
}

#if defined(DRIVER_CONFIG_IMPLEMENT_RX_FREE_MULTIPLE_QUEUE)
extern A_NETBUF *ep0_buf;

void Driver_ReportReverseCredits(void *pReq)
{
    A_DRIVER_CONTEXT *pDCxt = GET_DRIVER_COMMON(p_Global_Cxt);
    A_NETBUF *pCurrTxReq;

#if 0
    A_ENDPOINT_T *pEp;
    void *osbuf;
    void   *pReq;
    if (pDCxt->txQueue.count != 0)
    {
        pReq = A_NETBUF_PEEK_QUEUE(&(pDCxt->txQueue));
        pEp = Util_GetEndpoint(p_Global_Cxt, A_NETBUF_GET_ELEM(pReq, A_REQ_EPID));
        if (pEp->credits != 0)
            return;
    }
    osbuf = A_NETBUF_ALLOC(0);
    A_NETBUF_SET_ELEM(osbuf, A_REQ_EPID, 0);
    Driver_SubmitTxRequest(p_Global_Cxt, osbuf);
#else

    if (hasRCSReport() == false)
    {
        return;
    }

    pCurrTxReq = A_NETBUF_PEEK_QUEUE(&(pDCxt->txQueue));
    if (pCurrTxReq != NULL)
    {
        if (A_NETBUF_GET_ELEM(pCurrTxReq, A_REQ_EPID) == ENDPOINT_0)
            return;
    }

    a_netbuf_reinit(ep0_buf, 0);
    A_NETBUF_SET_ELEM(ep0_buf, A_REQ_EPID, 0);

    qosal_intr_disable();
    Driver_SubmitEp0TxRequest(p_Global_Cxt, ep0_buf);

    qosal_intr_enable();

#endif
}

#endif

A_STATUS
Driver_WaitForCondition(void *pCxt, volatile boolean *pCond, boolean value, uint32_t msec)
{
    A_STATUS status = A_OK;

    event_t *pEvent = &GET_DRIVER_CXT(pCxt)->userWakeEvent;

    while (1)
    {
        if (*pCond != value)
        {
            uint32_t setFlags;
            if (A_OK != A_EVENT_WAIT(pEvent, 0x01, true, msec, &setFlags))
            {
                status = A_ERROR;
                break;
            }
            else
            {
                A_EVENT_CLEAR(pEvent, 0x01);
            }
        }
        else
        {
            break;
        }
    }

    return status;
}

#if 1
A_STATUS setup_host_dset(void *handle)
{
    return wmi_dset_host_cfg_cmd(handle);
}
#else
A_STATUS setup_host_dset(void *handle)
{
    return A_OK;
}
#endif

void Atheros_Driver_Task(void *pCxt)
{
    boolean canBlock = false;
    uint16_t block_msec = 1000; // TODO: what should be a correct value??
    uint32_t timeout = 0;

    do
    {
        if (A_OK == Driver_Init(pCxt))
        {
            while (1)
            {
                if (canBlock)
                {
                    GET_DRIVER_COMMON(pCxt)->driverSleep = true;
                    timeout = block_msec;

                    if (block_msec && timeout == 0)
                    {
                        timeout = 1;
                    }
                    event_t *pEvent = &(GET_DRIVER_CXT(pCxt)->driverWakeEvent);
                    uint32_t setFlags;
                    if (A_OK != A_EVENT_WAIT(pEvent, 0x01, true, timeout, &setFlags))
                    {
                        if (timeout == 0)
                        {
                            break;
                        }
                    }
                }

                GET_DRIVER_COMMON(pCxt)->driverSleep = false;

                if (GET_DRIVER_COMMON(pCxt)->driverShutdown == true)
                {
                    break;
                }

                Driver_Main(pCxt, DRIVER_SCOPE_RX | DRIVER_SCOPE_TX, &canBlock, &block_msec);
            }

            Driver_DeInit(pCxt);
            DRIVER_WAKE_USER(pCxt);
        }
#if QOSAL_TASK_DESTRUCTION
    } while (0);

    atheros_wifi_task_id = QOSAL_NULL_TASK_HANDLE;
#else
        /* block on this event until a task wants to re-activate the driver thread */
        // qosal_task_suspend(NULL);
    } while (1);
#endif
}

// The WLAN driver task priority is set to the next highest priority (0 is highest priority)
// because blockForResponse() needs to temporarily boost caller task's priority
// over that of the driver task
#define ATHEROS_TASK_PRIORITY (osPriorityNormal1)
#define ATHEROS_TASK_STACKSIZE 2000

/* NOTE: the whole file will be included as part of freertos env */
/* TODO: check stackSize/sizeof(portSTACK_TYPE) */
uint16_t atheros_wifi_task_prio = ATHEROS_TASK_PRIORITY;

A_STATUS Driver_CreateThread(void *pCxt)
{
    assert(atheros_wifi_task_id == NULL);

    osThreadAttr_t attr = {0};
    attr.name = "Atheros_Wifi_Task";
    attr.stack_mem  = malloc(ATHEROS_TASK_STACKSIZE);
    attr.cb_mem  = &thread_cb;
    attr.stack_size = ATHEROS_TASK_STACKSIZE;
    attr.cb_size = sizeof(mbed_rtos_storage_thread_t);
    attr.priority = ATHEROS_TASK_PRIORITY;

    atheros_wifi_task_id = osThreadNew((osThreadFunc_t)Atheros_Driver_Task, pCxt, &attr);
    return (NULL == atheros_wifi_task_id) ? A_ERROR : A_OK;
}

// Compliments CreateDriverThread
A_STATUS
Driver_DestroyThread(void *pCxt)
{
    GET_DRIVER_COMMON(pCxt)->driverShutdown = true;
    DRIVER_WAKE_DRIVER(pCxt);
    return A_OK;
}

void Driver_WakeDriver(void *pCxt)
{
    A_EVENT_SET(&(GET_DRIVER_CXT(pCxt)->driverWakeEvent), 0x01);
}

void Driver_WakeUser(void *pCxt)
{
    A_EVENT_SET(&(GET_DRIVER_CXT(pCxt)->userWakeEvent), 0x01);
}

void qcadrv_assert_func(const char *func_name, int line)
{
    volatile int i = 1;
    while (i)
        ;
}
