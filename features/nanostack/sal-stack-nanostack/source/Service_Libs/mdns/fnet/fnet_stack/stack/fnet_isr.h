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
* @brief Private. Interrupt dispatcher API.
*
***************************************************************************/

#ifndef _FNET_ISR_H_

#define _FNET_ISR_H_

#ifndef FNET_ISR_HANDLER
    #define FNET_ISR_HANDLER    fnet_cpu_isr
#endif
#ifndef FNET_ISR_HANDLER_INSTALL
    #define FNET_ISR_HANDLER_INSTALL    fnet_cpu_isr_install
#endif

/* Defines number of the first event handler. MUST be higher than any HW-vector number. */
#define FNET_EVENT_VECTOR_NUMBER (1025)

/************************************************************************
*     Events
*************************************************************************/
typedef fnet_int32_t fnet_event_desc_t;

/************************************************************************
*     Function Prototypes
*************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

fnet_return_t fnet_isr_vector_init( fnet_uint32_t vector_number, void (*handler_top)(fnet_uint32_t cookie), void (*handler_bottom)(fnet_uint32_t cookie), fnet_uint32_t priority, fnet_uint32_t cookie );
fnet_event_desc_t fnet_event_init(void (*event_handler)(fnet_uint32_t cookie), fnet_uint32_t cookie);
void fnet_event_raise(fnet_event_desc_t event_number);
void fnet_isr_vector_release(fnet_uint32_t vector_number);
void fnet_isr_lock(void);
void fnet_isr_unlock(void);
void fnet_isr_init(void);
void fnet_isr_handler(fnet_uint32_t vector_number);
fnet_return_t fnet_cpu_isr_install(fnet_uint32_t vector_number, fnet_uint32_t priority);

#if defined(__cplusplus)
}
#endif

#endif
