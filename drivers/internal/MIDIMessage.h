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

#ifndef MIDIMESSAGE_H
#define MIDIMESSAGE_H

#include <stdint.h>

#define MAX_MIDI_MESSAGE_SIZE 256 // Max message size. SysEx can be up to 65536 but 256 should be fine for most usage

// MIDI Message Format
//
// [ msg(4) | channel(4) ] [ 0 | n(7) ] [ 0 | m(7) ]
//
// MIDI Data Messages (Channel Specific)
//
// Message               msg n          m
// ---------------------------------------------
// Note Off              0x8 Key        Velocity
// Note On               0x9 Key        Velocity
// Polyphonic Aftertouch 0xA Key        Pressure
// Control Change        0xB Controller Value
// Program Change        0xC Program    -
// Channel Aftertouch    0xD Pressure   -
// Pitch Wheel           0xE LSB        MSB

#define CABLE_NUM (0<<4)


/**
 * \defgroup drivers_MIDIMessage MIDIMessage class
 * \ingroup drivers-internal-usb
 * @{
 */

/** A MIDI message container */
class MIDIMessage {
public:

    MIDIMessage() : data(new uint8_t[MAX_MIDI_MESSAGE_SIZE + 1]), length(0) {}

    MIDIMessage(uint8_t *buf) : data(new uint8_t[MAX_MIDI_MESSAGE_SIZE + 1]), length(0)
    {
        for (int i = 0; i < 4; i++) {
            data[i] = buf[i];
        }
    }

    /**
     * Copy constructor
     */
    MIDIMessage(const MIDIMessage &other)
    {
        *this = other;
    }

    /**
     * Assignment operator
     */
    MIDIMessage &operator=(const MIDIMessage &other)
    {
        length = other.length;
        for (int i = 0; i < length; i++) {
            data[i] = other.data[i];
        }

        return *this;
    }

    ~MIDIMessage()
    {
        delete[] data;
    }

    /**
     * Set this MIDIMessage to a raw MIDI message
     *
     * @param buf is a true MIDI message (not USBMidi message)
     * @param buf_len size of message
     */
    void from_raw(uint8_t *buf, int buf_len)
    {
        length = buf_len + 1;
        if (length > MAX_MIDI_MESSAGE_SIZE) {
            // Message is too big
            length = 0;
            return;
        }

        // first byte keeped for retro-compatibility
        data[0] = CABLE_NUM | 0x08;

        for (int i = 0; i < buf_len; i++) {
            data[i + 1] = buf[i];
        }
    }

    // create messages

    /** Create a NoteOff message
     * @param key Key ID
     * @param velocity Key velocity (0-127, default = 127)
     * @param channel Key channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage NoteOff(int key, int velocity = 127, int channel = 0)
    {
        MIDIMessage msg;
        msg.data[0] = CABLE_NUM | 0x08;
        msg.data[1] = 0x80 | (channel & 0x0F);
        msg.data[2] = key & 0x7F;
        msg.data[3] = velocity & 0x7F;
        msg.length = 4;
        return msg;
    }

    /** Create a NoteOn message
     * @param key Key ID
     * @param velocity Key velocity (0-127, default = 127)
     * @param channel Key channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage NoteOn(int key, int velocity = 127, int channel = 0)
    {
        MIDIMessage msg;
        msg.data[0] = CABLE_NUM | 0x09;
        msg.data[1] = 0x90 | (channel & 0x0F);
        msg.data[2] = key & 0x7F;
        msg.data[3] = velocity & 0x7F;
        msg.length = 4;
        return msg;
    }

    /** Create a PolyPhonic Aftertouch message
     * @param key Key ID
     * @param pressure Aftertouch pressure (0-127)
     * @param channel Key channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage PolyphonicAftertouch(int key, int pressure, int channel = 0)
    {
        MIDIMessage msg;
        msg.data[0] = CABLE_NUM | 0x0A;
        msg.data[1] = 0xA0 | (channel & 0x0F);
        msg.data[2] = key & 0x7F;
        msg.data[3] = pressure & 0x7F;
        msg.length = 4;
        return msg;
    }

    /** Create a Control Change message
     * @param control Controller ID
     * @param value Controller value (0-127)
     * @param channel Controller channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage ControlChange(int control, int value, int channel = 0)
    {
        MIDIMessage msg;
        msg.data[0] = CABLE_NUM | 0x0B;
        msg.data[1] = 0xB0 | (channel & 0x0F);
        msg.data[2] = control & 0x7F;
        msg.data[3] = value & 0x7F;
        msg.length = 4;
        return msg;
    }

    /** Create a Program Change message
     * @param program Program ID
     * @param channel Channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage ProgramChange(int program, int channel = 0)
    {
        MIDIMessage msg;
        msg.data[0] = CABLE_NUM | 0x0C;
        msg.data[1] = 0xC0 | (channel & 0x0F);
        msg.data[2] = program & 0x7F;
        msg.data[3] = 0x00;
        msg.length = 4;
        return msg;
    }

    /** Create a Channel Aftertouch message
     * @param pressure Pressure
     * @param channel Key channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage ChannelAftertouch(int pressure, int channel = 0)
    {
        MIDIMessage msg;
        msg.data[0] = CABLE_NUM | 0x0D;
        msg.data[1] = 0xD0 | (channel & 0x0F);
        msg.data[2] = pressure & 0x7F;
        msg.data[3] = 0x00;
        msg.length = 4;
        return msg;
    }

    /** Create a Pitch Wheel message
     * @param pitch Pitch (-8192 - 8191, default = 0)
     * @param channel Channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage PitchWheel(int pitch = 0, int channel = 0)
    {
        MIDIMessage msg;
        int p = pitch + 8192;    // 0 - 16383, 8192 is center
        msg.data[0] = CABLE_NUM | 0x0E;
        msg.data[1] = 0xE0 | (channel & 0x0F);
        msg.data[2] = p & 0x7F;
        msg.data[3] = (p >> 7) & 0x7F;
        msg.length = 4;
        return msg;
    }

    /** Create an All Notes Off message
     * @param channel Channel (0-15, default 0)
     * @returns A MIDIMessage
     */
    static MIDIMessage AllNotesOff(int channel = 0)
    {
        return ControlChange(123, 0, channel);
    }

