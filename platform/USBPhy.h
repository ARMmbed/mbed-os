/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#ifndef USBPHY_H
#define USBPHY_H

#include <stdint.h>

/**
 * \defgroup hal_usb_device USB Device HAL
 * Abstract interface to physical USB hardware
 *
 * # Defined behavior
 * * Any endpoint configurations which fit in the parameters of the table returned
 *      by USBPhy::endpointTable can be used.
 * * All endpoints in any valid endpoint configuration can be used concurrently
 * * Device supports use of at least one control, bulk, interrupt and
 *      isochronous in each direction at the same time - at least 8 endpoints.
 * * Device supports all standard endpoint sizes (wMaxPacketSize)
 * * Device can handle an interrupt latency of at least 100ms if reset is not being performed and address is not being set
 * * USBPhyEvents events are only sent when USBPhy is in the initialized state
 * * When unpowered only the USBPhyEvents::power event can be sent
 * * On USB reset all endpoints are removed except for endpoint 0
 * * USBPhyEvents::out and USBPhyEvents::in events only occur for endpoints which have been added
 * * A call to USBPhy::ep0_write results in USBPhyEvents::in getting called if not
 *      interrupted by a power loss or reset
 * * A call to endpoint_read followed by endpoint_read_result results in USBPhyEvents::out getting called if not
 *      interrupted by a power loss or reset
 * * Endpoint 0 naks all transactions aside from setup packets until one
 *      of ep0_read, ep0_write or ep0_stall has been called
 * * Endpoint 0 stall is automatically cleared on reception of a setup packet
 *
 * # Undefined behavior
 * * Calling USBPhy::endpoint_add or USBPhy::endpoint_remove outside of the control requests SetInterface or SetConfiguration
 * * Devices behavior is undefined if latency is greater than 2ms when address is being set - see USB spec 9.2.6.3
 * * Devices behavior is undefined if latency is greater than 10ms when a reset occurs - see USB spec 7.1.7.5
 * * Calling any of the USBPhy::endpoint_* functions on endpoint 0
 *
 * # Potential bugs
 * * Processing control packets in the wrong order when multiple packets are present
 * * Back to back setup packets handled incorrectly
 * * Signal corruption not handled correctly
 * * USB race conditions
 *
 */

typedef uint8_t usb_ep_t;

typedef enum {
    USB_EP_TYPE_CTRL = 0,
    USB_EP_TYPE_ISO = 1,
    USB_EP_TYPE_BULK = 2,
    USB_EP_TYPE_INT = 3
} usb_ep_type_t;

enum  {
    USB_EP_ATTR_ALLOW_CTRL = 1 << USB_EP_TYPE_CTRL,
    USB_EP_ATTR_ALLOW_BULK = 1 << USB_EP_TYPE_BULK,
    USB_EP_ATTR_ALLOW_INT = 1 << USB_EP_TYPE_INT,
    USB_EP_ATTR_ALLOW_ISO = 1 << USB_EP_TYPE_ISO,
    USB_EP_ATTR_ALLOW_ALL = USB_EP_ATTR_ALLOW_CTRL | USB_EP_ATTR_ALLOW_BULK |
                             USB_EP_ATTR_ALLOW_INT | USB_EP_ATTR_ALLOW_ISO,

    USB_EP_ATTR_DIR_IN = 0 << 4,
    USB_EP_ATTR_DIR_OUT = 1 << 4,
    USB_EP_ATTR_DIR_IN_OR_OUT = 2 << 4,
    USB_EP_ATTR_DIR_IN_AND_OUT = 3 << 4,
    USB_EP_ATTR_DIR_MASK = 3 << 4
};
typedef uint8_t usb_ep_attr_t;

struct usb_ep_entry_t {
    usb_ep_attr_t attributes;
    uint8_t byte_cost;
    uint16_t base_cost;
};

struct usb_ep_table_t {
    uint32_t resources;
    usb_ep_entry_t table[16];
};

class USBPhyEvents {
public:
    USBPhyEvents() {};
    virtual ~USBPhyEvents() {};

    /**
     * Callback called when a bus reset occurs
     * @note called in the contex of USBPhy::process
     */
    virtual void reset() = 0;

