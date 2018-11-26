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

#ifdef YOTTA_CFG_MBED_OS
    #include "mbed-drivers/mbed.h"
#else
    #include "mbed.h"
#endif
#include "nRF5xn.h"
#include "ble/blecommon.h"
#include "nrf_soc.h"

#include "btle/btle.h"
#include "btle/custom/custom_helper.h"
#include "nrf_delay.h"

extern "C" {
#include "nrf_sdh.h"
}

#include "nRF5xPalGattClient.h"

/**
 * The singleton which represents the nRF51822 transport for the BLE.
 */
static nRF5xn& getDeviceInstance() {
    static nRF5xn deviceInstance;
    return deviceInstance;
}


/**
 * BLE-API requires an implementation of the following function in order to
 * obtain its transport handle.
 */
BLEInstanceBase *
createBLEInstance(void)
{
    return &nRF5xn::Instance(BLE::DEFAULT_INSTANCE);
}

nRF5xn& nRF5xn::Instance(BLE::InstanceID_t instanceId)
{
    return getDeviceInstance();
}

nRF5xn::nRF5xn(void) :
    initialized(false),
    instanceID(BLE::DEFAULT_INSTANCE),
    gapInstance(),
    gattServerInstance(NULL),
    gattClient(&(ble::pal::vendor::nordic::nRF5xGattClient::get_client()))
{
}

nRF5xn::~nRF5xn(void)
{
}

const char *nRF5xn::getVersion(void)
{
    if (!initialized) {
        return "INITIALIZATION_INCOMPLETE";
    }

    static char versionString[32];
    static bool versionFetched = false;

    if (!versionFetched) {
        ble_version_t version;
        if ((sd_ble_version_get(&version) == NRF_SUCCESS) && (version.company_id == 0x0059)) {
            switch (version.version_number) {
                case 0x07:
                case 0x08:
                    snprintf(versionString, sizeof(versionString), "Nordic BLE4.1 ver:%u fw:%04x", version.version_number, version.subversion_number);
                    break;
                default:
                    snprintf(versionString, sizeof(versionString), "Nordic (spec unknown) ver:%u fw:%04x", version.version_number, version.subversion_number);
                    break;
            }
            versionFetched = true;
        } else {
            strncpy(versionString, "unknown", sizeof(versionString));
        }
    }

    return versionString;
}

/**************************************************************************/
/*!
    @brief  Initialize the BLE stack.

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE if everything executed properly and
                BLE_ERROR_ALREADY_INITIALIZED if the stack has already
                been initialized (possibly through a call to nRF5xn::init()).
                BLE_ERROR_INTERNAL_STACK_FAILURE is returned if initialization
                of the internal stack (SoftDevice) failed.

*/
/**************************************************************************/
ble_error_t nRF5xn::init(BLE::InstanceID_t instanceID, FunctionPointerWithContext<BLE::InitializationCompleteCallbackContext *> callback)
{
    if (initialized) {
        BLE::InitializationCompleteCallbackContext context = {
            BLE::Instance(instanceID),
            BLE_ERROR_ALREADY_INITIALIZED
        };
        callback.call(&context);
        return BLE_ERROR_ALREADY_INITIALIZED;
    }

    this->instanceID = instanceID;

    /* ToDo: Clear memory contents, reset the SD, etc. */
    if (btle_init() != ERROR_NONE) {
        return BLE_ERROR_INTERNAL_STACK_FAILURE;
    }

    initialized = true;
    BLE::InitializationCompleteCallbackContext context = {
        BLE::Instance(instanceID),
        BLE_ERROR_NONE
    };
    callback.call(&context);
    return BLE_ERROR_NONE;
}

/**************************************************************************/
/*!
    @brief  Purge the BLE stack of GATT and GAP state.

    @returns    ble_error_t

    @retval     BLE_ERROR_NONE
                Everything executed properly

    @note  When using S110, GattClient::shutdown() will not be called
           since Gatt client features are not supported.
*/
/**************************************************************************/
ble_error_t nRF5xn::shutdown(void)
{
    if (!initialized) {
        return BLE_ERROR_INITIALIZATION_INCOMPLETE;
    }

    /*
     * Shutdown the SoftDevice first. This is because we need to disable all
     * interrupts. Otherwise if we clear the BLE API and glue code first there
     * will be many NULL references and no config information which could lead
     * to errors if the shutdown process is interrupted.
     */
    #if NRF_SD_BLE_API_VERSION >= 5
    if (nrf_sdh_disable_request() != NRF_SUCCESS) {
        return BLE_STACK_BUSY;
    }
    #else
    if (softdevice_handler_sd_disable() != NRF_SUCCESS) {
        return BLE_STACK_BUSY;
    }
    #endif

    /* Shutdown the BLE API and nRF51 glue code */
    ble_error_t error;

    if (gattServerInstance != NULL) {
        error = gattServerInstance->reset();
        if (error != BLE_ERROR_NONE) {
            return error;
        }
    }

    /* S110 does not support BLE client features, nothing to reset. */
#if !defined(TARGET_MCU_NRF51_16K_S110) && !defined(TARGET_MCU_NRF51_32K_S110)
    error = getGattClient().reset();
    if (error != BLE_ERROR_NONE) {
        return error;
    }
#endif

    /* Gap instance is always present */
    error = gapInstance.reset();
    if (error != BLE_ERROR_NONE) {
        return error;
    }

    custom_reset_128bits_uuid_table();

    initialized = false;
    return BLE_ERROR_NONE;
}

SecurityManager& nRF5xn::getSecurityManager()
{
    const nRF5xn* self = this;
    return const_cast<SecurityManager&>(self->getSecurityManager());
}

const SecurityManager& nRF5xn::getSecurityManager() const
{
    ble::pal::vendor::nordic::nRF5xSecurityManager &m_pal =
        ble::pal::vendor::nordic::nRF5xSecurityManager::get_security_manager();
    static struct : ble::pal::SigningEventMonitor {
        virtual void set_signing_event_handler(EventHandler *signing_event_handler) { }
    } dummy_signing_event_monitor;

    static ble::generic::GenericSecurityManager m_instance(
        m_pal,
        const_cast<nRF5xGap&>(getGap()),
        dummy_signing_event_monitor
    );

    return m_instance;
}

void
nRF5xn::waitForEvent(void)
{
    processEvents();
    sd_app_evt_wait();
}

void nRF5xn::processEvents() {
    core_util_critical_section_enter();
    if (isEventsSignaled) {
        isEventsSignaled = false;
        core_util_critical_section_exit();
#if NRF_SD_BLE_API_VERSION >= 5
        // We use the "polling" dispatch model
        // http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk5.v14.2.0/group__nrf__sdh.html?cp=4_0_0_6_11_60_20#gab4d7be69304d4f5feefd1d440cc3e6c7
        // This will process any pending events from the Softdevice
        nrf_sdh_evts_poll();
#else
        intern_softdevice_events_execute();
#endif
    } else {
        core_util_critical_section_exit();
    }
}
