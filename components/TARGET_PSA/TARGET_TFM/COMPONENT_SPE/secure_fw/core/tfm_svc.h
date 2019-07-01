/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SVC_H__
#define __TFM_SVC_H__

#include "cmsis.h"

typedef enum {
    TFM_SVC_SFN_REQUEST = 0,
    TFM_SVC_SFN_RETURN,
    TFM_SVC_VALIDATE_SECURE_CALLER,
    TFM_SVC_GET_CALLER_CLIENT_ID,
    TFM_SVC_MEMORY_CHECK,
    TFM_SVC_SET_SHARE_AREA,
    TFM_SVC_SPM_REQUEST,
    TFM_SVC_PRINT,
    TFM_SVC_GET_BOOT_DATA,
#ifdef TFM_PSA_API
    TFM_SVC_IPC_REQUEST,
    TFM_SVC_SCHEDULE,
    TFM_SVC_EXIT_THRD,
    /* PSA Client SVC */
    TFM_SVC_PSA_FRAMEWORK_VERSION,
    TFM_SVC_PSA_VERSION,
    TFM_SVC_PSA_CONNECT,
    TFM_SVC_PSA_CALL,
    TFM_SVC_PSA_CLOSE,
    /* PSA Service SVC */
    TFM_SVC_PSA_WAIT,
    TFM_SVC_PSA_GET,
    TFM_SVC_PSA_SET_RHANDLE,
    TFM_SVC_PSA_READ,
    TFM_SVC_PSA_SKIP,
    TFM_SVC_PSA_WRITE,
    TFM_SVC_PSA_REPLY,
    TFM_SVC_PSA_NOTIFY,
    TFM_SVC_PSA_CLEAR,
    TFM_SVC_PSA_EOI,
#endif
} tfm_svc_number_t;

#define SVC(code) __ASM volatile("svc %0" : : "I" (code))

#endif /* __TFM_SVC_H__ */
