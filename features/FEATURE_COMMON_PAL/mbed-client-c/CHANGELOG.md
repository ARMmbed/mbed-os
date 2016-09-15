# Change Log

## [v3.0.1](https://github.com/ARMmbed/mbed-client-c/releases/tag/v3.0.1) (15-Sep-2016)
[Full Changelog](https://github.com/ARMmbed/mbed-client-c/compare/mbed-os-5.0-rc1...v3.0.1)

** New feature **

- Creates a new sn_nsdl_put_resource function, which takes ownership of sn_nsdl_resource_info_s resource object (not members) instead of creationg copy of it
- Rearranged struct member variable to pack better into memory
- Refactored coap_options_list to treat number members as numbers instead of strings
- Add option to pass incoming blocks to application instead storing them internally
- Added API for deleting one message from resending

**Closed issues:**

- IOTCLT-1001 - mDS does not sent RST anymore for Client after DELETE /subscriptions

**Merged pull requests:**

commit 7f0eaea060e1274f5ea6e239960a74fecb8a075d (HEAD, tag: v3.0.1, origin/master, origin/HEAD, master)
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 15 11:22:59 2016 +0300

    version v3.0.1

commit 82677cffcafb06111b81f750782a9f5f5dc2fd21
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 15 11:22:07 2016 +0300

    Changed bool type to unsigned int

commit e704a90cee81df0b5ee4a6839819ec4f946aad32 (tag: v3.0.0)
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 15 10:30:56 2016 +0300

    version v3.0.0

commit b273d51fd6f38b1938d8e2a942805dfdde3a59ef
Merge: 45e836b 4fdf010
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 15 10:25:34 2016 +0300

    Merge pull request #75 from ARMmbed/coap-option-tidy
    
    Coap option tidy

commit 4fdf010727d8bf45a6e2041b1202dbf7685b7c1e (origin/coap-option-tidy)
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 15 10:18:53 2016 +0300

    Remove commented out code

commit 60a92da6e9315740cff41ec301895c8803a480c0
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Sep 14 15:14:28 2016 +0300

    Fix payload size calculation when blockwise is needed

commit ac4dcda4d59821a6434205b26b4a4ea88e37f7b4
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Wed Sep 14 10:00:09 2016 +0300

    Blockwise transfer handling improved, fixes issue IOTCLT-1004
    + Unittests updated

commit 2352759cb8b927bec229d4ac1bbe83472ceb39f2
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Mon Sep 12 16:41:46 2016 +0300

    Added sn_coap_parser_alloc_options function to create list with default values
    
    Updated unittests

commit d5bf80d6d2374db924111c47d3e7f5cd45060ca3
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Wed Sep 7 14:22:52 2016 +0300

    Handle Size1/Size2 options as numbers
    
    This will simplify the handling of sizeX options.
    Unittests have been updated also

commit 58695b71ee218df4053344e5ca26accc894338ee
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Fri Sep 2 16:29:30 2016 +0300

    Added put method, rearranged structs, updated unittests

commit 754f4c015d675e5e0d7ef777e914df3910bb7320
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 1 16:08:13 2016 +0300

    Unittests updated & small fixes

commit 626ac227c3de1837b7667c69087f5659dfdbd436
Author: Kevin Bracey <kevin.bracey@arm.com>
Date:   Mon Jun 22 12:58:08 2015 +0300

    Treat Block2/Block1 options as numbers
    
    Rather than treat them as arbitary byte strings, process them as numbers.
    This greatly simplifies a lot of the Block-wise transfer code.
    
    No public API change - users shouldn't be touching these options.
    
    Change-Id: I67f9eb4ff2610aeb0d7f7638dfb28c279f916889
    
    Conflicts:
        nsdl-c/sn_coap_header.h
        source/libCoap/src/sn_coap_builder.c
        source/libCoap/src/sn_coap_parser.c
        source/libCoap/src/sn_coap_protocol.c

commit acf48a4026304705797bc0c12382994d160afa37
Author: Kevin Bracey <kevin.bracey@arm.com>
Date:   Mon Jun 22 12:54:14 2015 +0300

    Treat Observe option as a number
    
    Treat Observe option as a number, rather than a byte string. This is an
    incompatible API change to both libCoap and libNsdl.
    
    Change-Id: I115afa988afbc239b45adfd7a21a33b2ee3bc42b
    
    Conflicts:
        nsdl-c/sn_nsdl_lib.h
        source/libCoap/src/sn_coap_builder.c
        source/libCoap/src/sn_coap_protocol.c

commit 48b5b2cb9fc4341656de54c38d10589d8fcc9db4
Author: Kevin Bracey <kevin.bracey@arm.com>
Date:   Mon Jun 22 12:37:58 2015 +0300

    Treat Uri-Port as a number
    
    Treat the Uri-Port option as a number, rather than a byte string.
    This is an incompatible API change for libCoap.
    
    Change-Id: I5e04f37cbe1f67a0cafa03befb746d6abff6f790
    
    Conflicts:
        source/libCoap/src/sn_coap_builder.c

commit 9fa86767bd4efa7810223683d37df03a2ad4dd85
Author: Kevin Bracey <kevin.bracey@arm.com>
Date:   Mon Jun 22 12:34:06 2015 +0300

    Treat Max-Age as a number
    
    Treat the Max-Age option as a number, rather than a byte string. This is
    an incompatible API change for libCoap.
    
    Change-Id: I6abdbecb093f5ffa6ec009d591737592ca91e197
    
    Conflicts:
        source/libCoap/src/include/sn_coap_protocol_internal.h
        source/libCoap/src/sn_coap_builder.c
        source/libCoap/src/sn_coap_protocol.c

commit e582e8ad318b469aebd5d6d59032fe4c18fde435
Author: Kevin Bracey <kevin.bracey@arm.com>
Date:   Mon Jun 22 12:11:12 2015 +0300

    Treat Accept as a number
    
    In line with Content-Format, treat the Accept option as a number.
    This is an incompatible API change for libCoap.
    
    This drops any ability to handle multiple Accept options - this existed
    in early versions of CoAP, but was removed in draft 13.
    
    Change-Id: I5d0233bb01dfff7d944f5dcb1c0ec6bec88b710e
    
    Conflicts:
        source/libCoap/src/sn_coap_builder.c
        source/libCoap/src/sn_coap_parser.c

commit 3bb5696d119a05323bbda3aaa8460bd56bef32ac
Author: Kevin Bracey <kevin.bracey@arm.com>
Date:   Mon Jun 22 11:32:42 2015 +0300

    Treat Content-Format as a number
    
    Treat the Content-Format option as a number - this is an incompatible API
    change to libCoap.
    
    Clients will now correctly handle Content-Formats > 255, or Content-Format
    options with zero-padding.
    
    NSDL-C seems to have some oddities in its Content-Format handling - it has
    some lingering assumptions about "0=text/plain" being the default, which
    is no longer true in the final CoRE/CoAP RFCs. This should be looked at;
    no functional change is made to this by this commit.
    
    Change-Id: I4c25741964e562178b31508d30184b68783eadc2
    
    Conflicts:
        nsdl-c/sn_nsdl_lib.h
        source/libCoap/src/sn_coap_builder.c

commit 46b1bcf756ae5523e32a09612a0b049d0cdb62fa
Author: Kevin Bracey <kevin.bracey@arm.com>
Date:   Mon Jun 22 09:50:33 2015 +0300

    Add functions to build and parse "uint" options
    
    Many CoAP options are variable-length integers - create some
    infrastructure to manipulate these by value, rather than treating as
    byte strings.
    
    Change-Id: I0cf5c2b62d1745ca349f70bb272f726ead86ac0a
    
    Conflicts:
        source/libCoap/src/sn_coap_builder.c

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
