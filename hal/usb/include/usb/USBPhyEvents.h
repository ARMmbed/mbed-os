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

#ifndef USBPHY_EVENTS_H
#define USBPHY_EVENTS_H

#include "USBPhyTypes.h"

/** Event handler for USBPhy
 *
 * This class is the event handler for the USBPhy class. Any events generated
 * by USBPhy are passed to this class via the virtual functions.
 *
 * @ingroup usb_device_core
 *
 */
class USBPhyEvents {
public:
    USBPhyEvents() {};
    virtual ~USBPhyEvents() {};

    /**
     * Callback called when a bus reset occurs
     * @note called in the contex of USBPhy::process
     */
    virtual void reset() = 0;

    /**
     * Callback called when an endpoint 0 setup packet is received
     * @note called in the contex of USBPhy::process
     */
    virtual void ep0_setup() = 0;

    /**
     * Callback called when an endpoint 0 out packet is received
     * @note called in the contex of USBPhy::process
     */
    virtual void ep0_out() = 0;

    /**
     * Callback called when an endpoint 0 in packet is received
     * @note called in the contex of USBPhy::process
     */
    virtual void ep0_in() = 0;

    /**
     * Callback called USB power is applied or removed
     *
     * @param powered true if USB power is present, false otherwise
     * @note called in the contex of USBPhy::process
     */
    virtual void power(bool powered) = 0;

    /**
     * Callback called when entering or leaving suspend mode
     *
     * @param suspended true if entering suspend mode false otherwise
     * @note called in the contex of USBPhy::process
     */
    virtual void suspend(bool suspended) = 0;

    /**
     * Callback called on start of frame
     *
     * @param frame_number The current frame number
     * @note This callback is enabled/disabled by
     *  calling USBPhy::sof_enable / USBPhy::sof_disable
     * @note called in the contex of USBPhy::process
     */
    virtual void sof(int frame_number) = 0;

    /**
     * Callback called on the reception of an OUT packet
     *
     * @param endpoint Endpoint which received the OUT packet
     * @note called in the contex of USBPhy::process
     */
    virtual void out(usb_ep_t endpoint) = 0;

    /**
     * Callback called on the transmission of an IN packet
     *
     * @param endpoint Endpoint which sent the IN packet
     * @note called in the contex of USBPhy::process
     */
    virtual void in(usb_ep_t endpoint) = 0;

    /**
     * Callback called to indicate the USB processing needs to be done
     */
    virtual void start_process() = 0;
};

#endif
