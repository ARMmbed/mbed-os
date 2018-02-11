/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_AT_CELLULARSIM_H
#define TEST_AT_CELLULARSIM_H

class Test_AT_CellularSIM
{
public:
    Test_AT_CellularSIM();

    virtual ~Test_AT_CellularSIM();

    void test_AT_CellularSIM_constructor();

    void test_AT_CellularSIM_set_pin();

    void test_AT_CellularSIM_change_pin();

    void test_AT_CellularSIM_set_pin_query();

    void test_AT_CellularSIM_get_sim_state();
};

#endif // TEST_AT_CELLULARSIM_H

