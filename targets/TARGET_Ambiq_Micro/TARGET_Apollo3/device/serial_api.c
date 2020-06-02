
/** \addtogroup hal */
/** @{*/
/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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

#if DEVICE_SERIAL

#include "serial_api.h"

#include "mbed_assert.h"
#include "PeripheralPins.h"

// globals?
int stdio_uart_inited = 0;
serial_t stdio_uart;
bool value = false;

// interrupt variables
static uart_irq_handler irq_handler;
static ap3_uart_control_t ap3_uart_control[AM_REG_UART_NUM_MODULES];

// forward declarations
extern void am_uart_isr(void);
extern void am_uart1_isr(void);
void uart_configure_pin_function(PinName pin, UARTName uart, const PinMap *map);

/**
 * \defgroup hal_GeneralSerial Serial Configuration Functions
 *
 * # Defined behavior
 * * ::serial_init initializes the ::serial_t
 * * ::serial_init sets the default parameters for serial peripheral (9600 bps, 8N1 format)
 * * ::serial_init configures the specified pins
 * * ::serial_free releases the serial peripheral
 * * ::serial_baud configures the baud rate
 * * at least 9600 bps the baud rate  must be supported
 * * ::serial_format configures the transmission format (number of bits, parity and the number of stop bits)
 * * at least 8N1 format must be supported
 * * ::serial_irq_handler registers the interrupt handler which will be invoked when the interrupt fires.
 * * ::serial_irq_set enables or disables the serial RX or TX IRQ.
 * * If `RxIrq` is enabled by ::serial_irq_set, ::serial_irq_handler will be invoked whenever
 * Receive Data Register Full IRQ is generated.
 * * If `TxIrq` is enabled by ::serial_irq_set, ::serial_irq_handler will be invoked whenever
 * Transmit Data Register Empty IRQ is generated.
 * * If the interrupt condition holds true, when the interrupt is enabled with ::serial_irq_set,
 * the ::serial_irq_handler is called instantly.
 * * ::serial_getc returns the character from serial buffer.
 * * ::serial_getc is a blocking call (waits for the character).
 * * ::serial_putc sends a character.
 * * ::serial_putc is a blocking call (waits for a peripheral to be available).
 * * ::serial_readable returns non-zero value if a character can be read, 0 otherwise.
 * * ::serial_writable returns non-zero value if a character can be written, 0 otherwise.
 * * ::serial_clear clears the ::serial_t RX/TX buffers
 * * ::serial_break_set sets the break signal.
 * * ::serial_break_clear clears the break signal.
 * * ::serial_pinout_tx configures the TX pin as an output (to be used in half-duplex mode).
 * * ::serial_set_flow_control configures serial flow control.
 * * ::serial_set_flow_control sets flow control in the hardware if a serial peripheral supports it,
 * otherwise software emulation is used.
 * * ::serial_tx_asynch starts the serial asynchronous transfer.
 * * ::serial_tx_asynch writes `tx_length` bytes from the `tx` to the bus.
 * * ::serial_tx_asynch must support 8 data bits
 * * The callback given to ::serial_tx_asynch is invoked when the transfer completes.
 * * ::serial_tx_asynch specifies the logical OR of events to be registered.
 * * The ::serial_tx_asynch function may use the `DMAUsage` hint to select the appropriate async algorithm.
 * * ::serial_rx_asynch starts the serial asynchronous transfer.
 * * ::serial_rx_asynch reads `rx_length` bytes to the `rx` buffer.
 * * ::serial_rx_asynch must support 8 data bits
 * * The callback given to ::serial_rx_asynch is invoked when the transfer completes.
 * * ::serial_rx_asynch specifies the logical OR of events to be registered.
 * * The ::serial_rx_asynch function may use the `DMAUsage` hint to select the appropriate async algorithm.
 * * ::serial_rx_asynch specifies a character in range 0-254 to be matched, 255 is a reserved value.
 * * If SERIAL_EVENT_RX_CHARACTER_MATCH event is not registered, the `char_match` is ignored.
 * * The SERIAL_EVENT_RX_CHARACTER_MATCH event is set in the callback when SERIAL_EVENT_RX_CHARACTER_MATCH event is
 * registered AND `char_match` is present in the received data.
 * * ::serial_tx_active returns non-zero if the TX transaction is ongoing, 0 otherwise.
 * * ::serial_rx_active returns non-zero if the RX transaction is ongoing, 0 otherwise.
 * * ::serial_irq_handler_asynch returns event flags if a transfer termination condition was met, otherwise returns 0.
 * * ::serial_irq_handler_asynch takes no longer than one packet transfer time (packet_bits / baudrate) to execute.
 * * ::serial_tx_abort_asynch aborts the ongoing TX transaction.
 * * ::serial_tx_abort_asynch disables the enabled interupt for TX.
 * * ::serial_tx_abort_asynch flushes the TX hardware buffer if TX FIFO is used.
 * * ::serial_rx_abort_asynch aborts the ongoing RX transaction.
 * * ::serial_rx_abort_asynch disables the enabled interupt for RX.
 * * ::serial_rx_abort_asynch flushes the TX hardware buffer if RX FIFO is used.
 * * Correct operation guaranteed when interrupt latency is shorter than one packet transfer time (packet_bits / baudrate)
 * if the flow control is not used.
 * * Correct operation guaranteed regardless of interrupt latency if the flow control is used.
 *
 * # Undefined behavior
 * * Calling ::serial_init multiple times on the same `serial_t` without ::serial_free.
 * * Passing invalid pin to ::serial_init, ::serial_pinout_tx.
 * * Calling any function other than ::serial_init on am uninitialized or freed `serial_t`.
 * * Passing an invalid pointer as `obj` to any function.
 * * Passing an invalid pointer as `handler` to ::serial_irq_handler, ::serial_tx_asynch, ::serial_rx_asynch.
 * * Calling ::serial_tx_abort while no async TX transfer is being processed.
 * * Calling ::serial_rx_abort while no async RX transfer is being processed.
 * * Devices behavior is undefined when the interrupt latency is longer than one packet transfer time
 * (packet_bits / baudrate) if the flow control is not used.
 * @{
 */

