/*
 * Copyright (c) 2017-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* This file contains the APIs exported by the SPM to tfm core */

#include <stdio.h>
#include <string.h>
#include "spm_api.h"
#include "platform/include/tfm_spm_hal.h"
#include "tfm_memory_utils.h"
#include "spm_db_setup.h"
#include "tfm_internal.h"
#include "tfm_api.h"
#include "tfm_nspm.h"
#include "secure_fw/core/tfm_core.h"
#include "tfm_peripherals_def.h"
#include "spm_partition_defs.h"


struct spm_partition_db_t g_spm_partition_db = {0,};

typedef enum {
    TFM_INIT_FAILURE,
} sp_error_type_t;

/*
 * This function is called when a secure partition causes an error.
 * In case of an error in the error handling, a non-zero value have to be
 * returned.
 */
#ifndef TFM_PSA_API
static void tfm_spm_partition_err_handler(
    const struct spm_partition_desc_t *partition,
    sp_error_type_t err_type,
    int32_t err_code)
{
#ifdef TFM_CORE_DEBUG
    if (err_type == TFM_INIT_FAILURE) {
        printf("Partition init failed for partition id 0x%08X\r\n",
                partition->static_data.partition_id);
    } else {
        printf(
            "Unknown partition error %d (code: %d) for partition id 0x%08X\r\n",
            err_type, err_code, partition->static_data.partition_id);
    }
#else
    (void)err_type;
    (void)err_code;
#endif
    tfm_spm_partition_set_state(partition->static_data.partition_id,
            SPM_PARTITION_STATE_CLOSED);
}
#endif /* !defined(TFM_PSA_API) */

/*
 * This function prevents name clashes between the variable names accessibles in
 * the scope of where tfm_partition_list.inc is included and the varaible names
 * defined inside tfm_partition_list.inc file.
 */
static inline enum spm_err_t add_user_defined_partitions(void) {
    #include "tfm_partition_list.inc"

    return SPM_ERR_OK;
}

uint32_t get_partition_idx(uint32_t partition_id)
{
    uint32_t i;

    if (partition_id == INVALID_PARTITION_ID) {
        return SPM_INVALID_PARTITION_IDX;
    }

    for (i = 0; i < g_spm_partition_db.partition_count; ++i) {
        if (g_spm_partition_db.partitions[i].static_data.partition_id ==
                partition_id) {
            return i;
        }
    }
    return SPM_INVALID_PARTITION_IDX;
}

enum spm_err_t tfm_spm_db_init(void)
{
    struct spm_partition_desc_t *part_ptr;
    enum spm_err_t err;

    (void)tfm_memset (&g_spm_partition_db, 0, sizeof(g_spm_partition_db));

    /* This function initialises partition db */
    g_spm_partition_db.running_partition_idx = SPM_INVALID_PARTITION_IDX;
    g_spm_partition_db.partition_count = 0;

    /* There are a few partitions that are used by TF-M internally.
     * These are explicitly added to the partition db here.
     */

    /* For the non secure Execution environment */
#if (TFM_LVL != 1) || defined(TFM_PSA_API)
    extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Base[];
    extern uint32_t Image$$ARM_LIB_STACK$$ZI$$Limit[];
    uint32_t psp_stack_bottom = (uint32_t)Image$$ARM_LIB_STACK$$ZI$$Base;
    uint32_t psp_stack_top    = (uint32_t)Image$$ARM_LIB_STACK$$ZI$$Limit;
#endif
    if (g_spm_partition_db.partition_count >= SPM_MAX_PARTITIONS) {
        return SPM_ERR_INVALID_CONFIG;
    }
    part_ptr = &(g_spm_partition_db.partitions[
            g_spm_partition_db.partition_count]);
    part_ptr->static_data.partition_id = TFM_SP_NON_SECURE_ID;
#ifdef TFM_PSA_API
    part_ptr->static_data.partition_flags = SPM_PART_FLAG_APP_ROT |
                                            SPM_PART_FLAG_IPC;
    part_ptr->static_data.partition_priority = TFM_PRIORITY_LOW;
    part_ptr->static_data.partition_init = tfm_nspm_thread_entry;
#else
    part_ptr->static_data.partition_flags = 0;
#endif

#if (TFM_LVL != 1) || defined(TFM_PSA_API)
    part_ptr->memory_data.stack_bottom = psp_stack_bottom;
    part_ptr->memory_data.stack_top    = psp_stack_top;
    /* Since RW, ZI and stack are configured as one MPU region, configure
     * RW start address to psp_stack_bottom to get RW access to stack
     */
    part_ptr->memory_data.rw_start     = psp_stack_bottom;
#endif

