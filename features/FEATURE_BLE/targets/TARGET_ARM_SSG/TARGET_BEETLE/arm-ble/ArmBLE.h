/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ARMBLE_H_
#define _ARMBLE_H_

#include "BLE.h"
#include "blecommon.h"
#include "BLEInstanceBase.h"

#include "ArmGap.h"
#include "ArmGattServer.h"
#include "ArmGattClient.h"
#include "ArmSecurityManager.h"

class ArmBLE : public BLEInstanceBase
{
public:
    ArmBLE(void);
    virtual ~ArmBLE(void);

    virtual ble_error_t init(BLE::InstanceID_t instanceID, FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> initCallback);
    virtual bool        hasInitialized(void) const {
        return initialized;
    }
    virtual ble_error_t shutdown(void);
    virtual const char *getVersion(void);

    virtual Gap &getGap() {
        return ArmGap::getInstance();
    };
    virtual const Gap &getGap() const {
        return ArmGap::getInstance();
    };
    virtual GattServer &getGattServer() {
        return ArmGattServer::getInstance();
    };
    virtual const GattServer &getGattServer() const {
        return ArmGattServer::getInstance();
    };
    virtual GattClient &getGattClient() {
        return ArmGattClient::getInstance();
    };
    virtual SecurityManager &getSecurityManager() {
        return ArmSecurityManager::getInstance();
    };
    virtual const SecurityManager &getSecurityManager() const {
        return ArmSecurityManager::getInstance();
    };

    virtual void waitForEvent(void);

    virtual void processEvents();

private:
    bool              initialized;
    BLE::InstanceID_t instanceID;

    static void timeoutCallback(void);
    void callDispatcher(void);
};

#endif /* _ARMBLE_H_ */
