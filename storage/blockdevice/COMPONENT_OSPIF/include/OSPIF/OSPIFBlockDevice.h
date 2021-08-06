/* mbed Microcontroller Library
 * Copyright (c) 2020 ARM Limited
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
#ifndef MBED_OSPIF_BLOCK_DEVICE_H
#define MBED_OSPIF_BLOCK_DEVICE_H

#include "drivers/OSPI.h"
#include "blockdevice/internal/SFDP.h"
#include "blockdevice/BlockDevice.h"
#include "platform/Callback.h"

#if defined(TARGET_MX25LM51245G)
#include "MX25LM51245G_config.h"
#endif

#if defined(TARGET_MX25LW51245G)
#include "MX25LW51245G_config.h"
#endif

#ifndef MBED_CONF_OSPIF_OSPI_IO0
#define MBED_CONF_OSPIF_OSPI_IO0 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_IO1
#define MBED_CONF_OSPIF_OSPI_IO1 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_IO2
#define MBED_CONF_OSPIF_OSPI_IO2 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_IO3
#define MBED_CONF_OSPIF_OSPI_IO3 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_IO4
#define MBED_CONF_OSPIF_OSPI_IO4 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_IO5
#define MBED_CONF_OSPIF_OSPI_IO5 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_IO6
#define MBED_CONF_OSPIF_OSPI_IO6 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_IO7
#define MBED_CONF_OSPIF_OSPI_IO7 NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_SCK
#define MBED_CONF_OSPIF_OSPI_SCK NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_CSN
#define MBED_CONF_OSPIF_OSPI_CSN NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_DQS
#define MBED_CONF_OSPIF_OSPI_DQS NC
#endif
#ifndef MBED_CONF_OSPIF_OSPI_POLARITY_MODE
#define MBED_CONF_OSPIF_OSPI_POLARITY_MODE 0
#endif
#ifndef MBED_CONF_OSPIF_OSPI_FREQ
#define MBED_CONF_OSPIF_OSPI_FREQ 40000000
#endif

/** Enum ospif standard error codes
 *
 *  @enum ospif_bd_error
 */
enum ospif_bd_error {
    OSPIF_BD_ERROR_OK                    = 0,     /*!< no error */
    OSPIF_BD_ERROR_DEVICE_ERROR          = mbed::BD_ERROR_DEVICE_ERROR, /*!< device specific error -4001 */
    OSPIF_BD_ERROR_PARSING_FAILED        = -4002, /* SFDP Parsing failed */
    OSPIF_BD_ERROR_READY_FAILED          = -4003, /* Wait for Mem Ready failed */
    OSPIF_BD_ERROR_WREN_FAILED           = -4004, /* Write Enable Failed */
    OSPIF_BD_ERROR_INVALID_ERASE_PARAMS  = -4005, /* Erase command not on sector aligned addresses or exceeds device size */
    OSPIF_BD_ERROR_DEVICE_NOT_UNIQUE     = -4006, /* Only one instance per csel is allowed */
    OSPIF_BD_ERROR_DEVICE_MAX_EXCEED     = -4007  /* Max active OSPIF devices exceeded */
};

enum _mode {
    SPI,
    SOPI,
    DOPI
};

/** Enum ospif polarity mode
 *
 *  @enum ospif_polarity_mode
 */
enum ospif_polarity_mode {
    OSPIF_POLARITY_MODE_0 = 0, /* CPOL=0, CPHA=0 */
    OSPIF_POLARITY_MODE_1      /* CPOL=1, CPHA=1 */
};

#define OSPIF_MAX_ACTIVE_FLASH_DEVICES 10

