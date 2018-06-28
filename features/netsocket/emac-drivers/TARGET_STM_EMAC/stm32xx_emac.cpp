#include <stdlib.h>

#include "cmsis_os.h"

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "mbed_shared_queues.h"
#include "netsocket/nsapi_types.h"

#include "stm32xx_emac_config.h"
#include "stm32xx_emac.h"

/* \brief Flags for worker thread */
#define FLAG_RX                 1

/** \brief  Driver thread priority */
#define THREAD_PRIORITY         (osPriorityHigh)

#define PHY_TASK_PERIOD_MS      200
#define ETH_ARCH_PHY_ADDRESS    (0x00)

#define STM_HWADDR_SIZE         (6)
#define STM_ETH_MTU_SIZE        1500
#define STM_ETH_IF_NAME         "st"

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

__weak uint8_t mbed_otp_mac_address(char *mac);
void mbed_default_mac_address(char *mac);

#ifdef __cplusplus
extern "C" {
#endif

void _eth_config_mac(ETH_HandleTypeDef *heth);
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth);
void ETH_IRQHandler(void);

#ifdef __cplusplus
}
#endif

/**
 * Ethernet Rx Transfer completed callback
 *
 * @param  heth: ETH handle
 * @retval None
 */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    STM32_EMAC &emac = STM32_EMAC::get_instance();
    if (emac.thread) {
        osThreadFlagsSet(emac.thread, FLAG_RX);
    }
}

/**
 * Ethernet IRQ Handler
 *
 * @param  None
 * @retval None
 */
void ETH_IRQHandler(void)
{
    STM32_EMAC &emac = STM32_EMAC::get_instance();
    HAL_ETH_IRQHandler(&emac.EthHandle);
}

STM32_EMAC::STM32_EMAC()
    : thread(0)
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

/**
 * In this function, the hardware should be initialized.
 */
bool STM32_EMAC::low_level_init_successful()
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
    EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_SOFTWARE;
    EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    HAL_ETH_Init(&EthHandle);

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

    /* Configure MAC */
    _eth_config_mac(&EthHandle);

    /* Enable MAC and DMA transmission and reception */
    HAL_ETH_Start(&EthHandle);

    return true;
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the memory buffer chain that is passed to the function.
 *
 * @param buf the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return true if the packet could be sent
 *         false value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
