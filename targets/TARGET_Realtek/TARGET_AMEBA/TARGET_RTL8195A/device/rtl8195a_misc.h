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
#ifndef MBED_RTL8195A_MISC_H
#define MBED_RTL8195A_MISC_H

// Interface to ROM functions
extern __longcall void *_memset( void *s, int c, size_t n);
extern __longcall void *_memcpy( void *s1, const void *s2, size_t n );
extern __longcall int _memcmp( const void *av, const void *bv, size_t len);
extern __longcall size_t _strlen(const char *s);
extern __longcall int _strcmp(const char *cs, const char *ct);

#define __memset     _memset
#define __memcpy     _memcpy
#define __memcmp     _memcmp
#define __strlen     _strlen
#define __strcmp     _strcmp

#endif
