/*
 * Copyright 2021, Cypress Semiconductor Corporation (an Infineon company)
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

#include "whd.h"
#include "bus_protocols/whd_bus_protocol_interface.h"

#ifndef INCLUDED_SDIO_WHD_BUS_PROTOCOL_H
#define INCLUDED_SDIO_WHD_BUS_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
*                      Macros
******************************************************/
#define BIT_MASK(x)         ( (1 << x) - 1 )

#define WHD_BUS_HEADER_SIZE                     (0)

#define WHD_BUS_SDIO_MAX_BACKPLANE_TRANSFER_SIZE     (1536)
#define WHD_BUS_SDIO_BACKPLANE_READ_PADD_SIZE        (0)

/******************************************************
*             Structures
******************************************************/
#pragma pack(1)
typedef struct
{
    unsigned char stuff_bits;
    unsigned int ocr : 24;
} sdio_cmd5_argument_t;

typedef struct
{
    unsigned int _unique2         : 9;  /* 0-8   */
    unsigned int register_address : 17; /* 9-25  */
    unsigned int _unique          : 2;  /* 26-27 */
    unsigned int function_number  : 3;  /* 28-30 */
    unsigned int rw_flag          : 1;  /* 31    */
} sdio_cmd5x_argument_t;

typedef struct
{
    uint8_t write_data;                 /* 0 - 7 */
    unsigned int _stuff2          : 1;  /* 8     */
    unsigned int register_address : 17; /* 9-25  */
    unsigned int _stuff           : 1;  /* 26    */
    unsigned int raw_flag         : 1;  /* 27    */
    unsigned int function_number  : 3;  /* 28-30 */
    unsigned int rw_flag          : 1;  /* 31    */
} whd_bus_sdio_cmd52_argument_t;

typedef struct
{
    unsigned int count            : 9;  /* 0-8   */
    unsigned int register_address : 17; /* 9-25  */
    unsigned int op_code          : 1;  /* 26    */
    unsigned int block_mode       : 1;  /* 27    */
    unsigned int function_number  : 3;  /* 28-30 */
    unsigned int rw_flag          : 1;  /* 31    */
} whd_bus_sdio_cmd53_argument_t;

typedef union
{
    uint32_t value;
    sdio_cmd5_argument_t cmd5;
    sdio_cmd5x_argument_t cmd5x;
    whd_bus_sdio_cmd52_argument_t cmd52;
    whd_bus_sdio_cmd53_argument_t cmd53;
} sdio_cmd_argument_t;

typedef struct
{
    unsigned int ocr              : 24; /* 0-23  */
    unsigned int stuff_bits       : 3;  /* 24-26 */
    unsigned int memory_present   : 1;  /* 27    */
    unsigned int function_count   : 3;  /* 28-30 */
    unsigned int c                : 1;  /* 31    */
} sdio_response4_t;

typedef struct
{
    uint8_t data;                      /* 0-7   */
    uint8_t response_flags;             /* 8-15  */
    uint16_t stuff;                    /* 16-31 */
} sdio_response5_t;

typedef struct
{
    uint16_t card_status;               /* 0-15  */
    uint16_t rca;                      /* 16-31 */
} sdio_response6_t;

typedef union
{
    uint32_t value;
    sdio_response4_t r4;
    sdio_response5_t r5;
    sdio_response6_t r6;
} sdio_response_t;

typedef enum
{
    SDIO_BLOCK_MODE = (0 << 2),   /* These are STM32 implementation specific */
    SDIO_BYTE_MODE = (1 << 2)   /* These are STM32 implementation specific */
} sdio_transfer_mode_t;

typedef enum
{
    SDIO_1B_BLOCK = 1, SDIO_2B_BLOCK = 2, SDIO_4B_BLOCK = 4, SDIO_8B_BLOCK = 8, SDIO_16B_BLOCK = 16,
    SDIO_32B_BLOCK = 32, SDIO_64B_BLOCK = 64, SDIO_128B_BLOCK = 128, SDIO_256B_BLOCK = 256, SDIO_512B_BLOCK = 512,
    SDIO_1024B_BLOCK = 1024, SDIO_2048B_BLOCK = 2048
} sdio_block_size_t;

typedef enum
{
    RESPONSE_NEEDED, NO_RESPONSE
} sdio_response_needed_t;


#pragma pack()

/******************************************************
*             Macros
******************************************************/
#define WHD_BUS_STATS_INCREMENT_VARIABLE(bus_priv, var) \
    do { bus_priv->whd_bus_stats.var++; } while (0)

#define WHD_BUS_STATS_CONDITIONAL_INCREMENT_VARIABLE(bus_priv, condition, var) \
    do { if (condition){ bus_priv->whd_bus_stats.var++; }} while (0)

