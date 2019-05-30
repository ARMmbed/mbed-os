/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

/* This target doesn't support USB */
#if !defined(DEVICE_USBDEVICE) || !DEVICE_USBDEVICE
#define USBGD_HAL_UNSUPPORTED
#endif

/* TARGET NOT GD does not support this HAL */
#ifndef TARGET_GigaDevice
#define USBGD_HAL_UNSUPPORTED
#endif

#ifndef USBGD_HAL_UNSUPPORTED

#include "USBPhyHw.h"
#include "pinmap.h"

#include "drv_usbd_int.h"

static uint32_t usbd_ep_setup (usb_core_driver *udev, uint8_t ep_addr, uint16_t ep_mps, uint8_t ep_type);
static uint32_t usbd_ep_clear (usb_core_driver *udev, uint8_t ep_addr);
static uint32_t usbd_ep_recev (usb_core_driver *udev, uint8_t ep_addr, uint8_t *pbuf, uint16_t len);
static uint32_t usbd_ep_send (usb_core_driver *udev, uint8_t ep_addr, uint8_t *pbuf, uint16_t len);
static uint32_t usbd_ep_stall (usb_core_driver *udev, uint8_t ep_addr);
static uint32_t usbd_ep_stall_clear (usb_core_driver *udev, uint8_t ep_addr);
static uint16_t usbd_rxcount_get (usb_core_driver *udev, uint8_t ep_num);

/* endpoint conversion macros */
#define EP_TO_LOG(ep)       ((ep) & 0x7F)
#define EP_TO_IDX(ep)       (((ep) << 1) | ((ep) & 0x80 ? 1 : 0))
#define LOG_IN_TO_EP(ep)    ((ep) | 0x80)
#define LOG_OUT_TO_EP(ep)   ((ep) | 0x00)
#define IDX_TO_EP(ep)       (((ep) >> 1)|((ep) & 1) << 7)

/* endpoint defines */
#define NUM_ENDPOINTS           4
#define MAX_PACKET_NON_ISO      64
#define MAX_PACKET_ISO          (256 + 128)     // Spec can go up to 1023, only ram for this though
#define ENDPOINT_NON_ISO        (USB_EP_ATTR_ALLOW_BULK | USB_EP_ATTR_ALLOW_INT)

static const uint32_t tx_ep_sizes[NUM_ENDPOINTS] = {
        MAX_PACKET_NON_ISO,
        MAX_PACKET_NON_ISO,
        MAX_PACKET_NON_ISO,
        MAX_PACKET_ISO
};

uint32_t USBD_PCDEx_GetTxFiFo(usb_core_driver *hpcd, uint8_t fifo)
{
    uint32_t len;
    if (fifo == 0) {
        len = hpcd->regs.gr->DIEP0TFLEN_HNPTFLEN >> 16;
    } else {
        len = hpcd->regs.gr->DIEPTFLEN[fifo - 1] >> 16;
    }
    return len * 4;
}

void USBD_PCD_SOFCallback(usb_core_driver *hpcd)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));

    if (priv->sof_enabled) {
        priv->events->sof((hpcd->regs.dr->DSTAT & DSTAT_FNRSOF) >> 8);
    }
}

/*  this call at device reception completion on a Out Endpoint  */
void usbd_out_transc(usb_core_driver *hpcd, uint8_t epnum)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));

    uint8_t endpoint = LOG_OUT_TO_EP(epnum);

    priv->epComplete[EP_TO_IDX(endpoint)] = 1;

    /* -2 endpoint 0 In out are not in call back list */
    if (epnum) {
        priv->events->out(endpoint);
    } else {
        priv->events->ep0_out();
    }
}

/*  this is call at device transmission completion on In endpoint */
void usbd_in_transc(usb_core_driver *hpcd, uint8_t epnum)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));

    uint8_t endpoint = LOG_IN_TO_EP(epnum);
    priv->epComplete[EP_TO_IDX(endpoint)] = 1;
    /*  -2 endpoint 0 In out are not in call back list */
    if (epnum) {
        priv->events->in(endpoint);
    } else {
        priv->events->ep0_in();
    }
}
/*  This is call at device set up reception  */
void usbd_setup_transc(usb_core_driver *hpcd)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    priv->events->ep0_setup();
}

void USBD_PCD_SuspendCallback(usb_core_driver *hpcd)
{
    USBPhyHw *priv = ((USBPhyHw *)(hpcd->pData));
    priv->events->suspend(1);
}

