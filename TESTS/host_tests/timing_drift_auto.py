"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

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

from mbed_host_tests import BaseHostTest


class TimingDriftTest(BaseHostTest):
    """ This test is reading single characters from stdio
        and measures time between their occurrences.
    """
    __result = None
    
    # This is calculated later: average_drift_max * number of tick events
    total_drift_max = None
    
    average_drift_max = 0.05
    ticks = []
    start_time = None
    finish_time = None
    dut_seconds_passed = None
    total_time = None
    total_drift = None
    average_drift = None
    
    def _callback_result(self, key, value, timestamp):
        # We should not see result data in this test
        self.__result = False

    def _callback_end(self, key, value, timestamp):
        """ {{end;%s}}} """
        self.log("Received end event, timestamp: %f" % timestamp)
        self.notify_complete(result=self.result(print_stats=True))


    def _callback_tick(self, key, value, timestamp):
        """ {{tick;%d}}} """
        self.log("tick! %f" % timestamp)
        self.ticks.append((key, value, timestamp))


    def setup(self):
        self.register_callback("end", self._callback_end)
        self.register_callback('tick', self._callback_tick)


    def result(self, print_stats=True):        
        self.dut_seconds_passed = len(self.ticks) - 1
        
        if self.dut_seconds_passed < 1:
            if print_stats:
                self.log("FAIL: failed to receive at least two tick events")
            self.__result = False
            return self.__result

        self.total_drift_max = self.dut_seconds_passed * self.average_drift_max

        self.start_time = self.ticks[0][2]
        self.finish_time = self.ticks[-1][2]
        self.total_time = self.finish_time - self.start_time
        self.total_drift = self.total_time - self.dut_seconds_passed
        self.average_drift = self.total_drift / self.dut_seconds_passed
        
        if print_stats:
            self.log("Start: %f" % self.start_time)
            self.log("Finish: %f" % self.finish_time)
            self.log("Total time taken: %f" % self.total_time)
        
            total_drift_ratio_string = "Total drift/Max total drift: %f/%f"
            self.log(total_drift_ratio_string % (self.total_drift,
                                                 self.total_drift_max))
                                                 
            average_drift_ratio_string = "Average drift/Max average drift: %f/%f"
            self.log(average_drift_ratio_string % (self.average_drift,
                                                   self.average_drift_max))
        

        if abs(self.total_drift) > self.total_drift_max:
            if print_stats:
                self.log("FAIL: Total drift exceeded max total drift")
            self.__result = False
        elif self.average_drift > self.average_drift_max:
            if print_stats:
                self.log("FAIL: Average drift exceeded max average drift")
            self.__result = False
        else:
            self.__result = True
        
        return self.__result


    def teardown(self):
        pass