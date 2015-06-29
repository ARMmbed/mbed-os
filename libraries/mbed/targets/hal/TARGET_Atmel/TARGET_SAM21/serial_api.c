/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#include "cmsis.h"
#include "serial_api.h"
#include "pinmap.h"
#include "PeripheralPins.h"
#include "usart.h"
#include "samr21_xplained_pro.h"

#if DEVICE_SERIAL_ASYNCH
#define pUSART_S(obj)			obj->serial.usart
#define pSERIAL_S(obj)			((struct serial_s*)&(obj->serial))
#else
#define pUSART_S(obj)			obj->serial
#define pSERIAL_S(obj)			((struct serial_s*)obj)
#endif
#define _USART(obj)			pUSART_S(obj)->USART
#define USART_NUM 6


uint8_t serial_get_index(serial_t *obj);
IRQn_Type get_serial_irq_num (serial_t *obj);

static uint32_t serial_irq_ids[USART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

extern uint8_t g_sys_init;

static inline bool usart_syncing(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    return (_USART(obj).SYNCBUSY.reg);
}

static inline void enable_usart(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Enable USART module */
    _USART(obj).CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}

static inline void disable_usart(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Disable USART module */
    _USART(obj).CTRLA.reg &= ~SERCOM_USART_CTRLA_ENABLE;
}

static inline void reset_usart(serial_t *obj)
{
    /* Sanity check arguments */
    MBED_ASSERT(obj);

    disable_usart(obj);

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Reset module */
    _USART(obj).CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
}

static enum status_code usart_set_config_default( serial_t *obj)
{

    /* Index for generic clock */
    uint32_t sercom_index = _sercom_get_sercom_inst_index(pUSART_S(obj));
    uint32_t gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;

    /* Cache new register values to minimize the number of register writes */
    uint32_t ctrla = 0;
    uint32_t ctrlb = 0;
    uint16_t baud  = 0;

    enum sercom_asynchronous_operation_mode mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
    enum sercom_asynchronous_sample_num sample_num = SERCOM_ASYNC_SAMPLE_NUM_16;

    /* Set data order, internal muxing, and clock polarity */
    ctrla = (uint32_t)USART_DATAORDER_LSB |         // data order
            (uint32_t)pSERIAL_S(obj)->mux_setting;  // mux setting  // clock polarity is not used


    /* Get baud value from mode and clock */
    _sercom_get_async_baud_val(pSERIAL_S(obj)->baudrate,system_gclk_chan_get_hz(gclk_index), &baud, mode, sample_num);  // for asynchronous transfer mode

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /*Set baud val */
    _USART(obj).BAUD.reg = baud;

    /* Set sample mode */
    ctrla |= USART_TRANSFER_ASYNCHRONOUSLY;

    /* for disabled external clock source */
    ctrla |= SERCOM_USART_CTRLA_MODE(0x1);

    /* Set stopbits, character size and enable transceivers */
    ctrlb = (uint32_t)pSERIAL_S(obj)->stopbits | (uint32_t)pSERIAL_S(obj)->character_size |
            (0x1ul << SERCOM_USART_CTRLB_RXEN_Pos) |   // receiver enable
            (0x1ul << SERCOM_USART_CTRLB_TXEN_Pos);  // transmitter enable

    /* Check parity mode bits */
    if (pSERIAL_S(obj)->parity != USART_PARITY_NONE) {
        ctrla |= SERCOM_USART_CTRLA_FORM(1);
        ctrlb |= pSERIAL_S(obj)->parity;
    } else {
        ctrla |= SERCOM_USART_CTRLA_FORM(0);
    }

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Write configuration to CTRLB */
    _USART(obj).CTRLB.reg = ctrlb;

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Write configuration to CTRLA */
    _USART(obj).CTRLA.reg = ctrla;

    return STATUS_OK;
}

void get_default_serial_values(serial_t *obj)
{
    /* Set default config to object */
    pSERIAL_S(obj)->parity = USART_PARITY_NONE;
    pSERIAL_S(obj)->stopbits = USART_STOPBITS_1;
    pSERIAL_S(obj)->character_size = USART_CHARACTER_SIZE_8BIT;
    pSERIAL_S(obj)->baudrate = 9600;
    pSERIAL_S(obj)->mux_setting = USART_RX_1_TX_2_XCK_3;
    pSERIAL_S(obj)->pinmux_pad0 = PINMUX_DEFAULT;
    pSERIAL_S(obj)->pinmux_pad1 = PINMUX_DEFAULT;
    pSERIAL_S(obj)->pinmux_pad2 = PINMUX_DEFAULT;
    pSERIAL_S(obj)->pinmux_pad3 = PINMUX_DEFAULT;
};

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    if (g_sys_init == 0) {
        system_init();
        g_sys_init = 1;
    }

    struct system_gclk_chan_config gclk_chan_conf;
    UARTName uart;
    uint32_t gclk_index;
    uint32_t pm_index;
    uint32_t sercom_index = 0;
    uint32_t muxsetting = 0;
    uint32_t padsetting[4] = {0};

    /* Disable USART module */
    disable_usart(obj);

    get_default_serial_values(obj);

    find_pin_settings(tx, rx, NC, NC, &padsetting[0]);  // tx, rx, clk(rts), chipsel(cts) pad array  // getting pads from pins
    muxsetting = find_mux_setting(tx, rx, NC, NC);  // getting mux setting from pins
    sercom_index = pinmap_sercom_peripheral(tx, rx);  // same variable sercom_index reused for optimization
    switch (sercom_index) {
        case 0:
            uart = UART_0;
            pUSART_S(obj) = SERCOM0;
            break;
        case 1:
            uart = UART_1;
            pUSART_S(obj) = SERCOM1;
            break;
        case 2:
            uart = UART_2;
            pUSART_S(obj) = SERCOM2;
            break;
        case 3:
            uart = UART_3;
            pUSART_S(obj) = SERCOM3;
            break;
        case 4:
            uart = UART_4;
            pUSART_S(obj) = SERCOM4;
            break;
        case 5:
            uart = UART_5;
            pUSART_S(obj) = SERCOM5;
            break;
    }

    pSERIAL_S(obj)->txpin = tx;
    pSERIAL_S(obj)->rxpin = rx;
    pSERIAL_S(obj)->mux_setting = muxsetting;//EDBG_CDC_SERCOM_MUX_SETTING;
    pSERIAL_S(obj)->pinmux_pad0 = padsetting[0];//EDBG_CDC_SERCOM_PINMUX_PAD0;
    pSERIAL_S(obj)->pinmux_pad1 = padsetting[1];//EDBG_CDC_SERCOM_PINMUX_PAD1;
    pSERIAL_S(obj)->pinmux_pad2 = padsetting[2];//EDBG_CDC_SERCOM_PINMUX_PAD2;
    pSERIAL_S(obj)->pinmux_pad3 = padsetting[3];//EDBG_CDC_SERCOM_PINMUX_PAD3;

    pm_index     = sercom_index + PM_APBCMASK_SERCOM0_Pos;
    gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;

    if (_USART(obj).CTRLA.reg & SERCOM_USART_CTRLA_SWRST) {
        /* The module is busy resetting itself */
    }

    if (_USART(obj).CTRLA.reg & SERCOM_USART_CTRLA_ENABLE) {
        /* Check the module is enabled */
    }

    /* Turn on module in PM */
    system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);

    /* Set up the GCLK for the module */
    gclk_chan_conf.source_generator = GCLK_GENERATOR_0;
    system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
    system_gclk_chan_enable(gclk_index);
    sercom_set_gclk_generator(GCLK_GENERATOR_0, false);

    /* Set configuration according to the config struct */
    usart_set_config_default(obj);
    struct system_pinmux_config pin_conf;
    system_pinmux_get_config_defaults(&pin_conf);
    pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
    pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;

    uint32_t pad_pinmuxes[] = {
        pSERIAL_S(obj)->pinmux_pad0, pSERIAL_S(obj)->pinmux_pad1,
        pSERIAL_S(obj)->pinmux_pad2, pSERIAL_S(obj)->pinmux_pad3
    };

    /* Configure the SERCOM pins according to the user configuration */
    for (uint8_t pad = 0; pad < 4; pad++) {
        uint32_t current_pinmux = pad_pinmuxes[pad];

        if (current_pinmux == PINMUX_DEFAULT) {
            current_pinmux = _sercom_get_default_pad(pUSART_S(obj), pad);
        }

        if (current_pinmux != PINMUX_UNUSED) {
            pin_conf.mux_position = current_pinmux & 0xFFFF;
            system_pinmux_pin_set_config(current_pinmux >> 16, &pin_conf);
        }
    }

    if (uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Enable USART module */
    enable_usart(obj);

}

