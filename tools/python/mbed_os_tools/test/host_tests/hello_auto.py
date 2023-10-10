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

from .. import BaseHostTest

class HelloTest(BaseHostTest):
    HELLO_WORLD = "Hello World"

    __result = None

    def _callback_hello_world(self, key, value, timestamp):
        self.__result = value == self.HELLO_WORLD
        self.notify_complete()

    def setup(self):
        self.register_callback("hello_world", self._callback_hello_world)

    def result(self):
        return self.__result

    def teardown(self):
        pass
