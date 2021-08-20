/*
 * Mbed-OS Microcontroller Library
 * Copyright (c) 2021 Embedded Planet
 * Copyright (c) 2021 ARM Limited
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
 * limitations under the License
 */

#ifndef MBED_INTERFACE_CAN_H_
#define MBED_INTERFACE_CAN_H_

#include "hal/can_helper.h"

#include <cstring>

#include "platform/Callback.h"

namespace mbed {

#ifndef FEATURE_EXPERIMENTAL_API
// Forward declare CAN
class CAN;
#endif

/** \defgroup drivers-public-api-can CAN
 * \ingroup drivers-public-api
 */

/**
 * \defgroup drivers_CANMessage CANMessage class
 * \ingroup drivers-public-api-can
 * @{
 */

/** CANMessage class
 *
 * @note Synchronization level: Thread safe
 */
class CANMessage : public CAN_Message {

public:
    /** Creates empty CAN message.
     */
    CANMessage() : CAN_Message()
    {
        len    = 8U;
        type   = CANData;
        format = CANStandard;
        id     = 0U;
        memset(data, 0, 8);
    }

    /** Creates CAN message with specific content.
     *
     *  @param _id      Message ID
     *  @param _data    Mesaage Data
     *  @param _len     Message Data length
     *  @param _type    Type of Data: Use enum CANType for valid parameter values
     *  @param _format  Data Format: Use enum CANFormat for valid parameter values
     */
    CANMessage(unsigned int _id, const unsigned char *_data, unsigned char _len = 8, CANType _type = CANData, CANFormat _format = CANStandard)
    {
        len    = (_len > 8) ? 8 : _len;
        type   = _type;
        format = _format;
        id     = _id;
        memcpy(data, _data, len);
    }


    /** Creates CAN message with specific content.
     *
     *  @param _id      Message ID
     *  @param _data    Mesaage Data
     *  @param _len     Message Data length
     *  @param _type    Type of Data: Use enum CANType for valid parameter values
     *  @param _format  Data Format: Use enum CANFormat for valid parameter values
     */
    CANMessage(unsigned int _id, const char *_data, unsigned char _len = 8, CANType _type = CANData, CANFormat _format = CANStandard)
    {
        len    = (_len > 8) ? 8 : _len;
        type   = _type;
        format = _format;
        id     = _id;
        memcpy(data, _data, len);
    }

    /** Creates CAN remote message.
     *
     *  @param _id      Message ID
     *  @param _format  Data Format: Use enum CANType for valid parameter values
     */
    CANMessage(unsigned int _id, CANFormat _format = CANStandard)
    {
        len    = 0;
        type   = CANRemote;
        format = _format;
        id     = _id;
        memset(data, 0, 8);
    }

    /**
     * "Deep" comparison operator (ie: compare value of each data member)
     */
    bool operator ==(const CANMessage &b) const
    {
        if (id != b.id) {
            return false;
        }
        if (len != b.len) {
            return false;
        }
        if (format != b.format) {
            return false;
        }
        if (type != b.type) {
            return false;
        }
        if (memcmp(data, b.data, len) != 0) {
            return false;
        }

        return true;
    }

    bool operator !=(const CANMessage &b) const
    {
        return !(*this == b);
    }
};

/** @}*/

namespace interface {

/* Having this as a struct allows interface::CAN and/or mbed::CAN to inherit the enums */
struct can {

    enum Mode {
        Reset = 0,
        Normal,
        Silent,
        LocalTest,
        GlobalTest,
        SilentTest
    };

    enum IrqType {
        RxIrq = 0,
        TxIrq,
        EwIrq,
        DoIrq,
        WuIrq,
        EpIrq,
        AlIrq,
        BeIrq,
        IdIrq,

        IrqCnt
    };

// Prevent slicing and user creation of base class.
protected:
    can() = default;
    ~can() = default;

public:

    /* Copy constructor and copy assignment operators will be deleted in subclasses as well */
    can(const can &) = delete;
    can &operator=(const can &) = delete;

};

#ifdef FEATURE_EXPERIMENTAL_API

// Pure virtual interface for CAN
struct CAN : public interface::can {

    virtual ~CAN() = default;
    virtual int frequency(int hz) = 0;
    virtual int write(CANMessage msg) = 0;
    virtual int read(CANMessage &msg, int handle = 0) = 0;
    virtual void reset() = 0;
    virtual void monitor(bool silent) = 0;
    virtual int mode(Mode mode) = 0;
    virtual int filter(unsigned int id, unsigned int mask, CANFormat format = CANAny, int handle = 0) = 0;
    virtual unsigned char rderror() = 0;
    virtual unsigned char tderror() = 0;
    virtual void attach(Callback<void()> func, IrqType type = IrqType::RxIrq) = 0;
};

#else
using CAN = ::mbed::CAN;
#endif

} // namespace interface

#if defined(FEATURE_EXPERIMENTAL_API) && !DEVICE_CAN
using CAN = interface::CAN;
#endif

} // namespace mbed

#endif /* MBED_INTERFACE_CAN_H_ */
