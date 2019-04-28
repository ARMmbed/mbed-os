/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_NS_LOCK_H__
#define __TFM_NS_LOCK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tfm_api.h"

typedef int32_t (*veneer_fn) (uint32_t arg0, uint32_t arg1,
                      uint32_t arg2, uint32_t arg3);

/**
 * \brief NS world, NS lock based dispatcher
 *
 * \details To be called from the wrapper API interface
 */

uint32_t tfm_ns_lock_dispatch(veneer_fn fn,
                              uint32_t arg0, uint32_t arg1,
                              uint32_t arg2, uint32_t arg3);

/**
 * \brief NS world, Init NS lock
 *
 * \details Needs to be called during non-secure app init
 *          to initialize the TFM NS lock object
 */
enum tfm_status_e tfm_ns_lock_init();

#ifdef __cplusplus
}
#endif

#endif /* __TFM_NS_LOCK_H__ */
