/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularsim.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularSIM.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;

uint32_t mbed::cellular_log_time = 0;

Test_AT_CellularSIM::Test_AT_CellularSIM()
{

}

Test_AT_CellularSIM::~Test_AT_CellularSIM()
{
}

void Test_AT_CellularSIM::test_AT_CellularSIM_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSIM *sim = new AT_CellularSIM(at);

    delete sim;
}

void Test_AT_CellularSIM::test_AT_CellularSIM_set_pin()
{

}

void Test_AT_CellularSIM::test_AT_CellularSIM_change_pin()
{

}

void Test_AT_CellularSIM::test_AT_CellularSIM_set_pin_query()
{

}

void Test_AT_CellularSIM::test_AT_CellularSIM_get_sim_state()
{

}
