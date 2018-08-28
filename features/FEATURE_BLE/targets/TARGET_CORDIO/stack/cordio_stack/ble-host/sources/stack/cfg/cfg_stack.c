/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Stack configuration.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "cfg_stack.h"
#include "hci_api.h"
#include "dm_api.h"
#include "l2c_api.h"
#include "att_api.h"
#include "smp_api.h"

/**************************************************************************************************
  HCI
**************************************************************************************************/

/**************************************************************************************************
  DM
**************************************************************************************************/

/* Configuration structure */
dmCfg_t dmCfg =
{
  0
};

/* Configuration pointer */
dmCfg_t *pDmCfg = &dmCfg;

/**************************************************************************************************
  L2C
**************************************************************************************************/

/* Configuration structure */
l2cCfg_t l2cCfg =
{
  30                                /* Request timeout in seconds */
};

/* Configuration pointer */
l2cCfg_t *pL2cCfg = &l2cCfg;

/**************************************************************************************************
  ATT
**************************************************************************************************/

/* Configuration structure */
attCfg_t attCfg =
{
  15,                               /* ATT server service discovery connection idle timeout in seconds */
  ATT_DEFAULT_MTU,                  /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,            /* transcation timeout in seconds */
  4                                 /* number of queued prepare writes supported by server */
};

/* Configuration pointer */
attCfg_t *pAttCfg = &attCfg;

/**************************************************************************************************
  SMP
**************************************************************************************************/

/* Configuration structure */
smpCfg_t smpCfg =
{
  3000,                             /* 'Repeated attempts' timeout in msec */
  SMP_IO_NO_IN_NO_OUT,              /* I/O Capability */
  7,                                /* Minimum encryption key length */
  16,                               /* Maximum encryption key length */
  3,                                /* Attempts to trigger 'repeated attempts' timeout */
  0,                                /* Device authentication requirements */
  60000,                            /* Maximum repeated attempts timeout in msec */
  60000,                            /* Time msec before attemptExp decreases */
  2                                 /* Repeated attempts multiplier exponent */
};

/* Configuration pointer */
smpCfg_t *pSmpCfg = &smpCfg;

/*************************************************************************************************/
/*!
*  \brief  Get Stack version number.
*
*  \param  pVersion  output parameter for version number.
*
*  \return None.
*/
/*************************************************************************************************/
void StackGetVersionNumber(const char **pVersion)
{
  *pVersion = STACK_VERSION;
}
