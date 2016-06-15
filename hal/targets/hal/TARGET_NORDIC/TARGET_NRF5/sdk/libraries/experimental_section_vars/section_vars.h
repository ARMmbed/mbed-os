/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
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
