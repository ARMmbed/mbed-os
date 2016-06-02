/* @file cfstore_test.c
 *
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
 * test support code implementation file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "cfstore_config.h"
#include "cfstore_debug.h"
#include "cfstore_test.h"
#include <configuration-store/configuration_store.h>


/* ruler for measuring text strings */
/*                                                                                                    1         1         1         1         1         1         1         1         1         1         2         2         2 */
/* 0        1         2         3         4         5         6         7         8         9         0         1         2         3         4         5         6         7         8         9         0         1         2 */
/* 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890 */

const uint8_t cfstore_test_byte_data_table[CFSTORE_TEST_BYTE_DATA_TABLE_SIZE] =
{
    0x00U, 0x07U, 0x0EU, 0x09U, 0x1CU, 0x1BU, 0x12U, 0x15U,
    0x38U, 0x3FU, 0x36U, 0x31U, 0x24U, 0x23U, 0x2AU, 0x2DU,
    0x70U, 0x77U, 0x7EU, 0x79U, 0x6CU, 0x6BU, 0x62U, 0x65U,
    0x48U, 0x4FU, 0x46U, 0x41U, 0x54U, 0x53U, 0x5AU, 0x5DU,
    0xE0U, 0xE7U, 0xEEU, 0xE9U, 0xFCU, 0xFBU, 0xF2U, 0xF5U,
    0xD8U, 0xDFU, 0xD6U, 0xD1U, 0xC4U, 0xC3U, 0xCAU, 0xCDU,
    0x90U, 0x97U, 0x9EU, 0x99U, 0x8CU, 0x8BU, 0x82U, 0x85U,
    0xA8U, 0xAFU, 0xA6U, 0xA1U, 0xB4U, 0xB3U, 0xBAU, 0xBDU,
    0xC7U, 0xC0U, 0xC9U, 0xCEU, 0xDBU, 0xDCU, 0xD5U, 0xD2U,
    0xFFU, 0xF8U, 0xF1U, 0xF6U, 0xE3U, 0xE4U, 0xEDU, 0xEAU,
    0xB7U, 0xB0U, 0xB9U, 0xBEU, 0xABU, 0xACU, 0xA5U, 0xA2U,
    0x8FU, 0x88U, 0x81U, 0x86U, 0x93U, 0x94U, 0x9DU, 0x9AU,
    0x27U, 0x20U, 0x29U, 0x2EU, 0x3BU, 0x3CU, 0x35U, 0x32U,
    0x1FU, 0x18U, 0x11U, 0x16U, 0x03U, 0x04U, 0x0DU, 0x0AU,
    0x57U, 0x50U, 0x59U, 0x5EU, 0x4BU, 0x4CU, 0x45U, 0x42U,
    0x6FU, 0x68U, 0x61U, 0x66U, 0x73U, 0x74U, 0x7DU, 0x7AU,
    0x89U, 0x8EU, 0x87U, 0x80U, 0x95U, 0x92U, 0x9BU, 0x9CU,
    0xB1U, 0xB6U, 0xBFU, 0xB8U, 0xADU, 0xAAU, 0xA3U, 0xA4U,
    0xF9U, 0xFEU, 0xF7U, 0xF0U, 0xE5U, 0xE2U, 0xEBU, 0xECU,
    0xC1U, 0xC6U, 0xCFU, 0xC8U, 0xDDU, 0xDAU, 0xD3U, 0xD4U,
    0x69U, 0x6EU, 0x67U, 0x60U, 0x75U, 0x72U, 0x7BU, 0x7CU,
    0x51U, 0x56U, 0x5FU, 0x58U, 0x4DU, 0x4AU, 0x43U, 0x44U,
    0x19U, 0x1EU, 0x17U, 0x10U, 0x05U, 0x02U, 0x0BU, 0x0CU,
    0x21U, 0x26U, 0x2FU, 0x28U, 0x3DU, 0x3AU, 0x33U, 0x34U,
    0x4EU, 0x49U, 0x40U, 0x47U, 0x52U, 0x55U, 0x5CU, 0x5BU,
    0x76U, 0x71U, 0x78U, 0x7FU, 0x6AU, 0x6DU, 0x64U, 0x63U,
    0x3EU, 0x39U, 0x30U, 0x37U, 0x22U, 0x25U, 0x2CU, 0x2BU,
    0x06U, 0x01U, 0x08U, 0x0FU, 0x1AU, 0x1DU, 0x14U, 0x13U,
    0xAEU, 0xA9U, 0xA0U, 0xA7U, 0xB2U, 0xB5U, 0xBCU, 0xBBU,
    0x96U, 0x91U, 0x98U, 0x9FU, 0x8AU, 0x8DU, 0x84U, 0x83U,
    0xDEU, 0xD9U, 0xD0U, 0xD7U, 0xC2U, 0xC5U, 0xCCU, 0xCBU,
    0xE6U, 0xE1U, 0xE8U, 0xEFU, 0xFAU, 0xFDU, 0xF4U, 0xF3U
};

