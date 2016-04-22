#! /usr/bin/env python2
"""
mbed SDK
Copyright (c) 2011-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


TEST BUILD & RUN
"""
import sys
import os
import json
from optparse import OptionParser

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.test_api import test_path_to_name, find_tests, print_tests

if __name__ == '__main__':
    try:
        # Parse Options
        parser = OptionParser()

        parser.add_option("-l", "--list", action="store_true", dest="list",
                          default=False, help="List (recursively) available tests in order and exit")

        parser.add_option("-p", "--paths", dest="paths",
                          default=None, help="Limit the tests to those within the specified comma separated list of paths")
        
        parser.add_option("-f", "--format", type="choice", dest="format",
                          choices=["list", "json"], default="list", help="List available tests in order and exit")

        (options, args) = parser.parse_args()

        # Print available tests in order and exit
        if options.list is True:
            tests = {}
            
            if options.paths:
                all_paths = options.paths.split(",")
                for path in all_paths:
                    tests.update(find_tests(path))
            else:
                tests = find_tests('.')
            
            print_tests(tests, options.format)
            sys.exit()

    except KeyboardInterrupt, e:
        print "\n[CTRL+c] exit"
    except Exception,e:
        import traceback
        traceback.print_exc(file=sys.stdout)
        print "[ERROR] %s" % str(e)
        sys.exit(1)
