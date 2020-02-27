/***************************************************************************//**
 * @file
 * @brief Emlib peripheral API "assert" implementation.
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

#ifndef EM_ASSERT_H
#define EM_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emlib
 * @{
 ******************************************************************************/

#if defined(DOXY_DOC_ONLY)
/** @brief Included for documentation purposes only. This define is not present by default.
 *  @ref DEBUG_EFM should be defined from the compiler to enable the default internal
 *  assert handler. */
#define DEBUG_EFM
/** @endcond */
#endif

#if defined(DEBUG_EFM) || defined(DEBUG_EFM_USER)
/***************************************************************************//**
 * @addtogroup ASSERT
 * @brief Error checking module.
 * @details
 * By default, EMLIB library assert usage is not included to reduce
 * footprint and processing overhead. Further, EMLIB assert usage is decoupled
 * from ISO C assert handling (NDEBUG usage) to allow using ISO C
 * assert without including EMLIB assert statements.
 *
 * Below are available defines for controlling EMLIB assert inclusion. The defines
 * are typically for a project to be used by the preprocessor.
 *
 * @li If @ref DEBUG_EFM is defined, the internal EMLIB library assert handling will
 * be used. This is implemented as a simple while(true) loop. @ref DEBUG_EFM is not
 * defined by default.
 *
 * @li If DEBUG_EFM_USER is defined, the user must provide custom
 * implementation of the assertEFM() function.
 *
 * @li If both @ref DEBUG_EFM and DEBUG_EFM_USER are undefined, all EFM_ASSERT()
 * statements are not operational.
 *
 * @note
 * The internal EMLIB assert is documented because @ref DEBUG_EFM is defined in
 * the doxygen configuration.
 * @{
 ******************************************************************************/
/* Due to footprint considerations, we only pass file name and line number, */
/* not the assert expression (nor function name (C99)) */
void assertEFM(const char *file, int line);
#define EFM_ASSERT(expr)    ((expr) ? ((void)0) : assertEFM(__FILE__, __LINE__))

#else

/** Default assertion is not operational */
#define EFM_ASSERT(expr)    ((void)(expr))

#endif /* defined(DEBUG_EFM) || defined(DEBUG_EFM_USER) */

/** @} (end addtogroup ASSERT) */
/** @} (end addtogroup emlib) */

#ifdef __cplusplus
}
#endif

#endif /* EM_ASSERT_H */
