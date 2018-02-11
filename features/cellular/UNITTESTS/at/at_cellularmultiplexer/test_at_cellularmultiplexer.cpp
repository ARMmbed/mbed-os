/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularmultiplexer.h"
#include <string.h>
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularDevice.h"
#include "AT_CellularMultiplexer.h"
#include "FileHandle_stub.h"

using namespace mbed;
using namespace events;

Test_AT_CellularMultiplexer::Test_AT_CellularMultiplexer()
{

}

Test_AT_CellularMultiplexer::~Test_AT_CellularMultiplexer()
{
}

void Test_AT_CellularMultiplexer::test_AT_CellularMultiplexer_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularMultiplexer *cm = new AT_CellularMultiplexer(at);
    delete cm;
}

void Test_AT_CellularMultiplexer::test_AT_CellularMultiplexer_multiplexer_mode_start()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularMultiplexer cm(at);
    CHECK(!cm.multiplexer_mode_start());
}

