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

#ifndef USBDEVICE_H
#define USBDEVICE_H

#include "mbed.h"
#include "USBDevice_Types.h"
#include "USBPhy.h"
#include "mbed_critical.h"

class USBDevice;


class USBDevice: public  USBPhyEvents
{
public:
    typedef void (USBDevice::*ep_cb_t)(usb_ep_t endpoint);

    enum Direction {
        RECEIVE = 0,
        SEND = 1
    };

    USBDevice(uint16_t vendor_id, uint16_t product_id, uint16_t product_release);
    USBDevice(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

    /**
     * Acqurie exclusive access to this instance USBDevice
     */
    void lock() {
        core_util_critical_section_enter();
        locked++;
        MBED_ASSERT(locked > 0);
    }

    /**
     * Release exclusive access to this instance USBDevice
     */
    void unlock() {
        MBED_ASSERT(locked > 0);
        locked--;
        core_util_critical_section_exit();
    }

    /**
     * Assert that the current thread of execution holds the lock
     *
     */
    void assert_locked() {
        MBED_ASSERT(locked > 0);
    }

    /**
    * Check if the device is configured
    *
    * @returns true if configured, false otherwise
    */
    bool configured(void);

    /**
    * Connect a device
    *
    * @param blocking: block if not configured
    */
    void connect(bool blocking = true);

    /**
    * Disconnect a device
    */
    void disconnect(void);

    /**
    * Add an endpoint
    *
    * @param endpoint Endpoint to enable
    * @param max_packet Maximum size of a packet which can be sent or received on this endpoint
    * @param type Endpoint type - USB_EP_TYPE_BULK, USB_EP_TYPE_INT or USB_EP_TYPE_ISO
    * @param callback Method pointer to be called when a packet is transferred
    * @returns true if successful, false otherwise
    */
    bool endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type, ep_cb_t callback=NULL);

    /**
    * Add an endpoint
    *
    * @param endpoint Endpoint to enable
    * @param max_packet Maximum size of a packet which can be sent or received on this endpoint
    * @param type Endpoint type - USB_EP_TYPE_BULK, USB_EP_TYPE_INT or USB_EP_TYPE_ISO
    * @param callback Method pointer to be called when a packet is transferred
    * @returns true if successful, false otherwise
    */
    template<typename T>
    bool endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type, void (T::*callback)(usb_ep_t endpoint)) {
        return endpoint_add(endpoint, max_packet, type, static_cast<ep_cb_t>(callback));
    }

    /**
    * Remove an endpoint
    *
    * @param endpoint Endpoint to disable
    * @note This endpoint must already have been setup with endpoint_add
    */
    void endpoint_remove(usb_ep_t endpoint);

    /**
    * Stall an endpoint
    *
    * @param endpoint Endpoint to stall
    * @note You cannot stall endpoint 0 with this function
    * @note This endpoint must already have been setup with endpoint_add
    */
    void endpoint_stall(usb_ep_t endpoint);

    /**
    * Unstall an endpoint
    *
    * @param endpoint Endpoint to unstall
    * @note This endpoint must already have been setup with endpoint_add
    */
    void endpoint_unstall(usb_ep_t endpoint);

    /**
     * Get the current maximum size for this endpoint
     *
     * Return the currently configured maximum packet size, wMaxPacketSize,
     * for this endpoint.
     * @note This endpoint must already have been setup with endpoint_add
     */
    uint32_t endpoint_max_packet_size(usb_ep_t endpoint);

    /** Start a read on the given endpoint
     *
     * After the read is finished call read_start to get the result.
     *
     * @param endpoint endpoint to perform the read on
     * @return true if the read was started, false if no more reads can be started
     * @note This endpoint must already have been setup with endpoint_add
     */
    bool read_start(usb_ep_t endpoint);

    /**
     * Finish a read on the given endpoint
     *
     * Get the contents of a read started with read_start. To ensure all
     * the data from this endpoint is read make sure the buffer and size
     * passed is at least as big as the maximum packet for this endpoint.
     *
     * @param endpoint endpoint to read data from
     * @param buffer buffer to fill with read data
     * @param max_size the total size of the data buffer. This must be at least
     * the max packet size of this endpoint
     * @param size The size of data that was read
     * @return true if the read was completed, otherwise false
     * @note This endpoint must already have been setup with endpoint_add
     */
    bool read_finish(usb_ep_t endpoint, uint8_t *buffer, uint32_t max_size, uint32_t *size);

    /**
     * Write a data to the given endpoint
     *
     * Write data to an endpoint.
     *
     * @param endpoint endpoint to write data to
     * @param buffer data to write
     * @param size the size of data to send. This must be less than or equal to the
     * max packet size of this endpoint
     * @note This endpoint must already have been setup with endpoint_add
     */
    bool write(usb_ep_t endpoint, uint8_t *buffer, uint32_t size);

    /*
    * Get device descriptor.
    *
    * @returns pointer to the device descriptor
    */
    virtual const uint8_t * device_desc();

    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t * configuration_desc(){return NULL;};

    /*
    * Get string lang id descriptor
    *
    * @return pointer to the string lang id descriptor
    */
    virtual const uint8_t * string_langid_desc();

    /*
    * Get string manufacturer descriptor
    *
    * @returns pointer to the string manufacturer descriptor
    */
    virtual const uint8_t * string_imanufacturer_desc();

    /*
    * Get string product descriptor
    *
    * @returns pointer to the string product descriptor
    */
    virtual const uint8_t * string_iproduct_desc();

    /*
    * Get string serial descriptor
    *
    * @returns pointer to the string serial descriptor
    */
    virtual const uint8_t * string_iserial_desc();

    /*
    * Get string configuration descriptor
    *
    * @returns pointer to the string configuration descriptor
    */
    virtual const uint8_t * string_iconfiguration_desc();

    /*
    * Get string interface descriptor
    *
    * @returns pointer to the string interface descriptor
    */
    virtual const uint8_t * string_iinterface_desc();

    /*
    * Get the length of the report descriptor
    *
    * @returns length of the report descriptor
    */
    virtual uint16_t report_desc_dength() { return 0; };