void USBD_PCD_ResumeCallback(usb_core_driver *hpcd)
{
    USBPhyHw *priv=((USBPhyHw *)(hpcd->pData));
    priv->events->suspend(0);
}

void USBD_PCD_ConnectCallback(usb_core_driver *hpcd)
{
    // Nothing to do
}

void USBD_PCD_DisconnectCallback(usb_core_driver *hpcd)
{
    // Nothing to do
}

void USBD_PCD_ResetCallback(usb_core_driver *hpcd)
{
    USBPhyHw *obj = ((USBPhyHw *)(hpcd->pData));

    obj->endpoint_add(EP0IN, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
    obj->endpoint_add(EP0OUT, MAX_PACKET_SIZE_EP0, USB_EP_TYPE_CTRL);
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
    uint32_t total_bytes = 0;

    NVIC_DisableIRQ(USBHAL_IRQn);

    if (this->events == NULL) {
        sleep_manager_lock_deep_sleep();
    }
    this->events = events;
    sof_enabled = false;
    memset(epComplete, 0, sizeof(epComplete));
    memset(&hpcd.bp, 0, sizeof(hpcd.bp));


    hpcd.bp.core_enum = USB_CORE_ENUM_FS;
    hpcd.bp.phy_itf = USB_EMBEDDED_PHY;
    hpcd.bp.sof_enable = 0;
    hpcd.bp.core_speed = USB_SPEED_FULL;
    hpcd.bp.num_ep = NB_ENDPOINT;
    hpcd.bp.transfer_mode = USB_USE_FIFO;

    // Pass instance for usage inside call back
    instance = this;

#ifdef GD32F450
    // Configure USB pins and other clocks
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SYSCFG);

    pin_function(USBFS_DM, SET_PIN_FUNCTION(PIN_MODE_AF, PIN_OTYPE_PP, PIN_PUPD_NONE, PIN_AF_10)); // DM
    pin_function(USBFS_DP, SET_PIN_FUNCTION(PIN_MODE_AF, PIN_OTYPE_PP, PIN_PUPD_NONE, PIN_AF_10)); // DP

    rcu_pll48m_clock_config(RCU_PLL48MSRC_PLLQ);

    rcu_ck48m_clock_config(RCU_CK48MSRC_PLL48M);

#else
    rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5);
#endif

    rcu_periph_clock_enable(RCU_USBFS);

    // Configure PCD and FIFOs
    hpcd.pData = (void*)this;

    /* configure USB capabilites */
    usb_basic_init (hpcd.bp, &hpcd.regs);

    /* initailizes the USB core*/
    usb_core_init (hpcd.bp, &hpcd.regs);

    /* initailizes device mode */
    usb_devcore_init (&hpcd);

    usb_dev_disconnect(&hpcd);

    /* Reserve space in the RX buffer for:
     * - 1 isochonous packet
     * - 2 max sized non-isochonous packets
     * - setup buffer - 10 words as specified by Reference Manual
     * - global nak out - 1 words as specified by Reference Manual
     */
    uint32_t fifo_size = (MAX_PACKET_ISO + 4) + (MAX_PACKET_NON_ISO + 4) * 2 + (10 * 4) + (1 * 4);

    usb_set_rxfifo(&hpcd.regs, (fifo_size / 4));
    total_bytes += fifo_size;

    /* Reserve Tx space up front */
    for (int i = 0; i < NUM_ENDPOINTS; i++) {
        fifo_size = tx_ep_sizes[i] + 4;
        usb_set_txfifo(&hpcd.regs, i, fifo_size / 4);
        total_bytes += fifo_size;
    }

    /* flush all Tx FIFOs */
    usb_txfifo_flush (&hpcd.regs, 0x10);

    /* flush entire Rx FIFO */
    usb_rxfifo_flush (&hpcd.regs);

    /* 1.25 kbytes */
    MBED_ASSERT(total_bytes <= 1280);

    // Configure interrupt vector
    NVIC_SetVector(USBHAL_IRQn, (uint32_t)&_usbisr);
    NVIC_SetPriority(USBHAL_IRQn, 1);
    NVIC_EnableIRQ(USBHAL_IRQn);
}

void USBPhyHw::deinit()
{
    usb_dev_stop(&hpcd);

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
    usb_dev_connect(&hpcd);

    usb_globalint_enable(&hpcd);
}

