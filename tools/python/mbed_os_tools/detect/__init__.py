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

"""! @package mbed-ls

mbed-ls package is a set of tools inherited from mbed-lmtools used to detect
mbed enabled devices on host (Windows, Linux and MacOS).

mbed-lstools is a Python module that detects and lists mbed-enabled devices
connected to the host computer. It will be delivered as a redistributable
Python module (package) and command line tool.

Currently supported operating system:
* Windows 7.
* Ubuntu.
* Mac OS X (Darwin).

The stand-alone mbed-lstools Python package is still under development,
but it's already delivered as part of the mbed SDK's test suite and a command
line tool (see below).
"""

from .main import create

create = create
