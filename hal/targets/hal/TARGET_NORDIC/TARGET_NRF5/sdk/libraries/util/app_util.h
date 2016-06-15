/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
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

/** @file
 *
 * @defgroup app_util Utility Functions and Definitions
 * @{
 * @ingroup app_common
 *
 * @brief Various types and definitions available to all applications.
 */

#ifndef APP_UTIL_H__
#define APP_UTIL_H__

#include <stdint.h>
#include <stdbool.h>
#include "compiler_abstraction.h"
#include "nrf.h"

//lint -save -e27 -e10 -e19
#if defined ( __CC_ARM )
extern char STACK$$Base;
extern char STACK$$Length;
#define STACK_BASE    &STACK$$Base
#define STACK_TOP    ((void*)((uint32_t)STACK_BASE + (uint32_t)&STACK$$Length))
#elif defined ( __ICCARM__ )
extern char CSTACK$$Base;
extern char CSTACK$$Length;
#define STACK_BASE    &CSTACK$$Base
#define STACK_TOP    ((void*)((uint32_t)STACK_BASE + (uint32_t)&CSTACK$$Length))
#elif defined   ( __GNUC__ )
extern uint32_t __StackTop;
extern uint32_t __StackLimit;
#define STACK_BASE    &__StackLimit
#define STACK_TOP     &__StackTop
#endif
//lint -restore

enum
{
    UNIT_0_625_MS = 625,                                /**< Number of microseconds in 0.625 milliseconds. */
    UNIT_1_25_MS  = 1250,                               /**< Number of microseconds in 1.25 milliseconds. */
    UNIT_10_MS    = 10000                               /**< Number of microseconds in 10 milliseconds. */
};


/**@brief Implementation specific macro for delayed macro expansion used in string concatenation
*
* @param[in]   lhs   Left hand side in concatenation
* @param[in]   rhs   Right hand side in concatenation
*/
#define STRING_CONCATENATE_IMPL(lhs, rhs) lhs ## rhs


/**@brief Macro used to concatenate string using delayed macro expansion
*
* @note This macro will delay concatenation until the expressions have been resolved
*
* @param[in]   lhs   Left hand side in concatenation
* @param[in]   rhs   Right hand side in concatenation
*/
#define STRING_CONCATENATE(lhs, rhs) STRING_CONCATENATE_IMPL(lhs, rhs)


// Disable lint-warnings/errors for STATIC_ASSERT
//lint --emacro(10,STATIC_ASSERT)
//lint --emacro(18,STATIC_ASSERT)
//lint --emacro(19,STATIC_ASSERT)
//lint --emacro(30,STATIC_ASSERT)
//lint --emacro(37,STATIC_ASSERT)
//lint --emacro(42,STATIC_ASSERT)
//lint --emacro(26,STATIC_ASSERT)
//lint --emacro(102,STATIC_ASSERT)
//lint --emacro(533,STATIC_ASSERT)
//lint --emacro(534,STATIC_ASSERT)
//lint --emacro(132,STATIC_ASSERT)
//lint --emacro(414,STATIC_ASSERT)
//lint --emacro(578,STATIC_ASSERT)
//lint --emacro(628,STATIC_ASSERT)
//lint --emacro(648,STATIC_ASSERT)
//lint --emacro(830,STATIC_ASSERT)


/**@brief Macro for doing static (i.e. compile time) assertion.
*
* @note If the EXPR isn't resolvable, then the error message won't be shown.
*
* @note The output of STATIC_ASSERT will be different across different compilers.
*
* @param[in] EXPR Constant expression to be verified.
*/
#if defined ( __COUNTER__ )

#define STATIC_ASSERT(EXPR) \
    ;enum { STRING_CONCATENATE(static_assert_, __COUNTER__) = 1/(!!(EXPR)) }

#else

#define STATIC_ASSERT(EXPR) \
    ;enum { STRING_CONCATENATE(assert_line_, __LINE__) = 1/(!!(EXPR)) }

#endif


/**@brief Implementation details for NUM_VAR_ARGS */
#define NUM_VA_ARGS_IMPL(                              \
    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,       \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,  \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,  \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,  \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,  \
    _61, _62, N, ...) N