    /** Create a SysEx message
    * @param data SysEx data (including 0xF0 .. 0xF7)
    * @param len SysEx data length
    * @returns A MIDIMessage
    */
    static MIDIMessage SysEx(uint8_t *data, int len)
    {
        MIDIMessage msg;
        msg.from_raw(data, len);
        return msg;
    }

    // decode messages

    /** MIDI Message Types */
    enum MIDIMessageType {
        ErrorType,
        NoteOffType,
        NoteOnType,
        PolyphonicAftertouchType,
        ControlChangeType,
        ProgramChangeType,
        ChannelAftertouchType,
        PitchWheelType,
        ResetAllControllersType,
        AllNotesOffType,
        SysExType
    };

    /** Read the message type
     *
     * @returns MIDIMessageType
     */
    MIDIMessageType type()
    {
        MIDIMessageType message_type;
        uint8_t min_size;
        switch ((data[1] >> 4) & 0xF) {
            case 0x8:
                // message, channel
                // key
                // velocity
                min_size = 3;
                message_type = NoteOffType;
                break;
            case 0x9:
                // message, channel
                // key
                // velocity
                min_size = 3;
                message_type = NoteOnType;
                break;
            case 0xA:
                // message, channel
                // key
                // pressure
                min_size = 3;
                message_type = PolyphonicAftertouchType;
                break;
            case 0xB:
                // message, channel
                // controller
                min_size = 2;
                if ((data[2] & 0x7F) < 120) { // standard controllers
                    message_type = ControlChangeType;
                } else if ((data[2] & 0x7F) == 121) {
                    message_type = ResetAllControllersType;
                } else if ((data[2] & 0x7F) == 123) {
                    message_type = AllNotesOffType;
                } else {
                    message_type = ErrorType; // unsupported atm
                }
                break;
            case 0xC:
                // message, channel
                // program
                min_size = 2;
                message_type = ProgramChangeType;
                break;
            case 0xD:
                // message, channel
                // pressure
                min_size = 2;
                message_type = ChannelAftertouchType;
                break;
            case 0xE:
                // message, channel
                // pitch lsb
                // pitch msb
                min_size = 3;
                message_type = PitchWheelType;
                break;
            case 0xF:
                min_size = 2;
                message_type = SysExType;
                break;
            default:
                message_type = ErrorType;
                break;
        }


        if (length < min_size) {
            // too small to be a valid message
            message_type = ErrorType;
        }
        return message_type;
    }

    /**
     * Read the channel number
     *
     * @return channel number or -1 on error
     */

    int channel()
    {
        return (data[1] & 0x0F);
    }

    /**
     * Read the key ID
     *
     * @return key ID or -1 on error
     */
    int key()
    {
        MIDIMessageType msg_type = type();
        if ((msg_type != NoteOffType) &&
                (msg_type != NoteOnType) &&
                (msg_type != PolyphonicAftertouchType)) {
            return -1;
        }

        return data[2] & 0x7F;
    }

    /**
     * Read the velocity
     *
     * @return velocity or -1 on error
     */
    int velocity()
    {
        MIDIMessageType msg_type = type();
        if ((msg_type != NoteOffType) &&
                (msg_type != NoteOnType)) {
            return -1;
        }

        return data[3] & 0x7F;
    }

    /**
     * Read the controller value
     *
     * @return controller value or -1 on error
     */
    int value()
    {
        MIDIMessageType msg_type = type();
        if ((msg_type != ControlChangeType) &&
                (msg_type != ResetAllControllersType) &&
                (msg_type != AllNotesOffType)) {
            return -1;
        }

        return data[3] & 0x7F;
    }

    /**
     * Read the aftertouch pressure
     *
     * @return aftertouch pressure or -1 on error
     */
    int pressure()
    {
        MIDIMessageType msg_type = type();
        if ((msg_type != PolyphonicAftertouchType) &&
                (msg_type != ChannelAftertouchType)) {
            return -1;
        }

        if (type() == PolyphonicAftertouchType) {
            return data[3] & 0x7F;
        } else {
            return data[2] & 0x7F;
        }
    }

    /**
     * Read the controller number
     *
     * @return controller number or -1 on error
     */
    int controller()
    {
        MIDIMessageType msg_type = type();
        if ((msg_type != ControlChangeType) &&
                (msg_type != ResetAllControllersType) &&
                (msg_type != AllNotesOffType)) {
            return -1;
        }

        return data[2] & 0x7F;
    }

    /**
     * Read the program number
     *
     * @return program number or -1 on error
     */
    int program()
    {
        MIDIMessageType msg_type = type();
        if (msg_type != ProgramChangeType) {
            return -1;
        }

        return data[2] & 0x7F;
    }

    /**
     * Read the pitch value
     *
     * @return pitch value or -1 on error
     */
    int pitch()
    {
        MIDIMessageType msg_type = type();
        if (msg_type != PitchWheelType) {
            return -1;
        }

        int p = ((data[3] & 0x7F) << 7) | (data[2] & 0x7F);
        return p - 8192; // 0 - 16383, 8192 is center
    }

    uint8_t *data;
    uint16_t length;
};

/** @}*/

#endif
