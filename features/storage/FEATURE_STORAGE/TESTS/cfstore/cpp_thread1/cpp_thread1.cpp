/*
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 *
 */

/** @file cpp_thread1.cpp    Cfstore threaded test case
 *
 * Test to do the following:
 * - thread1 & thread2 compete to make updates to a shared KV.
 *   - main thread creates shared KV.
 *   - thread1 grows KV and rseeks byte values from pos near end.
 *   - thread2 shrinks KV and rseeks byte values from pos near beginning.
 *   - no thread locking to co-ordinate access.
 * - thread1 & thread2 complete to delete a key
 *   - main thread creates shared key integer counter inited to large value
 *   - thread1 decrements counter; if 0 deletes key.
 *   - thread2 decrements counter; if 0 deletes key.
 *   - thread1 & thread2 in race to delete key. Test that key is gracefully closed.
 * - n threads calls Cfstore() more than once which will invoke C-HAL Initialize() more
 *   than once. Then go onto make create/delete KVs.
 * - thread1 & thread2 compete to add/del KVs in the same part of the cfstore
 *   name space, with the same key name
 */

#include "stdio.h"
#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"
#include "rtos.h"
#include "cfstore_debug.h"
#include "cfstore_test.h"
#include "cfstore_os.h"
#include "cfstore.h"

#if defined(MBED_RTOS_SINGLE_THREAD)
  #error [NOT_SUPPORTED] test not supported
#endif


using namespace utest::v1;
using namespace cfstore;

#define SEMAPHORE_SLOTS  1
#define CFSTORE_CPP_THREAD1_SEM_CHANGES      50

/// @cond CFSTORE_DOXYGEN_DISABLE
#ifdef CFSTORE_DEBUG
#define CFSTORE_CPP_THREAD1_GREENTEA_TIMEOUT_S     60
#else
#define CFSTORE_CPP_THREAD1_GREENTEA_TIMEOUT_S     30
#endif
/// @endcond

#define CFSTORE_CPP_THREAD1_TEST_ASSERT_MSG(Expr, _fmt, ...)        \
    do                                                              \
    {                                                               \
        if (!(Expr))                                                \
        {                                                           \
            printf(_fmt, __VA_ARGS__);                              \
            while (1) ;                                             \
        }                                                           \
    } while (0);

/*
 * The stack size is defined in cmsis_os.h mainly dependent on the underlying toolchain and
 * the C standard library. For GCC, ARM_STD and IAR it is defined with a size of 2048 bytes
 * and for ARM_MICRO 512. Because of reduce RAM size some targets need a reduced stacksize.
 */
#if (defined(TARGET_STM32L053R8) || defined(TARGET_STM32L053C8)) && defined(TOOLCHAIN_GCC)
    #define STACK_SIZE DEFAULT_STACK_SIZE/16
#elif (defined(TARGET_STM32F030R8) || defined(TARGET_STM32F070RB)) && defined(TOOLCHAIN_GCC)
    #define STACK_SIZE DEFAULT_STACK_SIZE/8
#elif defined(TARGET_STM32F334R8) && (defined(TOOLCHAIN_GCC) || defined(TOOLCHAIN_IAR))
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif defined(TARGET_STM32F103RB) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif defined(TARGET_STM32F030R8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif defined(TARGET_STM32F070RB) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F072RB) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F302R8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/2
#elif defined(TARGET_STM32F303K8) && defined(TOOLCHAIN_IAR)
    #define STACK_SIZE DEFAULT_STACK_SIZE/4
#elif (defined(TARGET_EFM32HG_STK3400)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 512
#elif (defined(TARGET_EFM32LG_STK3600) || defined(TARGET_EFM32WG_STK3800) || defined(TARGET_EFM32PG_STK3401)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 768
#elif (defined(TARGET_EFM32GG_STK3700)) && !defined(TOOLCHAIN_ARM_MICRO)
    #define STACK_SIZE 1536
#elif defined(TARGET_MCU_NRF51822) || defined(TARGET_MCU_NRF52832)
    #define STACK_SIZE 768
#else
    #define STACK_SIZE DEFAULT_STACK_SIZE
#endif

#define CFSTORE_CPP_THREAD1_MODULUS_ODD     1
#define CFSTORE_CPP_THREAD1_MODULUS_EVEN    2
#define CFSTORE_CPP_THREAD1_KEY_NAME        "com.arm.mbed.mbedos.cfstore.tests.cpp_thread1.test_case_01.count"
#define CFSTORE_CPP_THREAD1_TABLE1_PATHNAME "com.arm.mbed.mbedos.cfstore.tests.cpp_thread1.test_case_02.table1"
#define CFSTORE_CPP_THREAD1_TABLE2_PATHNAME "com.arm.mbed.mbedos.cfstore.tests.cpp_thread1.test_case_02.table2"

static char cfstore_utest_msg_g[CFSTORE_UTEST_MSG_BUF_SIZE];
static cfstore_kv_data_t cfstore_cpp_thread1_kv_data = { CFSTORE_CPP_THREAD1_KEY_NAME, "dummy_data"};

/** @brief  Worker Thread Class with context attributes controlling the behaviour of the thread testing */
class CfstoreCppThread : public Thread
{
public:
    CfstoreCppThread(osPriority priority=osPriorityNormal, uint32_t stack_size=DEFAULT_STACK_SIZE, unsigned char *stack_pointer=NULL)
    {
        ::Thread(priority, stack_size, stack_pointer);
    }

    int32_t modulus;            //!< modulus parameter used in test case 01 to determine whether thread increments odd or even numbers
    int32_t id;                 //!< thread identifier used by the test
    int32_t status;             //!< final status of the thread. used by main thread to check for successful worker thread completion.
    char *table_pathname;       //!< key_name string to prepend to table data.
    int32_t subset;             //!< subset of KV attributes to manipulate e.g. delete */
};


