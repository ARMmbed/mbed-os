/**************************************************************************
*
* Copyright (c) 2017, Pelion and affiliates.
* SPDX-License-Identifier: Apache-2.0
* Copyright 2011-2016 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
*
***************************************************************************
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
**********************************************************************/ /*!
*
* @file fnet_config.h
*
* @author Andrey Butok
*
* @brief Main FNET default configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_CONFIG_H_

#define _FNET_CONFIG_H_

/* !! Do not change the order !! */

#ifndef __DOXYGEN__
    #include "fnet_user_config.h"               /* User configuration. Highest priority. */
#else
    #include "fnet_doxygen_user_config.h"       /* Configuration used during generation of documentation.*/
#endif

#include "port/compiler/fnet_comp_config.h"      /* Default compiler specific configuration. */
#include "stack/fnet_stack_config.h"        /* Default TCP/IP stack configuration. */
#include "services/fnet_services_config.h"  /* Default services configuration. */

// Add nanomesh data types
#include "ns_fnet_types.h"

#endif /* _FNET_CONFIG_H_ */
