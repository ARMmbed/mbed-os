/*
 * Copyright (c) 2018 Nuvoton Technology Corp. 
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * Description:   NuMaker EMAC
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmsis_os.h"

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "netsocket/nsapi_types.h"
#include "mbed_shared_queues.h"

//#define NU_TRACE

#include "numaker_emac_config.h"
#include "numaker_emac.h"
#include "numaker_eth_hal.h"

/********************************************************************************
 * 
 ********************************************************************************/
#define NU_BUFF_ALIGNMENT       4
#define PHY_LINKED_STATE        1
#define PHY_UNLINKED_STATE      0
#define PACKET_BUFFER_SIZE      NU_ETH_MAX_FLEN

extern "C" void numaker_eth_rx_next(void);
/* \brief Flags for worker thread */
#define FLAG_TX  1
#define FLAG_RX  2

/** \brief  Driver thread priority */
#define THREAD_PRIORITY (osPriorityNormal)

#define PHY_TASK_PERIOD_MS      200

NUMAKER_EMAC::NUMAKER_EMAC() : thread(0), hwaddr()
{
}

static osThreadId_t create_new_thread(const char *threadName, void (*thread)(void *arg), void *arg, int stacksize, osPriority_t priority, os_thread_t *thread_cb)
{
    osThreadAttr_t attr = {0};
    attr.name = threadName;
    attr.stack_mem  = malloc(stacksize);
    attr.cb_mem  = thread_cb;
    attr.stack_size = stacksize;
    attr.cb_size = sizeof(os_thread_t);
    attr.priority = priority;
    return osThreadNew(thread, arg, &attr);
}


/** \brief Ethernet receive interrupt handler
 *
 *  This function handles the receive interrupt.
 */
void NUMAKER_EMAC::rx_isr()
{
    if (thread) {
        osThreadFlagsSet(thread, FLAG_RX);
    }
}

void NUMAKER_EMAC::tx_isr()
{
  /* No-op at this stage */    
}    

void NUMAKER_EMAC::ethernet_callback(char event, void *param)
{
    NUMAKER_EMAC *enet = static_cast<NUMAKER_EMAC *>(param);
    switch (event)
    {
      case 'R': //For RX event
        enet->rx_isr();
        break;
      case 'T': //For TX event
        enet->tx_isr();
        break;
      default:
        break;
    }
}


/**
 * In this function, the hardware should be initialized.
 */
bool NUMAKER_EMAC::low_level_init_successful()
{
    /* Init ETH */

    mbed_mac_address((char *)hwaddr);
    printf("mac address %02x-%02x-%02x-%02x-%02x-%02x \r\n", hwaddr[0], hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);
    /* Enable clock & set EMAC configuration         */
    /* Enable MAC and DMA transmission and reception */
    numaker_eth_init(hwaddr);
    
    numaker_eth_set_cb(&NUMAKER_EMAC::ethernet_callback, this);
    
    return true;
}


int NUMAKER_EMAC::low_level_input(emac_mem_buf_t **buf)
{
    uint16_t len = 0;
    uint8_t *buffer;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    emac_mem_buf_t *q;
    uint32_t payloadoffset = 0;

    /* get received frame */
    if ( numaker_eth_get_rx_buf(&len, &buffer) != 0) {
        return -1;
    }
    byteslefttocopy = len;

    if (len > 0) {
        /* Allocate a memory buffer chain from buffer pool */
        *buf = memory_manager->alloc_pool(len, 0);
    }
    NU_DEBUGF(("%s... length=%d, buf=0x%x\r\n", __FUNCTION__, len, *buf));
    if (*buf != NULL) {
        bufferoffset = 0;
        for (q = *buf; q != NULL; q = memory_manager->get_next(q)) {
            byteslefttocopy = memory_manager->get_len(q);
            payloadoffset = 0;
            NU_DEBUGF(("offset=[%d], bytes-to-copy[%d]\r\n",bufferoffset,byteslefttocopy));
            /* Copy data in pbuf */
            memcpy(static_cast<uint8_t *>(memory_manager->get_ptr(q)) + payloadoffset, static_cast<uint8_t *>(buffer) + bufferoffset, byteslefttocopy);
            
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }

    return 0;
}


/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */

void NUMAKER_EMAC::thread_function(void* pvParameters)
{
    static struct NUMAKER_EMAC *nu_enet = static_cast<NUMAKER_EMAC *>(pvParameters);

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX, osFlagsWaitAny, osWaitForever);

        if (flags & FLAG_RX) {
            nu_enet->packet_rx();
        }
    }
}

/** \brief  Packet reception task
 *
 * This task is called when a packet is received. It will
 * pass the packet to the LWIP core.
 */
void NUMAKER_EMAC::packet_rx()
{
  /* move received packet into a new buf */
  while (1) {
      emac_mem_buf_t *p = NULL;
      if (low_level_input(&p) < 0) {
          break;
      }
      if (p) {
          NU_DEBUGF(("%s ... p=0x%x\r\n",__FUNCTION__,p));
          emac_link_input_cb(p);
      }
      numaker_eth_rx_next();
  }
  numaker_eth_trigger_rx();
  
}


/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the buf that is passed to the function. This buf
 * might be chained.
 *
 * @param buf the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return true if the packet could be sent
 *         false value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