void serial_free(serial_t *obj)
{
    serial_irq_ids[serial_get_index(obj)] = 0;
    disable_usart(obj);
}

void serial_baud(serial_t *obj, int baudrate)
{
    MBED_ASSERT((baudrate == 110) || (baudrate == 150) || (baudrate == 300) || (baudrate == 1200) ||
                (baudrate == 2400) || (baudrate == 4800) || (baudrate == 9600) || (baudrate == 19200) || (baudrate == 38400) ||
                (baudrate == 57600) || (baudrate == 115200) || (baudrate == 230400) || (baudrate == 460800) || (baudrate == 921600) );

    struct system_gclk_chan_config gclk_chan_conf;
    uint32_t gclk_index;
    uint16_t baud  = 0;
    uint32_t sercom_index = 0;
    enum sercom_asynchronous_operation_mode mode = SERCOM_ASYNC_OPERATION_MODE_ARITHMETIC;
    enum sercom_asynchronous_sample_num sample_num = SERCOM_ASYNC_SAMPLE_NUM_16;

    pSERIAL_S(obj)->baudrate = baudrate;
    disable_usart(obj);

    sercom_index = _sercom_get_sercom_inst_index(pUSART_S(obj));
    gclk_index   = sercom_index + SERCOM0_GCLK_ID_CORE;

    gclk_chan_conf.source_generator = GCLK_GENERATOR_0;
    system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
    system_gclk_chan_enable(gclk_index);
    sercom_set_gclk_generator(GCLK_GENERATOR_0, false);

    /* Get baud value from mode and clock */
    _sercom_get_async_baud_val(pSERIAL_S(obj)->baudrate, system_gclk_chan_get_hz(gclk_index), &baud, mode, sample_num);

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /*Set baud val */
    _USART(obj).BAUD.reg = baud;
    /* Wait until synchronization is complete */
    usart_syncing(obj);

    enable_usart(obj);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    MBED_ASSERT((stop_bits == 1) || (stop_bits == 2));
    MBED_ASSERT((parity == ParityNone) || (parity == ParityOdd) || (parity == ParityEven));
    MBED_ASSERT((data_bits == 5) || (data_bits == 6) || (data_bits == 7) || (data_bits == 8) /*|| (data_bits == 9)*/);

    /* Cache new register values to minimize the number of register writes */
    uint32_t ctrla = 0;
    uint32_t ctrlb = 0;

    disable_usart(obj);

    ctrla = _USART(obj).CTRLA.reg;
    ctrlb = _USART(obj).CTRLB.reg;

    ctrla &= ~(SERCOM_USART_CTRLA_FORM_Msk);
    ctrlb &= ~(SERCOM_USART_CTRLB_CHSIZE_Msk);
    ctrlb &= ~(SERCOM_USART_CTRLB_SBMODE);
    ctrlb &= ~(SERCOM_USART_CTRLB_PMODE);

    switch (stop_bits) {
        case 1:
            pSERIAL_S(obj)->stopbits = USART_STOPBITS_1;
            break;
        case 2:
            pSERIAL_S(obj)->stopbits = USART_STOPBITS_2;
            break;
        default:
            pSERIAL_S(obj)->stopbits = USART_STOPBITS_1;
    }

    switch (parity) {
        case ParityNone:
            pSERIAL_S(obj)->parity = USART_PARITY_NONE;
            break;
        case ParityOdd:
            pSERIAL_S(obj)->parity = USART_PARITY_ODD;
            break;
        case ParityEven:
            pSERIAL_S(obj)->parity = USART_PARITY_EVEN;
            break;
        default:
            pSERIAL_S(obj)->parity = USART_PARITY_NONE;
    }

    switch (data_bits) {
        case 5:
            pSERIAL_S(obj)->character_size = USART_CHARACTER_SIZE_5BIT;
            break;
        case 6:
            pSERIAL_S(obj)->character_size = USART_CHARACTER_SIZE_6BIT;
            break;
        case 7:
            pSERIAL_S(obj)->character_size = USART_CHARACTER_SIZE_7BIT;
            break;
        case 8:
            pSERIAL_S(obj)->character_size = USART_CHARACTER_SIZE_8BIT;
            break;  //  9 bit transfer not required in mbed
        default:
            pSERIAL_S(obj)->character_size = USART_CHARACTER_SIZE_8BIT;
    }


    /* Set stopbits, character size and enable transceivers */
    ctrlb = (uint32_t)pSERIAL_S(obj)->stopbits | (uint32_t)pSERIAL_S(obj)->character_size;

    /* Check parity mode bits */
    if (pSERIAL_S(obj)->parity != USART_PARITY_NONE) {
        ctrla |= SERCOM_USART_CTRLA_FORM(1);
        ctrlb |= pSERIAL_S(obj)->parity;
    } else {
        ctrla |= SERCOM_USART_CTRLA_FORM(0);
    }

    /* Write configuration to CTRLB */
    _USART(obj).CTRLB.reg = ctrlb;

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    /* Write configuration to CTRLA */
    _USART(obj).CTRLA.reg = ctrla;

    /* Wait until synchronization is complete */
    usart_syncing(obj);

    enable_usart(obj);
}

