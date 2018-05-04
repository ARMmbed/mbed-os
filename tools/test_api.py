"""
mbed SDK
Copyright (c) 2011-2014 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: Przemyslaw Wirkus <Przemyslaw.wirkus@arm.com>
"""
from __future__ import print_function
import six

import os
import sys
import json
import ctypes
import functools
from copy import copy, deepcopy
from time import sleep
from os.path import join, exists, basename, relpath
from multiprocessing import Pool, cpu_count

import mbed_lstools

# Imports related to mbed build api
from tools.tests import TESTS
from tools.utils import ToolException
from tools.utils import NotSupportedException
from tools.targets import Target
from tools.test_configs import get_config_path
from tools.build_api import build_project, build_mbed_libs, build_lib
from tools.build_api import prepare_toolchain
from tools.build_api import scan_resources
from tools.build_api import get_config
from tools.toolchains import TOOLCHAIN_PATHS
from tools.notifier.mock import MockNotifier


def get_module_avail(module_name):
    """ This function returns True if module_name is already imported module
    """
    return module_name in sys.modules.keys()

def get_autodetected_MUTS_list(platform_name_filter=None):
    oldError = None
    if os.name == 'nt':
        # Disable Windows error box temporarily
        oldError = ctypes.windll.kernel32.SetErrorMode(1) #note that SEM_FAILCRITICALERRORS = 1

    mbeds = mbed_lstools.create()
    detect_muts_list = mbeds.list_mbeds()

    if os.name == 'nt':
        ctypes.windll.kernel32.SetErrorMode(oldError)

    return get_autodetected_MUTS(detect_muts_list, platform_name_filter=platform_name_filter)

def get_autodetected_MUTS(mbeds_list, platform_name_filter=None):
    """ Function detects all connected to host mbed-enabled devices and generates artificial MUTS file.
        If function fails to auto-detect devices it will return empty dictionary.

        if get_module_avail('mbed_lstools'):
            mbeds = mbed_lstools.create()
            mbeds_list = mbeds.list_mbeds()

        @param mbeds_list list of mbeds captured from mbed_lstools
        @param platform_name You can filter 'platform_name' with list of filtered targets from 'platform_name_filter'
    """
    result = {}   # Should be in muts_all.json format
    # Align mbeds_list from mbed_lstools to MUT file format (JSON dictionary with muts)
    # mbeds_list = [{'platform_name': 'NUCLEO_F302R8', 'mount_point': 'E:', 'target_id': '07050200623B61125D5EF72A', 'serial_port': u'COM34'}]
    index = 1
    for mut in mbeds_list:
        # Filter the MUTS if a filter is specified

        if platform_name_filter and not mut['platform_name'] in platform_name_filter:
            continue

        # For mcu_unique - we are assigning 'platform_name_unique' value from  mbedls output (if its existing)
        # if not we  are creating our own unique value (last few chars from platform's target_id).
        m = {'mcu': mut['platform_name'],
             'mcu_unique' : mut['platform_name_unique'] if 'platform_name_unique' in mut else "%s[%s]" % (mut['platform_name'], mut['target_id'][-4:]),
             'port': mut['serial_port'],
             'disk': mut['mount_point'],
             'peripherals': []     # No peripheral detection
             }
        if index not in result:
            result[index] = {}
        result[index] = m
        index += 1
    return result

def test_path_to_name(path, base):
    """Change all slashes in a path into hyphens
    This creates a unique cross-platform test name based on the path
    This can eventually be overriden by a to-be-determined meta-data mechanism"""
    name_parts = []
    head, tail = os.path.split(relpath(path,base))
    while (tail and tail != "."):
        name_parts.insert(0, tail)
        head, tail = os.path.split(head)

    return "-".join(name_parts).lower()

def get_test_config(config_name, target_name):
    """Finds the path to a test configuration file
    config_name: path to a custom configuration file OR mbed OS interface "ethernet, wifi_odin, etc"
    target_name: name of target to determing if mbed OS interface given is valid
    returns path to config, will return None if no valid config is found
    """
    # If they passed in a full path
    if exists(config_name):
        # This is a module config
        return config_name
    # Otherwise find the path to configuration file based on mbed OS interface
    return get_config_path(config_name, target_name)

