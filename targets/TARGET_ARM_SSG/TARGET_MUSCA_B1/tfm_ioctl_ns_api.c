/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "tfm_platform_api.h"
#include "tfm_ioctl_api.h"

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

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_PIN_SERVICE,
                             &in_vec,
                             &out_vec);

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

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_PIN_SERVICE,
                             &in_vec,
                             &out_vec);

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

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_PIN_SERVICE,
                             &in_vec,
                             &out_vec);

    return ret;
}

enum tfm_platform_err_t tfm_platform_gpio_init(uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_gpio_service_args_t args;
    struct tfm_gpio_service_out_t out;

    args.type = TFM_GPIO_SERVICE_TYPE_INIT;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)&out;
    out_vec.len = sizeof(out);

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_GPIO_SERVICE,
                             &in_vec,
                             &out_vec);

    *result = out.u.result;

    return ret;
}

enum tfm_platform_err_t
tfm_platform_gpio_pin_config(uint32_t pin_num, uint32_t direction,
                             uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_gpio_service_args_t args;
    struct tfm_gpio_service_out_t out;

    args.type = TFM_GPIO_SERVICE_TYPE_PIN_CONFIG;
    args.u.gpio_config.pin_num_or_mask = pin_num;
    args.u.gpio_config.direction = direction;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)&out;
    out_vec.len = sizeof(out);

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_GPIO_SERVICE,
                             &in_vec,
                             &out_vec);

    *result = out.u.result;

    return ret;
}

enum tfm_platform_err_t
tfm_platform_gpio_pin_write(uint32_t pin_num, uint32_t value, uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_gpio_service_args_t args;
    struct tfm_gpio_service_out_t out;

    args.type = TFM_GPIO_SERVICE_TYPE_PIN_WRITE;
    args.u.gpio_write.pin_num_or_mask = pin_num;
    args.u.gpio_write.value = value;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)&out;
    out_vec.len = sizeof(out);

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_GPIO_SERVICE,
                             &in_vec,
                             &out_vec);

    *result = out.u.result;

    return ret;

}

enum tfm_platform_err_t
tfm_platform_gpio_pin_read(uint32_t pin_num, uint32_t *data, uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_gpio_service_args_t args;
    struct tfm_gpio_service_out_t out;

    args.type = TFM_GPIO_SERVICE_TYPE_PIN_READ;
    args.u.gpio_read.pin_num_or_mask = pin_num;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)&out;
    out_vec.len = sizeof(out);

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_GPIO_SERVICE,
                             &in_vec,
                             &out_vec);

    *result = out.u.gpio_read_result.result;
    *data = out.u.gpio_read_result.data;
    return ret;
}

enum tfm_platform_err_t
tfm_platform_gpio_port_config(uint32_t pin_mask, uint32_t direction,
                             uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_gpio_service_args_t args;
    struct tfm_gpio_service_out_t out;

    args.type = TFM_GPIO_SERVICE_TYPE_PORT_CONFIG;
    args.u.gpio_config.pin_num_or_mask = pin_mask;
    args.u.gpio_config.direction = direction;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)&out;
    out_vec.len = sizeof(out);

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_GPIO_SERVICE,
                             &in_vec,
                             &out_vec);

    *result = out.u.result;

    return ret;
}

enum tfm_platform_err_t
tfm_platform_gpio_port_write(uint32_t pin_mask, uint32_t value,
                             uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_gpio_service_args_t args;
    struct tfm_gpio_service_out_t out;

    args.type = TFM_GPIO_SERVICE_TYPE_PORT_WRITE;
    args.u.gpio_write.pin_num_or_mask = pin_mask;
    args.u.gpio_write.value = value;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)&out;
    out_vec.len = sizeof(out);

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_GPIO_SERVICE,
                             &in_vec,
                             &out_vec);

    *result = out.u.result;

    return ret;

}

enum tfm_platform_err_t
tfm_platform_gpio_port_read(uint32_t pin_mask, uint32_t *data, uint32_t *result)
{
    enum tfm_platform_err_t ret;
    psa_invec in_vec;
    psa_outvec out_vec;
    struct tfm_gpio_service_args_t args;
    struct tfm_gpio_service_out_t out;

    args.type = TFM_GPIO_SERVICE_TYPE_PORT_READ;
    args.u.gpio_read.pin_num_or_mask = pin_mask;

    in_vec.base = (const void *)&args;
    in_vec.len = sizeof(args);

    out_vec.base = (void *)&out;
    out_vec.len = sizeof(out);

    ret = tfm_platform_ioctl(TFM_PLATFORM_IOCTL_GPIO_SERVICE,
                             &in_vec,
                             &out_vec);

    *result = out.u.gpio_read_result.result;
    *data = out.u.gpio_read_result.data;
    return ret;
}

