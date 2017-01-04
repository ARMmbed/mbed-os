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

#ifndef __NRF51822_H__
#define __NRF51822_H__

#include "ble/BLE.h"
#include "ble/blecommon.h"
#include "ble/BLEInstanceBase.h"

#include "nRF5xGap.h"
#include "nRF5xGattServer.h"
#include "nRF5xGattClient.h"
#include "nRF5xSecurityManager.h"

#include "btle.h"

class nRF5xn : public BLEInstanceBase
{
public:
    nRF5xn(void);
    virtual ~nRF5xn(void);

    virtual ble_error_t init(BLE::InstanceID_t instanceID, FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> callback);
    virtual bool        hasInitialized(void) const {
        return initialized;
    }
    virtual ble_error_t shutdown(void);
    virtual const char *getVersion(void);

    /**
     * Accessors to GAP. This function checks whether gapInstance points to an
     * object. If if does not, then the gapInstance is updated to
     * &_getInstance before returning.
     *
     * @return  A reference to GattServer.
     *
     * @note  Unlike the GattClient, GattServer and SecurityManager, Gap is
     *        always needed in a BLE application. Therefore it is allocated
     *        statically.
     */
    virtual Gap &getGap() {
        return gapInstance;
    };

    /**
     * Accessors to GATT Server. This function checks whether a GattServer
     * object was previously instantiated. If such object does not exist, then
     * it is created before returning.
     *
     * @return  A reference to GattServer.
     */
    virtual GattServer &getGattServer() {
        if (gattServerInstance == NULL) {
            gattServerInstance = new nRF5xGattServer();
        }
        return *gattServerInstance;
    };

    /**
     * Accessors to GATT Client. This function checks whether a GattClient
     * object was previously instantiated. If such object does not exist, then
     * it is created before returning.
     *
     * @return  A reference to GattClient.
     */
    virtual nRF5xGattClient &getGattClient() {
        if (gattClientInstance == NULL) {
            gattClientInstance = new nRF5xGattClient();
        }
        return *gattClientInstance;
    }

    /**
     * Accessors to Security Manager. This function checks whether a SecurityManager
     * object was previously instantiated. If such object does not exist, then
     * it is created before returning.
     *
     * @return  A reference to GattServer.
     */
    virtual nRF5xSecurityManager &getSecurityManager() {
        if (securityManagerInstance == NULL) {
            securityManagerInstance = new nRF5xSecurityManager();
        }
        return *securityManagerInstance;
    }

    /**
     * Accessors to GAP. This function checks whether gapInstance points to an
     * object. If if does not, then the gapInstance is updated to
     * &_getInstance before returning.
     *
     * @return  A const reference to GattServer.
     *
     * @note  Unlike the GattClient, GattServer and SecurityManager, Gap is
     *        always needed in a BLE application. Therefore it is allocated
     *        statically.
     *
     * @note  The accessor is able to modify the object's state because the
     *        internal pointer has been declared mutable.
     */
    virtual const nRF5xGap &getGap() const  {
        return gapInstance;
    };

    /**
     * Accessors to GATT Server. This function checks whether a GattServer
     * object was previously instantiated. If such object does not exist, then
     * it is created before returning.
     *
     * @return  A const reference to GattServer.
     *
     * @note  The accessor is able to modify the object's state because the
     *        internal pointer has been declared mutable.
     */
    virtual const nRF5xGattServer &getGattServer() const {
        if (gattServerInstance == NULL) {
            gattServerInstance = new nRF5xGattServer();
        }
        return *gattServerInstance;
    };

    /**
     * Accessors to Security Manager. This function checks whether a SecurityManager
     * object was previously instantiated. If such object does not exist, then
     * it is created before returning.
     *
     * @return  A const reference to GattServer.
     *
     * @note  The accessor is able to modify the object's state because the
     *        internal pointer has been declared mutable.
     */
    virtual const nRF5xSecurityManager &getSecurityManager() const {
        if (securityManagerInstance == NULL) {
            securityManagerInstance = new nRF5xSecurityManager();
        }
        return *securityManagerInstance;
    }

    virtual void waitForEvent(void);

    virtual void processEvents();

public:
    static nRF5xn& Instance(BLE::InstanceID_t instanceId);

private:
    bool              initialized;
    BLE::InstanceID_t instanceID;

private:
    mutable nRF5xGap gapInstance; /**< Gap instance whose reference is returned from a call to
                                   * getGap(). Unlike the GattClient, GattServer and
                                   * SecurityManager, Gap is always needed in a BLE application. */

private:
    mutable nRF5xGattServer      *gattServerInstance;      /**< Pointer to the GattServer object instance.
                                                            *   If NULL, then GattServer has not been initialized.
                                                            *   The pointer has been declared as 'mutable' so that
                                                            *   it can be assigned inside a 'const' function. */
    mutable nRF5xGattClient      *gattClientInstance;      /**< Pointer to the GattClient object instance.
                                                            *   If NULL, then GattClient has not been initialized.
                                                            *   The pointer has been declared as 'mutable' so that
                                                            *   it can be assigned inside a 'const' function. */
    mutable nRF5xSecurityManager *securityManagerInstance; /**< Pointer to the SecurityManager object instance.
                                                            *   If NULL, then SecurityManager has not been initialized.
                                                            *   The pointer has been declared as 'mutable' so that
                                                            *   it can be assigned inside a 'const' function. */
};

#endif