#ifdef DEVICE_SERIAL_FC
void uart0_irq();
void uart1_irq();
void uart2_irq();
void uart3_irq();
void uart4_irq();
void uart5_irq();

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow)
{
    uint32_t muxsetting = 0;
    uint32_t sercom_index = 0;
    uint32_t padsetting[4] = {0};

    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    switch ((int)pUSART_S(obj)) {
        case UART_0:
            vector = (uint32_t)uart0_irq;
            break;
        case UART_1:
            vector = (uint32_t)uart1_irq;
            break;
        case UART_2:
            vector = (uint32_t)uart2_irq;
            break;
        case UART_3:
            vector = (uint32_t)uart3_irq;
            break;
        case UART_4:
            vector = (uint32_t)uart4_irq;
            break;
        case UART_5:
            vector = (uint32_t)uart5_irq;
            break;
    }
    irq_n = get_serial_irq_num(obj);

    disable_usart(obj);
    //TODO : assert for rxflow and txflow pis to be added
    find_pin_settings(pSERIAL_S(obj)->txpin, pSERIAL_S(obj)->rxpin, rxflow, txflow, &padsetting[0]);  // tx, rx, clk(rts), chipsel(cts) pad array  // getting pads from pins
    muxsetting = find_mux_setting(pSERIAL_S(obj)->txpin, pSERIAL_S(obj)->rxpin, rxflow, txflow);  // getting mux setting from pins

    pSERIAL_S(obj)->mux_setting = muxsetting;//EDBG_CDC_SERCOM_MUX_SETTING;
    pSERIAL_S(obj)->pinmux_pad0 = padsetting[0];//EDBG_CDC_SERCOM_PINMUX_PAD0;
    pSERIAL_S(obj)->pinmux_pad1 = padsetting[1];//EDBG_CDC_SERCOM_PINMUX_PAD1;
    pSERIAL_S(obj)->pinmux_pad2 = padsetting[2];//EDBG_CDC_SERCOM_PINMUX_PAD2;
    pSERIAL_S(obj)->pinmux_pad3 = padsetting[3];//EDBG_CDC_SERCOM_PINMUX_PAD3;

    /* Set configuration according to the config struct */
    usart_set_config_default(obj);

    struct system_pinmux_config pin_conf;
    system_pinmux_get_config_defaults(&pin_conf);
    pin_conf.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
    pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;

    uint32_t pad_pinmuxes[] = {
        pSERIAL_S(obj)->pinmux_pad0, pSERIAL_S(obj)->pinmux_pad1,
        pSERIAL_S(obj)->pinmux_pad2, pSERIAL_S(obj)->pinmux_pad3
    };

    /* Configure the SERCOM pins according to the user configuration */
    for (uint8_t pad = 0; pad < 3; pad++) {
        uint32_t current_pinmux = pad_pinmuxes[pad];

        if (current_pinmux == PINMUX_DEFAULT) {
            current_pinmux = _sercom_get_default_pad(pUSART_S(obj), pad);
        }

        if (current_pinmux != PINMUX_UNUSED) {
            pin_conf.mux_position = current_pinmux & 0xFFFF;
            system_pinmux_pin_set_config(current_pinmux >> 16, &pin_conf);
        }
    }
    if (pSERIAL_S(obj)->pinmux_pad3 != PINMUX_UNUSED) {
        pin_conf.input_pull = SYSTEM_PINMUX_PIN_PULL_UP;
        pin_conf.mux_position = pSERIAL_S(obj)->pinmux_pad3 & 0xFFFF;
        system_pinmux_pin_set_config(pSERIAL_S(obj)->pinmux_pad3 >> 16, &pin_conf);
    }

    NVIC_SetVector(irq_n, vector);
    NVIC_EnableIRQ(irq_n);

    enable_usart(obj);
    _USART(obj).INTENSET.reg = SERCOM_USART_INTENCLR_CTSIC;
}

