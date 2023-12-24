/* mbed Microcontroller Library
 * Copyright (c) 2018-2023 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) 2018-2023 Geehy Semiconductor
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

#if DEVICE_USBDEVICE

#include "USBPhyHw.h"
#include "pinmap.h"

/* endpoint conversion macros */
#define ENDPOINT_TO_LOG(endpoint)     ((endpoint) & 0xF)
#define LOGIN_TO_ENDPOINT(log)        ((log) | 0x80)
#define LOGOUT_TO_ENDPOINT(log)       ((log) | 0x00)
#define ENDPOINT_TO_INDEX(endpoint)   ((ENDPOINT_TO_LOG(endpoint) << 1) | ((endpoint) & 0x80 ? 1 : 0))
#define INDEX_TO_ENDPOINT(index)      (((index) >> 1)|((index) & 1) << 7)

/* endpoint defines */

#if (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_HS)

#define NUM_ENDPOINTS                6
#define MAX_PACKET_SIZE_NON_ISO      512
#define MAX_PACKET_SIZE_ISO          1023

#else

#define NUM_ENDPOINTS                4
#define MAX_PACKET_SIZE_NON_ISO      64
#define MAX_PACKET_SIZE_ISO          (256 + 128)     // Spec can go up to 1023, only ram for this though

#endif

static const uint32_t tx_ep_sizes[NUM_ENDPOINTS] = 
{
    MAX_PACKET_SIZE_NON_ISO,
    MAX_PACKET_SIZE_NON_ISO,
    MAX_PACKET_SIZE_NON_ISO,
#if (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_HS)
    MAX_PACKET_SIZE_NON_ISO,
    MAX_PACKET_SIZE_NON_ISO,
#endif
    MAX_PACKET_SIZE_ISO
};

/*!
 * @brief     USB device reads tx fifo cache.
 *
 * @param     usb    The pointer of USBD_HANDLE_T
 *
 * @param     fifo   Rx buffer
 *
 * @retval    Length of usb cache array
 *
 */
uint32_t USBD_ReadTxFiFo(USBD_HANDLE_T *usb, uint8_t fifo)
{
    uint32_t length;
    if (fifo == 0)
    {
        length = usb->usbGlobal->GTXFCFG >> 16;
    }
    else
    {
        length = usb->usbGlobal->DTXFIFO[fifo - 1].bit.INEPTXFDEP;
    }
    return length * 4;
}

/*!
 * @brief     Configure the size of the USB device's send buffer.
 *
 * @param     usbx    The pointer of USB_OTG_GLOBAL_T
 *
 * @param     fifo    Tx buffer
 *
 * @param     size    Tx buffer size
 *
 * @retval    Length of usb cache array
 *
 */
void USB_OTG_ConfigTxFifoSize(USB_OTG_GLOBAL_T* usbx, uint8_t fifo ,uint16_t size)
{
    uint8_t i;
    uint32_t txOffset;

    txOffset = usbx->GRXFIFO;

    if (fifo == 0)
    {
        usbx->GTXFCFG = ((uint32_t)size << 16) | txOffset;
    }
    else
    {
        txOffset += usbx->GTXFCFG >> 16;
        for (i = 0; i < (fifo - 1U); i++)
        {
            txOffset += usbx->DTXFIFO[i].bit.INEPTXFDEP;
        }
    }

    usbx->DTXFIFO[fifo - 1].word = (((uint32_t)size << 16) | txOffset);
}

