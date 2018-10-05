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
#ifndef _CC_CONFIG_TRNG90B_H
#define _CC_CONFIG_TRNG90B_H

/*
This file should be updated according to the characterization process.
*/

/* sample count for each ring oscillator */
/* for unallowed rosc, sample count = 0 */
#define CC_CONFIG_SAMPLE_CNT_ROSC_1		200
#define CC_CONFIG_SAMPLE_CNT_ROSC_2		200
#define CC_CONFIG_SAMPLE_CNT_ROSC_3		300
#define CC_CONFIG_SAMPLE_CNT_ROSC_4		100

#endif

