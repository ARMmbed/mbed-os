#include "lwip/opt.h"

#include "netif/etharp.h"
#include "lwip/tcpip.h"
#include "lwip/ethip6.h"
#include <string.h>
#include "cmsis_os2.h"
#include "mbed_interface.h"

// Check for LWIP having Ethernet enabled
#if LWIP_ARP || LWIP_ETHERNET

// Check for Ethernet HAL being present
#ifdef ETH_SUCCESS

#define RECV_TASK_PRI           (osPriorityHigh)
#define PHY_TASK_PRI            (osPriorityLow)
#define PHY_TASK_WAIT           (200)
#define ETH_ARCH_PHY_ADDRESS    (0x00)

ETH_HandleTypeDef EthHandle;

#if defined (__ICCARM__)   /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END; /* Ethernet Rx DMA Descriptor */

#if defined (__ICCARM__)   /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END; /* Ethernet Tx DMA Descriptor */

#if defined (__ICCARM__)   /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */

#if defined (__ICCARM__)   /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */

static sys_sem_t rx_ready_sem;    /* receive ready semaphore */
static sys_mutex_t tx_lock_mutex;

/* function */
static void _eth_arch_rx_task(void *arg);
static void _eth_arch_phy_task(void *arg);
#if defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx)\
    || defined (STM32F779xx)
static void _rmii_watchdog(void *arg);
#endif

#if LWIP_IPV4
static err_t _eth_arch_netif_output_ipv4(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr);
#endif
#if LWIP_IPV6
static err_t _eth_arch_netif_output_ipv6(struct netif *netif, struct pbuf *q, const ip6_addr_t *ipaddr);
#endif

static err_t _eth_arch_low_level_output(struct netif *netif, struct pbuf *p);
static struct pbuf * _eth_arch_low_level_input(struct netif *netif);
__weak uint8_t mbed_otp_mac_address(char *mac);
void mbed_default_mac_address(char *mac);

void _eth_config_mac(ETH_HandleTypeDef *heth);

/**
 * Ethernet Rx Transfer completed callback
 *
 * @param  heth: ETH handle
 * @retval None
 */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    sys_sem_signal(&rx_ready_sem);
}


/**
 * Ethernet IRQ Handler
 *
 * @param  None
 * @retval None
 */
void ETH_IRQHandler(void)
{
    HAL_ETH_IRQHandler(&EthHandle);
}



/**
 * In this function, the hardware should be initialized.
 * Called from eth_arch_enetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void _eth_arch_low_level_init(struct netif *netif)
{
    /* Init ETH */
    uint8_t MACAddr[6];
    EthHandle.Instance = ETH;
    EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    EthHandle.Init.Speed = ETH_SPEED_100M;
    EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    EthHandle.Init.PhyAddress = ETH_ARCH_PHY_ADDRESS;
#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    MACAddr[0] = MBED_MAC_ADDR_0;
    MACAddr[1] = MBED_MAC_ADDR_1;
    MACAddr[2] = MBED_MAC_ADDR_2;
    MACAddr[3] = MBED_MAC_ADDR_3;
    MACAddr[4] = MBED_MAC_ADDR_4;
    MACAddr[5] = MBED_MAC_ADDR_5;
#else
    mbed_mac_address((char *)MACAddr);
#endif
    EthHandle.Init.MACAddr = &MACAddr[0];
    EthHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
    EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    HAL_ETH_Init(&EthHandle);

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

 #if LWIP_ARP || LWIP_ETHERNET
    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] = EthHandle.Init.MACAddr[0];
    netif->hwaddr[1] = EthHandle.Init.MACAddr[1];
    netif->hwaddr[2] = EthHandle.Init.MACAddr[2];
    netif->hwaddr[3] = EthHandle.Init.MACAddr[3];
    netif->hwaddr[4] = EthHandle.Init.MACAddr[4];
    netif->hwaddr[5] = EthHandle.Init.MACAddr[5];

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* Configure MAC */
    _eth_config_mac(&EthHandle);

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(&EthHandle);
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