/**
 * \defgroup hal_GeneralSerial_tests Serial hal tests
 * The Serial HAL tests ensure driver conformance to defined behavior.
 *
 * To run the Serial hal tests use the command:
 *
 *     mbed test -t <toolchain> -m <target> -n tests-mbed_hal_fpga_ci_test_shield-uart
 *
 */

/** Initialize the serial peripheral. It sets the default parameters for serial
 *  peripheral, and configures its specifieds pins.
 *
 * @param obj The serial object
 * @param tx  The TX pin name
 * @param rx  The RX pin name
 */
void serial_init(serial_t *obj, PinName tx, PinName rx)
{ //TODO: we should be able to call this multiple times
  // determine the UART to use
  UARTName uart_tx = (UARTName)pinmap_peripheral(tx, serial_tx_pinmap());
  UARTName uart_rx = (UARTName)pinmap_peripheral(rx, serial_rx_pinmap());
  UARTName uart = (UARTName)pinmap_merge(uart_tx, uart_rx);
  MBED_ASSERT((int)uart != NC);
  obj->serial.uart_control = &ap3_uart_control[uart];
  obj->serial.uart_control->inst = uart;

  // ensure that HAL queueing is disabled (we want to use the FIFOs directly)
  obj->serial.uart_control->cfg.pui8RxBuffer = NULL;
  obj->serial.uart_control->cfg.pui8TxBuffer = NULL;
  obj->serial.uart_control->cfg.ui32RxBufferSize = 0;
  obj->serial.uart_control->cfg.ui32TxBufferSize = 0;

  // memset(obj->serial.uart_control->cfg, 0x00, sizeof(am_hal_uart_config_t)); // ensure config begins zeroed

  // // pinout the chosen uart // todo: better????
  // pinmap_pinout(tx, serial_tx_pinmap());
  // pinmap_pinout(rx, serial_rx_pinmap());
  uart_configure_pin_function(tx, uart, serial_tx_pinmap());
  uart_configure_pin_function(rx, uart, serial_rx_pinmap());

  // start UART instance
  MBED_ASSERT(am_hal_uart_initialize(uart, &(obj->serial.uart_control->handle)) == AM_HAL_STATUS_SUCCESS);
  MBED_ASSERT(am_hal_uart_power_control(obj->serial.uart_control->handle, AM_HAL_SYSCTRL_WAKE, false) == AM_HAL_STATUS_SUCCESS);
  MBED_ASSERT(am_hal_uart_configure(obj->serial.uart_control->handle, &(obj->serial.uart_control->cfg)) == AM_HAL_STATUS_SUCCESS);

  // set default baud rate and format
  serial_baud(obj, 9600);
  serial_format(obj, 8, ParityNone, 1);
}