/**@brief Macro to get the number of arguments in a call variadic macro call
 *
 * param[in]    ...     List of arguments
 *
 * @retval  Number of variadic arguments in the argument list
 */
#define NUM_VA_ARGS(...) NUM_VA_ARGS_IMPL(__VA_ARGS__, 63, 62, 61,  \
    60, 59, 58, 57, 56, 55, 54, 53, 52, 51,                         \
    50, 49, 48, 47, 46, 45, 44, 43, 42, 41,                         \
    40, 39, 38, 37, 36, 35, 34, 33, 32, 31,                         \
    30, 29, 28, 27, 26, 25, 24, 23, 22, 21,                         \
    20, 19, 18, 17, 16, 15, 14, 13, 12, 11,                         \
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


/**@brief type for holding an encoded (i.e. little endian) 16 bit unsigned integer. */
typedef uint8_t uint16_le_t[2];

/**@brief Type for holding an encoded (i.e. little endian) 32 bit unsigned integer. */
typedef uint8_t uint32_le_t[4];

/**@brief Byte array type. */
typedef struct
{
    uint16_t  size;                 /**< Number of array entries. */
    uint8_t * p_data;               /**< Pointer to array entries. */
} uint8_array_t;


/**@brief Macro for performing rounded integer division (as opposed to truncating the result).
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Rounded (integer) result of dividing A by B.
 */
#define ROUNDED_DIV(A, B) (((A) + ((B) / 2)) / (B))


/**@brief Macro for checking if an integer is a power of two.
 *
 * @param[in]   A   Number to be tested.
 *
 * @return      true if value is power of two.
 * @return      false if value not power of two.
 */
#define IS_POWER_OF_TWO(A) ( ((A) != 0) && ((((A) - 1) & (A)) == 0) )


/**@brief Macro for converting milliseconds to ticks.
 *
 * @param[in] TIME          Number of milliseconds to convert.
 * @param[in] RESOLUTION    Unit to be converted to in [us/ticks].
 */
#define MSEC_TO_UNITS(TIME, RESOLUTION) (((TIME) * 1000) / (RESOLUTION))


/**@brief Macro for performing integer division, making sure the result is rounded up.
 *
 * @details One typical use for this is to compute the number of objects with size B is needed to
 *          hold A number of bytes.
 *
 * @param[in]   A   Numerator.
 * @param[in]   B   Denominator.
 *
 * @return      Integer result of dividing A by B, rounded up.
 */
#define CEIL_DIV(A, B)      \
    (((A) + (B) - 1) / (B))


/**@brief Macro for creating a buffer aligned to 4 bytes.
 *
 * @param[in]   NAME        Name of the buffor.
 * @param[in]   MIN_SIZE    Size of this buffor (it will be rounded up to multiples of 4 bytes).
 */
#define WORD_ALIGNED_MEM_BUFF(NAME, MIN_SIZE) static uint32_t NAME[CEIL_DIV(MIN_SIZE, sizeof(uint32_t))]


/**@brief Macro for calculating the number of words that are needed to hold a number of bytes.
 *
 * @details Adds 3 and divides by 4.
 *
 * @param[in]  n_bytes  The number of bytes.
 *
 * @return The number of words that @p n_bytes take up (rounded up).
 */
#define BYTES_TO_WORDS(n_bytes) (((n_bytes) + 3) >> 2)


/**@brief The number of bytes in a word.
 */
#define BYTES_PER_WORD (4)


/**@brief Macro for increasing a number to the nearest (larger) multiple of another number.
 *
 * @param[in]  alignment  The number to align to.
 * @param[in]  number     The number to align (increase).
 *
 * @return The aligned (increased) @p number.
 */
#define ALIGN_NUM(alignment, number) ((number - 1) + alignment - ((number - 1) % alignment))


