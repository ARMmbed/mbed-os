/**
***************************************************************************
* @file ncs36510_init.h
* @brief Initialization of Orion SoC
* @internal
* @author ON Semiconductor.
* $Rev:
* $Date: $
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


