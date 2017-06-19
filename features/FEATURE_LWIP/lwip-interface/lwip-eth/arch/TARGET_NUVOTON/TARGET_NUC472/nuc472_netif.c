/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */
 
#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "netif/ppp/pppoe.h"
#include "nuc472_eth.h"
#include "string.h"

#include "eth_arch.h"
#include "sys_arch.h"
#include <ctype.h>
#include <stdio.h>

#include "mbed_interface.h"
#include "cmsis.h"


/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

// Fow now, all interrupt handling happens inside one single handler, so we need to figure
// out what actually triggered the interrupt.
static volatile uint8_t emac_timer_fired;
volatile uint8_t allow_net_callbacks;

struct netif *_netif;

unsigned char my_mac_addr[6] = {0x02, 0x00, 0xac, 0x55, 0x66, 0x77};
extern u8_t my_mac_addr[6];
extern int ETH_link_ok(void);
extern void EMAC_RX_Action(void);

sys_sem_t RxReadySem; /**< RX packet ready semaphore */

static void __phy_task(void *data);
static void __packet_rx_task(void *data);
/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
    struct eth_addr *ethaddr;
    /* Add whatever per-interface state that is needed here. */
};

// Override mbed_mac_address of mbed_interface.c to provide ethernet devices with a semi-unique MAC address
void mbed_mac_address(char *mac)
{
    uint32_t uID1;
    // Fetch word 0
    uint32_t word0 = *(uint32_t *)0x7F804; // 2KB Data Flash at 0x7F800
    // Fetch word 1
    // we only want bottom 16 bits of word1 (MAC bits 32-47)
    // and bit 9 forced to 1, bit 8 forced to 0
    // Locally administered MAC, reduced conflicts
    // http://en.wikipedia.org/wiki/MAC_address
    uint32_t word1 = *(uint32_t *)0x7F800; // 2KB Data Flash at 0x7F800

	if( word0 == 0xFFFFFFFF )		// Not burn any mac address at 1st 2 words of Data Flash
	{
        // with a semi-unique MAC address from the UUID
        /* Enable FMC ISP function */
        SYS_UnlockReg();
        FMC_Open();
        // = FMC_ReadUID(0);
        uID1 = FMC_ReadUID(1);
        word1 = (uID1 & 0x003FFFFF) | ((uID1 & 0x030000) << 6) >> 8;
        word0 = ((FMC_ReadUID(0) >> 4) << 20) | ((uID1 & 0xFF)<<12) | (FMC_ReadUID(2) & 0xFFF);
        /* Disable FMC ISP function */
        FMC_Close();
        /* Lock protected registers */
        SYS_LockReg();
	}

    word1 |= 0x00000200;
    word1 &= 0x0000FEFF;

    mac[0] = (word1 & 0x0000ff00) >> 8;    
    mac[1] = (word1 & 0x000000ff);
    mac[2] = (word0 & 0xff000000) >> 24;
    mac[3] = (word0 & 0x00ff0000) >> 16;
    mac[4] = (word0 & 0x0000ff00) >> 8;
    mac[5] = (word0 & 0x000000ff);
    
    LWIP_DEBUGF(LWIP_DBG_LEVEL_WARNING|LWIP_DBG_ON, ("mac address %02x-%02x-%02x-%02x-%02x-%02x \r\n", mac[0], mac[1],mac[2],mac[3],mac[4],mac[5]));
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{

    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

  /* set MAC hardware address */
#if 1  // set MAC HW address
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
  netif->hwaddr[0] = MBED_MAC_ADDR_0;
  netif->hwaddr[1] = MBED_MAC_ADDR_1;
  netif->hwaddr[2] = MBED_MAC_ADDR_2;
  netif->hwaddr[3] = MBED_MAC_ADDR_3;
  netif->hwaddr[4] = MBED_MAC_ADDR_4;
  netif->hwaddr[5] = MBED_MAC_ADDR_5;
#else
  mbed_mac_address((char *)netif->hwaddr);
#endif  /* set MAC HW address */

#else
    netif->hwaddr[0] = my_mac_addr[0];
    netif->hwaddr[1] = my_mac_addr[1];
    netif->hwaddr[2] = my_mac_addr[2];
    netif->hwaddr[3] = my_mac_addr[3];
    netif->hwaddr[4] = my_mac_addr[4];
    netif->hwaddr[5] = my_mac_addr[5];
#endif // endif 
	
    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* NETIF_FLAG_LINK_UP should be enabled by netif_set_link_up() */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET; 
#ifdef LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
#endif
#if LWIP_IPV6_MLD
    netif->flags |= NETIF_FLAG_MLD6;
#endif
    // TODO: enable clock & configure GPIO function
    ETH_init(netif->hwaddr);

#if LWIP_IGMP
    EMAC_ENABLE_RECV_BCASTPKT();
#endif

#if LWIP_IPV6_MLD
    EMAC_ENABLE_RECV_MCASTPKT();
#endif
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    u8_t *buf = NULL;
    u16_t len = 0;


    buf = ETH_get_tx_buf();
    if(buf == NULL)
        return ERR_MEM;
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    for(q = p; q != NULL; q = q->next) {
        memcpy((u8_t*)&buf[len], q->payload, q->len);
        len = len + q->len;
    }
#ifdef TIME_STAMPING
    ETH_trigger_tx(len, p->flags & PBUF_FLAG_GET_TXTS ? p : NULL);
#else
    ETH_trigger_tx(len, NULL);
#endif

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.xmit);

    return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif, u16_t len, u8_t *buf)
{
    struct pbuf *p, *q;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL) {

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif


        len = 0;
        /* We iterate over the pbuf chain until we have read the entire
        * packet into the pbuf. */
        for(q = p; q != NULL; q = q->next) {
            memcpy((u8_t*)q->payload, (u8_t*)&buf[len], q->len);
            len = len + q->len;
        }


#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    } else {
        // do nothing. drop the packet
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
    }

    return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void
