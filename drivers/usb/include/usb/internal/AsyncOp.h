/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
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

#ifndef MBED_ASYNC_OP_H
#define MBED_ASYNC_OP_H

#include "rtos/Mutex.h"
#include "rtos/Semaphore.h"
#include "Callback.h"

#include "LinkEntry.h"
#include "OperationListBase.h"

/** \defgroup mbed-os-internal Internal API */

/** \addtogroup drivers-internal-api Drivers
 * \ingroup mbed-os-internal
 */

/** \defgroup drivers-internal-api-usb USB
 * \ingroup drivers-internal-api
 */

/**
 * \defgroup drivers_AsyncOp AsyncOp class
 * \ingroup drivers-internal-api-usb
 * @{
 */
class AsyncOp: public LinkEntry {
public:

    /**
     * Construct a new AsyncOp object
     */
    AsyncOp();

    /**
     * Construct a new AsyncOp object
     *
     * @param callback Completion callback
     */
    AsyncOp(mbed::Callback<void()> &callback);

    /**
     * Cleanup resources used by this AsyncOp
     */
    virtual ~AsyncOp();

    /**
     * Wait for this asynchronous operation to complete
     *
     * If the timeout expires then this asynchronous operation is
     * aborted and the timeout flag is set.
     *
     * @note - the host object's lock MUST NOT be held when this call is made
     */
    void wait(rtos::Mutex *host_mutex, rtos::Kernel::Clock::duration_u32 rel_time = rtos::Kernel::wait_for_u32_forever);

    /**
     * Abort this asynchronous operation
     *
     * This function has no effect if the operation is complete. Otherwise
     * the aborted flag is set.
     *
     * @note - the host object's lock MUST be held when this call is made
     */
    void abort();

    /**
     * Check if this operation timed out
     *
     * @return true if this operation timed out, false otherwise
     */
    bool timeout();

    /**
     * Check if this operation was aborted
     *
     * @return true if this operation was aborted, false otherwise
     */
    bool aborted();

protected:

    /**
     * Callback indicating that something changed
     *
     * @return true if finished false if not
     */
    virtual bool process() = 0;

    /**
     * Callback indicating that this event finished
     */
    virtual void complete();

private:
    friend class OperationListBase;

    mbed::Callback<void()> _callback;
    OperationListBase *_list;
    rtos::Semaphore *_wait;
    bool _aborted;
    bool _timeout;

    void _abort(bool timeout);

    static void _host_lock(rtos::Mutex *host_mutex);

    static void _host_unlock(rtos::Mutex *host_mutex);
};

/** @}*/

#endif
