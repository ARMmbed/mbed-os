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

#include "tztrng_test_pal.h"
#include "FreeRTOS.h"

void tztrngTest_pal_unmapCcRegs(unsigned long regBase)
{
	if (regBase == 0)
		regBase = 0;

	return;
}

unsigned long tztrngTest_pal_mapCcRegs(unsigned long hwBase)
{
	return hwBase;
}

void *tztrngTest_pal_malloc(size_t size)
{
	return (void *)pvPortMalloc(size);
}

void tztrngTest_pal_free(void *pvAddress)
{
	vPortFree(pvAddress);
}

int tztrngTest_pal_dumpData(unsigned char *large_buf, size_t outputlen)
{
	unsigned int i;

	printf("data: ");

	for (i = 0; i < outputlen; ++i) {
		printf("%02x ", large_buf[i]);
	}

	printf("\n");

	return 0;
}
