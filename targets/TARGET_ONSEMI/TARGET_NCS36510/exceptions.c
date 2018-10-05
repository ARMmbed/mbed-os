/**
******************************************************************************
* @file exceptions.c
* @brief   Contains relocatable exception table.
* @internal
* @author   ON Semiconductor
* $Rev: 2074 $
* $Date: 2013-07-10 14:36:15 +0200 (Wed, 10 Jul 2013) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a “ON Semiconductor”).
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * (“ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software”) and
 * if applicable the software license agreement.  Do not use this software and/or
 * documentation unless you have carefully read and you agree to the limited terms and
 * conditions.  By using this software and/or documentation, you agree to the limited
 * terms and conditions.
*
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
* INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
* @endinternal
*
* @ingroup app
*
* @details
*/

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

/* BSP inclusions */
#include "device.h"
#include "uart_16c550.h"

#include "memory_map.h"
#include "timer_ncs36510.h"

/* Other inclusions */
#include "types.h"


#include "rtc.h"
#include "gpio.h"
#include "uart_16c550.h"

/*************************************************************************************************
*                                                                                                *
*  Externally declarewd functions                                                                *
*                                                                                                *
*************************************************************************************************/
extern void fSysTickHandler(void);
/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/

/** Not implemented exception, exception handler */
void NotImplemented_Handler(void)
{
    while (1) {};
}

/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/
/** MacHw IRQ handler implemented in the RF driver stack */

/** Call the Timer0 IRQ handler */
void fIrqTim0Handler()
{
    us_timer_isr();
}

/** Call the Timer1 IRQ handler */
void fIrqTim1Handler()
{
    us_ticker_isr();
}

/** Call the Timer2 IRQ handler */
void fIrqTim2Handler()
{
    NotImplemented_Handler();
}

/** Call the Gpio IRQ handler */
void fIrqGpioHandler()
{
    fGpioHandler();
}

/** Call the Spi IRQ handler */
void fIrqSpiHandler()
{
    NotImplemented_Handler();
}

/** Call the Uart 1 IRQ handler */
void fIrqUart1Handler(void)
{
    Uart1_Irq();
}

/** Call the Uart 2 IRQ handler */
void fIrqUart2Handler(void)
{
    Uart2_Irq();
}

#ifdef DEVICE_RTC
/** Call the RTC IRQ handler */
void fIrqRtcHandler(void)
{
    fRtcHandler();
}
#endif

/** Call the I2C IRQ handler */
void fIrqI2CHandler(void)
{
    NotImplemented_Handler();
}

/** Call the I2c2 IRQ handler */
void fIrqI2C2Handler(void)
{
    NotImplemented_Handler();
}

/** Call the Spi2 IRQ handler */
void fIrqSpi2Handler(void)
{
    NotImplemented_Handler();
}

/** Call the UVI IRQ handler */
void fIrqUviHandler(void)
{
    NotImplemented_Handler();
}

/** Call the ADC handler */
void fIrqAdcHandler(void)
{
    NotImplemented_Handler();
}
