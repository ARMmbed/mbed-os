# Copyright (c) 2018, Arm Limited and affiliates.
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import re
import sys
import traceback
from time import time
from sre_compile import error

from multiprocessing import Process, Queue
from .. import host_tests_plugins, BaseHostTest
from ..host_tests_registry import HostRegistry

# Host test supervisors
from ..host_tests.echo import EchoTest
from ..host_tests.rtc_auto import RTCTest
from ..host_tests.hello_auto import HelloTest
from ..host_tests.detect_auto import DetectPlatformTest
from ..host_tests.wait_us_auto import WaitusTest
from ..host_tests.default_auto import DefaultAuto
from ..host_tests.dev_null_auto import DevNullTest

from .host_test import DefaultTestSelectorBase
from ..host_tests_logger import HtrunLogger
from ..host_tests_conn_proxy import conn_process
from ..host_tests_toolbox.host_functional import handle_send_break_cmd
if (sys.version_info > (3, 0)):
    from queue import Empty as QueueEmpty
else:
    from Queue import Empty as QueueEmpty


class DefaultTestSelector(DefaultTestSelectorBase):
    """! Select default host_test supervision (replaced after auto detection) """
    RESET_TYPE_SW_RST   = "software_reset"
    RESET_TYPE_HW_RST   = "hardware_reset"

    def __init__(self, options):
        """! ctor
        """
        self.options = options

        self.logger = HtrunLogger('HTST')

        self.registry = HostRegistry()
        self.registry.register_host_test("echo", EchoTest())
        self.registry.register_host_test("default", DefaultAuto())
        self.registry.register_host_test("rtc_auto", RTCTest())
        self.registry.register_host_test("hello_auto", HelloTest())
        self.registry.register_host_test("detect_auto", DetectPlatformTest())
        self.registry.register_host_test("default_auto", DefaultAuto())
        self.registry.register_host_test("wait_us_auto", WaitusTest())
        self.registry.register_host_test("dev_null_auto", DevNullTest())

        # Handle extra command from
        if options:
            if options.enum_host_tests:
                for path in options.enum_host_tests:
                    self.registry.register_from_path(
                        path, verbose=options.verbose
                    )

            if options.list_reg_hts:    # --list option
                print(self.registry.table(options.verbose))
                sys.exit(0)

            if options.list_plugins:    # --plugins option
                host_tests_plugins.print_plugin_info()
                sys.exit(0)

            if options.global_resource_mgr or options.fast_model_connection:
                # If Global/Simulator Resource Mgr is working it will handle reset/flashing workflow
                # So local plugins are offline
                self.options.skip_reset = True
                self.options.skip_flashing = True

        if options.compare_log:
            with open(options.compare_log, "r") as f:
                self.compare_log = f.read().splitlines()

        else:
            self.compare_log = None
        self.serial_output_file = options.serial_output_file
        self.compare_log_idx = 0
        DefaultTestSelectorBase.__init__(self, options)

    def is_host_test_obj_compatible(self, obj_instance):
        """! Check if host test object loaded is actually host test class
             derived from 'mbed_os_tools.test.BaseHostTest()'
             Additionaly if host test class implements custom ctor it should
             call BaseHostTest().__Init__()
        @param obj_instance Instance of host test derived class
        @return True if obj_instance is derived from mbed_os_tools.test.BaseHostTest()
                and BaseHostTest.__init__() was called, else return False
        """
        result = False
        if obj_instance:
            result = True
            self.logger.prn_inf("host test class: '%s'"% obj_instance.__class__)

            # Check if host test (obj_instance) is derived from mbed_os_tools.test.BaseHostTest()
            if not isinstance(obj_instance, BaseHostTest):
                # In theory we should always get host test objects inheriting from BaseHostTest()
                # because loader will only load those.
                self.logger.prn_err("host test must inherit from mbed_os_tools.test.BaseHostTest() class")
                result = False

            # Check if BaseHostTest.__init__() was called when custom host test is created
            if not obj_instance.base_host_test_inited():
                self.logger.prn_err("custom host test __init__() must call BaseHostTest.__init__(self)")
                result = False

        return result

    def run_test(self):
        """! This function implements key-value protocol state-machine.
            Handling of all events and connector are handled here.
        @return Return self.TestResults.RESULT_* enum
        """
        result = None
        timeout_duration = 10       # Default test case timeout
        coverage_idle_timeout = 10  # Default coverage idle timeout
        event_queue = Queue()       # Events from DUT to host
        dut_event_queue = Queue()   # Events from host to DUT {k;v}

        def callback__notify_prn(key, value, timestamp):
            """! Handles __norify_prn. Prints all lines in separate log line """
            for line in value.splitlines():
                self.logger.prn_inf(line)

        callbacks = {
            "__notify_prn" : callback__notify_prn
        }

        # if True we will allow host test to consume all events after test is finished
        callbacks_consume = True
        # Flag check if __exit event occurred
        callbacks__exit = False
        # Flag check if __exit_event_queue event occurred
        callbacks__exit_event_queue = False
        # Handle to dynamically loaded host test object
        self.test_supervisor = None
        # Version: greentea-client version from DUT
        self.client_version = None

        self.logger.prn_inf("starting host test process...")


        # Create device info here as it may change after restart.
        config = {
            "digest" : "serial",
            "port" : self.mbed.port,
            "baudrate" : self.mbed.serial_baud,
            "mcu" : self.mbed.mcu,
            "program_cycle_s" : self.options.program_cycle_s,
            "reset_type" : self.options.forced_reset_type,
            "target_id" : self.options.target_id,
            "disk" : self.options.disk,
            "polling_timeout" : self.options.polling_timeout,
            "forced_reset_timeout" : self.options.forced_reset_timeout,
            "sync_behavior" : self.options.sync_behavior,
            "platform_name" : self.options.micro,
            "image_path" : self.mbed.image_path,
            "skip_reset": self.options.skip_reset,
            "tags" : self.options.tag_filters,
            "sync_timeout": self.options.sync_timeout
        }

        if self.options.global_resource_mgr:
            grm_config = self._parse_grm(self.options.global_resource_mgr)
            grm_config["conn_resource"] = "grm"
            config.update(grm_config)

        if self.options.fast_model_connection:

            config.update({
                "conn_resource" : 'fmc',
                "fm_config" : self.options.fast_model_connection
            })

        def start_conn_process():
            # DUT-host communication process
            args = (event_queue, dut_event_queue, config)
            p = Process(target=conn_process, args=args)
            p.deamon = True
            p.start()
            return p

        def process_code_coverage(key, value, timestamp):
            """! Process the found coverage key value and perform an idle
                 loop checking for more timeing out if there is no response from
                 the target within the idle timeout.
            @param key The key from the first coverage event
            @param value The value from the first coverage event
            @param timestamp The timestamp from the first coverage event
            @return The elapsed time taken by the processing of code coverage,
                    and the (key, value, and timestamp) of the next event
            """
            original_start_time = time()
            start_time = time()

            # Perform callback on first event
            callbacks[key](key, value, timestamp)

            # Start idle timeout loop looking for other events
            while (time() - start_time) < coverage_idle_timeout:
                try:
                    (key, value, timestamp) = event_queue.get(timeout=1)
                except QueueEmpty:
                    continue

                # If coverage detected use idle loop
                # Prevent breaking idle loop for __rxd_line (occurs between keys)
                if key == '__coverage_start' or key == '__rxd_line':
                    start_time = time()

                    # Perform callback
                    callbacks[key](key, value, timestamp)
                    continue

                elapsed_time = time() - original_start_time
                return elapsed_time, (key, value, timestamp)

        p = start_conn_process()
        conn_process_started = False
        try:
            # Wait for the start event. Process start timeout does not apply in
            # Global resource manager case as it may take a while for resource
            # to be available.
            (key, value, timestamp) = event_queue.get(
                timeout=None if self.options.global_resource_mgr else self.options.process_start_timeout)

            if key == '__conn_process_start':
                conn_process_started = True
            else:
                self.logger.prn_err("First expected event was '__conn_process_start', received '%s' instead"% key)

        except QueueEmpty:
            self.logger.prn_err("Conn process failed to start in %f sec"% self.options.process_start_timeout)

        if not conn_process_started:
            p.terminate()
            return self.RESULT_TIMEOUT

        start_time = time()

        try:
            consume_preamble_events = True

            while (time() - start_time) < timeout_duration:
                # Handle default events like timeout, host_test_name, ...
                try:
                    (key, value, timestamp) = event_queue.get(timeout=1)
                except QueueEmpty:
                    continue

                # Write serial output to the file if specified in options.
                if self.serial_output_file:
                    if key == '__rxd_line':
                        with open(self.serial_output_file, "a") as f:
                            f.write("%s\n" % value)

                # In this mode we only check serial output against compare log.
                if self.compare_log:
                    if key == '__rxd_line':
                        if self.match_log(value):
                            self.logger.prn_inf("Target log matches compare log!")
                            result = True
                            break

                if consume_preamble_events:
                    if key == '__timeout':
                        # Override default timeout for this event queue
                        start_time = time()
                        timeout_duration = int(value) # New timeout
                        self.logger.prn_inf("setting timeout to: %d sec"% int(value))
                    elif key == '__version':
                        self.client_version = value
                        self.logger.prn_inf("DUT greentea-client version: " + self.client_version)
                    elif key == '__host_test_name':
                        # Load dynamically requested host test
                        self.test_supervisor = self.registry.get_host_test(value)

                        # Check if host test object loaded is actually host test class
                        # derived from 'mbed_os_tools.test.BaseHostTest()'
                        # Additionaly if host test class implements custom ctor it should
                        # call BaseHostTest().__Init__()
                        if self.test_supervisor and self.is_host_test_obj_compatible(self.test_supervisor):
                            # Pass communication queues and setup() host test
                            self.test_supervisor.setup_communication(event_queue, dut_event_queue, config)
                            try:
                                # After setup() user should already register all callbacks
                                self.test_supervisor.setup()
                            except (TypeError, ValueError):
                                # setup() can throw in normal circumstances TypeError and ValueError
                                self.logger.prn_err("host test setup() failed, reason:")
                                self.logger.prn_inf("==== Traceback start ====")
                                for line in traceback.format_exc().splitlines():
                                    print(line)
                                self.logger.prn_inf("==== Traceback end ====")
                                result = self.RESULT_ERROR
                                event_queue.put(('__exit_event_queue', 0, time()))

                            self.logger.prn_inf("host test setup() call...")
                            if self.test_supervisor.get_callbacks():
                                callbacks.update(self.test_supervisor.get_callbacks())
                                self.logger.prn_inf("CALLBACKs updated")
                            else:
                                self.logger.prn_wrn("no CALLBACKs specified by host test")
                            self.logger.prn_inf("host test detected: %s"% value)
                        else:
                            self.logger.prn_err("host test not detected: %s"% value)
                            result = self.RESULT_ERROR
                            event_queue.put(('__exit_event_queue', 0, time()))

                        consume_preamble_events = False
                    elif key == '__sync':
                        # This is DUT-Host Test handshake event
                        self.logger.prn_inf("sync KV found, uuid=%s, timestamp=%f"% (str(value), timestamp))
                    elif key == '__notify_sync_failed':
                        # This event is sent by conn_process, SYNC failed
                        self.logger.prn_err(value)
                        self.logger.prn_wrn("stopped to consume events due to %s event"% key)
                        callbacks_consume = False
                        result = self.RESULT_SYNC_FAILED
                        event_queue.put(('__exit_event_queue', 0, time()))
                    elif key == '__notify_conn_lost':
                        # This event is sent by conn_process, DUT connection was lost
                        self.logger.prn_err(value)
                        self.logger.prn_wrn("stopped to consume events due to %s event"% key)
                        callbacks_consume = False
                        result = self.RESULT_IO_SERIAL
                        event_queue.put(('__exit_event_queue', 0, time()))
                    elif key == '__exit_event_queue':
                        # This event is sent by the host test indicating no more events expected
                        self.logger.prn_inf("%s received"% (key))
                        callbacks__exit_event_queue = True
                        break
                    elif key.startswith('__'):
                        # Consume other system level events
                        pass
                    else:
                        self.logger.prn_err("orphan event in preamble phase: {{%s;%s}}, timestamp=%f"% (key, str(value), timestamp))
                else:
                    # If coverage detected switch to idle loop
                    if key == '__coverage_start':
                        self.logger.prn_inf("starting coverage idle timeout loop...")
                        elapsed_time, (key, value, timestamp) = process_code_coverage(key, value, timestamp)

                        # Ignore the time taken by the code coverage
                        timeout_duration += elapsed_time
                        self.logger.prn_inf("exiting coverage idle timeout loop (elapsed_time: %.2f" % elapsed_time)

                    if key == '__notify_complete':
                        # This event is sent by Host Test, test result is in value
                        # or if value is None, value will be retrieved from HostTest.result() method
                        self.logger.prn_inf("%s(%s)" % (key, str(value)))
                        result = value
                        event_queue.put(('__exit_event_queue', 0, time()))
                    elif key == '__reset':
                        # This event only resets the dut, not the host test
                        dut_event_queue.put(('__reset', True, time()))
                    elif key == '__reset_dut':
                        # Disconnect to avoid connection lost event
                        dut_event_queue.put(('__host_test_finished', True, time()))
                        p.join()

                        if value == DefaultTestSelector.RESET_TYPE_SW_RST:
                            self.logger.prn_inf("Performing software reset.")
                            # Just disconnecting and re-connecting comm process will soft reset DUT
                        elif value == DefaultTestSelector.RESET_TYPE_HW_RST:
                            self.logger.prn_inf("Performing hard reset.")
                            # request hardware reset
                            self.mbed.hw_reset()
                        else:
                            self.logger.prn_err("Invalid reset type (%s). Supported types [%s]." %
                                                (value, ", ".join([DefaultTestSelector.RESET_TYPE_HW_RST,
                                                                   DefaultTestSelector.RESET_TYPE_SW_RST])))
                            self.logger.prn_inf("Software reset will be performed.")

                        # connect to the device
                        p = start_conn_process()
                    elif key == '__notify_conn_lost':
                        # This event is sent by conn_process, DUT connection was lost
                        self.logger.prn_err(value)
                        self.logger.prn_wrn("stopped to consume events due to %s event"% key)
                        callbacks_consume = False
                        result = self.RESULT_IO_SERIAL
                        event_queue.put(('__exit_event_queue', 0, time()))
                    elif key == '__exit':
                        # This event is sent by DUT, test suite exited
                        self.logger.prn_inf("%s(%s)"% (key, str(value)))
                        callbacks__exit = True
                        event_queue.put(('__exit_event_queue', 0, time()))
                    elif key == '__exit_event_queue':
                        # This event is sent by the host test indicating no more events expected
                        self.logger.prn_inf("%s received"% (key))
                        callbacks__exit_event_queue = True
                        break
                    elif key == '__timeout_set':
                        # Dynamic timeout set
                        timeout_duration = int(value) # New timeout
                        self.logger.prn_inf("setting timeout to: %d sec"% int(value))
                    elif key == '__timeout_adjust':
                        # Dynamic timeout adjust
                        timeout_duration = timeout_duration + int(value) # adjust time
                        self.logger.prn_inf("adjusting timeout with %d sec (now %d)" % (int(value), timeout_duration))
                    elif key in callbacks:
                        # Handle callback
                        callbacks[key](key, value, timestamp)
                    else:
                        self.logger.prn_err("orphan event in main phase: {{%s;%s}}, timestamp=%f"% (key, str(value), timestamp))
        except Exception:
            self.logger.prn_err("something went wrong in event main loop!")
            self.logger.prn_inf("==== Traceback start ====")
            for line in traceback.format_exc().splitlines():
                print(line)
            self.logger.prn_inf("==== Traceback end ====")
            result = self.RESULT_ERROR

        time_duration = time() - start_time
        self.logger.prn_inf("test suite run finished after %.2f sec..."% time_duration)

        if self.compare_log and result is None:
            if self.compare_log_idx < len(self.compare_log):
                self.logger.prn_err("Expected output [%s] not received in log." % self.compare_log[self.compare_log_idx])

        # Force conn_proxy process to return
        dut_event_queue.put(('__host_test_finished', True, time()))
        p.join()
        self.logger.prn_inf("CONN exited with code: %s"% str(p.exitcode))

        # Callbacks...
        self.logger.prn_inf("No events in queue" if event_queue.empty() else "Some events in queue")

        # If host test was used we will:
        # 1. Consume all existing events in queue if consume=True
        # 2. Check result from host test and call teardown()

        # NOTE: with the introduction of the '__exit_event_queue' event, there
        # should never be left events assuming the DUT has stopped sending data
        # over the serial data. Leaving this for now to catch anything that slips through.

        if callbacks_consume:
            # We are consuming all remaining events if requested
            while not event_queue.empty():
                try:
                    (key, value, timestamp) = event_queue.get(timeout=1)
                except QueueEmpty:
                    break

                if key == '__notify_complete':
                    # This event is sent by Host Test, test result is in value
                    # or if value is None, value will be retrieved from HostTest.result() method
                    self.logger.prn_inf("%s(%s)"% (key, str(value)))
                    result = value
                elif key.startswith('__'):
                    # Consume other system level events
                    pass
                elif key in callbacks:
                    callbacks[key](key, value, timestamp)
                else:
                    self.logger.prn_wrn(">>> orphan event: {{%s;%s}}, timestamp=%f"% (key, str(value), timestamp))
            self.logger.prn_inf("stopped consuming events")

        if result is not None:  # We must compare here against None!
            # Here for example we've received some error code like IOERR_COPY
            self.logger.prn_inf("host test result() call skipped, received: %s"% str(result))
        else:
            if self.test_supervisor:
                result = self.test_supervisor.result()
            self.logger.prn_inf("host test result(): %s"% str(result))

        if not callbacks__exit:
            self.logger.prn_wrn("missing __exit event from DUT")

        if not callbacks__exit_event_queue:
            self.logger.prn_wrn("missing __exit_event_queue event from host test")

        #if not callbacks__exit_event_queue and not result:
        if not callbacks__exit_event_queue and result is None:
            self.logger.prn_err("missing __exit_event_queue event from " + \
                "host test and no result from host test, timeout...")
            result = self.RESULT_TIMEOUT

        self.logger.prn_inf("calling blocking teardown()")
        if self.test_supervisor:
            self.test_supervisor.teardown()
        self.logger.prn_inf("teardown() finished")

        return result

    def execute(self):
        """! Test runner for host test.

        @details This function will start executing test and forward test result via serial port
                 to test suite. This function is sensitive to work-flow flags such as --skip-flashing,
                 --skip-reset etc.
                 First function will flash device with binary, initialize serial port for communication,
                 reset target. On serial port handshake with test case will be performed. It is when host
                 test reads property data from serial port (sent over serial port).
                 At the end of the procedure proper host test (defined in set properties) will be executed
                 and test execution timeout will be measured.
        """
        result = self.RESULT_UNDEF

        # hello sting with htrun version, for debug purposes
        self.logger.prn_inf(self.get_hello_string())

        try:
            # Copy image to device
            if self.options.skip_flashing:
                self.logger.prn_inf("copy image onto target... SKIPPED!")
            else:
                self.logger.prn_inf("copy image onto target...")
                result = self.mbed.copy_image()
                if not result:
                    result = self.RESULT_IOERR_COPY
                    return self.get_test_result_int(result)

            # Execute test if flashing was successful or skipped
            test_result = self.run_test()

            if test_result == True:
                result = self.RESULT_SUCCESS
            elif test_result == False:
                result = self.RESULT_FAILURE
            elif test_result is None:
                result = self.RESULT_ERROR
            else:
                result = test_result

            # This will be captured by Greentea
            self.logger.prn_inf("{{result;%s}}"% result)
            return self.get_test_result_int(result)

        except KeyboardInterrupt:
            return(-3)    # Keyboard interrupt

    def match_log(self, line):
        """
        Matches lines from compare log with the target serial output. Compare log lines are matched in seq using index
        self.compare_log_idx. Lines can be strings to be matched as is or regular expressions.

        :param line:
        :return:
        """
        if self.compare_log_idx < len(self.compare_log):
            regex = self.compare_log[self.compare_log_idx]
            # Either the line is matched as is or it is checked as a regular expression.
            try:
                if regex in line or re.search(regex, line):
                    self.compare_log_idx += 1
            except error:
                # May not be a regular expression
                return False
        return self.compare_log_idx == len(self.compare_log)

    @staticmethod
    def _parse_grm(grm_arg):
        grm_module, leftover = grm_arg.split(':', 1)
        parts = leftover.rsplit(':', 1)

        try:
            grm_host, grm_port = parts
            _ = int(grm_port)
        except ValueError:
            # No valid port was found, so assume no port was supplied
            grm_host = leftover
            grm_port = None

        return {
            "grm_module" : grm_module,
            "grm_host" : grm_host,
            "grm_port" : grm_port,
        }
