/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <stddef.h>
#include "USBDevice_Types.h"
#include "USBPhy.h"
#include "mbed_critical.h"
#include "Callback.h"

/**
 * \defgroup drivers_USBDevice USBDevice class
 * \ingroup drivers-internal-api-usb
 * @{
 */

/**
 * Core USB Device driver
 *
 * USB driver which wraps and provides synchronization for a USBPhy object.
 */
class USBDevice: public  USBPhyEvents {
public:
    typedef void (USBDevice::*ep_cb_t)();

    enum RequestResult {
        Receive = 0,
        Send = 1,
        Success = 2,
        Failure = 3,
        PassThrough = 4,
    };

    enum DeviceState {
        Attached,
        Powered,
        Default,
        Address,
        Configured
    };

    struct setup_packet_t {
        struct {
            uint8_t dataTransferDirection;
            uint8_t Type;
            uint8_t Recipient;
        } bmRequestType;
        uint8_t  bRequest;
        uint16_t wValue;
        uint16_t wIndex;
        uint16_t wLength;
    };

    /**
     * Instantiate a new USBDevice with the given parameters
     *
     * @param phy The USBPhy providing physical USB access
     * @param vendor_id The USB vendor ID
     * @param product_id The USB product ID
     * @param product_release The device release number
     */
    USBDevice(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

    /**
     * Cleanup this USBDevice
     *
     * This USBDevice must be uninitialized when the destructor is
     * called or the behavior is undefined.
     */
    virtual ~USBDevice();

    /**
     * Initialize this instance
     *
     * This function must be called before calling
     * any other functions of this class, unless specifically
     */
    void init();

    /**
     * Power down this instance
     *
     * Disable interrupts and stop sending events.
     * This method can be used for temporary power-saving; This call can allow
     * USB to be temporarily disabled to permit power saving.
     * However, it is up to the user to make sure all the
     * transfers have concluded (for example when USB power is lost).
     * USBDevice::connect can be used to resume USB operation.
     */
    void deinit();

    /**
    * Check if the device is configured
    *
    * @returns true if configured, false otherwise
    */
    bool configured();

    /**
    * Connect a device
    * This method can also be used to resume USB operation when USB power is
    * detected after it was suspended via USBDevice::deinit.
    */
    void connect();

    /**
    * Disconnect a device
    */
    void disconnect();

    /**
     * Enable the start of frame interrupt
     *
     * Call USBDevice::callback_sof on every frame.
     */
    void sof_enable();

    /**
     * Disable the start of frame interrupt
     *
     * Stop calling USBDevice::callback_sof.
     */
    void sof_disable();

    /**
    * Add an endpoint
    *
    * @param endpoint Endpoint to enable
    * @param max_packet Maximum size of a packet which can be sent or received on this endpoint
    * @param type Endpoint type - USB_EP_TYPE_BULK, USB_EP_TYPE_INT or USB_EP_TYPE_ISO
    * @param callback Method pointer to be called when a packet is transferred
    * @returns true if successful, false otherwise
    */
    bool endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type, mbed::Callback<void()> callback = NULL);

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
    bool endpoint_add(usb_ep_t endpoint, uint32_t max_packet, usb_ep_type_t type, void (T::*callback)())
    {
        return endpoint_add(endpoint, max_packet, type, mbed::callback(this, static_cast<ep_cb_t>(callback)));
    }

    /**
    * Remove an endpoint
    *
    * @param endpoint Endpoint to disable
    * @note This endpoint must already have been setup with endpoint_add
    */
    void endpoint_remove(usb_ep_t endpoint);

    /**
    * Remove all non-zero endpoints
    */
    void endpoint_remove_all();

    /**
    * Stall an endpoint
    *
    * If there is an ongoing transfer on this endpoint then it will
    * be aborted.
    *
    * @param endpoint Endpoint to stall
    * @note You cannot stall endpoint 0 with this function
    * @note This endpoint must already have been setup with endpoint_add
    */
    void endpoint_stall(usb_ep_t endpoint);

    /**
    * Un-stall an endpoint
    *
    * Un-stalling an endpoint resets data toggle back to DATA0.
    * Additionally, if there is an ongoing transfer on this endpoint
    * it will be aborted.
    *
    * @param endpoint Endpoint to un-stall
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

    /**
     * Abort the current transfer on this endpoint
     *
     * @param endpoint endpoint with transfer to abort
     * @note This endpoint must already have been setup with endpoint_add
     */
    void endpoint_abort(usb_ep_t endpoint);