def find_tests(base_dir, target_name, toolchain_name, app_config=None):
    """ Finds all tests in a directory recursively
    base_dir: path to the directory to scan for tests (ex. 'path/to/project')
    target_name: name of the target to use for scanning (ex. 'K64F')
    toolchain_name: name of the toolchain to use for scanning (ex. 'GCC_ARM')
    options: Compile options to pass to the toolchain (ex. ['debug-info'])
    app_config - location of a chosen mbed_app.json file

    returns a dictionary where keys are the test name, and the values are
    lists of paths needed to biuld the test.
    """

    # Temporary structure: tests referenced by (name, base, group, case) tuple
    tests = {}
    # List of common folders: (predicate function, path) tuple
    commons = []

    # Prepare the toolchain
    toolchain = prepare_toolchain([base_dir], None, target_name, toolchain_name,
                                  app_config=app_config)

    # Scan the directory for paths to probe for 'TESTS' folders
    base_resources = scan_resources([base_dir], toolchain)

    dirs = base_resources.inc_dirs
    for directory in dirs:
        subdirs = os.listdir(directory)

        # If the directory contains a subdirectory called 'TESTS', scan it for test cases
        if 'TESTS' in subdirs:
            walk_base_dir = join(directory, 'TESTS')
            test_resources = toolchain.scan_resources(walk_base_dir, base_path=base_dir)

            # Loop through all subdirectories
            for d in test_resources.inc_dirs:

                # If the test case folder is not called 'host_tests' or 'COMMON' and it is
                # located two folders down from the main 'TESTS' folder (ex. TESTS/testgroup/testcase)
                # then add it to the tests
                relative_path = relpath(d, walk_base_dir)
                relative_path_parts = os.path.normpath(relative_path).split(os.sep)
                if len(relative_path_parts) == 2:
                    test_group_directory_path, test_case_directory = os.path.split(d)
                    test_group_directory = os.path.basename(test_group_directory_path)

                    # Check to make sure discoverd folder is not in a host test directory or common directory
                    special_dirs = ['host_tests', 'COMMON']
                    if test_group_directory not in special_dirs and test_case_directory not in special_dirs:
                        test_name = test_path_to_name(d, base_dir)
                        tests[(test_name, walk_base_dir, test_group_directory, test_case_directory)] = [d]

                # Also find any COMMON paths, we'll add these later once we find all the base tests
                if 'COMMON' in relative_path_parts:
                    if relative_path_parts[0] != 'COMMON':
                        def predicate(base_pred, group_pred, name_base_group_case):
                            (name, base, group, case) = name_base_group_case
                            return base == base_pred and group == group_pred
                        commons.append((functools.partial(predicate, walk_base_dir, relative_path_parts[0]), d))
                    else:
                        def predicate(base_pred, name_base_group_case):
                            (name, base, group, case) = name_base_group_case
                            return base == base_pred
                        commons.append((functools.partial(predicate, walk_base_dir), d))

    # Apply common directories
    for pred, path in commons:
        for test_identity, test_paths in six.iteritems(tests):
            if pred(test_identity):
                test_paths.append(path)

    # Drop identity besides name
    return {name: paths for (name, _, _, _), paths in six.iteritems(tests)}

def print_tests(tests, format="list", sort=True):
    """Given a dictionary of tests (as returned from "find_tests"), print them
    in the specified format"""
    if format == "list":
        for test_name in sorted(tests.keys()):
            test_path = tests[test_name][0]
            print("Test Case:")
            print("    Name: %s" % test_name)
            print("    Path: %s" % test_path)
    elif format == "json":
        print(json.dumps({test_name: test_path[0] for test_name, test_paths
                          in tests}, indent=2))
    else:
        print("Unknown format '%s'" % format)
        sys.exit(1)

def norm_relative_path(path, start):
    """This function will create a normalized, relative path. It mimics the
    python os.path.relpath function, but also normalizes a Windows-syle path
    that use backslashes to a Unix style path that uses forward slashes."""
    path = os.path.normpath(path)
    path = os.path.relpath(path, start)
    path = path.replace("\\", "/")
    return path


def build_test_worker(*args, **kwargs):
    """This is a worker function for the parallel building of tests. The `args`
    and `kwargs` are passed directly to `build_project`. It returns a dictionary
    with the following structure:

    {
        'result': `True` if no exceptions were thrown, `False` otherwise
        'reason': Instance of exception that was thrown on failure
        'bin_file': Path to the created binary if `build_project` was
                    successful. Not present otherwise
        'kwargs': The keyword arguments that were passed to `build_project`.
                  This includes arguments that were modified (ex. report)
    }
    """
    bin_file = None
    ret = {
        'result': False,
        'args': args,
        'kwargs': kwargs
    }

    # Use parent TOOLCHAIN_PATHS variable
    for key, value in kwargs['toolchain_paths'].items():
        TOOLCHAIN_PATHS[key] = value

    del kwargs['toolchain_paths']

    try:
        bin_file = build_project(*args, **kwargs)
        ret['result'] = True
        ret['bin_file'] = bin_file
        ret['kwargs'] = kwargs

    except NotSupportedException as e:
        ret['reason'] = e
    except ToolException as e:
        ret['reason'] = e
    except KeyboardInterrupt as e:
        ret['reason'] = e
    except:
        # Print unhandled exceptions here
        import traceback
        traceback.print_exc(file=sys.stdout)

    return ret


