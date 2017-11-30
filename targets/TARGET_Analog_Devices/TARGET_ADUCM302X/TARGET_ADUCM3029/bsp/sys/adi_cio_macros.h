/*
** adi_cio_macros.h
**
** Copyright (C) 2016 Analog Devices, Inc. All Rights Reserved.
**
*/

#ifndef _ADI_CIO_MACROS_H
#define _ADI_CIO_MACROS_H

/*
 * Macro definitions in adi_ADuCM4*50_cdef.h and the struct definitions
 * in adi_ADuCM4*50_device.h use macros "__I __C", "__O" and "__IO" to
 * represent read-only, write-only and read/write register attributes.
 *
 * The core_cm4.h include file will define macros __I, __O and __IO as below
 * but it does not define __C.
 *
 * The __C macro is defined to nothing here. The __C macro is intended for
 * the proprietary compilers in CCES to avoid MISRA Rule 19.4 errors regarding
 * permitted macro expansions. The iccarm.exe MISRA checking does not fault
 * the combined "volatile const" __I macro so __C is not required.
 *
 * Each of the macro defines is guarded by a #ifndef check to allow them
 * to be redefined if required.
 *
 * Workaround for 01-00-0757 / 01-00-0759
 */

#ifndef __I
  #ifdef __cplusplus
    #define   __I   volatile       /*!< Defines 'read only' permissions    */
  #else
    #define   __I   volatile const /*!< Defines 'read only' permissions    */
  #endif
#endif

#ifndef __O
  #define   __O     volatile       /*!< Defines 'write only' permissions   */
#endif

#ifndef __IO
  #define   __IO    volatile       /*!< Defines 'read / write' permissions */
#endif

#ifndef __C
  #define     __C     /*nothing*/
#endif

#endif /* _ADI_CIO_MACROS_H */
