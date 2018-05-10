/**************************************************************************
*
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
**********************************************************************/
/*!
* @brief Private. FNET Timers API.
*
***************************************************************************/

#ifndef _FNET_TIMER_PRV_H_

#define _FNET_TIMER_PRV_H_

#include "fnet_timer.h"

/* SW Timer descriptor.*/
typedef void *fnet_timer_desc_t;

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

fnet_return_t fnet_timer_init( fnet_time_t period_ms );
void fnet_cpu_timer_release( void );
void fnet_timer_release( void );
void fnet_timer_reset_all( void );
fnet_timer_desc_t fnet_timer_new( fnet_time_t period_ticks, void (*handler)( fnet_uint32_t cookie ), fnet_uint32_t cookie );
void fnet_timer_free( fnet_timer_desc_t timer );
void fnet_timer_ticks_inc( void );
void fnet_timer_handler_bottom(fnet_uint32_t cookie);
fnet_return_t fnet_cpu_timer_init( fnet_time_t period_ms );

#if defined(__cplusplus)
}
#endif

#endif
