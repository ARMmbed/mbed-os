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

#ifndef CORDIO_HCI_DRIVER_H_
#define CORDIO_HCI_DRIVER_H_

#include <stddef.h>
#include <stdint.h>
#include <BLETypes.h>
#include "wsf_buf.h"
#include "CordioHCITransportDriver.h"

namespace ble {
namespace vendor {
namespace cordio {

/**
 * Contain description of the memory pool used by the Cordio stack.
 */
struct buf_pool_desc_t {
    /**
     * Create a new memory pool description
     * @param buffer the Buffer used by the memory pool.
     * @param pool_desc How the memory is split
     */
    template<size_t BufferSize, size_t PoolCount>
    buf_pool_desc_t(
        uint8_t (&buffer)[BufferSize],
        const wsfBufPoolDesc_t (&pool_desc)[PoolCount]
    ) : buffer_memory(buffer), buffer_size(BufferSize),
        pool_description(pool_desc), pool_count(PoolCount)
    {
    }

    uint8_t* buffer_memory;         /// Pointer to the buffer memory
    size_t buffer_size;             /// Size of the buffer
    const wsfBufPoolDesc_t* pool_description;   /// Pointer to the first element describing the pool
    size_t pool_count;      /// Number of pools
};

/**
 * Base class of the HCI driver use by the BLE port of the Cordio stack.
 * This class provide to the stack:
 *   - The buffer necessary to run BLE API
 *   - The reset sequence of the BLE module
 *   - Access to the write function of the underlying HCITransport driver.
 */
class CordioHCIDriver {

    // hook for internal tests and passthrough driver
    friend class CordioHCIHook;

public:
    /**
     * Construct a new instance of an HCI driver.
     * @param transport_driver The driver used to communicate with the chip.
     */
    CordioHCIDriver(CordioHCITransportDriver& transport_driver);

    /**
     * Driver destructor
     */
    virtual ~CordioHCIDriver() { }

    /**
     * Return the set of memory pool which will be used by the Cordio stack
     */
    virtual buf_pool_desc_t get_buffer_pool_description() = 0;

    /**
     * Initialize the HCI driver.
     * This function start by initializing the transport driver then it delegates
     * what's remain of the initialization to the function do_initialize.
     */
    void initialize();

    /**
     * Termination of the driver.
     * It call in sequence:
     *   - do_terminate
     *   - terminate the transport driver.
     */
    void terminate();

    /**
     * Start the reset sequence of the BLE module.
     */
    virtual void start_reset_sequence();

    /**
     * Handle HCI messages received during the reset sequence.
     *
     * @param msg The HCI message received.
     * @note The driver should signal to the stack that the initialization
     * sequence is done by calling the function: signal_reset_sequence_done.
     */
    virtual void handle_reset_sequence(uint8_t *msg);

    /**
     * Get the random static address of the controller
     *
     * @return false if the address has not been set and true otherwise.
     */
    virtual bool get_random_static_address(ble::address_t& address);

    /**
     * Signal to the stack that the reset sequence has been done.
     */
    void signal_reset_sequence_done();

    /**
     * Write data in the transport channel.
     *
     * @param type The type of packet to transmit. It might be an HCI command
     * packet, ACL packet or EVT packet. Depending on the type of transport
     * it can prefix the packet itself.
     * @param len Number of bytes to transmit.
     * @param pData pointer to the data to transmit.
     *
     * @return The number of bytes which have been transmited.
     */
    uint16_t write(uint8_t type, uint16_t len, uint8_t *pData);

    /**
     * React to host stack inactivity.
     *
     * The host stack invoke this function when it is inactive. It allows a
     * driver to put its controller to sleep if all the conditions are met.
     *
     * Any call to write signals to the driver that the host stack is active.
     */
     virtual void on_host_stack_inactivity();

protected:
    /**
     * Return a default set of memory pool that the Cordio stack can use.
     * This function can be used to implement get_buffer_pool_description().
     */
    buf_pool_desc_t get_default_buffer_pool_description();

    /**
     * Allows the driver to set a random static address. Unlike the HCI command
     * this function reports the random static address to the whole BLE system.
     * @param random_static_address The random static address to set.
     */
    void set_random_static_address(const ble::address_t& random_static_address);

private:
    /**
     * Initialize the chip.
     * The transport is up at that time.
     */
    virtual void do_initialize() = 0;

    /**
     * Terminate the driver
     */
    virtual void do_terminate() = 0;

    CordioHCITransportDriver& _transport_driver;
};


} // namespace cordio
} // namespace vendor
} // namespace ble

#endif /* CORDIO_HCI_DRIVER_H_ */
