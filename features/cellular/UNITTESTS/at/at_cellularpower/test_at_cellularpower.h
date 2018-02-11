/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_AT_CELLULARPOWER_H
#define TEST_AT_CELLULARPOWER_H

class Test_AT_CellularPower
{
public:
    Test_AT_CellularPower();

    virtual ~Test_AT_CellularPower();

    void test_AT_CellularPower_constructor();

    void test_AT_CellularPower_on();

    void test_AT_CellularPower_off();

    void test_AT_CellularPower_set_at_mode();

    void test_AT_CellularPower_set_power_level();

    void test_AT_CellularPower_reset();

    void test_AT_CellularPower_opt_power_save_mode();

    void test_AT_CellularPower_opt_receive_period();
};

#endif // TEST_AT_CELLULARPOWER_H

