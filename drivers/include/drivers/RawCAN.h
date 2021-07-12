/*
 *  Copyright (C) 2021, STMicroelectronics, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef RAWCAN_H
#define RAWCAN_H

#include "platform/platform.h"
#include "drivers/CAN.h"

#if DEVICE_CAN || defined(DOXYGEN_ONLY)

#include "interfaces/InterfaceCAN.h"
#include "hal/can_api.h"
#include "platform/Callback.h"
#include "platform/PlatformMutex.h"

namespace mbed {
#ifndef FEATURE_EXPERIMENTAL_API
class RawCAN: public CAN {
public:
    /** Creates an unlocked CAN interface connected to specific pins.
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
     * RawCAN can1(MBED_CONF_APP_CAN1_RD, MBED_CONF_APP_CAN1_TD);
     * RawCAN can2(MBED_CONF_APP_CAN2_RD, MBED_CONF_APP_CAN2_TD);
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

    /* Note: The can apis are unlocked hence using this when multiple
    *  threads are accessing a single instance of CAN will lead to
    *  race conditions, can be used in single threaded CAN.
    */
    using CAN::CAN;


    // override lock apis to create unlocked CAN
    void lock() override {};
    void unlock() override {};

};
#endif //FEATURE_EXPERIMENTAL_API
}

#endif
#endif //RAWCAN_H