/* @brief   set of test data for sequential write tests */
cfstore_test_rw_data_entry_t cfstore_test_rw_data_table[] =
{
        { 25, 'z' },
        { 00, 'a' },
        { 24, 'y' },
        { 01, 'b' },
        { 23, 'x' },
        { 02, 'c' },
        { 22, 'w' },
        { 03, 'd' },
        { 21, 'v' },
        { 04, 'e' },
        { 20, 'u' },
        { 05, 'f' },
        { 19, 't' },
        { 06, 'g' },
        { 18, 's' },
        { 07, 'h' },
        { 17, 'r' },
        {  8, 'i' },
        { 16, 'q' },
        {  9, 'j' },
        { 15, 'p' },
        { 10, 'k' },
        { 14, 'o' },
        { 11, 'l' },
        { 13, 'n' },
        { 12, 'm' },
        { CFSTORE_TEST_RW_TABLE_SENTINEL, '@' },
};

const char* cfstore_test_opcode_str[] =
{
    "UNDEFINED",
    "CFSTORE_OPCODE_CLOSE",
    "CFSTORE_OPCODE_CREATE",
    "CFSTORE_OPCODE_DELETE",
    "CFSTORE_OPCODE_FIND",
    "CFSTORE_OPCODE_FLUSH",
    "CFSTORE_OPCODE_GET_KEY_NAME",
    "CFSTORE_OPCODE_GET_STATUS",
    "CFSTORE_OPCODE_GET_VALUE_LEN",
    "CFSTORE_OPCODE_INITIALIZE",
    "CFSTORE_OPCODE_OPEN",
    "CFSTORE_OPCODE_POWER_CONTROL",
    "CFSTORE_OPCODE_READ",
    "CFSTORE_OPCODE_RSEEK",
    "CFSTORE_OPCODE_UNINITIALIZE",
    "CFSTORE_OPCODE_WRITE",
    "CFSTORE_OPCODE_MAX"
};
/* @brief   test utility function to check a node appears correctly in the cfstore
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_check_node_correct(const cfstore_kv_data_t* node)
{
    char* read_buf;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    memset(&flags, 0, sizeof(flags));

    ret = drv->Open(node->key_name, flags, hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to open node (key_name=\"%s\", value=\"%s\")(ret=%d)\r\n", __func__, node->key_name, node->value, (int) ret);
        goto out0;
    }
    len = strlen(node->value) + 1;
    read_buf = (char*) malloc(len);
    if(read_buf == NULL) {
        CFSTORE_ERRLOG("%s:Error: failed to allocated read buffer \r\n", __func__);
        goto out1;
    }
    memset(read_buf, 0, len);
    ret = drv->Read(hkey, read_buf, &len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to write key (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        goto out2;
    }
    /* check read data is as expected */
    if(strncmp(read_buf, node->value, strlen(node->value)) != 0){
        CFSTORE_ERRLOG("%s:Error: read value data (%s) != KV value data (key_name=\"%s\", value=\"%s\")\r\n", __func__, read_buf, node->key_name, node->value);
        ret = ARM_DRIVER_ERROR;
    }
out2:
    if(read_buf) free(read_buf);
out1:
    drv->Close(hkey);
    hkey = NULL;
out0:
    return ret;
}


