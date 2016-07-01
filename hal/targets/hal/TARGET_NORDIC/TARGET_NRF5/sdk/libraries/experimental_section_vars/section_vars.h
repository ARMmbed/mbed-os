/* 
 * Copyright (c) 2016 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
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


/**
 * @defgroup section_vars Section variables
 * @ingroup app_common
 * @{
 *
 * @brief Section variables.
 */

#if defined(__ICCARM__)
// Enable IAR language extensions
#pragma language=extended
#endif


// Macro to delay macro expansion.
#define NRF_PRAGMA(x)       _Pragma(#x)


/**@brief   Macro to register a named section.
 *
 * @param[in]   section_name    Name of the section to register.
 */
#if defined(__CC_ARM)

// Not required by this compiler.
#define NRF_SECTION_VARS_REGISTER_SECTION(section_name)

#elif defined(__GNUC__)

// Not required by this compiler.
#define NRF_SECTION_VARS_REGISTER_SECTION(section_name)

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_REGISTER_SECTION(section_name)     NRF_PRAGMA(section = #section_name)

#endif


/*lint -save -e27 */

/**@brief   Macro to obtain the linker symbol for the beginning of a given section.
 *
 * @details The symbol that this macro resolves to is used to obtain a section start address.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         section_name ## $$Base

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         __start_ ## section_name

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_START_SYMBOL(section_name)         __section_begin(#section_name)

#endif


/**@brief   Macro to obtain the linker symbol for the end of a given section.
 *
 * @details The symbol that this macro resolves to is used to obtain a section end address.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           section_name ## $$Limit

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           __stop_ ## section_name

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_END_SYMBOL(section_name)           __section_end(#section_name)

#endif

/*lint -restore */


/**@brief   Macro for retrieving the length of a given section, in bytes.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name))

#elif defined(__GNUC__)

 #define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name))

#elif defined(__ICCARM__)

 #define NRF_SECTION_VARS_LENGTH(section_name) \
    ((uint32_t)NRF_SECTION_VARS_END_SYMBOL(section_name) - (uint32_t)NRF_SECTION_VARS_START_SYMBOL(section_name))

#endif
      

/**@brief   Macro to obtain the start address of a named section.
 *
 * param[in]    section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name)

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)&NRF_SECTION_VARS_START_SYMBOL(section_name)

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_START_ADDR(section_name)       (uint32_t)iar_ ## section_name ## _start

#endif

      
/*@brief    Macro to obtain the end address of a named section.
 *
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)&NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_END_ADDR(section_name)         (uint32_t)iar_ ## section_name ## _end

#endif


/**@brief   Macro to extern a named section start and stop symbols.
 *
 * @note    These declarations are required for GCC and Keil linkers (but not for IAR's).
 *
 * @param[in]   type_name       Name of the type stored in the section.
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_REGISTER_SYMBOLS(type_name, section_name)  \
    extern type_name * NRF_SECTION_VARS_START_SYMBOL(section_name); \
    extern void      * NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_REGISTER_SYMBOLS(type_name, section_name)  \
    extern type_name * NRF_SECTION_VARS_START_SYMBOL(section_name); \
    extern void      * NRF_SECTION_VARS_END_SYMBOL(section_name)

#elif defined(__ICCARM__)

// No symbol registration required for IAR.
#define NRF_SECTION_VARS_REGISTER_SYMBOLS(type_name, section_name)                  \
    extern void * iar_ ## section_name ## _start = __section_begin(#section_name);  \
    extern void * iar_ ## section_name ## _end   = __section_end(#section_name)

#endif


/**@brief   Macro to declare a variable to be placed in a named section.
 *
 * @details Declares a variable to be placed in a named section. This macro ensures that its symbol
 *          is not stripped by the linker because of optimizations.
 *
 * @warning The order with which variables are placed in a section is implementation dependant.
 *          Generally, variables are placed in a section depending on the order with which they
 *          are found by the linker.
 *
 * @warning The symbols added in the named section must be word aligned to prevent padding.
 *
 * @param[in]   section_name    Name of the section.
 * @param[in]   type_def        Datatype of the variable to place in the given section.
 */
#if defined(__CC_ARM)
    
#define NRF_SECTION_VARS_ADD(section_name, type_def) \
    static type_def __attribute__ ((section(#section_name))) __attribute__((used))

#elif defined(__GNUC__)

#define NRF_SECTION_VARS_ADD(section_name, type_def) \
    static type_def __attribute__ ((section("."#section_name))) __attribute__((used))

#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_ADD(section_name, type_def) \
    __root type_def @ #section_name

#endif


/**@brief   Macro to get symbol from named section.
 *
 * @warning     The stored symbol can only be resolved using this macro if the 
 *              type of the data is word aligned. The operation of acquiring 
 *              the stored symbol relies on sizeof of the stored type, no 
 *              padding can exist in the named section in between individual 
 *              stored items or this macro will fail.
 *
 * @param[in]   i               Index of item in section.
 * @param[in]   type_name       Type name of item in section.
 * @param[in]   section_name    Name of the section.
 */
#if defined(__CC_ARM)

#define NRF_SECTION_VARS_GET(i, type_name, section_name) \
    (type_name*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(type_name))
      
#elif defined(__GNUC__)

#define NRF_SECTION_VARS_GET(i, type_name, section_name) \
    (type_name*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(type_name))
      
#elif defined(__ICCARM__)

#define NRF_SECTION_VARS_GET(i, type_name, section_name) \
    (type_name*)(NRF_SECTION_VARS_START_ADDR(section_name) + i * sizeof(type_name))

#endif


/**@brief   Macro to get number of items in named section.
 *
 * @param[in]   type_name       Type name of item in section.
 * @param[in]   section_name    Name of the section.
 */
#define NRF_SECTION_VARS_COUNT(type_name, section_name) \
    NRF_SECTION_VARS_LENGTH(section_name) / sizeof(type_name)

/** @} */

#endif // SECTION_VARS_H__
