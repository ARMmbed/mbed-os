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

#include "cmsis_os.h"
#include "platform/mbed_stats.h"

void wd_log(int level, const char *format, va_list args) {
	if (WD_DEBUG_LEVEL_CURRENT < level) {
		return;
	}
	mbed_error_vfprintf(format, args);
	mbed_error_printf("\r\n");
}

void wd_log_always(const char *format, ...) {
	va_list args;
	va_start(args, format);
	wd_log(WD_DEBUG_LEVEL_ERROR, format, args);
	va_end(args);
}

void wd_log_debug(const char *format, ...) {
	va_list args;
	va_start(args, format);
	wd_log(WD_DEBUG_LEVEL_DEBUG, format, args);
	va_end(args);
}

void wd_log_info(const char *format, ...) {
	va_list args;
	va_start(args, format);
	wd_log(WD_DEBUG_LEVEL_INFO, format, args);
	va_end(args);
}

void wd_log_warn(const char *format, ...) {
	va_list args;
	va_start(args, format);
	wd_log(WD_DEBUG_LEVEL_WARN, format, args);
	va_end(args);
}
	
void wd_log_error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	wd_log(WD_DEBUG_LEVEL_ERROR, format, args);
	va_end(args);
}

void wd_log_heap_stats(const char *issue){

	if(!WD_DEBUG_ENABLE_MEMORY_STATS){
		return;
	}
	wd_log_heap_stats_internal(issue);
	
}

void wd_log_heap_stats_internal(const char *issue){

	osKernelLock();

	mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    wd_log_info(
        "heap (%s): current=%d, max=%d, reserved=%d, free=%d", 
		issue,
        heap_stats.current_size, 
        heap_stats.max_size, 
        heap_stats.reserved_size,
        heap_stats.reserved_size - heap_stats.max_size
    );
	
	int thread_count = osThreadGetCount();
	mbed_stats_stack_t stacks_stats[thread_count];
	mbed_stats_stack_get_each(stacks_stats, thread_count);
    for(int i = 0; i < thread_count; i++){
		wd_log_info(
			"stack (%s): id=%d, max=%d, reserved=%d, free=%d", 
			issue,
			stacks_stats[i].thread_id,
			stacks_stats[i].max_size, 
			stacks_stats[i].reserved_size,
			stacks_stats[i].reserved_size - stacks_stats[i].max_size
		);
    }

    osKernelUnlock();
}