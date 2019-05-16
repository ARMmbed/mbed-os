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
#include "SimpleEventQueue.h"
#include "drivers/LowPowerTimer.h"
#include "SigningMonitorProxy.h"
#include "CordioPalSecurityManager.h"
#include "BleImplementationForward.h"

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
    virtual impl::GenericGapImpl& getGap();

    /**
     * @see BLEInstanceBase::getGap
     */
    virtual const impl::GenericGapImpl& getGap() const;

#if BLE_FEATURE_GATT_SERVER
    /**
     * @see BLEInstanceBase::getGattServer
     */
    virtual GattServer &getGattServer();

    /**
     * @see BLEInstanceBase::getGattServer
     */
    virtual const GattServer &getGattServer() const;
#endif // BLE_FEATURE_GATT_SERVER

#if BLE_FEATURE_GATT_CLIENT
    /**
     * @see BLEInstanceBase::getGattClient
     */
    virtual impl::GenericGattClientImpl &getGattClient();

    /**
     * Get the PAL Gatt Client.
     *
     * @return PAL Gatt Client.
     */
    impl::PalGattClientImpl &getPalGattClient();
#endif // BLE_FEATURE_GATT_CLIENT

#if BLE_FEATURE_SECURITY
    /**
     * @see BLEInstanceBase::getSecurityManager
     */
    virtual SecurityManager &getSecurityManager();

    /**
     * @see BLEInstanceBase::getSecurityManager
     */
    virtual const SecurityManager &getSecurityManager() const;

#endif // BLE_FEATURE_SECURITY

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
    const impl::GenericGapImpl& getGenericGap() const;

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
    mutable SimpleEventQueue _event_queue;
    mbed::LowPowerTimer _timer;
    uint64_t _last_update_us;
};

} // namespace cordio
} // namespace vendor
} // namespace ble


#endif /* CORDIO_BLE_H_ */
