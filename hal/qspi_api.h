/** \addtogroup hal */
/** @{*/

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
#ifndef MBED_QSPI_API_H
#define MBED_QSPI_API_H

#include "device.h"
#include "hal/dma_api.h"
#include <stdint.h>

#if DEVICE_QSPI

#define QSPI_EVENT_COMPLETE    (1 << 1)

typedef enum {
    QSPI_ADDRMODE_24BIT,
    QSPI_ADDRMODE_32BIT,
} qspi_addrmode_t;

typedef enum {
    QSPI_READMODE_1_1_1,
    QSPI_READMODE_1_1_2,
    QSPI_READMODE_1_1_4,
    QSPI_READMODE_1_2_2,
    QSPI_READMODE_1_4_4
} qspi_readmode_t;

typedef enum {
    QSPI_WRITEMODE_1_1_1,
    QSPI_WRITEMODE_1_1_2,
    QSPI_WRITEMODE_1_1_4,
    QSPI_WRITEMODE_1_4_4
} qspi_writemode_t;

typedef struct {
    PinName sclk;
    PinName ssel;
    PinName io0;
    PinName io1;
    PinName io2;
    PinName io3;
} qspi_pins_t;

typedef struct qspi_s qspi_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup qspi_hal Flash HAL API
 * @{
 */

/** Initialize the qspi peripheral and the qspi_t object
 *
 * @param[in] obj     The flash object
 * @param[in] hz      The baud rate in Hz
 * @param[in] pins    The pins structure to use for configuring pins
 * @param[in] spimode The SPI mode (clock polarity, phase, and shift direction)
 * @return 0 for success, -1 for error
 */
int32_t qspi_init(qspi_t *obj, int hz, qspi_pins_t * pins, int spimode);

/** Initialize the qspi peripheral and the qspi_t object
 *
 * @param[in] obj       The flash object
 * @param[in] addrmode  The address mode (24 or 32 bit)
 * @param[in] readmode  The read lines configuration
 * @param[in] writemode The write  lines configuration
 * @return 0 for success, -1 for error
 */
int32_t qspi_prepare(qspi_t *obj,
                     qspi_addrmode_t  addrmode,
                     qspi_readmode_t  readmode,
                     qspi_writemode_t writemode);

/** Start the qspi peripheral and the qspi_t object
 *
 * @param[in] obj       The flash object
 * @return 0 for success, -1 for error
 */
int32_t qspi_start(qspi_t *obj);

/** Uninitialize the qspi peripheral and the qspi_t object
 *
 * @param[in] obj       The flash object
 * @return 0 for success, -1 for error
 */
int32_t qspi_free(qspi_t *obj);

/** Erase one sector starting at defined address
 *
 * The address should be at sector boundary. This function does not do any check for address alignments
 * Finished function informs only about finish sending command sequence.
 * It is important to test status register of memory device using @ref qspi_is_busy function if device
 * is ready to execute next statements.
 * @param obj     The flash object
 * @param address The sector starting address
 * @return 0 for success, -1 for error
 */
int32_t qspi_erase_block(qspi_t *obj, uint32_t address);

/** Erase whole chip
 *
 * Finished function informs only about finish sending command sequence.
 * It is important to test status register of memory device using @ref qspi_is_busy function if device
 * is ready to execute next statements.
 * @param obj     The flash object
 * @return 0 for success, -1 for error
 */
int32_t qspi_erase_chip(qspi_t *obj);

/** Program one page starting at defined address
 *
 * The page should be at page boundary, should not cross multiple sectors.
 * This function does not do any check for address alignments or if size is aligned to a page size.
 * Finished function informs only about finish sending command sequence.
 * It is important to test status register of memory device using @ref qspi_is_busy function if device
 * is ready to execute next statements.
 * @param obj     The flash object
 * @param address The sector starting address
 * @param data    The data buffer to be programmed
 * @param size    The number of bytes to program
 * @return 0 for success, -1 for error
 */
int32_t qspi_program_page(qspi_t *obj, uint32_t address, const uint8_t *data, uint32_t size);

/** Program one page starting at defined address
 *
 * Finished function informs also about finish operation. Data is valid in memory pointed in data pointer.
 * @param obj     The flash object
 * @param address The sector starting address
 * @param data    The data buffer to be programmed
 * @param size    The number of bytes to program
 * @return 0 for success, -1 for error
 */
int32_t qspi_page_read(qspi_t *obj, uint32_t address, uint8_t *data, uint32_t size);

/** Send custom instruction
 *
 * Synchronous function. No callback will be execute after using this this function.
 *
 * @param obj     The flash object
 * @param address The sector starting address
 * @param data    The data buffer to be programmed
 * @param size    The number of bytes to program
 * @return 0 for success, -1 for error
 */
int32_t qspi_send_instruction(qspi_t *obj,
                              uint8_t cmd,
                              uint32_t tx_size,
                              const uint8_t *tx_data,
                              uint32_t rx_size,
                              uint8_t *rx_data);


int32_t qspi_is_busy(qspi_t *obj);

#if DEVICE_QSPI_ASYNCH
/**
 * \defgroup AsynchSPI Asynchronous QSPI Hardware Abstraction Layer
 * @{
 */

/** The asynchronous IRQ handler
 *
 * @param[in] obj     The QSPI object that holds the transfer information
 * @return Event flags if a transfer termination condition was met; otherwise 0.
 */
uint32_t qspi_irq_handler_asynch(qspi_t *obj);

/** Erase one sector starting at defined address
 *
 * The address should be at sector boundary. This function does not do any check for address alignments
 * @param[in] obj     The flash object
 * @param[in] address The sector starting address
 * @param[in] handler SPI interrupt handler
 * @param[in] event   The logical OR of events to be registered
 * @param[in] hint    A suggestion for how to use DMA with this transfer
 * @return 0 for success, -1 for error
 */
int32_t qspi_start_erase_block(qspi_t *obj,
                               uint32_t address,
                               uint32_t handler,
                               uint32_t event,
                               DMAUsage hint);

/** Erase whole chip
 *
 * Received acknowledge in interrupt routine informs only about finish sending command sequence.
 * It is important to test status register of memory device using @ref qspi_is_busy function if device
 * is ready to execute next statements.
 * @param[in] obj     The flash object
 * @param[in] address The sector starting address
 * @param[in] handler SPI interrupt handler
 * @param[in] event   The logical OR of events to be registered
 * @param[in] hint    A suggestion for how to use DMA with this transfer
 * @return 0 for success, -1 for error
 */
int32_t qspi_start_erase_chip(qspi_t *obj, uint32_t handler, uint32_t event, DMAUsage hint);

/** Program one page starting at defined address
 *
 * The page should be at page boundary, should not cross multiple sectors.
 * This function does not do any check for address alignments or if size is aligned to a page size.
 * Received acknowledge in interrupt routine informs only about finish sending command sequence.
 * It is important to test status register of memory device using @ref qspi_is_busy function if device
 * is ready to execute next statements.
 * @param[in] obj     The flash object
 * @param[in] address The sector starting address
 * @param[in] data    The data buffer to be programmed
 * @param[in] size    The number of bytes to program
 * @param[in] handler SPI interrupt handler
 * @param[in] event   The logical OR of events to be registered
 * @param[in] hint    A suggestion for how to use DMA with this transfer
 * @return 0 for success, -1 for error
 */
int32_t qspi_start_program_page(qspi_t *obj,
                                uint32_t address,
                                const uint8_t *data,
                                uint32_t size,
                                uint32_t handler,
                                uint32_t event,
                                DMAUsage hint);

/** Program one page starting at defined address
 *
 * The page should be at page boundary, should not cross multiple sectors.
 * This function does not do any check for address alignments or if size is aligned to a page size.
 * Received acknowledge in interrupt routine informs about finish sending and reading command sequence.
 * It is not necessary to test status register of memory device. Memory used by QSPI device can be processed
 * in interrupt routine. Data is valid.
 * @param[in] obj     The flash object
 * @param[in] address The sector starting address
 * @param[in] data    The data buffer to be programmed
 * @param[in] size    The number of bytes to program
 * @param[in] handler SPI interrupt handler
 * @param[in] event   The logical OR of events to be registered
 * @param[in] hint    A suggestion for how to use DMA with this transfer
 * @return 0 for success, -1 for error
 */
int32_t qspi_start_page_read(qspi_t *obj,
                             uint32_t address,
                             uint8_t *data,
                             uint32_t size,
                             uint32_t handler,
                             uint32_t event,
                             DMAUsage hint);

#endif

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

#endif

/** @}*/
