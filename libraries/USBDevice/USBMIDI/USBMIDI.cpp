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
#include "USBMIDI.h"


USBMIDI::USBMIDI(uint16_t vendor_id, uint16_t product_id, uint16_t product_release): USBDevice(vendor_id, product_id, product_release) {
    midi_evt = NULL;
    USBDevice::connect();
}

void USBMIDI::write(MIDIMessage m) {
    USBDevice::write(EPBULK_IN, m.data, 4, MAX_PACKET_SIZE_EPBULK);
}


void USBMIDI::attach(void (*fptr)(MIDIMessage)) {
    midi_evt = fptr;
}


bool USBMIDI::EPBULK_OUT_callback() {
    uint8_t buf[64];
    uint32_t len;
    readEP(EPBULK_OUT, buf, &len, 64);

    if (midi_evt != NULL) {
        for (uint32_t i=0; i<len; i+=4) {
            midi_evt(MIDIMessage(buf+i));
        }
    }

    // We reactivate the endpoint to receive next characters
    readStart(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);
    return true;
}



// Called in ISR context
// Set configuration. Return false if the
// configuration is not supported.
bool USBMIDI::USBCallback_setConfiguration(uint8_t configuration) {
    if (configuration != DEFAULT_CONFIGURATION) {
        return false;
    }

    // Configure endpoints > 0
    addEndpoint(EPBULK_IN, MAX_PACKET_SIZE_EPBULK);
    addEndpoint(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);

    // We activate the endpoint to be able to receive data
    readStart(EPBULK_OUT, MAX_PACKET_SIZE_EPBULK);
    return true;
}


uint8_t * USBMIDI::stringIinterfaceDesc() {
    static uint8_t stringIinterfaceDescriptor[] = {
        0x0c,                           //bLength
        STRING_DESCRIPTOR,              //bDescriptorType 0x03
        'A',0,'u',0,'d',0,'i',0,'o',0   //bString iInterface - Audio
    };
    return stringIinterfaceDescriptor;
}

uint8_t * USBMIDI::stringIproductDesc() {
    static uint8_t stringIproductDescriptor[] = {
        0x16,                                                       //bLength
        STRING_DESCRIPTOR,                                          //bDescriptorType 0x03
        'M',0,'b',0,'e',0,'d',0,' ',0,'A',0,'u',0,'d',0,'i',0,'o',0 //bString iProduct - Mbed Audio
    };
    return stringIproductDescriptor;
}


uint8_t * USBMIDI::configurationDesc() {
    static uint8_t configDescriptor[] = {
        // configuration descriptor
        0x09, 0x02, 0x65, 0x00, 0x02, 0x01, 0x00, 0xc0, 0x50,

        // The Audio Interface Collection
        0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, // Standard AC Interface Descriptor
        0x09, 0x24, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x01, // Class-specific AC Interface Descriptor
        0x09, 0x04, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x00, // MIDIStreaming Interface Descriptors
        0x07, 0x24, 0x01, 0x00, 0x01, 0x41, 0x00,             // Class-Specific MS Interface Header Descriptor

        // MIDI IN JACKS
        0x06, 0x24, 0x02, 0x01, 0x01, 0x00,
        0x06, 0x24, 0x02, 0x02, 0x02, 0x00,

        // MIDI OUT JACKS
        0x09, 0x24, 0x03, 0x01, 0x03, 0x01, 0x02, 0x01, 0x00,
        0x09, 0x24, 0x03, 0x02, 0x06, 0x01, 0x01, 0x01, 0x00,

        // OUT endpoint descriptor
        0x09, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
        0x05, 0x25, 0x01, 0x01, 0x01,

        // IN endpoint descriptor
        0x09, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
        0x05, 0x25, 0x01, 0x01, 0x03,
    };
    return configDescriptor;
}
