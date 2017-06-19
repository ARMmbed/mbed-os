Library Timer API
=================
This chapter describes the timer functionality.

If an application requires a timer, it can allocate one with the `eventOS_event_timer_request_in( )` family of functions. Further to a successful allocation, the system timer event occurs.

Times are specified in ticks, which are currently 10ms, but use macros and converter functions in the header to avoid depending on this.

See the [Eventing API](08_API_events.md) chapter for more detail on events.

<span class="notes">**Note**: The library supports dynamic timer count at the application layer. The stack can multiplex a single timer for multiple purposes.</span>

## API Headers

To use Library Timer API, include the following header in your application:

```
#include eventOS_event_timer.h
```

## Read the current time

To read the current time:

```
uint32_t eventOS_event_timer_ticks(void)
```

<dl>
<dt>Return value</dt>
<dd>Current time in ticks.</dd>
</dl>

## Allocate timer

To allocate timer events:

```
arm_event_storage_t *eventOS_event_timer_request_at
(
	const arm_event_t *event
	unt32_t		at
)
```

Parameter|Description
---------|-----------
`event`|A pointer to an event to be sent.
`at`|The time to send the event.

<dl>
<dt>Return value</dt>
<dd>A pointer to event structure, if the timer allocation was successful.</dd>
<dd>NULL if the timer allocation failed.</dd>
</dl>

At the specified time, the event OS library will transmit the specified event.

```
arm_event_storage_t *eventOS_event_timer_request_in
(
	const arm_event_t *event
	int32_t		in
)
```

Parameter|Description
---------|-----------
`event`|A pointer to an event to be sent.
`in`|The requested period in ticks.

<dl>
<dt>Return value</dt>
<dd>A pointer to event structure, if the timer allocation was successful.</dd>
<dd>NULL if the timer allocation failed.</dd>
</dl>

Within the chosen time period, the event OS library will transmit the specified event. Requesting two ticks means that
the event will trigger on the second tick from now. The elapsed time between requesting and sending is 1-2 ticks.

```
arm_event_storage_t *eventOS_event_timer_request_after
(
	const arm_event_t *event
	int32_t		after
)
```

Parameter|Description
---------|-----------
`event`|A pointer to an event to be sent.
`after`|The requested period in ticks.

<dl>
<dt>Return value</dt>
<dd>A pointer to event structure, if the timer allocation was successful.</dd>
<dd>NULL if the timer allocation failed.</dd>
</dl>

Within the chosen time period, the event OS library will transmit the specified event. Requesting two ticks means
the event will trigger on the second tick from now. The elapsed time between requesting and sending is 1-2 ticks.

```
arm_event_storage_t *eventOS_event_timer_request_every
(
	const arm_event_t *event
	int32_t		every
)
```

Parameter|Description
---------|-----------
`event`|A pointer to an event to be sent.
`every`|The requested period in ticks.

<dl>
<dt>Return value</dt>
<dd>A pointer to event structure, if the timer allocation was successful.</dd>
<dd>NULL if the timer allocation failed.</dd>
</dl>

Every chosen time period, the event OS library will transmit the specified event.

## Cancel timer

To cancel allocated timer id:

```
int8_t eventOS_event_timer_cancel
(
	uint8_t		event_id,
	int8_t		tasklet_id
)
```

Parameter|Description
---------|-----------
`event_id`|The event ID of the event to be cancelled.
`tasklet_id`|The tasklet ID of the event receiver.

<dl>
<dt>Return value</dt>
<dd>0 If the timer cancellation was successful.</dd>
<dd>-1 If the timer cancellation failed, timer ID is invalid or the timer is already released.</dd>
</dl>

To cancel an periodic event or event by its `arm_event_storage_t *` reference, use `eventOS_cancel()` API. Refer to [Eventing API](08_API_events.md).

## Millisecond timeout API

Timeout API provides simpler API for calling functions periodically or after a timeout.

Two forms of timeouts are provided, periodic one and single-shot.
```
timeout_t *eventOS_timeout_ms(void (*callback)(void *), uint32_t ms, void *arg);
timeout_t *eventOS_timeout_every_ms(void (*callback)(void *), uint32_t every, void *arg);
```

Parameter|Description
---------|-----------
`callback`|A pointer to the function to be called.
`ms` or `every`|The timeout or period in milliseconds.
`arg`|An argument to be used in the function call.

<dl>
<dt>Return value</dt>
<dd>Reference to timeout structure.</dd>
<dd>NULL on failure.</dd>
</dl>

The `timeout_t` reference can be used to cancel an event before it has happened, or to cancel an periodically called event.

After a single shot event, or after cancelling the periodic one, the reference to `timeout_t` is invalid and should not be used.

To cancel the event, use:

```
void eventOS_timeout_cancel(timeout_t *t);
```
Where `timeout_t *t` is the reference returned to you by `eventOS_timeout_ms()` or `eventOS_timeout_every_ms()`.

After cancelling the event, the handle is internally freed and becomes invalid and should not be used.
