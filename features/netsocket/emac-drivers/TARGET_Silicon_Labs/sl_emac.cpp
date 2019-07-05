/***************************************************************************//**
 * @file sl_emac.cpp
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2018 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#include "device.h"

#if defined(ETH_PRESENT)
#include "sl_emac_config.h"
#include "sl_emac.h"
#include "sl_eth_hw.h"
#include "sl_eth_phy.h"
#include "mbed_power_mgmt.h"

#include <stdlib.h>

#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP "SEth"

#define FLAG_TX     1
#define FLAG_RX     2
#define FLAG_POLL   4

// -----------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------
bool SL_EMAC::power_up()
{
    // If the worker thread doesn't exist, launch it
    if(thread == 0) {
        /* Worker thread */
        osThreadAttr_t attr = {0};
        attr.name = "sl_emac_thread";
        attr.stack_mem  = malloc(SL_ETH_THREAD_STACKSIZE);
        attr.cb_mem  = &thread_cb;
        attr.stack_size = SL_ETH_THREAD_STACKSIZE;
        attr.cb_size = sizeof(mbed_rtos_storage_thread_t);
        attr.priority = SL_ETH_THREAD_PRIORITY;
        thread = osThreadNew(&this->eth_thread, this, &attr);
    }

    // Can't turn off HF clock as long as Ethernet is active
    sleep_manager_lock_deep_sleep();

    // Bring up data structures
    data_init();

    // Bring up clocks
    sl_eth_hw_init();

    // Point to RX queue
    ETH->RXQPTR = (uint32_t)rx_bds;
    ETH->DMACFG = (ETH->DMACFG & ~_ETH_DMACFG_RXBUFSIZE_MASK)
                  | ((SL_ETH_RX_BUF_SIZE/64) << _ETH_DMACFG_RXBUFSIZE_SHIFT);

    // Set up MAC address
    uint8_t addr[6];
    get_hwaddr(addr);
    set_hwaddr(addr);

    ETH->IFCR |= _ETH_IFCR_MASK;
    ETH->RXSTATUS = 0xFFFFFFFF;
    ETH->TXSTATUS = 0xFFFFFFFF;
    ETH->IENS = ETH_IENS_RXCMPLT |
                ETH_IENS_RXUSEDBITREAD |
                ETH_IENS_TXCMPLT |
                ETH_IENS_TXUNDERRUN |
                ETH_IENS_RTRYLMTORLATECOL |
                ETH_IENS_TXUSEDBITREAD |
                ETH_IENS_AMBAERR |
                ETH_IENS_MNGMNTDONE;

    ETH->NETWORKCFG |=  ETH_NETWORKCFG_FCSREMOVE |
                        ETH_NETWORKCFG_UNICASTHASHEN |
                        ETH_NETWORKCFG_MULTICASTHASHEN |
                        ETH_NETWORKCFG_RXCHKSUMOFFLOADEN;

    ETH->NETWORKCFG |=  ETH_NETWORKCFG_FULLDUPLEX |
                        ETH_NETWORKCFG_SPEED;

    ETH->DMACFG |=  _ETH_DMACFG_AMBABRSTLEN_MASK |
                    ETH_DMACFG_FRCDISCARDONERR |
                    ETH_DMACFG_TXPBUFTCPEN;

    ETH->DMACFG &= ~ETH_DMACFG_HDRDATASPLITEN;

    ETH->NETWORKCTRL |= ETH_NETWORKCTRL_ENBTX |
                        ETH_NETWORKCTRL_ENBRX |
                        ETH_NETWORKCTRL_MANPORTEN;

    phy_init();

    NVIC_EnableIRQ(ETH_IRQn);

    up = true;

    tr_debug("Link booted");

    osThreadFlagsSet(thread, FLAG_POLL);

    return true;
}

void SL_EMAC::power_down()
{
    up = false;

    tr_debug("Link coming down, waiting for TX to be done.");

    tx_sem.acquire();

    NVIC_DisableIRQ(ETH_IRQn);

    sl_eth_hw_deinit();

    data_deinit();

    /* link is down */
    if(connected && emac_link_state_cb) {
        emac_link_state_cb(false);
    }
    connected = false;

    tr_debug("Link down");

    // Ethernet went down, HF clock is no longer required here
    sleep_manager_unlock_deep_sleep();
}

