/* mbed Microcontroller Library
 * Copyright (c) 2017 Nordic Semiconductor
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

#include <string.h>
#include "QSPI.h"
#include "qspi_api.h"
#include "mbed_assert.h"


#ifdef DEVICE_QSPI

// Command for reading status register
#define QSPI_STD_CMD_RDSR 0x05
// Command for writing status register
#define QSPI_STD_CMD_WRSR 0x01
// Command for reading control register (supported only by some memories)
#define QSPI_STD_CMD_RDCR 0x35
// Command for writing control register (supported only by some memories)
#define QSPI_STD_CMD_WRCR 0x3E

namespace mbed {

SingletonPtr<PlatformMutex> QSPI::_mutex;

static inline bool is_aligned(uint32_t number, uint32_t alignment)
{
    if ((number % alignment) != 0) {
        return false;
    } else {
        return true;
    }
}

QSPI::QSPI(PinName miso, PinName mosi, PinName sclk, PinName ssel, int hz, int spi_mode) :
    #if DEVICE_SPI_ASYNCH
            _irq(this),
            _usage(DMA_USAGE_NEVER)
    #endif
{
    qspi_pins_t pins;

    pins.sclk = sclk;
    pins.ssel = ssel;
    pins.io0 =  miso;
    pins.io1 =  mosi;
    pins.io2 =  NC;
    pins.io3 =  NC;

    /* Preparing physical communication part */
    qspi_init(&_qspi, hz, &pins, spi_mode);
    quad_mode_hw_support = false;
}

QSPI::QSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, int hz, int spi_mode) :
    #if DEVICE_SPI_ASYNCH
            _irq(this),
            _usage(DMA_USAGE_NEVER)
    #endif
{
    qspi_pins_t pins;

    pins.sclk = sclk;
    pins.ssel = ssel;
    pins.io0 = io0;
    pins.io1 = io1;
    pins.io2 = io2;
    pins.io3 = io3;

    /* Preparing physical communication part */
    qspi_init(&_qspi, hz, &pins, spi_mode);
    quad_mode_hw_support = true;
}

QSPI::~QSPI()
{

}

int QSPI::init()
{
    /* Preparing protocol communication part. We do not care about return values because
     * we are using standard, most generic values. */
    (void)qspi_prepare(&_qspi,
                       QSPI_ADDRMODE_24BIT,
                       QSPI_READMODE_1_1_1,
                       QSPI_WRITEMODE_1_1_1);

    /* Setting default, generic flash memory values. Could works with every flash memory. */
    flash_size = 0;
    page_size = 256;
    sector_size = 4096;

    /* Starting QSPI */
    int ret = 0;
    _mutex->lock();
    if (qspi_start(&_qspi)) {
        ret = -1;
    }
    _mutex->unlock();
    return ret;
}

