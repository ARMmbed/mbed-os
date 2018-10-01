/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
#ifndef __PLATFORM_STDLIB_H__
#define __PLATFORM_STDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_PLATFORM_8195A)+\
		defined(CONFIG_PLATFORM_8711B)+\
		defined(CONFIG_PLATFORM_8721D)+\
		defined(CONFIG_PLATFORM_8195BHP)+\
		defined(USE_STM322xG_EVAL)+\
		defined(USE_STM324xG_EVAL)+\
		defined(STM32F10X_XL) > 1
		#error "Cannot define two or more platform at one time"
#endif

#if defined(CONFIG_PLATFORM_8195A)
	#include "platform_stdlib_rtl8195a.h"
#elif defined (CONFIG_PLATFORM_8711B)
	#include "platform_stdlib_rtl8711b.h"
#elif defined (CONFIG_PLATFORM_8721D)
	#include "platform_stdlib_rtl8721d.h"
#elif defined(CONFIG_PLATFORM_8195BHP)
	#include "platform_stdlib_rtl8195bhp.h"	
#elif defined(USE_STM322xG_EVAL) || defined(USE_STM324xG_EVAL)  || defined(STM32F10X_XL) 
	#include "platform_stdlib_stm32.h"
#elif defined (CONFIG_PLATFORM_8710C)
	#include "platform_stdlib_rtl8710c.h"
#else
	#error "Undefined Platform stdlib"
#endif

#if (CONFIG_PLATFORM_AMEBA_X == 0)
#include "basic_types.h"
#endif

#ifdef __cplusplus
}
#endif
#endif //__PLATFORM_STDLIB_H__