void SL_EMAC::data_init(void)
{
    size_t i;

    /* Allocate a full-frame buffer for each RX BD and set up said BD */
    for(i = 0; i < SL_ETH_NUM_RX_BD; i++) {
        rx_bufs[i] = memory_manager->alloc_heap(SL_ETH_RX_BUF_SIZE, SL_ETH_ALIGN);
        rx_bds[i].addr = ((uint32_t)memory_manager->get_ptr(rx_bufs[i])) & ~0x3;
        if(i == SL_ETH_NUM_RX_BD-1) {
            rx_bds[i].addr |= 0x2;
        }
        rx_bds[i].status = 0;
    }

    /* Set up TX BDs */
    tx_buf = (emac_mem_buf_t*)NULL;
    for(i = 0; i < SL_ETH_NUM_TX_BD; i++) {
        tx_bds[i].addr = 0;
        tx_bds[i].status = 0;
        if(i == SL_ETH_NUM_TX_BD-1) {
            tx_bds[i].status |= (0x1 << 30);
        }
    }

    /* Start RX reception at index 0 */
    rx_idx = 0;
}

void SL_EMAC::data_deinit(void)
{
    for(size_t i = 0; i < SL_ETH_NUM_RX_BD; i++) {
        memory_manager->free(rx_bufs[i]);
    }
}

// -----------------------------------------------------------------------------
// IRQ & IRQ de-escalation logic
// -----------------------------------------------------------------------------
/* IRQ handler for ethernet interrupts */
void ETH_IRQHandler(void)
{
    uint32_t int_clr = 0;
    uint32_t int_status = ETH->IFCR;
    uint32_t txdone_mask = ETH_IFCR_TXCMPLT |
                           ETH_IFCR_TXUNDERRUN |
                           ETH_IFCR_RTRYLMTORLATECOL |
                           ETH_IFCR_TXUSEDBITREAD |
                           ETH_IFCR_AMBAERR;
    uint32_t rxdone_mask = ETH_IFCR_RXCMPLT |
                           ETH_IFCR_RXUSEDBITREAD;


    SL_EMAC &emac = SL_EMAC::get_instance();

    if(int_status & rxdone_mask) {
        if (emac.thread) {
            osThreadFlagsSet(emac.thread, FLAG_RX);
        }
        int_clr |= rxdone_mask;
    }

    if(int_status & txdone_mask) {
        if (emac.thread) {
            osThreadFlagsSet(emac.thread, FLAG_TX);
        }
        int_clr |= txdone_mask;
    }

    int_clr |= ETH_IFCR_MNGMNTDONE;
    ETH->IFCR = int_clr;
}

