/* mbed USBHost Library
 * Copyright (c) 2006-2013 ARM Limited
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

#include "USBHostSerial.h"

#if USBHOST_SERIAL

#include "dbg.h"

#define SET_LINE_CODING 0x20

USBHostSerial::USBHostSerial(): circ_buf() {
    host = USBHost::getHostInst();
    size_bulk_in = 0;
    size_bulk_out = 0;
    init();
}

void USBHostSerial::init() {
    dev = NULL;
    bulk_in = NULL;
    bulk_out = NULL;
    dev_connected = false;
    serial_intf = -1;
    serial_device_found = false;
    line_coding.baudrate = 9600;
    line_coding.data_bits = 8;
    line_coding.parity = None;
    line_coding.stop_bits = 1;
    circ_buf.flush();
}

bool USBHostSerial::connected()
{
    return dev_connected;
}

bool USBHostSerial::connect() {

    if (dev_connected) {
        return true;
    }
    for (uint8_t i = 0; i < MAX_DEVICE_CONNECTED; i++) {
        if ((dev = host->getDevice(i)) != NULL) {
            
            USB_DBG("Trying to connect serial device\r\n");

            if(host->enumerate(dev, this))
                break;
            
            if (serial_device_found) {
                bulk_in = dev->getEndpoint(serial_intf, BULK_ENDPOINT, IN);
                bulk_out = dev->getEndpoint(serial_intf, BULK_ENDPOINT, OUT);
                
                if (!bulk_in || !bulk_out)
                    break;
                
                USB_INFO("New Serial device: VID:%04x PID:%04x [dev: %p - intf: %d]", dev->getVid(), dev->getPid(), dev, serial_intf);
                dev->setName("Serial", serial_intf);
                host->registerDriver(dev, serial_intf, this, &USBHostSerial::init);
                
                baud(9600);
                
                size_bulk_in = bulk_in->getSize();
                size_bulk_out = bulk_out->getSize();
                
                bulk_in->attach(this, &USBHostSerial::rxHandler);
                bulk_out->attach(this, &USBHostSerial::txHandler);
                
                host->bulkRead(dev, bulk_in, buf, size_bulk_in, false);
                dev_connected = true;
                return true;
            }
        }
    }
    init();
    return false;
}

void USBHostSerial::rxHandler() {
    if (bulk_in) {
        int len = bulk_in->getLengthTransferred();
        if (bulk_in->getState() == USB_TYPE_IDLE) {
            for (int i = 0; i < len; i++) {
                circ_buf.queue(buf[i]);
            }
            rx.call();
            host->bulkRead(dev, bulk_in, buf, size_bulk_in, false);
        }
    }
}

void USBHostSerial::txHandler() {
    if (bulk_out) {
        if (bulk_out->getState() == USB_TYPE_IDLE) {
            tx.call();
        }
    }
}

int USBHostSerial::_putc(int c) {
    if (bulk_out) {
        if (host->bulkWrite(dev, bulk_out, (uint8_t *)&c, 1) == USB_TYPE_OK) {
            return 1;
        }
    }
    return -1;
}

void USBHostSerial::baud(int baudrate) {
    line_coding.baudrate = baudrate;
    format(line_coding.data_bits, (Parity)line_coding.parity, line_coding.stop_bits);
}

void USBHostSerial::format(int bits, Parity parity, int stop_bits) {
    line_coding.data_bits = bits;
    line_coding.parity = parity;
    line_coding.stop_bits = (stop_bits == 1) ? 0 : 2;
    
    // set line coding
    int res = host->controlWrite(   dev,
                                    USB_RECIPIENT_INTERFACE | USB_HOST_TO_DEVICE | USB_REQUEST_TYPE_CLASS,
                                    SET_LINE_CODING,
                                    0, serial_intf, (uint8_t *)&line_coding, 7);
}

int USBHostSerial::_getc() {
    uint8_t c = 0;
    if (bulk_in == NULL) {
        init();
        return -1;
    }
    while (circ_buf.isEmpty());
    circ_buf.dequeue(&c);
    return c;
}


uint8_t USBHostSerial::available() {
    return circ_buf.available();
}

/*virtual*/ void USBHostSerial::setVidPid(uint16_t vid, uint16_t pid)
{
    // we don't check VID/PID for MSD driver
}

/*virtual*/ bool USBHostSerial::parseInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) //Must return true if the interface should be parsed
{
    if ((serial_intf == -1) &&
        (intf_class == SERIAL_CLASS) &&
        (intf_subclass == 0x00) &&
        (intf_protocol == 0x00)) {
        serial_intf = intf_nb;
        return true;
    }
    return false;
}

/*virtual*/ bool USBHostSerial::useEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir) //Must return true if the endpoint will be used
{
    if (intf_nb == serial_intf) {
        if (type == BULK_ENDPOINT) {
            serial_device_found = true;
            return true;
        }
    }
    return false;
}

#endif
