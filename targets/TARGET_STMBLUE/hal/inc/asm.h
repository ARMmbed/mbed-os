/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : compiler.h
* Author             : AMS - VMA RF Application team 
* Version            : V1.0.0
* Date               : 14-September-2015
* Description        : Compiler-dependent macros.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef __ASM_H__
#define __ASM_H__

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#if defined(__ICCARM__) || defined(__IAR_SYSTEMS_ASM__)
#define __CODE__                    SECTION .text:CODE:REORDER:NOROOT(2)
#define __BSS__                     SECTION .bss:DATA:NOROOT(2)
#define __EXPORT__                  EXPORT
#define __IMPORT__                  IMPORT
#define __THUMB__                   THUMB
#define EXPORT_FUNC(f)		            f:
#define __END__                     END
#define __SPACE__                   DS8
#define GLOBAL_VAR(val)               val:
#define ENDFUNC							
#define ALIGN_MEM(n)

#else
#ifdef __GNUC__
.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb
#define __CODE__                    .text
#define __BSS__                     .bss
#define __EXPORT__                  .global
#define __IMPORT__                  .extern
#define __THUMB__                   .thumb_func
#define __END__                     .end
#define __SPACE__                   .space
#define EXPORT_FUNC(f)		            f:
#define GLOBAL_VAR(val)               val:
#define ENDFUNC
#define ALIGN_MEM(n)				.align n>>1

#else
#ifdef __CC_ARM
#define __CODE__		    AREA	|.text|, CODE, READONLY
#define __THUMB__                   
#define EXPORT_FUNC(f)			    f PROC	
#define __BSS__                     AREA	|.bss|, DATA, READWRITE, NOINIT
#define __EXPORT__                  EXPORT
#define __IMPORT__                  IMPORT
#define __SPACE__                   SPACE
#define GLOBAL_VAR(val)             val
#define __END__					END
#define ENDFUNC			    ENDP
#define ALIGN_MEM(n)			  ALIGN n
#else

#endif
#endif
#endif

/* Change this define to 1 if zero-length arrays are not supported by your compiler. */
//#ifndef __CC_ARM
//#define VARIABLE_SIZE 0
//#else
//#define VARIABLE_SIZE 1
//#endif

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
#endif /* __ASM_H__ */
