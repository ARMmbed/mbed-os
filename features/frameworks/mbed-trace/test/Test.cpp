/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
/**
 * \file \test_libTrace\Test.c
 *
 * \brief Unit tests for mbed_trace
 */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "mbed-cpputest/CppUTest/TestHarness.h"
#include "mbed-cpputest/CppUTest/SimpleString.h"
#include "mbed-cpputest/CppUTest/CommandLineTestRunner.h"

#define MBED_CONF_MBED_TRACE_ENABLE 1
#define MBED_CONF_MBED_TRACE_FEA_IPV6 1

#include "mbed-trace/mbed_trace.h"
#include "ip6tos_stub.h"

int main(int ac, char **av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

static int mutex_wait_count = 0;
static int mutex_release_count = 0;
static bool check_mutex_lock_status = true;
void my_mutex_wait()
{
  mutex_wait_count++;
}
void my_mutex_release()
{
  mutex_release_count++;
}

char buf[1024];
#include <stdio.h>
void myprint(const char* str)
{
  if ( check_mutex_lock_status ) {
      CHECK( (mutex_wait_count - mutex_release_count) > 0 );
  }
  strcpy(buf, str);
}
TEST_GROUP(trace)
{
  void setup()
  {

    mbed_trace_init();
    mbed_trace_config_set(TRACE_MODE_PLAIN|TRACE_ACTIVE_LEVEL_ALL);
    mbed_trace_print_function_set( myprint );
    mbed_trace_mutex_wait_function_set( my_mutex_wait );
    mbed_trace_mutex_release_function_set( my_mutex_release );
  }
  void teardown()
  {
    CHECK(mutex_wait_count == mutex_release_count); // Check the mutex count with every test
    mbed_trace_free();
  }
};

/* Unity test code starts */
TEST(trace, MutexNotSet)
{
  mbed_trace_mutex_wait_function_set( 0 );
  mbed_trace_mutex_release_function_set( 0 );
  int mutex_call_count_at_entry = mutex_wait_count;
  check_mutex_lock_status = false;

  char expectedStr[] = "Hello hello!";
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "Hello hello!");
  STRCMP_EQUAL(expectedStr, buf);

  CHECK( mutex_call_count_at_entry == mutex_wait_count );
  CHECK( mutex_call_count_at_entry == mutex_release_count );

  mbed_trace_mutex_wait_function_set( my_mutex_wait );
  mbed_trace_mutex_release_function_set( my_mutex_release );
  check_mutex_lock_status = true;
}

TEST(trace, Array)
{
  unsigned char longStr[200] = {0x66};
  for(int i=0;i<200;i++) {longStr[i] = 0x66; }
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", mbed_trace_array(longStr, 200) );
}

TEST(trace, Null0Array)
{
  static const unsigned char array[2] = { 0x23, 0x45 };
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", mbed_trace_array(array, 2));
  STRCMP_EQUAL("23:45", buf);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", mbed_trace_array(array, 0));
  STRCMP_EQUAL("", buf);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", mbed_trace_array(NULL, 0));
  STRCMP_EQUAL("", buf);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", mbed_trace_array(NULL, 2));
  STRCMP_EQUAL("<null>", buf);
}

TEST(trace, LongString)
{
  char longStr[1000] = {0x36};
  for(int i=0;i<999;i++) {longStr[i] = 0x36; }
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", longStr );
}

TEST(trace, TooLong)
{
  #define TOO_LONG_SIZE 9400
  #define TRACE_LINE_SIZE 1024
  char longStr[TOO_LONG_SIZE] = {0};
  for(int i=0;i<TOO_LONG_SIZE;i++) { longStr[i] = 0x36; }  
  
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", longStr );
  
  char shouldStr[TRACE_LINE_SIZE] = "[DBG ][mygr]: ";
  for(int i=14;i<TRACE_LINE_SIZE;i++) { shouldStr[i] = 0x36; }
  shouldStr[TRACE_LINE_SIZE-1] = 0;
  STRCMP_EQUAL(shouldStr, buf);
}

TEST(trace, BufferResize)
{
    uint8_t arr[20] = {0};
    memset(arr, '0', 20);

    mbed_trace_buffer_sizes(0, 10);
    STRCMP_EQUAL("30:30:30*", mbed_trace_array(arr, 20));
    mbed_trace_buffer_sizes(0, 15);
    STRCMP_EQUAL("30:30:30:30*", mbed_trace_array(arr, 20));
    mbed_trace_buffer_sizes(0, 15);
    STRCMP_EQUAL("30:30:30:30", mbed_trace_array(arr, 4));

    const char * expectedStr = "0123456789";
    mbed_trace_buffer_sizes(11, 0);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "01234567890123456789");
    STRCMP_EQUAL(expectedStr, buf);
    expectedStr = "012345678901234";
    mbed_trace_buffer_sizes(16, 0);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "01234567890123456789");
    STRCMP_EQUAL(expectedStr, buf);
    expectedStr = "012345678901234";
    mbed_trace_buffer_sizes(16, 0);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "012345678901234");
    STRCMP_EQUAL(expectedStr, buf);
}