/*!
 * @brief     USB OTG device resume callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_ResumeCallback(USBD_HANDLE_T* usbdh)
{
    USBPhyHw *obj = ((USBPhyHw *)(usbdh->dataPoint));
    obj->events->suspend(0);
}

/*!
 * @brief     USB OTG device suspend callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_SuspendCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
    USBPhyHw *obj = ((USBPhyHw *)(usbdh->dataPoint));
    obj->events->suspend(1);
}

/*!
 * @brief     USB OTG device enum done callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_EnumDoneCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
    USBPhyHw *priv = ((USBPhyHw *)(usbdh->dataPoint));
    unsigned int i;
    for (i = 0; i < NB_ENDPOINT; i++) 
    {
        priv->epComplete[2 * i] = 0;
        USBD_EP_Close(usbdh, INDEX_TO_ENDPOINT(2 * i));
        USBD_EP_Flush(usbdh, INDEX_TO_ENDPOINT(2 * i));
        priv->epComplete[2 * i + 1] = 0;
        USBD_EP_Close(usbdh, INDEX_TO_ENDPOINT(2 * i + 1));
        USBD_EP_Flush(usbdh, INDEX_TO_ENDPOINT(2 * i + 1));
    }
    priv->endpoint_add(0x80, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
    priv->endpoint_add(0x00, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
    priv->events->reset();
}

/*!
 * @brief     USB OTG device SETUP stage callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_SetupStageCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
    USBPhyHw *obj = ((USBPhyHw *)(usbdh->dataPoint));
    obj->events->ep0_setup();
}

/*!
 * @brief     USB OTG device data IN stage callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
void USBD_DataInStageCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
    USBPhyHw *obj = ((USBPhyHw *)(usbdh->dataPoint));
    uint8_t endpoint = LOGIN_TO_ENDPOINT(epNum);
    obj->epComplete[ENDPOINT_TO_INDEX(endpoint)] = 1;

    if (epNum)
    {
        obj->events->in(endpoint);
    }
    else
    {
        obj->events->ep0_in();
    }
}

/*!
 * @brief     USB OTG device data OUT stage callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
void USBD_DataOutStageCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
    USBPhyHw *obj = ((USBPhyHw *)(usbdh->dataPoint));
    uint8_t endpoint = LOGOUT_TO_ENDPOINT(epNum);
    obj->epComplete[ENDPOINT_TO_INDEX(endpoint)] = 1;

    if (epNum)
    {
        obj->events->out(endpoint);
    }
    else
    {
        obj->events->ep0_out();
    }
}

/*!
 * @brief       USB OTG device SOF event callback function
 *
 * @param       usbhh: USB host handler.
 *
 * @retval      None
 */
void USBD_SOFCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
    USBPhyHw *obj = ((USBPhyHw *)(usbdh->dataPoint));
    if (obj->sof_enabled)
    {
        obj->events->sof(USB_OTG_FS_D->DSTS_B.SOFNUM);
    }
}

/*!
 * @brief     USB OTG device ISO IN in complete callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
void USBD_IsoInInCompleteCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device ISO OUT in complete callback
 *
 * @param     usbdh: USB device handler
 *
 * @param     epNum: endpoint number
 *
 * @retval    None
 */
void USBD_IsoOutInCompleteCallback(USBD_HANDLE_T* usbdh, uint8_t epNum)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device connect callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_ConnectCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

/*!
 * @brief     USB OTG device disconnect callback
 *
 * @param     usbdh: USB device handler
 *
 * @retval    None
 */
void USBD_DisconnectCallback(USBD_HANDLE_T* usbdh)
{
    /* callback interface */
}

static USBPhyHw *instance;

USBPhy *get_usb_phy()
{
    static USBPhyHw usbphy;
    return &usbphy;
}

USBPhyHw::USBPhyHw(): events(NULL), sof_enabled(false)
{

}

USBPhyHw::~USBPhyHw()
{

}

/*!
 * @brief     USB Init
 *
 * @param     events    USBPhyEvents
 *
 * @retval    None
 *
 */
void USBPhyHw::init(USBPhyEvents *events)
{
    const PinMap *pinmap = NULL;

    NVIC_DisableIRQ(USB_OTG_IRQn);

    if (this->events == NULL)
    {
        sleep_manager_lock_deep_sleep();
    }
    this->events = events;
    sof_enabled = false;
    memset(epComplete, 0, sizeof(epComplete));

#if (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_FS)
    memset(&usbdh.usbCfg,0,sizeof(usbdh.usbCfg));

    usbdh.usbCfg.devEndpointNum = 8;
    usbdh.usbCfg.ep0MaxPackSize = USDB_EP0_MAXPS_64_BYTES;
    usbdh.usbCfg.lowPowerStatus = DISABLE;
    usbdh.usbCfg.batteryStatus = DISABLE;
    usbdh.usbCfg.powerManageStatus = DISABLE;
    usbdh.usbCfg.speedChannel = USBD_SPEED_CH_FS;
    usbdh.usbCfg.mode = USB_OTG_MODE_DEVICE;

    usbdh.usbCfg.phyType = USB_OTG_PHY_EMB;
    usbdh.usbCfg.sofStatus = ENABLE;
    usbdh.usbCfg.speed = USB_OTG_SPEED_FSLS;

    RCM_EnableAHB2PeriphClock(RCM_AHB2_PERIPH_OTG_FS);

    pinmap = PinMap_USB_FS;

#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_HS)
    memset(&usbdh.usbCfg,0,sizeof(usbdh.usbCfg));

    usbdh.usbCfg.devEndpointNum = 8;
    usbdh.usbCfg.ep0MaxPackSize = USDB_EP0_MAXPS_64_BYTES;
    usbdh.usbCfg.lowPowerStatus = DISABLE;
    usbdh.usbCfg.batteryStatus = DISABLE;
    usbdh.usbCfg.powerManageStatus = DISABLE;
    usbdh.usbCfg.mode = USB_OTG_MODE_DEVICE;
    usbdh.usbCfg.speedChannel = USBD_SPEED_CH_HS;

    usbdh.usbCfg.phyType = USB_OTG_PHY_EX;
    usbdh.usbCfg.sofStatus = ENABLE;
    usbdh.usbCfg.dmaStatus = DISABLE;
    usbdh.usbCfg.vbusSense = ENABLE;
    usbdh.usbCfg.extVbusStatus = DISABLE;
    usbdh.usbCfg.speed = USB_OTG_SPEED_HSFSLS;

    RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_OTG_HS);
    RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_OTG_HS_ULPI);

    pinmap = PinMap_USB_HS;

