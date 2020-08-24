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

/**
 * @file
 */

#ifndef MBED_BLE_DEVICE_INSTANCE_BASE__
#define MBED_BLE_DEVICE_INSTANCE_BASE__

#include "ble/BLE.h"

namespace ble {

/**
 * @addtogroup ble
 * @{
 * @addtogroup porting
 * @{
 */

/**
 * Private interface used to implement the BLE class.
 *
 * The BLE class delegates all its abstract operations to an instance of this
 * abstract class, which every vendor port of Mbed BLE shall implement.
 *
 * The vendor port shall also define an implementation of the freestanding function
 * createBLEInstance(). The BLE API uses this singleton function to gain
 * access to a concrete implementation of this class defined in the vendor port.
 *
 * @attention This class is part of the porting API and is not meant to be used
 * by end users of BLE API.
 *
 * @see BLE
 */
class BLEInstanceBase {
public:
    /**
     * Process ALL pending events living in the vendor BLE subsystem.
     *
     * Return once all pending events have been consumed.
     *
     * @see BLE::processEvents()
     */
    virtual void processEvents() = 0;

    /**
     * Signal to BLE that events needing processing are available.
     *
     * The vendor port shall call this function whenever there are events
     * ready to be processed in the internal stack or BLE subsystem. As a result
     * of this call, the callback registered by the end user via
     * BLE::onEventsToProcess will be invoked.
     * @deprecated Call BLE::signalEventsToProcess directly.
     */
    virtual void signalEventsToProcess()
    {
        BLE::Instance().signalEventsToProcess();
    };

    /**
     * Start the initialization of the vendor BLE subsystem.
     *
     * Calls to this function are initiated by BLE::init, instanceID identify
     * the BLE instance which issue that call while the initCallback is used to
     * signal asynchronously the completion of the initialization process.
     *
     * @param[in] initCallback Callback which the vendor port shall invoke
     * when the initialization completes.
     *
     * This is an optional parameter set to NULL when not supplied.
     *
     * @return BLE_ERROR_NONE if the initialization procedure started
     * successfully.
     *
     * @post initCallback shall be invoked upon completion of the initialization
     * process.
     *
     * @post hasInitialized() shall return false until the initialization is
     * complete, and it shall return true after succesful completion of the
     * initialization process.
     *
     * @see BLE::init()
     */
    virtual ble_error_t init(
        FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> initCallback
    ) = 0;

    /**
     * Check whether the vendor BLE subsystem has been initialized or not.
     *
     * @return true if the initialization has completed for the vendor BLE
     * subsystem.
     *
     * @note this function is invoked by BLE::hasInitialized()
     *
     * @see BLE::init() BLE::hasInitialized()
     */
    virtual bool hasInitialized() const = 0;

    /**
     * Shutdown the vendor BLE subsystem.
     *
     * This operation includes purging the stack of GATT and GAP state and
     * clearing all state from other BLE components, such as the SecurityManager.
     * Clearing all states may be realized by a call to Gap::reset(),
     * GattClient::reset(), GattServer::reset() and SecurityManager::reset().
     *
     * BLE::init() must be called afterward to reinstantiate services and GAP
     * state.
     *
     * @return BLE_ERROR_NONE if the underlying stack and all other services of
     * the BLE API were shut down correctly.
     *
     * @post hasInitialized() shall return false.
     *
     * @note This function is invoked by BLE::shutdown().
     *
     * @see BLE::shutdown() BLE::init() BLE::hasInitialized() Gap::reset()
     * GattClient::reset() GattServer::reset() SecurityManager::reset() .
     */
    virtual ble_error_t shutdown() = 0;

    /**
     * Fetches a NULL terminated string representation of the underlying BLE
     * vendor subsystem.
     *
     * @return A pointer to the NULL terminated string representation of the
     * underlying BLE stack's version.
     *
     * @see BLE::getVersion()
     */
    virtual const char *getVersion() = 0;

    /**
     * Accessor to the vendor implementation of the Gap interface.
     *
     * @return A reference to a Gap object associated to this BLEInstanceBase
     * instance.
     *
     * @see BLE::gap() Gap
     */
    virtual ble::Gap &getGap() = 0;

    /**
     * Const alternative to getGap().
     *
     * @return A const reference to a Gap object associated to this
     * BLEInstanceBase instance.
     *
     * @see BLE::gap() Gap
     */
    virtual const ble::Gap &getGap() const = 0;


#if BLE_FEATURE_GATT_SERVER

    /**
     * Accessor to the vendor implementation of the GattServer interface.
     *
     * @return A reference to a GattServer object associated to this
     * BLEInstanceBase instance.
     *
     * @see BLE::gattServer() GattServer
     */
    virtual ble::GattServer &getGattServer() = 0;

    /**
     * A const alternative to getGattServer().
     *
     * @return A const reference to a GattServer object associated to this
     * BLEInstanceBase instance.
     *
     * @see BLE::gattServer() GattServer
     */
    virtual const ble::GattServer &getGattServer() const = 0;

#endif // BLE_FEATURE_GATT_SERVER

#if BLE_FEATURE_GATT_CLIENT

    /**
     * Accessor to the vendor implementation of the GattClient interface.
     *
     * @return A reference to a GattClient object associated to this
     * BLEInstanceBase instance.
     *
     * @see BLE::gattClient() GattClient
     */
    virtual ble::GattClient &getGattClient() = 0;

#endif

#if BLE_FEATURE_SECURITY

    /**
     * Accessor to the vendor implementation of the SecurityManager interface.
     *
     * @return A reference to a SecurityManager object associated to this
     * BLEInstanceBase instance.
     *
     * @see BLE::securityManager() SecurityManager
     */
    virtual ble::SecurityManager &getSecurityManager() = 0;

    /**
     * A const alternative to getSecurityManager().
     *
     * @return A const reference to a SecurityManager object associated to this
     * BLEInstancebase instance.
     *
     * @see BLE::securityManager() SecurityManager
     */
    virtual const ble::SecurityManager &getSecurityManager() const = 0;

#endif // BLE_FEATURE_SECURITY
};

/**
 * Return the instance of the vendor implementation of BLEInstanceBase.
 *
 * @attention Contrary to its name, this function does not return a new instance
 * at each call. It rather acts like an accessor to a singleton.
 *
 * @attention The vendor library must provide an implementation for this function
 * library. Otherwise, there will be a linker error.
 */
extern ble::BLEInstanceBase *createBLEInstance();

/**
 * @}
 * @}
 */

} // namespace ble

#endif // ifndef MBED_BLE_DEVICE_INSTANCE_BASE__
