/****************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __S5JS100_ERROR_H__
#define __S5JS100_ERROR_H__

#include <stdio.h>

#ifndef S5JS100_ERROR_RETUN_LOG
#define S5JS100_ERROR_RETUN_LOG 0
#endif

#ifdef S5JS100_ERROR_IS_HALT
#define return_error(ret)       {                                                                   \
    if (S5JS100_ERROR_RETUN_LOG) printf("S5JS100 HALT!!! %s:%d:%s\n", __FILE__, __LINE__, __func__);    \
    while(1);                                                                                       \
    return ret; }
#else
#define return_error(ret)       {                                                                   \
    if (S5JS100_ERROR_RETUN_LOG) printf("Return Error %s:%d:%s ret_value=0x%p\n", __FILE__, __LINE__, __func__, (void *)ret);   \
    return ret; }
#endif

//#define system_halt()     reboot()
#define system_halt()       {                                                                   \
    if (S5JS100_ERROR_RETUN_LOG) printf("S5JS100 HALT!!! %s:%d:%s\n", __FILE__, __LINE__, __func__);    \
    while(1); }

/* Error code */
#undef  EINVAL
#define EINVAL          22      /* Invalid argument */

#endif
