/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
 * Copyright (c) 2018-2019 STMicroelectronics
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
#define EP_TO_LOG(ep)       ((ep) & 0xF)
#define LOG_IN_TO_EP(log)   ((log) | 0x80)
#define LOG_OUT_TO_EP(log)  ((log) | 0x00)
#define EP_TO_IDX(ep)       ((EP_TO_LOG(ep) << 1) | ((ep) & 0x80 ? 1 : 0))
#define IDX_TO_EP(idx)      (((idx) >> 1)|((idx) & 1) << 7)

/* endpoint defines */
#define NUM_ENDPOINTS           4
#define MAX_PACKET_SIZE_NON_ISO      64
#define MAX_PACKET_SIZE_ISO          (256 + 128)     // Spec can go up to 1023, only ram for this though

static const uint32_t tx_ep_sizes[NUM_ENDPOINTS] = {
    MAX_PACKET_SIZE_NON_ISO,
    MAX_PACKET_SIZE_NON_ISO,
    MAX_PACKET_SIZE_NON_ISO,
    MAX_PACKET_SIZE_ISO
};

uint32_t HAL_PCDEx_GetTxFiFo(PCD_HandleTypeDef *hpcd, uint8_t fifo)
{
    uint32_t len;
    if (fifo == 0) {
        len = hpcd->Instance->DIEPTXF0_HNPTXFSIZ >> 16;
    } else {
        len =  hpcd->Instance->DIEPTXF[fifo - 1] >> 16;
    }
    return len * 4;
}

void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    USB_OTG_GlobalTypeDef *USBx = hpcd->Instance;
    uint32_t USBx_BASE = (uint32_t)USBx;
    if (priv->sof_enabled) {
        priv->events->sof((USBx_DEVICE->DSTS & USB_OTG_DSTS_FNSOF) >> 8);
    }
}

/*  this call at device reception completion on a Out Enpoint  */
/*  weak function redefinition  */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    uint8_t endpoint = LOG_OUT_TO_EP(epnum);
    priv->epComplete[EP_TO_IDX(endpoint)] = 1;

    if (epnum) {
        priv->events->out(endpoint);
    } else {
        priv->events->ep0_out();
    }
}

/*  this is call at device transmission completion on In endpoint */
/*  weak function redefinition  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    uint8_t endpoint = LOG_IN_TO_EP(epnum);
    priv->epComplete[EP_TO_IDX(endpoint)] = 1;

    if (epnum) {
        priv->events->in(endpoint);
    } else {
        priv->events->ep0_in();
    }
}

/*  This is call at device set up reception  */
/*  weak function redefinition  */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    priv->events->ep0_setup();
}

/*  weak function redefinition  */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    priv->events->suspend(1);
}

/*  weak function redefinition  */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    priv->events->suspend(0);
}

/*  weak function redefinition  */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
    // Nothing to do
}

/*  weak function redefinition  */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
    // Nothing to do
}

