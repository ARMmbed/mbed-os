/*
 * Copyright (c) 2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PLATFORM_DEFS__
#define __TFM_PLATFORM_DEFS__

#include <stdint.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \enum tfm_pin_service_type_t
 *
 * \brief Pin service types (supported types may vary based on the platform)
 */
enum tfm_pin_service_type_t {
    TFM_PIN_SERVICE_TYPE_SET_ALTFUNC     = 0,   /*!< Set alternate function type */
    TFM_PIN_SERVICE_TYPE_SET_DEFAULT_IN,        /*!< Set default in function type */
    TFM_PIN_SERVICE_TYPE_SET_PIN_MODE,          /*!< Set pin mode function type */
    TFM_PIN_SERVICE_TYPE_MAX = INT_MAX          /*!< Max to force enum max size */
};

/*!
 * \struct tfm_pin_service_args_t
 *
 * \brief Argument list for each platform pin service
 */
struct tfm_pin_service_args_t {
    enum tfm_pin_service_type_t type;
    union {
        struct set_altfunc { /*!< TFM_PIN_SERVICE_TYPE_SET_ALTFUNC */
            uint32_t alt_func;
            uint64_t pin_mask;
        } set_altfunc;
        struct set_default_in { /*!< TFM_PIN_SERVICE_TYPE_SET_DEFAULT_IN */
            uint32_t alt_func;
            uint32_t pin_value;
            bool default_in_value;
        } set_default_in;
        struct set_pin_mode { /*!< TFM_PIN_SERVICE_TYPE_SET_PIN_MODE */
            uint64_t pin_mask;
            uint32_t pin_mode;
        } set_pin_mode;
    } u;
};

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PLATFORM_DEFS__ */
