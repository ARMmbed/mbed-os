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
#include "drivers/SPI.h"
#include "platform/mbed_critical.h"
#include "mbed_error.h"

#if DEVICE_SPI_ASYNCH
#include "platform/mbed_power_mgmt.h"
#endif

#if DEVICE_SPI

namespace mbed {

SPI::spi_peripheral_s SPI::_peripherals[SPI_PERIPHERALS_USED];
int SPI::_peripherals_used;

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
    _transfer_and_wait_flags("SPI::transfer_and_wait() flags"),
#endif
    _mosi(mosi),
    _miso(miso),
    _sclk(sclk),
    _hw_ssel(ssel),
    _sw_ssel(NC),
    _static_pinmap(NULL),
    _init_func(_do_init)
{
    // Need backwards compatibility with HALs not providing API
#ifdef DEVICE_SPI_COUNT
    _peripheral_name = spi_get_peripheral_name(_mosi, _miso, _sclk);
#else
    _peripheral_name = GlobalSPI;
#endif

    _do_construct();
}

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel, use_gpio_ssel_t) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
    _transfer_and_wait_flags("SPI::transfer_and_wait() flags"),
#endif
    _mosi(mosi),
    _miso(miso),
    _sclk(sclk),
    _hw_ssel(NC),
    _sw_ssel(ssel, 1),
    _static_pinmap(NULL),
    _init_func(_do_init)
{
    // Need backwards compatibility with HALs not providing API
#ifdef DEVICE_SPI_COUNT
    _peripheral_name = spi_get_peripheral_name(_mosi, _miso, _sclk);
#else
    _peripheral_name = GlobalSPI;
#endif
    _do_construct();
}

SPI::SPI(const spi_pinmap_t &pinmap) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
    _transfer_and_wait_flags("SPI::transfer_and_wait() flags"),
#endif
    _mosi(pinmap.mosi_pin),
    _miso(pinmap.miso_pin),
    _sclk(pinmap.sclk_pin),
    _hw_ssel(pinmap.ssel_pin),
    _sw_ssel(NC),
    _static_pinmap(&pinmap),
    _peripheral_name((SPIName)pinmap.peripheral),
    _init_func(_do_init_direct)

{
    _do_construct();
}

SPI::SPI(const spi_pinmap_t &pinmap, PinName ssel) :
#if DEVICE_SPI_ASYNCH
    _irq(this),
    _transfer_and_wait_flags("SPI::transfer_and_wait() flags"),
#endif
    _mosi(pinmap.mosi_pin),
    _miso(pinmap.miso_pin),
    _sclk(pinmap.sclk_pin),
    _hw_ssel(NC),
    _sw_ssel(ssel, 1),
    _static_pinmap(&pinmap),
    _peripheral_name((SPIName)pinmap.peripheral),
    _init_func(_do_init_direct)
{
    _do_construct();
}

void SPI::_do_init(SPI *obj)
{
    obj->_peripheral->initialized = true;
    spi_init(&obj->_peripheral->spi, obj->_mosi, obj->_miso, obj->_sclk, obj->_hw_ssel);
}

void SPI::_do_init_direct(SPI *obj)
{
    obj->_peripheral->initialized = true;
    spi_init_direct(&obj->_peripheral->spi, obj->_static_pinmap);
}

rtos::Mutex &SPI::_get_peripherals_mutex()
{
    static rtos::Mutex peripherals_mutex;
    return peripherals_mutex;
}

void SPI::_do_construct()
{
    // No lock needed in the constructor
#if DEVICE_SPI_ASYNCH
    _usage = DMA_USAGE_NEVER;
    _deep_sleep_locked = false;
#endif
    _select_count = 0;
    _bits = 8;
    _mode = 0;
    _hz = 1000000;
    _write_fill = SPI_FILL_CHAR;

    {
        rtos::ScopedMutexLock lock(_get_peripherals_mutex());

        // lookup and claim the peripheral with the mutex locked in case another thread is
        // also trying to claim it
        _peripheral = SPI::_lookup(_peripheral_name);
        if (!_peripheral) {
            _peripheral = SPI::_alloc();
            _peripheral->name = _peripheral_name;
        }

        if (_peripheral->numUsers == std::numeric_limits<uint8_t>::max()) {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_MUTEX_LOCK_FAILED), "Ref count at max!");
        }

        _peripheral->numUsers++;
    }

