/*
** ADuCM4050_typedefs.h
**
** Copyright (C) 2016 Analog Devices, Inc. All Rights Reserved.
**
*/

#ifndef _WRAP_ADUCM4050_TYPEDEFS_H
#define _WRAP_ADUCM4050_TYPEDEFS_H

#include <stdint.h>

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm008 (rule 2.4): sections of code should not be 'commented out'.
 *   Some comments are wrongly identified as code.
 *
 * Pm093 (rule 18.4): use of union - overlapping storage shall not be used.
 *    Unions are required by sys/adi_ADuCM4050_typedefs.h.
 */
_Pragma("diag_suppress=Pm008,Pm093")
#endif /* __ICCARM__ */

#include <sys/adi_ADuCM4050_typedefs.h>

#ifdef __ICCARM__
_Pragma("diag_default=Pm008,Pm093")
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM4050_TYPEDEFS_H */
