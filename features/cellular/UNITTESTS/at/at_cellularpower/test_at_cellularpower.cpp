/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularpower.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularPower.h"
#include "FileHandle_stub.h"

using namespace mbed;
using namespace events;

Test_AT_CellularPower::Test_AT_CellularPower()
{

}

Test_AT_CellularPower::~Test_AT_CellularPower()
{
}

void Test_AT_CellularPower::test_AT_CellularPower_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularPower *pow = new AT_CellularPower(at);

    delete pow;
}

void Test_AT_CellularPower::test_AT_CellularPower_on()
{

}

void Test_AT_CellularPower::test_AT_CellularPower_off()
{

}

void Test_AT_CellularPower::test_AT_CellularPower_set_at_mode()
{

}

void Test_AT_CellularPower::test_AT_CellularPower_set_power_level()
{

}

void Test_AT_CellularPower::test_AT_CellularPower_reset()
{

}

void Test_AT_CellularPower::test_AT_CellularPower_opt_power_save_mode()
{

}

void Test_AT_CellularPower::test_AT_CellularPower_opt_receive_period()
{

}
