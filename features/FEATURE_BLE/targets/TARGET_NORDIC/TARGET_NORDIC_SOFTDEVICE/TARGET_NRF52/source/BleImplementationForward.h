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


    namespace generic {

        template <
            template<class> class TPalSecurityManager,
            template<class> class SigningMonitor
        >
        class GenericSecurityManager;

        template<
            template<class> class TPalGattClient,
            class SigningMonitorEventHandler
        >
        class GenericGattClient;

    } // namespace generic


    namespace pal {

        template<class Impl, class EventHandler>
        class SecurityManager;

        template<class Impl>
        class SigningMonitorEventHandler;

        template<class Impl>
        class ConnectionEventMonitorEventHandler;

        namespace vendor {
            namespace nordic {

                template <class EventHandler>
                class nRF5xSecurityManager;

                template<class EventHandler>
                class nRF5xGattClient;

            } // nordic
        } // vendor
    } // pal

    namespace vendor {
        namespace nordic {

            template<class>
            class DummySigningEventMonitor;

        } // nordic
    } // vendor


} // ble

class nRF5xGap;
class nRF5xGattServer;

// implementation assembly
namespace ble {
    namespace impl {
        // SECURITY MANAGER
        typedef generic::GenericSecurityManager<
            pal::vendor::nordic::nRF5xSecurityManager,
            vendor::nordic::DummySigningEventMonitor
        > GenericSecurityManagerImpl;

        typedef interface::SecurityManager<GenericSecurityManagerImpl> SecurityManager;

        typedef GenericSecurityManagerImpl SigningEventHandler;

        typedef GenericSecurityManagerImpl ConnectionEventHandler;

        typedef pal::vendor::nordic::nRF5xSecurityManager<
            GenericSecurityManagerImpl
        > PalSecurityManagerImpl;

        typedef ::ble::pal::SecurityManager<
            PalSecurityManagerImpl,
            GenericSecurityManagerImpl
        > PalSecurityManager;

        // GAP
        typedef interface::LegacyGap<nRF5xGap> LegacyGap;

        typedef interface::Gap<nRF5xGap> Gap;

        // GATT CLIENT

        typedef generic::GenericGattClient<
            ble::pal::vendor::nordic::nRF5xGattClient,
            SigningEventHandler
        > GenericGattClientImpl;

        typedef ble::pal::vendor::nordic::nRF5xGattClient<
            GenericGattClientImpl
        > PalGattClientImpl;

        // construct the final GattClient type
        typedef interface::GattClient<GenericGattClientImpl> GattClient;


        // GATT SERVER
        typedef ble::interface::GattServer<nRF5xGattServer> GattServer;
    } // impl
} // ble


#endif //BLE_IMPLEMENTATION_FORWARD_H_