#if DEVICE_SPI_ASYNCH && MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
    // prime the SingletonPtr, so we don't have a problem trying to
    // construct the buffer if asynch operation initiated from IRQ
    _peripheral->transaction_buffer.get();
#endif
    // we don't need to _acquire at this stage.
    // this will be done anyway before any operation.
}

SPI::~SPI()
{
    if (_peripheral->numUsers == 0) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_MUTEX_UNLOCK_FAILED), "Ref count at 0?");
    }

    {
        rtos::ScopedMutexLock lock(_get_peripherals_mutex());

        /* Make sure a stale pointer isn't left in peripheral's owner field */
        if (_peripheral->owner == this) {
            _peripheral->owner = nullptr;
        }

        if (--_peripheral->numUsers == 0) {
            _dealloc(_peripheral);
        }
    }
}

SPI::spi_peripheral_s *SPI::_lookup(SPI::SPIName name)
{
    SPI::spi_peripheral_s *result = nullptr;
    for (int idx = 0; idx < _peripherals_used; idx++) {
        if (_peripherals[idx].numUsers > 0 && _peripherals[idx].name == name) {
            result = &_peripherals[idx];
            break;
        }
    }
    return result;
}

SPI::spi_peripheral_s *SPI::_alloc()
{
    MBED_ASSERT(_peripherals_used < SPI_PERIPHERALS_USED);

    // Find an unused peripheral to return
    for (spi_peripheral_s &peripheral : _peripherals) {
        if (peripheral.numUsers == 0) {
            _peripherals_used++;
            return &peripheral;
        }
    }

    MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_INVALID_DATA_DETECTED), "Can't find new peripheral!");
}

void SPI::_dealloc(SPI::spi_peripheral_s *peripheral)
{
    if (peripheral->initialized) {
        spi_free(&peripheral->spi);
        peripheral->initialized = false;
    }
    --_peripherals_used;
}

void SPI::format(int bits, int mode)
{
    lock();
    _bits = bits;
    _mode = mode;
    // If changing format while you are the owner then just
    // update format, but if owner is changed then even frequency should be
    // updated which is done by acquire.
    if (_peripheral->owner == this) {
        spi_format(&_peripheral->spi, _bits, _mode, 0);
    } else {
        _acquire();
    }
    unlock();
}

void SPI::frequency(int hz)
{
    lock();
    _hz = hz;
    // If changing format while you are the owner then just
    // update frequency, but if owner is changed then even frequency should be
    // updated which is done by acquire.
    if (_peripheral->owner == this) {
        spi_frequency(&_peripheral->spi, _hz);
    } else {
        _acquire();
    }
    unlock();
}

// Note: Private function with no locking
void SPI::_acquire()
{
    if (_peripheral->owner != this) {
        _init_func(this);
        spi_format(&_peripheral->spi, _bits, _mode, 0);
        spi_frequency(&_peripheral->spi, _hz);
        _peripheral->owner = this;
    }
}

int SPI::write(int value)
{
    select();
    int ret = spi_master_write(&_peripheral->spi, value);
    deselect();
    return ret;
}

int SPI::write_internal(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length)
{
    select();
    int ret = spi_master_block_write(&_peripheral->spi,
                                     reinterpret_cast<char const *>(tx_buffer), tx_length,
                                     reinterpret_cast<char *>(rx_buffer), rx_length,
                                     _write_fill);
    deselect();
    return ret;
}

void SPI::_set_ssel(int val)
{
    if (_sw_ssel.is_connected()) {
        _sw_ssel = val;
    }
}

void SPI::lock()
{
    _peripheral->mutex->lock();
}

void SPI::select()
{
    lock();
    if (_select_count++ == 0) {
        _acquire();
        _set_ssel(0);
    }
}

void SPI::unlock()
{
    _peripheral->mutex->unlock();
}

void SPI::deselect()
{
    if (--_select_count == 0) {
        _set_ssel(1);
    }
    unlock();
}

void SPI::set_default_write_value(char data)
{
    // this does not actually need to lock the peripheral.
    lock();
    _write_fill = data;
    unlock();
}

