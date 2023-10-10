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

"""! @package mbed-host-test-plugins

This package contains plugins used by host test to reset, flash devices etc.
This package can be extended with new packages to add more generic functionality

"""

from . import host_test_registry

# This plugins provide 'flashing' and 'reset' methods to host test scripts
from . import module_copy_shell
from . import module_copy_mbed
from . import module_reset_mbed
from . import module_power_cycle_mbed
from . import module_copy_pyocd
from . import module_reset_pyocd

# Additional, non standard platforms
from . import module_copy_silabs
from . import module_reset_silabs
from . import module_copy_stlink
from . import module_reset_stlink
from . import module_copy_ublox
from . import module_reset_ublox
from . import module_reset_mps2
from . import module_copy_mps2
#import module_copy_jn51xx
#import module_reset_jn51xx


# Plugin registry instance
HOST_TEST_PLUGIN_REGISTRY = host_test_registry.HostTestRegistry()

# Static plugin registration
# Some plugins are commented out if they are not stable or not commonly used
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_mbed.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_shell.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_reset_mbed.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_pyocd.load_plugin())

# Extra platforms support
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_mps2.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_reset_mps2.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_silabs.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_reset_silabs.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_stlink.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_reset_stlink.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_power_cycle_mbed.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_reset_pyocd.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_reset_ublox.load_plugin())
HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_ublox.load_plugin())
#HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_copy_jn51xx.load_plugin())
#HOST_TEST_PLUGIN_REGISTRY.register_plugin(module_reset_jn51xx.load_plugin())

# TODO: extend plugin loading to files with name module_*.py loaded ad-hoc

###############################################################################
# Functional interface for host test plugin registry
###############################################################################
def call_plugin(type, capability, *args, **kwargs):
    """! Interface to call plugin registry functional way
    @param capability Plugin capability we want to call
    @param args Additional parameters passed to plugin
    @param kwargs Additional parameters passed to plugin
    @return Returns return value from call_plugin call
    """
    return HOST_TEST_PLUGIN_REGISTRY.call_plugin(type, capability, *args, **kwargs)

def get_plugin_caps(type):
    """! Get list of all capabilities for plugin family with the same type
    @param type Type of a plugin
    @return Returns list of all capabilities for plugin family with the same type. If there are no capabilities empty list is returned
    """
    return HOST_TEST_PLUGIN_REGISTRY.get_plugin_caps(type)

def get_plugin_info():
    """! Return plugins information
    @return Dictionary HOST_TEST_PLUGIN_REGISTRY
    """
    return HOST_TEST_PLUGIN_REGISTRY.get_dict()

def print_plugin_info():
    """! Prints plugins' information in user friendly way
    """
    print(HOST_TEST_PLUGIN_REGISTRY)
