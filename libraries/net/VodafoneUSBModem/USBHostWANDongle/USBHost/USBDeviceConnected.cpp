/* Copyright (c) 2010-2012 mbed.org, MIT License
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

#include "USBDeviceConnected.h"

USBDeviceConnected::USBDeviceConnected() {
    init();
}

void USBDeviceConnected::init() {
    hub = 0;
    port = 0;
    vid = 0;
    pid = 0;
    nb_interf = 0;
    enumerated = false;
    activeAddr = false;
    sizeControlEndpoint = 8;
    device_class = 0;
    device_subclass = 0;
    proto = 0;
    speed = false;
    for (int i = 0; i < MAX_INTF; i++) {
        memset((void *)&intf[i], 0, sizeof(INTERFACE));
        intf[i].in_use = false;
        for (int j = 0; j < MAX_ENDPOINT_PER_INTERFACE; j++) {
            intf[i].ep[j] = NULL;
        }
    }
}

INTERFACE * USBDeviceConnected::getInterface(uint8_t index) {
    if (index >= MAX_INTF) {
        return NULL;
    }
    return &intf[index];
}

bool USBDeviceConnected::addInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol) {
    if ((intf_nb >= MAX_INTF) || (intf[intf_nb].in_use)) {
        return false;
    }
    intf[intf_nb].in_use = true;
    intf[intf_nb].intf_class = intf_class;
    intf[intf_nb].intf_subclass = intf_subclass;
    intf[intf_nb].intf_protocol = intf_protocol;
    intf[intf_nb].nb_endpoint = 0;
    nb_interf++;
    return true;
}

bool USBDeviceConnected::addEndpoint(uint8_t intf_nb, USBEndpoint * ept) {
    if ((intf_nb >= MAX_INTF) || (intf[intf_nb].in_use == false) || (intf[intf_nb].nb_endpoint >= MAX_ENDPOINT_PER_INTERFACE)) {
        return false;
    }
    intf[intf_nb].nb_endpoint++;

    for (int i = 0; i < MAX_ENDPOINT_PER_INTERFACE; i++) {
        if (intf[intf_nb].ep[i] == NULL) {
            intf[intf_nb].ep[i] = ept;
            return true;
        }
    }
    return false;
}

void USBDeviceConnected::init(uint8_t hub, uint8_t port, bool lowSpeed) {
    init();
    this->hub = hub;
    this->port = port;
    this->speed = lowSpeed;
}

void USBDeviceConnected::disconnect() {
    for(int i = 0; i < nb_interf; i++) {
        intf[i].detach.call();
    }
    init();
}



USBEndpoint * USBDeviceConnected::getEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir, uint8_t index) {
    if (intf_nb >= MAX_INTF) {
        return NULL;
    }
    for (int i = 0; i < MAX_ENDPOINT_PER_INTERFACE; i++) {
        if ((intf[intf_nb].ep[i]->getType() == type) && (intf[intf_nb].ep[i]->getDir() == dir)) {
            if(index)
            {
              index--;
            }
            else
            {
              return intf[intf_nb].ep[i];
            }
        }
    }
    return NULL;
}

USBEndpoint * USBDeviceConnected::getEndpoint(uint8_t intf_nb, uint8_t index) {
    if ((intf_nb >= MAX_INTF) || (index >= MAX_ENDPOINT_PER_INTERFACE)) {
        return NULL;
    }
    return intf[intf_nb].ep[index];
}
