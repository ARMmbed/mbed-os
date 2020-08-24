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

#ifndef MBED_BLE_H__
#define MBED_BLE_H__

#include "platform/mbed_error.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_toolchain.h"

#include "ble/Gap.h"
#include "ble/GattClient.h"
#include "ble/GattServer.h"
#include "ble/SecurityManager.h"

#include "ble/common/BLERoles.h"
#include "ble/common/BLETypes.h"
#include "ble/common/blecommon.h"
#include "ble/common/FunctionPointerWithContext.h"

/* Forward declaration for the implementation class */

namespace ble {
class BLEInstanceBase;

/**
 * @addtogroup ble
 * @{
 */

/**
 * Abstract away BLE-capable radio transceivers or SOCs.
 *
 * Instances of this class have three responsibilities:
 *   - Initialize the inner BLE subsystem.
 *   - Signal user code that BLE events are available and an API to process them.
 *   - Manage access to the instances abstracting each BLE layer:
 *        + GAP: Handle advertising and scan, as well as connection and
 *          disconnection.
 *        + GATTServer: API to construct and manage a GATT server, which connected peers can
 *          access.
 *        + GATTClient: API to interact with a peer GATT server.
 *        + SecurityManager: API to manage security.
 *
 * The user should not create BLE instances directly but rather access to the
 * singleton(s) holding the BLE interfaces present in the system by using the
 * static function Instance().
 *
 * @code
 * #include "ble/BLE.h"
 *
 * BLE& ble_interface = BLE::Instance();
 * @endcode
 *
 * Next, the signal handling/process mechanism should be set up. By design,
 * Mbed BLE does not impose to the user an event handling/processing mechanism;
 * however, it exposes APIs, which allows an application to compose its own:
 *   - onEventsToProcess(), which registers a callback that
 *     the BLE subsystem will call when there is an event ready to be processed.
 *   - processEvents(), which processes all the events present in the BLE subsystem.
 *
 * It is common to bind BLE event mechanism with Mbed EventQueue:
 *
 * @code
 * #include <events/mbed_events.h>
 * #include "ble/BLE.h"
 *
 * // declare the event queue, which the whole application will share.
 * static EventQueue event_queue(4 * EVENTS_EVENT_SIZE);
 *
 * // Function invoked when there is a BLE event available.
 * // Event processing is put into the event queue.
 * void schedule_ble_processing(BLE::OnEventsToProcessCallbackContext* context) {
 *   event_queue.call(callback(&(context->ble), &BLE::processEvents));
 * }
 *
 * int main()
 * {
 *   BLE &ble_interface = BLE::Instance();
 *
 *   // Bind event signaling to schedule_ble_processing
 *   ble_interface.onEventsToProcess(schedule_ble_processing);
 *
 *   // Launch BLE initialisation
 *
 *   // Dispatch events in the event queue
 *   event_queue.dispatch_forever();
 *   return 0;
 * }
 * @endcode
 *
 * Once the event processing mechanism is in place, the Bluetooth subsystem can
 * be initialized with the init() function. That function accepts in input a
 * callback, which will be invoked once the initialization process has finished.
 *
 * @code
 * void on_ble_init_complete(BLE::InitializationCompleteCallbackContext *context)
 * {
 *   BLE& ble_interface = context->ble;
 *   ble_error_t initialization_error = context->error;
 *
 *   if (initialization_error) {
 *     // handle error
 *     return;
 *   }
 *
 *   // The BLE interface can be accessed now.
 * }
 *
 * int main() {
 *   BLE &ble_interface = BLE::Instance();
 *   ble_interface.onEventsToProcess(schedule_ble_processing);
 *
 *   // Initialize the BLE interface
 *   ble_interface.init(on_ble_init_complete);
 *
 *   event_queue.dispatch_forever();
 *   return 0;
 * }
 * @endcode
 */
class BLE {
public:
    /**
     * Opaque type used to store the ID of a BLE instance.
     * @deprecated BLE singleton supports one instance. You may create multiple instances by using the constructor.
     */
    typedef unsigned InstanceID_t;

    /**
     * The value of the BLE::InstanceID_t for the default BLE instance.
     * @deprecated BLE singleton supports one instance. You may create multiple instances by using the constructor.
     */
    static const InstanceID_t DEFAULT_INSTANCE = 0;

    /**
     * The number of permitted BLE instances for the application.
     * @deprecated BLE singleton supports one instance. You may create multiple instances by using the constructor.
     */
    static const InstanceID_t NUM_INSTANCES = 1;

    // Prevent copy construction and copy assignment of BLE.
    BLE(const BLE &) = delete;
    BLE &operator=(const BLE &) = delete;

