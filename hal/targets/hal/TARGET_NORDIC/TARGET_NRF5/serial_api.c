/* 
 * Copyright (c) 2013 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "serial_api.h"

#if DEVICE_SERIAL

#include <string.h>
#include "mbed_assert.h"
#include "mbed_error.h"
#include "nrf_uart.h"
#include "nrf_drv_common.h"
#include "nrf_drv_config.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"

#define UART_INSTANCE_COUNT 1
#define UART_INSTANCE       NRF_UART0
#define UART_IRQn           UART0_IRQn
#define UART_IRQ_HANDLER    UART0_IRQHandler
#define UART_INSTANCE_ID    0
#define UART_CB uart_cb[UART_INSTANCE_ID]

#define UART_DEFAULT_BAUDRATE   UART0_CONFIG_BAUDRATE
#define UART_DEFAULT_PARITY     UART0_CONFIG_PARITY
#define UART_DEFAULT_HWFC       UART0_CONFIG_HWFC
#define UART_DEFAULT_CTS        UART0_CONFIG_PSEL_CTS
#define UART_DEFAULT_RTS        UART0_CONFIG_PSEL_RTS

// Required by "retarget.cpp".
int stdio_uart_inited = 0;
serial_t stdio_uart;

typedef struct {
    bool                initialized;
    uint32_t            irq_context;
    uart_irq_handler    irq_handler;

    uint32_t            pselrxd;
    uint32_t            pseltxd;
    uint32_t            pselcts;
    uint32_t            pselrts;
    nrf_uart_hwfc_t     hwfc;
    nrf_uart_parity_t   parity;
    nrf_uart_baudrate_t baudrate;

#if DEVICE_SERIAL_ASYNCH
    bool volatile       rx_active;
    uint8_t            *rx_buffer;
    size_t              rx_length;
    size_t              rx_pos;
    void              (*rx_asynch_handler)();
    uint8_t             char_match;

    bool volatile       tx_active;
    const uint8_t      *tx_buffer;
    size_t              tx_length;
    size_t              tx_pos;
    void              (*tx_asynch_handler)();

    uint32_t            events_wanted;
    uint32_t            events_occured;

    #define UART_IRQ_TX 1
    #define UART_IRQ_RX 2
    uint8_t             irq_enabled;
#endif // DEVICE_SERIAL_ASYNCH
} uart_ctlblock_t;

static uart_ctlblock_t uart_cb[UART_INSTANCE_COUNT];


#if DEVICE_SERIAL_ASYNCH
static void end_asynch_rx(void)
{
    // If RX interrupt is activated for synchronous operations,
    // don't disable it, just stop handling it here.
    if (!(UART_CB.irq_enabled & UART_IRQ_RX)) {
        nrf_uart_int_disable(UART_INSTANCE, NRF_UART_INT_MASK_RXDRDY);
    }
    UART_CB.rx_active = false;
}
static void end_asynch_tx(void)
{
    // If TX interrupt is activated for synchronous operations,
    // don't disable it, just stop handling it here.
    if (!(UART_CB.irq_enabled & UART_IRQ_TX)) {
        nrf_uart_int_disable(UART_INSTANCE, NRF_UART_INT_MASK_TXDRDY);
    }
    UART_CB.tx_active = false;
}
#endif // DEVICE_SERIAL_ASYNCH

void UART_IRQ_HANDLER(void)
{
    if (nrf_uart_int_enable_check(UART_INSTANCE, NRF_UART_INT_MASK_RXDRDY) &&
        nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_RXDRDY)) {

    #if DEVICE_SERIAL_ASYNCH
        if (UART_CB.rx_active) {
            nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_RXDRDY);

            uint8_t rx_data = nrf_uart_rxd_get(UART_INSTANCE);
            UART_CB.rx_buffer[UART_CB.rx_pos] = rx_data;

            bool end_rx = false;
            // If character matching should be performed, check if the current
            // data matches the given one.
            if (UART_CB.char_match != SERIAL_RESERVED_CHAR_MATCH &&
                rx_data == UART_CB.char_match) {
                // If it does, report the match and abort further receiving.
                UART_CB.events_occured |= SERIAL_EVENT_RX_CHARACTER_MATCH;
                if (UART_CB.events_wanted & SERIAL_EVENT_RX_CHARACTER_MATCH) {
                    end_rx = true;
                }
            }
            if (++UART_CB.rx_pos >= UART_CB.rx_length) {
                UART_CB.events_occured |= SERIAL_EVENT_RX_COMPLETE;
                end_rx = true;
            }
            if (end_rx) {
                end_asynch_rx();

                if (UART_CB.rx_asynch_handler) {
                    // Use local variable to make it possible to start a next
                    // transfer from callback routine.
                    void (*handler)() = UART_CB.rx_asynch_handler;
                    UART_CB.rx_asynch_handler = NULL;
                    handler();
                }
            }
        }
        else
    #endif

        if (UART_CB.irq_handler) {
            UART_CB.irq_handler(UART_CB.irq_context, RxIrq);
        }
    }

    if (nrf_uart_int_enable_check(UART_INSTANCE, NRF_UART_INT_MASK_TXDRDY) &&
        nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_TXDRDY)) {

    #if DEVICE_SERIAL_ASYNCH
        if (UART_CB.tx_active) {
            if (++UART_CB.tx_pos <= UART_CB.tx_length) {
                // When there is still something to send, clear the TXDRDY event
                // and put next byte to transmitter.
                nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_TXDRDY);
                nrf_uart_txd_set(UART_INSTANCE,
                    UART_CB.tx_buffer[UART_CB.tx_pos]);
            }
            else {
                // When the TXDRDY event is set after the last byte to be sent
                // has been passed to the transmitter, the job is done and TX
                // complete can be indicated.
                // Don't clear the TXDRDY event, it needs to remain set for the
                // 'serial_writable' function to work properly.
                end_asynch_tx();

                UART_CB.events_occured |= SERIAL_EVENT_TX_COMPLETE;
                if (UART_CB.tx_asynch_handler) {
                    // Use local variable to make it possible to start a next
                    // transfer from callback routine.
                    void (*handler)() = UART_CB.tx_asynch_handler;
                    UART_CB.tx_asynch_handler = NULL;
                    handler();
                }
            }
        }
        else
    #endif

        if (UART_CB.irq_handler) {
            UART_CB.irq_handler(UART_CB.irq_context, TxIrq);
        }
    }

#if DEVICE_SERIAL_ASYNCH
    if (nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_ERROR)) {
        nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_ERROR);

        uint8_t errorsrc = nrf_uart_errorsrc_get_and_clear(UART_INSTANCE);
        if (UART_CB.rx_asynch_handler) {
            UART_CB.events_occured |= SERIAL_EVENT_ERROR;
            if (errorsrc & NRF_UART_ERROR_PARITY_MASK) {
                UART_CB.events_occured |= SERIAL_EVENT_RX_PARITY_ERROR;
            }
            if (errorsrc & NRF_UART_ERROR_FRAMING_MASK) {
                UART_CB.events_occured |= SERIAL_EVENT_RX_FRAMING_ERROR;
            }
            if (errorsrc & NRF_UART_ERROR_OVERRUN_MASK) {
                UART_CB.events_occured |= SERIAL_EVENT_RX_OVERRUN_ERROR;
            }
            UART_CB.rx_asynch_handler();
        }
    }
#endif // DEVICE_SERIAL_ASYNCH
}

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    UART_CB.pseltxd =
        (tx == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)tx;
    UART_CB.pselrxd =
        (rx == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)rx;
    if (UART_CB.pseltxd != NRF_UART_PSEL_DISCONNECTED) {
        nrf_gpio_pin_set(UART_CB.pseltxd);
        nrf_gpio_cfg_output(UART_CB.pseltxd);
    }
    if (UART_CB.pselrxd != NRF_UART_PSEL_DISCONNECTED) {
        nrf_gpio_cfg_input(UART_CB.pselrxd, NRF_GPIO_PIN_NOPULL);
    }

    // UART pins must only be configured when the peripheral is disabled.
    nrf_uart_disable(UART_INSTANCE);

    if (UART_CB.initialized) {
        // Reconfigure RX/TX pins only.
        nrf_uart_txrx_pins_set(UART_INSTANCE, UART_CB.pseltxd, UART_CB.pselrxd);
        nrf_uart_enable(UART_INSTANCE);
    }
    else {
        UART_CB.baudrate = UART_DEFAULT_BAUDRATE;
        UART_CB.parity   = UART_DEFAULT_PARITY;
        UART_CB.hwfc     = UART_DEFAULT_HWFC;
        UART_CB.pselcts  = UART_DEFAULT_CTS;
        UART_CB.pselrts  = UART_DEFAULT_RTS;

        nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_RXDRDY);
        nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_TXDRDY);
        nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTRX);
        nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTTX);

        nrf_uart_int_disable(UART_INSTANCE, NRF_UART_INT_MASK_RXDRDY |
                                            NRF_UART_INT_MASK_TXDRDY);
    #if DEVICE_SERIAL_ASYNCH
        nrf_uart_int_enable(UART_INSTANCE, NRF_UART_INT_MASK_ERROR);
    #endif
        nrf_drv_common_irq_enable(UART_IRQn, APP_IRQ_PRIORITY_LOW);

        // TX interrupt needs to be signaled when transmitter buffer is empty,
        // so a dummy transmission is needed to get the TXDRDY event initially
        // set.
        nrf_uart_configure(UART_INSTANCE,
            NRF_UART_PARITY_EXCLUDED, NRF_UART_HWFC_DISABLED);
        // Use maximum baud rate, so this dummy transmission takes as little
        // time as possible.
        nrf_uart_baudrate_set(UART_INSTANCE, NRF_UART_BAUDRATE_1000000);
        // Perform it with disconnected TX pin, so nothing actually comes out
        // of the device.
        nrf_uart_txrx_pins_disconnect(UART_INSTANCE);
        nrf_uart_hwfc_pins_disconnect(UART_INSTANCE);
        nrf_uart_enable(UART_INSTANCE);
        nrf_uart_txd_set(UART_INSTANCE, 0);
        while (!nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_TXDRDY)) {
        }
        nrf_uart_disable(UART_INSTANCE);

        // Now everything is prepared to set the default configuration and
        // connect the peripheral to actual pins.
        nrf_uart_txrx_pins_set(UART_INSTANCE, UART_CB.pseltxd, UART_CB.pselrxd);
        nrf_uart_baudrate_set(UART_INSTANCE, UART_CB.baudrate);
        nrf_uart_configure(UART_INSTANCE, UART_CB.parity, UART_CB.hwfc);
        if (UART_CB.hwfc == NRF_UART_HWFC_ENABLED) {
            serial_set_flow_control(obj, FlowControlRTSCTS,
                UART_CB.pselrts, UART_CB.pselcts);
        }
        nrf_uart_enable(UART_INSTANCE);

        UART_CB.initialized = true;
    }

    if (tx == STDIO_UART_TX && rx == STDIO_UART_RX) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
    else {
        stdio_uart_inited = 0;
    }
}

void serial_free(serial_t *obj)
{
    (void)obj;

    if (UART_CB.initialized) {
        nrf_uart_disable(UART_INSTANCE);
        nrf_uart_int_disable(UART_INSTANCE, NRF_UART_INT_MASK_RXDRDY |
                                            NRF_UART_INT_MASK_TXDRDY |
                                            NRF_UART_INT_MASK_ERROR);
        nrf_drv_common_irq_disable(UART_IRQn);
        UART_CB.initialized = false;

        // There is only one UART instance, thus at this point the stdio UART
        // can no longer be initialized.
        stdio_uart_inited = 0;
    }
}

static nrf_uart_baudrate_t baud_translate(int rate)
{
    nrf_uart_baudrate_t baud;

    if (rate < 57600) {
        if (rate < 14400) {
            if (rate < 2400) {
                baud = NRF_UART_BAUDRATE_1200;
            } else if (rate < 4800) {
                baud = NRF_UART_BAUDRATE_2400;
            } else if (rate < 9600) {
                baud = NRF_UART_BAUDRATE_4800;
            } else {
                baud = NRF_UART_BAUDRATE_9600;
            }
        } else {
            if (rate < 19200) {
                baud = NRF_UART_BAUDRATE_14400;
            } else if (rate < 28800) {
                baud = NRF_UART_BAUDRATE_19200;
            } else if (rate < 38400) {
                baud = NRF_UART_BAUDRATE_28800;
            } else {
                baud = NRF_UART_BAUDRATE_38400;
            }
        }
    } else {
        if (rate < 250000) {
            if (rate < 76800) {
                baud = NRF_UART_BAUDRATE_57600;
            } else if (rate < 115200) {
                baud = NRF_UART_BAUDRATE_76800;
            } else if (rate < 230400) {
                baud = NRF_UART_BAUDRATE_115200;
            } else {
                baud = NRF_UART_BAUDRATE_230400;
            }
        } else {
            if (rate < 460800) {
                baud = NRF_UART_BAUDRATE_250000;
            } else if (rate < 921600) {
                baud = NRF_UART_BAUDRATE_460800;
            } else if (rate < 1000000) {
                baud = NRF_UART_BAUDRATE_921600;
            } else {
                baud = NRF_UART_BAUDRATE_1000000;
            }
        }
    }
    return baud;
}
void serial_baud(serial_t *obj, int baudrate)
{
    (void)obj;
    UART_CB.baudrate = baud_translate(baudrate);

    // Reconfigure UART peripheral.
    nrf_uart_baudrate_set(UART_INSTANCE, UART_CB.baudrate);
}

void serial_format(serial_t *obj,
                   int data_bits, SerialParity parity, int stop_bits)
{
    (void)obj;

    if (data_bits != 8) {
        error("UART supports only 8 data bits.\r\n");
    }
    if (stop_bits != 1) {
        error("UART supports only 1 stop bits.\r\n");
    }
    if (parity == ParityNone) {
        UART_CB.parity = NRF_UART_PARITY_EXCLUDED;
    } else if (parity == ParityEven) {
        UART_CB.parity = NRF_UART_PARITY_INCLUDED;
    } else {
        error("UART supports only even parity.\r\n");
    }

    // Reconfigure UART peripheral.
    nrf_uart_configure(UART_INSTANCE, UART_CB.parity, UART_CB.hwfc);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    (void)obj;
    UART_CB.irq_handler = handler;
    UART_CB.irq_context = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    (void)obj;
    if (enable) {
        switch (irq) {
            case RxIrq:
            #if DEVICE_SERIAL_ASYNCH
                UART_CB.irq_enabled |= UART_IRQ_RX;
            #endif
                nrf_uart_int_enable(UART_INSTANCE, NRF_UART_INT_MASK_RXDRDY);
                break;

            case TxIrq:
            #if DEVICE_SERIAL_ASYNCH
                UART_CB.irq_enabled |= UART_IRQ_TX;
            #endif
                nrf_uart_int_enable(UART_INSTANCE, NRF_UART_INT_MASK_TXDRDY);
                break;
        }
    } else {
        switch (irq) {
            case RxIrq:
            #if DEVICE_SERIAL_ASYNCH
                UART_CB.irq_enabled &= ~UART_IRQ_RX;
                if (!UART_CB.rx_active)
            #endif
                {
                    nrf_uart_int_disable(UART_INSTANCE,
                        NRF_UART_INT_MASK_RXDRDY);
                }
                break;

            case TxIrq:
            #if DEVICE_SERIAL_ASYNCH
                UART_CB.irq_enabled &= ~UART_IRQ_TX;
                if (!UART_CB.tx_active)
            #endif
                {
                    nrf_uart_int_disable(UART_INSTANCE,
                        NRF_UART_INT_MASK_TXDRDY);
                }
                break;
        }
    }
}

int serial_getc(serial_t *obj)
{
    while (!serial_readable(obj)) {
    }

    nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_RXDRDY);
    return nrf_uart_rxd_get(UART_INSTANCE);
}

void serial_putc(serial_t *obj, int c)
{
    while (!serial_writable(obj)) {
    }

    nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_TXDRDY);
    nrf_uart_txd_set(UART_INSTANCE, (uint8_t)c);
    // Wait until sending is completed.
    while (!nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_TXDRDY)) {
    }
}

int serial_readable(serial_t *obj)
{
    (void)obj;
#if DEVICE_SERIAL_ASYNCH
    if (UART_CB.rx_active) {
        return 0;
    }
#endif
    return (nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_RXDRDY));
}

int serial_writable(serial_t *obj)
{
    (void)obj;
#if DEVICE_SERIAL_ASYNCH
    if (UART_CB.tx_active) {
        return 0;
    }
#endif
    return (nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_TXDRDY));
}

void serial_break_set(serial_t *obj)
{
    (void)obj;
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_SUSPEND);
    nrf_uart_txrx_pins_disconnect(UART_INSTANCE);
    nrf_gpio_pin_clear(UART_CB.pseltxd);
}

void serial_break_clear(serial_t *obj)
{
    (void)obj;
    nrf_gpio_pin_set(UART_CB.pseltxd);
    nrf_uart_txrx_pins_set(UART_INSTANCE, UART_CB.pseltxd, UART_CB.pselrxd);
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTRX);
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTTX);
}

void serial_set_flow_control(serial_t *obj, FlowControl type,
                             PinName rxflow, PinName txflow)
{
    (void)obj;

    UART_CB.pselrts =
        (rxflow == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)rxflow;
    UART_CB.pselcts =
        (txflow == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)txflow;

    if (UART_CB.pselrts != NRF_UART_PSEL_DISCONNECTED) {
        nrf_gpio_pin_set(UART_CB.pselrts);
        nrf_gpio_cfg_output(UART_CB.pselrts);
    }
    if (UART_CB.pselcts != NRF_UART_PSEL_DISCONNECTED) {
        nrf_gpio_cfg_input(UART_CB.pselcts, NRF_GPIO_PIN_NOPULL);
    }
    nrf_uart_disable(UART_INSTANCE);
    nrf_uart_hwfc_pins_set(UART_INSTANCE, UART_CB.pselrts, UART_CB.pselcts);
    nrf_uart_enable(UART_INSTANCE);
}

void serial_clear(serial_t *obj) {
    (void)obj;
}

#if DEVICE_SERIAL_ASYNCH

int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length,
                     uint8_t tx_width, uint32_t handler, uint32_t event,
                     DMAUsage hint)
{
    (void)obj;
    (void)tx_width;
    (void)hint;
    if (UART_CB.tx_active || !tx_length) {
        return 0;
    }

    UART_CB.tx_buffer = tx;
    UART_CB.tx_length = tx_length;
    UART_CB.tx_pos    = 0;
    UART_CB.tx_asynch_handler = (void(*)())handler;
    UART_CB.events_wanted &= ~SERIAL_EVENT_TX_ALL;
    UART_CB.events_wanted |= event;

    UART_CB.tx_active = true;
    nrf_uart_int_enable(UART_INSTANCE, NRF_UART_INT_MASK_TXDRDY);

    return 0;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length,
                      uint8_t rx_width, uint32_t handler, uint32_t event,
                      uint8_t char_match, DMAUsage hint)
{
    (void)obj;
    (void)rx_width;
    (void)hint;
    if (UART_CB.rx_active || !rx_length) {
        return;
    }

    UART_CB.rx_buffer = rx;
    UART_CB.rx_length = rx_length;
    UART_CB.rx_pos    = 0;
    UART_CB.rx_asynch_handler = (void(*)())handler;
    UART_CB.events_wanted &= ~SERIAL_EVENT_RX_ALL;
    UART_CB.events_wanted |= event;
    UART_CB.char_match = char_match;

    UART_CB.rx_active = true;
    nrf_uart_int_enable(UART_INSTANCE, NRF_UART_INT_MASK_RXDRDY);
}

uint8_t serial_tx_active(serial_t *obj)
{
    (void)obj;
    return UART_CB.tx_active;
}

uint8_t serial_rx_active(serial_t *obj)
{
    (void)obj;
    return UART_CB.rx_active;
}

int serial_irq_handler_asynch(serial_t *obj)
{
    (void)obj;
    uint32_t events_to_report = UART_CB.events_wanted & UART_CB.events_occured;
    UART_CB.events_occured &= (~events_to_report);
    return events_to_report;
}

void serial_tx_abort_asynch(serial_t *obj)
{
    (void)obj;
    end_asynch_tx();
    UART_CB.tx_asynch_handler = NULL;
}

void serial_rx_abort_asynch(serial_t *obj)
{
    (void)obj;
    end_asynch_rx();
    UART_CB.rx_asynch_handler = NULL;
}

#endif // DEVICE_SERIAL_ASYNCH

#endif // DEVICE_SERIAL
