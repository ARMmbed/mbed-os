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

#include "mbed.h"
#include "us_ticker_api.h"
#include "BLE.h"
#include "wsf_types.h"
#include "wsf_msg.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "hci_handler.h"
#include "dm_handler.h"
#include "l2c_handler.h"
#include "att_handler.h"
#include "smp_handler.h"
#include "l2c_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "hci_drv.h"
#include "CordioBLE.h"
#include "mbed_assert.h"

#include "CordioPalAttClient.h"
#include "CordioPalSecurityManager.h"

/*! WSF handler ID */
wsfHandlerId_t stack_handler_id;

/**
 * Weak definition of ble_cordio_get_hci_driver.
 * A runtime error is generated if the user does not define any
 * ble_cordio_get_hci_driver.
 */
MBED_WEAK ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver()
{
    error("Please provide an implementation for the HCI driver");
    ble::vendor::cordio::CordioHCIDriver* bad_instance = NULL;
    return *bad_instance;
}

/**
 * Low level HCI interface between Cordio stack and the port.
 */
extern "C" uint16_t hci_mbed_os_drv_write(uint8_t type, uint16_t len, uint8_t *pData)
{
    return ble_cordio_get_hci_driver().write(type, len, pData);
}

extern "C" void hci_mbed_os_start_reset_sequence(void)
{
    ble_cordio_get_hci_driver().start_reset_sequence();
}

extern "C" void hci_mbed_os_handle_reset_sequence(uint8_t* msg)
{
     ble_cordio_get_hci_driver().handle_reset_sequence(msg);
}

/*
 * This function will signal to the user code by calling signalEventsToProcess.
 * It is registered and called into the Wsf Stack.
 */
extern "C" void wsf_mbed_ble_signal_event(void)
{
    ble::vendor::cordio::BLE::deviceInstance().signalEventsToProcess(::BLE::DEFAULT_INSTANCE);
}

/**
 * BLE-API requires an implementation of the following function in order to
 * obtain its transport handle.
 */
BLEInstanceBase *createBLEInstance()
{
    return (&(ble::vendor::cordio::BLE::deviceInstance()));
}

namespace ble {
namespace vendor {
namespace cordio {

BLE::BLE(CordioHCIDriver& hci_driver) :
    initialization_status(NOT_INITIALIZED),
    instanceID(::BLE::DEFAULT_INSTANCE),
    _event_queue()
{
    _hci_driver = &hci_driver;
    stack_setup();
}

BLE::~BLE() { }

/**
 * The singleton which represents the BLE transport for the BLE.
 */
BLE& BLE::deviceInstance()
{
    static BLE instance(
        ble_cordio_get_hci_driver()
    );
    return instance;
}

ble_error_t BLE::init(
    ::BLE::InstanceID_t instanceID,
    FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext *> initCallback)
{
    switch (initialization_status) {
        case NOT_INITIALIZED:
            _event_queue.initialize(this, instanceID);
            _init_callback = initCallback;
            start_stack_reset();
            return BLE_ERROR_NONE;

        case INITIALIZING:
            return BLE_ERROR_INITIALIZATION_INCOMPLETE;

        case INITIALIZED:
            return BLE_ERROR_NONE;

        default:
            return BLE_ERROR_UNSPECIFIED;
    }
}

bool BLE::hasInitialized() const
{
    return initialization_status == INITIALIZED;
}

ble_error_t BLE::shutdown()
{
    if (initialization_status != INITIALIZED) {
        return BLE_ERROR_INITIALIZATION_INCOMPLETE;
    }

    initialization_status = NOT_INITIALIZED;
    _hci_driver->terminate();

    getGattServer().reset();
    getGattClient().reset();
    getGap().reset();
    _event_queue.clear();

    return BLE_ERROR_NONE;
}

const char* BLE::getVersion()
{
    static const char version[] = "generic-cordio";
    return version;
}

generic::GenericGap& BLE::getGap()
{
    static pal::vendor::cordio::GenericAccessService cordio_gap_service;
    static ble::generic::GenericGap gap(
        _event_queue,
        pal::vendor::cordio::Gap::get_gap(),
        cordio_gap_service,
        pal::vendor::cordio::CordioSecurityManager::get_security_manager()
    );

    return gap;
}

const generic::GenericGap& BLE::getGap() const
{
    BLE &self = const_cast<BLE&>(*this);
    return const_cast<const generic::GenericGap&>(self.getGap());
};

GattServer& BLE::getGattServer()
{
    return cordio::GattServer::getInstance();
}

const GattServer& BLE::getGattServer() const
{
    return cordio::GattServer::getInstance();
}

generic::GenericGattClient& BLE::getGattClient()
{
    static pal::AttClientToGattClientAdapter pal_client(
        pal::vendor::cordio::CordioAttClient::get_client()
    );
    static generic::GenericGattClient client(&pal_client);

    return client;
}

SecurityManager& BLE::getSecurityManager()
{
    static SigningEventMonitorProxy signing_event_monitor(*this);
    static generic::GenericSecurityManager m_instance(
        pal::vendor::cordio::CordioSecurityManager::get_security_manager(),
        getGap(),
        signing_event_monitor
    );

    return m_instance;
}

const SecurityManager& BLE::getSecurityManager() const
{
    const BLE &self = const_cast<BLE&>(*this);
    return const_cast<const SecurityManager&>(self.getSecurityManager());
}

void BLE::waitForEvent()
{
    static Timeout nextTimeout;
    timestamp_t nextTimestamp;
    bool_t pTimerRunning;

    callDispatcher();

    if (wsfOsReadyToSleep()) {
        // setup an mbed timer for the next cordio timeout
        nextTimestamp = (timestamp_t)(WsfTimerNextExpiration(&pTimerRunning) * WSF_MS_PER_TICK) * 1000;
        if (pTimerRunning) {
            nextTimeout.attach_us(timeoutCallback, nextTimestamp);
        }
    }
}

void BLE::processEvents()
{
    callDispatcher();
}

