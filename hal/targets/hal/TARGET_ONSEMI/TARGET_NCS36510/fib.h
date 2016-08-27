/**
******************************************************************************
* @file sys.h
* @brief Defines a FIB (Firmware Information Base)
* @internal
* @author ON Semiconductor
* $Rev: 2074 $
* $Date: 2013-07-10 14:36:15 +0200 (Wed, 10 Jul 2013) $
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
* @ingroup sys
*
* @details
*
*/

#ifndef FIB_H_
#define FIB_H_

#include "mib.h"
/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

typedef struct {
    unsigned int fwbase_address;
    mib_systemRevision_t fw_version;
} fibtable_t;


#endif