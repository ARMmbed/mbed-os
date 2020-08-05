/* mbed Microcontroller Library
 * Copyright (c) 2013-2014 ARM Limited
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

#include "mbed.h"
#include "platform/Stream.h"
#include "greentea-client/test_env.h"

class DevNull : public Stream {
public:
    DevNull(const char *name = NULL) : Stream(name) {}

protected:
    virtual int _getc()
    {
        return 0;
    }
    virtual int _putc(int c)
    {
        return c;
    }
};

DevNull null("null");

int main()
{
    GREENTEA_SETUP(2, "dev_null_auto");

    printf("MBED: before re-routing stdout to /null\n");   // This shouldn't appear
    greentea_send_kv("to_stdout", "re-routing stdout to /null");

    if (freopen("/null", "w", stdout)) {
        // This shouldn't appear on serial
        // We should use pure printf here to send KV
        printf("{{to_null;printf redirected to /null}}\n");
        printf("MBED: this printf is already redirected to /null\n");
    }

    while (1) {
        // Success is determined by the host test at this point, so busy wait
    }
}
