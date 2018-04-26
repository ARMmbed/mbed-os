/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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
#include "mbed.h"
#include "platform/mbed_trace_internal.h"
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using utest::v1::Case;

#if !defined(MBED_CONF_MBED_TRACE_FEA_IPV6) || !defined(MBED_CONF_MBED_TRACE_ENABLE)
#warning "Helper functions will not be tested, as MBED_CONF_MBED_TRACE_FEA_IPV6/MBED_CONF_MBED_TRACE_ENABLE is not set"
#endif

LOG_DATA_TYPE_ *buf = NULL;

static void test_log_reset()
{
    log_reset();
    if (NULL == buf) {
        buf = new LOG_DATA_TYPE_[MBED_CONF_PLATFORM_LOG_MAX_BUFFER_SIZE];
    }
    memset(buf, 0x0, MBED_CONF_PLATFORM_LOG_MAX_BUFFER_SIZE);
    log_buffer_data(buf);
    log_disable_time_capture();
    return;
}

static void test_log_clean()
{
    delete[] buf;
}

static void test_log_levels()
{
    test_log_reset();
    MBED_ERR("TEST", "Error performing XYZ operation, errno = %d", -10);
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[ERR ][TEST]: Error performing XYZ operation, errno = -10", buf);

    test_log_reset();
    MBED_WARN("TEST", "This is warning message");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[WARN][TEST]: This is warning message", buf);
    
    test_log_reset();
    MBED_DBG_IF("TEST", false, "This should not be printed");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("", buf);
    
    test_log_reset();
    MBED_DBG_IF("TEST", (1 == 1), "Cool 1 is 1");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[DBG ][TEST]: Cool 1 is 1", buf);
    
    test_log_reset();
    MBED_DBG("TEST", "%s %s %d", "Hello World!", "Count is:", 20);
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[DBG ][TEST]: Hello World! Count is: 20", buf);

    test_log_reset();
    MBED_INFO_IF("TEST", (1 == 0) , "Really ???");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("", buf);
    
    test_log_reset();
    MBED_INFO_IF("TEST", (1 != 0), "Correct %s", "Always");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[INFO][TEST]: Correct Always", buf);
    
    test_log_reset();
    MBED_INFO("TEST", "Again here!! Entry is: %d Exit is: %d", 120, 121);
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[INFO][TEST]: Again here!! Entry is: 120 Exit is: 121", buf);
    
    test_log_reset();
    MBED_TRACE_IF("TEST", 0 , "No trace");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("", buf);
    
    test_log_reset();
    MBED_TRACE_IF("TEST", (1 != 0), "Trace level not enabled by default");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("", buf);
    
    test_log_reset();
    MBED_TRACE("TEST", "No trace - disabled");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("", buf);
    return;
}

static void test_gen_log_api()
{
    test_log_reset();
    MBED_LOG(2, "GENx", "Error performing XYZ operation, errno = %d", -10);
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[ERR ][GENx]: Error performing XYZ operation, errno = -10", buf);

    test_log_reset();
    MBED_LOG(4, "GENx", "This is warning message");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[WARN][GENx]: This is warning message", buf);
    
    test_log_reset();
    MBED_LOG(10, "GENx", "%s %s %d", "Hello World!", "Count is:", 20);
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[DBG ][GENx]: Hello World! Count is: 20", buf);
   
    test_log_reset();
    MBED_LOG(8, "GENx", "Again here!! Entry is: %d Exit is: %d", 120, 121);
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[INFO][GENx]: Again here!! Entry is: 120 Exit is: 121", buf);
    
    test_log_reset();
    MBED_LOG(20, "GENx", "MBED_LOG prints all levels");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[TRAC][GENx]: MBED_LOG prints all levels", buf);
    return;
}

static void test_log_helper_arrays()
{
    static uint8_t longStr[5];

    longStr[0] = 0x23;
    longStr[1] = 0x45;
    test_log_reset();
    MBED_INFO("TEST", "%s", mbed_trace_array(longStr, 2));
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[INFO][TEST]: 23:45", buf);
    
    test_log_reset();
    MBED_DBG("TEST", "%s", mbed_trace_array(longStr, 0));
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[DBG ][TEST]: ", buf);
    
    test_log_reset();
    MBED_WARN("Testing", "%s", mbed_trace_array(NULL, 0));
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[WARN][Test]: ", buf);
    
    test_log_reset();
    MBED_ERR("TEST", "%s", mbed_trace_array(NULL, 2));
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[ERR ][TEST]: <null>", buf);
    return;
}

