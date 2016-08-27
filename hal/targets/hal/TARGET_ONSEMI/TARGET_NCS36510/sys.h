/**
******************************************************************************
* @file sys.h
* @brief Defines the System module
* @internal
* @author  ON Semiconductor
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

#ifndef SYS_H_
#define SYS_H_

/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/

#define SYS_MODULE_ID		0x04

#define SYS_RESET_CODE		0x00
#define SYS_SLEEP_CODE		0x10
#define SYS_DEEPSLEEP_CODE	0x11
#define SYS_COMA_CODE		0x12

#define SYS_RESET_WATCHDOG		0x00
#define SYS_RESET_CORTEX		0x01

#define PWM_ACCESS_CODE     0x30
#define PWM_IOCTLS_CODE		0x31

/*************************************************************************************************
*                                                                                                *
*  Functions                                                                                     *
*                                                                                                *
*************************************************************************************************/

/** Initializes the system module. */
void fSysUiInit(void);

#endif /* SYS_H_ */
