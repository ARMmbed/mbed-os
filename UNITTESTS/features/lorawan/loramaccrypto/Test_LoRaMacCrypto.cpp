/*
 * Copyright (c) 2018, Arm Limited and affiliates
 * SPDX-License-Identifier: Apache-2.0
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

#include "gtest/gtest.h"
#include "LoRaMacCrypto.h"

#include "cipher_stub.h"
#include "cmac_stub.h"
#include "aes_stub.h"

class Test_LoRaMacCrypto : public testing::Test {
protected:
    LoRaMacCrypto *object;

    virtual void SetUp()
    {
        cipher_stub.info_value = NULL;
        cipher_stub.int_zero_counter = 0;
        cipher_stub.int_value = 0;
        cmac_stub.int_zero_counter = 0;
        cmac_stub.int_value = 0;
        aes_stub.int_zero_counter = 0;
        aes_stub.int_value = 0;
        object = new LoRaMacCrypto();
    }

    virtual void TearDown()
    {
        delete object;
    }
};

TEST_F(Test_LoRaMacCrypto, constructor)
{
    EXPECT_TRUE(object);
    LoRaMacCrypto obj;
}

TEST_F(Test_LoRaMacCrypto, compute_mic)
{
    EXPECT_TRUE(MBEDTLS_ERR_CIPHER_ALLOC_FAILED == object->compute_mic(NULL, 0, NULL, 0, 0, 0, 0, NULL));

    mbedtls_cipher_info_t info;
    cipher_stub.info_value = &info;
    cipher_stub.int_zero_counter = 0;
    cipher_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_mic(NULL, 0, NULL, 0, 0, 0, 0, NULL));

    cipher_stub.int_value = 0;
    cmac_stub.int_zero_counter = 0;
    cmac_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_mic(NULL, 0, NULL, 0, 0, 0, 0, NULL));

    cmac_stub.int_zero_counter = 1;
    cmac_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_mic(NULL, 0, NULL, 0, 0, 0, 0, NULL));

    cmac_stub.int_zero_counter = 2;
    cmac_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_mic(NULL, 0, NULL, 0, 0, 0, 0, NULL));

    cmac_stub.int_zero_counter = 3;
    cmac_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_mic(NULL, 0, NULL, 0, 0, 0, 0, NULL));

    uint32_t mic[16];
    cmac_stub.int_zero_counter = 3;
    cmac_stub.int_value = 0;
    EXPECT_TRUE(0 == object->compute_mic(NULL, 0, NULL, 0, 0, 0, 0, mic));

}

TEST_F(Test_LoRaMacCrypto, encrypt_payload)
{
    aes_stub.int_zero_counter = 0;
    aes_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->encrypt_payload(NULL, 0, NULL, 0, 0, 0, 0, NULL));

    aes_stub.int_zero_counter = 1;
    aes_stub.int_value = -2;
    uint8_t buf[60];
    uint8_t enc[60];
    EXPECT_TRUE(-2 == object->encrypt_payload(buf, 20, NULL, 0, 0, 0, 0, enc));

    aes_stub.int_zero_counter = 2;
    aes_stub.int_value = -3;
    EXPECT_TRUE(-3 == object->encrypt_payload(buf, 20, NULL, 0, 0, 0, 0, enc));

    aes_stub.int_value = 0;
    EXPECT_TRUE(0 == object->encrypt_payload(buf, 20, NULL, 0, 0, 0, 0, enc));

    EXPECT_TRUE(0 == object->encrypt_payload(buf, 60, NULL, 0, 0, 0, 0, enc));

    aes_stub.int_zero_counter = 0;
    EXPECT_TRUE(0 == object->encrypt_payload(NULL, 0, NULL, 0, 0, 0, 0, NULL));
}

TEST_F(Test_LoRaMacCrypto, decrypt_payload)
{
    EXPECT_TRUE(0 == object->decrypt_payload(NULL, 0, NULL, 0, 0, 0, 0, NULL));
}

TEST_F(Test_LoRaMacCrypto, compute_join_frame_mic)
{
    uint32_t mic[16];
    EXPECT_TRUE(MBEDTLS_ERR_CIPHER_ALLOC_FAILED == object->compute_join_frame_mic(NULL, 0, NULL, 0, NULL));
    mbedtls_cipher_info_t info;
    cipher_stub.info_value = &info;
    cipher_stub.int_zero_counter = 0;
    cipher_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_join_frame_mic(NULL, 0, NULL, 0, NULL));

    cipher_stub.int_value = 0;
    cmac_stub.int_zero_counter = 0;
    cmac_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_join_frame_mic(NULL, 0, NULL, 0, NULL));

    cmac_stub.int_zero_counter = 1;
    cmac_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_join_frame_mic(NULL, 0, NULL, 0, NULL));

    cmac_stub.int_zero_counter = 2;
    cmac_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_join_frame_mic(NULL, 0, NULL, 0, NULL));

    cmac_stub.int_zero_counter = 3;
    cmac_stub.int_value = 0;
    EXPECT_TRUE(0 == object->compute_join_frame_mic(NULL, 0, NULL, 0, mic));
}

TEST_F(Test_LoRaMacCrypto, decrypt_join_frame)
{
    aes_stub.int_zero_counter = 0;
    aes_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->decrypt_join_frame(NULL, 0, NULL, 0, NULL));

    aes_stub.int_zero_counter = 1;
    aes_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->decrypt_join_frame(NULL, 0, NULL, 0, NULL));

    aes_stub.int_value = 0;
    uint8_t buf[60];
    uint8_t enc[60];
    EXPECT_TRUE(0 == object->decrypt_join_frame(buf, 60, NULL, 0, enc));
}

TEST_F(Test_LoRaMacCrypto, compute_skeys_for_join_frame)
{
    uint8_t nwk_key[16];
    uint8_t app_key[16];
    uint8_t nonce[16];

    aes_stub.int_zero_counter = 0;
    aes_stub.int_value = -1;
    EXPECT_TRUE(-1 == object->compute_skeys_for_join_frame(NULL, 0, nonce, 0, nwk_key, app_key));

    aes_stub.int_zero_counter = 1;
    aes_stub.int_value = -2;
    EXPECT_TRUE(-2 == object->compute_skeys_for_join_frame(NULL, 0, nonce, 0, nwk_key, app_key));

    aes_stub.int_zero_counter = 0;
    aes_stub.int_value = 0;
    EXPECT_TRUE(0 == object->compute_skeys_for_join_frame(NULL, 0, nonce, 0, nwk_key, app_key));
}
