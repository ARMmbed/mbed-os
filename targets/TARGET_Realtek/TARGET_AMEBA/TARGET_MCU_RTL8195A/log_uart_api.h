/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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

#ifndef LOG_UART_API_H
#define LOG_UART_API_H

#if defined(CONFIG_PLATFORM_8195A) && (CONFIG_PLATFORM_8195A == 1)
#include "device.h"
#include "serial_api.h"
#include "hal_log_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup log_uart LOG_UART
 *  @ingroup    hal
 *  @brief      log_uart functions
 *  @{
 */

///@name Ameba1 Only
///@{

/******************************************************
 *                 Type Definitions
 ******************************************************/
/** Log uart irq handler function pointer type
 *
 * @param id  : The argument for log uart interrupt handler
 * @param event   : The log uart interrupt indication ID. More details is shown in hal_log_uart.h
 */
typedef void (*loguart_irq_handler)(uint32_t id, LOG_UART_INT_ID event);
typedef struct log_uart_s log_uart_t;

/******************************************************
 *               Function Declarations
 ******************************************************/
/**
 * @brief  Initialize Realtek log uart.
 * 		   Initialize the required parts of the log uart.
 *   			i.e. baudrate, data bits, parity, etc.
 * @param[in]  obj: The address of log uart object.
 * @param[in]  baudrate: Baud rate of the log uart object. 
 * @param[in]  data_bits: Data bits of the log uart object.
 * @param[in]  parity: Parity type of the log uart object
                        - ParityNone,           - Do not use parity
                        - ParityOdd,            - Use odd parity
                        - ParityEven,           - Use even parity
                        - ParityForced1,        - Use even parity, the same as ParityEven
                        - ParityForced0         - Use odd parity, the same as ParityOdd
 * @param[in]  stop_bits: The number of stop bits for the log uart object.
 * @return 0 if initialization is successful, -1 otherwise
 */
int32_t log_uart_init(log_uart_t *obj, int baudrate, int data_bits, SerialParity parity, int stop_bits);

/**
 * @brief  Release the resources related to Realtek log uart.
 
 * @param[in]  obj: The address of log uart object.
 * @return None
 */
void log_uart_free(log_uart_t *obj);

/**
 * @brief  Set the baud rate of log uart.
 
 * @param[in]  obj: The address of log uart object.
 * @param[in]  baudrate: Baud rate of the log uart object.
 * @return None
 */
void log_uart_baud(log_uart_t *obj, int baudrate);

/**
 * @brief  Set parameters for log uart.
 *   			including data bits, parity type and stop bits
 
 * @param[in]  obj: The address of log uart object.
 * @param[in]  data_bits: Data bits of log uart object.
 * @param[in]  parity: Parity type of the log uart object
                        - ParityNone,           - Do not use parity
                        - ParityOdd,            - Use odd parity
                        - ParityEven,           - Use even parity
                        - ParityForced1,        - Use even parity, the same as ParityEven
                        - ParityForced0         - Use odd parity, the same as ParityOdd
 * @param[in]  stop_bits: The number of stop bits for the log uart object.
 * @return None
 */
void log_uart_format(log_uart_t *obj, int data_bits, SerialParity parity, int stop_bits);

/**
 * @brief  Set irq handler for log uart.
 * @param[in]  obj: The address of log uart object.
 * @param[in]  handler: The interrupt handler for log uart.
 * @param[in]  id: The argument for log uart interrupt handler.
 * @return None
 */
void log_uart_irq_handler(log_uart_t *obj, loguart_irq_handler handler, uint32_t id);

/**
 * @brief  Enable/disable the specific irq indication ID.
 * @param[in]  obj: The address of log uart object.
 * @param[in]  irq: The log uart interrupt indication ID which will be enabled/disabled.
 * @param[in]  enable: 1 enable, 0 disable
 * @return None
 */
void log_uart_irq_set(log_uart_t *obj, LOG_UART_INT_ID irq, uint32_t enable);

/**
 * @brief  Read one character from log uart.
           This function will block untill the log uart gets something to read 
 * @param[in]  obj: The address of log uart object.
 * @return the character read from log uart
 */
char log_uart_getc(log_uart_t *obj);

/**
 * @brief  Write one character to log uart.
           This function will block untill the data is successfully written to log uart
 * @param[in]  obj: The address of log uart object.
 * @param[in]  c: The one byte data to be written to log uart.
 * @return None
 */
void log_uart_putc(log_uart_t *obj, char c);

/**
 * @brief  Check whether log uart is ready to read data
 * @param[in]  obj: The address of log uart object.
 * @return 1 if there is data at log uart to be read, 0 otherwise
 */
int log_uart_readable(log_uart_t *obj);

/**
 * @brief  Check whether log uart is ready to write data
 * @param[in]  obj: The address of log uart object.
 * @return 1 if log uart is ready for writing, 0 otherwise
 */
int log_uart_writable(log_uart_t *obj);

/**
 * @brief  Clear both data at log uart
           This function will clear data in both TX FIFO and RX FIFO of log uart
 * @param[in]  obj: The address of log uart object.
 * @return None
 */
void log_uart_clear(log_uart_t *obj);

/**
 * @brief  Clear TX FIFO of log uart
 * @param[in]  obj: The address of log uart object.
 * @return None
 */
void log_uart_clear_tx(log_uart_t *obj);

/**
 * @brief  Clear RX FIFO of log uart
 * @param[in]  obj: The address of log uart object.
 * @return None
 */
void log_uart_clear_rx(log_uart_t *obj);

/**
 * @brief  Set break control for log uart
 * @param[in]  obj: The address of log uart object.
 * @return None
 */
void log_uart_break_set(log_uart_t *obj);

/**
 * @brief  Clear break control for log uart
 * @param[in]  obj: The address of log uart object.
 * @return None
 */
void log_uart_break_clear(log_uart_t *obj);

/**
 * @brief  Set the handler for complete TX
 * @param[in]  obj: The address of log uart object.
 * @param[in]  handler: The function which is called when log uart has finished transmitting data.
 * @param[in]  id: The parameter for handler.  
 * @return None
 */
void log_uart_tx_comp_handler(log_uart_t *obj, void *handler, uint32_t id);

/**
 * @brief  Set the handler for complete RX
 * @param[in]  obj: The address of log uart object.
 * @param[in]  handler: The function which is called when log uart has finished receving data
 * @param[in]  id: The parameter for handler.  
 * @return None
 */
void log_uart_rx_comp_handler(log_uart_t *obj, void *handler, uint32_t id);

/**
 * @brief  Set the handler for line status
 * @param[in]  obj: The address of log uart object.
 * @param[in]  handler: The function which is called when log uart gets an line status indication ID. 
 * @param[in]  id: The parameter for handler.  
 * @return None
 */
void log_uart_line_status_handler(log_uart_t *obj, void *handler, uint32_t id);

/**
 * @brief  Read data from log uart in blocking mode.
 * @param[in]  obj: The address of log uart object.
 * @param[out]  prxbuf: The buffer to store received data.
 * @param[in]  len: The maximum length of data to be read
 * @param[in]  timeout_ms: Blocking time in ms.
 * @return the length of received data in bytes
 */
int32_t log_uart_recv(log_uart_t *obj, char *prxbuf, uint32_t len, uint32_t timeout_ms);

/**
 * @brief  Send data to log uart in blocking mode
 * @param[in]  obj: The address of log uart object.
 * @param[in]  ptxbuf: Data buffer to be sent to log uart
 * @param[in]  len: Length of data to be sent to log uart
 * @param[in]  timeout_ms: Blocking time in ms.
 * @return the length of sent data in bytes
 */
int32_t log_uart_send(log_uart_t *obj, char *ptxbuf, uint32_t len, uint32_t timeout_ms);

/**
 * @brief  Read data from log uart in interrupt mode(Non-blocking)
 * @param[in]  obj: The address of log uart object.
 * @param[out]  prxbuf: The buffer to store received data.
 * @param[in]  len: The maximum length of data to be read
 * @return 0 if success
 */
int32_t log_uart_recv_stream(log_uart_t *obj, char *prxbuf, uint32_t len);

/**
 * @brief  Send data to log uart in interrupt mode(Non-blocking)
 * @param[in]  obj: The address of log uart object.
 * @param[in]  ptxbuf: Data buffer to be sent to log uart
 * @param[in]  len: Length of data to be sent to log uart
 * @return 0 if success
 */
int32_t log_uart_send_stream(log_uart_t *obj, char *ptxbuf, uint32_t len);

/**
 * @brief  Read data from log uart with a given timeout in interrupt mode(Non-blocking)
 * @param[in]  obj: The address of log uart object.
 * @param[out]  prxbuf: The buffer to store received data.
 * @param[in]  len: The maximum length of data to be read
 * @param[in]  timeout_ms: The timeout for reading data in ms
 * @param[in]  force_cs: User callback function 
 * @return the length in Byte of received data before timeout, or error (< 0)
 */
int32_t log_uart_recv_stream_timeout(log_uart_t *obj, char *prxbuf, uint32_t len,
                                        uint32_t timeout_ms, void *force_cs);

/**
 * @brief  Abort interrupt mode of sending data 
 * @param[in]  obj: The address of log uart object.
 * @return the length of data sent to log uart.
 */
int32_t log_uart_send_stream_abort(log_uart_t *obj);

/**
 * @brief  Abort interrupt mode of receiving data 
 * @param[in]  obj: The address of log uart object.
 * @return the length of data received from log uart.
 */
int32_t log_uart_recv_stream_abort(log_uart_t *obj);

/**
 * @brief  Disable log uart
 * @param[in]  obj: The address of log uart object.
 * @return None.
 */
void log_uart_disable(log_uart_t *obj);

/**
 * @brief  Enable log uart
 * @param[in]  obj: The address of log uart object.
 * @return None.
 */
void log_uart_enable(log_uart_t *obj);

/**
 * @brief  Read Line-Status register
 * @return value:
 *         - Bit 0: RX Data Ready
 *         - Bit 1: Overrun Error
 *         - Bit 2: Parity Error
 *         - Bit 3: Framing Error
 *         - Bit 4: Break Interrupt (received data input is held in 0 state for a longer than a full word tx time)
 *         - Bit 5: TX FIFO empty (THR empty)
 *         - Bit 6: TX FIFO empty (THR & TSR both empty)
 *         - Bit 7: Receiver FIFO Error (parity error, framing error or break indication) 
 */
uint8_t log_uart_raed_lsr(log_uart_t *obj);

/**
 * @brief  Read Modem-Status register
 * @return value:
 *         - Bit 0: DCTS, The CTS line has changed its state
 *         - Bit 1: DDSR, The DSR line has changed its state
 *         - Bit 2: TERI, RI line has changed its state from low to high state
 *         - Bit 3: DDCD, DCD line has changed its state
 *         - Bit 4: Complement of the CTS input 
 *         - Bit 5: Complement of the DSR input 
 *         - Bit 6: Complement of the RI input 
 *         - Bit 7: Complement of the DCD input  
 */
uint8_t log_uart_raed_msr(log_uart_t *obj);

///@}
/*\@}*/

#ifdef __cplusplus
}
#endif

#endif //CONFIG_PLATFORM_8195A
#endif  // end of "#ifndef LOG_UART_API_H"