/** Release the serial peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param obj The serial object
 */
void serial_free(serial_t *obj)
{
  // nothing to do unless resources are allocated for members of the serial_s serial member of obj
  // assuming mbed handles obj and its members
}

/** Configure the baud rate
 *
 * @param obj      The serial object
 * @param baudrate The baud rate to be configured
 */
void serial_baud(serial_t *obj, int baudrate)
{
  obj->serial.uart_control->cfg.ui32BaudRate = (uint32_t)baudrate;
  MBED_ASSERT(am_hal_uart_configure(obj->serial.uart_control->handle, &(obj->serial.uart_control->cfg)) == AM_HAL_STATUS_SUCCESS);
}

/** Configure the format. Set the number of bits, parity and the number of stop bits
 *
 * @param obj       The serial object
 * @param data_bits The number of data bits
 * @param parity    The parity
 * @param stop_bits The number of stop bits
 */
void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
  uint32_t am_hal_data_bits = 0;
  switch (data_bits)
  {
  case 5:
    am_hal_data_bits = AM_HAL_UART_DATA_BITS_5;
    break;
  case 6:
    am_hal_data_bits = AM_HAL_UART_DATA_BITS_6;
    break;
  case 7:
    am_hal_data_bits = AM_HAL_UART_DATA_BITS_7;
    break;
  case 8:
    am_hal_data_bits = AM_HAL_UART_DATA_BITS_8;
    break;
  default:
    MBED_ASSERT(0);
    break;
  }

  uint32_t am_hal_parity = AM_HAL_UART_PARITY_NONE;
  switch (parity)
  {
  case ParityNone:
    am_hal_parity = AM_HAL_UART_PARITY_NONE;
    break;
  case ParityOdd:
    am_hal_parity = AM_HAL_UART_PARITY_ODD;
    break;
  case ParityEven:
    am_hal_parity = AM_HAL_UART_PARITY_EVEN;
    break;
  default: // fall-through intentional after default
  case ParityForced1:
  case ParityForced0:
    MBED_ASSERT(0);
    break;
  }

  uint32_t am_hal_stop_bits = 0;
  switch (stop_bits)
  {
  case 1:
    am_hal_stop_bits = AM_HAL_UART_ONE_STOP_BIT;
    break;
  case 2:
    am_hal_stop_bits = AM_HAL_UART_TWO_STOP_BITS;
    break;
  default:
    MBED_ASSERT(0);
  }

  obj->serial.uart_control->cfg.ui32DataBits = (uint32_t)am_hal_data_bits;
  obj->serial.uart_control->cfg.ui32Parity = (uint32_t)am_hal_parity;
  obj->serial.uart_control->cfg.ui32StopBits = (uint32_t)am_hal_stop_bits;
  MBED_ASSERT(am_hal_uart_configure(obj->serial.uart_control->handle, &(obj->serial.uart_control->cfg)) == AM_HAL_STATUS_SUCCESS);
}

