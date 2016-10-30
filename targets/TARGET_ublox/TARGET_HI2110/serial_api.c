/* mbed Microcontroller Library
 * Copyright (c) 2016 u-blox
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

/* The serial driver connects UART HW to mbed and also associates the UART
 * HW with physical pins.  Any physical pin can be linked to any UART,
 * however the mbed serial port initialisation API makes no mention of
 * which UART HW is to be used (only the pins) and hence the driver needs
 * to make some decisions for itself.
 *
 * There are two and a half UARTs on the chip: UART0, UART1 and a
 * lower-power, receive-only UART that is clocked from 32 kHz and can
 * therefore be awake while the rest of the chip is sleeping peacefully.
 * This provides maximal power saving, however the LP UART can only run
 * at 9600 bits/s (which is quite sufficient for all NB-IoT needs).
 *
 * So, if the baud rate is 9600 the driver code configures the LP UART
 * for Rx and UART0 for Tx. If the baud rate is not 9600 then it configures
 * UART0 for both Rx and Tx.  Unless... the Tx pin is the pin UART1_TX (it
 * is an mbed convention to use the Tx pin), which is p6, in which case UART1
 * is configured instead.  This latter is not the normal case as this pin
 * is intended to be used as a GPIO.
 *
 * If the baud rate is changed the driver reconfigures to match.
 *
 * TODO: implement asynchronous and flow control APIs.
 */

#include "mbed_assert.h"
#include "serial_api.h"
#include "pinmap.h"

#include "cmsis.h"

/* ----------------------------------------------------------------
 * MACROS
 * ----------------------------------------------------------------*/

/* Registers banks for the standard UARTs */
#define UART0_REG (*(volatile uart_ctrl_t *) UART0_BASE)
#define UART1_REG (*(volatile uart_ctrl_t *) UART1_BASE)

/* Masks for the UART control bits in the reset and clock enable registers */
#define UART0_CTRL  (1 << 3)
#define UART1_CTRL  (1 << 4)
#define UARTLP_CTRL (1 << 6)

/* Convert number of data bits to register values */
#define MIN_NUM_UART_DATA_BITS 5
#define MAX_NUM_UART_DATA_BITS 8
#define REGISTER_DATA_BITS(x) ((x) - MIN_NUM_UART_DATA_BITS)

/* Number of stop bits */
#define NUM_UART_STOP_BITS_1 1
#define NUM_UART_STOP_BITS_2 2

/* ----------------------------------------------------------------
 * TYPES
 * ----------------------------------------------------------------*/

/* Enum to identify the interrupt to the UART handler */
typedef enum {
    IRQ_UART_ID_0_AND_LP,
    IRQ_UART_ID_1,
    NUM_IRQ_IDS
} irq_uart_id_t;

/* ----------------------------------------------------------------
 * GLOBAL VARIABLES
 * ----------------------------------------------------------------*/

/* The IRQ configuration variables, set up and named by mbed */
static uint32_t serial_irq_ids[NUM_IRQ_IDS] = {0};
static uart_irq_handler irq_handler = NULL;

/* RTX needs these */
int stdio_uart_inited = 0;
serial_t stdio_uart;

/* ----------------------------------------------------------------
 * FUNCTION PROTOTYPES
 * ----------------------------------------------------------------*/

static void init_config(serial_t *obj);
static void deinit_config(serial_t *obj);
static void set_baud(serial_t *obj, uint32_t baud_rate);
static void irq_enable(serial_t *obj);
static void irq_disable(serial_t *obj);

/* ----------------------------------------------------------------
 * NON-API FUNCTIONS
 * ----------------------------------------------------------------*/

/* Initialise the given serial config by setting the pin functions
 * and then resetting the relevant HW */
