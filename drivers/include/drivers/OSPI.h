/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
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
#ifndef MBED_OSPI_H
#define MBED_OSPI_H

#include "platform/platform.h"

#if DEVICE_OSPI || defined(DOXYGEN_ONLY)

#include "hal/ospi_api.h"
#include "platform/PlatformMutex.h"
#include "platform/SingletonPtr.h"
#include "platform/NonCopyable.h"

#define ONE_MHZ     1000000

#define OSPI_NO_INST (-1)

namespace mbed {
/** \defgroup drivers-public-api-spi SPI
 * \ingroup drivers-public-api
 */

/**
 * \defgroup drivers_OSPI OSPI class
 * \ingroup drivers-public-api-spi
 * @{
 */

/** Type representing a OSPI instruction
 */
typedef int ospi_inst_t;

/** A OSPI Driver, used for communicating with OSPI slave devices
 *
 * The default format is set to OCTO-SPI(1-1-1), and a clock frequency of 1MHz
 * Most OSPI devices will also require Chip Select which is indicated by ssel.
 *
 * @note Synchronization level: Thread safe
 *
 * Example:
 * @code
 * // Write 4 byte array to a OSPI slave, and read the response, note that each device will have its specific read/write/alt values defined
 *
 * #include "mbed.h"
 *
 * #define CMD_WRITE 0x02
 * #define CMD_READ  0x03
 * #define ADDRESS   0x1000
 *
 * // hardware ssel (where applicable)
 * OSPI ospi_device(OSPI_FLASH1_IO0, OSPI_FLASH1_IO1, OSPI_FLASH1_IO2, OSPI_FLASH1_IO3, OSPI_FLASH1_IO4, OSPI_FLASH1_IO5, OSPI_FLASH1_IO6,
 *                  OSPI_FLASH1_SCK, OSPI_FLASH1_CSN, OSPI_FLASH1_DQS); // io0, io1, io2, io3, io4, io5, io6, io7, sclk, ssel, dqs
 *
 *
 * int main() {
 *     char tx_buf[] = { 0x11, 0x22, 0x33, 0x44 };
 *     char rx_buf[4];
 *     int buf_len = sizeof(tx_buf);
 *
 *     ospi_status_t result = ospi_device.write(CMD_WRITE, 0, ADDRESS, tx_buf, &buf_len);
 *     if (result != OSPI_STATUS_OK) {
 *        printf("Write failed");
 *     }
 *     result = ospi_device.read(CMD_READ, 0, ADDRESS, rx_buf, &buf_len);
 *     if (result != OSPI_STATUS_OK) {
 *        printf("Read failed");
 *     }
 *
 * }
 * @endcode
 */
class OSPI : private NonCopyable<OSPI> {

public:

    /** Create a OSPI master connected to the specified pins
     *
     *  io0-io3 is used to specify the Pins used for Quad SPI mode
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
     *  @param ssel OSPI chip select pin
     *  @param dqs OSPI dqs pin
     *  @param mode Clock polarity and phase mode (0 - 3) of SPI
     *         (Default: Mode=0 uses CPOL=0, CPHA=0, Mode=1 uses CPOL=1, CPHA=1)
     *
     */
    OSPI(PinName io0, PinName io1, PinName io2, PinName io3, PinName io4, PinName io5, PinName io6, PinName io7,
         PinName sclk, PinName ssel = NC, PinName dqs = NC, int mode = 0);

    /** Create a OSPI master connected to the specified pins
     *
     *  io0-io3 is used to specify the Pins used for Quad SPI mode
     *
     *  @param pinmap reference to structure which holds static pinmap
     *  @param mode Clock polarity and phase mode (0 - 3) of SPI
     *         (Default: Mode=0 uses CPOL=0, CPHA=0, Mode=1 uses CPOL=1, CPHA=1)
     *
     */
    OSPI(const ospi_pinmap_t &pinmap, int mode = 0);
    OSPI(const ospi_pinmap_t &&, int = 0) = delete; // prevent passing of temporary objects

    virtual ~OSPI()
    {
    }

    /** Configure the data transmission format
     *
     *  @param inst_width Bus width used by instruction phase(Valid values are OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_DUAL, OSPI_CFG_BUS_QUAD)
     *  @param inst_size Size in bits used by instruction phase
     *  @param address_width Bus width used by address phase(Valid values are OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_DUAL, OSPI_CFG_BUS_QUAD)
     *  @param address_size Size in bits used by address phase(Valid values are OSPI_CFG_ADDR_SIZE_8, OSPI_CFG_ADDR_SIZE_16, OSPI_CFG_ADDR_SIZE_24, OSPI_CFG_ADDR_SIZE_32)
     *  @param alt_width Bus width used by alt phase(Valid values are OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_DUAL, OSPI_CFG_BUS_QUAD)
     *  @param alt_size Size in bits used by alt phase (must be a multiple of the number of bus lines indicated in alt_width)
     *  @param data_width Bus width used by data phase(Valid values are OSPI_CFG_BUS_SINGLE, OSPI_CFG_BUS_DUAL, OSPI_CFG_BUS_QUAD)
     *  @param dummy_cycles Number of dummy clock cycles to be used after alt phase
     *
     */
    ospi_status_t configure_format(ospi_bus_width_t inst_width,
                                   ospi_inst_size_t inst_size,
                                   ospi_bus_width_t address_width,
                                   ospi_address_size_t address_size,
                                   ospi_bus_width_t alt_width,
                                   ospi_alt_size_t alt_size,
                                   ospi_bus_width_t data_width,
                                   int dummy_cycles);