/* @brief  test utility function to delete the cfstore key identified by key_name
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_delete(const char* key_name)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered.\r\n", __func__);
    memset(&flags, 0, sizeof(flags));
    ret = drv->Open(key_name, flags, hkey);
    if(ret < ARM_DRIVER_OK){
        return ret;
    }
    if(hkey != NULL){
        ret = drv->Delete(hkey);
        drv->Close(hkey);
    }
    return ret;
}

/* @brief   test utility function to delete all of the KVs in the cfstore
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_delete_all(void)
{
    const char* key_name_query = "*";
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(next);
    ARM_CFSTORE_HANDLE_INIT(prev);

    CFSTORE_FENTRYLOG("%s:entered.\r\n", __func__);
    while((ret = drv->Find(key_name_query, prev, next)) == ARM_DRIVER_OK)
    {
        len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        drv->GetKeyName(next, key_name, &len);
        CFSTORE_TP(CFSTORE_TP_DELETE, "%s:deleting key_name=%s, len=%d\r\n", __func__, key_name, (int) len);
        ret = drv->Delete(next);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: failed to delete key_name=%s, len=%d\r\n", __func__, key_name, (int) len);
            return ret;
        }
        CFSTORE_HANDLE_SWAP(prev, next);
    }
    if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
        /* as expected, no more keys have been found by the Find()*/
        ret = ARM_DRIVER_OK;
    }
    // todo: find portable format specification CFSTORE_FENTRYLOG("%s:exiting (ret=%ld).\r\n", __func__, ret);
    CFSTORE_FENTRYLOG("%s:exiting (ret=%" PRId32 ").\r\n", __func__, ret);
    return ret;
}


