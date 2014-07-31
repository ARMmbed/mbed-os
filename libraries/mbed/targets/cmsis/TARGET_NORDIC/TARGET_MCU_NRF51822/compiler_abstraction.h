/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic
 * Semiconductor ASA.Terms and conditions of usage are described in detail
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef _COMPILER_ABSTRACTION_H
#define _COMPILER_ABSTRACTION_H

/*lint ++flb "Enter library region" */

#if defined ( __CC_ARM )
  #define __ASM            __asm                                      /*!< asm keyword for ARM Compiler          */
  #define __INLINE         __inline                                   /*!< inline keyword for ARM Compiler       */
  #define __STATIC_INLINE  static __inline

#elif defined ( __ICCARM__ )
  #define __ASM            __asm                                      /*!< asm keyword for IAR Compiler          */
  #define __INLINE         inline                                     /*!< inline keyword for IAR Compiler. Only available in High optimization mode! */
  #define __STATIC_INLINE  static inline
  #define __current_sp() __get_SP()
  
#elif defined ( __GNUC__ )
  #define __ASM            __asm                                      /*!< asm keyword for GNU Compiler          */
  #define __INLINE         inline                                     /*!< inline keyword for GNU Compiler       */
  #define __STATIC_INLINE  static inline

static __INLINE unsigned int __current_sp(void)
  {
      register unsigned sp asm("sp");
      return sp;
  }

#elif defined ( __TASKING__ )
  #define __ASM            __asm                                      /*!< asm keyword for TASKING Compiler      */
  #define __INLINE         inline                                     /*!< inline keyword for TASKING Compiler   */
  #define __STATIC_INLINE  static inline

#endif

/*lint --flb "Leave library region" */

#endif
