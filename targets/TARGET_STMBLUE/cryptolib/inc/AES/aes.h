/**
  ******************************************************************************
  * @file    aes.h
  * @author  MCD Application Team
  * @version V2.1
  * @date    22-June-2012
  * @brief   Container for the AES functionalities
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http:/www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************/

/*!
* \page Tutorial_AES AES Tutorials
*
*
*  In this tutorial there will be described the high level functions for AES in various modes.
*  The API is composed by separate functions for doing Initialization, Data Processing, Finalization. \n
*  To correctly use these API
*    the user should call once the Init function, which initialize a context, then the user can call the data
*    processing (Append) function as many times as it needs, to process different input, and at the end he should
*    call, once, the finalization function, to finish the processing. \n
*    Please refer to the following tutorials according to actual modes:
*    - \subpage Tutorial_AES_ECB
*    - \subpage Tutorial_AES_CBC
*    - \subpage Tutorial_AES_CTR
*    - \subpage Tutorial_AES_CFB
*    - \subpage Tutorial_AES_OFB
*
*   There are analogous functions for DES and TDES ( \ref DES, \ref TDES )
*
*  Note that, however, AES authenticated encryption modes (CCM, GCM) has also a function to process an input
*  in authentication mode but without encrypting it, commonly used to process header 
*  (i.e. \ref AES_GCM_Header_Append), see tutorials about:
*    -  \subpage Tutorial_AES_GCM
*    -  \subpage Tutorial_AES_CCM
*
*  For CMAC and KEYWRAP modes see their tutorials: \subpage Tutorial_AES_CMAC , \subpage Tutorial_AES_Key_Wrap
*  The tutorial oin XTS can be found here: \subpage Tutorial_AES_XTS
*/
#ifndef AES_INCLUDE_H
#define AES_INCLUDE_H
  
#include "Common/aes_low_level.h"
#include "Common/aes_common.h"

  
#ifdef INCLUDE_ECB
#include "ECB/aes_ecb.h"
#endif

#ifdef INCLUDE_CBC
#include "CBC/aes_cbc.h"
#endif

#ifdef INCLUDE_CTR
#include "CTR/aes_ctr.h"
#endif

#ifdef INCLUDE_CFB
#include "CFB/aes_cfb.h"
#endif

#ifdef INCLUDE_OFB
#include "OFB/aes_ofb.h"
#endif

#ifdef INCLUDE_XTS
#include "XTS/aes_xts.h"
#endif

#ifdef INCLUDE_GCM
#include "GCM/aes_gcm.h"
#endif

#ifdef INCLUDE_CCM
#include "CCM/aes_ccm.h"
#endif

#ifdef INCLUDE_CMAC
#include "CMAC/aes_cmac.h"
#endif

#ifdef INCLUDE_KEY_WRAP
#include "KEYWRAP/aes_keywrap.h"
#endif

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
