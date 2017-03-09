# Change Log

## [mbed-os.5.2-rc1](https://github.com/ARMmbed/sal-stack-nanostack-eventloop/releases/tag/mbed-os.5.2-rc1) (24-Sep-2016)
[Full Changelog](https://github.com/ARMmbed/sal-stack-nanostack-eventloop/compare/mbed-os-5.0-rc1...mbed-os.5.2-rc1)

**New features**

- Move configuration parameter resolution to separate header and allow user definable configuration header to be used.
- Port eventloop system timer to use new tick timer functions platform_tick_timer_register, platform_tick_timer_start and platform_tick_timer_stop.

**Merged pull requests:**

commit 283299dbd744c91cb30810ad263798842905c054 (HEAD, tag: mbed-os.5.2-rc1, origin/master, origin/HEAD, master)
Author: Jaakko Kukkohovi <jkukkohovi@gmail.com>
Date:   Fri Sep 23 10:05:55 2016 +0300

    Change configuration method (#23)
    
    Move configuration parameter resolution to separate header and allow
    user definable configuration header to be used.

commit 269b00fba6a637c363f7d1690db0889118bca010
Author: Jaakko Kukkohovi <jkukkohovi@gmail.com>
Date:   Wed Sep 21 11:27:40 2016 +0300

    Systimer using platform tick timer (#22)
    
    * Port eventloop system timer to use new tick timer functions platform_tick_timer_register, platform_tick_timer_start and platform_tick_timer_stop.Existing functionality is preserved by implementing these functions by default using the high resolution timer as before
    * New configuration parameter use_platform_tick_timer disables the high resolution timer based tick timer implementation and allows platform to implement the tick timer functions.
    * New configuration parameter exclude_highres_timer removes the high resolution timer from the build thus providing only eventloop.
    * Minar support now implemented by providing platform tick timer using minar timers and implicitly using platform tick timers for eventloop when using minar

