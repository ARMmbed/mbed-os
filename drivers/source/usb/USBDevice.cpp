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

#include <stdint.h>
#include <string.h>

#include "USBDevice.h"
#include "USBDescriptor.h"
#include "usb_phy_api.h"
#include "mbed_assert.h"

//#define DEBUG

/* Device status */
#define DEVICE_STATUS_SELF_POWERED  (1U<<0)
#define DEVICE_STATUS_REMOTE_WAKEUP (1U<<1)

/* Endpoint status */
#define ENDPOINT_STATUS_HALT        (1U<<0)

/* Standard feature selectors */
#define DEVICE_REMOTE_WAKEUP        (1)
#define ENDPOINT_HALT               (0)

/* Endpoint macros */
#define EP_INDEXABLE(endpoint)  (EP_VALID(endpoint) && !EP_CONTROL(endpoint))
#define EP_TO_INDEX(endpoint)   ((((endpoint & 0xf) << 1) | (endpoint & 0x80 ? 1 : 0)) - 2)
#define INDEX_TO_EP(index)      ((usb_ep_t)((((index) >> 1) | (index & 1 ? 0x80 : 0)) + 1))
#define EP_VALID(endpoint)      (((endpoint) & ~0x8F) == 0)
#define EP_CONTROL(endpoint)    (((endpoint) & 0xF) == 0)
#define EP_RX(endpoint)         ((endpoint) & 0x80)

/* Other defines */
#define ENDPOINT_ENABLED (1 << 0)
#define ENDPOINT_STALLED (1 << 1)

/* The maximum wMaxPacketSize for endpoint 0 */
#if defined(MAX_PACKET_SIZE_EP0)
#undef MAX_PACKET_SIZE_EP0
#endif
#define MAX_PACKET_SIZE_EP0 64

#define USB_MIN(a, b) ((a) > (b) ? (b) : (a))


bool USBDevice::_request_get_descriptor()
{
    assert_locked();

    bool success = false;
#ifdef DEBUG
    printf("get descr: type: %d\r\n", DESCRIPTOR_TYPE(_transfer.setup.wValue));
#endif
    switch (DESCRIPTOR_TYPE(_transfer.setup.wValue)) {
        case DEVICE_DESCRIPTOR: {
            if (device_desc() != NULL) {
                if ((device_desc()[0] == DEVICE_DESCRIPTOR_LENGTH) \
                        && (device_desc()[1] == DEVICE_DESCRIPTOR)) {
#ifdef DEBUG
                    printf("device descr\r\n");
#endif
                    _transfer.remaining = DEVICE_DESCRIPTOR_LENGTH;
                    _transfer.ptr = (uint8_t *)device_desc();
                    _transfer.direction = Send;
                    success = true;
                }
            }
            break;
        }
        case CONFIGURATION_DESCRIPTOR: {
            const uint8_t idx = DESCRIPTOR_INDEX(_transfer.setup.wValue);
            if (configuration_desc(idx) != NULL) {
                if ((configuration_desc(idx)[0] == CONFIGURATION_DESCRIPTOR_LENGTH) \
                        && (configuration_desc(idx)[1] == CONFIGURATION_DESCRIPTOR)) {
#ifdef DEBUG
                    printf("conf descr request\r\n");
#endif
                    /* Get wTotalLength */
                    _transfer.remaining = configuration_desc(idx)[2] \
                                          | (configuration_desc(idx)[3] << 8);

                    _transfer.ptr = (uint8_t *)configuration_desc(idx);
                    _transfer.direction = Send;
                    success = true;
                }
            }
            break;
        }
        case STRING_DESCRIPTOR: {
#ifdef DEBUG
            printf("str descriptor\r\n");
#endif
            switch (DESCRIPTOR_INDEX(_transfer.setup.wValue)) {
                case STRING_OFFSET_LANGID:
#ifdef DEBUG
                    printf("1\r\n");
#endif
                    _transfer.remaining = string_langid_desc()[0];
                    _transfer.ptr = (uint8_t *)string_langid_desc();
                    _transfer.direction = Send;
                    success = true;
                    break;
                case STRING_OFFSET_IMANUFACTURER:
#ifdef DEBUG
                    printf("2\r\n");
#endif
                    _transfer.remaining =  string_imanufacturer_desc()[0];
                    _transfer.ptr = (uint8_t *)string_imanufacturer_desc();
                    _transfer.direction = Send;
                    success = true;
                    break;
                case STRING_OFFSET_IPRODUCT:
#ifdef DEBUG
                    printf("3\r\n");
#endif
                    _transfer.remaining = string_iproduct_desc()[0];
                    _transfer.ptr = (uint8_t *)string_iproduct_desc();
                    _transfer.direction = Send;
                    success = true;
                    break;
                case STRING_OFFSET_ISERIAL:
#ifdef DEBUG
                    printf("4\r\n");
#endif
                    _transfer.remaining = string_iserial_desc()[0];
                    _transfer.ptr = (uint8_t *)string_iserial_desc();
                    _transfer.direction = Send;
                    success = true;
                    break;
                case STRING_OFFSET_ICONFIGURATION:
#ifdef DEBUG
                    printf("5\r\n");
#endif
                    _transfer.remaining = string_iconfiguration_desc()[0];
                    _transfer.ptr = (uint8_t *)string_iconfiguration_desc();
                    _transfer.direction = Send;
                    success = true;
                    break;
                case STRING_OFFSET_IINTERFACE:
#ifdef DEBUG
                    printf("6\r\n");
#endif
                    _transfer.remaining = string_iinterface_desc()[0];
                    _transfer.ptr = (uint8_t *)string_iinterface_desc();
                    _transfer.direction = Send;
                    success = true;
                    break;
            }
            break;
        }
        case INTERFACE_DESCRIPTOR: {
#ifdef DEBUG
            printf("interface descr\r\n");
#endif
            break;
        }
        case ENDPOINT_DESCRIPTOR: {
#ifdef DEBUG
            printf("endpoint descr\r\n");
#endif
            /* TODO: Support is optional, not implemented here */
            break;
        }
        default: {
#ifdef DEBUG
            printf("ERROR\r\n");
#endif
            break;
        }
    }

    return success;
}