bool NUMAKER_EMAC::link_out(emac_mem_buf_t *buf)
{
    bool result;
    emac_mem_buf_t *q;
    uint8_t *buffer = numaker_eth_get_tx_buf();
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;

    /* Get exclusive access */
    TXLockMutex.lock();
    NU_DEBUGF(("%s ... buffer=0x%x\r\n",__FUNCTION__, buffer));
    /* copy frame from buf to driver buffers */
    for (q = buf; q != NULL; q = memory_manager->get_next(q)) {

        /* Get bytes in current lwIP buffer */
        byteslefttocopy = memory_manager->get_len(q);
        payloadoffset = 0;
        NU_DEBUGF(("offset=%d, bytes-to-copy=%d\r\n",bufferoffset, byteslefttocopy));
        /* Check if the length of data to copy is bigger than Tx buffer size*/
        while ((byteslefttocopy + bufferoffset) > PACKET_BUFFER_SIZE) {
            /* Copy data to Tx buffer*/
            memcpy(static_cast<uint8_t *>(buffer) + bufferoffset, static_cast<uint8_t *>(memory_manager->get_ptr(q)) + payloadoffset, (PACKET_BUFFER_SIZE - bufferoffset));

            /* Point to next descriptor */
            numaker_eth_trigger_tx(PACKET_BUFFER_SIZE, NULL);
            buffer = numaker_eth_get_tx_buf();
            if( buffer == NULL ) goto error;
            
            byteslefttocopy = byteslefttocopy - (PACKET_BUFFER_SIZE - bufferoffset);
            payloadoffset = payloadoffset + (PACKET_BUFFER_SIZE - bufferoffset);
            framelength = framelength + (PACKET_BUFFER_SIZE - bufferoffset);
            bufferoffset = 0;
        }
        
        /* Copy the remaining bytes */
        memcpy(static_cast<uint8_t *>(buffer) + bufferoffset, static_cast<uint8_t *>(memory_manager->get_ptr(q)) + payloadoffset, byteslefttocopy);
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength = framelength + byteslefttocopy;
    }
    
    /* Prepare transmit descriptors to give to DMA */
    numaker_eth_trigger_tx(framelength, NULL);

    result = true;

error:

    memory_manager->free(buf);

    /* Restore access */
    TXLockMutex.unlock();

    return result;
}


void NUMAKER_EMAC::phy_task()
{
    
    // Get current status
    int state;
    state = numaker_eth_link_ok();

    
    if ((state & PHY_LINKED_STATE) && !(phy_state & PHY_LINKED_STATE)) {
        NU_DEBUGF(("Link Up\r\n"));
        if (emac_link_state_cb) emac_link_state_cb(true);
    } else if (!(state & PHY_LINKED_STATE) && (phy_state & PHY_LINKED_STATE)) {
        NU_DEBUGF(("Link Down\r\n"));
        if (emac_link_state_cb) emac_link_state_cb(false);
    }
    phy_state = state;        

}

bool NUMAKER_EMAC::power_up()
{
  /* Initialize the hardware */
  if (!low_level_init_successful())
    return false;

  /* Worker thread */
  thread = create_new_thread("numaker_emac_thread", &NUMAKER_EMAC::thread_function, this, THREAD_STACKSIZE, THREAD_PRIORITY, &thread_cb);

  /* PHY monitoring task */
  phy_state = PHY_UNLINKED_STATE;

  phy_task_handle = mbed::mbed_event_queue()->call_every(PHY_TASK_PERIOD_MS, mbed::callback(this, &NUMAKER_EMAC::phy_task));

  /* Allow the PHY task to detect the initial link state and set up the proper flags */
  osDelay(10);
  numaker_eth_enable_interrupts();
  return true;
}


uint32_t NUMAKER_EMAC::get_mtu_size() const
{
  return NU_ETH_MTU_SIZE;
}

uint32_t NUMAKER_EMAC::get_align_preference() const
{
  return NU_BUFF_ALIGNMENT;
}

void NUMAKER_EMAC::get_ifname(char *name, uint8_t size) const
{
  memcpy(name, NU_ETH_IF_NAME, (size < sizeof(NU_ETH_IF_NAME)) ? size : sizeof(NU_ETH_IF_NAME));
}

uint8_t NUMAKER_EMAC::get_hwaddr_size() const
{
    return NU_HWADDR_SIZE;
}

bool NUMAKER_EMAC::get_hwaddr(uint8_t *addr) const
{
    mbed_mac_address((char *)addr);
    return true;
}

void NUMAKER_EMAC::set_hwaddr(const uint8_t *addr)
{
  memcpy(hwaddr, addr, sizeof hwaddr);
  numaker_set_mac_addr(const_cast<uint8_t*>(addr));
}

void NUMAKER_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
  emac_link_input_cb = input_cb;
}

void NUMAKER_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
  emac_link_state_cb = state_cb;
}

void NUMAKER_EMAC::add_multicast_group(const uint8_t *addr)
{
  /* No-op at this stage */
}

void NUMAKER_EMAC::remove_multicast_group(const uint8_t *addr)
{
  /* No-op at this stage */
}

void NUMAKER_EMAC::set_all_multicast(bool all)
{
  /* No-op at this stage */
}


void NUMAKER_EMAC::power_down()
{
  /* No-op at this stage */
}

void NUMAKER_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}


NUMAKER_EMAC &NUMAKER_EMAC::get_instance() {
    static NUMAKER_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance() {
    return NUMAKER_EMAC::get_instance();
}