    /**
     * Get a reference to the BLE singleton.
     *
     * @note Calling Instance() is preferred over constructing a BLE object
     * directly because it returns a reference to singleton.
     *
     * @return A reference to a single object.
     */
    static BLE &Instance();

    /**
     * Get a reference to the BLE singleton corresponding to a given interface.
     *
     * There is a static array of BLE singletons.
     *
     * @note Calling Instance() is preferred over constructing a BLE object
     * directly because it returns references to singletons.
     *
     * @param[in] id BLE Instance ID to get.
     *
     * @return A reference to a single object.
     *
     * @pre id shall be less than NUM_INSTANCES.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "BLE singleton supports one instance. You may create multiple"
                          "instances by using the constructor. Please use BLE::Instance().")
    static BLE &Instance(InstanceID_t id)
    {
        return Instance();
    }

    /**
     * Fetch the ID of a BLE instance.
     *
     * @return Instance id of this BLE instance.
     */
    MBED_DEPRECATED_SINCE("mbed-os-6.3.0", "BLE singleton supports one instance. You may create multiple"
                          "instances by using the constructor.")
    InstanceID_t getInstanceID() const
    {
        return DEFAULT_INSTANCE;
    }

    /**
     * Events to process event.
     *
     * Instances of OnEventsToProcessCallbackContext are passed to the event
     * handler registered with onEventsToProcess().
     */
    struct OnEventsToProcessCallbackContext {
        /**
         * The ble instance which have events to process.
         */
        BLE &ble;
    };

    /**
     * Events to process event handler
     */
    typedef FunctionPointerWithContext<OnEventsToProcessCallbackContext *>
        OnEventsToProcessCallback_t;

    /**
     * Register a callback called when the BLE stack has pending work.
     *
     * By registering a callback, application code can know when event processing
     * has to be scheduled.
     *
     * @param on_event_cb Callback invoked when there are new events to process.
     */
    void onEventsToProcess(const OnEventsToProcessCallback_t &on_event_cb);

    /**
     * Process ALL pending events living in the BLE stack and return once all
     * events have been consumed.
     *
     * @see onEventsToProcess()
     */
    void processEvents();

    /**
     * Initialization complete event.
     *
     * This event is generated at the end of the init() procedure and is passed
     * to the completion callback passed to init().
     */
    struct InitializationCompleteCallbackContext {
        /**
         * Reference to the BLE object that has been initialized
         */
        BLE &ble;

        /**
         * Error status of the initialization.
         *
         * That value is set to BLE_ERROR_NONE if initialization completed
         * successfully or the appropriate error code otherwise.
         * */
        ble_error_t error;
    };

    /**
     * Initialization complete event handler.
     *
     * @note There are two versions of init(). In addition to the
     * function-pointer, init() can also take an <Object, member> tuple as its
     * callback target. In case of the latter, the following declaration doesn't
     * apply.
     */
    typedef void (*InitializationCompleteCallback_t)(
        InitializationCompleteCallbackContext *context
    );

    /**
     * Initialize the BLE controller/stack.
     *
     * init() hands control to the underlying BLE module to accomplish
     * initialization. This initialization may tacitly depend on other hardware
     * setup (such as clocks or power-modes) that happens early on during system
     * startup. It may not be safe to call init() from a global static context
     * where ordering is compiler-specific and can't be guaranteed - it is safe
     * to call BLE::init() from within main().
     *
     * @param[in] completion_cb A callback for when initialization completes for
     * a BLE instance. This is an optional parameter; if no callback is set up,
     * the application can still determine the status of initialization using
     * BLE::hasInitialized() (see below).
     *
     * @return BLE_ERROR_NONE if the initialization procedure started
     * successfully.
     *
     * @note If init() returns BLE_ERROR_NONE, the underlying stack must invoke
     * the initialization completion callback at some point.
     *
     * @note Nearly all BLE APIs would return BLE_ERROR_INITIALIZATION_INCOMPLETE
     * if used on an instance before the corresponding transport is initialized.
     *
     * @note There are two versions of init(). In addition to the
     * function-pointer, init() can also take an <Object, member> pair as its
     * callback target.
     *
     * @attention This should be called before using anything else in the BLE
     * API.
     */
    ble_error_t init(InitializationCompleteCallback_t completion_cb = nullptr)
    {
        FunctionPointerWithContext<InitializationCompleteCallbackContext *> callback(completion_cb);
        return initImplementation(callback);
    }