void USBDevice::_decode_setup_packet(uint8_t *data, setup_packet_t *packet)
{
    // No lock needed - stateless function

    /* Fill in the elements of a setup_packet_t structure from raw data */
    packet->bmRequestType.dataTransferDirection = (data[0] & 0x80) >> 7;
    packet->bmRequestType.Type = (data[0] & 0x60) >> 5;
    packet->bmRequestType.Recipient = data[0] & 0x1f;
    packet->bRequest = data[1];
    packet->wValue = (data[2] | (uint16_t)data[3] << 8);
    packet->wIndex = (data[4] | (uint16_t)data[5] << 8);
    packet->wLength = (data[6] | (uint16_t)data[7] << 8);
}


bool USBDevice::_control_out()
{
    assert_locked();

    /* Control transfer data OUT stage */
    uint32_t packetSize;

    /* Check we should be transferring data OUT */
    if (_transfer.direction != Receive) {
        /* for other platforms, count on the HAL to handle this case */
        return false;
    }

    /* Read from endpoint */
    packetSize = _phy->ep0_read_result();

    /* Check if transfer size is valid */
    if (packetSize > _transfer.remaining) {
        /* Too big */
        return false;
    }

    /* Update transfer */
    _transfer.ptr += packetSize;
    _transfer.remaining -= packetSize;

    /* Check if transfer has completed */
    if (_transfer.remaining == 0) {
        /* Transfer completed */
        _transfer.user_callback = RequestXferDone;
        if (_transfer.notify) {
            /* Notify class layer. */
            _transfer.notify = false;
            callback_request_xfer_done(&_transfer.setup, false);
        } else {
            complete_request_xfer_done(true);
        }
    } else {
        _phy->ep0_read(_transfer.ptr, USB_MIN(_transfer.remaining, _max_packet_size_ep0));
    }

    return true;
}

bool USBDevice::_control_in()
{
    assert_locked();

    /* Control transfer data IN stage */
    uint32_t packetSize;


    /* Check we should be transferring data IN */
    if (_transfer.direction != Send) {
        return false;
    }

    if (_transfer.remaining == 0) {
        if (!_transfer.zlp) {
            /* Status as already been sent so ignore this IN. */
            return true;
        }
        /* ZLP will be sent below */
        _transfer.zlp = false;
    }

    packetSize = _transfer.remaining;

    if (packetSize > _max_packet_size_ep0) {
        packetSize = _max_packet_size_ep0;
    }

    /* Write to endpoint */
    _phy->ep0_write(_transfer.ptr, packetSize);

    /* Update transfer */
    _transfer.ptr += packetSize;
    _transfer.remaining -= packetSize;

    /* Send status if all the data has been sent
     * NOTE - Start the status stage immediately
     * after writing the last packet. Do not wait
     * for the next IN event, as this can be dropped
     * if the ACK by the host is corrupted.
     *
     * For more info on this see section
     * 8.5.3.2 of the USB2.0 specification.
     */
    if ((_transfer.remaining == 0) && !_transfer.zlp) {
        /* Transfer completed */
        _transfer.user_callback = RequestXferDone;
        if (_transfer.notify) {
            /* Notify class layer. */
            _transfer.notify = false;
            callback_request_xfer_done(&_transfer.setup, false);
        } else {
            complete_request_xfer_done(true);
        }


        /* Completed */
        return true;
    }


    return true;
}

void USBDevice::complete_request_xfer_done(bool success)
{
    lock();

    MBED_ASSERT(_transfer.user_callback == RequestXferDone);
    _transfer.args.status = success;
    _run_later(&USBDevice::_complete_request_xfer_done);

    unlock();
}

