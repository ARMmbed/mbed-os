/***************************************************************************//**
 * @file
 * @brief RAM code support.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef EM_RAMFUNC_H
#define EM_RAMFUNC_H

#ifdef __cplusplus
extern "C" {
#endif

/* *INDENT-OFF* */
/***************************************************************************//**
 * @addtogroup emlib
 * @{
 * @addtogroup RAMFUNC
 * @brief RAM code support
 * @details
 *  Provides support for executing code from RAM.
 *  Provides a unified method to manage RAM code across all supported tools.
 * @{

  @note
   Other cross-compiler support macros are implemented in @ref COMMON.

  @note
    Functions executing from RAM should not be declared as static.

  @warning
    Standard library facilities are available to the tool with GCC in hosted
	mode (default), regardless of the section attribute. Calls to standard
    libraries placed in the default section may therefore occur. To disable
    hosted mode, add '-ffreestanding' to the build command line. This is the
    only way to guarantee no calls to standard libraries with GCC.
    Read more at www.gcc.gnu.org/onlinedocs/gcc-5.3.0/gcc/Standards.html

  @warning
    Keil/ARM uVision users must add a section named "ram_code" in their linker
    scatter file. This section must be in RAM memory. Look in the MCU SDK for
    example scatter files (ram_code.sct).

  @n @section ramfunc_usage Usage

  In your .h file:
  @verbatim
  #include "em_ramfunc.h"

  SL_RAMFUNC_DECLARATOR
  void MyPrint(const char* string);
  @endverbatim

  Issues have been observed with armgcc when there is no declarator. It is
  recommended to have a declarator also for internal functions, but move the
  declarator to the .c file.

  In your .c file:
  @verbatim
  #include "em_ramfunc.h"

  SL_RAMFUNC_DEFINITION_BEGIN
  void MyPrint(const char* string)
  {
  ...
  }
  SL_RAMFUNC_DEFINITION_END
  @endverbatim

 ******************************************************************************/
/* *INDENT-ON* */

/*******************************************************************************
 ******************************   DEFINES    ***********************************
 ******************************************************************************/

/**
 * @brief
 *    This define is not present by default. By compiling with define
 *    @ref SL_RAMFUNC_DISABLE, code placed in RAM by SL_RAMFUNC macros
 *    will be placed in default code space (Flash) instead.
 *
 * @note
 *    This define is not present by default.
 */
#if defined(DOXY_DOC_ONLY)
#define SL_RAMFUNC_DISABLE
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/* Deprecated macro name  */
#if defined(RAMFUNC_DISABLE)
#define SL_RAMFUNC_DISABLE
#endif
/** @endcond */

#if defined(SL_RAMFUNC_DISABLE)
/** @brief Compiler ported function declarator for RAM code. */
#define SL_RAMFUNC_DECLARATOR

/** @brief Compiler ported function definition begin marker for RAM code. */
#define SL_RAMFUNC_DEFINITION_BEGIN

/** @brief Compiler ported function definition end marker for RAM code. */
#define SL_RAMFUNC_DEFINITION_END

#elif defined(__CC_ARM)
/* MDK-ARM compiler */
#define SL_RAMFUNC_DECLARATOR
#define SL_RAMFUNC_DEFINITION_BEGIN    _Pragma("arm section code=\"ram_code\"")
#define SL_RAMFUNC_DEFINITION_END      _Pragma("arm section code")

#elif defined(__ICCARM__)
/* IAR Embedded Workbench */
#define SL_RAMFUNC_DECLARATOR          __ramfunc
#define SL_RAMFUNC_DEFINITION_BEGIN    SL_RAMFUNC_DECLARATOR
#define SL_RAMFUNC_DEFINITION_END

#elif defined(__GNUC__) && (defined(__CROSSWORKS_ARM) || defined(__SES_ARM))
/* Rowley Crossworks and Segger Embedded Studio */
#define SL_RAMFUNC_DECLARATOR          __attribute__ ((section(".fast")))
#define SL_RAMFUNC_DEFINITION_BEGIN    SL_RAMFUNC_DECLARATOR
#define SL_RAMFUNC_DEFINITION_END

#elif defined(__GNUC__)
/* Simplicity Studio, Atollic and Vanilla armgcc */
#define SL_RAMFUNC_DECLARATOR          __attribute__ ((section(".ram")))
#define SL_RAMFUNC_DEFINITION_BEGIN    SL_RAMFUNC_DECLARATOR
#define SL_RAMFUNC_DEFINITION_END

#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
/* Deprecated macro names */
#define RAMFUNC_DECLARATOR          SL_RAMFUNC_DECLARATOR
#define RAMFUNC_DEFINITION_BEGIN    SL_RAMFUNC_DEFINITION_BEGIN
#define RAMFUNC_DEFINITION_END      SL_RAMFUNC_DEFINITION_END
/** @endcond */

/** @} (end addtogroup RAMFUNC) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif /* EM_RAMFUNC_H */
