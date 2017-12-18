/* mbed Microcontroller Library
 * Copyright (c) 2006-2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "BMComm.h"

typedef enum
{
    PASSED, FAILED
} test_case_status_t;

typedef enum
{
    SYNC_OK, SYNC_ERR
} sync_status_t;

#define BUFFER_SIZE 100

#define BM_TEST_ASSERT(TEST_OBJ, CONDITION) TEST_OBJ.assert_cond(CONDITION, __LINE__)

/* Test class to support bare metal tests.
 *
 * Example usage:
 *
 *  BMTest test;
 *
 *  test.set_transmission_params(USBTX, USBRX, BAUD_RATE);
 *  test.sync_host_dev();
 *  test.set_timeout(10);
 *  test.exec_test_case(test_func_1, "Verify that...", NULL);
 *  test.exec_test_case(test_func_2, "Verify that...", "<host support function>");
 *  test.done();
 *
 *
 * Note:
 *  <host support function> is used to define function which should be executed
 *  on the host side for this test case.
 *
 */
class BMTest
{

public:
    typedef void (*Test_function_t)(BMTest& test);

    BMTest()
    {
        _comm = NULL;
        _sync_complete = SYNC_ERR;
        _test_status = PASSED;
        _tc_total_cnt = 0;
        _tc_pass_cnt = 0;
        _tc_fail_cnt = 0;
    }

    ~BMTest()
    {
        delete _comm;
    }

    /* Send string to the host. */
    void send(char * buffer)
    {
        if (_comm != NULL) {
            _comm->write_line(buffer);
        }
    }

    /* Receive string from the host. */
    int receive(char * buffer)
    {
        if (_comm != NULL) {
            return _comm->read_line(buffer);
        }

        return 0;
    }

    /* Set UART pins used for transmission and select transmission speed. */
    void set_transmission_params(PinName tx, PinName rx, int baud)
    {
        _comm = new BMComm(tx, rx, baud);
    }

    /* Sync communication between device and host. */
    void sync_host_dev()
    {
        if (_comm != NULL) {
            _comm->sync_host_dev();

            /* If we are here, then sync was successful. */
            _sync_complete = SYNC_OK;
        }
    }

    /* Set test timeout. */
    void set_timeout(unsigned int seconds)
    {
        snprintf(_buffer, sizeof(_buffer), "{\"cmd\": \"timeout\", \"val\": \"%u\"}", seconds);
        send(_buffer);
    }

    /* Execute single test case. */
    void exec_test_case(Test_function_t test_case, const char * description, const char * host_test_func)
    {
        _tc_status = PASSED;
        _tc_total_cnt++;

        if (_sync_complete == SYNC_OK) {
            snprintf(_buffer, sizeof(_buffer), "{\"cmd\": \"description\", \"val\": \"%s\"}", description);
            send(_buffer);

            if (host_test_func) {
                snprintf(_buffer, sizeof(_buffer), "{\"cmd\": \"host_test\", \"val\": \"%s\"}", host_test_func);
            } else {
                snprintf(_buffer, sizeof(_buffer), "{\"cmd\": \"host_test\", \"val\": \"%s\"}", "(null)");
            }

            send(_buffer);

            test_case(*this);

            if (_tc_status == PASSED) {
                snprintf(_buffer, sizeof(_buffer), "{\"cmd\": \"status\", \"val\": \"%s\"}", "PASSED");
                _tc_pass_cnt++;
            } else {
                snprintf(_buffer, sizeof(_buffer), "{\"cmd\": \"status\", \"val\": \"%s\"}", "FAILED");
                _tc_fail_cnt++;
            }

            send(_buffer);
        }
    }

    /* Test case assertion to verify result. */
    void assert_cond(bool condition, int line)
    {
        if (!condition) {
            snprintf(_buffer, sizeof(_buffer), "{\"cmd\": \"assert\", \"val\": \"%d%s\"}", line,
                    "::FAIL: Expression Evaluated To FALSE");
            send(_buffer);
            _tc_status = FAILED;
            _test_status = FAILED;
        }
    }

    /* Indicate that test is finished. */
    void done()
    {
        snprintf(_buffer, sizeof(_buffer),
                "{\"cmd\": \"done\", \"val\": {\"total\": \"%d\", \"passed\": \"%d\",\"failed\": \"%d\"}}",
                _tc_total_cnt, _tc_pass_cnt, _tc_fail_cnt);
        send(_buffer);
    }

private:
    BMComm * _comm;
    sync_status_t _sync_complete;
    test_case_status_t _tc_status;
    test_case_status_t _test_status;
    unsigned int _tc_total_cnt;
    unsigned int _tc_pass_cnt;
    unsigned int _tc_fail_cnt;
    char _buffer[BUFFER_SIZE];
};