def build_tests(tests, base_source_paths, build_path, target, toolchain_name,
                clean=False, notify=None, jobs=1, macros=None,
                silent=False, report=None, properties=None,
                continue_on_build_fail=False, app_config=None,
                build_profile=None, stats_depth=None, ignore=None):
    """Given the data structure from 'find_tests' and the typical build parameters,
    build all the tests

    Returns a tuple of the build result (True or False) followed by the test
    build data structure"""

    execution_directory = "."
    base_path = norm_relative_path(build_path, execution_directory)

    target_name = target.name if isinstance(target, Target) else target
    cfg, _, _ = get_config(base_source_paths, target_name, toolchain_name, app_config=app_config)

    baud_rate = 9600
    if 'platform.stdio-baud-rate' in cfg:
        baud_rate = cfg['platform.stdio-baud-rate'].value

    test_build = {
        "platform": target_name,
        "toolchain": toolchain_name,
        "base_path": base_path,
        "baud_rate": baud_rate,
        "binary_type": "bootable",
        "tests": {}
    }

    result = True

    jobs_count = int(jobs if jobs else cpu_count())
    p = Pool(processes=jobs_count)
    results = []
    for test_name, test_paths in tests.items():
        if not isinstance(test_paths, list):
            test_paths = [test_paths]

        test_build_path = os.path.join(build_path, test_paths[0])
        src_paths = base_source_paths + test_paths
        bin_file = None
        test_case_folder_name = os.path.basename(test_paths[0])

        args = (src_paths, test_build_path, target, toolchain_name)
        kwargs = {
            'jobs': 1,
            'clean': clean,
            'macros': macros,
            'name': test_case_folder_name,
            'project_id': test_name,
            'report': report,
            'properties': properties,
            'app_config': app_config,
            'build_profile': build_profile,
            'toolchain_paths': TOOLCHAIN_PATHS,
            'stats_depth': stats_depth,
            'notify': MockNotifier()
        }

        results.append(p.apply_async(build_test_worker, args, kwargs))

    p.close()
    result = True
    itr = 0
    while len(results):
        itr += 1
        if itr > 360000:
            p.terminate()
            p.join()
            raise ToolException("Compile did not finish in 10 minutes")
        else:
            sleep(0.01)
            pending = 0
            for r in results:
                if r.ready() is True:
                    try:
                        worker_result = r.get()
                        results.remove(r)

                        # Push all deferred notifications out to the actual notifier
                        new_notify = deepcopy(notify)
                        for message in worker_result['kwargs']['notify'].messages:
                            new_notify.notify(message)

                        # Take report from the kwargs and merge it into existing report
                        if report:
                            report_entry = worker_result['kwargs']['report'][target_name][toolchain_name]
                            report_entry[worker_result['kwargs']['project_id'].upper()][0][0]['output'] = new_notify.get_output()
                            for test_key in report_entry.keys():
                                report[target_name][toolchain_name][test_key] = report_entry[test_key]

                        # Set the overall result to a failure if a build failure occurred
                        if ('reason' in worker_result and
                            not worker_result['reason'] and
                            not isinstance(worker_result['reason'], NotSupportedException)):
                            result = False
                            break


                        # Adding binary path to test build result
                        if ('result' in worker_result and
                            worker_result['result'] and
                            'bin_file' in worker_result):
                            bin_file = norm_relative_path(worker_result['bin_file'], execution_directory)

                            test_build['tests'][worker_result['kwargs']['project_id']] = {
                                "binaries": [
                                    {
                                        "path": bin_file
                                    }
                                ]
                            }

                            test_key = worker_result['kwargs']['project_id'].upper()
                            print('Image: %s\n' % bin_file)

                    except:
                        if p._taskqueue.queue:
                            p._taskqueue.queue.clear()
                            sleep(0.5)
                        p.terminate()
                        p.join()
                        raise
                else:
                    pending += 1
                    if pending >= jobs_count:
                        break

            # Break as soon as possible if there is a failure and we are not
            # continuing on build failures
            if not result and not continue_on_build_fail:
                if p._taskqueue.queue:
                    p._taskqueue.queue.clear()
                    sleep(0.5)
                p.terminate()
                break

    p.join()

    test_builds = {}
    test_builds["%s-%s" % (target_name, toolchain_name)] = test_build

    return result, test_builds


def test_spec_from_test_builds(test_builds):
    return {
        "builds": test_builds
    }
