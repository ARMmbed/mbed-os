/**
******************************************************************************
* @file rfAna.h
* @brief Header of rfAna hw module functions
* @internal
* @author ON Semiconductor
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
* @ingroup rfAna
*/

#ifndef RFANA_H_
#define RFANA_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

#include "types.h"

/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/

/** Miscellaneous I/O codes /
 * @details
 */
#define SET_RF_CHANNEL      (0x0) /**< <b>Ioctl request code</b>: Set Rf channel frequency */
#define SET_TX_POWER        (0x1) /**< <b>Ioctl request code</b>: Set Tx output power */

/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/

/** Function to initialize rfAna
 * @details
 */
void fRfAnaInit(void);

/** Function for miscellaneous I/O and control operations on rfAna
 * @details
 */
boolean fRfAnaIoctl (uint32_t request, void *argument);

#endif /* RFANA_H_ */
