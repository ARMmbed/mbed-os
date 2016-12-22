# Change Log

## [v4.0.0](https://github.com/ARMmbed/mbed-client-c/releases/tag/v3.0.2) (21-dec-2016)
[Full Changelog](https://github.com/ARMmbed/mbed-client-c/compare/v3.0.2...v4.0.0)

**New feature**

- Added user definable void* context field to nsdl handle for passing context to callbacks
- CoAP message ID randomization
- Removed OMA LWM2M specific functionality that's implemented in mbed-client
- Optimized memory usage of nsdl resource structures by splitting it into dynamic and static parts and removing unnecessary fields
- Added API sn_grs_pop_resource for removing resource from nsdl grs internal list

**Closed issues:**

- IOTCLT-1207 - sn_coap_builder_options_build_add_uint_option function produce wrong option value
- IOTCLT-828 / #59 - Random CoAP message ID
 
**Merged pull requests:**

commit bad16be2eb5345e8eabd85f6a60d5a3fc84fb53c (HEAD -> release-4.0, tag: v4.0.0, origin/release-4.0)
Author: Jaakko Kukkohovi <jaakko.kukkohovi@arm.com>
Date:   Thu Dec 22 13:20:33 2016 +0200

    version v4.0.0

commit 858d9f63a6574f8c80e23ab433518a51d0ef3468 (origin/memory_optimizations, memory_optimizations)
Merge: 63a12d3 a5e35ef
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 22 11:17:30 2016 +0200

    Merge pull request #110 from ARMmbed/counterpart_of_put_api
    
    nsdl: add sn_grs_pop_resource function

commit a5e35efcf5b25d132ff96dfddbc7fdf46dfb1eda
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 22 10:09:57 2016 +0200

    nsdl: add sn_grs_pop_resource function
    
    The sn_grs_put_resource() needs a counterpart for removing
    the structure from the list. This makes it possible to manage
    the lifetime of nsdl structures from client code.

commit 63a12d3422755641eab3db2113741e68983df8a9
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Dec 20 16:02:05 2016 +0200

    sn_grs: add more debug prints when processing coap message

commit 56cab3e967f5c264c4b13a10cf34bbee5b937cda
Merge: 650bc00 b1d33e2
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Mon Dec 19 12:15:20 2016 +0200

    Merge pull request #108 from ARMmbed/if_change
    
    Remove unnecessary if clause in sn_grs_mark_resources_as_registered

commit 650bc002767a50a7faddc0abab3fcebaaa5509b2
Merge: c3a9e85 00e4465
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Mon Dec 19 11:24:22 2016 +0200

    Merge pull request #107 from ARMmbed/remove_asserts
    
    Remove assert macro usage due to comments in review

commit b1d33e2bb2ca9762e23d06c8c49a718fbc3449af
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Fri Dec 16 12:18:10 2016 +0200

    Remove unnecessary if clause in sn_grs_mark_resources_as_registered

commit 00e4465ad39d2e00db6775197d1aa71e68e4cb30
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Fri Dec 16 11:09:47 2016 +0200

    Remove assert macro usage due to comments in review
    
    Remove assert() for now, let's bring it back later when we
    have better platform and test framework support for it.

commit c3a9e85687c3c2f2d3343892a704a465c9b29fa4
Merge: a95da18 08c0820
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 15 15:30:06 2016 +0200

    Merge pull request #106 from ARMmbed/unit_test_fixes
    
    Update nsdl and grs unit tests to work with memory optimization changes

commit a95da18263ef2fe41bdc9cdd172df3c0fbe888e3
Merge: 2dd3528 5fb08ad
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 15 15:24:45 2016 +0200

    Merge pull request #105 from ARMmbed/assert_fix
    
    Fix possible null pointer access in assert calls.

commit 08c08203fb0734a12a85ac0f25a0d7a6e6b93859
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Dec 15 15:00:48 2016 +0200

    Update nsdl and grs unit tests to work with memory optimization changes
    build_message_options_uri_path unit tests updated due to commit https://github.com/ARMmbed/mbed-client-c/pull/101/commits/91af549eacf1334df829430b9cb29b305fc73792

commit 5fb08ad357e83ad3f42d8a00a55fdacbd7da3e8c
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Dec 15 14:28:35 2016 +0200

    * Fix possible null pointer access in assert calls.
    * Assert not needed in sn_nsdl_build_registration_body since params are checked already when adding resource to list.

commit 2dd3528def9618123665f08c057fc69871f1aa0f
Merge: fd987a9 91af549
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Dec 14 14:09:00 2016 +0200

    Merge pull request #104 from ARMmbed/reset_rebased
    
    Revert 3b172c6 Notify client when observation is cancelled with RESET…

commit 91af549eacf1334df829430b9cb29b305fc73792
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Dec 14 14:07:59 2016 +0200

    Revert 3b172c6 Notify client when observation is cancelled with RESET message
    
    Reset message is now passed into application and mbed-client will use the token to match request to right object.
    
    With this change we get rid of unnecessary uri_path() API in mbed-client side.

commit fd987a91c5a33560ffb8a02b1bb579ece2d7289a
Author: Jaakko Kukkohovi <jaakko.kukkohovi@arm.com>
Date:   Mon Dec 12 13:18:26 2016 +0200

    Change bitfields to unsigned (int) (#100)
    
    Use unsigned int for bitfields for C99 compliance

commit 3d2ef058682942f058a1b808280b14da2b0715a4
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Fri Dec 9 13:33:40 2016 +0200

    Move access parameter to dynamic struct

commit e21287d8a4a37c5070f0e346b00f46ccddfa7945
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 8 19:55:35 2016 +0200

    static structs: reorganize the 16 bit variables to avoid padding
    
    Removal of previous field left variables not naturally aligned,
    so we need to reorganize them to avoid padding.

commit ffd3c09e4c0451f678f0e613bad552172a82d3f1
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 8 19:42:45 2016 +0200

    static structs: remove the separate interface_description_len field
    
    There is no need to have separate interface_description_len field,
    if the string is zero terminated. This saves some memory and eases
    filling the structure at application side. While here, make the
    interface_description_ptr to be "char*" instead of "uint8_t*" to
    document the data type.

commit dc3e10485c3743568c95538d056029d5350e8dda
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 8 18:36:00 2016 +0200

    static structs: remove separate resource_type_len field
    
    Remove separate resource_type_len field from
    sn_nsdl_static_resource_parameters_s. Make the resource_type_ptr
    to be "char*" instead of "uint8_t*" to make it clearer that
    the data is a zero temrinated string from now on.
    
    This saves some memory and makes it easier to fill this structure
    from client side. We're exposing API from C++ side for application
    to optionally provide this struct as pre-filled const static data
    from ROM.

commit 949496e12cacda4c3ec23720382bbc24babbac5d
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Fri Dec 9 09:49:18 2016 +0200

    Rearrange resource struct variables.
    Fix static resource handling(PUT,POST,DELETE)

commit 4968a62a9f9bdb6c9bf2c044ce8255c48150a0b4
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Dec 1 10:02:24 2016 +0200

    Update sn_nsdl_lib.h

commit a69c3ba0db68fc54cebf871eaddb9c3a6ac8cf48
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Nov 29 12:28:08 2016 +0200

    Fix resource freeing

commit 5200efdf040a091b5bd2c0459587a091a7a50c73
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Nov 17 13:37:29 2016 +0200

    Update sn_grs.c

commit 7cb24bb238f423ddf1af5e9b4a027e5c520d957b
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Nov 17 13:34:46 2016 +0200

    Fixes according to review comments

commit cf5b8ca959f717b79a75da1950a533f4347977ed
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Nov 15 15:04:22 2016 +0200

    Split resource struct to dynamic and static ones

commit 606c39ab035814e3fb7fedc3af5b69e469154ade
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Nov 15 14:54:37 2016 +0200

    Remove sn_nsdl_oma_device_error_t struct

commit 4c0f5a232e4e69041ad8f65cb666c0ffe5656edd
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Nov 15 14:50:10 2016 +0200

    Remove OMA LWM2M specific functionality

commit d6a4ece6c305a14030a97e340566893560c00496 (origin/master, origin/HEAD, master)
Author: Jaakko Kukkohovi <jaakko.kukkohovi@arm.com>
Date:   Fri Dec 9 17:55:17 2016 +0200

    Add application definable context to nsdl handle (#96)
    
    Added application definable context field to nsdl handle. This is useful for example when interfacing with c++ objects where a pointer to object is set as the context, and in the callback functions the context pointer can be used to call methods for the correct instance of the c++ object.

commit e502b67a90ff96e52b98828e1e0c93d40071f171
Author: simosillankorva <simo.sillankorva@arm.com>
Date:   Mon Nov 21 12:02:23 2016 +0200

    Changed sn_coap_protocol.c to use randLIB for random message ID. (#91)
    
    * Changed sn_coap_protocol.c to use randLIB for random message ID. randLIB now needed to build the coap library.
    
    * Added randLIB dependency to module.json
    
    * Added check for message_id==0 when randomizing, as we dont want to change the api for sn_nsdl.c, that uses it for error cases when sending some messages.
    
    * Added randLiIB include path to unit tests.
    
    * Added randLIB_stub for sn_coap_protocol unit test.

commit 4cdc3570f3a4dad1cef9787755718fec6917f8f2
Merge: cfe1e4e 2f7e733
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Nov 16 10:46:52 2016 +0200

    Merge pull request #95 from ARMmbed/anttiylitokola-patch-1
    
    Disable message duplication

commit 2f7e7333799082b59346173c8c12fc71fb93ccde
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Nov 16 09:45:38 2016 +0200

    Disable message duplication
    
    Message duplication is currently missing some features and it doesn't work reliable yet. Can be enabled again once "IOTCLT-1038
    CoAP duplicate message detection missing features" is implemented.

commit cfe1e4e8c464a828eb6dfd4550b2f82831b0f489 (origin/memory_optimizations_base, memory_optimizations_base)
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Nov 10 15:20:47 2016 +0200

    Update sn_coap_builder.c
    
    1 compilation warning fixed

commit e9c5e25492914bcd583e74ae14f71c9c8465398c
Merge: fc1f9eb c0bb893
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Nov 9 09:41:00 2016 +0200

    Merge pull request #92 from ARMmbed/iotclt_1207
    
    Fix CoAP option building

commit c0bb8936b9d44cda49611bfee9ae55969b717811
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Nov 9 08:20:29 2016 +0200

    Replace unnecessary started flag with len variable.

commit 1deac48ddb51a9e9d85ecb682b9b82c4072b5c44
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Tue Nov 8 14:20:34 2016 +0200

    Fix CoAP option building

commit fc1f9eb790d08306ee435dd3a8452cfc82d7d740
Merge: 59be2f1 316a9db
Author: Yogesh Pande <yogpan01@users.noreply.github.com>
Date:   Thu Nov 3 16:35:47 2016 +0200

    Merge pull request #90 from ARMmbed/valgrind_unittest_fixes
    
    Valgrind unittest fixes

commit 316a9dbb11cf2b842255655501c30a5d0d040ca7
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Tue Oct 25 19:24:57 2016 +0300

    sn_coap_builder_unit_tests: fix 1976 valgrind errors
    
    Fix 1976 valgrind errors for uninitialized memory accesses by
    initializing the buffers before trying to parse them.
    
    One example of error being fixed:
    ---8<---8<---8<---
    ==8405==
    ==8405== Conditional jump or move depends on uninitialised value(s)
    ==8405==    at 0x40EA6E: sn_coap_builder_options_get_option_part_count (sn_coap_builder.c:926)
    ==8405==    by 0x40E7CF: sn_coap_builder_options_calc_option_size (sn_coap_builder.c:834)
    ==8405==    by 0x40CDA2: sn_coap_builder_calc_needed_packet_data_size_2 (sn_coap_builder.c:238)
    ==8405==    by 0x40C8C5: sn_coap_builder_calc_needed_packet_data_size (sn_coap_builder.c:147)
    ==8405==    by 0x404609: TEST_libCoap_builder_sn_coap_builder_calc_needed_packet_data_size_Test::testBody() (libCoap_builder_test.cpp:339)
    ==8405==    by 0x418660: PlatformSpecificSetJmp (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_builder/sn_coap_builder_unit_tests)
    ==8405==    by 0x416C24: Utest::run() (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_builder/sn_coap_builder_unit_tests)
    ==8405==    by 0x4172DE: UtestShell::runOneTest(TestPlugin*, TestResult&) (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_builder/sn_coap_builder_unit_tests)
    ==8405==    by 0x418660: PlatformSpecificSetJmp (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_builder/sn_coap_builder_unit_tests)
    ==8405==    by 0x416B3B: UtestShell::runOneTestWithPlugins(TestPlugin*, TestResult&) (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_builder/sn_coap_builder_unit_tests)
    ==8405==    by 0x419F9D: TestRegistry::runAllTests(TestResult&) (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_builder/sn_coap_builder_unit_tests)
    ==8405==    by 0x4107D7: CommandLineTestRunner::runAllTests() (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_builder/sn_coap_builder_unit_tests)

commit 7fe1b032d117aec24bacf929bff72aee2d4b1000
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Tue Oct 25 17:32:26 2016 +0300

    sn_nsdl_unit_tests: fix 23 valgrind errors
    
    Fix 23 valgrind error for uninitialized memory accesses.
    
    One example of warning being fixed:
    ---8<---8<---
    ==3916==
    ==3916== Conditional jump or move depends on uninitialised value(s)
    ==3916==    at 0x418976: sn_coap_parser_release_allocated_coap_msg_mem (sn_coap_parser_stub.c:42)
    ==3916==    by 0x420E3A: sn_nsdl_release_allocated_coap_msg_mem (sn_nsdl.c:2535)
    ==3916==    by 0x417A92: test_sn_nsdl_release_allocated_coap_msg_mem (test_sn_nsdl.c:2885)
    ==3916==    by 0x402E85: TEST_sn_nsdl_test_sn_nsdl_release_allocated_coap_msg_mem_Test::testBody() (sn_nsdltest.cpp:166)
    ==3916==    by 0x42A8B0: PlatformSpecificSetJmp (in mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)
    ==3916==    by 0x428E74: Utest::run() (in mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)
    ==3916==    by 0x42952E: UtestShell::runOneTest(TestPlugin*, TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)
    ==3916==    by 0x42A8B0: PlatformSpecificSetJmp (in mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)
    ==3916==    by 0x428D8B: UtestShell::runOneTestWithPlugins(TestPlugin*, TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)
    ==3916==    by 0x42C1ED: TestRegistry::runAllTests(TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)
    ==3916==    by 0x422A27: CommandLineTestRunner::runAllTests() (in mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)
    ==3916==    by 0x422ACC: CommandLineTestRunner::runAllTestsMain() (in mbed-client-c/test/nsdl-c/unittest/sn_nsdl/sn_nsdl_unit_tests)

commit 36933741993f71dbd3e02521de5cf69876106030
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Tue Oct 25 16:38:02 2016 +0300

    sn_grs_unit_tests: fix 4 valgrind errors
    
    Fix 4 valgrind findings of uninitialized memory usage.
    
    One example of error being fixed:
    ---8<----8<---
    ==30551==
    ==30551== Conditional jump or move depends on uninitialised value(s)
    ==30551==    at 0x40F01B: sn_grs_convert_uri (sn_grs.c:949)
    ==30551==    by 0x40E7E7: sn_grs_search_resource (sn_grs.c:769)
    ==30551==    by 0x40D42F: sn_grs_process_coap (sn_grs.c:413)
    ==30551==    by 0x40869C: test_sn_grs_process_coap (test_sn_grs.c:654)
    ==30551==    by 0x401E7D: TEST_sn_grs_test_sn_grs_process_coap_Test::testBody() (sn_grstest.cpp:31)
    ==30551==    by 0x418C20: PlatformSpecificSetJmp (in mbed-client-c/test/nsdl-c/unittest/sn_grs/sn_grs_unit_tests)
    ==30551==    by 0x4171E4: Utest::run() (in mbed-client-c/test/nsdl-c/unittest/sn_grs/sn_grs_unit_tests)
    ==30551==    by 0x41789E: UtestShell::runOneTest(TestPlugin*, TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_grs/sn_grs_unit_tests)
    ==30551==    by 0x418C20: PlatformSpecificSetJmp (in mbed-client-c/test/nsdl-c/unittest/sn_grs/sn_grs_unit_tests)
    ==30551==    by 0x4170FB: UtestShell::runOneTestWithPlugins(TestPlugin*, TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_grs/sn_grs_unit_tests)
    ==30551==    by 0x41A55D: TestRegistry::runAllTests(TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_grs/sn_grs_unit_tests)
    ==30551==    by 0x410D97: CommandLineTestRunner::runAllTests() (in mbed-client-c/test/nsdl-c/unittest/sn_grs/sn_grs_unit_tests)

commit 1eebc512fe157227702b81684b36cb9bad179af2
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Tue Oct 25 16:13:35 2016 +0300

    sn_coap_protocol_unit_tests: Fix 297 valgrind errors for unint. memory.
    
    Fix 297 valgrind errors for uses of uninitialized memory. Most cases
    were caused by same copy-pasted piece which allocated 3 bytes for a
    buffer but used it as it was much, much larger.
    
    One sample of the fixed error:
    ---8<---8<---
    ==22740== Invalid read of size 8
    ==22740==    at 0x4C2F79E: memcpy@@GLIBC_2.14 (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==22740==    by 0x419329: sn_coap_protocol_build (sn_coap_protocol.c:492)
    ==22740==    by 0x41268E: TEST_libCoap_protocol_sn_coap_protocol_exec_Test::testBody() (libCoap_protocol_test.cpp:2076)
    ==22740==    by 0x427DE0: PlatformSpecificSetJmp (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x4263A4: Utest::run() (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x426A5E: UtestShell::runOneTest(TestPlugin*, TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x427DE0: PlatformSpecificSetJmp (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x4262BB: UtestShell::runOneTestWithPlugins(TestPlugin*, TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x42971D: TestRegistry::runAllTests(TestResult&) (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x41FF57: CommandLineTestRunner::runAllTests() (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x41FFFC: CommandLineTestRunner::runAllTestsMain() (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)
    ==22740==    by 0x42011E: CommandLineTestRunner::RunAllTests(int, char const**) (in mbed-client-c/test/nsdl-c/unittest/sn_coap_protocol/sn_coap_protocol_unit_tests)

commit 1393616579ff888dc23384cc5a50a8be3b7f9935
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Tue Oct 25 15:21:48 2016 +0300

    sn_coap_parser_unit_tests: Fix uses of uninitialized memory.
    
    Fix 10 valgrind erros for uses of uninitialized memory. All the errors
    are caused by same root cause, where code allocated memory for test buffer
    and assumed it contains something useful or predictable.
    
    One error beind fixed:
    --8<---8<---
    ==21841== Conditional jump or move depends on uninitialised value(s)
    ==21841==    at 0x4093CA: sn_coap_parser_options_parse (sn_coap_parser.c:262)
    ==21841==    by 0x408E5C: sn_coap_parser (sn_coap_parser.c:133)
    ==21841==    by 0x403FBC: test_sn_coap_parser (test_sn_coap_parser.c:61)
    ==21841==    by 0x401CFD: TEST_sn_coap_parser_test_sn_coap_parser_Test::testBody() (sn_coap_parsertest.cpp:20)
    ==21841==    by 0x414350: PlatformSpecificSetJmp (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)
    ==21841==    by 0x412914: Utest::run() (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)
    ==21841==    by 0x412FCE: UtestShell::runOneTest(TestPlugin*, TestResult&) (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)
    ==21841==    by 0x414350: PlatformSpecificSetJmp (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)
    ==21841==    by 0x41282B: UtestShell::runOneTestWithPlugins(TestPlugin*, TestResult&) (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)
    ==21841==    by 0x415C8D: TestRegistry::runAllTests(TestResult&) (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)
    ==21841==    by 0x40C4C7: CommandLineTestRunner::runAllTests() (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)
    ==21841==    by 0x40C56C: CommandLineTestRunner::runAllTestsMain() (in /home/tero/work/mbed-github/mbed-client-c/test/nsdl-c/unittest/sn_coap_parser/sn_coap_parser_unit_tests)

commit 59be2f154af9b8388ae3e3e6a763cd5b0d2700f1 (tag: v3.0.3)
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 29 10:52:54 2016 +0300

    version v3.0.3

commit 3b89e2a465c3876cc184c92c72bdbe66193502c7
Merge: 58f712b 4369a46
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 29 10:50:20 2016 +0300

    Merge pull request #81 from ARMmbed/duplicate_fix
    
    Fix for message duplication handling

commit 4369a4656e4a1ef5e81adf6316513d7a06e9a488
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 29 09:19:36 2016 +0300

    Fix one failing unit test

commit 72bfc3e7199d040b8bfa637e1840a0869f786231
Author: Tero Heinonen <tero.heinonen@arm.com>
Date:   Wed Sep 28 13:54:56 2016 +0300

    Enable duplicate message detection

commit 852c1f642afe7eb342016c76807d8b10a0fe7504
Author: Tero Heinonen <tero.heinonen@arm.com>
Date:   Thu Sep 22 14:12:15 2016 +0300

    Fix for message duplication handling
    
    RFC 7252 chapter 4.5 talks only duplication of Confirmable- and
    Non-confirmable messages. Adding check for message type before
    processing duplication store and check.

## [v3.0.2](https://github.com/ARMmbed/mbed-client-c/releases/tag/v3.0.2) (24-Sep-2016)
[Full Changelog](https://github.com/ARMmbed/mbed-client-c/compare/v3.0.1...v3.0.2)

**New feature**

- Make resending flags configurable through Yotta and mbed-cli build systems
- Introducing Configuration management through Config file

**Closed issues:**

- IOTCLT-882 - Confirmable messages sending

**Merged pull requests:**

commit 58f712b9f684bede1905ce35a1a39ebb257c049c (HEAD, tag: v3.0.2, origin/master, origin/HEAD, master)
Author: Yogesh Pande <Yogesh.Pande@arm.com>
Date:   Sat Sep 24 15:04:36 2016 +0300

    version v3.0.2

commit 39d5e98025f3d8067cea85c4a06470e0e9374812
Merge: cf4c119 31d6566
Author: Yogesh Pande <yogpan01@users.noreply.github.com>
Date:   Sat Sep 24 14:51:30 2016 +0300

    Merge pull request #79 from ARMmbed/config-mechanism
    
    Introducing Configuration management through Config file

commit 31d6566dddfe449e87819244027b847f53f53a96 (origin/config-mechanism)
Author: Yogesh Pande <Yogesh.Pande@arm.com>
Date:   Sat Sep 24 12:53:47 2016 +0300

    Fixing valgrind with unit tests

commit a95a90a31e8f6123d14df84688b4e2e2f32e5507
Author: Yogesh Pande <Yogesh.Pande@arm.com>
Date:   Sat Sep 24 12:00:44 2016 +0300

    Fixing valgrind memory leaks for unit tests

commit 248fc713a4303cb1f724d10f4aa6d68933bb5aed
Author: Yogesh Pande <Yogesh.Pande@arm.com>
Date:   Sat Sep 24 11:32:01 2016 +0300

    Fix for failing unit tests

commit cec0980b4ccff9708cd0130fea936a69839739b8
Merge: cf09b2b cf4c119
Author: Yogesh Pande <yogpan01@users.noreply.github.com>
Date:   Thu Sep 22 18:02:39 2016 +0300

    Merge branch 'master' into config-mechanism

commit cf4c1196520d64bf863b39efefc663ee60e58c61
Merge: 330a16b f5f15cc
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 22 12:55:33 2016 +0300

    Merge pull request #80 from ARMmbed/revert-77-duplication
    
    Revert "Enable message duplication by default"

commit f5f15ccce5fd719aa8d7de14a583d02fd0fd05b9 (origin/revert-77-duplication)
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Thu Sep 22 12:30:50 2016 +0300

    Revert "Enable message duplication by default"

commit cf09b2b6233bf897a17bdb0102962947df87f4ae
Author: Yogesh Pande <Yogesh.Pande@arm.com>
Date:   Thu Sep 22 09:57:41 2016 +0300

    Fixed comments from PR#79
    
    This commit includes
     - Fixing comments from PR-79

commit 8fd7cc258168047c29aca27a659de448f45e0e6f
Author: Yogesh Pande <Yogesh.Pande@arm.com>
Date:   Wed Sep 21 20:42:28 2016 +0300

    Introducing Configuration management through Config file
    
    This commit includes
     - Support for configuring various build time parameters throuh user supplied config file
      that can be passed at compile time through Makefile system which will work for
      non mbed-OS and non yotta build system as well.

commit 330a16bbfc5dc597015c7bdd935b603282d7064e
Merge: b7d909e e268081
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Sep 21 12:05:29 2016 +0300

    Merge pull request #78 from ARMmbed/resend_flags
    
    Make resend flags configurable through Yotta and mbed-cli build sy…

commit e268081bae1fa1db3b5b64dfb77d5b7a5bfed748
Merge: dc16b29 b7d909e
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Sep 21 12:02:32 2016 +0300

    Merge branch 'master' into resend_flags

commit b7d909eca1c2259453dbe9c094b8a0c53d638fea
Merge: 7f0eaea ad3b468
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Wed Sep 21 12:02:15 2016 +0300

    Merge pull request #77 from ARMmbed/duplication
    
    Enable message duplication by default

commit dc16b299ab9ff4bd1eaa9d3ec120fa58180820c4
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Mon Sep 19 14:35:31 2016 +0300

    Make resending flags configurable through Yotta and mbed-cli build systems

commit ad3b46881bb61dfc2510d13d373bcd1faad114f4
Author: Antti Yli-Tokola <antti.yli-tokola@arm.com>
Date:   Fri Sep 16 08:58:38 2016 +0300

    Enable message duplication by default

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
