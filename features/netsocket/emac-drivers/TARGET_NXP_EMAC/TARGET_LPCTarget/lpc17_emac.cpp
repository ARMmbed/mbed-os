/**********************************************************************
* $Id$        lpc17_emac.c            2011-11-20
*//**
* @file        lpc17_emac.c
* @brief    LPC17 ethernet driver for LWIP
* @version    1.0
* @date        20. Nov. 2011
* @author    NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
**********************************************************************/

#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "netsocket/nsapi_types.h"

#include "lpc_emac_config.h"
#include "lpc17_emac.h"
#include "lpc17xx_emac.h"
#include "lpc_phy.h"
#include "mbed_interface.h"

#ifndef LPC_EMAC_RMII
#error LPC_EMAC_RMII is not defined!
#endif

#if LPC_NUM_BUFF_TXDESCS < 2
#error LPC_NUM_BUFF_TXDESCS must be at least 2
#endif

#if LPC_NUM_BUFF_RXDESCS < 3
#error LPC_NUM_BUFF_RXDESCS must be at least 3
#endif

/** @defgroup lwip17xx_emac_DRIVER    lpc17 EMAC driver for LWIP
 * @ingroup lwip_emac
 *
 * @{
 */

/** \brief  Driver transmit and receive thread priorities
 *
 * Thread priorities for receive thread and TX cleanup thread. Alter
 * to prioritize receive or transmit bandwidth. In a heavily loaded
 * system or with LEIP_DEBUG enabled, the priorities might be better
 * the same. */
#define RX_PRIORITY   (osPriorityNormal)
#define TX_PRIORITY   (osPriorityNormal)
#define PHY_PRIORITY  (osPriorityNormal)

/** \brief  Debug output formatter lock define
 *
 * When using FreeRTOS and with LWIP_DEBUG enabled, enabling this
 * define will allow RX debug messages to not interleave with the
 * TX messages (so they are actually readable). Not enabling this
 * define when the system is under load will cause the output to
 * be unreadable. There is a small tradeoff in performance for this
 * so use it only for debug. */
//#define LOCK_RX_THREAD

/** \brief  Receive group interrupts
 */
#define RXINTGROUP (EMAC_INT_RX_OVERRUN | EMAC_INT_RX_ERR | EMAC_INT_RX_DONE)

/** \brief  Transmit group interrupts
 */
#define TXINTGROUP (EMAC_INT_TX_UNDERRUN | EMAC_INT_TX_ERR | EMAC_INT_TX_DONE)

/** \brief  Signal used for ethernet ISR to signal packet_rx() thread.
 */
#define RX_SIGNAL  1

 /** \brief  Structure of a TX/RX descriptor
 */
typedef struct
{
    volatile uint32_t packet;        /**< Pointer to buffer */
    volatile uint32_t control;       /**< Control word */
} LPC_TXRX_DESC_T;

/** \brief  Structure of a RX status entry
 */
typedef struct
{
    volatile uint32_t statusinfo;   /**< RX status word */
    volatile uint32_t statushashcrc; /**< RX hash CRC */
} LPC_TXRX_STATUS_T;

/* LPC EMAC driver data structure */
struct lpc_enetdata {
    /* prxs must be 8 byte aligned! */
    LPC_TXRX_STATUS_T prxs[LPC_NUM_BUFF_RXDESCS]; /**< Pointer to RX statuses */
    LPC_TXRX_DESC_T ptxd[LPC_NUM_BUFF_TXDESCS];   /**< Pointer to TX descriptor list */
    LPC_TXRX_STATUS_T ptxs[LPC_NUM_BUFF_TXDESCS]; /**< Pointer to TX statuses */
    LPC_TXRX_DESC_T prxd[LPC_NUM_BUFF_RXDESCS];   /**< Pointer to RX descriptor list */
    emac_mem_buf_t *rxb[LPC_NUM_BUFF_RXDESCS]; /**< RX pbuf pointer list, zero-copy mode */
    uint32_t rx_fill_desc_index; /**< RX descriptor next available index */
    volatile uint32_t rx_free_descs; /**< Count of free RX descriptors */
    emac_mem_buf_t *txb[LPC_NUM_BUFF_TXDESCS]; /**< TX pbuf pointer list, zero-copy mode */
    uint32_t lpc_last_tx_idx; /**< TX last descriptor index, zero-copy mode */
    uint32_t lpc_reserved_tx_num; /**< Number of reserved TX descriptors, zero-copy mode */
};