void serial_break_set(serial_t *obj)
{
    disable_usart(obj);
    _USART(obj).CTRLB.reg &= ~SERCOM_SPI_CTRLB_RXEN;
    usart_syncing(obj);
    enable_usart(obj);
}

void serial_break_clear(serial_t *obj)
{
    disable_usart(obj);
    _USART(obj).CTRLB.reg |= SERCOM_SPI_CTRLB_RXEN;
    usart_syncing(obj);
    enable_usart(obj);
}

#endif  //DEVICE_SERIAL_FC

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
inline uint8_t serial_get_index(serial_t *obj)
{
    switch ((int)pUSART_S(obj)) {
        case UART_0:
            return 0;
        case UART_1:
            return 1;
        case UART_2:
            return 2;
        case UART_3:
            return 3;
        case UART_4:
            return 4;
        case UART_5:
            return 5;
    }
    return 0;
}
static inline void uart_irq(SercomUsart *const usart, uint32_t index)
{
    uint16_t interrupt_status;
    interrupt_status = usart->INTFLAG.reg;
    interrupt_status &= usart->INTENSET.reg;

    if (serial_irq_ids[index] != 0) {
        if (interrupt_status & SERCOM_USART_INTFLAG_TXC) { // for transmit complete
            usart->INTFLAG.reg = SERCOM_USART_INTFLAG_TXC;
            irq_handler(serial_irq_ids[index], TxIrq);
        }
        if (interrupt_status & SERCOM_USART_INTFLAG_RXC) { // for receive complete
            usart->INTFLAG.reg = SERCOM_USART_INTFLAG_RXC;
            irq_handler(serial_irq_ids[index], RxIrq);
        }
        if (interrupt_status & SERCOM_USART_INTFLAG_CTSIC) { // hardware flow control
            usart->INTENCLR.reg = SERCOM_USART_INTENCLR_CTSIC;
            usart->INTFLAG.reg = SERCOM_USART_INTENCLR_CTSIC;
        }
    }
}

