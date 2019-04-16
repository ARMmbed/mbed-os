/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdbool.h>
#include "tfm_platform_api.h"
#include "tfm_platform_veneers.h"
#include "tfm_ns_lock.h"
#include "tfm_platform_defs.h"
#include "tfm_veneers.h"

enum tfm_platform_err_t tfm_platform_system_reset(void)
{
    return tfm_ns_lock_dispatch((veneer_fn)tfm_platform_veneer_system_reset,
                                0,
                                0,
                                0,
                                0);
}

enum tfm_platform_err_t
tfm_platform_set_pin_alt_func(uint32_t alt_func, uint64_t pin_mask,
                              uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_pin_service_args_t args;

    if (result == NULL) {
        return TFM_PLATFORM_ERR_INVALID_PARAM;
    }

    args.type = TFM_PIN_SERVICE_TYPE_SET_ALTFUNC;
    args.u.set_altfunc.alt_func = alt_func;
    args.u.set_altfunc.pin_mask = pin_mask;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)result;
    out_vec.len = sizeof(*result);

    ret = tfm_ns_lock_dispatch((veneer_fn)tfm_platform_sp_pin_service_veneer,
                                (uint32_t)&in_vec,  1,
                                (uint32_t)&out_vec, 1);

    return ret;
}

enum tfm_platform_err_t
tfm_platform_set_pin_default_in(uint32_t alt_func, uint32_t pin_value,
                              bool default_in_value, uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_pin_service_args_t args;

    if (result == NULL) {
        return TFM_PLATFORM_ERR_INVALID_PARAM;
    }

    args.type = TFM_PIN_SERVICE_TYPE_SET_DEFAULT_IN;
    args.u.set_default_in.alt_func = alt_func;
    args.u.set_default_in.pin_value = pin_value;
    args.u.set_default_in.default_in_value = default_in_value;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)result;
    out_vec.len = sizeof(*result);

    ret = tfm_ns_lock_dispatch((veneer_fn)tfm_platform_sp_pin_service_veneer,
                                (uint32_t)&in_vec,  1,
                                (uint32_t)&out_vec, 1);

    return ret;
}

enum tfm_platform_err_t
tfm_platform_set_pin_mode(uint64_t pin_mask, uint32_t pin_mode,
                          uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_pin_service_args_t args;

    if (result == NULL) {
        return TFM_PLATFORM_ERR_INVALID_PARAM;
    }

    args.type = TFM_PIN_SERVICE_TYPE_SET_PIN_MODE;
    args.u.set_pin_mode.pin_mask = pin_mask;
    args.u.set_pin_mode.pin_mode = pin_mode;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)result;
    out_vec.len = sizeof(*result);

    ret = tfm_ns_lock_dispatch((veneer_fn)tfm_platform_sp_pin_service_veneer,
                                (uint32_t)&in_vec,  1,
                                (uint32_t)&out_vec, 1);

    return ret;
}

