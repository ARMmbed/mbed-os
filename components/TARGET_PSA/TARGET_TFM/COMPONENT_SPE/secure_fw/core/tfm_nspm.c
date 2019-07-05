/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include "secure_utilities.h"
#include "tfm_api.h"
#ifdef TFM_PSA_API
#include "tfm_utils.h"
#include "tfm_internal.h"
#endif

#ifndef TFM_MAX_NS_THREAD_COUNT
#define TFM_MAX_NS_THREAD_COUNT 8
#endif
#define INVALID_CLIENT_ID 0

#define DEFAULT_NS_CLIENT_ID ((int32_t)-1)

#define INVALID_NS_CLIENT_IDX (-1)
#define DEFAULT_NS_CLIENT_IDX   0

typedef uint32_t TZ_ModuleId_t;
typedef uint32_t TZ_MemoryId_t;

static struct ns_client_list_t {
    int32_t ns_client_id;
    int32_t next_free_index;
} NsClientIdList[TFM_MAX_NS_THREAD_COUNT];

static int32_t free_index = 0U;
static int32_t active_ns_client_idx = INVALID_NS_CLIENT_IDX;

static int get_next_ns_client_id()
{
#ifdef TFM_NS_CLIENT_IDENTIFICATION
    static int32_t next_ns_client_id = DEFAULT_NS_CLIENT_ID;

    if (next_ns_client_id > 0)
    {
        next_ns_client_id = DEFAULT_NS_CLIENT_ID;
    }
    return next_ns_client_id--;
#else
    return DEFAULT_NS_CLIENT_ID;
#endif
}

void tfm_nspm_configure_clients(void)
{
    int32_t i;

    /* Default to one NS client */
    free_index = 1;
    NsClientIdList[0].ns_client_id = get_next_ns_client_id();
    for (i = 1; i < TFM_MAX_NS_THREAD_COUNT; ++i) {
        NsClientIdList[i].ns_client_id = INVALID_CLIENT_ID;
    }
    active_ns_client_idx = DEFAULT_NS_CLIENT_IDX;
}

int32_t tfm_nspm_get_current_client_id()
{
    if (active_ns_client_idx == INVALID_NS_CLIENT_IDX)
    {
        return 0;
    } else {
        return NsClientIdList[active_ns_client_idx].ns_client_id;
    }
}

/* TF-M implementation of the CMSIS TZ RTOS thread context management API */

/// Initialize secure context memory system
/// \return execution status (1: success, 0: error)
/* This veneer is TF-M internal, not a secure service */
__attribute__((cmse_nonsecure_entry))
uint32_t TZ_InitContextSystem_S(void)
{
    int32_t i;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return 0U;
    }

    /* NS RTOS supports TZ context management, override defaults */
#ifdef PRINT_NSPM_DEBUG
    LOG_MSG("NS RTOS initialized TZ RTOS context management");
#endif /* PRINT_NSPM_DEBUG */
    for (i = 1; i < TFM_MAX_NS_THREAD_COUNT; ++i) {
        NsClientIdList[i].ns_client_id = INVALID_CLIENT_ID;
        NsClientIdList[i].next_free_index = i + 1;
    }

    /* Terminate list */
    NsClientIdList[i - 1].next_free_index = INVALID_NS_CLIENT_IDX;
    /* Success */
    return 1U;
}


/// Allocate context memory for calling secure software modules in TrustZone
/// \param[in]  module   identifies software modules called from non-secure mode
/// \return value != 0 id TrustZone memory slot identifier
/// \return value 0    no memory available or internal error
/* This veneer is TF-M internal, not a secure service */
__attribute__((cmse_nonsecure_entry))
TZ_MemoryId_t TZ_AllocModuleContext_S (TZ_ModuleId_t module)
{
    TZ_MemoryId_t tz_id;
    (void) module; /* Currently unused */

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return 0U;
    }

    if (free_index < 0) {
        /* No more free slots */
        return 0U;
    }

    /* TZ_MemoryId_t must be a positive integer */
    tz_id = (TZ_MemoryId_t)free_index + 1;
    NsClientIdList[free_index].ns_client_id = get_next_ns_client_id();
#ifdef PRINT_NSPM_DEBUG
    printf("TZ_AllocModuleContext_S called, returning id %d\r\n",
        NsClientIdList[free_index].ns_client_id);
#endif /* PRINT_NSPM_DEBUG */
    free_index = NsClientIdList[free_index].next_free_index;

    return tz_id;
}


/// Free context memory that was previously allocated with \ref TZ_AllocModuleContext_S
/// \param[in]  id  TrustZone memory slot identifier
/// \return execution status (1: success, 0: error)
/* This veneer is TF-M internal, not a secure service */
__attribute__((cmse_nonsecure_entry))
uint32_t TZ_FreeModuleContext_S (TZ_MemoryId_t id)
{
    uint32_t index;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return 0U;
    }

    if ((id == 0U) || (id > TFM_MAX_NS_THREAD_COUNT)) {
        /* Invalid TZ_MemoryId_t */
        return 0U;
    }

    index = id - 1;

    if (NsClientIdList[index].ns_client_id == INVALID_CLIENT_ID) {
        /* Non-existent client */
        return 0U;
    }

