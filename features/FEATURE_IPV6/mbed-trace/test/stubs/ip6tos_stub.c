/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */
#include "ip6string.h"
#include <string.h>
#include <stdio.h>
#include "common_functions.h"

#include "ip6tos_stub.h"

ip6tos_stub_def_t ip6tos_stub; // extern variable

uint8_t ip6tos(const void *ip6addr, char *p)
{
    memcpy(ip6tos_stub.input_array, ip6addr, 16);
    strcpy(p, ip6tos_stub.output_string);
    return strlen(p);
}

uint_fast8_t ip6_prefix_tos(const void *prefix, uint_fast8_t prefix_len, char *p)
{
    bitcopy(ip6tos_stub.input_array, prefix, prefix_len);
    strcpy(p, ip6tos_stub.output_string);
    return strlen(p);
}