static err_t _eth_arch_low_level_output(struct netif *netif, struct pbuf *p)
{
    err_t errval;
    struct pbuf *q;
    uint8_t *buffer = (uint8_t*)(EthHandle.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc;
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;
    DmaTxDesc = EthHandle.TxDesc;
    bufferoffset = 0;

    sys_mutex_lock(&tx_lock_mutex);

    /* copy frame from pbufs to driver buffers */
    for (q = p; q != NULL; q = q->next) {
        /* Is this buffer available? If not, goto error */
        if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET) {
            errval = ERR_USE;
            goto error;
        }

        /* Get bytes in current lwIP buffer */
        byteslefttocopy = q->len;
        payloadoffset = 0;

        /* Check if the length of data to copy is bigger than Tx buffer size*/
        while ((byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE) {
            /* Copy data to Tx buffer*/
            memcpy((uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset));

            /* Point to next descriptor */
            DmaTxDesc = (ETH_DMADescTypeDef*)(DmaTxDesc->Buffer2NextDescAddr);

            /* Check if the buffer is available */
            if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET) {
                errval = ERR_USE;
                goto error;
            }

            buffer = (uint8_t*)(DmaTxDesc->Buffer1Addr);

            byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
            payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
            framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
            bufferoffset = 0;
        }

        /* Copy the remaining bytes */
        memcpy((uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy);
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength = framelength + byteslefttocopy;
    }

    /* Prepare transmit descriptors to give to DMA */
    HAL_ETH_TransmitFrame(&EthHandle, framelength);

    errval = ERR_OK;

error:

    /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
    if ((EthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET) {
        /* Clear TUS ETHERNET DMA flag */
        EthHandle.Instance->DMASR = ETH_DMASR_TUS;

        /* Resume DMA transmission*/
        EthHandle.Instance->DMATPDR = 0;
    }

    sys_mutex_unlock(&tx_lock_mutex);

    return errval;
}


/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf * _eth_arch_low_level_input(struct netif *netif)
{
    struct pbuf *p = NULL;
    struct pbuf *q;
    uint16_t len = 0;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;
    uint32_t bufferoffset = 0;
    uint32_t payloadoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t i = 0;


    /* get received frame */
    if (HAL_ETH_GetReceivedFrame(&EthHandle) != HAL_OK)
        return NULL;

    /* Obtain the size of the packet and put it into the "len" variable. */
    len = EthHandle.RxFrameInfos.length;
    buffer = (uint8_t*)EthHandle.RxFrameInfos.buffer;

    if (len > 0) {
        /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }

    if (p != NULL) {
        dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
        bufferoffset = 0;
        for (q = p; q != NULL; q = q->next) {
            byteslefttocopy = q->len;
            payloadoffset = 0;

            /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
            while ((byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE) {
                /* Copy data to pbuf */
                memcpy((uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

                /* Point to next descriptor */
                dmarxdesc = (ETH_DMADescTypeDef*)(dmarxdesc->Buffer2NextDescAddr);
                buffer = (uint8_t*)(dmarxdesc->Buffer1Addr);

                byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
                payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
                bufferoffset = 0;
            }
            /* Copy remaining data in pbuf */
            memcpy((uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }

    /* Release descriptors to DMA */
    /* Point to first descriptor */
    dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
    /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
    for (i = 0; i < EthHandle.RxFrameInfos.SegCount; i++) {
        dmarxdesc->Status |= ETH_DMARXDESC_OWN;
        dmarxdesc = (ETH_DMADescTypeDef*)(dmarxdesc->Buffer2NextDescAddr);
    }

    /* Clear Segment_Count */
    EthHandle.RxFrameInfos.SegCount = 0;

    /* When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((EthHandle.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET) {
        /* Clear RBUS ETHERNET DMA flag */
        EthHandle.Instance->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        EthHandle.Instance->DMARPDR = 0;
    }
    return p;
}

/**
 * This task receives input data
 *
 * \param[in] netif the lwip network interface structure
 */
static void _eth_arch_rx_task(void *arg)
{
    struct netif   *netif = (struct netif*)arg;
    struct pbuf    *p;

    while (1) {
        sys_arch_sem_wait(&rx_ready_sem, 0);
        p = _eth_arch_low_level_input(netif);
        if (p != NULL) {
            if (netif->input(p, netif) != ERR_OK) {
                pbuf_free(p);
                p = NULL;
            }
        }
    }
}

/**
 * This task checks phy link status and updates net status
 *
 * \param[in] netif the lwip network interface structure
 */
static void _eth_arch_phy_task(void *arg)
{
    struct netif   *netif = (struct netif*)arg;
    uint32_t phy_status = 0;

    while (1) {
        uint32_t status;
        if (HAL_ETH_ReadPHYRegister(&EthHandle, PHY_BSR, &status) == HAL_OK) {
            if ((status & PHY_LINKED_STATUS) && !(phy_status & PHY_LINKED_STATUS)) {
                tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, (void*) netif, 1);
            } else if (!(status & PHY_LINKED_STATUS) && (phy_status & PHY_LINKED_STATUS)) {
                tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, (void*) netif, 1);
            }
            phy_status = status;
        }
        osDelay(PHY_TASK_WAIT);
    }
}

#if defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx)\
    || defined (STM32F779xx)
/**
 * workaround for the ETH RMII bug in STM32F76x and STM32F77x revA
 *
 * \param[in] netif the lwip network interface structure
 */
static void _rmii_watchdog(void *arg)
{
    while(1) {
        /* some good packets are received */
        if (EthHandle.Instance->MMCRGUFCR > 0) {
            /* RMII Init is OK - would need service to terminate or suspend
             * the thread */
            while(1) {
                /*  don't do anything anymore */
                osDelay(0xFFFFFFFF);
            }
        } else if (EthHandle.Instance->MMCRFCECR > 10) {
            /* ETH received too many packets with CRC errors, resetting RMII */
            SYSCFG->PMC &= ~SYSCFG_PMC_MII_RMII_SEL;
            SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;
            EthHandle.Instance->MMCCR |= ETH_MMCCR_CR;
        } else {
            osDelay(100);
        }
    }
}
#endif

/**
 * This function is the ethernet IPv4 packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this lpc_enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
#if LWIP_IPV4
static err_t _eth_arch_netif_output_ipv4(struct netif *netif, struct pbuf *q, const ip4_addr_t *ipaddr)
{
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return etharp_output(netif, q, ipaddr);
    }
    return ERR_CONN;
}
#endif

/**
 * This function is the ethernet packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network IPv6 interface structure for this lpc_enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
#if LWIP_IPV6
static err_t _eth_arch_netif_output_ipv6(struct netif *netif, struct pbuf *q, const ip6_addr_t *ipaddr)
{
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return ethip6_output(netif, q, ipaddr);
    }
    return ERR_CONN;
}
#endif

/**
 * Should be called at the beginning of the program to set up the
 * network interface.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param[in] netif the lwip network interface structure for this lpc_enetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t eth_arch_enetif_init(struct netif *netif)
{
    /* set MAC hardware address */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwipstm32";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'e';
    netif->name[1] = 'n';

#if LWIP_IPV4
    netif->output = _eth_arch_netif_output_ipv4;
    netif->flags |= NETIF_FLAG_IGMP;
#endif
#if LWIP_IPV6
    netif->output_ip6 = _eth_arch_netif_output_ipv6;
    netif->flags |= NETIF_FLAG_MLD6;
#endif

    netif->linkoutput = _eth_arch_low_level_output;

    /* semaphore */
    sys_sem_new(&rx_ready_sem, 0);

    sys_mutex_new(&tx_lock_mutex);

    /* task */
    sys_thread_new("stm32_emac_rx_thread", _eth_arch_rx_task, netif, DEFAULT_THREAD_STACKSIZE, RECV_TASK_PRI);
    sys_thread_new("stm32_emac_phy_thread", _eth_arch_phy_task, netif, DEFAULT_THREAD_STACKSIZE, PHY_TASK_PRI);

    /* initialize the hardware */
    _eth_arch_low_level_init(netif);

#if defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx)\
    || defined (STM32F779xx)
    sys_thread_new("stm32_rmii_watchdog", _rmii_watchdog, netif, DEFAULT_THREAD_STACKSIZE, osPriorityLow);
#endif

    return ERR_OK;
}

void eth_arch_enable_interrupts(void)
{
    HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}

void eth_arch_disable_interrupts(void)
{
    NVIC_DisableIRQ(ETH_IRQn);
}

/** This returns a unique 6-byte MAC address, based on the device UID
*  This function overrides hal/common/mbed_interface.c function
*  @param mac A 6-byte array to write the MAC address
*/

void mbed_mac_address(char *mac) {
    if (mbed_otp_mac_address(mac)) {
        return;
    } else {
        mbed_default_mac_address(mac);
    }
    return;
}

__weak uint8_t mbed_otp_mac_address(char *mac) {
    return 0;
}

void mbed_default_mac_address(char *mac) {
    unsigned char ST_mac_addr[3] = {0x00, 0x80, 0xe1}; // default STMicro mac address

    // Read unic id
#if defined (TARGET_STM32F2)
    uint32_t word0 = *(uint32_t *)0x1FFF7A10;
#elif defined (TARGET_STM32F4)
    uint32_t word0 = *(uint32_t *)0x1FFF7A10;
#elif defined (TARGET_STM32F7)
    uint32_t word0 = *(uint32_t *)0x1FF0F420;
#else
    #error MAC address can not be derived from target unique Id
#endif

    mac[0] = ST_mac_addr[0];
    mac[1] = ST_mac_addr[1];
    mac[2] = ST_mac_addr[2];
    mac[3] = (word0 & 0x00ff0000) >> 16;
    mac[4] = (word0 & 0x0000ff00) >> 8;
    mac[5] = (word0 & 0x000000ff);

    return;
}

#endif //ETH_SUCCESS

#endif // LWIP_ARP || LWIP_ETHERNET
