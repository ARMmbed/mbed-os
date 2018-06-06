/*
 * Copyright (c) 2016 ARM. All rights reserved.
 */
#include "test_randlib.h"
#include <string.h>
#include <inttypes.h>
#include "randLIB.h"

bool test_randLIB_seed_random()
{
    randLIB_reset();
    randLIB_seed_random();
    return true;
}

bool test_randLIB_get_8bit()
{
    randLIB_reset();
    randLIB_seed_random();
    uint8_t test = randLIB_get_8bit();
    if( test == 0 ) {
        test = randLIB_get_8bit();
        if( test == 0 ) {
            return false;
        }
    }
    return true;
}

bool test_randLIB_get_16bit()
{
    randLIB_reset();
    randLIB_seed_random();
    uint16_t test = randLIB_get_16bit();
    if( test == 0 ) {
        test = randLIB_get_16bit();
        if( test == 0 ) {
            return false;
        }
    }
    return true;
}

bool test_randLIB_get_32bit()
{
    randLIB_reset();
    randLIB_seed_random();
    uint32_t test = randLIB_get_32bit();
    if( test == 0 ) {
        test = randLIB_get_32bit();
        if( test == 0 ) {
            return false;
        }
    }
    return true;
}

static bool test_output(uint32_t seed, bool seed_inc, const uint64_t expected[8])
{
    random_stub_set_seed(seed, seed_inc);
    randLIB_reset();
    randLIB_seed_random();
    for (int i = 0; i < 8; i++) {
        if (randLIB_get_64bit() != expected[i]) {
            return false;
        }
    }

    return true;
}

bool test_randLIB_get_64bit()
{
    /* Initial 8 xoroshiro128+ values with initial seed of
     * (0x0000000100000002, 0x00000003000000004).
     */
    static const uint64_t expected1234[] = {
        UINT64_C(0x0000000400000006),
        UINT64_C(0x0100806200818026),
        UINT64_C(0x2a30826271904706),
        UINT64_C(0x918d7ea50109290d),
        UINT64_C(0x5dcbd701c1e1c64c),
        UINT64_C(0xaa129b152055f299),
        UINT64_C(0x4c95c2b1e1038a4d),
        UINT64_C(0x6479e7a3a75d865a)
    };

    if (!test_output(1, true, expected1234)) {
        goto fail;
    }

    /* If passed all zero seeds, seeding should detect this
     * and use splitmix64 to create the actual seed
     * (0xe220a8397b1dcdaf, 0x6e789e6aa1b965f4),
     * and produce this output:
     */
    static const uint64_t expected0000[] = {
        UINT64_C(0x509946a41cd733a3),
        UINT64_C(0x00885667b1934bfa),
        UINT64_C(0x1061f9ad258fd5d5),
        UINT64_C(0x3f8be44897a4317c),
        UINT64_C(0x60da683bea50e6ab),
        UINT64_C(0xd6b52f5379de1de0),
        UINT64_C(0x2608bc9fedc5b750),
        UINT64_C(0xb9fac9c7ec9de02a)
    };

    if (!test_output(0, false, expected0000)) {
        goto fail;
    }

    /* If passed all "4" seeds, seeding should detect this
     * and use splitmix64 to create the actual seed
     * (0x03910b0eab9b37e1, 0x0b309ab53d42b2a2),
     * and produce this output:
     */
    static const uint64_t expected4444[] = {
        UINT64_C(0x0ec1a5c3e8ddea83),
        UINT64_C(0x09e710b8faf5a491),
        UINT64_C(0xd4102776f79448b4),
        UINT64_C(0x5d61988b60091900),
        UINT64_C(0xf6c8a72a9c72cb4b),
        UINT64_C(0xb06923e0cf0f2fb1),
        UINT64_C(0x24bbed475153f573),
        UINT64_C(0xfff0b4bd08c5581f),
    };

    if (!test_output(4, false, expected4444)) {
        goto fail;
    }

    /* Last test used constant seed of 4, which is the default */
    return true;

fail:
    /* Put back the default seed of 4 (other tests might rely on it) */
    random_stub_set_seed(4, false);
    return false;
}

bool test_randLIB_get_n_bytes_random()
{
    randLIB_reset();
    randLIB_seed_random();

    uint8_t dat[5];
    void *ret = randLIB_get_n_bytes_random(dat, 5);
    if(ret != dat){
        return false;
    }

    uint8_t dat2[5];
    randLIB_get_n_bytes_random(dat2, 5);
    if (memcmp(dat, dat2, 5) == 0) {
        return false;
    }

    return true;
}

bool test_randLIB_get_random_in_range()
{
    randLIB_reset();
    randLIB_seed_random();

    uint16_t ret = randLIB_get_random_in_range(2, 2);
    if( ret != 2 ){
        return false;
    }

    ret = randLIB_get_random_in_range(2, 3);
    if( ret != 2 && ret != 3){
        return false;
    }

    ret = randLIB_get_random_in_range(0, 0xFFFF);

    return true;
}

bool test_randLIB_randomise_base()
{
    randLIB_reset();
    randLIB_seed_random();

    uint32_t ret = randLIB_randomise_base(0,0,0);
    if( ret ){
        return false;
    }
    ret = randLIB_randomise_base(0xffff0000,0x8888,0x8888);
    if( ret != 0xffffffff ){
        return false;
    }
    return true;
}