void USBDevice::_complete_request_xfer_done()
{
    assert_locked();

    bool success = _transfer.args.status;

    _transfer.user_callback = None;
    if (_abort_control) {
        _control_abort();
        return;
    }

    if (!success) {
        _phy->ep0_stall();
        return;
    }

    /* Status stage */
    if (_transfer.stage == DataOut) {
        _transfer.stage = Status;
        _phy->ep0_write(NULL, 0);
    }  else if (_transfer.stage == DataIn) {
        _transfer.stage = Status;
        _phy->ep0_read(NULL, 0);
    }
}

bool USBDevice::_request_set_address()
{
    assert_locked();

    /* Set the device address */
    _phy->set_address(_transfer.setup.wValue);

    if (_transfer.setup.wValue == 0) {
        _change_state(Default);
    } else {
        _change_state(Address);
    }

    return true;
}

bool USBDevice::_request_set_configuration()
{
    assert_locked();

    _device.configuration = _transfer.setup.wValue;
    /* Set the device configuration */
    if (_device.configuration == 0) {
        /* Not configured */
        _phy->unconfigure();
        _change_state(Address);
    } else {
        _endpoint_add_remove_allowed = true;
        _transfer.user_callback = SetConfiguration;
        callback_set_configuration(_device.configuration);
    }

    return true;
}

void USBDevice::complete_set_configuration(bool success)
{
    lock();

    MBED_ASSERT(_transfer.user_callback == SetConfiguration);
    _transfer.args.status = success;
    _run_later(&USBDevice::_complete_set_configuration);

    unlock();
}

void USBDevice::_complete_set_configuration()
{
    assert_locked();

    bool success = _transfer.args.status;
    if ((_abort_control || !success) && !configured()) {
        // The set configuration request was aborted or failed so
        // reset any endpoints which may have been added.
        memset(_endpoint_info, 0, sizeof(_endpoint_info));
        _device.configuration = 0;
        _endpoint_add_remove_allowed = false;
    }


    _transfer.user_callback = None;
    if (_abort_control) {
        _control_abort();
        return;
    }

    if (success) {
        /* Valid configuration */
        _phy->configure();
        _change_state(Configured);
        _control_setup_continue();
    } else {
        _phy->ep0_stall();
        return;
    }
}

bool USBDevice::_request_get_configuration()
{
    assert_locked();

    /* Send the device configuration */
    _transfer.ptr = &_device.configuration;
    _transfer.remaining = sizeof(_device.configuration);
    _transfer.direction = Send;
    return true;
}

bool USBDevice::_request_get_interface()
{
    assert_locked();

    /* Return the selected alternate setting for an interface */

    if (_device.state != Configured) {
        return false;
    }

    /* Send the alternate setting */
    _transfer.setup.wIndex = _current_interface;
    _transfer.ptr = &_current_alternate;
    _transfer.remaining = sizeof(_current_alternate);
    _transfer.direction = Send;
    return true;
}

bool USBDevice::_request_set_interface()
{
    assert_locked();

    _transfer.user_callback = SetInterface;
    callback_set_interface(_transfer.setup.wIndex, _transfer.setup.wValue);
    return true;
}

void USBDevice::complete_set_interface(bool success)
{
    lock();

    MBED_ASSERT(_transfer.user_callback == SetInterface);
    _transfer.args.status = success;
    _run_later(&USBDevice::_complete_set_interface);

    unlock();
}

void USBDevice::_complete_set_interface()
{
    assert_locked();

    bool success = _transfer.args.status;

    _transfer.user_callback = None;
    if (_abort_control) {
        _control_abort();
        return;
    }

    if (success) {
        _current_interface = _transfer.setup.wIndex;
        _current_alternate = _transfer.setup.wValue;
        _control_setup_continue();
    } else {
        _phy->ep0_stall();
        return;
    }
}

bool USBDevice::_request_set_feature()
{
    assert_locked();
    bool success = false;

    if (_device.state != Configured) {
        /* Endpoint or interface must be zero */
        if (_transfer.setup.wIndex != 0) {
            return false;
        }
    }

    switch (_transfer.setup.bmRequestType.Recipient) {
        case DEVICE_RECIPIENT:
            /* TODO: Remote wakeup feature not supported */
            break;
        case ENDPOINT_RECIPIENT:
            if (!EP_INDEXABLE(_transfer.setup.wIndex)) {
                break;
            } else if (_transfer.setup.wValue == ENDPOINT_HALT) {
                endpoint_stall(_transfer.setup.wIndex);
                success = true;
            }
            break;
        default:
            break;
    }

    return success;
}

bool USBDevice::_request_clear_feature()
{
    assert_locked();

    bool success = false;

    if (_device.state != Configured) {
        /* Endpoint or interface must be zero */
        if (_transfer.setup.wIndex != 0) {
            return false;
        }
    }

    switch (_transfer.setup.bmRequestType.Recipient) {
        case DEVICE_RECIPIENT:
            /* TODO: Remote wakeup feature not supported */
            break;
        case ENDPOINT_RECIPIENT:
            if (!EP_INDEXABLE(_transfer.setup.wIndex)) {
                break;
            } else if (_transfer.setup.wValue == ENDPOINT_HALT) {
                endpoint_unstall(_transfer.setup.wIndex);
                success = true;
            }
            break;
        default:
            break;
    }

    return success;
}

