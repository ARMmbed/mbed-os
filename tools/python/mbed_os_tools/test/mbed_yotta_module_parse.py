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

import os
import json


class YottaConfig(object):

    yotta_config = None

    def __init__(self):
        self.BUILD_DIR = 'build'
        self.YOTTA_CONFIG_NAME = 'yotta_config.json'
        self.DEFAULT_BAUDRATE = 115200

    def init(self, target_name):
        """! Loads yotta_config.json as an object from local yotta build directory
        @return True if data was successfuly loaded from the file
        """
        try:
            path = os.path.join(self.BUILD_DIR, target_name, self.YOTTA_CONFIG_NAME)
            with open(path, 'r') as data_file:
                self.yotta_config = json.load(data_file)
        except IOError as e:
            self.yotta_config = {}
        return bool(len(self.yotta_config))

    def set_yotta_config(self, yotta_config):
        self.yotta_config = yotta_config

    def get_baudrate(self):
        """! Returns default baudrate for stdio serial
        @return Configuration baudrate of default on (115200)
        Example yotta_config.json
        {
          "minar": {
            "initial_event_pool_size": 50,
            "additional_event_pools_size": 100
          },
          "mbed-os": {
            "net": {
              "stacks": {
                "lwip": true
              }
            },
            "stdio": {
              "default-baud": 9600
            }
          },
        """
        # Get default baudrate for this target
        if self.yotta_config and 'mbed-os' in self.yotta_config:
            if 'stdio' in self.yotta_config['mbed-os']:
                if 'default-baud' in self.yotta_config['mbed-os']['stdio']:
                    return int(self.yotta_config['mbed-os']['stdio']['default-baud'])
        return self.DEFAULT_BAUDRATE

    def get_test_pins(self):
        if self.yotta_config and 'hardware' in self.yotta_config:
            if 'test-pins' in self.yotta_config['hardware']:
                return self.yotta_config['hardware']['test-pins']
        return None


class YottaModule(object):

    __yotta_module = None
    __greentea_client = 'greentea-client'

    def __init__(self):
        self.MODULE_PATH = '.'
        self.YOTTA_CONFIG_NAME = 'module.json'

    def init(self):
        """! Loads yotta_module.json as an object from local yotta build directory
        @return True if data was successfuly loaded from the file
        """
        self.__yotta_module = dict()

        try:
            path = os.path.join(self.MODULE_PATH, self.YOTTA_CONFIG_NAME)
            if os.path.exists(path):
                # Load module.json only if it exists
                with open(path, 'r') as data_file:
                    self.__yotta_module = json.load(data_file)
        except IOError as e:
            print("YottaModule: error - %s" % str(e))
        return bool(self.__yotta_module)    # bool({}) == False

    def set_yotta_module(self, yotta_module):
        self.__yotta_module = yotta_module

    def get_data(self):
        return self.__yotta_module

    def get_name(self):
        return self.__yotta_module.get('name', 'unknown')

    def check_greentea_client(self):
        if self.get_name() == self.__greentea_client:
            return True

        dependencies = self.__yotta_module.get('dependencies', False)
        testDependencies = self.__yotta_module.get('testDependencies', False)
        if dependencies:
            if dependencies.get(self.__greentea_client, False):
                return True
        if testDependencies:
            if testDependencies.get(self.__greentea_client, False):
                return True
        return False
