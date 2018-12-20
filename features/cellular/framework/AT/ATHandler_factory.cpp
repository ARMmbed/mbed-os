/*
 * Copyright (c) 2018, Arm Limited and affiliates.
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

#include "ATHandler.h"
#include "SingletonPtr.h"

using namespace mbed;

ATHandler *ATHandler::_atHandlers = NULL;

// each parser is associated with one filehandle (that is UART)
ATHandler *ATHandler::get_instance(FileHandle *fileHandle, events::EventQueue &queue, uint32_t timeout,
                                   const char *delimiter, uint16_t send_delay, bool debug_on)
{
    if (!fileHandle) {
        return NULL;
    }

    singleton_lock();
    ATHandler *atHandler = _atHandlers;
    while (atHandler) {
        if (atHandler->get_file_handle() == fileHandle) {
            atHandler->inc_ref_count();
            singleton_unlock();
            return atHandler;
        }
        atHandler = atHandler->_nextATHandler;
    }

    atHandler = new ATHandler(fileHandle, queue, timeout, delimiter, send_delay);
    if (debug_on) {
        atHandler->set_debug(debug_on);
    }
    atHandler->_nextATHandler = _atHandlers;
    _atHandlers = atHandler;

    singleton_unlock();
    return atHandler;
}

nsapi_error_t ATHandler::close()
{
    if (get_ref_count() == 0) {
        return NSAPI_ERROR_PARAMETER;
    }

    singleton_lock();
    dec_ref_count();
    if (get_ref_count() == 0) {
        // we can delete this at_handler
        ATHandler *atHandler = _atHandlers;
        ATHandler *prev = NULL;
        while (atHandler) {
            if (atHandler == this) {
                if (prev == NULL) {
                    _atHandlers = _atHandlers->_nextATHandler;
                } else {
                    prev->_nextATHandler = atHandler->_nextATHandler;
                }
                delete this;
                break;
            } else {
                prev = atHandler;
                atHandler = atHandler->_nextATHandler;
            }
        }
    }
    singleton_unlock();
    return NSAPI_ERROR_OK;
}

void ATHandler::set_at_timeout_list(uint32_t timeout_milliseconds, bool default_timeout)
{
    ATHandler *atHandler = _atHandlers;
    singleton_lock();
    while (atHandler) {
        atHandler->set_at_timeout(timeout_milliseconds, default_timeout);
        atHandler = atHandler->_nextATHandler;
    }
    singleton_unlock();
}

void ATHandler::set_debug_list(bool debug_on)
{
    ATHandler *atHandler = _atHandlers;
    singleton_lock();
    while (atHandler) {
        atHandler->set_debug(debug_on);
        atHandler = atHandler->_nextATHandler;
    }
    singleton_unlock();
}
