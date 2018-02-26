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
*                                                                             *                                            *
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tztrng_test.h"
#include "tztrng.h"
#include "tztrng_test_pal.h"
#include "tztrng_test_pal_api.h"

#include "dx_reg_base_host.h"

#define TEST_ITERATIONS 				100
#define TEST_VECTOR_SIZE_BYTES 			256
#define LARGE_BUFFER_TEST_SIZE_BYTES 	10000

/* Enables the output of the largeBuffer to uart/file */
#define DUMP_DATA 						0

void tztrngTest(void)
{
	int err, i, j;
	int compare_buf[TEST_ITERATIONS] = { 0 };
	unsigned int baseRngReg = 0;
	size_t outputLen = LARGE_BUFFER_TEST_SIZE_BYTES;
	int testVectorSize = TEST_VECTOR_SIZE_BYTES;
	int testIterate = TEST_ITERATIONS;
	unsigned char* buf;
	unsigned char* largeBuf;

	/* These buffers, buf and largeBuf, cannot be global since they need to be stored in a DMA-able address */
	buf = tztrngTest_pal_malloc(TEST_VECTOR_SIZE_BYTES);
	if (buf == NULL)
	{
		TZTRNG_PRINTF("failed to allocate buf %d bytes\n", TEST_VECTOR_SIZE_BYTES);
		goto EndNoFree;
	}

	largeBuf = tztrngTest_pal_malloc(LARGE_BUFFER_TEST_SIZE_BYTES);
	if (largeBuf == NULL)
	{
		TZTRNG_PRINTF("failed to allocate largeBuf %d bytes\n", LARGE_BUFFER_TEST_SIZE_BYTES);

		/* free the successfully allocated buffer */
		tztrngTest_pal_free(buf);

		goto EndNoFree;
	}

	/* map register base addr */
	baseRngReg = tztrngTest_pal_mapCcRegs(DX_BASE_RNG);
	memset(largeBuf, 0, LARGE_BUFFER_TEST_SIZE_BYTES);
	memset(buf, 0, TEST_VECTOR_SIZE_BYTES);

	/* collect a large buffer */
	TZTRNG_PRINTF("\nbaseReg[%p]\n", (void*)baseRngReg);
	TZTRNG_PRINTF("get entropy size of %d bytes.\n", outputLen);
	err = CC_TrngGetSource(baseRngReg, largeBuf, &outputLen, outputLen * 8 /*in bits*/);
	if (err)
	{
		TZTRNG_PRINTF("cc_trng_getSoruce error(0x%X)\n", err);
		goto End;
	}

#if DUMP_DATA
	if (tztrngTest_pal_dumpData(largeBuf, outputLen) != 0)
	{
		TZTRNG_PRINTF("Failed to dump trng data\n");
	}
#endif /* DUMP_DATA */

	/* Collect and compare N buffers */
	TZTRNG_PRINTF("start %d times test iterations\n", testIterate);
	for (i = 0; i < testIterate; i++)
	{

		err = CC_TrngGetSource(baseRngReg, buf, &outputLen, testVectorSize * 8 /*in bits*/);
		TZTRNG_PRINTF("%d [0x%02x%02x%02x%02x]\n", i, buf[3],buf[2],buf[1],buf[0]);
		if (err)
		{
			fprintf(stderr, "\nTest error (0x%X)\n", err);
			goto End;
		}
		/* compare the first word */
		memcpy(&compare_buf[i], buf, sizeof(int));

		for (j = 0; j < i; j++)
		{
			if (!(memcmp(&compare_buf[j], &compare_buf[i], sizeof(int))))
			{
				TZTRNG_PRINTF("\nIdentical word,index %d,%d, (0x%x, 0x%x)\n", j, i, compare_buf[j], compare_buf[i]);
				goto End;
			}
		}
	}

	TZTRNG_PRINTF("\nTest passed - received %d different buffers\n", i);

End:
	tztrngTest_pal_free(buf);
	tztrngTest_pal_free(largeBuf);

EndNoFree:
	tztrngTest_pal_unmapCcRegs(baseRngReg);

}

#ifndef DX_PLAT_MPS2_PLUS
int main(void)
{
	tztrngTest();
	return 0;
}
#endif
