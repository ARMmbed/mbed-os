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

#include "stdint.h"
#include "USBHID.h"
#include "EndpointResolver.h"
#include "usb_phy_api.h"

class USBHID::AsyncSend: public AsyncOp {
public:
    AsyncSend(USBHID *hid, const HID_REPORT *report): hid(hid), report(report), result(false)
    {

    }

    virtual ~AsyncSend()
    {

    }

    virtual bool process()
    {
        if (!hid->configured()) {
            result = false;
            return true;
        }

        if (hid->send_nb(report)) {
            result = true;
            return true;
        }

        return false;
    }

    USBHID *hid;
    const HID_REPORT *report;
    bool result;
};

class USBHID::AsyncRead: public AsyncOp {
public:
    AsyncRead(USBHID *hid, HID_REPORT *report): hid(hid), report(report), result(false)
    {

    }

    virtual ~AsyncRead()
    {

    }

    virtual bool process()
    {
        if (!hid->configured()) {
            result = false;
            return true;
        }

        if (hid->read_nb(report)) {
            result = true;
            return true;
        }

        return false;
    }

    USBHID *hid;
    HID_REPORT *report;
    bool result;
};

class USBHID::AsyncWait: public AsyncOp {
public:
    AsyncWait(USBHID *hid): hid(hid)
    {

    }

    virtual ~AsyncWait()
    {

    }

    virtual bool process()
    {
        if (hid->configured()) {
            return true;
        }

        return false;
    }

    USBHID *hid;
};

