#ifndef INCLUDED_TIMER
#define INCLUDED_TIMER
/*
 * Copyright (C) u-blox 
 * All rights reserved. 
 * This source file is the sole property of u-blox. Reproduction or utilization 
 * of this source in whole or part is forbidden without the written consent of 
 * u-blox.
 *
 */

#include <stdint.h>

/** timer HAL Spreadsheet version number */
#define TIMER_HAL_VERSION 1

/** TIMER IP.  
This module has the following features : 
- APB slave interface
- 32 bit down counters with 3 different settings of 8-bit prescale counter 
- Timers programmable as 32 or 16 bit counters
- Timers can run continuously or for single-shot 
- Can be programmed in free running or periodic mode
- Stall feature supported via external stall input
*/
struct timer_s {
   /** Timer 1 control regiter  at address offset 0x000, read-write */
   uint32_t timer1_con;
   /** Timer1 load register at address offset 0x004, read-write */
   uint32_t timer1_load;
   /** Timer 1 current count register at address offset 0x008, read-only */
   uint32_t timer1_value;
   /** Timer 1 Status register at address offset 0x00C, read-write */
   uint32_t timer1_stat;
   /** Reserved space */
   uint8_t fill0[16];
   /** Timer 2control regiter  at address offset 0x020, read-write */
   uint32_t timer2_con;
   /** Timer 2 load register at address offset 0x024, read-write */
   uint32_t timer2_load;
   /** Timer 2 current count register at address offset 0x028, read-only */
   uint32_t timer2_value;
   /** Timer 2 Status register at address offset 0x02C, read-write */
   uint32_t timer2_stat;
};

/** bit field defines for timer_s#timer1_con */
#define TIMER_TIMER1_CON_EN_OFFSET 0
#define TIMER_TIMER1_CON_EN_SIZE 1
#define TIMER_TIMER1_CON_IRQEN_OFFSET 1
#define TIMER_TIMER1_CON_IRQEN_SIZE 1
#define TIMER_TIMER1_CON_MODE_OFFSET 2
#define TIMER_TIMER1_CON_MODE_SIZE 1
#define TIMER_TIMER1_CON_SIZE_OFFSET 3
#define TIMER_TIMER1_CON_SIZE_SIZE 1
#define TIMER_TIMER1_CON_ONESHOT_OFFSET 4
#define TIMER_TIMER1_CON_ONESHOT_SIZE 1
#define TIMER_TIMER1_CON_PRESCALE_OFFSET 5
#define TIMER_TIMER1_CON_PRESCALE_SIZE 2

/** bit field defines for timer_s#timer1_load */
#define TIMER_TIMER1_LOAD_LOADVAL_OFFSET 0
#define TIMER_TIMER1_LOAD_LOADVAL_SIZE 32

/** bit field defines for timer_s#timer1_value */
#define TIMER_TIMER1_VALUE_CURVAL_OFFSET 0
#define TIMER_TIMER1_VALUE_CURVAL_SIZE 32

/** bit field defines for timer_s#timer1_stat */
#define TIMER_TIMER1_STAT_OVF_OFFSET 0
#define TIMER_TIMER1_STAT_OVF_SIZE 1

/** bit field defines for timer_s#timer2_con */
#define TIMER_TIMER2_CON_EN_OFFSET 0
#define TIMER_TIMER2_CON_EN_SIZE 1
#define TIMER_TIMER2_CON_IRQEN_OFFSET 1
#define TIMER_TIMER2_CON_IRQEN_SIZE 1
#define TIMER_TIMER2_CON_MODE_OFFSET 2
#define TIMER_TIMER2_CON_MODE_SIZE 1
#define TIMER_TIMER2_CON_SIZE_OFFSET 3
#define TIMER_TIMER2_CON_SIZE_SIZE 1
#define TIMER_TIMER2_CON_ONESHOT_OFFSET 4
#define TIMER_TIMER2_CON_ONESHOT_SIZE 1
#define TIMER_TIMER2_CON_PRESCALE_OFFSET 5
#define TIMER_TIMER2_CON_PRESCALE_SIZE 2

/** bit field defines for timer_s#timer2_load */
#define TIMER_TIMER2_LOAD_LOADVAL_OFFSET 0
#define TIMER_TIMER2_LOAD_LOADVAL_SIZE 32

/** bit field defines for timer_s#timer2_value */
#define TIMER_TIMER2_VALUE_CURVAL_OFFSET 0
#define TIMER_TIMER2_VALUE_CURVAL_SIZE 32

/** bit field defines for timer_s#timer2_stat */
#define TIMER_TIMER2_STAT_OVF_OFFSET 0
#define TIMER_TIMER2_STAT_OVF_SIZE 1

/* EOF timer.h */
#endif
