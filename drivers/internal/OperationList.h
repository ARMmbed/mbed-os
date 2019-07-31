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

#ifndef MBED_OPERATION_LIST_H
#define MBED_OPERATION_LIST_H

#include "OperationListBase.h"
#include "AsyncOp.h"

/**
 * \defgroup drivers_OperationList OperationList class
 * \ingroup drivers-internal-api-usb
 * @{
 */
template<class T>
class OperationList: public OperationListBase {
public:

    /**
     * Create a new empty operation list
     */
    OperationList()
    {

    }

    /**
     * Destroy this object and abort all operations
     */
    ~OperationList()
    {

    }
    /**
     * Add an operation to the list
     *
     * If the list was empty then call process on this
     * operation
     *
     * @param op Operation to add
     */
    void add(T *op)
    {
        OperationListBase::add(op);
    }

    /**
     * Remove an operation from the list
     *
     * If this was the head of the list then process the
     * next element in the list.
     *
     * @param op Operation to remove
     */
    void remove(T *op)
    {
        OperationListBase::remove(op);
    }

    /**
     * Dequeue the head of the list
     *
     * Remove the head of the operation list without completing it
     * or processing the next element. The caller must call the
     * AsnycOp::complete() function of the returned object.
     * Additionally process() must be called on this object
     * if there are still elements in the list.
     *
     * @return The asynchronous op at the head of the list
     */
    T *dequeue_raw()
    {
        return static_cast<AsyncOp *>(OperationListBase::dequeue_raw());
    }

};

/** @}*/

#endif