    part_ptr->runtime_data.partition_state = SPM_PARTITION_STATE_UNINIT;
    tfm_nspm_configure_clients();
    ++g_spm_partition_db.partition_count;

    /* For the TF-M core environment itself */
    if (g_spm_partition_db.partition_count >= SPM_MAX_PARTITIONS) {
        return SPM_ERR_INVALID_CONFIG;
    }
    part_ptr = &(g_spm_partition_db.partitions[
            g_spm_partition_db.partition_count]);
    part_ptr->static_data.partition_id = TFM_SP_CORE_ID;
    part_ptr->static_data.partition_flags =
                    SPM_PART_FLAG_APP_ROT | SPM_PART_FLAG_PSA_ROT;
    part_ptr->runtime_data.partition_state = SPM_PARTITION_STATE_UNINIT;
    ++g_spm_partition_db.partition_count;

    err = add_user_defined_partitions();
    if (err != SPM_ERR_OK) {
        return err;
    }

    g_spm_partition_db.is_init = 1;

    return SPM_ERR_OK;
}

#ifndef TFM_PSA_API
enum spm_err_t tfm_spm_partition_init(void)
{
    struct spm_partition_desc_t *part;
    struct tfm_sfn_req_s desc;
    int32_t args[4] = {0};
    int32_t fail_cnt = 0;
    uint32_t idx;

    /* Call the init function for each partition */
    for (idx = 0; idx < g_spm_partition_db.partition_count; ++idx) {
        part = &g_spm_partition_db.partitions[idx];
        tfm_spm_hal_configure_default_isolation(part->platform_data);
        if (part->static_data.partition_init == NULL) {
            tfm_spm_partition_set_state(idx, SPM_PARTITION_STATE_IDLE);
            tfm_spm_partition_set_caller_partition_idx(idx,
                                                     SPM_INVALID_PARTITION_IDX);
        } else {
            int32_t res;

            desc.args = args;
            desc.ns_caller = 0U;
            desc.iovec_api = TFM_SFN_API_IOVEC;
            desc.sfn = (sfn_t)part->static_data.partition_init;
            desc.sp_id = part->static_data.partition_id;
            res = tfm_core_sfn_request(&desc);
            if (res == TFM_SUCCESS) {
                tfm_spm_partition_set_state(idx, SPM_PARTITION_STATE_IDLE);
            } else {
                tfm_spm_partition_err_handler(part, TFM_INIT_FAILURE, res);
                fail_cnt++;
            }
        }
    }

    tfm_secure_api_init_done();

    if (fail_cnt == 0) {
        return SPM_ERR_OK;
    } else {
        return SPM_ERR_PARTITION_NOT_AVAILABLE;
    }
}
#endif /* !defined(TFM_PSA_API) */

#if (TFM_LVL != 1) || defined(TFM_PSA_API)
uint32_t tfm_spm_partition_get_stack_bottom(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].
            memory_data.stack_bottom;
}

uint32_t tfm_spm_partition_get_stack_top(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].memory_data.stack_top;
}
#endif

#if (TFM_LVL != 1) && !defined(TFM_PSA_API)
enum spm_err_t tfm_spm_partition_sandbox_config(uint32_t partition_idx)
{
    struct spm_partition_desc_t *part;
    if (!g_spm_partition_db.is_init) {
        return SPM_ERR_PARTITION_DB_NOT_INIT;
    }

    part = &g_spm_partition_db.partitions[partition_idx];

    return tfm_spm_hal_partition_sandbox_config(&(part->memory_data),
                                                part->platform_data);

}

enum spm_err_t tfm_spm_partition_sandbox_deconfig(uint32_t partition_idx)
{
    /* This function takes a partition id and disables the
     * SPM partition for that partition
     */

    struct spm_partition_desc_t *part;

    part = &g_spm_partition_db.partitions[partition_idx];

    return tfm_spm_hal_partition_sandbox_deconfig(&(part->memory_data),
                                                  part->platform_data);
}

uint32_t tfm_spm_partition_get_zi_start(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].
            memory_data.zi_start;
}

uint32_t tfm_spm_partition_get_zi_limit(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].
            memory_data.zi_limit;
}

uint32_t tfm_spm_partition_get_rw_start(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].
            memory_data.rw_start;
}

uint32_t tfm_spm_partition_get_rw_limit(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].
            memory_data.rw_limit;
}

void tfm_spm_partition_set_stack(uint32_t partition_idx, uint32_t stack_ptr)
{
    g_spm_partition_db.partitions[partition_idx].
            runtime_data.stack_ptr = stack_ptr;
}
#endif

uint32_t tfm_spm_partition_get_partition_id(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].static_data.
            partition_id;
}

uint32_t tfm_spm_partition_get_flags(uint32_t partition_idx)
{
    return g_spm_partition_db.partitions[partition_idx].static_data.
            partition_flags;
}

