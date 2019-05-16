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

#ifndef MBED_OS_EXAMPLE_BLINKY_BLEIMPLEMENTATIONFORWARD_H
#define MBED_OS_EXAMPLE_BLINKY_BLEIMPLEMENTATIONFORWARD_H

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

        template<
            template<class> class TGapImpl,
            class PalSecurityManager,
            class ConnectionEventMonitorEventHandler
        >
        class GenericGap;

        template <
            template<class> class TPalSecurityManager,
            template<class> class SigningMonitor
        >
        class GenericSecurityManager;

        template<template<class> class TPalGattClient, class SigningMonitorEventHandler>
        class GenericGattClient;

    } // namespace generic

    namespace pal {

        template<class Impl, class EventHandler>
        class SecurityManager;

        template<class AttClient, class EventHandler>
        class AttClientToGattClientAdapter;

        namespace vendor {
            namespace cordio {

                template <class EventHandler>
                class CordioSecurityManager;

                template<class EventHandler>
                class Gap;

                template<class EventHandler>
                class CordioGattClient;
            } // cordio
        } // vendor
    } // pal

    namespace vendor {
        namespace cordio {

            template<class EventHandler>
            class SigningEventMonitor;

            class GattServer;

        } // cordio
    } // vendor
} // ble


// implementation assembly
namespace ble {
    namespace impl {
        // SECURITY MANAGER
        typedef generic::GenericSecurityManager<
            pal::vendor::cordio::CordioSecurityManager,
            vendor::cordio::SigningEventMonitor
        > GenericSecurityManagerImpl;

        typedef interface::SecurityManager<GenericSecurityManagerImpl> SecurityManager;

        typedef GenericSecurityManagerImpl SigningEventHandler;

        typedef pal::vendor::cordio::CordioSecurityManager<
            GenericSecurityManagerImpl
        > PalSecurityManagerImpl;

        typedef ::ble::pal::SecurityManager<
            PalSecurityManagerImpl,
            GenericSecurityManagerImpl
        > PalSecurityManager;

        // GAP
        typedef generic::GenericGap<
            pal::vendor::cordio::Gap,
            PalSecurityManager,
            GenericSecurityManagerImpl
        > GenericGapImpl;

        typedef pal::vendor::cordio::Gap<impl::GenericGapImpl> PalGapImpl;

        typedef interface::LegacyGap<GenericGapImpl> LegacyGap;

        typedef interface::Gap<GenericGapImpl> Gap;

        // GATT CLIENT
        using pal::AttClientToGattClientAdapter;
        using pal::vendor::cordio::CordioGattClient;
        using generic::GenericGattClient;

        // construct the pal::GattClient
        typedef GenericGattClient<
            CordioGattClient,
            SigningEventHandler
        > GenericGattClientImpl;

        typedef CordioGattClient<GenericGattClientImpl> PalGattClientImpl;

        // construct the final GattClient type
        typedef interface::GattClient<
            GenericGattClientImpl
        > GattClient;

        // GATT SERVER
        typedef ble::vendor::cordio::GattServer GattServerImpl;

        typedef ble::interface::GattServer<
            GattServerImpl
        > GattServer;

    } // impl
} // ble


#endif //MBED_OS_EXAMPLE_BLINKY_BLEIMPLEMENTATIONFORWARD_H