/** The serial interrupt handler registration
 *
 * @param obj     The serial object
 * @param handler The interrupt handler which will be invoked when the interrupt fires
 * @param id      The SerialBase object
 */
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
  irq_handler = handler;
  obj->serial.uart_control->serial_irq_id = id;
  // todo: revisit with Kyle
}

/** Configure serial interrupt. This function is used for word-approach
 *
 * @param obj    The serial object
 * @param irq    The serial IRQ type (RX or TX)
 * @param enable Set to non-zero to enable events, or zero to disable them
 */
void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);

  am_hal_uart_interrupt_enable(obj->serial.uart_control->handle, (AM_HAL_UART_INT_RX | AM_HAL_UART_INT_TX | AM_HAL_UART_INT_TXCMP));

  switch (obj->serial.uart_control->inst)
  {
  case 0:
    NVIC_SetVector((IRQn_Type)UART0_IRQn, (uint32_t)am_uart_isr);
    break;
  case 1:
    NVIC_SetVector((IRQn_Type)UART1_IRQn, (uint32_t)am_uart1_isr);
    break;
  }

  NVIC_EnableIRQ((IRQn_Type)(UART0_IRQn + obj->serial.uart_control->inst));
}

/** Get character. This is a blocking call, waiting for a character
 *
 * @param obj The serial object
 */
int serial_getc(serial_t *obj)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);

  uint8_t rx_c = 0x00;
  volatile uint32_t bytes_read = 0x00;
  am_hal_uart_transfer_t am_hal_uart_xfer_read_single =
      {
          .ui32Direction = AM_HAL_UART_READ,
          .pui8Data = (uint8_t *)&rx_c,
          .ui32NumBytes = 1,
          .ui32TimeoutMs = 0,
          .pui32BytesTransferred = (uint32_t *)&bytes_read,
      };

  do
  {
    am_hal_uart_transfer(obj->serial.uart_control->handle, &am_hal_uart_xfer_read_single);
  } while (bytes_read == 0);

  return (int)rx_c;
}

/** Send a character. This is a blocking call, waiting for a peripheral to be available
 *  for writing
 *
 * @param obj The serial object
 * @param c   The character to be sent
 */
void serial_putc(serial_t *obj, int c)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);

  volatile uint32_t bytes_sent = 0;
  am_hal_uart_transfer_t am_hal_uart_xfer_write_single =
      {
          .ui32Direction = AM_HAL_UART_WRITE,
          .pui8Data = (uint8_t *)(&c),
          .ui32NumBytes = 1,
          .ui32TimeoutMs = 0,
          .pui32BytesTransferred = (uint32_t *)&bytes_sent,
      };

  do
  {
    am_hal_uart_transfer(obj->serial.uart_control->handle, &am_hal_uart_xfer_write_single);
  } while (bytes_sent == 0);
}

/** Check if the serial peripheral is readable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be read, 0 if nothing to read
 */
int serial_readable(serial_t *obj)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);
  return !(UARTn(obj->serial.uart_control->inst)->FR_b.RXFE);
}

/** Check if the serial peripheral is writable
 *
 * @param obj The serial object
 * @return Non-zero value if a character can be written, 0 otherwise.
 */
int serial_writable(serial_t *obj)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);
  return !(UARTn(obj->serial.uart_control->inst)->FR_b.TXFF);
}

/** Clear the serial peripheral
 *
 * @param obj The serial object
 */
void serial_clear(serial_t *obj)
{
  MBED_ASSERT(0); // todo: WTF is this?
}

/** Set the break
 *
 * @param obj The serial object
 */
void serial_break_set(serial_t *obj)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);
  UARTn(obj->serial.uart_control->inst)->LCRH |= UART0_LCRH_BRK_Msk;
}

/** Clear the break
 *
 * @param obj The serial object
 */
