/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_mbed_mux_data_service.h"
#include "mbed_mux.h"
#include "mbed_mux_stub.h"
#include <string.h>

using namespace mbed;

Test_mbed_mux_data_service::Test_mbed_mux_data_service()
{

}

Test_mbed_mux_data_service::~Test_mbed_mux_data_service()
{
}

void Test_mbed_mux_data_service::test_mbed_mux_write()
{
    MuxDataService mux;
    char table[] = "ss";
    mbed_mux_stub::size_value = 6;
    CHECK(6 == mux.write(table, 2));
}

void Test_mbed_mux_data_service::test_mbed_mux_read()
{
    MuxDataService mux;
    char table[6];

    mbed_mux_stub::size_value = 67;
    CHECK(67 == mux.read(table, 6));
}

void Test_mbed_mux_data_service::test_mbed_mux_seek()
{
    MuxDataService mux;
    CHECK(0 == mux.seek(0));
}

void Test_mbed_mux_data_service::test_mbed_mux_close()
{
    MuxDataService mux;
    CHECK(0 == mux.close());
}

void cb()
{

}

void Test_mbed_mux_data_service::test_mbed_mux_sigio()
{
    MuxDataService mux;
    mux.sigio(cb);
}