#if DEVICE_SPI_ASYNCH

int SPI::transfer_internal(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, const event_callback_t &callback, int event)
{
    if (spi_active(&_peripheral->spi)) {
        return queue_transfer(tx_buffer, tx_length, rx_buffer, rx_length, _bits, callback, event);
    }
    start_transfer(tx_buffer, tx_length, rx_buffer, rx_length, _bits, callback, event);
    return 0;
}

int SPI::transfer_and_wait_internal(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, rtos::Kernel::Clock::duration_u32 timeout)
{
    // Simple callback from the transfer that sets the EventFlags using the I2C result event
    event_callback_t transferCallback([&](int event) {
        _transfer_and_wait_flags.set(event);
    });

    int txRet = transfer_internal(tx_buffer, tx_length, rx_buffer, rx_length, transferCallback, SPI_EVENT_ALL);
    if (txRet != 0) {
        return txRet;
    }

    // Wait until transfer complete, error, or timeout
    uint32_t result = _transfer_and_wait_flags.wait_any_for(SPI_EVENT_ALL, timeout);

    if (result & osFlagsError) {
        if (result == osFlagsErrorTimeout) {
            // Timeout expired, cancel transfer.
            abort_transfer();
            return 1;
        } else {
            // Other event flags error.  Transfer might be still running so cancel it.
            abort_transfer();
            return 2;
        }
    } else {
        // Note: Cannot use a switch here because multiple flags might be set at the same time (possible
        // in the STM32 HAL code at least).
        if (result == SPI_EVENT_COMPLETE) {
            return 0;
        } else {
            // SPI peripheral level error
            return 2;
        }
    }
}


void SPI::abort_transfer()
{
    // There is a potential for race condition here which we need to be aware of.
    // There may or may not be a transfer actually running when we enter this function.
    // To work through this, if there is a transfer in progress, we use spi_abort_asynch
    // which disables the transfer interrupt.
    // Then, we check _transfer_in_progress again.  If it is true, then it means the ISR
    // fired during the call to spi_abort_async, so the transfer has already completed normally.

    if (_transfer_in_progress) {
        spi_abort_asynch(&_peripheral->spi);
    }

    if (_transfer_in_progress) {
        // End-of-transfer ISR never fired, clean up.
        unlock_deep_sleep();

        if (--_select_count == 0) {
            _set_ssel(1);
        }

#if __DCACHE_PRESENT
        if (_transfer_in_progress_uses_dma && _transfer_in_progress_rx_len > 0) {
            // If the cache is present, invalidate the Rx data so it's loaded from main RAM.
            // We only want to do this if DMA actually got used for the transfer because, if interrupts
            // were used instead, the cache might have the correct data and NOT the main memory.
            SCB_InvalidateDCache_by_Addr(_transfer_in_progress_rx_buffer, _transfer_in_progress_rx_len);
        }
#endif
        _transfer_in_progress = false;

#if MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
        dequeue_transaction();
#endif
    }
}

void SPI::clear_transfer_buffer()
{
#if MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
    _peripheral->transaction_buffer->reset();
#endif
}

void SPI::abort_all_transfers()
{
    clear_transfer_buffer();
    abort_transfer();
}

int SPI::set_dma_usage(DMAUsage usage)
{
    if (spi_active(&_peripheral->spi)) {
        return -1;
    }
    _usage = usage;
    return  0;
}

int SPI::queue_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event)
{
#if MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
    transaction_t t;

    t.tx_buffer = const_cast<void *>(tx_buffer);
    t.tx_length = tx_length;
    t.rx_buffer = rx_buffer;
    t.rx_length = rx_length;
    t.event = event;
    t.callback = callback;
    t.width = bit_width;
    Transaction<SPI> transaction(this, t);
    if (_peripheral->transaction_buffer->full()) {
        return -1; // the buffer is full
    } else {
        core_util_critical_section_enter();
        _peripheral->transaction_buffer->push(transaction);
        if (!spi_active(&_peripheral->spi)) {
            dequeue_transaction();
        }
        core_util_critical_section_exit();
        return 0;
    }
#else
    return -1;
#endif
}

