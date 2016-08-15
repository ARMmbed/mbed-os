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

#ifndef __BLE_DEVICE_INSTANCE_BASE__
#define __BLE_DEVICE_INSTANCE_BASE__

#include "Gap.h"
#include "ble/SecurityManager.h"
#include "ble/BLE.h"

/* Forward declarations. */
class GattServer;
class GattClient;

/**
 *  The interface for the transport object to be created by the target library's
 *  createBLEInstance().
 *
 * @note This class is part of the interface of BLE API with the implementation;
 *       therefore, it is meant to be used only by porters rather than normal
 *       BLE API users.
 */
class BLEInstanceBase
{
public:
    BLEInstanceBase() {}

    /**
     * Virtual destructor of the interface.
     */
    virtual ~BLEInstanceBase();

    /**
     * Initialize the underlying BLE stack. This should be called before
     * anything else in the BLE API.
     *
     * @param[in] instanceID
     *              The ID of the instance to initialize.
     * @param[in] initCallback
     *              A callback for when initialization completes for a BLE
     *              instance. This is an optional parameter set to NULL when not
     *              supplied.
     *
     * @return BLE_ERROR_NONE if the initialization procedure was started
     *         successfully.
     */
    virtual ble_error_t            init(BLE::InstanceID_t instanceID,
                                        FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> initCallback) = 0;

    /**
     * Check whether the underlying stack has already been initialized,
     * possible with a call to init().
     *
     * @return true if the initialization has completed for the underlying BLE
     *         stack.
     */
    virtual bool                   hasInitialized(void) const = 0;

    /**
     * Shutdown the underlying BLE stack. This includes purging the stack of
     * GATT and GAP state and clearing all state from other BLE components
     * such as the SecurityManager. init() must be called afterwards to
     * re-instantiate services and GAP state.
     *
     * @return BLE_ERROR_NONE if the underlying stack and all other services of
     *         the BLE API were shutdown correctly.
     */
    virtual ble_error_t            shutdown(void)             = 0;

    /**
     * Fetches a string representation of the underlying BLE stack's version.
     *
     * @return A pointer to the string representation of the underlying
     *         BLE stack's version.
     */
    virtual const char *           getVersion(void)           = 0;

    /**
     * Accessor to Gap. This function is used by BLE::gap().
     *
     * @return A reference to a Gap object associated to this BLE instance.
     */
    virtual Gap&                   getGap()                   = 0;

    /**
     * A const alternative to getGap().
     *
     * @return A const reference to a Gap object associated to this BLE instance.
     */
    virtual const Gap&             getGap() const             = 0;

    /**
     * Accessor to GattServer. This function is used by BLE::gattServer().
     *
     * @return A reference to a GattServer object associated to this BLE instance.
     */
    virtual GattServer&            getGattServer()            = 0;

    /**
     * A const alternative to getGattServer().
     *
     * @return A const reference to a GattServer object associated to this BLE instance.
     */
    virtual const GattServer&      getGattServer() const      = 0;

    /**
     * Accessors to GattClient. This function is used by BLE::gattClient().
     *
     * @return A reference to a GattClient object associated to this BLE instance.
     */
    virtual GattClient&            getGattClient()            = 0;

    /**
     * Accessors to SecurityManager. This function is used by BLE::securityManager().
     *
     * @return A reference to a SecurityManager object associated to this BLE instance.
     */
    virtual SecurityManager&       getSecurityManager()       = 0;

    /**
     * A const alternative to getSecurityManager().
     *
     * @return A const reference to a SecurityManager object associated to this BLE instance.
     */
    virtual const SecurityManager& getSecurityManager() const = 0;

    /**
     * Yield control to the BLE stack or to other tasks waiting for events.
     * refer to BLE::waitForEvent().
     */
    virtual void                   waitForEvent(void)         = 0;

    /**
     * Process ALL pending events living in the BLE stack .
     * Return once all events have been consumed.
     */
    virtual void processEvents() = 0;

    /**
     * This function allow the BLE stack to signal that their is work to do and
     * event processing should be done (BLE::processEvent()).
     * @param id: The ID of the BLE instance which does have events to process.
     */
    void signalEventsToProcess(BLE::InstanceID_t id);

private:
    // this class is not a value type.
    // prohibit copy construction and copy assignement
    BLEInstanceBase(const BLEInstanceBase&);
    BLEInstanceBase& operator=(const BLEInstanceBase&);
};

/**
 * BLE uses composition to hide an interface object encapsulating the
 * backend transport.
 *
 * The following API is used to create the singleton interface object. An
 * implementation for this function must be provided by the device-specific
 * library, otherwise there will be a linker error.
 */
extern BLEInstanceBase *createBLEInstance(void);

#endif // ifndef __BLE_DEVICE_INSTANCE_BASE__