void SL_EMAC::eth_thread(void* instance)
{
    struct SL_EMAC *emac = static_cast<SL_EMAC *>(instance);

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX|FLAG_TX|FLAG_POLL, osFlagsWaitAny, SL_ETH_LINK_POLL_PERIOD_MS);
        if ((flags == osFlagsErrorTimeout) && emac->up) {
            // Rather than calling strictly every period, we call when idle
            // for that period - hopefully good enough. We run this task
            // from lwIP's thread rather than our RX/TX thread, as PHY reads can
            // be slow, and we don't want them to interfere with data pumping.
            // This is analogous to the way the PHY polling works in the Nanostack
            // version of the driver
            emac->link_state_poll();
            continue;
        }

        if((flags & FLAG_POLL)) {
            emac->link_state_poll();
            continue;
        }

        MBED_ASSERT((flags & osFlagsError) == 0);

        /* Packet received */
        if ((flags & FLAG_RX) && emac->up) {
            /* Find packets in the RX BD chain which have been fully received. Feed the
             * corresponding buffer upstream, and attach a new buffer to the BD. */
            while(emac->rx_bds[emac->rx_idx].addr & 0x1) {
                /* DMA has relinquished control over this packet */
                emac_mem_buf_t* buf = emac->rx_bufs[emac->rx_idx];
                emac->memory_manager->set_len(buf, emac->rx_bds[emac->rx_idx].status & 0x1FFF);

                tr_debug("Received packet of size %d", emac->memory_manager->get_len(buf));
                /* Attempt to queue new buffer */
                emac_mem_buf_t* temp_rxbuf = emac->memory_manager->alloc_heap(SL_ETH_RX_BUF_SIZE, 4);
                if (NULL == temp_rxbuf) {
                    /* out of memory, drop packet */
                    tr_warn("Packet index %d dropped for OOM",
                             emac->rx_idx);
                    emac->rx_bds[emac->rx_idx].addr &= ~0x1;
                } else {
                    emac->rx_bufs[emac->rx_idx] = temp_rxbuf;

                    emac->rx_bds[emac->rx_idx].status = 0;
                    if(emac->rx_bds[emac->rx_idx].addr & 0x2) {
                        emac->rx_bds[emac->rx_idx].addr = (uint32_t)emac->memory_manager->get_ptr(temp_rxbuf) | 0x2;
                    } else {
                        emac->rx_bds[emac->rx_idx].addr = (uint32_t)emac->memory_manager->get_ptr(temp_rxbuf);
                    }

                    /* pass all packets to ethernet_input, which decides what packets it supports */
                    if(emac->emac_link_input_cb) {
                        emac->emac_link_input_cb(buf);
                    }
                }
                emac->rx_idx = (emac->rx_idx + 1) % SL_ETH_NUM_RX_BD;
            }
        }

        /* Packet transmission done */
        if (flags & FLAG_TX) {
            /* Free the buffer */
            if(emac->tx_buf != NULL) {
                emac->memory_manager->free(emac->tx_buf);
                emac->tx_buf = NULL;
            }

            /* Signal TX thread(s) we're ready to start TX'ing */
            emac->tx_sem.release();
        }
    }
}

// -----------------------------------------------------------------------------
// PHY manipulation
// -----------------------------------------------------------------------------
void SL_EMAC::phy_init(void)
{
    uint8_t phy_addr = 0;
    uint16_t regid1, regid2;
    /* PHY has been enabled by hw_init. Figure out address first */
    for(; phy_addr < 32; phy_addr++) {
        read_phy(PHY_PHYSID1, &regid1);
        read_phy(PHY_PHYSID2, &regid2);

        if (((regid1 == 0x0000u) && (regid2 == 0x0000u)) ||
            ((regid1 == 0x3FFFu) && (regid2 == 0x0000u)) ||
            ((regid1 == 0x0000u) && (regid2 == 0x3FFFu)) ||
            ((regid1 == 0x3FFFu) && (regid2 == 0x3FFFu)) ||
            ((regid1 == 0xFFFFu) && (regid2 == 0x0000u)) ||
            ((regid1 == 0x0000u) && (regid2 == 0xFFFFu)) ||
            ((regid1 == 0x3FFFu) && (regid2 == 0xFFFFu)) ||
            ((regid1 == 0xFFFFu) && (regid2 == 0xFFFFu))) {
            continue;
        } else {
            break;
        }
    }

    if(phy_addr >= 32) {
        /* no PHY found */
        this->phy_addr = 0xFF;
        return;
    } else {
        this->phy_addr = phy_addr;
    }

    /* Reset PHY */
    write_phy(PHY_BMCR, BMCR_RESET);
    read_phy(PHY_BMCR, &regid1);

    /* wait for PHY to come out of reset */
    while(regid1 & BMCR_RESET) {
        osDelay(2);
        read_phy(PHY_BMCR, &regid1);
    }

    /* Enable PHY */
    if(regid1 & BMCR_PDOWN) {
        write_phy(PHY_BMCR, regid1 & (~BMCR_PDOWN));
    }

    /* Set up auto-negotiation */
    read_phy(PHY_BMCR, &regid1);
    regid1 |= BMCR_ANENABLE | BMCR_ANRESTART;
    write_phy(PHY_BMCR, regid1);
}