 void BLE::stack_handler(wsfEventMask_t event, wsfMsgHdr_t* msg)
 {
    if (msg == NULL) {
        return;
    }

    if (ble::pal::vendor::cordio::CordioSecurityManager::get_security_manager().sm_handler(msg)) {
        return;
    }

    switch(msg->event) {
        case DM_RESET_CMPL_IND: {
            ::BLE::InitializationCompleteCallbackContext context = {
                ::BLE::Instance(::BLE::DEFAULT_INSTANCE),
                BLE_ERROR_NONE
            };
            deviceInstance().getGattServer().initialize();
            deviceInstance().initialization_status = INITIALIZED;
            _init_callback.call(&context);
        }   break;

        default:
            ble::pal::vendor::cordio::Gap::gap_handler(msg);
            break;
    }
}

void BLE::device_manager_cb(dmEvt_t* dm_event)
{
    BLE::deviceInstance().stack_handler(0, &dm_event->hdr);
}

/*
 * AttServerInitDeInitCback callback is used to Initialize/Deinitialize
 * the CCC Table of the ATT Server when a remote peer requests to Open
 * or Close the connection.
 */
 void BLE::connection_handler(dmEvt_t* dm_event)
 {
    dmConnId_t connId = (dmConnId_t)dm_event->hdr.param;

    switch (dm_event->hdr.event) {
        case DM_CONN_OPEN_IND:
            /* set up CCC table with uninitialized (all zero) values */
            AttsCccInitTable(connId, NULL);
            break;
        case DM_CONN_CLOSE_IND:
            /* clear CCC table on connection close */
            AttsCccClearTable(connId);
            break;
        default:
            break;
    }
}

void BLE::timeoutCallback()
{
    wsf_mbed_ble_signal_event();
}

void BLE::stack_setup()
{
    MBED_ASSERT(_hci_driver != NULL);

    wsfHandlerId_t handlerId;

    buf_pool_desc_t buf_pool_desc = _hci_driver->get_buffer_pool_description();

    // Initialize buffers with the ones provided by the HCI driver
    uint16_t bytes_used = WsfBufInit(
        buf_pool_desc.buffer_size, buf_pool_desc.buffer_memory,
        buf_pool_desc.pool_count, buf_pool_desc.pool_description
    );

    MBED_ASSERT(bytes_used != 0);

    WsfTimerInit();
    SecInit();

    // Note: enable once security is supported
    SecRandInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DmHandler);
    DmAdvInit();
    DmScanInit();
    DmConnInit();
    DmConnMasterInit();
    DmConnSlaveInit();
    DmSecInit();

    // Note: enable once security is supported
    DmSecLescInit();
    DmPrivInit();
    DmHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();
    L2cMasterInit();

    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    AttsInit();
    AttsIndInit();
    AttsSignInit();
    AttsAuthorRegister(GattServer::atts_auth_cb);
    AttcInit();
    AttcSignInit();

    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    SmprInit();
    SmprScInit();
    SmpiInit();
    SmpiScInit();

    stack_handler_id = WsfOsSetNextHandler(&BLE::stack_handler);

    HciSetMaxRxAclLen(100);

    DmRegister(BLE::device_manager_cb);
    DmConnRegister(DM_CLIENT_ID_APP, BLE::device_manager_cb);
    AttConnRegister(BLE::connection_handler);
    AttRegister((attCback_t) ble::pal::vendor::cordio::CordioAttClient::att_client_handler);
}

void BLE::start_stack_reset()
{
    _hci_driver->initialize();
    DmDevReset();
}

void BLE::callDispatcher()
{
    // process the external event queue
    _event_queue.process();

    // follow by stack events
    static uint32_t lastTimeUs = us_ticker_read();
    uint32_t currTimeUs, deltaTimeMs;

    // Update the current cordio time
    currTimeUs = us_ticker_read();
    deltaTimeMs = (currTimeUs - lastTimeUs) / 1000;
    if (deltaTimeMs > 0) {
        WsfTimerUpdate(deltaTimeMs / WSF_MS_PER_TICK);
        lastTimeUs += deltaTimeMs * 1000;
    }

    wsfOsDispatcher();

    if (wsfOsReadyToSleep()) {
        static Timeout nextTimeout;
        // setup an mbed timer for the next Cordio timeout
        bool_t pTimerRunning;
        timestamp_t nextTimestamp = (timestamp_t) (WsfTimerNextExpiration(&pTimerRunning) * WSF_MS_PER_TICK) * 1000;
        if (pTimerRunning) {
            nextTimeout.attach_us(timeoutCallback, nextTimestamp);
        }
    }
}

CordioHCIDriver* BLE::_hci_driver = NULL;

FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> BLE::_init_callback;

} // namespace cordio
} // namespace vendor
} // namespace ble
