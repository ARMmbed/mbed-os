/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */

#ifndef _MAXIMBLE_H_
#define _MAXIMBLE_H_

#include "ble/BLE.h"
#include "ble/blecommon.h"
#include "ble/BLEInstanceBase.h"

#include "MaximGap.h"
#include "MaximGattServer.h"
#include "MaximGattClient.h"
#include "MaximSecurityManager.h"

class MaximBLE : public BLEInstanceBase
{
public:
    MaximBLE(void);
    virtual ~MaximBLE(void);

    virtual ble_error_t init(BLE::InstanceID_t instanceID, FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> initCallback);
    virtual bool        hasInitialized(void) const {
        return initialized;
    }
    virtual ble_error_t shutdown(void);
    virtual const char *getVersion(void);

    virtual Gap &getGap() {
        return MaximGap::getInstance();
    };
    virtual const Gap &getGap() const {
        return MaximGap::getInstance();
    };
    virtual GattServer &getGattServer() {
        return MaximGattServer::getInstance();
    };
    virtual const GattServer &getGattServer() const {
        return MaximGattServer::getInstance();
    };
    virtual GattClient &getGattClient() {
        return MaximGattClient::getInstance();
    };
    virtual SecurityManager &getSecurityManager() {
        return MaximSecurityManager::getInstance();
    };
    virtual const SecurityManager &getSecurityManager() const {
        return MaximSecurityManager::getInstance();
    };

    virtual void waitForEvent(void);
    virtual void processEvents(void);

private:
    bool              initialized;
    BLE::InstanceID_t instanceID;

    static void timeoutCallback(void);
    void callDispatcher(void);
};

#endif /* _MAXIMBLE_H_ */
