
#if DEVICE_SERIAL

#include <string.h>

#include "cmsis.h"
#include "mbed_assert.h"
#include "mbed_error.h"
#include "PeripheralPins.h"
#include "pinmap.h"

#include "serial_api.h"

#include "drivers/peripheral/sysclk/cy_sysclk.h"
#include "drivers/peripheral/gpio/cy_gpio.h"
#include "drivers/peripheral/scb/cy_scb_uart.h"

serial_t stdio_uart;
int stdio_uart_inited = 0;


void serial_init_clock(serial_t *obj, uint32_t baudrate)
{
    uint32_t periClk = 50*1000*1000; // UART uses periClk (50 MHz)
    uint32_t oversample = 12; // this must match the cy_stc_scb_uart_config_t cy_UART_config.oversample in serial_init_periph()

    uint32_t divider = periClk/(oversample*baudrate);

    Cy_SysClk_PeriphAssignDivider(PCLK_SCB5_CLOCK, CY_SYSCLK_DIV_16_BIT, 0);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0, divider);
 //   Cy_SysClk_PeriphPhaseAlignDivider(CY_SYSCLK_DIV_16_BIT, 0, CY_SYSCLK_DIV_24_5_BIT, 0xFF);
}

/*
    Returns the UART's SCBx_BASE corresponding to the pin, NULL if not found.
*/
CySCB_Type *pin_to_scb(PinName pin)
{
    switch (pin) {
        case P0_2:  // P0_2_SCB0_UART_RX
        case P0_3:  // P0_3_SCB0_UART_TX
            return SCB0;
            break;

        case P10_0:  // P10_0_SCB1_UART_RX
        case P10_1:  // P10_1_SCB1_UART_TX
            return SCB1;
            break;

        case P9_0:  // P9_0_SCB2_UART_RX
        case P9_1:  // P9_1_SCB2_UART_TX
            return SCB2;
            break;

        case P6_0:  // P6_0_SCB3_UART_RX
        case P6_1:  // P6_1_SCB3_UART_TX
            return SCB3;
            break;

        case P7_0:  // P7_0_SCB4_UART_RX
        case P7_1:  // P7_1_SCB4_UART_TX
        case P8_0:  // P8_0_SCB4_UART_RX
        case P8_1:  // P8_1_SCB4_UART_TX
            return SCB4;
            break;

        case P5_0:  // P5_0_SCB5_UART_RX
        case P5_1:  // P5_1_SCB5_UART_TX
        case P11_0:  // P11_0_SCB5_UART_RX
        case P11_1:  // P11_1_SCB5_UART_TX
            return SCB5;
            break;

        case P12_0:  // P12_0_SCB6_UART_RX
        case P12_1:  // P12_1_SCB6_UART_TX
        case P13_0:  // P13_0_SCB6_UART_RX
        case P13_1:  // P13_1_SCB6_UART_TX
        case P6_4:  // P6_4_SCB6_UART_RX
        case P6_5:  // P6_5_SCB6_UART_TX
            return SCB6;
            break;

        case P1_0:  // P1_0_SCB7_UART_RX
        case P1_1:  // P1_1_SCB7_UART_TX
            return SCB7;
            break;

        default:
            return NULL;
            break;
    }
}

void serial_init_pins(serial_t *obj, PinName tx, PinName rx)
{
    CySCB_Type *scb_tx = NULL, *scb_rx = NULL;
    GPIO_PRT_Type *port = NULL;
    uint32_t pin = 0;

    obj->pin_tx = tx;
    obj->pin_rx = rx;

    scb_tx = pin_to_scb(tx);
    scb_rx = pin_to_scb(rx);

    if ((scb_tx == NULL) || (scb_rx == NULL))
        error("Invalid Serial pinout. Try another TX or RX pin.");

    if (scb_tx != scb_rx)
        error("Serial pinout mismatch. Requested pins Rx and Tx can't be used for the same Serial communication.");

    obj->base = scb_tx;

    // init tx
    port = Cy_GPIO_PortToAddr(CYPRESS_PORT(obj->pin_tx));
    pin = CYPRESS_PIN(obj->pin_tx);
    cy_stc_gpio_pin_config_t config_tx = {
        .outVal     = 0x01,
        .driveMode  = 6,
        .hsiom      = HSIOM_SEL_ACT_6,
    };
    Cy_GPIO_Pin_Init(port, pin, &config_tx);

    // init rx
    port = Cy_GPIO_PortToAddr(CYPRESS_PORT(obj->pin_rx));
    pin = CYPRESS_PIN(obj->pin_rx);
    cy_stc_gpio_pin_config_t config_rx = {
        .outVal     = 0x01,
        .driveMode  = 8,
        .hsiom      = HSIOM_SEL_ACT_6,
    };
    Cy_GPIO_Pin_Init(port, pin, &config_rx);
}

void serial_init_periph(serial_t *obj)
{
    cy_stc_scb_uart_config_t cy_UART_config =
    {
        .uartMode                   = CY_SCB_UART_STANDARD,
        .enableMutliProcessorMode   = false,
        .smartCardRetryOnNack       = false,
        .irdaInvertRx               = false,
        .irdaEnableLowPowerReceiver = false,

        .oversample                 = 12UL,

        .enableMsbFirst             = false,
        .dataWidth                  = 8UL,
        .parity                     = CY_SCB_UART_PARITY_NONE,
        .stopBits                   = CY_SCB_UART_STOP_BITS_1,
        .enableInputFilter          = false,
        .breakWidth                 = 11UL,
        .dropOnFrameError           = false,
        .dropOnParityError          = false,

        .receiverAddress            = 0x0UL,
        .receiverAddressMask        = 0x0UL,
        .acceptAddrInFifo           = false,

        .enableCts                  = false,
        .ctsPolarity                = CY_SCB_UART_ACTIVE_LOW,
        .rtsRxFifoLevel             = 0UL,
        .rtsPolarity                = CY_SCB_UART_ACTIVE_LOW,

        .rxFifoTriggerLevel         = 0UL,
        .rxFifoIntEnableMask        = 0x0UL,

        .txFifoTriggerLevel         = 0UL,
        .txFifoIntEnableMask        = 0x0UL
    };

    Cy_SCB_UART_Init(obj->base, &cy_UART_config, NULL);
    Cy_SCB_UART_Enable(obj->base);
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    if (stdio_uart_inited)
        return;
    stdio_uart_inited = 1;

    serial_init_clock(obj, 9600);
    serial_init_pins(obj, tx, rx);
    serial_init_periph(obj);
}

void serial_baud(serial_t *obj, int baudrate)
{
    serial_init_clock(obj, baudrate);
    serial_init_periph(obj);
}

void serial_putc(serial_t *obj, int c)
{
    while (!serial_writable(obj))
        ;
    Cy_SCB_UART_Put(obj->base, c);
}

int serial_getc(serial_t *obj)
{
    while (!serial_readable(obj))
        ;
    return Cy_SCB_UART_Get(obj->base);
}

int serial_readable(serial_t *obj)
{
    return Cy_SCB_GetNumInRxFifo(obj->base) != 0;
}

int serial_writable(serial_t *obj)
{
    return Cy_SCB_GetNumInTxFifo(obj->base) != Cy_SCB_GetFifoSize(obj->base);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{

}

#endif