void uart0_irq()
{
    uart_irq((SercomUsart *)UART_0, 0);
}

void uart1_irq()
{
    uart_irq((SercomUsart *)UART_1, 1);
}

void uart2_irq()
{
    uart_irq((SercomUsart *)UART_2, 2);
}

void uart3_irq()
{
    uart_irq((SercomUsart *)UART_3, 3);
}

void uart4_irq()
{
    uart_irq((SercomUsart *)UART_4, 4);
}

void uart5_irq()
{
    uart_irq((SercomUsart *)UART_5, 5);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[serial_get_index(obj)] = id;
}

IRQn_Type get_serial_irq_num (serial_t *obj)
{
    switch ((int)pUSART_S(obj)) {
        case UART_0:
            return SERCOM0_IRQn;
        case UART_1:
            return SERCOM1_IRQn;
        case UART_2:
            return SERCOM2_IRQn;
        case UART_3:
            return SERCOM3_IRQn;
        case UART_4:
            return SERCOM4_IRQn;
        case UART_5:
            return SERCOM5_IRQn;
        default:
            MBED_ASSERT(0);
    }
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;
    uint32_t vector = 0;

    switch ((int)pUSART_S(obj)) {
        case UART_0:
            vector = (uint32_t)uart0_irq;
            break;
        case UART_1:
            vector = (uint32_t)uart1_irq;
            break;
        case UART_2:
            vector = (uint32_t)uart2_irq;
            break;
        case UART_3:
            vector = (uint32_t)uart3_irq;
            break;
        case UART_4:
            vector = (uint32_t)uart4_irq;
            break;
        case UART_5:
            vector = (uint32_t)uart5_irq;
            break;
    }
    irq_n = get_serial_irq_num(obj);

    if (enable) {
        switch (irq) {
            case RxIrq:
                _USART(obj).INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
                break;
            case TxIrq:
                _USART(obj).INTENSET.reg = SERCOM_USART_INTFLAG_TXC;
                break;
        }
        NVIC_SetVector(irq_n, vector);
        NVIC_EnableIRQ(irq_n);

    } else {
        switch (irq) {
            case RxIrq:
                _USART(obj).INTENCLR.reg = SERCOM_USART_INTFLAG_RXC;
                break;
            case TxIrq:
                _USART(obj).INTENCLR.reg = SERCOM_USART_INTFLAG_TXC;
                break;
        }
        NVIC_DisableIRQ(irq_n);
    }
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/
int serial_getc(serial_t *obj)
{
    while (!serial_readable(obj));
    return _USART(obj).DATA.reg ;
}

void serial_putc(serial_t *obj, int c)
{
    uint16_t q = (c & SERCOM_USART_DATA_MASK);
    while (!serial_writable(obj));
    _USART(obj).DATA.reg = q;
    while (!(_USART(obj).INTFLAG.reg & SERCOM_USART_INTFLAG_TXC));  // wait till data is sent
}

int serial_readable(serial_t *obj)
{
    uint32_t status = 1;
    if (!(_USART(obj).INTFLAG.reg & SERCOM_USART_INTFLAG_RXC)) {
        status = 0;
    } else {
        status = 1;
    }
    return status;
}

int serial_writable(serial_t *obj)
{
    uint32_t status = 1;
    if (!(_USART(obj).INTFLAG.reg & SERCOM_USART_INTFLAG_DRE)) {
        status = 0;
    } else {
        status = 1;
    }
    return status;
}

/************************************************************************************
 * 			ASYNCHRONOUS HAL														*
 ************************************************************************************/

#if DEVICE_SERIAL_ASYNCH

/************************************
 * HELPER FUNCTIONS					*
 ***********************************/
void serial_tx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    if(enable) {
        pSERIAL_S(obj)->events |= event;
    } else {
        pSERIAL_S(obj)->events &= ~ event;
    }
}

