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

#ifndef __BLE_H__
#define __BLE_H__

#include "blecommon.h"
#include "Gap.h"
#include "GattServer.h"
#include "GattClient.h"

#include "ble/FunctionPointerWithContext.h"

#ifdef YOTTA_CFG_MBED_OS
#include "mbed-drivers/mbed_error.h"
#else
#include "mbed_error.h"
#endif

/* Forward declaration for the implementation class */
class BLEInstanceBase;

/**
 * The base class used to abstract away BLE-capable radio transceivers or SOCs,
 * so that the BLE API can work with any radio transparently.
 */
class BLE
{
public:
    typedef unsigned InstanceID_t; /**< The type returned by BLE::getInstanceID(). */

    /**
     * Parameters provided to the callback registered by onEventsToProcess
     * when there is events to process.
     */
    struct OnEventsToProcessCallbackContext {
        /**
         * The ble instance which have events to process.
         */
        BLE& ble;
    };

    /**
     * Callback type used by the onEventsToProcess function.
     */
    typedef FunctionPointerWithContext<OnEventsToProcessCallbackContext*> OnEventsToProcessCallback_t;

    /**
     * The context provided to init-completion-callbacks (see init() below).
     *
     * @param  ble
     *             A reference to the BLE instance being initialized.
     * @param  error
     *             Captures the result of initialization. It is set to
     *             BLE_ERROR_NONE if initialization completed successfully. Else
     *             the error value is implementation specific.
     */
    struct InitializationCompleteCallbackContext {
        BLE&        ble;   /**< Reference to the BLE object that has been initialized */
        ble_error_t error; /**< Error status of the initialization. It is set to BLE_ERROR_NONE if initialization completed successfully. */
    };

    /**
     * The signature for function-pointer like callbacks for initialization-completion.
     *
     * @note There are two versions of init(). In addition to the simple
     *     function-pointer, init() can also take a <Object, member> tuple as its
     *     callback target. In case of the latter, the following declaration doesn't apply.
     */
    typedef void (*InitializationCompleteCallback_t)(InitializationCompleteCallbackContext *context);

    /**
     * Initialize the BLE controller. This should be called before using
     * anything else in the BLE API.
     *
     * init() hands control to the underlying BLE module to accomplish
     * initialization. This initialization may tacitly depend on other hardware
     * setup (such as clocks or power-modes) that happens early on during
     * system startup. It may not be safe to call init() from a global static
     * context where ordering is compiler-specific and can't be guaranteed - it
     * is safe to call BLE::init() from within main().
     *
     * @param  initCompleteCallback
     *           A callback for when initialization completes for a BLE
     *           instance. This is an optional parameter; if no callback is
     *           set up the application can still determine the status of
     *           initialization using BLE::hasInitialized() (see below).
     *
     * @return  BLE_ERROR_NONE if the initialization procedure was started
     *     successfully.
     *
     * @note If init() returns BLE_ERROR_NONE, the underlying stack must invoke
     *     the initialization completion callback at some point.
     *
     * @note Nearly all BLE APIs would return
     *     BLE_ERROR_INITIALIZATION_INCOMPLETE if used on an instance before the
     *     corresponding transport is initialized.
     *
     * @note There are two versions of init(). In addition to the simple
     *     function-pointer, init() can also take an <Object, member> tuple as its
     *     callback target.
     */
    ble_error_t init(InitializationCompleteCallback_t initCompleteCallback = NULL) {
        FunctionPointerWithContext<InitializationCompleteCallbackContext *> callback(initCompleteCallback);
        return initImplementation(callback);
    }

    /**
     * An alternate declaration for init(). This one takes an <Object, member> tuple as its
     * callback target.
     */
    template<typename T>
    ble_error_t init(T *object, void (T::*initCompleteCallback)(InitializationCompleteCallbackContext *context)) {
        FunctionPointerWithContext<InitializationCompleteCallbackContext *> callback(object, initCompleteCallback);
        return initImplementation(callback);
    }

    /**
     * @return true if initialization has completed for the underlying BLE
     *     transport.
     *
     * The application can set up a callback to signal completion of
     * initialization when using init(). Otherwise, this method can be used to
     * poll the state of initialization.
     */
    bool hasInitialized(void) const;

    /**
     * Purge the BLE stack of GATT and GAP state. init() must be called
     * afterwards to re-instate services and GAP state. This API offers a way to
     * repopulate the GATT database with new services and characteristics.
     */
    ble_error_t shutdown(void);

    /**
     * This call allows the application to get the BLE stack version information.
     *
     * @return  A pointer to a const string representing the version.
     *
     * @note The string returned is owned by BLE API.
     */
    const char *getVersion(void);

    /**
     * Accessor to Gap. All Gap related functionality requires
     * going through this accessor.
     *
     * @return A reference to a Gap object associated to this BLE instance.
     */
    Gap &gap();

    /**
     * A const alternative to gap().
     *
     * @return A const reference to a Gap object associated to this BLE instance.
     */
    const Gap &gap() const;

    /**
     * Accessor to GattServer. All GattServer related functionality requires
     * going through this accessor.
     *
     * @return A reference to a GattServer object associated to this BLE instance.
     */
    GattServer& gattServer();

    /**
     * A const alternative to gattServer().
     *
     * @return A const reference to a GattServer object associated to this BLE instance.
     */
    const GattServer& gattServer() const;

    /**
     * Accessors to GattClient. All GattClient related functionality requires going
     * through this accessor.
     *
     * @return A reference to a GattClient object associated to this BLE instance.
     */
    GattClient& gattClient();

    /**
     * A const alternative to gattClient().
     *
     * @return A const reference to a GattClient object associated to this BLE instance.
     */
    const GattClient& gattClient() const;

    /**
     * Accessors to SecurityManager. All SecurityManager related functionality requires
     * going through this accessor.
     *
     * @return A reference to a SecurityManager object associated to this BLE instance.
     */
    SecurityManager& securityManager();

    /**
     * A const alternative to securityManager().
     *
     * @return A const reference to a SecurityManager object associated to this BLE instance.
     */
    const SecurityManager& securityManager() const;

    /**
     * Yield control to the BLE stack or to other tasks waiting for events. This
     * is a sleep function that will return when there is an application-specific
     * interrupt, but the MCU might wake up several times before
     * returning (to service the stack). This is not always interchangeable with
     * WFE().
     */
    void waitForEvent(void);

public:
    /**
     * The value of the BLE::InstanceID_t for the default BLE instance.
     */
    static const InstanceID_t DEFAULT_INSTANCE = 0;
#ifndef YOTTA_CFG_BLE_INSTANCES_COUNT
    /**
     * The number of permitted BLE instances for the application.
     */
    static const InstanceID_t NUM_INSTANCES = 1;
#else
    /**
     * The number of permitted BLE instances for the application.
     */
    static const InstanceID_t NUM_INSTANCES = YOTTA_CFG_BLE_INSTANCES_COUNT;
#endif

