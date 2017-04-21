/*
 * Copyright (c) 2016 ARM Limited, All Rights Reserved
 */

#ifndef NS_HAL_INIT_H_
#define NS_HAL_INIT_H_

#include <stddef.h>
#include "nsdynmemLIB.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialise core Nanostack HAL components.
 *
 * Calls after the first do nothing. So "major" users should make sure
 * they call this first with a "large" heap size, before anyone
 * requests a smaller one.
 *
 * Parameters are as for ns_dyn_mem_init (but note that nsdynmemlib
 * currently limits heap size to 16-bit, so be wary of passing large
 * sizes.
 *
 * If heap is NULL, h_size will be allocated from the malloc() heap,
 * else the passed-in pointer will be used.
 */
void ns_hal_init(void *heap, size_t h_size, void (*passed_fptr)(heap_fail_t), mem_stat_t *info_ptr);

#ifdef __cplusplus
}
#endif

#endif /* NS_HAL_INIT_H_ */