#if defined(TARGET_LPC1768) || defined(TARGET_LPC1769)
/** \brief Group LPC17xx processors into one definition
 */
#define TARGET_LPC17XX
#endif

#if defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
#  if defined (__ICCARM__)
#     define ETHMEM_SECTION
#  elif defined(TOOLCHAIN_GCC_CR)
#     define ETHMEM_SECTION __attribute__((section(".data.$RamPeriph32"), aligned))
#  else
#     define ETHMEM_SECTION __attribute__((section("AHBSRAM0"),aligned))
#  endif
#elif defined(TARGET_LPC17XX)
#  if defined(TOOLCHAIN_GCC_ARM) || defined(TOOLCHAIN_ARM)
#     define ETHMEM_SECTION __attribute__((section("AHBSRAM1"),aligned))
#  endif
#endif

#ifndef ETHMEM_SECTION
#define ETHMEM_SECTION
#endif

/** \brief  LPC EMAC driver work data
 */
#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#pragma data_alignment = 8
#endif
ETHMEM_SECTION struct lpc_enetdata lpc_enetdata;

#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#pragma data_alignment = 8
#endif
ETHMEM_SECTION uint8_t rx_thread_stack[DEFAULT_THREAD_STACKSIZE];

#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#pragma data_alignment = 8
#endif
ETHMEM_SECTION uint8_t tx_clean_thread_stack[DEFAULT_THREAD_STACKSIZE];

#if defined (__ICCARM__)
#pragma location = ".ethusbram"
#pragma data_alignment = 8
#endif
ETHMEM_SECTION uint8_t phy_thread_stack[DEFAULT_THREAD_STACKSIZE];

static osThreadId_t create_new_thread(const char *threadName, void (*thread)(void *arg), void *arg, void *stack_ptr, int stacksize, osPriority_t priority, mbed_rtos_storage_thread_t *thread_cb)
{
    osThreadAttr_t attr = {0};
    attr.name = threadName;
    attr.stack_mem  = stack_ptr;
    attr.cb_mem  = thread_cb;
    attr.stack_size = stacksize;
    attr.cb_size = sizeof(mbed_rtos_storage_thread_t);
    attr.priority = priority;
    return osThreadNew(thread, arg, &attr);
}

/** \brief  Queues a memory buffer into the RX descriptor list
 *
 *  \param[in] p            Pointer to buffer to queue
 */
void LPC17_EMAC::lpc_rxqueue_pbuf(emac_mem_buf_t *p)
{
    uint32_t idx;

    /* Get next free descriptor index */
    idx = lpc_enetdata.rx_fill_desc_index;

    /* Setup descriptor and clear statuses */
    lpc_enetdata.prxd[idx].control = EMAC_RCTRL_INT | ((uint32_t) (memory_manager->get_len(p) - 1));
    lpc_enetdata.prxd[idx].packet = (uint32_t) memory_manager->get_ptr(p);
    lpc_enetdata.prxs[idx].statusinfo = 0xFFFFFFFF;
    lpc_enetdata.prxs[idx].statushashcrc = 0xFFFFFFFF;

    /* Save pbuf pointer for push to network layer later */
    lpc_enetdata.rxb[idx] = p;

    /* Wrap at end of descriptor list */
    idx++;
    if (idx >= LPC_NUM_BUFF_RXDESCS) {
        idx = 0;
    }

    /* Queue descriptor(s) */
    lpc_enetdata.rx_free_descs -= 1;
    lpc_enetdata.rx_fill_desc_index = idx;
    LPC_EMAC->RxConsumeIndex = idx;
}

/** \brief  Attempt to allocate and requeue a new memory buffer for RX
 *
 *  \returns         >= 1 if a packet or packets were allocated and requeued, otherwise 0
 */
int32_t LPC17_EMAC::lpc_rx_queue()
{
    //struct lpc_enetdata *lpc_enetif = netif->state;
    emac_mem_buf_t *p;
    int32_t queued = 0;

    /* Attempt to requeue as many packets as possible */
    while (lpc_enetdata.rx_free_descs > 0) {
        /* Allocate a pbuf from the pool. We need to allocate at the
           maximum size as we don't know the size of the yet to be
           received packet. */
        p = memory_manager->alloc_heap(EMAC_ETH_MAX_FLEN, 0);
        if (p == NULL) {
            return queued;
        }

        /* Queue packet */
        lpc_rxqueue_pbuf(p);

        /* Update queued count */
        queued++;
    }

    return queued;
}

