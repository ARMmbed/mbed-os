/*
 * Copyright (c) 2017-2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_SECURE_API_H__
#define __TFM_SECURE_API_H__

#include "arm_cmse.h"
#include "tfm_svc.h"
#include "secure_utilities.h"
#include "tfm_core.h"
#include "tfm_api.h"

/*!
 * \def __tfm_secure_gateway_attributes__
 *
 * \brief Attributes for secure gateway functions
 */
#define __tfm_secure_gateway_attributes__ \
        __attribute__((cmse_nonsecure_entry, noinline, section("SFN")))

/* Hide specific errors if not debugging */
#ifdef TFM_CORE_DEBUG
#define TFM_ERROR_STATUS(status) (status)
#else
#define TFM_ERROR_STATUS(status) (TFM_PARTITION_BUSY)
#endif

#ifndef TFM_LVL
#error TFM_LVL is not defined!
#endif

extern void tfm_secure_api_error_handler(void);

typedef int32_t(*sfn_t)(int32_t, int32_t, int32_t, int32_t);

struct tfm_sfn_req_s {
    uint32_t sp_id;
    sfn_t sfn;
    int32_t *args;
    uint32_t caller_part_idx;
    int32_t ns_caller : 1;
};

enum tfm_buffer_share_region_e {
    TFM_BUFFER_SHARE_DISABLE,
    TFM_BUFFER_SHARE_NS_CODE,
    TFM_BUFFER_SHARE_SCRATCH,
    TFM_BUFFER_SHARE_PRIV, /* only for TCB in level 2, all in level 1 */
    TFM_BUFFER_SHARE_DEFAULT,
};

enum tfm_ns_region_e {
    TFM_NS_REGION_CODE = 0,
    TFM_NS_REGION_DATA,
    TFM_NS_REGION_VENEER,
    TFM_NS_REGION_PERIPH_1,
    TFM_NS_REGION_PERIPH_2,
    TFM_NS_SECONDARY_IMAGE_REGION,
};

enum tfm_memory_access_e {
    TFM_MEMORY_ACCESS_RO = 1,
    TFM_MEMORY_ACCESS_RW = 2,
};

extern int32_t tfm_core_set_buffer_area(enum tfm_buffer_share_region_e share);

extern int32_t tfm_core_validate_secure_caller(void);

extern int32_t tfm_core_get_caller_client_id(int32_t *caller_client_id);

extern int32_t tfm_core_memory_permission_check(
        void *ptr, uint32_t size, int32_t access);

extern int32_t tfm_core_get_boot_data(uint8_t major_type, void *ptr,
                                      uint32_t len);

int32_t tfm_core_sfn_request(struct tfm_sfn_req_s *desc_ptr);

int32_t tfm_core_sfn_request_thread_mode(struct tfm_sfn_req_s *desc_ptr);

#define TFM_CORE_SFN_REQUEST(id, fn, a, b, c, d) \
        return tfm_core_partition_request(id, fn, (int32_t)a, (int32_t)b, \
            (int32_t)c, (int32_t)d)

__attribute__ ((always_inline)) __STATIC_INLINE
int32_t tfm_core_partition_request(uint32_t id, void *fn,
            int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4)
{
    int32_t args[4] = {arg1, arg2, arg3, arg4};
    struct tfm_sfn_req_s desc, *desc_ptr = &desc;

    desc.sp_id = id;
    desc.sfn = fn;
    desc.args = args;
    desc.ns_caller = cmse_nonsecure_caller();
    if (__get_active_exc_num() != EXC_NUM_THREAD_MODE) {
        /* FixMe: Error severity TBD */
        return TFM_ERROR_GENERIC;
    } else {
#if TFM_LVL == 1
        if (desc.ns_caller) {
            return tfm_core_sfn_request(desc_ptr);
        } else {
            return tfm_core_sfn_request_thread_mode(desc_ptr);
        }
#else
        return tfm_core_sfn_request(desc_ptr);
#endif

    }
}

#endif /* __TFM_SECURE_API_H__ */
