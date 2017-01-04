/*!
 *****************************************************************************
   @file:    adi_global_config.h
   @brief:   Global configuration options which will affect all the drivers.
   @version: $Revision: 35198 $
   @date:    $Date: 2016-07-29 16:14:00 +0100 (Fri, 29 Jul 2016) $
  -----------------------------------------------------------------------------

Copyright (c) 2012-2016 Analog Devices, Inc.

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

/* This file contains all the global configurations */
#ifndef __ADI_GLOBAL_CONFIG_H__
#define __ADI_GLOBAL_CONFIG_H__

/**  @defgroup Configuration_macros Device Driver Configuration Macros
 */

/** @defgroup Global_Config Global Configuration Settings
 *  @ingroup Configuration_macros
 */

/** @addtogroup Global_Config Global Configuration Settings
 *  @{
 */

/*! Set this macro to 1 to enable multi threaded support.
    Do not change this macro if you are using CrossCore Embedded Studio. The
    CCES add-ins define a own to indicate that the RTOS is present. 
 */
#if !defined (__GNUC__)
    #define ADI_CFG_ENABLE_RTOS_SUPPORT                   0
#else  /* __GNUC__ */
    #if defined (ADI_THREADS)
        #define ADI_CFG_ENABLE_RTOS_SUPPORT               1
    #else
        #define ADI_CFG_ENABLE_RTOS_SUPPORT               0
    #endif
#endif

/*! Set this macro to put the processor in low power mode when waiting for the buffer processing to complete */
#define ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT                0

#if (ADI_CFG_ENTER_LOW_PWR_MODE_SUPPORT ==1 ) && (ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
#error " Low power mode supported only in NON-RTOS environment"
#endif
/*! @} */

/*! Set this macro to enable the profiling*/
#define ADI_PROFILE_ENABLED             0

#if (ADI_CFG_ENABLE_RTOS_SUPPORT ==1)
#if !defined(USER_SPECIFIED_RTOS)
#define ADI_DEVICE_DRIVER_RTOS_MEMORY  68u
#endif
#else
#define ADI_DEVICE_DRIVER_RTOS_MEMORY  4u
#endif
#endif /* __ADI_GLOBAL_CONFIG_H__ */
