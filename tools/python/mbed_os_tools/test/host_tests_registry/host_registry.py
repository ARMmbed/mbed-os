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

try:
    from imp import load_source
except ImportError:
    import importlib
    import sys

    def load_source(module_name, file_path):
        spec = importlib.util.spec_from_file_location(module_name, file_path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        sys.modules[module_name] = module
        return module


from inspect import getmembers, isclass
from os import listdir
from os.path import abspath, exists, isdir, isfile, join

from ..host_tests.base_host_test import BaseHostTest


class HostRegistry:
    """ Class stores registry with host tests and objects representing them
    """
    HOST_TESTS = {}  # Map between host_test_name -> host_test_object

    def register_host_test(self, ht_name, ht_object):
        """! Registers host test object by name

        @param ht_name Host test unique name
        @param ht_object Host test class object
        """
        if ht_name not in self.HOST_TESTS:
            self.HOST_TESTS[ht_name] = ht_object

    def unregister_host_test(self, ht_name):
        """! Unregisters host test object by name

        @param ht_name Host test unique name
        """
        if ht_name in self.HOST_TESTS:
            del self.HOST_TESTS[ht_name]

    def get_host_test(self, ht_name):
        """! Fetches host test object by name

        @param ht_name Host test unique name

        @return Host test callable object or None if object is not found
        """
        return self.HOST_TESTS[ht_name] if ht_name in self.HOST_TESTS else None

    def is_host_test(self, ht_name):
        """! Checks (by name) if host test object is registered already

        @param ht_name Host test unique name

        @return True if ht_name is registered (available), else False
        """
        return (ht_name in self.HOST_TESTS and
                self.HOST_TESTS[ht_name] is not None)

    def table(self, verbose=False):
        """! Prints list of registered host test classes (by name)
            @Detail For devel & debug purposes
        """
        from prettytable import PrettyTable, HEADER
        column_names = ['name', 'class', 'origin']
        pt = PrettyTable(column_names, junction_char="|", hrules=HEADER)
        for column in column_names:
            pt.align[column] = 'l'

        for name, host_test in sorted(self.HOST_TESTS.items()):
            cls_str = str(host_test.__class__)
            if host_test.script_location:
                src_path = host_test.script_location
            else:
                src_path = 'mbed-host-tests'
            pt.add_row([name, cls_str, src_path])
        return pt.get_string()

    def register_from_path(self, path, verbose=False):
        """ Enumerates and registers locally stored host tests
            Host test are derived from mbed_os_tools.test.BaseHostTest classes
        """
        if path:
            path = path.strip('"')
            if verbose:
                print("HOST: Inspecting '%s' for local host tests..." % path)
            if exists(path) and isdir(path):
                python_modules = [
                    f for f in listdir(path)
                    if isfile(join(path, f)) and f.endswith(".py")
                ]
                for module_file in python_modules:
                    self._add_module_to_registry(path, module_file, verbose)

    def _add_module_to_registry(self, path, module_file, verbose):
        module_name = module_file[:-3]
        try:
            mod = load_source(module_name, abspath(join(path, module_file)))
        except Exception as e:
            print(
                "HOST: Error! While loading local host test module '%s'"
                % join(path, module_file)
            )
            print("HOST: %s" % str(e))
            return
        if verbose:
            print("HOST: Loading module '%s': %s" % (module_file, str(mod)))

        for name, obj in getmembers(mod):
            if (
                isclass(obj) and
                issubclass(obj, BaseHostTest) and
                str(obj) != str(BaseHostTest)
            ):
                if obj.name:
                    host_test_name = obj.name
                else:
                    host_test_name = module_name
                host_test_cls = obj
                host_test_cls.script_location = join(path, module_file)
                if verbose:
                    print(
                        "HOST: Found host test implementation: %s -|> %s"
                        % (str(obj), str(BaseHostTest))
                    )
                    print(
                        "HOST: Registering '%s' as '%s'"
                        % (str(host_test_cls), host_test_name)
                    )
                self.register_host_test(
                    host_test_name, host_test_cls()
                )