#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_HS_IN_FS)
    memset(&usbdh.usbCfg,0,sizeof(usbdh.usbCfg));

    usbdh.usbCfg.devEndpointNum = 8;
    usbdh.usbCfg.ep0MaxPackSize = USDB_EP0_MAXPS_64_BYTES;
    usbdh.usbCfg.lowPowerStatus = DISABLE;
    usbdh.usbCfg.batteryStatus = DISABLE;
    usbdh.usbCfg.powerManageStatus = DISABLE;
    usbdh.usbCfg.mode = USB_OTG_MODE_DEVICE;
    usbdh.usbCfg.speedChannel = USBD_SPEED_CH_FS;

    usbdh.usbCfg.phyType = USB_OTG_PHY_EMB;
    usbdh.usbCfg.sofStatus = ENABLE;
    usbdh.usbCfg.dmaStatus = DISABLE;
    usbdh.usbCfg.vbusSense = DISABLE;
    usbdh.usbCfg.extVbusStatus = DISABLE;
    usbdh.usbCfg.speed = USB_OTG_SPEED_FSLS;

    RCM_EnableAHB1PeriphClock(RCM_AHB1_PERIPH_OTG_HS);

    pinmap = PinMap_USB_HS;

#endif

    // Pass instance for usage inside call back
    instance = this;

    // Configure USB pins
    while (pinmap->pin != NC)
    {
        pin_function(pinmap->pin, pinmap->function);
        pinmap++;
    }
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_PMU);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_SYSCFG);

    // Configure PCD and FIFOs
#if (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_FS)
    usbdh.dataPoint = (void *)this;
    usbdh.usbGlobal = USB_OTG_FS;
    usbdh.usbDevice = USB_OTG_FS_D;
    usbdh.usbFifo = USB_OTG_FS_FIFO;
    usbdh.usbPower = USB_OTG_FS_PWR;
    USBD_Config(&usbdh);
#else
    usbdh.dataPoint = (void *)this;
    usbdh.usbGlobal = USB_OTG_HS;
    usbdh.usbDevice = USB_OTG_HS_D;
    usbdh.usbPower = USB_OTG_HS_PWR;
    usbdh.usbFifo = USB_OTG_HS_FIFO;

    USBD_Config(&usbdh);
#endif
    USBD_Start(&usbdh);

    uint32_t sum_bytes = 0;

    /* Reserve space in the RX buffer for:
     * - 1 isochonous packet
     * - 2 max sized non-isochonous packets
     * - setup buffer - 10 words as specified by Reference Manual
     * - global nak out - 1 words as specified by Reference Manual
     */
    uint32_t fifo_buffer = (MAX_PACKET_SIZE_ISO + 4) + (MAX_PACKET_SIZE_NON_ISO + 4) * 2 + (10 * 4) + (1 * 4);
    USB_OTG_ConfigRxFifoSize(usbdh.usbGlobal, (fifo_buffer / 4));
    sum_bytes += fifo_buffer;

    /* Reserve Tx space up front */
    for (int i = 0; i < NUM_ENDPOINTS; i++)
    {
        fifo_buffer =  tx_ep_sizes[i] + 4;
        USB_OTG_ConfigTxFifoSize(usbdh.usbGlobal, i, fifo_buffer / 4);
        sum_bytes += fifo_buffer;
    }

