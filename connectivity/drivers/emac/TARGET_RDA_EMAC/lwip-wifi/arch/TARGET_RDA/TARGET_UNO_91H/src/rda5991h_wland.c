/* Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
 */

/******************************************************************************
 * @file     rda5991h_wland.c
 * @brief    RDA5991H wlan driver for LWIP
 * @version: V1.0
 * @date:    25. July 2016
 *
 * @note
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "sys_arch.h"
#include "rda5991h_wland.h"
#include "cmsis.h"
#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)
#include "entropy_poll.h"
#else
#include "trng_api.h"
#endif
#include "gpio_api.h"
#include "gpio_irq_api.h"
#include "maclib_task.h"
#include "rda_sys_wrapper.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Global variables */
rda_enetdata_t r91h_wifidata;

int wland_dbg_dump = WLAND_DBG_DUMP;
int wland_dbg_level = WLAND_DBG_LEVEL;
int wpa_dbg_dump = WPA_DBG_DUMP;
int wpa_dbg_level = WPA_DBG_LEBEL;
int wlandlib_dbg_level = WLANDLIB_DBG_LEVEL;
int hut_dbg_dump = HUT_DBG_DUMP;

//#define CONFIG_DISABLE_ALL_INT
#define CRI_SEC_START_PRI_LEVEL 0xF8
#define CRI_SEC_END_PRI_LEVEL   0x00
static unsigned int g_critical_sec_counter = 0U;
#if defined(CONFIG_DISABLE_ALL_INT)
static unsigned int g_critical_ctxt_saved  = 0U;
#endif /* CONFIG_DISABLE_ALL_INT */
void *packet_rx_queue;

/* Function statements */
void r91h_wifiif_input(struct netif *netif, u8_t *data, u32_t len, int idx);
void mbed_mac_address(char *mac);

/** \brief  Driver transmit and receive thread priorities
 *
 * Thread priorities for receive thread and TX cleanup thread. Alter
 * to prioritize receive or transmit bandwidth. In a heavily loaded
 * system or with LEIP_DEBUG enabled, the priorities might be better
 * the same. */
#define RX_PRIORITY   (osPriorityNormal)
#define TX_PRIORITY   (osPriorityNormal)
#define PHY_PRIORITY  (osPriorityNormal)

void rda_netif_down(int netif)
{
    if (netif == 0) {
        netif_set_down(r91h_wifidata.netif_sta);
    } else {
        netif_set_down(r91h_wifidata.netif_ap);
    }
}

void rda_netif_link_down(int netif)
{
    rda_msg msg;
    msg.type = 1;
    msg.arg1 = 0;
    rda_mail_put(packet_rx_queue, (void*)&msg, 0);
}

void rda_netif_up(int netif)
{
    if (netif == 0) {
        netif_set_up(r91h_wifidata.netif_sta);
    } else {
        netif_set_up(r91h_wifidata.netif_ap);
    }
}

void rda_netif_link_up(int netif)
{
    rda_msg msg;
    msg.type = 1;
    msg.arg1 = 1;
    rda_mail_put(packet_rx_queue, (void*)&msg, 0);
    return;
}

void rda_netif_input(u8_t *data, u32_t len, int idx, int netif)
{
    if (netif == 0) {
        r91h_wifiif_input(r91h_wifidata.netif_sta, data, len, idx++);
    } else {
        r91h_wifiif_input(r91h_wifidata.netif_ap, data, len, idx++);
    }
}

void rda_get_macaddr(u8_t *macaddr, int mode)
{
    mbed_mac_address((char *)macaddr);
    if (mode == 1) {
        if(macaddr[0] & 0x04) {
           macaddr[0] &= 0xFB;
        } else {
           macaddr[0] |= 0x04;
        }
    }
    return;
}

void rda_critical_sec_start(void)
{
    if (__get_IPSR() == 0U) {
        if (0U == g_critical_sec_counter) {
#if defined(CONFIG_DISABLE_ALL_INT)
            g_critical_ctxt_saved = __disable_irq();
#else  /* CONFIG_DISABLE_ALL_INT */
            __set_BASEPRI(CRI_SEC_START_PRI_LEVEL);
#endif /* CONFIG_DISABLE_ALL_INT */
        }
        g_critical_sec_counter++;
    }
}

void rda_critical_sec_end(void)
{
    if (__get_IPSR() == 0U) {
        g_critical_sec_counter--;
        if (0U == g_critical_sec_counter) {
#if defined(CONFIG_DISABLE_ALL_INT)
            __set_PRIMASK(g_critical_ctxt_saved);
#else  /* CONFIG_DISABLE_ALL_INT */
            __set_BASEPRI(CRI_SEC_END_PRI_LEVEL);
#endif /* CONFIG_DISABLE_ALL_INT */
        }
    }
}

