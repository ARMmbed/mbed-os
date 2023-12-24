/* mbed Microcontroller Library
 * Copyright (c) 2006-2023 ARM Limited
 * Copyright (c) 2023 Geehy Semiconductor Inc.
 *
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

#include <stdlib.h>

#include "cmsis_os.h"

#include "mbed_interface.h"
#include "mbed_assert.h"
#include "events/mbed_shared_queues.h"
#include "netsocket/nsapi_types.h"

#include "apm32xx_emac.h"
#include "apm32f4xx_eth.h"

using namespace std::chrono;

/* \brief Flags for worker thread */
#define _ETH_FLAG_RX                       (1)

/** \brief  Driver thread priority */
#define _THREAD_STACKSIZE                  (1024)
#define _THREAD_PRIORITY                   (osPriorityHigh)

#define _PHY_TASK_PERIOD                   (200ms)

#define _ETH_HW_ADDR_SIZE                  (6)
#define _ETH_MTU_SIZE                      (1500)
#define _ETH_IF_NAME                       "ap"

#define _ETH_BOARD_PHY_ADDRESS             (0x00)
#define _ETH_HARDWARE_CHECKSUM             (0)

#define _APM_MAC_ADDR0                     0x00
#define _APM_MAC_ADDR1                     0x08
#define _APM_MAC_ADDR2                     0x10
#define _APM32_ID_ADDR                     0x1FFF7A10
/* ENET RxDMA/TxDMA descriptor */

extern ETH_DMADescConfig_T DMARxDscrTab[ETH_RXBUFNB];
extern ETH_DMADescConfig_T DMATxDscrTab[ETH_TXBUFNB];

/* ENET receive buffer  */
extern uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE];
/* ENET transmit buffer */
extern uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];
/*global transmit and receive descriptors pointers */
extern ETH_DMADescConfig_T  *DMATxDescToSet;
extern ETH_DMADescConfig_T  *DMARxDescToGet;

ETH_Config_T ETH_InitStructure;

#ifdef __cplusplus
extern "C" {
#endif

void ETH_IRQHandler(void);
#ifdef __cplusplus
}
#endif

/*!
 * @brief  Delay
 *
 * @param  count     the num of delay
 *
 * @retval None
 *
 * @note
 */
void EthResetDelay(__IO uint32_t count)
{
    __IO uint32_t i = 0;
    for (i = count; i != 0; i--)
    {
    }
}

/*!
 * @brief  ETH Tx packet
 *
 * @param  Length   The length of packet
 *
 * @retval The status of sending packet.
 *
 * @note
 */
int ETH_TxPacket(u16 Length)
{
    /** Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
    if ((DMATxDescToSet->Status & ETH_DMATXDESC_OWN) != (u32)RESET)
    {
        /** Return ERROR: OWN bit set */
        return ETH_ERROR;
    }

    /** Setting the Frame Length: bits[12:0] */
    DMATxDescToSet->ControlBufferSize = (Length & ETH_DMATXDESC_TXBS1);

    /** Setting the last segment and first segment bits */
    DMATxDescToSet->Status |= ETH_DMATXDESC_LS | ETH_DMATXDESC_FS;

    /** Set Own bit of the Tx descriptor Status: gives the buffer back to ETHERNET DMA */
    DMATxDescToSet->Status |= ETH_DMATXDESC_OWN;

    /** When Tx Buffer unavailable flag is set: clear it and resume transmission */
    if ((ETH->DMASTS & BIT2) != (u32)RESET)
    {
        /** Clear TBUS ETHERNET DMA flag */
        ETH->DMASTS = BIT2;
        /** Resume DMA transmission*/
        ETH->DMATXPD = 0;
    }

    /** Update the ETHERNET DMA global Tx descriptor with next Tx decriptor */
    /** Chained Mode */
    /** Selects the next DMA Tx descriptor list for next buffer to send */
    DMATxDescToSet = (ETH_DMADescConfig_T*)(DMATxDescToSet->Buffer2NextDescAddr);

    /** Return SUCCESS */
    return ETH_SUCCESS;
}

