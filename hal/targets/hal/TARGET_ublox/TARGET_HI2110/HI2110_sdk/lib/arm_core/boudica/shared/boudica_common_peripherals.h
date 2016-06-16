/******************************************************************************
 * @brief    BOUDICA Common peripheral Types and Register Flags Definitions Header
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/
#include "neulfw.h"

#ifndef BOUDICA_COMMON_PERIPHERALS_H_
#define BOUDICA_COMMON_PERIPHERALS_H_


/*------------- LOW POWER CONTROLLER  -----------------------------*/
/* Some LPC  Register Definitions */
#define LPC_CTRL_POWER_DOWN_FLASH_WHEN_ALL_ASSERTING_DEEP_SLEEP_SHIFT (0)
#define LPC_CTRL_POWER_DOWN_FLASH_WHEN_ALL_ASSERTING_DEEP_SLEEP_MASK (1u<<LPC_CTRL_POWER_DOWN_FLASH_WHEN_ALL_ASSERTING_DEEP_SLEEP_SHIFT)

#define LPC_CTRL_ENTER_DEEP_SLEEP_WHEN_ALL_ASSERTING_DEEP_SLEEP_SHIFT (1)
#define LPC_CTRL_ENTER_DEEP_SLEEP_WHEN_ALL_ASSERTING_DEEP_SLEEP_MASK (1u<<LPC_CTRL_ENTER_DEEP_SLEEP_WHEN_ALL_ASSERTING_DEEP_SLEEP_SHIFT)

#define LPC_CTRL_GATE_OFF_CLOCK_SHIFT (7)
#define LPC_CTRL_GATE_OFF_CLOCK_MASK (1u<<LPC_CTRL_GATE_OFF_CLOCK_SHIFT)

#define LPC_CTRL_USE_RTC_CLK_FOR_GPIO_SHIFT (8)
#define LPC_CTRL_USE_RTC_CLK_FOR_GPIO_MASK (1u<<LPC_CTRL_USE_RTC_CLK_FOR_GPIO_SHIFT)

/*------------- CLOCKS  -----------------------------*/
/* Some CLK_GATE_SYS  Register Definitions */
#define CLK_GATE_SYS_CLOCK_DIVIDER_SHIFT       0
#define CLK_GATE_SYS_CLOCK_DIVIDER_MASK        0xF

#define CLK_GATE_SYS_RADIO_ROUTE_ENABLE_SHIFT  8  	// according to the code, nevertheless it is not consistent with the information in CIOTSW-95.
// A confirmation is needed.
#define CLK_GATE_SYS_RADIO_ROUTE_ENABLE_MASK   (1u<<CLK_GATE_SYS_RADIO_ROUTE_ENABLE_SHIFT)


/*------------- UART -----------------------------*/
/**Uart register structure*/
typedef struct
{
    uint32 UARTDR;
    uint32 UARTRSR;
    uint32 res0;
    uint32 res1;
    uint32 res2;
    uint32 res3;
    uint32 UARTFR;
    uint32 res4;
    uint32 UARTILPR;
    uint32 UARTIBRD; // Integer baud divider
    uint32 UARTFBRD; // Fractional Baud divider
    uint32 UARTLCR_H;
    uint32 UARTCR;
    uint32 UARTIFLS;
    uint32 UARTIMSC;
    uint32 UARTRIS;
    uint32 UARTMIS;
    uint32 UARTICR;
    uint32 UARTDMACR;
} uart_ctrl_t;

#endif /* BOUDICA_COMMON_PERIPHERALS_H_ */
