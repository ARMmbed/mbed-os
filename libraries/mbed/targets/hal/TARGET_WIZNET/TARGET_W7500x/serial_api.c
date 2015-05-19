#include "mbed_assert.h"
#include "serial_api.h"

#if DEVICE_SERIAL

#include "cmsis.h"
#include "pinmap.h"
#include <string.h>
#include "PeripheralPins.h"

#define UART_NUM (2)

static uint32_t serial_irq_ids[UART_NUM] = {0, 0};


static uart_irq_handler irq_handler;
static UART_TypeDef *UART;


UART_InitTypeDef UART_InitStructure;

int stdio_uart_inited = 0;
serial_t stdio_uart;

static void init_uart(serial_t *obj)
{
    UART = (UART_TypeDef *)(obj->uart);
    UART_InitStructure.UART_BaudRate            = obj->baudrate;
    UART_InitStructure.UART_WordLength          = obj->databits;
    UART_InitStructure.UART_StopBits            = obj->stopbits;
    UART_InitStructure.UART_Parity              = obj->parity;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;


    if (obj->pin_rx == NC) {
        UART_InitStructure.UART_Mode = UART_Mode_Tx;
    } else if (obj->pin_tx == NC) {
        UART_InitStructure.UART_Mode = UART_Mode_Rx;
    } else {
        UART_InitStructure.UART_Mode = (UART_Mode_Rx | UART_Mode_Tx);
    }

    UART_Init(UART,&UART_InitStructure);
}

void serial_init(serial_t *obj, PinName tx, PinName rx)
{
    // Determine the UART to use (UART_1, UART_2, ...)
    UARTName uart_tx = (UARTName)pinmap_peripheral(tx, PinMap_UART_TX);
    UARTName uart_rx = (UARTName)pinmap_peripheral(rx, PinMap_UART_RX);

    // Get the peripheral name (UART_1, UART_2, ...) from the pin and assign it to the object
    obj->uart = (UARTName)pinmap_merge(uart_tx, uart_rx);

    MBED_ASSERT(obj->uart != (UARTName)NC);

    // Enable USART clock
    if (obj->uart == UART_0) {
        obj->index = 0;
    }

    if (obj->uart == UART_1) {
        obj->index = 1;
    }

    // Configure the UART pins
    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);
    if (tx != NC) {
        pin_mode(tx, PullUp);
    }
    if (rx != NC) {
        pin_mode(rx, PullUp);
    }

    // Configure UART
    obj->baudrate = 9600;
    obj->databits = UART_WordLength_8b;
    obj->stopbits = UART_StopBits_1;
    obj->parity   = UART_Parity_No;

    obj->pin_tx = tx;
    obj->pin_rx = rx;


    init_uart(obj);

    // For stdio management
    if (obj->uart == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
}

void serial_free(serial_t *obj)
{
    // Reset UART and disable clock
    if (obj->uart == UART_0) {
    }

    if (obj->uart == UART_1) {
    }


    // Configure GPIOs
//    pin_function(obj->pin_tx, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
//    pin_function(obj->pin_rx, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));

    serial_irq_ids[obj->index] = 0;
}

void serial_baud(serial_t *obj, int baudrate)
{
    obj->baudrate = baudrate;
    init_uart(obj);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits)
{
    if (data_bits == 7) {
        obj->databits = UART_WordLength_7b;
    } else {
        obj->databits = UART_WordLength_8b;
    }

    switch (parity) {
        case ParityOdd:
        case ParityForced0:
            obj->parity = UART_Parity_Odd;
            break;
        case ParityEven:
        case ParityForced1:
            obj->parity = UART_Parity_Even;
            break;
        default: // ParityNone
            obj->parity = UART_Parity_No;
            break;
    }

    if (stop_bits == 2) {
        obj->stopbits = UART_StopBits_2;
    } else {
        obj->stopbits = UART_StopBits_1;
    }

    init_uart(obj);
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/

static void uart_irq(UARTName name, int id)
{
    UART = (UART_TypeDef *)name;
    if (serial_irq_ids[id] != 0) {
       if( UART_GetITStatus(UART,UART_IT_FLAG_TXI) != RESET ){
           irq_handler(serial_irq_ids[id], TxIrq);
           UART_ClearITPendingBit(UART,UART_IT_FLAG_TXI);
        }
       if( UART_GetITStatus(UART,UART_IT_FLAG_RXI) != RESET ){
           irq_handler(serial_irq_ids[id], RxIrq);
       }
    }
}

#ifdef __cplusplus
extern "C"{
#endif
void UART0_Handler()
{
    uart_irq(UART_0, 0);
}

void UART1_Handler()
{
    uart_irq(UART_1, 1);
}
#ifdef __cplusplus
}
#endif


void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    irq_handler = handler;
    serial_irq_ids[obj->index] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    IRQn_Type irq_n = (IRQn_Type)0;

    UART = (UART_TypeDef *)(obj->uart);

    if (obj->uart == UART_0) {
        irq_n = UART0_IRQn;
    }

    if (obj->uart == UART_1) {
        irq_n = UART1_IRQn;
    }

    if (enable) {
        if (irq == RxIrq) {
            UART_ITConfig(UART,UART_IT_FLAG_RXI,ENABLE);
        } else { // TxIrq
            UART_ITConfig(UART,UART_IT_FLAG_TXI,ENABLE);
        }

        NVIC_ClearPendingIRQ(irq_n);
        NVIC_EnableIRQ(irq_n);
    } else { // disable
        UART_ITConfig(UART,(UART_IT_FLAG_RXI|UART_IT_FLAG_TXI),DISABLE);
        NVIC_DisableIRQ(irq_n);
    }
}

/******************************************************************************
 * READ/WRITE
 ******************************************************************************/

int serial_getc(serial_t *obj)
{
    UART_TypeDef *uart = (UART_TypeDef *)(obj->uart);
//    while(!serial_readable(obj));
    while(uart->FR & UART_FR_RXFE);

    return (uart->DR & 0xFF);
}

void serial_putc(serial_t *obj, int c)
{
    UART_TypeDef *uart = (UART_TypeDef *)(obj->uart);

    uart->DR = (uint32_t)(c & (uint16_t)0xFF);
    while(uart->FR & UART_FR_BUSY);
}

int serial_readable(serial_t *obj)
{
    int status;
    UART_TypeDef *uart = (UART_TypeDef *)(obj->uart);
    // Check if data is received
    status = ((uart->FR & UART_FR_RXFE) ? 0: 1);
    return status;
}

int serial_writable(serial_t *obj)
{
    int status;
    UART_TypeDef *uart = (UART_TypeDef *)(obj->uart);
    // Check if data is transmitted
    status = ((uart->FR & UART_FR_BUSY) ? 0: 1);
    return status;
}

void serial_clear(serial_t *obj)
{
//    UartHandle.Instance = (USART_TypeDef *)(obj->uart);
//    __HAL_UART_CLEAR_IT(&UartHandle, UART_FLAG_TC);
//    __HAL_UART_SEND_REQ(&UartHandle, UART_RXDATA_FLUSH_REQUEST);
}

void serial_pinout_tx(PinName tx)
{
    //pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj)
{
    // [TODO]
}

void serial_break_clear(serial_t *obj)
{
    // [TODO]
}

#endif