bool USBDevice::_request_get_status()
{
    assert_locked();

    static uint16_t status;
    bool success = false;

    if (_device.state != Configured) {
        /* Endpoint or interface must be zero */
        if (_transfer.setup.wIndex != 0) {
            return false;
        }
    }

    switch (_transfer.setup.bmRequestType.Recipient) {
        case DEVICE_RECIPIENT:
            /* TODO: Currently only supports self powered devices */
            status = DEVICE_STATUS_SELF_POWERED;
            success = true;
            break;
        case INTERFACE_RECIPIENT:
            status = 0;
            success = true;
            break;
        case ENDPOINT_RECIPIENT:
            if (!EP_VALID(_transfer.setup.wIndex)) {
                break;
            } else if (EP_CONTROL(_transfer.setup.wIndex)) {
                /* Control endpoint can't be halted */
                status = 0;
            } else if (_endpoint_info[EP_TO_INDEX(_transfer.setup.wIndex & 0xFF)].flags & ENDPOINT_STALLED) {
                status = ENDPOINT_STATUS_HALT;
            } else {
                status = 0;
            }
            success = true;
            break;
        default:
            break;
    }

    if (success) {
        /* Send the status */
        _transfer.ptr = (uint8_t *)&status; /* Assumes little endian */
        _transfer.remaining = sizeof(status);
        _transfer.direction = Send;
    }

    return success;
}

bool USBDevice::_request_setup()
{
    assert_locked();

    bool success = false;

    /* Process standard requests */
    if ((_transfer.setup.bmRequestType.Type == STANDARD_TYPE)) {
        switch (_transfer.setup.bRequest) {
            case GET_STATUS:
                success = _request_get_status();
                break;
            case CLEAR_FEATURE:
                success = _request_clear_feature();
                break;
            case SET_FEATURE:
                success = _request_set_feature();
                break;
            case SET_ADDRESS:
                success = _request_set_address();
                break;
            case GET_DESCRIPTOR:
                success = _request_get_descriptor();
                break;
            case SET_DESCRIPTOR:
                /* TODO: Support is optional, not implemented here */
                success = false;
                break;
            case GET_CONFIGURATION:
                success = _request_get_configuration();
                break;
            case SET_CONFIGURATION:
                success = _request_set_configuration();
                break;
            case GET_INTERFACE:
                success = _request_get_interface();
                break;
            case SET_INTERFACE:
                success = _request_set_interface();
                break;
            default:
                break;
        }
    }

    return success;
}

void USBDevice::_control_setup()
{
    assert_locked();

    /* Control transfer setup stage */
    uint8_t buffer[MAX_PACKET_SIZE_EP0];

    _phy->ep0_setup_read_result(buffer, _max_packet_size_ep0);

    /* Initialise control transfer state */
    _decode_setup_packet(buffer, &_transfer.setup);
    _transfer.ptr = NULL;
    _transfer.remaining = 0;
    _transfer.direction = 0;
    _transfer.zlp = false;
    _transfer.notify = false;
    _transfer.stage = Setup;
    _transfer.user_callback = Request;

#ifdef DEBUG
    printf("dataTransferDirection: %d\r\nType: %d\r\nRecipient: %d\r\nbRequest: %d\r\nwValue: %d\r\nwIndex: %d\r\nwLength: %d\r\n", _transfer.setup.bmRequestType.dataTransferDirection,
           _transfer.setup.bmRequestType.Type,
           _transfer.setup.bmRequestType.Recipient,
           _transfer.setup.bRequest,
           _transfer.setup.wValue,
           _transfer.setup.wIndex,
           _transfer.setup.wLength);
#endif

    /* Class / vendor specific */
    callback_request(&_transfer.setup);
}

void USBDevice::complete_request(RequestResult direction, uint8_t *data, uint32_t size)
{
    lock();

    MBED_ASSERT(_transfer.user_callback == Request);
    _transfer.args.request.result = direction;
    _transfer.args.request.data = data;
    _transfer.args.request.size = size;
    _run_later(&USBDevice::_complete_request);

    unlock();
}

void USBDevice::_complete_request()
{
    assert_locked();

    RequestResult direction = _transfer.args.request.result;
    uint8_t *data = _transfer.args.request.data;
    uint32_t size = _transfer.args.request.size;

    _transfer.user_callback = None;
    if (_abort_control) {
        if ((direction == Receive) || (direction == Send)) {
            _transfer.user_callback = RequestXferDone;
            callback_request_xfer_done(&_transfer.setup, true);
        } else {
            _control_abort();
        }
        return;
    }

    if (direction == PassThrough) {
        /* Standard requests */
        if (!_request_setup()) {
            _phy->ep0_stall();
            return;
        }

        /* user_callback may be set by _request_setup() */
        if (_transfer.user_callback == None) {
            _control_setup_continue();
        }
    } else if (direction == Failure) {
        _phy->ep0_stall();
        return;
    } else {
        _transfer.notify = true;
        _transfer.remaining = size;
        _transfer.ptr = data;
        _transfer.direction = direction;
        _control_setup_continue();
    }
}

