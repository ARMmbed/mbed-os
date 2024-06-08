/* mbed Microcontroller Library
 * Copyright (c) 2023 ARM Limited
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
#ifndef MBED_SECUREFLASH_BLOCK_DEVICE_H
#define MBED_SECUREFLASH_BLOCK_DEVICE_H

#include "platform/SingletonPtr.h"
#include "drivers/SPI.h"
#include "drivers/DigitalOut.h"
#include "blockdevice/internal/SFDP.h"
#include "blockdevice/BlockDevice.h"

#include "../../TG424_3/vendor_impl/vendor_secureflash_defs.h"

#ifndef MBED_CONF_SECUREF_DRIVER_SPI_MOSI
#define MBED_CONF_SECUREF_DRIVER_SPI_MOSI NC
#endif
#ifndef MBED_CONF_SECUREF_DRIVER_SPI_MISO
#define MBED_CONF_SECUREF_DRIVER_SPI_MISO NC
#endif
#ifndef MBED_CONF_SECUREF_DRIVER_SPI_CLK
#define MBED_CONF_SECUREF_DRIVER_SPI_CLK NC
#endif
#ifndef MBED_CONF_SECUREF_DRIVER_SPI_CS
#define MBED_CONF_SECUREF_DRIVER_SPI_CS NC
#endif
#ifndef MBED_CONF_SECUREF_DRIVER_SPI_FREQ
#define MBED_CONF_SECUREF_DRIVER_SPI_FREQ 40000000
#endif

#define APP_INFO_MAX_NUM        (0x10)

/** Enum spif standard error codes
 *
 *  @enum spif_bd_error
 */
enum securef_bd_error {
    SECUREF_BD_ERROR_OK                    = 0,     /*!< no error */
    SECUREF_BD_ERROR_DEVICE_ERROR          = mbed::BD_ERROR_DEVICE_ERROR, /*!< device specific error -4001 */
    SECUREF_BD_ERROR_PARSING_FAILED        = -4002, /* SFDP Parsing failed */
    SECUREF_BD_ERROR_READY_FAILED          = -4003, /* Wait for Memory Ready failed */
    SECUREF_BD_ERROR_WREN_FAILED           = -4004, /* Write Enable Failed */
    SECUREF_BD_ERROR_INVALID_ERASE_PARAMS  = -4005, /* Erase command not on sector aligned addresses or exceeds device size */
    SECUREF_BD_ERROR_DEVICE_UNSUPPORT      = -4006, /* Device id match failed  */
    SECUREF_BD_ERROR_PROVISION_FLASH       = -4007, /* Provision secure flash failed */
    SECUREF_BD_ERROR_GET_PROVISION         = -4007, /* Get the information of provision failed */
    SECUREF_BD_ERROR_ILLEGAL_ACCESS        = -4008, /* Operation illegal */
    SECUREF_BD_ERROR_CREATE_SESSION        = -4009, /* Create sessopm failed */
    SECUREF_BD_ERROR_TERMINATE_SESSION     = -4010, /* Terminate sessopm failed */
    SECUREF_BD_ERROR_SECURE_READ           = -4011, /* Secure read failed */
    SECUREF_BD_ERROR_SECURE_PROGRAM        = -4012, /* Secure program failed */
    SECUREF_BD_ERROR_SECURE_ERASE          = -4013, /* Secure erase failed */
    SECUREF_BD_ERROR_INVALID_PGM_PARAMS    = -4014, /* Program command invalid arguments */
    SECUREF_BD_ERROR_INVALID_READ_PARAMS   = -4015, /* Read command invalid arguments */  
};

/**
 * app_data_t
 * 
 * Structure to store the pre-provisioned application & secure zone binding information.
 */
typedef struct { 
    int32_t app_id;     /*!< The id of applications */
    uint32_t key_id;    /*!< The id of crypto root keys */
    uint32_t zone_id:8, /*!< The id of security zone id */
             mc_id:8,   /*!< The id of monotonic counter */
             reserved:16;
} app_data_t;

/**
 * app_info_t
 * 
 * Structure to store the pre-provisioned application information.
 */
typedef struct { 
    uint8_t id;                            /*!< The id of app info */
    uint8_t num;                           /*!< The number of app_data items */
    app_data_t app_data[APP_INFO_MAX_NUM]; /*!< The detailed app_data */
} app_info_t;      

/**
 * session_info_t
 * 
 * Structure to store secure Flash session information.
 */
typedef struct{
    uint32_t key_id;         /*!< Root key id */
    uint32_t session_key_id; /*!< Session key id */
    uint32_t session_id;     /*!< Session id */
} session_info_t;

/**
 * secureflash_t
 * 
 * Structure indicating secure Flash API layer informations
 */
typedef struct {    
    uint32_t _init_ref_count;       /*!< The initialization count of secure Flash */
    bool _is_initialized;           /*!< Secure Flash initialization status */
    app_info_t app_info;            /*!< The pre-provisioned application information of secure Flash */
    secure_flash_info_t flash_info; /*!< The specific secure Flash information */        
} secureflash_t;