void SL_EMAC::write_phy(uint8_t reg_addr, uint16_t data)
{
    unsigned int timeout;

    ETH->PHYMNGMNT = ETH_PHYMNGMNT_WRITE0_DEFAULT
                   | ETH_PHYMNGMNT_WRITE1
                   | (0x01 << _ETH_PHYMNGMNT_OPERATION_SHIFT)
                   | ((phy_addr << _ETH_PHYMNGMNT_PHYADDR_SHIFT)
                      & _ETH_PHYMNGMNT_PHYADDR_MASK)
                   | ((reg_addr << _ETH_PHYMNGMNT_REGADDR_SHIFT)
                      & _ETH_PHYMNGMNT_REGADDR_MASK)
                   | (0x2 << _ETH_PHYMNGMNT_WRITE10_SHIFT)
                   | (data & _ETH_PHYMNGMNT_PHYRWDATA_MASK);

    for(timeout = 0; timeout < 10000u; timeout++) {
        if(ETH->NETWORKSTATUS & ETH_NETWORKSTATUS_MANDONE) {
            break;
        }
    }
}

void SL_EMAC::read_phy(uint8_t reg_addr, uint16_t *data)
{
    unsigned int timeout;

    ETH->PHYMNGMNT = ETH_PHYMNGMNT_WRITE0_DEFAULT
                   | ETH_PHYMNGMNT_WRITE1
                   | (0x02 << _ETH_PHYMNGMNT_OPERATION_SHIFT)
                   | ((phy_addr << _ETH_PHYMNGMNT_PHYADDR_SHIFT)
                      & _ETH_PHYMNGMNT_PHYADDR_MASK)
                   | ((reg_addr << _ETH_PHYMNGMNT_REGADDR_SHIFT)
                      & _ETH_PHYMNGMNT_REGADDR_MASK)
                   | (0x2 << _ETH_PHYMNGMNT_WRITE10_SHIFT);

    for(timeout = 0; timeout < 10000u; timeout++) {
        if(ETH->NETWORKSTATUS & ETH_NETWORKSTATUS_MANDONE) {
        break;
        }
    }

    *data = ETH->PHYMNGMNT & _ETH_PHYMNGMNT_PHYRWDATA_MASK;
}

void SL_EMAC::link_state_poll(void)
{
	uint16_t phy_val, link_val;

    /* read BMSR twice, since it needs to latch */
    read_phy(PHY_BMSR, &phy_val);
    read_phy(PHY_BMSR, &phy_val);

    if((phy_val & BMSR_LSTATUS) == 0) {
        /* link is down */
        tr_debug("link down");
        if(connected && emac_link_state_cb) {
            emac_link_state_cb(false);
            /* TODO: Reset all buffers here */
            /* For now, this is not a problem. In-transit buffers will
             * still be sent the next time the link comes up, so the
             * only impact is that we'd be sending stale packets. */
        }
        connected = false;
        return;
    }

    /* link is up, get speed and duplex status */
    read_phy(PHY_ANAR, &phy_val);
    read_phy(PHY_ANLPAR, &link_val);

    link_val &= (ANLPAR_100BASE4 |
                 ANLPAR_100FULL  |
                 ANLPAR_100HALF  |
                 ANLPAR_10FULL   |
                 ANLPAR_10HALF);

    phy_val &= link_val;

    uint32_t old_link_state = ETH->NETWORKCFG & 0x3;

    if (phy_val >= ANLPAR_100FULL) {
        /* 100 mbit full duplex */
        if (old_link_state != 0x3 || !connected) {
            tr_debug("renegotiated to 100 full");
            ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x3;
        }
    } else if (phy_val >= ANLPAR_100HALF) {
        /* 100 mbit half duplex */
        if (old_link_state != 0x1 || !connected) {
            tr_debug("renegotiated to 100 half");
            ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x1;
        }
    } else if (phy_val >= ANLPAR_10FULL) {
        /* 10 mbit full duplex */
        if (old_link_state != 0x2 || !connected) {
            tr_debug("renegotiated to 10 full");
            ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x2;
        }
    } else {
        /* 10 mbit half duplex */
        if (old_link_state != 0x0 || !connected) {
            tr_debug("renegotiated to 10 half");
            ETH->NETWORKCFG = (ETH->NETWORKCFG & ~0x3) | 0x0;
        }
    }

    if(!connected && emac_link_state_cb) {
        tr_debug("link up");
        emac_link_state_cb(true);
    }
    connected = true;
}

// -----------------------------------------------------------------------------
// Receive
// -----------------------------------------------------------------------------

/* Handled inside processing thread */

