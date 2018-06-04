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
from os.path import join, exists, basename, relpath, normpath, split
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
        # note that SEM_FAILCRITICALERRORS = 1
        oldError = ctypes.windll.kernel32.SetErrorMode(1)

    mbeds = mbed_lstools.create()
    detect_muts_list = mbeds.list_mbeds()

    if os.name == 'nt':
        ctypes.windll.kernel32.SetErrorMode(oldError)

    return get_autodetected_MUTS(detect_muts_list,
                                 name_filter=platform_name_filter)


def get_autodetected_MUTS(mbeds_list, name_filter=None):
    """ Convert devices to artificial MUTS file in muts_all.json format
    @param mbeds_list list of mbeds captured from mbed_lstools
    @param name_filet Filter 'platform_name' to include only the names specified
    """
    result = {}
    index = 1
    for index, mut in enumerate(mbeds_list):
        if platform_name_filter and mut['platform_name'] not in name_filter:
            continue

        m = {
            'mcu': mut['platform_name'],
            'port': mut['serial_port'],
            'disk': mut['mount_point'],
            'peripherals': []
        }

        if 'platform_name_unique' in mut:
            m['mcu_unique'] = mut['platform_name_unique']
        else:
            m['mcu_unique'] = "%s[%s]" % (mut['platform_name'],
                                          mut['target_id'][-4:])

        result[index + 1] = m
    return result


def test_path_to_name(path, base):
    """ Create a unique cross-platform test name based on the path """
    name_parts = []
    head, tail = split(relpath(path, base))
    while tail and tail != ".":
        name_parts.insert(0, tail)
        head, tail = split(head)
    return "-".join(name_parts).lower()


def get_test_config(config_name, target_name):
    """Find the path to a test configuration file

    config_name: path to a custom configuration file or mbed OS interface
                 "ethernet, wifi_odin, etc"
    target_name: name of target to determing if mbed OS interface given is valid
    returns path to config or None if unable to find a valid config
    """
    if exists(config_name):
        return config_name
    else:
        return get_config_path(config_name, target_name)


def find_tests(base_dir, target_name, toolchain_name, app_config=None):
    """ Finds all tests in a directory recursively
    base_dir: path to the directory to scan for tests
    target_name: name of the target to use for scanning
    toolchain_name: name of the toolchain to use for scanning
    app_config - location of a chosen mbed_app.json file

    returns a dictionary where keys are the test name, and the values are
    lists of paths needed to biuld the test.
    """

    # Temporary structure: tests referenced by (name, base, group, case) tuple
    tests = {}
    # List of common folders: (predicate function, path) tuple
    commons = []

    toolchain = prepare_toolchain([base_dir], None, target_name, toolchain_name,
                                  app_config=app_config)

    base_resources = scan_resources([base_dir], toolchain)
    for directory in base_resources.inc_dirs:
        walk_base_dir = join(directory, 'TESTS')
        # If there is a subdirectory called 'TESTS', scan it for tests
        if exists(walk_base_dir):
            test_resources = toolchain.scan_resources(
                walk_base_dir, base_path=base_dir)
            for d in test_resources.inc_dirs:

                # If the test case folder is not called 'host_tests' or 'COMMON'
                # and it is located two folders down from the main 'TESTS'
                # folder (ex. TESTS/testgroup/testcase), then add it to the tests
                relative_path = relpath(d, walk_base_dir)
                relative_path_parts = normpath(relative_path).split(os.sep)
                if len(relative_path_parts) == 2:
                    test_group_directory_path, test_case_directory = split(d)
                    test_group_directory = basename(test_group_directory_path)

                    # Check to make sure discoverd folder is not in a host test
                    # directory or common directory
                    special_dirs = ['host_tests', 'COMMON']
                    if (test_group_directory not in special_dirs and
                        test_case_directory not in special_dirs):
                        test_name = test_path_to_name(d, base_dir)
                        test_index = (test_name, walk_base_dir,
                                      test_group_directory, test_case_directory)
                        tests[test_index] = [d]

                # Also find any COMMON paths, we'll add these later once we find
                # all the base tests
                if 'COMMON' in relative_path_parts:
                    if relative_path_parts[0] != 'COMMON':
                        def predicate(base_pred, group_pred,
                                      name_base_group_case):
                            (name, base, group, case) = name_base_group_case
                            return base == base_pred and group == group_pred
                        commons.append((functools.partial(
                            predicate, walk_base_dir, relative_path_parts[0]), d))
                    else:
                        def predicate(base_pred, name_base_group_case):
                            (name, base, group, case) = name_base_group_case
                            return base == base_pred
                        commons.append((functools.partial(
                            predicate, walk_base_dir), d))

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
    path = normpath(path)
    path = relpath(path, start)
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

        test_build_path = join(build_path, test_paths[0])
        src_paths = base_source_paths + test_paths
        bin_file = None
        test_case_folder_name = basename(test_paths[0])

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

                        if report:
                            report_arg = worker_result['kwargs']['report']
                            report_entry = report_arg[target_name][toolchain_name]
                            this_build = report[target_name][toolchain_name]
                            this_build.update(report_entry)

                        # Set the overall result to a failure on build failure
                        if ('reason' in worker_result and
                            not worker_result['reason'] and
                            not isinstance(worker_result['reason'], NotSupportedException)):
                            result = False
                            break


                        # Adding binary path to test build result
                        if ('result' in worker_result and
                            worker_result['result'] and
                            'bin_file' in worker_result):
                            bin_file = norm_relative_path(
                                worker_result['bin_file'], execution_directory)
                            project_id = worker_result['kwargs']['project_id']
                            test_build['tests'][project_id] = {
                                "binaries": [
                                    {
                                        "path": bin_file
                                    }
                                ]
                            }

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