unsigned int rda_critical_sec_counter_get(void)
{
    if (__get_IPSR() == 0U) {
        return g_critical_sec_counter;
    } else {
        return 0xFFFFFFFF;
    }
}

void rda_critical_sec_start_resume(unsigned int cnt)
{
    unsigned int i = 0;
    for(i=0; i<cnt; i++) {
        rda_critical_sec_start();
    }
}

unsigned int rda_critical_sec_end_all(void)
{
    unsigned int ret = g_critical_sec_counter;
    while(g_critical_sec_counter !=0 ) {
        rda_critical_sec_end();
    }
    return ret;
}

void * rda_create_interrupt(unsigned int vec, unsigned int pri, void *isr)
{
    NVIC_SetPriority((IRQn_Type)vec, (uint32_t) pri);
    NVIC_SetVector((IRQn_Type)vec, (uint32_t) isr);

    return NULL;
}

void rda_delete_interrupt(unsigned int vec)
{
    NVIC_SetVector((IRQn_Type)vec, 0);
}

void rda_enable_interrupt(unsigned int vec)
{
    NVIC_EnableIRQ((IRQn_Type)vec);
}

void rda_disable_interrupt(unsigned int vec)
{
    NVIC_DisableIRQ((IRQn_Type)vec);
}

/** \brief  Allocates a pbuf and returns the data from the incoming packet.
 *
 *  \param[in] netif the lwip network interface structure
 *  \param[in] idx   index of packet to be read
 *  \return a pbuf filled with the received packet (including MAC header)
 */
static struct pbuf *r91h_low_level_input(struct netif *netif, u8_t *data, u32_t len, int idx)
{
    struct pbuf *p, *q;
    u16_t index = 0;

    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input enter, rxlen:%d\n", len));

    /* Obtain the size of the packet and put it into the "len"
     variable. */
    if(!len) {
        return NULL;
    }

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL) {

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf. */
        for (q = p; q != NULL; q = q->next) {
            /* Read enough bytes to fill this pbuf in the chain. The
            * available data in the pbuf is given by the q->len
            * variable.
            * This does not necessarily have to be a memcpy, you can also preallocate
            * pbufs for a DMA-enabled MAC and after receiving truncate it to the
            * actually received size. In this case, ensure the tot_len member of the
            * pbuf is the sum of the chained pbuf len members.
            */
            /* load rx data from 96 to local mem_pool */
            MEMCPY(q->payload, &data[index], q->len);
            index += q->len;

            if (index >= len) {
                break;
            }
        }

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    } else {
        /* Drop this packet */
        LWIP_DEBUGF(NETIF_DEBUG, ("low_level_input pbuf_alloc fail, rxlen:%d\n", len));
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);

        return NULL;
    }

    return p;
}

/** \brief  Attempt to read a packet from the EMAC interface.
 *
 *  \param[in] netif the lwip network interface structure
 *  \param[in] idx   index of packet to be read
 */
void r91h_wifiif_input(struct netif *netif, u8_t *data, u32_t len, int idx)
{
    rda_msg msg;
    msg.type = 0;
    msg.arg1 = (int)data;
    msg.arg2 = len;
    rda_mail_put(packet_rx_queue, (void*)&msg, 0);
    return;
}

/** \brief  Low level init of the MAC and PHY.
 *
 *  \param[in]      netif  Pointer to LWIP netif structure
 */

static err_t low_level_init(struct netif *netif)
{
    static int init_flag = 0;
    if (init_flag == 0) {
        wland_reg_func();
        init_flag = 1;
    }
    return ERR_OK;
}

/**
 * This function is the ethernet packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
err_t rda91h_etharp_output(struct netif *netif, struct pbuf *q, const ip_addr_t *ipaddr)
{
  /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP)
        return etharp_output(netif, q, ipaddr);

    return ERR_CONN;
}

/** \brief  Low level output of a packet. Never call this from an
 *          interrupt context, as it may block until TX descriptors
 *          become available.
 *
 *  \param[in] netif the lwip network interface structure for this netif
 *  \param[in] p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 *  \return ERR_OK if the packet could be sent or an err_t value if the packet couldn't be sent
 */
