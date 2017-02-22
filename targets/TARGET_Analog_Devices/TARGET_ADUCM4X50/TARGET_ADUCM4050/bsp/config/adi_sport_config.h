/*! ****************************************************************************
 * @file    adi_sport_config.h
 * @brief   Configuration options for SPORT driver.
 * @details This is specific to the SPORT driver and will be included by the
 *          driver. It is not required for the application to include this
 *          header file.
  -----------------------------------------------------------------------------
Copyright (c) 2016 Analog Devices, Inc.

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

*******************************************************************************/
#ifndef ADI_SPORT_CONFIG_H
#define ADI_SPORT_CONFIG_H
#include <adi_global_config.h>

/** @addtogroup SPORT_Driver_Config Static Configuration
 *  @ingroup SPORT_Driver
 *  @{
 */

/************* SPORT Driver configurations FOR SPORT-0-A ***************/
/*!
   Frame Sync Multiplexer Select.\n
   0 - Disable frame sync multiplexing\n
   1 - Enable frame sync multiplexing.
*/
#define ADI_CFG_SPORT0A_ENABLE_FSMUXSEL             (0u)

/*!
   Clock Multiplexer Select.\n
   0 - Disable serial clock multiplexing\n
   1 - Enable serial clock multiplexing.
*/
#define ADI_CFG_SPORT0A_ENABLE_CKMUXSEL             (1u)

/*!
   Least-Significant Bit First.\n
   0 - MSB first sent/received.\n
   1 - LSB first sent/received.
*/
#define ADI_CFG_SPORT0A_LSB_FIRST                   (0u)


/*!
   Serial Word Length in bits.\n
   1 - 32 - SPORT word length
*/
#define ADI_CFG_SPORT0A_SERIAL_WLEN                 (32u)


/*!
   Internal Clock.\n
   0 - External clock.\n
   1 - Internal clock.
*/
#define ADI_CFG_SPORT0A_INTERNAL_CLK                (1u)

/*!
   Operation Mode\n
   0 - DSP standard.\n
   1 - Timer_enable mode.
*/
#define ADI_CFG_SPORT0A_OPERATION_MODE              (0u)


/*!
   Clock Rising Edge\n
   0 - Clock falling edge\n
   1 - Clock rising edge.
*/
#define ADI_CFG_SPORT0A_CLOCK_EDGE                  (0u)

/*!
   Frame Sync Required\n
   0 - No frame sync required \n
   1 - Frame sync required.
*/
#define ADI_CFG_SPORT0A_FS_REQUIRED                 (1u)

/*!
   Internal Frame Sync\n
   0 - External frame sync\n
   1 - Internal frame sync
*/
#define ADI_CFG_SPORT0A_INTERNAL_FS                 (0u)


/*!
   Data-Independent Frame Sync\n
   0 - Data-dependent frame sync\n
   1 - Data-independent frame 
*/
#define ADI_CFG_SPORT0A_DATA_INDEPENDENT_FS         (0u)

/*!
   Active-Low Frame Sync\n
   0 - Active high frame sync\n
   1 - Active low frame sync 
*/
#define ADI_CFG_SPORT0A_ACTIVE_LOW_FS               (0u)

/*!
   Late Frame Sync\n
   0 - Early frame sync\n
   1 - Late frame sync
*/
#define ADI_CFG_SPORT0A_LATE_FS                     (0u)

/*!
   Enable Packing \n
   0 - Disable\n
   1 - 8-bit packing enable\n
   2 - 16-bit packing enable
*/
#define ADI_CFG_SPORT0A_ENABLE_PACKING              (0u)

/*!
   Frame Sync Error Operation
   0 - Flag the Frame Sync error\n
   1 - When frame Sync error occurs, discard the receive data
*/
#define ADI_CFG_SPORT0A_FS_ERROR_OPERATION          (1u)

/*!
   Enabling Gated Clock\n
   0 - Disable Gated Clock\n
   1 - Enable Gated Clock
*/
#define ADI_CFG_SPORT0A_GATED_CLOCK                 (0u)

/*!
   Serial Clock divisor.\n
   0 - 65535 -   Serial Clock Divisor which  SPORT device use to  calculate the serial 
                 clock (ACLK) from the processor system clock (PCLK).
*/
#define ADI_CFG_SPORT0A_CLOCK_DIVISOR               (2u)

/*!
   Frame Sync Divisor.\n
   0 - 128 -    Frame Sync Divisor which select the number of transmit or receive clock
                cycles that the half SPORT counts before generating a frame sync pulse.
*/
#define ADI_CFG_SPORT0A_FS_DIVISOR                  (0x40u)


/*!
   CONVT to FS duration.\n
   0 - 128 -    Specify the value of the number of clocks which would be programmed 
                corresponding to the desired time duration from assertion of CONVT 
                signal to Frame sync signal
*/
#define ADI_CFG_SPORT0A_CONVT_FS_DURATION           (1u)

/*!
   Polarity of the Convt signal.\n
   0 - Active High Polarity\n
   1 - Active low Polarity
*/
#define ADI_CFG_SPORT0A_CONVT_POLARITY              (0u)

