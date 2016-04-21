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

def test_path_to_name(path):
    # Change all slashes in a path into hyphens
    # This creates a unique cross-platform test name based on the path
    # This can eventually be overriden by a to-be-determined meta-data mechanism
    name_parts = []
    head, tail = os.path.split(path)
    while (tail != "" and tail != "."):
        name_parts.insert(0, tail)
        head, tail = os.path.split(head)
    
    return "-".join(name_parts)

def find_tests_in_tests_directory(directory):
    tests = {}
    
    for d in os.listdir(directory):
        # dir name host_tests is reserved for host python scripts.
        if d != "host_tests":
            # Loop on test case directories
            for td in os.listdir(os.path.join(directory, d)):
                # Add test case to the results if it is a directory
                test_case_path = os.path.join(directory, d, td)
                if os.path.isdir(test_case_path):
                    tests[test_path_to_name(test_case_path)] = test_case_path
    
    return tests

def find_tests(base_dir):
    tests_path = 'TESTS'
    
    # Determine if "base_dir" is already a "TESTS" directory
    _, top_folder = os.path.split(base_dir)
    
    if top_folder == tests_path:
        # Already pointing at a "TESTS" directory
        return find_tests_in_tests_directory(base_dir)
    else:
        # Not pointing at a "TESTS" directory, so go find one!
        tests = {}
        
        for root, dirs, files in os.walk(base_dir):
            # Don't search build directories
            if '.build' in dirs:
                dirs.remove('.build')
            
            # If a "TESTS" directory is found, find the tests inside of it
            if tests_path in dirs:
                # Remove it from the directory walk
                dirs.remove(tests_path)
                
                # Get the tests inside of the "TESTS" directory
                new_tests = find_tests_in_tests_directory(os.path.join(root, tests_path))
                if new_tests:
                    tests.update(new_tests)
        
        return tests
    
def print_tests(tests, format="list"):
    if format == "list":
        for test_name, test_path in tests.iteritems():
            print "Test Case:"
            print "    Name: %s" % test_name
            print "    Path: %s" % test_path
    elif format == "json":
        print json.dumps(tests)
    else:
        print "Unknown format '%s'" % format
        sys.exit(1)

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
