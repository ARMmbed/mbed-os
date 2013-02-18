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

#ifndef USBHOST_H
#define USBHOST_H

#include "USBHALHost.h"
#include "USBDeviceConnected.h"
#include "USBEndpoint.h"
#include "IUSBEnumerator.h"

#define MAX_DEVICE_NB 1

// singleton class
class USBHost : public USBHALHost {
public:
    /*
    * Static method to create or retrieve the single USBHost instance
    */
    static USBHost * getHostInst();
    
    USB_TYPE getDeviceDescriptor(USBDeviceConnected * dev, uint8_t * buf) ;
    USB_TYPE getConfigurationDescriptor(USBDeviceConnected * dev, uint8_t * buf, uint16_t * len_conf_descr = NULL) ;
    USB_TYPE setConfiguration(USBDeviceConnected * dev, uint8_t conf) ;
    USB_TYPE getStringDescriptor(USBDeviceConnected * dev, uint8_t index, uint8_t * buf) ;
    USB_TYPE getReportDescriptor(USBDeviceConnected * dev, uint8_t * buf, uint8_t len) ;

    /*
    * Control read: setup stage, data stage and status stage
    *
    * @param dev the control read will be done for this device
    * @param requestType request type
    * @param request request
    * @param value value
    * @param index index
    * @param buf pointer on a buffer where will be store the data received
    * @param len length of the transfer
    *
    * @returns status of the control read
    */
    USB_TYPE controlRead(USBDeviceConnected * dev, uint8_t requestType, uint8_t request, uint32_t value, uint32_t index, uint8_t * buf, uint32_t len) ;

    /*
    * Control write: setup stage, data stage and status stage
    *
    * @param dev the control write will be done for this device
    * @param requestType request type
    * @param request request
    * @param value value
    * @param index index
    * @param buf pointer on a buffer which will be written
    * @param len length of the transfer
    *
    * @returns status of the control write
    */
    USB_TYPE controlWrite(USBDeviceConnected * dev, uint8_t requestType, uint8_t request, uint32_t value, uint32_t index, uint8_t * buf, uint32_t len) ;


    /*
    * Bulk read
    *
    * @param dev the bulk transfer will be done for this device
    * @param ep USBEndpoint which will be used to read a packet
    * @param buf pointer on a buffer where will be store the data received
    * @param len length of the transfer
    * @param blocking if true, the read is blocking (wait for completion)
    *
    * @returns status of the bulk read
    */
    USB_TYPE bulkRead(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking = true) ;

    /*
    * Bulk write
    *
    * @param dev the bulk transfer will be done for this device
    * @param ep USBEndpoint which will be used to write a packet
    * @param buf pointer on a buffer which will be written
    * @param len length of the transfer
    * @param blocking if true, the write is blocking (wait for completion)
    *
    * @returns status of the bulk write
    */
    USB_TYPE bulkWrite(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking = true) ;

    /*
    * Interrupt read
    *
    * @param dev the bulk transfer will be done for this device
    * @param ep USBEndpoint which will be used to write a packet
    * @param buf pointer on a buffer which will be written
    * @param len length of the transfer
    * @param blocking if true, the read is blocking (wait for completion)
    *
    * @returns status of the interrupt read
    */
    USB_TYPE interruptRead(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking = true) ;

    /*
    * Interrupt write
    *
    * @param dev the bulk transfer will be done for this device
    * @param ep USBEndpoint which will be used to write a packet
    * @param buf pointer on a buffer which will be written
    * @param len length of the transfer
    * @param blocking if true, the write is blocking (wait for completion)
    *
    * @returns status of the interrupt write
    */
    USB_TYPE interruptWrite(USBDeviceConnected * dev, USBEndpoint * ep, uint8_t * buf, uint32_t len, bool blocking = true) ;

    /*
    * Enumerate a device. This method is responsible for:
    *   - set the address of the device
    *   - fill a USBDeviceConnected object:
    *       - add interfaces, endpoints, ...
    *   - set a configuration
    *
    * @param dev device which will be enumerated
    *
    * @returns status of the enumeration
    */
    USB_TYPE enumerate(USBDeviceConnected * dev, IUSBEnumerator* pEnumerator) ;

    /*
    * Get a device
    *
    * @param index index of the device which will be returned
    *
    * @returns pointer on the "index" device
    */
    USBDeviceConnected * getDevice(uint8_t index) ;

    /*
    * reset port and hub of a specific device
    *
    * @param pointer on the device hich will be reseted
    */
    USB_TYPE resetDevice(USBDeviceConnected * dev) ;