void USBPhyHw::disconnect()
{
    usb_dev_disconnect(&hpcd);

    usb_globalint_disable(&hpcd);
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
    usb_devaddr_set(&hpcd, address);
}

void USBPhyHw::remote_wakeup()
{

}

const usb_ep_table_t *USBPhyHw::endpoint_table()
{
    static const usb_ep_table_t table = {
        1280, // 1.25K for endpoint buffers but space is allocated up front
        {
            {USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_DIR_IN_AND_OUT, 0, 0},
            {ENDPOINT_NON_ISO      | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {ENDPOINT_NON_ISO      | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
            {USB_EP_ATTR_ALLOW_ALL | USB_EP_ATTR_DIR_IN_AND_OUT,  0, 0},
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
    memcpy(buffer, hpcd.dev.setup, size);
    memset(hpcd.dev.setup, 0, MAX_PACKET_SIZE_SETUP);
}

void USBPhyHw::ep0_read(uint8_t *data, uint32_t size)
{
    uint32_t ret;

    epComplete[EP_TO_IDX(EP0OUT)] = 2;

    ret = usbd_ep_recev(&hpcd, EP0OUT, data, size > MAX_PACKET_SIZE_EP0 ? MAX_PACKET_SIZE_EP0 : size);

    MBED_ASSERT(ret == 0U);
}

uint32_t USBPhyHw::ep0_read_result()
{
    epComplete[EP_TO_IDX(EP0OUT)] = 0;

    return usbd_rxcount_get(&hpcd, 0);
}

void USBPhyHw::ep0_write(uint8_t *buffer, uint32_t size)
{
    /*  check that endpoint maximum size is not exceeding TX fifo */
    MBED_ASSERT(hpcd.dev.transc_in[0].max_len >= size);

    endpoint_write(EP0IN, buffer, size);
}

void USBPhyHw::ep0_stall()
{
    endpoint_stall(EP0IN);
    endpoint_stall(EP0OUT);
}

bool USBPhyHw::endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type)
{
    uint32_t len;

    /*
     * Endpoints are configured in init since re-configuring
     * fifos when endpoints are added or removed causes tests to fail.
     */
    if (endpoint & 0x80) {
        len = USBD_PCDEx_GetTxFiFo(&hpcd, endpoint & 0x7f);

        MBED_ASSERT(len >= max_packet);
    }

    uint32_t ret = usbd_ep_setup(&hpcd, endpoint, max_packet, type);

    MBED_ASSERT(ret == 0U);

    return (ret == 0U) ? true : false;
}

void USBPhyHw::endpoint_remove(usb_ep_t endpoint)
{
    uint32_t ret = usbd_ep_clear(&hpcd, endpoint);

    MBED_ASSERT(ret == 0U);
}

void USBPhyHw::endpoint_stall(usb_ep_t endpoint)
{
    uint32_t ret;

    ret = usbd_ep_stall(&hpcd, endpoint);

    MBED_ASSERT(ret == 0U);
}

void USBPhyHw::endpoint_unstall(usb_ep_t endpoint)
{
    uint32_t ret;

    ret = usbd_ep_stall_clear(&hpcd, endpoint);

    MBED_ASSERT(ret == 0U);
}

bool USBPhyHw::endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    // clean reception end flag before requesting reception
    uint32_t ret = usbd_ep_recev(&hpcd, endpoint, data, size);

    MBED_ASSERT(ret == 0U);

    return true;
}

uint32_t USBPhyHw::endpoint_read_result(usb_ep_t endpoint)
{
    if (epComplete[EP_TO_IDX(endpoint)]==0) {
        /*  no reception possible !!! */
        return 0;
    } else if ((epComplete[EP_TO_IDX(endpoint)]!=1)) {
        return 0;
    }

    epComplete[EP_TO_IDX(endpoint)]= 0;

    return usbd_rxcount_get(&hpcd, endpoint);;
}

bool USBPhyHw::endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size)
{
    uint32_t ret;

    // clean transmission end flag before requesting transmission
    epComplete[EP_TO_IDX(endpoint)] = 2;
    ret = usbd_ep_send(&hpcd, endpoint, data, size);
    MBED_ASSERT(ret == 0U);

    // update the status
    if (ret != 0U) return false;

    // fix me return is too simple
    return true;
}

void USBPhyHw::endpoint_abort(usb_ep_t endpoint)
{

}

void USBPhyHw::process()
{
    usbd_isr(&instance->hpcd);

    // Re-enable interrupt
    NVIC_ClearPendingIRQ(USBHAL_IRQn);
    NVIC_EnableIRQ(USBHAL_IRQn);
}

void USBPhyHw::_usbisr(void) {
    NVIC_DisableIRQ(USBHAL_IRQn);
    instance->events->start_process();
}

/*!
    \brief      endpoint initialization
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_desc: pointer to endpoint descriptor
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_setup (usb_core_driver *udev, uint8_t ep_addr, uint16_t ep_mps, uint8_t ep_type)
{
    usb_transc *transc;

    /* set endpoint direction */
    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];

        transc->ep_addr.dir = 1U;
    } else {
        transc = &udev->dev.transc_out[ep_addr];

        transc->ep_addr.dir = 0U;
    }

    transc->ep_addr.num = EP_ID(ep_addr);
    transc->max_len = ep_mps;
    transc->ep_type = ep_type;

    /* active USB endpoint function */
    usb_transc_active (udev, transc);

    return 0;
}

