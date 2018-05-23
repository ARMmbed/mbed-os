/**
 * @file xmc_common.h
 * @date 2017-08-03
 *
 * @cond
  *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-06-20:
 *     - Initial
 *     - Brief section updated
 *     - Added XMC_LIB_VERSION macro
 *
 * 2016-02-26:
 *     - Updated XMC_LIB_VERSION macro to v2.1.6
 *      
 * 2016-05-30:
 *     - Updated XMC_LIB_VERSION macro to v2.1.8
 *
 * 2016-11-18:
 *     - Updated XMC_LIB_VERSION macro to v2.1.10
 *     - Changed type of size in XMC_PRIOARRAY_t to fix compilation warnings
 *
 * 2017-04-04:
 *     - Updated XMC_LIB_VERSION macro to v2.1.12
 *
 * 2017-06-24:
 *     - Updated XMC_LIB_VERSION macro to v2.1.14
 *
 * 2017-08-03:
 *     - Updated XMC_LIB_VERSION macro to v2.1.16
 *     - Added __RAM_FUNC macro
 *
 * @endcond 
 *
 */

#ifndef XMC_COMMON_H
#define XMC_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "xmc_device.h"

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup COMMON
 * @brief Common APIs to all peripherals for XMC microcontroller family 
 * @{
 */

/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
#define XMC_LIB_MAJOR_VERSION	(2U)
#define XMC_LIB_MINOR_VERSION	(1U)
#define XMC_LIB_PATCH_VERSION	(16U)
 
#define XMC_LIB_VERSION         ((XMC_LIB_MAJOR_VERSION << 16U) + (XMC_LIB_MINOR_VERSION << 8U) + XMC_LIB_PATCH_VERSION)

/* Define WEAK attribute */
#if !defined(__WEAK)
#if defined ( __CC_ARM )
#define __WEAK __attribute__ ((weak))
#elif defined ( __ICCARM__ )
#define __WEAK __weak
#elif defined ( __GNUC__ )
#define __WEAK __attribute__ ((weak))
#elif defined ( __TASKING__ )
#define __WEAK __attribute__ ((weak))
#endif
#endif

#if !defined(__RAM_FUNC)
#if defined ( __CC_ARM )
// http://www.keil.com/support/docs/3723.htm
#define __RAM_FUNC __attribute__((section("RAMCODESECTION")))
#elif defined ( __ICCARM__ )
// https://www.iar.com/support/tech-notes/linker/controlling-placement-of-the-section-where-__ramfunc-functions-reside-ewarm-5.x--6.x/
#define __RAM_FUNC __ramfunc
#elif defined ( __GNUC__ )
#define __RAM_FUNC __attribute__((section(".ram_code"), long_call))
#elif defined ( __TASKING__ )
#define __RAM_FUNC __attribute__((section(".ram_code")))
#endif
#endif

#ifdef XMC_ASSERT_ENABLE
 #define XMC_ASSERT(msg, exp) { if(!(exp)) {XMC_AssertHandler(msg, __FILE__, __LINE__);} }
#else
 #define XMC_ASSERT(msg, exp) { ; }
#endif

#ifdef XMC_DEBUG_ENABLE
 #include <stdio.h>
 #define XMC_DEBUG(...) { printf(__VA_ARGS__); }
#else
 #define XMC_DEBUG(...) { ; }
#endif

#define XMC_UNUSED_ARG(x) (void)x

#define XMC_STRUCT_INIT(m) memset(&m, 0, sizeof(m))

#define XMC_PRIOARRAY_DEF(name, size) \
XMC_PRIOARRAY_ITEM_t prioarray_m_##name[size + 2]; \
XMC_PRIOARRAY_t prioarray_def_##name = {(size), (prioarray_m_##name)};

#define XMC_PRIOARRAY(name) \
&prioarray_def_##name

/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/
/*
 *
 */
typedef struct XMC_DRIVER_VERSION 
{
  uint8_t major;
  uint8_t minor;
  uint8_t patch;
} XMC_DRIVER_VERSION_t;

/*
 *
 */
typedef void *XMC_LIST_t;

/*
 *
 */
typedef struct XMC_PRIOARRAY_ITEM
{
  int32_t priority;
  int32_t previous;
  int32_t next;
} XMC_PRIOARRAY_ITEM_t;

/*
 *
 */
typedef struct XMC_PRIOARRAY
{
  int32_t size;
  XMC_PRIOARRAY_ITEM_t *items;
} XMC_PRIOARRAY_t;

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 */
void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line);

/*
 *
 */
void XMC_LIST_Init(XMC_LIST_t *list);

/*
 *
 */
void XMC_LIST_Add(XMC_LIST_t *list, void *const item);

/*
 *
 */
void XMC_LIST_Remove(XMC_LIST_t *list, void *const item);

/*
 *
 */
uint32_t XMC_LIST_GetLength(XMC_LIST_t *list);

/*
 *
 */
void *XMC_LIST_GetHead(XMC_LIST_t *list);

/*
 *
 */
void *XMC_LIST_GetTail(XMC_LIST_t *list);

/*
 *
 */
void XMC_LIST_Insert(XMC_LIST_t *list, void *prev_item, void *new_item);

/*
 *
 */
void XMC_PRIOARRAY_Init(XMC_PRIOARRAY_t *prioarray);

/*
 *
 */
void XMC_PRIOARRAY_Add(XMC_PRIOARRAY_t *prioarray, int32_t item, int32_t priority);

/*
 *
 */
void XMC_PRIOARRAY_Remove(XMC_PRIOARRAY_t *prioarray, int32_t item);

/*
 *
 */
__STATIC_INLINE int32_t XMC_PRIOARRAY_GetHead(XMC_PRIOARRAY_t *prioarray)
{
  XMC_ASSERT("XMC_PRIOARRAY_Init: NULL pointer", prioarray != NULL);
  return prioarray->items[prioarray->size].next;
}

/*
 *
 */
__STATIC_INLINE int32_t XMC_PRIOARRAY_GetTail(XMC_PRIOARRAY_t *prioarray)
{
  XMC_ASSERT("XMC_PRIOARRAY_Init: NULL pointer", prioarray != NULL);
  return prioarray->items[prioarray->size + 1].previous;
}

/*
 *
 */
__STATIC_INLINE int32_t XMC_PRIOARRAY_GetItemPriority(XMC_PRIOARRAY_t *prioarray, int32_t item)
{
  XMC_ASSERT("XMC_PRIOARRAY_GetItemPriority: item out of range", (item >= 0) && (item < prioarray->size));
  return prioarray->items[item].priority;
}

/*
 *
 */
__STATIC_INLINE int32_t XMC_PRIOARRAY_GetItemNext(XMC_PRIOARRAY_t *prioarray, int32_t item)
{
  XMC_ASSERT("XMC_PRIOARRAY_GetItemNext: item out of range", (item >= 0) && (item < prioarray->size));
  return prioarray->items[item].next;
}

/*
 *
 */
__STATIC_INLINE int32_t XMC_PRIOARRAY_GetItemPrevious(XMC_PRIOARRAY_t *prioarray, int32_t item)
{
  XMC_ASSERT("XMC_PRIOARRAY_GetItemPrevious: item out of range", (item >= 0) && (item < prioarray->size));
  return prioarray->items[item].previous;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* XMC_COMMON_H */