void USBDevice::_control_abort_start()
{
    assert_locked();

    _setup_ready = false;
    if (_transfer.user_callback == None) {
        _control_abort();
    } else {
        _abort_control = true;
    }
}

void USBDevice::_control_setup_continue()
{
    assert_locked();

    /* Check transfer size and direction */
    if (_transfer.setup.wLength > 0) {
        if (_transfer.setup.bmRequestType.dataTransferDirection \
                == Send) {
            /* IN data stage is required */
            if (_transfer.direction != Send) {
                _phy->ep0_stall();
                return;
            }

            /* Transfer must be less than or equal to the size */
            /* requested by the host */
            if (_transfer.remaining > _transfer.setup.wLength) {
                _transfer.remaining = _transfer.setup.wLength;
            }
        } else {

            /* OUT data stage is required */
            if (_transfer.direction != Receive) {
                _phy->ep0_stall();
                return;
            }

            /* Transfer must be equal to the size requested by the host */
            if (_transfer.remaining != _transfer.setup.wLength) {
                _phy->ep0_stall();
                return;
            }
        }
    } else {
        /* No data stage; transfer size must be zero */
        if (_transfer.remaining != 0) {
            _phy->ep0_stall();
            return;
        }
    }

    /* Data or status stage if applicable */
    if (_transfer.setup.wLength > 0) {
        if (_transfer.setup.bmRequestType.dataTransferDirection \
                == Send) {
            /* Check if we'll need to send a zero length packet at */
            /* the end of this transfer */
            if (_transfer.setup.wLength > _transfer.remaining) {
                /* Device wishes to transfer less than host requested */
                if ((_transfer.remaining % _max_packet_size_ep0) == 0) {
                    /* Transfer is a multiple of EP0 max packet size */
                    _transfer.zlp = true;
                }
            }

            /* IN stage */
            _transfer.stage = DataIn;
            _control_in();
        } else {
            /* OUT stage */
            _transfer.stage = DataOut;
            _phy->ep0_read(_transfer.ptr, USB_MIN(_transfer.remaining, _max_packet_size_ep0));
        }
    } else {
        /* Status stage */
        _transfer.stage = Status;
        _phy->ep0_write(NULL, 0);
    }
}

void USBDevice::_control_abort()
{
    assert_locked();

    _abort_control = false;
    _transfer.stage = Status;
}

void USBDevice::reset()
{
    assert_locked();

    _change_state(Default);
    _device.suspended = false;
    _control_abort_start();

    /* Call class / vendor specific busReset function */
    callback_reset();
}

void USBDevice::ep0_setup()
{
    assert_locked();

    if (_device.state < Default) {
        MBED_ASSERT(0);
        return;
    }

    _setup_ready = true;

    /* Endpoint 0 setup event */
    if (_transfer.user_callback == None) {
        _control_setup();
    } else {
        /* A new setup packet has arrived so abort the
        current control transfer */
        _abort_control = true;
    }

}

void USBDevice::ep0_out()
{
    assert_locked();

    if (_device.state < Default) {
        MBED_ASSERT(0);
        return;
    }

    if (_transfer.user_callback != None) {
        /* EP0 OUT should not receive data if the stack is waiting
           on a user callback for the buffer to fill or status */
        MBED_ASSERT(0);
        return;
    }

    if (_transfer.stage == Status) {
        // No action needed on status stage
        return;
    }

    /* Endpoint 0 OUT data event */
    if (!_control_out()) {
        /* Protocol stall; this will stall both endpoints */
        _phy->ep0_stall();
        return;
    }
}

void USBDevice::ep0_in()
{
    assert_locked();

    if (_device.state < Default) {
        MBED_ASSERT(0);
        return;
    }

#ifdef DEBUG
    printf("ep0_in\r\n");
#endif
    if (_transfer.stage == Status) {
        // No action needed on status stage
        return;
    }

    /* Endpoint 0 IN data event */
    if (!_control_in()) {
        /* Protocol stall; this will stall both endpoints */
        _phy->ep0_stall();
        return;
    }
}

void USBDevice::out(usb_ep_t endpoint)
{
    assert_locked();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        return;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];

    MBED_ASSERT(info->pending >= 1);
    info->pending -= 1;
    if (info->callback) {
        info->callback();
    }
}

void USBDevice::in(usb_ep_t endpoint)
{
    assert_locked();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        return;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];

    MBED_ASSERT(info->pending >= 1);
    info->pending -= 1;
    if (info->callback) {
        info->callback();
    }
}

void USBDevice::init()
{
    lock();

    if (!_initialized) {
        this->_phy->init(this);
        _max_packet_size_ep0 = this->_phy->ep0_set_max_packet(MAX_PACKET_SIZE_EP0);
        _initialized = true;
    }

    unlock();
}

