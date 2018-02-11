/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellulardevice.h"
#include "AT_CellularDevice.h"
#include "ATHandler_stub.h"
#include "AT_CellularBase_stub.h"
#include <string.h>

using namespace mbed;
using namespace events;

Test_AT_CellularDevice::Test_AT_CellularDevice()
{

}

Test_AT_CellularDevice::~Test_AT_CellularDevice()
{
}

void Test_AT_CellularDevice::test_AT_CellularDevice_constructor()
{
    EventQueue que;
    AT_CellularDevice dev(que);

    CellularDevice *dev2 = new AT_CellularDevice(que);
    delete dev2;
}

void Test_AT_CellularDevice::test_AT_CellularDevice_get_at_handler()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    FileHandle_stub fh2;
    FileHandle_stub fh3;

    dev.open_network(&fh1);
    dev.open_sms(&fh2);
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;
    dev.open_sim(&fh3);
    ATHandler_stub::fh_value = &fh1;
    dev.open_power(&fh1);

    ATHandler_stub::fh_value = NULL;
}

void Test_AT_CellularDevice::test_AT_CellularDevice_open_network()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    CHECK(!dev.open_network(NULL));
    CHECK(dev.open_network(&fh1));
}

void Test_AT_CellularDevice::test_AT_CellularDevice_open_sms()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    CHECK(!dev.open_sms(NULL));
    CHECK(dev.open_sms(&fh1));
}

void Test_AT_CellularDevice::test_AT_CellularDevice_open_power()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    CHECK(!dev.open_power(NULL));
    CHECK(dev.open_power(&fh1));
}

void Test_AT_CellularDevice::test_AT_CellularDevice_open_sim()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    CHECK(! dev.open_sim(NULL));
    CHECK(dev.open_sim(&fh1));
}

void Test_AT_CellularDevice::test_AT_CellularDevice_open_multiplexer()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    CHECK(!dev.open_multiplexer(NULL));
    CHECK(dev.open_multiplexer(&fh1));
}

void Test_AT_CellularDevice::test_AT_CellularDevice_open_information()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;

    CHECK(!dev.open_information(NULL));
    CHECK(dev.open_information(&fh1));
}

void Test_AT_CellularDevice::test_AT_CellularDevice_close_network()
{

}

void Test_AT_CellularDevice::test_AT_CellularDevice_close_sms()
{

}

void Test_AT_CellularDevice::test_AT_CellularDevice_close_power()
{

}

void Test_AT_CellularDevice::test_AT_CellularDevice_close_sim()
{

}

void Test_AT_CellularDevice::test_AT_CellularDevice_close_multiplexer()
{

}

void Test_AT_CellularDevice::test_AT_CellularDevice_close_information()
{
    EventQueue que;
    AT_CellularDevice dev(que);
    FileHandle_stub fh1;
    ATHandler_stub::int_value = 0;

    CHECK(dev.open_information(&fh1));

    ATHandler_stub::fh_value = NULL;
    AT_CellularBase_stub::handler_value = NULL;
    dev.close_information();

    ATHandler_stub::fh_value = &fh1;
    ATHandler at(&fh1, que, 0);
    AT_CellularBase_stub::handler_value = &at;

    CHECK(dev.open_information(&fh1));
    AT_CellularBase_stub::handler_value = AT_CellularBase_stub::handler_at_constructor_value;

    dev.close_information();

    ATHandler_stub::fh_value = NULL;
}

