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

#ifndef USBCDC_H
#define USBCDC_H

/* These headers are included for child class. */
#include "USBEndpoints.h"
#include "USBDescriptor.h"
#include "USBDevice_Types.h"

#include "USBDevice.h"

class USBCDC: public USBDevice {
public:

    /*
    * Constructor
    *
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your preoduct_release
    */
    USBCDC(uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

protected:
    
    /*
    * Get device descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
    *
    * @returns pointer to the device descriptor
    */
    virtual uint8_t * deviceDesc();
    
    /*
    * Get string product descriptor
    *
    * @returns pointer to the string product descriptor
    */
    virtual uint8_t * stringIproductDesc();
    
    /*
    * Get string interface descriptor
    *
    * @returns pointer to the string interface descriptor
    */
    virtual uint8_t * stringIinterfaceDesc();
    
    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
    virtual uint8_t * configurationDesc();
    
    /*
    * Send a buffer
    *
    * @param endpoint endpoint which will be sent the buffer
    * @param buffer buffer to be sent
    * @param size length of the buffer
    * @returns true if successful
    */
    bool send(uint8_t * buffer, uint32_t size);
    
    /*
    * Read a buffer from a certain endpoint. Warning: blocking
    *
    * @param endpoint endpoint to read
    * @param buffer buffer where will be stored bytes
    * @param size the number of bytes read will be stored in *size
    * @param maxSize the maximum length that can be read
    * @returns true if successful
    */
    bool readEP(uint8_t * buffer, uint32_t * size);
    
    /*
    * Read a buffer from a certain endpoint. Warning: non blocking
    *
    * @param endpoint endpoint to read
    * @param buffer buffer where will be stored bytes
    * @param size the number of bytes read will be stored in *size
    * @param maxSize the maximum length that can be read
    * @returns true if successful
    */
    bool readEP_NB(uint8_t * buffer, uint32_t * size);
    
protected:
    virtual bool USBCallback_request();
    virtual bool USBCallback_setConfiguration(uint8_t configuration);
    volatile bool terminal_connected;

};

#endif