/*  weak function redefinition  */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{
    USBPhyHw *obj = ((USBPhyHw *)(hpcd->pData));
    unsigned int i;
    for (i = 0; i < NB_ENDPOINT; i++) {
        obj->epComplete[2 * i] = 0;
        HAL_PCD_EP_Close(hpcd, IDX_TO_EP(2 * i));
        HAL_PCD_EP_Flush(hpcd, IDX_TO_EP(2 * i));
        obj->epComplete[2 * i + 1] = 0;
        HAL_PCD_EP_Close(hpcd, IDX_TO_EP(2 * i + 1));
        HAL_PCD_EP_Flush(hpcd, IDX_TO_EP(2 * i + 1));

    }
    obj->endpoint_add(0x80, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
    obj->endpoint_add(0x00, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
    obj->events->reset();
}


/* hal pcd handler , used for STM32 HAL PCD Layer */

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

void USBPhyHw::init(USBPhyEvents *events)
{
    const PinMap *map = NULL;

    NVIC_DisableIRQ(USBHAL_IRQn);

    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }
    this->events = events;
    sof_enabled = false;
    memset(epComplete, 0, sizeof(epComplete));
    memset(&hpcd.Init, 0, sizeof(hpcd.Init));

    hpcd.Init.dev_endpoints = NB_ENDPOINT;
    hpcd.Init.ep0_mps = MAX_PACKET_SIZE_EP0;
    hpcd.Init.low_power_enable = DISABLE;
#if !defined(TARGET_STM32F2)
    hpcd.Init.lpm_enable = DISABLE;
    hpcd.Init.battery_charging_enable = DISABLE;
#endif
#if (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_HS)
    hpcd.Instance = USB_OTG_HS;
    hpcd.Init.phy_itface = PCD_PHY_ULPI;
    hpcd.Init.Sof_enable = 1;
    hpcd.Init.dma_enable = DISABLE;
    hpcd.Init.vbus_sensing_enable = ENABLE;
    hpcd.Init.use_external_vbus = DISABLE;
    hpcd.Init.speed = PCD_SPEED_HIGH;
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
    map = PinMap_USB_HS;
#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_HS)
    hpcd.Instance = USB_OTG_HS;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = 1;
    hpcd.Init.speed = PCD_SPEED_HIGH;
#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_FS)

    hpcd.Instance = USB_OTG_FS;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = 1;
    hpcd.Init.speed = PCD_SPEED_FULL;
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    map = PinMap_USB_FS;
#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_NO_OTG)
    hpcd.Instance = USB;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.speed = PCD_SPEED_FULL;

    __HAL_RCC_USB_CLK_ENABLE();
    map = PinMap_USB_FS;
#endif

    // Pass instance for usage inside call back
    instance = this;

    // Configure USB pins and other clocks

    while (map->pin != NC) {
        pin_function(map->pin, map->function);
        map++;
    }

    __HAL_RCC_PWR_CLK_ENABLE();

#if !defined(TARGET_STM32WB)
    __HAL_RCC_SYSCFG_CLK_ENABLE();
#endif

#if defined(PWR_CR2_USV)
    HAL_PWREx_EnableVddUSB();
#endif

    // Configure PCD and FIFOs
    hpcd.pData = (void *)this;
    hpcd.State = HAL_PCD_STATE_RESET;
    HAL_StatusTypeDef ret = HAL_PCD_Init(&hpcd);
    MBED_ASSERT(ret == HAL_OK);


    uint32_t total_bytes = 0;

    /* Reserve space in the RX buffer for:
     * - 1 isochonous packet
     * - 2 max sized non-isochonous packets
     * - setup buffer - 10 words as specified by Reference Manual
     * - global nak out - 1 words as specified by Reference Manual
     */
    uint32_t fifo_size = (MAX_PACKET_SIZE_ISO + 4) + (MAX_PACKET_SIZE_NON_ISO + 4) * 2 + (10 * 4) + (1 * 4);
    HAL_PCDEx_SetRxFiFo(&hpcd, (fifo_size / 4));
    total_bytes += fifo_size;

    /* Reserve Tx space up front */
    for (int i = 0; i < NUM_ENDPOINTS; i++) {
        fifo_size =  tx_ep_sizes[i] + 4;
        HAL_PCDEx_SetTxFiFo(&hpcd, i, fifo_size / 4);
        total_bytes += fifo_size;
    }

    /* 1.25 kbytes */
    MBED_ASSERT(total_bytes <= 1280);

    // Configure interrupt vector
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)&_usbisr);
    NVIC_SetPriority(USBHAL_IRQn, 1);
    NVIC_EnableIRQ(USBHAL_IRQn);
}

void USBPhyHw::deinit()
{
    HAL_StatusTypeDef ret = HAL_PCD_DeInit(&hpcd);
    MBED_ASSERT(ret == HAL_OK);

    NVIC_DisableIRQ(USBHAL_IRQn);

    if (events != NULL) {
        sleep_manager_unlock_deep_sleep();
    }
    events = NULL;
}

bool USBPhyHw::powered()
{
    return true;
}

void USBPhyHw::connect()
{
    HAL_StatusTypeDef ret = HAL_PCD_Start(&hpcd);
    MBED_ASSERT(ret == HAL_OK);
}

void USBPhyHw::disconnect()
{
    HAL_StatusTypeDef ret = HAL_PCD_Stop(&hpcd);
    MBED_ASSERT(ret == HAL_OK);
}

void USBPhyHw::configure()
{
    // Not needed
}

void USBPhyHw::unconfigure()
{
    // Not needed
}

void USBPhyHw::sof_enable()
{
    sof_enabled = true;
}

void USBPhyHw::sof_disable()
{
    sof_enabled = false;
}

void USBPhyHw::set_address(uint8_t address)
{
    HAL_StatusTypeDef ret = HAL_PCD_SetAddress(&hpcd, address);
    MBED_ASSERT(ret == HAL_OK);
}

void USBPhyHw::remote_wakeup()
{

}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t table = {
        1280, // 1.25K for endpoint buffers but space is allocated up front
        {
            {USB_EP_ATTR_ALLOW_CTRL                         | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0}, // NON ISO
            {USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0}, // NON ISO
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
            {0                     | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0}
        }
    };
    return &table;
}

