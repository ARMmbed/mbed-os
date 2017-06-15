/*---------------------------------------------------------------------------
 * Copyright (c) 2016, u-blox Malmö, All Rights Reserved
 * SPDX-License-Identifier: LicenseRef-PBL
 *
 * This file and the related binary are licensed under the
 * Permissive Binary License, Version 1.0 (the "License");
 * you may not use these files except in compliance with the License.
 *
 * You may obtain a copy of the License here:
 * LICENSE-permissive-binary-license-1.0.txt and at
 * https://www.mbed.com/licenses/PBL-1.0
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Component   : RTSL
 * File        : cb_types.h
 *
 * Description : Common type definitions
 *-------------------------------------------------------------------------*/

/**
 * @file cb_types.h Defines type required for the entire driver.
 * The defines in this file will have to be adapted for the platform.
 * @ingroup platform
 */

#ifndef _CB_TYPES_H_
#define _CB_TYPES_H_

#include "cb_port_types.h"

/*===========================================================================
 * TYPES
 *=========================================================================*/

/*===========================================================================
 * COMMON SYSTEM DEFINES
 *=========================================================================*/

#ifndef FALSE
#  define FALSE           (0)
#endif
#ifndef TRUE
#  define TRUE            (!FALSE)
#endif

#ifndef NULL
#  define NULL            ((void *) 0)
#endif

/**
 * Returns the maximum value of the two parameters.
 */
#ifndef cb_MAX
#  define cb_MAX(x , y)   (((x) > (y)) ? (x) : (y))
#endif
/**
 * Returns the minimum value of the two parameters.
 */
#ifndef cb_MIN
#  define cb_MIN(x , y)   (((x) < (y)) ? (x) : (y))
#endif

#ifndef ELEMENTS_OF
#  define ELEMENTS_OF(_array) (sizeof((_array)) / sizeof((_array)[0]))
#endif

#define cbWM_ARRAY_SIZE(a)          ELEMENTS_OF(a)

/**
 * Used when declaring an empty array that does not take up space in a struct.
 * Example: struct { cb_uint8 payload[cb_EMPTY_ARRAY]; }
 * In some compilers this is empty i.e. payload[]. While in some it requires a zero.
 * I.e. payload[0];
 * Use this define to get it working for your system.
 */
#ifndef cb_EMPTY_ARRAY
#  define cb_EMPTY_ARRAY  (0)
#endif

/*===========================================================================
 * DEFINES
 *=========================================================================*/
/**
 * Used in function definitions to declare an inparameter unused to avoid warnings.
 */
#ifndef cb_UNUSED
#  define cb_UNUSED(x)    x
#endif

#ifndef cb_ASSERT
#  error "No port definition for ASSERT!"
#endif

#ifndef cb_ARG_POINTER_CHECK
#  define cb_ARG_POINTER_CHECK(ptr) if((ptr) == NULL) {cb_ASSERT(FALSE); return;}
#endif
#ifndef cb_ARG_POINTER_CHECK_RETURN
#  define cb_ARG_POINTER_CHECK_RETURN(ptr, returnValue) if((ptr) == NULL) {cb_ASSERT(FALSE); return (returnValue);}
#endif

#ifndef cb_BIT_0
#define cb_BIT_0        (1ul)
#endif
#ifndef cb_BIT_1
#define cb_BIT_1        (1ul << 1)
#endif
#ifndef cb_BIT_2
#define cb_BIT_2        (1ul << 2)
#endif
#ifndef cb_BIT_3
#define cb_BIT_3        (1ul << 3)
#endif
#ifndef cb_BIT_4
#define cb_BIT_4        (1ul << 4)
#endif
#ifndef cb_BIT_5
#define cb_BIT_5        (1ul << 5)
#endif
#ifndef cb_BIT_6
#define cb_BIT_6        (1ul << 6)
#endif
#ifndef cb_BIT_7
#define cb_BIT_7        (1ul << 7)
#endif
#ifndef cb_BIT_8
#define cb_BIT_8        (1ul << 8)
#endif
#ifndef cb_BIT_9
#define cb_BIT_9        (1ul << 9)
#endif
#ifndef cb_BIT_10
#define cb_BIT_10       (1ul << 10)
#endif
#ifndef cb_BIT_11
#define cb_BIT_11       (1ul << 11)
#endif
#ifndef cb_BIT_12
#define cb_BIT_12       (1ul << 12)
#endif
#ifndef cb_BIT_13
#define cb_BIT_13       (1ul << 13)
#endif
#ifndef cb_BIT_14
#define cb_BIT_14       (1ul << 14)
#endif
#ifndef cb_BIT_15
#define cb_BIT_15       (1ul << 15)
#endif
#ifndef cb_BIT_16
#define cb_BIT_16       (1ul << 16)
#endif
#ifndef cb_BIT_17
#define cb_BIT_17       (1ul << 17)
#endif
#ifndef cb_BIT_18
#define cb_BIT_18       (1ul << 18)
#endif
#ifndef cb_BIT_19
#define cb_BIT_19       (1ul << 19)
#endif
#ifndef cb_BIT_20
#define cb_BIT_20       (1ul << 20)
#endif
#ifndef cb_BIT_21
#define cb_BIT_21       (1ul << 21)
#endif
#ifndef cb_BIT_22
#define cb_BIT_22       (1ul << 22)
#endif
#ifndef cb_BIT_23
#define cb_BIT_23       (1ul << 23)
#endif
#ifndef cb_BIT_24
#define cb_BIT_24       (1ul << 24)
#endif
#ifndef cb_BIT_25
#define cb_BIT_25       (1ul << 25)
#endif
#ifndef cb_BIT_26
#define cb_BIT_26       (1ul << 26)
#endif
#ifndef cb_BIT_27
#define cb_BIT_27       (1ul << 27)
#endif
#ifndef cb_BIT_28
#define cb_BIT_28       (1ul << 28)
#endif
#ifndef cb_BIT_29
#define cb_BIT_29       (1ul << 29)
#endif
#ifndef cb_BIT_30
#define cb_BIT_30       (1ul << 30)
#endif
#ifndef cb_BIT_31
#define cb_BIT_31       (1ul << 31)
#endif

