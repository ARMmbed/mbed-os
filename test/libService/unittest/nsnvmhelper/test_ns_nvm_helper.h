/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */
#ifndef TEST_NS_NVM_HELPER_H
#define TEST_NS_NVM_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_ns_nvm_helper_write();
bool test_ns_nvm_helper_read();
bool test_ns_nvm_helper_delete();
bool test_ns_nvm_helper_concurrent_requests();
bool test_ns_nvm_helper_platform_error();
bool test_ns_nvm_helper_platform_error_in_write();

#ifdef __cplusplus
}
#endif

#endif // TEST_NS_NVM_HELPER_H

