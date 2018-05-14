/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef APP_UART_H_
#define APP_UART_H_

// This replacement <app_uart.h> provides forwarding of nrf logging to  
// stdout when NRF_LOG_USES_UART is enabled

#include <stdio.h>
#include "nrf_error.h"
#include "nrf51_bitfields.h"
#include "cmsis_compiler.h"

#define app_uart_put(c) putc(c, stdout)

__STATIC_INLINE uint32_t app_uart_get(uint8_t *c) {
    *c = getc(stdin);
    return NRF_SUCCESS;
} 

/**@brief UART Flow Control modes for the peripheral.
 */
typedef enum
{
    APP_UART_FLOW_CONTROL_DISABLED, /**< UART Hw Flow Control is disabled. */
    APP_UART_FLOW_CONTROL_ENABLED,  /**< Standard UART Hw Flow Control is enabled. */
    APP_UART_FLOW_CONTROL_LOW_POWER /**< Specialized UART Hw Flow Control is used. The Low Power setting allows the \nRFXX to Power Off the UART module when CTS is in-active, and re-enabling the UART when the CTS signal becomes active. This allows the \nRFXX to safe power by only using the UART module when it is needed by the remote site. */
} app_uart_flow_control_t;


/**@brief Enumeration which defines events used by the UART module upon data reception or error.
 *
 * @details The event type is used to indicate the type of additional information in the event
 * @ref app_uart_evt_t.
 */
typedef enum
{
    APP_UART_DATA_READY,          /**< An event indicating that UART data has been received. The data is available in the FIFO and can be fetched using @ref app_uart_get. */
    APP_UART_FIFO_ERROR,          /**< An error in the FIFO module used by the app_uart module has occured. The FIFO error code is stored in app_uart_evt_t.data.error_code field. */
    APP_UART_COMMUNICATION_ERROR, /**< An communication error has occured during reception. The error is stored in app_uart_evt_t.data.error_communication field. */
    APP_UART_TX_EMPTY,            /**< An event indicating that UART has completed transmission of all available data in the TX FIFO. */
    APP_UART_DATA,                /**< An event indicating that UART data has been received, and data is present in data field. This event is only used when no FIFO is configured. */
} app_uart_evt_type_t;


/**@brief UART communication structure holding configuration settings for the peripheral.
 */
typedef struct
{
    uint8_t                 rx_pin_no;    /**< RX pin number. */
    uint8_t                 tx_pin_no;    /**< TX pin number. */
    uint8_t                 rts_pin_no;   /**< RTS pin number, only used if flow control is enabled. */
    uint8_t                 cts_pin_no;   /**< CTS pin number, only used if flow control is enabled. */
    app_uart_flow_control_t flow_control; /**< Flow control setting, if flow control is used, the system will use low power UART mode, based on CTS signal. */
    bool                    use_parity;   /**< Even parity if TRUE, no parity if FALSE. */
    uint32_t                baud_rate;    /**< Baud rate configuration. */
} app_uart_comm_params_t;

/**@brief Struct containing events from the UART module.
 *
 * @details The app_uart_evt_t is used to notify the application of asynchronous events when data
 * are received on the UART peripheral or in case an error occured during data reception.
 */
typedef struct
{
    app_uart_evt_type_t evt_type; /**< Type of event. */
    union
    {
        uint32_t error_communication; /**< Field used if evt_type is: APP_UART_COMMUNICATION_ERROR. This field contains the value in the ERRORSRC register for the UART peripheral. The UART_ERRORSRC_x defines from nrf5x_bitfields.h can be used to parse the error code. See also the \nRFXX Series Reference Manual for specification. */
        uint32_t error_code;          /**< Field used if evt_type is: NRF_ERROR_x. Additional status/error code if the error event type is APP_UART_FIFO_ERROR. This error code refer to errors defined in nrf_error.h. */
        uint8_t  value;               /**< Field used if evt_type is: NRF_ERROR_x. Additional status/error code if the error event type is APP_UART_FIFO_ERROR. This error code refer to errors defined in nrf_error.h. */
    } data;
} app_uart_evt_t;

//#define APP_IRQ_PRIORITY_LOW -1,
//#define APP_IRQ_PRIORITY_LOWEST -1,

#define APP_UART_FIFO_INIT(P_COMM_PARAMS, RX_BUF_SIZE, TX_BUF_SIZE, EVT_HANDLER, IRQ_PRIO, ERR_CODE) \
    do                                                                                             \
    {                                                                                              \
        (void)P_COMM_PARAMS;                                                                       \
        ERR_CODE = 0;                                                                              \
    } while (0)
#endif // APP_UART_H_