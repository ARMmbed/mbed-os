# Copyright 2015 ARM Limited, All rights reserved
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

import unittest

from mbed_host_tests.host_tests.base_host_test import BaseHostTest, event_callback


class TestEvenCallbackDecorator(unittest.TestCase):
    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_event_callback_decorator(self):
        class Ht(BaseHostTest):

            @event_callback('Hi')
            def hi(self, key, value, timestamp):
                print('hi')

            @event_callback('Hello')
            def hello(self, key, value, timestamp):
                print('hello')
        h = Ht()
        h.setup()
        callbacks = h.get_callbacks()
        self.assertIn('Hi', callbacks)
        self.assertIn('Hello', callbacks)
