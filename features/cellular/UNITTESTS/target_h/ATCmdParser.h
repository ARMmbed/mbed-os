/*
 * Copyright (c) , Arm Limited and affiliates.
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
#ifndef __AT_CMD_PARSER_H__
#define __AT_CMD_PARSER_H__

#include "mbed.h"
#include <cstdarg>
#include "FileHandle.h"

class ATCmdParser
{
public:
    ATCmdParser(mbed::FileHandle *fh, const char *output_delimiter = "\r",
                 int buffer_size = 256, int timeout = 8000, bool debug = false){}

    ~ATCmdParser(){}
};

#endif //__AT_CMD_PARSER_H__