/** \brief  Sets up the RX descriptor ring buffers.
 *
 *  This function sets up the descriptor list used for receive packets.
 *
 *  \returns                   Always returns ERR_OK
 */
bool LPC17_EMAC::lpc_rx_setup()
{
    /* Setup pointers to RX structures */
    LPC_EMAC->RxDescriptor = (uint32_t) &lpc_enetdata.prxd[0];
    LPC_EMAC->RxStatus = (uint32_t) &lpc_enetdata.prxs[0];
    LPC_EMAC->RxDescriptorNumber = LPC_NUM_BUFF_RXDESCS - 1;

    lpc_enetdata.rx_free_descs = LPC_NUM_BUFF_RXDESCS;
    lpc_enetdata.rx_fill_desc_index = 0;

    /* Build RX buffer and descriptors */
    lpc_rx_queue();

    return true;
}

/** \brief  Allocates a memory buffer and returns the data from the incoming packet.
 *
 *  \return a buffer filled with the received packet (including MAC header)
 *         NULL on memory error
 */
emac_mem_buf_t *LPC17_EMAC::lpc_low_level_input()
{
    emac_mem_buf_t *p = NULL;
    uint32_t idx, length;
    uint16_t origLength;

#ifdef LOCK_RX_THREAD
    TXLockMutex.lock();
#endif

    /* Monitor RX overrun status. This should never happen unless
       (possibly) the internal bus is behing held up by something.
       Unless your system is running at a very low clock speed or
       there are possibilities that the internal buses may be held
       up for a long time, this can probably safely be removed. */
    if (LPC_EMAC->IntStatus & EMAC_INT_RX_OVERRUN) {

        /* Temporarily disable RX */
        LPC_EMAC->MAC1 &= ~EMAC_MAC1_REC_EN;

        /* Reset the RX side */
        LPC_EMAC->MAC1 |= EMAC_MAC1_RES_RX;
        LPC_EMAC->IntClear = EMAC_INT_RX_OVERRUN;

        /* De-allocate all queued RX pbufs */
        for (idx = 0; idx < LPC_NUM_BUFF_RXDESCS; idx++) {
            if (lpc_enetdata.rxb[idx] != NULL) {
                memory_manager->free(lpc_enetdata.rxb[idx]);
                lpc_enetdata.rxb[idx] = NULL;
            }
        }

        /* Start RX side again */
        lpc_rx_setup();

        /* Re-enable RX */
        LPC_EMAC->MAC1 |= EMAC_MAC1_REC_EN;

#ifdef LOCK_RX_THREAD
        TXLockMutex.unlock();
#endif
        return NULL;
    }

    /* Determine if a frame has been received */
    length = 0;
    idx = LPC_EMAC->RxConsumeIndex;

    if (LPC_EMAC->RxProduceIndex != idx) {
        /* Handle errors */
        if (lpc_enetdata.prxs[idx].statusinfo & (EMAC_RINFO_CRC_ERR |
            EMAC_RINFO_SYM_ERR | EMAC_RINFO_ALIGN_ERR | EMAC_RINFO_LEN_ERR)) {

            /* Re-queue the buffer for receive */
            lpc_enetdata.rx_free_descs++;
            p = lpc_enetdata.rxb[idx];
            lpc_enetdata.rxb[idx] = NULL;
            lpc_rxqueue_pbuf(p);

            p = NULL;
        } else {
            /* A packet is waiting, get length */
            length = (lpc_enetdata.prxs[idx].statusinfo & 0x7FF) + 1;
            length -= 4;

            /* Zero-copy */
            p = lpc_enetdata.rxb[idx];
            origLength = memory_manager->get_len(p);
            memory_manager->set_len(p, length);

            /* Free buffer from descriptor */
            lpc_enetdata.rxb[idx] = NULL;
            lpc_enetdata.rx_free_descs++;

            /* Attempt to queue new buffer(s) */
            if (lpc_rx_queue() == 0) {
                /* Re-queue the buffer for receive */
                memory_manager->set_len(p, origLength);
                lpc_rxqueue_pbuf(p);

#ifdef LOCK_RX_THREAD
                TXLockMutex.unlock();
#endif
                return NULL;
            }
        }
    }

#ifdef LOCK_RX_THREAD
    TXLockMutex.unlock();
#endif

    return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 */
void LPC17_EMAC::lpc_enetif_input()
{
    emac_mem_buf_t *p;

    /* move received packet into a new memory buffer */
    p = lpc_low_level_input();
    if (p == NULL) {
        return;
    }

    emac_link_input_cb(p);
}

/** \brief  Determine if the passed address is usable for the ethernet
 *          DMA controller.
 *
 *  \param[in] addr Address of packet to check for DMA safe operation
 *  \return          1 if the packet address is not safe, otherwise 0
 */
int32_t LPC17_EMAC::lpc_packet_addr_notsafe(void *addr)
{
    /* Check for legal address ranges */
#if defined(TARGET_LPC17XX)
    if ((((uint32_t) addr >= 0x2007C000) && ((uint32_t) addr < 0x20083FFF))) {
#elif defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
    if ((((uint32_t) addr >= 0x20000000) && ((uint32_t) addr < 0x20007FFF))) {
#endif
        return 0;
    }
    return 1;
}

/** \brief  Sets up the TX descriptor ring buffers.
 *
 *  This function sets up the descriptor list used for transmit packets.
 */
bool LPC17_EMAC::lpc_tx_setup()
{
    int32_t idx;

    /* Build TX descriptors for local buffers */
    for (idx = 0; idx < LPC_NUM_BUFF_TXDESCS; idx++) {
        lpc_enetdata.ptxd[idx].control = 0;
        lpc_enetdata.ptxs[idx].statusinfo = 0xFFFFFFFF;
    }

    /* Setup pointers to TX structures */
    LPC_EMAC->TxDescriptor = (uint32_t) &lpc_enetdata.ptxd[0];
    LPC_EMAC->TxStatus = (uint32_t) &lpc_enetdata.ptxs[0];
    LPC_EMAC->TxDescriptorNumber = LPC_NUM_BUFF_TXDESCS - 1;

    lpc_enetdata.lpc_last_tx_idx = 0;
    lpc_enetdata.lpc_reserved_tx_num = 0;

    return true;
}

/** \brief  Free TX buffers that are complete
 *
 *  \param[in] cidx  EMAC current descriptor comsumer index
 */
void LPC17_EMAC::lpc_tx_reclaim_st(uint32_t cidx)
{
    TXLockMutex.lock();

    // If consume index not last freed index or all descriptors in use
    while (cidx != lpc_enetdata.lpc_last_tx_idx || lpc_enetdata.lpc_reserved_tx_num == LPC_NUM_BUFF_TXDESCS) {
        if (lpc_enetdata.txb[lpc_enetdata.lpc_last_tx_idx] != NULL) {
            memory_manager->free(lpc_enetdata.txb[lpc_enetdata.lpc_last_tx_idx]);
            lpc_enetdata.txb[lpc_enetdata.lpc_last_tx_idx] = NULL;
        }

        xTXDCountSem.release();

        lpc_enetdata.lpc_last_tx_idx++;
        if (lpc_enetdata.lpc_last_tx_idx >= LPC_NUM_BUFF_TXDESCS) {
            lpc_enetdata.lpc_last_tx_idx = 0;
        }
        lpc_enetdata.lpc_reserved_tx_num--;
    }

    TXLockMutex.unlock();
}

/** \brief  User call for freeingTX buffers that are complete
 *
 */
void LPC17_EMAC::lpc_tx_reclaim()
{
    lpc_tx_reclaim_st(LPC_EMAC->TxConsumeIndex);
}

 /** \brief  Polls if an available TX descriptor is ready. Can be used to
 *           determine if the low level transmit function will block.
 *
 *  \return 0 if no descriptors are read, or >0
 */
int32_t LPC17_EMAC::lpc_tx_ready()
{
    int32_t fb;
    uint32_t idx, cidx;

    cidx = LPC_EMAC->TxConsumeIndex;
    idx = LPC_EMAC->TxProduceIndex;

    /* Determine number of free buffers */
    if (idx == cidx) {
        fb = LPC_NUM_BUFF_TXDESCS;
    } else if (cidx > idx) {
        fb = (LPC_NUM_BUFF_TXDESCS - 1) -
            ((idx + LPC_NUM_BUFF_TXDESCS) - cidx);
    } else {
        fb = (LPC_NUM_BUFF_TXDESCS - 1) - (cidx - idx);
    }

    return fb;
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
bool LPC17_EMAC::link_out(emac_mem_buf_t *p)
{
    emac_mem_buf_t *q;
    uint32_t idx, notdmasafe = 0;
    emac_mem_buf_t *np;
    int32_t dn;

    /* Zero-copy TX buffers may be fragmented across a memory buffer chain. Determine
       the number of descriptors needed for the transfer and make sure packet addresses
       are DMA safe.

       A DMA safe address is once that uses external memory or periphheral RAM.
       IRAM and FLASH are not safe! */
    dn = 0;
    for (q = p; q != NULL; q = memory_manager->get_next(q)) {
        ++dn;
        void *ptr = memory_manager->get_ptr(q);
        notdmasafe += lpc_packet_addr_notsafe(ptr);
    }

#if LPC_TX_PBUF_BOUNCE_EN==1
    /* If the buffer chain is not DMA safe, a new bounce buffer will be
       created that will be used instead. This requires an copy from the
       non-safe DMA region to the new buffer. */
    if (notdmasafe) {
        /* Allocate a buffer in DMA memory.
           MEMORY MANAGER HEAP MUST BE IN DMA SAFE MEMORY. */
        np = memory_manager->alloc_heap(memory_manager->get_total_len(p), 0);
        if (np == NULL) {
            memory_manager->free(p);
            return false;
        }
        memory_manager->copy(np, p);
        /* use the new buffer for descriptor queueing. The original buffer will
           be de-allocated. */
        memory_manager->free(p);
        p = np;
        dn = 1;
    }
#else
    if (notdmasafe) {
        MBED_ASSERT(0);
    }
#endif

    /* Wait until enough descriptors are available for the transfer. */
    /* THIS WILL BLOCK UNTIL THERE ARE ENOUGH DESCRIPTORS AVAILABLE */
    for (int32_t count = 0; count < dn; count++) {
        xTXDCountSem.acquire();
    }

    MBED_ASSERT(dn <= lpc_tx_ready());

    TXLockMutex.lock();

    /* Get free TX buffer index */
    idx = LPC_EMAC->TxProduceIndex;

    /* Setup transfers */
    q = p;
    while (dn > 0) {
        dn--;

        /* Only save pointer to free on last descriptor */
        if (dn == 0) {
            /* Save size of packet and signal it's ready */
            lpc_enetdata.ptxd[idx].control = (memory_manager->get_len(q) - 1) | EMAC_TCTRL_INT |
                EMAC_TCTRL_LAST;
            lpc_enetdata.txb[idx] = p;
        }
        else {
            /* Save size of packet, descriptor is not last */
            lpc_enetdata.ptxd[idx].control = (memory_manager->get_len(q) - 1) | EMAC_TCTRL_INT;
            lpc_enetdata.txb[idx] = NULL;
        }

        lpc_enetdata.ptxd[idx].packet = (uint32_t) memory_manager->get_ptr(q);

        q = memory_manager->get_next(q);

        idx++;
        if (idx >= LPC_NUM_BUFF_TXDESCS) {
            idx = 0;
        }
        lpc_enetdata.lpc_reserved_tx_num++;
    }

    LPC_EMAC->TxProduceIndex = idx;

    TXLockMutex.unlock();

    return true;
}

/** \brief  LPC EMAC interrupt handler.
 *
 *  This function handles the transmit, receive, and error interrupt of
 *  the LPC177x_8x. This is meant to be used when NO_SYS=0.
 */
void LPC17xxEthernetHandler(void)
{
    LPC17_EMAC &emac = LPC17_EMAC::get_instance();

    uint32_t ints;

    /* Interrupts are of 2 groups - transmit or receive. Based on the
       interrupt, kick off the receive or transmit (cleanup) task */

    /* Get pending interrupts */
    ints = LPC_EMAC->IntStatus;

    if (ints & RXINTGROUP) {
        /* RX group interrupt(s): Give signal to wakeup RX receive task.*/
        osThreadFlagsSet(emac.RxThread, RX_SIGNAL);
    }

    if (ints & TXINTGROUP) {
        /* TX group interrupt(s): Give semaphore to wakeup TX cleanup task. */
        emac.TxCleanSem.release();
    }

    /* Clear pending interrupts */
    LPC_EMAC->IntClear = ints;
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the IP stacks core.
 *
 *  \param[in] pvParameters Not used yet
 */
void LPC17_EMAC::packet_rx(void* pvParameters)
{
    LPC17_EMAC *lpc17_emac = static_cast<LPC17_EMAC *>(pvParameters);

    while (1) {
        /* Wait for receive task to wakeup */
        osThreadFlagsWait(RX_SIGNAL, 0, osWaitForever);

        /* Process packets until all empty */
        while (LPC_EMAC->RxConsumeIndex != LPC_EMAC->RxProduceIndex) {
            lpc17_emac->lpc_enetif_input();
        }
    }
}

/** \brief  Transmit cleanup task
 *
 * This task is called when a transmit interrupt occurs and
 * reclaims the memory buffer and descriptor used for the packet once
 * the packet has been transferred.
 *
 *  \param[in] pvParameters Not used yet
 */
void LPC17_EMAC::packet_tx(void* pvParameters)
{
    LPC17_EMAC *lpc17_emac = static_cast<LPC17_EMAC *>(pvParameters);
    int32_t idx;

    while (1) {
        /* Wait for transmit cleanup task to wakeup */
        lpc17_emac->TxCleanSem.acquire();

        /* Error handling for TX underruns. This should never happen unless
           something is holding the bus or the clocks are going too slow. It
            can probably be safely removed. */
        if (LPC_EMAC->IntStatus & EMAC_INT_TX_UNDERRUN) {
            lpc17_emac->TXLockMutex.lock();

            /* Reset the TX side */
            LPC_EMAC->MAC1 |= EMAC_MAC1_RES_TX;
            LPC_EMAC->IntClear = EMAC_INT_TX_UNDERRUN;

            /* De-allocate all queued TX buffers */
            for (idx = 0; idx < LPC_NUM_BUFF_TXDESCS; idx++) {
                if (lpc_enetdata.txb[idx] != NULL) {
                    lpc17_emac->memory_manager->free(lpc_enetdata.txb[idx]);
                    lpc_enetdata.txb[idx] = NULL;
                }
            }

            lpc17_emac->TXLockMutex.unlock();

            /* Start TX side again */
            lpc17_emac->lpc_tx_setup();
        } else {
            /* Free TX buffers that are done sending */
            lpc17_emac->lpc_tx_reclaim();
        }
    }
}

/** \brief  Low level init of the MAC and PHY.
 *
 */
bool LPC17_EMAC::low_level_init()
{
    bool err = true;

    /* Enable MII clocking */
    LPC_SC->PCONP |= CLKPWR_PCONP_PCENET;

#if defined(TARGET_LPC17XX)
    LPC_PINCON->PINSEL2 = 0x50150105;                  /* Enable P1 Ethernet Pins. */
    LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & ~0x0000000F) | 0x00000005;
#elif defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)
  LPC_IOCON->P1_0  &= ~0x07;    /*  ENET I/O config */
  LPC_IOCON->P1_0  |= 0x01;     /* ENET_TXD0 */
  LPC_IOCON->P1_1  &= ~0x07;
  LPC_IOCON->P1_1  |= 0x01;     /* ENET_TXD1 */
  LPC_IOCON->P1_4  &= ~0x07;
  LPC_IOCON->P1_4  |= 0x01;     /* ENET_TXEN */
  LPC_IOCON->P1_8  &= ~0x07;
  LPC_IOCON->P1_8  |= 0x01;     /* ENET_CRS */
  LPC_IOCON->P1_9  &= ~0x07;
  LPC_IOCON->P1_9  |= 0x01;     /* ENET_RXD0 */
  LPC_IOCON->P1_10 &= ~0x07;
  LPC_IOCON->P1_10 |= 0x01;     /* ENET_RXD1 */
  LPC_IOCON->P1_14 &= ~0x07;
  LPC_IOCON->P1_14 |= 0x01;     /* ENET_RX_ER */
  LPC_IOCON->P1_15 &= ~0x07;
  LPC_IOCON->P1_15 |= 0x01;     /* ENET_REF_CLK */
  LPC_IOCON->P1_16 &= ~0x07;    /* ENET/PHY I/O config */
  LPC_IOCON->P1_16 |= 0x01;     /* ENET_MDC */
  LPC_IOCON->P1_17 &= ~0x07;
  LPC_IOCON->P1_17 |= 0x01;     /* ENET_MDIO */
#endif

    /* Reset all MAC logic */
    LPC_EMAC->MAC1 = EMAC_MAC1_RES_TX | EMAC_MAC1_RES_MCS_TX |
        EMAC_MAC1_RES_RX | EMAC_MAC1_RES_MCS_RX | EMAC_MAC1_SIM_RES |
        EMAC_MAC1_SOFT_RES;
    LPC_EMAC->Command = EMAC_CR_REG_RES | EMAC_CR_TX_RES | EMAC_CR_RX_RES |
        EMAC_CR_PASS_RUNT_FRM;
    osDelay(10);

    /* Initial MAC initialization */
    LPC_EMAC->MAC1 = EMAC_MAC1_PASS_ALL;
    LPC_EMAC->MAC2 = EMAC_MAC2_CRC_EN | EMAC_MAC2_PAD_EN |
        EMAC_MAC2_VLAN_PAD_EN;
    LPC_EMAC->MAXF = EMAC_ETH_MAX_FLEN;

    /* Set RMII management clock rate to lowest speed */
    LPC_EMAC->MCFG = EMAC_MCFG_CLK_SEL(11) | EMAC_MCFG_RES_MII;
    LPC_EMAC->MCFG &= ~EMAC_MCFG_RES_MII;

    /* Maximum number of retries, 0x37 collision window, gap */
    LPC_EMAC->CLRT = EMAC_CLRT_DEF;
    LPC_EMAC->IPGR = EMAC_IPGR_P1_DEF | EMAC_IPGR_P2_DEF;

#if LPC_EMAC_RMII
    /* RMII setup */
    LPC_EMAC->Command = EMAC_CR_PASS_RUNT_FRM | EMAC_CR_RMII;
#else
    /* MII setup */
    LPC_EMAC->CR = EMAC_CR_PASS_RUNT_FRM;
#endif

    /* Initialize the PHY and reset */
    err = lpc_phy_init(this, LPC_EMAC_RMII);
    if (err == false) {
         return false;
    }

    /* Save station address */
    LPC_EMAC->SA2 = (uint32_t) hwaddr[0] |
        (((uint32_t) hwaddr[1]) << 8);
    LPC_EMAC->SA1 = (uint32_t) hwaddr[2] |
        (((uint32_t) hwaddr[3]) << 8);
    LPC_EMAC->SA0 = (uint32_t) hwaddr[4] |
        (((uint32_t) hwaddr[5]) << 8);

    /* Setup transmit and receive descriptors */
    if (lpc_tx_setup() != true) {
        return false;
    }
    if (lpc_rx_setup() != true) {
        return false;
    }

    /* Enable packet reception */
    LPC_EMAC->RxFilterCtrl = EMAC_RFC_PERFECT_EN | EMAC_RFC_BCAST_EN | EMAC_RFC_MCAST_EN;

    /* Clear and enable rx/tx interrupts */
    LPC_EMAC->IntClear = 0xFFFF;
    LPC_EMAC->IntEnable = RXINTGROUP | TXINTGROUP;

    /* Enable RX and TX */
    LPC_EMAC->Command |= EMAC_CR_RX_EN | EMAC_CR_TX_EN;
    LPC_EMAC->MAC1 |= EMAC_MAC1_REC_EN;

    return err;
}

/* This function provides a method for the PHY to setup the EMAC
   for the PHY negotiated duplex mode */
void lpc_emac_set_duplex(int full_duplex)
{
    if (full_duplex) {
        LPC_EMAC->MAC2    |= EMAC_MAC2_FULL_DUP;
        LPC_EMAC->Command |= EMAC_CR_FULL_DUP;
        LPC_EMAC->IPGT     = EMAC_IPGT_FULL_DUP;
    } else {
        LPC_EMAC->MAC2    &= ~EMAC_MAC2_FULL_DUP;
        LPC_EMAC->Command &= ~EMAC_CR_FULL_DUP;
        LPC_EMAC->IPGT = EMAC_IPGT_HALF_DUP;
    }
}

/* This function provides a method for the PHY to setup the EMAC
   for the PHY negotiated bit rate */
void lpc_emac_set_speed(int mbs_100)
{
    if (mbs_100) {
        LPC_EMAC->SUPP = EMAC_SUPP_SPEED;
    } else {
        LPC_EMAC->SUPP = 0;
    }
}

/* periodic PHY status update */
void LPC17_EMAC::phy_update(void *nif)
{
    LPC17_EMAC *lpc17_emac = static_cast<LPC17_EMAC *>(nif);

    while (true) {
        lpc_phy_sts_sm(lpc17_emac);
        osDelay(250);
    }
}

void LPC17_EMAC::update_link_status(bool up)
{
    emac_link_state_cb(up);
}

void LPC17_EMAC::eth_arch_enable_interrupts(void) {
    NVIC_SetVector(ENET_IRQn, (uint32_t)LPC17xxEthernetHandler);
    NVIC_SetPriority(ENET_IRQn, ((0x01 << 3) | 0x01));
    NVIC_EnableIRQ(ENET_IRQn);
}

void LPC17_EMAC::eth_arch_disable_interrupts(void) {
    NVIC_DisableIRQ(ENET_IRQn);
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
bool LPC17_EMAC::power_up()
{
    bool err = low_level_init();
    if (err != true) {
        return false;
    }

    RxThread = create_new_thread("lpc17_emac_rx_thread", LPC17_EMAC::packet_rx, this, rx_thread_stack, DEFAULT_THREAD_STACKSIZE, RX_PRIORITY, &RxThread_cb);
    TxCleanThread = create_new_thread("lpc17_emac_txclean_thread", LPC17_EMAC::packet_tx, this, tx_clean_thread_stack, DEFAULT_THREAD_STACKSIZE, TX_PRIORITY, &TxCleanThread_cb);
    PhyThread = create_new_thread("lpc17_emac_phy_thread", LPC17_EMAC::phy_update, this, phy_thread_stack, DEFAULT_THREAD_STACKSIZE, TX_PRIORITY, &PhyThread_cb);

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    osDelay(10);

    eth_arch_enable_interrupts();

    return true;
}

uint32_t LPC17_EMAC::get_mtu_size() const
{
    return 1500;
}

uint32_t LPC17_EMAC::get_align_preference() const
{
    return 0;
}

void LPC17_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, LPC17_ETH_IF_NAME, (size < sizeof(LPC17_ETH_IF_NAME)) ? size : sizeof(LPC17_ETH_IF_NAME));
}

uint8_t LPC17_EMAC::get_hwaddr_size() const
{
    return LPC17_ETH_HWADDR_SIZE;
}

bool LPC17_EMAC::get_hwaddr(uint8_t *addr) const
{
    return false;
}

void LPC17_EMAC::set_hwaddr(const uint8_t *addr)
{
    memcpy(hwaddr, addr, LPC17_ETH_HWADDR_SIZE);

    /* Save station address */
    LPC_EMAC->SA2 = (uint32_t) hwaddr[0] |
        (((uint32_t) hwaddr[1]) << 8);
    LPC_EMAC->SA1 = (uint32_t) hwaddr[2] |
        (((uint32_t) hwaddr[3]) << 8);
    LPC_EMAC->SA0 = (uint32_t) hwaddr[4] |
        (((uint32_t) hwaddr[5]) << 8);
}

void LPC17_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void LPC17_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void LPC17_EMAC::add_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void LPC17_EMAC::remove_multicast_group(const uint8_t *address)
{
    /* No-op at this stage */
}

void LPC17_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

void LPC17_EMAC::power_down()
{
    /* No-op at this stage */
}

LPC17_EMAC::LPC17_EMAC()
    : RxThread(),
      RxThread_cb(),
      TxCleanSem(),
      hwaddr(),
      TxCleanThread(),
      TxCleanThread_cb(),
      PhyThread(),
      PhyThread_cb(),
      TXLockMutex(),
      xTXDCountSem(LPC_NUM_BUFF_TXDESCS),
      emac_link_input_cb(0),
      emac_link_state_cb(0),
      memory_manager(0)
{
}

void LPC17_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

LPC17_EMAC &LPC17_EMAC::get_instance() {
    static LPC17_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance() {
    return LPC17_EMAC::get_instance();
}

/* --------------------------------- End Of File ------------------------------ */