// -----------------------------------------------------------------------------
// Send
// -----------------------------------------------------------------------------
bool SL_EMAC::link_out(emac_mem_buf_t *buf)
{
    size_t num_bufs = 1, i;
    emac_mem_buf_t * next = buf;

    /* If the link is down (or going down), don't even attempt sending anything */
    if(!up) {
        tr_debug("Trying to send a packet while link is down");
        memory_manager->free(buf);
        return false;
    }

    /* Figure out how many buffers the buffer consists of */
    while((next = memory_manager->get_next(next)) != (emac_mem_buf_t*)NULL) {
        num_bufs++;
    }

    if(num_bufs >= SL_ETH_NUM_TX_BD) {
        /* We've been passed more chained buffers than we can handle */
        tr_err("More TX buffers passed than provisioned!");
        memory_manager->free(buf);
        return false;
    }

    /* Wait for previous packet to finish transmitting */
    if (!tx_sem.try_acquire_for(100)) {
        tr_warn("TX process didn't complete within 100ms");
        memory_manager->free(buf);
        return false;
    }

    tr_debug("Sending packet of %d bytes over %d buffers", memory_manager->get_total_len(buf), num_bufs);

    /* Set up TX descriptors with buffer, keep track of buffer reference */
    tx_buf = buf;
    for(i = 0; i < num_bufs; i++) {
        uint32_t statusword = memory_manager->get_len(buf) & 0x3FFF;

        if(i == (SL_ETH_NUM_TX_BD-1)) {
            /* Mark as last BD in list */
            statusword |= (0x1 << 30);
        }
        if(i == num_bufs - 1) {
            /* Mark as last BD for this frame */
            statusword |= (0x1 << 15);
        }

        tx_bds[i].addr = (uint32_t)memory_manager->get_ptr(buf);
        tx_bds[i].status = statusword;

        buf = memory_manager->get_next(buf);
    }

    /* (Re-)Kick off ETH TX */
    ETH->TXQPTR = (uint32_t)tx_bds;
    ETH->NETWORKCTRL |= ETH_NETWORKCTRL_TXSTRT;
    return true;
}

// -----------------------------------------------------------------------------
// Multicast manipulation
// -----------------------------------------------------------------------------
static uint8_t sl_eth_calc_hash(const uint8_t* const mac)
{
    return (uint8_t)((    (mac[0]       & 0x3F)                         & 0x3F)
                     ^ ((((mac[0] >> 6) & 0x3) | ((mac[1] & 0xF) << 2)) & 0x3F)
                     ^ ((((mac[1] >> 4) & 0xF) | ((mac[2] & 0x3) << 4)) & 0x3F)
                     ^ ((((mac[2] >> 2) & 0x3F))                        & 0x3F)
                     ^   ((mac[3]       & 0x3F)                         & 0x3F)
                     ^ ((((mac[3] >> 6) & 0x3) | ((mac[4] & 0xF) << 2)) & 0x3F)
                     ^ ((((mac[4] >> 4) & 0xF) | ((mac[5] & 0x3) << 4)) & 0x3F)
                     ^ ((((mac[5] >> 2) & 0x3F))                        & 0x3F));
}

void SL_EMAC::add_multicast_group(const uint8_t *address)
{
	uint8_t bitnr;
    /* Calculate bit number for hash of this address */
    bitnr = sl_eth_calc_hash(address);
    /* Increment refcnt */
    if (mcast_hash_refcnt[bitnr] == 0) {
        if(bitnr > 31) {
            ETH->HASHTOP |= (0x1 << (bitnr - 32));
        } else {
            ETH->HASHBOTTOM |= (0x1 << bitnr);
        }
    }
    mcast_hash_refcnt[bitnr]++;
}

void SL_EMAC::remove_multicast_group(const uint8_t *address)
{
	uint8_t bitnr;
    /* Calculate bit number for hash of this address */
    bitnr = sl_eth_calc_hash(address);
    /* Decrement refcnt, remove bit if 0 */
    if(mcast_hash_refcnt[bitnr] == 1) {
        mcast_hash_refcnt[bitnr] = 0;
        if(bitnr > 31) {
            ETH->HASHTOP &= ~(0x1 << (bitnr - 32));
        } else {
            ETH->HASHBOTTOM &= ~(0x1 << bitnr);
        }
    } else {
        mcast_hash_refcnt[bitnr]--;
    }
}