/** BlockDevice for SFDP based flash devices over OSPI bus
 *
 *  @code
 *  // Here's an example using OSPI flash device on NUCLEO_L4R9I target
 *  #include "mbed.h"
 *  #include "OSPIFBlockDevice.h"
 *
 *  OSPIFBlockDevice block_device(OSPI_FLASH1_IO0, OSPI_FLASH1_IO1, OSPI_FLASH1_IO2, OSPI_FLASH1_IO3, OSPI_FLASH1_IO4, OSPI_FLASH1_IO5, OSPI_FLASH1_IO6, OSPI_FLASH1_IO7
 *                                OSPI_FLASH1_SCK, OSPI_FLASH1_CSN, OSPI_FLASH1_DQS, OSPIF_POLARITY_MODE_0, MBED_CONF_OSPIF_OSPI_FREQ);
 *
 *  int main()
 *  {
 *      printf("OSPI SFDP Flash Block Device example\n");
 *
 *      // Initialize the OSPI flash device and print the memory layout
 *      block_device.init();
 *      bd_size_t sector_size_at_address_0 = block_device.get_erase_size(0);
 *
 *      printf("OSPIF BD size: %llu\n",         block_device.size());
 *      printf("OSPIF BD read size: %llu\n",    block_device.get_read_size());
 *      printf("OSPIF BD program size: %llu\n", block_device.get_program_size());
 *      printf("OSPIF BD erase size (at address 0): %llu\n", sector_size_at_address_0);
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
class OSPIFBlockDevice : public mbed::BlockDevice {
public:
    /** Create OSPIFBlockDevice - An SFDP based Flash Block Device over OSPI bus
     *
     *  @param io0 1st IO pin used for sending/receiving data during data phase of a transaction
     *  @param io1 2nd IO pin used for sending/receiving data during data phase of a transaction
     *  @param io2 3rd IO pin used for sending/receiving data during data phase of a transaction
     *  @param io3 4th IO pin used for sending/receiving data during data phase of a transaction
     *  @param io4 5th IO pin used for sending/receiving data during data phase of a transaction
     *  @param io5 6th IO pin used for sending/receiving data during data phase of a transaction
     *  @param io6 7th IO pin used for sending/receiving data during data phase of a transaction
     *  @param io7 8th IO pin used for sending/receiving data during data phase of a transaction
     *  @param sclk OSPI Clock pin
     *  @param csel OSPI chip select pin
     *  @param dqs OSPI dqs pin
     *  @param clock_mode specifies the OSPI Clock Polarity mode (OSPIF_POLARITY_MODE_0/OSPIF_POLARITY_MODE_1)
     *         default value = 0
     *  @param freq Clock frequency of the OSPI bus (defaults to 40MHz)
     */
    OSPIFBlockDevice(PinName io0 = MBED_CONF_OSPIF_OSPI_IO0,
                     PinName io1 = MBED_CONF_OSPIF_OSPI_IO1,
                     PinName io2 = MBED_CONF_OSPIF_OSPI_IO2,
                     PinName io3 = MBED_CONF_OSPIF_OSPI_IO3,
                     PinName io4 = MBED_CONF_OSPIF_OSPI_IO4,
                     PinName io5 = MBED_CONF_OSPIF_OSPI_IO5,
                     PinName io6 = MBED_CONF_OSPIF_OSPI_IO6,
                     PinName io7 = MBED_CONF_OSPIF_OSPI_IO7,
                     PinName sclk = MBED_CONF_OSPIF_OSPI_SCK,
                     PinName csel = MBED_CONF_OSPIF_OSPI_CSN,
                     PinName dqs = MBED_CONF_OSPIF_OSPI_DQS,
                     int clock_mode = MBED_CONF_OSPIF_OSPI_POLARITY_MODE,
                     int freq = MBED_CONF_OSPIF_OSPI_FREQ);

    /** Initialize a block device
     *
     *  @return         OSPIF_BD_ERROR_OK(0) - success
     *                  OSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  OSPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timedout
     *                  OSPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     */
    virtual int init();

    /** Deinitialize a block device
     *
     *  @return         OSPIF_BD_ERROR_OK(0) - success
     *                  OSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int deinit();

    /** Desctruct OSPIFBlockDevie
      */
    ~OSPIFBlockDevice()
    {
        deinit();
    }

    /** Read blocks from a block device
     *
     *  @param buffer   Buffer to write blocks to
     *  @param addr     Address of block to begin reading from
     *  @param size     Size to read in bytes, must be a multiple of read block size
     *  @return         OSPIF_BD_ERROR_OK(0) - success
     *                  OSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     */
    virtual int read(void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Program blocks to a block device
     *
     *  The blocks must have been erased prior to being programmed
     *
     *  @param buffer   Buffer of data to write to blocks
     *  @param addr     Address of block to begin writing to
     *  @param size     Size to write in bytes, must be a multiple of program block size
     *  @return         OSPIF_BD_ERROR_OK(0) - success
     *                  OSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  OSPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  OSPIF_BD_ERROR_WREN_FAILED - Write Enable failed
     *                  OSPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     */
    virtual int program(const void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    /** Erase blocks on a block device
     *
     *  The state of an erased block is undefined until it has been programmed
     *
     *  @param addr     Address of block to begin erasing
     *  @param size     Size to erase in bytes, must be a multiple of erase block size
     *  @return         OSPIF_BD_ERROR_OK(0) - success
     *                  OSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  OSPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  OSPIF_BD_ERROR_WREN_FAILED - Write Enable failed
     *                  OSPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     *                  OSPIF_BD_ERROR_INVALID_ERASE_PARAMS - Trying to erase unaligned address or size
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

    /** Change the operation mode(SPI, SOPI or DOPI) of flash.
     *
     *  @return         OSPIF_BD_ERROR_OK(0) - success
     *                  OSPIF_BD_ERROR_DEVICE_ERROR - device driver transaction failed
     *                  OSPIF_BD_ERROR_READY_FAILED - Waiting for Memory ready failed or timed out
     *                  OSPIF_BD_ERROR_WREN_FAILED - Write Enable failed
     *                  OSPIF_BD_ERROR_PARSING_FAILED - unexpected format or values in one of the SFDP tables
     *                  OSPIF_BD_ERROR_INVALID_ERASE_PARAMS - Trying to erase unaligned address or size
     */
    virtual int change_mode(int mode);

private:
    /********************************/
    /*   Different Device Csel Mgmt */
    /********************************/
    // Add a new OSPI device CS to existing devices list.
    // Only one OSPIFBlockDevice instance per CS is allowed
    int add_new_csel_instance(PinName csel);

    // Remove device CS from existing device list upon destroying object (last deinit is called)
    int remove_csel_instance(PinName csel);

    /********************************/
    /*   Calls to OSPI Driver APIs  */
    /********************************/
    // Send Program/Write command to Driver
    ospi_status_t _ospi_send_program_command(mbed::ospi_inst_t prog_instruction, const void *buffer,
                                             mbed::bd_addr_t addr, mbed::bd_size_t *size);

    // Send Read command to Driver
    ospi_status_t _ospi_send_read_command(mbed::ospi_inst_t read_instruction, void *buffer, mbed::bd_addr_t addr, mbed::bd_size_t size);

    // Send Erase Instruction using command_transfer command to Driver
    ospi_status_t _ospi_send_erase_command(mbed::ospi_inst_t erase_instruction, mbed::bd_addr_t addr, mbed::bd_size_t size);

    // Send Generic command_transfer command to Driver
    ospi_status_t _ospi_send_general_command(mbed::ospi_inst_t instruction_int, mbed::bd_addr_t addr, const char *tx_buffer,
                                             mbed::bd_size_t tx_length, const char *rx_buffer, mbed::bd_size_t rx_length);

    // Send command to read from the SFDP table
    int _ospi_send_read_sfdp_command(mbed::bd_addr_t addr, mbed::sfdp_cmd_addr_size_t addr_size,
                                     uint8_t inst, uint8_t dummy_cycles,
                                     void *rx_buffer, mbed::bd_size_t rx_length);

    // Read the contents of status registers 1 and 2 into a buffer (buffer must have a length of 2)
    ospi_status_t _ospi_read_status_registers(uint8_t *reg_buffer);

    // Set the contents of status registers 1 and 2 from a buffer (buffer must have a length of 2)
    ospi_status_t _ospi_write_status_registers(uint8_t *reg_buffer);

    // Send set_frequency command to Driver
    ospi_status_t _ospi_set_frequency(int freq);

    // Update the 4-byte addressing extension register with the MSB of the address if it is in use
    ospi_status_t _ospi_update_4byte_ext_addr_reg(mbed::bd_addr_t addr);

    /*********************************/
    /* Flash Configuration Functions */
    /*********************************/
    // Clear the device's block protection
    int _clear_block_protection();

    // Configure Write Enable in Status Register
    int _set_write_enable();

    // Wait on status register until write not-in-progress
    bool _is_mem_ready();

    // Enable Fast Mode - for flash chips with low power default
    int _enable_fast_mode();

    // Enable OPI Mode
    int _enable_opi_mdoe();

    // Query vendor ID and handle special behavior that isn't covered by SFDP data
    int _handle_vendor_quirks();

    /****************************************/
    /* SFDP Detection and Parsing Functions */
    /****************************************/
    // Parse and Detect required Basic Parameters from Table
    int _sfdp_parse_basic_param_table(mbed::Callback<int(mbed::bd_addr_t, mbed::sfdp_cmd_addr_size_t, uint8_t, uint8_t, void *, mbed::bd_size_t)> sfdp_reader,
                                      mbed::sfdp_hdr_info &sfdp_info);

    // Parse and Detect 4-Byte Address Instruction Parameters from Table
    int _sfdp_parse_4_byte_inst_table(mbed::Callback<int(mbed::bd_addr_t, mbed::sfdp_cmd_addr_size_t, uint8_t, uint8_t, void *, mbed::bd_size_t)> sfdp_reader,
                                      mbed::sfdp_hdr_info &sfdp_info);

    // Detect the soft reset protocol and reset - returns error if soft reset is not supported
    int _sfdp_detect_reset_protocol_and_reset(uint8_t *basic_param_table_ptr);

    // Detect fastest read Bus mode supported by device
    int _sfdp_detect_best_bus_read_mode(uint8_t *basic_param_table_ptr, int basic_param_table_size,
                                        bool &set_quad_enable, bool &is_qpi_mode, bool &is_opi_mode);

    // Enable Quad mode if supported (1-1-4, 1-4-4, 4-4-4 bus modes)
    int _sfdp_set_quad_enabled(uint8_t *basic_param_table_ptr);

    // Enable QPI mode (4-4-4)
    int _sfdp_set_qpi_enabled(uint8_t *basic_param_table_ptr);

    // Detect 4-byte addressing mode and enable it if supported
    int _sfdp_detect_and_enable_4byte_addressing(uint8_t *basic_param_table_ptr, int basic_param_table_size);

#ifdef MX_FLASH_SUPPORT_RWW
    bool _is_mem_ready_rww(bd_addr_t addr, uint8_t rw);
#endif

private:
    enum ospif_clear_protection_method_t {
        OSPIF_BP_ULBPR,    // Issue global protection unlock instruction
        OSPIF_BP_CLEAR_SR, // Clear protection bits in status register 1
    };

    // OSPI Driver Object
    mbed::OSPI _ospi;

    // Static List of different OSPI based Flash devices csel that already exist
    // Each OSPI Flash device csel can have only 1 OSPIFBlockDevice instance
    // _devices_mutex is used to lock csel list - only one OSPIFBlockDevice instance per csel is allowed
    static SingletonPtr<PlatformMutex> _devices_mutex;
    static int _number_of_active_ospif_flash_csel;
    static PinName *_active_ospif_flash_csel_arr;

    int _unique_device_status;
    PinName _csel;

    // Mutex is used to protect Flash device for some OSPI Driver commands that must be done sequentially with no other commands in between
    // e.g. (1)Set Write Enable, (2)Program, (3)Wait Memory Ready
    PlatformMutex _mutex;

    // Command Instructions
    mbed::ospi_inst_t _read_instruction;
    mbed::ospi_inst_t _prog_instruction;
    mbed::ospi_inst_t _legacy_erase_instruction;

    // Status register write/read instructions
    unsigned int _num_status_registers;
    mbed::ospi_inst_t _write_status_reg_2_inst;
    mbed::ospi_inst_t _read_status_reg_2_inst; // If three registers, this instruction reads the latter two

    // Attempt to enable 4-byte addressing. True by default, but may be disabled for some vendors
    bool _attempt_4_byte_addressing;
    // 4-byte addressing extension register write instruction
    mbed::ospi_inst_t _4byte_msb_reg_write_inst;

    // Flash support 4-Byte instructions,like READ4B(0x13).
    bool _support_4_byte_inst;

    // Quad mode enable status register and bit
    int _quad_enable_register_idx;
    int _quad_enable_bit;

    bool _needs_fast_mode;

    // Clear block protection
    ospif_clear_protection_method_t _clear_protection_method;

    // Data extracted from the devices SFDP structure
    mbed::sfdp_hdr_info _sfdp_info;

    unsigned int _page_size_bytes; // Page size - 256 Bytes default
    int _freq;

    // Bus speed configuration
    ospi_bus_width_t _inst_width; //Bus width for Instruction phase
    ospi_inst_size_t _inst_size; //Instruction Size
    ospi_bus_width_t _address_width; //Bus width for Address phase
    ospi_address_size_t _address_size; //Number of bits for address
    ospi_alt_size_t _alt_size; //Number of bits for alt
    bool _alt_enabled; //Whether alt is enabled
    uint8_t _dummy_cycles; //Number of Dummy cycles required by Current Bus Mode
    ospi_bus_width_t _data_width; //Bus width for Data phase

    uint32_t _init_ref_count;
    bool _is_initialized;
#ifdef MX_FLASH_SUPPORT_RWW
    enum wait_flag {
        NOT_STARTED,         // no wait is started
        WRITE_WAIT_STARTED,  // write wait is started
        ERASE_WAIT_STARTED,  // erase wait is started
    };
    uint32_t _busy_bank;    // Current busy bank
    wait_flag _wait_flag;  // wait flag
    PlatformMutex _busy_mutex;
#endif
};

#endif
