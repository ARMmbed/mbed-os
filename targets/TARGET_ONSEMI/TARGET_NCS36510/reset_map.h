/**
******************************************************************************
* @file reset_map.h
* @brief Reset hw module register map
* @internal
* @author  ON Semiconductor
* $Rev: 2848 $
* $Date: 2014-04-01 22:48:18 +0530 (Tue, 01 Apr 2014) $
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
* @ingroup reset
*
* @details
*/

#ifndef RESET_MAP_H_
#define RESET_MAP_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "architecture.h"

/**************************************************************************************************
*                                                                                                 *
*  Type definitions                                                                               *
*                                                                                                 *
**************************************************************************************************/

/** Reset status and clear register.
 * Also contains HW revision ID.
 */
typedef struct {
    union {
        struct {
            __I uint32_t LOCKUP:1;         /**< 1:Core did lock up */
            __I uint32_t WDOGRES:1;        /**< 1:Watchdog reset occurred */
            __I uint32_t EXTRESET:1;       /**< 1:External reset occurred */
            __I uint32_t SYSRESETREQ:1;    /**< 1:System reset occurred */
            __I uint32_t POR:1;            /**< 1:POR reset occurred */
        } BITS;
        __I uint32_t WORD;
    } SOURCE;
    __O uint32_t CLEARSOURCE;        /**< writing any value to this register will clear the reset source register */
    __I uint32_t HWREVID;            /**< Hardware ID, 0x80215400 */
    __IO uint32_t CONTROL;           /**< External Reset & Watchdog behavior: 0 – External Reset & Watchdog will reset debug logic 1 – External Reset & Watchdog will not reset debug logic */

} ResetReg_t, *ResetReg_pt;

#endif /* RESET_MAP_H_ */
