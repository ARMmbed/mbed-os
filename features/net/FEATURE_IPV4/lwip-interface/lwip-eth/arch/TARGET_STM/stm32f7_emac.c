#if defined(TARGET_NUCLEO_F746ZG)
#include "stm32f7xx_hal.h"
#include "lwip/opt.h"

#include "lwip/timers.h"
#include "netif/etharp.h"
#include "lwip/tcpip.h"
#include <string.h>
#include "cmsis_os.h"
#include "mbed_interface.h"

/** @defgroup lwipstm32f7xx_emac_DRIVER	stm32f7 EMAC driver for LWIP
 * @ingroup lwip_emac
 *
 * @{
 */

#define RECV_TASK_PRI           (osPriorityHigh)
#define PHY_TASK_PRI            (osPriorityLow)
#define PHY_TASK_WAIT           (200)
/* LAN8742A PHY Address*/
#define LAN8742A_PHY_ADDRESS            0x00


#if defined ( __CC_ARM   )
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__((at(0x20002000)));/* Ethernet Rx MA Descriptor */

ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__((at(0x200020A0)));/* Ethernet Tx DMA Descriptor */

uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__((at(0x20002140))); /* Ethernet Receive Buffer */

uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__((at(0x20004480))); /* Ethernet Transmit Buffer */

#elif defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4 

#pragma location=0x20002000
__no_init ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB];/* Ethernet Rx MA Descriptor */

#pragma location=0x200020A0
__no_init ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB];/* Ethernet Tx DMA Descriptor */

#pragma location=0x20002140
__no_init uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE]; /* Ethernet Receive Buffer */

#pragma location=0x20004480
__no_init uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE]; /* Ethernet Transmit Buffer */

#elif defined ( __GNUC__ ) /*!< GNU Compiler */

ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __attribute__((section(".RxDecripSection")));/* Ethernet Rx MA Descriptor */

ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __attribute__((section(".TxDescripSection")));/* Ethernet Tx DMA Descriptor */

uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __attribute__((section(".RxarraySection"))); /* Ethernet Receive Buffer */

uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __attribute__((section(".TxarraySection"))); /* Ethernet Transmit Buffer */

#endif

ETH_HandleTypeDef EthHandle;

static sys_sem_t rx_ready_sem;    /* receive ready semaphore */
static sys_mutex_t tx_lock_mutex;

/* function */
static void stm32f7_rx_task(void *arg);
static void stm32f7_phy_task(void *arg);
static err_t stm32f7_etharp_output(struct netif *netif, struct pbuf *q, ip_addr_t *ipaddr);
static err_t stm32f7_low_level_output(struct netif *netif, struct pbuf *p);

/**
 * Override HAL Eth Init function
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef* heth)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if (heth->Instance == ETH) {

        /* Enable GPIOs clocks */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PB13
         */
        /* Configure PA1, PA2 and PA7 */
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL; 
        GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* Configure PB13 */
        GPIO_InitStructure.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* Configure PC1, PC4 and PC5 */
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

        /* Configure PG2, PG11 and PG13 */
        GPIO_InitStructure.Pin =  GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

        /* Enable the Ethernet global Interrupt */
        HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
        HAL_NVIC_EnableIRQ(ETH_IRQn);
        
        /* Enable ETHERNET clock  */
        __HAL_RCC_ETH_CLK_ENABLE();
    }
}