bool STM32_EMAC::link_out(emac_mem_buf_t *buf)
{
    bool success;
    emac_mem_buf_t *q;
    uint8_t *buffer = reinterpret_cast<uint8_t *>(EthHandle.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc;
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;
    DmaTxDesc = EthHandle.TxDesc;

    /* Get exclusive access */
    TXLockMutex.lock();

    /* copy frame from pbufs to driver buffers */
    for (q = buf; q != NULL; q = memory_manager->get_next(q)) {
        /* Is this buffer available? If not, goto error */
        if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET) {
            success = false;
            goto error;
        }

        /* Get bytes in current lwIP buffer */
        byteslefttocopy = memory_manager->get_len(q);
        payloadoffset = 0;

        /* Check if the length of data to copy is bigger than Tx buffer size*/
        while ((byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE) {
            /* Copy data to Tx buffer*/
            memcpy(static_cast<uint8_t *>(buffer) + bufferoffset, static_cast<uint8_t *>(memory_manager->get_ptr(q)) + payloadoffset, (ETH_TX_BUF_SIZE - bufferoffset));

            /* Point to next descriptor */
            DmaTxDesc = reinterpret_cast<ETH_DMADescTypeDef *>(DmaTxDesc->Buffer2NextDescAddr);

            /* Check if the buffer is available */
            if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET) {
                success = false;
                goto error;
            }

            buffer = reinterpret_cast<uint8_t *>(DmaTxDesc->Buffer1Addr);

            byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
            payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
            framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
            bufferoffset = 0;
        }

        /* Copy the remaining bytes */
        memcpy(static_cast<uint8_t *>(buffer) + bufferoffset, static_cast<uint8_t *>(memory_manager->get_ptr(q)) + payloadoffset, byteslefttocopy);
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength = framelength + byteslefttocopy;
    }

    /* Prepare transmit descriptors to give to DMA */
    HAL_ETH_TransmitFrame(&EthHandle, framelength);

    success = true;

error:

    /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
    if ((EthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET) {
        /* Clear TUS ETHERNET DMA flag */
        EthHandle.Instance->DMASR = ETH_DMASR_TUS;

        /* Resume DMA transmission*/
        EthHandle.Instance->DMATPDR = 0;
    }

    memory_manager->free(buf);

    /* Restore access */
    TXLockMutex.unlock();

    return success;
}

/**
 * Should allocate a contiguous memory buffer and transfer the bytes of the incoming
 * packet to the buffer.
 *
 * @param buf If a frame was received and the memory buffer allocation was successful, a memory
 *            buffer filled with the received packet (including MAC header)
 * @return negative value when no more frames,
 *         zero when frame is received
 */
int STM32_EMAC::low_level_input(emac_mem_buf_t **buf)
{
    uint16_t len = 0;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    emac_mem_buf_t *q;
    uint32_t payloadoffset = 0;

    /* get received frame */
    if (HAL_ETH_GetReceivedFrame_IT(&EthHandle) != HAL_OK) {
        return -1;
    }

    /* Obtain the size of the packet and put it into the "len" variable. */
    len = EthHandle.RxFrameInfos.length;
    buffer = reinterpret_cast<uint8_t *>(EthHandle.RxFrameInfos.buffer);
    byteslefttocopy = len;

    dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;

    if (len > 0) {
        /* Allocate a memory buffer chain from buffer pool */
        *buf = memory_manager->alloc_pool(len, 0);
    }

    if (*buf != NULL) {
        dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
        bufferoffset = 0;
        for (q = *buf; q != NULL; q = memory_manager->get_next(q)) {
            byteslefttocopy = memory_manager->get_len(q);
            payloadoffset = 0;

            /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
            while ((byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE) {
                /* Copy data to pbuf */
                memcpy(static_cast<uint8_t *>(memory_manager->get_ptr(q)) + payloadoffset, static_cast<uint8_t *>(buffer) + bufferoffset, ETH_RX_BUF_SIZE - bufferoffset);

                /* Point to next descriptor */
                dmarxdesc = reinterpret_cast<ETH_DMADescTypeDef *>(dmarxdesc->Buffer2NextDescAddr);
                buffer = reinterpret_cast<uint8_t *>(dmarxdesc->Buffer1Addr);

                byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
                payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
                bufferoffset = 0;
            }
            /* Copy remaining data in pbuf */
            memcpy(static_cast<uint8_t *>(memory_manager->get_ptr(q)) + payloadoffset, static_cast<uint8_t *>(buffer) + bufferoffset, byteslefttocopy);
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }

    /* Release descriptors to DMA */
    /* Point to first descriptor */
    dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
    /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
    for (uint32_t i = 0; i < EthHandle.RxFrameInfos.SegCount; i++) {
        dmarxdesc->Status |= ETH_DMARXDESC_OWN;
        dmarxdesc = reinterpret_cast<ETH_DMADescTypeDef *>(dmarxdesc->Buffer2NextDescAddr);
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
    return 0;
}


/** \brief  Attempt to read a packet from the EMAC interface.
 *
 */
void STM32_EMAC::packet_rx()
{
    /* move received packet into a new buf */
    while (1) {
        emac_mem_buf_t *p = NULL;
        if (low_level_input(&p) < 0) {
            break;
        }
        if (p) {
            emac_link_input_cb(p);
        }
    }
}

/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */
void STM32_EMAC::thread_function(void *pvParameters)
{
    static struct STM32_EMAC *stm32_enet = static_cast<STM32_EMAC *>(pvParameters);

    for (;;) {
        uint32_t flags = osThreadFlagsWait(FLAG_RX, osFlagsWaitAny, osWaitForever);

        if (flags & FLAG_RX) {
            stm32_enet->packet_rx();
        }
    }
}

/**
 * This task checks phy link status and updates net status
 */
void STM32_EMAC::phy_task()
{
    uint32_t status;

    if (HAL_ETH_ReadPHYRegister(&EthHandle, PHY_BSR, &status) == HAL_OK) {
        if (emac_link_state_cb) {
            if ((status & PHY_LINKED_STATUS) && !(phy_status & PHY_LINKED_STATUS)) {
                emac_link_state_cb(true);
            } else if (!(status & PHY_LINKED_STATUS) && (phy_status & PHY_LINKED_STATUS)) {
                emac_link_state_cb(false);
            }
        }
        phy_status = status;
    }

}

#if defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx)\
    || defined (STM32F779xx)
/**
 * workaround for the ETH RMII bug in STM32F76x and STM32F77x revA
 *
 * \param[in] netif the lwip network interface structure
 */

/** \brief  Worker thread.
 *
 * Woken by thread flags to receive packets or clean up transmit
 *
 *  \param[in] pvParameters pointer to the interface data
 */
void STM32_EMAC::rmii_watchdog_thread_function(void *pvParameters)
{
    struct STM32_EMAC *stm32_enet = static_cast<STM32_EMAC *>(pvParameters);

    while (1) {
        /* some good packets are received */
        if (stm32_enet->EthHandle.Instance->MMCRGUFCR > 0) {
            /* RMII Init is OK - would need service to terminate or suspend
             * the thread */
            while (1) {
                /*  don't do anything anymore */
                osDelay(0xFFFFFFFF);
            }
        } else if (stm32_enet->EthHandle.Instance->MMCRFCECR > 10) {
            /* ETH received too many packets with CRC errors, resetting RMII */
            SYSCFG->PMC &= ~SYSCFG_PMC_MII_RMII_SEL;
            SYSCFG->PMC |= SYSCFG_PMC_MII_RMII_SEL;
            stm32_enet->EthHandle.Instance->MMCCR |= ETH_MMCCR_CR;
        } else {
            osDelay(100);
        }
    }
}
#endif

void STM32_EMAC::enable_interrupts(void)
{
    HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}

void STM32_EMAC::disable_interrupts(void)
{
    NVIC_DisableIRQ(ETH_IRQn);
}

/** This returns a unique 6-byte MAC address, based on the device UID
*  This function overrides hal/common/mbed_interface.c function
*  @param mac A 6-byte array to write the MAC address
*/

void mbed_mac_address(char *mac)
{
    if (mbed_otp_mac_address(mac)) {
        return;
    } else {
        mbed_default_mac_address(mac);
    }
    return;
}

__weak uint8_t mbed_otp_mac_address(char *mac)
{
    return 0;
}

void mbed_default_mac_address(char *mac)
{
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

bool STM32_EMAC::power_up()
{
    /* Initialize the hardware */
    if (!low_level_init_successful()) {
        return false;
    }

    /* Worker thread */
    thread = create_new_thread("stm32_emac_thread", &STM32_EMAC::thread_function, this, THREAD_STACKSIZE, THREAD_PRIORITY, &thread_cb);

    phy_task_handle = mbed::mbed_event_queue()->call_every(PHY_TASK_PERIOD_MS, mbed::callback(this, &STM32_EMAC::phy_task));

#if defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx)\
      || defined (STM32F779xx)
    rmii_watchdog_thread = create_new_thread("stm32_rmii_watchdog", &STM32_EMAC::rmii_watchdog_thread_function, this, THREAD_STACKSIZE, THREAD_PRIORITY, &rmii_watchdog_thread_cb);
#endif

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    osDelay(10);

    enable_interrupts();

    return true;
}

uint32_t STM32_EMAC::get_mtu_size() const
{
    return STM_ETH_MTU_SIZE;
}

uint32_t STM32_EMAC::get_align_preference() const
{
    return 0;
}

void STM32_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, STM_ETH_IF_NAME, (size < sizeof(STM_ETH_IF_NAME)) ? size : sizeof(STM_ETH_IF_NAME));
}

uint8_t STM32_EMAC::get_hwaddr_size() const
{
    return STM_HWADDR_SIZE;
}

bool STM32_EMAC::get_hwaddr(uint8_t *addr) const
{
    mbed_mac_address((char *)addr);
    return true;
}

void STM32_EMAC::set_hwaddr(const uint8_t *addr)
{
    /* No-op at this stage */
}

void STM32_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

void STM32_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

void STM32_EMAC::add_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void STM32_EMAC::remove_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

void STM32_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

void STM32_EMAC::power_down()
{
    /* No-op at this stage */
}

void STM32_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

STM32_EMAC &STM32_EMAC::get_instance()
{
    static STM32_EMAC emac;
    return emac;
}

// Weak so a module can override
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return STM32_EMAC::get_instance();
}