/**@brief Function for changing the value unit.
 *
 * @param[in]   value               Value to be rescaled.
 * @param[in]   old_unit_reversal   Reversal of the incoming unit.
 * @param[in]   new_unit_reversal   Reversal of the desired unit.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint64_t value_rescale(uint32_t value, uint32_t old_unit_reversal, uint16_t new_unit_reversal)
{
    return (uint64_t)ROUNDED_DIV((uint64_t)value * new_unit_reversal, old_unit_reversal);
}

/**@brief Function for encoding a uint16 value.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data is to be written.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint16_encode(uint16_t value, uint8_t * p_encoded_data)
{
    p_encoded_data[0] = (uint8_t) ((value & 0x00FF) >> 0);
    p_encoded_data[1] = (uint8_t) ((value & 0xFF00) >> 8);
    return sizeof(uint16_t);
}

/**@brief Function for encoding a three-byte value.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data is to be written.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint24_encode(uint32_t value, uint8_t * p_encoded_data)
{
    p_encoded_data[0] = (uint8_t) ((value & 0x000000FF) >> 0);
    p_encoded_data[1] = (uint8_t) ((value & 0x0000FF00) >> 8);
    p_encoded_data[2] = (uint8_t) ((value & 0x00FF0000) >> 16);
    return 3;
}

/**@brief Function for encoding a uint32 value.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data is to be written.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint32_encode(uint32_t value, uint8_t * p_encoded_data)
{
    p_encoded_data[0] = (uint8_t) ((value & 0x000000FF) >> 0);
    p_encoded_data[1] = (uint8_t) ((value & 0x0000FF00) >> 8);
    p_encoded_data[2] = (uint8_t) ((value & 0x00FF0000) >> 16);
    p_encoded_data[3] = (uint8_t) ((value & 0xFF000000) >> 24);
    return sizeof(uint32_t);
}

/**@brief Function for encoding a uint48 value.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data is to be written.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint48_encode(uint64_t value, uint8_t * p_encoded_data)
{
    p_encoded_data[0] = (uint8_t) ((value & 0x0000000000FF) >> 0);
    p_encoded_data[1] = (uint8_t) ((value & 0x00000000FF00) >> 8);
    p_encoded_data[2] = (uint8_t) ((value & 0x000000FF0000) >> 16);
    p_encoded_data[3] = (uint8_t) ((value & 0x0000FF000000) >> 24);
    p_encoded_data[4] = (uint8_t) ((value & 0x00FF00000000) >> 32);
    p_encoded_data[5] = (uint8_t) ((value & 0xFF0000000000) >> 40);
    return 6;
}

/**@brief Function for decoding a uint16 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint16_t uint16_decode(const uint8_t * p_encoded_data)
{
        return ( (((uint16_t)((uint8_t *)p_encoded_data)[0])) |
                 (((uint16_t)((uint8_t *)p_encoded_data)[1]) << 8 ));
}

/**@brief Function for decoding a uint16 value in big-endian format.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint16_t uint16_big_decode(const uint8_t * p_encoded_data)
{
        return ( (((uint16_t)((uint8_t *)p_encoded_data)[0]) << 8 ) |
                 (((uint16_t)((uint8_t *)p_encoded_data)[1])) );
}

/**@brief Function for decoding a three-byte value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value (uint32_t).
 */
static __INLINE uint32_t uint24_decode(const uint8_t * p_encoded_data)
{
    return ( (((uint32_t)((uint8_t *)p_encoded_data)[0]) << 0)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[1]) << 8)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[2]) << 16));
}

