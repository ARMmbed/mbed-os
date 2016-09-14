/******************************************************************************
 * @brief    This should be #included by every C source file for the target.
 *           This file should be changed only infrequently and with great care.
 *           This file should contain the minimum necessary to get the job done.
 * Copyright (c) 2015 NEUL LIMITED
******************************************************************************/


#ifndef __TYPES_H
#define __TYPES_H

#ifndef NEULFW_H
#error "Please include types.h via neulfw.h"
#endif

/* Get bool, true and false.
 * If using GCC then get stdbool, otherwise create definitions here,
 */
#if defined(__GNUC__) || defined(C99)
#include <stdbool.h>
#elif defined(__cplusplus)
/* bool is already defined in C++ */
#else
#define true 1
#define false 0
typedef int bool;
#endif

typedef signed char             int8;
typedef short                   int16;
typedef long                    int32;
typedef long long               int64;

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;
typedef unsigned long long      uint64;

typedef uint8                   byte;

#define FALSE false
#define TRUE  true

#define MAX_UINT8  0xFF
#define MAX_UINT16 0xFFFF
#define MAX_UINT32 0xFFFFFFFF

/** Integer type big enough to hold a data pointer. */
typedef unsigned int            uintptr;

typedef void(*void_func_void_t)(void);

typedef void (*CONST_VOID_PTR__UINT16__ACTION)(const void *, uint16);

#define UNUSED(var) do { (void)var; } while(0)

#endif
