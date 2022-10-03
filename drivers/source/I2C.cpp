/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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

#include "drivers/I2C.h"
#include "drivers/DigitalInOut.h"
#include "platform/mbed_wait_api.h"

#if DEVICE_I2C

#if DEVICE_I2C_ASYNCH
#include "platform/mbed_power_mgmt.h"
#include "rtos/EventFlags.h"
#endif

namespace mbed {

I2C::I2C(PinName sda, PinName scl) :
#if DEVICE_I2C_ASYNCH
    _irq(this), _usage(DMA_USAGE_NEVER), _deep_sleep_locked(false),
#endif
    _i2c(), _hz(100000)
{
    lock();
    // The init function also set the frequency to 100000
    _sda = sda;
    _scl = scl;
    recover(sda, scl);
    i2c_init(&_i2c, _sda, _scl);
    unlock();
}

I2C::I2C(const i2c_pinmap_t &static_pinmap) :
#if DEVICE_I2C_ASYNCH
    _irq(this), _usage(DMA_USAGE_NEVER), _deep_sleep_locked(false),
#endif
    _i2c(), _hz(100000)
{
    lock();
    // The init function also set the frequency to 100000
    _sda = static_pinmap.sda_pin;
    _scl = static_pinmap.scl_pin;
    recover(static_pinmap.sda_pin, static_pinmap.scl_pin);
    i2c_init_direct(&_i2c, &static_pinmap);
    unlock();
}

void I2C::frequency(int hz)
{
    lock();
    _hz = hz;

    // We want to update the frequency even if we are already the bus owners
    i2c_frequency(&_i2c, _hz);

    unlock();
}

// write - Master Transmitter Mode
I2C::Result I2C::write(int address, const char *data, int length, bool repeated)
{
    lock();

    int stop = (repeated) ? 0 : 1;
    int written = i2c_write(&_i2c, address, data, length, stop);

    unlock();

    // Note: C i2c_write() function does not distinguish between NACKs and errors, so assume NACK if read did not go through
    return length == written ? Result::ACK : Result::NACK;
}

// read - Master Receiver Mode
I2C::Result I2C::read(int address, char *data, int length, bool repeated)
{
    lock();

    int stop = (repeated) ? 0 : 1;
    int read = i2c_read(&_i2c, address, data, length, stop);

    unlock();

    // Note: C i2c_read() function does not distinguish between NACKs and errors, so assume NACK if read did not go through
    return length == read ? Result::ACK : Result::NACK;
}

void I2C::start(void)
{
    lock();
    i2c_start(&_i2c);
    unlock();
}

int I2C::read_byte(bool ack)
{
    lock();
    int ret;
    if (ack) {
        ret = i2c_byte_read(&_i2c, 0);
    } else {
        ret = i2c_byte_read(&_i2c, 1);
    }
    unlock();
    return ret;
}

I2C::Result I2C::write_byte(int data)
{
    lock();
    int ret = i2c_byte_write(&_i2c, data);
    unlock();

    switch (ret) {
        case 0:
            return Result::NACK;
        case 1:
            return Result::ACK;
        case 2:
            return Result::TIMEOUT;
        default:
            return Result::OTHER_ERROR;
    }
}

int I2C::write(int data)
{
    auto result = write_byte(data);

    // Replicate the legacy return code
    switch (result) {
        case Result::ACK:
            return 1;
        case Result::NACK:
            return 0;
        case Result::TIMEOUT:
            return 2;
        default:
            return static_cast<int>(result);
    }
}

void I2C::stop(void)
{
    lock();
    i2c_stop(&_i2c);
    unlock();
}

void I2C::lock()
{
    _mutex->lock();
}

void I2C::unlock()
{
    _mutex->unlock();
}

int I2C::recover(PinName sda, PinName scl)
{
    DigitalInOut pin_sda(sda, PIN_INPUT, PullNone, 1);
    DigitalInOut pin_scl(scl, PIN_INPUT, PullNone, 1);

    // Return as SCL is low and no access to become master.
    if (pin_scl == 0) {
        return I2C_ERROR_BUS_BUSY;
    }

    // Return successfully as SDA and SCL is high
    if (pin_sda == 1) {
        return 0;
    }

    // Send clock pulses, for device to recover 9
    pin_scl.mode(PullNone);
    pin_scl.output();
    for (int count = 0; count < 10; count++) {
        pin_scl.mode(PullNone);
        pin_scl = 0;
        wait_us(5);
        pin_scl.mode(PullUp);
        pin_scl = 1;
        wait_us(5);
    }

    // Send Stop
    pin_sda.output();
    pin_sda = 0;
    wait_us(5);
    pin_scl = 1;
    wait_us(5);
    pin_sda = 1;
    wait_us(5);

    pin_sda.input();
    pin_scl.input();
    if ((pin_scl == 0) || (pin_sda == 0)) {
        // Return as SCL is low and no access to become master.
        return I2C_ERROR_BUS_BUSY;
    }

    return 0;
}

#if DEVICE_I2C_ASYNCH

int I2C::transfer(int address, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, const event_callback_t &callback, int event, bool repeated)
{
    lock();
    if (i2c_active(&_i2c)) {
        unlock();
        return -1; // transaction ongoing
    }
    lock_deep_sleep();

    _callback = callback;
    int stop = (repeated) ? 0 : 1;
    _irq.callback(&I2C::irq_handler_asynch);
    i2c_transfer_asynch(&_i2c, (void *)tx_buffer, tx_length, (void *)rx_buffer, rx_length, address, stop, _irq.entry(), event, _usage);
    unlock();
    return 0;
}

void I2C::abort_transfer(void)
{
    lock();
    i2c_abort_asynch(&_i2c);
    unlock_deep_sleep();
    unlock();
}

I2C::Result I2C::transfer_and_wait(int address, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, rtos::Kernel::Clock::duration_u32 timeout, bool repeated)
{
    // Use EventFlags to suspend the thread until the transfer finishes
    rtos::EventFlags transferResultFlags("I2C::Result EvFlags");

    // Simple callback from the transfer that sets the EventFlags using the I2C result event
    event_callback_t transferCallback([&](int event) {
        transferResultFlags.set(event);
    });

    transfer(address, tx_buffer, tx_length, rx_buffer, rx_length, transferCallback, I2C_EVENT_ALL, repeated);

    // Wait until transfer complete, error, or timeout
    uint32_t result = transferResultFlags.wait_any_for(I2C_EVENT_ALL, timeout);

    if (result & osFlagsError) {
        if (result == osFlagsErrorTimeout) {
            // Timeout expired, cancel transfer.
            abort_transfer();
            return Result::TIMEOUT;
        } else {
            // Other event flags error.  Transfer might be still running so cancel it.
            abort_transfer();
            return Result::OTHER_ERROR;
        }
    } else {
        // Note: Cannot use a switch here because multiple flags might be set at the same time (possible
        // in the STM32 HAL code at least).
        if (result & I2C_EVENT_TRANSFER_COMPLETE) {
            return Result::ACK;
        } else if ((result & I2C_EVENT_ERROR_NO_SLAVE) || (result & I2C_EVENT_TRANSFER_EARLY_NACK)) {
            // Both of these events mean that a NACK was received somewhere.  Theoretically NO_SLAVE means
            // NACK while transmitting address and EARLY_NACK means nack during the write operation.
            // But these aren't distinguished in the Result enum and even some of the HALs treat them
            // interchangeably.
            return Result::NACK;
        } else {
            // Other / unknown error code
            return Result::OTHER_ERROR;
        }
    }
}

void I2C::irq_handler_asynch(void)
{
    int event = i2c_irq_handler_asynch(&_i2c);
    if (_callback && event) {
        _callback.call(event);
    }

    if (event) {
        unlock_deep_sleep();
    }
}

void I2C::lock_deep_sleep()
{
    if (_deep_sleep_locked == false) {
        sleep_manager_lock_deep_sleep();
        _deep_sleep_locked = true;
    }
}

void I2C::unlock_deep_sleep()
{
    if (_deep_sleep_locked == true) {
        sleep_manager_unlock_deep_sleep();
        _deep_sleep_locked = false;
    }
}

#endif

} // namespace mbed

#endif
