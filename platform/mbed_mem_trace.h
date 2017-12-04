
/** \addtogroup platform */
/** @{*/

/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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

#ifndef __MBED_MEM_TRACE_H__
#define __MBED_MEM_TRACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* Operation types for tracer */
enum {
    MBED_MEM_TRACE_MALLOC,
    MBED_MEM_TRACE_REALLOC,
    MBED_MEM_TRACE_CALLOC,
    MBED_MEM_TRACE_FREE
};

/**
 * \defgroup platform_mem_trace mem_trace functions
 * @{
 */

/* Prefix for the output of the default tracer */
#define MBED_MEM_DEFAULT_TRACER_PREFIX  "#"

/**
 * Type of the callback used by the memory tracer. This callback is called when a memory
 * allocation operation (malloc, realloc, calloc, free) is called and tracing is enabled
 * for that memory allocation function.
 *
 * @param op the ID of the operation (MBED_MEM_TRACE_MALLOC, MBED_MEM_TRACE_REALLOC,
 *           MBED_MEM_TRACE_CALLOC or MBED_MEM_TRACE_FREE).
 * @param res the result that the memory operation returned (NULL for 'free').
 * @param caller the caller of the memory operation. Note that the value of 'caller' might be
 *               unreliable.
 *
 * The rest of the parameters passed 'mbed_mem_trace_cb_t' are the same as the memory operations
 * that triggered its call (see 'man malloc' for details):
 *
 * - for malloc: cb(MBED_MEM_TRACE_MALLOC, res, caller, size).
 * - for realloc: cb(MBED_MEM_TRACE_REALLOC, res, caller, ptr, size).
 * - for calloc: cb(MBED_MEM_TRACE_CALLOC, res, caller, nmemb, size).
 * - for free: cb(MBED_MEM_TRACE_FREE, NULL, caller, ptr).
 */
typedef void (*mbed_mem_trace_cb_t)(uint8_t op, void *res, void* caller, ...);

/**
 * Set the callback used by the memory tracer (use NULL for disable tracing).
 *
 * @param cb the callback to call on each memory operation.
 */
void mbed_mem_trace_set_callback(mbed_mem_trace_cb_t cb);

/**
 * Trace a call to 'malloc'.
 * @param res the result of running 'malloc'.
 * @param size the 'size' argument given to 'malloc'.
 * @param caller the caller of the memory operation.
 * @return 'res' (the first argument).
 */
void *mbed_mem_trace_malloc(void *res, size_t size, void *caller);

/**
 * Trace a call to 'realloc'.
 * @param res the result of running 'realloc'.
 * @param ptr the 'ptr' argument given to 'realloc'.
 * @param size the 'size' argument given to 'realloc'.
 * @param caller the caller of the memory operation.
 * @return 'res' (the first argument).
 */
void *mbed_mem_trace_realloc(void *res, void *ptr, size_t size, void *caller);

/**
 * Trace a call to 'calloc'.
 * @param res the result of running 'calloc'.
 * @param num the 'nmemb' argument given to 'calloc'.
 * @param size the 'size' argument given to 'calloc'.
 * @param caller the caller of the memory operation.
 * @return 'res' (the first argument).
 */
void *mbed_mem_trace_calloc(void *res, size_t num, size_t size, void *caller);

/**
 * Trace a call to 'free'.
 * @param ptr the 'ptr' argument given to 'free'.
 * @param caller the caller of the memory operation.
 */
void mbed_mem_trace_free(void *ptr, void *caller);

/**
 * Default memory trace callback. DO NOT CALL DIRECTLY. It is meant to be used
 * as the second argument of 'mbed_mem_trace_setup'.
 *
 * The default callback outputs trace data using 'printf', in a format that's
 * easily parsable by an external tool. For each memory operation, the callback
 * outputs a line that begins with "#<op>:<0xresult>;<0xcaller>-":
 *
 * @param op        identifies the memory operation ('m' for 'malloc', 'r' for 'realloc',
 *                  'c' for 'calloc' and 'f' for 'free').
 * @param res       (base 16) is the result of the memor operation. This is always NULL
 *                  for 'free', since 'free' doesn't return anything.
 * @param caller    (base 16) is the caller of the memory operation. Note that the value
 *                  of 'caller' might be unreliable.
 *
 * The rest of the output depends on the operation being traced:
 *
 * - for 'malloc': 'size', where 'size' is the original argument to 'malloc'.
 * - for 'realloc': '0xptr;size', where 'ptr' (base 16) and 'size' are the original arguments to 'realloc'.
 * - for 'calloc': 'nmemb;size', where 'nmemb' and 'size' are the original arguments to 'calloc'.
 * - for 'free': '0xptr', where 'ptr' (base 16) is the original argument to 'free'.
 *
 * Examples:
 *
 * - "#m:0x20003240;0x600d-50" encodes a 'malloc' that returned 0x20003240, was called
 *   by the instruction at 0x600D with a the 'size' argument equal to 50.
 * - "#f:0x0;0x602f-0x20003240" encodes a 'free' that was called by the instruction at
 *   0x602f with the 'ptr' argument equal to 0x20003240.
 */
void mbed_mem_trace_default_callback(uint8_t op, void *res, void *caller, ...);

/** @}*/

#ifdef __cplusplus
}
#endif

#endif// #ifndef __MBED_MEM_TRACE_H__


/** @}*/