    /**
     * Get a reference to the BLE singleton corresponding to a given interface.
     * There is a static array of BLE singletons.
     *
     * @note Calling Instance() is preferred over constructing a BLE object
     * directly, as it returns references to singletons.
     *
     * @param[in] id
     *              Instance-ID. This should be less than NUM_INSTANCES
     *              for the returned BLE singleton to be useful.
     *
     * @return A reference to a single object.
     */
    static BLE &Instance(InstanceID_t id = DEFAULT_INSTANCE);

    /**
     * Constructor for a handle to a BLE instance (the BLE stack). BLE handles
     * are thin wrappers around a transport object (that is, ptr. to
     * BLEInstanceBase).
     *
     * It is better to create BLE objects as singletons accessed through the
     * Instance() method. If multiple BLE handles are constructed for the same
     * interface (using this constructor), they will share the same underlying
     * transport object.
     */
    BLE(InstanceID_t instanceID = DEFAULT_INSTANCE);

    /**
     * Fetch the ID of a BLE instance. Typically there would only be the DEFAULT_INSTANCE.
     */
    InstanceID_t getInstanceID(void) const {
        return instanceID;
    }

    /*
     * Deprecation alert!
     * All of the following are deprecated and may be dropped in a future
     * release. Documentation should refer to alternative APIs.
     */

    /* GAP specific APIs. */
public:
    /**
     * Set the BTLE MAC address and type.
     * @return BLE_ERROR_NONE on success.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAddress(). A former call to
     *             ble.setAddress(...) should be replaced with
     *             ble.gap().setAddress(...).
     */
    ble_error_t setAddress(BLEProtocol::AddressType_t type, const BLEProtocol::AddressBytes_t address) {
        return gap().setAddress(type, address);
    }

    /**
     * Fetch the Bluetooth Low Energy MAC address and type.
     * @return BLE_ERROR_NONE on success.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getAddress(). A former call to
     *             ble.getAddress(...) should be replaced with
     *             ble.gap().getAddress(...).
     */
    ble_error_t getAddress(BLEProtocol::AddressType_t *typeP, BLEProtocol::AddressBytes_t address) {
        return gap().getAddress(typeP, address);
    }

    /**
     * Set the GAP advertising mode to use for this device.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAdvertisingType(). A former call to
     *             ble.setAdvertisingType(...) should be replaced with
     *             ble.gap().setAdvertisingType(...).
     */
    void setAdvertisingType(GapAdvertisingParams::AdvertisingType advType) {
        gap().setAdvertisingType(advType);
    }

    /**
     * @param[in] interval
     *              Advertising interval in units of milliseconds. Advertising
     *              is disabled if interval is 0. If interval is smaller than
     *              the minimum supported value, then the minimum supported
     *              value is used instead. This minimum value can be discovered
     *              using getMinAdvertisingInterval().
     *
     *              This field must be set to 0 if connectionMode is equal
     *              to ADV_CONNECTABLE_DIRECTED.
     *
     * @note Decreasing this value allows central devices to detect a
     * peripheral faster, at the expense of more power being used by the radio
     * due to the higher data transmit rate.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAdvertisingInterval(). A former call to
     *             ble.setAdvertisingInterval(...) should be replaced with
     *             ble.gap().setAdvertisingInterval(...).
     *
     * @note WARNING: This API previously used 0.625ms as the unit for its
     * 'interval' argument. That required an explicit conversion from
     * milliseconds using Gap::MSEC_TO_GAP_DURATION_UNITS(). This conversion is
     * no longer required as the new units are milliseconds. Any application
     * code depending on the old semantics needs to be updated accordingly.
     */
    void setAdvertisingInterval(uint16_t interval) {
        gap().setAdvertisingInterval(interval);
    }

    /**
     * @return Minimum Advertising interval in milliseconds.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getMinAdvertisingInterval(). A former call to
     *             ble.getMinAdvertisingInterval(...) should be replaced with
     *             ble.gap().getMinAdvertisingInterval(...).
     */
    uint16_t getMinAdvertisingInterval(void) const {
        return gap().getMinAdvertisingInterval();
    }

    /**
     * @return Minimum Advertising interval in milliseconds for non-connectible mode.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::MinNonConnectableAdvertisingInterval(). A former call to
     *             ble.getMinNonConnectableAdvertisingInterval(...) should be replaced with
     *             ble.gap().getMinNonConnectableAdvertisingInterval(...).
     */
    uint16_t getMinNonConnectableAdvertisingInterval(void) const {
        return gap().getMinNonConnectableAdvertisingInterval();
    }

    /**
     * @return Maximum Advertising interval in milliseconds.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getMaxAdvertisingInterval(). A former call to
     *             ble.getMaxAdvertisingInterval(...) should be replaced with
     *             ble.gap().getMaxAdvertisingInterval(...).
     */
    uint16_t getMaxAdvertisingInterval(void) const {
        return gap().getMaxAdvertisingInterval();
    }

    /**
     * @param[in] timeout
     *              Advertising timeout (in seconds) between 0x1 and 0x3FFF (1
     *              and 16383). Use 0 to disable the advertising timeout.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAdvertisingTimeout(). A former call to
     *             ble.setAdvertisingTimeout(...) should be replaced with
     *             ble.gap().setAdvertisingTimeout(...).
     */
    void setAdvertisingTimeout(uint16_t timeout) {
        gap().setAdvertisingTimeout(timeout);
    }

    /**
     * Set up a particular, user-constructed set of advertisement parameters for
     * the underlying stack. It would be uncommon for this API to be used
     * directly; there are other APIs to tweak advertisement parameters
     * individually (see above).
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAdvertisingParams(). A former call to
     *             ble.setAdvertisingParams(...) should be replaced with
     *             ble.gap().setAdvertisingParams(...).
     */
    void setAdvertisingParams(const GapAdvertisingParams &advParams) {
        gap().setAdvertisingParams(advParams);
    }

    /**
     * @return  Read back advertising parameters. Useful for storing and
     *          restoring parameters rapidly.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getAdvertisingParams(). A former call to
     *             ble.getAdvertisingParams(...) should be replaced with
     *             ble.gap().getAdvertisingParams(...).
     */
    const GapAdvertisingParams &getAdvertisingParams(void) const {
        return gap().getAdvertisingParams();
    }

