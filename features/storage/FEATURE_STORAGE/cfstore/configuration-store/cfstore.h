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

/** @file  cfstore.h
 * C++ Interface to Configuration Store.
 *
 */

#ifndef CFSTORE_CPP_H_
#define CFSTORE_CPP_H_

#include "rtos.h"
#include "cfstore_debug.h"
#include "cfstore_list.h"
#include "cfstore_types.h"

namespace cfstore {

/* forward declaration for use in */
class Cfstore;

/** @brief   Class representing Configuration Store Key-Value attribute.
 *
 * See the discussion of the Cfstore class for examples of how to use this
 * class.
 *
 * @note
 *  All class methods are synchronous.
 */
class CfstoreKey {
    /** Cfstore is a friend of CfstoreKey so that Cfstore can instantiate
     *  CfstoreKey objects and set private data.
     */
    friend class Cfstore;

public:
    CfstoreKey(void);
    ~CfstoreKey(void);

    /** Close a key-value pair in the store.
     *
     * @return
     *  On success, the function returns ARM_DRIVER_OK (i.e. 0).
     *  On failure, the function returns one of the error codes from:
     *   - ARM_DRIVER_Xxx codes defined in Driver_Storage.h
     *   - ARM_CFSTORE_DRIVER_ERROR_Xxx codes defined in configuration_store.h
     *
     * @note
     *  The following example demonstrates the use of the close(). The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06a() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06a(void)
     * {
     *     int32_t counter = 6;
     *     size_t value_len = 0;
     *     const char* key_name = "com.arm.mbed.count";
     *     cfstore_keydesc_t kdesc;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *
     *     cfstore.initialize();
     *     memset(&kdesc, 0, sizeof(kdesc));
     *     kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
     *     value_len = sizeof(counter);
     *     key = cfstore.create(key_name, value_len, &kdesc);
     *     key->write((const char*) &counter, &value_len);
     *     cfstore.flush();
     *     key->close();
     *     delete key;
     *     cfstore.uninitialize();
     *     return 0;
     * }
     *
     */
    int32_t close(void);

    /** Delete a key-value pair from the store.
     *
     * @return
     *  On success, the function returns ARM_DRIVER_OK (i.e. 0).
     *  On failure, the function returns one of the error codes from:
     *   - ARM_DRIVER_Xxx codes defined in Driver_Storage.h
     *   - ARM_CFSTORE_DRIVER_ERROR_Xxx codes defined in configuration_store.h
     */
    int32_t destroy(void);

