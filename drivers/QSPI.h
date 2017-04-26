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
#ifndef MBED_QSPI_H
#define MBED_QSPI_H

#ifdef DEVICE_QSPI

#include "qspi_api.h"
#include "QSPIMemSpec.h"
#include "platform/SingletonPtr.h"
#include "platform/PlatformMutex.h"

#if DEVICE_QSPI_ASYNCH
#include "platform/CThunk.h"
#include "hal/dma_api.h"
#include "platform/FunctionPointer.h"
#include "platform/Transaction.h"
#endif

namespace mbed {

/** \addtogroup drivers */
/** @{*/

/** QSPI driver. It invokes flash HAL functions.
 *
 * Note Synchronization level: Thread safe
 */
class QSPI {
public:
    /** Create a QSPI connected to specified pins and set choosen frequency.
     *
     * In this case QSPI will be configured in SPI mode. There will be no possibilities to init
     * QSPI device and peripheral in quad mode.
     *
     * @param miso     QSPI Master In, Slave Out pin
     * @param mosi     QSPI Master Out, Slave In pin
     * @param sclk     QSPI Clock pin
     * @param ssel     QSPI chip select pin
     * @param hz       SCLK frequency in hz
     * @param spi_mode Clock polarity and phase mode (0 or 3)
     */
    QSPI(PinName miso, PinName mosi, PinName sclk, PinName ssel, int hz, int spi_mode = 0);

    /** Create a QSPI connected to specified pins and set choosen frequency.
     *
     * In this case QSPI will be prepared to start in quad moge. There will be possibilities to init
     * QSPI device and peripheral in quad mode.
     *
     * @param io0      QSPI IO0 pin
     * @param io1      QSPI IO1 pin
     * @param io2      QSPI IO2 pin
     * @param io3      QSPI IO3 pin
     * @param sclk     QSPI Clock pin
     * @param ssel     QSPI chip select pin
     * @param hz       SCLK frequency in hz
     * @param spi_mode Clock polarity and phase mode (0 or 3)
     */
    QSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName ssel, int hz, int spi_mode = 0);

    ~QSPI();

    /** Initialize a QSPI device
     *
     *  Should be called once per lifetime of the object.
     *  @return 0 on success or a negative error code on failure
     */
    int init();

    /** Initialize a QSPI device in most suitable mode.
     *
     *  Should be called once per lifetime of the object.
     *  @return 0 on success or a negative error code on failure
     */
    int init(QSPIMemSpec *memspec);

    /** Deinitialize QSPI device
     *
     *  @return 0 on success or a negative error code on failure
     */
    int deinit();

    /** Read data from a QSPI device.
     *
     *  This method invokes transmission between mcu and qspi memory device
     *
     *  @param buffer Buffer to write to
     *  @param addr   Flash address to begin reading from
     *  @param size   Size to read in bytes
     *  @return       0 on success, negative error code on failure
     */
    int read(void *buffer, uint32_t addr, uint32_t size);

    /** Program data to pages
     *
     *  The sectors must have been erased prior to being programmed (in most cases)
     *
     *  @param buffer Buffer of data to be written
     *  @param addr   Address of a page to begin writing to, must be a multiple of program and sector sizes
     *  @param size   Size to write in bytes, must be a multiple of program and sector sizes
     *  @return       0 on success, negative error code on failure
     */
    int program(const void *buffer, uint32_t addr, uint32_t size);

    /** Erase sectors
     *
     *  The state of an erased sector is undefined until it has been programmed
     *
     *  @param addr Address of a sector to begin erasing, must be a multiple of the sector size
     *  @param size Size to erase in bytes, must be a multiple of the sector size
     *  @return     0 on success, negative error code on failure
     */
    int erase(uint32_t addr, uint32_t size);

    /** Check busy bit
     *
     *  The state of an memory after program or erase command is undefined until busy bit has checked.
     *
     *  @return     0 on free, negative error code on busy
     */
    int is_memory_busy(void);

    /** Get the sector size, generic erase unit
     *
     *  @return Size of a sector in bytes, MBED_FLASH_INVALID_SIZE if not mapped or zero if erasing is not needed to write data.
     */
    uint32_t get_erase_size(void) const;

    /** Get the program unit size
     *
     * Minimum programmable unit is at most cases 256B, but sometimes passes other values. Addresses should be aligned to this value.
     *
     * @return Size of a minimum programmable unit.
     */
    uint32_t get_program_size(void) const;

    /** Get the flash size
     *
     *  @return Flash size
     */
    uint32_t get_flash_size(void) const;

    /** Get the minimum read size
     *
     *  @return Size of a minimum read unit in bytes
     */
    uint32_t get_read_size(void) const;

#if DEVICE_QSPI_ASYNCH
    /** Read data from a QSPI device.
     *
     *  This method invokes transmission between mcu and qspi memory device
     *
     *  @param buffer   Buffer to write to
     *  @param addr     Flash address to begin reading from
     *  @param size     Size to read in bytes
     *  @param event    The logical OR of events to modify
     *  @param callback The event callback function
     *  @return         0 on success, -1 if peripheral is busy
     */
    int start_read(void *buffer, uint32_t addr, uint32_t size, const event_callback_t& callback, int event);

    /** Program data to pages
     *
     *  The sectors must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to be written
     *  @param addr     Address of a page to begin writing to, must be a multiple of program and sector sizes
     *  @param size     Size to write in bytes, must be a multiple of program and sector sizes
     *  @param event    The logical OR of events to modify
     *  @param callback The event callback function
     *  @return         0 on success, -1 if peripheral is busy
     */
    int start_program(const void *buffer, uint32_t addr, uint32_t size, const event_callback_t& callback, int event);

    /** Erase sectors
     *
     *  The state of an erased sector is undefined until it has been programmed
     *
     *  @param addr     Address of a sector to begin erasing, must be a multiple of the sector size
     *  @param size     Size to erase in bytes, must be a multiple of the sector size
     *  @param event    The logical OR of events to modify
     *  @param callback The event callback function
     *  @return         0 on success, -1 if peripheral is busy
     */
    int start_erase(uint32_t addr, uint32_t size, const event_callback_t& callback, int event);
#endif

protected:
#if DEVICE_QSPI_ASYNCH
    /** QSPI IRQ handler */
    void irq_handler_asynch(void);
    CThunk<QSPI> _irq;
    event_callback_t _callback;
    DMAUsage _usage;
#endif
private:

    /** Check if address and size are aligned to a sector
     *
     *  @param addr Address of block to check for alignment
     *  @param size Size of block to check for alignment
     *  @return true if the block is sector aligned, false otherwise
     */
    bool is_aligned_to_sector(uint32_t addr, uint32_t size);

    bool quad_mode_hw_support;
    qspi_t _qspi;
    memspec_size_t flash_size;
    uint32_t page_size;
    uint32_t sector_size;

    static SingletonPtr<PlatformMutex> _mutex;
};

} /* namespace mbed */

#endif  /* DEVICE_FLASH */

#endif  /* MBED_QSPI_H */

/** @}*/