uint32_t USBPhyHw::ep0_set_max_packet(uint32_t max_packet)
{
    // FUTURE - set endpoint 0 size and return this size
    return MAX_PACKET_SIZE_EP0;
}

// read setup packet
void USBPhyHw::ep0_setup_read_result(uint8_t *buffer, uint32_t size)
{
    if (size > MAX_PACKET_SIZE_SETUP) {
        size = MAX_PACKET_SIZE_SETUP;
    }
    memcpy(buffer,  hpcd.Setup, size);
    memset(hpcd.Setup, 0, MAX_PACKET_SIZE_SETUP);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    HAL_StatusTypeDef ret;
    epComplete[EP_TO_IDX(0x00)] = 2;
    ret = HAL_PCD_EP_Receive(&hpcd, 0x00, data, size > MAX_PACKET_SIZE_EP0 ? MAX_PACKET_SIZE_EP0 : size);
    MBED_ASSERT(ret != HAL_BUSY);
}

uint32_t USBPhyHw::ep0_read_result()
{
    epComplete[EP_TO_IDX(0x00)] = 0;
    return HAL_PCD_EP_GetRxCount(&hpcd, 0);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    /*  check that endpoint maximum size is not exceeding TX fifo */
    MBED_ASSERT(hpcd.IN_ep[0].maxpacket >= size);
    endpoint_write(0x80, buffer, size);
}

void USBPhyHw::ep0_stall()
{
    endpoint_stall(0x80);
    endpoint_stall(0x00);
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint32_t len;

    /*
     * Endpoints are configured in init since re-configuring
     * fifos when endpoints are added or removed causes tests to fail.
     */
    if (endpoint & 0x80) {
        len = HAL_PCDEx_GetTxFiFo(&hpcd, endpoint & 0x7f);
        MBED_ASSERT(len >= max_packet);
    }
    HAL_StatusTypeDef ret = HAL_PCD_EP_Open(&hpcd, endpoint, max_packet, type);
    MBED_ASSERT(ret != HAL_BUSY);
    return (ret == HAL_OK) ? true : false;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    HAL_StatusTypeDef ret = HAL_PCD_EP_Close(&hpcd, endpoint);
    MBED_ASSERT(ret == HAL_OK);
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    HAL_StatusTypeDef ret;
    ret = HAL_PCD_EP_SetStall(&hpcd, endpoint);
    MBED_ASSERT(ret != HAL_BUSY);
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    HAL_StatusTypeDef ret;
    ret = HAL_PCD_EP_ClrStall(&hpcd, endpoint);
    MBED_ASSERT(ret != HAL_BUSY);
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    // clean reception end flag before requesting reception
    HAL_StatusTypeDef ret = HAL_PCD_EP_Receive(&hpcd, endpoint, data, size);
    MBED_ASSERT(ret != HAL_BUSY);
    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    if (epComplete[EP_TO_IDX(endpoint)] == 0) {
        /*  no reception possible !!! */
        return 0;
    } else if ((epComplete[EP_TO_IDX(endpoint)] != 1)) {
        return 0;
    }
    epComplete[EP_TO_IDX(endpoint)] = 0;
    return HAL_PCD_EP_GetRxCount(&hpcd, endpoint);;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    HAL_StatusTypeDef ret;
    // clean transmission end flag before requesting transmission
    epComplete[EP_TO_IDX(endpoint)] = 2;
    ret = HAL_PCD_EP_Transmit(&hpcd, endpoint, data, size);
    MBED_ASSERT(ret != HAL_BUSY);
    // update the status
    if (ret != HAL_OK) {
        return false;
    }
    // fix me return is too simple
    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{
#if (TARGET_STM32F2)
    HAL_StatusTypeDef ret = HAL_PCD_EP_Abort(&hpcd, endpoint); // fix me: ST driver should not be modified
#else
    HAL_StatusTypeDef ret = HAL_PCD_EP_Close(&hpcd, endpoint); // fix me: implementation not correct
#endif
    MBED_ASSERT(ret == HAL_OK);
}

void USBPhyHw::process()
{
    HAL_PCD_IRQHandler(&instance->hpcd);
    // Re-enable interrupt
    NVIC_ClearPendingIRQ(USBHAL_IRQn);
    NVIC_EnableIRQ(USBHAL_IRQn);
}

void USBPhyHw::_usbisr(void)
{
    NVIC_DisableIRQ(USBHAL_IRQn);
    instance->events->start_process();
}

#endif /* DEVICE_USBDEVICE */
