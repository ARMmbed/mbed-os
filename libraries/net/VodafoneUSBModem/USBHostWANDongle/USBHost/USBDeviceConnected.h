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

#ifndef USBDEVICECONNECTED_H
#define USBDEVICECONNECTED_H

#include "stdint.h"
#include "USBEndpoint.h"

#define MAX_ENDPOINT_PER_INTERFACE  2
#define MAX_INTF                    2

typedef struct {
    bool in_use;
    uint8_t nb_endpoint;
    uint8_t intf_class;
    uint8_t intf_subclass;
    uint8_t intf_protocol;
    USBEndpoint * ep[MAX_ENDPOINT_PER_INTERFACE];
    FunctionPointer detach;
}INTERFACE;


class USBDeviceConnected {
public:

    /*
    * Constructor
    */
    USBDeviceConnected();

    /*
    * Attach an USBEndpoint to this device
    *
    * @param ep pointeur on the USBEndpoint which will be attached
    * @returns true if successful, false otherwise
    */
    bool addEndpoint(uint8_t intf_nb, USBEndpoint * ep);

    /*
    * Retrieve an USBEndpoint by its TYPE and DIRECTION
    *
    * @param intf_nb the interface on which to lookup the USBEndpoint
    * @param type type of the USBEndpoint looked for
    * @param direction of the USBEndpoint looked for
    * @param index the index of the USBEndpoint whitin the interface
    * @returns pointer on the USBEndpoint if found, NULL otherwise
    */
    USBEndpoint * getEndpoint(uint8_t intf_nb, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir, uint8_t index = 0);
    
    /*
    * Retrieve an USBEndpoint by its index
    *
    * @param index index of the USBEndpoint
    * @returns pointer on the USBEndpoint if found, NULL otherwise
    */
    USBEndpoint * getEndpoint(uint8_t intf_nb, uint8_t index);
    
    bool addInterface(uint8_t intf_nb, uint8_t intf_class, uint8_t intf_subclass, uint8_t intf_protocol);
    
    uint8_t getNbInterface() {return nb_interf;};
    
    INTERFACE * getInterface(uint8_t index);

    /**
     *  Attach a member function to call when a the device has been disconnected
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     */
    template<typename T>
    void onDisconnect(uint8_t intf_nb, T* tptr, void (T::*mptr)(void)) {
        if ((mptr != NULL) && (tptr != NULL)) {
            intf[intf_nb].detach.attach(tptr, mptr);
        }
    }

    /**
     * Attach a callback called when the device has been disconnected
     *
     * @param fptr function pointer
     */
    void onDisconnect(uint8_t intf_nb, void (*fn)(void)) {
        if (fn != NULL) {
            intf[intf_nb].detach.attach(fn);
        }
    }

    /*
    * Disconnect the device by calling a callback function registered by a driver
    */
    void disconnect();

    /*
    * Setters
    */
    void init(uint8_t hub, uint8_t port, bool lowSpeed);
    void setAddress(uint8_t addr) {
        this->addr = addr;
    };
    void setVid(uint16_t vid) {
        this->vid = vid;
    };
    void setPid(uint16_t pid) {
        this->pid = pid;
    };
    void setClass(uint8_t device_class) {
        this->device_class = device_class;
    };
    void setSubClass(uint8_t device_subclass) {
        this->device_subclass = device_subclass;
    };
    void setProtocol(uint8_t pr) {
        proto = pr;
    };
    void setSizeControlEndpoint(uint32_t size) {
        sizeControlEndpoint = size;
    };
    void activeAddress() {
        activeAddr = true;
    };
    void setEnumerated() {
        enumerated = true;
    };

    /*
    * Getters
    */
    uint8_t     getPort() {
        return port;
    };
    uint8_t     getHub() {
        return hub;
    };
    uint8_t     getAddress() {
        return addr;
    };
    uint16_t    getVid() {
        return vid;
    };
    uint16_t    getPid() {
        return pid;
    };
    uint8_t     getClass() {
        return device_class;
    };
    uint8_t     getSubClass() {
        return device_subclass;
    };
    uint8_t     getProtocol() {
        return proto;
    };
    bool        getSpeed() {
        return speed;
    };
    uint32_t    getSizeControlEndpoint() {
        return sizeControlEndpoint;
    };
    bool        isActiveAddress() {
        return activeAddr;
    };
    bool        isEnumerated() {
        return enumerated;
    };


private:
    INTERFACE intf[MAX_INTF];
    //USBEndpoint * ep[MAX_ENDPOINT_PER_DEVICE];
    uint32_t sizeControlEndpoint;
    uint8_t hub;
    uint8_t port;
    uint16_t vid;
    uint16_t pid;
    uint8_t addr;
    uint8_t device_class;
    uint8_t device_subclass;
    uint8_t proto;
    bool speed;
    bool activeAddr;
    bool enumerated;
    
    uint8_t nb_interf;

    
    void init();

};

#endif
