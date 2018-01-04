#ifndef PPPOS_UTILS_H
#define PPPOS_UTILS_H

#include <inttypes.h>

typedef struct pppos_context {
	void* serial_stream_adapter;
	int(*pppos_write_callback)(struct pppos_context* ctx, uint8_t* buf, size_t length, uint32_t timeout);
} pppos_context_t;

#endif //PPPOS_UTILS_H