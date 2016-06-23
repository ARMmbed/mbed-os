/* mbed Microcontroller Library
 * Copyright (c) 2013 Nordic Semiconductor
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
#include <string.h>
#include "mbed_assert.h"
#include "mbed_error.h"
#include "serial_api.h"
#include "nrf_drv_uart.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"

#if DEVICE_SERIAL

#if DEVICE_SERIAL_ASYNCH
#define SERIAL_S(obj)   (&obj->serial)
#else
#define SERIAL_S(obj)   (obj)
#endif

#define UART_INSTANCE_COUNT    1
#define UART_INSTANCE          NRF_UART0
#define UART_INSTANCE_ID       0
#define UART_IRQn              UART0_IRQn
#define UART_CB uart_cb[UART_INSTANCE_ID]

int stdio_uart_inited = 0;
serial_t stdio_uart;

static nrf_drv_uart_config_t uart_config = NRF_DRV_UART_DEFAULT_CONFIG;

typedef struct {
    volatile bool     tx_active;
    volatile bool     rx_active;
    bool              async_mode;
    bool              initialized;
    uint8_t           irqs_enabled;
    uint32_t          irq_context;
    uart_irq_handler  irq_handler;
    uint32_t          registered_events;
    uint32_t          event_flags;
    void              (*async_handler)();
    uint8_t           *rx_buffer;
    uint8_t           rx_pos;
    uint8_t           rx_length;
} uart_ctlblock_t;

static uart_ctlblock_t uart_cb[UART_INSTANCE_COUNT];

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
    uart_config.baudrate = baud_translate(baudrate);

    // Reconfigure UART peripheral.
    nrf_uart_baudrate_set(UART_INSTANCE, uart_config.baudrate);
}

void uart_event_handler(nrf_drv_uart_event_t *p_event, void *p_context)
{
    (void)p_context;
    if (p_event->type == NRF_DRV_UART_EVT_TX_DONE) {
        UART_CB.tx_active = false;
        if (UART_CB.async_mode) {
            if (UART_CB.async_handler) {
                UART_CB.event_flags |= SERIAL_EVENT_TX_COMPLETE;
                UART_CB.async_handler();
            }
        }
        else {
            if (UART_CB.irqs_enabled & (1 << NRF_DRV_UART_EVT_TX_DONE)) {
                if (UART_CB.irq_handler) {
                    UART_CB.irq_handler(UART_CB.irq_context, TxIrq);
                }
            }
        }
    }
    if (p_event->type == NRF_DRV_UART_EVT_RX_DONE) {
        if (UART_CB.async_mode) {
            bool rx_end = true;
            if (UART_CB.registered_events & SERIAL_EVENT_RX_CHARACTER_MATCH) {
                serial_t *serial = (serial_t *)(uart_config.p_context);
                uint8_t *rx_buffer = (uint8_t *)(serial->rx_buff.buffer);
                uint8_t last_char = rx_buffer[serial->rx_buff.pos];

                ++serial->rx_buff.pos;
                if (last_char == serial->char_match) {
                    UART_CB.event_flags |= SERIAL_EVENT_RX_CHARACTER_MATCH;
                    serial->char_found = 1;
                }
                else {
                    if (serial->rx_buff.pos < serial->rx_buff.length) {
                        rx_end = false;
                        nrf_drv_uart_rx(&rx_buffer[serial->rx_buff.pos], 1);
                    }
                }
            }

            if (rx_end && UART_CB.async_handler) {
                UART_CB.rx_active = false;
                UART_CB.event_flags |= SERIAL_EVENT_RX_COMPLETE;
                UART_CB.async_handler();
            }
        }
        else {
            UART_CB.rx_active = false;
            if (UART_CB.irqs_enabled & (1 << NRF_DRV_UART_EVT_RX_DONE)) {
                if (UART_CB.irq_handler) {
                    UART_CB.irq_handler(UART_CB.irq_context, RxIrq);
                }
            }
        }
    }
    if (p_event->type == NRF_DRV_UART_EVT_ERROR) {
        if (UART_CB.async_mode && p_event->data.error.error_mask) {
            if (UART_CB.async_handler) {
                UART_CB.event_flags |= SERIAL_EVENT_ERROR;
                if (p_event->data.error.error_mask & NRF_UART_ERROR_PARITY_MASK) {
                    UART_CB.event_flags |= SERIAL_EVENT_RX_PARITY_ERROR;
                }
                if (p_event->data.error.error_mask & NRF_UART_ERROR_FRAMING_MASK) {
                    UART_CB.event_flags |= SERIAL_EVENT_RX_FRAMING_ERROR;
                }
                if (p_event->data.error.error_mask & NRF_UART_ERROR_OVERRUN_MASK) {
                    UART_CB.event_flags |= SERIAL_EVENT_RX_OVERRUN_ERROR;
                }
                UART_CB.async_handler();
            }
        }
    }
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    uart_config.pseltxd =
        (tx == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)tx;
    uart_config.pselrxd =
        (rx == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)rx;
    uart_config.p_context = (void *)obj;
    if (UART_CB.initialized) {
        // Reconfigure RX/TX pins only.
        nrf_uart_txrx_pins_set(UART_INSTANCE,
            uart_config.pseltxd, uart_config.pselrxd);
        return;
    }

    memset(&UART_CB, 0, sizeof(uart_ctlblock_t));
    ret_code_t err_code = nrf_drv_uart_init(&uart_config, uart_event_handler);
    if (err_code == NRF_SUCCESS) {
        UART_CB.initialized = true;
        nrf_drv_uart_rx_enable();

        if (tx == STDIO_UART_TX && rx == STDIO_UART_RX) {
            stdio_uart_inited = 1;
            memcpy(&stdio_uart, obj, sizeof(serial_t));
        }
    }
    else {
        error("UART init failure.");
    }
}

void serial_free(serial_t *obj)
{
    (void)obj;
    if (UART_CB.initialized) {
        nrf_drv_uart_uninit();
        UART_CB.initialized = false;

        stdio_uart_inited = 0;
    }
}

int serial_writable(serial_t *obj)
{
    (void)obj;
    if (!UART_CB.async_mode) {
        return true;
    }
    return !UART_CB.tx_active;
}

int serial_readable(serial_t *obj)
{
    (void)obj;
    return (!UART_CB.rx_active &&
            nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_RXDRDY));
}

void serial_putc(serial_t *obj, int c)
{
    (void)obj;

    // Interrupt on the TXDRDY event must be temporarily disabled, otherwise
    // the driver would try to handle (and clear) this event in the interrupt
    // handler.
    nrf_uart_int_disable(UART_INSTANCE, NRF_UART_INT_MASK_TXDRDY);

    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTTX);
    nrf_uart_txd_set(UART_INSTANCE, (uint8_t)c);
    while (!nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_TXDRDY)) {
    }
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STOPTX);

    nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_TXDRDY);
    nrf_uart_int_enable(UART_INSTANCE, NRF_UART_INT_MASK_TXDRDY);
}

int serial_getc(serial_t *obj)
{
    (void)obj;

    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTRX);
    while (!nrf_uart_event_check(UART_INSTANCE, NRF_UART_EVENT_RXDRDY)) {
    }
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STOPRX);
    nrf_uart_event_clear(UART_INSTANCE, NRF_UART_EVENT_RXDRDY);

    return nrf_uart_rxd_get(UART_INSTANCE);
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
                UART_CB.irqs_enabled |= (1 << NRF_DRV_UART_EVT_RX_DONE);
                break;
            case TxIrq:
                UART_CB.irqs_enabled |= (1 << NRF_DRV_UART_EVT_TX_DONE);
                break;
        }
    } else {
        switch (irq) {
            case RxIrq:
                UART_CB.irqs_enabled &= ~(1 << NRF_DRV_UART_EVT_RX_DONE);
                break;
            case TxIrq:
                UART_CB.irqs_enabled &= ~(1 << NRF_DRV_UART_EVT_TX_DONE);
                break;
        }
    }
}

#if DEVICE_SERIAL_ASYNCH

int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length,
                     uint8_t tx_width, uint32_t handler, uint32_t event,
                     DMAUsage hint)
{
    (void)obj;
    (void)tx_width;
    (void)hint;
    if (UART_CB.tx_active) {
        return 0;
    }
    // TX length is limited to 255 in uart driver.
    if (tx_length > 255) {
        tx_length = 255;
    }

    UART_CB.async_mode = true;
    UART_CB.tx_active = true;
    UART_CB.registered_events &= ~SERIAL_EVENT_TX_ALL;
    UART_CB.registered_events |= event;
    UART_CB.async_handler = (void(*)())handler;
    if (nrf_drv_uart_tx((uint8_t *)tx, (uint8_t)tx_length) == NRF_SUCCESS) {
        return tx_length;
    }
    return 0;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length,
                      uint8_t rx_width, uint32_t handler, uint32_t event,
                      uint8_t char_match, DMAUsage hint)
{
    (void)rx_width;
    (void)hint;
    if (UART_CB.rx_active || !rx_length) {
        return;
    }
    // RX length is limited to 255 in uart driver.
    if (rx_length > 255) {
        rx_length = 255;
    }

    UART_CB.async_mode = true;
    UART_CB.rx_active = true;
    if (char_match == SERIAL_RESERVED_CHAR_MATCH) {
        event &= ~SERIAL_EVENT_RX_CHARACTER_MATCH;
    }
    UART_CB.registered_events &= ~SERIAL_EVENT_RX_ALL;
    UART_CB.registered_events |= event;
    UART_CB.async_handler = (void(*)())handler;
    if (event & SERIAL_EVENT_RX_CHARACTER_MATCH) {
        obj->char_match = char_match;
        obj->rx_buff.buffer = rx;
        obj->rx_buff.length = rx_length;
        obj->rx_buff.pos    = 0;
        obj->rx_buff.width  = 8;
        nrf_drv_uart_rx((uint8_t *)rx, 1);
    } else {
        nrf_drv_uart_rx((uint8_t *)rx, (uint8_t)rx_length);
    }
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
    uint32_t result = UART_CB.registered_events & UART_CB.event_flags;
    UART_CB.event_flags &= (~result);
    return result;
}

void serial_tx_abort_asynch(serial_t *obj)
{
    (void)obj;
    nrf_drv_uart_tx_abort();
}

void serial_rx_abort_asynch(serial_t *obj)
{
    (void)obj;
    nrf_drv_uart_rx_abort();
}

#endif

void serial_format(serial_t *obj,
                   int data_bits, SerialParity parity, int stop_bits)
{
    (void)obj;
    (void)data_bits;
    (void)stop_bits;
    if (parity == ParityNone) {
        uart_config.parity = NRF_UART_PARITY_EXCLUDED;
    } else {
        uart_config.parity = NRF_UART_PARITY_INCLUDED;
    }

    // Reconfigure UART peripheral.
    nrf_uart_configure(UART_INSTANCE, uart_config.parity, uart_config.hwfc);
}

void serial_break_set(serial_t *obj)
{
    (void)obj;
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_SUSPEND);
    nrf_uart_txrx_pins_disconnect(UART_INSTANCE);
    nrf_gpio_pin_set(uart_config.pseltxd);
    nrf_gpio_pin_clear(uart_config.pseltxd);
}

void serial_break_clear(serial_t *obj)
{
    (void)obj;
    nrf_gpio_pin_set(uart_config.pseltxd);
    nrf_uart_txrx_pins_set(UART_INSTANCE,
        uart_config.pseltxd, uart_config.pselrxd);
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTTX);
    nrf_uart_task_trigger(UART_INSTANCE, NRF_UART_TASK_STARTRX);

}

void serial_set_flow_control(serial_t *obj,
                             FlowControl type, PinName rxflow, PinName txflow)
{
    (void)obj;
    if (type == FlowControlNone) {
        uart_config.hwfc = NRF_UART_HWFC_DISABLED;
        rxflow = NC;
        txflow = NC;
    } else {
        uart_config.hwfc = NRF_UART_HWFC_ENABLED;
        if (type == FlowControlRTS) {
            txflow = NC;
        } else if (type == FlowControlCTS) {
            rxflow = NC;
        }
    }
    uart_config.pselrts =
        (rxflow == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)rxflow;
    uart_config.pselcts =
        (txflow == NC) ? NRF_UART_PSEL_DISCONNECTED : (uint32_t)txflow;

    // Reconfigure UART peripheral.
    if (uart_config.hwfc == NRF_UART_HWFC_ENABLED) {
        nrf_gpio_cfg_input(uart_config.pselcts, NRF_GPIO_PIN_NOPULL);
        nrf_gpio_pin_set(uart_config.pselrts);
        nrf_gpio_cfg_output(uart_config.pselrts);
        nrf_uart_hwfc_pins_set(UART_INSTANCE,
            uart_config.pselrts, uart_config.pselcts);
    }
    nrf_uart_configure(UART_INSTANCE, uart_config.parity, uart_config.hwfc);
}

void serial_clear(serial_t *obj)
{
    (void)obj;
}

#endif // DEVICE_SERIAL