TEST(trace, PreInitConfiguration)
{
    uint8_t arr[20] = {0};
    memset(arr, '0', 20);

    mbed_trace_free();
    mbed_trace_config_set(TRACE_MODE_PLAIN|TRACE_ACTIVE_LEVEL_ALL);
    mbed_trace_print_function_set( myprint );
    mbed_trace_buffer_sizes(11, 10);
    mbed_trace_mutex_wait_function_set( my_mutex_wait );
    mbed_trace_mutex_release_function_set( my_mutex_release );
    mbed_trace_init();

    STRCMP_EQUAL("30:30:30*", mbed_trace_array(arr, 20));

    const char * expectedStr = "0123456789";
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "01234567890123456789");
    STRCMP_EQUAL(expectedStr, buf);
}

#if YOTTA_CFG_MBED_TRACE_FEA_IPV6 == 1
ip6tos_stub_def_t ip6tos_stub; // extern variable

TEST(trace, ipv6)
{
    uint8_t prefix[] = { 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 };
    int prefix_len = 64;

    char expected_str1[] = "146e:a00::/64";
    ip6tos_stub.output_string = "146e:a00::/64";
    char *str = mbed_trace_ipv6_prefix(prefix, prefix_len);
    CHECK(memcmp(ip6tos_stub.input_array, prefix, 8) == 0);
    STRCMP_EQUAL(expected_str1, str);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "flush buffers and locks");

    char expected_str2[] = "::/0";
    ip6tos_stub.output_string = "::/0";
    str = mbed_trace_ipv6_prefix(NULL, 0);
    STRCMP_EQUAL(expected_str2, str);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "flush buffers and locks");

    char expected_str3[] = "<err>";
    str = mbed_trace_ipv6_prefix(NULL, 1);
    STRCMP_EQUAL(expected_str3, str);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "flush buffers and locks");

    char expected_str4[] = "<err>";
    str = mbed_trace_ipv6_prefix(prefix, 200);
    STRCMP_EQUAL(expected_str4, str);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "flush buffers and locks");

    char expected_str5[] = "";
    ip6tos_stub.output_string = "0123456789012345678901234567890123456789";
    str = mbed_trace_ipv6_prefix(prefix, 64); // Fill the tmp_data buffer
    str = mbed_trace_ipv6_prefix(prefix, 64);
    str = mbed_trace_ipv6_prefix(prefix, 64);
    str = mbed_trace_ipv6_prefix(prefix, 64);
    STRCMP_EQUAL(expected_str5, str);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "flush buffers and locks");
}

TEST(trace, active_level_all_ipv6)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  
  uint8_t arr[] = { 0x20, 0x01, 0xd, 0xb8, 0,0,0,0,0,1,0,0,0,0,0,1 };
  ip6tos_stub.output_string = "2001:db8::1:0:0:1";
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "my addr: %s", mbed_trace_ipv6(arr));
  CHECK(memcmp(ip6tos_stub.input_array, arr, 16) == 0);
  STRCMP_EQUAL("[DBG ][mygr]: my addr: 2001:db8::1:0:0:1", buf);
}
#endif //YOTTA_CFG_MBED_TRACE_FEA_IPV6

TEST(trace, config_change)
{
    mbed_trace_config_set(TRACE_MODE_COLOR|TRACE_ACTIVE_LEVEL_ALL);
    CHECK(mbed_trace_config_get() == TRACE_MODE_COLOR|TRACE_ACTIVE_LEVEL_ALL);
    mbed_trace_config_set(TRACE_MODE_PLAIN|TRACE_ACTIVE_LEVEL_NONE);
    CHECK(mbed_trace_config_get() == TRACE_MODE_PLAIN|TRACE_ACTIVE_LEVEL_NONE);
    mbed_trace_config_set(TRACE_MODE_PLAIN|TRACE_ACTIVE_LEVEL_ALL);
    CHECK(mbed_trace_config_get() == TRACE_MODE_PLAIN|TRACE_ACTIVE_LEVEL_ALL);
}

TEST(trace, active_level_all_color)
{
  mbed_trace_config_set(TRACE_MODE_COLOR|TRACE_ACTIVE_LEVEL_ALL);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hello");
  STRCMP_EQUAL("\x1b[90m[DBG ][mygr]: hello\x1b[0m", buf);
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "to one");
  STRCMP_EQUAL("\x1b[39m[INFO][mygr]: to one\x1b[0m", buf);
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "and all");
  STRCMP_EQUAL("\x1b[33m[WARN][mygr]: and all\x1b[0m", buf);
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "even you");
  STRCMP_EQUAL("\x1b[31m[ERR ][mygr]: even you\x1b[0m", buf);
}

