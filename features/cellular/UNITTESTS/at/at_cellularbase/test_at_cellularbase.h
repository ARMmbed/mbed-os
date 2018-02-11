/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_AT_CELLULARBASE_H
#define TEST_AT_CELLULARBASE_H

class Test_AT_CellularBase
{
public:
    Test_AT_CellularBase();

    virtual ~Test_AT_CellularBase();

    void test_AT_CellularBase_get_at_handler();

    void test_AT_CellularBase_get_device_error();
};

#endif // TEST_AT_CELLULARBASE_H

