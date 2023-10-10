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

class MockMbedDevice(object):
    
    KV_REGEX = re.compile("\{\{([\w\d_-]+);([^\}]+)\}\}")

    def __init__(self, serial):
        self._synced = False
        self._kvs = []
        self._serial = serial
        self._serial.on_upstream_write(self.on_write)

    def handle_kv(self, key, value):
        if not self._synced:
            if key == "__sync":
                self._synced = True
                self.send_kv(key, value)
                self.on_sync()
        else:
            pass

    def send_kv(self, key, value):
        self._serial.downstream_write("{{{{{};{}}}}}\r\n".format(key, value))

    def on_write(self, data):
        kvs = self.KV_REGEX.findall(data.decode("utf-8"))

        for key, value in kvs:
            self.handle_kv(key, value)
            self._kvs.append((key, value))

    def on_sync(self):
        self._serial.downstream_write(
            "{{__timeout;15}}\r\n"
            "{{__host_test_name;default_auto}}\r\n"
            "{{end;success}}\n"
            "{{__exit;0}}\r\n"
        )
