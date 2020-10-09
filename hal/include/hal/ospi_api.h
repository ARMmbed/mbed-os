
/** \addtogroup hal */
/** @{*/
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
#ifndef MBED_OSPI_API_H
#define MBED_OSPI_API_H

#include "device.h"
#include "pinmap.h"
#include <stdbool.h>

#if DEVICE_OSPI

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup hal_ospi OSPI HAL
 * @{
 */

/** OSPI HAL object
 */
typedef struct ospi_s ospi_t;

typedef struct {
    int peripheral;
    PinName data0_pin;
    int data0_function;
    PinName data1_pin;
    int data1_function;
    PinName data2_pin;
    int data2_function;
    PinName data3_pin;
    int data3_function;
    PinName data4_pin;
    int data4_function;
    PinName data5_pin;
    int data5_function;
    PinName data6_pin;
    int data6_function;
    PinName data7_pin;
    int data7_function;
    PinName sclk_pin;
    int sclk_function;
    PinName ssel_pin;
    int ssel_function;
    PinName dqs_pin;
    int dqs_function;

} ospi_pinmap_t;

/** OSPI Bus width
 *
 * Some parts of commands provide variable bus width
 */
typedef enum ospi_bus_width {
    OSPI_CFG_BUS_SINGLE,
    OSPI_CFG_BUS_DUAL,
    OSPI_CFG_BUS_QUAD,
    OSPI_CFG_BUS_OCTA,
    OSPI_CFG_BUS_OCTA_DTR,
} ospi_bus_width_t;

/** Instruction size in bits
 */
typedef enum ospi_inst_size {
    OSPI_CFG_INST_SIZE_8,    /* 1 byte for SPI mode */
    OSPI_CFG_INST_SIZE_16,   /* 2 byte for OPI mode */
} ospi_inst_size_t;

/** Address size in bits
 */
typedef enum ospi_address_size {
    OSPI_CFG_ADDR_SIZE_8,
    OSPI_CFG_ADDR_SIZE_16,
    OSPI_CFG_ADDR_SIZE_24,
    OSPI_CFG_ADDR_SIZE_32,
} ospi_address_size_t;

/** Alternative size in bits
 */
typedef uint8_t ospi_alt_size_t;

// The following defines are provided for backwards compatibilty. New code should explicitly
// specify the required number of alt bits.
#define OSPI_CFG_ALT_SIZE_8 8u
#define OSPI_CFG_ALT_SIZE_16 16u
#define OSPI_CFG_ALT_SIZE_24 24u
#define OSPI_CFG_ALT_SIZE_32 32u

/** OSPI command
 *
 * Defines a frame format. It consists of instruction, address, alternative, dummy count and data
 */
typedef struct ospi_command {
    struct {
        ospi_bus_width_t bus_width; /**< Bus width for the instruction >*/
        ospi_inst_size_t size; /**< Inst size >*/
        uint32_t value;  /**< Instruction value >*/
        bool disabled; /**< Instruction phase skipped if disabled is set to true >*/
    } instruction;
    struct {
        ospi_bus_width_t bus_width; /**< Bus width for the address >*/
        ospi_address_size_t size; /**< Address size >*/
        uint32_t value; /**< Address value >*/
        bool disabled; /**< Address phase skipped if disabled is set to true >*/
    }  address;
    struct {
        ospi_bus_width_t bus_width; /**< Bus width for alternative  >*/
        ospi_alt_size_t size; /**< Alternative size >*/
        uint32_t value; /**< Alternative value >*/
        bool disabled; /**< Alternative phase skipped if disabled is set to true >*/
    } alt;
    uint8_t dummy_count; /**< Dummy cycles count >*/
    struct {
        ospi_bus_width_t bus_width; /**< Bus width for data >*/
    } data;
} ospi_command_t;

/** OSPI return status
 */
typedef enum ospi_status {
    OSPI_STATUS_ERROR = -1, /**< Generic error >*/
    OSPI_STATUS_INVALID_PARAMETER = -2, /**< The parameter is invalid >*/
    OSPI_STATUS_OK    =  0, /**< Function executed sucessfully  >*/
} ospi_status_t;

/** Initialize OSPI peripheral.
 *
 * It should initialize OSPI pins (io0-io7, sclk, ssel and dqs), set frequency, clock polarity and phase mode. The clock for the peripheral should be enabled
 *
 * @param obj OSPI object
 * @param io0 Data pin 0
 * @param io1 Data pin 1
 * @param io2 Data pin 2
 * @param io3 Data pin 3
 * @param io4 Data pin 4
 * @param io5 Data pin 5
 * @param io6 Data pin 6
 * @param io7 Data pin 7
 * @param sclk The clock pin
 * @param ssel The chip select pin
 * @param dqs The chip dqs pin
 * @param hz The bus frequency
 * @param mode Clock polarity and phase mode (0 - 3)
 * @return OSPI_STATUS_OK if initialisation successfully executed
           OSPI_STATUS_INVALID_PARAMETER if invalid parameter found
           OSPI_STATUS_ERROR otherwise
 */
ospi_status_t ospi_init(ospi_t *obj, PinName io0, PinName io1, PinName io2, PinName io3, PinName io4, PinName io5, PinName io6, PinName io7,
                        PinName sclk, PinName ssel, PinName dqs, uint32_t hz, uint8_t mode);

/** Initialize OSPI peripheral.
 *
 * It should initialize OSPI pins (io0-io7, sclk, ssel and dqs), set frequency, clock polarity and phase mode. The clock for the peripheral should be enabled
 *
 * @param obj OSPI object
 * @param pinmap pointer to structure which holds static pinmap
 * @param hz The bus frequency
 * @param mode Clock polarity and phase mode (0 - 3)
 * @return OSPI_STATUS_OK if initialisation successfully executed
           OSPI_STATUS_INVALID_PARAMETER if invalid parameter found
           OSPI_STATUS_ERROR otherwise
 */
ospi_status_t ospi_init_direct(ospi_t *obj, const ospi_pinmap_t *pinmap, uint32_t hz, uint8_t mode);

/** Deinitialize OSPI peripheral
 *
 * It should release pins that are associated with the OSPI object, and disable clocks for OSPI peripheral module that was associated with the object
 *
 * @param obj OSPI object
 * @return OSPI_STATUS_OK if deinitialisation successfully executed
           OSPI_STATUS_INVALID_PARAMETER if invalid parameter found
           OSPI_STATUS_ERROR otherwise
 */
ospi_status_t ospi_free(ospi_t *obj);

/** Set the OSPI baud rate
 *
 * Actual frequency may differ from the desired frequency due to available dividers and the bus clock
 * Configures the OSPI peripheral's baud rate
 * @param obj The SPI object to configure
 * @param hz  The baud rate in Hz
 * @return OSPI_STATUS_OK if frequency was set
           OSPI_STATUS_INVALID_PARAMETER if invalid parameter found
           OSPI_STATUS_ERROR otherwise
 */
ospi_status_t ospi_frequency(ospi_t *obj, int hz);

/** Send a command and block of data
 *
 * @param obj OSPI object
 * @param command OSPI command
 * @param data TX buffer
 * @param[in,out] length in - TX buffer length in bytes, out - number of bytes written
 * @return OSPI_STATUS_OK if the data has been succesfully sent
           OSPI_STATUS_INVALID_PARAMETER if invalid parameter found
           OSPI_STATUS_ERROR otherwise
 */
ospi_status_t ospi_write(ospi_t *obj, const ospi_command_t *command, const void *data, size_t *length);

/** Send a command (and optionally data) and get the response. Can be used to send/receive device specific commands
 *
 * @param obj OSPI object
 * @param command OSPI command
 * @param tx_data TX buffer
 * @param tx_size TX buffer length in bytes
 * @param rx_data RX buffer
 * @param rx_size RX buffer length in bytes
 * @return OSPI_STATUS_OK if the data has been succesfully sent
           OSPI_STATUS_INVALID_PARAMETER if invalid parameter found
           OSPI_STATUS_ERROR otherwise
 */
ospi_status_t ospi_command_transfer(ospi_t *obj, const ospi_command_t *command, const void *tx_data, size_t tx_size, void *rx_data, size_t rx_size);

/** Receive a command and block of data
 *
 * @param obj OSPI object
 * @param command OSPI command
 * @param data RX buffer
 * @param[in,out] length in - RX buffer length in bytes, out - number of bytes read
 * @return OSPI_STATUS_OK if data has been succesfully received
           OSPI_STATUS_INVALID_PARAMETER if invalid parameter found
           OSPI_STATUS_ERROR otherwise
 */
ospi_status_t ospi_read(ospi_t *obj, const ospi_command_t *command, void *data, size_t *length);

/** Get the pins that support OSPI SCLK
 *
 * Return a PinMap array of pins that support OSPI SCLK in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_sclk_pinmap(void);

/** Get the pins that support OSPI SSEL
 *
 * Return a PinMap array of pins that support OSPI SSEL in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_ssel_pinmap(void);

/** Get the pins that support OSPI DQS
 *
 * Return a PinMap array of pins that support OSPI DQS in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_dqs_pinmap(void);

/** Get the pins that support OSPI DATA0
 *
 * Return a PinMap array of pins that support OSPI DATA0 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data0_pinmap(void);

/** Get the pins that support OSPI DATA1
 *
 * Return a PinMap array of pins that support OSPI DATA1 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data1_pinmap(void);

/** Get the pins that support OSPI DATA2
 *
 * Return a PinMap array of pins that support OSPI DATA2 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data2_pinmap(void);

/** Get the pins that support OSPI DATA3
 *
 * Return a PinMap array of pins that support OSPI DATA3 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data3_pinmap(void);

/** Get the pins that support OSPI DATA4
 *
 * Return a PinMap array of pins that support OSPI DATA4 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data4_pinmap(void);

/** Get the pins that support OSPI DATA5
 *
 * Return a PinMap array of pins that support OSPI DATA5 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data5_pinmap(void);

/** Get the pins that support OSPI DATA6
 *
 * Return a PinMap array of pins that support OSPI DATA6 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data6_pinmap(void);

/** Get the pins that support OSPI DATA7
 *
 * Return a PinMap array of pins that support OSPI DATA7 in
 * master mode. The array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *ospi_master_data7_pinmap(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
