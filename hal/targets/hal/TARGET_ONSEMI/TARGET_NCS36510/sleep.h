/**
 *******************************************************************************
 * @file    sleep.c
 * @brief Implementation of a sleep functionality
 * @internal
 * @author  ON Semiconductor
 * $Rev:  $
 * $Date: $
 ******************************************************************************
 * @copyright (c) 2015 ON Semiconductor. All rights reserved.
 * ON Semiconductor is supplying this software for use with ON Semiconductor
 * processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ON SEMICONDUCTOR SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * @endinternal
 *
 * @ingroup sleep
 *
 * @details
 * Sleep implementation TBD - Dummy function is fine for first release
 *
 */

#ifndef SLEEP_H_
#define SLEEP_H_

/* Orion specific includes */
#include "types.h"
#include "memory_map.h"
#include "gpio_map.h"
#include "pad_map.h"
#include "crossbar.h"
#include "clock.h"

#define SLEEP_TYPE_NONE			0
#define SLEEP_TYPE_SLEEP		1
#define SLEEP_TYPE_DEEPSLEEP	2
#define SLEEP_TYPE_COMA			3

#define SLEEP_DURATION_SLEEP_MIN		10  		/* msec */
#define SLEEP_DURATION_SLEEP_MAX		200 		/* msec */
#define SLEEP_DURATION_DEEPSLEEP_MAX	500 		/* msec */
#define SLEEP_DURATION_COMA_MAX			1000000000 	/* TODO 1000 sec */

void sleep(void);
void deepsleep(void);
void coma(void);

#endif // SLEEP_H_