void USBDevice::deinit()
{
    lock();

    if (_initialized) {
        disconnect();
        this->_phy->deinit();
        _initialized = false;
    }

    unlock();
}

bool USBDevice::configured()
{
    lock();

    /* Returns true if device is in the Configured state */
    bool ret = (_device.state == Configured);

    unlock();
    return ret;
}

void USBDevice::connect()
{
    lock();

    /* Ensure device has been initialized */
    init();

    /* Connect device */
    if (!_connected) {
        _phy->connect();
        _connected = true;
    }

    unlock();
}

void USBDevice::disconnect()
{
    lock();

    /* Disconnect device */
    if (_connected) {
        _phy->disconnect();
        _connected = false;
    }

    /* Set initial device state */
    if (_device.state > Powered) {
        _change_state(Powered);
    }
    //TODO - remove these?
    _device.configuration = 0;
    _device.suspended = false;

    unlock();
}

void USBDevice::sof_enable()
{
    lock();

    _phy->sof_enable();

    unlock();
}

void USBDevice::sof_disable()
{
    lock();

    _phy->sof_disable();

    unlock();
}

bool USBDevice::endpoint_add(usb_ep_t endpoint, uint32_t max_packet_size, usb_ep_type_t type, mbed::Callback<void()> callback)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return false;
    }

    if (!_endpoint_add_remove_allowed) {
        unlock();
        return false;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(!(info->flags & ENDPOINT_ENABLED));
    MBED_ASSERT(max_packet_size <= 1024);

    bool ret = _phy->endpoint_add(endpoint, max_packet_size, type);
    if (ret) {
        info->callback = callback;
        info->flags |= ENDPOINT_ENABLED;
        info->pending = 0;
        info->max_packet_size = max_packet_size;
    }

    unlock();
    return ret;
}

void USBDevice::endpoint_remove(usb_ep_t endpoint)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return;
    }

    if (!_endpoint_add_remove_allowed) {
        unlock();
        return;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    MBED_ASSERT(info->flags & ENDPOINT_ENABLED);

    if (info->pending) {
        _phy->endpoint_abort(endpoint);
    }

    info->callback = NULL;
    info->flags = 0;
    info->pending = 0;
    info->max_packet_size = 0;

    _phy->endpoint_remove(endpoint);

    unlock();
}

void USBDevice::endpoint_remove_all()
{
    lock();

    for (uint32_t i = 0; i < sizeof(_endpoint_info) / sizeof(_endpoint_info[0]); i++) {
        endpoint_info_t *info = _endpoint_info + i;
        if (info->flags & ENDPOINT_ENABLED) {
            endpoint_remove(INDEX_TO_EP(i));
        }
    }

    unlock();
}

void USBDevice::endpoint_stall(usb_ep_t endpoint)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    if (!(info->flags & ENDPOINT_ENABLED)) {
        // Invalid endpoint is being used
        MBED_ASSERT(!configured());
        unlock();
        return;
    }

    info->flags |= ENDPOINT_STALLED;
    _phy->endpoint_stall(endpoint);

    if (info->pending) {
        endpoint_abort(endpoint);
    }

    unlock();
}

void USBDevice::endpoint_unstall(usb_ep_t endpoint)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    if (!(info->flags & ENDPOINT_ENABLED)) {
        // Invalid endpoint is being used
        MBED_ASSERT(!configured());
        unlock();
        return;
    }

    if (info->pending) {
        endpoint_abort(endpoint);
    }

    info->flags &= ~ENDPOINT_STALLED;
    _phy->endpoint_unstall(endpoint);

    unlock();
}

uint8_t *USBDevice::find_descriptor(uint8_t descriptorType, uint8_t index)
{
    /* Find a descriptor within the list of descriptors */
    /* following a configuration descriptor. */
    uint16_t wTotalLength;
    uint8_t *ptr;

    if (configuration_desc(index) == NULL) {
        return NULL;
    }

    /* Check this is a configuration descriptor */
    if ((configuration_desc(index)[0] != CONFIGURATION_DESCRIPTOR_LENGTH) \
            || (configuration_desc(index)[1] != CONFIGURATION_DESCRIPTOR)) {
        return NULL;
    }

    wTotalLength = configuration_desc(index)[2] | (configuration_desc(index)[3] << 8);

    /* Check there are some more descriptors to follow */
    if (wTotalLength <= (CONFIGURATION_DESCRIPTOR_LENGTH + 2))
        /* +2 is for bLength and bDescriptorType of next descriptor */
    {
        return NULL;
    }

    /* Start at first descriptor after the configuration descriptor */
    ptr = &(((uint8_t *)configuration_desc(index))[CONFIGURATION_DESCRIPTOR_LENGTH]);

    do {
        if (ptr[1] /* bDescriptorType */ == descriptorType) {
            /* Found */
            return ptr;
        }

        /* Skip to next descriptor */
        ptr += ptr[0]; /* bLength */
    } while (ptr < (configuration_desc(index) + wTotalLength));

    /* Reached end of the descriptors - not found */
    return NULL;
}

