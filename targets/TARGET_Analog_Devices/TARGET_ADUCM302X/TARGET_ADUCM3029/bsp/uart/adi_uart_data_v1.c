/*! *****************************************************************************
 * @file      adi_uart_data_v1.c
 * @brief     This file contains the data deceleration required by device driver for
 *             the operating  the  UART device
 * @version: $Revision:
 * @date:    $Date:
 -----------------------------------------------------------------------------
Copyright (c) 2010-2014 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-
INFRINGEMENT, TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF
CLAIMS OF INTELLECTUAL PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef _ADI_UART_DATA_V1_C_
/*! \cond PRIVATE */
#define _ADI_UART_DATA_V1_C_

#include "adi_uart_def_v1.h"
    
#ifdef __ICCARM__
/*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for MMR address casts as in the pADI_UART0 macro here.
*/
#pragma diag_suppress=Pm140
#endif /* __ICCARM__ */
static  ADI_UART_DEVICE_INFO uart_device_info [ ]=
{
	{
		ADI_UART_STATE_UNINITIALIZED,   /*!< Default state of the device   */

		DMA0_CH8_DONE_IRQn,               /*!< Peripheral map for RX-DMA     */

		DMA0_CH9_DONE_IRQn,                  /*!< Peripheral map for TX-DMA     */

		ADI_DMA_CHANNEL_UART_TX,                     /*!< SID for UART error interrupt */
                
		ADI_DMA_CHANNEL_UART_RX,                    /*!< SID for UART error interrupt */
                
                pADI_UART0,   /*!< Start address of the device  */

	        NULL                                     /*!< Location for storing the device handle */
	},

};
#ifdef __ICCARM__
#pragma diag_default=Pm140
#endif /* __ICCARM__ */

#define ADI_UART_NUM_DEVICES (sizeof(uart_device_info)/sizeof(ADI_UART_DEVICE_INFO))
/*! \endcond */

#endif /* _ADI_UART_DATA_V1_C_ */
