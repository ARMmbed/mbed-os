/*
 * Copyright (c) 2015 ARM. All rights reserved.
 */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestPlugin.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/MockSupportPlugin.h"
int main(int ac, char **av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

IMPORT_TEST_GROUP(randLIB);

