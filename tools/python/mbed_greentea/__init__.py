"""
mbed SDK
Copyright (c) 2011-2015 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.Wirkus@arm.com>
"""

from .mbed_greentea_cli import main

"""! @package mbed-greentea

This is a test suite used by mbed project. If you have yotta package with tests you can run them on supported hardware
This test suite supports:
* mbed-ls           - mbed-enabled device auto detection module
* mbed-host-test    - mbed-enabled device test framework (flash, reset and make host tests)

"""