    /**
     * Accumulate an AD structure in the advertising payload. Please note that
     * the payload is limited to 31 bytes. The SCAN_RESPONSE message may be used
     * as an additional 31 bytes if the advertising payload is too
     * small.
     *
     * @param[in] flags
     *              The flags to add. Please refer to
     *              GapAdvertisingData::Flags for valid flags. Multiple
     *              flags may be specified in combination.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::accumulateAdvertisingPayload(uint8_t). A former call to
     *             ble.accumulateAdvertisingPayload(flags) should be replaced with
     *             ble.gap().accumulateAdvertisingPayload(flags).
     */
    ble_error_t accumulateAdvertisingPayload(uint8_t flags) {
        return gap().accumulateAdvertisingPayload(flags);
    }

    /**
     * Accumulate an AD structure in the advertising payload. Please note that
     * the payload is limited to 31 bytes. The SCAN_RESPONSE message may be used
     * as an additional 31 bytes if the advertising payload is too
     * small.
     *
     * @param[in] app
     *              The appearance of the peripheral.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::accumulateAdvertisingPayload(GapAdvertisingData::Appearance).
     *             A former call to ble.accumulateAdvertisingPayload(appearance)
     *             should be replaced with
     *             ble.gap().accumulateAdvertisingPayload(appearance).
     */
    ble_error_t accumulateAdvertisingPayload(GapAdvertisingData::Appearance app) {
        return gap().accumulateAdvertisingPayload(app);
    }

    /**
     * Accumulate an AD structure in the advertising payload. Please note that
     * the payload is limited to 31 bytes. The SCAN_RESPONSE message may be used
     * as an additional 31 bytes if the advertising payload is too
     * small.
     *
     * @param[in] power
     *              The max transmit power to be used by the controller. This
     *              is only a hint.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::accumulateAdvertisingPayloadTxPower(). A former call to
     *             ble.accumulateAdvertisingPayloadTxPower(txPower) should be replaced with
     *             ble.gap().accumulateAdvertisingPayloadTxPower(txPower).
     */
    ble_error_t accumulateAdvertisingPayloadTxPower(int8_t power) {
        return gap().accumulateAdvertisingPayloadTxPower(power);
    }

    /**
     * Accumulate a variable length byte-stream as an AD structure in the
     * advertising payload. Please note that the payload is limited to 31 bytes.
     * The SCAN_RESPONSE message may be used as an additional 31 bytes if the
     * advertising payload is too small.
     *
     * @param  type The type that describes the variable length data.
     * @param  data Data bytes.
     * @param  len  Data length.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::accumulateAdvertisingPayload(GapAdvertisingData::DataType, const uint8_t, uint8_t).
     *             A former call to ble.accumulateAdvertisingPayload(...) should
     *             be replaced with ble.gap().accumulateAdvertisingPayload(...).
     */
    ble_error_t accumulateAdvertisingPayload(GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len) {
        return gap().accumulateAdvertisingPayload(type, data, len);
    }

    /**
     * Setup a particular, user-constructed advertisement payload for the
     * underlying stack. It would be uncommon for this API to be used directly;
     * there are other APIs to build an advertisement payload (see above).
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAdvertisingData(). A former call to
     *             ble.setAdvertisingData(...) should be replaced with
     *             ble.gap().setAdvertisingPayload(...).
     */
    ble_error_t setAdvertisingData(const GapAdvertisingData &advData) {
        return gap().setAdvertisingPayload(advData);
    }

    /**
     * @return  Read back advertising data. Useful for storing and
     *          restoring payload.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getAdvertisingData(). A former call to
     *             ble.getAdvertisingData(...) should be replaced with
     *             ble.gap().getAdvertisingPayload()(...).
     */
    const GapAdvertisingData &getAdvertisingData(void) const {
        return gap().getAdvertisingPayload();
    }

    /**
     * Reset any advertising payload prepared from prior calls to
     * accumulateAdvertisingPayload(). This automatically propagates the re-
     * initialized advertising payload to the underlying stack.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::clearAdvertisingPayload(). A former call to
     *             ble.clearAdvertisingPayload(...) should be replaced with
     *             ble.gap().clearAdvertisingPayload(...).
     */
    void clearAdvertisingPayload(void) {
        gap().clearAdvertisingPayload();
    }

    /**
     * Dynamically reset the accumulated advertising
     * payload and scanResponse. The application must clear and re-
     * accumulates a new advertising payload (and scanResponse) before using this
     * API.
     *
     * @return BLE_ERROR_NONE when the advertising payload is set successfully.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAdvertisingPayload().
     *
     * @note The new APIs in Gap update the underlying advertisement payload
     * implicitly.
     */
    ble_error_t setAdvertisingPayload(void) {
        return BLE_ERROR_NONE;
    }

    /**
     * Accumulate a variable length byte-stream as an AD structure in the
     * scanResponse payload.
     *
     * @param[in] type The type that describes the variable length data.
     * @param[in] data Data bytes.
     * @param[in] len  Data length.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::accumulateScanResponse(). A former call to
     *             ble.accumulateScanResponse(...) should be replaced with
     *             ble.gap().accumulateScanResponse(...).
     */
    ble_error_t accumulateScanResponse(GapAdvertisingData::DataType type, const uint8_t *data, uint8_t len) {
        return gap().accumulateScanResponse(type, data, len);
    }

    /**
     * Reset any scan response prepared from prior calls to
     * accumulateScanResponse().
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::clearScanResponse(). A former call to
     *             ble.clearScanResponse(...) should be replaced with
     *             ble.gap().clearScanResponse(...).
     */
    void clearScanResponse(void) {
        gap().clearScanResponse();
    }

    /**
     * Start advertising.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::startAdvertising(). A former call to
     *             ble.startAdvertising(...) should be replaced with
     *             ble.gap().startAdvertising(...).
     */
    ble_error_t startAdvertising(void) {
        return gap().startAdvertising();
    }

    /**
     * Stop advertising.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::stopAdvertising(). A former call to
     *             ble.stopAdvertising(...) should be replaced with
     *             ble.gap().stopAdvertising(...).
     */
    ble_error_t stopAdvertising(void) {
        return gap().stopAdvertising();
    }

