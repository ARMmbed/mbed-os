#ifndef __EVENTOS_EVENT_STUB_H__
#define __EVENTOS_EVENT_STUB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "eventOS_event.h"

typedef struct {
    void (*event_ptr)(arm_event_s *);
    int8_t int8_value;
} eventOs_event_stub_def;

extern eventOs_event_stub_def eventOs_event_stub;

#ifdef __cplusplus
}
#endif

#endif // __EVENTOS_EVENT_STUB_H__