#if (MBED_CONF_TARGET_USB_SPEED != USE_USB_OTG_HS)
    /* 1.25 kbytes */
    MBED_ASSERT(sum_bytes <= 1280);
#endif

    // Configure interrupt vector
    NVIC_SetVector(USB_OTG_IRQn, (uint32_t)&_usbisr);
    NVIC_SetPriority(USB_OTG_IRQn, 1);
    NVIC_EnableIRQ(USB_OTG_IRQn);
}

/*!
 * @brief     USB deinit
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::deinit()
{
    USB_OTG_CoreReset(usbdh.usbGlobal);

    NVIC_DisableIRQ(USB_OTG_IRQn);

    if (events != NULL)
    {
        sleep_manager_unlock_deep_sleep();
    }
    events = NULL;
}

/*!
 * @brief     USB device powered
 *
 * @param     None
 *
 * @retval    True is the usb is powered, False otherwise.
 *
 */
bool USBPhyHw::powered()
{
    return true;
}

/*!
 * @brief     Connect usb.
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::connect()
{
    USBD_Start(&usbdh);
    wait_us(10000);
}

/*!
 * @brief     Disconnect usb.
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::disconnect()
{
    /* Disable DP Pull-Up bit to disconnect the Internal PU resistor on USB DP line */
    usbdh.usbPower->PCGCTRL_B.GCLK = 0;
    usbdh.usbPower->PCGCTRL_B.PCLKSTOP = 0;
    usbdh.usbDevice->DCTRL_B.SDCNNT = 1;

    wait_us(10000);

    USBD_Stop(&usbdh);
}

void USBPhyHw::configure()
{
    // Not needed
}

void USBPhyHw::unconfigure()
{
    // Not needed
}

/*!
 * @brief     Enable USB Software.
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::sof_enable()
{
    sof_enabled = true;
}

/*!
 * @brief     Disabl  USB Software.
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::sof_disable()
{
    sof_enabled = false;
}

/*!
 * @brief     Config USB device's address.
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::set_address(uint8_t address)
{
    USBD_SetDevAddress(&usbdh,address);
}

void USBPhyHw::remote_wakeup()
{

}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t table = {
#if (MBED_CONF_TARGET_USB_SPEED != USE_USB_OTG_HS)
        1280, // 1.25K for endpoint buffers but space is allocated up front
#else
        4096,
#endif
        {
            {USB_EP_ATTR_ALLOW_CTRL                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0}, // NON ISO
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0}, // NON ISO
#if (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_HS)
            {USB_EP_ATTR_ALLOW_ALL                          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
#endif
            {USB_EP_ATTR_ALLOW_ALL                          | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
#if (MBED_CONF_TARGET_USB_SPEED != USE_USB_OTG_HS)
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0}
#endif
        }
    };
    return &table;
}

/*!
 * @brief     Config max packet of ep0
 *
 * @param     max_packet    The max packet size.
 *
 * @retval    uint32_t      MAX_PACKET_SIZE_EP0
 *
 */
uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    // FUTURE - set endpoint 0 size and return this size
    return MAX_PACKET_SIZE_EP0;
}

/*!
 * @brief     read setup packet
 *
 * @param     buffer    Store the read data.
 *
 * @param     size      The size of setup packet.
 *
 * @retval    None
 *
 */
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    if (size > MAX_PACKET_SIZE_SETUP)
    {
        size = MAX_PACKET_SIZE_SETUP;
    }
    memcpy(buffer, usbdh.setup, size);
    memset(usbdh.setup, 0, MAX_PACKET_SIZE_SETUP);
}

/*!
 * @brief     read ep0
 *
 * @param     data    ep0 data
 *
 * @param     size    The size of data.
 *
 * @retval    None
 *
 */
void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    epComplete[ENDPOINT_TO_INDEX(0x00)] = 2;
    USBD_EP_Receive(&usbdh,0x00,data,size > MAX_PACKET_SIZE_EP0 ? MAX_PACKET_SIZE_EP0 : size );
}

/*!
 * @brief     read ep0
 *
 * @param     None
 *
 * @retval    Data length of ep0
 *
 */
uint32_t USBPhyHw::ep0_read_result()
{
    epComplete[ENDPOINT_TO_INDEX(0x00)] = 0;
    return USBD_EP_ReadRxDataLen(&usbdh, 0);
}

