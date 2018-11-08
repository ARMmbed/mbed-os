/* mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#ifndef MBED_TRANSACTION_H
#define MBED_TRANSACTION_H

#include "platform/platform.h"
#include "platform/FunctionPointer.h"

namespace mbed {
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_Transaction Transaction class
 * @{
 */

/** Transaction structure
 */
typedef struct {
    void *tx_buffer;           /**< Tx buffer */
    size_t tx_length;          /**< Length of Tx buffer*/
    void *rx_buffer;           /**< Rx buffer */
    size_t rx_length;          /**< Length of Rx buffer */
    uint32_t event;            /**< Event for a transaction */
    event_callback_t callback; /**< User's callback */
    uint8_t width;             /**< Buffer's word width (8, 16, 32, 64) */
} transaction_t;

/** Transaction class defines a transaction.
 *
 * @note Synchronization level: Not protected
 */
template<typename Class>
class Transaction {
public:
    Transaction(Class *tpointer, const transaction_t &transaction) : _obj(tpointer), _data(transaction)
    {
    }

    Transaction() : _obj(), _data()
    {
    }

    ~Transaction()
    {
    }

    /** Get object's instance for the transaction
     *
     * @return The object which was stored
     */
    Class *get_object()
    {
        return _obj;
    }

    /** Get the transaction
     *
     * @return The transaction which was stored
     */
    transaction_t *get_transaction()
    {
        return &_data;
    }

private:
    Class *_obj;
    transaction_t _data;
};
/**@}*/

/**@}*/
}

#endif
