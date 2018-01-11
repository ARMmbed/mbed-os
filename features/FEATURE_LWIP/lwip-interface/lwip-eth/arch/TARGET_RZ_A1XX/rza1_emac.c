#include "lwip/opt.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "lwip/ethip6.h"
#include "mbed_interface.h"
#include "ethernet_api.h"
#include "ethernetext_api.h"
#include "platform/mbed_toolchain.h"

#define RECV_TASK_PRI           (osPriorityNormal)
#define PHY_TASK_PRI            (osPriorityNormal)
#define PHY_TASK_WAIT           (200)

WEAK int ethernetext_init(ethernet_cfg_t *p_ethcfg)
{
    return -1;
}

WEAK void ethernetext_start_stop(int32_t mode)
{
}

WEAK int ethernetext_chk_link_mode(void)
{
    return NEGO_FAIL;
}

WEAK void ethernetext_set_link_mode(int32_t link)
{
}

WEAK int ethernet_init(void)
{
    return -1;
}

WEAK void ethernet_free(void)
{
}

WEAK int ethernet_write(const char *data, int size)
{
    return 0;
}

WEAK int ethernet_send(void)
{
    return 0;
}

WEAK int ethernet_receive(void)
{
    return 0;
}

WEAK int ethernet_read(char *data, int size)
{
    return 0;
}

WEAK void ethernet_address(char *mac)
{
}

WEAK int ethernet_link(void)
{
    return 0;
}

WEAK void ethernet_set_link(int speed, int duplex)
{
}


/* memory */
static sys_sem_t recv_ready_sem;    /* receive ready semaphore */

/* function */
static void rza1_recv_task(void *arg);
static void rza1_phy_task(void *arg);
#if LWIP_IPV4
static err_t rza1_etharp_output_ipv4(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr);
#endif
#if LWIP_IPV6
static err_t rza1_etharp_output_ipv6(struct netif *netif, struct pbuf *q, const ip6_addr_t *ipaddr);
#endif
static err_t rza1_low_level_output(struct netif *netif, struct pbuf *p);
static void rza1_recv_callback(void);

static void rza1_recv_task(void *arg) {
    struct netif   *netif = (struct netif*)arg;
    u16_t          recv_size;
    struct pbuf    *p;
    int            cnt;

    while (1) {
        sys_arch_sem_wait(&recv_ready_sem, 0);
        for (cnt = 0; cnt < 16; cnt++) {
            recv_size = ethernet_receive();
            if (recv_size != 0) {
                p = pbuf_alloc(PBUF_RAW, recv_size, PBUF_RAM);
                if (p != NULL) {
                    (void)ethernet_read((char *)p->payload, p->len);
                    /* full packet send to tcpip_thread to process */
                    if (netif->input(p, netif) != ERR_OK) {
                        /* Free buffer */
                        pbuf_free(p);
                    }
                }
            } else {
                break;
            }
        }
    }
}

static void rza1_phy_task(void *arg) {
    struct netif *netif = (struct netif*)arg;
    s32_t        connect_sts = 0;   /* 0: disconnect, 1:connect */
    s32_t        link_sts;
    s32_t        link_mode_new = NEGO_FAIL;
    s32_t        link_mode_old = NEGO_FAIL;

    while (1) {
        link_sts = ethernet_link();
        if (link_sts == 1) {
            link_mode_new = ethernetext_chk_link_mode();
            if (link_mode_new != link_mode_old) {
                if (connect_sts == 1) {
                    tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, (void*) netif, 1);
                }
                if (link_mode_new != NEGO_FAIL) {
                    ethernetext_set_link_mode(link_mode_new);
                    tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, (void*) netif, 1);
                    connect_sts = 1;
                }
            }
        } else {
            if (connect_sts != 0) {
                tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, (void*) netif, 1);
                link_mode_new = NEGO_FAIL;
                connect_sts   = 0;
            }
        }
        link_mode_old = link_mode_new;
        osDelay(PHY_TASK_WAIT);
    }
}

#if LWIP_IPV4
static err_t rza1_etharp_output_ipv4(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr) {
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return etharp_output(netif, q, ipaddr);
    }

    return ERR_CONN;
}
#endif

#if LWIP_IPV6
static err_t rza1_etharp_output_ipv6(struct netif *netif, struct pbuf *q, const ip6_addr_t *ipaddr) {
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return ethip6_output(netif, q, ipaddr);
    }

    return ERR_CONN;
}
#endif

static err_t rza1_low_level_output(struct netif *netif, struct pbuf *p) {
    struct pbuf *q;
    s32_t       cnt;
    err_t       err        = ERR_MEM;
    s32_t       write_size = 0;

    if ((p->payload != NULL) && (p->len != 0)) {
        /* If the first data can't be written, transmit descriptor is full. */
        for (cnt = 0; cnt < 100; cnt++) {
            write_size = ethernet_write((char *)p->payload, p->len);
            if (write_size != 0) {
                break;
            }
            osDelay(1);
        }
        if (write_size != 0) {
            for (q = p->next; q != NULL; q = q->next) {
                (void)ethernet_write((char *)q->payload, q->len);
            }
            if (ethernet_send() == 1) {
                err = ERR_OK;
            }
        }
    }

    return err;
}

static void rza1_recv_callback(void) {
    sys_sem_signal(&recv_ready_sem);
}

err_t eth_arch_enetif_init(struct netif *netif)
{
    ethernet_cfg_t ethcfg;

    /* set MAC hardware address */
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    netif->hwaddr[0] = MBED_MAC_ADDR_0;
    netif->hwaddr[1] = MBED_MAC_ADDR_1;
    netif->hwaddr[2] = MBED_MAC_ADDR_2;
    netif->hwaddr[3] = MBED_MAC_ADDR_3;
    netif->hwaddr[4] = MBED_MAC_ADDR_4;
    netif->hwaddr[5] = MBED_MAC_ADDR_5;
#else
    mbed_mac_address((char *)netif->hwaddr);
#endif
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;
#ifdef LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
#endif
#if LWIP_IPV6_MLD
    netif->flags |= NETIF_FLAG_MLD6;
#endif

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwiprza1";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'e';
    netif->name[1] = 'n';

#if LWIP_IPV4
    netif->output = rza1_etharp_output_ipv4;
#endif
#if LWIP_IPV6
  netif->output_ip6 = rza1_etharp_output_ipv6;
#endif

    netif->linkoutput = rza1_low_level_output;

    /* Initialize the hardware */
    ethcfg.int_priority = 6;
    ethcfg.recv_cb      = &rza1_recv_callback;
    ethcfg.ether_mac    = (char *)netif->hwaddr;
    ethernetext_init(&ethcfg);

    /* semaphore */
    sys_sem_new(&recv_ready_sem, 0);

    /* task */
    sys_thread_new("rza1_recv_task", rza1_recv_task, netif, DEFAULT_THREAD_STACKSIZE, RECV_TASK_PRI);
    sys_thread_new("rza1_phy_task", rza1_phy_task, netif, DEFAULT_THREAD_STACKSIZE, PHY_TASK_PRI);

    return ERR_OK;
}

void eth_arch_enable_interrupts(void) {
    ethernetext_start_stop(1);
}

void eth_arch_disable_interrupts(void) {
    ethernetext_start_stop(0);
}
