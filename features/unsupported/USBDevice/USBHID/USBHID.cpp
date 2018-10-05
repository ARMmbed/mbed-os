/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stdint.h"
#include "USBHAL.h"
#include "USBHID.h"


USBHID::USBHID(uint8_t output_report_length, uint8_t input_report_length, uint16_t vendor_id, uint16_t product_id, uint16_t product_release, bool connect): USBDevice(vendor_id, product_id, product_release)
{
    output_length = output_report_length;
    input_length = input_report_length;
    if(connect) {
        USBDevice::connect();
    }
}


bool USBHID::send(HID_REPORT *report)
{
    return write(EPINT_IN, report->data, report->length, MAX_HID_REPORT_SIZE);
}

bool USBHID::sendNB(HID_REPORT *report)
{
    return writeNB(EPINT_IN, report->data, report->length, MAX_HID_REPORT_SIZE);
}


bool USBHID::read(HID_REPORT *report)
{
    uint32_t bytesRead = 0;
    bool result;
    result = USBDevice::readEP(EPINT_OUT, report->data, &bytesRead, MAX_HID_REPORT_SIZE);
    if(!readStart(EPINT_OUT, MAX_HID_REPORT_SIZE))
        return false;
    report->length = bytesRead;
    return result;
}


bool USBHID::readNB(HID_REPORT *report)
{
    uint32_t bytesRead = 0;
    bool result;
    result = USBDevice::readEP_NB(EPINT_OUT, report->data, &bytesRead, MAX_HID_REPORT_SIZE);
    // if readEP_NB did not succeed, does not issue a readStart
    if (!result)
        return false;
    report->length = bytesRead;
    if(!readStart(EPINT_OUT, MAX_HID_REPORT_SIZE))
        return false;
    return result;
}


uint16_t USBHID::reportDescLength() {
    reportDesc();
    return reportLength;
}



//
//  Route callbacks from lower layers to class(es)
//


// Called in ISR context
// Called by USBDevice on Endpoint0 request
// This is used to handle extensions to standard requests
// and class specific requests
// Return true if class handles this request
bool USBHID::USBCallback_request() {
    bool success = false;
    CONTROL_TRANSFER * transfer = getTransferPtr();
    uint8_t *hidDescriptor;

    // Process additional standard requests

    if ((transfer->setup.bmRequestType.Type == STANDARD_TYPE))
    {
        switch (transfer->setup.bRequest)
        {
            case GET_DESCRIPTOR:
                switch (DESCRIPTOR_TYPE(transfer->setup.wValue))
                {
                    case REPORT_DESCRIPTOR:
                        if ((reportDesc() != NULL) \
                            && (reportDescLength() != 0))
                        {
                            transfer->remaining = reportDescLength();
                            transfer->ptr = (uint8_t*)reportDesc();
                            transfer->direction = DEVICE_TO_HOST;
                            success = true;
                        }
                        break;
                    case HID_DESCRIPTOR:
                            // Find the HID descriptor, after the configuration descriptor
                            hidDescriptor = findDescriptor(HID_DESCRIPTOR);
                            if (hidDescriptor != NULL)
                            {
                                transfer->remaining = HID_DESCRIPTOR_LENGTH;
                                transfer->ptr = hidDescriptor;
                                transfer->direction = DEVICE_TO_HOST;
                                success = true;
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

    if (transfer->setup.bmRequestType.Type == CLASS_TYPE)
    {
        switch (transfer->setup.bRequest)
        {
             case SET_REPORT:
                // First byte will be used for report ID
                outputReport.data[0] = transfer->setup.wValue & 0xff;
                outputReport.length = transfer->setup.wLength + 1;

                transfer->remaining = sizeof(outputReport.data) - 1;
                transfer->ptr = &outputReport.data[1];
                transfer->direction = HOST_TO_DEVICE;
                transfer->notify = true;
                success = true;
            default:
                break;
        }
    }

    return success;
}


#define DEFAULT_CONFIGURATION (1)


// Called in ISR context
// Set configuration. Return false if the
// configuration is not supported
bool USBHID::USBCallback_setConfiguration(uint8_t configuration) {
    if (configuration != DEFAULT_CONFIGURATION) {
        return false;
    }

    // Configure endpoints > 0
    addEndpoint(EPINT_IN, MAX_PACKET_SIZE_EPINT);
    addEndpoint(EPINT_OUT, MAX_PACKET_SIZE_EPINT);

    // We activate the endpoint to be able to recceive data
    readStart(EPINT_OUT, MAX_PACKET_SIZE_EPINT);
    return true;
}


const uint8_t * USBHID::stringIinterfaceDesc() {
    static const uint8_t stringIinterfaceDescriptor[] = {
        0x08,               //bLength
        STRING_DESCRIPTOR,  //bDescriptorType 0x03
        'H',0,'I',0,'D',0,  //bString iInterface - HID
    };
    return stringIinterfaceDescriptor;
}

const uint8_t * USBHID::stringIproductDesc() {
    static const uint8_t stringIproductDescriptor[] = {
        0x16,                                                       //bLength
        STRING_DESCRIPTOR,                                          //bDescriptorType 0x03
        'H',0,'I',0,'D',0,' ',0,'D',0,'E',0,'V',0,'I',0,'C',0,'E',0 //bString iProduct - HID device
    };
    return stringIproductDescriptor;
}



const uint8_t * USBHID::reportDesc() {
    uint8_t reportDescriptorTemp[] = {
        USAGE_PAGE(2), LSB(0xFFAB), MSB(0xFFAB),
        USAGE(2), LSB(0x0200), MSB(0x0200),
        COLLECTION(1), 0x01, // Collection (Application)

        REPORT_SIZE(1), 0x08, // 8 bits
        LOGICAL_MINIMUM(1), 0x00,
        LOGICAL_MAXIMUM(1), 0xFF,

        REPORT_COUNT(1), input_length,
        USAGE(1), 0x01,
        INPUT(1), 0x02, // Data, Var, Abs

        REPORT_COUNT(1), output_length,
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

const uint8_t * USBHID::configurationDesc() {
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
        (uint8_t)(LSB(reportDescLength())), // wDescriptorLength (LSB)
        (uint8_t)(MSB(reportDescLength())), // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        PHY_TO_DESC(EPINT_IN),              // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        LSB(MAX_PACKET_SIZE_EPINT),         // wMaxPacketSize (LSB)
        MSB(MAX_PACKET_SIZE_EPINT),         // wMaxPacketSize (MSB)
        1,                                  // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH,         // bLength
        ENDPOINT_DESCRIPTOR,                // bDescriptorType
        PHY_TO_DESC(EPINT_OUT),             // bEndpointAddress
        E_INTERRUPT,                        // bmAttributes
        LSB(MAX_PACKET_SIZE_EPINT),         // wMaxPacketSize (LSB)
        MSB(MAX_PACKET_SIZE_EPINT),         // wMaxPacketSize (MSB)
        1,                                  // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(configurationDescriptorTemp) == sizeof(configurationDescriptor));
    memcpy(configurationDescriptor, configurationDescriptorTemp, sizeof(configurationDescriptor));
    return configurationDescriptor;
}