USBHID::USBHID(bool connect_blocking, uint8_t output_report_length, uint8_t input_report_length, uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
    : USBDevice(get_usb_phy(), vendor_id, product_id, product_release)
{
    _init(output_report_length, input_report_length);
    if (connect_blocking) {
        connect();
        wait_ready();
    } else {
        init();
    }

}

USBHID::USBHID(USBPhy *phy, uint8_t output_report_length, uint8_t input_report_length, uint16_t vendor_id, uint16_t product_id, uint16_t product_release)
    : USBDevice(phy, vendor_id, product_id, product_release)
{
    _init(output_report_length, input_report_length);
}

USBHID::~USBHID()
{
    deinit();
}

void USBHID::_init(uint8_t output_report_length, uint8_t input_report_length)
{
    EndpointResolver resolver(endpoint_table());

    resolver.endpoint_ctrl(64);
    _int_in = resolver.endpoint_in(USB_EP_TYPE_INT, MAX_HID_REPORT_SIZE);
    _int_out = resolver.endpoint_out(USB_EP_TYPE_INT, MAX_HID_REPORT_SIZE);
    MBED_ASSERT(resolver.valid());

    _send_idle = true;
    _read_idle = true;
    _output_length = output_report_length;
    _input_length = input_report_length;
}

bool USBHID::ready()
{
    return configured();
}

void USBHID::wait_ready()
{
    lock();

    AsyncWait wait_op(this);
    _connect_list.add(&wait_op);

    unlock();

    wait_op.wait(NULL);
}


bool USBHID::send(const HID_REPORT *report)
{
    lock();

    AsyncSend send_op(this, report);
    _send_list.add(&send_op);

    unlock();

    send_op.wait(NULL);
    return send_op.result;
}

bool USBHID::send_nb(const HID_REPORT *report)
{
    lock();

    if (!configured()) {
        unlock();
        return false;
    }

    bool success = false;
    if (_send_idle) {
        memcpy(&_input_report, report, sizeof(_input_report));
        write_start(_int_in, _input_report.data, _input_report.length);
        _send_idle = false;
        success = true;
    }

    unlock();
    return success;
}

bool USBHID::read(HID_REPORT *report)
{
    lock();

    AsyncRead read_op(this, report);
    _read_list.add(&read_op);

    unlock();

    read_op.wait(NULL);
    return read_op.result;
}


bool USBHID::read_nb(HID_REPORT *report)
{
    lock();

    if (!configured()) {
        unlock();
        return false;
    }

    bool success = false;
    if (_read_idle) {
        memcpy(report, &_output_report, sizeof(_output_report));
        read_start(_int_out, _output_report.data, MAX_HID_REPORT_SIZE);
        _read_idle = false;
        success = true;
    }

    unlock();
    return success;
}

void USBHID::_send_isr()
{
    assert_locked();

    write_finish(_int_in);
    _send_idle = true;

    _send_list.process();
    if (_send_idle) {
        report_tx();
    }

}

void USBHID::_read_isr()
{
    assert_locked();

    _output_report.length = read_finish(_int_out);
    _read_idle = true;

    _read_list.process();
    if (_read_idle) {
        report_rx();
    }
}

uint16_t USBHID::report_desc_length()
{
    report_desc();
    return reportLength;
}


void USBHID::callback_state_change(DeviceState new_state)
{
    if (new_state != Configured) {
        if (!_send_idle) {
            endpoint_abort(_int_in);
            _send_idle = true;
        }
        if (!_read_idle) {
            endpoint_abort(_int_out);
            _read_idle = true;
        }
    }
    _send_list.process();
    _read_list.process();
    _connect_list.process();
}

//
//  Route callbacks from lower layers to class(es)
//


// Called in ISR context
// Called by USBDevice on Endpoint0 request
// This is used to handle extensions to standard requests
// and class specific requests
// Return true if class handles this request
void USBHID::callback_request(const setup_packet_t *setup)
{
    uint8_t *hidDescriptor;
    RequestResult result = PassThrough;
    uint8_t *data = NULL;
    uint32_t size = 0;

    // Process additional standard requests

    if ((setup->bmRequestType.Type == STANDARD_TYPE)) {
        switch (setup->bRequest) {
            case GET_DESCRIPTOR:
                switch (DESCRIPTOR_TYPE(setup->wValue)) {
                    case REPORT_DESCRIPTOR:
                        if ((report_desc() != NULL) \
                                && (report_desc_length() != 0)) {
                            size = report_desc_length();
                            data = (uint8_t *)report_desc();
                            result = Send;
                        }
                        break;
                    case HID_DESCRIPTOR:
                        // Find the HID descriptor, after the configuration descriptor
                        hidDescriptor = find_descriptor(HID_DESCRIPTOR);
                        if (hidDescriptor != NULL) {
                            size = HID_DESCRIPTOR_LENGTH;
                            data = hidDescriptor;
                            result = Send;
                        }
                        break;

                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    // Process class-specific requests

    if (setup->bmRequestType.Type == CLASS_TYPE) {
        switch (setup->bRequest) {
            case SET_REPORT:
                // First byte will be used for report ID
                _output_report.data[0] = setup->wValue & 0xff;
                _output_report.length = setup->wLength + 1;

                size = sizeof(_output_report.data) - 1;
                data = &_output_report.data[1];
                result = Send;
                break;
            default:
                break;
        }
    }

    complete_request(result, data, size);
}

void USBHID::callback_request_xfer_done(const setup_packet_t *setup, bool aborted)
{
    (void)aborted;
    complete_request_xfer_done(true);
}


#define DEFAULT_CONFIGURATION (1)


// Called in ISR context
// Set configuration. Return false if the
// configuration is not supported
void USBHID::callback_set_configuration(uint8_t configuration)
{
    if (configuration == DEFAULT_CONFIGURATION) {
        complete_set_configuration(false);
    }

    // Configure endpoints > 0
    endpoint_add(_int_in, MAX_HID_REPORT_SIZE, USB_EP_TYPE_INT, &USBHID::_send_isr);
    endpoint_add(_int_out, MAX_HID_REPORT_SIZE, USB_EP_TYPE_INT, &USBHID::_read_isr);

    // We activate the endpoint to be able to recceive data
    read_start(_int_out, _output_report.data, MAX_HID_REPORT_SIZE);
    _read_idle = false;


    complete_set_configuration(true);
}

void USBHID::callback_set_interface(uint16_t interface, uint8_t alternate)
{
    assert_locked();
    complete_set_interface(true);
}


const uint8_t *USBHID::string_iinterface_desc()
{
    static const uint8_t stringIinterfaceDescriptor[] = {
        0x08,               //bLength
        STRING_DESCRIPTOR,  //bDescriptorType 0x03
        'H', 0, 'I', 0, 'D', 0, //bString iInterface - HID
    };
    return stringIinterfaceDescriptor;
}

const uint8_t *USBHID::string_iproduct_desc()
{
    static const uint8_t stringIproductDescriptor[] = {
        0x16,                                                       //bLength
        STRING_DESCRIPTOR,                                          //bDescriptorType 0x03
        'H', 0, 'I', 0, 'D', 0, ' ', 0, 'D', 0, 'E', 0, 'V', 0, 'I', 0, 'C', 0, 'E', 0 //bString iProduct - HID device
    };
    return stringIproductDescriptor;
}



const uint8_t *USBHID::report_desc()
{
    uint8_t reportDescriptorTemp[] = {
        USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
        USAGE(2), LSB(0x0200), MSB(0x0200),
        COLLECTION(1), 0x01, // Collection (Application)

        REPORT_SIZE(1), 0x08, // 8 bits
        LOGICAL_MINIMUM(1), 0x00,
        LOGICAL_MAXIMUM(1), 0xFF,

        REPORT_COUNT(1), _input_length,
        USAGE(1), 0x01,
        INPUT(1), 0x02, // Data, Var, Abs

        REPORT_COUNT(1), _output_length,
        USAGE(1), 0x02,
        OUTPUT(1), 0x02, // Data, Var, Abs

        END_COLLECTION(0),
    };
    reportLength = sizeof(reportDescriptor);
    MBED_ASSERT(sizeof(reportDescriptorTemp) == sizeof(reportDescriptor));
    memcpy(reportDescriptor, reportDescriptorTemp, sizeof(reportDescriptor));
    return reportDescriptor;
}

#define DEFAULT_CONFIGURATION (1)
#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) \
                               + (1 * INTERFACE_DESCRIPTOR_LENGTH) \
                               + (1 * HID_DESCRIPTOR_LENGTH) \
                               + (2 * ENDPOINT_DESCRIPTOR_LENGTH))

const uint8_t *USBHID::configuration_desc(uint8_t index)
{
    if (index != 0) {
        return NULL;
    }

    uint8_t configurationDescriptorTemp[] = {
        CONFIGURATION_DESCRIPTOR_LENGTH,    // bLength
        CONFIGURATION_DESCRIPTOR,           // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),       // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),       // wTotalLength (MSB)
        0x01,                               // bNumInterfaces
        DEFAULT_CONFIGURATION,              // bConfigurationValue
        0x00,                               // iConfiguration
        C_RESERVED | C_SELF_POWERED,        // bmAttributes
        C_POWER(0),                         // bMaxPower

        INTERFACE_DESCRIPTOR_LENGTH,        // bLength
        INTERFACE_DESCRIPTOR,               // bDescriptorType
        0x00,                               // bInterfaceNumber
        0x00,                               // bAlternateSetting
        0x02,                               // bNumEndpoints
        HID_CLASS,                          // bInterfaceClass
        HID_SUBCLASS_NONE,                  // bInterfaceSubClass
        HID_PROTOCOL_NONE,                  // bInterfaceProtocol
        0x00,                               // iInterface

        HID_DESCRIPTOR_LENGTH,              // bLength
        HID_DESCRIPTOR,                     // bDescriptorType
        LSB(HID_VERSION_1_11),              // bcdHID (LSB)
        MSB(HID_VERSION_1_11),              // bcdHID (MSB)
        0x00,                               // bCountryCode
        0x01,                               // bNumDescriptors
        REPORT_DESCRIPTOR,                  // bDescriptorType
        (uint8_t)(LSB(report_desc_length())),   // wDescriptorLength (LSB)
        (uint8_t)(MSB(report_desc_length())),   // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        _int_in,                            // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
        1,                                  // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        _int_out,                           // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),           // wMaxPacketSize (MSB)
        1,                                  // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(configurationDescriptorTemp) == sizeof(_configuration_descriptor));
    memcpy(_configuration_descriptor, configurationDescriptorTemp, sizeof(_configuration_descriptor));
    return _configuration_descriptor;
}
