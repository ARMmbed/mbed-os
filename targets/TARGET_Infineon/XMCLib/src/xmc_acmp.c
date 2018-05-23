/**
 * @file xmc_acmp.c
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
 * 2014-12-10:
 *     - Initial <br>
 * 2015-02-20:
 *     - Removed unused declarations<br> 
 * 2015-05-08:
 *     - Fixed sequence problem of low power mode in XMC_ACMP_Init() API<br>
 *     - Fixed wrong register setting in XMC_ACMP_SetInput() API<br> 
 *     - Removed return type variable and by default comparator enable from XMC_ACMP_Init() API. <br>
 *       Additional call to XMC_ACMP_EnableComparator() API needed to start Comparator after Init.<br>
 * 2015-06-04:
 *     - Removed return type variable and by default comparator enable from XMC_ACMP_Init() API. <br>
 *     - Divided XMC_ACMP_SetInput into two 3 APIs to reduce the code size and complexity as stated below<br> 
 *       (a)XMC_ACMP_EnableReferenceDivider <br>
 *       (b)XMC_ACMP_DisableReferenceDivider <br> 
 *       (c)XMC_ACMP_SetInput <br> 
 *     - Optimized enable and disable API's and moved to header file as static inline APIs.
 *     - XMC_ACMP_t typedef changed to structure which overrides the standard header file structure.
 * 2015-06-20: 
 *     - Removed definition of GetDriverVersion API 
 * @endcond 
 *
 */


/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_acmp.h>

/* If ACMP is available*/
#if defined (COMPARATOR)

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/

#define XMC_ACMP_INSTANCE_1    (1U) /* Instance number for Slice-1 */

/*********************************************************************************************************************
 * API IMPLEMENTATION
 ********************************************************************************************************************/

/* API to initialize an instance of ACMP module */
void XMC_ACMP_Init(XMC_ACMP_t *const peripheral, uint32_t instance, const XMC_ACMP_CONFIG_t *const config)
{

  XMC_ASSERT("XMC_ACMP_Init:NULL Configuration", (config != (XMC_ACMP_CONFIG_t *)NULL))
  XMC_ASSERT("XMC_ACMP_Init:Wrong module pointer", XMC_ACMP_CHECK_MODULE_PTR(peripheral))
  XMC_ASSERT("XMC_ACMP_Init:Wrong instance number", XMC_ACMP_CHECK_INSTANCE(instance) )

  /*
   * Initializes the comparator with configuration supplied. Low power node setting is retained during initialization.
   * All the instances passed are handled with low power setting, to avoid conditional check for ACMP0 instance.
   * This reduces the code size. No side effects, because this register bit field is empty for other instances.
   */
  peripheral->ANACMP[instance] = ((peripheral->ANACMP[instance] & (uint32_t)COMPARATOR_ANACMP0_CMP_LPWR_Msk)) |
                                 (uint32_t)config->anacmp;
}

/* API to select INP source */
void XMC_ACMP_SetInput(XMC_ACMP_t *const peripheral, uint32_t instance, XMC_ACMP_INP_SOURCE_t source)
{
  XMC_ASSERT("XMC_ACMP_SetInput:Wrong module pointer", XMC_ACMP_CHECK_MODULE_PTR(peripheral))
  XMC_ASSERT("XMC_ACMP_SetInput:Wrong instance number", ((instance != XMC_ACMP_INSTANCE_1) &&
                                                          XMC_ACMP_CHECK_INSTANCE(instance)) )
  XMC_ASSERT("XMC_ACMP_SetInput:Wrong input source", ((source == XMC_ACMP_INP_SOURCE_STANDARD_PORT) ||
                                                      (source == XMC_ACMP_INP_SOURCE_ACMP1_INP_PORT)) )

  /*
   * Three options of Input Setting are listed below
   * 1. The comparator inputs aren't connected to other comparator inputs
   * 2. Can program the comparators to connect ACMP0.INP to ACMP1.INP in XMC1200 AA or XMC1300 AA
   *    Can program the comparators to connect ACMP0.INN to ACMP1.INP in XMC1200 AB or XMC1300 AB or XMC1400 AA
   * 3. Can program the comparators to connect ACMP2.INP to ACMP1.INP
   * 4. Can program the comparators to connect ACMP3.INP to ACMP1.INP in XMC1400
   */
  peripheral->ANACMP[instance] = ((peripheral->ANACMP[instance] & (uint32_t)(~COMPARATOR_ANACMP0_ACMP0_SEL_Msk))) |
                                 (uint32_t)source;
}

#endif /* #ifdef ACMP_AVAILABLE */
