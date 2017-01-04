/*!
 *****************************************************************************
   @file:    adi_uart_config.h
   @brief:   Configuration options for UART driver.
             This is specific to the UART driver and will be included by the driver.
             It is not required for the application to include this header file.
   @version: $Revision: 
   @date:    $Date: 2016-07-05 05:49:46 +0100 (Tue, 05 Jul 2016) $
  -----------------------------------------------------------------------------

Copyright (c) 2014 Analog Devices, Inc.

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

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

#ifndef __ADI_UART_CONFIG_H__
#define __ADI_UART_CONFIG_H__

/** @defgroup UART_Driver_Cfg UART Device Driver Configuration
 *  @ingroup Configuration_macros
 */

/*! @addtogroup UART_Driver_Cfg UART Device Driver Configuration
 *  @{
 */

#include <adi_global_config.h>

/*!
   Set this macro to 1  to enable static controller initializations \n
   during the driver initialization routing.
   To eliminate static driver configuration, set this macro to 0.*/
   
#define ADI_UART_CFG_ENABLE_STATIC_CONFIG_SUPPORT  1


/*!
   Enable DMA support in the driver code.\n
   1 -  To have the DMA support code in the driver.\n
   0 -  To eliminate the DMA support. Operates in core mode.
*/
#define ADI_UART_CFG_ENABLE_DMA_SUPPORT           1

/*!
  Enable/Disable DMA by default.\n
  1 - Enable DMA by default. \n
  0 - Disable DMA by default. PIO mode will be enabled by default.
*/
#define ADI_UART_CFG_ENABLE_DMA                    0

/*!
  Enable/Disable DMA for TX by default.\n
  1 - Enable DMA for TX by default. \n
  0 - Disable DMA for TX by default. PIO mode will be enabled by default.
*/
#define ADI_UART_CFG_ENABLE_DMA_TX                 0

/*!
  Enable/Disable DMA for RX  by default.\n
  1 - Enable DMA for RX by default. \n
  0 - Disable DMA for RX by default. PIO mode will be enabled by default.
*/
#define ADI_UART_CFG_ENABLE_DMA_RX                 0
/************* UART Driver configurations FOR UART ***************/
/*!
   Word length Select.\n
   0 -  5 Bits word length.\n
   1 -  6 Bits word length. \n
   2 -  7 Bits word length.\n
   3 -  8 Bits word length.
*/
#define ADI_UART_CFG_WORD_LENGTH                 3

/*!
   Stop bit selection .\n
   0 -  Send 1 stop bit regardless of the word length \n
   1 -  Send a number of stop bits based on the word length.\n
        WORD-LENGTH 5 Bits       => 1.5 Stop Bits\n
        WORD-LENGTH (6/7/8) Bits => 2   Stop Bits\n
*/
#define ADI_UART_CFG_STOP_BIT                    0

/*!
     Parity Enable. Used to control of the parity bit.\n
   0 -  Parity will not be transmitted or checked.\n
   1 -  Parity will be transmitted and checked.
*/
#define ADI_UART_CFG_ENABLE_PARITY               0


/*!
   Parity Select. This bit only has meaning if parity is enabled. \n
   0 -  Odd parity will be transmitted and checked.\n
   1 - Even parity will be transmitted and checked.
*/
#define ADI_UART_CFG_PARITY_SELECTION            0


/*!
   Stick Parity. Used to force parity to defined values.\n
   0 -  Parity will not be forced \n
   1 -  Set parity based on the following bit settings\n
        EPS = 1 and PEN = 1, parity will be forced to 0.\n
	EPS = 0 and PEN = 1, parity will be forced to 1.\n
	EPS = 1/0 and PEN = 0, no parity will be transmitted.
*/
#define ADI_UART_CFG_ENABLE_STICKY_PARITY       0

/*!
   Fractional baud rate N divide value. \n
   Range: 0 to 2047.
*/
#define ADI_UART_CFG_DIVN                       1734

/*!
   Fractional baud rate N divide value. \n
   Range: 1 to 3.
*/
#define ADI_UART_CFG_DIVM                       3

/*!
   Fractional baud rate N divide value. \n
   Range: 1 to 65535.
*/
#define ADI_UART_CFG_DIVC                    22


/*!
   Over Sample Rate value. \n
   Range: 0 to 3.
*/
#define ADI_UART_CFG_OSR                      3

/*!
   Enable Internal FIFO. \n
   Range: 0 to 1.
*/
#define ADI_UART_CFG_ENABLE_FIFO              0

/*!
   TRIG Level for  UART device. \n
   Range: 0 to 1.
*/
#define ADI_UART_CFG_TRIG_LEVEL               0

/*!
   Number of bits for autobaud detection.  \n
   Range: 0 to 9.
*/
#define ADI_UART_CFG_NUM_BITS                 1

/*!
   Input clock to the UART device.  \n
   
*/
#define ADI_UART_CFG_CLOCK                 26000000

/*!
   To enable the inversion of RX line.  \n
   Range: 0 to 1.
*/
#define ADI_UART_CFG_INVERT_RX_LINE           0

/*!
   Hold TX while RX is active.  \n
   Range: 0 to 1.
*/
#define ADI_UART_CFG_HOLD_TX                  0

/*!
   Disable RX when TX is active. \n
   Range: 0 to 1.
*/
#define ADI_UART_CFG_DISABLE_RX               0

/*!
   To configure the De-assertion.  \n
   Range: 0 to 1.   
*/
#define ADI_UART_CFG_DEASSERTION              0

/*!
   To set the SOUT polarity.  \n
   Range: 0 to 1.   
*/
#define ADI_UART_CFG_SOUT_POLARITY            0

/*!
   To enable the autobaud detection.  \n
   Range: 0 to 1.   
*/
#define ADI_UART_CFG_ENABLE_AUTOBAUD            1

/*!
   To enable the RX status interrupt.  \n
   Range: 0 to 1.   
*/
#define ADI_UART_CFG_ENABLE_RX_STATUS_INTERRUPT        1

/*!
   To enable the Modem status  interrupt.  \n
   Range: 0 to 1.   
*/
#define ADI_UART_CFG_ENABLE_MODEM_STATUS_INTERRUPT     0

#if ((ADI_UART_CFG_ENABLE_DMA == 1) && (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 0))
#error "DMA cannot be enabled if DMA support is disabled"
#endif

#if ((ADI_UART_CFG_ENABLE_DMA_TX == 1) && (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 0))
#error "DMA for TX cannot be enabled if DMA support is disabled"
#endif

#if ((ADI_UART_CFG_ENABLE_DMA_RX == 1) && (ADI_UART_CFG_ENABLE_DMA_SUPPORT == 0))
#error "DMA for RX cannot be enabled if DMA support is disabled"
#endif

/*! @} */

#endif /* __ADI_UART_CONFIG_H__ */
