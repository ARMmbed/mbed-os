# Change Log

## [v3.0.4](https://github.com/ARMmbed/mbed-client-c/releases/tag/v3.0.4) (23-dec-2016)
[Full Changelog](https://github.com/ARMmbed/mbed-client-c/compare/v3.0.2...v3.0.4)

**New feature**

- CoAP message ID randomization
- Initial memory optimized API added behind compile time flag

**Closed issues:**

- IOTCLT-1207 - sn_coap_builder_options_build_add_uint_option function produce wrong option value
- IOTCLT-828 / ARMmbed/mbed-client-c#59 - Random CoAP message ID
 
**Merged pull requests:**

commit 8d247d6baf16d7171dbc1d0e61383aeae59f9f20 (HEAD -> release-3.0.4, tag: v3.0.4, origin/release-3.0.4)
Author: Jaakko Kukkohovi <jaakko.kukkohovi@arm.com>
Date:   Fri Dec 23 13:48:23 2016 +0200

    version v3.0.4

commit 6f6d604dc9984dbae4bd183c4151be986de81a1b (origin/mem_opt_part_2, mem_opt_part_2)
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 22 20:12:48 2016 +0200

    Make the code compile two different versions of the API
    
    If one defines MBED_CLIENT_C_NEW_API, the new versions of
    sn_grs and sn_nsdl are selected and old code will continue to work
    as is if the define is not there.

commit 947bec9fd2a997a1f29633fa83f67fde40c16e0f
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 22 19:12:30 2016 +0200

    Copy the changed files from memory_optimizations -branch to new name

commit a3de842af0b19c8760482cc451b7a2e8520547fb
Author: Tero Jääskö <tero.jaasko@arm.com>
Date:   Thu Dec 22 18:46:13 2016 +0200

    Copy the sn_nsdl and sn_grs files to fork the API via define

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

commit cfe1e4e8c464a828eb6dfd4550b2f82831b0f489 (origin/memory_optimizations_base)
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
