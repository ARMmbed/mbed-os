/**************************************************************************//**
 * @file     ADuCM3029.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File for
 *           Device ADuCM3029
 * @version  V3.10
 * @date     23. November 2012
 *
 * @note     Modified 14. November 2016 Analog Devices
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   
   Portions Copyright (c) 2016 Analog Devices, Inc.
   ---------------------------------------------------------------------------*/

#ifndef ADUCM3029_H
#define ADUCM3029_H

#ifndef __ADUCM30xx__
#define __ADUCM30xx__ /*!< PreProcessor feature macro */
#endif

#include <ADuCM3029_cdef.h>
#include <ADuCM3029_device.h>

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV                 0x0201u   /*!< Core Revision r2p1                               */
#define __NVIC_PRIO_BITS          3u        /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             1u        /*!< MPU present                                      */
#define __FPU_PRESENT             0u        /*!< FPU not present                                  */

#include <core_cm3.h>                       /* Cortex-M3 processor and core peripherals           */

#include "system_ADuCM3029.h"

#endif  /* ADUCM3029_H */