typedef struct
{
    uint32_t cmd52;             /* Number of cmd52 reads/writes issued */
    uint32_t cmd53_read;        /* Number of cmd53 reads */
    uint32_t cmd53_write;       /* Number of cmd53 writes */
    uint32_t cmd52_fail;        /* Number of cmd52 read/write fails */
    uint32_t cmd53_read_fail;   /* Number of cmd53 read fails */
    uint32_t cmd53_write_fail;  /* Number of cmd53 write fails */
    uint32_t oob_intrs;         /* Number of OOB interrupts generated by wlan chip */
    uint32_t sdio_intrs;        /* Number of SDIO interrupts generated by wlan chip */
    uint32_t error_intrs;       /* Number of SDIO error interrupts generated by wlan chip */
    uint32_t read_aborts;       /* Number of times read aborts are called */
} whd_bus_stats_t;

/******************************************************
*             Structures
******************************************************/

/******************************************************
*             Function declarations
******************************************************/
/* Initialisation functions */
extern whd_result_t whd_bus_sdio_init(whd_driver_t whd_driver);
extern whd_result_t whd_bus_sdio_resume_after_deep_sleep(whd_driver_t whd_driver);
extern whd_result_t whd_bus_sdio_deinit(whd_driver_t whd_driver);

/* Device register access functions */
extern whd_result_t whd_bus_sdio_write_backplane_value(whd_driver_t whd_driver, uint32_t address,
                                                       uint8_t register_length, uint32_t value);
extern whd_result_t whd_bus_sdio_read_backplane_value(whd_driver_t whd_driver, uint32_t address,
                                                      uint8_t register_length, uint8_t *value);
extern whd_result_t whd_bus_sdio_write_register_value(whd_driver_t whd_driver, whd_bus_function_t function,
                                                      uint32_t address, uint8_t value_length, uint32_t value);
extern whd_result_t whd_bus_sdio_read_register_value(whd_driver_t whd_driver, whd_bus_function_t function,
                                                     uint32_t address, uint8_t value_length, uint8_t *value);

/* Device data transfer functions */
extern whd_result_t whd_bus_sdio_send_buffer(whd_driver_t whd_driver, whd_buffer_t buffer);
extern whd_result_t whd_bus_sdio_transfer_bytes(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                                whd_bus_function_t function, uint32_t address, uint16_t size,
                                                whd_transfer_bytes_packet_t *data);
extern whd_result_t whd_bus_sdio_transfer_backplane_bytes(whd_driver_t whd_driver,
                                                          whd_bus_transfer_direction_t direction, uint32_t address,
                                                          uint32_t size, uint8_t *data);

/* Frame transfer function */
extern whd_result_t whd_bus_sdio_read_frame(whd_driver_t whd_driver, whd_buffer_t *buffer);

extern whd_result_t whd_bus_sdio_poke_wlan(whd_driver_t whd_driver);
extern uint32_t     whd_bus_sdio_packet_available_to_read(whd_driver_t whd_driver);
extern whd_result_t whd_bus_sdio_ack_interrupt(whd_driver_t whd_driver, uint32_t intstatus);

extern whd_result_t whd_bus_sdio_set_backplane_window(whd_driver_t whd_driver, uint32_t addr);

extern void whd_delayed_bus_release_schedule_update(whd_driver_t whd_driver, whd_bool_t is_scheduled);
#define DELAYED_BUS_RELEASE_SCHEDULE(whd_driver, schedule) \
    do {  whd_delayed_bus_release_schedule_update(whd_driver, schedule); } while (0)

extern whd_bool_t whd_bus_sdio_wake_interrupt_present(whd_driver_t whd_driver);

extern whd_result_t whd_bus_sdio_wakeup(whd_driver_t whd_driver);
extern whd_result_t whd_bus_sdio_sleep(whd_driver_t whd_driver);

extern void         whd_bus_sdio_init_stats(whd_driver_t whd_driver);
extern whd_result_t whd_bus_sdio_print_stats(whd_driver_t whd_driver, whd_bool_t reset_after_print);
extern whd_result_t whd_bus_sdio_reinit_stats(whd_driver_t whd_driver, whd_bool_t wake_from_firmware);

uint8_t whd_bus_sdio_backplane_read_padd_size(whd_driver_t whd_driver);

extern whd_result_t  whd_bus_sdio_wait_for_wlan_event(whd_driver_t whd_driver,
                                                      cy_semaphore_t *transceive_semaphore);
extern whd_bool_t whd_bus_sdio_use_status_report_scheme(whd_driver_t whd_driver);
extern uint32_t whd_bus_sdio_get_max_transfer_size(whd_driver_t whd_driver);
/******************************************************
*             Global variables
******************************************************/

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif /* ifndef INCLUDED_SDIO_WHD_BUS_PROTOCOL_H */

