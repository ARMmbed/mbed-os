/*
 * Copyright (c) 2015, 2017-2018, Arm Limited and affiliates.
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
#include "ns_crc.h"

//#define CRC_TEST

#define POLYNOMIAL_CRC15_CCIT 0x1021
#define POLYNOMIAL_CRC15_ANSI 0x8005

uint16_t crc16_ansi(uint8_t *message, int nBytes)
{
    return crc16_calc(message, nBytes, POLYNOMIAL_CRC15_ANSI);
}
uint16_t crc16_ccitt(uint8_t *message, int nBytes)
{
    return crc16_calc(message, nBytes, POLYNOMIAL_CRC15_CCIT);
}

uint16_t crc16_calc(uint8_t *data, uint16_t data_length, uint16_t polynomial)
{
    int crc = 0;
    int i, bit;
    for (i = 0; i < data_length; i++) {
        crc ^= (int)data[i] << 8;
        for (bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc & 0xffff;
}



#ifdef CRC_TEST //unity test
#include <stdio.h>
#include <string.h>
//#include "unity.h"

#ifndef TEST_ASSERT_EQUAL_INT
#define TEST_ASSERT_EQUAL_INT(a,b, s) do{ if(a!=b){printf("%s: 0x%04x != 0x%04x - FAIL\n", s, a, b);} else printf("%s: 0x%04x == 0x%04x - PASS\n", s,a,b);}while(0)
#endif

char data[] = "arm";
/* Unity test code starts */
void setUp(void)
{
}

void tearDown(void)
{
}

void test_param_crc16_ansi(void)
{
    uint16_t crc = crc16_ibm(data, strlen(data));
    TEST_ASSERT_EQUAL_INT(crc, 0x2406, "CRC16-ansi");
}
void test_param_crc16_ccitt(void)
{
    uint16_t crc = crc16_ccitt(data, strlen(data));
    TEST_ASSERT_EQUAL_INT(crc, 0x7F6B, "CRC16-CCIT");
}

void main(void)
{
    test_param_crc16_ansi();
    test_param_crc16_ccitt();
}

#endif
