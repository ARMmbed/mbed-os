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

#include "cfstore_config.h"
#include "cfstore_debug.h"
#include "cfstore_test.h"
#include "configuration_store.h"

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED
#include "flash_journal_strategy_sequential.h"
#include "flash_journal.h"
#include "Driver_Common.h"
#endif /* CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <ctype.h>


/* ruler for measuring text strings */
/*                                                                                                    1         1         1         1         1         1         1         1         1         1         2         2         2 */
/* 0        1         2         3         4         5         6         7         8         9         0         1         2         3         4         5         6         7         8         9         0         1         2 */
/* 1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890 */

const uint8_t cfstore_test_byte_data_table[CFSTORE_TEST_BYTE_DATA_TABLE_SIZE] = {
    0x2d, 0xf3, 0x31, 0x4c, 0x11, 0x4f, 0xde, 0x0d, 0xbd, 0xbc, 0xa6, 0x78, 0x36, 0x5c, 0x1d, 0x28,
    0x5f, 0xa9, 0x10, 0x65, 0x54, 0x45, 0x21, 0x1a, 0x88, 0xfe, 0x76, 0x45, 0xb9, 0xac, 0x65, 0x9a,
    0x34, 0x9d, 0x73, 0x10, 0xb4, 0xa9, 0x2e, 0x90, 0x95, 0x68, 0xac, 0xfe, 0xc5, 0x2d, 0x15, 0x03,
    0x34, 0x70, 0xf1, 0x1d, 0x48, 0xa1, 0xa0, 0xed, 0x5c, 0x2f, 0xf5, 0x2b, 0xb9, 0x84, 0xbb, 0x45,
    0x32, 0xdd, 0xb1, 0x33, 0x95, 0x2a, 0xbc, 0x26, 0xf0, 0x89, 0xba, 0xf4, 0xbd, 0xf9, 0x5d, 0x2e,
    0x6e, 0x11, 0xc6, 0xa7, 0x78, 0xfc, 0xc9, 0x0e, 0x6b, 0x38, 0xba, 0x14, 0x1b, 0xab, 0x4c, 0x20,
    0x91, 0xe4, 0xb0, 0xf1, 0x2b, 0x14, 0x07, 0x6b, 0xb5, 0xcd, 0xe3, 0x49, 0x75, 0xac, 0xe8, 0x98,
    0xf1, 0x58, 0x8f, 0xd9, 0xc4, 0x8f, 0x00, 0x17, 0xb5, 0x06, 0x6a, 0x33, 0xbd, 0xa7, 0x40, 0x5a,
    0xbf, 0x49, 0xf7, 0x27, 0x1b, 0x4c, 0x3e, 0x6f, 0xe3, 0x08, 0x1f, 0xfd, 0xa6, 0xd4, 0xc7, 0x5f,
    0xa4, 0xa6, 0x82, 0xad, 0x19, 0xd5, 0x5c, 0xd8, 0x3a, 0x49, 0x85, 0xc9, 0x21, 0x83, 0xf6, 0xc6,
    0x84, 0xf9, 0x76, 0x89, 0xf3, 0x2d, 0x17, 0x50, 0x97, 0x38, 0x48, 0x9a, 0xe1, 0x82, 0xcd, 0xac,
    0xa8, 0x1d, 0xd7, 0x96, 0x5e, 0xb3, 0x08, 0xa8, 0x3a, 0xc7, 0x2b, 0x05, 0xaf, 0xdc, 0x16, 0xdf,
    0x48, 0x0f, 0x2a, 0x7e, 0x3a, 0x82, 0xd7, 0x80, 0xd6, 0x49, 0x27, 0x5d, 0xe3, 0x07, 0x62, 0xb3,
    0xc3, 0x6c, 0xba, 0xb2, 0xaa, 0x9f, 0xd9, 0x03, 0x0d, 0x27, 0xa8, 0xe0, 0xd6, 0xee, 0x79, 0x4b,
    0xd6, 0x97, 0x99, 0xb7, 0x11, 0xd6, 0x0d, 0x34, 0xae, 0x99, 0x4a, 0x93, 0x95, 0xd0, 0x5a, 0x34,
    0x19, 0xa2, 0x69, 0x57, 0xcf, 0x7c, 0x3d, 0x98, 0x88, 0x5d, 0x04, 0xf2, 0xd7, 0xac, 0xa5, 0x63
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


static int32_t cfstore_test_dump_print_array(const char* data, ARM_CFSTORE_SIZE len)
{
    int i;
    char buf[80];
    char sbuf[80];
    char* outbuf = buf;
    char* soutbuf = sbuf;

    memset(outbuf, 0, 80);
    memset(soutbuf, 0, 80);
    outbuf += sprintf(outbuf, "      ");
    soutbuf += sprintf(soutbuf, "      ");
    for (i = 0; i < (int) len; i++){
        outbuf += sprintf(outbuf, "%02X ", data[i]);

        if( !(isalnum( (int) data[i]) || ispunct( (int) data[i])) ){
            *soutbuf++ =  '*';
        } else {
            *soutbuf++ = data[i];
        }

        if( (i % 16 == 0) && i > 0){
            CFSTORE_LOG("%s", buf);
            CFSTORE_LOG("%s\n", sbuf);
            outbuf = buf;
            soutbuf = sbuf;
            memset(outbuf, 0, 80);
            memset(soutbuf, 0, 80);
            outbuf += sprintf(outbuf, "      ");
            soutbuf += sprintf(soutbuf, "      ");
        }
    }
    if(i % 16){
        /* Pad the end of the string to align string data. */
        while(i % 16){
            outbuf += sprintf(outbuf, "   ");
            i++;
        }
        CFSTORE_LOG("%s", buf);
        CFSTORE_LOG("      %s", sbuf);
    }
    CFSTORE_LOG("%s", "\n");
    return ARM_DRIVER_OK;
}


/* @brief   function to dump contents of cfstore
 */
int32_t cfstore_test_dump(void)
{
    const char* key_name_query = "*";
    char* read_buf = NULL;
    char key_name[CFSTORE_KEY_NAME_MAX_LENGTH+1];
    uint8_t len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
    ARM_CFSTORE_SIZE vlen = 0;
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* drv = &cfstore_driver;
    ARM_CFSTORE_HANDLE_INIT(next);
    ARM_CFSTORE_HANDLE_INIT(prev);
    ARM_CFSTORE_CAPABILITIES caps = cfstore_driver.GetCapabilities();

    CFSTORE_FENTRYLOG("%s:entered\n", __func__);

    CFSTORE_LOG("CFSTORE Flash Entries%s", "\n");
    CFSTORE_LOG("=====================%s", "\n\n");
    while((ret = drv->Find(key_name_query, prev, next)) == ARM_DRIVER_OK)
    {
        len = CFSTORE_KEY_NAME_MAX_LENGTH+1;
        ret = drv->GetKeyName(next, key_name, &len);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("Error: failed to get key name%s", "\n");
            break;
        }
        ret = drv->GetValueLen(next, &vlen);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("Error: failed to get value length%s", "\n");
            break;
        }
        read_buf = (char*) malloc(vlen+1);
        if(read_buf == NULL){
            CFSTORE_ERRLOG("Error: failed to malloc() read buffer%s", "\n");
            break;
        }
        ret = drv->Read(next, read_buf, &vlen);
        if(ret < ARM_DRIVER_OK){
            CFSTORE_ERRLOG("Error: failed to read key value%s", "\n");
            free(read_buf);
            break;
        }
        CFSTORE_LOG("  keyname : %s\n", key_name);
        CFSTORE_LOG("    name len : %d\n", (int) len);
        CFSTORE_LOG("    value len : %d\n", (int) vlen);
        CFSTORE_LOG("    data :%s", "\n");
        cfstore_test_dump_print_array((const char*) read_buf, vlen);
        CFSTORE_LOG("%s", ".\n");
        free(read_buf);
        CFSTORE_HANDLE_SWAP(prev, next);
    }
    CFSTORE_LOG("%s", ".\n");
    CFSTORE_LOG("  caps.asynchronous_ops : %d\n", (int) caps.asynchronous_ops);
    CFSTORE_LOG("%s", ".\n");
    CFSTORE_LOG("== End ==============%s", "\n\n");

    if(ret == ARM_CFSTORE_DRIVER_ERROR_KEY_NOT_FOUND) {
        /* As expected, no more keys have been found by the Find(). */
        ret = ARM_DRIVER_OK;
    }
    return ret;
}


