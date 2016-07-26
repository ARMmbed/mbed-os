# Platform API

Platform API is the base where the 6LoWPAN stack runs. It is the separation between the actual hardware or operating system and the 6LoWPAN stack itself. This is the first layer to study to port the 6LoWPAN stack to a new platform.

The most important parts of Platform API are the timers and critical section functions. These are required for the event-based stack to start up and run.

## API Headers

```
#include arm_hal_interrupt.h  
#include arm_hal_timer.h
#include eventOS_scheduler.h
#include arm_hal_random.h
#include arm_hal_aes.h
```

_Table 3-1_ lists the scopes of the Platform API functions.

**Table 3-1 Scopes of Platform API functions**


Scope|API responsibility
-----|------------------
Critical section|The 6LoWPAN stack uses the Platform API to signal that a device is about to enter or exit a critical section where interruptions are not tolerated.<br><b>Functions:</b><br><code>platform_enter_critical()</code><br><code>platform_exit_critical()</code>
Idle and sleep functions|The 6LoWPAN stack signals a possible idle or sleep condition. This part of the API is used to manage the power consumption of a processor core and radio drivers.<br><b>Functions prefixed with:</b><br><code>eventOS_scheduler</code>
Random number generation|The 6LoWPAN stack relies on platform drivers to generate random numbers.<br><b>Functions prefixed with:</b><br><code>platform_random</code>
Timers|The eventing system requires timers to be implemented on platform drivers.<br><b>Functions prefixed with:</b><br><code>eventOS_event_timer</code>
AES encryption|To use security on a network layer, AES encryption must be implemented on platform drivers. This allows using hardware encryption where available. ARM supports sample software implementation for platforms without HW encryption. <code>platform_aes_block_encode()</code>

For more information on Platform API, see the chapter [**Porting 6LoWPAN Stack**](16_API_porting.md).