    /** Set the ospi bus clock frequency
     *
     *  @param hz SCLK frequency in hz (default = 1MHz)
     *  @returns
     *    Returns OSPI_STATUS_SUCCESS on successful, fails if the interface is already init-ed
     */
    ospi_status_t set_frequency(int hz = ONE_MHZ);

    /** Read from OSPI peripheral with the preset read_instruction and alt_value
     *
     *  @param address Address to be accessed in OSPI peripheral
     *  @param rx_buffer Buffer for data to be read from the peripheral
     *  @param rx_length Pointer to a variable containing the length of rx_buffer, and on return this variable will be updated with the actual number of bytes read
     *
     *  @returns
     *    Returns OSPI_STATUS_SUCCESS on successful reads and OSPI_STATUS_ERROR on failed reads.
     */
    ospi_status_t read(int address, char *rx_buffer, size_t *rx_length);

    /** Write to OSPI peripheral using custom write instruction
     *
     *  @param address Address to be accessed in OSPI peripheral
     *  @param tx_buffer Buffer containing data to be sent to peripheral
     *  @param tx_length Pointer to a variable containing the length of data to be transmitted, and on return this variable will be updated with the actual number of bytes written
     *
     *  @returns
     *    Returns OSPI_STATUS_SUCCESS on successful reads and OSPI_STATUS_ERROR on failed reads.
     */
    ospi_status_t write(int address, const char *tx_buffer, size_t *tx_length);

    /** Read from OSPI peripheral using custom read instruction, alt values
     *
     *  @param instruction Instruction value to be used in instruction phase. Use OSPI_NO_INST to skip the instruction phase
     *  @param alt Alt value to be used in Alternate-byte phase. Use -1 for ignoring Alternate-byte phase
     *  @param address Address to be accessed in OSPI peripheral
     *  @param rx_buffer Buffer for data to be read from the peripheral
     *  @param rx_length Pointer to a variable containing the length of rx_buffer, and on return this variable will be updated with the actual number of bytes read
     *
     *  @returns
     *    Returns OSPI_STATUS_SUCCESS on successful reads and OSPI_STATUS_ERROR on failed reads.
     */
    ospi_status_t read(ospi_inst_t instruction, int alt, int address, char *rx_buffer, size_t *rx_length);

    /** Write to OSPI peripheral using custom write instruction, alt values
     *
     *  @param instruction Instruction value to be used in instruction phase. Use OSPI_NO_INST to skip the instruction phase
     *  @param alt Alt value to be used in Alternate-byte phase. Use -1 for ignoring Alternate-byte phase
     *  @param address Address to be accessed in OSPI peripheral
     *  @param tx_buffer Buffer containing data to be sent to peripheral
     *  @param tx_length Pointer to a variable containing the length of data to be transmitted, and on return this variable will be updated with the actual number of bytes written
     *
     *  @returns
     *    Returns OSPI_STATUS_SUCCESS on successful reads and OSPI_STATUS_ERROR on failed reads.
     */
    ospi_status_t write(ospi_inst_t instruction, int alt, int address, const char *tx_buffer, size_t *tx_length);

    /** Perform a transaction to write to an address(a control register) and get the status results
     *
     *  @param instruction Instruction value to be used in instruction phase. Use OSPI_NO_INST to skip the instruction phase
     *  @param address Some instruction might require address. Use -1 if no address
     *  @param tx_buffer Buffer containing data to be sent to peripheral
     *  @param tx_length Pointer to a variable containing the length of data to be transmitted, and on return this variable will be updated with the actual number of bytes written
     *  @param rx_buffer Buffer for data to be read from the peripheral
     *  @param rx_length Pointer to a variable containing the length of rx_buffer, and on return this variable will be updated with the actual number of bytes read
     *
     *  @returns
     *    Returns OSPI_STATUS_SUCCESS on successful reads and OSPI_STATUS_ERROR on failed reads.
     */
    ospi_status_t command_transfer(ospi_inst_t instruction, int address, const char *tx_buffer, size_t tx_length, const char *rx_buffer, size_t rx_length);

#if !defined(DOXYGEN_ONLY)
protected:
    /** Acquire exclusive access to this SPI bus
     */
    virtual void lock(void);

    /** Release exclusive access to this SPI bus
     */
    virtual void unlock(void);

    ospi_t _ospi;

    static SingletonPtr<PlatformMutex> _mutex;
    ospi_bus_width_t _inst_width; //Bus width for Instruction phase
    ospi_inst_size_t _inst_size; //Instruction Size
    ospi_bus_width_t _address_width; //Bus width for Address phase
    ospi_address_size_t _address_size;
    ospi_bus_width_t _alt_width; //Bus width for Alt phase
    ospi_alt_size_t _alt_size;
    ospi_bus_width_t _data_width; //Bus width for Data phase
    ospi_command_t _ospi_command; //OSPI Hal command struct
    unsigned int _num_dummy_cycles; //Number of dummy cycles to be used
    int _hz; //Bus Frequency
    int _mode; //SPI mode
    bool _initialized;
    PinName _ospi_io0, _ospi_io1, _ospi_io2, _ospi_io3, _ospi_io4, _ospi_io5, _ospi_io6, _ospi_io7, _ospi_clk, _ospi_cs, _ospi_dqs; //IO lines, clock, chip select and dqs
    const ospi_pinmap_t *_static_pinmap;
    bool (OSPI::* _init_func)(void);

private:
    bool _initialize();
    bool _initialize_direct();

    /*
     * This function builds the ospi command struct to be send to Hal
     */
    inline void _build_ospi_command(ospi_inst_t instruction, int address, int alt);
#endif
};

/** @}*/

} // namespace mbed

#endif

#endif
