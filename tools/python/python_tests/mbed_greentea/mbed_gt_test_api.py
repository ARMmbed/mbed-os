"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import unittest
from mbed_greentea import mbed_test_api



class GreenteaTestAPI(unittest.TestCase):

    def setUp(self):
        self.OUTPUT_FAILURE = """mbedgt: mbed-host-test-runner: started
[1459245784.59][CONN][RXD] >>> Test cases: 7 passed, 1 failed with reason 'Test Cases Failed'
[1459245784.61][CONN][RXD] >>> TESTS FAILED!
[1459245784.64][CONN][INF] found KV pair in stream: {{__testcase_summary;7;1}}, queued...
[1459245784.64][CONN][RXD] {{__testcase_summary;7;1}}
[1459245784.66][CONN][INF] found KV pair in stream: {{end;failure}}, queued...
[1459245784.66][HTST][INF] __notify_complete(False)
[1459245784.66][HTST][INF] test suite run finished after 2.37 sec...
[1459245784.66][CONN][RXD] {{end;failure}}
[1459245784.67][HTST][INF] CONN exited with code: 0
[1459245784.67][HTST][INF] Some events in queue
[1459245784.67][HTST][INF] stopped consuming events
[1459245784.67][HTST][INF] host test result() call skipped, received: False
[1459245784.67][HTST][WRN] missing __exit event from DUT
[1459245784.67][HTST][INF] calling blocking teardown()
[1459245784.67][HTST][INF] teardown() finished
[1459245784.67][HTST][INF] {{result;failure}}
"""

        self.OUTPUT_SUCCESS = """mbedgt: mbed-host-test-runner: started
[1459245860.90][CONN][RXD] {{__testcase_summary;4;0}}
[1459245860.92][CONN][INF] found KV pair in stream: {{end;success}}, queued...
[1459245860.92][CONN][RXD] {{end;success}}
[1459245860.92][HTST][INF] __notify_complete(True)
[1459245860.92][HTST][INF] test suite run finished after 0.90 sec...
[1459245860.94][HTST][INF] CONN exited with code: 0
[1459245860.94][HTST][INF] No events in queue
[1459245860.94][HTST][INF] stopped consuming events
[1459245860.94][HTST][INF] host test result() call skipped, received: True
[1459245860.94][HTST][WRN] missing __exit event from DUT
[1459245860.94][HTST][INF] calling blocking teardown()
[1459245860.94][HTST][INF] teardown() finished
[1459245860.94][HTST][INF] {{result;success}}
"""

        self.OUTPUT_TIMEOUT = """mbedgt: mbed-host-test-runner: started
[1459246047.80][HTST][INF] copy image onto target...
        1 file(s) copied.
[1459246055.05][HTST][INF] starting host test process...
[1459246055.47][CONN][INF] starting connection process...
[1459246055.47][CONN][INF] initializing serial port listener...
[1459246055.47][SERI][INF] serial(port=COM205, baudrate=9600)
[1459246055.47][SERI][INF] reset device using 'default' plugin...
[1459246055.73][SERI][INF] wait for it...
[1459246056.74][CONN][INF] sending preamble '56bdcd85-b88a-460b-915e-1b9b41713b5a'...
[1459246056.74][SERI][TXD] mbedmbedmbedmbedmbedmbedmbedmbedmbedmbed
[1459246056.74][SERI][TXD] {{__sync;56bdcd85-b88a-460b-915e-1b9b41713b5a}}
[1459246065.06][HTST][INF] test suite run finished after 10.00 sec...
[1459246065.07][HTST][INF] CONN exited with code: 0
[1459246065.07][HTST][INF] No events in queue
[1459246065.07][HTST][INF] stopped consuming events
[1459246065.07][HTST][INF] host test result(): None
[1459246065.07][HTST][WRN] missing __exit event from DUT
[1459246065.07][HTST][ERR] missing __exit event from DUT and no result from host test, timeout...
[1459246065.07][HTST][INF] calling blocking teardown()
[1459246065.07][HTST][INF] teardown() finished
[1459246065.07][HTST][INF] {{result;timeout}}
"""

        self.OUTPUT_STARTTAG_MISSING = """
mbedgt: mbed-host-test-runner: started
[1507470727.39][HTST][INF] host test executor ver. 1.2.0
[1507470727.39][HTST][INF] copy image onto target... SKIPPED!
[1507470727.39][HTST][INF] starting host test process...
[1507470727.40][CONN][INF] starting connection process...
[1507470727.40][HTST][INF] setting timeout to: 120 sec
[1507470756.96][CONN][RXD] >>> Running 4 test cases...
[1507470756.96][CONN][RXD]
[1507470756.96][CONN][RXD] >>> Running case #1: 'DNS query'...
[1507470756.96][CONN][INF] found KV pair in stream: {{__testcase_name;DNS query}}, queued...
[1507470756.96][CONN][INF] found KV pair in stream: {{__testcase_name;DNS preference query}}, queued...
[1507470756.96][CONN][INF] found KV pair in stream: {{__testcase_name;DNS literal}}, queued...
[1507470756.96][CONN][INF] found KV pair in stream: {{__testcase_name;DNS preference literal}}, queued...
[1507470757.04][CONN][RXD] DNS: query "connector.mbed.com" => "169.45.82.19"
[1507470757.04][CONN][INF] found KV pair in stream: {{__testcse_start;DNS query}}, queued...
[1507470757.13][CONN][RXD] >>> 'DNS query': 1 passed, 0 failed
[1507470757.13][CONN][RXD]
[1507470757.13][CONN][INF] found KV pair in stream: {{__testcase_finish;DNS query;1;0}}, queued...
[1507470757.32][CONN][RXD] >>> Running case #2: 'DNS preference query'...
[1507470757.41][CONN][RXD] DNS: query ipv4 "connector.mbed.com" => "169.45.82.19"
[1507470757.41][CONN][RXD] >>> 'DNS preference query': 1 passed, 0 failed
[1507470757.41][CONN][RXD]
[1507470757.41][CONN][INF] found KV pair in stream: {{__testcase_start;DNS preference query}}, queued...
[1507470757.41][CONN][INF] found KV pair in stream: {{__testcase_finish;DNS preference query;1;0}}, queued...
[1507470757.57][CONN][RXD] >>> Running case #3: 'DNS literal'...
[1507470757.66][CONN][RXD] DNS: literal "10.118.13.253" => "10.118.13.253"
[1507470757.66][CONN][RXD] {{__testcase_finish;DNS l<DAPLink:Overflow>
[1507470757.66][CONN][RXD] sed, 0 fai<DAPLink:Overflow>
[1507470757.66][CONN][INF] found KV pair in stream: {{__testcase_start;DNS literal}}, queued...
[1507470757.80][CONN][RXD] case #4: 'DNS preference literal'...
[1507470757.80][CONN][RXD] {{__test<DAPLink:Overflow>
[1507470757.80][CONN][RXD] reference literal}}
[1507470757.83][CONN][RXD] DNS: literal ipv4 "10.118.13.253" => "10.118.13.253"
[1507470757.83][CONN][INF] found KV pair in stream: {{__testcase_finish;DNS preference literal;1;0}}, queued...
[1507470757.92][CONN][RXD] >>> 'DNS preference literal': 1 passed, 0 failed
[1507470757.92][CONN][RXD]
[1507470757.92][CONN][RXD] >>> Test cases: 4 passed, 0 failed
[1507470758.00][CONN][INF] found KV pair in stream: {{__testcase_summary;4;0}}, queued...
[1507470758.00][CONN][INF] found KV pair in stream: {{max_heap_usage;2800}}, queued...
[1507470758.00][CONN][INF] found KV pair in stream: {{reserved_heap;19336}}, queued...
[1507470758.00][HTST][ERR] orphan event in main phase: {{max_heap_usage;2800}}, timestamp=1507470757.999258
[1507470758.00][HTST][ERR] orphan event in main phase: {{reserved_heap;19336}}, timestamp=1507470757.999260
[1507470758.00][CONN][INF] found KV pair in stream: {{__thread_info;"0x010002ca8",600,4096}}, queued...
[1507470758.00][HTST][ERR] orphan event in main phase: {{__thread_info;"0x010002ca8",600,4096}}, timestamp=1507470757.999261
[1507470758.09][CONN][INF] found KV pair in stream: {{__thread_info;"0x01000045c",72,512}}, queued...
[1507470758.09][CONN][INF] found KV pair in stream: {{__thread_info;"0x010001168",128,512}}, queued...
[1507470758.09][HTST][ERR] orphan event in main phase: {{__thread_info;"0x01000045c",72,512}}, timestamp=1507470758.085627
[1507470758.09][HTST][ERR] orphan event in main phase: {{__thread_info;"0x010001168",128,512}}, timestamp=1507470758.085635
[1507470758.16][CONN][INF] found KV pair in stream: {{__thread_info;"0x010001018",560,1200}}, queued...
[1507470758.16][HTST][ERR] orphan event in main phase: {{__thread_info;"0x010001018",560,1200}}, timestamp=1507470758.162780
[1507470758.25][CONN][INF] found KV pair in stream: {{__thread_info;"0x0100004a4",112,768}}, queued...
[1507470758.25][CONN][INF] found KV pair in stream: {{__thread_info;"0x0100010f8",96,512}}, queued...
[1507470758.25][CONN][INF] found KV pair in stream: {{__thread_info;"0x010001088",152,512}}, queued...
[1507470758.25][CONN][INF] found KV pair in stream: {{end;success}}, queued...
[1507470758.25][HTST][ERR] orphan event in main phase: {{__thread_info;"0x0100004a4",112,768}}, timestamp=1507470758.248291
[1507470758.25][HTST][ERR] orphan event in main phase: {{__thread_info;"0x0100010f8",96,512}}, timestamp=1507470758.248296
[1507470758.25][HTST][ERR] orphan event in main phase: {{__thread_info;"0x010001088",152,512}}, timestamp=1507470758.248299
[1507470758.25][HTST][INF] __notify_complete(True)
[1507470758.25][HTST][INF] __exit_event_queue received
[1507470760.47][HTST][INF] CONN exited with code: 0
[1507470760.47][HTST][INF] No events in queue
[1507470760.47][HTST][INF] stopped consuming events
[1507470760.47][HTST][INF] host test result() call skipped, received: True
[1507470760.47][HTST][WRN] missing __exit event from DUT
[1507470760.47][HTST][INF] calling blocking teardown()
[1507470760.47][HTST][INF] teardown() finished
[1507470760.47][HTST][INF] {{result;success}}
        """

        self.OUTPUT_UNDEF = """mbedgt: mbed-host-test-runner: started
{{result;some_random_value}}
"""

        self.OUTOUT_CSTRING_TEST = """
[1459246264.88][HTST][INF] copy image onto target...
        1 file(s) copied.
[1459246272.76][HTST][INF] starting host test process...
[1459246273.18][CONN][INF] starting connection process...
[1459246273.18][CONN][INF] initializing serial port listener...
[1459246273.18][SERI][INF] serial(port=COM205, baudrate=9600)
[1459246273.18][SERI][INF] reset device using 'default' plugin...
[1459246273.43][SERI][INF] wait for it...
[1459246274.43][CONN][INF] sending preamble '5daa5ff9-a9c1-4b47-88a2-9295f1de7c64'...
[1459246274.43][SERI][TXD] mbedmbedmbedmbedmbedmbedmbedmbedmbedmbed
[1459246274.43][SERI][TXD] {{__sync;5daa5ff9-a9c1-4b47-88a2-9295f1de7c64}}
[1459246274.58][CONN][INF] found SYNC in stream: {{__sync;5daa5ff9-a9c1-4b47-88a2-9295f1de7c64}}, queued...
[1459246274.58][CONN][RXD] {{__sync;5daa5ff9-a9c1-4b47-88a2-9295f1de7c64}}
[1459246274.58][HTST][INF] sync KV found, uuid=5daa5ff9-a9c1-4b47-88a2-9295f1de7c64, timestamp=1459246274.575000
[1459246274.60][CONN][INF] found KV pair in stream: {{__version;1.1.0}}, queued...
[1459246274.60][CONN][RXD] {{__version;1.1.0}}
[1459246274.60][HTST][INF] DUT greentea-client version: 1.1.0
[1459246274.61][CONN][INF] found KV pair in stream: {{__timeout;5}}, queued...
[1459246274.61][HTST][INF] setting timeout to: 5 sec
[1459246274.62][CONN][RXD] {{__timeout;5}}
[1459246274.64][CONN][INF] found KV pair in stream: {{__host_test_name;default_auto}}, queued...
[1459246274.64][HTST][INF] host test setup() call...
[1459246274.64][HTST][INF] CALLBACKs updated
[1459246274.64][HTST][INF] host test detected: default_auto
[1459246274.64][CONN][RXD] {{__host_test_name;default_auto}}
[1459246274.66][CONN][INF] found KV pair in stream: {{__testcase_count;8}}, queued...
[1459246274.66][CONN][RXD] {{__testcase_count;8}}
[1459246274.69][CONN][RXD] >>> Running 8 test cases...
[1459246274.74][CONN][RXD] >>> Running case #1: 'C strings: strtok'...
[1459246274.79][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: strtok}}, queued...
[1459246274.79][CONN][RXD] {{__testcase_start;C strings: strtok}}
[1459246274.84][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: strtok;1;0}}, queued...
[1459246274.84][CONN][RXD] {{__testcase_finish;C strings: strtok;1;0}}
[1459246274.88][CONN][RXD] >>> 'C strings: strtok': 1 passed, 0 failed
[1459246274.93][CONN][RXD] >>> Running case #2: 'C strings: strpbrk'...
[1459246274.97][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: strpbrk}}, queued...
[1459246274.97][CONN][RXD] {{__testcase_start;C strings: strpbrk}}
[1459246275.01][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: strpbrk;1;0}}, queued...
[1459246275.01][CONN][RXD] {{__testcase_finish;C strings: strpbrk;1;0}}
[1459246275.06][CONN][RXD] >>> 'C strings: strpbrk': 1 passed, 0 failed
[1459246275.13][CONN][RXD] >>> Running case #3: 'C strings: %i %d integer formatting'...
[1459246275.18][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %i %d integer formatting}}, queued...
[1459246275.18][CONN][RXD] {{__testcase_start;C strings: %i %d integer formatting}}
[1459246275.24][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %i %d integer formatting;1;0}}, queued...
[1459246275.24][CONN][RXD] {{__testcase_finish;C strings: %i %d integer formatting;1;0}}
[1459246275.32][CONN][RXD] >>> 'C strings: %i %d integer formatting': 1 passed, 0 failed
[1459246275.38][CONN][RXD] >>> Running case #4: 'C strings: %u %d integer formatting'...
[1459246275.44][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %u %d integer formatting}}, queued...
[1459246275.44][CONN][RXD] {{__testcase_start;C strings: %u %d integer formatting}}
[1459246275.50][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %u %d integer formatting;1;0}}, queued...
[1459246275.50][CONN][RXD] {{__testcase_finish;C strings: %u %d integer formatting;1;0}}
[1459246275.57][CONN][RXD] >>> 'C strings: %u %d integer formatting': 1 passed, 0 failed
[1459246275.64][CONN][RXD] >>> Running case #5: 'C strings: %x %E integer formatting'...
[1459246275.68][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %x %E integer formatting}}, queued...
[1459246275.68][CONN][RXD] {{__testcase_start;C strings: %x %E integer formatting}}
[1459246275.74][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %x %E integer formatting;1;0}}, queued...
[1459246275.74][CONN][RXD] {{__testcase_finish;C strings: %x %E integer formatting;1;0}}
[1459246275.82][CONN][RXD] >>> 'C strings: %x %E integer formatting': 1 passed, 0 failed
[1459246275.88][CONN][RXD] >>> Running case #6: 'C strings: %f %f float formatting'...
[1459246275.94][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %f %f float formatting}}, queued...
[1459246275.94][CONN][RXD] {{__testcase_start;C strings: %f %f float formatting}}
[1459246276.10][CONN][RXD] :57::FAIL: Expected '0.002000 0.924300 15.913200 791.773680 6208.200000 25719.495200 426815.982588 6429271.046000 42468024.930000 212006462.910000' Was '
'
[1459246276.18][CONN][RXD] >>> failure with reason 'Assertion Failed' during 'Case Handler'
[1459246276.25][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %f %f float formatting;0;1}}, queued...
[1459246276.25][CONN][RXD] {{__testcase_finish;C strings: %f %f float formatting;0;1}}
[1459246276.34][CONN][RXD] >>> 'C strings: %f %f float formatting': 0 passed, 1 failed with reason 'Test Cases Failed'
[1459246276.41][CONN][RXD] >>> Running case #7: 'C strings: %e %E float formatting'...
[1459246276.46][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %e %E float formatting}}, queued...
[1459246276.46][CONN][RXD] {{__testcase_start;C strings: %e %E float formatting}}
[1459246276.52][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %e %E float formatting;1;0}}, queued...
[1459246276.53][CONN][RXD] {{__testcase_finish;C strings: %e %E float formatting;1;0}}
[1459246276.59][CONN][RXD] >>> 'C strings: %e %E float formatting': 1 passed, 0 failed
[1459246276.65][CONN][RXD] >>> Running case #8: 'C strings: %g %g float formatting'...
[1459246276.71][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %g %g float formatting}}, queued...
[1459246276.71][CONN][RXD] {{__testcase_start;C strings: %g %g float formatting}}
[1459246276.77][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %g %g float formatting;1;0}}, queued...
[1459246276.77][CONN][RXD] {{__testcase_finish;C strings: %g %g float formatting;1;0}}
[1459246276.83][CONN][RXD] >>> 'C strings: %g %g float formatting': 1 passed, 0 failed
[1459246276.90][CONN][RXD] >>> Test cases: 7 passed, 1 failed with reason 'Test Cases Failed'
[1459246276.92][CONN][RXD] >>> TESTS FAILED!
[1459246276.95][CONN][INF] found KV pair in stream: {{__testcase_summary;7;1}}, queued...
[1459246276.95][CONN][RXD] {{__testcase_summary;7;1}}
[1459246276.97][CONN][INF] found KV pair in stream: {{end;failure}}, queued...
[1459246276.97][CONN][RXD] {{end;failure}}
[1459246276.97][HTST][INF] __notify_complete(False)
[1459246276.97][HTST][INF] test suite run finished after 2.37 sec...
[1459246276.98][HTST][INF] CONN exited with code: 0
[1459246276.98][HTST][INF] Some events in queue
[1459246276.98][HTST][INF] stopped consuming events
[1459246276.98][HTST][INF] host test result() call skipped, received: False
[1459246276.98][HTST][WRN] missing __exit event from DUT
[1459246276.98][HTST][INF] calling blocking teardown()
[1459246276.98][HTST][INF] teardown() finished
[1459246276.98][HTST][INF] {{result;failure}}
"""

        self.OUTOUT_CSTRING_TEST_CASE_COUNT_AND_NAME = """
[1467197417.13][SERI][TXD] {{__sync;3018cb93-f11c-417e-bf61-240c338dfec9}}
[1467197417.27][CONN][RXD] {{__sync;3018cb93-f11c-417e-bf61-240c338dfec9}}
[1467197417.27][CONN][INF] found SYNC in stream: {{__sync;3018cb93-f11c-417e-bf61-240c338dfec9}} it is #0 sent, queued...
[1467197417.27][HTST][INF] sync KV found, uuid=3018cb93-f11c-417e-bf61-240c338dfec9, timestamp=1467197417.272000
[1467197417.29][CONN][RXD] {{__version;1.1.0}}
[1467197417.29][CONN][INF] found KV pair in stream: {{__version;1.1.0}}, queued...
[1467197417.29][HTST][INF] DUT greentea-client version: 1.1.0
[1467197417.31][CONN][RXD] {{__timeout;5}}
[1467197417.31][CONN][INF] found KV pair in stream: {{__timeout;5}}, queued...
[1467197417.31][HTST][INF] setting timeout to: 5 sec
[1467197417.34][CONN][RXD] {{__host_test_name;default_auto}}
[1467197417.34][CONN][INF] found KV pair in stream: {{__host_test_name;default_auto}}, queued...
[1467197417.34][HTST][INF] host test class: '<class 'mbed_host_tests.host_tests.default_auto.DefaultAuto'>'
[1467197417.34][HTST][INF] host test setup() call...
[1467197417.34][HTST][INF] CALLBACKs updated
[1467197417.34][HTST][INF] host test detected: default_auto
[1467197417.36][CONN][RXD] {{__testcase_count;2}}
[1467197417.36][CONN][INF] found KV pair in stream: {{__testcase_count;2}}, queued...
[1467197417.39][CONN][RXD] >>> Running 2 test cases...
[1467197417.43][CONN][RXD] {{__testcase_name;C strings: strtok}}
[1467197417.43][CONN][INF] found KV pair in stream: {{__testcase_name;C strings: strtok}}, queued...
[1467197417.47][CONN][RXD] {{__testcase_name;C strings: strpbrk}}
[1467197417.47][CONN][INF] found KV pair in stream: {{__testcase_name;C strings: strpbrk}}, queued...
[1467197417.52][CONN][RXD] >>> Running case #1: 'C strings: strtok'...
[1467197417.56][CONN][RXD] {{__testcase_start;C strings: strtok}}
[1467197417.56][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: strtok}}, queued...
[1467197422.31][HTST][INF] test suite run finished after 5.00 sec...
[1467197422.31][CONN][INF] received special even '__host_test_finished' value='True', finishing
[1467197422.33][HTST][INF] CONN exited with code: 0
[1467197422.33][HTST][INF] No events in queue
[1467197422.33][HTST][INF] stopped consuming events
[1467197422.33][HTST][INF] host test result(): None
[1467197422.33][HTST][WRN] missing __exit event from DUT
[1467197422.33][HTST][ERR] missing __exit event from DUT and no result from host test, timeout...
[1467197422.33][HTST][INF] calling blocking teardown()
[1467197422.33][HTST][INF] teardown() finished
[1467197422.33][HTST][INF] {{result;timeout}}
"""

        self.OUTOUT_GENERIC_TESTS_TESCASE_NAME_AND_COUNT = """
[1467205002.74][HTST][INF] host test executor ver. 0.2.19
[1467205002.74][HTST][INF] copy image onto target...
        1 file(s) copied.
Plugin info: HostTestPluginCopyMethod_Shell::CopyMethod: Waiting up to 60 sec for '0240000033514e450019500585d40008e981000097969900' mount point (current is 'F:')...
[1467205011.16][HTST][INF] starting host test process...
[1467205011.74][CONN][INF] starting serial connection process...
[1467205011.74][CONN][INF] notify event queue about extra 60 sec timeout for serial port pooling
[1467205011.74][CONN][INF] initializing serial port listener...
[1467205011.74][HTST][INF] setting timeout to: 60 sec
[1467205011.74][SERI][INF] serial(port=COM219, baudrate=9600, timeout=0)
Plugin info: HostTestPluginBase::BasePlugin: Waiting up to 60 sec for '0240000033514e450019500585d40008e981000097969900' serial port (current is 'COM219')...
[1467205011.83][SERI][INF] reset device using 'default' plugin...
[1467205012.08][SERI][INF] waiting 1.00 sec after reset
[1467205013.08][SERI][INF] wait for it...
[1467205013.08][SERI][TXD] mbedmbedmbedmbedmbedmbedmbedmbedmbedmbed
[1467205013.08][CONN][INF] sending up to 2 __sync packets (specified with --sync=2)
[1467205013.08][CONN][INF] sending preamble 'f82e0251-bb3e-4434-bc93-b780b5d0e82a'
[1467205013.08][SERI][TXD] {{__sync;f82e0251-bb3e-4434-bc93-b780b5d0e82a}}
[1467205013.22][CONN][RXD] {{__sync;f82e0251-bb3e-4434-bc93-b780b5d0e82a}}
[1467205013.22][CONN][INF] found SYNC in stream: {{__sync;f82e0251-bb3e-4434-bc93-b780b5d0e82a}} it is #0 sent, queued...
[1467205013.22][HTST][INF] sync KV found, uuid=f82e0251-bb3e-4434-bc93-b780b5d0e82a, timestamp=1467205013.219000
[1467205013.24][CONN][RXD] {{__version;1.1.0}}
[1467205013.24][CONN][INF] found KV pair in stream: {{__version;1.1.0}}, queued...
[1467205013.24][HTST][INF] DUT greentea-client version: 1.1.0
[1467205013.25][CONN][RXD] {{__timeout;20}}
[1467205013.26][CONN][INF] found KV pair in stream: {{__timeout;20}}, queued...
[1467205013.26][HTST][INF] setting timeout to: 20 sec
[1467205013.29][CONN][RXD] {{__host_test_name;default_auto}}
[1467205013.29][CONN][INF] found KV pair in stream: {{__host_test_name;default_auto}}, queued...
[1467205013.29][HTST][INF] host test class: '<class 'mbed_host_tests.host_tests.default_auto.DefaultAuto'>'
[1467205013.29][HTST][INF] host test setup() call...
[1467205013.29][HTST][INF] CALLBACKs updated
[1467205013.29][HTST][INF] host test detected: default_auto
[1467205013.31][CONN][RXD] {{__testcase_count;4}}
[1467205013.31][CONN][INF] found KV pair in stream: {{__testcase_count;4}}, queued...
[1467205013.34][CONN][RXD] >>> Running 4 test cases...
[1467205013.37][CONN][RXD] {{__testcase_name;Basic}}
[1467205013.37][CONN][INF] found KV pair in stream: {{__testcase_name;Basic}}, queued...
[1467205013.40][CONN][RXD] {{__testcase_name;Blinky}}
[1467205013.40][CONN][INF] found KV pair in stream: {{__testcase_name;Blinky}}, queued...
[1467205013.43][CONN][RXD] {{__testcase_name;C++ stack}}
[1467205013.43][CONN][INF] found KV pair in stream: {{__testcase_name;C++ stack}}, queued...
[1467205013.46][CONN][RXD] {{__testcase_name;C++ heap}}
[1467205013.46][CONN][INF] found KV pair in stream: {{__testcase_name;C++ heap}}, queued...
[1467205013.49][CONN][RXD] >>> Running case #1: 'Basic'...
[1467205013.52][CONN][RXD] {{__testcase_start;Basic}}
[1467205013.52][CONN][INF] found KV pair in stream: {{__testcase_start;Basic}}, queued...
[1467205013.56][CONN][RXD] {{__testcase_finish;Basic;1;0}}
[1467205013.56][CONN][INF] found KV pair in stream: {{__testcase_finish;Basic;1;0}}, queued...
[1467205013.59][CONN][RXD] >>> 'Basic': 1 passed, 0 failed
[1467205013.62][CONN][RXD] >>> Running case #2: 'Blinky'...
[1467205013.65][CONN][RXD] {{__testcase_start;Blinky}}
[1467205013.65][CONN][INF] found KV pair in stream: {{__testcase_start;Blinky}}, queued...
[1467205013.69][CONN][RXD] {{__testcase_finish;Blinky;1;0}}
[1467205013.69][CONN][INF] found KV pair in stream: {{__testcase_finish;Blinky;1;0}}, queued...
[1467205013.72][CONN][RXD] >>> 'Blinky': 1 passed, 0 failed
[1467205013.75][CONN][RXD] >>> Running case #3: 'C++ stack'...
[1467205013.78][CONN][RXD] {{__testcase_start;C++ stack}}
[1467205013.78][CONN][INF] found KV pair in stream: {{__testcase_start;C++ stack}}, queued...
[1467205013.79][CONN][RXD] Static::init
[1467205013.81][CONN][RXD] Static::stack_test
[1467205013.82][CONN][RXD] Stack::init
[1467205013.85][CONN][RXD] Stack::hello
[1467205013.87][CONN][RXD] Stack::destroy
[1467205013.89][CONN][RXD] Static::check_init: OK
[1467205013.91][CONN][RXD] Static::destroy
[1467205013.94][CONN][RXD] {{__testcase_finish;C++ stack;1;0}}
[1467205013.95][CONN][INF] found KV pair in stream: {{__testcase_finish;C++ stack;1;0}}, queued...
[1467205013.98][CONN][RXD] >>> 'C++ stack': 1 passed, 0 failed
[1467205014.02][CONN][RXD] >>> Running case #4: 'C++ heap'...
[1467205014.05][CONN][RXD] {{__testcase_start;C++ heap}}
[1467205014.05][CONN][INF] found KV pair in stream: {{__testcase_start;C++ heap}}, queued...
[1467205014.06][CONN][RXD] Heap::init
[1467205014.07][CONN][RXD] Heap::hello
[1467205014.10][CONN][RXD] Heap::check_init: OK
[1467205014.11][CONN][RXD] Heap::destroy
[1467205014.15][CONN][RXD] {{__testcase_finish;C++ heap;1;0}}
[1467205014.15][CONN][INF] found KV pair in stream: {{__testcase_finish;C++ heap;1;0}}, queued...
[1467205014.18][CONN][RXD] >>> 'C++ heap': 1 passed, 0 failed
[1467205014.22][CONN][RXD] >>> Test cases: 4 passed, 0 failed
[1467205014.25][CONN][RXD] {{__testcase_summary;4;0}}
[1467205014.25][CONN][INF] found KV pair in stream: {{__testcase_summary;4;0}}, queued...
[1467205014.27][CONN][RXD] {{end;success}}
[1467205014.27][CONN][INF] found KV pair in stream: {{end;success}}, queued...
[1467205014.28][CONN][RXD] {{__exit;0}}
[1467205014.28][CONN][INF] found KV pair in stream: {{__exit;0}}, queued...
[1467205014.28][HTST][INF] __exit(0)
[1467205014.28][HTST][INF] test suite run finished after 1.02 sec...
[1467205014.28][CONN][INF] received special even '__host_test_finished' value='True', finishing
[1467205014.31][HTST][INF] CONN exited with code: 0
[1467205014.31][HTST][INF] Some events in queue
[1467205014.31][HTST][INF] __notify_complete(True)
[1467205014.31][HTST][INF] stopped consuming events
[1467205014.31][HTST][INF] host test result() call skipped, received: True
[1467205014.31][HTST][INF] calling blocking teardown()
[1467205014.31][HTST][INF] teardown() finished
[1467205014.31][HTST][INF] {{result;success}}
"""

        self.OUTPUT_WITH_MEMORY_METRICS = """mbedgt: mbed-host-test-runner: started
[1459245860.90][CONN][RXD] {{__testcase_summary;4;0}}
[1459245860.92][CONN][INF] found KV pair in stream: {{end;success}}, queued...
[1459245860.92][CONN][RXD] {{end;success}}
[1459245860.92][CONN][INF] found KV pair in stream: {{max_heap_usage;2284}}, queued...
[1459245860.92][CONN][RXD] {{end;success}}
[1459245860.92][CONN][INF] found KV pair in stream: {{reserved_heap;124124}}, queued...
[1459245860.92][CONN][RXD] {{end;success}}
[1459245860.92][CONN][INF] found KV pair in stream: {{__thread_info;"BE-EF",42,24}}, queued...
[1459245860.92][CONN][RXD] {{end;success}}
[1459245860.92][HTST][INF] __notify_complete(True)
[1459245860.92][CONN][RXD] {{__coverage_start;c:\Work\core-util/source/PoolAllocator.cpp.gcda;6164636772393034c2733f32...a33e...b9}}
[1459245860.92][HTST][INF] test suite run finished after 0.90 sec...
[1459245860.94][HTST][INF] CONN exited with code: 0
[1459245860.94][HTST][INF] No events in queue
[1459245860.94][HTST][INF] stopped consuming events
[1459245860.94][HTST][INF] host test result() call skipped, received: True
[1459245860.94][HTST][WRN] missing __exit event from DUT
[1459245860.94][HTST][INF] calling blocking teardown()
[1459245860.94][HTST][INF] teardown() finished
[1459245860.94][HTST][INF] {{result;success}}
"""

    def tearDown(self):
        pass

    def test_get_test_result(self):
        self.assertEqual(mbed_test_api.TEST_RESULT_OK, mbed_test_api.get_test_result(self.OUTPUT_SUCCESS))
        self.assertEqual(mbed_test_api.TEST_RESULT_FAIL, mbed_test_api.get_test_result(self.OUTPUT_FAILURE))
        self.assertEqual(mbed_test_api.TEST_RESULT_TIMEOUT, mbed_test_api.get_test_result(self.OUTPUT_TIMEOUT))
        self.assertEqual(mbed_test_api.TEST_RESULT_UNDEF, mbed_test_api.get_test_result(self.OUTPUT_UNDEF))

    def test_get_test_result_ok_len(self):
        r = mbed_test_api.get_testcase_utest(self.OUTOUT_CSTRING_TEST, 'C strings: %e %E float formatting')

        self.assertEqual(len(r), 6)
        self.assertIn("[1459246276.41][CONN][RXD] >>> Running case #7: 'C strings: %e %E float formatting'...", r)
        self.assertIn("[1459246276.46][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %e %E float formatting}}, queued...", r)
        self.assertIn("[1459246276.46][CONN][RXD] {{__testcase_start;C strings: %e %E float formatting}}", r)
        self.assertIn("[1459246276.52][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %e %E float formatting;1;0}}, queued...", r)
        self.assertIn("[1459246276.53][CONN][RXD] {{__testcase_finish;C strings: %e %E float formatting;1;0}}", r)
        self.assertIn("[1459246276.59][CONN][RXD] >>> 'C strings: %e %E float formatting': 1 passed, 0 failed", r)


    def test_get_test_result_fail_len(self):
        r = mbed_test_api.get_testcase_utest(self.OUTOUT_CSTRING_TEST, 'C strings: %f %f float formatting')

        self.assertEqual(len(r), 9)
        self.assertIn("[1459246275.88][CONN][RXD] >>> Running case #6: 'C strings: %f %f float formatting'...", r)
        self.assertIn("[1459246275.94][CONN][INF] found KV pair in stream: {{__testcase_start;C strings: %f %f float formatting}}, queued...", r)
        self.assertIn("[1459246275.94][CONN][RXD] {{__testcase_start;C strings: %f %f float formatting}}", r)
        self.assertIn("[1459246276.10][CONN][RXD] :57::FAIL: Expected '0.002000 0.924300 15.913200 791.773680 6208.200000 25719.495200 426815.982588 6429271.046000 42468024.930000 212006462.910000' Was '", r)
        self.assertIn("'", r)
        self.assertIn("[1459246276.18][CONN][RXD] >>> failure with reason 'Assertion Failed' during 'Case Handler'", r)
        self.assertIn("[1459246276.25][CONN][INF] found KV pair in stream: {{__testcase_finish;C strings: %f %f float formatting;0;1}}, queued...", r)
        self.assertIn("[1459246276.25][CONN][RXD] {{__testcase_finish;C strings: %f %f float formatting;0;1}}", r)
        self.assertIn("[1459246276.34][CONN][RXD] >>> 'C strings: %f %f float formatting': 0 passed, 1 failed with reason 'Test Cases Failed'", r)

    def get_testcase_count_and_names(self):
        tc_count, tc_names = mbed_test_api.get_testcase_count_and_names(self.OUTOUT_CSTRING_TEST_CASE_COUNT_AND_NAME)

        self.assertEqual(tc_count, 2)
        self.assertIn('C strings: strtok', tc_names)
        self.assertIn('C strings: strpbrk', tc_names)

    def test_get_test_result_return_val(self):

        test_case_names = [
            'C strings: %e %E float formatting',
            'C strings: %g %g float formatting',
            'C strings: %i %d integer formatting',
            'C strings: %u %d integer formatting',
            'C strings: %x %E integer formatting',
            'C strings: strpbrk',
            'C strings: strtok'
        ]

        for test_case in test_case_names:
            r = mbed_test_api.get_testcase_utest(self.OUTOUT_CSTRING_TEST, test_case)
            self.assertEqual(len(r), 6)

        # This failing test case has different long lenght
        r = mbed_test_api.get_testcase_utest(self.OUTOUT_CSTRING_TEST, 'C strings: %f %f float formatting')
        self.assertEqual(len(r), 9)

    def test_get_testcase_summary_failures(self):
        r = mbed_test_api.get_testcase_summary("{{__testcase_summary;;}}")
        self.assertEqual(None, r)

        r = mbed_test_api.get_testcase_summary("{{__testcase_summary;-1;-2}}")
        self.assertEqual(None, r)

        r = mbed_test_api.get_testcase_summary("{{__testcase_summary;A;0}}")
        self.assertEqual(None, r)

    def test_get_testcase_summary_value_failures(self):
        r = mbed_test_api.get_testcase_summary("[1459246276.95][CONN][INF] found KV pair in stream: {{__testcase_summary;;}}")
        self.assertEqual(None, r)

        r = mbed_test_api.get_testcase_summary("[1459246276.95][CONN][INF] found KV pair in stream: {{__testcase_summary;-1;-2}}")
        self.assertEqual(None, r)

        r = mbed_test_api.get_testcase_summary("[1459246276.95][CONN][INF] found KV pair in stream: {{__testcase_summary;A;0}}")
        self.assertEqual(None, r)

    def test_get_testcase_summary_ok(self):

        r = mbed_test_api.get_testcase_summary("[1459246276.95][CONN][INF] found KV pair in stream: {{__testcase_summary;0;0}}")
        self.assertNotEqual(None, r)
        self.assertEqual((0, 0), r)

        r = mbed_test_api.get_testcase_summary(self.OUTOUT_CSTRING_TEST)
        self.assertNotEqual(None, r)
        self.assertEqual((7, 1), r)     # {{__testcase_summary;7;1}}

        r = mbed_test_api.get_testcase_summary(self.OUTPUT_SUCCESS)
        self.assertNotEqual(None, r)
        self.assertEqual((4, 0), r)     # {{__testcase_summary;4;0}}

    def test_get_testcase_result(self):
        r = mbed_test_api.get_testcase_result(self.OUTOUT_CSTRING_TEST)
        self.assertEqual(len(r), 8)

        test_case_names = [
            'C strings: %e %E float formatting',
            'C strings: %g %g float formatting',
            'C strings: %i %d integer formatting',
            'C strings: %u %d integer formatting',
            'C strings: %x %E integer formatting',
            'C strings: strpbrk',
            'C strings: strtok'
        ]

        for test_case in test_case_names:
            tc = r[test_case]
            # If data structure is correct
            self.assertIn('utest_log', tc)
            self.assertIn('time_start', tc)
            self.assertIn('time_end', tc)
            self.assertIn('failed', tc)
            self.assertIn('result', tc)
            self.assertIn('passed', tc)
            self.assertIn('duration', tc)
            # values passed
            self.assertEqual(tc['passed'], 1)
            self.assertEqual(tc['failed'], 0)
            self.assertEqual(tc['result_text'], 'OK')

        # Failing test case
        tc = r['C strings: %f %f float formatting']
        self.assertEqual(tc['passed'], 0)
        self.assertEqual(tc['failed'], 1)
        self.assertEqual(tc['result_text'], 'FAIL')

    def test_get_testcase_result_tescase_name_and_count(self):
        r = mbed_test_api.get_testcase_result(self.OUTOUT_GENERIC_TESTS_TESCASE_NAME_AND_COUNT)
        self.assertEqual(len(r), 4)

        self.assertIn('Basic', r)
        self.assertIn('Blinky', r)
        self.assertIn('C++ heap', r)
        self.assertIn('C++ stack', r)

    def test_get_testcase_result_tescase_name_and_count(self):
        r = mbed_test_api.get_testcase_result(self.OUTOUT_CSTRING_TEST_CASE_COUNT_AND_NAME)
        self.assertEqual(len(r), 2)

        self.assertIn('C strings: strpbrk', r)
        self.assertIn('C strings: strtok', r)

        self.assertEqual(r['C strings: strpbrk']['result_text'], 'SKIPPED')
        self.assertEqual(r['C strings: strtok']['result_text'], 'ERROR')


    def test_get_test_results_empty_output(self):
        result = mbed_test_api.get_test_result("")
        self.assertEqual(result, "TIMEOUT")

    def test_get_memory_metrics(self):
        result = mbed_test_api.get_memory_metrics(self.OUTPUT_WITH_MEMORY_METRICS)

        self.assertEqual(result[0], 2284)
        self.assertEqual(result[1], 124124)
        thread_info = list(result[2])[0]
        self.assertIn("entry", thread_info)
        self.assertEqual(thread_info["entry"], "BE")
        self.assertIn("stack_size", thread_info)
        self.assertEqual(thread_info["stack_size"], 24)
        self.assertIn("max_stack", thread_info)
        self.assertEqual(thread_info["max_stack"], 42)
        self.assertIn("arg", thread_info)
        self.assertEqual(thread_info["arg"], "EF")

    def test_get_testcase_result_start_tag_missing(self):
        result = mbed_test_api.get_testcase_result(self.OUTPUT_STARTTAG_MISSING)
        self.assertEqual(result['DNS query']['utest_log'], "__testcase_start tag not found.")

if __name__ == '__main__':
    unittest.main()
