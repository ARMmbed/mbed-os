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

#ifndef USBPHYHW_H
#define USBPHYHW_H

#include "mbed.h"
#include "USBPhy.h"
#include "PeripheralPins.h"

#if !defined(MBED_CONF_TARGET_USB_SPEED)

#if defined (USB)
#define MBED_CONF_TARGET_USB_SPEED USE_USB_NO_OTG
#elif defined(USB_OTG_FS)
#define MBED_CONF_TARGET_USB_SPEED USE_USB_OTG_FS
#else
#define MBED_CONF_TARGET_USB_SPEED USE_USB_OTG_HS
#endif

#endif /* !defined(MBED_CONF_TARGET_USB_SPEED) */

#if MBED_CONF_TARGET_USB_SPEED == USE_USB_NO_OTG

#if defined(TARGET_STM32F3) || defined(TARGET_STM32WB)
#define USBHAL_IRQn  USB_HP_IRQn
#else
#define USBHAL_IRQn  USB_IRQn
#endif

#elif (MBED_CONF_TARGET_USB_SPEED == USE_USB_OTG_FS)
#define USBHAL_IRQn  OTG_FS_IRQn

#else
#define USBHAL_IRQn  OTG_HS_IRQn

#endif

#define NB_ENDPOINT  16

// #define MAXTRANSFER_SIZE  0x200
#define MAX_PACKET_SIZE_SETUP (48)
#define MAX_PACKET_SIZE_EP0 (64)

class USBPhyHw : public USBPhy {
public:
    USBPhyHw();
    virtual ~USBPhyHw();
    virtual void init(USBPhyEvents *events);
    virtual void deinit();
    virtual bool powered();
    virtual void connect();
    virtual void disconnect();
    virtual void configure();
    virtual void unconfigure();
    virtual void sof_enable();
    virtual void sof_disable();
    virtual void set_address(uint8_t address);
    virtual void remote_wakeup();
    virtual const usb_ep_table_t *endpoint_table();

    virtual uint32_t ep0_set_max_packet(uint32_t max_packet);
    virtual void ep0_setup_read_result(uint8_t *buffer, uint32_t size);
    virtual void ep0_read(uint8_t *data, uint32_t size);
    virtual uint32_t ep0_read_result();
    virtual void ep0_write(uint8_t *buffer, uint32_t size);
    virtual void ep0_stall();

    virtual bool endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type);
    virtual void endpoint_remove(usb_ep_t endpoint);
    virtual void endpoint_stall(usb_ep_t endpoint);
    virtual void endpoint_unstall(usb_ep_t endpoint);

    virtual bool endpoint_read(usb_ep_t endpoint, uint8_t *data, uint32_t size);
    virtual uint32_t endpoint_read_result(usb_ep_t endpoint);
    virtual bool endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size);
    virtual void endpoint_abort(usb_ep_t endpoint);

    virtual void process();

    USBPhyEvents *events;
    bool sof_enabled;

    uint8_t epComplete[2 * NB_ENDPOINT];
    PCD_HandleTypeDef hpcd;

private:

    static void _usbisr(void);
};

#endif