int QSPI::init(QSPIMemSpec *memspec)
{
    uint8_t status_value[2];
    uint8_t cmd_to_send = 0;
    uint8_t *p_data = NULL;
    uint32_t data_len = 0;

    int ret = 0;

    /* If there is no enough pins so why we could give a chance to start quad mode? */
    if (!quad_mode_hw_support                                   &&
        (memspec->read_mode_get()  != QSPI_READMODE_1_1_1)      &&
        (memspec->write_mode_get() != QSPI_WRITEMODE_1_1_1))
    {
        return -1;
    }

    /* Filling peripheral registers. */
    if (qspi_prepare(&_qspi,
                     memspec->addr_mode_get(),
                     memspec->read_mode_get(),
                     memspec->write_mode_get()))
    {
        return -1;
    }

    /* Filling runtime variables. */
    flash_size = memspec->flash_size_get();
    page_size = memspec->page_size_get();
    sector_size = memspec->sector_size_get();

    _mutex->lock();
    if (qspi_start(&_qspi)) {
        _mutex->unlock();
        return -1;
    }

    /* Read status registers. They can be used in near future. */
    if (qspi_send_instruction(&_qspi,
                              QSPI_STD_CMD_RDSR, // command to send
                              0,                 // do not transmit
                              NULL,              // do not transmit
                              2,                 // just receive two bytes of data
                              status_value))     // store received values in status_value
    {
        _mutex->unlock();
        return -1;
    }
    _mutex->unlock();

    /* Activate quad mode if necessary.
     * Lot of code placed below is based on text placed here: https://chromium.googlesource.com/chromiumos/platform/ec/+/master/include/sfdp.h#613
     * Bits are ordered from zero. Bit0 -> first position in byte. Bit1 -> second position. */

    if ((memspec->read_mode_get() != QSPI_READMODE_1_1_1) && (memspec->write_mode_get() != QSPI_WRITEMODE_1_1_1))
    {
        switch(memspec->quad_mode_req_get())
        {
            case QSPIMemSpec::MEMSPEC_QUADMODREQ_NO_REQ:
                break;
            case QSPIMemSpec::MEMSPEC_QUADMODREQ_1BIT_2SREG_CLR2B:
                status_value[1] |= 0x02;
                // Send both bytes with WRSR.

                cmd_to_send = QSPI_STD_CMD_WRSR; // command to send
                data_len    = 2;                 // transmit 2 bytes of data
                p_data      = status_value;      // source of data

                break;
            case QSPIMemSpec::MEMSPEC_QUADMODREQ_6BIT_1SREG:
                status_value[0] |= 0x40;
                // Send only one byte with WRSR. Could be enough.

                cmd_to_send = QSPI_STD_CMD_WRSR; // command to send
                data_len    = 1;                 // transmit 1 byte of data
                p_data      = status_value;      // source of data

                break;
            case QSPIMemSpec::MEMSPEC_QUADMODREQ_7BIT_2SREG_W3EH_R3FH:
                status_value[1] |= 0x80;
                // Send only second byte with WRCR command.

                cmd_to_send = QSPI_STD_CMD_WRCR; // command to send
                data_len    = 1;                 // transmit 1 byte of data
                p_data      = &status_value[1];  // source of data

                break;
            case QSPIMemSpec::MEMSPEC_QUADMODREQ_1BIT_2SREG_CLR1B:
                status_value[1] |= 0x02;
                // Send both bytes with WRSR. Probably can be merged with MEMSPEC_QUADMODREQ_1BIT_2SREG_CLR2B case.

                cmd_to_send = QSPI_STD_CMD_WRSR; // command to send
                data_len    = 2;                 // transmit 2 bytes of data
                p_data      = status_value;      // source of data

                break;
            case QSPIMemSpec::MEMSPEC_QUADMODREQ_1BIT_2SREG_R35H_W01H_W2B:
                status_value[1] |= 0x02;
                // Send both bytes with WRSR. Probably can be merged with MEMSPEC_QUADMODREQ_1BIT_2SREG_CLR2B case.

                cmd_to_send = QSPI_STD_CMD_WRSR; // command to send
                data_len    = 2;                 // transmit 1 byte of data
                p_data      = status_value;      // source of data

                break;
            default:
                qspi_free(&_qspi);
                ret = -1;
        }

        /* If there is setting to send (with memory configuration) just send it. */
        if (data_len > 0)
        {
            _mutex->lock();
            ret = qspi_send_instruction(&_qspi,
                                        cmd_to_send, // command to send
                                        data_len,    // transmit 2 bytes of data
                                        p_data,      // source of data
                                        0,           // do not receive data
                                        NULL);       // do not receive data
            _mutex->unlock();
        }
    }
    return ret;
}

int QSPI::deinit()
{
    int ret = 0;
    _mutex->lock();
    if (qspi_free(&_qspi)) {
        ret = -1;
    }
    _mutex->unlock();
    return ret;
}


int QSPI::read(void *buffer, uint32_t addr, uint32_t size)
{
    _mutex->lock();
    int result = qspi_page_read(&_qspi, addr, (uint8_t *)buffer, size);
    _mutex->unlock();
    return result;
}