    /**
     * Initialize the BLE controller/stack.
     *
     * This is an alternate declaration for init(). This one takes an
     * <Object, member> pair as its callback target.
     *
     * @param[in] object Object, which will be used to invoke the completion callback.
     * @param[in] completion_cb Member function pointer, which will be invoked when
     * initialization is complete.
     */
    template<typename T>
    ble_error_t init(T *object, void (T::*completion_cb)(InitializationCompleteCallbackContext *context))
    {
        FunctionPointerWithContext<InitializationCompleteCallbackContext *> callback(object, completion_cb);
        return initImplementation(callback);
    }

    /**
     * Indicate if the BLE instance has been initialized.
     *
     * @return true if initialization has completed for the underlying BLE
     * transport.
     *
     * @note The application should set up a callback to signal completion of
     * initialization when using init().
     */
    bool hasInitialized() const;

    /**
     * Shut down the underlying stack, and reset state of this BLE instance.
     *
     * @return BLE_ERROR_NONE if the instance was shut down without error or the
     * appropriate error code.
     *
     * @attention init() must be called afterward to reinstate services and
     * GAP state. This API offers a way to repopulate the GATT database with new
     * services and characteristics.
     */
    ble_error_t shutdown();

    /**
     * This call allows the application to get the BLE stack version information.
     *
     * @return A pointer to a const string representing the version.
     *
     * @note The BLE API owns the string returned.
     */
    const char *getVersion();

    /**
     * Accessor to Gap. All Gap-related functionality requires going through
     * this accessor.
     *
     * @return A reference to a Gap object associated to this BLE instance.
     */
    ble::Gap &gap();

    /**
     * A const alternative to gap().
     *
     * @return A const reference to a Gap object associated to this BLE instance.
     */
    const ble::Gap &gap() const;

#if BLE_FEATURE_GATT_SERVER
    /**
     * Accessor to GattServer. All GattServer related functionality requires
     * going through this accessor.
     *
     * @return A reference to a GattServer object associated to this BLE instance.
     */
    ble::GattServer &gattServer();

    /**
     * A const alternative to gattServer().
     *
     * @return A const reference to a GattServer object associated to this BLE
     * instance.
     */
    const ble::GattServer &gattServer() const;
#endif // BLE_FEATURE_GATT_SERVER

#if BLE_FEATURE_GATT_CLIENT
    /**
     * Accessors to GattClient. All GattClient related functionality requires
     * going through this accessor.
     *
     * @return A reference to a GattClient object associated to this BLE instance.
     */
    ble::GattClient &gattClient();

    /**
     * A const alternative to gattClient().
     *
     * @return A const reference to a GattClient object associated to this BLE
     * instance.
     */
    const ble::GattClient &gattClient() const;
#endif // BLE_FEATURE_GATT_CLIENT

#if BLE_FEATURE_SECURITY
    /**
     * Accessors to SecurityManager. All SecurityManager-related functionality
     * requires going through this accessor.
     *
     * @return A reference to a SecurityManager object associated to this BLE
     * instance.
     */
    ble::SecurityManager &securityManager();

    /**
     * A const alternative to securityManager().
     *
     * @return A const reference to a SecurityManager object associated to this
     * BLE instance.
     */
    const ble::SecurityManager &securityManager() const;
#endif // BLE_FEATURE_SECURITY

    /**
     * Translate error code into a printable string.
     *
     * @param[in] error Error code returned by BLE functions.
     *
     * @return A pointer to a const string describing the error.
     */
    static const char *errorToString(ble_error_t error);

    /**
     * This function allows the BLE stack to signal that there is work to do and
     * event processing should be done (BLE::processEvent()).
     *
     * @note This function should be called by the port of BLE_API. It is not
     * meant to be used by end users.
     */
    void signalEventsToProcess();

private:
    friend class ble::BLEInstanceBase;

    /**
     * Constructor for a handle to a BLE instance (the BLE stack). BLE handles
     * are thin wrappers around a transport object (that is, ptr. to
     * ble::BLEInstanceBase).
     *
     * @param[in] transport Ble transport used for the BLE instance.
     * @note Cordio supports only one instance.
     */
    BLE(ble::BLEInstanceBase &transport);

    /**
     * Implementation of init() [internal to BLE_API].
     *
     * The implementation is separated into a private method because it isn't
     * suitable to be included in the header.
     */
    ble_error_t initImplementation(
        FunctionPointerWithContext<InitializationCompleteCallbackContext *> callback
    );

private:
    ble::BLEInstanceBase &transport; /* The device-specific backend */
    OnEventsToProcessCallback_t whenEventsToProcess;
    bool event_signaled;
};

}

using ble::BLE;
/**
 * @namespace ble Entry namespace for all BLE API definitions.
 */

/**
 * @}
 */

#endif /* ifndef MBED_BLE_H__ */
