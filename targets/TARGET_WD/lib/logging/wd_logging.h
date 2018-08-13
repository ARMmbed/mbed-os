/*
 * wd_logging.h
 *
 * Copyright (C) 2017 world-direct.at, MIT License
 *
 * Created: 21.04.2017
 * Author:	Simon Pfeifhofer
 * EMail:	simon.pfeifhofer@world-direct.at
 *
 * Description:
 *	 Contains logging-helpers.
 *
 */ 

#ifndef WD_LOGGING_H
#define WD_LOGGING_H

#define WD_DEBUG_LEVEL_DISABLED	0
#define WD_DEBUG_LEVEL_ERROR	1
#define WD_DEBUG_LEVEL_WARN		2
#define WD_DEBUG_LEVEL_INFO		3
#define WD_DEBUG_LEVEL_DEBUG	4

#ifndef WD_DEBUG_LEVEL_CURRENT
#define WD_DEBUG_LEVEL_CURRENT	WD_DEBUG_LEVEL_WARN
#endif

#ifndef WD_DEBUG_ENABLE_MEMORY_STATS
#define WD_DEBUG_ENABLE_MEMORY_STATS	0
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
	
void wd_log(int level, const char *format, va_list args);
	
void wd_log_debug(const char *format, ...);
	
void wd_log_info(const char *format, ...);
	
void wd_log_warn(const char *format, ...);
	
void wd_log_error(const char *format, ...);

void wd_log_always(const char *format, ...);

void wd_log_heap_stats(const char *issue);

void wd_log_heap_stats_internal(const char *issue);

#ifdef __cplusplus
}
#endif

#endif //WD_LOGGING_H
