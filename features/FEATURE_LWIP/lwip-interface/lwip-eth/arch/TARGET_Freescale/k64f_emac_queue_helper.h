/*
 * Copyright (c) 2017 ARM Limited. All rights reserved.
 */

#ifndef K64F_EMAC_QUEUE_HELPER_H_
#define K64F_EMAC_QUEUE_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

int k64f_emac_call_every(int ms, void (*fn)(void *), void *handle);

#ifdef __cplusplus
}
#endif

#endif /* K64F_EMAC_QUEUE_HELPER_H_ */
