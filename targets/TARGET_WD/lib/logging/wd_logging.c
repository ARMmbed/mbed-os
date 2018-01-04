/*
 * wd_logging.c
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

#include "wd_logging.h"

void wd_log(int level, const char *format, va_list args) {
	if (WD_DEBUG_LEVEL_CURRENT < level) {
		return;
	}
	vfprintf(stderr, format, args);
	printf("\r\n");
}

void wd_log_debug(const char *format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);
	wd_log(WD_DEBUG_LEVEL_DEBUG, format, args);
}

void wd_log_info(const char *format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);
	wd_log(WD_DEBUG_LEVEL_INFO, format, args);
}

void wd_log_warn(const char *format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);
	wd_log(WD_DEBUG_LEVEL_WARN, format, args);
}
	
void wd_log_error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	va_end(args);
	wd_log(WD_DEBUG_LEVEL_ERROR, format, args);
}