void serial_rx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    if(enable) {
        pSERIAL_S(obj)->events |= event;
    } else {
        pSERIAL_S(obj)->events &= ~ event;
    }
}

void serial_tx_buffer_set(serial_t *obj, void *tx, int tx_length, uint8_t width)
{
    // We only support byte buffers for now
    MBED_ASSERT(width == 8);

    if(serial_tx_active(obj)) return;

    obj->tx_buff.buffer = tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;

    return;
}

void serial_rx_buffer_set(serial_t *obj, void *rx, int rx_length, uint8_t width)
{
    // We only support byte buffers for now
    MBED_ASSERT(width == 8);

    if(serial_rx_active(obj)) return;

    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;

    return;
}

void serial_set_char_match(serial_t *obj, uint8_t char_match)
{
    if (char_match != SERIAL_RESERVED_CHAR_MATCH) {
        obj->char_match = char_match;
    }
}

/************************************
 * TRANSFER FUNCTIONS				*
 ***********************************/
int serial_tx_asynch(serial_t *obj, const void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    MBED_ASSERT(tx != (void*)0);
    if(tx_length == 0) return 0;

    serial_tx_buffer_set(obj, (void *)tx, tx_length, tx_width);
    serial_tx_enable_event(obj, event, true);

//    if( hint == DMA_USAGE_NEVER) {  //TODO: DMA to be implemented later
    NVIC_ClearPendingIRQ(get_serial_irq_num(obj));
    NVIC_DisableIRQ(get_serial_irq_num(obj));
    NVIC_SetPriority(get_serial_irq_num(obj), 1);
    NVIC_SetVector(get_serial_irq_num(obj), (uint32_t)handler);
    NVIC_EnableIRQ(get_serial_irq_num(obj));

    if (pUSART_S(obj)) {
        _USART(obj).INTENCLR.reg = SERCOM_USART_INTFLAG_TXC;
        _USART(obj).INTENSET.reg = SERCOM_USART_INTFLAG_DRE;
    }
//	}
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    MBED_ASSERT(rx != (void*)0);

    serial_rx_enable_event(obj, SERIAL_EVENT_RX_ALL, false);
    serial_rx_enable_event(obj, event, true);
    serial_set_char_match(obj, char_match);

    serial_rx_buffer_set(obj, rx, rx_length, rx_width);

//    if( hint == DMA_USAGE_NEVER) {  //TODO: DMA to be implemented later
    NVIC_ClearPendingIRQ(get_serial_irq_num(obj));
    NVIC_SetVector(get_serial_irq_num(obj), (uint32_t)handler);
    NVIC_EnableIRQ(get_serial_irq_num(obj));

    if (pUSART_S(obj)) {
        _USART(obj).INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
    }
//	}
}

