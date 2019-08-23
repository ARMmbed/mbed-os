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

#ifndef USBSERIAL_H
#define USBSERIAL_H

#include "USBCDC.h"
#include "platform/Stream.h"
#include "Callback.h"

/**
 * \defgroup drivers_USBSerial USBSerial class
 * \ingroup drivers-public-api-usb
 * @{
 */

/**
* USBSerial example
*
* @code
* #include "mbed.h"
* #include "USBSerial.h"
*
* //Virtual serial port over USB
* USBSerial serial;
*
* int main(void) {
*
*    while(1)
*    {
*        serial.printf("I am a virtual serial port\n");
*        wait(1);
*    }
* }
* @endcode
*/
class USBSerial: public USBCDC, public mbed::Stream {
public:

    /**
    * Basic constructor
    *
    * Construct this object optionally connecting and blocking until it is ready.
    *
    * @note Do not use this constructor in derived classes.
    *
    * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
    * @param vendor_id Your vendor_id (default: 0x1f00)
    * @param product_id Your product_id (default: 0x2012)
    * @param product_release Your product_release (default: 0x0001)
    *
    */
    USBSerial(bool connect_blocking = true, uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012, uint16_t product_release = 0x0001);

    /**
    * Fully featured constructor
    *
    * Construct this object with the supplied USBPhy and parameters. The user
    * this object is responsible for calling connect() or init().
    *
    * @note Derived classes must use this constructor and call init() or
    * connect() themselves. Derived classes should also call deinit() in
    * their destructor. This ensures that no interrupts can occur when the
    * object is partially constructed or destroyed.
    *
    * @param phy USB phy to use
    * @param vendor_id Your vendor_id (default: 0x1f00)
    * @param product_id Your product_id (default: 0x2012)
    * @param product_release Your product_release (default: 0x0001)
    *
    */
    USBSerial(USBPhy *phy, uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012, uint16_t product_release = 0x0001);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
    virtual ~USBSerial();

    /**
    * Send a character. You can use puts, printf.
    *
    * @param c character to be sent
    * @returns true if there is no error, false otherwise
    */
    virtual int _putc(int c);

    /**
    * Read a character: blocking
    *
    * @returns character read
    */
    virtual int _getc();

    /**
    * Check the number of bytes available.
    *
    * @returns the number of bytes available
    */
    uint8_t available();

    /**
    * Check if the terminal is connected.
    *
    * @returns connection status
    */
    bool connected();

    /** Determine if there is a character available to read
     *
     *  @returns
     *    1 if there is a character available to read,
     *    0 otherwise
     */
    int readable()
    {
        return available() ? 1 : 0;
    }

    /** Determine if there is space available to write a character
     *
     *  @returns
     *    1 if there is space to write a character,
     *    0 otherwise
     */
    int writeable()
    {
        return 1;    // always return 1, for write operation is blocking
    }

    /**
     *  Attach a member function to call when a packet is received.
     *
     *  @param tptr pointer to the object to call the member function on
     *  @param mptr pointer to the member function to be called
     */
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        USBCDC::lock();

        if ((mptr != NULL) && (tptr != NULL)) {
            rx = mbed::Callback<void()>(tptr, mptr);
        }

        USBCDC::unlock();
    }

    /**
     * Attach a callback called when a packet is received
     *
     * @param fptr function pointer
     */
    void attach(void (*fptr)(void))
    {
        USBCDC::lock();

        if (fptr != NULL) {
            rx = mbed::Callback<void()>(fptr);
        }

        USBCDC::unlock();
    }

    /**
     * Attach a Callback called when a packet is received
     *
     * @param cb Callback to attach
     */
    void attach(mbed::Callback<void()> &cb)
    {
        USBCDC::lock();

        rx = cb;

        USBCDC::unlock();
    }

    /**
     * Attach a callback to call when serial's settings are changed.
     *
     * @param fptr function pointer
     */
    void attach(void (*fptr)(int baud, int bits, int parity, int stop))
    {
        USBCDC::lock();

        _settings_changed_callback = fptr;

        USBCDC::unlock();
    }

protected:
    virtual void data_rx();
    virtual void line_coding_changed(int baud, int bits, int parity, int stop)
    {
        assert_locked();

        if (_settings_changed_callback) {
            _settings_changed_callback(baud, bits, parity, stop);
        }
    }

private:
    mbed::Callback<void()> rx;
    void (*_settings_changed_callback)(int baud, int bits, int parity, int stop);
};

/** @}*/

#endif
