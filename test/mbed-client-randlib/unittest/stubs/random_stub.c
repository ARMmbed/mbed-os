/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 */
#include "random_stub.h"

static uint32_t seed_value = 4;
static bool seed_inc = false;

void arm_random_module_init(void)
{

}

uint32_t arm_random_seed_get(void)
{
    uint32_t result = seed_value;
    if (seed_inc) {
        ++seed_value;
    }
    return result;
}

void random_stub_set_seed(uint32_t value, bool increment)
{
    seed_value = value;
    seed_inc = increment;
}
