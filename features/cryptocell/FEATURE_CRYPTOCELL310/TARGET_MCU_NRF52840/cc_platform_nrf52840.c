 /*
  *  cc_platform_nrf52840.c
  *
  *  Copyright (C) 2018, Arm Limited, All Rights Reserved
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

#include "platform_alt.h"
#include "nrf52840.h"

int cc_platform_setup( cc_platform_ctx *ctx )
{
    NRF_CRYPTOCELL->ENABLE = 1;
    return ( 0 );
}

void cc_platform_terminate( cc_platform_ctx *ctx )
{
    NRF_CRYPTOCELL->ENABLE = 0;
}
