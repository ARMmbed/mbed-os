/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#include <stdio.h>
#include "platform/mbed_critical.h"
#include "BLEInstanceBase.h"

#if defined(TARGET_OTA_ENABLED)
#include "services/DFUService.h"
#endif

#include <mbed_error.h>
#include <mbed_toolchain.h>

#include "ble/BLE.h"
#include "BLEInstanceBase.h"

static const char *error_strings[] = {
    "BLE_ERROR_NONE: No error",
    "BLE_ERROR_BUFFER_OVERFLOW: The requested action would cause a buffer overflow and has been aborted",
    "BLE_ERROR_NOT_IMPLEMENTED: Requested a feature that isn't yet implement or isn't supported by the target HW",
    "BLE_ERROR_PARAM_OUT_OF_RANGE: One of the supplied parameters is outside the valid range",
    "BLE_ERROR_INVALID_PARAM: One of the supplied parameters is invalid",
    "BLE_STACK_BUSY: The stack is busy",
    "BLE_ERROR_INVALID_STATE: Invalid state",
    "BLE_ERROR_NO_MEM: Out of Memory",
    "BLE_ERROR_OPERATION_NOT_PERMITTED: The operation requested is not permitted",
    "BLE_ERROR_INITIALIZATION_INCOMPLETE: The BLE subsystem has not completed its initialisation",
    "BLE_ERROR_ALREADY_INITIALIZED: The BLE system has already been initialised",
    "BLE_ERROR_UNSPECIFIED: Unknown error",
    "BLE_ERROR_INTERNAL_STACK_FAILURE: The platform-specific stack failed"
};

const char *BLE::errorToString(ble_error_t error)
{
    if (error > BLE_ERROR_INTERNAL_STACK_FAILURE) {
        return "Illegal error code";
    }
    return error_strings[error];
}

ble_error_t
BLE::initImplementation(FunctionPointerWithContext<InitializationCompleteCallbackContext *> callback)
{
    ble_error_t err = transport.init(callback);
    if (err != BLE_ERROR_NONE) {
        return err;
    }

    /* Platforms enabled for DFU should introduce the DFU Service into
     * applications automatically. */
#if defined(TARGET_OTA_ENABLED)
    static DFUService dfu(*this); // defined static so that the object remains alive
#endif // TARGET_OTA_ENABLED

    return BLE_ERROR_NONE;
}

#if defined(INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS)
#error "INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS no longer supported, you may now create BLE objects freely by passing in the transport. Cordio Transport supports only one instance."
#endif

// this stub is required by ARMCC otherwise link will systematically fail
MBED_WEAK ble::BLEInstanceBase *createBLEInstance()
{
    MBED_ASSERT("No BLE instance implementation.");
    printf("Please provide an implementation for mbed BLE");
    return nullptr;
}

BLE &BLE::Instance()
{
    static ble::BLEInstanceBase *transport = createBLEInstance();

    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED),
                   "bad handle to underlying transport");
    }

    static BLE ble_instance(*transport);

    return ble_instance;
}

bool BLE::hasInitialized(void) const
{
    return transport.hasInitialized();
}

ble_error_t BLE::shutdown(void)
{
    event_signaled = false;
    return transport.shutdown();
}

const char *BLE::getVersion(void)
{
    return transport.getVersion();
}

const ble::Gap &BLE::gap() const
{
    return transport.getGap();
}

ble::Gap &BLE::gap()
{
    return transport.getGap();
}

#if BLE_FEATURE_GATT_SERVER

const ble::GattServer &BLE::gattServer() const
{
    return transport.getGattServer();
}

ble::GattServer &BLE::gattServer()
{
    return transport.getGattServer();
}

#endif // BLE_FEATURE_GATT_SERVER

#if BLE_FEATURE_GATT_CLIENT

const ble::GattClient &BLE::gattClient() const
{
    return transport.getGattClient();
}

ble::GattClient &BLE::gattClient()
{
    return transport.getGattClient();
}

#endif // BLE_FEATURE_GATT_CLIENT

#if BLE_FEATURE_SECURITY

const ble::SecurityManager &BLE::securityManager() const
{
    return transport.getSecurityManager();
}

ble::SecurityManager &BLE::securityManager()
{
    return transport.getSecurityManager();
}

#endif // BLE_FEATURE_SECURITY

void BLE::processEvents()
{
    core_util_critical_section_enter();
    if (event_signaled == false) {
        core_util_critical_section_exit();
        return;
    }

    event_signaled = false;
    core_util_critical_section_exit();

    transport.processEvents();
}

void BLE::onEventsToProcess(const BLE::OnEventsToProcessCallback_t &callback)
{
    whenEventsToProcess = callback;

    // If events were previously signaled but the handler was not in place then
    // signal immediately events availability
    if (event_signaled && whenEventsToProcess) {
        OnEventsToProcessCallbackContext params = {
            *this
        };
        whenEventsToProcess(&params);
    }
}

void BLE::signalEventsToProcess()
{
    core_util_critical_section_enter();
    if (event_signaled == true) {
        core_util_critical_section_exit();
        return;
    }

    event_signaled = true;
    core_util_critical_section_exit();

    if (whenEventsToProcess) {
        OnEventsToProcessCallbackContext params = {
            *this
        };
        whenEventsToProcess(&params);
    }
}

BLE::BLE(ble::BLEInstanceBase &base) :
    transport(base),
    whenEventsToProcess(nullptr),
    event_signaled(false)
{
}