void serial_break_clear(serial_t *obj)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);
  UARTn(obj->serial.uart_control->inst)->LCRH &= ~UART0_LCRH_BRK_Msk;
}

/** Configure the TX pin for UART function.
 *
 * @param tx The pin name used for TX
 */
void serial_pinout_tx(PinName tx)
{
  MBED_ASSERT(0); // todo: (this seems like a vestigial function)
}

#if DEVICE_SERIAL_FC
/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * @param obj    The serial object
 * @param type   The type of the flow control. Look at the available FlowControl types.
 * @param rxflow The TX pin name
 * @param txflow The RX pin name
 */
void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
  // todo:
}

/** Configure the serial for the flow control. It sets flow control in the hardware
 *  if a serial peripheral supports it, otherwise software emulation is used.
 *
 * @param obj    The serial object
 * @param type   The type of the flow control. Look at the available FlowControl types.
 * @param pinmap Pointer to structure which holds static pinmap
 */
void serial_set_flow_control_direct(serial_t *obj, FlowControl type, const serial_fc_pinmap_t *pinmap)
{
  // todo:
}
#endif

/** Get the pins that support Serial TX
 *
 * Return a PinMap array of pins that support Serial TX. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_tx_pinmap(void)
{
  return PinMap_UART_TX;
}

/** Get the pins that support Serial RX
 *
 * Return a PinMap array of pins that support Serial RX. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_rx_pinmap(void)
{
  return PinMap_UART_RX;
}

#if DEVICE_SERIAL_FC
/** Get the pins that support Serial CTS
 *
 * Return a PinMap array of pins that support Serial CTS. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_cts_pinmap(void)
{
  return PinMap_UART_CTS;
}

/** Get the pins that support Serial RTS
 *
 * Return a PinMap array of pins that support Serial RTS. The
 * array is terminated with {NC, NC, 0}.
 *
 * @return PinMap array
 */
const PinMap *serial_rts_pinmap(void)
{
  return PinMap_UART_RTS;
}
#endif

#if DEVICE_SERIAL_ASYNCH

/**@}*/

/**
 * \defgroup hal_AsynchSerial Asynchronous Serial Hardware Abstraction Layer
 * @{
 */

/** Begin asynchronous TX transfer. The used buffer is specified in the serial object,
 *  tx_buff
 *
 * @param obj       The serial object
 * @param tx        The transmit buffer
 * @param tx_length The number of bytes to transmit
 * @param tx_width  Deprecated argument
 * @param handler   The serial handler
 * @param event     The logical OR of events to be registered
 * @param hint      A suggestion for how to use DMA with this transfer
 * @return Returns number of data transfered, otherwise returns 0
 */
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
  MBED_ASSERT(obj->serial.uart_control != NULL);
  uint32_t bytes_written = 0;

  am_hal_uart_transfer_t am_hal_uart_xfer_write =
      {
          .ui32Direction = AM_HAL_UART_WRITE,
          .pui8Data = (uint8_t *)obj->tx_buff.buffer,
          .ui32NumBytes = tx_length, // todo: consider maybe this? (uint32_t)obj->tx_buff.length,
          .ui32TimeoutMs = 0,
          .pui32BytesTransferred = &bytes_written,
      };

  am_hal_uart_transfer(obj->serial.uart_control->handle, &am_hal_uart_xfer_write);

  return (int)bytes_written;
}

/** Begin asynchronous RX transfer (enable interrupt for data collecting)
 *  The used buffer is specified in the serial object - rx_buff
 *
 * @param obj        The serial object
 * @param rx         The receive buffer
 * @param rx_length  The number of bytes to receive
 * @param rx_width   Deprecated argument
 * @param handler    The serial handler
 * @param event      The logical OR of events to be registered
 * @param char_match A character in range 0-254 to be matched
 * @param hint       A suggestion for how to use DMA with this transfer
 */
