/*
 * Copyright (c) 2016, 2021, Pelion and affiliates.
 */
#ifndef TEST_RANDLIB_H
#define TEST_RANDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_randLIB_seed_random();

bool test_randLIB_get_8bit();

bool test_randLIB_get_16bit();

bool test_randLIB_get_32bit();

bool test_randLIB_get_64bit();

bool test_randLIB_get_n_bytes_random();

bool test_randLIB_get_random_in_range();

bool test_randLIB_randomise_base();


#ifdef __cplusplus
}
#endif

#endif // TEST_RANDLIB_H
