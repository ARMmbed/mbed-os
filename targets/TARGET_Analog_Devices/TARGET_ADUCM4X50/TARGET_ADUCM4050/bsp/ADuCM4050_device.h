/*
** ADuCM4050_device.h
**
** Copyright (C) 2016 Analog Devices, Inc. All Rights Reserved.
**
*/

#ifndef _WRAP_ADUCM4050_DEVICE_H
#define _WRAP_ADUCM4050_DEVICE_H

#include <ADuCM4050_typedefs.h>
#include <sys/adi_cio_macros.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm093 (rule 18.4): use of union - overlapping storage shall not be used.
 *    Unions are required by sys/adi_ADuCM4050_device.h.
 */
_Pragma("diag_suppress=Pm093")
#endif /* __ICCARM__ */

#include <sys/adi_ADuCM4050_device.h>

#ifdef __ICCARM__
_Pragma("diag_default=Pm093")
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM4050_DEVICE_H */
