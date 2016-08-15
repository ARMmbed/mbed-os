/*
 *  Hardware entropy collector for NUC472's RNGA
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */


#include <stdlib.h>
#include <string.h>
#include "cmsis.h"
#include "NUC472_442.h"
#include "us_ticker_api.h"

/*
 * Get Random number generator.
 */
static volatile int  g_PRNG_done;

void CRYPTO_IRQHandler()
{
    if (PRNG_GET_INT_FLAG()) {
        g_PRNG_done = 1;
        PRNG_CLR_INT_FLAG();
    }
} 

static void rng_get(unsigned char *pConversionData)
{
	uint32_t *p32ConversionData;

	p32ConversionData = (uint32_t *)pConversionData;
	
    /* Unlock protected registers */
    SYS_UnlockReg();	
    /* Enable IP clock */
    CLK_EnableModuleClock(CRPT_MODULE);
	
    /* Lock protected registers */
    SYS_LockReg();	
	
    NVIC_EnableIRQ(CRPT_IRQn);
    PRNG_ENABLE_INT();
	
//	PRNG_Open(PRNG_KEY_SIZE_64, 0, 0);
	PRNG_Open(PRNG_KEY_SIZE_256, 1, us_ticker_read());
    PRNG_Start();
    while (!g_PRNG_done);


    PRNG_Read(p32ConversionData);

//    printf("    0x%08x  0x%08x  0x%08x  0x%08x\n\r", *p32ConversionData, *(p32ConversionData+1), *(p32ConversionData+2), *(p32ConversionData+3));
//    printf("    0x%08x  0x%08x  0x%08x  0x%08x\n\r", *(p32ConversionData+4), *(p32ConversionData+5), *(p32ConversionData+6), *(p32ConversionData+7));

    PRNG_DISABLE_INT();
    NVIC_DisableIRQ(CRPT_IRQn);
//    CLK_DisableModuleClock(CRPT_MODULE);
		
}


/*
 * Get len bytes of entropy from the hardware RNG.
 */
 
int mbedtls_hardware_poll( void *data,
                    unsigned char *output, size_t len, size_t *olen )
{
#if 0
    unsigned long timer = us_ticker_read();
    ((void) data);
    *olen = 0;
 
    if( len < sizeof(unsigned long) )
        return( 0 );
 
    memcpy( output, &timer, sizeof(unsigned long) );
    *olen = sizeof(unsigned long);
#else
	*olen = 0;
    if( len < 32 )
	{
		unsigned char tmpBuff[32];		
		rng_get(tmpBuff);
		memcpy( output, &tmpBuff, len );
		*olen = len;
        return( 0 );
	}	
	for( int i = 0; i < (len/32) ; i++)
	{
		rng_get(output);
		*olen += 32;
//		printf("Output result of len[%d][%d]: 0x%08x 0x%08x\n\r", len, *olen, *((int32_t *)output), *(((int32_t *)output)+1));
		output += 32;
	}
#endif
	
    return( 0 );
}
 


