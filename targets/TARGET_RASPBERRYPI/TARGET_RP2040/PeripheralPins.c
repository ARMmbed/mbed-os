#include "pinmap.h"
#include "objects.h"
#include "PeripheralPins.h"

const PinMap PinMap_FULL[] = {
    {p0,  0, 0},
    {p1,  0, 0},
    {p2,  0, 0},
    {p3,  0, 0},
    {p4,  0, 0},
    {p5,  0, 0},
    {p6,  0, 0},
    {p7,  0, 0},
    {p8,  0, 0},
    {p9,  0, 0},
    {p10, 0, 0},
    {p11, 0, 0},
    {p12, 0, 0},
    {p13, 0, 0},
    {p14, 0, 0},
    {p15, 0, 0},
    {p16, 0, 0},
    {p17, 0, 0},
    {p18, 0, 0},
    {p19, 0, 0},
    {p20, 0, 0},
    {p21, 0, 0},
    {p22, 0, 0},
    {p23, 0, 0},
    {p24, 0, 0},
    {p25, 0, 0},
    {p26, 0, 0},
    {p27, 0, 0},
    {p28, 0, 0},
    {p29, 0, 0},
    {NC, NC, 0}
};


/************UART***************/
const PinMap PinMap_UART_TX[] = {
    {p0,  UART_0, (uint32_t) uart0},
    {p4,  UART_1, (uint32_t) uart1},
    {p8,  UART_1, (uint32_t) uart1},
    {p12, UART_0, (uint32_t) uart0},
    {p16, UART_0, (uint32_t) uart0},
    {p20, UART_1, (uint32_t) uart1},
    {p24, UART_1, (uint32_t) uart1},
    {p28, UART_0, (uint32_t) uart0},
    {NC, NC, 0}
};

const PinMap PinMap_UART_RX[] = {
    {p1,  UART_0, (uint32_t) uart0},
    {p5,  UART_1, (uint32_t) uart1},
    {p9,  UART_1, (uint32_t) uart1},
    {p13, UART_0, (uint32_t) uart0},
    {p17, UART_0, (uint32_t) uart0},
    {p21, UART_1, (uint32_t) uart1},
    {p25, UART_1, (uint32_t) uart1},
    {p29, UART_0, (uint32_t) uart0},
    {NC, NC, 0}
};

const PinMap PinMap_UART_CTS[] = {
    {p2,  UART_0, (uint32_t) uart0},
    {p6,  UART_1, (uint32_t) uart1},
    {p10, UART_1, (uint32_t) uart1},
    {p14, UART_0, (uint32_t) uart0},
    {p18, UART_0, (uint32_t) uart0},
    {p22, UART_1, (uint32_t) uart1},
    {p26, UART_1, (uint32_t) uart1},
    {NC, NC, 0}
};

const PinMap PinMap_UART_RTS[] = {
    {p3,  UART_0, (uint32_t) uart0},
    {p7,  UART_1, (uint32_t) uart1},
    {p11, UART_1, (uint32_t) uart1},
    {p15, UART_0, (uint32_t) uart0},
    {p19, UART_0, (uint32_t) uart0},
    {p23, UART_1, (uint32_t) uart1},
    {p27, UART_1, (uint32_t) uart1},
    {NC, NC, 0}
};