    /**
     * start a read on the given endpoint
     *
     * Start a read on the given endpoint. The data buffer must remain
     * unchanged until the transfer either completes or is aborted.
     *
     * @param endpoint endpoint to read data from
     * @param buffer buffer to fill with read data
     * @param size The size of data to read. This must be greater than or equal
     *        to the max packet size for this endpoint
     * @return true if the read was completed, otherwise false
     * @note This endpoint must already have been setup with endpoint_add
     */
    bool read_start(usb_ep_t endpoint, uint8_t *buffer, uint32_t size);

    /**
     * Get the status of a read
     *
     * @param endpoint endpoint to get the status of
     * @return number of bytes read by this endpoint
     */
    uint32_t read_finish(usb_ep_t endpoint);

    /**
     * Write a data to the given endpoint
     *
     * Write data to an endpoint. The data sent must remain unchanged until
     * the transfer either completes or is aborted.
     *
     * @param endpoint endpoint to write data to
     * @param buffer data to write
     * @param size the size of data to send. This must be less than or equal to the
     * max packet size of this endpoint
     * @note This endpoint must already have been setup with endpoint_add
     */
    bool write_start(usb_ep_t endpoint, uint8_t *buffer, uint32_t size);

    /**
     * Get the status of a write
     *
     * @param endpoint endpoint to get the status of
     * @return number of bytes sent by this endpoint
     */
    uint32_t write_finish(usb_ep_t endpoint);

    /*
    * Get device descriptor.
    *
    * @returns pointer to the device descriptor
    */
    virtual const uint8_t *device_desc();

    /*
    * Get configuration descriptor
    *
    * @param index descriptor index
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t *configuration_desc(uint8_t index) = 0;

    /*
    * Get string lang id descriptor
    *
    * @return pointer to the string lang id descriptor
    */
    virtual const uint8_t *string_langid_desc();

    /*
    * Get string manufacturer descriptor
    *
    * @returns pointer to the string manufacturer descriptor
    */
    virtual const uint8_t *string_imanufacturer_desc();

    /*
    * Get string product descriptor
    *
    * @returns pointer to the string product descriptor
    */
    virtual const uint8_t *string_iproduct_desc();

    /*
    * Get string serial descriptor
    *
    * @returns pointer to the string serial descriptor
    */
    virtual const uint8_t *string_iserial_desc();

    /*
    * Get string configuration descriptor
    *
    * @returns pointer to the string configuration descriptor
    */
    virtual const uint8_t *string_iconfiguration_desc();

    /*
    * Get string interface descriptor
    *
    * @returns pointer to the string interface descriptor
    */
    virtual const uint8_t *string_iinterface_desc();

    /*
    * Get the length of the report descriptor
    *
    * @returns length of the report descriptor
    */
    virtual uint16_t report_desc_dength()
    {
        return 0;
    };

protected:

    /**
    * Called by USBDevice layer on power state change.
    *
    * @param powered true if device is powered, false otherwise
    *
    * Warning: Called in ISR context
    */
    virtual void callback_power(bool powered)
    {

    }

    /**
    * Called by USBDevice layer on each new USB frame.
    *
    * Callbacks are enabled and disabled by calling sof_enable
    * and sof_disable.
    *
    * @param frame_number The current frame number
    *
    * Warning: Called in ISR context
    */
    virtual void callback_sof(int frame_number)
    {

    }

    /**
    * Called by USBDevice layer on bus reset.
    *
    * complete_reset must be called after
    * the device is fully reset.
    *
    * Warning: Called in ISR context
    */
    virtual void callback_reset()
    {

    }

    /**
    * Called when USB changes state
    *
    * @param new_state The new state of the USBDevice
    *
    * Warning: Called in ISR context
    */
    virtual void callback_state_change(DeviceState new_state) = 0;

    /**
    * Called by USBDevice on Endpoint0 request.
    *
    * This is used to handle extensions to standard requests
    * and class specific requests. The function complete_request
    * must be always be called in response to this callback.
    *
    * Warning: Called in ISR context
    */
    virtual void callback_request(const setup_packet_t *setup) = 0;

    /**
     * Called to complete the setup stage of a callback request
     *
     * Possible options that can be passed as a result are:
     * - Receive - Start the data OUT phase of this control transfer
     * - Send - Start the data IN phase of this control transfer
     * - Success - Operation was a success so start the status phase
     * - Failure - Operation failed or is unsupported so send a stall
     * - PassThrough - Pass on the request for standard processing
     *
     * @param result The result of the setup phase.
     * @param data Buffer to send or receive if the result is Send or Receive
     * @param size Size to transfer if the result is Send or Receive
     */
    void complete_request(RequestResult result, uint8_t *data = NULL, uint32_t size = 0);