    /**
     * Set up parameters for GAP scanning (observer mode).
     * @param[in] interval
     *              Scan interval (in milliseconds) [valid values lie between 2.5ms and 10.24s].
     * @param[in] window
     *              Scan Window (in milliseconds) [valid values lie between 2.5ms and 10.24s].
     * @param[in] timeout
     *              Scan timeout (in seconds) between 0x0001 and 0xFFFF; 0x0000 disables timeout.
     * @param[in] activeScanning
     *              Set to True if active-scanning is required. This is used to fetch the
     *              scan response from a peer if possible.
     *
     * The scanning window divided by the interval determines the duty cycle for
     * scanning. For example, if the interval is 100ms and the window is 10ms,
     * then the controller will scan for 10 percent of the time. It is possible
     * to have the interval and window set to the same value. In this case,
     * scanning is continuous, with a change of scanning frequency once every
     * interval.
     *
     * Once the scanning parameters have been configured, scanning can be
     * enabled by using startScan().
     *
     * @note The scan interval and window are recommendations to the BLE stack.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setScanParams(). A former call to
     *             ble.setScanParams(...) should be replaced with
     *             ble.gap().setScanParams(...).
     */
    ble_error_t setScanParams(uint16_t interval       = GapScanningParams::SCAN_INTERVAL_MAX,
                              uint16_t window         = GapScanningParams::SCAN_WINDOW_MAX,
                              uint16_t timeout        = 0,
                              bool     activeScanning = false) {
        return gap().setScanParams(interval, window, timeout, activeScanning);
    }

    /**
     * Set up the scanInterval parameter for GAP scanning (observer mode).
     * @param[in] interval
     *              Scan interval (in milliseconds) [valid values lie between 2.5ms and 10.24s].
     *
     * The scanning window divided by the interval determines the duty cycle for
     * scanning. For example, if the interval is 100ms and the window is 10ms,
     * then the controller will scan for 10 percent of the time. It is possible
     * to have the interval and window set to the same value. In this case,
     * scanning is continuous, with a change of scanning frequency once every
     * interval.
     *
     * Once the scanning parameters have been configured, scanning can be
     * enabled by using startScan().
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setScanInterval(). A former call to
     *             ble.setScanInterval(interval) should be replaced with
     *             ble.gap().setScanInterval(interval).
     */
    ble_error_t setScanInterval(uint16_t interval) {
        return gap().setScanInterval(interval);
    }

    /**
     * Set up the scanWindow parameter for GAP scanning (observer mode).
     * @param[in] window
     *              Scan Window (in milliseconds) [valid values lie between 2.5ms and 10.24s].
     *
     * The scanning window divided by the interval determines the duty cycle for
     * scanning. For example, if the interval is 100ms and the window is 10ms,
     * then the controller will scan for 10 percent of the time. It is possible
     * to have the interval and window set to the same value. In this case,
     * scanning is continuous, with a change of scanning frequency once every
     * interval.
     *
     * Once the scanning parameters have been configured, scanning can be
     * enabled by using startScan().
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setScanWindow(). A former call to
     *             ble.setScanWindow(window) should be replaced with
     *             ble.gap().setScanWindow(window).
     */
    ble_error_t setScanWindow(uint16_t window) {
        return gap().setScanWindow(window);
    }

    /**
     * Set up parameters for GAP scanning (observer mode).
     * @param[in] timeout
     *              Scan timeout (in seconds) between 0x0001 and 0xFFFF; 0x0000 disables timeout.
     *
     * The scanning window divided by the interval determines the duty cycle for
     * scanning. For example, if the interval is 100ms and the window is 10ms,
     * then the controller will scan for 10 percent of the time. It is possible
     * to have the interval and window set to the same value. In this case,
     * scanning is continuous, with a change of scanning frequency once every
     * interval.
     *
     * Once the scanning parameters have been configured, scanning can be
     * enabled by using startScan().
     *
     * @note The scan interval and window are recommendations to the BLE stack.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setScanTimeout(). A former call to
     *             ble.setScanTimeout(...) should be replaced with
     *             ble.gap().setScanTimeout(...).
     */
    ble_error_t setScanTimeout(uint16_t timeout) {
        return gap().setScanTimeout(timeout);
    }

    /**
     * Set up parameters for GAP scanning (observer mode).
     * @param[in] activeScanning
     *              Set to True if active-scanning is required. This is used to fetch the
     *              scan response from a peer if possible.
     *
     * Once the scanning parameters have been configured, scanning can be
     * enabled by using startScan().
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setActiveScan(). A former call to
     *             ble.setActiveScan(...) should be replaced with
     *             ble.gap().setActiveScanning(...).
     */
    void setActiveScan(bool activeScanning) {
        gap().setActiveScanning(activeScanning);
    }

    /**
     * Start scanning (Observer Procedure) based on the parameters currently in
     * effect.
     *
     * @param[in] callback
     *              The application-specific callback to be invoked upon
     *              receiving every advertisement report. This can be passed in
     *              as NULL, in which case scanning may not be enabled at all.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::startScan(). A former call to
     *             ble.startScan(callback) should be replaced with
     *             ble.gap().startScan(callback).
     */
    ble_error_t startScan(void (*callback)(const Gap::AdvertisementCallbackParams_t *params)) {
        return gap().startScan(callback);
    }

    /**
     * Same as above, but this takes an (object, method) pair for a callback.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::startScan(). A former call to
     *             ble.startScan(callback) should be replaced with
     *             ble.gap().startScan(object, callback).
     */
    template<typename T>
    ble_error_t startScan(T *object, void (T::*memberCallback)(const Gap::AdvertisementCallbackParams_t *params));

    /**
     * Stop scanning. The current scanning parameters remain in effect.
     *
     * @retval BLE_ERROR_NONE if successfully stopped scanning procedure.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::stopScan(). A former call to
     *             ble.stopScan() should be replaced with
     *             ble.gap().stopScan().
     */
    ble_error_t stopScan(void) {
        return gap().stopScan();
    }

    /**
     * Create a connection (GAP Link Establishment).
     * @param peerAddr
     *          48-bit address, LSB format.
     * @param peerAddrType
     *          Address type of the peer.
     * @param connectionParams
     *         Connection parameters.
     * @param scanParams
     *          Parameters to use while scanning for the peer.
     * @return  BLE_ERROR_NONE if connection establishment procedure is started
     *     successfully. The onConnection callback (if set) is invoked upon
     *     a connection event.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::connect(). A former call to
     *             ble.connect(...) should be replaced with
     *             ble.gap().connect(...).
     */
    ble_error_t connect(const BLEProtocol::AddressBytes_t  peerAddr,
                        BLEProtocol::AddressType_t         peerAddrType = BLEProtocol::AddressType::RANDOM_STATIC,
                        const Gap::ConnectionParams_t     *connectionParams = NULL,
                        const GapScanningParams           *scanParams = NULL) {
        return gap().connect(peerAddr, peerAddrType, connectionParams, scanParams);
    }

