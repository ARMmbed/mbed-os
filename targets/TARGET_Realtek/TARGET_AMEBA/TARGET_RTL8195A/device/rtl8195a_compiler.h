/*
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
 */
#ifndef MBED_RTL8195A_COMPILER_H
#define MBED_RTL8195A_COMPILER_H

#if defined (__ICCARM__)
#ifndef STRINGIFY
#define STRINGIFY(a) #a
#endif
#ifndef SECTION
#define SECTION(_name)      _Pragma( STRINGIFY(location=##_name##))
#endif
#ifndef ALIGNMTO
#define ALIGNMTO(_bound)    _Pragma( STRINGIFY(data_alignment=##_bound##))
#endif
#define __romcall
#define __longcall

#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)

#ifndef __longcall
#define __longcall
#endif

#elif defined(__CC_ARM)

#ifndef __longcall
#define __longcall          __attribute__ ((long_call))
#endif

#else
#define SECTION(_name)      __attribute__ ((__section__(_name)))
#define ALIGNMTO(_bound)    __attribute__ ((aligned (_bound)))

#ifndef __packed
#define __packed            __attribute__ ((packed))
#endif

#ifndef __romcall
#define __romcall           __attribute__ ((long_call))
#endif
#ifndef __longcall
#define __longcall          __attribute__ ((long_call))
#endif
#ifndef __weak
#define __weak              __attribute__ ((weak))
#endif

#endif

#endif
