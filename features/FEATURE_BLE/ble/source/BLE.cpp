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

#include "ble/BLE.h"
#include "ble/BLEInstanceBase.h"

#if defined(TARGET_OTA_ENABLED)
#include "ble/services/DFUService.h"
#endif

#ifdef YOTTA_CFG_MBED_OS
#include <minar/minar.h>
#endif

#if !defined(YOTTA_CFG_MBED_OS)
#include <mbed_error.h>
#include <toolchain.h>
#endif

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
    error("Please provide an implementation for mbed BLE");
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


BLE::BLE(InstanceID_t instanceIDIn) : instanceID(instanceIDIn), transport(),
    whenEventsToProcess(defaultSchedulingCallback)
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

bool BLE::hasInitialized(void) const
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->hasInitialized();
}

ble_error_t BLE::shutdown(void)
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->shutdown();
}

const char *BLE::getVersion(void)
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getVersion();
}

const Gap &BLE::gap() const
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getGap();
}

Gap &BLE::gap()
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getGap();
}

const GattServer& BLE::gattServer() const
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getGattServer();
}

GattServer& BLE::gattServer()
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getGattServer();
}

const GattClient& BLE::gattClient() const
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getGattClient();
}

GattClient& BLE::gattClient()
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getGattClient();
}

const SecurityManager& BLE::securityManager() const
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getSecurityManager();
}

SecurityManager& BLE::securityManager()
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    return transport->getSecurityManager();
}

void BLE::waitForEvent(void)
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    transport->waitForEvent();
}

void BLE::processEvents()
{
    if (!transport) {
        error("bad handle to underlying transport");
    }

    transport->processEvents();
}

void BLE::onEventsToProcess(const BLE::OnEventsToProcessCallback_t& callback)
{
    whenEventsToProcess = callback;
}

void BLE::signalEventsToProcess()
{
    if (whenEventsToProcess) {
        OnEventsToProcessCallbackContext params = {
            *this
        };
        whenEventsToProcess(&params);
    }
}