void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
  // todo: revisit
  MBED_ASSERT(obj->serial.uart_control != NULL);
  uint32_t bytes_read = 0;

  am_hal_uart_transfer_t am_hal_uart_xfer_read =
      {
          .ui32Direction = AM_HAL_UART_READ,
          .pui8Data = (uint8_t *)obj->rx_buff.buffer,
          .ui32NumBytes = rx_length, // todo: consider this (uint32_t)obj->rx_buff.length,
          .ui32TimeoutMs = 0,
          .pui32BytesTransferred = &bytes_read,
      };

  am_hal_uart_transfer(obj->serial.uart_control->handle, &am_hal_uart_xfer_read);
}

/** Attempts to determine if the serial peripheral is already in use for TX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_tx_active(serial_t *obj)
{
  // todo:
}

/** Attempts to determine if the serial peripheral is already in use for RX
 *
 * @param obj The serial object
 * @return Non-zero if the RX transaction is ongoing, 0 otherwise
 */
uint8_t serial_rx_active(serial_t *obj)
{
  // todo:
}

/** The asynchronous TX and RX handler.
 *
 * @param obj The serial object
 * @return Returns event flags if an RX transfer termination condition was met; otherwise returns 0
 */
int serial_irq_handler_asynch(serial_t *obj)
{
  // todo:
}

/** Abort the ongoing TX transaction. It disables the enabled interupt for TX and
 *  flushes the TX hardware buffer if TX FIFO is used
 *
 * @param obj The serial object
 */
void serial_tx_abort_asynch(serial_t *obj)
{
  // todo:
}

/** Abort the ongoing RX transaction. It disables the enabled interrupt for RX and
 *  flushes the RX hardware buffer if RX FIFO is used
 *
 * @param obj The serial object
 */
void serial_rx_abort_asynch(serial_t *obj)
{
  // todo:
}

/**@}*/

#endif

static inline void uart_irq(uint32_t instance)
{
  void *handle = ap3_uart_control[instance].handle;
  MBED_ASSERT(handle != NULL);

  // check flags
  uint32_t status = 0x00;
  MBED_ASSERT(am_hal_uart_interrupt_status_get(handle, &status, true) == AM_HAL_STATUS_SUCCESS);
  MBED_ASSERT(am_hal_uart_interrupt_clear(handle, status) == AM_HAL_STATUS_SUCCESS);

  if (ap3_uart_control[instance].serial_irq_id != 0)
  {
    if (status & AM_HAL_UART_INT_TXCMP)
    { // for transmit complete
      if (irq_handler)
      {
        irq_handler(ap3_uart_control[instance].serial_irq_id, TxIrq);
      }
    }
    if (status & AM_HAL_UART_INT_RX)
    { // for receive complete
      if (irq_handler)
      {
        irq_handler(ap3_uart_control[instance].serial_irq_id, RxIrq);
      }
    }
  }
}

extern void am_uart_isr(void)
{
  // void* handle = &(g_am_hal_uart_states[0]) // bad - breaks barrier between AMHAL and mbed
  // value = !value;
  // (value) ? am_hal_gpio_output_set(16) : am_hal_gpio_output_clear(16);

  am_hal_gpio_output_set(16);

  uart_irq(UART_0);
}

extern void am_uart1_isr(void)
{
  // void* handle = &(g_am_hal_uart_states[1]) // bad - breaks barrier between AMHAL and mbed
  uart_irq(UART_1);
}

void uart_configure_pin_function(PinName pin, UARTName uart, const PinMap *map)
{
  while (map->pin != NC)
  {
    if (map->peripheral == uart)
    {
      if (map->pin == pin)
      {
        // pin_function(pin, map->function);

        am_hal_gpio_pincfg_t cfg =
            {
                .uFuncSel = map->function,
                // .eDriveStrength = (tx) ? AM_HAL_GPIO_PIN_DRIVESTRENGTH_2MA : 0,
            };

        am_hal_gpio_pinconfig(pin, cfg);
        break;
      }
    }
    map++;
  }
}

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
