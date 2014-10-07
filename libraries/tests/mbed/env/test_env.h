#ifndef TEST_ENV_H_
#define TEST_ENV_H_

#include <stdio.h>
#include "mbed.h"

#define NL "\n"
#define RCNL "\r\n"

// Const strings used in test_end
extern const char* TEST_ENV_START;
extern const char* TEST_ENV_SUCCESS;
extern const char* TEST_ENV_FAILURE;
extern const char* TEST_ENV_MEASURE;
extern const char* TEST_ENV_END;

// Test result related notification functions
void notify_start();
void notify_completion(bool success);
bool notify_completion_str(bool success, char* buffer);
void notify_performance_coefficient(const char* measurement_name, const int value);
void notify_performance_coefficient(const char* measurement_name, const unsigned int value);
void notify_performance_coefficient(const char* measurement_name, const double value);

// Test functionality useful during testing
unsigned int testenv_randseed();

// Macros, unit test like to provide basic comparisons
#define TESTENV_STRCMP(GIVEN,EXPECTED) (strcmp(GIVEN,EXPECTED) == 0)

// macros passed via test suite
#ifndef TEST_SUITE_TARGET_NAME
#define TEST_SUITE_TARGET_NAME "Unknown"
#endif

#ifndef TEST_SUITE_TEST_ID
#define TEST_SUITE_TEST_ID "Unknown"
#endif

#ifndef TEST_SUITE_UUID
#define TEST_SUITE_UUID "Unknown"
#endif

#endif
