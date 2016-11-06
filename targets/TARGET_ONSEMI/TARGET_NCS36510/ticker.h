/**
 ******************************************************************************
 * @file ticker.h
 * @brief Microcontroller uSec ticker
 * @internal
 * @author ON Semiconductor.
 * $Rev:
 * $Date:
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
 *
 */

#ifndef TICKER_H_
#define TICKER_H_

#include "types.h"

/** Core frequency definitions.  /
 *
 * These definitions should be adjusted to setup Orion core frequencies.
 */
#define    CPU_CLOCK_ROOT_HZ           ( ( unsigned long ) 32000000) /**< <b> Orion 32MHz root frequency </b> */
#define    CPU_CLOCK_DIV_32M           ( 1 ) /**< <b> Divider to set up core frequency at 32MHz </b> */
#define    CPU_CLOCK_DIV_16M           ( 2 ) /**< <b> Divider to set up core frequency at 16MHz </b> */
#define    CPU_CLOCK_DIV_8M            ( 4 ) /**< <b> Divider to set up core frequency at 8MHz </b> */
#define    CPU_CLOCK_DIV_4M            ( 8 ) /**< <b> Divider to set up core frequency at 4MHz </b> */

#define    CPU_CLOCK_DIV               CPU_CLOCK_DIV_32M /**< <b> Selected divider to be used by application code </b> */

#define configCPU_CLOCK_HZ             ( ( unsigned long ) (CPU_CLOCK_ROOT_HZ/CPU_CLOCK_DIV) )
#define configTICK_RATE_HZ             ( ( unsigned long ) 1000000 ) // 1uSec ticker rate


/* Lowest priority */

#define configKERNEL_INTERRUPT_PRIORITY         ( 0xFF )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( 0x8F )

#define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ

/* Constants required to manipulate the core.  Registers first... */
#define portNVIC_SYSTICK_CTRL_REG            ( * ( ( volatile unsigned long * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG            ( * ( ( volatile unsigned long * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG   ( * ( ( volatile unsigned long * ) 0xe000e018 ) )
#define portNVIC_INT_CTRL_REG                ( * ( ( volatile unsigned long * ) 0xe000ed04 ) )
#define portNVIC_SYSPRI2_REG                 ( * ( ( volatile unsigned long * ) 0xe000ed20 ) )

/* ...then bits in the registers. */
#define portNVIC_SYSTICK_CLK_BIT            ( 1UL << 2UL )
#define portNVIC_SYSTICK_INT_BIT            ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT         ( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT     ( 1UL << 16UL )

/* Orion has 4 interrupt priority bits
 */
#define portNVIC_SYSTICK_PRI                ( ( ( unsigned long ) configKERNEL_INTERRUPT_PRIORITY ) << 24 )

/* API definitions */
void fSysTickInit(void);

void fSysTickHandler(void);

uint32_t fSysTickRead(void);

void fSysTickEnableInterrupt (void);

void fSysTickDisableInterrupt (void);
#endif // TICKER_H_
