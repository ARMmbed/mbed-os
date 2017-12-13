/******************************************************************************
* Copyright (c) 2017-2017, ARM, All Rights Reserved                           *
* SPDX-License-Identifier: Apache-2.0                                         *
*                                                                             *
* Licensed under the Apache License, Version 2.0 (the "License");             *
* you may not use this file except in compliance with the License.            *
*                                                                             *
* You may obtain a copy of the License at                                     *
* http://www.apache.org/licenses/LICENSE-2.0                                  *
*                                                                             *
* Unless required by applicable law or agreed to in writing, software         *
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT   *
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.            *
*                                                                             *
* See the License for the specific language governing permissions and         *
* limitations under the License.                                              *
******************************************************************************/

#include "stdlib.h"
#include "stdint.h"

#include "test_pal_cli.h"
#include "tztrng_test.h"

static int tztrngTest_cliCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	pcWriteBuffer = pcWriteBuffer;
	xWriteBufferLen = xWriteBufferLen;
	pcCommandString = pcCommandString;

	tztrngTest();

	return 0;
}

int libtztrng_test_initLib(void)
{
	static Test_PalCliCommand commandToRegister =
	{
		"tztrng-test",
		"\r\ntztrng-test:\r\n Iterate over tztrng genration verify that all results were different\r\n",
		tztrngTest_cliCommand,
		0
	};

	return Test_PalCLIRegisterCommand(&commandToRegister);
}
