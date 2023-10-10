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

"""! @package mbed-host-test-plugins

This package contains plugins used by host test to reset, flash devices etc.
This package can be extended with new packages to add more generic functionality

"""

from mbed_os_tools.test.host_tests_plugins import (
    HOST_TEST_PLUGIN_REGISTRY,
    call_plugin,
    get_plugin_caps,
    get_plugin_info,
    print_plugin_info,
)