uint8_t serial_tx_active(serial_t *obj)
{
    return ((_USART(obj).INTENSET.reg & SERCOM_USART_INTFLAG_DRE) ? true : false);
}

uint8_t serial_rx_active(serial_t *obj)
{
    return ((_USART(obj).INTENSET.reg & SERCOM_USART_INTFLAG_RXC) ? true : false);
}

int serial_tx_irq_handler_asynch(serial_t *obj)
{
    _USART(obj).INTENCLR.reg = SERCOM_USART_INTFLAG_TXC;
    return SERIAL_EVENT_TX_COMPLETE & obj->serial.events;
}

int serial_rx_irq_handler_asynch(serial_t *obj)
{
    int event = 0;
    /* This interrupt handler is called from USART irq */
    uint8_t *buf = (uint8_t*)obj->rx_buff.buffer;
    uint8_t error_code = 0;
    uint16_t received_data = 0;


    error_code = (uint8_t)(_USART(obj).STATUS.reg & SERCOM_USART_STATUS_MASK);

    /* Check if an error has occurred during the receiving */
    if (error_code) {
        /* Check which error occurred */
        if (error_code & SERCOM_USART_STATUS_FERR) {
            /* Store the error code and clear flag by writing 1 to it */
            _USART(obj).STATUS.reg |= SERCOM_USART_STATUS_FERR;
            return SERIAL_EVENT_RX_FRAMING_ERROR;
        } else if (error_code & SERCOM_USART_STATUS_BUFOVF) {
            /* Store the error code and clear flag by writing 1 to it */
            _USART(obj).STATUS.reg |= SERCOM_USART_STATUS_BUFOVF;
            return SERIAL_EVENT_RX_OVERFLOW;
        } else if (error_code & SERCOM_USART_STATUS_PERR) {
            /* Store the error code and clear flag by writing 1 to it */
            _USART(obj).STATUS.reg |= SERCOM_USART_STATUS_PERR;
            return SERIAL_EVENT_RX_PARITY_ERROR;
        }
    }

    /* Read current packet from DATA register,
    * increment buffer pointer and decrement buffer length */
    received_data = (_USART(obj).DATA.reg & SERCOM_USART_DATA_MASK);

    /* Read value will be at least 8-bits long */
    buf[obj->rx_buff.pos] = received_data;
    /* Increment 8-bit pointer */
    obj->rx_buff.pos++;

    /* Check if the last character have been received */
    if(--(obj->rx_buff.length) == 0) {
        event |= SERIAL_EVENT_RX_COMPLETE;
        if((buf[obj->rx_buff.pos - 1] == obj->char_match) && (obj->serial.events & SERIAL_EVENT_RX_CHARACTER_MATCH)) {
            event |= SERIAL_EVENT_RX_CHARACTER_MATCH;
        }
        serial_rx_abort_asynch(obj);
        return event & obj->serial.events;
    }

    /* Check for character match event */
    if((buf[obj->rx_buff.pos - 1] == obj->char_match) && (obj->serial.events & SERIAL_EVENT_RX_CHARACTER_MATCH)) {
        event |= SERIAL_EVENT_RX_CHARACTER_MATCH;
    }

    /* check for final char event */
    if((obj->rx_buff.length) == 0) {
        event |= SERIAL_EVENT_RX_COMPLETE & obj->serial.events;
    }

    if(event != 0) {
        serial_rx_abort_asynch(obj);
        return event & obj->serial.events;
    }
}

