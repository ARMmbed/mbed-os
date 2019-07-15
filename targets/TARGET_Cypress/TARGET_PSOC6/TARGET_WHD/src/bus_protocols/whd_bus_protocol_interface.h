/*
 * Copyright 2019 Cypress Semiconductor Corporation
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

#include "cyabs_rtos.h"

#include "whd.h"
#include "whd_network_types.h"
#include "whd_types_int.h"

#ifndef INCLUDED_WHD_BUS_PROTOCOL_INTERFACE_H_
#define INCLUDED_WHD_BUS_PROTOCOL_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
*             Constants
******************************************************/

typedef enum
{
    BUS_FUNCTION       = 0,
    BACKPLANE_FUNCTION = 1,
    WLAN_FUNCTION      = 2
} whd_bus_function_t;

#define BUS_FUNCTION_MASK (0x3) /* Update this if adding functions */

/******************************************************
*             Macros
******************************************************/
#define PLATFORM_WLAN_ALLOW_BUS_TO_SLEEP_DELAY_MS 10

#define DELAYED_BUS_RELEASE_SCHEDULE(whd_driver, schedule) do {  whd_delayed_bus_release_schedule_update(whd_driver, \
                                                                                                         schedule); \
} while (0)

/******************************************************
*             Structures
******************************************************/

#pragma pack(1)

typedef struct
{
    uint8_t bus_header[MAX_BUS_HEADER_SIZE];
    uint32_t data[1];
} whd_transfer_bytes_packet_t;

#pragma pack()

typedef void (*whd_bus_irq_callback_t)(void *handler_arg, uint32_t event);

/******************************************************
*             Function declarations
******************************************************/

/* Initialisation functions */
extern whd_result_t whd_bus_init(whd_driver_t whd_driver);
extern whd_result_t whd_bus_deinit(whd_driver_t whd_driver);

/* Device register access functions */
extern whd_result_t whd_bus_write_backplane_value(whd_driver_t whd_driver, uint32_t address, uint8_t register_length,
                                                  uint32_t value);
extern whd_result_t whd_bus_read_backplane_value(whd_driver_t whd_driver, uint32_t address, uint8_t register_length,
                                                 uint8_t *value);
extern whd_result_t whd_bus_write_register_value(whd_driver_t whd_driver, whd_bus_function_t function, uint32_t address,
                                                 uint8_t value_length, uint32_t value);
extern whd_result_t whd_bus_read_register_value(whd_driver_t whd_driver, whd_bus_function_t function, uint32_t address,
                                                uint8_t value_length, uint8_t *value);

/* Device data transfer functions */
extern whd_result_t whd_bus_send_buffer(whd_driver_t whd_driver, whd_buffer_t buffer);
extern whd_result_t whd_bus_transfer_bytes(whd_driver_t whd_driver, whd_bus_transfer_direction_t direction,
                                           whd_bus_function_t function, uint32_t address, uint16_t size,
                                           whd_transfer_bytes_packet_t *data);

/* Frame transfer function */
extern whd_result_t whd_bus_read_frame(whd_driver_t whd_driver, whd_buffer_t *buffer);

extern uint32_t     whd_bus_packet_available_to_read(whd_driver_t whd_driver);
extern whd_result_t whd_bus_poke_wlan(whd_driver_t whd_driver);
extern whd_result_t   whd_bus_wait_for_wlan_event(whd_driver_t whd_driver,  cy_semaphore_t *transceive_semaphore);

extern whd_result_t whd_bus_ack_interrupt(whd_driver_t whd_driver, uint32_t intstatus);
extern whd_bool_t whd_bus_wake_interrupt_present(whd_driver_t whd_driver);

extern whd_result_t whd_bus_set_flow_control(whd_driver_t whd_driver, uint8_t value);
extern whd_bool_t whd_bus_is_flow_controlled(whd_driver_t whd_driver);

extern whd_result_t whd_bus_wakeup(whd_driver_t whd_driver);
extern whd_result_t whd_bus_sleep(whd_driver_t whd_driver);

extern uint8_t whd_bus_backplane_read_padd_size(whd_driver_t whd_driver);
extern whd_bool_t whd_bus_use_status_report_scheme(whd_driver_t whd_driver);
extern uint32_t whd_bus_get_max_transfer_size(whd_driver_t whd_driver);

extern void         whd_bus_init_stats(whd_driver_t whd_driver);
extern whd_result_t whd_bus_print_stats(whd_driver_t whd_driver, whd_bool_t reset_after_print);
extern whd_result_t whd_bus_reinit_stats(whd_driver_t whd_driver, whd_bool_t wake_from_firmware);
extern whd_result_t whd_bus_irq_enable(whd_driver_t whd_driver, whd_bool_t enable);
extern whd_result_t whd_bus_irq_register(whd_driver_t whd_driver);
/******************************************************
*             Global variables
******************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef INCLUDED_WHD_BUS_PROTOCOL_INTERFACE_H_ */

