/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */
#ifndef MBED_TRACE_TEST_STUBS_IP6STRING_H_
#define MBED_TRACE_TEST_STUBS_IP6STRING_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

typedef struct {
    const char *output_string;
    uint8_t input_array[16];
} ip6tos_stub_def_t;

extern ip6tos_stub_def_t ip6tos_stub;

#ifdef __cplusplus
}
#endif
#endif /* MBED_TRACE_TEST_STUBS_IP6STRING_H_ */
