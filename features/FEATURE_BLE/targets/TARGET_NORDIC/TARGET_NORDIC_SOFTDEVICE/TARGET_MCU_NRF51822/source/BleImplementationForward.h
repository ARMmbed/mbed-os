/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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

#ifndef BLE_IMPLEMENTATION_FORWARD_H_
#define BLE_IMPLEMENTATION_FORWARD_H_

////////////////////////////////////////////////////////////////////////////////
// Forward declarations of the implementation types
//
namespace ble {

    namespace interface {

        template<class Impl>
        class LegacyGap;

        template<class Impl>
        class Gap;

        template<class Impl>
        class GattClient;

        template<class Impl>
        class SecurityManager;

        template<class Impl>
        class GattServer;

    } // namespace interface
} // ble

class nRF5xGap;
class nRF5xGattClient;
class nRF5xGattServer;
class nRF5xSecurityManager;

// implementation assembly
namespace ble {
    namespace impl {
        // SECURITY MANAGER
        typedef interface::SecurityManager<nRF5xSecurityManager> SecurityManager;

        // GAP
        typedef interface::LegacyGap<nRF5xGap> LegacyGap;

        typedef interface::Gap<nRF5xGap> Gap;

        // GATT CLIENT
        typedef interface::GattClient<nRF5xGattClient> GattClient;

        // GATT SERVER
        typedef ble::interface::GattServer<nRF5xGattServer> GattServer;
    } // impl
} // ble


#endif //BLE_IMPLEMENTATION_FORWARD_H_