    /**
    * Called by USBDevice on data stage completion
    *
    * The function complete_request_xfer_done must be always be called
    * in response to this callback.
    *
    * @param setup Setup packet of the current request
    * @param aborted false if the operation was aborted, true otherwise
    *
    * Warning: Called in ISR context
    */
    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted) = 0;

    /**
     * Called to complete the data stage of a callback request
     *
     * @param success true if the operation was successful, false otherwise
     */
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

    /**
     * Called to complete a set configuration command
     *
     * @param success true if the configuration was set, false otherwise
     */
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

    /**
     * Called to complete a set interface command
     *
     * @param success true if the interface was set, false otherwise
     */
    void complete_set_interface(bool success);

    /**
     * Find a descriptor type inside the configuration descriptor
     *
     * @param descriptor_type Type of descriptor to find
     * @param index Configuration descriptor index ( 0 if only one configuration present )
     * @return A descriptor of the given type or NULL if none were found
     */
    uint8_t *find_descriptor(uint8_t descriptor_type, uint8_t index = 0);

    /**
     * Get the endpoint table of this device
     *
     * @return Endpoint table of the USBPhy attached to this USBDevice
     */
    const usb_ep_table_t *endpoint_table();

    /**
     * Callback called to indicate the USB processing needs to be done
     */
    virtual void start_process();

    /**
     * Acquire exclusive access to this instance USBDevice
     */
    virtual void lock();

    /**
     * Release exclusive access to this instance USBDevice
     */
    virtual void unlock();

    /**
     * Assert that the current thread of execution holds the lock
     *
     */
    virtual void assert_locked();

    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t product_release;
    uint8_t device_descriptor[18];

private:
    // USBPhyEvents
    virtual void power(bool powered);
    virtual void suspend(bool suspended);
    virtual void sof(int frame_number);
    virtual void reset();
    virtual void ep0_setup();
    virtual void ep0_out();
    virtual void ep0_in();
    virtual void out(usb_ep_t endpoint);
    virtual void in(usb_ep_t endpoint);

    bool _request_get_descriptor();
    bool _control_out();
    bool _control_in();
    bool _request_set_address();
    bool _request_set_configuration();
    bool _request_set_feature();
    bool _request_clear_feature();
    bool _request_get_status();
    bool _request_setup();
    void _control_setup();
    void _control_abort();
    void _control_abort_start();
    void _control_setup_continue();
    void _decode_setup_packet(uint8_t *data, setup_packet_t *packet);
    bool _request_get_configuration();
    bool _request_get_interface();
    bool _request_set_interface();
    void _change_state(DeviceState state);
    void _run_later(void (USBDevice::*function)());

    void _complete_request();
    void _complete_request_xfer_done();
    void _complete_set_configuration();
    void _complete_set_interface();

    struct endpoint_info_t {
        mbed::Callback<void()> callback;
        uint16_t max_packet_size;
        uint16_t transfer_size;
        uint8_t flags;
        uint8_t pending;
    };

    struct usb_device_t {
        volatile DeviceState state;
        uint8_t configuration;
        bool suspended;
    };

    enum ControlState {
        Setup,
        DataOut,
        DataIn,
        Status
    };

    enum UserCallback {
        None,
        Request,
        RequestXferDone,
        SetConfiguration,
        SetInterface
    };

    struct complete_request_t {
        RequestResult result;
        uint8_t *data;
        uint32_t size;
    };

    union complete_args_t {
        complete_request_t request;
        bool status;
    };

    struct control_transfer_t {
        setup_packet_t setup;
        uint8_t *ptr;
        uint32_t remaining;
        uint8_t direction;
        bool zlp;
        bool notify;
        ControlState stage;
        UserCallback user_callback;
        complete_args_t args;
    };

    endpoint_info_t _endpoint_info[32 - 2];

    USBPhy *_phy;
    bool _initialized;
    bool _connected;
    bool _endpoint_add_remove_allowed;
    control_transfer_t _transfer;
    usb_device_t _device;
    uint32_t _max_packet_size_ep0;
    void (USBDevice::*_post_process)();

    bool _setup_ready;
    bool _abort_control;

    uint16_t _current_interface;
    uint8_t _current_alternate;
    uint32_t _locked;
};

/** @}*/

#endif
