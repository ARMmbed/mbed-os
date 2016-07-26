/*
 * Copyright (c) 2016 ARM Limited. All rights reserved.
 */
#ifndef TEST_NS_NVM_HELPER_H
#define TEST_NS_NVM_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool test_cs_nvm_init_finalize();
bool test_cs_nvm_key_create();
bool test_cs_nvm_key_delete();
bool test_cs_nvm_key_delete_failure_in_close();
bool test_cs_nvm_read();
bool test_cs_nvm_read_failure();
bool test_cs_nvm_read_failure_in_close();
bool test_cs_nvm_write();
bool test_cs_nvm_write_failure();
bool test_cs_nvm_write_failure_in_close();
bool test_cs_nvm_flush();
bool test_cs_nvm_operation_in_progress();

#ifdef __cplusplus
}
#endif

#endif // TEST_NS_NVM_HELPER_H

