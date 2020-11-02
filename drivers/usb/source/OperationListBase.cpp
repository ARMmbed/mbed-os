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

#include "OperationListBase.h"
#include "AsyncOp.h"
#include "mbed_assert.h"

OperationListBase::OperationListBase()
{

}

OperationListBase::~OperationListBase()
{
    remove_all();
}

bool OperationListBase::empty()
{
    return _list.head() == NULL;
}

void OperationListBase::add(AsyncOp *op)
{
    bool was_empty = _list.head() == NULL;
    op->_list = this;
    _list.enqueue(op);
    if (was_empty) {
        process();
    }
}

void OperationListBase::process()
{
    while (true) {
        AsyncOp *op = static_cast<AsyncOp *>(_list.head());
        if (op == NULL) {
            // List empty, nothing left to do
            break;
        }
        if (!op->process()) {
            // Processing is in progress
            break;
        }
        _list.dequeue();
        op->complete();
    }
}

void OperationListBase::remove(AsyncOp *op)
{
    bool head = _list.head() == op;
    _list.remove(op);
    if (head) {
        process();
    }
}

AsyncOp *OperationListBase::dequeue_raw()
{
    return static_cast<AsyncOp *>(_list.dequeue());
}

void OperationListBase::remove_all()
{
    while (true) {
        AsyncOp *op = static_cast<AsyncOp *>(_list.head());
        if (op == NULL) {
            // List empty, nothing left to do
            break;
        }
        op->complete();
    }
}
