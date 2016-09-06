# Change Log

## [v2.9.0](https://github.com/ARMmbed/mbed-client-c/releases/tag/v2.9.0) (05-Sep-2016)
[Full Changelog](https://github.com/ARMmbed/mbed-client-c/compare/mbed-os-5.0-rc1...v2.9.0)

** New feature **

- Add option to pass incoming blocks to application instead storing them internally
- Added API for deleting one message from resending

**Closed issues:**

- IOTCLT-1001 - mDS does not sent RST anymore for Client after DELETE /subscriptions

**Merged pull requests:**

commit 45e836b9a7b9a46dc9a97c7aae5570cb00f2d284 (HEAD, tag: v2.9.0, origin/master, origin/HEAD, master)
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Mon Sep 5 15:27:34 2016 +0300

    version v2.9.0

commit 9022e05adb231fd8f619bb4fc2b0f05da99413a9
Merge: ec0b281 87dfd6f
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Mon Sep 5 15:26:26 2016 +0300

    Merge pull request #73 from ARMmbed/IOTCLT-1001
    
    implement platform agnostic IP handling, Iotclt 1001

commit 87dfd6f11fc5a8a724e090e34540a8fc9bf8b290
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Mon Sep 5 08:51:21 2016 +0300

    add unit test

commit b8ddefdcbfb58d355e170e8411eebc23ffe3c25c
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Sat Sep 3 14:14:28 2016 +0300

    refactor address handling

commit d89b6266fd52322fd7beafcd51365c980afbff05
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Sat Sep 3 14:10:31 2016 +0300

    Fix for IOTCLT-1001, implement platform agnostic IP handling

commit ec0b281264f4859f28cfe207b89816835c56f392 (tag: v2.8.0)
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Fri Aug 12 14:33:56 2016 +0300

    version v2.8.0

commit 5ab0f01c31d6551547388307e0c0cfda272b611e
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Fri Aug 12 14:30:24 2016 +0300

    Add option to pass incoming blocks to application instead storing them internally (#71)
    
    * Add option to pass incoming blocks to application. If external_memory_block variable is set blocks are passed into application and not stored anymore internally.

commit 168739c81618d105bfb3ccf77d14b0d38340ba6b
Merge: 758fd1e fbe4eeb
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Aug 10 12:49:42 2016 +0300

    Merge pull request #65 from ARMmbed/retransmission_delete
    
    Added API for deleting one message from resending

commit fbe4eeb8db1c7ed107dc3da6fd2fa5e3e9062ccb (origin/retransmission_delete)
Author: Tero Heinonen <tero.heinonen@arm.com>
Date:   Fri Aug 5 11:48:17 2016 +0300

    Added API for deleting one message from resending
    queue.
