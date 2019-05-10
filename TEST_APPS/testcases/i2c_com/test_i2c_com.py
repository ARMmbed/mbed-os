"""
Copyright (c) 2019, Arm Limited and affiliates.
SPDX-License-Identifier: Apache-2.0

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

from icetea_lib.bench import Bench
from icetea_lib.bench import TestStepError
from icetea_lib.tools.tools import test_case
from icetea_lib.TestStepError import TestStepFail
from icetea_lib.bench import ReturnCodes
#from mbed_clitest.TestStepError import SkippedTestcaseException
import re


class I2CComTest(Bench):

    capabilities_names = ("minimum_frequency", "maximum_frequency", "supports_slave_mode",
                          "supports_10bit_addressing", "supports_multi_master", "supports_clock_stretching", "supports_async")
    master_init_config_str = "test_init_master frequency: {0}"
    master_exec_rw_config_str = "test_exec_master communication: {0} iterations: {1} write_size: {2} read_size: {3} address{4}: {5} stop: {6}"
    master_exec_r_or_w_config_str = "test_exec_master communication: {0} iterations: {1} write_size: {2} write_resulting_size: {3} read_size: {4} read_resulting_size: {5} address{6}: {7}"
    slave_init_config_str = "test_init_slave frequency: {0}"
    slave_exec_rw_config_str = "test_exec_slave iterations: {0} write_size: {1} read_size: {2} address{3}: {4}"
    slave_exec_r_or_w_config_str = "test_exec_slave iterations: {0} write_size: {1} write_resulting_size: {2} read_size: {3} read_resulting_size: {4} address{5}: {6}"

    caps_master = {}
    caps_slave = {}
    caps = {}

    STOP_EACH_TRANSFER = 2
    STOP_EACH_ITERATION = 1
    STOP_AT_THE_END = 0

    I2C_7BIT_ADDRESS_MIN = 8
    I2C_7BIT_ADDRESS_MAX = 119
    I2C_7BIT_ADDRESS = 85
    address_range_7bit = (I2C_7BIT_ADDRESS_MIN,
                          I2C_7BIT_ADDRESS, I2C_7BIT_ADDRESS_MAX)
    I2C_10BIT_ADDRESS_MIN = 0
    I2C_10BIT_ADDRESS_MAX = 1023
    I2C_10BIT_ADDRESS = 512
    address_range_10bit = (I2C_10BIT_ADDRESS_MIN,
                           I2C_10BIT_ADDRESS, I2C_10BIT_ADDRESS_MAX)

    def __init__(self, **kwargs):
        testcase_args = {
            'name': "test_i2c_com",
            'title': "I2C Communication Test",
            'status': "released",
            'purpose': "Verify I2C HAL API",
                       'component': ["I2C"],
                       'type': "compatibility",
                       'requirements': {
                           "duts": {
                               '*': {
                                   "count": 2,
                                   "type": "hardware"
                               },
                               "1": {"nick": "master",
                                     "allowed_platforms": ["K64F", "NUCLEO_F070RB", "NUCLEO_F429ZI"],
                                     "application": {
                                         "name": "TEST_APPS-device-i2c_com"
                                     }},
                               "2": {"nick": "slave",
                                     "allowed_platforms": ["K64F", "NUCLEO_F070RB", "NUCLEO_F429ZI"],
                                     "application": {
                                         "name": "TEST_APPS-device-i2c_com"
                                     }}
                           }
                       }}

        testcase_args.update(kwargs)
        Bench.__init__(self, **testcase_args)

    def log_capabilities(self, cap):
        for name in self.capabilities_names:
            print("{}: {} ({})".format(name, cap[name], type(cap[name])))

    def parse_capabilities(self, resp):
        regex = re.compile(r'''
        [\S]+:                # a key (any word followed by a colon)
        (?:
        \s                    # then a space in between
        (?!\S+:)\S+       # then a value (any word not followed by a colon)
        )+                    # match multiple values if present
        ''', re.VERBOSE)
        matches = regex.findall(resp.lines[0])
        cap = dict([matches.split(': ', 1) for matches in matches])

        cap["minimum_frequency"] = int(cap["min_freq"])
        cap["maximum_frequency"] = int(cap["max_freq"])
        cap["supports_slave_mode"] = cap["slave_mode"] == "True"
        cap["supports_10bit_addressing"] = cap["10bit_addressing"] == "True"
        cap["supports_multi_master"] = cap["multi_master"] == "True"
        cap["supports_clock_stretching"] = cap["clock_stretching"] == "True"
        cap["supports_async"] = cap["async"] == "True"

        return cap

    def get_capabilities(self):
        resp_slave = self.command("slave", "get_capabilities")
        self.caps_slave = self.parse_capabilities(resp_slave)

        resp_master = self.command("master", "get_capabilities")
        self.caps_master = self.parse_capabilities(resp_master)

    def set_test_capabilities(self):
        self.caps["minimum_frequency"] = max(
            self.caps_slave["minimum_frequency"], self.caps_master["minimum_frequency"])
        self.caps["maximum_frequency"] = min(
            self.caps_slave["maximum_frequency"], self.caps_master["maximum_frequency"])
        self.caps["supports_slave_mode"] = self.caps_slave["supports_slave_mode"] and self.caps_master["supports_slave_mode"]
        self.caps["supports_10bit_addressing"] = self.caps_slave["supports_10bit_addressing"] and self.caps_master["supports_10bit_addressing"]
        self.caps["supports_multi_master"] = self.caps_slave["supports_multi_master"] and self.caps_master["supports_multi_master"]
        self.caps["supports_clock_stretching"] = self.caps_slave["supports_clock_stretching"] and self.caps_master["supports_clock_stretching"]
        self.caps["supports_async"] = self.caps_slave["supports_async"] and self.caps_master["supports_async"]

    def test_execute(self, config):

        if not self.caps_slave["supports_slave_mode"]:
            raise TestStepFail("Slave mode UNSUPPORTED")
            #raise SkippedTestcaseException("Slave mode UNSUPPORTED")

        slave_init_config = self.slave_init_config_str.format(
            config["frequency"])
        master_init_config = self.master_init_config_str.format(
            config["frequency"])

        addressing_types = {"7bit": (0, 7), "10bit": (1, 10)}

        for addressing_key, addressing_value in addressing_types.iteritems():
            if addressing_key == "10bit" and not self.caps["supports_10bit_addressing"]:
                continue
            for stop in config["stop"]:
                for address in config["address"][addressing_value[0]]:
                    if type(config["write_size"]) is tuple and type(config["read_size"]) is tuple:
                        slave_exec_config = self.slave_exec_r_or_w_config_str.format(
                            config["iterations"], config["write_size"][2], config["write_size"][3], config["read_size"][2], config["read_size"][3], addressing_value[1], address)
                        master_exec_config = self.master_exec_r_or_w_config_str.format(
                            config["communication"], config["iterations"], config["write_size"][0], config["write_size"][1], config["read_size"][0], config["read_size"][1], addressing_value[1], address)
                    else:
                        slave_exec_config = self.slave_exec_rw_config_str.format(
                            config["iterations"], config["write_size"], config["read_size"], addressing_value[1], address)
                        master_exec_config = self.master_exec_rw_config_str.format(
                            config["communication"], config["iterations"], config["write_size"], config["read_size"], addressing_value[1], address, stop)

                    resp_slave_init = self.command(
                        "slave", slave_init_config, report_cmd_fail=False)
                    resp_master_init = self.command(
                        "master", master_init_config, report_cmd_fail=False)

                    async_slave_exec = self.command(
                        "slave", slave_exec_config, asynchronous=True)
                    resp_master_exec = self.command(
                        "master", master_exec_config, report_cmd_fail=False)

                    resp_slave_finish = self.command(
                        "slave", "test_stop_slave", report_cmd_fail=False)
                    resp_slave_exec = self.wait_for_async_response(
                        slave_exec_config, async_slave_exec)

                    self.command("slave", "test_deinit_slave",
                                report_cmd_fail=False)
                    self.command("master", "test_deinit_master",
                                report_cmd_fail=False)

                    master_error = resp_master_init.retcode != ReturnCodes.RETCODE_SUCCESS or resp_master_exec.retcode != ReturnCodes.RETCODE_SUCCESS
                    slave_error = resp_slave_init.retcode != ReturnCodes.RETCODE_SUCCESS or resp_slave_exec.retcode != ReturnCodes.RETCODE_SUCCESS

                    if(master_error or slave_error):
                        raise TestStepFail("Communication failure")


    def setup(self):
        self.get_capabilities()
        self.set_test_capabilities()

    def teardown(self):
        pass

###########################
# blocking_write-read
###########################

# minimum_frequency

@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_1B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_1B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "blocking_write-read", "iterations": 1000,
                       "write_size": 1, "read_size": 1, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_10B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_10B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "blocking_write-read", "iterations": 100,
                       "write_size": 10, "read_size": 10, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_257B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_257B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "blocking_write-read", "iterations": 10,
                       "write_size": 257, "read_size": 257, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_1025B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_1025B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "blocking_write-read", "iterations": 1,
                       "write_size": 1025, "read_size": 1025, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

# maximum_frequency

@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_1B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_1B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "blocking_write-read", "iterations": 1000,
                       "write_size": 1, "read_size": 1, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_10B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_10B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "blocking_write-read", "iterations": 100,
                       "write_size": 10, "read_size": 10, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})


@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_257B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_257B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "blocking_write-read", "iterations": 10,
                       "write_size": 257, "read_size": 257, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_BLOCKING_WRITE-READ_1025B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_BLOCKING_WRITE_READ_1025B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "blocking_write-read", "iterations": 1,
                       "write_size": 1025, "read_size": 1025, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

###########################
# async_write-read
###########################

# minimum_frequency

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_1B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_1B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_write-read", "iterations": 1000,
                       "write_size": 1, "read_size": 1, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_10B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_10B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_write-read", "iterations": 100,
                       "write_size": 10, "read_size": 10, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_257B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_257B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_write-read", "iterations": 10,
                       "write_size": 257, "read_size": 257, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_1025B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_1025B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_write-read", "iterations": 1,
                       "write_size": 1025, "read_size": 1025, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

# maximum_frequency

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_1B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_1B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_write-read", "iterations": 1000,
                       "write_size": 1, "read_size": 1, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_10B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_10B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_write-read", "iterations": 100,
                       "write_size": 10, "read_size": 10, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_257B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_257B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_write-read", "iterations": 10,
                       "write_size": 257, "read_size": 257, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_WRITE-READ_1025B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_WRITE_READ_1025B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_write-read", "iterations": 1,
                       "write_size": 1025, "read_size": 1025, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

###########################
# async_transfer
###########################

# minimum_frequency

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_1B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_TRANSFER_1B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_transfer", "iterations": 1000,
                       "write_size": 1, "read_size": 1, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_10B_MIN_FREQUENCY",
           title="test write read")
def ASYNCI2C_COM_ASYNC_TRANSFER_10B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_transfer", "iterations": 100,
                       "write_size": 10, "read_size": 10, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_257B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_TRANSFER_257B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_transfer", "iterations": 10,
                       "write_size": 257, "read_size": 257, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_1025B_MIN_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_TRANSFER_1025B_MIN_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["minimum_frequency"], "communication": "async_transfer", "iterations": 1,
                       "write_size": 1025, "read_size": 1025, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

# maximum_frequency

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_1B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_TRANSFER_1B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_transfer", "iterations": 1000,
                       "write_size": 1, "read_size": 1, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_10B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_TRANSFER_10B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_transfer", "iterations": 100,
                       "write_size": 10, "read_size": 10, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_257B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_TRANSFER_257B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_transfer", "iterations": 10,
                       "write_size": 257, "read_size": 257, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})

@test_case(I2CComTest,
           name="I2C_COM_ASYNC_TRANSFER_1025B_MAX_FREQUENCY",
           title="test write read")
def I2C_COM_ASYNC_TRANSFER_1025B_MAX_FREQUENCY(self):
    self.test_execute({"frequency": self.caps["maximum_frequency"], "communication": "async_transfer", "iterations": 1,
                       "write_size": 1025, "read_size": 1025, "address": [self.address_range_7bit, self.address_range_10bit], "stop": [self.STOP_EACH_TRANSFER, self.STOP_EACH_ITERATION, self.STOP_AT_THE_END]})