#ifdef PRINT_NSPM_DEBUG
    printf("TZ_FreeModuleContext_S called for id %d\r\n",
        NsClientIdList[index].ns_client_id);
#endif /* PRINT_NSPM_DEBUG */
    if (active_ns_client_idx == index) {
#ifdef PRINT_NSPM_DEBUG
        printf("Freeing active NS client, NS inactive\r\n");
#endif /* PRINT_NSPM_DEBUG */
        active_ns_client_idx = DEFAULT_NS_CLIENT_IDX;
    }
    NsClientIdList[index].ns_client_id = INVALID_CLIENT_ID;
    NsClientIdList[index].next_free_index = free_index;

    free_index = index;

    return 1U;    // Success
}


/// Load secure context (called on RTOS thread context switch)
/// \param[in]  id  TrustZone memory slot identifier
/// \return execution status (1: success, 0: error)
/* This veneer is TF-M internal, not a secure service */
__attribute__((cmse_nonsecure_entry))
uint32_t TZ_LoadContext_S (TZ_MemoryId_t id)
{
    uint32_t index;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return 0U;
    }

#ifdef PRINT_NSPM_DEBUG
    LOG_MSG("TZ_LoadContext_S called");
#endif /* PRINT_NSPM_DEBUG */
    if ((id == 0U) || (id > TFM_MAX_NS_THREAD_COUNT)) {
        /* Invalid TZ_MemoryId_t */
        return 0U;
    }

    index = id - 1;

    if (NsClientIdList[index].ns_client_id == INVALID_CLIENT_ID) {
        /* Non-existent client */
        return 0U;
    }

    active_ns_client_idx = index;
#ifdef PRINT_NSPM_DEBUG
    printf("TZ_LoadContext_S called for id %d\r\n",
        NsClientIdList[index].ns_client_id);
#endif /* PRINT_NSPM_DEBUG */

    return 1U;    // Success
}


/// Store secure context (called on RTOS thread context switch)
/// \param[in]  id  TrustZone memory slot identifier
/// \return execution status (1: success, 0: error)
/* This veneer is TF-M internal, not a secure service */
__attribute__((cmse_nonsecure_entry))
uint32_t TZ_StoreContext_S (TZ_MemoryId_t id)
{
    uint32_t index;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return 0U;
    }

#ifdef PRINT_NSPM_DEBUG
    LOG_MSG("TZ_StoreContext_S called");
#endif /* PRINT_NSPM_DEBUG */
    /* id corresponds to context being swapped out on NS side */
    if ((id == 0U) || (id > TFM_MAX_NS_THREAD_COUNT)) {
        /* Invalid TZ_MemoryId_t */
        return 0U;
    }

    index = id - 1;

    if (NsClientIdList[index].ns_client_id == INVALID_CLIENT_ID) {
        /* Non-existent client */
        return 0U;
    }

    if (active_ns_client_idx != index) {
#ifdef PRINT_NSPM_DEBUG
        printf("TZ_StoreContext_S called for id %d, active id: %d\r\n",
            NsClientIdList[index].ns_client_id,
            NsClientIdList[active_ns_client_idx].ns_client_id);
#endif /* PRINT_NSPM_DEBUG */
        return 0U;
    }

#ifdef PRINT_NSPM_DEBUG
    printf("TZ_StoreContext_S called for id %d\r\n",
        NsClientIdList[index].ns_client_id);
#endif /* PRINT_NSPM_DEBUG */
    active_ns_client_idx = DEFAULT_NS_CLIENT_IDX;

    return 1U;    // Success
}

#ifdef TFM_NS_CLIENT_IDENTIFICATION
__attribute__((cmse_nonsecure_entry))
enum tfm_status_e tfm_register_client_id (int32_t ns_client_id)
{
    int current_client_id;

    if (__get_active_exc_num() == EXC_NUM_THREAD_MODE) {
        /* This veneer should only be called by NS RTOS in handler mode */
        return TFM_ERROR_NS_THREAD_MODE_CALL;
    }

    if (ns_client_id >= 0) {
        /* The client ID is invalid */
        return TFM_ERROR_INVALID_PARAMETER;
    }

    if (active_ns_client_idx < 0) {
        /* No client is active */
        return TFM_ERROR_GENERIC;
    }

    current_client_id = NsClientIdList[active_ns_client_idx].ns_client_id;
    if (current_client_id >= 0 ) {
        /* The client ID is invalid */
        return TFM_ERROR_INVALID_PARAMETER;
    }

    NsClientIdList[active_ns_client_idx].ns_client_id = ns_client_id;
#ifdef PRINT_NSPM_DEBUG
    printf("tfm_register_client_id called with id %d\r\n", ns_client_id);
#endif /* PRINT_NSPM_DEBUG */

    return TFM_SUCCESS;
}
#endif

#ifdef TFM_PSA_API
__attribute__((section("SFN")))
psa_status_t tfm_nspm_thread_entry(void)
{
#ifdef TFM_CORE_DEBUG
    /* Jumps to non-secure code */
    LOG_MSG("Jumping to non-secure code...");
#endif

    jump_to_ns_code();

    /* Should not run here */
    TFM_ASSERT(false);
    return PSA_SUCCESS;
}
#endif
