/*
** ADuCM4050.h
**
** Copyright (C) 2016 Analog Devices, Inc. All Rights Reserved.
**
*/

#ifndef ADUCM4050_H
#define ADUCM4050_H

#include <ADuCM4050_cdef.h>
#include <ADuCM4050_device.h>

#define __CM4_REV              0x0001U /*!< CM4 Core Revision r0p1                */
#define __MPU_PRESENT          1u      /*!< MPU present                           */
#define __FPU_PRESENT          1u      /*!< FPU present                           */
#define __NVIC_PRIO_BITS       3u      /*!< Number of Bits for Priority Levels    */
#define __Vendor_SysTickConfig 0       /*!< 1 if different SysTick Config is used */

#include <core_cm4.h>

#include "system_ADuCM4050.h"

#endif /* ADUCM4050_H */
