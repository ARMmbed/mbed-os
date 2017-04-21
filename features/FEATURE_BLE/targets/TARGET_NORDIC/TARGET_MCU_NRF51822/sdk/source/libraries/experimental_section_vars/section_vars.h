/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef SECTION_VARS_H__
#define SECTION_VARS_H__

#include "app_util.h"

#if defined __ICC_ARM__

// turn on language extensions for iar
#pragma language=extended

#endif

/**
 * @defgroup section_vars Section variables
 * @ingroup app_common
 * @{
 * @brief Section variables.
 */

/**@brief Macro to delay macro expression of pragma
 *
 */
#define NRF_PRAGMA(x) _Pragma(#x)


/**@brief Macro to register section by name in code
 *
 * @param[in]   section_name    Name of the section to register
 **/
#if defined __CC_ARM

// Not required by this compiler
#define NRF_SECTION_VARS_REGISTER_SECTION(section_name)

#elif defined __GNUC__

// Not required by this compiler
#define NRF_SECTION_VARS_REGISTER_SECTION(section_name)

#elif defined __ICCARM__

#define NRF_SECTION_VARS_REGISTER_SECTION(section_name) NRF_PRAGMA(section = ## #section_name )

#else

#error TODO

#endif

/*lint -save -e27 */

/**@brief Macro for accessing start of a named data section by symbol 
 *
 * @details     The symbol that this macro resolves to is used to access the section 
 *              by start address.
 *
 * @param[in]   section_name    Name of the section
 */
#if defined __CC_ARM

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         section_name ## $$Base

#elif defined __GNUC__

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         __start_ ## section_name

#elif defined __ICCARM__

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         __section_begin(#section_name)

#else

#error TODO

#endif


/**@brief Macro for accessing end of a named data section by symbol
 *
 * @details     The symbol that this macro resolves to is used to access the section
 *              by end address.
 *
 * @param[in]   section_name    Name of the section    
 */
#if defined __CC_ARM

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           section_name ## $$Limit

#elif defined __GNUC__

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           __stop_ ## section_name

#elif defined __ICCARM__

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           __section_end(#section_name)

#endif

/*lint -restore */


/**@brief Macro for accessing Length of a named section
 *
 * @details     This macro is used to get the size of a named section.
 *
 * @param[in]   section_name    Name of the section
 */

#if defined __CC_ARM

#define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name))

#elif defined __GNUC__

 #define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name))

#elif defined __ICCARM__

 #define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)NRF_SECTION_VARS_START_SYMBOL(section_name))

#else

#error TODO

#endif
      

/**@brief Macro for accessing the start address of a named section
 *
 * param[in]    section_name    Name of the section to get the start address from
 */
#if defined __CC_ARM

#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name)
      
#elif defined __GNUC__
      
#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name)
      
#elif defined __ICCARM__
      
#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)iar_ ## section_name ## _start

#else
      
#error TODO
      
#endif

      
/*@brief Macro for accessing the end address of a named section
 *
 * @param[in]   section_name    Name of the section to get end address from
 */
#if defined __CC_ARM

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name)
      
#elif defined __GNUC__

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name)
      
#elif defined __ICCARM__

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)iar_ ## section_name ## _end

#else
      
#error TODO
      
#endif


/**@brief Macro for declaring symbols for named sections
 *
 * @note    These external declarations of section specific symbols are required for the linker in GCC and Keil (not IAR)
 *
 * @param[in]   type_name       Name of the type stored in the section
 * @param[in]   section_name    Name of the section
 */
#if defined __CC_ARM

#define NRF_SECTION_VARS_REGISTER_SYMBOLS(type_name, section_name)      \
    extern type_name* NRF_SECTION_VARS_START_SYMBOL(section_name);      \
    extern void* NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined __GNUC__

#define NRF_SECTION_VARS_REGISTER_SYMBOLS(type_name, section_name)      \
    extern type_name* NRF_SECTION_VARS_START_SYMBOL(section_name);      \
    extern void* NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined __ICCARM__

// No symbol registration required for IAR      
#define NRF_SECTION_VARS_REGISTER_SYMBOLS(type_name, section_name)                              \
    extern void*   iar_ ## section_name ## _start   = __section_begin(#section_name);            \
    extern void*   iar_ ## section_name ## _end     = __section_end(#section_name)
      
#else
      
#error TODO

#endif


/**@brief Macro to add symbols to a named section
 *
 * @details     The symbols are placed in a named section. All calls to this macro
 *              will result in symbols being placed in a contiguous manner in the named section. 
 *              This macro ensures that the symbol is not removed because of optimization level.
 *
 * @warning     There is no guarantee for ordering of placement. If ordering is required
 *
 * @warning     The symbols added in the named section must be word aligned to
 *              ensure that compilers do not pad the section during symbol placement.
 *
 * @param[in]   section_name    Name of the section
 * @param[in]   type_def        Datatype of the symbol to place in the given section
 */
#if defined __CC_ARM
    
#define NRF_SECTION_VARS_ADD(section_name, type_def) \
    static type_def __attribute__((section( #section_name ))) __attribute__((used))

#elif defined __GNUC__

#define NRF_SECTION_VARS_ADD(section_name, type_def) \
    static type_def __attribute__ ((section( #section_name ))) __attribute__ ((used))

#elif defined __ICCARM__

#define NRF_SECTION_VARS_ADD(section_name, type_def) \
    __root type_def @ #section_name

#else
      
#error    TODO
      
#endif    

      
/**@brief Macro to get symbol from named section
 *
 * @warning     The stored symbol can only be resolved using this macro if the 
 *              type of the data is word aligned. The operation of acquiring 
 *              the stored symbol relies on sizeof of the stored type, no 
 *              padding can exist in the named section in between individual 
 *              stored items or this macro will fail.
 *
 * @param[in]   i               Index of item in section
 * @param[in]   type_name       Type name of item in section
 * @param[in]   section_name    Name of the section
 */
      
#if defined __CC_ARM

#define NRF_SECTION_VARS_GET(i, type_name, section_name) \
    (type_name*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(type_name))
      
#elif defined __GNUC__

#define NRF_SECTION_VARS_GET(i, type_name, section_name) \
    (type_name*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(type_name))
      
#elif defined __ICCARM__

#define NRF_SECTION_VARS_GET(i, type_name, section_name) \
      (type_name*)iar_ ## section_name ## _start + (i * sizeof(type_name))

#else

#error TODO

#endif


/**@brief Macro to get number of items in named section
 *
 * @param[in]   type_name       Type name of item in section
 * @param[in]   section_name    Name of the section
 */
#define NRF_SECTION_VARS_COUNT(type_name, section_name) \
    NRF_SECTION_VARS_LENGTH(section_name) / sizeof(type_name)

/** @} */

#endif // SECTION_VARS_H__
