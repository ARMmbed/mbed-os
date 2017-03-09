/**
******************************************************************************
* @file crossbar.h
* @brief CROSSBAR hw module register map
* @internal
* @author ON Semiconductor
* $Rev: 2033 $
* $Date: 2013-06-28 17:12:31 +0200 (Fri, 28 Jun 2013) $
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
* @ingroup crossbar
*
* @details
* Defines magic numbers for the different peripheral devices.
*/

#ifndef _CROSSBAR_H_
#define _CROSSBAR_H_

/*************************************************************************************************
*                                                                                                *
*  Symbolic Constants                                                                            *
*                                                                                                *
*************************************************************************************************/

#define CONFIGURE_AS_GPIO        (uint8_t)0x00
#define CONFIGURE_AS_RESERVED_0  (uint8_t)0x01
#define CONFIGURE_AS_RESERVED_1  (uint8_t)0x02
#define CONFIGURE_AS_RESERVED_2  (uint8_t)0x03
#define CONFIGURE_AS_PWM         (uint8_t)0x04
#define CONFIGURE_AS_I2C         (uint8_t)0x05
#define CONFIGURE_AS_SPI         (uint8_t)0x06
#define CONFIGURE_AS_UART        (uint8_t)0x07

#endif //_CROSSBAR_H_
