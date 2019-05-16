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

#include "ble/GattServer.h"

#include "ble/Gap.h"
#include "ble/GattClient.h"
#include "ble/SecurityManager.h"

#include "ble/generic/GenericGap.h"
#include "ble/generic/GenericSecurityManager.h"
#include "ble/pal/PalSecurityManager.h"
#include "ble/pal/AttClientToGattClientAdapter.h"
#include "ble/pal/PalGap.h"

#include "source/LegacyGap.tpp"
#include "source/gap/Gap.tpp"
#include "source/GattClient.tpp"
#include "source/SecurityManager.tpp"

#include "source/generic/GenericGap.tpp"
#include "source/generic/GenericGattClient.tpp"
#include "source/generic/GenericSecurityManager.tpp"

#include "CordioPalGap.h"
#include "CordioPalSecurityManager.h"
#include "SigningMonitorProxy.h"
#include "CordioPalAttClient.h"

#include "source/CordioPalSecurityManager.tpp"
#include "source/CordioPalGap.tpp"

// Generic GattClient
template class ble::generic::GenericGattClient<
    ble::pal::vendor::cordio::CordioGattClient,
    ble::generic::GenericSecurityManager<
        ble::pal::vendor::cordio::CordioSecurityManager,
        ble::vendor::cordio::SigningEventMonitor
    >
>;

typedef ble::generic::GenericGattClient<
    ble::pal::vendor::cordio::CordioGattClient,
    ble::generic::GenericSecurityManager<
        ble::pal::vendor::cordio::CordioSecurityManager,
        ble::vendor::cordio::SigningEventMonitor
    >
> GenericGattClientImpl;

// construct the pal::GattClient
template class ble::pal::vendor::cordio::CordioGattClient<
    GenericGattClientImpl
>;

// construct the final GattClient type
template class ble::interface::GattClient<
    GenericGattClientImpl
>;


// ---------------------------- SecurityManager --------------------------------

template class ble::pal::vendor::cordio::CordioSecurityManager<
    ble::generic::GenericSecurityManager<
        ble::pal::vendor::cordio::CordioSecurityManager,
        ble::vendor::cordio::SigningEventMonitor
    >
>;

template class ble::generic::GenericSecurityManager<
    ble::pal::vendor::cordio::CordioSecurityManager,
    ble::vendor::cordio::SigningEventMonitor
>;


template class ble::interface::SecurityManager<
    ble::generic::GenericSecurityManager<
        ble::pal::vendor::cordio::CordioSecurityManager,
        ble::vendor::cordio::SigningEventMonitor
    >
>;

// --------------------------------- Gap ---------------------------------------

typedef ble::generic::GenericSecurityManager<
    ble::pal::vendor::cordio::CordioSecurityManager,
    ble::vendor::cordio::SigningEventMonitor
> SecurityManagerImpl;

template class ble::pal::vendor::cordio::Gap<
    ble::generic::GenericGap<
        ble::pal::vendor::cordio::Gap,
        SecurityManagerImpl::PalSecurityManager,
        SecurityManagerImpl
    >
>;

template class ble::generic::GenericGap<
    ble::pal::vendor::cordio::Gap,
    SecurityManagerImpl::PalSecurityManager,
    SecurityManagerImpl
>;

template class ble::interface::Gap<
    ble::generic::GenericGap<
        ble::pal::vendor::cordio::Gap,
        SecurityManagerImpl::PalSecurityManager,
        SecurityManagerImpl
    >
>;

template class ble::interface::LegacyGap<
    ble::generic::GenericGap<
        ble::pal::vendor::cordio::Gap,
        SecurityManagerImpl::PalSecurityManager,
        SecurityManagerImpl
    >
>;



