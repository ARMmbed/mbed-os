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

#ifndef USBMIDI_H
#define USBMIDI_H

/* These headers are included for child class. */
#include "USBDescriptor.h"
#include "USBDevice_Types.h"

#include "USBDevice.h"
#include "MIDIMessage.h"
#include "EventFlags.h"
#include "Mutex.h"
#include "Callback.h"

#define DEFAULT_CONFIGURATION (1)

/**
 * \defgroup drivers_USBMIDI USBMIDI class
 * \ingroup drivers-public-api-usb
 * @{
 */

/**
* USBMIDI example
*
* @code
* #include "mbed.h"
* #include "USBMIDI.h"
*
* USBMIDI midi;
*
* int main() {
*    while (1) {
*        for(int i=48; i<83; i++) {     // send some messages!
*            midi.write(MIDIMessage::NoteOn(i));
*            ThisThread::sleep_for(250);
*            midi.write(MIDIMessage::NoteOff(i));
*            ThisThread::sleep_for(500);
*        }
*    }
* }
* @endcode
*/
class USBMIDI: public USBDevice {
public:

    /**
    * Basic constructor
    *
    * Construct this object optionally connecting and blocking until it is ready.
    *
    * @note Do not use this constructor in derived classes.
    *
    * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBMIDI(bool connect_blocking = true, uint16_t vendor_id = 0x0700, uint16_t product_id = 0x0101, uint16_t product_release = 0x0001);

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
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
    USBMIDI(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release);

    /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
    virtual ~USBMIDI();

    /**
     * Check if this class is ready
     *
     * @return true if configured, false otherwise
     */
    bool ready();

    /**
     * Block until this device is configured
     */
    void wait_ready();

    /**
     * Send a MIDIMessage
     *
     * @param m The MIDIMessage to send
     * @return true if the message was sent, false otherwise
     */
    bool write(MIDIMessage m);

    /**
     * Check if a message can be read
     *
     * @return true if a packet can be read false otherwise
     * @note USBMIDI::attach must be called to enable the receiver
     */
    bool readable();

    /**
     * Read a message
     *
     * @param m The MIDIMessage to fill
     * @return true if a message was read, false otherwise
     */
    bool read(MIDIMessage *m);

    /**
     * Attach a callback for when a MIDIEvent is received
     *
     * @param callback code to call when a packet is received
     */
    void attach(mbed::Callback<void()> callback);


protected:

    virtual void callback_state_change(DeviceState new_state);

    virtual void callback_request(const setup_packet_t *setup);

    virtual void callback_request_xfer_done(const setup_packet_t *setup, bool aborted);

    virtual void callback_set_configuration(uint8_t configuration);

    virtual void callback_set_interface(uint16_t interface, uint8_t alternate);

    virtual const uint8_t *string_iproduct_desc();

    virtual const uint8_t *string_iinterface_desc();

    virtual const uint8_t *configuration_desc(uint8_t index);

private:
    static const uint32_t MaxSize = 64;

    uint8_t _bulk_buf[MaxSize];
    uint32_t _bulk_buf_pos;
    uint32_t _bulk_buf_size;

    bool _data_ready;
    uint8_t _data[MAX_MIDI_MESSAGE_SIZE + 1];
    uint32_t _cur_data;

    rtos::EventFlags _flags;
    rtos::Mutex _write_mutex;

    usb_ep_t _bulk_in;
    usb_ep_t _bulk_out;
    uint8_t _config_descriptor[0x65];

    mbed::Callback<void()> _callback;

    void _init();
    void _in_callback();
    void _out_callback();
    bool _next_message();
};

/** @}*/

#endif