class SecureFBlockDevice : public mbed::BlockDevice {
public:
    /** Creates a SecureFBlockDevice on a SPI bus specified by pins
     *
     *  @param mosi     SPI master out, slave in pin
     *  @param miso     SPI master in, slave out pin
     *  @param sclk     SPI clock pin
     *  @param csel     SPI chip select pin
     *  @param freq     Clock speed of the SPI bus (defaults to 40MHz)
     *
     *
     */
    SecureFBlockDevice(PinName mosi = MBED_CONF_SECUREF_DRIVER_SPI_MOSI,
                       PinName miso = MBED_CONF_SECUREF_DRIVER_SPI_MISO,
                       PinName sclk = MBED_CONF_SECUREF_DRIVER_SPI_CLK,
                       PinName csel = MBED_CONF_SECUREF_DRIVER_SPI_CS,
                           int freq = MBED_CONF_SECUREF_DRIVER_SPI_FREQ);

    /** Initialize a block device
     *
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  SPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  SPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         SPIF_BD_ERROR_OK(0) - success
     */
    virtual int deinit();

    /** Desctruct SPIFBlockDevice
      */
    ~SecureFBlockDevice()
    {
        deinit();
    }

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  @note The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  SPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  SPIF_BD_ERROR_WREN_FAILED - Write Enable failed
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Erase blocks on a block device
     *
     *  @note The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         SPIF_BD_ERROR_OK(0) - success
     *                  SPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  SPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  SPIF_BD_ERROR_INVALID_ERASE_PARAMS - Trying to erase unaligned address or size
     */
    virtual int erase(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */

    virtual int secure_read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size, int app_id);    
    virtual int secure_program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size, int app_id);
    virtual int secure_erase(mbed::bd_addr_t addr, mbed::bd_size_t size, int app_id);
    virtual bd_size_t secure_zone_number() const;    
    virtual bd_size_t secure_zone_size() const;
    
    virtual mbed::bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a programable block in bytes
     *  @note Must be a multiple of the read size
     */
    virtual mbed::bd_size_t get_program_size() const;

    /** Get the size of an erasable block
     *
     *  @return         Size of an erasable block in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size() const;

    /** Get the size of minimal erasable sector size of given address
     *
     *  @param addr     Any address within block queried for erase sector size (can be any address within flash size offset)
     *  @return         Size of minimal erase sector size, in given address region, in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size(mbed::bd_addr_t addr) const;

    /** Get the value of storage byte after it was erased
     *
     *  If get_erase_value returns a non-negative byte value, the underlying
     *  storage is set to that value when erased, and storage containing
     *  that value can be programmed without another erase.
     *
     *  @return         The value of storage when erased, or -1 if you can't
     *                  rely on the value of erased storage
     */
    virtual int get_erase_value() const;

    /** Get the total size of the underlying device
     *
     *  @return         Size of the underlying device in bytes
     */
    virtual mbed::bd_size_t size() const;

    /** Get the BlockDevice class type.
     *
     *  @return         A string representation of the BlockDevice class type.
     */
    virtual const char *get_type() const;

private:    
    /********************************/
    /*   Calls to SPI Driver APIs   */
    /********************************/

    // Send set_frequency command to Driver
    securef_bd_error _spi_set_frequency(int freq);
    /********************************/

    // Soft Reset Flash Memory
    int _reset_flash_mem();

    // Configure Write Enable in Status Register
    int _set_write_enable();

    // Wait on status register until write not-in-progress
    bool _is_mem_ready();

    // Query vendor ID and handle special behavior that isn't covered by SFDP data
    int _handle_vendor_quirks();

    // Probe secure flash
    int _probe();

    // Find the flash_info
    int _find_flash_info(uint8_t *id);   

    // Get the information of provision
    int _secureflash_get_app_info();

    // SPI protocol read
    static int _spi_read(uint8_t *tx_buf, uint32_t tx_len, uint8_t *rx_buf, uint32_t rx_len);

    // SPI protocol program
    static int _spi_write(uint8_t *tx_buf, uint32_t tx_len);

    app_data_t *_query_app_info(mbed::bd_addr_t addr, int app_id);    

private:
    // Master side hardware
    static mbed::SPI *_spi;

    PinName _mosi;
    PinName _miso;
    PinName _sclk;
    PinName _csel;
    int _freq;

    // Mutex is used to protect Flash device for some SPI Driver commands that must be done sequentially with no other commands in between
    // e.g. (1)Set Write Enable, (2)Program, (3)Wait Memory Ready
    static SingletonPtr<PlatformMutex> _mutex;        

    // Bus configuration    
    uint32_t _init_ref_count;
    bool _is_initialized;

    // Struct for secure flash information
    secureflash_t _secureflash;
};

#endif  /* MBED_SECUREFLASH_BLOCK_DEVICE_H */
