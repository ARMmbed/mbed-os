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

#include "mbed.h"
#include "us_ticker_api.h"
#include "ble/BLE.h"
#include "CriticalSectionLock.h"
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
#include "mbed_assert.h"
#include "bstream.h"

#include "ble/internal/cordio/CordioPalAttClient.h"
#include "ble/internal/cordio/CordioPalSecurityManager.h"
#include "ble/internal/cordio/CordioPalGap.h"
#include "ble/internal/cordio/CordioPalSigningMonitor.h"
#include "ble/internal/cordio/CordioBLEInstanceBase.h"
#include "CordioHCIDriver.h"

using namespace std::chrono;

/*! WSF handler ID */
wsfHandlerId_t stack_handler_id;

/* WSF heap allocation */
uint8_t *SystemHeapStart;
uint32_t SystemHeapSize;

/**
 * Weak definition of ble_cordio_get_hci_driver.
 * A runtime error is generated if the user does not define any
 * ble_cordio_get_hci_driver.
 */
MBED_WEAK ble::CordioHCIDriver& ble_cordio_get_hci_driver()
{
    MBED_ASSERT("No HCI driver");
    printf("Please provide an implementation for the HCI driver");
    ble::CordioHCIDriver* bad_instance = nullptr;
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
extern "C" MBED_WEAK void wsf_mbed_ble_signal_event(void)
{
    ble::CordioBLEInstanceBase::deviceInstance().signalEventsToProcess(::BLE::DEFAULT_INSTANCE);
}

/**
 * BLE-API requires an implementation of the following function in order to
 * obtain its transport handle.
 */
BLEInstanceBase *createBLEInstance()
{
    return (&(ble::CordioBLEInstanceBase::deviceInstance()));
}

namespace ble {

CordioBLEInstanceBase::CordioBLEInstanceBase(CordioHCIDriver& hci_driver) :
    initialization_status(NOT_INITIALIZED),
    instanceID(::BLE::DEFAULT_INSTANCE),
    _event_queue(),
    _last_update_us(0)
{
    _hci_driver = &hci_driver;
    stack_setup();
}

CordioBLEInstanceBase::~CordioBLEInstanceBase() { }

/**
 * The singleton which represents the BLE transport for the BLE.
 */
CordioBLEInstanceBase& CordioBLEInstanceBase::deviceInstance()
{
    static CordioBLEInstanceBase instance(
        ble_cordio_get_hci_driver()
    );
    return instance;
}

ble_error_t CordioBLEInstanceBase::init(
    ::BLE::InstanceID_t instanceID,
    FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext *> initCallback)
{
    switch (initialization_status) {
        case NOT_INITIALIZED:
            _timer.reset();
            _timer.start();
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

bool CordioBLEInstanceBase::hasInitialized() const
{
    return initialization_status == INITIALIZED;
}

ble_error_t CordioBLEInstanceBase::shutdown()
{
    if (initialization_status != INITIALIZED) {
        return BLE_ERROR_INITIALIZATION_INCOMPLETE;
    }

    initialization_status = NOT_INITIALIZED;
    _hci_driver->terminate();

#if BLE_FEATURE_GATT_SERVER
    getGattServer().reset();
#endif

#if BLE_FEATURE_GATT_CLIENT
    getGattClient().reset();
#endif // BLE_FEATURE_GATT_CLIENT

    getGap().reset();
    _event_queue.clear();

    return BLE_ERROR_NONE;
}

const char* CordioBLEInstanceBase::getVersion()
{
    static const char version[] = "generic-cordio";
    return version;
}

ble::Gap& CordioBLEInstanceBase::getGap()
{
    static ble::PalGenericAccessService cordio_gap_service;
    static ble::Gap gap(
        _event_queue,
        ble::PalGap::get_gap(),
        cordio_gap_service,
        ble::PalSecurityManager::get_security_manager()
    );

    return gap;
}

const ble::Gap& CordioBLEInstanceBase::getGap() const
{
    CordioBLEInstanceBase &self = const_cast<CordioBLEInstanceBase&>(*this);
    return const_cast<const ble::Gap&>(self.getGap());
};

#if BLE_FEATURE_GATT_SERVER
GattServer& CordioBLEInstanceBase::getGattServer()
{
    return GattServer::getInstance();
}

const GattServer& CordioBLEInstanceBase::getGattServer() const
{
    return GattServer::getInstance();
}
#endif // BLE_FEATURE_GATT_SERVER

#if BLE_FEATURE_GATT_CLIENT
ble::GattClient& CordioBLEInstanceBase::getGattClient()
{
    static ble::GattClient gatt_client(getPalGattClient());

    return gatt_client;
}

PalGattClient& CordioBLEInstanceBase::getPalGattClient()
{
    static PalGattClient pal_gatt_client(PalAttClient::get_client());
    return pal_gatt_client;
}
#endif // BLE_FEATURE_GATT_CLIENT

#if BLE_FEATURE_SECURITY
SecurityManager& CordioBLEInstanceBase::getSecurityManager()
{
    static PalSigningMonitor signing_event_monitor;
    static ble::SecurityManager m_instance(
        ble::PalSecurityManager::get_security_manager(),
        getGap(),
        signing_event_monitor
    );

    return m_instance;
}

const SecurityManager& CordioBLEInstanceBase::getSecurityManager() const
{
    const CordioBLEInstanceBase &self = const_cast<CordioBLEInstanceBase&>(*this);
    return const_cast<const SecurityManager&>(self.getSecurityManager());
}
#endif // BLE_FEATURE_SECURITY

void CordioBLEInstanceBase::waitForEvent()
{
    static Timeout nextTimeout;
    timestamp_t nextTimestamp;
    bool_t pTimerRunning;

    callDispatcher();

    if (wsfOsReadyToSleep()) {
        // setup an mbed timer for the next cordio timeout
        nextTimestamp = (timestamp_t)(WsfTimerNextExpiration(&pTimerRunning) * WSF_MS_PER_TICK);
        if (pTimerRunning) {
            nextTimeout.attach(timeoutCallback, milliseconds(nextTimestamp));
        }
    }
}

void CordioBLEInstanceBase::processEvents()
{
    callDispatcher();
}

 void CordioBLEInstanceBase::stack_handler(wsfEventMask_t event, wsfMsgHdr_t* msg)
 {
    if (msg == nullptr) {
        return;
    }

#if BLE_FEATURE_SECURITY
    if (ble::PalSecurityManager::get_security_manager().sm_handler(msg)) {
        return;
    }
#endif // BLE_FEATURE_SECURITY

    switch(msg->event) {
        case DM_RESET_CMPL_IND: {
            ::BLE::InitializationCompleteCallbackContext context = {
                ::BLE::Instance(::BLE::DEFAULT_INSTANCE),
                BLE_ERROR_NONE
            };
#if BLE_FEATURE_EXTENDED_ADVERTISING
            // initialize extended module if supported
            if (HciGetLeSupFeat() & HCI_LE_SUP_FEAT_LE_EXT_ADV) {
#if BLE_ROLE_BROADCASTER
                DmExtAdvInit();
#endif // BLE_ROLE_BROADCASTER
#if BLE_ROLE_OBSERVER
                DmExtScanInit();
#endif // BLE_ROLE_OBSERVER
#if BLE_ROLE_CENTRAL
                DmExtConnMasterInit();
#endif // BLE_ROLE_CENTRAL
#if BLE_ROLE_PERIPHERAL
                DmExtConnSlaveInit();
#endif // BLE_ROLE_PERIPHERAL
            }
#endif // BLE_FEATURE_EXTENDED_ADVERTISING
#if BLE_FEATURE_GATT_SERVER
            deviceInstance().getGattServer().initialize();
#endif
            deviceInstance().initialization_status = INITIALIZED;
            _init_callback.call(&context);
        }   break;
#if MBED_CONF_CORDIO_ROUTE_UNHANDLED_COMMAND_COMPLETE_EVENTS
        case DM_UNHANDLED_CMD_CMPL_EVT_IND: {
            // upcast to unhandled command complete event to access the payload
            hciUnhandledCmdCmplEvt_t* unhandled = (hciUnhandledCmdCmplEvt_t*)msg;
            if (unhandled->hdr.status == HCI_SUCCESS && unhandled->hdr.param == HCI_OPCODE_LE_TEST_END) {
                // unhandled events are not parsed so we need to parse the payload ourselves
                uint8_t status;
                uint16_t packet_number;
                status = unhandled->param[0];
                BYTES_TO_UINT16(packet_number, unhandled->param + 1);

                _hci_driver->handle_test_end(status == 0, packet_number);
                return;
            }
        }   break;
#endif // MBED_CONF_CORDIO_ROUTE_UNHANDLED_COMMAND_COMPLETE_EVENTS

        default:
            ble::PalGap::gap_handler(msg);
            break;
    }
}

void CordioBLEInstanceBase::device_manager_cb(dmEvt_t* dm_event)
{
    if (dm_event->hdr.status == HCI_SUCCESS && dm_event->hdr.event == DM_CONN_DATA_LEN_CHANGE_IND) {
        // this event can only happen after a connection has been established therefore gap is present
        ble::PalGapEventHandler *handler;
        handler = ble::PalGap::get_gap().get_event_handler();
        if (handler) {
            handler->on_data_length_change(
                dm_event->hdr.param,
                dm_event->dataLenChange.maxTxOctets,
                dm_event->dataLenChange.maxRxOctets
            );
        }
        return;
    }

    CordioBLEInstanceBase::deviceInstance().stack_handler(0, &dm_event->hdr);
}

/*
 * AttServerInitDeInitCback callback is used to Initialize/Deinitialize
 * the CCC Table of the ATT Server when a remote peer requests to Open
 * or Close the connection.
 */
 void CordioBLEInstanceBase::connection_handler(dmEvt_t* dm_event)
 {
    dmConnId_t connId = (dmConnId_t)dm_event->hdr.param;

    switch (dm_event->hdr.event) {
        case DM_CONN_OPEN_IND:
            /* set up CCC table with uninitialized (all zero) values */
            AttsCccInitTable(connId, nullptr);
            break;
        case DM_CONN_CLOSE_IND:
            /* clear CCC table on connection close */
            AttsCccClearTable(connId);
            break;
        default:
            break;
    }
}

void CordioBLEInstanceBase::timeoutCallback()
{
    wsf_mbed_ble_signal_event();
}

void CordioBLEInstanceBase::stack_setup()
{
    MBED_ASSERT(_hci_driver != nullptr);

    wsfHandlerId_t handlerId;

    buf_pool_desc_t buf_pool_desc = _hci_driver->get_buffer_pool_description();

    // use the buffer for the WSF heap
    SystemHeapStart = buf_pool_desc.buffer_memory;
    SystemHeapSize = buf_pool_desc.buffer_size;

    // Initialize buffers with the ones provided by the HCI driver
    uint16_t bytes_used = WsfBufInit(buf_pool_desc.pool_count, (wsfBufPoolDesc_t*)buf_pool_desc.pool_description);

    // Raise assert if not enough memory was allocated
    MBED_ASSERT(bytes_used != 0);

    SystemHeapStart += bytes_used;
    SystemHeapSize -= bytes_used;

    // This warning will be raised if we've allocated too much memory
    if(bytes_used < buf_pool_desc.buffer_size)
    {
        MBED_WARNING1(MBED_MAKE_ERROR(MBED_MODULE_BLE, MBED_ERROR_CODE_INVALID_SIZE), "Too much memory allocated for Cordio memory pool, reduce buf_pool_desc.buffer_size by value below.", buf_pool_desc.buffer_size - bytes_used);
    }

    WsfTimerInit();

    // Note: SecInit required for RandInit.
    SecInit();
    SecRandInit();

#if BLE_FEATURE_SECURITY
    SecAesInit();
    SecCmacInit();
    SecEccInit();
#endif

    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(DmHandler);

#if BLE_ROLE_BROADCASTER
    DmAdvInit();
#endif

#if BLE_ROLE_OBSERVER
    DmScanInit();
#endif

#if BLE_FEATURE_CONNECTABLE
    DmConnInit();
#endif

#if BLE_ROLE_CENTRAL
    DmConnMasterInit();
#endif

#if BLE_ROLE_PERIPHERAL
    DmConnSlaveInit();
#endif

#if BLE_FEATURE_SECURITY
    DmSecInit();
#endif

#if BLE_FEATURE_PHY_MANAGEMENT
    DmPhyInit();
#endif

#if BLE_FEATURE_SECURE_CONNECTIONS
    DmSecLescInit();
#endif

#if BLE_FEATURE_PRIVACY
    DmPrivInit();
#endif

    DmHandlerInit(handlerId);

#if BLE_ROLE_PERIPHERAL
    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
#endif

#if BLE_FEATURE_CONNECTABLE
    L2cInit();
#endif

#if BLE_ROLE_PERIPHERAL
    L2cSlaveInit();
#endif

#if BLE_ROLE_CENTRAL
    L2cMasterInit();
#endif

#if BLE_FEATURE_ATT
    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);

#if BLE_FEATURE_GATT_SERVER
    AttsInit();
    AttsIndInit();
#if BLE_FEATURE_SECURITY
    AttsAuthorRegister(GattServer::atts_auth_cb);
#endif
#if BLE_FEATURE_SIGNING
    AttsSignInit();
#endif
#endif // BLE_FEATURE_GATT_SERVER

#if BLE_FEATURE_GATT_CLIENT
    AttcInit();
#if BLE_FEATURE_SIGNING
    AttcSignInit();
#endif
#endif // BLE_FEATURE_GATT_CLIENT

#endif // BLE_FEATURE_ATT

#if BLE_FEATURE_SECURITY
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);

#if BLE_ROLE_PERIPHERAL
    SmprInit();
#if BLE_FEATURE_SECURE_CONNECTIONS
    SmprScInit();
#endif
#endif

#if BLE_ROLE_CENTRAL
    SmpiInit();
#if BLE_FEATURE_SECURE_CONNECTIONS
    SmpiScInit();
#endif
#endif // BLE_ROLE_CENTRAL

#endif // BLE_FEATURE_SECURITY

    stack_handler_id = WsfOsSetNextHandler(&CordioBLEInstanceBase::stack_handler);

    HciSetMaxRxAclLen(MBED_CONF_CORDIO_RX_ACL_BUFFER_SIZE);

    DmRegister(CordioBLEInstanceBase::device_manager_cb);
#if BLE_FEATURE_CONNECTABLE
    DmConnRegister(DM_CLIENT_ID_APP, CordioBLEInstanceBase::device_manager_cb);
#endif

#if BLE_FEATURE_GATT_SERVER
    AttConnRegister(CordioBLEInstanceBase::connection_handler);
#endif

#if BLE_FEATURE_ATT
#if BLE_FEATURE_GATT_CLIENT
    AttRegister((attCback_t) ble::PalAttClient::att_client_handler);
#else
    AttRegister((attCback_t) ble::GattServer::att_cb);
#endif // BLE_FEATURE_GATT_CLIENT
#endif
}

void CordioBLEInstanceBase::start_stack_reset()
{
    _hci_driver->initialize();
    DmDevReset();
}

void CordioBLEInstanceBase::callDispatcher()
{
    // process the external event queue
    _event_queue.process();

    _last_update_us += (uint64_t)_timer.elapsed_time().count();
    _timer.reset();

    uint64_t last_update_ms   = (_last_update_us / 1000);
    wsfTimerTicks_t wsf_ticks = (last_update_ms / WSF_MS_PER_TICK);

    if (wsf_ticks > 0) {
        WsfTimerUpdate(wsf_ticks);

        _last_update_us -= (last_update_ms * 1000);
    }

    wsfOsDispatcher();

    static LowPowerTimeout nextTimeout;
    CriticalSectionLock critical_section;

    if (wsfOsReadyToSleep()) {
        // setup an mbed timer for the next Cordio timeout
        bool_t pTimerRunning;
        timestamp_t nextTimestamp = (timestamp_t) (WsfTimerNextExpiration(&pTimerRunning) * WSF_MS_PER_TICK);
        if (pTimerRunning) {
            nextTimeout.attach(timeoutCallback, milliseconds(nextTimestamp));
        } else {
            critical_section.disable();
            _hci_driver->on_host_stack_inactivity();
        }
    }
}

CordioHCIDriver* CordioBLEInstanceBase::_hci_driver = nullptr;

FunctionPointerWithContext< ::BLE::InitializationCompleteCallbackContext*> CordioBLEInstanceBase::_init_callback;

} // namespace ble