ethernetif_input(u16_t len, u8_t *buf, u32_t s, u32_t ns)
{
    struct eth_hdr *ethhdr;
    struct pbuf *p;


    /* move received packet into a new pbuf */
    p = low_level_input(_netif, len, buf);
    /* no packet could be read, silently ignore this */
    if (p == NULL) return;
#ifdef TIME_STAMPING
    p->ts_sec = s;
    p->ts_nsec = ns;
#endif

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch (htons(ethhdr->type)) {
    /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /* full packet send to tcpip_thread to process */
        if (_netif->input(p, _netif)!=ERR_OK) {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
            p = NULL;
        }
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }
}

#ifdef    TIME_STAMPING
void
ethernetif_loopback_input(struct pbuf *p)           // TODO: make sure packet not drop in input()
{
    /* pass all packets to ethernet_input, which decides what packets it supports */
    if (netif->input(p, netif) != ERR_OK) {
        LWIP_DEBUGF(NETIF_DEBUG, ("netif_input: input error\n"));
        /* Free buffer */
        pbuf_free(p);
    }
}

#endif

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
 eth_arch_enetif_init(struct netif *netif)
{
    err_t err;
    struct ethernetif *ethernetif;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    _netif = netif;
    ethernetif = mem_malloc(sizeof(struct ethernetif));
    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, (" eth_arch_enetif_init: out of memory\n"));
        return ERR_MEM;
    }

   // Chris: The initialization code uses osDelay, so timers neet to run
   // SysTick_Init();
	
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "nuc472";
#endif /* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

    netif->state = ethernetif;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
#if LWIP_IPV4
    netif->output = etharp_output;
#endif
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif
    netif->linkoutput = low_level_output;

    ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

    /* initialize the hardware */
    low_level_init(netif);

    /* Packet receive task */

    err = sys_sem_new(&RxReadySem, 0);
    if(err != ERR_OK) LWIP_ASSERT("RxReadySem creation error", (err == ERR_OK));
	// In GCC code, DEFAULT_THREAD_STACKSIZE 512 bytes is not enough for rx_task