TEST(trace, change_levels)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("[DBG ][mygr]: hep", buf);
  
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG|TRACE_MODE_PLAIN);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("hep", buf);
  
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG|TRACE_MODE_COLOR);
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "hep");
  STRCMP_EQUAL("\x1b[31m[ERR ][mygr]: hep\x1b[0m", buf);
  
}

TEST(trace, active_level_debug)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("[DBG ][mygr]: hep", buf);
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("[INFO][mygr]: test", buf);
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("[WARN][mygr]: hups", buf);
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}

TEST(trace, active_level_info)
{
  buf[0] = 0;
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_INFO);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("[INFO][mygr]: test", buf);
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("[WARN][mygr]: hups", buf);
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}

TEST(trace, active_level_warn)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_WARN);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("[WARN][mygr]: hups", buf);
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}

TEST(trace, active_level_error)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ERROR);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}
TEST(trace, active_level_none)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_NONE);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("", mbed_trace_last());
}

TEST(trace, active_level_all_1)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_trace_exclude_filters_set((char*)"mygr");
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  STRCMP_EQUAL("[DBG ][mygu]: hep", buf);
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
}
TEST(trace, active_level_all_2)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_trace_exclude_filters_set((char*)"mygr,mygu");
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
}
TEST(trace, active_level_all_3)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_trace_include_filters_set((char*)"mygr");
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("[INFO][mygr]: test", buf);
}

TEST(trace, active_level_all_array)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  
  uint8_t arr[] = {0x01, 0x02, 0x03};
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "my addr: %s", mbed_trace_array(arr, 3));
  STRCMP_EQUAL("[DBG ][mygr]: my addr: 01:02:03", buf);
}


size_t time_length;
char trace_prefix_str[] = "[<TIME>]";
char* trace_prefix(size_t length){ 
  time_length = length;
  return trace_prefix_str; 
}
TEST(trace, prefix)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_trace_prefix_function_set( &trace_prefix );
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "test %d %d", 1, 2);
  STRCMP_EQUAL("[<TIME>][DBG ][mygr]: test 1 2", buf);
  //TEST_ASSERT_EQUAL_INT(4, time_length);
  
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_PLAIN);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  STRCMP_EQUAL("test", buf);
  
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_COLOR);
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "test");
  STRCMP_EQUAL("\x1b[31m[<TIME>][ERR ][mygr]: test\x1b[0m", buf);
}
char trace_suffix_str[] = "[END]";
char* trace_suffix()
{
  return trace_suffix_str;
}
TEST(trace, suffix)
{
  mbed_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_trace_prefix_function_set( &trace_prefix );
  mbed_trace_suffix_function_set( &trace_suffix );
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  STRCMP_EQUAL("[<TIME>][DBG ][mygr]: test[END]", buf);
}
TEST(trace, formatting)
{
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hello %d %d %.1f", 12, 13, 5.5);
    STRCMP_EQUAL("hello 12 13 5.5", buf);
    
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hello %d %d %d %d %d %d", 12, 13, 5, 6, 8, 9);
    STRCMP_EQUAL("hello 12 13 5 6 8 9", buf);
    
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "HOH %d HAH %d %d %d %d %d", 12, 13, 5, 6, 8, 9);
    STRCMP_EQUAL("HOH 12 HAH 13 5 6 8 9", buf);
}
TEST(trace, filters_control)
{
    mbed_trace_include_filters_set((char*)"hello");
    STRCMP_EQUAL("hello", mbed_trace_include_filters_get());
    
    mbed_trace_include_filters_set(0);
    STRCMP_EQUAL("", mbed_trace_include_filters_get());
    
    mbed_trace_exclude_filters_set((char*)"hello");
    STRCMP_EQUAL("hello", mbed_trace_exclude_filters_get());
    
    mbed_trace_exclude_filters_set(0);
    STRCMP_EQUAL("", mbed_trace_exclude_filters_get());
}
TEST(trace, no_printer)
{
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hello");
    STRCMP_EQUAL("hello", buf);
    
    mbed_trace_print_function_set(NULL);
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "this shoudnt be printed because printer is missing");
    STRCMP_EQUAL("hello", buf);
}
TEST(trace, uninitialized)
{
    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hello");
    STRCMP_EQUAL("hello", buf);

    mbed_trace_free();

    mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "this shoudnt be printed because mtrace is not initialized");
    STRCMP_EQUAL("hello", buf);
}