/* @brief   test utility function to create a KV in the cfstore
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_create(const char* key_name, const char* data, ARM_CFSTORE_SIZE* len, ARM_CFSTORE_KEYDESC* kdesc)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE value_len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(hkey);

    CFSTORE_FENTRYLOG("%s:entered.\r\n", __func__);
    value_len = *len;
    kdesc->drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    ret = drv->Create(key_name, value_len, kdesc, hkey);
    if(ret < ARM_DRIVER_OK){
        return ret;
    }
    value_len = *len;
    ret = drv->Write(hkey, data, &value_len);
    if(ret < ARM_DRIVER_OK){
        drv->Close(hkey);
        return ret;
    }
    if(value_len != *len){
        drv->Close(hkey);
        return ARM_DRIVER_ERROR;
    }
    drv->Close(hkey);
    return ret;
}

/* @brief   test utility function to create KVs from the supplied table
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_create_table(const cfstore_kv_data_t* table)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    cfstore_kv_data_t* node = NULL;
    ARM_CFSTORE_KEYDESC kdesc;

    CFSTORE_FENTRYLOG("%s:entered.\r\n", __func__);
    memset(&kdesc, 0, sizeof(kdesc));
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    while(table->key_name != NULL)
    {
        len = strlen(table->value);
        ret = cfstore_test_create(table->key_name, table->value, &len, &kdesc);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: failed to create node (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
            return ret;
        }
        table++;
    }
    return ret;
}

cfstore_kv_data_t cfstore_test_init_1_data[] = {
        CFSTORE_INIT_1_TABLE_HEAD,
        { "b", "1"},
        { "c", "12"},
        { "d", "123"},
        { "e", "1234"},
        { "g", "12345"},
        { "h", "123456"},
        { "i", "1234567"},
        { "j", "12345678"},
        { "k", "123456789"},
        { "l", "1234567890"},
        { "m", "12345678901"},
        { "n", "123456789012"},
        { "o", "1234567890123"},
        { "p", "12345678901234"},
        { "q", "123456789012345"},
        { "r", "1234567890123456"},
        { "0", "a"},
        { "01", "ab"},
        { "012", "abc"},
        { "0123", "abcd"},
        { "01234", "abcde"},
        { "012345", "abcdef"},
        { "0123456", "abcdefg"},
        { "01234567", "abcdefgh"},
        { "012345678", "abcdefghi"},
        { "0123456789", "abcdefghj"},
        { "0123456789a", "abcdefghjk"},
        { "0123456789ab", "abcdefghjkl"},
        { "0123456789abc", "abcdefghjklm"},
        { "0123456789abcd", "abcdefghjklmn"},
        { "0123456789abcde", "abcdefghjklmno"},
        { "0123456789abcdef", "abcdefghjklmnop"},
        { "0123456789abcdef0", "abcdefghjklmnopq"},
        { "0123456789abcdef01", "abcdefghjklmnopqr"},
        { "0123456789abcdef012", "abcdefghjklmnopqrs"},
        { "0123456789abcdef0123", "abcdefghjklmnopqrst"},
        { "0123456789abcdef01234", "abcdefghjklmnopqrstu"},
        { "0123456789abcdef012345", "abcdefghjklmnopqrstuv"},
        CFSTORE_INIT_1_TABLE_MID_NODE,
        { "0123456789abcdef01234567", "abcdefghjklmnopqrstuvwx"},
        { "0123456789abcdef012345678", "abcdefghjklmnopqrstuvwxy"},
        { "0123456789abcdef0123456789", "abcdefghjklmnopqrstuvwxyz"},
        { "0123456789abcdef0123456789a", "b"},
        { "0123456789abcdef0123456789ab", "c"},
        { "0123456789abcdef0123456789abc", "d"},
        { "0123456789abcdef0123456789abcd", "e"},
        { "0123456789abcdef0123456789abcde", "f"},
        { "0123456789abcdef0123456789abcdef", "g"},
        { "com.arm.mbed.wifi.accesspoint.essid", ""},
        { "com.arm.mbed.wifi.accesspoint.essid2", ""},
        { "yotta.your-yotta-registry-module-name.module1", ""},
        { "yotta.hello-world.animal{wobbly-dog}{foot}frontLeft", "missing"},
        { "yotta.hello-world.animal{wobbly-dog}{foot}frontRight", "present"},
        { "yotta.hello-world.animal{wobbly-dog}{foot}backLeft", "half present"},
        { "piety.demands.us.to.honour.truth.above.our.friends", "Aristotle"},
        { "basement.medicine.pavement.government.trenchcoat.off.cough.off.kid.did.when.again.alleyway.friend.cap.pen.dollarbills.ten.foot.soot.put.but.anyway.say.May.DA.kid.did.toes.bows.those.hose.nose.clothes.man.blows.well.well", "TheRollingStone" },
        CFSTORE_INIT_1_TABLE_TAIL,
        { NULL, NULL},
};


/* @brief   utility test function to initialise cfstore sram area with some
 *          KV's to manipulate
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_init_1(void)
{
    char* read_buf = NULL;
    const uint8_t key_name_max_len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    uint8_t key_name_len = 0;
    char key_name_buf[key_name_max_len];
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_SIZE len = 0;
    ARM_CFSTORE_SIZE max_len = 0;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    cfstore_kv_data_t* node = NULL;
    ARM_CFSTORE_KEYDESC kdesc;
    ARM_CFSTORE_HANDLE_INIT(hkey);

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    memset(&kdesc, 0, sizeof(kdesc));
    memset(key_name_buf, 0, CFSTORE_KEY_NAME_MAX_LENGTH+1);

    /*scan for max length of value blob*/
    node = cfstore_test_init_1_data;
    while(node->key_name != NULL)
    {
        len = strlen(node->value);
        if(len > max_len){
            max_len = len;
            max_len++;
        }
        node++;
    }
    read_buf = (char*) malloc(max_len);
    if(read_buf == NULL) {
        CFSTORE_ERRLOG("%s:Error: failed to allocated read buffer \r\n", __func__);
        return ret;
    }
    kdesc.drl = ARM_RETENTION_WHILE_DEVICE_ACTIVE;
    node = cfstore_test_init_1_data;
    while(node->key_name != NULL)
    {
        CFSTORE_DBGLOG("%s:About to create new node (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
        ret = drv->Create(node->key_name, strlen(node->value), &kdesc, hkey);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: failed to create node (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
            return ret;
        }

        CFSTORE_DBGLOG("%s:length of KV=%d (key_name=\"%s\", value=\"%s\")\r\n", __func__, (int) len, node->key_name, node->value);
        len = strlen(node->value);
        ret = drv->Write(hkey, (char*) node->value, &len);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: failed to write key (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
            drv->Close(hkey);
            return ret;
        }
        if(len != strlen(node->value)){
            CFSTORE_ERRLOG("%s:Error: failed to write full value data (key_name=\"%s\", value=\"%s\"), len=%d\r\n", __func__, node->key_name, node->value, (int) len);
            drv->Close(hkey);
            return ARM_DRIVER_ERROR;
        }
        /* read the data back*/
        len = strlen(node->value);
        memset(read_buf, 0, max_len);
        ret = drv->Read(hkey, read_buf, &len);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("%s:Error: failed to read key (key_name=\"%s\", value=\"%s\")\r\n", __func__, node->key_name, node->value);
            drv->Close(hkey);
            return ret;
        }
        if(len != strlen(node->value)){
            CFSTORE_ERRLOG("%s:Error: failed to read full value data (key_name=\"%s\", value=\"%s\"), len=%d, ret=%d\r\n", __func__, node->key_name, node->value, (int) len, (int) ret);
            drv->Close(hkey);
            return ARM_DRIVER_ERROR;
        }
        key_name_len = key_name_max_len;
        memset(key_name_buf, 0, key_name_len);
        drv->GetKeyName(hkey, key_name_buf, &key_name_len);
        if(len != strlen(node->value)){
            CFSTORE_ERRLOG("%s:Error: failed to GetKeyName() (key_name=\"%s\", value=\"%s\"), len=%d\r\n", __func__, node->key_name, node->value, (int) len);
            drv->Close(hkey);
            return ARM_DRIVER_ERROR;
        }
        CFSTORE_LOG("Created KV successfully (key_name=\"%s\", value=\"%s\")\r\n", key_name_buf, read_buf);
        drv->Close(hkey);
        node++;
    }
    free(read_buf);
    return ret;
}