    /**
     * Callback called when an endpoint 0 setup packet is received
     * @note called in the contex of USBPhy::process
     */
    virtual void ep0_setup() = 0;

    /**
     * Callback called when an endpoint 0 out packet is received
     * @note called in the contex of USBPhy::process
     */
    virtual void ep0_out() = 0;

    /**
     * Callback called when an endpoint 0 in packet is received
     * @note called in the contex of USBPhy::process
     */
    virtual void ep0_in() = 0;

    /**
     * Callback called USB power is applied or removed
     *
     * @param powered true if USB power is present, false otherwise
     * @note called in the contex of USBPhy::process
     */
    virtual void power(bool powered) = 0;

    /**
     * Callback called when entering or leaving suspend mode
     *
     * @param suspended true if entering suspend mode false otherwise
     * @note called in the contex of USBPhy::process
     */
    virtual void suspend(bool suspended) = 0;

    /**
     * Callback called on start of frame
     *
     * @param frame_number The current frame number
     * @note This callback is enabled/disabled by
     *  calling USBPhy::sof_enable / USBPhy::sof_disable
     * @note called in the contex of USBPhy::process
     */
    virtual void sof(int frame_number) = 0;

    /**
     * Callback called on the reception of an OUT packet
     *
     * @param endpoint Endpoint which received the OUT packet
     * @note called in the contex of USBPhy::process
     */
    virtual void out(usb_ep_t endpoint) = 0;

    /**
     * Callback called on the transmission of an IN packet
     *
     * @param endpoint Endpoint which sent the IN packet
     * @note called in the contex of USBPhy::process
     */
    virtual void in(usb_ep_t endpoint) = 0;

    /**
     * Callback called to indicate the USB processing needs to be done
     */
    virtual void start_process() = 0;
};

class USBPhy {
public:
    USBPhy() {};
    virtual ~USBPhy() {};

    /**
     * Initialize this USBPhy instance
     *
     * This function must be called before calling
     * any other functions of this class, unless specifically
     * noted.
     *
     * @param events Callback class to handle USB events
     */
    virtual void init(USBPhyEvents *events) = 0;

    /**
     * Power down this USBPhy instance
     *
     * Disable interrupts and stop sending events.
     */
    virtual void deinit() = 0;

    /**
     * Check if USB power is present
     *
     * Devices which don't support checking the USB power state
     * must always return true.
     *
     * @return true if USB power is present, false otherwise
     */
    virtual bool powered() = 0;

    /**
     * Make the USB phy visible to the USB host
     *
     * Enable either the D+ or D-  pullup so the host can detect
     * the presence of this device.
     */
    virtual void connect() = 0;

    /**
     * Detach the USB phy
     *
     * Disable the D+ and D- pullup and stop responding to
     * USB traffic.
     */
    virtual void disconnect() = 0;

    /**
     * Set this device to the configured state
     *
     * Enable added endpoints if they are not enabled
     * already.
     */
    virtual void configure() = 0;

    /**
     * Leave the configured state
     *
     * This is a notification to the USBPhy indicating that the device
     * is leaving the configured state. The USBPhy can disable all
     * endpoints other than endpoint 0.
     *
     */
    virtual void unconfigure() = 0;

    /**
     * Enable the start of frame interrupt
     *
     * Call USBPhyEvents::sof on every frame.
     */
    virtual void sof_enable() = 0;

    /**
     * Disable the start of frame interrupt
     *
     * Stop calling USBPhyEvents::sof.
     */
    virtual void sof_disable() = 0;

    /**
     * Set the USBPhy's address
     *
     * @param address This device's USB address
     */
    virtual void set_address(uint8_t address) = 0;

    /**
     * Wake upstream devices
     */
    virtual void remote_wakeup() = 0;

    /**
     * Get the endpoint table
     *
     * This function returns a table which describes the endpoints
     * can be used, the functionality of those endpoints and the
     * resource cost.
     */
    virtual const usb_ep_table_t* endpoint_table() = 0;

    /**
     * Set wMaxPacketSize of endpoint 0
     *
     * @param max_packet The wMaxPacketSize value for endpoint 0
     * @return The actual size of endpoint 0
     */
    virtual uint32_t ep0_set_max_packet(uint32_t max_packet) = 0;

