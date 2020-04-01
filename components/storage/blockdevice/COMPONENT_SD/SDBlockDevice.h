/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#ifndef MBED_SD_BLOCK_DEVICE_H
#define MBED_SD_BLOCK_DEVICE_H

/* If the target has no SPI support, then SD Card is not supported. */
#if DEVICE_SPI

#include "features/storage/blockdevice/BlockDevice.h"
#include "drivers/SPI.h"
#include "drivers/Timer.h"
#include "drivers/MbedCRC.h"
#include "drivers/DigitalOut.h"
#include "platform/platform.h"
#include "platform/PlatformMutex.h"
#include "hal/static_pinmap.h"

#ifndef MBED_CONF_SD_SPI_MOSI
#define MBED_CONF_SD_SPI_MOSI NC
#endif
#ifndef MBED_CONF_SD_SPI_MISO
#define MBED_CONF_SD_SPI_MISO NC
#endif
#ifndef MBED_CONF_SD_SPI_CLK
#define MBED_CONF_SD_SPI_CLK NC
#endif
#ifndef MBED_CONF_SD_SPI_CS
#define MBED_CONF_SD_SPI_CS NC
#endif
#ifndef MBED_CONF_SD_INIT_FREQUENCY
#define MBED_CONF_SD_INIT_FREQUENCY 100000
#endif
#ifndef MBED_CONF_SD_TRX_FREQUENCY
#define MBED_CONF_SD_TRX_FREQUENCY  1000000
#endif
#ifndef MBED_CONF_SD_CRC_ENABLED
#define MBED_CONF_SD_CRC_ENABLED 0
#endif

/** SDBlockDevice class
 *
 * Access an SD Card using SPI bus
 */
class SDBlockDevice : public mbed::BlockDevice {
public:
    /** Creates an SDBlockDevice on a SPI bus specified by pins (using dynamic pin-map)
     *
     *  @param mosi     SPI master out, slave in pin
     *  @param miso     SPI master in, slave out pin
     *  @param sclk     SPI clock pin
     *  @param cs       SPI chip select pin
     *  @param hz       Clock speed of the SPI bus (defaults to 1MHz)
     *  @param crc_on   Enable cyclic redundancy check (defaults to disabled)
     */
    SDBlockDevice(PinName mosi = MBED_CONF_SD_SPI_MOSI,
                  PinName miso = MBED_CONF_SD_SPI_MISO,
                  PinName sclk = MBED_CONF_SD_SPI_CLK,
                  PinName cs = MBED_CONF_SD_SPI_CS,
                  uint64_t hz = MBED_CONF_SD_TRX_FREQUENCY,
                  bool crc_on = MBED_CONF_SD_CRC_ENABLED);

    /** Creates an SDBlockDevice on a SPI bus specified by pins (using static pin-map)
     *
     *  @param spi_pinmap Static SPI pin-map
     *  @param hz         Clock speed of the SPI bus (defaults to 1MHz)
     *  @param crc_on     Enable cyclic redundancy check (defaults to disabled)
     */
    SDBlockDevice(const spi_pinmap_t &spi_pinmap,
                  PinName cs = MBED_CONF_SD_SPI_CS,
                  uint64_t hz = MBED_CONF_SD_TRX_FREQUENCY,
                  bool crc_on = MBED_CONF_SD_CRC_ENABLED);

    virtual ~SDBlockDevice();