/*!
 * @brief  ETH Rx packet
 *
 * @param  None
 *
 * @retval Receive Frame
 *
 */
ETH_Frame_T ETH_RxPacket(void)
{
    u32 ethframelength = 0;
    ETH_Frame_T ethframe = {0,0};

    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
    if ((DMARxDescToGet->Status & ETH_DMARXDESC_OWN) != (u32)RESET)
    {
        ethframe.length = ETH_ERROR;

      if ((ETH->DMASTS & BIT7) != (u32)RESET)
      {
          /* Clear RBUS ETHERNET DMA flag */
          ETH->DMASTS = BIT7;
          /* Resume DMA reception */
          ETH->DMARXPD = 0;
      }

      /* Return error: OWN bit set */
      return ethframe;
    }

    if (((DMARxDescToGet->Status & ETH_DMARXDESC_ERRS) == (u32)RESET) && \
     ((DMARxDescToGet->Status & ETH_DMARXDESC_LDES) != (u32)RESET) && \
     ((DMARxDescToGet->Status & ETH_DMARXDESC_FDES) != (u32)RESET))
    {
        /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
        ethframelength = ((DMARxDescToGet->Status & ETH_DMARXDESC_FL) >> 16) - 4;

        /* Get the addrees of the actual buffer */
        ethframe.buffer = DMARxDescToGet->Buffer1Addr;
    }
    else
    {
        /* Return ERROR */
        ethframelength = ETH_ERROR;
    }

    ethframe.length = ethframelength;
    ethframe.descriptor = DMARxDescToGet;

    /* Selects the next DMA Rx descriptor list for next buffer to read */
    DMARxDescToGet = (ETH_DMADescConfig_T*) (DMARxDescToGet->Buffer2NextDescAddr);

    /* Return Frame */
    return ethframe;
}

/*!
 * @brief  configurate Ethernet GPIO.
 *
 * @param  None
 *
 * @retval None
 *
 * @note
 */
