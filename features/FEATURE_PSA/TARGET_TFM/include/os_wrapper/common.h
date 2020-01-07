/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __OS_WRAPPER_COMMON_H__
#define __OS_WRAPPER_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define OS_WRAPPER_SUCCESS            (0x0)
#define OS_WRAPPER_ERROR              (0xFFFFFFFFU)
#define OS_WRAPPER_WAIT_FOREVER       (0xFFFFFFFFU)
#define OS_WRAPPER_DEFAULT_STACK_SIZE (-1)

#ifdef __cplusplus
}
#endif

#endif /* __OS_WRAPPER_COMMON_H__ */
