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

#ifndef USBENDPOINT_H
#define USBENDPOINT_H

#include "stdint.h"
#include "FunctionPointer.h"
#include "USBHostTypes.h"


enum ENDPOINT_TYPE {
    CONTROL_ENDPOINT = 0,
    ISOCHRONOUS_ENDPOINT,
    BULK_ENDPOINT,
    INTERRUPT_ENDPOINT
};

enum ENDPOINT_DIRECTION {
    OUT = 1,
    IN  
};

class USBEndpoint {
public:
    /*
    * Constructor
    */
    USBEndpoint() {state = USB_TYPE_FREE; nextEp = NULL;};
    
    /*
    * Initialize an endpoint
    *
    * @param hced hced associated to the endpoint
    * @param type endpoint type
    * @param dir endpoint direction
    * @param size endpoint size
    * @param ep_number endpoint number
    */
    void init(HCED * hced, ENDPOINT_TYPE type, ENDPOINT_DIRECTION dir, uint32_t size, uint8_t ep_number, HCTD* td_list[2]);
    
    /*
    * Set next token. Warining: only useful for the control endpoint
    *
    * @param token IN, OUT or SETUP token
    */
    void setNextToken(uint32_t token);
    
    /*
    * Queue an endpoint
    *
    * endpoint endpoint which will be queued in the linked list
    */
    void queueEndpoint(USBEndpoint * endpoint);
    
    /*
    * Get a td to be queued
    *
    * @returns td hctd which will be queued
    */
    volatile HCTD* getNextTD();

    /*
    * Queue a transfer on the endpoint
    *
    */
    void queueTransfer();
    
    /*
    * Get the currently processed td
    *
    * @returns td hctd that was queued
    */
    volatile HCTD * getProcessedTD();

    /*
    * Unqueue a transfer from the endpoint
    *
    * @param td hctd which will be unqueued
    */
    void unqueueTransfer(volatile HCTD * td);
    
    /*
    * Return the next endpoint in the linked list
    *
    * @returns next endpoint
    */
    USBEndpoint * nextEndpoint();
    
    /**
     *  Attach a member function to call when a transfer is finished
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     */
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        if((mptr != NULL) && (tptr != NULL)) {
            rx.attach(tptr, mptr);
        }
    }

    /**
     * Attach a callback called when a transfer is finished
     *
     * @param fptr function pointer
     */
    void attach(void (*fn)(void)) {
        if(fn != NULL) {
            rx.attach(fn);
        }
    }
    
    /*
    * Call the handler associted to the end of a transfer
    */
    void call() {
        rx.call();
    };
    
    
    /*
    * Setters
    */
    void setState(USB_TYPE st) {state = st;}
    void setDeviceAddress(uint8_t addr);
    void setLengthTransferred(int len);
    void setSpeed(uint8_t speed);
    void setSize(uint32_t size);
    void setDir(ENDPOINT_DIRECTION d) {dir = d;}
    
    /*
    * Getters
    */
    USB_TYPE            getState() {return state;}
    ENDPOINT_TYPE       getType();
    uint8_t             getDeviceAddress();
    int        getLengthTransferred() {return transferred;}
    uint32_t   getBufStart();
    uint32_t            getSize();
    volatile HCTD *     getHeadTD();
    volatile HCTD**     getTDList();
    volatile HCED *     getHCED();
    ENDPOINT_DIRECTION  getDir() {return dir;}
    bool                isSetup() {return setup;}
    
    
private:
    ENDPOINT_TYPE type;
    volatile USB_TYPE state;
    ENDPOINT_DIRECTION dir;
    bool setup;
    
    int transfer_len;
    int transferred;
    uint32_t buf_start;
    
    FunctionPointer rx;
    
    USBEndpoint* nextEp;
    
    // USBEndpoint descriptor
    volatile HCED * hced;
    
    volatile HCTD * td_list[2];
    volatile HCTD * td_current;
    volatile HCTD * td_next;
    /*bool carry;*/
    
    int count;
    
};

#endif