void ConfigEthernetGPIO(void)
{
    /*
          RMII_REF_CLK ----------------------> PA1
          RMII_MDIO -------------------------> PA2
          RMII_MDC --------------------------> PC1
          RMII_MII_CRS_DV -------------------> PA7
          RMII_MII_RXD0 ---------------------> PC4
          RMII_MII_RXD1 ---------------------> PC5
          RMII_MII_RXER ---------------------> none
          RMII_MII_TX_EN --------------------> PG11
          RMII_MII_TXD0 ---------------------> PG13
          RMII_MII_TXD1 ---------------------> PG14
    */
    GPIO_Config_T gpioConfig;
    GPIO_ConfigStructInit(&gpioConfig);

    RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_GPIOA | RCM_AHB1_PERIPH_GPIOG | \
                            RCM_AHB1_PERIPH_GPIOC | RCM_AHB1_PERIPH_GPIOD);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);

    gpioConfig.otype = GPIO_OTYPE_PP;
    gpioConfig.mode = GPIO_MODE_AF;
    gpioConfig.pupd = GPIO_PUPD_NOPULL;
    gpioConfig.speed = GPIO_SPEED_100MHz;

    /* Configure ETH_RESET# */
    gpioConfig.pin = GPIO_PIN_1;
    GPIO_Config(GPIOA,&gpioConfig);
    GPIO_ConfigPinAF(GPIOA,GPIO_PIN_SOURCE_1,GPIO_AF_ETH);

    /* Configure ETH_MDIO */
    gpioConfig.pin = GPIO_PIN_2;
    GPIO_Config(GPIOA,&gpioConfig);
    GPIO_ConfigPinAF(GPIOA,GPIO_PIN_SOURCE_2,GPIO_AF_ETH);

    /* Configure ETH_MDC */
    gpioConfig.pin = GPIO_PIN_1;
    GPIO_Config(GPIOC,&gpioConfig);
    GPIO_ConfigPinAF(GPIOC,GPIO_PIN_SOURCE_1,GPIO_AF_ETH);

    /* Configure ETH_RMII_CRS_DV */
    gpioConfig.pin = GPIO_PIN_7;
    GPIO_Config(GPIOA,&gpioConfig);
    GPIO_ConfigPinAF(GPIOA,GPIO_PIN_SOURCE_7,GPIO_AF_ETH);

    /* Configure ETH_RMII_RXD0 */
    gpioConfig.pin = GPIO_PIN_4;
    GPIO_Config(GPIOC,&gpioConfig);
    GPIO_ConfigPinAF(GPIOC,GPIO_PIN_SOURCE_4,GPIO_AF_ETH);

    /* Configure ETH_RMII_RXD1 */
    gpioConfig.pin = GPIO_PIN_5;
    GPIO_Config(GPIOC,&gpioConfig);
    GPIO_ConfigPinAF(GPIOC,GPIO_PIN_SOURCE_5,GPIO_AF_ETH);

    /* Configure ETH_RMII_TX_EN */
    gpioConfig.pin = GPIO_PIN_11;
    GPIO_Config(GPIOG,&gpioConfig);
    GPIO_ConfigPinAF(GPIOG,GPIO_PIN_SOURCE_11,GPIO_AF_ETH);

    /* Configure ETH_RMII_TXD0 */
    gpioConfig.pin = GPIO_PIN_13;
    GPIO_Config(GPIOG,&gpioConfig);
    GPIO_ConfigPinAF(GPIOG,GPIO_PIN_SOURCE_13,GPIO_AF_ETH);

    /* Configure ETH_RMII_TXD1 */
    gpioConfig.pin = GPIO_PIN_14;
    GPIO_Config(GPIOG,&gpioConfig);
    GPIO_ConfigPinAF(GPIOG,GPIO_PIN_SOURCE_14,GPIO_AF_ETH);

    SYSCFG_ConfigMediaInterface(SYSCFG_INTERFACE_RMII);

    NVIC_EnableIRQRequest(ETH_IRQn,0x7,0);
}

/*!
 * @brief  configurate Ethernet Interface.
 *
 * @param  None
 *
 * @retval None
 *
 * @note
 */
void ConfigEthernetMACDMA(void)
{
    /* Enable ETHERNET clock  */
    RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_ETH_MAC | RCM_AHB1_PERIPH_ETH_MAC_Tx |
                              RCM_AHB1_PERIPH_ETH_MAC_Rx);

    /* Reset ETH on AHB Bus */
    ETH_Reset();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    while (ETH_ReadSoftwareReset() == SET);
    /* ETH Config Struct */
    ETH_ConfigStructInit(&ETH_InitStructure);

    /* ETH Config MAC */
    ETH_InitStructure.autoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    ETH_InitStructure.loopbackMode = ETH_LOOPBACKMODE_DISABLE;
    ETH_InitStructure.retryTransmission = ETH_RETRYTRANSMISSION_DISABLE;
    ETH_InitStructure.automaticPadCRCStrip = ETH_AUTOMATICPADCRCSTRIP_DISABLE;
    ETH_InitStructure.receiveAll = ETH_RECEIVEAll_DISABLE;
    ETH_InitStructure.broadcastFramesReception = ETH_BROADCASTFRAMESRECEPTION_ENABLE;
    ETH_InitStructure.promiscuousMode = ETH_PROMISCUOUS_MODE_DISABLE;
    ETH_InitStructure.multicastFramesFilter = ETH_MULTICASTFRAMESFILTER_NONE;
    ETH_InitStructure.unicastFramesFilter = ETH_UNICASTFRAMESFILTER_PERFECT;
#if (_ETH_HARDWARE_CHECKSUM == 1)
    ETH_InitStructure.checksumOffload = ETH_CHECKSUMOFFLAOD_ENABLE;
