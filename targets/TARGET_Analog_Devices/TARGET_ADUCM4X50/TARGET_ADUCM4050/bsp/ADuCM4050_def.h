/*
** ADuCM4050_def.h
**
** Copyright (C) 2016-2017 Analog Devices, Inc. All Rights Reserved.
**
*/

#ifndef _WRAP_ADUCM4050_DEF_H
#define _WRAP_ADUCM4050_DEF_H

#ifdef __ICCARM__
/* IAR MISRA C 2004 error suppressions:
 *
 * Pm008 (rule 2.4): sections of code should not be 'commented out'.
 *   Some comments are wrongly identified as code.
 *
 * Pm009 (rule 5.1): identifiers shall not rely on significance of more than 31 characters.
 *   The YODA-generated headers rely on more. The IAR compiler supports that.
 */
_Pragma("diag_suppress=Pm008,Pm009")
#endif /* __ICCARM__ */

#ifdef __IASMARM__
/* Define masks to plain numeric literal for IAR assembler. */
#define _ADI_MSK_3( mask, smask, type ) (mask)
#endif /* __IASMARM__ */

#include <sys/adi_ADuCM4050.h>

#ifdef __ICCARM__
_Pragma("diag_default=Pm008,Pm009")
#endif /* __ICCARM__ */

#endif /* _WRAP_ADUCM4050_DEF_H */
