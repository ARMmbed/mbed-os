/**
 ******************************************************************************
 * @file types.h
 * @brief Defines a number of general purpose data types.
 * @internal
 * @author ON Semiconductor
 * $Rev: 2074 $
 * $Date: 2013-07-10 18:06:15 +0530 (Wed, 10 Jul 2013) $
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
 * @ingroup util
 */

#ifndef _UTIL_TYPES_H_
#define _UTIL_TYPES_H_

#include "architecture.h"

#include <stdint.h>
#include <stdio.h>

typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long	QWORD;


typedef unsigned char boolean;

#define True		(1)
#define False		(0)

#define Null		NULL

#endif /* _UTIL_TYPES_H_ */
