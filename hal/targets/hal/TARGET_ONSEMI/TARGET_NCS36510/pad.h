#ifdef REVD
/**
******************************************************************************
* @file pad.h
* @brief PAD Power management support code
* @internal
* @author ON Semiconductor
* $Rev: 2848 $
* $Date: 2014-04-01 22:48:18 +0530 (Tue, 01 Apr 2014) $
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
* @ingroup pad
*
* @details
*/

#ifndef _PAD_H_
#define _PAD_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "types.h"
#include "memory_map.h"
#include "gpio_map.h"
#include "pad_map.h"
#include "crossbar.h"
#include "clock.h"

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/



#define CROSS_REG_ADRS_BYTE_SIZE                4

#define PAD_REG_ADRS_BYTE_SIZE                  4


/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/

/**
* @brief
* Configures the PAD IO for desired behavior
*/
extern void fPadInit();

/**
* @brief
* Set ouptput drive strength, output drive type & pull type for PAD IO
*
* @param PadNum Pad number
* @param OutputDriveStrength Ouput drive strength. Ref Table: O/p drive strength
* @param OutputDriveType Push/pull: 0; opern dran: 1
* @param PullType Pull down active: 0; no pull active:1 or 2; pull up active: 3
*
* @return true for success; false for invalid parameters
*/
extern boolean fPadIOCtrl(uint8_t, uint8_t, uint8_t, uint8_t);

#endif //_PAD_H_
#endif /* REVD */