/*!
   CONVT signal width.\n
   0 - 15 -     Specify the value of the number of serial clocks for which CONVT
                signal should be active
   
*/
#define ADI_CFG_SPORT0A_CONVT_WIDTH                 (1u)

#if defined(ADI_CFG_SPORT0A_SERIAL_WLEN)
#if (ADI_CFG_SPORT0A_SERIAL_WLEN <= 3u) || (ADI_CFG_SPORT0A_SERIAL_WLEN > 32u)
#error "Invalid word length : it must be between 4 and 32"
#endif
#else
#error "ADI_CFG_SPORT0A_SERIAL_WLEN undefined!!! "
#endif

/************* SPORT Driver configurations FOR SPORT-0-B ***************/
/*!
   Least-Significant Bit First.\n
   0 - MSB first sent/received.\n
   1 - LSB first sent/received.
*/
#define ADI_CFG_SPORT0B_LSB_FIRST                   (0u)


/*!
   Serial Word Length in bits.\n
   1 - 32 - SPORT word length
*/
#define ADI_CFG_SPORT0B_SERIAL_WLEN                 (32u)


/*!
   Internal Clock.\n
   0 - External clock.\n
   1 - Internal clock.
*/
#define ADI_CFG_SPORT0B_INTERNAL_CLK                (1u)

/*!
   Operation Mode\n
   0 - DSP standard.\n
   1 - Timer_enable mode.
*/
#define ADI_CFG_SPORT0B_OPERATION_MODE              (0u)


/*!
   Clock Rising Edge\n
   0 - Clock falling edge\n
   1 - Clock rising edge.
*/
#define ADI_CFG_SPORT0B_CLOCK_EDGE                  (0u)

/*!
   Frame Sync Required\n
   0 - No frame sync required \n
   1 - Frame sync required.
*/
#define ADI_CFG_SPORT0B_FS_REQUIRED                 (1u)

/*!
   Internal Frame Sync\n
   0 - External frame sync\n
   1 - Internal frame sync
*/
#define ADI_CFG_SPORT0B_INTERNAL_FS                 (1u)


/*!
   Data-Independent Frame Sync\n
   0 - Data-dependent frame sync\n
   1 - Data-independent frame 
*/
#define ADI_CFG_SPORT0B_DATA_INDEPENDENT_FS         (0u)

/*!
   Active-Low Frame Sync\n
   0 - Active high frame sync\n
   1 - Active low frame sync 
*/
#define ADI_CFG_SPORT0B_ACTIVE_LOW_FS               (0u)

/*!
   Late Frame Sync\n
   0 - Early frame sync\n
   1 - Late frame sync
*/
#define ADI_CFG_SPORT0B_LATE_FS                     (0u)

/*!
   Enable Packing \n
   0 - Disable\n
   1 - 8-bit packing enable\n
   2 - 16-bit packing enable\n
*/
#define ADI_CFG_SPORT0B_ENABLE_PACKING              (0u)

/*!
   Frame Sync Error Operation\n
   0 - Flag the Frame Sync error\n
   1 - When frame Sync error occurs, discard the receive data
*/
#define ADI_CFG_SPORT0B_FS_ERROR_OPERATION          (1u)

/*!
   Enabling Gated Clock\n
   0 - Disable Gated Clock\n
   1 - Enable Gated Clock
*/
#define ADI_CFG_SPORT0B_GATED_CLOCK                 (0u)

/*!
   Serial Clock divisor.\n
   0 - 65535 -   Serial Clock Divisor which SPORT device use to calculate the serial 
                 clock (ACLK) from the processor system clock (PCLK).
*/
#define ADI_CFG_SPORT0B_CLOCK_DIVISOR               (2u)

/*!
   Frame Sync Divisor.\n
   0 - 128 -    Frame Sync Divisor which select the number of transmit or receive clock
                cycles that the half SPORT counts before generating a frame sync pulse.
*/
#define ADI_CFG_SPORT0B_FS_DIVISOR                  (0x40u)


/*!
   CONVT to FS duration.\n
   0 - 128 -    Specify the value of the number of clocks which would be programmed 
                corresponding to the desired time duration from assertion of CONVT 
                signal to Frame sync signal
*/
#define ADI_CFG_SPORT0B_CONVT_FS_DURATION           (1u)

/*!
   Polarity of the Convt signal.\n
   0 - Active High Polarity\n
   1 - Active low Polarity
*/
#define ADI_CFG_SPORT0B_CONVT_POLARITY              (0u)

/*!
   CONVT signal width.\n
   0-15 -       Specify the value of the number of serial clocks for which CONVT
                signal should be active
   
*/
#define ADI_CFG_SPORT0B_CONVT_WIDTH                 (1u)

#if defined(ADI_CFG_SPORT0B_SERIAL_WLEN)
#if (ADI_CFG_SPORT0B_SERIAL_WLEN <= 3u) || (ADI_CFG_SPORT0B_SERIAL_WLEN > 32u)
#error "Invalid word length : it must be between 4 and 32"
#endif
#else
#error "ADI_CFG_SPORT0B_SERIAL_WLEN undefined!!! "
#endif

/*! @} */

#endif /* ADI_SPORT_CONFIG_H */
