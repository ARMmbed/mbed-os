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
#include "source/GattServer.tpp"
#include "nRF5xGattServer.h"

#include "ble/GattClient.h"
#include "source/GattClient.tpp"
#include "ble/generic/GenericGattClient.h"
#include "source/generic/GenericGattClient.tpp"
#include "nRF5xPalGattClient.h"

#include "ble/gap/Gap.h"
#include "ble/Gap.h"
#include "source/gap/Gap.tpp"
#include "source/LegacyGap.tpp"
#include "nRF5xGap.h"

#include "ble/SecurityManager.h"
#include "source/SecurityManager.tpp"
#include "generic/GenericSecurityManager.h"
#include "source/generic/GenericSecurityManager.tpp"
#include "nRF5xPalSecurityManager.h"
#include "nRF5xPalSecurityManager.tpp"
#include "nRF5xPalGattClient.tpp"

#include "DummySigningEventMonitor.h"

// SECURITY MANAGER

template class ble::generic::GenericSecurityManager<
    ble::pal::vendor::nordic::nRF5xSecurityManager,
    ble::vendor::nordic::DummySigningEventMonitor
>;

template class ::ble::pal::SecurityManager<
    ble::impl::PalSecurityManagerImpl,
    ble::impl::GenericSecurityManagerImpl
>;

template class ble::pal::vendor::nordic::nRF5xSecurityManager<
    ble::generic::GenericSecurityManager<
        ble::pal::vendor::nordic::nRF5xSecurityManager,
        ble::vendor::nordic::DummySigningEventMonitor
    >
>;

template class ble::interface::SecurityManager<
    ble::impl::GenericSecurityManagerImpl
>;

// GATT CLIENT

template class ble::generic::GenericGattClient<
    ble::pal::vendor::nordic::nRF5xGattClient,
    ble::impl::SigningEventHandler
>;

template class ble::pal::vendor::nordic::nRF5xGattClient<
    ble::impl::GenericGattClientImpl
>;

template class ble::interface::GattClient<
    ble::impl::GenericGattClientImpl
>;

// GATT SERVER

template class ble::interface::GattServer<nRF5xGattServer>;

// GAP

// see nRF5xGap.cpp (work around for armcc5)

