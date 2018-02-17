/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef ATCA_H
#define ATCA_H


#include "ATCADevice.h"
#include "ATCAKey.h"
#include "MbedPlatform.h"

/** ATCA device factory.
 *
 * It bootstraps device creation and provides high level interface for the
 * application.
 * In current implementation it is assumed that only one device of type
 * ATCAECC508A is required to be handled.
 * This may need replacing if in the future more than one ATCA devices of same
 * or different types will be required to be handled.
 */
class ATCAFactory
{
private:
    MbedPlatform            plt;
    ATCADevice              device;
    static ATCAFactory *    instance;

    /** Private constructor to allow controlled instantiation of devices
     *  using method GetDevice().
     */
    ATCAFactory();
public:
    ~ATCAFactory();

    /** Create and/or return instantiated device pointer.
     */
    static ATCADevice * GetDevice();

    /** Cleans up device.
     */
    static void Deinit();
};


#endif /* ATCA_H */

