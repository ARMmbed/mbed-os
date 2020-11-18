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

#ifndef USBMOUSEKEYBOARD_H
#define USBMOUSEKEYBOARD_H

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_MOUSE 2
#define REPORT_ID_VOLUME 3

#include "USBMouse.h"
#include "USBKeyboard.h"
#include "platform/Stream.h"
#include "USBHID.h"
#include "PlatformMutex.h"

/**
 * \defgroup drivers_USBMouseKeyboard USBMouseKeyboard class
 * \ingroup drivers-public-api-usb
 * @{
 */

/**
 * USBMouseKeyboard example
 * @code
 *
 * #include "mbed.h"
 * #include "USBMouseKeyboard.h"
 *
 * USBMouseKeyboard key_mouse;
 *
 * int main(void)
 * {
 *   while(1)
 *   {
 *       key_mouse.move(20, 0);
 *       key_mouse.printf("Hello From MBED\r\n");
 *       ThisThread::sleep_for(1000);
 *   }
 * }
 * @endcode
 *
 *
 * @code
 *
 * #include "mbed.h"
 * #include "USBMouseKeyboard.h"
 *
 * USBMouseKeyboard key_mouse(ABS_MOUSE);
 *
 * int main(void)
 * {
 *   while(1)
 *   {
 *       key_mouse.move(X_MAX_ABS/2, Y_MAX_ABS/2);
 *       key_mouse.printf("Hello from MBED\r\n");
 *       ThisThread::sleep_for(1000);
 *   }
 * }
 * @endcode
 *
 * @note Synchronization level: Thread safe
 */
class USBMouseKeyboard: public USBHID, public mbed::Stream {
public:

    /**
    * Basic constructor
    *
    * Construct this object optionally connecting and blocking until it is ready.
    *
    * @note Do not use this constructor in derived classes.
    *
    * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
    * @param mouse_type Mouse type: ABS_MOUSE (absolute mouse) or REL_MOUSE (relative mouse) (default: REL_MOUSE)
    * @param vendor_id Your vendor_id (default: 0x1234)
    * @param product_id Your product_id (default: 0x0001)
    * @param product_release Your preoduct_release (default: 0x0001)
    *
    */
    USBMouseKeyboard(bool connect_blocking = true, MOUSE_TYPE mouse_type = REL_MOUSE, uint16_t vendor_id = 0x0021, uint16_t product_id = 0x0011, uint16_t product_release = 0x0001);

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
    * @param mouse_type Mouse type: ABS_MOUSE (absolute mouse) or REL_MOUSE (relative mouse) (default: REL_MOUSE)
    * @param vendor_id Your vendor_id (default: 0x1234)
    * @param product_id Your product_id (default: 0x0001)
    * @param product_release Your preoduct_release (default: 0x0001)
    *
    */
    USBMouseKeyboard(USBPhy *phy, MOUSE_TYPE mouse_type = REL_MOUSE, uint16_t vendor_id = 0x0021, uint16_t product_id = 0x0011, uint16_t product_release = 0x0001);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
    virtual ~USBMouseKeyboard();

    /**
    * Write a state of the mouse
    *
    * @param x x-axis position
    * @param y y-axis position
    * @param buttons buttons state (first bit represents MOUSE_LEFT, second bit MOUSE_RIGHT and third bit MOUSE_MIDDLE)
    * @param z wheel state (>0 to scroll down, <0 to scroll up)
    * @returns true if there is no error, false otherwise
    */
    bool update(int16_t x, int16_t y, uint8_t buttons, int8_t z);


    /**
    * Move the cursor to (x, y)
    *
    * @param x x-axis position
    * @param y y-axis position
    * @returns true if there is no error, false otherwise
    */
    bool move(int16_t x, int16_t y);

    /**
    * Press one or several buttons
    *
    * @param button button state (ex: press(MOUSE_LEFT))
    * @returns true if there is no error, false otherwise
    */
    bool press(uint8_t button);

    /**
    * Release one or several buttons
    *
    * @param button button state (ex: release(MOUSE_LEFT))
    * @returns true if there is no error, false otherwise
    */
    bool release(uint8_t button);

    /**
    * Double click (MOUSE_LEFT)
    *
    * @returns true if there is no error, false otherwise
    */
    bool doubleClick();

    /**
    * Click
    *
    * @param button state of the buttons ( ex: clic(MOUSE_LEFT))
    * @returns true if there is no error, false otherwise
    */
    bool click(uint8_t button);

    /**
    * Scrolling
    *
    * @param z value of the wheel (>0 to go down, <0 to go up)
    * @returns true if there is no error, false otherwise
    */
    bool scroll(int8_t z);

    /**
    * To send a character defined by a modifier(CTRL, SHIFT, ALT) and the key
    *
    * @code
    * //To send CTRL + s (save)
    *  keyboard.keyCode('s', KEY_CTRL);
    * @endcode
    *
    * @param modifier bit 0: KEY_CTRL, bit 1: KEY_SHIFT, bit 2: KEY_ALT (default: 0)
    * @param key character to send
    * @returns true if there is no error, false otherwise
    */
    bool key_code(uint8_t key, uint8_t modifier = 0);

    /**
    * Send a character
    *
    * @param c character to be sent
    * @returns true if there is no error, false otherwise
    */
    virtual int _putc(int c);

    /**
    * Control media keys
    *
    * @param key media key pressed (KEY_NEXT_TRACK, KEY_PREVIOUS_TRACK, KEY_STOP, KEY_PLAY_PAUSE, KEY_MUTE, KEY_VOLUME_UP, KEY_VOLUME_DOWN)
    * @returns true if there is no error, false otherwise
    */
    bool media_control(MEDIA_KEY key);

    /**
    * Read status of lock keys. Useful to switch-on/off LEDs according to key pressed. Only the first three bits of the result is important:
    *   - First bit: NUM_LOCK
    *   - Second bit: CAPS_LOCK
    *   - Third bit: SCROLL_LOCK
    *
    * @returns status of lock keys
    */
    uint8_t lock_status();

    /*
    * To define the report descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
    *
    * @returns pointer to the report descriptor
    */
    virtual const uint8_t *report_desc();

    /*
    * Called when a data is received on the OUT endpoint. Useful to switch on LED of LOCK keys
    *
    * @returns if handle by subclass, return true
    */
    virtual void report_rx();


private:
    MOUSE_TYPE _mouse_type;
    uint8_t _button;
    uint8_t _lock_status;
    PlatformMutex _mutex;

    bool _mouse_send(int8_t x, int8_t y, uint8_t buttons, int8_t z);

    //dummy otherwise it doesn't compile (we must define all methods of an abstract class)
    virtual int _getc();
};

/** @}*/

#endif
