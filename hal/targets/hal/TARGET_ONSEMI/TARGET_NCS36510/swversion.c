/**
******************************************************************************
* @file swversion.c
* @brief Defines the system revision for the current application.
* @internal
* @author  ON Semiconductor
* $Rev: 2199 $
* $Date: 2013-08-07 12:17:27 +0200 (Wed, 07 Aug 2013) $
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

#ifdef IAR
/** Define a fib table constant region, to be located at fixed offset in the binary
 * such that flash loader knows where to find it and gets the build dependent data
 * it needs for programming the new fib.
 */
__root const fibtable_t fib_table @ "FIBTABLE" = { LOAD_ADDRESS,{0x0,0x00,0x00,0x00}};
#endif /* IAR */

const mib_systemRevision_t systemRevision = {
    0x82,	/**< hardware revision */
    0x00,	/**< patch level */
    0x01,	/**< Build number */
    0x00,	/**< feature set, Minor version */
    0x01,	/**< generation, Major version */
    'E'		/**< release */
};