    /*
    * If there is a HID device connected, the host stores the length of the report descriptor.
    * This avoid to the driver to re-ask the configuration descriptor to request the report descriptor
    *
    * @returns length of the report descriptor
    */
    uint16_t getLengthReportDescr()  {
        return lenReportDescr;
    };

    /**
     *  register a driver into the host associated with a callback function called when the device is disconnected
     *
     *  @param dev device
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     */
    template<typename T>
    void registerDriver(USBDeviceConnected * dev, uint8_t intf, T* tptr, void (T::*mptr)(void))  {
        int index = findDevice(dev);
        if ((index != -1) && (mptr != NULL) && (tptr != NULL)) {
            dev->onDisconnect(intf, tptr, mptr);
        }
    }

    /**
     * register a driver into the host associated with a callback function called when the device is disconnected
     *
     * @param dev device
     * @param fn callback called when the specified device has been disconnected
     */
    void registerDriver(USBDeviceConnected * dev, uint8_t intf, void (*fn)(void))  {
        int index = findDevice(dev);
        if ((index != -1) && (fn != NULL)) {
            dev->onDisconnect(intf, fn);
        }
    }


protected:

    /*
    * Virtual method called when a device has been connected
    *
    * @param hub hub number of the device
    * @param port port number of the device
    * @param lowSpeed 1 if low speed, 0 otherwise
    */
    virtual void deviceConnected(int hub, int port, bool lowSpeed) ;

    /*
    * Virtuel method called when a device has been disconnected
    *
    * @param hub hub number of the device
    * @param port port number of the device
    * @param addr list of the TDs which have been completed to dequeue freed TDs
    */
    virtual void deviceDisconnected(int hub, int port, volatile uint32_t addr) ;

    /*
    * Virtual method called when a transfer has been completed
    *
    * @param addr list of the TDs which have been completed
    */
    virtual void transferCompleted(volatile uint32_t addr) ;


private:
    // singleton class -> constructor is private
    USBHost();

    static USBHost * instHost;

    uint8_t  nb_devices;
    uint16_t  lenReportDescr;

    void fillControlBuf(uint8_t requestType, uint8_t request, uint32_t value, uint32_t index, int len) ;
    void parseConfDescr(USBDeviceConnected * dev, uint8_t * conf_descr, uint32_t len, IUSBEnumerator* pEnumerator) ;
    void freeDevice(USBDeviceConnected * dev) ;
    int findDevice(USBDeviceConnected * dev) ;


    // endpoints
    void unqueueEndpoint(USBEndpoint * ep) ;
    USBEndpoint  endpoints[MAX_ENDPOINT];
    USBEndpoint* volatile  control;

    USBEndpoint* volatile  headControlEndpoint;
    USBEndpoint* volatile  headBulkEndpoint;
    USBEndpoint* volatile  headInterruptEndpoint;

    USBEndpoint* volatile  tailControlEndpoint;
    USBEndpoint* volatile  tailBulkEndpoint;
    USBEndpoint* volatile  tailInterruptEndpoint;

    bool controlEndpointAllocated;


    // devices connected
    USBDeviceConnected  devices[MAX_DEVICE_NB];
    volatile bool  deviceInUse[MAX_DEVICE_NB];
    volatile bool  deviceReset[MAX_DEVICE_NB];
    
    /*
    * Add a transfer on the TD linked list associated to an ED
    *
    * @param ed the transfer is associated to this ed
    * @param buf pointer on a buffer where will be read/write data to send or receive
    * @param len transfer length
    *
    * @return status of the transfer
    */
    USB_TYPE addTransfer(USBEndpoint * ed, uint8_t * buf, uint32_t len) ;
    
    /*
    * Link the USBEndpoint to the linked list and attach an USBEndpoint this USBEndpoint to a device
    *
    * @param dev pointer on a USBDeviceConnected object
    * @param ep pointer on the USBEndpoint which will be added
    *
    * return true if successful
    */
    bool addEndpoint(USBDeviceConnected * dev, uint8_t intf_nb, USBEndpoint * ep) ;

    /*
    * Create an USBEndpoint descriptor. Warning: the USBEndpoint is not linked.
    *
    * @param type USBEndpoint type (CONTROL_ENDPOINT, BULK_ENDPOINT, INTERRUPT_ENDPOINT)
    * @param dir USBEndpoint direction (no meaning for CONTROL_ENDPOINT)
    * @param size USBEndpoint max packet size
    * @param addr USBEndpoint address
    *
    * @returns pointer on the USBEndpoint created
    */
    USBEndpoint * newEndpoint(ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir, uint32_t size, uint8_t addr) ;


    // to store a setup packet
    uint8_t  setupPacket[8];


    /////////////////////////
    /// FOR DEBUG
    /////////////////////////
    void printBulk();
    void printInt();

};

#endif