#ifndef cb_UINT8_MAX
#define cb_UINT8_MAX    ((cb_uint8)0xff)
#endif
#ifndef cb_UINT16_MAX
#define cb_UINT16_MAX   ((cb_uint16)0xffff)
#endif
#ifndef cb_UINT32_MAX
#define cb_UINT32_MAX   ((cb_uint32)0xffffffff)
#endif
#ifndef cb_INT8_MAX
#define cb_INT8_MAX     ((cb_uint8)0x7f)
#endif
#ifndef cb_INT16_MAX
#define cb_INT16_MAX    ((cb_uint16)0x7fff)
#endif
#ifndef cb_INT32_MAX
#define cb_INT32_MAX    ((cb_uint32)0x7fffffff)
#endif
#ifndef cb_INT8_MIN
#define cb_INT8_MIN     ((cb_uint8)0x80)
#endif
#ifndef cb_INT16_MIN
#define cb_INT16_MIN    ((cb_uint16)0x8000)
#endif
#ifndef cb_INT32_MIN
#define cb_INT32_MIN    ((cb_uint32)0x80000000)
#endif

/**
 * Clears (set to zero) a bit or bits in a variable. 
 * @param variable The variable.
 * @param bit The bit or bits to clear
 */
#ifndef cb_CLEAR_BIT
#  define cb_CLEAR_BIT(variable,bit) ((variable) &= ~((bit)))
#endif

/**
 * Gets a bit i.e. checks if it is set in a variable.
 *
 * Also works to see if any of several bits are set.
 *
 * @param variable The variable.
 * @param bit The bit to check if it set.
 * @return @ref TRUE if any of the bits are set, @ref FALSE otherwise.
 */
#ifndef cb_GET_BIT
#  define cb_GET_BIT(variable,bit) (((variable) & ((bit))) ? TRUE : FALSE)
#endif

/**
 * Sets (set to 1) a bit or bits in a variable.
 *
 * @param variable The variable.
 * @param bit The bit or bits to set in the variable.
 */
#ifndef cb_SET_BIT
#  define cb_SET_BIT(variable,bit) ((variable) |= (bit))
#endif


/*Packed struct defines*/
#ifndef cb_PACKED_STRUCT_ATTR_INLINE_POST
#  define cb_PACKED_STRUCT_ATTR_INLINE_POST
#endif
#ifndef cb_PACKED_STRUCT_ATTR_INLINE_PRE
#  define cb_PACKED_STRUCT_ATTR_INLINE_PRE
#endif
#ifndef cb_PACKED_STRUCT_ATTR_PRE
#  define cb_PACKED_STRUCT_ATTR_PRE
#endif
#ifndef cb_PACKED_STRUCT_ATTR_POST
#  define cb_PACKED_STRUCT_ATTR_POST
#endif

#ifndef cb_PACKED_STRUCT_BEGIN
#  define cb_PACKED_STRUCT_BEGIN(name) \
        cb_PACKED_STRUCT_ATTR_PRE \
        typedef cb_PACKED_STRUCT_ATTR_INLINE_PRE struct name##_t
#endif

#ifndef cb_PACKED_STRUCT_END
#  define cb_PACKED_STRUCT_END(name) \
        cb_PACKED_STRUCT_ATTR_INLINE_POST name; \
        cb_PACKED_STRUCT_ATTR_POST
#endif

#endif /* _CB_TYPES_H_ */
