/**
******************************************************************************
* @file swversion.c
* @brief Defines the system revision for the current application.
* @internal
* @author  ON Semiconductor
* $Rev: 2199 $
* $Date: 2013-08-07 12:17:27 +0200 (Wed, 07 Aug 2013) $
******************************************************************************
 * Copyright 2016 Semiconductor Components Industries LLC (d/b/a "ON Semiconductor").
 * All rights reserved.  This software and/or documentation is licensed by ON Semiconductor
 * under limited terms and conditions.  The terms and conditions pertaining to the software
 * and/or documentation are available at http://www.onsemi.com/site/pdf/ONSEMI_T&C.pdf
 * ("ON Semiconductor Standard Terms and Conditions of Sale, Section 8 Software") and
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
* @ingroup common_mib
*/

/* Other inclusions */
#include <stdint.h>
#include "fib.h"

/*************************************************************************************************
*                                                                                                *
*  Global variables                                                                              *
*                                                                                                *
*************************************************************************************************/

#ifdef __ICCARM__
/** Define a fib table constant region, to be located at fixed offset in the binary
 * such that flash loader knows where to find it and gets the build dependent data
 * it needs for programming the new fib.
 */
__root const fibtable_t fib_table @ "FIBTABLE" = {LOAD_ADDRESS,{0x0,0x00,0x00,0x00}};
#endif /* __ICCARM__ */

const mib_systemRevision_t systemRevision = {
    0x82,    /**< hardware revision */
    0x00,    /**< patch level */
    0x01,    /**< Build number */
    0x00,    /**< feature set, Minor version */
    0x01,    /**< generation, Major version */
    'E'      /**< release */
};
