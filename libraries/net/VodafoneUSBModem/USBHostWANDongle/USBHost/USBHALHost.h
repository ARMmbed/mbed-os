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

#ifndef USBHALHOST_H
#define USBHALHOST_H

#include "rtos.h"

#include "USBHostTypes.h"

#define MAX_ENDPOINT 5
#define MAX_TD (MAX_ENDPOINT*2)

#define USBHALHOST_SIG_INIT 0x01
#define USBHALHOST_SIG_IRQ 0x02

class USBHALHost {
public:

    /*
    * Constructor
    *     init variables and memory where will be stored HCCA, ED and TD
    */
    USBHALHost();
    
    /*
    * Initialize host controller. Enable USB interrupts. This part is not in the constructor because, 
    * this function calls a virtual method if a device is already connected
    */
    void init();
    
    /*
    * reset a port of a specific hub
    * TODO: support hub
    */
    void resetPort(int hub, int port);
    
    /*
    * return the value contained in the control HEAD ED register
    *
    * @returns address of the control Head ED
    */
    uint32_t controlHeadED();
    
    /*
    * return the value contained in the bulk HEAD ED register
    *
    * @returns address of the bulk head ED
    */
    uint32_t bulkHeadED();
    
    /*
    * return the value of the head interrupt ED contained in the HCCA
    *
    * @returns address of the head interrupt ED contained in the HCCA
    */
    uint32_t interruptHeadED();
    
    
    /*
    * Update the head ED for control transfers
    */
    void updateControlHeadED(uint32_t addr);
    
    /*
    * Update the head ED for bulk transfers
    */
    void updateBulkHeadED(uint32_t addr);
    
    /*
    * Update the head ED for interrupt transfers
    */
    void updateInterruptHeadED(uint32_t addr);
    
    /* 
    * Enable control list ED
    */
    void enableControlList();
    
    /* 
    * Enable bulk list ED
    */
    void enableBulkList();
    
    /* 
    * Enable Interrupt list ED
    */
    void enableInterruptList();
    
    /*
    * Disable control list ED
    */
    bool disableControlList();

    /*
    * Disable bulk list ED
    */
    bool disableBulkList();

    /*
    * Disable Interrupt list ED
    */
    bool disableInterruptList();

    //Lock processing
    void lock();

    void unlock();


protected:
    /*
    * Virtual method called when a device has been connected
    *
    * @param hub hub number of the device
    * @param port port number of the device
    * @param lowSpeed 1 if low speed, 0 otherwise
    */
    virtual void deviceConnected(int hub, int port, bool lowSpeed) {};
    
    /*
    * Virtuel method called when a device has been disconnected
    *
    * @param hub hub number of the device
    * @param port port number of the device
    * @param addr list of the TDs which have been completed to dequeue freed TDs
    */
    virtual void deviceDisconnected(int hub, int port, volatile uint32_t addr) {};
    
    /*
    * Virtual method called when a transfer has been completed
    *
    * @param addr list of the TDs which have been completed
    */
    virtual void transferCompleted(volatile uint32_t addr){};
    
    /*
    * Find a memory section for a new ED
    *
    * @returns the address of this section
    */
    volatile uint8_t * getED();
    
    /*
    * Find a memory section for a new TD
    *
    * @returns the address of this section
    */
    volatile uint8_t * getTD();
    
    /*
    * Release a previous memory section reserved for an ED
    *
    * @param ed address of the ED
    */
    void freeED(volatile uint8_t * ed);
    
    /*
    * Release a previous memory section reserved for an TD
    *
    * @param ed address of the TD
    */
    void freeTD(volatile uint8_t * td);
    

private:
    static void _usbisr(void);
    void UsbIrqhandler();

    void memInit();

    void process();
    
    static void staticCb(void const* p);

    HCCA volatile * usb_hcca;       //256 bytes aligned!
    uint8_t volatile  * usb_edBuf;   //4 bytes aligned!
    uint8_t volatile  * usb_tdBuf;   //4 bytes aligned!

    static USBHALHost * instHost;
    
    bool volatile  edBufAlloc[MAX_ENDPOINT];
    bool volatile tdBufAlloc[MAX_TD];
    
    //RTOS impl
    Thread thread;
    Mutex mtx;

};

#endif
