/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#include "platform/mbed_wait_api.h"

#if DEVICE_I2C

namespace mbed {

I2C *I2C::_owner = NULL;
SingletonPtr<PlatformMutex> I2C::_mutex;

I2C::I2C(PinName sda, PinName scl) :
#if DEVICE_I2C_ASYNCH
                                     _irq(this), _usage(DMA_USAGE_NEVER),
#endif
                                      _i2c(), _hz(100000) {
    // No lock needed in the constructor

    // The init function also set the frequency to 100000
    _sda = sda;
    _scl = scl;

    i2c_init(&_i2c, _sda, _scl);

    // Used to avoid unnecessary frequency updates
    _owner = this;
}

void I2C::frequency(int hz) {
    lock();
    _hz = hz;

    // We want to update the frequency even if we are already the bus owners
    i2c_frequency(&_i2c, _hz);

    // Updating the frequency of the bus we become the owners of it
    _owner = this;
    unlock();
}

void I2C::aquire() {
    lock();
    if (_owner != this) {
        i2c_frequency(&_i2c, _hz);
        _owner = this;
    }
    unlock();
}

// write - Master Transmitter Mode
int I2C::write(int address, const char* data, int length, bool repeated) {
    lock();
    aquire();

    int stop = (repeated) ? 0 : 1;
    int written = i2c_write(&_i2c, address, data, length, stop);

    unlock();
    return length != written;
}

int I2C::write(int data) {
    lock();
    int ret = i2c_byte_write(&_i2c, data);
    unlock();
    return ret;
}

// read - Master Reciever Mode
int I2C::read(int address, char* data, int length, bool repeated) {
    lock();
    aquire();

    int stop = (repeated) ? 0 : 1;
    int read = i2c_read(&_i2c, address, data, length, stop);

    unlock();
    return length != read;
}

int I2C::read(int ack) {
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

void I2C::start(void) {
    lock();
    i2c_start(&_i2c);
    unlock();
}

void I2C::stop(void) {
    lock();
    i2c_stop(&_i2c);
    unlock();
}

void I2C::lock() {
    _mutex->lock();
}

void I2C::unlock() {
    _mutex->unlock();
}

int I2C::reset() {
    lock();
    // Try i2c_reset() i.e. target specific reset
    i2c_reset(&_i2c);

    // SCL and SDA as input pins with pullup
    gpio_init_inout(&_gpio_scl, _scl, PIN_INPUT, PullUp, 1);
    gpio_init_inout(&_gpio_sda, _sda, PIN_INPUT, PullUp, 1);

    // Read and verify if reset was successful
    if (gpio_read(&_gpio_scl)) {
        if (gpio_read(&_gpio_sda)) {
            // Return successfuly as SDA and SCL is high
            unlock();
            // i2c pin initialization required again, since current mux and pin
            // settings are as GPIO's
            i2c_init(&_i2c, _sda, _scl);
            return 0;
        }
    } else {
        // Return as SCL is low and no access to become master.
        unlock();
        return I2C_ERROR_BUS_BUSY;
    }

    // Send clock pulses, for device to recover 2*9
    int count = 20;
    while ( (!gpio_read(&_gpio_sda)) && (count > 0)) {
        count--;

        gpio_mode(&_gpio_scl, PullNone);
        gpio_dir(&_gpio_scl, PIN_OUTPUT);
        gpio_write(&_gpio_scl, 0);
        wait_us(10);

        gpio_mode(&_gpio_scl, PullUp);
        wait_us(10);

        // Wait for SCL to become high (clock-stretching)
        gpio_dir(&_gpio_scl, PIN_INPUT);
        int scl_count = 20;
        while ((scl_count--) && (!gpio_read(&_gpio_scl))) {
            wait_us(10);
        }
        // SCL line low, return with error
        if (!gpio_read(&_gpio_scl)) {
            unlock();
            return I2C_ERROR_BUS_BUSY;
        }
    }

    // SDA line still low, return with error
    if (!gpio_read(&_gpio_sda)) {
        unlock();
        return I2C_ERROR_BUS_BUSY;
    }

    // Send Stop
    gpio_dir(&_gpio_sda, PIN_OUTPUT);
    gpio_mode(&_gpio_sda, PullNone);
    gpio_write(&_gpio_sda, 1);

    // Check if recovery was successful?
    if (!gpio_read(&_gpio_scl) || !gpio_read(&_gpio_sda)) {
        unlock();
        return I2C_ERROR_BUS_BUSY;
    }

    // i2c pin initialization required again, since current mux and pin
    // settings are as GPIO's
    i2c_init(&_i2c, _sda, _scl);

    unlock();
    return 0;
}

#if DEVICE_I2C_ASYNCH

int I2C::transfer(int address, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, const event_callback_t& callback, int event, bool repeated)
{
    lock();
    if (i2c_active(&_i2c)) {
        unlock();
        return -1; // transaction ongoing
    }
    aquire();

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
    unlock();
}

void I2C::irq_handler_asynch(void)
{
    int event = i2c_irq_handler_asynch(&_i2c);
    if (_callback && event) {
        _callback.call(event);
    }
}


#endif

} // namespace mbed

#endif
