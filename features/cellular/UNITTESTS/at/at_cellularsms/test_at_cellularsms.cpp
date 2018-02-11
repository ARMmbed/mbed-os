/*
 * Copyright (c) 2018 ARM. All rights reserved.
 */
#include "CppUTest/TestHarness.h"
#include "test_at_cellularsms.h"
#include <string.h>
#include "AT_CellularNetwork.h"
#include "EventQueue.h"
#include "ATHandler.h"
#include "AT_CellularSMS.h"
#include "FileHandle_stub.h"
#include "CellularLog.h"

using namespace mbed;
using namespace events;

uint32_t mbed::cellular_log_time = 0;

Test_AT_CellularSMS::Test_AT_CellularSMS()
{

}

Test_AT_CellularSMS::~Test_AT_CellularSMS()
{
}

void Test_AT_CellularSMS::test_AT_CellularSMS_constructor()
{
    EventQueue que;
    FileHandle_stub fh1;
    ATHandler at(&fh1, que, 0);

    AT_CellularSMS *sms = new AT_CellularSMS(at);

    delete sms;
}

void Test_AT_CellularSMS::test_AT_CellularSMS_initialize(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_send_sms(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_get_sms(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_sms_callback(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_cpms(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_csca(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_cscs(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_delete_all_messages(){

}


void Test_AT_CellularSMS::test_AT_CellularSMS_set_extra_sim_wait_time(){

}

