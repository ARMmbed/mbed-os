/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_nspm_svc_handler.h"
#include "tfm_api.h"
#include "tfm_ns_svc.h"

/* SVC function implementations */
uint32_t tfm_nspm_svc_register_client_id(uint32_t client_id)
{
    if (tfm_register_client_id(client_id) == TFM_SUCCESS) {
        return 1;
    }

    return 0;
}