static void init_config(serial_t *obj)
{
    uint32_t x;

    switch (obj->config) {
        case SERIAL_CONFIG_UARTLP_RX_UART0_TX:
        {
            pin_function(obj->rx_pin, PIN_FUNCTION_LP_UART);
            pin_function(obj->tx_pin, PIN_FUNCTION_UART0_TXD);
            CLKEN_REG_BITSET = UARTLP_CTRL | UART0_CTRL;
            obj->reg_base = &UART0_REG;
            obj->index = IRQ_UART_ID_0_AND_LP;
            /* Reset the LPUART and UART0 HW */
            /* NOTE: RESET_REG_BITTOG doesn't have the desired
             * effect, need to use BITSET and then BITCLR */
            RESET_REG_BITSET |= 1ul << 6;
            RESET_REG_BITCLR |= 1ul << 6;
            RESET_REG_BITSET |= 1ul << 3;
            RESET_REG_BITCLR |= 1ul << 3;
        }
        break;
        case SERIAL_CONFIG_UART0_RX_UART0_TX:
        {
            pin_function(obj->rx_pin, PIN_FUNCTION_UART0_RXD);
            pin_function(obj->tx_pin, PIN_FUNCTION_UART0_TXD);
            CLKEN_REG_BITSET = UART0_CTRL;
            obj->reg_base = &UART0_REG;
            obj->index = IRQ_UART_ID_0_AND_LP;
            /* Reset the UART0 HW */
            RESET_REG_BITSET |= 1ul << 3;
            RESET_REG_BITCLR |= 1ul << 3;
        }
        break;
        case SERIAL_CONFIG_UART1_RX_UART1_TX:
        {
            pin_function(obj->rx_pin, PIN_FUNCTION_UART1_RXD);
            pin_function(obj->tx_pin, PIN_FUNCTION_UART1_TXD);
            CLKEN_REG_BITSET = UART1_CTRL;
            obj->reg_base = &UART1_REG;
            obj->index = IRQ_UART_ID_1;
            /* Reset the UART1 HW */
            RESET_REG_BITSET |= 1ul << 4;
            RESET_REG_BITCLR |= 1ul << 4;
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }

    /* Tickle the UART control register to make sure it is updated */
    x = obj->reg_base->UARTLCR_H;
    obj->reg_base->UARTLCR_H = x;

    /* Set the FIFO. The meaning of the three FIFO interrupt-level
     * bits are as follows:
     *
     * 0 = 1/8 full
     * 1 = 1/4 full
     * 2 = 1/2 full
     * 3 = 3/4 full
     * 4 = 7/8 full
     *
     * Set up the Rx FIFO to be used fully (but we will also set
     * a timeout to get immediate notice) and also the Tx FIFO
     * to be fully used. */
     obj->reg_base->UARTIFLS = (obj->reg_base->UARTIFLS & ~(0x07 << 0)) | (4 << 0);
     obj->reg_base->UARTIFLS = (obj->reg_base->UARTIFLS & ~(0x07 << 3)) | (4 << 3);
     obj->reg_base->UARTLCR_H |= 1 << 4;

    /* Enable for Tx and Rx (TODO: add CTS when we add flow control) */
    obj->reg_base->UARTCR |= (1 << 8) | (1 << 9);

    /* Now enable it */
    obj->reg_base->UARTCR |= 1 << 0;

    obj->format_set = false;
    obj->baud_rate = 0;
    obj->irq_rx_setting = IRQ_NOT_SET;
    obj->irq_tx_setting = IRQ_NOT_SET;
}

/* Release a serial port */
static void deinit_config(serial_t *obj)
{
    pin_function(obj->rx_pin, PIN_FUNCTION_UNCLAIMED);
    pin_function(obj->tx_pin, PIN_FUNCTION_UNCLAIMED);

    /* Disable UART */
    obj->reg_base->UARTCR &= ~(1 << 0);

    /* Flush transmit FIFO */
    obj->reg_base->UARTLCR_H = 0;

    /* Disable everything */
    obj->reg_base->UARTCR = 0;

    switch (obj->config) {
        case SERIAL_CONFIG_UARTLP_RX_UART0_TX:
        {
            CLKEN_REG_BITCLR = UARTLP_CTRL | UART0_CTRL;
            LP_UART_CTRL &= ~(0xF << 20); /* Disable all LP interrupts */
        }
        break;
        case SERIAL_CONFIG_UART0_RX_UART0_TX:
        {
            CLKEN_REG_BITCLR = UART0_CTRL;
        }
        break;
        case SERIAL_CONFIG_UART1_RX_UART1_TX:
        {
            CLKEN_REG_BITCLR = UART1_CTRL;
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }

    obj->config = MAX_NUM_SERIAL_CONFIGS;
    obj->reg_base = NULL;
}

/* Set the baud rate for either of the two (non-LP) UARTS */
static void set_baud(serial_t *obj, uint32_t baud_rate)
{
    uint32_t baud_rate_divider_i;
    uint32_t baud_rate_divider_f;
    uint32_t remainder;
    uint32_t core_clock;
    uint32_t x;

    /* Baud rate divider calculation:
     *
     * The integer part is: clock / (16 * baud)
     *
     * The fractional part is:  round (decimal_part * 64),
     * ...where decimal part is, for example, 0.085
     *
     * decimal_part is: remainder / (16 * baud),
     * ...where: remainder = core_clock % (baud * 16),
     *
     * So the fractional part becomes:
     * round (decimal_part * 64) = round (remainder * 64 / (16 * baud)) = round (remainder * 4 / baud)
     */

    /* Get the mean clock frequency */
    core_clock = (CLK_FREQ_HIGHTARGET >> 1) + (CLK_FREQ_LOWTARGET >> 1);
    /* Work out the actual clock frequency */
    core_clock = (core_clock * CLOCKS_REFERENCE_CLOCK_FREQ) / (((CLK_FREQ_NREFCLKS + 1) & 0xFFFF) * (CLK_GATE_SYS & 0xFF));
    baud_rate_divider_i = core_clock / (baud_rate << 4);
    remainder = core_clock % (baud_rate << 4);
    baud_rate_divider_f = ((remainder << 3) / baud_rate) >> 1;
    /* Round it */
    baud_rate_divider_f += ((remainder << 3) / baud_rate) & 1;

    /* Disable UART while writing to control registers */
    obj->reg_base->UARTCR &= ~(1 << 0);

    obj->reg_base->UARTIBRD = baud_rate_divider_i;
    obj->reg_base->UARTFBRD = baud_rate_divider_f;

    /* Make IBRD and FBRD update */
    x = obj->reg_base->UARTLCR_H;
    obj->reg_base->UARTLCR_H = x;

    /* Now enable the UART again */
    obj->reg_base->UARTCR |= 1 << 0;
}

/* Set the NVIC bits */
static void irq_enable(serial_t *obj)
{
    switch (obj->config) {
        case SERIAL_CONFIG_UARTLP_RX_UART0_TX:
        {
            NVIC_EnableIRQ(UART0_IRQn);
            NVIC_EnableIRQ(LPUART_IRQn);
        }
        break;
        case SERIAL_CONFIG_UART0_RX_UART0_TX:
        {
            NVIC_EnableIRQ(UART0_IRQn);
        }
        break;
        case SERIAL_CONFIG_UART1_RX_UART1_TX:
        {
            NVIC_EnableIRQ(UART1_IRQn);
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }
}

/* Unset the NVIC bits */
static void irq_disable(serial_t *obj)
{
    switch (obj->config) {
        case SERIAL_CONFIG_UARTLP_RX_UART0_TX:
        {
            NVIC_DisableIRQ(UART0_IRQn);
            NVIC_DisableIRQ(LPUART_IRQn);
        }
        break;
        case SERIAL_CONFIG_UART0_RX_UART0_TX:
        {
            NVIC_DisableIRQ(UART0_IRQn);
        }
        break;
        case SERIAL_CONFIG_UART1_RX_UART1_TX:
        {
            NVIC_DisableIRQ(UART1_IRQn);
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }
}

/* UART0 IRQ */
void IRQ7_UART0_Handler()
{
    uint32_t id = serial_irq_ids[IRQ_UART_ID_0_AND_LP];

    /* Check Rx and Rx Timeout bit */
    if (UART0_REG.UARTMIS & ((1 << 4) | (1 << 6))) {
        if (id != 0) {
            irq_handler(id, RxIrq);
            /* Reading the character clears the interrupt,
             * no way to protect against another arriving
             * while processing one */
        }
    }
    /* Check Tx bit */
    if (UART0_REG.UARTMIS & (1 << 5)) {
        if (id != 0) {
            irq_handler(id, TxIrq);
        }
        /* Not sure what clears the interrupt so clear it explicitly */
        NVIC_ClearPendingIRQ(UART1_IRQn);
    }
}

/* UART1 IRQ */
void IRQ8_UART1_Handler()
{
    uint32_t id = serial_irq_ids[IRQ_UART_ID_1];

    /* Check Rx and Rx Timeout bit */
    if (UART1_REG.UARTMIS & ((1 << 4) | (1 << 6))) {
        if (id != 0) {
            irq_handler(id, RxIrq);
        }
        /* Reading the character clears the interrupt,
         * no way to protect against another arriving
         * while processing one */
    }
    /* Check Tx bit */
    if (UART1_REG.UARTMIS & (1 << 5)) {
        if (id != 0) {
            irq_handler(id, TxIrq);
        }
        /* Not sure what clears the interrupt so clear it explicitly */
        NVIC_ClearPendingIRQ(UART1_IRQn);
    }
}

/* LP UART IRQ, receive only */
void IRQ16_LPUART_Handler()
{
    uint32_t id = serial_irq_ids[IRQ_UART_ID_0_AND_LP];

    if (id != 0) {
        irq_handler(id, RxIrq);

        /* Another character might have arrived while
         * we are processing the last, so
         * check status bits 8 to 10 again and pend
         * interrupt if there's something there */
        if (((LP_UART_STATUS >> 8) & 0x07) != 0) {
            NVIC_SetPendingIRQ(LPUART_IRQn);
        } else {
            LP_UART_CTRL |= 1 << 27; /* Clear the interrupt */
        }
    }
}

/* ----------------------------------------------------------------
 * MBED API CALLS: SETUP FUNCTIONS
 * ----------------------------------------------------------------*/

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    uint32_t clock = CLK_FREQ_DIG_CLKS;

    /* There are two and a half UARTs on the chip. The normal
     * configuration is to use the LP_UART for Rx and UART0 for
     * Tx.  This gives maximal power saving in that the chip can
     * wake up on receipt of data. However, this only works if the
     * data rate is 9600 because that's the only data rate that
     * the 32 kHz (i.e. RTC) clock driving the LP UART can support.
     *
     * So, if the data rate is 9600, use the LP_UART/UART0
     * combination, otherwise use UART0 for both Rx and Tx.  However,
     * we don't know the data rate at this point so assume LP_UART
     * (as this works at the default baud rate) and we can change
     * our minds later.
     *
     * There is another serial port, UART1, which is normally used
     * by the modem processor to send out debug.  We only initialise
     * that here if the Tx pin is UART1_TX. */

    /* Wait for the clock to be stable */
    while ((clock < CLK_FREQ_LOWTARGET) || (clock > CLK_FREQ_HIGHTARGET)) {
        clock = CLK_FREQ_DIG_CLKS;
    }

    if (tx == UART1_TX) {
        /* Use UART1 for Rx and Tx */
        obj->config = SERIAL_CONFIG_UART1_RX_UART1_TX;
    } else {
        /* Use LP_UART for Rx, UART0 for Tx */
        obj->config = SERIAL_CONFIG_UARTLP_RX_UART0_TX;
    }

    obj->rx_pin = rx;
    obj->tx_pin = tx;
    init_config(obj);

    /* TODO: set rx pin Pull mode ? */

    /* set default baud rate and format */
    serial_baud(obj, 9600);
    serial_format(obj, 8, ParityNone, 1);

    if (tx == UART0_TX) {
        /* The UART0 pins are the stdio pins */
        stdio_uart_inited = 1;
        stdio_uart = *obj;
    }
}

void serial_free(serial_t *obj)
{
    if (obj->tx_pin == UART0_TX) {
        stdio_uart_inited = 0;
    }

    serial_irq_ids[obj->index] = 0;

    /* Release the port HW */
    deinit_config(obj);
}

void serial_baud(serial_t *obj, int baudrate)
{
    bool switch_port_config = false;
    bool format_set = obj->format_set;
    uint8_t stop_bits = obj->format.stop_bits;
    uint8_t data_bits = obj->format.data_bits;
    SerialParity parity = (SerialParity) obj->format.parity;
    irq_setting_t irq_rx_setting = obj->irq_rx_setting;
    irq_setting_t irq_tx_setting = obj->irq_tx_setting;

    if ((obj->config == SERIAL_CONFIG_UARTLP_RX_UART0_TX) && (baudrate != 9600)) {
        /* If we were on LP UART but the baud rate is not 9600 then
         * switch to the standard UART (as the LP UART can't go any higher
         * because it's clocked from 32 kHz) */
        deinit_config(obj);
        obj->config = SERIAL_CONFIG_UART0_RX_UART0_TX;
        init_config(obj);
        switch_port_config = true;
    } else if ((obj->config == SERIAL_CONFIG_UART0_RX_UART0_TX) && (baudrate == 9600)) {
        /* If we were on UART0 but the baud rate is 9600 then switch to the
         * LP UART for receive */
        deinit_config(obj);
        obj->config = SERIAL_CONFIG_UARTLP_RX_UART0_TX;
        init_config(obj);
        switch_port_config = true;
    }

    /* Disable UART while writing to control registers */
    obj->reg_base->UARTCR &= ~(1 << 0);

    if (switch_port_config) {
        /* If the port was switched, switch the port configuration also */
        if (format_set) {
            /* This serial port has been previously set up so switch the
             * settings across to this new configuration */
            serial_format(obj, data_bits, parity, stop_bits);
        }
        if (irq_rx_setting != IRQ_NOT_SET) {
            /* Do the same for Rx interrupts, if they were set */
            serial_irq_set(obj, RxIrq, (irq_rx_setting == IRQ_ON));
        }
        if (irq_tx_setting != IRQ_NOT_SET) {
            /* Do the same for Tx interrupts, if they were set */
            serial_irq_set(obj, TxIrq, (irq_tx_setting == IRQ_ON));
        }
    }

    switch (obj->config) {
        case SERIAL_CONFIG_UARTLP_RX_UART0_TX:
        {
            /* Set LP UART to 9600 (numerator 75 (0x4B), denominator 256 (00 == 256)) */
            LP_UART_CTRL = (LP_UART_CTRL & ~0xFFFF) | 0x004B;
            set_baud(obj, baudrate);
        }
        break;
        case SERIAL_CONFIG_UART0_RX_UART0_TX:
        case SERIAL_CONFIG_UART1_RX_UART1_TX:
        {
            set_baud(obj, baudrate);
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }

    /* Enable the UART again */
    obj->reg_base->UARTCR |= 1 << 0;

    obj->baud_rate = baudrate;
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    bool lp_also = false;

    MBED_ASSERT(data_bits >= MIN_NUM_UART_DATA_BITS);
    MBED_ASSERT(data_bits <= MAX_NUM_UART_DATA_BITS);
    MBED_ASSERT(stop_bits >= NUM_UART_STOP_BITS_1);
    MBED_ASSERT(stop_bits <= NUM_UART_STOP_BITS_2);

    /* The LP UART is different to UARTs 0 and 1 so deal with it
     * explicitly when required */
    if (obj->config == SERIAL_CONFIG_UARTLP_RX_UART0_TX) {
        lp_also = true;
    }

    /* Disable UART while writing to control registers */
    obj->reg_base->UARTCR &= ~(1 << 0);

    /* Set data bits (bits 5 and 6 of the UART0/1 register, bits 18 and 19 of the LP UART register) */
    obj->reg_base->UARTLCR_H = (obj->reg_base->UARTLCR_H & ~(0x03 << 5)) | (REGISTER_DATA_BITS(data_bits) << 5);
    if (lp_also) {
        LP_UART_CTRL = (LP_UART_CTRL & ~(0x03 << 18)) | (REGISTER_DATA_BITS(data_bits) << 18);
    }
    obj->format.data_bits = (uint8_t) data_bits;

    /* Set stop bits (bit 7 of the UART0/1 register) (there is no such setting for the LP UART) */
    if (stop_bits == NUM_UART_STOP_BITS_1) {
        /* Clear 2-stop-bits bit */
        obj->reg_base->UARTLCR_H &= ~(1 << 7);
    } else {
        /* Set 2-stop-bits bit */
        obj->reg_base->UARTLCR_H |= 1 << 7;
    }
    obj->format.stop_bits = (uint8_t) stop_bits;

    /* Set parity */
    switch (parity) {
        case ParityNone:
        {
            /* Disable parity */
            obj->reg_base->UARTLCR_H &= ~0x02;
            if (lp_also)
            {
                LP_UART_CTRL &= ~(1 << 24);
            }
        }
        break;
        case ParityOdd:
        {
            /* Set even bit and enable parity */
            obj->reg_base->UARTLCR_H = (obj->reg_base->UARTLCR_H | (1 << 3)) | (1 << 2);
            if (lp_also)
            {
                LP_UART_CTRL |= (1 << 24) | (1 << 25);
            }
        }
        break;
        case ParityEven:
        {
            /* Clear even bit and enable parity */
            obj->reg_base->UARTLCR_H = (obj->reg_base->UARTLCR_H & ~(1 << 3)) | (1 << 2);
            if (lp_also)
            {
                LP_UART_CTRL &= ~(1 << 25);
                LP_UART_CTRL |= 1 << 24;
            }
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }

    /* Enable the UART again */
    obj->reg_base->UARTCR |= 1 << 0;

    obj->format.parity = (uint8_t) parity;
    obj->format_set = true;
}

/* ----------------------------------------------------------------
 * MBED API CALLS: INTERRUPT FUNCTIONS
 * ----------------------------------------------------------------*/

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    bool lp_also = false;

    if (obj->config == SERIAL_CONFIG_UARTLP_RX_UART0_TX) {
        lp_also = true;
    }

    /* Disable UART while writing to control registers */
    obj->reg_base->UARTCR &= ~(1 << 0);

    if (enable) {
        switch (irq) {
            case RxIrq:
            {
                /* Bit 4 for Rx and bit 6 for Rx Timeout */
                obj->reg_base->UARTIMSC |= (1 << 4) | (1 << 6);
                if (lp_also)
                {
                    /* "Word Received" IRQ */
                    LP_UART_CTRL |= 1 << 23;
                }
                obj->irq_rx_setting = IRQ_ON;
                irq_enable(obj);
            }
            break;
            case TxIrq:
            {
                /* Bit 5 */
                obj->reg_base->UARTIMSC |= 1 << 5;
                obj->irq_tx_setting = IRQ_ON;
                irq_enable(obj);
            }
            break;
            default:
            {
                MBED_ASSERT(false);
            }
            break;
        }
    } else {
        switch (irq) {
            case RxIrq:
            {
                /* Bit 4  for Rx and bit 6 for Rx Timeout */
                obj->reg_base->UARTIMSC &= ~((1 << 4) | (1 << 6));
                if (lp_also)
                {
                    /* "Word Received" IRQ */
                    LP_UART_CTRL &= ~(1 << 23);
                }
                obj->irq_rx_setting = IRQ_OFF;
            }
            break;
            case TxIrq:
            {
                /* Bit 5 */
                obj->reg_base->UARTIMSC &= ~(1 << 5);
                obj->irq_tx_setting = IRQ_OFF;
            }
            break;
            default:
            {
                MBED_ASSERT(false);
            }
            break;
        }

        if ((obj->irq_rx_setting == IRQ_OFF) && (obj->irq_tx_setting == IRQ_OFF)) {
            irq_disable(obj);
        }
    }

    /* Enable the UART again */
    obj->reg_base->UARTCR |= 1 << 0;
}

/* ----------------------------------------------------------------
 * MBED API CALLS: TRANSMIT AND RECEIVE FUNCTIONS
 * ----------------------------------------------------------------*/

int serial_getc(serial_t *obj)
{
    uint8_t data = 0;

    /* Block until there is data to read */
    while (!serial_readable(obj)) {}

    /* Read the data */
    switch (obj->config) {
        case SERIAL_CONFIG_UARTLP_RX_UART0_TX:
        {
            data = (uint8_t) LP_UART_DATA;
        }
        break;
        case SERIAL_CONFIG_UART0_RX_UART0_TX:
        case SERIAL_CONFIG_UART1_RX_UART1_TX:
        {
            data = (uint8_t) obj->reg_base->UARTDR;
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }

    return (int) data;
}

void serial_putc(serial_t *obj, int c)
{
    /* Block until there is room to write */
    while (!serial_writable(obj)) {}

    /* Write the data */
    obj->reg_base->UARTDR = (uint8_t) c;
}

int serial_readable(serial_t *obj)
{
    bool readable = false;

    switch (obj->config) {
        case SERIAL_CONFIG_UARTLP_RX_UART0_TX:
        {
            /* Check the status register, bits 8 to 10 indicate
             * the number of Rx bytes (make sure it's the status
             * register not the data register as a read from that
             * register would clear the Rx interrupt) */
            readable = (((LP_UART_STATUS >> 8) & 0x07) != 0);
        }
        break;
        case SERIAL_CONFIG_UART0_RX_UART0_TX:
        case SERIAL_CONFIG_UART1_RX_UART1_TX:
        {
            /* Check the Rx FIFO Empty bit */
            readable = ((obj->reg_base->UARTFR & (1 << 4)) != (1 << 4));
        }
        break;
        default:
        {
            MBED_ASSERT(false);
        }
        break;
    }

    return (int) readable;
}

int serial_writable(serial_t *obj)
{
    /* Check the "UART TX FIFO full" bit:
     * only if this is 0 can we transmit  */
    return (obj->reg_base->UARTFR & (1 << 5)) != (1 << 5);
}

void serial_break_set(serial_t *obj)
{
    /* Disable UART while writing to control registers */
    obj->reg_base->UARTCR &= ~(1 << 0);

    /* Set bit 1 of the line control register */
    obj->reg_base->UARTLCR_H |= 1 << 0;

    /* Enable the UART again */
    obj->reg_base->UARTCR |= 1 << 0;
}

void serial_break_clear(serial_t *obj)
{
    /* Disable UART while writing to control registers */
    obj->reg_base->UARTCR &= ~(1 << 0);

    /* Clear bit 1 of the line control register */
    obj->reg_base->UARTLCR_H &= ~(1 << 0);

    /* Enable the UART again */
    obj->reg_base->UARTCR |= 1 << 0;
}
