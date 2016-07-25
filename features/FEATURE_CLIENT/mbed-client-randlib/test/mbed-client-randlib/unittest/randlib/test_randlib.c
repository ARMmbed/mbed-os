/*
 * Copyright (c) 2016 ARM. All rights reserved.
 */
#include "test_randlib.h"
#include <string.h>
#include <inttypes.h>
#include "randLIB.h"

bool test_randLIB_seed_random()
{
    randLIB_seed_random();
    return true;
}

bool test_randLIB_get_8bit()
{
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

bool test_randLIB_get_n_bytes_random()
{
    int8_t ret = randLIB_get_n_bytes_random(NULL, 0);
    if( ret != -1){
        return false;
    }

    uint8_t dat[5];
    ret = randLIB_get_n_bytes_random(&dat, 5);
    if( ret != 0){
        return false;
    }
    return true;
}

bool test_randLIB_get_random_in_range()
{
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