    /**
     * This call initiates the disconnection procedure, and its completion is
     * communicated to the application with an invocation of the
     * onDisconnection callback.
     *
     * @param[in] connectionHandle
     * @param[in] reason
     *              The reason for disconnection; sent back to the peer.
     */
    ble_error_t disconnect(Gap::Handle_t connectionHandle, Gap::DisconnectionReason_t reason) {
        return gap().disconnect(connectionHandle, reason);
    }

    /**
     * This call initiates the disconnection procedure, and its completion
     * is communicated to the application with an invocation of the
     * onDisconnection callback.
     *
     * @param  reason
     *           The reason for disconnection; sent back to the peer.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::disconnect(). A former call to
     *             ble.disconnect(reason) should be replaced with
     *             ble.gap().disconnect(reason).
     *
     * @note This version of disconnect() doesn't take a connection handle. It
     * works reliably only for stacks that are limited to a single
     * connection.
     */
    ble_error_t disconnect(Gap::DisconnectionReason_t reason) {
        return gap().disconnect(reason);
    }

    /**
     * Returns the current Gap state of the device using a bitmask that
     * describes whether the device is advertising or connected.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getState(). A former call to
     *             ble.getGapState() should be replaced with
     *             ble.gap().getState().
     */
    Gap::GapState_t getGapState(void) const {
        return gap().getState();
    }

    /**
     * Get the GAP peripheral's preferred connection parameters. These are the
     * defaults that the peripheral would like to have in a connection. The
     * choice of the connection parameters is eventually up to the central.
     *
     * @param[out] params
     *               The structure where the parameters will be stored. Memory
     *               for this is owned by the caller.
     *
     * @return BLE_ERROR_NONE if the parameters were successfully filled into
     * the given structure pointed to by params.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getPreferredConnectionParams(). A former call to
     *             ble.getPreferredConnectionParams() should be replaced with
     *             ble.gap().getPreferredConnectionParams().
     */
    ble_error_t getPreferredConnectionParams(Gap::ConnectionParams_t *params) {
        return gap().getPreferredConnectionParams(params);
    }

    /**
     * Set the GAP peripheral's preferred connection parameters. These are the
     * defaults that the peripheral would like to have in a connection. The
     * choice of the connection parameters is eventually up to the central.
     *
     * @param[in] params
     *               The structure containing the desired parameters.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setPreferredConnectionParams(). A former call to
     *             ble.setPreferredConnectionParams() should be replaced with
     *             ble.gap().setPreferredConnectionParams().
     */
    ble_error_t setPreferredConnectionParams(const Gap::ConnectionParams_t *params) {
        return gap().setPreferredConnectionParams(params);
    }

    /**
     * Update connection parameters while in the peripheral role.
     * @details In the peripheral role, this will send the corresponding L2CAP request to the connected peer and wait for
     *          the central to perform the procedure.
     * @param[in] handle
     *              Connection Handle
     * @param[in] params
     *              Pointer to desired connection parameters. If NULL is provided on a peripheral role,
     *              the parameters in the PPCP characteristic of the GAP service will be used instead.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::updateConnectionParams(). A former call to
     *             ble.updateConnectionParams() should be replaced with
     *             ble.gap().updateConnectionParams().
     */
    ble_error_t updateConnectionParams(Gap::Handle_t handle, const Gap::ConnectionParams_t *params) {
        return gap().updateConnectionParams(handle, params);
    }

    /**
     * Set the device name characteristic in the Gap service.
     * @param[in] deviceName
     *              The new value for the device-name. This is a UTF-8 encoded, <b>NULL-terminated</b> string.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setDeviceName(). A former call to
     *             ble.setDeviceName() should be replaced with
     *             ble.gap().setDeviceName().
     */
    ble_error_t setDeviceName(const uint8_t *deviceName) {
        return gap().setDeviceName(deviceName);
    }

    /**
     * Get the value of the device name characteristic in the Gap service.
     * @param[out]    deviceName
     *                  Pointer to an empty buffer where the UTF-8 *non NULL-
     *                  terminated* string will be placed. Set this
     *                  value to NULL in order to obtain the deviceName-length
     *                  from the 'length' parameter.
     *
     * @param[in,out] lengthP
     *                  (on input) Length of the buffer pointed to by deviceName;
     *                  (on output) the complete device name length (without the
     *                     null terminator).
     *
     * @note If the device name is longer than the size of the supplied buffer,
     *     length will return the complete device name length, and not the
     *     number of bytes actually returned in deviceName. The application may
     *     use this information to retry with a suitable buffer size.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getDeviceName(). A former call to
     *             ble.getDeviceName() should be replaced with
     *             ble.gap().getDeviceName().
     */
    ble_error_t getDeviceName(uint8_t *deviceName, unsigned *lengthP) {
        return gap().getDeviceName(deviceName, lengthP);
    }

    /**
     * Set the appearance characteristic in the Gap service.
     * @param[in] appearance
     *              The new value for the device-appearance.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setAppearance(). A former call to
     *             ble.setAppearance() should be replaced with
     *             ble.gap().setAppearance().
     */
    ble_error_t setAppearance(GapAdvertisingData::Appearance appearance) {
        return gap().setAppearance(appearance);
    }

    /**
     * Get the appearance characteristic in the Gap service.
     * @param[out] appearanceP
     *               The new value for the device-appearance.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getAppearance(). A former call to
     *             ble.getAppearance() should be replaced with
     *             ble.gap().getAppearance().
     */
    ble_error_t getAppearance(GapAdvertisingData::Appearance *appearanceP) {
        return gap().getAppearance(appearanceP);
    }

    /**
     * Set the radio's transmit power.
     * @param[in] txPower Radio transmit power in dBm.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::setTxPower(). A former call to
     *             ble.setTxPower() should be replaced with
     *             ble.gap().setTxPower().
     */
    ble_error_t setTxPower(int8_t txPower) {
        return gap().setTxPower(txPower);
    }

    /**
     * Query the underlying stack for permitted arguments for setTxPower().
     *
     * @param[out] valueArrayPP
     *                 Out parameter to receive the immutable array of Tx values.
     * @param[out] countP
     *                 Out parameter to receive the array's size.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::getPermittedTxPowerValues(). A former call to
     *             ble.getPermittedTxPowerValues() should be replaced with
     *             ble.gap().getPermittedTxPowerValues().
     */
    void getPermittedTxPowerValues(const int8_t **valueArrayPP, size_t *countP) {
        gap().getPermittedTxPowerValues(valueArrayPP, countP);
    }

    /**
     * Add a service declaration to the local server ATT table. Also add the
     * characteristics contained within.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::addService(). A former call
     *             to ble.addService() should be replaced with
     *             ble.gattServer().addService().
     */
    ble_error_t addService(GattService &service) {
        return gattServer().addService(service);
    }