    /** Get the key name of a key-value pair in the store.
     *
     * @param   key_name
     *  On input, this is an empty data buffer used to receive the KV key name.
     *  On successful return, this will return the key-name of the KV attribute.
     *  The maximum length of the key-name is CFSTORE_KEY_NAME_MAX_LENGTH chars.
     *
     * @param   key_len
     *  On input, this is the length of the buffer supplied at key_name.
     *  On successful return, it contains the number of characters written to the
     *  supplied buffer.
     *
     * @return
     *  On success, the function returns ARM_DRIVER_OK (i.e. 0).
     *  On failure, the function returns one of the error codes from:
     *   - ARM_DRIVER_Xxx codes defined in Driver_Storage.h
     *   - ARM_CFSTORE_DRIVER_ERROR_Xxx codes defined in configuration_store.h
     *
     * @note
     *  The following example demonstrates the use of the get_key_name() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06c() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06c(void)
     * {
     *     uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH;
     *     const char *key_name_query = "*";
     *     int32_t counter = 0;
     *     size_t value_len = 0;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *     char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
     *
     *     cfstore.initialize();
     *     memset(key_name, 0, len+1);
     *     key = cfstore.find(key_name_query, NULL);
     *     key->get_key_name(key_name, &len);
     *     printf("key_name=%s\n", key_name);
     *     key->get_value_len(&value_len);
     *     // Read the value for the first time
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (first time) from Cfstore is %d\n", (int) counter);
     *     // rseek(0) to reset KV offset to start of value field so can read the attribute value a 2nd time
     *     key->rseek(0);
     *     // Read the value for the second time
     *     counter = 0;
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (second time) from Cfstore is %d\n", (int) counter);
     *     // delete the key-value attribute from the cfstore
     *     key->destroy();
     *     key->close();
     *     delete key;
     *     cfstore.flush();
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t get_key_name(char *key_name, uint8_t *key_len);

    /** Get the length of a key-value value.
     *
     * @param   value_len
     *  On input, this is a pointer to a empty variable to take the
     *  On successful return, the supplied input variable of type size_t
     *  contains the size in octets of the KV value.
     *
     * @return
     *  On success, the function returns ARM_DRIVER_OK (i.e. 0).
     *  On failure, the function returns one of the error codes from:
     *   - ARM_DRIVER_Xxx codes defined in Driver_Storage.h
     *   - ARM_CFSTORE_DRIVER_ERROR_Xxx codes defined in configuration_store.h
     *
     * @note
     *  The following example demonstrates the use of the get_value_len() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06c() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06c(void)
     * {
     *     uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH;
     *     const char *key_name_query = "*";
     *     int32_t counter = 0;
     *     size_t value_len = 0;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *     char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
     *
     *     cfstore.initialize();
     *     memset(key_name, 0, len+1);
     *     key = cfstore.find(key_name_query, NULL);
     *     key->get_key_name(key_name, &len);
     *     printf("key_name=%s\n", key_name);
     *     key->get_value_len(&value_len);
     *     // Read the value for the first time
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (first time) from Cfstore is %d\n", (int) counter);
     *     // rseek(0) to reset KV offset to start of value field so can read the attribute value a 2nd time
     *     key->rseek(0);
     *     // Read the value for the second time
     *     counter = 0;
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (second time) from Cfstore is %d\n", (int) counter);
     *     // delete the key-value attribute from the cfstore
     *     key->destroy();
     *     key->close();
     *     delete key;
     *     cfstore.flush();
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t get_value_len(size_t *value_len);

    /** Read the value field of the key-value attribute.
     *
     * @param   data
     *  On input, this is an empty data buffer used to receive the KV value data.
     *  The read operation attempts to read up to len octets from the KVs value
     *  field into the supplied data buffer. The read supports seeking i.e.
     *  the read operation is performed relative to the current KV offset, and
     *  the KV offset is incremented by the number of octets read. If the
     *  current KV offset is at or beyond the end of the value field, no octets
     *  will be read and read() will return 0.
     *
     * @param   len
     *  On input, this is the length of the buffer supplied at key_name.
     *  On successful return, it contains the number of characters written to the
     *  supplied buffer.
     *
     * @return
     *  On success, the function the number of octets read from the KV value field.
     *  len is also set to be the number of octets read from the KV value field.
     *  On failure, the function returns one of the error codes from:
     *   - ARM_DRIVER_Xxx codes defined in Driver_Storage.h
     *   - ARM_CFSTORE_DRIVER_ERROR_Xxx codes defined in configuration_store.h
     *
     *
     * @note
     *  The following example demonstrates the use of the read() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06b() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06b(void)
     * {
     *     int32_t counter = 0;
     *     size_t value_len = 0;
     *     const char* key_name = "com.arm.mbed.count";
     *     cfstore_fmode_t flags;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *
     *     cfstore.initialize();
     *     memset(&flags, 0, sizeof(flags));
     *     flags.read = true;
     *     flags.write = true;
     *     key = cfstore.open(key_name, flags);
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read from Cfstore is %d\n", (int) counter);
     *     key->close();
     *     delete key;
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t read(void *data, size_t *len);

    /** Set the KV offset position for reading.
     *
     * @param   offset
     *  On success, the rseek() function repositions the KV offset to the value
     *  specified by offset. If offset is greater than the size of the KV value
     *  field then it will be set to the length of the value.
     *
     * @return
     *  On success, the function returns the current KV offset location measured
     *  from the beginning of the value field.
     *  On failure, the function returns one of the error codes from:
     *   - ARM_DRIVER_Xxx codes defined in Driver_Storage.h
     *   - ARM_CFSTORE_DRIVER_ERROR_Xxx codes defined in configuration_store.h.
     *
     * @note
     *  The following example demonstrates the use of the rseek() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06c() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06c(void)
     * {
     *     uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH;
     *     const char *key_name_query = "*";
     *     int32_t counter = 0;
     *     size_t value_len = 0;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *     char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
     *
     *     cfstore.initialize();
     *     memset(key_name, 0, len+1);
     *     key = cfstore.find(key_name_query, NULL);
     *     key->get_key_name(key_name, &len);
     *     printf("key_name=%s\n", key_name);
     *     key->get_value_len(&value_len);
     *     // Read the value for the first time
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (first time) from Cfstore is %d\n", (int) counter);
     *     // rseek(0) to reset KV offset to start of value field so can read the attribute value a 2nd time
     *     key->rseek(0);
     *     // Read the value for the second time
     *     counter = 0;
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (second time) from Cfstore is %d\n", (int) counter);
     *     // delete the key-value attribute from the cfstore
     *     key->destroy();
     *     key->close();
     *     delete key;
     *     cfstore.flush();
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t rseek(size_t offset);

    /** Write the KV value field.
     *
     * @param   data
     *  On input, this is an data buffer containing the data to write to the KV value
     *  field. The write operation attempts to write up to len octets to the KVs value
     *  field. The write does not support seeking i.e.
     *  the write operation is always performed relative to start of the value field.
     *
     * @param   len
     *  On input, this is the requested number of octets that should be written from the
     *  supplied data buffer to the KV value field.
     *  On successful return, this argument contains the number of octets successfully
     *  written to the KV value field, which may be less than the requested number e.g.
     *  because the requested length was larger than the value field length.
     *
     * @return
     *  On success, the function returns the number of octets successfully written
     *  to the value field. Note this may be less than the size of the write requested.
     *  On failure, the function returns one of the error codes from:
     *   - ARM_DRIVER_Xxx codes defined in Driver_Storage.h
     *   - ARM_CFSTORE_DRIVER_ERROR_Xxx codes defined in configuration_store.h
     *
     * @note
     *  The following example demonstrates the use of the write() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06a() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06a(void)
     * {
     *     int32_t counter = 6;
     *     size_t value_len = 0;
     *     const char* key_name = "com.arm.mbed.count";
     *     cfstore_keydesc_t kdesc;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *
     *     cfstore.initialize();
     *     memset(&kdesc, 0, sizeof(kdesc));
     *     kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
     *     value_len = sizeof(counter);
     *     key = cfstore.create(key_name, value_len, &kdesc);
     *     key->write((const char*) &counter, &value_len);
     *     cfstore.flush();
     *     key->close();
     *     delete key;
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t write(const char *data, size_t *len);

private:

    /**
     * @param   hkey
     *  This is insecure storage in the caller box. Cfstore stores the pointer to
     *  the cfstore secured storage in this buffer, which is opaque to the caller.
     *  Should the caller dereference the pointer, uvisor which create a
     *  protection fault.
     */
    uint8_t hkey[CFSTORE_HANDLE_BUFSIZE];   //!< Insecure key handle storage. Cfstore stores the pointer to
                                            //!< the cfstore secured storage in this buffer, which is opaque to the caller.
                                            //!< Should the caller dereference the opaque pointer, uvisor will generate a
                                            //!< protection fault.
    Cfstore *owner;                         //!< Pointer to cfstore that created this key.
                                            //!< This is required so key can lock transactions.
};

/** @brief   Class representing interface to Configuration Store
 */
class Cfstore
{
    friend class CfstoreKey;
public:
    Cfstore(void);
    ~Cfstore(void);

