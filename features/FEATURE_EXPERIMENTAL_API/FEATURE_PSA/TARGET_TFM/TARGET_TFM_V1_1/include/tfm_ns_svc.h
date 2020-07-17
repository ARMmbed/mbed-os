/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "cmsis_compiler.h"

#ifndef __TFM_NS_SVC_H__
#define __TFM_NS_SVC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Include all the SVC handler headers
 */
#include "tfm_nspm_svc_handler.h"

/**
 * \brief Macro to encode an svc instruction
 *
 */
#define SVC(code) __ASM volatile("svc %0" : : "I" (code))

/**
 * \def LIST_SVC_NSPM
 *
 * \brief This is an X macro which lists
 *        the SVC interface exposed by TF-M
 *        for the NS OS.
 *
 */
#define LIST_SVC_NSPM \
    X(SVC_TFM_NSPM_REGISTER_CLIENT_ID, tfm_nspm_svc_register_client_id) \

/**
 * \brief Numbers associated to each SVC available
 *
 * \details Start from 1 as 0 is reserved by RTX
 */
enum tfm_svc_num {
    SVC_INVALID = 0,

#define X(SVC_ENUM, SVC_HANDLER) SVC_ENUM,

    /* SVC API for Services */
#ifdef TFM_NS_CLIENT_IDENTIFICATION
    LIST_SVC_NSPM
#endif

#undef X

    /* add all the new entries above this line */
    SVC_TFM_MAX,
};

/* number of user SVC functions */
#define USER_SVC_COUNT ((uint32_t)SVC_TFM_MAX - 1)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NS_SVC_H__ */
