/*
* Copyright (c) 2018 ARM Limited. All rights reserved.
* SPDX-License-Identifier: Apache-2.0
* Licensed under the Apache License, Version 2.0 (the License); you may
* not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an AS IS BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "base64b.h"

using namespace std;

static char IntToBase64Char(uint8_t intVal)
{
    const char *base64Digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    return base64Digits[intVal & 0x3F];
}

#define BASE_64_PAD 0xFF
static base64_result_e Base64CharToInt(char base64, uint8_t *intVal)
{
    if (NULL == intVal) {
        return BASE64_INVALID_PARAMETER;
    }

    if ((base64 >= 'A') && (base64 <= 'Z')) {
        *intVal = base64 - 'A' ;
    } else if ((base64 >= 'a') && (base64 <= 'z')) {
        *intVal = base64 - 'a' + 26;
    } else if ((base64 >= '0') && (base64 <= '9')) {
        *intVal = base64 - '0' + 52;
    } else if (base64 == '+') {
        *intVal = 62;
    } else if (base64 == '/') {
        *intVal = 63;
    } else if (base64 == '=') {
        *intVal = BASE_64_PAD;
    } else {
        return BASE64_ERROR;
    }

    return BASE64_SUCCESS;
}

base64_result_e trng_DecodeNBase64(const char *string,
                                   uint32_t stringMaxSize,
                                   void *buffer,
                                   uint32_t bufferSize,
                                   uint32_t *lengthWritten,
                                   uint32_t *charsProcessed)
{
    base64_result_e result = BASE64_SUCCESS;
    uint32_t bitOffset = 0;
    uint8_t *writePtr = (uint8_t *)buffer;
    uint8_t *bufferEnd = (uint8_t *)buffer + bufferSize;
    uint8_t tempVal = 0;
    uint32_t currPos = 0;
    uint32_t localBytesWritten = 0;
    uint32_t localCharsProcessed = 0;
    bool isEndOfString = false;

    if ((NULL == string) || (NULL == buffer) || (bufferSize == 0)) {
        return BASE64_INVALID_PARAMETER;
    }

    *writePtr = 0;
    while ((currPos < stringMaxSize) &&
            (string[currPos] != 0) &&
            (writePtr  <  bufferEnd) &&
            (!isEndOfString)) {
        uint8_t val;

        if (string[currPos] == 0) {
            break;
        }

        result = Base64CharToInt(string[currPos++], &val);
        if (result != BASE64_SUCCESS) {
            break;
        }

        if (val != BASE_64_PAD) {
            if (bitOffset <= 2) {
                tempVal |= val << (2 - bitOffset);
                if (bitOffset == 2) {
                    *writePtr++ = tempVal;
                    tempVal = 0;
                }
            } else {
                *writePtr++ = (uint8_t)(tempVal | (val >> (bitOffset - 2)));
                tempVal = (uint8_t)(val << (10 - bitOffset));
            }
        } else { // found BASE_64_PAD
            // At most two pad characters may occur at the end of the encoded stream
            if (bitOffset == 2) {
                isEndOfString = true;    // The last padding byte has been processed.
            } else if (bitOffset != 4) {
                return BASE64_ERROR;    // Incorrect padding
            }
        }

        bitOffset = (bitOffset + 6) & 0x7;
        if (bitOffset == 0) {
            localBytesWritten = (uint32_t)(writePtr - (uint8_t *)buffer);
            localCharsProcessed = currPos;
        }
    }
    if (charsProcessed == NULL) {
        localBytesWritten = (uint32_t)(writePtr - (uint8_t *)buffer);
    } else {
        *charsProcessed = localCharsProcessed;
    }
    if (lengthWritten != NULL) {
        *lengthWritten = localBytesWritten;
    } else if (bufferSize != localBytesWritten) {
        return BASE64_BUFFER_TOO_SMALL;
    }

    // Check if additional bytes should have been processed but buffer isn't sufficient.
    if ((result == BASE64_SUCCESS) &&
            (!isEndOfString) &&
            (currPos < stringMaxSize) &&
            (string[currPos] != 0) &&
            (string[currPos] != '=')) {
        return BASE64_BUFFER_TOO_SMALL;
    }

    if (result != BASE64_SUCCESS) {
        return result;
    }

    return BASE64_SUCCESS;
}

base64_result_e trng_EncodeBase64(const void *buffer, uint32_t bufferSize, char *string, uint32_t stringSize)
{
    uint32_t bitOffset = 0;

    const uint8_t *readPtr = (const uint8_t *)buffer;
    const uint8_t *bufferEnd = (const uint8_t *)buffer + bufferSize;

    char *writePtr = string;
    char *stringEnd = string + stringSize - 1;

    if ((NULL == string) || (NULL == buffer) || (stringSize == 0)) {
        return BASE64_INVALID_PARAMETER;
    }

    stringSize--;
    while (readPtr < bufferEnd && writePtr < stringEnd) {
        uint8_t tempVal = 0;
        switch (bitOffset) {
            case 0:
                *writePtr++ = IntToBase64Char(*readPtr >> 2);                 // take upper 6 bits
                break;
            case 6:
                tempVal = *readPtr++ << 4;
                if (readPtr < bufferEnd) {
                    tempVal |= *readPtr >> 4;
                }
                *writePtr++ = IntToBase64Char(tempVal);
                break;
            case 4:
                tempVal = *readPtr++ << 2;
                if (readPtr < bufferEnd) {
                    tempVal |= *readPtr >> 6;
                }
                *writePtr++ = IntToBase64Char(tempVal);
                break;
            case 2:
                *writePtr++ = IntToBase64Char(*readPtr++);
                break;
            default:
                return BASE64_ERROR; // we should never reach this code.
        }
        bitOffset = (bitOffset + 6) & 0x7;
    }
    while (bitOffset > 0 && writePtr < stringEnd) {
        *writePtr++ = '=';
        bitOffset = (bitOffset + 6) & 0x7;
    }
    *writePtr = 0;

    if ((readPtr < bufferEnd) || (bitOffset != 0)) {
        return (BASE64_BUFFER_TOO_SMALL);
    }

    return (BASE64_SUCCESS);
}