/** @brief  test case 01 helper functions to get integer value from shared KV */
int32_t cfstore_cpp_thread1_counter_get(int32_t *val)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t val_int;
    size_t value_len = 0;
    cfstore_fmode_t flags;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    key = cfstore.open(cfstore_cpp_thread1_kv_data.key_name, flags);
    if (key == NULL) {
        return ret;
    }
    value_len = sizeof(val_int);
    ret = key->read((void*) &val_int, &value_len);
    if (ret < ARM_DRIVER_OK) {
        return ret;
    }
    if (val) {
        *val = val_int;
    }

    ret = key->close();
    delete key;
    return ret;
}


/** @brief  test case 01 helper functions to set integer value for shared KV */
int32_t cfstore_cpp_thread1_counter_set(int32_t val)
{
    int32_t ret = ARM_DRIVER_ERROR;
    size_t value_len = 0;
    cfstore_fmode_t flags;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    flags.write = true;
    key = cfstore.open(cfstore_cpp_thread1_kv_data.key_name, flags);
    if (key == NULL) {
        return ret;
    }
    value_len = sizeof(int32_t);
    ret = key->write((const char*) &val, &value_len);
    if (ret < ARM_DRIVER_OK) {
        return ret;
    }
    ret = key->close();
    delete key;
    return ret;
}

/** @brief  cfstore client worker thread for test case 01 */
void cfstore_cpp_thread1_tc_01_worker_thread(CfstoreCppThread *self)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t counter = 0;
    size_t value_len = 0;
    CfstoreKey *key = NULL;
    cfstore_fmode_t flags;
    int32_t thread_modulus = self->modulus;
    /* initialize the config store so can access the interface */
    Cfstore cfstore = Cfstore();

    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    flags.write = true;
    key = cfstore.open(cfstore_cpp_thread1_kv_data.key_name, flags);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Cfstore::open() failed to return valid key failed\n", __func__);
    TEST_ASSERT_MESSAGE(key != NULL, cfstore_utest_msg_g);

    while (true) {

        /* reset file position to start of attribute so can re-read attribute
         * This checks that rseek works */
        ret = key->rseek(0);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE rseek(0) failed (ret=%d, key_name=%s)\n", __func__, (int) ret, cfstore_cpp_thread1_kv_data.key_name);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

        value_len = sizeof(counter);
        ret = key->read((void*) &counter, &value_len);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE read() failed (ret=%d, key_name=%s)\n", __func__, (int) ret, cfstore_cpp_thread1_kv_data.key_name);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

        if (counter < CFSTORE_CPP_THREAD1_SEM_CHANGES) {
            if (counter % CFSTORE_CPP_THREAD1_MODULUS_EVEN == thread_modulus-1) {
                /* thread == CFSTORE_CPP_THREAD1_MODULUS_ODD and number is even, or
                 * thread == CFSTORE_CPP_THREAD1_MODULUS_EVEN and number is odd
                 * then increment counter
                 */
                counter++;

                ret = cfstore_cpp_thread1_counter_set(counter);
                CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE write() failed (ret=%d, key_name=%s counter=%d)\n", __func__, (int) ret, cfstore_cpp_thread1_kv_data.key_name, (int) counter);
                TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

                cfstore_debug_log("thread modulus:%d:threadId:%p:incremented counter to counter=%d, ret=%d, value_len=%d, after write\n", thread_modulus, (void *)osThreadGetId(), (int) counter, (int) ret, (int) value_len);
            }
        } else {
            break;
        }
        wait_ms(100);
    }
}

/** @brief  cfstore client worker thread for test case 01
 *
 * initialize the cfstore in the main thread, and create 2 thread,
 * thread1 and thread2, which are cfstore client worker threads.
 *   - thread1 & thread2 co-operatively increment a shared KV storing
 *     integer counter:
 *   - main creates shared KV counter
 *   - thread 1 increments even numbers to the next odd value.
 *   - thread 2 increments odd numbers to the next even value
 *   - values incremented to 100
 *   - threads use a semaphore to lock-step incrementing the counter
 *   - terminates the worker threads
 *   - destroys the cfstore object
 *
 */
control_t cfstore_cpp_thread1_case_01(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t counter = 1;
    size_t value_len = 0;
    cfstore_fmode_t flags;
    cfstore_keydesc_t kdesc;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    cfstore_debug_log("%s:debug: entered.\n", __func__);
    (void) call_count;
    /* Initialize CFSTORE before starting worker threads. */
    ret = cfstore.initialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE initialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    memset(&flags, 0, sizeof(flags));
    memset(&kdesc, 0, sizeof(kdesc));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    value_len = sizeof(counter);
    key = cfstore.create(cfstore_cpp_thread1_kv_data.key_name, value_len, &kdesc);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE create() failed (ret=%d, key_name=%s)\n", __func__, (int) ret, cfstore_cpp_thread1_kv_data.key_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    ret = key->write((const char*) &counter, &value_len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE write() failed (ret=%d, key_name=%s counter=%d)\n", __func__, (int) ret, cfstore_cpp_thread1_kv_data.key_name, (int) counter);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    CfstoreCppThread *t1 = new CfstoreCppThread(osPriorityNormal, STACK_SIZE);
    CfstoreCppThread *t2 = new CfstoreCppThread(osPriorityNormal, STACK_SIZE);
    t1->modulus = CFSTORE_CPP_THREAD1_MODULUS_ODD;
    t2->modulus = CFSTORE_CPP_THREAD1_MODULUS_EVEN;
    t1->start(t1, cfstore_cpp_thread1_tc_01_worker_thread);
    t2->start(t2, cfstore_cpp_thread1_tc_01_worker_thread);

    while (true) {
        ret = cfstore_cpp_thread1_counter_get((int32_t*) &counter);
        CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_cpp_thread1_counter_get() failed (ret=%d)\n", __func__, (int) ret);
        TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);
        if (counter >= CFSTORE_CPP_THREAD1_SEM_CHANGES) {
            t1->join();
            t2->join();
            break;
        }
        wait_ms(100);
    }

    /*  read the cfstore counter value and check its the expected value */
    value_len = sizeof(counter);
    ret = key->read((void*) &counter, &value_len);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE read() failed (ret=%d, key_name=%s)\n", __func__, (int) ret, cfstore_cpp_thread1_kv_data.key_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE counter != expected value (key_name=%s, counter=%d, expected value=%d)\n", __func__, cfstore_cpp_thread1_kv_data.key_name, (int) counter, (int) (CFSTORE_CPP_THREAD1_SEM_CHANGES));
    TEST_ASSERT_MESSAGE(counter == CFSTORE_CPP_THREAD1_SEM_CHANGES, cfstore_utest_msg_g);

    ret = key->close();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE close() failed (ret=%d, key_name=%s)\n", __func__, (int) ret, cfstore_cpp_thread1_kv_data.key_name);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);
    delete key;

    ret = cfstore.uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE uninitialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    /* ~Cfstore() is called when object on stack goes out of scope. */
    return CaseNext;
}