    /**
     * Read the value of a characteristic from the local GattServer.
     * @param[in]     attributeHandle
     *                  Attribute handle for the value attribute of the characteristic.
     * @param[out]    buffer
     *                  A buffer to hold the value being read.
     * @param[in,out] lengthP
     *                  Length of the buffer being supplied. If the attribute
     *                  value is longer than the size of the supplied buffer,
     *                  this variable will return the total attribute value length
     *                  (excluding offset). The application may use this
     *                  information to allocate a suitable buffer size.
     *
     * @return BLE_ERROR_NONE if a value was read successfully into the buffer.
     *
     * @deprecated You should use the parallel API from GattServer directly,
     *             GattServer::read(GattAttribute::Handle_t,uint8_t,uint16_t). A former call
     *             to ble.readCharacteristicValue() should be replaced with
     *             ble.gattServer().read().
     */
    ble_error_t readCharacteristicValue(GattAttribute::Handle_t attributeHandle, uint8_t *buffer, uint16_t *lengthP) {
        return gattServer().read(attributeHandle, buffer, lengthP);
    }

    /**
     * Read the value of a characteristic from the local GattServer.
     * @param[in]     connectionHandle
     *                  Connection Handle.
     * @param[in]     attributeHandle
     *                  Attribute handle for the value attribute of the characteristic.
     * @param[out]    buffer
     *                  A buffer to hold the value being read.
     * @param[in,out] lengthP
     *                  Length of the buffer being supplied. If the attribute
     *                  value is longer than the size of the supplied buffer,
     *                  this variable will return the total attribute value length
     *                  (excluding offset). The application may use this
     *                  information to allocate a suitable buffer size.
     *
     * @return BLE_ERROR_NONE if a value was read successfully into the buffer.
     *
     * @note This API is a version of the above, with an additional connection handle
     *     parameter to allow fetches for connection-specific multivalued
     *     attributes (such as the CCCDs).
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::read(Gap::Handle_t,GattAttribute::Handle_t,uint8_t,uint16_t).
     *             A former call to ble.readCharacteristicValue() should be replaced with
     *             ble.gattServer().read().
     */
    ble_error_t readCharacteristicValue(Gap::Handle_t connectionHandle, GattAttribute::Handle_t attributeHandle, uint8_t *buffer, uint16_t *lengthP) {
        return gattServer().read(connectionHandle, attributeHandle, buffer, lengthP);
    }

    /**
     * Update the value of a characteristic on the local GattServer.
     *
     * @param[in] attributeHandle
     *              Handle for the value attribute of the characteristic.
     * @param[in] value
     *              A pointer to a buffer holding the new value.
     * @param[in] size
     *              Size of the new value (in bytes).
     * @param[in] localOnly
     *              Should this update be kept on the local
     *              GattServer regardless of the state of the
     *              notify/indicate flag in the CCCD for this
     *              characteristic? If set to true, no notification
     *              or indication is generated.
     *
     * @return BLE_ERROR_NONE if we have successfully set the value of the attribute.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::write(GattAttribute::Handle_t,const uint8_t,uint16_t,bool).
     *             A former call to ble.updateCharacteristicValue() should be replaced with
     *             ble.gattServer().write().
     */
    ble_error_t updateCharacteristicValue(GattAttribute::Handle_t  attributeHandle,
                                          const uint8_t           *value,
                                          uint16_t                 size,
                                          bool                     localOnly = false) {
        return gattServer().write(attributeHandle, value, size, localOnly);
    }

    /**
     * Update the value of a characteristic on the local GattServer. A version
     * of the above, with a connection handle parameter to allow updates
     * for connection-specific multivalued attributes (such as the CCCDs).
     *
     * @param[in] connectionHandle
     *              Connection Handle.
     * @param[in] attributeHandle
     *              Handle for the value attribute of the Characteristic.
     * @param[in] value
     *              A pointer to a buffer holding the new value.
     * @param[in] size
     *              Size of the new value (in bytes).
     * @param[in] localOnly
     *              Should this update be kept on the local
     *              GattServer regardless of the state of the
     *              notify/indicate flag in the CCCD for this
     *              Characteristic? If set to true, no notification
     *              or indication is generated.
     *
     * @return BLE_ERROR_NONE if we have successfully set the value of the attribute.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::write(Gap::Handle_t,GattAttribute::Handle_t,const uint8_t,uint16_t,bool).
     *             A former call to ble.updateCharacteristicValue() should be replaced with
     *             ble.gattServer().write().
     */
    ble_error_t updateCharacteristicValue(Gap::Handle_t            connectionHandle,
                                          GattAttribute::Handle_t  attributeHandle,
                                          const uint8_t           *value,
                                          uint16_t                 size,
                                          bool                     localOnly = false) {
        return gattServer().write(connectionHandle, attributeHandle, value, size, localOnly);
    }

    /**
     * Enable the BLE stack's Security Manager. The Security Manager implements
     * the cryptographic algorithms and protocol exchanges that allow two
     * devices to securely exchange data and privately detect each other.
     * Calling this API is a prerequisite for encryption and pairing (bonding).
     *
     * @param[in]  enableBonding Allow for bonding.
     * @param[in]  requireMITM   Require protection against man-in-the-middle attacks.
     * @param[in]  iocaps        To specify the I/O capabilities of this peripheral,
     *                           such as availability of a display or keyboard, to
     *                           support out-of-band exchanges of security data.
     * @param[in]  passkey       To specify a static passkey.
     *
     * @return BLE_ERROR_NONE on success.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager.init(). A former
     *             call to ble.initializeSecurity(...) should be replaced with
     *             ble.securityManager().init(...).
     */
    ble_error_t initializeSecurity(bool                                      enableBonding = true,
                                   bool                                      requireMITM   = true,
                                   SecurityManager::SecurityIOCapabilities_t iocaps        = SecurityManager::IO_CAPS_NONE,
                                   const SecurityManager::Passkey_t          passkey       = NULL) {
        return securityManager().init(enableBonding, requireMITM, iocaps, passkey);
    }

    /**
     * Get the security status of a connection.
     *
     * @param[in]  connectionHandle   Handle to identify the connection.
     * @param[out] securityStatusP    Security status.
     *
     * @return BLE_SUCCESS or appropriate error code indicating the reason of failure.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager::getLinkSecurity(). A former
     *             call to ble.getLinkSecurity(...) should be replaced with
     *             ble.securityManager().getLinkSecurity(...).
     */
    ble_error_t getLinkSecurity(Gap::Handle_t connectionHandle, SecurityManager::LinkSecurityStatus_t *securityStatusP) {
        return securityManager().getLinkSecurity(connectionHandle, securityStatusP);
    }