void SPI::start_transfer(const void *tx_buffer, int tx_length, void *rx_buffer, int rx_length, unsigned char bit_width, const event_callback_t &callback, int event)
{
    lock_deep_sleep();

    // Acquire the hardware and (if using GPIO CS mode) select the chip.
    // But, if the user has already called select(), we can skip this step.
    if (_select_count++ == 0) {
        _acquire();
        _set_ssel(0);
    }

    _callback = callback;
    _irq.callback(&SPI::irq_handler_asynch);

#if __DCACHE_PRESENT
    // On devices with a cache, we need to carefully manage the Tx and Rx buffer cache invalidation.
    // We can assume that asynchronous SPI implementations might rely on DMA, and that DMA will
    // not interoperate with the CPU cache.  So, manual flushing/invalidation will be required.
    // This page is very useful for how to do this correctly:
    // https://community.st.com/t5/stm32-mcus-products/maintaining-cpu-data-cache-coherence-for-dma-buffers/td-p/95746
    if (tx_length > 0) {
        // For chips with a cache, we need to evict the Tx data from cache to main memory.
        // This ensures that the DMA controller can see the most up-to-date copy of the data.
        SCB_CleanDCache_by_Addr(const_cast<void *>(tx_buffer), tx_length);
    }

    // Additionally, we have to make sure that there aren't any pending changes which could be written back
    // to the Rx buffer memory by the cache at a later date, corrupting the DMA results.
    if (rx_length > 0) {
        SCB_InvalidateDCache_by_Addr(rx_buffer, rx_length);
    }
    _transfer_in_progress_rx_buffer = rx_buffer;
    _transfer_in_progress_rx_len = rx_length;
#endif

    _transfer_in_progress = true;

    _transfer_in_progress_uses_dma = spi_master_transfer(&_peripheral->spi, tx_buffer, tx_length, rx_buffer, rx_length, bit_width, _irq.entry(), event, _usage);
}

void SPI::lock_deep_sleep()
{
    if (_deep_sleep_locked == false) {
        sleep_manager_lock_deep_sleep();
        _deep_sleep_locked = true;
    }
}

void SPI::unlock_deep_sleep()
{
    if (_deep_sleep_locked == true) {
        sleep_manager_unlock_deep_sleep();
        _deep_sleep_locked = false;
    }
}

#if MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN

void SPI::start_transaction(transaction_t *data)
{
    start_transfer(data->tx_buffer, data->tx_length, data->rx_buffer, data->rx_length, data->width, data->callback, data->event);
}

void SPI::dequeue_transaction()
{
    Transaction<SPI> t;
    if (_peripheral->transaction_buffer->pop(t)) {
        SPI *obj = t.get_object();
        transaction_t *data = t.get_transaction();
        obj->start_transaction(data);
    }
}

#endif

void SPI::irq_handler_asynch(void)
{
    int event = spi_irq_handler_asynch(&_peripheral->spi);
    if ((event & SPI_EVENT_ALL)) {

#if __DCACHE_PRESENT
        if (_transfer_in_progress_uses_dma && _transfer_in_progress_rx_len > 0) {
            // If the cache is present, invalidate the Rx data so it's loaded from main RAM.
            // We only want to do this if DMA actually got used for the transfer because, if interrupts
            // were used instead, the cache might have the correct data and NOT the main memory.
            SCB_InvalidateDCache_by_Addr(_transfer_in_progress_rx_buffer, _transfer_in_progress_rx_len);
        }
#endif

        // If using GPIO CS mode, unless we were asked to keep the peripheral selected, deselect it.
        // If there's another transfer queued, we *do* want to deselect the peripheral now.
        // It will be reselected in start_transfer() which is called by dequeue_transaction() below.
        if (--_select_count == 0) {
            _set_ssel(1);
        }
        _transfer_in_progress = false;

        unlock_deep_sleep();

        if (_callback) {
            _callback.call(event & SPI_EVENT_ALL);
        }
    }
#if MBED_CONF_DRIVERS_SPI_TRANSACTION_QUEUE_LEN
    if (event & (SPI_EVENT_ALL | SPI_EVENT_INTERNAL_TRANSFER_COMPLETE)) {
        // SPI peripheral is free (event happened), dequeue transaction
        dequeue_transaction();
    }
#endif
}

#endif

} // namespace mbed

#endif
