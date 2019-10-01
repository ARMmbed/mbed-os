/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#ifndef MBED_QSPIF_BLOCK_DEVICE_H
#define MBED_QSPIF_BLOCK_DEVICE_H

#include "QSPI.h"
#include "BlockDevice.h"

/** Enum qspif standard error codes
 *
 *  @enum qspif_bd_error
 */
enum qspif_bd_error {
    QSPIF_BD_ERROR_OK                    = 0,     /*!< no error */
    QSPIF_BD_ERROR_DEVICE_ERROR          = BD_ERROR_DEVICE_ERROR, /*!< device specific error -4001 */
    QSPIF_BD_ERROR_PARSING_FAILED        = -4002, /* SFDP Parsing failed */
    QSPIF_BD_ERROR_READY_FAILED          = -4003, /* Wait for  Mem Ready failed */
    QSPIF_BD_ERROR_WREN_FAILED           = -4004, /* Write Enable Failed */
    QSPIF_BD_ERROR_INVALID_ERASE_PARAMS  = -4005, /* Erase command not on sector aligned addresses or exceeds device size */
    QSPIF_BD_ERROR_DEVICE_NOT_UNIQE      = -4006, /* Only one instance per csel is allowed */
    QSPIF_BD_ERROR_DEVICE_MAX_EXCEED     = -4007 /* Max active QSPIF devices exceeded */
};

/** Enum qspif polarity mode
 *
 *  @enum qspif_polarity_mode
 */
enum qspif_polarity_mode {
    QSPIF_POLARITY_MODE_0 = 0, /* CPOL=0, CPHA=0 */
    QSPIF_POLARITY_MODE_1      /* CPOL=1, CPHA=1 */
};

#define QSPIF_MAX_REGIONS   10
#define MAX_NUM_OF_ERASE_TYPES 4
#define QSPIF_MAX_ACTIVE_FLASH_DEVICES 10

/** BlockDevice for SFDP based flash devices over QSPI bus
 *
 *  @code
 *  // Here's an example using QSPI flash device on DISCO_L476VG target
 *  #include "mbed.h"
 *  #include "QSPIFBlockDevice.h"
 *
 *  QSPIFBlockDevice block_device(QSPI_FLASH1_IO0, QSPI_FLASH1_IO1, QSPI_FLASH1_IO2, QSPI_FLASH1_IO3,
 *                                QSPI_FLASH1_SCK, QSPI_FLASH1_CSN, QSPIF_POLARITY_MODE_0, MBED_CONF_QSPIF_QSPI_FREQ);
 *
 *  int main()
 *  {
 *      printf("QSPI SFDP Flash Block Device example\n");
 *
 *      // Initialize the SPI flash device and print the memory layout
 *      block_device.init();
 *      bd_size_t sector_size_at_address_0 = block_device.get_erase_size(0);
 *
 *      printf("QSPIF BD size: %llu\n",         block_device.size());
 *      printf("QSPIF BD read size: %llu\n",    block_device.get_read_size());
 *      printf("QSPIF BD program size: %llu\n", block_device.get_program_size());
 *      printf("QSPIF BD erase size (at address 0): %llu\n", sector_size_at_address_0);
 *
 *      // Write "Hello World!" to the first block
 *      char *buffer = (char *) malloc(sector_size_at_address_0);
 *      sprintf(buffer, "Hello World!\n");
 *      block_device.erase(0, sector_size_at_address_0);
 *      block_device.program(buffer, 0, sector_size_at_address_0);
 *
 *      // Read back what was stored
 *      block_device.read(buffer, 0, sector_size_at_address_0);
 *      printf("%s", buffer);
 *
 *      // Deinitialize the device
 *      block_device.deinit();
 *  }
 *  @endcode
 */
class QSPIFBlockDevice : public mbed::BlockDevice {
public:
    /** Create QSPIFBlockDevice - An SFDP based Flash Block Device over QSPI bus
     *
     *  @param io0 1st IO pin used for sending/receiving data during data phase of a transaction
     *  @param io1 2nd IO pin used for sending/receiving data during data phase of a transaction
     *  @param io2 3rd IO pin used for sending/receiving data during data phase of a transaction
     *  @param io3 4th IO pin used for sending/receiving data during data phase of a transaction
     *  @param sclk QSPI Clock pin
     *  @param csel QSPI chip select pin
     *  @param clock_mode specifies the QSPI Clock Polarity mode (QSPIF_POLARITY_MODE_0/QSPIF_POLARITY_MODE_1)
     *         default value = 0
     *  @param freq Clock frequency of the QSPI bus (defaults to 40MHz)
     *
     */
    QSPIFBlockDevice(PinName io0, PinName io1, PinName io2, PinName io3, PinName sclk, PinName csel,
                     int clock_mode, int freq = MBED_CONF_QSPIF_QSPI_FREQ);