int serial_irq_handler_asynch(serial_t *obj)
{
//TODO: DMA to be implemented
    uint16_t interrupt_status;
    uint8_t *buf = obj->tx_buff.buffer;

    interrupt_status = _USART(obj).INTFLAG.reg;
    interrupt_status &= _USART(obj).INTENSET.reg;

    if (pUSART_S(obj)) {
        if (interrupt_status & SERCOM_USART_INTFLAG_DRE) {
            /* Interrupt has another TX source */
            if(obj->tx_buff.pos >= obj->tx_buff.length) {

                /* Transfer complete. Switch off interrupt and return event. */
                _USART(obj).INTENCLR.reg = SERCOM_USART_INTFLAG_DRE;
                serial_tx_abort_asynch(obj);

                return SERIAL_EVENT_TX_COMPLETE & obj->serial.events;
            } else {
                while((serial_writable(obj)) && (obj->tx_buff.pos <= (obj->tx_buff.length - 1))) {
                    _USART(obj).DATA.reg = buf[obj->tx_buff.pos];
                    obj->tx_buff.pos++;
                }
            }
        }
        if (interrupt_status & SERCOM_USART_INTFLAG_TXC) {
            serial_tx_irq_handler_asynch(obj);
        }
        if (interrupt_status & SERCOM_USART_INTFLAG_RXC) {
            serial_rx_irq_handler_asynch(obj);
        }
    }

}

void serial_tx_abort_asynch(serial_t *obj)
{
//TODO: DMA to be implemented
    _USART(obj).INTENSET.reg = SERCOM_USART_INTFLAG_TXC;
    obj->tx_buff.pos = 0;
    obj->tx_buff.length = 0;
}

void serial_rx_abort_asynch(serial_t *obj)
{
//TODO: DMA to be implemented
    _USART(obj).INTENCLR.reg = SERCOM_USART_INTFLAG_RXC;
    obj->rx_buff.pos = 0;
    obj->rx_buff.length = 0;
}

#endif