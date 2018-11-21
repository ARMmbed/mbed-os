/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_platform_api.h"
#include "tfm_platform_veneers.h"
#include "tfm_ns_lock.h"

enum tfm_platform_err_t tfm_platform_system_reset(void)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_platform_veneer_system_reset,
                                0,
                                0,
                                0,
                                0);
}
