Library Timer API
=================
This chapter describes the timer functionality.

If an application requires a timer, it can allocate one with the `eventOS_event_timer_request( )` function. Further to a successful allocation, the system timer event occurs.

<span class="notes">**Note**: The library supports dynamic timer count at the application layer. The stack can multiplex a single timer for multiple purposes.</span>

## API Headers

To use Library Timer API, include the following header in your application:

```
#include eventOS_event_timer.h
``` 

## Allocate timer

To allocate timer events:

```
int8_t eventOS_event_timer_request
(
	uint8_t		snmessage,
	uint8_t		event_type,
	int8_t		tasklet_id,
	uint32_t	time
)
```

Parameter|Description
---------|-----------
`snmessage`|The timer ID defined by the application.
`event_type`|The event type to be sent to a tasklet when the timer expires. Usually `ARM_LIB_SYSTEM_TIMER_EVENT`.
`tasklet_id`|The tasklet ID of the event receiver.
`time`|The requested period in milliseconds. Resolution is 10ms.

<dl>
<dt>Return value</dt>
<dd>0 If timer allocation was successful.</dd>
<dd>-1 If timer allocation failed.</dd>
</dl>

Further to the allocated time period, the event OS library will transmit an event of which the sender, event type `(event->event_type)` and `event->event_id`, is indicating allocated timer identification.

## Release timer

To cancel allocated timer events:

```
int8_t eventOS_event_timer_cancel
(
	uint8_t		snmessage,
	int8_t		tasklet_id
)
```

Parameter|Description
---------|-----------
`snmessage`|The timer ID of the event to be cancelled.
`tasklet_id`|The tasklet ID of the event receiver.

<dl>
<dt>Return value</dt>
<dd>0 If timer release was successful.</dd>
<dd>-1 If timer release failed, timer ID is invalid or the timer is already released.</dd>
</dl>

