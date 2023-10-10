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

import os
import re
import json

from .mbed_common_api import run_cli_command
from .mbed_greentea_log import gt_logger
from .mbed_yotta_module_parse import YottaModule, YottaConfig
from .mbed_target_info import (
    get_mbed_target_from_current_dir, get_binary_type_for_platform
)
from .cmake_handlers import load_ctest_testsuite
from .tests_spec import TestSpec, TestBuild, Test, TestBinary



class YottaError(Exception):
    """
    Exception raised by this module when it fails to gather test information.
    """
    pass


def build_with_yotta(yotta_target_name, verbose=False, build_to_release=False, build_to_debug=False):
    cmd = ["yotta"] # "yotta %s --target=%s,* build"
    if verbose:
        cmd.append("-v")
    cmd.append("--target=%s,*"% yotta_target_name)
    cmd.append("build")
    if build_to_release:
        cmd.append("-r")
    elif build_to_debug:
        cmd.append("-d")

    gt_logger.gt_log("building your sources and tests with yotta...")
    gt_logger.gt_log_tab("calling yotta: %s"% (" ".join(cmd)))
    yotta_result, yotta_ret = run_cli_command(cmd, shell=False, verbose=verbose)
    if yotta_result:
        gt_logger.gt_log("yotta build for target '%s' was successful"% gt_logger.gt_bright(yotta_target_name))
    else:
        gt_logger.gt_log_err("yotta build failed!")
    return yotta_result, yotta_ret


def get_platform_name_from_yotta_target(target):
    """
    Parses target string and gives platform name and toolchain

    :param target:
    :return:
    """
    target_json_path = os.path.join('yotta_targets', target, 'target.json')

    if not os.path.exists(target_json_path):
        gt_logger.gt_log_err('Target json does not exist [%s].\n' % target_json_path +
                             'mbed TAS Executor {greentea} must be run inside a pre built yotta module!')
        return None

    with open(target_json_path, 'r') as f:
        data = f.read()
        try:
            target_json = json.loads(data)
        except (TypeError, ValueError) as e:
            gt_logger.gt_log_err('Failed to load json data from target.json! error [%s]\n' % str(e) +
                                 'Can not determine required mbed platform name!')
            return None

    if 'keywords' not in target_json:
        gt_logger.gt_log_err("No 'keywords' in target.json! Can not determine required mbed platform name!")
        return None

    platform_name = None
    for keyword in target_json['keywords']:
        m = re.search('mbed-target:(.*)', keyword)
        if m is not None:
            platform_name = m.group(1).upper()

    if platform_name is None:
        gt_logger.gt_log_err('No keyword with format "mbed-target:<platform name>" found in target.json!\n' +
                             'Can not determine required mbed platform name!')
        return None
    return platform_name


def get_test_spec_from_yt_module(opts):
    """
    Gives test specification created from yotta module environment.

    :return TestSpec:
    """
    ### Read yotta module basic information
    yotta_module = YottaModule()
    yotta_module.init() # Read actual yotta module data

    # Check if NO greentea-client is in module.json of repo to test, if so abort
    if not yotta_module.check_greentea_client():
        error = """
        *****************************************************************************************
        * We've noticed that NO 'greentea-client' module is specified in                        *
        * dependency/testDependency section of this module's 'module.json' file.                *
        *                                                                                       *
        * This version of Greentea requires 'greentea-client' module.                           *
        * Please downgrade to Greentea before v0.2.0:                                           *
        *                                                                                       *
        * $ pip install "mbed-greentea<0.2.0" --upgrade                                         *
        *                                                                                       *
        * or port your tests to new Async model: https://github.com/ARMmbed/greentea/pull/78    *
        *****************************************************************************************
        """
        raise YottaError(error)

    test_spec = TestSpec()

    ### Selecting yotta targets to process
    yt_targets = [] # List of yotta targets specified by user used to process during this run
    if opts.list_of_targets:
        yt_targets = opts.list_of_targets.split(',')
    else:
        # Trying to use locally set yotta target
        gt_logger.gt_log("checking for yotta target in current directory")
        gt_logger.gt_log_tab("reason: no --target switch set")
        current_target = get_mbed_target_from_current_dir()
        if current_target:
            gt_logger.gt_log("assuming default target as '%s'"% gt_logger.gt_bright(current_target))
            # Assuming first target printed by 'yotta search' will be used
            yt_targets = [current_target]
        else:
            gt_logger.gt_log_tab("yotta target in current directory is not set")
            gt_logger.gt_log_err("yotta target is not specified. Use '%s' or '%s' command to set target"%
            (
                gt_logger.gt_bright('mbedgt -t <yotta_target>'),
                gt_logger.gt_bright('yotta target <yotta_target>')
            ))
            raise YottaError("Yotta target not set in current directory!")

    ### Use yotta to search mapping between platform names and available platforms
    # Convert platform:target, ... mapping to data structure
    yt_target_to_map_platform = {}
    if opts.map_platform_to_yt_target:
        gt_logger.gt_log("user defined platform -> target supported mapping definition (specified with --map-target switch)")
        for mapping in opts.map_platform_to_yt_target.split(','):
            if len(mapping.split(':')) == 2:
                yt_target, platform = mapping.split(':')
                yt_target_to_map_platform[yt_target] = platform
                gt_logger.gt_log_tab("mapped yotta target '%s' to be compatible with platform '%s'"% (
                    gt_logger.gt_bright(yt_target),
                    gt_logger.gt_bright(platform)
                ))
            else:
                gt_logger.gt_log_tab("unknown format '%s', use 'target:platform' format"% mapping)

    for yt_target in yt_targets:
        if yt_target in yt_target_to_map_platform:
            platform = yt_target_to_map_platform[yt_target]
        else:
            # get it from local Yotta target
            platform = get_platform_name_from_yotta_target(yt_target)

        # Toolchain doesn't matter as Greentea does not have to do any selection for it unlike platform
        toolchain = yt_target
        yotta_config = YottaConfig()
        yotta_config.init(yt_target)
        baud_rate = yotta_config.get_baudrate()
        base_path = os.path.join('.', 'build', yt_target)
        tb = TestBuild(yt_target, platform, toolchain, baud_rate, base_path)
        test_spec.add_test_builds(yt_target, tb)

        # Find tests
        ctest_test_list = load_ctest_testsuite(base_path,
                                               binary_type=get_binary_type_for_platform(platform))
        for name, path in ctest_test_list.items():
            t = Test(name)
            t.add_binary(path, TestBinary.BIN_TYPE_BOOTABLE)
            tb.add_test(name, t)

    return test_spec