#if defined (__GNUC__)	
    // mbed OS 2.0, DEFAULT_THREAD_STACKSIZE*3
	// mbed OS 5.0, DEFAULT_THREAD_STACKSIZE*5 
	sys_thread_new("nuc472_emac_rx_thread", __packet_rx_task, &RxReadySem, DEFAULT_THREAD_STACKSIZE*5, osPriorityNormal);
#else
	sys_thread_new("nuc472_emac_rx_thread", __packet_rx_task, &RxReadySem, DEFAULT_THREAD_STACKSIZE, osPriorityNormal);
#endif
    /* PHY monitoring task */
#if defined (__GNUC__)		
    // mbed OS 2.0, DEFAULT_THREAD_STACKSIZE
	// mbed OS 5.0, DEFAULT_THREAD_STACKSIZE*2
    sys_thread_new("nuc472_emac_phy_thread", __phy_task, netif, DEFAULT_THREAD_STACKSIZE*2, osPriorityNormal);
#else
    sys_thread_new("nuc472_emac_phy_thread", __phy_task, netif, DEFAULT_THREAD_STACKSIZE, osPriorityNormal);
#endif	
    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    osDelay(10);
	
    return ERR_OK;
}

void eth_arch_enable_interrupts(void) {
//  enet_hal_config_interrupt(BOARD_DEBUG_ENET_INSTANCE_ADDR, (kEnetTxFrameInterrupt | kEnetRxFrameInterrupt), true);
    EMAC->INTEN |= EMAC_INTEN_RXIEN_Msk |
                   EMAC_INTEN_TXIEN_Msk ;
	NVIC_EnableIRQ(EMAC_RX_IRQn);
	NVIC_EnableIRQ(EMAC_TX_IRQn);
}

void eth_arch_disable_interrupts(void) {
	NVIC_DisableIRQ(EMAC_RX_IRQn);
	NVIC_DisableIRQ(EMAC_TX_IRQn);
}


/* Defines the PHY link speed */
typedef enum _phy_speed
{
    kPHY_Speed10M = 0U, /* ENET PHY 10M speed. */
    kPHY_Speed100M      /* ENET PHY 100M speed. */
} phy_speed_t;

/* Defines the PHY link duplex. */
typedef enum _phy_duplex
{
    kPHY_HalfDuplex = 0U, /* ENET PHY half duplex. */
    kPHY_FullDuplex       /* ENET PHY full duplex. */
} phy_duplex_t;

typedef struct {
    int connected;
    phy_speed_t speed;
    phy_duplex_t duplex;
} PHY_STATE;

#define STATE_UNKNOWN           (-1)

static void __phy_task(void *data) {
  struct netif *netif = (struct netif*)data;
//  PHY_STATE crt_state = {STATE_UNKNOWN, (phy_speed_t)STATE_UNKNOWN, (phy_duplex_t)STATE_UNKNOWN};
//  PHY_STATE prev_state;

//  prev_state = crt_state;
  while (1) {
    // Get current status
    // Get the actual PHY link speed
    // Compare with previous state
			
			if( !(ETH_link_ok()) && (netif->flags & NETIF_FLAG_LINK_UP) ) {
				/* tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, (void*) netif, 1); */
				netif_set_link_down(netif);
				LWIP_DEBUGF(LWIP_DBG_LEVEL_WARNING|LWIP_DBG_ON, ("Link Down\r\n"));	
			}else if ( ETH_link_ok() && !(netif->flags & NETIF_FLAG_LINK_UP) ) { 
				/* tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, (void*) netif, 1); */
				netif_set_link_up(netif);
				LWIP_DEBUGF(LWIP_DBG_LEVEL_WARNING|LWIP_DBG_ON, ("Link Up\r\n"));
			}
			
    osDelay(200);
  }
}

void ack_emac_rx_isr()
{
  sys_sem_signal(&RxReadySem);
}

static void __packet_rx_task(void *data) {

  while (1) {
    /* Wait for receive task to wakeup */
    sys_arch_sem_wait(&RxReadySem, 0);
	EMAC_RX_Action();
  }
}