protected:

    /**
    * Called by USBDevice layer on bus reset.
    *
    * complete_reset must be called after
    * the device is fully reset.
    *
    * Warning: Called in ISR context
    */
    virtual void callback_reset() = 0;
    void complete_reset();

    /**
    * Called by USBDevice on Endpoint0 request.
    *
    * This is used to handle extensions to standard requests
    * and class specific requests.
    *
    * If this request is handled outside of USBDevice
    * then complete_request must be called to either
    * reject or continue this request.
    *
    * @return true if this request is handled outside
    * of USBDevice, false if handled by USBDevice.
    *
    * Warning: Called in ISR context
    */
    virtual bool callback_request(const setup_packet_t *setup) = 0;
    void complete_request(bool success);
    void complete_request(uint8_t *data, uint32_t size, Direction direction);

    /**
    * Called by USBDevice on data stage completion
    *
    * Warning: Called in ISR context
    */
    virtual void callback_request_xfer_done(const setup_packet_t *setup) = 0;
    void complete_request_xfer_done(bool success);

    /*
    * Called by USBDevice layer in response to set_configuration.
    *
    * Upon reception of this command endpoints of the previous configuration
    * if any must be removed with endpoint_remove and new endpoint added with
    * endpoint_add.
    *
    * @param configuration Number of the configuration
    *
    * Warning: Called in ISR context
    */
    virtual void callback_set_configuration(uint8_t configuration) = 0;
    void complete_set_configuration(bool success);

    /*
    * Called by USBDevice layer in response to set_interface.
    *
    * Upon reception of this command endpoints of any previous interface
    * if any must be removed with endpoint_remove and new endpoint added with
    * endpoint_add.
    *
    * @param configuration Number of the configuration
    *
    * Warning: Called in ISR context
    */
    virtual void callback_set_interface(uint16_t interface, uint8_t alternate) = 0;
    void complete_set_interface(bool success);

    uint8_t * find_descriptor(uint8_t descriptorType);

    const usb_ep_table_t *endpoint_table();

    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t product_release;
    uint8_t device_descriptor[18];

private:
    // USBPhyEvents
    virtual void suspend(bool suspended);
    virtual void sof(int frameNumber) {};
    virtual void reset(void);
    virtual void ep0_setup(void);
    virtual void ep0_out(void);
    virtual void ep0_in(void);
    virtual void out(usb_ep_t endpoint);
    virtual void in(usb_ep_t endpoint);

    bool request_get_descriptor(void);
    bool control_out(void);
    bool control_in(void);
    bool request_set_address(void);
    bool request_set_configuration(void);
    bool request_set_feature(void);
    bool request_clear_feature(void);
    bool request_get_status(void);
    bool request_setup(void);
    bool control_setup(void);
    void control_setup_continue();
    void decode_setup_packet(uint8_t *data, setup_packet_t *packet);
    bool request_get_configuration(void);
    bool request_get_interface(void);
    bool request_set_interface(void);
    virtual void start_process(void);
    void usbisr_thread(void);
    void reset_endpoint_info(void);

    struct endpoint_info_t
    {
        void (USBDevice::*callback)(usb_ep_t endpoint);
        uint16_t max_packet_size;
        uint8_t flags;
        uint8_t pending;
    };

    endpoint_info_t endpoint_info[32 - 2];

    USBPhy *phy;
    control_transfer_t transfer;
    usb_device_t device;
    uint32_t max_packet_size_ep0;

    uint16_t current_interface;
    uint8_t current_alternate;
    uint32_t locked;
};


#endif