#ifndef TFM_PSA_API
void tfm_spm_partition_store_context(uint32_t partition_idx,
        uint32_t stack_ptr, uint32_t lr)
{
    g_spm_partition_db.partitions[partition_idx].
            runtime_data.stack_ptr = stack_ptr;
    g_spm_partition_db.partitions[partition_idx].
            runtime_data.lr = lr;
}

const struct spm_partition_runtime_data_t *
            tfm_spm_partition_get_runtime_data(uint32_t partition_idx)
{
    return &(g_spm_partition_db.partitions[partition_idx].runtime_data);
}

void tfm_spm_partition_set_state(uint32_t partition_idx, uint32_t state)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.partition_state =
            state;
    if (state == SPM_PARTITION_STATE_RUNNING) {
        g_spm_partition_db.running_partition_idx = partition_idx;
    }
}

void tfm_spm_partition_set_caller_partition_idx(uint32_t partition_idx,
                                                uint32_t caller_partition_idx)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            caller_partition_idx = caller_partition_idx;
}

void tfm_spm_partition_set_caller_client_id(uint32_t partition_idx,
                                            int32_t caller_client_id)
{
    g_spm_partition_db.partitions[partition_idx].runtime_data.
            caller_client_id = caller_client_id;
}

enum spm_err_t tfm_spm_partition_set_share(uint32_t partition_idx,
                                           uint32_t share)
{
    enum spm_err_t ret = SPM_ERR_OK;

#if TFM_LVL != 1
    /* Only need to set configuration on levels higher than 1 */
    ret = tfm_spm_hal_set_share_region(share);
#endif

    if (ret == SPM_ERR_OK) {
        g_spm_partition_db.partitions[partition_idx].runtime_data.share = share;
    }
    return ret;
}

enum spm_err_t tfm_spm_partition_set_iovec(uint32_t partition_idx,
                                           const int32_t *args)
{
    struct spm_partition_runtime_data_t *runtime_data =
            &g_spm_partition_db.partitions[partition_idx].runtime_data;
    size_t i;

    if ((args[1] < 0) || (args[3] < 0)) {
        return SPM_ERR_INVALID_PARAMETER;
    }

    runtime_data->iovec_args.in_len = (size_t)args[1];
    for (i = 0U; i < runtime_data->iovec_args.in_len; ++i) {
        runtime_data->iovec_args.in_vec[i].base =
                                                 ((psa_invec *)args[0])[i].base;
        runtime_data->iovec_args.in_vec[i].len = ((psa_invec *)args[0])[i].len;
    }
    runtime_data->iovec_args.out_len = (size_t)args[3];
    for (i = 0U; i < runtime_data->iovec_args.out_len; ++i) {
        runtime_data->iovec_args.out_vec[i].base =
                                                ((psa_outvec *)args[2])[i].base;
        runtime_data->iovec_args.out_vec[i].len =
                                                 ((psa_outvec *)args[2])[i].len;
    }
    runtime_data->orig_outvec = (psa_outvec *)args[2];
    runtime_data->iovec_api = 1;

    return SPM_ERR_OK;
}

uint32_t tfm_spm_partition_get_running_partition_idx(void)
{
    return g_spm_partition_db.running_partition_idx;
}

void tfm_spm_partition_cleanup_context(uint32_t partition_idx)
{
    struct spm_partition_desc_t *partition =
            &(g_spm_partition_db.partitions[partition_idx]);
    int32_t i;

    partition->runtime_data.caller_partition_idx = SPM_INVALID_PARTITION_IDX;
    partition->runtime_data.share = 0;
    partition->runtime_data.iovec_args.in_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        partition->runtime_data.iovec_args.in_vec[i].base = 0;
        partition->runtime_data.iovec_args.in_vec[i].len = 0;
    }
    partition->runtime_data.iovec_args.out_len = 0;
    for (i = 0; i < PSA_MAX_IOVEC; ++i) {
        partition->runtime_data.iovec_args.out_vec[i].base = 0;
        partition->runtime_data.iovec_args.out_vec[i].len = 0;
    }
    partition->runtime_data.orig_outvec = 0;
    partition->runtime_data.iovec_api = 0;
}
#endif /* !defined(TFM_PSA_API) */

__attribute__((section("SFN")))
void tfm_spm_partition_change_privilege(uint32_t privileged)
{
    CONTROL_Type ctrl;

    ctrl.w = __get_CONTROL();

    if (privileged == TFM_PARTITION_PRIVILEGED_MODE) {
        ctrl.b.nPRIV = 0;
    } else {
        ctrl.b.nPRIV = 1;
    }

    __set_CONTROL(ctrl.w);
}
