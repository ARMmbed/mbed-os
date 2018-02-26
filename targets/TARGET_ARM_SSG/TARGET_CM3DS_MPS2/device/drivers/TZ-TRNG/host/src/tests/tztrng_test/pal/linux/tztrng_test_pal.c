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

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#define CC_REGS_FILE_SIZE 				0x10000

static int ccRegMemFd = -1;

void tztrngTest_pal_unmapCcRegs(unsigned long regBase)
{
	munmap((void*) regBase, CC_REGS_FILE_SIZE);
	close(ccRegMemFd);
}

unsigned long tztrngTest_pal_mapCcRegs(unsigned long hwBase)
{
	unsigned long regBase;

	ccRegMemFd = open("/dev/mem", O_RDWR | O_SYNC);
	if (ccRegMemFd < 0)
	{
		fprintf(stderr, "Failed opening /dev/mem - %s.\n", strerror(errno));
		exit(1);
	}

	/* close-on-exec */
	(void) fcntl(ccRegMemFd, F_SETFD, FD_CLOEXEC);

	/* Map registers space */
	regBase = (unsigned long) mmap(NULL, CC_REGS_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, ccRegMemFd, hwBase);
	fprintf(stderr, "mapped regBase = 0x%lx\n", regBase);

	if ((void *) regBase == MAP_FAILED)
	{
		fprintf(stderr, "Failed mapping register - %s.\n", strerror(errno));
		exit(2);
	}

	return regBase;
}

void *tztrngTest_pal_malloc(size_t size)
{
	return malloc(size);
}

void tztrngTest_pal_free(void *pvAddress)
{
	free(pvAddress);
}

int tztrngTest_pal_dumpData(unsigned char *large_buf, size_t outputlen)
{
	FILE *fp;
	fp = fopen("trng.dat", "w");
	if (NULL == fp)
		return (-1);

	fwrite(large_buf, outputlen, 1, fp);
	fclose(fp);

	return 0;
}