/* @brief   test startup code to reset flash
 */
int32_t cfstore_test_startup(void)
{
    int32_t ret = ARM_DRIVER_ERROR;
    ARM_CFSTORE_DRIVER* cfstore_drv = &cfstore_driver;
    ARM_CFSTORE_CAPABILITIES caps = cfstore_driver.GetCapabilities();

    CFSTORE_LOG("INITIALIZING: caps.asynchronous_ops=%d\n", (int) caps.asynchronous_ops);

    /* Dump contents of CFSTORE */
    ret = cfstore_drv->Initialize(NULL, NULL);
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to initialize CFSTORE (ret=%d)\n", __func__, (int) ret);
        return ARM_DRIVER_ERROR;
    }
    ret = cfstore_test_dump();
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to dump CFSTORE (ret=%d)\n", __func__, (int) ret);
        return ARM_DRIVER_ERROR;
    }
    ret = cfstore_drv->Uninitialize();
    if(ret < ARM_DRIVER_OK){
        CFSTORE_ERRLOG("%s:Error: failed to uninitialize CFSTORE (ret=%d)\n", __func__, (int) ret);
        return ARM_DRIVER_ERROR;
    }

#ifdef CFSTORE_CONFIG_BACKEND_FLASH_ENABLED

    static FlashJournal_t jrnl;
    extern ARM_DRIVER_STORAGE ARM_Driver_Storage_MTD_K64F;
    const ARM_DRIVER_STORAGE *drv = &ARM_Driver_Storage_MTD_K64F;

    ret = FlashJournal_initialize(&jrnl, drv, &FLASH_JOURNAL_STRATEGY_SEQUENTIAL, NULL);
    if(ret < JOURNAL_STATUS_OK){
        CFSTORE_ERRLOG("%s:Error: failed to initialize flash journaling layer (ret=%d)\n", __func__, (int) ret);
        return ARM_DRIVER_ERROR;
    }
    ret = FlashJournal_reset(&jrnl);
    if(ret < JOURNAL_STATUS_OK){
        CFSTORE_ERRLOG("%s:Error: failed to reset flash journal (ret=%d)\n", __func__, (int) ret);
        return ARM_DRIVER_ERROR;
    }

#endif /*  CFSTORE_CONFIG_BACKEND_FLASH_ENABLED */

    return ARM_DRIVER_OK;
}


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
    CFSTORE_FENTRYLOG("%s:exiting (ret=%d).\r\n", __func__, (int) ret);
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

    (void) node; /* suppresses warning when building release */
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
    char key_name_buf[CFSTORE_KEY_NAME_MAX_LENGTH+1];
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
        /* revert CFSTORE_LOG for more trace */
        CFSTORE_DBGLOG("Created KV successfully (key_name=\"%s\", value=\"%s\")\r\n", key_name_buf, read_buf);
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


