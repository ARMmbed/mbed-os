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
#include "nRF5xGattClient.h"

#include "ble/gap/Gap.h"
#include "ble/Gap.h"
#include "source/gap/Gap.tpp"
#include "source/LegacyGap.tpp"
#include "nRF5xGap.h"

#include "ble/SecurityManager.h"
#include "source/SecurityManager.tpp"
#include "nRF5xSecurityManager.h"

template class ble::interface::GattServer<nRF5xGattServer>;

template class ble::interface::GattClient<nRF5xGattClient>;

template class ble::interface::LegacyGap<nRF5xGap>;
template class ble::interface::Gap<nRF5xGap>;

template class ble::interface::SecurityManager<nRF5xSecurityManager>;