static void test_log_helper_ipv6()
{
    uint8_t prefix[] = { 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 };
    int prefix_len = 64;

    test_log_reset();
    MBED_INFO("IPv6", "%s", mbed_trace_ipv6_prefix(prefix, prefix_len));
    wait_us(1000);
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: 146e:a00::/64", buf);
#else
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: ", buf);
#endif
    
    test_log_reset();
    MBED_INFO("IPv6", "%s", mbed_trace_ipv6_prefix(NULL, 0));
    wait_us(1000);
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: ::/0", buf);
#else
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: ", buf);
#endif
    
    test_log_reset();
    MBED_INFO("IPv6", "%s", mbed_trace_ipv6_prefix(NULL, 1));
    wait_us(1000);
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: <err>", buf);
#else
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: ", buf);
#endif

    test_log_reset();
    MBED_INFO("IPv6", "%s", mbed_trace_ipv6_prefix(prefix, 200));
    wait_us(1000);
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: <err>", buf);
#else
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: ", buf);
#endif

    uint8_t arr[] = { 0x20, 0x01, 0xd, 0xb8, 0,0,0,0,0,1,0,0,0,0,0,1 };
    test_log_reset();
    MBED_INFO("IPv6", "%s", mbed_trace_ipv6(arr));
    wait_us(1000);
#if defined(MBED_CONF_MBED_TRACE_FEA_IPV6)
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: 2001:db8::1:0:0:1", buf);
#else
    TEST_ASSERT_EQUAL_STRING("[INFO][IPv6]: ", buf);
#endif
    return;
}

void LogCallback(void)
{
    MBED_WARN("LOG", "Inside ISR");
}

static void test_log_isr() 
{
    Ticker t1;
    test_log_reset();
    t1.attach_us(LogCallback, 100000);
    wait_us(101000);
    TEST_ASSERT_EQUAL_STRING("[WARN][LOG ]: Inside ISR", buf);
    return;
}

static void test_log_interleave() 
{
    Ticker t1;
    test_log_reset();
    t1.attach(LogCallback, 1);
    uint32_t count = 0;
    
    while(count++ < 3) {
        wait_us(200000);
        MBED_DBG_IF("TEST", (0 == (count%2)), "Even");
        wait_us(200000);
        MBED_INFO_IF("TEST", (0 != (count%2)), "Odd");
    }
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[INFO][TEST]: Odd"
                             "[DBG ][TEST]: Even" 
                             "[WARN][LOG ]: Inside ISR"
                             "[INFO][TEST]: Odd", buf);
    return;
}

static void test_log_overflow()
{
    static uint8_t longStr[100];
    for(int i = 0; i < 100; i++)
    {
        longStr[i] = 0x88;
    }

    test_log_reset();
    MBED_WARN("TEST", "%s", mbed_trace_array(longStr, 100));
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[WARN][TEST]: 88:88:88:88:88:88:88:88:88:88:88:88:88:"
                             "88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:"
                             "88:88:88:88:88:88:88:88:88:88:88:*", buf);

    test_log_reset();
    MBED_WARN("TEST", "01:02:03:04:05:06:07:08:09:10:11:12:13:14:15:16:17:18:19:20:21:22:23:24:25:26:27:28:29:30:"
                      "31:32:33:34:35:36:37:38:39:40:41:42:43:44:45:46:47:48:49:50:51:52:53:54:55:56:57:58:59:60:"
                      "61:62:63:64:65:66:67:68:69:70:71:72:73:74:75:76:77:78:79:80:81:82:83:84:85:86:87:88:89:90:");
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[WARN][TEST]: 01:02:03:04:05:06:07:08:09:10:11:12:13:14:15:16:17:18:19:20:21:22:23:24:25:26:27:28:29:30:"
                             "31:32:33:34:35:36:37:38:39:40:41:42:43:44:45:46:47:48:49:50:51:52:53:54:55:56:57:58:59:60:"
                             "61:62:63:64:65:66:67:68:69:70:71:72:73:74:75:76:77:78:79:80*\n", buf);

    test_log_reset();
    MBED_WARN("TEST", "01:02:03:04:05:06:07:08:09:10:11:12:13:14:15:16:17:18:19:20:21:22:23:24:25:26:27:28:29:30:"
                      "31:32:33:34:35:36:37:38:39:40:41:42:43:44:45:46:47:48:49:50:51:52:53:54:55:56:57:58:59:60: %s", 
                      mbed_trace_array(longStr, 100));
    wait_us(1000);
    TEST_ASSERT_EQUAL_STRING("[WARN][TEST]: 01:02:03:04:05:06:07:08:09:10:11:12:13:14:15:16:17:18:19:20:21:22:23:24:25:26:27:28:29:30:"
                             "31:32:33:34:35:36:37:38:39:40:41:42:43:44:45:46:47:48:49:50:51:52:53:54:55:56:57:58:59:60: "
                             "88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:88:8*\n", buf);

    return;
}

// Test cases
Case cases[] = {
    Case("Test all log levels", test_log_levels),
    Case("Test generic log API", test_gen_log_api),
    Case("Test helper array function", test_log_helper_arrays),
    Case("Test helper ipv6 functions", test_log_helper_ipv6),
    Case("Test log in ISR context", test_log_isr),
    Case("Test log interleave test", test_log_interleave),
    Case("Test overflow test", test_log_overflow),
    Case("Cleanup buffers", test_log_clean)
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(15, "default_auto");
    return utest::v1::greentea_test_setup_handler(number_of_cases);
}

utest::v1::Specification specification(greentea_test_setup, cases, utest::v1::greentea_test_teardown_handler);

int main()
{
    utest::v1::Harness::run(specification);
}
