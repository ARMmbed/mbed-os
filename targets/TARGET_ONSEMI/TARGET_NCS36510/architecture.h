/**
******************************************************************************
* @file architecture.h
* @brief CM3 architecture specific inclusions and defines.
* @internal
* @author ON Semiconductor
* $Rev: $
* $Date:  $
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
* @ingroup bsp
*/

#ifndef ARCHITECTURE_H_
#define ARCHITECTURE_H_

/*************************************************************************************************
*                                                                                                *
*  Header files                                                                                  *
*                                                                                                *
*************************************************************************************************/

//#include <ncs36510.h>
//#include <system_ncs36510.h>
//#include <core_cm3.h>

#include "NCS36510.h"
#include "system_NCS36510.h"
#include "core_cm3.h"
#include "cmsis.h"


/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/

/* Interrupt Control and State Register (0xE000ED04)
 * 31         NMIPENDSET            R/W     0     NMI pended
 * 28         PENDSVSET             R/W     0     Write 1 to pend system call; Read value
 *                                                indicates pending status
 * 27         PENDSVCLR             W       0     Write 1 to clear PendSV pending status
 * 26         PENDSTSET             R/W     0     Write 1 to pend Systick exception; Read
 *                                                value indicates pending status
 * 25         PENDSTCLR             W       0     Write 1 to clear Systick pending status
 * 23         ISRPREEMPT            R       0     Indicate that a pending interrupt is going
 *                                                to be active in next step (for debug)
 * 22         ISRPENDING            R       0     External interrupt pending (excluding
 *                                                system exceptions such as NMI for
 *                                                fault)
 * 21:12     VECTPENDING            R       0     Pending ISR number
 * 11         RETTOBASE             R       0     Set to 1 when the processor is running
 *                                                an exception handler and will return to
 *                                                thread level if interrupt return and no
 *                                                other exceptions pending
 * 9:0         VECTACTIVE           R       0     Current running interrupt service routine
 */
#define RUNNING_IN_ISR  (((SCB->ICSR & 0x3FF) > 0 ) ? 1 : 0)

#endif /* ARCHITECTURE_H_ */
