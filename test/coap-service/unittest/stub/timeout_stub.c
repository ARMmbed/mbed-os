//todo:is this right place, or should this be located to event-loop modules stubs?


#include "ns_types.h"
#include "eventOS_event_timer.h"

timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg)
{
	return NULL;
}

void eventOS_timeout_cancel(timeout_t *t)
{

}
