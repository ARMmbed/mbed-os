# Change Log

## [v1.15.0](https://github.com/ARMmbed/mbed-client/releases/tag/v1.15.0) (06-Sep-2016)
[Full Changelog](https://github.com/ARMmbed/mbed-client/compare/v1.13.4...v1.15.0)

** New feature **

- Introduce new API's to handle block-wise messages in application level 

**Closed issues:**

- IOTCLT-1001 - mDS does not sent RST anymore for Client after DELETE /subscriptions

**Merged pull requests:**

commit ce709e08039407ee69ce1f638543652261742439 (HEAD, tag: v1.15.0, origin/master, origin/HEAD, master)
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Tue Sep 6 18:52:15 2016 +0300

    version v1.15.0

commit a2158dd9abf35fe98ef675ebfb77cd77b5a13747
Merge: 783678e c4bf82c
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Tue Sep 6 18:51:42 2016 +0300

    Merge pull request #278 from ARMmbed/IOTCLT-1001
    
    implement platform agnostic IP handling, Fix IOTCLT-1001

commit c4bf82cc792c9e5f5bc2ddcfc062234c31d586d9
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Tue Sep 6 18:17:22 2016 +0300

    update doxygen in header

commit d4c802be2e43fe60e19b416917d60e7bf4ae7375
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Mon Sep 5 19:34:28 2016 +0300

    fix unittests

commit 0cb20925e00ac4641e7b40c08c9798133d6c0755
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Sat Sep 3 14:20:46 2016 +0300

    remove unnessary assignments

commit 0d7f1080aa2f9c236f9740d6fcbd7e34d8cc9ed7
Author: Teemu Takaluoma <teemu.takaluoma@arm.com>
Date:   Sat Sep 3 13:56:43 2016 +0300

    Fix for IOTCLT-1001, implement platform agnostic IP handling

commit 783678ef926a4e52609b3f7e67eedcf60ff05161
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Sep 6 15:26:36 2016 +0300

    Update documentation to cover external block-wise message storing (#274)
    
    * Update documentation to cover external block-wise message storing

commit 98bb9cf8652d9fa3f7882d5c3d5e15c21a1e55a9 (tag: v1.14.0)
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Sep 6 09:19:08 2016 +0300

    version v1.14.0

commit 32cd77e498dda4cb54688fbab6032634a2ee909c
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Sep 6 09:05:31 2016 +0300

    Introduce new API's to handle block-wise messages in application level (#272)
    
    * Introduce new API's in resource level.
     * set_incoming_block_message_callback(), used when application wants to store block-wise messages in their own memory
     * set_outgoing_block_message_callback, used to read block wise message data from application memory

