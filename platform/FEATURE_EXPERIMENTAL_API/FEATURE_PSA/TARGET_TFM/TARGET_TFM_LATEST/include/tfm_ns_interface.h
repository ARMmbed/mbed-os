/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_NS_INTERFACE_H__
#define __TFM_NS_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "tfm_api.h"

typedef int32_t (*veneer_fn) (uint32_t arg0, uint32_t arg1,
                              uint32_t arg2, uint32_t arg3);

/**
 * \brief NS interface, veneer function dispatcher
 *
 * \details This function implements the dispatching mechanism for the
 *          desired veneer function, to be called with the parameters
 *          described from arg0 to arg3.
 *
 * \param[in] fn   Function pointer to the veneer function desired
 * \param[in] arg0 Argument 0
 * \param[in] arg1 Argument 1
 * \param[in] arg2 Argument 2
 * \param[in] arg3 Argument 3
 *
 * \return Returns the same return value of the requested veneer function
 *
 * \note This API must ensure the return value is from the veneer function.
 *       Other unrecoverable errors must be considered as fatal error and should
 *       not return.
 */
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3);

/**
 * \brief NS interface, Initialise the NS interface
 *
 * \details This function needs to be called from the NS world to
 *          properly initialise the NS interface towards TF-M. This
 *          function will initialise all the objects required for
 *          runtime dispatching of TF-M requests to services
 *
 * \return  A value according to \ref enum tfm_status_e
 */
enum tfm_status_e tfm_ns_interface_init(void);
#ifdef __cplusplus
}
#endif

#endif /* __TFM_NS_INTERFACE_H__ */