    /** Create a key-value pair in the store.
     *
     * @param key_name      The name of the key.
     * @param value_len     The size in bytes of the stored value.
     * @param kdesc         The key descriptor specifying the properties of the key-value.
     *
     * @return
     *  On success, the method returns a valid pointer to a CfstoreKey object through
     *  which the KV can be manipulated. On failure, the method returns NULL;
     *
     * @note
     *  The following example demonstrates the use of the create() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06a() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06a(void)
     * {
     *     int32_t counter = 6;
     *     size_t value_len = 0;
     *     const char* key_name = "com.arm.mbed.count";
     *     cfstore_keydesc_t kdesc;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *
     *     cfstore.initialize();
     *     memset(&kdesc, 0, sizeof(kdesc));
     *     kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
     *     value_len = sizeof(counter);
     *     key = cfstore.create(key_name, value_len, &kdesc);
     *     key->write((const char*) &counter, &value_len);
     *     cfstore.flush();
     *     key->close();
     *     delete key;
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    CfstoreKey* create(const char *key_name, size_t value_len, const cfstore_keydesc_t *kdesc);

    /** Find a key-value pair matching a query string.
     *
     * @param key_name_query    The key name query string (can include the wildcard '*').
     * @param kv_previous       Pointer to previously returned CfstoreKey object (used as hint to find next matching KV).
     *
     * @return
     *  On success, the method returns a valid pointer to a CfstoreKey object matching the query string.
     *  On failure, the method returns NULL;
     *
     * @note
     *  The following example demonstrates the use of the find() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06c() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06c(void)
     * {
     *     uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH;
     *     const char *key_name_query = "*";
     *     int32_t counter = 0;
     *     size_t value_len = 0;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *     char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
     *
     *     cfstore.initialize();
     *     memset(key_name, 0, len+1);
     *     key = cfstore.find(key_name_query, NULL);
     *     key->get_key_name(key_name, &len);
     *     printf("key_name=%s\n", key_name);
     *     key->get_value_len(&value_len);
     *     // Read the value for the first time
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (first time) from Cfstore is %d\n", (int) counter);
     *     // rseek(0) to reset KV offset to start of value field so can read the attribute value a 2nd time
     *     key->rseek(0);
     *     // Read the value for the second time
     *     counter = 0;
     *     value_len = sizeof(counter);
     *     key->read((void*) &counter, &value_len);
     *     printf("Counter value read (second time) from Cfstore is %d\n", (int) counter);
     *     // delete the key-value attribute from the cfstore
     *     key->destroy();
     *     key->close();
     *     delete key;
     *     cfstore.flush();
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    CfstoreKey *find(const char *key_name_query, CfstoreKey *kv_previous);

    /** Flush (commit) the key-values stored in memory to non-volatile backing store (flash).
     *
     * @return
     *  On success, the method returns a status code >= 0. Otherwise, as
     *  status code < 0 is returned.
     *
     * @note
     *  The following example demonstrates the use of the flush() method. The code
     *  is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06a() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06a(void)
     * {
     *     int32_t counter = 6;
     *     size_t value_len = 0;
     *     const char* key_name = "com.arm.mbed.count";
     *     cfstore_keydesc_t kdesc;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *
     *     cfstore.initialize();
     *     memset(&kdesc, 0, sizeof(kdesc));
     *     kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
     *     value_len = sizeof(counter);
     *     key = cfstore.create(key_name, value_len, &kdesc);
     *     key->write((const char*) &counter, &value_len);
     *     cfstore.flush();
     *     key->close();
     *     delete key;
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t flush(void);

    /** Initialize the configuration store.
     *
     * @return
     *  On success, the method returns a status code >= 0. Otherwise, as
     *  status code < 0 is returned.
     *
     * @note
     *  The following example demonstrates the use of the initialize() method.
     *  The code is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06a() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06a(void)
     * {
     *     int32_t counter = 6;
     *     size_t value_len = 0;
     *     const char* key_name = "com.arm.mbed.count";
     *     cfstore_keydesc_t kdesc;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *
     *     cfstore.initialize();
     *     memset(&kdesc, 0, sizeof(kdesc));
     *     kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
     *     value_len = sizeof(counter);
     *     key = cfstore.create(key_name, value_len, &kdesc);
     *     key->write((const char*) &counter, &value_len);
     *     cfstore.flush();
     *     key->close();
     *     delete key;
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t initialize(void);

    /** Open a pre-existing key-value pair in the store.
     *
     * @param key_name  The name of the key.
     * @param flags     The flags describing the required permissions, for example.
     *
     * @return
     *  On success, the method returns a status code >= 0. Otherwise, as
     *  status code < 0 is returned.
     */
    CfstoreKey *open(const char *key_name, cfstore_fmode_t flags);