const usb_ep_table_t *USBDevice::endpoint_table()
{
    return _phy->endpoint_table();
}

void USBDevice::power(bool powered)
{
    assert_locked();

    if (!powered && _device.state > Attached) {
        _change_state(Attached);
    }
}

void USBDevice::suspend(bool suspended)
{
}

void USBDevice::sof(int frame_number)
{
    callback_sof(frame_number);
}


USBDevice::USBDevice(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
{
    this->vendor_id = vendor_id;
    this->product_id = product_id;
    this->product_release = product_release;

    memset(_endpoint_info, 0, sizeof(_endpoint_info));
    memset(&_transfer, 0, sizeof(_transfer));
    _transfer.user_callback = None;

    _setup_ready = false;
    _abort_control = false;

    _phy = phy;
    _initialized = false;
    _connected = false;
    _endpoint_add_remove_allowed = false;
    _current_interface = 0;
    _current_alternate = 0;
    _locked = 0;
    _post_process = NULL;

    /* Set initial device state */
    _device.state = Powered;
    _device.configuration = 0;
    _device.suspended = false;
}

USBDevice::~USBDevice()
{
    MBED_ASSERT(!_initialized);
    deinit();
}

uint32_t USBDevice::endpoint_max_packet_size(usb_ep_t endpoint)
{
    lock();

    uint32_t size = 0;
    if (EP_CONTROL(endpoint)) {
        size = _max_packet_size_ep0;
    } else {
        endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
        size = info->max_packet_size;
    }

    unlock();
    return size;
}

void USBDevice::endpoint_abort(usb_ep_t endpoint)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    if (!(info->flags & ENDPOINT_ENABLED)) {
        // Assert that only valid endpoints are used when in the configured state
        MBED_ASSERT(!configured());
        unlock();
        return;
    }

    if (info->pending) {
        _phy->endpoint_abort(endpoint);
        info->pending = 0;
    }

    unlock();
}

bool USBDevice::read_start(usb_ep_t endpoint, uint8_t *buffer, uint32_t max_size)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return false;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    if (!(info->flags & ENDPOINT_ENABLED)) {
        // Assert that only valid endpoints are used when in the configured state
        MBED_ASSERT(!configured());
        unlock();
        return false;
    }

    if (max_size < info->max_packet_size) {
        MBED_ASSERT(0);
        unlock();
        return false;
    }

    if (info->pending) {
        // Only allow 1 packet
        unlock();
        return false;
    }

    bool ret = _phy->endpoint_read(endpoint, buffer, info->max_packet_size);
    if (ret) {
        info->pending += 1;
    }

    unlock();

    return ret;
}

uint32_t USBDevice::read_finish(usb_ep_t endpoint)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return 0;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    if (!(info->flags & ENDPOINT_ENABLED)) {
        // Assert that only valid endpoints are used when in the configured state
        MBED_ASSERT(!configured());
        unlock();
        return 0;
    }

    uint32_t size = 0;
    size = _phy->endpoint_read_result(endpoint);
    unlock();
    return size;
}

bool USBDevice::write_start(usb_ep_t endpoint, uint8_t *buffer, uint32_t size)
{
    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return false;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    if (!(info->flags & ENDPOINT_ENABLED)) {
        // Assert that only valid endpoints are used when in the configured state
        MBED_ASSERT(!configured());
        unlock();
        return false;
    }

    if (size > info->max_packet_size) {
        // Size being written is too large
        MBED_ASSERT(0);
        unlock();
        return false;
    }

    if (info->pending) {
        // Only allow 1 packet
        unlock();
        return false;
    }

    /* Send report */
    bool ret = _phy->endpoint_write(endpoint, buffer, size);
    if (ret) {
        info->transfer_size = size;
        info->pending += 1;
    } else {
        info->transfer_size = 0;
    }

    unlock();
    return ret;
}

uint32_t USBDevice::write_finish(usb_ep_t endpoint)
{
    uint32_t ret = 0;

    lock();

    if (!EP_INDEXABLE(endpoint)) {
        MBED_ASSERT(0);
        unlock();
        return 0;
    }

    endpoint_info_t *info = &_endpoint_info[EP_TO_INDEX(endpoint)];
    if (!(info->flags & ENDPOINT_ENABLED)) {
        // Assert that only valid endpoints are used when in the configured state
        MBED_ASSERT(!configured());
        unlock();
        return 0;
    }

    ret = info->transfer_size;

    unlock();
    return ret;
}

