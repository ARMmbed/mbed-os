/**
***************************************************************************
* @file ncs36510_init.h
* @brief Initialization of Orion SoC
* @internal
* @author Radhika R, ON Semiconductors
* $Rev: 
* $Date: $
******************************************************************************
* @copyright (c) 2012 ON Semiconductor. All rights reserved.
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
* @ingroup main
*
* @details
*/
#ifndef NCS36510_H_
#define NCS36510_H_

/* BSP inclusions */
#include "architecture.h"
#include "memory_map.h"
#include "ticker.h"
#include "gpio.h"
#include "gpio_api.h"

/* Other inclusions */
#include "types.h"

boolean fTrim(void);

void fClockInit(void) ;

uint32_t fClockGetPeriphClockfrequency(void);

void fNcs36510Init(void);

#endif //NCS36510_H_