/* @brief   test utility function to check a particular KV exists in the
 *          cfstore using Find() interface
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_kv_is_found(const char* key_name, bool* bfound)
{
    CFSTORE_FENTRYLOG("%s:entered.\r\n", __func__);
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_HANDLE_INIT(prev);
    ARM_CFSTORE_HANDLE_INIT(next);
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;

    CFSTORE_ASSERT(bfound != NULL);
    CFSTORE_ASSERT(key_name != NULL);
    *bfound = 0;

    ret = drv->Find(key_name, prev, next);
    if(ret == ARM_DRIVER_OK){
        *bfound = 1;
        CFSTORE_DBGLOG("%s:Found key_name=\"%s\", about to call close.\r\n", __func__, key_name);
        drv->Close(next);
    }
    return ret;
}


/* @brief   support function for generating a kv_name
 * @param   name    buffer to hold kv name
 * @param   len     length of kv name to generate
 * @note	braces are not included in the generated names as the names are
 *          of varible length and theyre may be unmatched
 *
 */
#define CFSTORE_TEST_KV_NAME_BUF_MAX_DATA (10+26+26+4)
int32_t cfstore_test_kv_name_gen(char* name, const size_t len)
{
    size_t i;
    const char buf[CFSTORE_TEST_KV_NAME_BUF_MAX_DATA+1] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-_@";
    CFSTORE_FENTRYLOG("%s:entered\n", __func__);
    for(i = 0; i < len; i++)
    {
        name[i] = buf[i % CFSTORE_TEST_KV_NAME_BUF_MAX_DATA];
    }
    return ARM_DRIVER_OK;
}

/* @brief   test utility function to read the value blob of a specified KV
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_read(const char* key_name, char* data, ARM_CFSTORE_SIZE* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    memset(&flags, 0, sizeof(flags));
    if(key_name == NULL) {
        CFSTORE_ERRLOG("%s:invalid key_name argument \r\n", __func__);
        goto out0;
    }
    if(data == NULL) {
        CFSTORE_ERRLOG("%s:invalid data argument \r\n", __func__);
        goto out0;
    }
    if(len == NULL) {
        CFSTORE_ERRLOG("%s:invalid len argument \r\n", __func__);
        goto out0;
    }
    ret = drv->Open(key_name, flags, hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to open node (key_name=\"%s\")(ret=%d)\r\n", __func__, key_name, (int) ret);
        goto out1;
    }
    ret = drv->Read(hkey, data, len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to read key (key_name=\"%s\"\r\n", __func__, key_name);
        goto out2;
    }
out2:
    drv->Close(hkey);
out1:
out0:
    return ret;
}

/* @brief   write the value blob of a specified KV
 * @note    this function expects cfstore to have been initialised with
 *          a call to ARM_CFSTORE_DRIVER::Initialize()
 */
int32_t cfstore_test_write(const char* key_name, const char* data, ARM_CFSTORE_SIZE* len)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(hkey);
    ARM_CFSTORE_FMODE flags;

    CFSTORE_FENTRYLOG("%s:entered\r\n", __func__);
    memset(&flags, 0, sizeof(flags));
    if(key_name == NULL) {
        CFSTORE_ERRLOG("%s:Error: invalid key_name argument \r\n", __func__);
        goto out0;
    }
    if(data == NULL) {
        CFSTORE_ERRLOG("%s:Error: invalid data argument \r\n", __func__);
        goto out0;
    }
    if(len == NULL) {
        CFSTORE_ERRLOG("%s:Error: invalid len argument \r\n", __func__);
        goto out0;
    }
    flags.write = 1;
    ret = drv->Open(key_name, flags, hkey);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to open node (key_name=\"%s\")(ret=%d)\r\n", __func__, key_name, (int) ret);
        goto out1;
    }
    ret = drv->Write(hkey, data, len);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to write key (key_name=\"%s\")\r\n", __func__, key_name);
        goto out2;
    }
out2:
    drv->Close(hkey);
out1:
out0:
    return ret;
}