/*!
 * @brief     write ep0
 *
 * @param     buffer    ep0 data
 *
 * @param     size    The size of buffer.
 *
 * @retval    None
 *
 */
void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    /*  check that endpoint maximum size is not exceeding TX fifo */
    if(usbdh.epIN[0].mps >= size)
    {
        endpoint_write(0x80, buffer, size);
    }
}

/*!
 * @brief     stall ep0
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::ep0_stall()
{
    endpoint_stall(0x80);
    endpoint_stall(0x00);
}

/*!
 * @brief     add endpoint
 *
 * @param     endpoint      Endpoints that need to be added to USB.
 *
 * @param     max_packet    Max packet
 *
 * @param     type          usb ep type
 *
 * @retval    True if the USB endpoint is seccessfully added, otherwise False.
 *
 */
bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint32_t length;
    /*
     * Endpoints are configured in init since re-configuring
     * fifos when endpoints are added or removed causes tests to fail.
     */
    if (endpoint & 0x80)
    {
        length = USBD_ReadTxFiFo(&usbdh, endpoint & 0x7f);
        MBED_ASSERT(length >= max_packet);
    }
    USBD_EP_Open(&usbdh, endpoint, (USB_EP_TYPE_T)type, max_packet);
    return true;
}

/*!
 * @brief     remove endpoint
 *
 * @param     endpoint      Endpoints that need to be removed to USB.
 *
 * @retval    True if the USB endpoint is seccessfully added, otherwise False.
 *
 */
void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    USBD_EP_Close(&usbdh, endpoint);
}

/*!
 * @brief     stall endpoint
 *
 * @param     endpoint      Endpoints that need to be stalled to USB.
 *
 * @retval    None
 *
 */
void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    USBD_EP_Stall(&usbdh, endpoint);
}

/*!
 * @brief     unstall endpoint
 *
 * @param     endpoint      Endpoints that need to be unstalled to USB.
 *
 * @retval    None
 *
 */
void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    USBD_EP_ClearStall(&usbdh,endpoint);
}

/*!
 * @brief     read endpoint
 *
 * @param     endpoint      Endpoints that need to be read to USB.
 *
 * @param     data          Store the read data.
 *
 * @param     size          The size of reading data.
 *
 * @retval    True if endpoint read successful.
 *
 */
bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    /* clean reception end flag before requesting reception */
    USBD_EP_Receive(&usbdh, endpoint, data, size);
    return true;
}

/*!
 * @brief     read endpoint
 *
 * @param     endpoint      Endpoints that need to be read to USB.
 *
 * @retval    Read FIFO data cache.
 *
 */
uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    if (epComplete[ENDPOINT_TO_INDEX(endpoint)] == 0)
    {
        /*  no reception possible !!! */
        return 0;
    }
    else if ((epComplete[ENDPOINT_TO_INDEX(endpoint)] != 1))
    {
        return 0;
    }
    epComplete[ENDPOINT_TO_INDEX(endpoint)] = 0;
    return USBD_EP_ReadRxDataLen(&usbdh, endpoint);
}

/*!
 * @brief     write endpoint
 *
 * @param     endpoint      Endpoints that need to be writen to USB.
 *
 * @param     data          Data to write
 *
 * @param     size          The size of data.
 *
 * @retval    True if read data successful.
 *
 */
bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    /* clean transmission end flag before requesting transmission */
    epComplete[ENDPOINT_TO_INDEX(endpoint)] = 2;
    USBD_EP_Transfer(&usbdh, endpoint, data, size);
    /* fix me return is too simple */
    return true;
}

/*!
 * @brief     abort endpoint
 *
 * @param     endpoint      Endpoints that need to be aborted to USB.
 *
 * @retval    None
 *
 */
void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
    USBD_EP_Close(&usbdh, endpoint);
}

/*!
 * @brief     USB process
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::process()
{
    USBD_OTG_IsrHandler(&instance->usbdh);
    /* Re-enable interrupt */
    NVIC_ClearPendingIRQ(USB_OTG_IRQn);
    NVIC_EnableIRQ(USB_OTG_IRQn);
}

/*!
 * @brief     USB _usbisr
 *
 * @param     None
 *
 * @retval    None
 *
 */
void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ(USB_OTG_IRQn);
    instance->events->start_process();
}

#endif /* DEVICE_USBDEVICE */