int QSPI::program(const void *buffer, uint32_t addr, uint32_t size)
{
    // addr and size should be aligned to page size, and multiple of page size
    // page program should not cross sector boundaries
    if (!is_aligned(addr, page_size) ||
        !is_aligned(size, page_size) ||
        (size < page_size) ||
        (((addr % sector_size) + size) > sector_size)) {
        return -1;
    }

    int ret = 0;
    _mutex->lock();
    if (qspi_program_page(&_qspi, addr, (const uint8_t *)buffer, size)) {
        ret = -1;
    }
    _mutex->unlock();
    return ret;
}

bool QSPI::is_aligned_to_sector(uint32_t addr, uint32_t size)
{
    if (!is_aligned(size, sector_size) ||
        !is_aligned(addr, sector_size)) {
        return false;
    } else {
        return true;
    }
}

int QSPI::erase(uint32_t addr, uint32_t size)
{
    if (!is_aligned_to_sector(addr, size)) {
        return -1;
    }

    int32_t ret = 0;
    _mutex->lock();
    while (size) {
        ret = qspi_erase_block(&_qspi, addr);
        if (ret != 0) {
            ret = -1;
            break;
        }
        while (qspi_is_busy(&_qspi) != 0);
        if (!is_aligned_to_sector(addr, size)) {
            ret = -1;
            break;
        }
        size -= sector_size;
        addr += sector_size;
    }
    _mutex->unlock();
    return ret;
}

int QSPI::is_memory_busy(void)
{
    uint8_t status_value = 0;
    int32_t ret = 0;
    if (_qspi.is_busy == 1)
    {
        return -1;
    }
    _mutex->lock();
    ret = qspi_send_instruction(&_qspi,
                                QSPI_STD_CMD_RDSR, // command to send
                                0,                 // do not transmit
                                NULL,              // do not transmit
                                1,                 // receive just one byte of data
                                &status_value);    // store received values in status_value
    _mutex->unlock();
    if (ret == 0)
    {
        if ((status_value & 0x01) != 0x00) // WIP bit is always first bit in status byte. 0 means ready, 1 means busy
        {
            return -1;
        }
    }

    return ret;
}

uint32_t QSPI::get_program_size() const
{
    return page_size;
}

uint32_t QSPI::get_read_size() const
{
    return 1;
}

uint32_t QSPI::get_erase_size() const
{
    return sector_size;
}

uint32_t QSPI::get_flash_size() const
{
    return flash_size;
}

#if DEVICE_QSPI_ASYNCH

void QSPI::irq_handler_asynch(void)
{
    int event = qspi_irq_handler_asynch(&_qspi);
    if (_callback && (event & QSPI_EVENT_COMPLETE)) {
        _callback.call(event & QSPI_EVENT_COMPLETE);
    }
}

int QSPI::start_read(void *buffer, uint32_t addr, uint32_t size, const event_callback_t& callback, int event)
{
    _mutex->lock();
    _callback = callback;
    _irq.callback(&QSPI::irq_handler_asynch);
    return qspi_start_page_read(&_qspi, addr, (uint8_t *)buffer, size, _irq.entry(), event , _usage);
}

int QSPI::start_program(const void *buffer, uint32_t addr, uint32_t size, const event_callback_t& callback, int event)
{
    _mutex->lock();
    _callback = callback;
    _irq.callback(&QSPI::irq_handler_asynch);
    return qspi_start_program_page(&_qspi, addr, (const uint8_t *)buffer, size, _irq.entry(), event , _usage);
}

int QSPI::start_erase(uint32_t addr, uint32_t size, const event_callback_t& callback, int event)
{
    /* Current implementation support only erasing of one sector with address aligned to this sector.
     * Erasing more sectors could be done on higher software layers using interrupts and testing
     * busy bit of memory device. */
    if (size > sector_size)
    {
        return -1;
    }
    _mutex->lock();
    _callback = callback;
    _irq.callback(&QSPI::irq_handler_asynch);
    return qspi_start_erase_block(&_qspi, addr, _irq.entry(), event , _usage);
}

#endif

}
#endif