    /**
     * Delete all peer device context and all related bonding information from
     * the database within the security manager.
     *
     * @retval BLE_ERROR_NONE             On success; else returns an error code indicating the reason for the failure.
     * @retval BLE_ERROR_INVALID_STATE    If the API is called without module initialization or
     *                                    application registration.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager::purgeAllBondingState(). A former
     *             call to ble.purgeAllBondingState() should be replaced with
     *             ble.securityManager().purgeAllBondingState().
     */
    ble_error_t purgeAllBondingState(void) {
        return securityManager().purgeAllBondingState();
    }

    /**
     * Set up a callback for timeout events. Refer to Gap::TimeoutSource_t for
     * possible event types.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::onTimeout(). A former call
     *             to ble.onTimeout(callback) should be replaced with
     *             ble.gap().onTimeout(callback).
     */
    void onTimeout(Gap::TimeoutEventCallback_t timeoutCallback) {
        gap().onTimeout(timeoutCallback);
    }

    /**
     * Set up a callback for connection events. Refer to Gap::ConnectionEventCallback_t.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::onConnection(). A former call
     *             to ble.onConnection(callback) should be replaced with
     *             ble.gap().onConnection(callback).
     */
    void onConnection(Gap::ConnectionEventCallback_t connectionCallback) {
        gap().onConnection(connectionCallback);
    }

    /**
     * Append to a chain of callbacks to be invoked upon GAP disconnection.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::onDisconnection(). A former call
     *             to ble.onDisconnection(callback) should be replaced with
     *             ble.gap().onDisconnection(callback).
     */
    void onDisconnection(Gap::DisconnectionEventCallback_t disconnectionCallback) {
        gap().onDisconnection(disconnectionCallback);
    }

    /**
     * The same as onDisconnection(), but allows an object reference and member function
     * to be added to the chain of callbacks.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::onDisconnection(). A former call
     *             to ble.onDisconnection(callback) should be replaced with
     *             ble.gap().onDisconnection(callback).
     */
    template<typename T>
    void onDisconnection(T *tptr, void (T::*mptr)(const Gap::DisconnectionCallbackParams_t*)) {
        gap().onDisconnection(tptr, mptr);
    }

    /**
     * Radio Notification is a feature that enables ACTIVE and INACTIVE
     * (nACTIVE) signals from the stack. These notify the application when the
     * radio is in use. The signal is sent using software interrupt.
     *
     * The ACTIVE signal is sent before the radio event starts. The nACTIVE
     * signal is sent at the end of the radio event. These signals can be used
     * by the application programmer to synchronize application logic with radio
     * activity. For example, the ACTIVE signal can be used to shut off external
     * devices to manage peak current drawn during periods when the radio is on,
     * or to trigger sensor data collection for transmission in the radio event.
     *
     * @param callback
     *          The application handler to be invoked in response to a radio
     *          ACTIVE/INACTIVE event.
     *
     * @deprecated You should use the parallel API from Gap directly, refer to
     *             Gap::onRadioNotification(). A former call
     *             to ble.onRadioNotification(...) should be replaced with
     *             ble.gap().onRadioNotification(...).
     */
    void onRadioNotification(void (*callback)(bool)) {
        gap().onRadioNotification(callback);
    }

    /**
     * Add a callback for the GATT event DATA_SENT (which is triggered when
     * updates are sent out by GATT in the form of notifications).
     *
     * @note It is possible to chain together multiple onDataSent callbacks
     * (potentially from different modules of an application) to receive updates
     * to characteristics.
     *
     * @note It is also possible to set up a callback into a member function of
     * some object.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onDataSent(). A former call
     *             to ble.onDataSent(...) should be replaced with
     *             ble.gattServer().onDataSent(...).
     */
    void onDataSent(void (*callback)(unsigned count)) {
        gattServer().onDataSent(callback);
    }

    /**
     * The same as onDataSent(), but allows an object reference and member function
     * to be added to the chain of callbacks.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onDataSent(). A former call
     *             to ble.onDataSent(...) should be replaced with
     *             ble.gattServer().onDataSent(...).
     */
    template <typename T> void onDataSent(T * objPtr, void (T::*memberPtr)(unsigned count)) {
        gattServer().onDataSent(objPtr, memberPtr);
    }

    /**
     * Set up a callback for when an attribute has its value updated by or at the
     * connected peer. For a peripheral, this callback is triggered when the local
     * GATT server has an attribute updated by a write command from the peer.
     * For a Central, this callback is triggered when a response is received for
     * a write request.
     *
     * @note It is possible to chain together multiple onDataWritten callbacks
     * (potentially from different modules of an application) to receive updates
     * to characteristics. Many services, such as DFU and UART, add their own
     * onDataWritten callbacks behind the scenes to trap interesting events.
     *
     * @note It is also possible to set up a callback into a member function of
     * some object.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onDataWritten(). A former call
     *             to ble.onDataWritten(...) should be replaced with
     *             ble.gattServer().onDataWritten(...).
     */
    void onDataWritten(void (*callback)(const GattWriteCallbackParams *eventDataP)) {
        gattServer().onDataWritten(callback);
    }

    /**
     * The same as onDataWritten(), but allows an object reference and member function
     * to be added to the chain of callbacks.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onDataWritten(). A former call
     *             to ble.onDataWritten(...) should be replaced with
     *             ble.gattServer().onDataWritten(...).
     */
    template <typename T> void onDataWritten(T * objPtr, void (T::*memberPtr)(const GattWriteCallbackParams *context)) {
        gattServer().onDataWritten(objPtr, memberPtr);
    }

    /**
     * Set up a callback to be invoked on the peripheral when an attribute is
     * being read by a remote client.
     *
     * @note This functionality may not be available on all underlying stacks.
     * You could use GattCharacteristic::setReadAuthorizationCallback() as an
     * alternative.
     *
     * @note It is possible to chain together multiple onDataRead callbacks
     * (potentially from different modules of an application) to receive updates
     * to characteristics. Services may add their own onDataRead callbacks
     * behind the scenes to trap interesting events.
     *
     * @note It is also possible to set up a callback into a member function of
     * some object.
     *
     * @return BLE_ERROR_NOT_IMPLEMENTED if this functionality isn't available;
     *         else BLE_ERROR_NONE.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onDataRead(). A former call
     *             to ble.onDataRead(...) should be replaced with
     *             ble.gattServer().onDataRead(...).
     */
    ble_error_t onDataRead(void (*callback)(const GattReadCallbackParams *eventDataP)) {
        return gattServer().onDataRead(callback);
    }