#endif

    /* ETH Config DMA */
    ETH_InitStructure.dropTCPIPChecksumErrorFrame = ETH_DROPTCPIPCHECKSUMERRORFRAME_ENABLE;
    ETH_InitStructure.receiveStoreForward = ETH_RECEIVESTOREFORWARD_ENABLE;
    ETH_InitStructure.flushReceivedFrame = ETH_FLUSHRECEIVEDFRAME_ENABLE;
    ETH_InitStructure.transmitStoreForward = ETH_TRANSMITSTOREFORWARD_ENABLE;

    ETH_InitStructure.forwardErrorFrames = ETH_FORWARDERRORFRAMES_DISABLE;
    ETH_InitStructure.forwardUndersizedGoodFrames = ETH_FORWARDUNDERSIZEDGOODFRAMES_DISABLE;
    ETH_InitStructure.secondFrameOperate = ETH_SECONDFRAMEOPERARTE_ENABLE;
    ETH_InitStructure.addressAlignedBeats = ETH_ADDRESSALIGNEDBEATS_ENABLE;
    ETH_InitStructure.fixedBurst = ETH_FIXEDBURST_ENABLE;
    ETH_InitStructure.rxDMABurstLength = ETH_RXDMABURSTLENGTH_32BEAT;
    ETH_InitStructure.txDMABurstLength = ETH_TXDMABURSTLENGTH_32BEAT;
    ETH_InitStructure.DMAArbitration = ETH_DMAARBITRATION_ROUNDROBIN_RXTX_1_1;

    /* Configure Ethernet */
    ETH_Config(&ETH_InitStructure, _ETH_BOARD_PHY_ADDRESS);
}


/*!
 * @brief  ETH_IRQHandler
 *
 * @param  None
 *
 * @retval None
 *
 * @note
 */
void ETH_IRQHandler(void)
{
    /* frame received */
    if (ETH_ReadDMAIntFlag(ETH_DMA_INT_RX) != RESET)
    {
        /* clear the enet DMA Rx interrupt pending bits */
        ETH_ClearDMAIntFlag(ETH_DMA_INT_RX);
        ETH_ClearDMAIntFlag(ETH_DMA_INT_NIS);
        /* Ethernet Rx Transfer completed callback */
        APM32_EMAC &emac = APM32_EMAC::get_instance();
        if (emac.rx_thread)
        {
            osThreadFlagsSet(emac.rx_thread, _ETH_FLAG_RX);
        }
    }
}

APM32_EMAC::APM32_EMAC()
    : rx_thread(0),
      phy_status(0)
{
}

static osThreadId_t create_new_thread(const char *threadName, void (*thread)(void *arg), void *arg, int stacksize, osPriority_t priority, mbed_rtos_storage_thread_t *thread_cb)
{
    osThreadAttr_t attr = {0};
    attr.name = threadName;
    attr.stack_mem  = malloc(stacksize);
    attr.cb_mem  = thread_cb;
    attr.stack_size = stacksize;
    attr.cb_size = sizeof(mbed_rtos_storage_thread_t);
    attr.priority = priority;
    return osThreadNew(thread, arg, &attr);
}

/*!
 * @brief  Low level init of the MAC and PHY.
 *
 * @param  count  Reset Delay Time
 *
 * @retval True if low_level_init success , otherwise false.
 *
 * @note
 */
