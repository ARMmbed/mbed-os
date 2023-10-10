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


import uuid
from .. import BaseHostTest

class EchoTest(BaseHostTest):

    __result = None
    echo_count = 0
    count = 0
    uuid_sent = []
    uuid_recv = []

    def __send_echo_uuid(self):
        if self.echo_count:
            str_uuid = str(uuid.uuid4())
            self.send_kv("echo", str_uuid)
            self.uuid_sent.append(str_uuid)
            self.echo_count -= 1

    def _callback_echo(self, key, value, timestamp):
        self.uuid_recv.append(value)
        self.__send_echo_uuid()

    def _callback_echo_count(self, key, value, timestamp):
        # Handshake
        self.echo_count = int(value)
        self.send_kv(key, value)
        # Send first echo to echo server on DUT
        self.__send_echo_uuid()

    def setup(self):
        self.register_callback("echo", self._callback_echo)
        self.register_callback("echo_count", self._callback_echo_count)

    def result(self):
        self.__result = self.uuid_sent == self.uuid_recv
        return self.__result

    def teardown(self):
        pass
