 /*
  *  objects_cryptocell.h
  *
  *  Copyright (C) 2017, Arm Limited, All Rights Reserved
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

#ifndef MBED_OBJECTS_CRYPTOCELL_H
#define MBED_OBJECTS_CRYPTOCELL_H
#include "sns_silib.h"

typedef struct  trng_s
{
    /* parameters defining TRNG */
    CRYS_RND_mode_t TrngMode;

    /* allowed ring oscillator lengths: bits 0,1,2,3  */
    uint32_t  RoscsAllowed;

    /* sampling interval: count of ring oscillator cycles between
       consecutive bits sampling */
    uint32_t  SubSamplingRatio;

    uint32_t  SubSamplingRatio1;
    uint32_t  SubSamplingRatio2;
    uint32_t  SubSamplingRatio3;
    uint32_t  SubSamplingRatio4;

}CRYS_RND_Params_t;

#endif // MBED_OBJECTS_CRYPTOCELL_H