    /** Initialize a block device
     *
     *  @return         QSPIF_BD_ERROR_OK(0) - success
     *                  QSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  QSPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timedout
     *                  QSPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         QSPIF_BD_ERROR_OK(0) - success
     *                  QSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int deinit();

    /** Desctruct QSPIFBlockDevie
      */
    ~QSPIFBlockDevice()
    {
        deinit();
    }

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         QSPIF_BD_ERROR_OK(0) - success
     *                  QSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         QSPIF_BD_ERROR_OK(0) - success
     *                  QSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  QSPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  QSPIF_BD_ERROR_WREN_FAILED - Write Enable failed
     *                  QSPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         QSPIF_BD_ERROR_OK(0) - success
     *                  QSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  QSPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  QSPIF_BD_ERROR_WREN_FAILED - Write Enable failed
     *                  QSPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     *                  QSPIF_BD_ERROR_INVALID_ERASE_PARAMS - Trying to erase unaligned address or size
     */
    virtual int erase(mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Get the size of a readable block
     *
     *  @return         Size of a readable block in bytes
     */
    virtual mbed::bd_size_t get_read_size() const;

    /** Get the size of a programable block
     *
     *  @return         Size of a program block size in bytes
     *  @note Must be a multiple of the read size
     */
    virtual mbed::bd_size_t get_program_size() const;

    /** Get the size of a eraseable block
     *
     *  @return         Size of a minimal erase block, common to all regions, in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size() const;

    /** Get the size of minimal eraseable sector size of given address
     *
     *  @param addr     Any address within block queried for erase sector size (can be any address within flash size offset)
     *  @return         Size of minimal erase sector size, in given address region, in bytes
     *  @note Must be a multiple of the program size
     */
    virtual mbed::bd_size_t get_erase_size(mbed::bd_addr_t addr);

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
     *  @return         A string represent the BlockDevice class type.
     */
    virtual const char *get_type() const;

private:
    // Internal functions


    /********************************/
    /*   Different Device Csel Mgmt */
    /********************************/
    // Add a new QSPI device CS to existing devices list.
    // Only one QSPIFBlockDevice instance per CS is allowed
    int add_new_csel_instance(PinName csel);

    // Remove device CS from existing device list upon destroying object (last deinit is called)
    int remove_csel_instance(PinName csel);

    /********************************/
    /*   Calls to QSPI Driver APIs  */
    /********************************/
    // Send Program => Write command to Driver
    qspi_status_t _qspi_send_program_command(unsigned int prog_instruction, const void *buffer, mbed::bd_addr_t addr,
                                             mbed::bd_size_t *size);

    // Send Read command to Driver
    qspi_status_t _qspi_send_read_command(unsigned int read_instruction, void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    // Send Erase Instruction using command_transfer command to Driver
    qspi_status_t _qspi_send_erase_command(unsigned int erase_instruction, mbed::bd_addr_t addr, mbed::bd_size_t size);

    // Send Generic command_transfer command to Driver
    qspi_status_t _qspi_send_general_command(unsigned int instruction_int, mbed::bd_addr_t addr, const char *tx_buffer,
                                             size_t tx_length, const char *rx_buffer, size_t rx_length);

    // Send Bus configure_format command to Driver
    qspi_status_t _qspi_configure_format(qspi_bus_width_t inst_width, qspi_bus_width_t address_width,
                                         qspi_address_size_t address_size, qspi_bus_width_t alt_width, qspi_alt_size_t alt_size, qspi_bus_width_t data_width,
                                         int dummy_cycles);

    // Send set_frequency command to Driver
    qspi_status_t _qspi_set_frequency(int freq);

    /*********************************/
    /* Flash Configuration Functions */
    /*********************************/
    // Soft Reset Flash Memory
    int _reset_flash_mem();

    // Configure Write Enable in Status Register
    int _set_write_enable();

    // Wait on status register until write not-in-progress
    bool _is_mem_ready();

    // Enable Fast Mode - for flash chips with low power default
    int _enable_fast_mdoe();

    /****************************************/
    /* SFDP Detection and Parsing Functions */
    /****************************************/
    // Parse SFDP Headers and retrieve Basic Param and Sector Map Tables (if exist)
    int _sfdp_parse_sfdp_headers(uint32_t &basic_table_addr, size_t &basic_table_size,
                                 uint32_t &sector_map_table_addr, size_t &sector_map_table_size);

    // Parse and Detect required Basic Parameters from Table
    int _sfdp_parse_basic_param_table(uint32_t basic_table_addr, size_t basic_table_size);

    // Parse and read information required by Regions Secotr Map
    int _sfdp_parse_sector_map_table(uint32_t sector_map_table_addr, size_t sector_map_table_size);

    // Detect fastest read Bus mode supported by device
    int _sfdp_detect_best_bus_read_mode(uint8_t *basic_param_table_ptr, int basic_param_table_size, bool &set_quad_enable,
                                        bool &is_qpi_mode, unsigned int &read_inst);

    // Enable Quad mode if supported (1-1-4, 1-4-4, 4-4-4 bus modes)
    int _sfdp_set_quad_enabled(uint8_t *basic_param_table_ptr);

    // Enable QPI mode (4-4-4) is supported
    int _sfdp_set_qpi_enabled(uint8_t *basic_param_table_ptr);

    // Set Page size for program
    int _sfdp_detect_page_size(uint8_t *basic_param_table_ptr, int basic_param_table_size);

    // Detect all supported erase types
    int _sfdp_detect_erase_types_inst_and_size(uint8_t *basic_param_table_ptr, int basic_param_table_size,
                                               unsigned int &erase4k_inst,
                                               unsigned int *erase_type_inst_arr, unsigned int *erase_type_size_arr);

    /***********************/
    /* Utilities Functions */
    /***********************/
    // Find the region to which the given offset belong to
    int _utils_find_addr_region(mbed::bd_size_t offset);

    // Iterate on all supported Erase Types of the Region to which the offset belong to.
    // Iterates from highest type to lowest
    int _utils_iterate_next_largest_erase_type(uint8_t &bitfield, int size, int offset, int boundry);

private:
    // QSPI Driver Object
    mbed::QSPI _qspi;

    // Static List of different QSPI based Flash devices csel that already exist
    // Each QSPI Flash device csel can have only 1 QSPIFBlockDevice instance
    // _devices_mutex is used to lock csel list - only one QSPIFBlockDevice instance per csel is allowed
    static SingletonPtr<PlatformMutex> _devices_mutex;
    static int _number_of_active_qspif_flash_csel;
    static PinName *_active_qspif_flash_csel_arr;

    int _unique_device_status;
    PinName _csel;

    // Mutex is used to protect Flash device for some QSPI Driver commands that must be done sequentially with no other commands in between
    // e.g. (1)Set Write Enable, (2)Program, (3)Wait Memory Ready
    PlatformMutex _mutex;

    // Command Instructions
    unsigned int _read_instruction;
    unsigned int _prog_instruction;
    unsigned int _erase_instruction;
    unsigned int _erase4k_inst;  // Legacy 4K erase instruction (default 0x20h)
    unsigned int _write_register_inst; // Write status/config register instruction may vary between chips
    unsigned int _read_register_inst; // Read status/config register instruction may vary between chips

    // Up To 4 Erase Types are supported by SFDP (each with its own command Instruction and Size)
    unsigned int _erase_type_inst_arr[MAX_NUM_OF_ERASE_TYPES];
    unsigned int _erase_type_size_arr[MAX_NUM_OF_ERASE_TYPES];

    // Sector Regions Map
    int _regions_count; //number of regions
    int _region_size_bytes[QSPIF_MAX_REGIONS]; //regions size in bytes
    bd_size_t _region_high_boundary[QSPIF_MAX_REGIONS]; //region high address offset boundary
    //Each Region can support a bit combination of any of the 4 Erase Types
    uint8_t _region_erase_types_bitfield[QSPIF_MAX_REGIONS];
    unsigned int _min_common_erase_size; // minimal common erase size for all regions (0 if none exists)

    unsigned int _page_size_bytes; // Page size - 256 Bytes default
    int _freq;
    bd_size_t _device_size_bytes;

    // Bus speed configuration
    qspi_bus_width_t _inst_width; //Bus width for Instruction phase
    qspi_bus_width_t _address_width; //Bus width for Address phase
    qspi_address_size_t _address_size; //Number of bits for address
    qspi_alt_size_t _alt_size; //Number of bits for alt
    bool _alt_enabled; //Whether alt is enabled
    uint8_t _dummy_cycles; //Number of Dummy cycles required by Current Bus Mode
    qspi_bus_width_t _data_width; //Bus width for Data phase

    uint32_t _init_ref_count;
    bool _is_initialized;
};

#endif
