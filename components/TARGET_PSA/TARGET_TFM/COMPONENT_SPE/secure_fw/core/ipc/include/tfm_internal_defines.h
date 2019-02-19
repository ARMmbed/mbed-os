/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_INTERNAL_DEFINES_H__
#define __TFM_INTERNAL_DEFINES_H__

/* IPC internal return status */
#define IPC_SUCCESS                     0
#define IPC_ERROR_BAD_PARAMETERS        (INT32_MIN)
#define IPC_ERROR_SHORT_BUFFER          (INT32_MIN + 1)
#define IPC_ERROR_VERSION               (INT32_MIN + 2)
#define IPC_ERROR_MEMORY_CHECK          (INT32_MIN + 3)
#define IPC_ERROR_GENERIC               (INT32_MIN + 0x1F)

#endif