/**@brief Function for decoding a uint32 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint32_t uint32_decode(const uint8_t * p_encoded_data)
{
    return ( (((uint32_t)((uint8_t *)p_encoded_data)[0]) << 0)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[1]) << 8)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[2]) << 16) |
             (((uint32_t)((uint8_t *)p_encoded_data)[3]) << 24 ));
}

/**@brief Function for decoding a uint32 value in big-endian format.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint32_t uint32_big_decode(const uint8_t * p_encoded_data)
{
    return ( (((uint32_t)((uint8_t *)p_encoded_data)[0]) << 24) |
             (((uint32_t)((uint8_t *)p_encoded_data)[1]) << 16) |
             (((uint32_t)((uint8_t *)p_encoded_data)[2]) << 8)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[3]) << 0) );
}

/**@brief Function for encoding a uint32 value in big-endian format.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data will be written.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint32_big_encode(uint32_t value, uint8_t * p_encoded_data)
{
#ifdef NRF51
    p_encoded_data[0] = (uint8_t) ((value & 0xFF000000) >> 24);
    p_encoded_data[1] = (uint8_t) ((value & 0x00FF0000) >> 16);
    p_encoded_data[2] = (uint8_t) ((value & 0x0000FF00) >> 8);
    p_encoded_data[3] = (uint8_t) ((value & 0x000000FF) >> 0);
#elif NRF52
    *(uint32_t *)p_encoded_data = __REV(value);
#endif
    return sizeof(uint32_t);
}

/**@brief Function for decoding a uint48 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value. (uint64_t)
 */
static __INLINE uint64_t uint48_decode(const uint8_t * p_encoded_data)
{
    return ( (((uint64_t)((uint8_t *)p_encoded_data)[0]) << 0)  |
             (((uint64_t)((uint8_t *)p_encoded_data)[1]) << 8)  |
             (((uint64_t)((uint8_t *)p_encoded_data)[2]) << 16) |
             (((uint64_t)((uint8_t *)p_encoded_data)[3]) << 24) |
             (((uint64_t)((uint8_t *)p_encoded_data)[4]) << 32) |
             (((uint64_t)((uint8_t *)p_encoded_data)[5]) << 40 ));
}

/** @brief Function for converting the input voltage (in milli volts) into percentage of 3.0 Volts.
 *
 *  @details The calculation is based on a linearized version of the battery's discharge
 *           curve. 3.0V returns 100% battery level. The limit for power failure is 2.1V and
 *           is considered to be the lower boundary.
 *
 *           The discharge curve for CR2032 is non-linear. In this model it is split into
 *           4 linear sections:
 *           - Section 1: 3.0V - 2.9V = 100% - 42% (58% drop on 100 mV)
 *           - Section 2: 2.9V - 2.74V = 42% - 18% (24% drop on 160 mV)
 *           - Section 3: 2.74V - 2.44V = 18% - 6% (12% drop on 300 mV)
 *           - Section 4: 2.44V - 2.1V = 6% - 0% (6% drop on 340 mV)
 *
 *           These numbers are by no means accurate. Temperature and
 *           load in the actual application is not accounted for!
 *
 *  @param[in] mvolts The voltage in mV
 *
 *  @return    Battery level in percent.
*/
static __INLINE uint8_t battery_level_in_percent(const uint16_t mvolts)
{
    uint8_t battery_level;

    if (mvolts >= 3000)
    {
        battery_level = 100;
    }
    else if (mvolts > 2900)
    {
        battery_level = 100 - ((3000 - mvolts) * 58) / 100;
    }
    else if (mvolts > 2740)
    {
        battery_level = 42 - ((2900 - mvolts) * 24) / 160;
    }
    else if (mvolts > 2440)
    {
        battery_level = 18 - ((2740 - mvolts) * 12) / 300;
    }
    else if (mvolts > 2100)
    {
        battery_level = 6 - ((2440 - mvolts) * 6) / 340;
    }
    else
    {
        battery_level = 0;
    }

    return battery_level;
}

/**@brief Function for checking if a pointer value is aligned to a 4 byte boundary.
 *
 * @param[in]   p   Pointer value to be checked.
 *
 * @return      TRUE if pointer is aligned to a 4 byte boundary, FALSE otherwise.
 */
static __INLINE bool is_word_aligned(void const* p)
{
    return (((uintptr_t)p & 0x03) == 0);
}

/**
 * @brief Function for checking if provided address is located in stack space.
 *
 * @param[in]   ptr Pointer to be checked.
 *
 * @return      true if address is in stack space, false otherwise.
 */
static __INLINE bool is_address_from_stack(void * ptr)
{
    if (((uint32_t)ptr >= (uint32_t)STACK_BASE) &&
        ((uint32_t)ptr <  (uint32_t)STACK_TOP) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // APP_UTIL_H__

/** @} */