    /** Initialize a block device
     *
     *  @return         BD_ERROR_OK(0) - success
     *                  BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  SD_BLOCK_DEVICE_ERROR_NO_DEVICE - device (SD card) is missing or not connected
     *                  SD_BLOCK_DEVICE_ERROR_UNUSABLE - unusable card
     *                  SD_BLOCK_DEVICE_ERROR_CRC - crc error
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         BD_ERROR_OK(0) - success
     */
    virtual int deinit();

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         BD_ERROR_OK(0) - success
     *                  SD_BLOCK_DEVICE_ERROR_NO_DEVICE - device (SD card) is missing or not connected
     *                  SD_BLOCK_DEVICE_ERROR_CRC - crc error
     *                  SD_BLOCK_DEVICE_ERROR_PARAMETER - invalid parameter
     *                  SD_BLOCK_DEVICE_ERROR_NO_RESPONSE - no response from device
     *                  SD_BLOCK_DEVICE_ERROR_UNSUPPORTED - unsupported command
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  @note The blocks must be erased prior to programming
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes. Must be a multiple of program block size
     *  @return         BD_ERROR_OK(0) - success
     *                  SD_BLOCK_DEVICE_ERROR_NO_DEVICE - device (SD card) is missing or not connected
     *                  SD_BLOCK_DEVICE_ERROR_CRC - crc error
     *                  SD_BLOCK_DEVICE_ERROR_PARAMETER - invalid parameter
     *                  SD_BLOCK_DEVICE_ERROR_UNSUPPORTED - unsupported command
     *                  SD_BLOCK_DEVICE_ERROR_NO_INIT - device is not initialized
     *                  SD_BLOCK_DEVICE_ERROR_WRITE - SPI write error
     *                  SD_BLOCK_DEVICE_ERROR_ERASE - erase error
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Mark blocks as no longer in use
     *
     *  This function provides a hint to the underlying block device that a region of blocks
     *  is no longer in use and may be erased without side effects. Erase must still be called
     *  before programming, but trimming allows flash-translation-layers to schedule erases when
     *  the device is not busy.
     *
     *  @param addr     Address of block to mark as unused
     *  @param size     Size to mark as unused in bytes, must be a multiple of erase block size
     *  @return         BD_ERROR_OK(0) - success
     *                  SD_BLOCK_DEVICE_ERROR_NO_DEVICE - device (SD card) is missing or not connected
     *                  SD_BLOCK_DEVICE_ERROR_CRC - crc error
     *                  SD_BLOCK_DEVICE_ERROR_PARAMETER - invalid parameter
     *                  SD_BLOCK_DEVICE_ERROR_UNSUPPORTED - unsupported command
     *                  SD_BLOCK_DEVICE_ERROR_NO_INIT - device is not initialized
     *                  SD_BLOCK_DEVICE_ERROR_ERASE - erase error
     */
    virtual int trim(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual mbed::bd_size_t get_read_size() const;

    /** Get the size of a programmable block
     *
     *  @return         Size of a programmable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual mbed::bd_size_t get_program_size() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual mbed::bd_size_t size() const;

    /** Enable or disable debugging
     *
     *  @param dbg        State of debugging
     */
    virtual void debug(bool dbg);

    /** Set the transfer frequency
     *
     *  @param freq     Transfer frequency
     *  @note Max frequency supported is 25MHZ
     */
    virtual int frequency(uint64_t freq);

    /** Get the BlockDevice class type.
     *
     *  @return         A string representation of the BlockDevice class type.
     */
    virtual const char *get_type() const;

private:
    /* Commands : Listed below are commands supported
     * in SPI mode for SD card : Only Mandatory ones
     */
    enum cmdSupported {
        CMD_NOT_SUPPORTED = -1,             /**< Command not supported error */
        CMD0_GO_IDLE_STATE = 0,             /**< Resets the SD Memory Card */
        CMD1_SEND_OP_COND = 1,              /**< Sends host capacity support */
        CMD6_SWITCH_FUNC = 6,               /**< Check and Switches card function */
        CMD8_SEND_IF_COND = 8,              /**< Supply voltage info */
        CMD9_SEND_CSD = 9,                  /**< Provides Card Specific data */
        CMD10_SEND_CID = 10,                /**< Provides Card Identification */
        CMD12_STOP_TRANSMISSION = 12,       /**< Forces the card to stop transmission */
        CMD13_SEND_STATUS = 13,             /**< Card responds with status */
        CMD16_SET_BLOCKLEN = 16,            /**< Length for SC card is set */
        CMD17_READ_SINGLE_BLOCK = 17,       /**< Read single block of data */
        CMD18_READ_MULTIPLE_BLOCK = 18,     /**< Card transfers data blocks to host until interrupted
                                                 by a STOP_TRANSMISSION command */
        CMD24_WRITE_BLOCK = 24,             /**< Write single block of data */
        CMD25_WRITE_MULTIPLE_BLOCK = 25,    /**< Continuously writes blocks of data until
                                                 'Stop Tran' token is sent */
        CMD27_PROGRAM_CSD = 27,             /**< Programming bits of CSD */
        CMD32_ERASE_WR_BLK_START_ADDR = 32, /**< Sets the address of the first write
                                                 block to be erased. */
        CMD33_ERASE_WR_BLK_END_ADDR = 33,   /**< Sets the address of the last write
                                                 block of the continuous range to be erased.*/
        CMD38_ERASE = 38,                   /**< Erases all previously selected write blocks */
        CMD55_APP_CMD = 55,                 /**< Extend to Applications specific commands */
        CMD56_GEN_CMD = 56,                 /**< General Purpose Command */
        CMD58_READ_OCR = 58,                /**< Read OCR register of card */
        CMD59_CRC_ON_OFF = 59,              /**< Turns the CRC option on or off*/
        // App Commands
        ACMD6_SET_BUS_WIDTH = 6,
        ACMD13_SD_STATUS = 13,
        ACMD22_SEND_NUM_WR_BLOCKS = 22,
        ACMD23_SET_WR_BLK_ERASE_COUNT = 23,
        ACMD41_SD_SEND_OP_COND = 41,
        ACMD42_SET_CLR_CARD_DETECT = 42,
        ACMD51_SEND_SCR = 51,
    };

    uint8_t _card_type;
    int _cmd(SDBlockDevice::cmdSupported cmd, uint32_t arg, bool isAcmd = 0, uint32_t *resp = NULL);
    int _cmd8();

    /*  Move the SD Card into the SPI Mode idle state
     *
     *  The card is transitioned from SD Card mode to SPI mode by sending the
     *  CMD0 (GO_IDLE_STATE) command with CS asserted. See the notes in the
     *  "SPI Startup" section of the comments at the head of the
     *  implementation file for further details and specification references.
     *
     *  @return         Response form the card. R1_IDLE_STATE (0x1), the successful
     *                  response from CMD0. R1_XXX_XXX for more response
     */
    uint32_t _go_idle_state();
    int _initialise_card();

    mbed::bd_size_t _sectors;
    mbed::bd_size_t _sd_sectors();

    bool _is_valid_trim(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /* SPI functions */
    mbed::Timer _spi_timer;               /**< Timer Class object used for busy wait */
    uint32_t _init_sck;             /**< Initial SPI frequency */
    uint32_t _transfer_sck;         /**< SPI frequency during data transfer/after initialization */
    mbed::SPI _spi;                       /**< SPI Class object */

    /* SPI initialization function */
    void _spi_init();
    uint8_t _cmd_spi(SDBlockDevice::cmdSupported cmd, uint32_t arg);
    void _spi_wait(uint8_t count);

    bool _wait_token(uint8_t token);        /**< Wait for token */
    bool _wait_ready(uint16_t ms = 300);    /**< 300ms default wait for card to be ready */
    int _read(uint8_t *buffer, uint32_t length);
    int _read_bytes(uint8_t *buffer, uint32_t length);
    uint8_t _write(const uint8_t *buffer, uint8_t token, uint32_t length);
    int _freq(void);
    void _preclock_then_select();
    void _postclock_then_deselect();

    virtual void lock()
    {
        _mutex.lock();
    }

    virtual void unlock()
    {
        _mutex.unlock();
    }

    PlatformMutex _mutex;
    static const uint32_t _block_size;
    uint32_t _erase_size;
    bool _is_initialized;
    bool _dbg;
    uint32_t _init_ref_count;

#if MBED_CONF_SD_CRC_ENABLED
    bool _crc_on;
#endif
};

#endif  /* DEVICE_SPI */

#endif  /* MBED_SD_BLOCK_DEVICE_H */