/**
 * Override HAL Eth DeInit function
 */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef* heth)
{
    if (heth->Instance == ETH) {
        /* Peripheral clock disable */
        __HAL_RCC_ETH_CLK_DISABLE();

        /** ETH GPIO Configuration
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> PG2
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PB13
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7);
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_2 | GPIO_PIN_11 | GPIO_PIN_13);

        /* Disable the Ethernet global Interrupt */
        NVIC_DisableIRQ(ETH_IRQn);
    }
}

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
static void stm32f7_low_level_init(struct netif *netif)
{
    uint32_t regvalue = 0;
    HAL_StatusTypeDef hal_eth_init_status;

    /* Init ETH */
    uint8_t MACAddr[6];
    EthHandle.Instance = ETH;
    EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    EthHandle.Init.Speed = ETH_SPEED_100M;
    EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    EthHandle.Init.PhyAddress = LAN8742A_PHY_ADDRESS;
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
    hal_eth_init_status = HAL_ETH_Init(&EthHandle);


    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

 #if LWIP_ARP || LWIP_ETHERNET
    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

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

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(&EthHandle);

    /**** Configure PHY to generate an interrupt when Eth Link state changes ****/
    /* Read Register Configuration */
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_MICR, &regvalue);

    regvalue |= (PHY_MICR_INT_EN | PHY_MICR_INT_OE);

    /* Enable Interrupts */
    HAL_ETH_WritePHYRegister(&EthHandle, PHY_MICR, regvalue);

    /* Read Register Configuration */
    HAL_ETH_ReadPHYRegister(&EthHandle, PHY_MISR, &regvalue);

    regvalue |= PHY_MISR_LINK_INT_EN;

    /* Enable Interrupt on change of link status */
    HAL_ETH_WritePHYRegister(&EthHandle, PHY_MISR, regvalue);
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

static err_t stm32f7_low_level_output(struct netif *netif, struct pbuf *p)
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
static struct pbuf * stm32f7_low_level_input(struct netif *netif)
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
    }

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
static void stm32f7_rx_task(void *arg)
{
    struct netif   *netif = (struct netif*)arg;
    struct pbuf    *p;

    while (1) {
        sys_arch_sem_wait(&rx_ready_sem, 0);
        p = stm32f7_low_level_input(netif);
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
static void stm32f7_phy_task(void *arg)
{
    struct netif   *netif = (struct netif*)arg;
    uint32_t phy_status = 0;
    
    while (1) {
        uint32_t status;
        if (HAL_ETH_ReadPHYRegister(&EthHandle, PHY_SR, &status) == HAL_OK) {
            if ((status & PHY_LINK_STATUS) && !(phy_status & PHY_LINK_STATUS)) {
                tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_up, (void*) netif, 1);
            } else if (!(status & PHY_LINK_STATUS) && (phy_status & PHY_LINK_STATUS)) {
                tcpip_callback_with_block((tcpip_callback_fn)netif_set_link_down, (void*) netif, 1);
            }
            
            phy_status = status;
        }
        
        osDelay(PHY_TASK_WAIT);
    }
}

/**
 * This function is the ethernet packet send function. It calls
 * etharp_output after checking link status.
 *
 * \param[in] netif the lwip network interface structure for this lpc_enetif
 * \param[in] q Pointer to pbug to send
 * \param[in] ipaddr IP address
 * \return ERR_OK or error code
 */
static err_t stm32f7_etharp_output(struct netif *netif, struct pbuf *q, ip_addr_t *ipaddr)
{
    /* Only send packet is link is up */
    if (netif->flags & NETIF_FLAG_LINK_UP) {
        return etharp_output(netif, q, ipaddr);
    }

    return ERR_CONN;
}

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
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* maximum transfer unit */
    netif->mtu = 1500;

    /* device capabilities */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP;

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwipstm32f7";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = 'e';
    netif->name[1] = 'n';

    netif->output = stm32f7_etharp_output;
    netif->linkoutput = stm32f7_low_level_output;

    /* semaphore */
    sys_sem_new(&rx_ready_sem, 0);
    
    sys_mutex_new(&tx_lock_mutex);

    /* task */
    sys_thread_new("stm32f7_recv_task", stm32f7_rx_task, netif, DEFAULT_THREAD_STACKSIZE, RECV_TASK_PRI);
    sys_thread_new("stm32f7_phy_task", stm32f7_phy_task, netif, DEFAULT_THREAD_STACKSIZE, PHY_TASK_PRI);
    
    /* initialize the hardware */
    stm32f7_low_level_init(netif);
    
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

/**
 * @}
 */
#endif //defined (TARGET_NUCLEO_F746ZG)

/* --------------------------------- End Of File ------------------------------ */