/* @brief   test utility function to read KV value from the cfstore */
static int32_t cfstore_cpp_thread1_get(const char *key_name, char *data, size_t *len, size_t offset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_fmode_t flags;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    /* cfstore_debug_log("%s:%p:entered, key_name=%s\n", __func__, (void *)osThreadGetId(), key_name); */
    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    key = cfstore.open(key_name, flags);
    if (key == NULL) {
        cfstore_error_log("%s:%p:Error: failed to open key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        return ret;
    }
    if (offset > 0) {
        ret = key->rseek(offset);
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:%p:Error: failed to rseek() to offset key (key_name=%s, offset=%d)\n", __func__, (void *)osThreadGetId(), key_name, (int) offset);
            goto out0;
        }
    }
    ret = key->read(data, len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:%p:Error: failed to read() key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        goto out0;
    }
out0:
    key->close();
    delete key;
    return ret;
}


/* @brief   test utility function to get key name from open KV from the cfstore */
static int32_t cfstore_cpp_thread1_get_key_name(const char *key_name, char *data, uint8_t *len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_fmode_t flags;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    /* cfstore_debug_log("%s:%p:entered, key_name=%s\n", __func__, (void *)osThreadGetId(), key_name); */
    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    key = cfstore.open(key_name, flags);
    if (key == NULL) {
        cfstore_error_log("%s:%p:Error: failed to open key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        return ret;
    }
    ret = key->get_key_name(data, len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:%p:Error: failed to get_key_name() on key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        goto out0;
    }
out0:
    key->close();
    delete key;
    return ret;
}

/* @brief   test utility function to get key name from open KV from the cfstore */
static int32_t cfstore_cpp_thread1_get_value_len(const char *key_name, size_t *len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    cfstore_fmode_t flags;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    /* cfstore_debug_log("%s:%p:entered, key_name=%s\n", __func__, (void *)osThreadGetId(), key_name); */
    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    key = cfstore.open(key_name, flags);
    if (key == NULL) {
        cfstore_error_log("%s:%p:Error: failed to open key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        return ret;
    }
    ret = key->get_value_len(len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:%p:Error: failed to get_value_len() on key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        goto out0;
    }
out0:
    key->close();
    delete key;
    return ret;
}


/* @brief   test utility function to create KV from the cfstore */
static int32_t cfstore_cpp_thread1_create(const char *key_name, const char *data, size_t *len, cfstore_keydesc_t *kdesc)
{
    int32_t ret = ARM_DRIVER_ERROR;
    size_t value_len = 0;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    value_len = *len;
    kdesc->drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    key = cfstore.create(key_name, value_len, kdesc);
    if (key == NULL) {
        cfstore_error_log("%s:%p:Error: failed to create() key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        return ret;
    }
    value_len = *len;
    ret = key->write(data, &value_len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:%p:Error: failed to write() key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        goto out0;
    }
    if (value_len != *len) {
        ret = ARM_DRIVER_ERROR;
        goto out0;
    }
out0:
    key->close();
    delete key;
    return ret;
}

/* @brief   test utility function to delete KV from the cfstore */
static int32_t cfstore_cpp_thread1_delete(const char *key_name)
{
    int32_t ret = ARM_DRIVER_ERROR;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    /* cfstore_debug_log("%s:%p:entered, key_name=%s\n", __func__, (void *)osThreadGetId(), key_name); */
    key = cfstore.find(key_name, NULL);
    if (key == NULL) {
        cfstore_error_log("%s:%p:Error: failed to find key (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        return ret;
    }

    ret = key->destroy();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:%p:Error: failed to destroy (key_name=%s)\n", __func__, (void *)osThreadGetId(), key_name);
        goto out1;
    }
out1:
    key->close();
    delete key;
    return ret;
}


/* @brief   test utility function to create KVs from the supplied table */
static int32_t cfstore_cpp_thread1_create_table(const cfstore_kv_data_t *table, const char *table_key_name)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t count = 0;
    size_t len = 0;
    cfstore_keydesc_t kdesc;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    memset(&kdesc, 0, sizeof(kdesc));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    while (table->key_name != NULL) {
        len = strlen(table->value);
        memset(key_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
        /* add nodes alternately into 2 subgroups: a) <table_key_name>.0.key_name and b) <table_key_name>.1.key_name.
         * group a) will be deleted first, group b) second. */
        snprintf(key_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%d.%s", table_key_name, (int) count % 2, table->key_name);
        ret = cfstore_cpp_thread1_create(key_name, table->value, &len, &kdesc);
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:Error: failed to create node (key_name=\"%s\", value=\"%s\")\n", __func__, key_name, table->value);
            return ret;
        }
        table++;
        count++;
    }
    return ret;
}

/** @brief   test utility function to check the cfstore contains KVs created from the supplied table */
static int32_t cfstore_cpp_thread1_table_data_check(const cfstore_kv_data_t *table, const char *table_key_name)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t count = 0;
    char data[CFSTORE_KEY_NAME_MAX_LENGTH];
    size_t len = 0;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    while (table->key_name != NULL) {
        len = strlen(table->value);
        memset(key_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
        memset(data, 0, CFSTORE_KEY_NAME_MAX_LENGTH);
        snprintf(key_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%d.%s", table_key_name, (int) count % 2, table->key_name);
        ret = cfstore_cpp_thread1_get(key_name, data, &len, 0);
        if (ret < ARM_DRIVER_OK) {
            cfstore_error_log("%s:Error: failed to create node (key_name=\"%s\", value=\"%s\")\n", __func__, key_name, table->value);
            return ret;
        }
        /* cfstore_debug_log("%s:Debug: (key_name=(%s), value=(%s), value len=%d)\n", __func__, key_name, data, (int) len); */
        if (strncmp(table->value, data, len) != 0) {
            cfstore_error_log("%s:Error: cfstore node value != table node value (key_name=(%s), table value=(%s), cfstore value=(%s))\n", __func__, key_name, table->value, data);
            return ARM_DRIVER_ERROR;
        }

        table++;
        count++;
    }
    return ret;
}


/* @brief   test utility function to delete KVs from the supplied subset
 *
 * The nodes to be deleted will have the following key_names:
 *      <table_key_name>.<subset>.key_name
 */
static int32_t cfstore_cpp_thread1_subtable_delete(const cfstore_kv_data_t *table, const char *table_key_name, int32_t subset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t count = 0;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    while (table->key_name != NULL) {
        if (count % 2 == subset) {
            /* the node should exist in the table */
            memset(key_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
            snprintf(key_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%d.%s", table_key_name, (int) subset, table->key_name);
            cfstore_debug_log("%s:Debug:(key_name=(%s), table_key_name=(%s)), table->key_name=(%s)\n", __func__, key_name, table_key_name, table->key_name);
            ret = cfstore_cpp_thread1_delete(key_name);
            if (ret < ARM_DRIVER_OK) {
                cfstore_error_log("%s:Error: failed to delete node (key_name=\"%s\", value=\"%s\")\n", __func__, key_name, table->value);
                return ret;
            }
        }
        table++;
        count++;
    }
    return ret;
}

/* @brief   test utility function to get_key_name() on KVs from the supplied subset */
static int32_t cfstore_cpp_thread1_subtable_get_key_name(const cfstore_kv_data_t *table, const char *table_key_name, int32_t subset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t count = 0;
    uint8_t len = 0;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    char data[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    while (table->key_name != NULL) {
        if (count % 2 == subset) {
            /* the node should exist in the table */
            memset(key_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
            memset(data, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
            len = CFSTORE_KEY_NAME_MAX_LENGTH;
            snprintf(key_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%d.%s", table_key_name, (int) subset, table->key_name);
            /* cfstore_debug_log("%s:Debug:(key_name=(%s), table_key_name=(%s)), table->key_name=(%s)\n", __func__, key_name, table_key_name, table->key_name); */
            ret = cfstore_cpp_thread1_get_key_name(key_name, data, &len);
            if (ret < ARM_DRIVER_OK) {
                cfstore_error_log("%s:Error: failed to get key name for node (key_name=\"%s\", value=\"%s\")\n", __func__, key_name, table->value);
                return ret;
            }
            len = strlen(key_name);
            if (strncmp(key_name, data, len) != 0) {
                cfstore_error_log("%s:Error: cfstore node name != table node name (key_name=(%s), get_key_name() name=(%s))\n", __func__, key_name, data);
                return ARM_DRIVER_ERROR;
            }
        }
        table++;
        count++;
    }
    return ret;
}

/* @brief   test utility function to get_value_len() on KVs from the supplied subset */
static int32_t cfstore_cpp_thread1_subtable_get_value_len(const cfstore_kv_data_t *table, const char *table_key_name, int32_t subset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t count = 0;
    size_t len = 0;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    while (table->key_name != NULL) {
        if (count % 2 == subset) {
            /* the node should exist in the table */
            memset(key_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
            len = CFSTORE_KEY_NAME_MAX_LENGTH;
            snprintf(key_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%d.%s", table_key_name, (int) subset, table->key_name);
            /* cfstore_debug_log("%s:Debug:(key_name=(%s), table_key_name=(%s)), table->key_name=(%s)\n", __func__, key_name, table_key_name, table->key_name); */
            ret = cfstore_cpp_thread1_get_value_len(key_name, &len);
            if (ret < ARM_DRIVER_OK) {
                cfstore_error_log("%s:Error: failed to get value len for node (key_name=\"%s\")\n", __func__, key_name);
                return ret;
            }
            if (strlen(table->value) != len) {
                cfstore_error_log("%s:Error: cfstore KV get_value_len() != table node value len (key_name=(%s), len=%d, get_value_len() len=%d)\n", __func__, key_name, (int) strlen(table->value), (int) len);
                return ARM_DRIVER_ERROR;
            }
        }
        table++;
        count++;
    }
    return ret;
}


#ifdef CFSTORE_CPP_THREAD1_RSEEK_TEST
/* @brief   test utility function to rseek(x) on KVs from the supplied subset */
static int32_t cfstore_cpp_thread1_subtable_rseek(const cfstore_kv_data_t *table, const char *table_key_name, int32_t subset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t count = 0;
    size_t len = 0;
    size_t value_len = 0;
    size_t offset = 0;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    char data[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    while (table->key_name != NULL) {
        if (count % 2 == subset) {
            /* the node should exist in the table */
            memset(key_name, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
            memset(data, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);
            len = CFSTORE_KEY_NAME_MAX_LENGTH;
            snprintf(key_name, CFSTORE_KEY_NAME_MAX_LENGTH, "%s.%d.%s", table_key_name, (int) subset, table->key_name);
            /* cfstore_debug_log("%s:Debug:(key_name=(%s), table_key_name=(%s)), table->key_name=(%s)\n", __func__, key_name, table_key_name, table->key_name); */

            ret = cfstore_cpp_thread1_get_value_len(key_name, &value_len);
            if (ret < ARM_DRIVER_OK) {
                cfstore_error_log("%s:Error: failed to get value len for node (key_name=\"%s\")\n", __func__, key_name);
                return ret;
            }
            offset = value_len / 2;
            ret = cfstore_cpp_thread1_get(key_name, data, &len, offset);
            if (ret < ARM_DRIVER_OK) {
                cfstore_error_log("%s:Error: failed to get value at offset for node (key_name=\"%s\", offset=%d)\n", __func__, key_name, (int) offset);
                return ret;
            }
            if (strncmp(&table->value[offset], data, value_len - offset) != 0) {
                cfstore_error_log("%s:Error: cfstore node value != table node value (key_name=(%s), table value=(%s), cfstore value=(%s))\n", __func__, key_name, table->value, data);
                return ARM_DRIVER_ERROR;
            }
        }
        table++;
        count++;
    }
    return ret;
}
#endif  /* CFSTORE_CPP_THREAD1_RSEEK_TEST */


/** @brief  cfstore client worker thread */
static void cfstore_cpp_thread1_tc_02_worker_thread(CfstoreCppThread *ctx)
{
    int32_t ret = ARM_DRIVER_ERROR;
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    ret = cfstore_cpp_thread1_create_table(cfstore_test_init_1_data, ctx->table_pathname);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s: Error: failed to create KV attributes in cfstore (thrd_id=%d, ret=%d)\n", (int) ctx->id, (int) ret);
        goto out0;
    }
    ret = cfstore.flush();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s: Error: failed to flush KV attributes to NV store (thrd_id=%d, ret=%d)\n", (int) ctx->id, (int) ret);
        goto out0;
    }
out0:
    ctx->status = ret;
    return;
}

/** @brief  find the number of data items in the test data table */
int32_t cfstore_cpp_thread1_table_find_rows(const cfstore_kv_data_t *table)
{
    int32_t count = 0;

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    while (table->key_name != NULL) {
        table++;
        count++;
    }
    return count;
}

/** @brief  cfstore test case to add KV/flush/check KV present,
 *          delete KV/flush/check no KV present.
 *
 * The following test cases collectively perform basic tests of
 * that the C++ wrapper is thread safe. In summary, the test cases
 * do the following:
 * - cfstore_cpp_thread1_case_02: 2 threads working concurrently each add ~20
 *   KV attributes to the cfstore, and then flush the data to flash.
 * - cfstore_cpp_thread1_case_03: 2 threads working concurrently for ~10
 *   KVs call C++ API functions including read/rseek/get_key_name/get_value_len,
 *   then deletes the attributes and flushes to flash.
 * - cfstore_cpp_thread1_case_04: 2 threads working concurrently for ~10
 *   KVs (the ones not deleted in case_03),  call C++ API functions
 *   including read/rseek/get_key_name/get_value_len,
 *   then deletes the attributes and flushes to flash.
 * - cfstore_cpp_thread1_case_05: main thread checks all the KVs
 *   have been successfully deleted.
 *
 * General comments for the test cases:
 * - the main thread is a controller thread
 * - thread1 & thread2 are a worker threads which work concurrently.
 * - thread1/2 are supplied with distinct data tables describing
 *   the KV names, value data to operate upon.
 *
 * cfstore_cpp_thread1_case_02:
 * - main thread initialises cfstore.
 * - the worker thread does the following with their data table:
 *   - adds the KVs in the table
 *   - flushes the data
 *   - terminates the worker threads
 * - main thread uninitialises so the sram memory area is freed.
 *
 * cfstore_cpp_thread1_case_03:
 * - main thread initialises cfstore so data is read from flash
 * - the worker thread does the following with their data table:
 *   1. finds half the entries in the table (not in order)
 *   2. get_key_name() on the key.
 *   3. get_value_len() on the key.
 *   4. read() the value data and check its is as expected.
 *   5. rseek(0)
 *   6. read() the value data and check its is as expected.
 *   7. delete the kv
 *   8. close the kv.
 *   9. once the attributes have been deleted, flushes the data
 *   10. terminates the worker thread
 * - main thread uninitialises so the sram memory area is freed.
 *
 * cfstore_cpp_thread1_case_04:
 * - main thread initialises cfstore so that data is reread from
 *   flash.
 * - the worker thread does the following with their data table:
 *   - finds the other half of the table entries
 *   - repeats steps 2-10 enumerated above.
 * - main thread uninitialises so the sram memory area is freed.
 *
 * cfstore_cpp_thread1_case_05:
 * - main thread initialises cfstore so that data
 *   is reread from flash.
 * - the main thread checks there are no entries in the store.
 * - main thread uninitialises so the sram memory area is freed.
 *
 */
control_t cfstore_cpp_thread1_case_02(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    (void) call_count;

    /* check the size of the data table i.e. must have at least 2 entries */
    ret = cfstore_cpp_thread1_table_find_rows(cfstore_test_init_1_data);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_test_init_1_data too small as must have at least 2 entries (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= 2, cfstore_utest_msg_g);

    /* Initialize CFSTORE before starting worker threads. */
    ret = cfstore.initialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE initialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    CfstoreCppThread *t1 = new CfstoreCppThread(osPriorityNormal, STACK_SIZE);
    CfstoreCppThread *t2 = new CfstoreCppThread(osPriorityNormal, STACK_SIZE);
    t1->modulus = CFSTORE_CPP_THREAD1_MODULUS_ODD;
    t2->modulus = CFSTORE_CPP_THREAD1_MODULUS_EVEN;

    t1->id = CFSTORE_CPP_THREAD1_MODULUS_ODD;
    t2->id = CFSTORE_CPP_THREAD1_MODULUS_EVEN;
    t1->table_pathname = (char *) CFSTORE_CPP_THREAD1_TABLE1_PATHNAME;
    t2->table_pathname = (char *) CFSTORE_CPP_THREAD1_TABLE2_PATHNAME;

    t1->start(t1, cfstore_cpp_thread1_tc_02_worker_thread);
    t2->start(t2, cfstore_cpp_thread1_tc_02_worker_thread);

    /* wait for the worker threads to complete their work*/
    t1->join();
    t2->join();

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_cpp_thread1_tc_02_worker_thread() failed to add attributes (thread_id=%d, status=%d)\n", __func__, (int) t1->id, (int) t1->status);
    TEST_ASSERT_MESSAGE(t1->status >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_cpp_thread1_tc_02_worker_thread() failed to add attributes (thread_id=%d, status=%d)\n", __func__, (int) t2->id, (int) t2->status);
    TEST_ASSERT_MESSAGE(t2->status >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    ret = cfstore_cpp_thread1_table_data_check(cfstore_test_init_1_data, CFSTORE_CPP_THREAD1_TABLE1_PATHNAME);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: data not as expected (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    ret = cfstore_cpp_thread1_table_data_check(cfstore_test_init_1_data, CFSTORE_CPP_THREAD1_TABLE2_PATHNAME);
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: data not as expected (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    ret = cfstore.uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE uninitialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    /* ~Cfstore() is called when object on stack goes out of scope. */
    return CaseNext;
}


/** @brief  cfstore client worker thread */
static void cfstore_cpp_thread1_tc_03_worker_thread(CfstoreCppThread *ctx)
{
    int32_t ret = ARM_DRIVER_ERROR;
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    ret = cfstore_cpp_thread1_subtable_get_key_name(cfstore_test_init_1_data, ctx->table_pathname, ctx->subset);
    if (ret < ARM_DRIVER_OK) {
        cfstore_debug_log("%s: Error: failed to delete KV attributes in cfstore (thrd_id=%d, ret=%d)\n", (int) ctx->id, (int) ret);
        goto out0;
    }
    ret = cfstore_cpp_thread1_subtable_get_value_len(cfstore_test_init_1_data, ctx->table_pathname, ctx->subset);
    if (ret < ARM_DRIVER_OK) {
        cfstore_debug_log("%s: Error: failed to delete KV attributes in cfstore (thrd_id=%d, ret=%d)\n", (int) ctx->id, (int) ret);
        goto out0;
    }
#ifdef CFSTORE_CPP_THREAD1_RSEEK_TEST
    ret = cfstore_cpp_thread1_subtable_rseek(cfstore_test_init_1_data, ctx->table_pathname, ctx->subset);
    if (ret < ARM_DRIVER_OK) {
        cfstore_debug_log("%s: Error: failed to delete KV attributes in cfstore (thrd_id=%d, ret=%d)\n", (int) ctx->id, (int) ret);
        goto out0;
    }
#endif /* CFSTORE_CPP_THREAD1_RSEEK_TEST */
    ret = cfstore_cpp_thread1_subtable_delete(cfstore_test_init_1_data, ctx->table_pathname, ctx->subset);
    if (ret < ARM_DRIVER_OK) {
        cfstore_debug_log("%s: Error: failed to delete KV attributes in cfstore (thrd_id=%d, ret=%d)\n", (int) ctx->id, (int) ret);
        goto out0;
    }
    ret = cfstore.flush();
    if (ret < ARM_DRIVER_OK) {
        cfstore_debug_log("%s: Error: failed to flush KV attributes to NV store (thrd_id=%d, ret=%d)\n", (int) ctx->id, (int) ret);
        goto out0;
    }
out0:
    ctx->status = ret;
    return;
}


/** @brief  main test case 03, 04 support function for deleting a subset
 *          of the cfstore KVs.
 *
 * @param   subset
 *  The parameter <subset> identfies the subset of KVs to be deleted
 *  as identified by the following key name:
 *      <table_key_name>.<subset>.key_name
 *  where table_key_name is specified by the CfstoreCppThread::table_path
 *  and key_name is from the cfstore_test_init_1_data table.
 */
control_t cfstore_cpp_thread1_case_delete_subtable(int32_t subset)
{
    int32_t ret = ARM_DRIVER_ERROR;
    Cfstore cfstore = Cfstore();

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    CfstoreCppThread *t1 = new CfstoreCppThread(osPriorityNormal, STACK_SIZE);
    CfstoreCppThread *t2 = new CfstoreCppThread(osPriorityNormal, STACK_SIZE);

    /* Initialize CFSTORE before starting worker threads. */
    ret = cfstore.initialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE initialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    t1->id = CFSTORE_CPP_THREAD1_MODULUS_ODD;
    t2->id = CFSTORE_CPP_THREAD1_MODULUS_EVEN;
    t1->table_pathname = (char *) CFSTORE_CPP_THREAD1_TABLE1_PATHNAME;
    t2->table_pathname = (char *) CFSTORE_CPP_THREAD1_TABLE2_PATHNAME;
    t1->subset = subset;
    t2->subset = subset;

    t1->start(t1, cfstore_cpp_thread1_tc_03_worker_thread);
    t2->start(t2, cfstore_cpp_thread1_tc_03_worker_thread);

    /* wait for the worker threads to complete their work*/
    t1->join();
    t2->join();

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_cpp_thread1_tc_0x_worker_thread() failed to delete attributes (thread_id=%d, status=%d)\n", __func__, (int) t1->id, (int) t1->status);
    TEST_ASSERT_MESSAGE(t1->status >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: cfstore_cpp_thread1_tc_0x_worker_thread() failed to delete attributes (thread_id=%d, status=%d)\n", __func__, (int) t2->id, (int) t2->status);
    TEST_ASSERT_MESSAGE(t2->status >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    ret = cfstore.uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE uninitialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    /* clean up*/
    delete t1;
    delete t2;

    /* ~Cfstore() is called when object on stack goes out of scope. */
    return CaseNext;
}


/** @brief  cfstore test case to add KV/flush/check KV present,
 *          delete KV/flush/check no KV present.
 *
 * See the overview description under cfstore_cpp_thread1_case_02
 * for  details.
 */
control_t cfstore_cpp_thread1_case_03(const size_t call_count)
{
    (void) call_count;
    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    return cfstore_cpp_thread1_case_delete_subtable(0);
}

/** @brief  cfstore test case to add KV/flush/check KV present,
 *          delete KV/flush/check no KV present.
 *
 * See the overview description under cfstore_cpp_thread1_case_02
 * for  details.
 */
control_t cfstore_cpp_thread1_case_04(const size_t call_count)
{
    (void) call_count;
    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    return cfstore_cpp_thread1_case_delete_subtable(1);
}


/** @brief  cfstore test case to add KV/flush/check KV present,
 *          delete KV/flush/check no KV present.
 *
 * See the test case description under cfstore_cpp_thread1_case_02
 * for details.
 */
control_t cfstore_cpp_thread1_case_05(const size_t call_count)
{
    const char *key_name_query = "*";
    int32_t ret = ARM_DRIVER_ERROR;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;
    uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    (void) call_count;

    /* Initialize CFSTORE before starting worker threads. */
    ret = cfstore.initialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE initialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    memset(key_name, 0, len+1);
    key = cfstore.find(key_name_query, NULL);
    if (key != NULL) {
        key->get_key_name(key_name, &len);
    }
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: Found key when none should be in CFSTORE (key_name=%s)\n", __func__, key_name);
    TEST_ASSERT_MESSAGE(key == NULL, cfstore_utest_msg_g);

    ret = cfstore.uninitialize();
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: CFSTORE uninitialize() failed (%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);

    /* ~Cfstore() is called when object on stack goes out of scope. */
    return CaseNext;
}

/** @brief  test case providing an example of create a key-value attribute
 *          using CFSTORE C++ API
 */
control_t cfstore_cpp_thread1_case_06a(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t counter = 6;
    size_t value_len = 0;
    const char* key_name = "com.arm.mbed.mbedos.cfstore.tests.cpp_thread1.test_case_06.count";
    cfstore_keydesc_t kdesc;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    (void) call_count;

    /* Initialize CFSTORE */
    ret = cfstore.initialize();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.initialize() failed.\n");
        goto out0;
    }
    /* Create a new key-value attribute. The Cfstore() factor generates an CfstoreKey() instance
     * representing the key-value object. */
    memset(&kdesc, 0, sizeof(kdesc));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    value_len = sizeof(counter);
    key = cfstore.create(key_name, value_len, &kdesc);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.create() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out1;
    }
    /* Write key-value attribute */
    ret = key->write((const char*) &counter, &value_len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->write() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    /* Save the key-value attribute to NV store. */
    ret = cfstore.flush();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.flush() failed (ret=%d).\n", (int) ret);
        goto out2;
    }
out2:
    /* close key-value attribute */
    ret = key->close();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->close() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    if (key != NULL) {
        /* Free the CfstoreKey() object generated by the Cfstore() factory. */
        delete key;
    }
out1:
    /* Uninitialize cfstore. */
    ret = cfstore.uninitialize();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.uninitialize() failed (ret=%d).\n", (int) ret);
        goto out0;
    }
out0:
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: test case failed (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);
    /* ~Cfstore() is called when object on stack goes out of scope. */
    return CaseNext;
}

/** @brief  test case providing an example of reading a pre-existing key-value attribute
 *          using CFSTORE C++ API
 */
control_t cfstore_cpp_thread1_case_06b(const size_t call_count)
{
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t counter = 0;
    size_t value_len = 0;
    const char* key_name = "com.arm.mbed.mbedos.cfstore.tests.cpp_thread1.test_case_06.count";
    cfstore_fmode_t flags;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;

    /* cfstore_error_log("%s:debug: entered.\n", __func__); */
    (void) call_count;

    /* Initialize CFSTORE */
    ret = cfstore.initialize();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.initialize() failed (ret=%d).\n", (int) ret);
        goto out0;
    }
    /* Open a pre-existing key-value attribute. The Cfstore() factor generates an CfstoreKey() instance
     * representing the key-value object. */
    memset(&flags, 0, sizeof(flags));
    flags.read = true;
    flags.write = true;
    key = cfstore.open(key_name, flags);
    if (key == NULL) {
        cfstore_error_log("%s:Error: cfstore.open() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out1;
    }
    /* Read the key-value attribute value */
    value_len = sizeof(counter);
    ret = key->read((void*) &counter, &value_len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->read() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    printf("Counter value read from Cfstore is %d\n", (int) counter);
    /* Write key-value attribute */
    counter++;
    ret = key->write((const char*) &counter, &value_len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.write() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    /* Save the key-value attribute to NV store. */
    ret = cfstore.flush();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.flush() failed (ret=%d).\n", (int) ret);
        goto out2;
    }
out2:
    /* Close the open key */
    ret = key->close();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->close() failed  (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    if (key != NULL) {
        delete key;
    }
out1:
    /* Unintialize cfstore. */
    ret = cfstore.uninitialize();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.uninitialize() failed (ret=%d).\n", (int) ret);
        goto out0;
    }
out0:
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: test case failed (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);
    /* ~Cfstore() is called when object on stack goes out of scope. */
    return CaseNext;
}


/** @brief  test case providing an example of finding a pre-existing key-value attribute
 *          using CFSTORE C++ API
 */
control_t cfstore_cpp_thread1_case_06c(const size_t call_count)
{
    uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH;
    const char *key_name_query = "*";
    int32_t ret = ARM_DRIVER_ERROR;
    int32_t counter = 0;
    size_t value_len = 0;
    Cfstore cfstore = Cfstore();
    CfstoreKey *key = NULL;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    const char* expected_key_name = "com.arm.mbed.mbedos.cfstore.tests.cpp_thread1.test_case_06.count";

    /* cfstore_debug_log("%s:debug: entered.\n", __func__); */
    (void) call_count;

    /* Initialize CFSTORE before starting worker threads. */
    ret = cfstore.initialize();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.initialize() failed (ret=%d).\n", (int) ret);
        goto out0;
    }
    /* find the key */
    memset(key_name, 0, len+1);
    key = cfstore.find(key_name_query, NULL);
    if (key == NULL) {
        cfstore_error_log("%s:Error: cfstore.find() failed to find key (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out1;
    }
    /* Get the key name of the key */
    ret = key->get_key_name(key_name, &len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->get_key_name() failed to get key name (ret=%d, key_name=%s).\n", (int) ret, expected_key_name);
        goto out2;
    }
    if (strncmp(key_name, expected_key_name, len) != 0) {
        cfstore_error_log("%s:Error: key_name not as expected (key_name=%s, expected_key_name=%s).\n", key_name, expected_key_name);
        goto out2;
    }
    /* Get the key value field length */
    ret = key->get_value_len(&value_len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->get_value_len() failed to length of value field (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    /* Read the value for the first time */
    value_len = sizeof(counter);
    ret = key->read((void*) &counter, &value_len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->read() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    printf("Counter value read (first time) from Cfstore is %d\n", (int) counter);
    /* rseek(0) to reset KV offset to start of value field so can read the attribute value a 2nd time */
    ret = key->rseek(0);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->rseek() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    /* Read the value for the second time */
    counter = 0;
    value_len = sizeof(counter);
    ret = key->read((void*) &counter, &value_len);
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->read() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    printf("Counter value read (second time) from Cfstore is %d\n", (int) counter);
    /* delete the key-value attribute from the cfstore */
    ret = key->destroy();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->destroy() failed (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
out2:
    /* Close the open key. The close actually results in the KV being deleted from the store
     * as there are as the last CfstoreKey() reference to the KV is closed. */
    ret = key->close();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: key->close() failed  (ret=%d, key_name=%s).\n", (int) ret, key_name);
        goto out2;
    }
    if (key != NULL) {
        delete key;
    }
    /* Persist to NV store the fact that the key has been deleted. */
    ret = cfstore.flush();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.flush() failed (ret=%d).\n", (int) ret);
        goto out1;
    }
out1:
    /* Unintialize cfstore. */
    ret = cfstore.uninitialize();
    if (ret < ARM_DRIVER_OK) {
        cfstore_error_log("%s:Error: cfstore.uninitialize() failed (ret=%d).\n", (int) ret);
        goto out0;
    }
out0:
    CFSTORE_TEST_UTEST_MESSAGE(cfstore_utest_msg_g, CFSTORE_UTEST_MSG_BUF_SIZE, "%s:Error: test case failed (ret=%d)\n", __func__, (int) ret);
    TEST_ASSERT_MESSAGE(ret >= ARM_DRIVER_OK, cfstore_utest_msg_g);
    /* ~Cfstore() is called when object on stack goes out of scope. */
    return CaseNext;
}


/// @cond CFSTORE_DOXYGEN_DISABLE
utest::v1::status_t greentea_setup(const size_t number_of_cases)
{
    GREENTEA_SETUP(CFSTORE_CPP_THREAD1_GREENTEA_TIMEOUT_S, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Case cases[] = {
    /*          1         2         3         4         5         6        7  */
    /* 1234567890123456789012345678901234567890123456789012345678901234567890 */
    Case("CPP_THREAD1_tc_01: 2 worker threads increment shared counter", cfstore_cpp_thread1_case_01),
    Case("CPP_THREAD1_tc_02: 2 worker threads add KVs & flushes", cfstore_cpp_thread1_case_02),
    Case("CPP_THREAD1_tc_03: 2 worker threads delete 1/2 KVs & flushes", cfstore_cpp_thread1_case_03),
    Case("CPP_THREAD1_tc_04: 2 worker threads delete 1/2  KVs & flushes", cfstore_cpp_thread1_case_04),
    Case("CPP_THREAD1_tc_05: 2 main thread check all KVs deleted", cfstore_cpp_thread1_case_05),
    Case("CPP_THREAD1_tc_6a: test case (example) to create new KV in store ", cfstore_cpp_thread1_case_06a),
    Case("CPP_THREAD1_tc_6b: test case (example) to read pre-existing KV in store ", cfstore_cpp_thread1_case_06b),
    Case("CPP_THREAD1_tc_6c: test case (example) to find/delete pre-existing KV in store ", cfstore_cpp_thread1_case_06c),
};


/* Declare your test specification with a custom setup handler */
Specification specification(greentea_setup, cases);

/** @brief  main entry point for test binary */
int main()
{
    return !Harness::run(specification);
}
/// @endcond