static err_t rda91h_low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;

    /* rda5996 initiate transfer */
    u32_t actual_txlen = 0;
    u8_t **data;
    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output enter, p:%08x\n", p));

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    data = (void*)wland_get_databuf();

    if(data == NULL){
        LWIP_DEBUGF(NETIF_DEBUG, ("rda91h_low_level_output, no PKT buf\r\n"));
        return ERR_BUF;
    }

    for(q = p; q != NULL; q = q->next)
    {
        /* Send the data from the pbuf to the interface, one pbuf at a
           time. The size of the data in each pbuf is kept in the ->len
           variable. */
        MEMCPY(&((*data)[actual_txlen+2]), q->payload, q->len);//reserve wid header length
        actual_txlen += q->len;
        if(actual_txlen > 1514 || actual_txlen > p->tot_len)
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output err, actual_txlen:%d, tot_len%d\n",
                    actual_txlen, p->tot_len));
            return ERR_BUF;
        }
    }

    /* Signal rda5996 that packet should be sent */
    if(actual_txlen == p->tot_len)
    {
        if(netif->name[0] == 's' && netif->name[1] == 't') {
            wland_txip_data((void*)data, actual_txlen, 0);
        } else if(netif->name[0] == 'a' && netif->name[1] == 'p') {
            wland_txip_data((void*)data, actual_txlen, 1);
        }

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.xmit);

        return ERR_OK;
    }

    LWIP_DEBUGF(NETIF_DEBUG, ("low_level_output pkt len mismatch, actual_txlen:%d, tot_len%d\n",
            actual_txlen, p->tot_len));


    return ERR_BUF;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param[in] netif the lwip network interface structure for this netif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t wifi_arch_enetif_init(struct netif *netif)
{
    err_t err;
    static int thread_init_flag = 0;
    LWIP_ASSERT("netif != NULL", (netif != NULL));

    if (*((int *)netif->state) == 0) {
        r91h_wifidata.netif_sta = netif;
        netif->name[0] = 's';
        netif->name[1] = 't';
#if LWIP_NETIF_HOSTNAME
        /* Initialize interface hostname */
        if(netif->hostname == NULL)
            netif->hostname = "lwipr91h_sta";
#endif /* LWIP_NETIF_HOSTNAME */
        rda_get_macaddr((u8_t *)(netif->hwaddr), 0);
    } else if(*((int *)netif->state) == 1) {
        r91h_wifidata.netif_ap = netif;
        netif->name[0] = 'a';
        netif->name[1] = 'p';
#if LWIP_NETIF_HOSTNAME
       /* Initialize interface hostname */
        if(netif->hostname == NULL)
            netif->hostname = "lwipr91h_ap";
#endif /* LWIP_NETIF_HOSTNAME */
        rda_get_macaddr((u8_t  *)(netif->hwaddr), 1);
    }

    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    // TODOETH: check if the flags are correct below
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP;

    /* Initialize the hardware */
    netif->state = &r91h_wifidata;
    err = low_level_init(netif);
    if (err != ERR_OK)
        return err;

    netif->output = rda91h_etharp_output;
    netif->linkoutput = rda91h_low_level_output;
    if(thread_init_flag == 0){
        /* PHY monitoring task */
        sys_thread_new("maclib_thread", maclib_task, netif->state, DEFAULT_THREAD_STACKSIZE*8, PHY_PRIORITY);
        sys_thread_new("wland_thread", wland_task, NULL, DEFAULT_THREAD_STACKSIZE*5, PHY_PRIORITY);
        /* Allow the PHY task to detect the initial link state and set up the proper flags */
        osDelay(10);
        thread_init_flag = 1;
    }
    return ERR_OK;
}

unsigned char rda_mac_addr[6] = "\0";
static int is_available_mac_addr(unsigned char *mac_addr)
{
    if (mac_addr[0]==0 && mac_addr[1]==0 && mac_addr[2]==0 &&
        mac_addr[3]==0 && mac_addr[4]==0 && mac_addr[5]==0)
        return 0;
    if (mac_addr[0]&0x1)
        return 0;
    return 1;
}
static void rda_get_macaddr_from_flash(unsigned char *macaddr)
{
    int ret;
    if (!mac_is_valid((char *)rda_mac_addr)) {
        ret = rda5981_flash_read_mac_addr(rda_mac_addr);
        if ((ret!=0) || (is_available_mac_addr(rda_mac_addr)==0)) {
#if defined(MBEDTLS_ENTROPY_HARDWARE_ALT)
            unsigned int out_len;
            ret = mbedtls_hardware_poll(NULL, rda_mac_addr, 6, &out_len);
            if (6 != out_len) {
                LWIP_DEBUGF(NETIF_DEBUG, ("out_len err:%d\n", out_len));
            }
#else
            ret = rda_trng_get_bytes(rda_mac_addr, 6);
#endif
            rda_mac_addr[0] &= 0xfe;    /* clear multicast bit */
            rda_mac_addr[0] |= 0x02;    /* set local assignment bit (IEEE802) */
            rda5981_flash_write_mac_addr(rda_mac_addr);
        }
    }
    memcpy(macaddr, rda_mac_addr, 6);
}
void mbed_mac_address(char *mac)
{
    mac[0] = 0xD6;
    mac[1] = 0x71;
    mac[2] = 0x36;
    mac[3] = 0x60;
    mac[4] = 0xD8;
#if !MBED_CONF_APP_ECHO_SERVER
    mac[5] = 0xF4;
#else
    mac[5] = 0xF3;
#endif
    return;
}