    /** Uninitialize the configuration store.
     *
     * @return
     *  On success, the method returns a valid pointer to a CfstoreKey object through
     *  which the KV can be manipulated. On failure, the method returns NULL;
     *
     * @note
     *  The following example demonstrates the use of the uninitialize()
     *  method. The code is a simplified version of the
     *  cpp_thread1.cpp::cfstore_cpp_thread1_case_06a() test case code (error
     *  handling removed).
     *
     * int32_t cfstore_cpp_thread1_case_06a(void)
     * {
     *     int32_t counter = 6;
     *     size_t value_len = 0;
     *     const char* key_name = "com.arm.mbed.count";
     *     cfstore_keydesc_t kdesc;
     *     Cfstore cfstore = Cfstore();
     *     CfstoreKey *key = NULL;
     *
     *     cfstore.initialize();
     *     memset(&kdesc, 0, sizeof(kdesc));
     *     kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
     *     value_len = sizeof(counter);
     *     key = cfstore.create(key_name, value_len, &kdesc);
     *     key->write((const char*) &counter, &value_len);
     *     cfstore.flush();
     *     key->close();
     *     delete key;
     *     cfstore.uninitialize();
     *     return 0;
     * }
     */
    int32_t uninitialize(void);

private:

    static int32_t initialised_count;           //!< count of the number of clients instantiating Cfstore objects.
    static Mutex mtx_lock;                      //!< Protects initialised_count, and other data items.
    static cfstore_driver_t *driver;            //!< Underlying cfstore C-HAL driver.
    static cfstore_driver_version_t version;    //!< Underlying version of the cfstore C-HAL driver.
    static cfstore_capabilities_t caps;         //!< Underlying capabilities of the cfstore C-HAL driver.
    static int32_t key_count;                   //!< count of the active CfstoreKey() instances created

};

void cfstore_debug_log(const char *fmt, ...);   //!< multithreaded debug logging
void cfstore_error_log(const char *fmt, ...);   //!< multithreaded error logging


}	/* namespace cfstore */

#endif /* CFSTORE_CPP_H_ */