bool APM32_EMAC::low_level_init()
{
    /* Init ETH */
    uint8_t macaddr[6];
    uint32_t i;
    ETH_Config_T ETH_InitStructure;

#if (MBED_MAC_ADDRESS_SUM != MBED_MAC_ADDR_INTERFACE)
    MACAddr[0] = MBED_MAC_ADDR_0;
    MACAddr[1] = MBED_MAC_ADDR_1;
    MACAddr[2] = MBED_MAC_ADDR_2;
    MACAddr[3] = MBED_MAC_ADDR_3;
    MACAddr[4] = MBED_MAC_ADDR_4;
    MACAddr[5] = MBED_MAC_ADDR_5;
#else
    mbed_mac_address((char *)macaddr);
#endif

    ConfigEthernetGPIO();

    ConfigEthernetMACDMA();

    /* initialize MAC address in ethernet MAC */
    ETH_ConfigMACAddress(ETH_MAC_ADDRESS0, macaddr);
    ETH_EnableDMAInterrupt(ETH_DMA_INT_NIS | ETH_DMA_INT_RX);

    /* Initialize Tx Descriptors list: Chain Mode */
    ETH_ConfigDMATxDescChain(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
    /* Initialize Rx Descriptors list: Chain Mode  */
    ETH_ConfigDMARxDescChain(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

    /* Enable Ethernet Rx interrrupt */
    for(i = 0; i < ETH_RXBUFNB; i++)
    {
        ETH_EnableDMARxDescReceiveInterrupt(&DMARxDscrTab[i]);
    }

#if (_ETH_HARDWARE_CHECKSUM == 1)
    /* Enable the checksum insertion for the Tx frames */
    for(i = 0; i < ETH_TXBUFNB; i++)
    {
        ETH_ConfigDMATxDescChecksumInsertion(&DMATxDscrTab[i], ETH_DMATXDESC_CHECKSUMTCPUDPICMPFULL);
    }
#endif

    /* Enable MAC and DMA transmission and reception */
    ETH_Start();

    return true;
}

/*!
 * @brief  Sends the packet over the link
 *
 *         That can not be called from an interrupt context.
 *
 * @param  buf  Packet to be send
 *
 * @retval True if the packet was send successfully, False otherwise
 *
 */
bool APM32_EMAC::link_out(emac_mem_buf_t *buf)
{
    emac_mem_buf_t *q;
    uint8_t *buffer;
    uint16_t framelength = 0;

    /* Get exclusive access */
    TXLockMutex.lock();

    /* copy frame from pbufs to driver buffers */
    buffer = reinterpret_cast<uint8_t *>(DMATxDescToSet->Buffer1Addr);

    for (q = buf; q != NULL; q = memory_manager->get_next(q))
    {
        memcpy(static_cast<uint8_t *>(&buffer[framelength]),\
               static_cast<uint8_t *>(memory_manager->get_ptr(q)), memory_manager->get_len(q));
        framelength = framelength + memory_manager->get_len(q);
    }

    /* Prepare transmit descriptors to give to DMA */
    ETH_TxPacket(framelength);

    memory_manager->free(buf);
    /* Restore access */
    TXLockMutex.unlock();

    return true;
}

/*!
 * @brief  Should allocate a contiguous memory buffer and transfer the bytes of the incoming
 *         packet to the buffer.
 *
 * @param  buf     If a frame was received and the memory buffer allocation was successful, a memory
 *                 buffer filled with the received packet (including MAC header)
 *
 * @retval negative value when no more frames,zero when frame is received
 */
emac_mem_buf_t *APM32_EMAC::low_level_input(void)
{
    emac_mem_buf_t *p = NULL, *q;
    unsigned short len;
    int l =0;
    ETH_Frame_T frame;
    u8 *buffer;

    frame = ETH_RxPacket();

    len = frame.length;

    buffer = (u8 *)frame.buffer;

    if (len > 0) 
    {
        /* Allocate a memory buffer chain from buffer pool */
        p = memory_manager->alloc_pool(len, 0);
    }
    else
    {
        return p;
    }

    if (p != NULL)
    {
        for (q = p; q != NULL; q = memory_manager->get_next(q))
        {
            memcpy(static_cast<uint8_t *>(memory_manager->get_ptr(q)),\
                   static_cast<uint8_t *>(&buffer[l]), memory_manager->get_len(q));
            l = l + memory_manager->get_len(q);
        }
    }
    frame.descriptor->Status = ETH_DMARXDESC_OWN;

      /** When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((ETH->DMASTS & BIT7) != (u32)RESET)
    {
        /** Clear RBUS ETHERNET DMA flag */
        ETH->DMASTS = BIT7;
        /** Resume DMA reception */
        ETH->DMARXPD = 0;
    }

    return p;
}


/*!
 * @brief  Attempt to read a packet from the EMAC interface.
 *
 * @param  None
 *
 * @retval None
 *
 * @note
 */
void APM32_EMAC::packet_rx()
{
    /* move received packet into a new buf */
    while (1)
    {
        emac_mem_buf_t *p = NULL;
        p = low_level_input();

        if (p)
        {
            emac_link_input_cb(p);
        }
        else
        {
            break;
        }
    }
}

/*!
 * @brief  Worker thread.
 *         Woken by thread flags to receive packets or clean up transmit
 *
 * @param  pvParameters pointer to the interface data
 *
 * @retval None
 *
 * @note
 */
void APM32_EMAC::thread_function(void *pvParameters)
{
    static struct APM32_EMAC *eint = static_cast<APM32_EMAC *>(pvParameters);

    while (1)
    {
        uint32_t flags = osThreadFlagsWait(_ETH_FLAG_RX, osFlagsWaitAny, osWaitForever);

        if (flags & _ETH_FLAG_RX)
        {
            eint->packet_rx();
        }
    }
}

/*!
 * @brief  This task checks phy link status and updates net status
 *
 * @param  None
 *
 * @retval None
 */
void APM32_EMAC::phy_task()
{
    uint16_t regval;

    regval = ETH_ReadPHYRegister(_ETH_BOARD_PHY_ADDRESS,PHY_BSR);
    if (emac_link_state_cb)
    {
        regval &= PHY_LINKED_STATUS;

        if (phy_status != regval)
        {
            if (regval == PHY_LINKED_STATUS)
            {
                emac_link_state_cb(true);;
            }
            else
            {
                emac_link_state_cb(false);
            }
        }
    }

    phy_status = regval;
}

/*!
 * @brief  Enable Interrupt.
 *
 * @param  None
 *
 * @retval None
 */
void APM32_EMAC::enable_interrupts(void)
{
    NVIC_EnableIRQRequest(ETH_IRQn,0x7,0);
}

void APM32_EMAC::disable_interrupts(void)
{
    NVIC_DisableIRQRequest(ETH_IRQn);
}

/*!
 * @brief  Disable Interrupt.
 *
 * @param  None
 *
 * @retval None
 */
void mbed_mac_address(char *mac)
{
    uint32_t unique_id;

    unique_id = *(uint32_t *)_APM32_ID_ADDR;
    mac[0] = _APM_MAC_ADDR0;
    mac[1] = _APM_MAC_ADDR1;
    mac[2] = _APM_MAC_ADDR2;
    mac[3] = (unique_id & 0x00ff0000) >> 16;
    mac[4] = (unique_id & 0x0000ff00) >> 8;
    mac[5] = (unique_id & 0x000000ff);
}

/*!
 * @brief  Initializes the HW
 *
 * @param  None
 *
 * @retval True on success, False in case of an error.
 */
bool APM32_EMAC::power_up()
{
    /* Initialize the hardware */
    if (low_level_init() == false)
    {
        return false;
    }

    /* Worker thread */
    rx_thread = create_new_thread("apm32_emac_thread", &APM32_EMAC::thread_function, \
                                  this, _THREAD_STACKSIZE, _THREAD_PRIORITY, &rx_thread_cb);

    phy_task_handle = mbed::mbed_event_queue()->call_every(_PHY_TASK_PERIOD, mbed::callback(this, &APM32_EMAC::phy_task));

    /* Allow the PHY task to detect the initial link state and set up the proper flags */
    osDelay(10);

    enable_interrupts();

    return true;
}

/*!
 * @brief  Return maximum transmission unit
 *
 * @param  None
 *
 * @retval MTU in bytes
 */
uint32_t APM32_EMAC::get_mtu_size() const
{
    return _ETH_MTU_SIZE;
}

/*!
 * @brief  Gets memory buffer alignment preference
 *
 * Gets preferred memory buffer alignment of the Emac device. IP stack may or may not
 * align link out memory buffer chains using the alignment.
 *
 * @param  None
 *
 * @retval Memory alignment requirement in bytes
 */
uint32_t APM32_EMAC::get_align_preference() const
{
    return 0;
}

/*!
 * @brief  Return interface name
 *
 * Gets preferred memory buffer alignment of the Emac device. IP stack may or may not
 * align link out memory buffer chains using the alignment.
 *
 * @param name Pointer to where the name should be written
 *
 * @param size Maximum number of character to copy
 *
 * @retval None
 */
void APM32_EMAC::get_ifname(char *name, uint8_t size) const
{
    memcpy(name, _ETH_IF_NAME, (size < sizeof(_ETH_IF_NAME)) ? size : sizeof(_ETH_IF_NAME));
}

/*!
 * @brief  Returns size of the underlying interface HW address size.
 *
 * Gets preferred memory buffer alignment of the Emac device. IP stack may or may not
 * align link out memory buffer chains using the alignment.
 *
 * @param  None
 *
 * @retval HW address size in bytes
 */
uint8_t APM32_EMAC::get_hwaddr_size() const
{
    return _ETH_HW_ADDR_SIZE;
}

/*!
 * @brief  Returns size of the underlying interface HW address size.
 *
 * Gets preferred memory buffer alignment of the Emac device. IP stack may or may not
 * align link out memory buffer chains using the alignment.
 *
 * @param  None
 *
 * @retval HW address size in bytes
 */
bool APM32_EMAC::get_hwaddr(uint8_t *addr) const
{
    mbed_mac_address((char *)addr);
    return true;
}

/*!
 * @brief  Set HW address for interface
 *
 *         Provided address has to be of correct size, see @a get_hwaddr_size
 *
 * @param  addr Address to be set
 *
 * @retval None
 */
void APM32_EMAC::set_hwaddr(const uint8_t *addr)
{
    /* No-op at this stage */
}

/*!
 * @brief  Sets a callback that needs to be called for packets received for that interface
 *
 * @param  input_cb Function to be register as a callback
 *
 * @retval None
 */
void APM32_EMAC::set_link_input_cb(emac_link_input_cb_t input_cb)
{
    emac_link_input_cb = input_cb;
}

/*!
 * @brief  Sets a callback that needs to be called on link status changes for given interface
 *
 * @param  state_cb Function to be register as a callback
 *
 * @retval None
 */
void APM32_EMAC::set_link_state_cb(emac_link_state_change_cb_t state_cb)
{
    emac_link_state_cb = state_cb;
}

/*!
 * @brief  Add device to a multicast group
 *
 * @param  addr  A multicast group hardware address
 *
 * @retval None
 */
void APM32_EMAC::add_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

/*!
 * @brief  Remove device from a multicast group
 *
 * @param  addr  A multicast group hardware address
 *
 * @retval None
 */
void APM32_EMAC::remove_multicast_group(const uint8_t *addr)
{
    /* No-op at this stage */
}

/*!
 * @brief  Request reception of all multicast packets
 *
 * @param  all True to receive all multicasts
 *             False to receive only multicasts addressed to specified groups
 *
 * @retval None
 */
void APM32_EMAC::set_all_multicast(bool all)
{
    /* No-op at this stage */
}

/*!
 * @brief  Deinitializes the HW
 *
 * @param  None
 *
 * @retval None
 */
void APM32_EMAC::power_down()
{
    /* No-op at this stage */
}

/*!
 * @brief  Sets memory manager that is used to handle memory buffers
 *
 * @param  mem_mngr Pointer to memory manager
 *
 * @retval None
 */
void APM32_EMAC::set_memory_manager(EMACMemoryManager &mem_mngr)
{
    memory_manager = &mem_mngr;
}

/*!
 * @brief  get instance
 *
 * @param  None
 *
 * @retval None
 */
APM32_EMAC &APM32_EMAC::get_instance()
{
    static APM32_EMAC emac;
    return emac;
}

/* Weak so a module can override */
MBED_WEAK EMAC &EMAC::get_default_instance()
{
    return APM32_EMAC::get_instance();
}
