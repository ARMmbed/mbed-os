/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#ifndef TEST_AT_CELLULARINFORMATION_H
#define TEST_AT_CELLULARINFORMATION_H

class Test_AT_CellularInformation
{
public:
    Test_AT_CellularInformation();

    virtual ~Test_AT_CellularInformation();

    void test_AT_CellularInformation_get_manufacturer();

    void test_AT_CellularInformation_get_model();

    void test_AT_CellularInformation_get_revision();
};

#endif // TEST_AT_CELLULARINFORMATION_H

