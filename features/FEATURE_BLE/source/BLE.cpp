/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "ble/BLE.h"
#include "ble/BLEInstanceBase.h"
#include "platform/mbed_critical.h"
#include "Deprecated.h"

#if defined(TARGET_OTA_ENABLED)
#include "ble/services/DFUService.h"
#endif

#ifdef YOTTA_CFG_MBED_OS
#include <minar/minar.h>
#endif

#if !defined(YOTTA_CFG_MBED_OS)
#include <mbed_error.h>
#include <mbed_toolchain.h>
#endif

static const char* error_strings[] = {
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

const char* BLE::errorToString(ble_error_t error)
{
    if (error > BLE_ERROR_INTERNAL_STACK_FAILURE) {
        return "Illegal error code";
    }
    return error_strings[error];
}

ble_error_t
BLE::initImplementation(FunctionPointerWithContext<InitializationCompleteCallbackContext*> callback)
{
    ble_error_t err = transport->init(instanceID, callback);
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

/**
 * BLE::Instance() and BLE constructor rely upon a static array of initializers
 * to create actual BLE transport instances. A description of these instances
 * and initializers is supposed to be put in some .json file contributing to
 * yotta's configuration (typically in the target definition described by
 * target.json). Here's a sample:
 *
 *  "config": {
 *    ...
 *    "ble_instances": {
 *      "count": 1,
 *      "0" : {
 *        "initializer" : "createBLEInstance"
 *      }
 *    }
 *    ...
 *  }
 *
 * The following macros result in translating the above config into a static
 * array: instanceConstructors.
 */
#ifdef YOTTA_CFG_BLE_INSTANCES_COUNT
#define CONCATENATE(A, B) A ## B
#define EXPAND(X) X /* this adds a level of indirection needed to allow macro-expansion following a token-paste operation (see use of CONCATENATE() below). */

#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_1 YOTTA_CFG_BLE_INSTANCES_0_INITIALIZER
#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_2 INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_1, YOTTA_CFG_BLE_INSTANCES_1_INITIALIZER
#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_3 INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_2, YOTTA_CFG_BLE_INSTANCES_2_INITIALIZER
#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_4 INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_3, YOTTA_CFG_BLE_INSTANCES_3_INITIALIZER
#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_5 INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_4, YOTTA_CFG_BLE_INSTANCES_4_INITIALIZER
/* ... add more of the above if ever needed */

#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS(N) EXPAND(CONCATENATE(INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS_, N))
#elif !defined(INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS)
/*
 * The following applies when building without yotta. By default BLE_API provides
 * a trivial initializer list containing a single constructor: createBLEInstance.
 * This may be overridden.
 */
#define INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS createBLEInstance

// yotta unlike mbed-cli has proper dependency mechanisms
// It is not required to defined a stub for createBLEInstance
#if !defined(YOTTA_CFG_MBED_OS)

// this stub is required by ARMCC otherwise link will systematically fail
MBED_WEAK BLEInstanceBase* createBLEInstance() {
    MBED_ASSERT("No BLE instance implementation.");
    printf("Please provide an implementation for mbed BLE");
    return NULL;
}

#endif


#endif /* YOTTA_CFG_BLE_INSTANCES_COUNT */

typedef BLEInstanceBase *(*InstanceConstructor_t)(void);
static const InstanceConstructor_t instanceConstructors[BLE::NUM_INSTANCES] = {
#ifndef YOTTA_CFG_BLE_INSTANCES_COUNT
    INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS
#else
    INITIALIZER_LIST_FOR_INSTANCE_CONSTRUCTORS(YOTTA_CFG_BLE_INSTANCES_COUNT)
#endif
};

BLE &
BLE::Instance(InstanceID_t id)
{
    static BLE *singletons[NUM_INSTANCES];
    if (id < NUM_INSTANCES) {
        if (singletons[id] == NULL) {
            singletons[id] = new BLE(id); /* This object will never be freed. */
        }

        return *singletons[id];
    }

    /* we come here only in the case of a bad interfaceID. */
    static BLE badSingleton(NUM_INSTANCES /* this is a bad index; and will result in a NULL transport. */);
    return badSingleton;
}

#ifdef YOTTA_CFG_MBED_OS
void defaultSchedulingCallback(BLE::OnEventsToProcessCallbackContext* params) {
    minar::Scheduler::postCallback(&params->ble, &BLE::processEvents);
}
#else
#define defaultSchedulingCallback NULL
#endif

bool BLE::hasInitialized(void) const
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->hasInitialized();
}

ble_error_t BLE::shutdown(void)
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    event_signaled = false;
    return transport->shutdown();
}

const char *BLE::getVersion(void)
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getVersion();
}

const Gap &BLE::gap() const
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getGap();
}

Gap &BLE::gap()
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getGap();
}

#if BLE_FEATURE_GATT_SERVER

const GattServer& BLE::gattServer() const
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getGattServer();
}

GattServer& BLE::gattServer()
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getGattServer();
}

#endif // BLE_FEATURE_GATT_SERVER

#if BLE_FEATURE_GATT_CLIENT

const GattClient& BLE::gattClient() const
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getGattClient();
}

GattClient& BLE::gattClient()
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getGattClient();
}

#endif // BLE_FEATURE_GATT_CLIENT

#if BLE_FEATURE_SECURITY

const SecurityManager& BLE::securityManager() const
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getSecurityManager();
}

SecurityManager& BLE::securityManager()
{
    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    return transport->getSecurityManager();
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

    if (!transport) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_BLE_BACKEND_NOT_INITIALIZED), "bad handle to underlying transport");
    }

    transport->processEvents();
}

void BLE::onEventsToProcess(const BLE::OnEventsToProcessCallback_t& callback)
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

BLE::BLE(InstanceID_t instanceIDIn) : instanceID(instanceIDIn), transport(),
    whenEventsToProcess(defaultSchedulingCallback),
    event_signaled(false)
{
    static BLEInstanceBase *transportInstances[NUM_INSTANCES];

    if (instanceID < NUM_INSTANCES) {
        if (!transportInstances[instanceID]) {
            transportInstances[instanceID] = instanceConstructors[instanceID](); /* Call the stack's initializer for the transport object. */
        }
        transport = transportInstances[instanceID];
    } else {
        transport = NULL;
    }
}

