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

from builtins import super

class MockSerial(object):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._args = args
        self._kwargs = kwargs
        self._open = True
        self._rx_counter = 0
        self._tx_buffer = b""
        self._rx_buffer = b""
        self._upstream_write_cb = None

    def read(self, count):
        contents = self._rx_buffer[self._rx_counter:count]
        self._rx_counter += len(contents)
        return contents

    def write(self, data):
        self._tx_buffer += data
        if self._upstream_write_cb:
            self._upstream_write_cb(data)

    def close(self):
        self._open = False

    def downstream_write(self, data):
        self._rx_buffer += data.encode("utf-8")

    def downstream_write_bytes(self, data):
        self._rx_buffer += data

    def on_upstream_write(self, func):
        self._upstream_write_cb = func
