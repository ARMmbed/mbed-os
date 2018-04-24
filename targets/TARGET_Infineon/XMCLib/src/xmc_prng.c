
/**
 * @file xmc_prng.c
 * @date 2015-06-20
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial <br>
 *     - Removed GetDriverVersion API <br>
 *
 * 2015-06-20
 *     - Removed definition of GetDriverVersion API <br>
 *
 * @endcond
 */
 
#include "xmc_prng.h"

#if defined (PRNG)

/*********************************************************************************************************************
 * API IMPLEMENTATION
 *********************************************************************************************************************/
 
/*
 * Initializes the PRNG peripheral with the settings in the
 * initialization structure XMC_PRNG_INIT_t
 */
XMC_PRNG_INIT_STATUS_t XMC_PRNG_Init(const XMC_PRNG_INIT_t *prng)
{
  volatile uint16_t read_warm_up;
  uint16_t reg_val, iter;
  XMC_PRNG_INIT_STATUS_t status = XMC_PRNG_INITIALIZED;
  
  XMC_ASSERT("XMC_PRNG_Init:Null Pointer", (prng != (XMC_PRNG_INIT_t *)NULL));

  /* Configure block size for key loading mode */
  XMC_PRNG_SetRandomDataBlockSize(XMC_PRNG_RDBS_WORD);

  /* Enable key loading mode */
  XMC_PRNG_EnableKeyLoadingMode();

  /* Load key words (80 bits) and wait till RDV is set */
  for (iter = (uint16_t)0UL; iter < (uint16_t)5UL; iter++)
  {
    XMC_PRNG_LoadKeyWords(prng->key_words[iter]);
    while (PRNG_CHK_RDV_Msk != XMC_PRNG_CheckValidStatus());
  }
  
  XMC_PRNG_EnableStreamingMode();

  /* Warm up phase: Read and discard 64 bits */
  read_warm_up = PRNG->WORD;
  read_warm_up = PRNG->WORD;
  read_warm_up = PRNG->WORD;
  reg_val      = PRNG->WORD;
  read_warm_up &= reg_val;

  /* Configure block size either byte (8 bit) or word (16 bit) */
  XMC_PRNG_SetRandomDataBlockSize(prng->block_size);

  /*
   * Checks for reset value for "random data block size". If reset,
   * PRNG is not initialized
   */
  if ((uint16_t)XMC_PRNG_RDBS_RESET == (PRNG->CTRL & (uint16_t)PRNG_CTRL_RDBS_Msk))
  {
    status = XMC_PRNG_NOT_INITIALIZED;
  }

  return status;
}

#endif /* #if defined (PRNG) */