    /**
     * Read the contents of the SETUP packet
     *
     * @param buffer Buffer to fill with data
     * @param size Size of buffer passed in
     */
    virtual void ep0_setup_read_result(uint8_t *buffer, uint32_t size) = 0;

    /**
     * Start receiving a packet of up to wMaxPacketSize on endpoint 0
     */
    virtual void ep0_read() = 0;

    /**
     * Read the contents of a received packet
     *
     * @param buffer Buffer to fill with the data read
     * @param size Size of buffer
     */
    virtual uint32_t ep0_read_result(uint8_t *buffer, uint32_t size) = 0;

    /**
     * Write a packet on endpoint 0
     *
     * @param buffer Buffer fill with data to send
     * @param size Size of data to send
     */
    virtual void ep0_write(uint8_t *buffer, uint32_t size) = 0;

    /**
     * Protocol stall on endpoint 0
     *
     * Stall all IN and OUT packets on endpoint 0 until a setup packet
     * is received.
     * @note The stall is cleared automatically when a setup packet is received
     */
    virtual void ep0_stall() = 0;

    /**
     * Configure and enable an endpoint
     *
     * @param endpoint Endpoint to configure and enable
     * @param max_packet The maximum packet size that can be sent or received
     * @param type The type of endpoint this should be configured as -
     *  USB_EP_TYPE_BULK, USB_EP_TYPE_INT or USB_EP_TYPE_ISO
     * @note This function cannot be used to configure endpoint 0. That must be done
     * with ep0_set_max_packet
     */
    virtual bool endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type) = 0;

    /**
     * Disable an endpoint
     *
     * @param endpoint Endpoint to disable
     */
    virtual void endpoint_remove(usb_ep_t endpoint) = 0;

    /**
     * Perform a functional stall on the given endpoint
     *
     * Set the HALT feature for this endpoint so that all further
     * communication is aborted.
     *
     * @param endpoint Endpoint to stall
     */
    virtual void endpoint_stall(usb_ep_t endpoint) = 0;

    /**
     * Unstall the endpoint
     *
     * Clear the HALT feature on this endpoint so communication can
     * resume.
     *
     * @param endpoint Endpoint to stall
     */
    virtual void endpoint_unstall(usb_ep_t endpoint) = 0;

    /**
     * Start a read on the given endpoint
     *
     * @param endpoint Endpoint to start the read on
     * @param max_packet A hint as to the wMaxPacketSize of this endpoint.
     *  This must match the size in endpoint_add.
     * @return true if the read was successfully started, false otherwise
     */
    virtual bool endpoint_read(usb_ep_t endpoint, uint32_t max_packet) = 0;

    /**
     * Finish a read on the given endpoint
     *
     * @param endpoint Endpoint to read data from
     * @param data Buffer to fill with data
     * @param size Size of buffer
     * @param bytes_read The number of bytes in the current packet. This can be larger than
     *  the size parameter if the buffer passed in was too small.
     * @return true if data was read false otherwise
     */
    virtual bool endpoint_read_result(usb_ep_t endpoint, uint8_t *data, uint32_t size, uint32_t *bytes_read) = 0;

    /**
     * Start a write on the given endpoint
     *
     * @param endpoint Endpoint to write to
     * @param data Buffer to write
     * @param size Size of data to write
     * @return true if the data was prepared for transmit, false otherwise
     */
    virtual bool endpoint_write(usb_ep_t endpoint, uint8_t *data, uint32_t size) = 0;

    /**
     * Abort the current transfer if it has not yet been sent
     *
     * @param endpoint Endpoint to abort the transfer on. It is implementation defined
     * if this function has an effect on receive endpoints.
     */
    virtual void endpoint_abort(usb_ep_t endpoint) = 0;

    /**
     * Callback used for performing USB processing
     *
     * USBPhy processing should be triggered by calling USBPhyEvents::start_process
     * and done inside process. All USBPhyEvents callbacks aside from
     * USBPhyEvents::start_process must be called in the context of process
     */
    virtual void process() = 0;
};

#endif