/*!
    \brief      configure the endpoint when it is disabled
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_clear (usb_core_driver *udev, uint8_t ep_addr)
{
    usb_transc *transc;

    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];
    } else {
        transc = &udev->dev.transc_out[ep_addr];
    }

    /* deactive USB endpoint function */
    usb_transc_deactivate (udev, transc);

    return 0;
}


/*!
    \brief      endpoint prepare to receive data
    \param[in]  udev: pointer to usb core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[in]  pbuf: user buffer address pointer
    \param[in]  len: buffer length
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_recev (usb_core_driver *udev, uint8_t ep_addr, uint8_t *pbuf, uint16_t len)
{
    usb_transc *transc = &udev->dev.transc_out[EP_ID(ep_addr)];

    /* setup the transfer */
    transc->xfer_buf = pbuf;
    transc->xfer_len = len;
    transc->xfer_count = 0;

    if (USB_USE_DMA == udev->bp.transfer_mode) {
        transc->dma_addr = (uint32_t)pbuf;
    }

    /* start the transfer */
    usb_transc_outxfer (udev, transc);

    return 0;
}

/*!
    \brief      endpoint prepare to transmit data
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[in]  pbuf: transmit buffer address pointer
    \param[in]  len: buffer length
    \param[out] none
    \retval     none
*/
uint32_t  usbd_ep_send (usb_core_driver *udev, uint8_t ep_addr, uint8_t *pbuf, uint16_t len)
{
    usb_transc *transc = &udev->dev.transc_in[EP_ID(ep_addr)];

    /* setup the transfer */
    transc->xfer_buf = pbuf;
    transc->xfer_len = len;
    transc->xfer_count = 0;

    if (USB_USE_DMA == udev->bp.transfer_mode) {
        transc->dma_addr = (uint32_t)pbuf;
    }

    /* start the transfer */
    usb_transc_inxfer (udev, transc);

    return 0;
}

/*!
    \brief      set an endpoint to STALL status
    \param[in]  udev: pointer to USB core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_stall (usb_core_driver *udev, uint8_t ep_addr)
{
    usb_transc *transc = NULL;

    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];
    } else {
        transc = &udev->dev.transc_out[ep_addr];
    }

    transc->ep_stall = 1;

    usb_transc_stall (udev, transc);

    return (0);
}

/*!
    \brief      clear endpoint STALLed status
    \param[in]  udev: pointer to usb core instance
    \param[in]  ep_addr: endpoint address
                  in this parameter:
                    bit0..bit6: endpoint number (0..7)
                    bit7: endpoint direction which can be IN(1) or OUT(0)
    \param[out] none
    \retval     none
*/
uint32_t usbd_ep_stall_clear (usb_core_driver *udev, uint8_t ep_addr)
{
    usb_transc *transc = NULL;

    if (EP_DIR(ep_addr)) {
        transc = &udev->dev.transc_in[EP_ID(ep_addr)];
    } else {
        transc = &udev->dev.transc_out[ep_addr];
    }

    transc->ep_stall = 0;

    usb_transc_clrstall (udev, transc);

    return (0);
}

/*!
    \brief      get the received data length
    \param[in]  udev: pointer to USB device instance
    \param[in]  ep_num: endpoint number
    \param[out] none
    \retval     USB device operation cur_status
*/
uint16_t  usbd_rxcount_get (usb_core_driver *udev, uint8_t ep_num)
{
    return udev->dev.transc_out[ep_num].xfer_count;
}

#endif
