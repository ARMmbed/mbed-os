/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#ifndef CORDIO_BLE_H_
#define CORDIO_BLE_H_

#include "ble/BLE.h"
#include "ble/blecommon.h"
#include "ble/BLEInstanceBase.h"

#include "CordioHCIDriver.h"
#include "CordioGattServer.h"
#include "CordioPalAttClient.h"
#include "ble/pal/AttClientToGattClientAdapter.h"
#include "ble/generic/GenericGattClient.h"
#include "CordioPalGap.h"
#include "CordioPalGenericAccessService.h"
#include "ble/generic/GenericGap.h"
#include "ble/generic/GenericSecurityManager.h"
#include "ble/pal/SimpleEventQueue.h"

namespace ble {
namespace vendor {
namespace cordio {

/**
 * @see BLEInstanceBase
 */
class BLE : public ::BLEInstanceBase {
    /**
     * Construction with an HCI driver.
     */
    BLE(CordioHCIDriver& hci_driver);

    /**
     * Destructor
     */
    virtual ~BLE();

public:

    /**
     * Access to the singleton containing the implementation of BLEInstanceBase
     * for the Cordio stack.
     */
    static BLE& deviceInstance();

    /**
    * @see BLEInstanceBase::init
    */
    virtual ble_error_t init(
        ::BLE::InstanceID_t instanceID,
        FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> initCallback
    );

    /**
     * @see BLEInstanceBase::hasInitialized
     */
    virtual bool hasInitialized() const;

    /**
     * @see BLEInstanceBase::shutdown
     */
    virtual ble_error_t shutdown();

    /**
     * @see BLEInstanceBase::getVersion
     */
    virtual const char *getVersion();

    /**
     * @see BLEInstanceBase::getGap
     */
    virtual generic::GenericGap& getGap();

    /**
     * @see BLEInstanceBase::getGap
     */
    virtual const generic::GenericGap& getGap() const;

    /**
     * @see BLEInstanceBase::getGattServer
     */
    virtual GattServer &getGattServer();

    /**
     * @see BLEInstanceBase::getGattServer
     */
    virtual const GattServer &getGattServer() const;

    /**
     * @see BLEInstanceBase::getGattClient
     */
    virtual generic::GenericGattClient &getGattClient();

    /**
     * @see BLEInstanceBase::getSecurityManager
     */
    virtual SecurityManager &getSecurityManager();

    /**
     * @see BLEInstanceBase::getSecurityManager
     */
    virtual const SecurityManager &getSecurityManager() const;

    /**
     * @see BLEInstanceBase::waitForEvent
     */
    virtual void waitForEvent();

    /**
     * @see BLEInstanceBase::processEvents
     */
    virtual void processEvents();

private:
    /**
     * Return singleton.
     * @return GenericGap instance.
     */
    const generic::GenericGap& getGenericGap() const;

    static void stack_handler(wsfEventMask_t event, wsfMsgHdr_t* msg);
    static void device_manager_cb(dmEvt_t* dm_event);
    static void connection_handler(dmEvt_t* dm_event);
    static void timeoutCallback();

    void stack_setup();
    void start_stack_reset();
    void callDispatcher();

    static CordioHCIDriver* _hci_driver;
    static FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> _init_callback;

    enum {
        NOT_INITIALIZED,
        INITIALIZING,
        INITIALIZED
    } initialization_status;

    ::BLE::InstanceID_t instanceID;
    mutable pal::SimpleEventQueue _event_queue;

    class SigningEventMonitorProxy : public pal::SigningEventMonitor {
    public:
        SigningEventMonitorProxy(BLE &ble) : _ble(ble) { }
        virtual void set_signing_event_handler(pal::SigningEventMonitor::EventHandler *handler) {
            _ble.getGattClient().set_signing_event_handler(handler);
            _ble.getGattServer().set_signing_event_handler(handler);
        }
    private:
        BLE &_ble;
    };
};

} // namespace cordio
} // namespace vendor
} // namespace ble


#endif /* CORDIO_BLE_H_ */
