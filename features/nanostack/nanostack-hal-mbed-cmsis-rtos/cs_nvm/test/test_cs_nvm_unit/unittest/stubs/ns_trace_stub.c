/*
 * Copyright (c) 2014-2016 ARM Limited. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "ns_trace.h"
#include "ip6string.h"
#include "common_functions.h"

#if defined(_WIN32) || defined(__unix__) || defined(__unix) || defined(unix)
#ifndef MEM_ALLOC
#define MEM_ALLOC malloc
#endif
#ifndef MEM_FREE
#define MEM_FREE free
#endif
#else
#include "nsdynmemLIB.h"
#ifndef MEM_ALLOC
#define MEM_ALLOC ns_dyn_mem_alloc
#endif
#ifndef MEM_FREE
#define MEM_FREE ns_dyn_mem_free
#endif
#endif

#define VT100_COLOR_ERROR "\x1b[31m"
#define VT100_COLOR_WARN  "\x1b[33m"
#define VT100_COLOR_INFO  "\x1b[39m"
#define VT100_COLOR_DEBUG "\x1b[90m"

/** default max trace line size in bytes */
#define DEFAULT_TRACE_LINE_LENGTH     1024
/** default max temporary buffer size in bytes, used in
    trace_ipv6, trace_array and trace_strn */
#define DEFAULT_TRACE_TMP_LINE_LEN        128
/** default max filters (include/exclude) length in bytes */
#define DEFAULT_TRACE_FILTER_LENGTH       24

static void default_print(const char *str);

typedef struct {
    /** trace configuration bits */
    uint8_t trace_config;
    /** exclude filters list, related group name */
    char *filters_exclude;
    /** include filters list, related group name */
    char *filters_include;
    /** Filters length */
    int filters_length;
    /** trace line */
    char *line;
    /** trace line length */
    int line_length;
    /** temporary data */
    char *tmp_data;
    /** temporary data array length */
    int tmp_data_length;
    /** temporary data pointer */
    char *tmp_data_ptr;

    /** prefix function, which can be used to put time to the trace line */
    char *(*prefix_f)(size_t);
    /** suffix function, which can be used to some string to the end of trace line */
    char *(*suffix_f)(void);
    /** print out function. Can be redirect to flash for example. */
    void (*printf)(const char *);
    /** print out function for TRACE_LEVEL_CMD */
    void (*cmd_printf)(const char *);
} trace_s;

static trace_s m_trace = {
    .filters_exclude = 0,
    .filters_include = 0,
    .line = 0,
    .tmp_data = 0,
    .prefix_f = 0,
    .suffix_f = 0,
    .printf  = 0,
    .cmd_printf = 0
};

int trace_init(void)
{
    return 0;
}
void trace_free(void)
{

}

void set_trace_config(uint8_t config)
{

}
uint8_t get_trace_config(void)
{
    return 0;
}
void set_trace_prefix_function(char *(*pref_f)(size_t))
{
}

void set_trace_suffix_function(char *(*suffix_f)(void))
{
}

void set_trace_print_function(void (*printf)(const char *))
{
}

void set_trace_cmdprint_function(void (*printf)(const char *))
{
}

void set_trace_exclude_filters(char *filters)
{
}
const char *get_trace_exclude_filters(void)
{
    return NULL;
}

const char *get_trace_include_filters(void)
{
    return NULL;
}

void set_trace_include_filters(char *filters)
{
}

static int8_t trace_skip(int8_t dlevel, const char *grp)
{
    return 0;
}

static void default_print(const char *str)
{
}

void tracef(uint8_t dlevel, const char *grp, const char *fmt, ...)
{

}
const char *trace_last(void)
{
    return "";
}

/* Helping functions */
#define tmp_data_left()  m_trace.tmp_data_length-(m_trace.tmp_data_ptr-m_trace.tmp_data)
char *trace_ipv6(const void *addr_ptr)
{
    return "";
}

char *trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len)
{
    return "";
}

char *trace_array(const uint8_t *buf, uint16_t len)
{
    return "";
}

// rest of debug print functions will be obsolete and will be overridden with new trace interface..
void debugf(const char *fmt, ...)
{
}

void debug(const char *s)
{
}

void debug_put(char c)
{
}

void debug_hex(uint8_t x)
{
}

void debug_int(int i)
{
}

void printf_array(const void *buf , uint16_t len)
{
}

void printf_ipv6_address(const void *addr_ptr)
{
}

void printf_string(const void *ptr, uint16_t len)
{
}

