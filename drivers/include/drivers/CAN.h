/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_CAN_H
#define MBED_CAN_H

#include "platform/platform.h"

#if DEVICE_CAN || defined(DOXYGEN_ONLY)

#include "hal/can_api.h"
#include "platform/Callback.h"
#include "platform/PlatformMutex.h"
#include "platform/NonCopyable.h"

namespace mbed {
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
};

/** @}*/

/**
 * \defgroup drivers_CAN CAN class
 * \ingroup drivers-public-api-can
 * @{
 */

/** A can bus client, used for communicating with can devices
 */
class CAN : private NonCopyable<CAN> {

public:
    /** Creates a CAN interface connected to specific pins.
     *
     *  @param rd read from transmitter
     *  @param td transmit to transmitter
     *
     * Example:
     * @code
     * #include "mbed.h"
     *
     *
     * Ticker ticker;
     * DigitalOut led1(LED1);
     * DigitalOut led2(LED2);
     * //The constructor takes in RX, and TX pin respectively.
     * //These pins, for this example, are defined in mbed_app.json
     * CAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);
     * CAN can2(MBED_CONF_APP_CAN2_RD, MBED_CONF_APP_CAN2_TD);
     *
     * unsigned char counter = 0;
     *
     * void send() {
     *     if(can1.write(CANMessage(1337U, &counter, 1))) {
     *         printf("Message sent: %d\n", counter);
     *         counter++;
     *     }
     *     led1 = !led1;
     * }
     *
     * int main() {
     *     ticker.attach(&send, 1);
     *     CANMessage msg;
     *     while(1) {
     *         if(can2.read(msg)) {
     *             printf("Message received: %d\n\n", msg.data[0]);
     *             led2 = !led2;
     *         }
     *         ThisThread::sleep_for(200);
     *     }
     * }
     *
     * @endcode
     */
    CAN(PinName rd, PinName td);

    /** Initialize CAN interface and set the frequency
      *
      * @param rd the read pin
      * @param td the transmit pin
      * @param hz the bus frequency in hertz
      */
    CAN(PinName rd, PinName td, int hz);

    /** Initialize CAN interface
      *
      * @param pinmap reference to structure which holds static pinmap
      * @param td the transmit pin
      * @param hz the bus frequency in hertz
      */
    CAN(const can_pinmap_t &pinmap);
    CAN(const can_pinmap_t &&) = delete; // prevent passing of temporary objects

    /** Initialize CAN interface and set the frequency
      *
      * @param pinmap reference to structure which holds static pinmap
      * @param td the transmit pin
      * @param hz the bus frequency in hertz
      */
    CAN(const can_pinmap_t &pinmap, int hz);
    CAN(const can_pinmap_t &&, int) = delete; // prevent passing of temporary objects

    virtual ~CAN();

    /** Set the frequency of the CAN interface
     *
     *  @param hz The bus frequency in hertz
     *
     *  @returns
     *    1 if successful,
     *    0 otherwise
     */
    int frequency(int hz);

    /** Write a CANMessage to the bus.
     *
     *  @param msg The CANMessage to write.
     *
     *  @returns
     *    0 if write failed,
     *    1 if write was successful
     */
    int write(CANMessage msg);

    /** Read a CANMessage from the bus.
     *
     *  @param msg A CANMessage to read to.
     *  @param handle message filter handle (0 for any message)
     *
     *  @returns
     *    0 if no message arrived,
     *    1 if message arrived
     */
    int read(CANMessage &msg, int handle = 0);

    /** Reset CAN interface.
     *
     * To use after error overflow.
     */
    void reset();

    /** Puts or removes the CAN interface into silent monitoring mode
     *
     *  @param silent boolean indicating whether to go into silent mode or not
     */
    void monitor(bool silent);

    enum Mode {
        Reset = 0,
        Normal,
        Silent,
        LocalTest,
        GlobalTest,
        SilentTest
    };

    /** Change CAN operation to the specified mode
     *
     *  @param mode The new operation mode (CAN::Normal, CAN::Silent, CAN::LocalTest, CAN::GlobalTest, CAN::SilentTest)
     *
     *  @returns
     *    0 if mode change failed or unsupported,
     *    1 if mode change was successful
     */
    int mode(Mode mode);

    /** Filter out incoming messages
     *
     *  @param id the id to filter on
     *  @param mask the mask applied to the id
     *  @param format format to filter on (Default CANAny)
     *  @param handle message filter handle (Optional)
     *
     *  @returns
     *    0 if filter change failed or unsupported,
     *    new filter handle if successful
     */
    int filter(unsigned int id, unsigned int mask, CANFormat format = CANAny, int handle = 0);

    /**  Detects read errors - Used to detect read overflow errors.
     *
     *  @returns number of read errors
     */
    unsigned char rderror();

    /** Detects write errors - Used to detect write overflow errors.
     *
     *  @returns number of write errors
     */
    unsigned char tderror();

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

    /** Attach a function to call whenever a CAN frame received interrupt is
     *  generated.
     *
     *  This function locks the deep sleep while a callback is attached
     *
     *  @param func A pointer to a void function, or 0 to set as none
     *  @param type Which CAN interrupt to attach the member function to (CAN::RxIrq for message received, CAN::TxIrq for transmitted or aborted, CAN::EwIrq for error warning, CAN::DoIrq for data overrun, CAN::WuIrq for wake-up, CAN::EpIrq for error passive, CAN::AlIrq for arbitration lost, CAN::BeIrq for bus error)
     */
    void attach(Callback<void()> func, IrqType type = RxIrq);

    static void _irq_handler(uint32_t id, CanIrqType type);

#if !defined(DOXYGEN_ONLY)
protected:
    virtual void lock();
    virtual void unlock();

    can_t               _can;
    Callback<void()>    _irq[IrqCnt];
    PlatformMutex       _mutex;
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif    // MBED_CAN_H