void SL_EMAC::set_all_multicast(bool all)
{
	uint32_t bottom = 0, top = 0;

	if(all == true) {
		/* temporarily allow all packets to get processed */
        tr_debug("Accept all multicast packets");
		top = 0xFFFFFFFFUL;
		bottom = 0xFFFFFFFFUL;
	} else {
		/* Revert to what was in the refcount */
        tr_debug("Revert to multicast filtering");
		size_t i = 0;
		for(; i < 32; i++) {
			if(mcast_hash_refcnt[i] > 0) {
				bottom |= (1 << i);
			}
		}
		for(; i < 64; i++) {
			if(mcast_hash_refcnt[i-32] > 0) {
				top |= (1 << (i-32));
			}
		}
	}

	/* Commit to peripheral */
	ETH->HASHTOP = top;
	ETH->HASHBOTTOM = bottom;
}

// -----------------------------------------------------------------------------
// MAC manipulation
// -----------------------------------------------------------------------------
uint8_t SL_EMAC::get_hwaddr_size() const
{
	// Ethernet uses EUI48
	return 6;
}

bool SL_EMAC::get_hwaddr(uint8_t *addr) const
{
	if (DEVINFO->EUI48L != 0xFFFFFFFF) {
        addr[0] = DEVINFO->EUI48H >> 8;
        addr[1] = DEVINFO->EUI48H >> 0;
        addr[2] = DEVINFO->EUI48L >> 24;
        addr[3] = DEVINFO->EUI48L >> 16;
        addr[4] = DEVINFO->EUI48L >> 8;
        addr[5] = DEVINFO->EUI48L >> 0;
    } else {
        addr[0] = DEVINFO->UNIQUEH >> 24;
        addr[1] = DEVINFO->UNIQUEH >> 16;
        addr[2] = DEVINFO->UNIQUEH >> 8;
        addr[3] = DEVINFO->UNIQUEL >> 16;
        addr[4] = DEVINFO->UNIQUEL >> 8;
        addr[5] = DEVINFO->UNIQUEL >> 0;
    }
    return true;
}

void SL_EMAC::set_hwaddr(const uint8_t *addr)
{
    tr_debug("Setting MAC address %02x:%02x:%02x:%02x:%02x:%02x",
             addr[0],
             addr[1],
             addr[2],
             addr[3],
             addr[4],
             addr[5]);
	ETH->SPECADDR1BOTTOM = ((uint32_t)addr[0] << (0)) |
                           ((uint32_t)addr[1] << (8)) |
                           ((uint32_t)addr[2] << (16))|
                           ((uint32_t)addr[3] << (24));

    ETH->SPECADDR1TOP = ((uint32_t)addr[4] << (0)) |
                        ((uint32_t)addr[5] << (8));
}

// -----------------------------------------------------------------------------
// Boilerplate
// -----------------------------------------------------------------------------
SL_EMAC::SL_EMAC()
    : thread(0),
    tx_sem(1, 1),
    phy_addr(0xFF),
    rx_idx(0),
    mcast_hash_refcnt(),
    emac_link_input_cb(NULL),
    emac_link_state_cb(NULL),
    memory_manager(NULL),
    connected(false),
    up(false)
{

}

uint32_t SL_EMAC::get_mtu_size() const
{
	return SL_ETH_MTU;
}

uint32_t SL_EMAC::get_align_preference() const
{
	return SL_ETH_ALIGN;
}

void SL_EMAC::get_ifname(char *name, uint8_t size) const
{
	memcpy(name, SL_ETH_IF_NAME, (size < sizeof(SL_ETH_IF_NAME)) ? size : sizeof(SL_ETH_IF_NAME));
}

void SL_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
  emac_link_input_cb = input_cb;
}

void SL_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
  emac_link_state_cb = state_cb;
}

void SL_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

SL_EMAC &SL_EMAC::get_instance() {
    static SL_EMAC emac;
    return emac;
}

MBED_WEAK EMAC &EMAC::get_default_instance() {
    return SL_EMAC::get_instance();
}

#endif //ETH_PRESENT