const uint8_t *USBDevice::device_desc()
{
    uint8_t device_descriptor_temp[] = {
        DEVICE_DESCRIPTOR_LENGTH,       /* bLength */
        DEVICE_DESCRIPTOR,              /* bDescriptorType */
        LSB(USB_VERSION_2_0),           /* bcdUSB (LSB) */
        MSB(USB_VERSION_2_0),           /* bcdUSB (MSB) */
        0x00,                           /* bDeviceClass */
        0x00,                           /* bDeviceSubClass */
        0x00,                           /* bDeviceprotocol */
        (uint8_t)_max_packet_size_ep0,  /* bMaxPacketSize0 */
        (uint8_t)(LSB(vendor_id)),                 /* idVendor (LSB) */
        (uint8_t)(MSB(vendor_id)),                 /* idVendor (MSB) */
        (uint8_t)(LSB(product_id)),                /* idProduct (LSB) */
        (uint8_t)(MSB(product_id)),                /* idProduct (MSB) */
        (uint8_t)(LSB(product_release)),           /* bcdDevice (LSB) */
        (uint8_t)(MSB(product_release)),           /* bcdDevice (MSB) */
        STRING_OFFSET_IMANUFACTURER,    /* iManufacturer */
        STRING_OFFSET_IPRODUCT,         /* iProduct */
        STRING_OFFSET_ISERIAL,          /* iSerialNumber */
        0x01                            /* bNumConfigurations */
    };
    MBED_ASSERT(sizeof(device_descriptor_temp) == sizeof(device_descriptor));
    memcpy(device_descriptor, device_descriptor_temp, sizeof(device_descriptor));
    return device_descriptor;
}

const uint8_t *USBDevice::string_langid_desc()
{
    static const uint8_t string_langid_descriptor[] = {
        0x04,               /*bLength*/
        STRING_DESCRIPTOR,  /*bDescriptorType 0x03*/
        0x09, 0x04,         /*bString Lang ID - 0x0409 - English*/
    };
    return string_langid_descriptor;
}

const uint8_t *USBDevice::string_imanufacturer_desc()
{
    static const uint8_t string_imanufacturer_descriptor[] = {
        0x12,                                            /*bLength*/
        STRING_DESCRIPTOR,                               /*bDescriptorType 0x03*/
        'm', 0, 'b', 0, 'e', 0, 'd', 0, '.', 0, 'o', 0, 'r', 0, 'g', 0, /*bString iManufacturer - mbed.org*/
    };
    return string_imanufacturer_descriptor;
}

const uint8_t *USBDevice::string_iserial_desc()
{
    static const uint8_t string_iserial_descriptor[] = {
        0x16,                                                           /*bLength*/
        STRING_DESCRIPTOR,                                              /*bDescriptorType 0x03*/
        '0', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0, '9', 0, /*bString iSerial - 0123456789*/
    };
    return string_iserial_descriptor;
}

const uint8_t *USBDevice::string_iconfiguration_desc()
{
    static const uint8_t string_iconfiguration_descriptor[] = {
        0x06,               /*bLength*/
        STRING_DESCRIPTOR,  /*bDescriptorType 0x03*/
        '0', 0, '1', 0,     /*bString iConfiguration - 01*/
    };
    return string_iconfiguration_descriptor;
}

const uint8_t *USBDevice::string_iinterface_desc()
{
    static const uint8_t string_iinterface_descriptor[] = {
        0x08,               /*bLength*/
        STRING_DESCRIPTOR,  /*bDescriptorType 0x03*/
        'U', 0, 'S', 0, 'B', 0, /*bString iInterface - USB*/
    };
    return string_iinterface_descriptor;
}

const uint8_t *USBDevice::string_iproduct_desc()
{
    static const uint8_t string_iproduct_descriptor[] = {
        0x16,                                                       /*bLength*/
        STRING_DESCRIPTOR,                                          /*bDescriptorType 0x03*/
        'U', 0, 'S', 0, 'B', 0, ' ', 0, 'D', 0, 'E', 0, 'V', 0, 'I', 0, 'C', 0, 'E', 0 /*bString iProduct - USB DEVICE*/
    };
    return string_iproduct_descriptor;
}

void USBDevice::start_process()
{
    lock();

    _phy->process();

    unlock();
}

void USBDevice::lock()
{
    core_util_critical_section_enter();
    _locked++;
    MBED_ASSERT(_locked > 0);
}

void USBDevice::unlock()
{
    if (_locked == 1) {
        // Perform post processing before fully unlocking
        while (_post_process != NULL) {
            void (USBDevice::*call)() = _post_process;
            _post_process = NULL;
            (this->*call)();
        }
    }

    MBED_ASSERT(_locked > 0);
    _locked--;
    core_util_critical_section_exit();
}

void USBDevice::assert_locked()
{
    MBED_ASSERT(_locked > 0);
}

void USBDevice::_change_state(DeviceState new_state)
{
    assert_locked();

    DeviceState old_state = _device.state;
    _device.state = new_state;

    if (old_state == new_state) {
        return;
    }

    bool leaving_configured_state = (old_state >= Configured) && (new_state < Configured);
    bool leaving_default_state = (old_state >= Default) && (new_state < Default);

    if (leaving_configured_state) {
        memset(_endpoint_info, 0, sizeof(_endpoint_info));
        _device.configuration = 0;
        _endpoint_add_remove_allowed = false;
    }

    if (leaving_default_state) {
        /* Abort any pending control transfers */
        _control_abort_start();
    }

    callback_state_change(new_state);
}

void USBDevice::_run_later(void (USBDevice::*function)())
{
    _post_process = function;
}
