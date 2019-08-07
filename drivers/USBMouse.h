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

#ifndef USBMOUSE_H
#define USBMOUSE_H

#include "USBHID.h"
#include "PlatformMutex.h"

#define REPORT_ID_MOUSE   2

/* Common usage */

enum MOUSE_BUTTON {
    MOUSE_LEFT = 1,
    MOUSE_RIGHT = 2,
    MOUSE_MIDDLE = 4,
};

/* X and Y limits */
/* These values do not directly map to screen pixels */
/* Zero may be interpreted as meaning 'no movement' */
#define X_MIN_ABS    (1)        /*!< Minimum value on x-axis */
#define Y_MIN_ABS    (1)        /*!< Minimum value on y-axis */
#define X_MAX_ABS    (0x7fff)   /*!< Maximum value on x-axis */
#define Y_MAX_ABS    (0x7fff)   /*!< Maximum value on y-axis */

#define X_MIN_REL    (-127)     /*!< The maximum value that we can move to the left on the x-axis */
#define Y_MIN_REL    (-127)     /*!< The maximum value that we can move up on the y-axis */
#define X_MAX_REL    (127)      /*!< The maximum value that we can move to the right on the x-axis */
#define Y_MAX_REL    (127)      /*!< The maximum value that we can move down on the y-axis */

enum MOUSE_TYPE {
    ABS_MOUSE,
    REL_MOUSE,
};

/**
 * \defgroup drivers_USBMouse USBMouse class
 * \ingroup drivers-public-api-usb
 * @{
 */

/**
 *
 * USBMouse example
 * @code
 * #include "mbed.h"
 * #include "USBMouse.h"
 *
 * USBMouse mouse;
 *
 * int main(void)
 * {
 *   while (1)
 *   {
 *      mouse.move(20, 0);
 *      wait(0.5);
 *   }
 * }
 *
 * @endcode
 *
 *
 * @code
 * #include "mbed.h"
 * #include "USBMouse.h"
 * #include <math.h>
 *
 * USBMouse mouse(true, ABS_MOUSE);
 *
 * int main(void)
 * {
 *   uint16_t x_center = (X_MAX_ABS - X_MIN_ABS)/2;
 *   uint16_t y_center = (Y_MAX_ABS - Y_MIN_ABS)/2;
 *   uint16_t x_screen = 0;
 *   uint16_t y_screen = 0;
 *
 *   uint32_t x_origin = x_center;
 *   uint32_t y_origin = y_center;
 *   uint32_t radius = 5000;
 *   uint32_t angle = 0;
 *
 *   while (1)
 *   {
 *       x_screen = x_origin + cos((double)angle*3.14/180.0)*radius;
 *       y_screen = y_origin + sin((double)angle*3.14/180.0)*radius;
 *
 *       mouse.move(x_screen, y_screen);
 *       angle += 3;
 *       wait(0.01);
 *   }
 * }
 *
 * @endcode
 *
 * @note Synchronization level: Thread safe
 */
class USBMouse: public USBHID {
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
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBMouse(bool connect_blocking = true, MOUSE_TYPE mouse_type = REL_MOUSE, uint16_t vendor_id = 0x1234, uint16_t product_id = 0x0001, uint16_t product_release = 0x0001);

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
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBMouse(USBPhy *phy, MOUSE_TYPE mouse_type = REL_MOUSE, uint16_t vendor_id = 0x1234, uint16_t product_id = 0x0001, uint16_t product_release = 0x0001);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
    virtual ~USBMouse();

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
    bool double_click();

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

    /*
    * To define the report descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
    *
    * @returns pointer to the report descriptor
    */
    virtual const uint8_t *report_desc();

protected:
    /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
    virtual const uint8_t *configuration_desc(uint8_t index);

private:
    MOUSE_TYPE _mouse_type;
    uint8_t _button;
    uint8_t _configuration_descriptor[41];
    PlatformMutex _mutex;

    bool mouse_send(int8_t x, int8_t y, uint8_t buttons, int8_t z);
};

/** @}*/

#endif
