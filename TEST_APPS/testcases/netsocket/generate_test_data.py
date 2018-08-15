"""
Copyright 2018 ARM Limited
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
import random, string

'''
This script is intended to be a common test data generator.
Currently it implements randomUppercaseAsciiString, randomLowercaseAsciiString methods.
'''

def randomUppercaseAsciiString(length):
    return ''.join(random.choice(string.ascii_uppercase) for i in range(length))


def randomLowercaseAsciiString(length):
    return ''.join(random.choice(string.ascii_lowercase) for i in range(length))