    /**
     * The same as onDataRead(), but allows an object reference and member function
     * to be added to the chain of callbacks.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onDataRead(). A former call
     *             to ble.onDataRead(...) should be replaced with
     *             ble.gattServer().onDataRead(...).
     */
    template <typename T> ble_error_t onDataRead(T * objPtr, void (T::*memberPtr)(const GattReadCallbackParams *context)) {
        return gattServer().onDataRead(objPtr, memberPtr);
    }

    /**
     * Set up a callback for when notifications or indications are enabled for a
     * characteristic on the local GattServer.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onUpdatesEnabled(). A former call
     *             to ble.onUpdatesEnabled(callback) should be replaced with
     *             ble.gattServer().onUpdatesEnabled(callback).
     */
    void onUpdatesEnabled(GattServer::EventCallback_t callback) {
        gattServer().onUpdatesEnabled(callback);
    }

    /**
     * Set up a callback for when notifications or indications are disabled for a
     * characteristic on the local GattServer.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onUpdatesDisabled(). A former call
     *             to ble.onUpdatesEnabled(callback) should be replaced with
     *             ble.gattServer().onUpdatesEnabled(callback).
     */
    void onUpdatesDisabled(GattServer::EventCallback_t callback) {
        gattServer().onUpdatesDisabled(callback);
    }

    /**
     * Set up a callback for when the GATT server receives a response for an
     * indication event sent previously.
     *
     * @deprecated You should use the parallel API from GattServer directly, refer to
     *             GattServer::onConfirmationReceived(). A former call
     *             to ble.onConfirmationReceived(callback) should be replaced with
     *             ble.gattServer().onConfirmationReceived(callback).
     */
    void onConfirmationReceived(GattServer::EventCallback_t callback) {
        gattServer().onConfirmationReceived(callback);
    }

    /**
     * Set up a callback for when the security setup procedure (key generation
     * and exchange) for a link has started. This will be skipped for bonded
     * devices. The callback is passed in parameters received from the peer's
     * security request: bool allowBonding, bool requireMITM, and
     * SecurityIOCapabilities_t.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager::onSecuritySetupInitiated(). A former
     *             call to ble.onSecuritySetupInitiated(callback) should be replaced with
     *             ble.securityManager().onSecuritySetupInitiated(callback).
     */
    void onSecuritySetupInitiated(SecurityManager::SecuritySetupInitiatedCallback_t callback) {
        securityManager().onSecuritySetupInitiated(callback);
    }

    /**
     * Set up a callback for when the security setup procedure (key generation
     * and exchange) for a link has completed. This will be skipped for bonded
     * devices. The callback is passed in the success/failure status of the
     * security setup procedure.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager::onSecuritySetupCompleted(). A former
     *             call to ble.onSecuritySetupCompleted(callback) should be replaced with
     *             ble.securityManager().onSecuritySetupCompleted(callback).
     */
    void onSecuritySetupCompleted(SecurityManager::SecuritySetupCompletedCallback_t callback) {
        securityManager().onSecuritySetupCompleted(callback);
    }

    /**
     * Set up a callback for when a link with the peer is secured. For bonded
     * devices, subsequent reconnections with a bonded peer will result only in
     * this callback when the link is secured, and setup procedures will not
     * occur unless the bonding information is either lost or deleted on either
     * or both sides. The callback is passed in a SecurityManager::SecurityMode_t according
     * to the level of security in effect for the secured link.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager::onLinkSecured(). A former
     *             call to ble.onLinkSecured(callback) should be replaced with
     *             ble.securityManager().onLinkSecured(callback).
     */
    void onLinkSecured(SecurityManager::LinkSecuredCallback_t callback) {
        securityManager().onLinkSecured(callback);
    }

    /**
     * Set up a callback for successful bonding, meaning that link-specific security
     * context is stored persistently for a peer device.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager::onSecurityContextStored(). A former
     *             call to ble.onSecurityContextStored(callback) should be replaced with
     *             ble.securityManager().onSecurityContextStored(callback).
     */
    void onSecurityContextStored(SecurityManager::HandleSpecificEvent_t callback) {
        securityManager().onSecurityContextStored(callback);
    }

    /**
     * Set up a callback for when the passkey needs to be displayed on a
     * peripheral with DISPLAY capability. This happens when security is
     * configured to prevent Man-In-The-Middle attacks, and the peers need to exchange
     * a passkey (or PIN) to authenticate the connection
     * attempt.
     *
     * @deprecated You should use the parallel API from SecurityManager directly, refer to
     *             SecurityManager::onPasskeyDisplay(). A former
     *             call to ble.onPasskeyDisplay(callback) should be replaced with
     *             ble.securityManager().onPasskeyDisplay(callback).
     */
    void onPasskeyDisplay(SecurityManager::PasskeyDisplayCallback_t callback) {
        return securityManager().onPasskeyDisplay(callback);
    }

    /**
     * Process ALL pending events living in the BLE stack .
     * Return once all events have been consumed.
     * This function is called by user in their while loop (mbed Classic)
     * or automatically by Minar (mbed OS) when BLE event processing is scheduled.
     * Internally, this function will call BLEInstanceBase::processEvent.
     */
    void processEvents();

    /**
     * Register a hook which will be called every time the BLE stack has pending
     * work.
     * By registering a callback, user code can know when event processing has to be
     * scheduled.
     * Callback format is void (*)(BLE& ble);
     */
    void onEventsToProcess(const OnEventsToProcessCallback_t& callback);

private:

    friend class BLEInstanceBase;

    /**
     * This function allow the BLE stack to signal that their is work to do and
     * event processing should be done (BLE::processEvent()).
     * This function should be called by the port of BLE_API, it shouldn't be
     * accessible to end users.
     */
    void signalEventsToProcess();

    /**
     * Implementation of init() [internal to BLE_API].
     *
     * The implementation is separated into a private method because it isn't
     * suitable to be included in the header.
     */
    ble_error_t initImplementation(FunctionPointerWithContext<InitializationCompleteCallbackContext *> callback);

private:
    BLE(const BLE&);
    BLE &operator=(const BLE &);

private:
    InstanceID_t     instanceID;
    BLEInstanceBase *transport; /* The device-specific backend */
    OnEventsToProcessCallback_t whenEventsToProcess;
};

typedef BLE BLEDevice; /**< @deprecated This type alias is retained for the
                        * sake of compatibility with older
                        * code. Will be dropped at some point soon.*/

#endif /* ifndef __BLE_H__ */
