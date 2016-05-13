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

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.test_api import test_path_to_name, find_tests, print_tests, build_tests, test_spec_from_test_build
from tools.options import get_default_options_parser
from tools.build_api import build_project, build_library
from tools.targets import TARGET_MAP
from tools.utils import mkdir

if __name__ == '__main__':
    try:
        # Parse Options
        parser = get_default_options_parser()
        
        parser.add_option("-j", "--jobs",
                          type="int",
                          dest="jobs",
                          default=1,
                          help="Number of concurrent jobs (default 1). Use 0 for auto based on host machine's number of CPUs")

        parser.add_option("--source", dest="source_dir",
                          default=None, help="The source (input) directory (for sources other than tests). Defaults to current directory.", action="append")

        parser.add_option("--build", dest="build_dir",
                          default=None, help="The build (output) directory")

        parser.add_option("-l", "--list", action="store_true", dest="list",
                          default=False, help="List (recursively) available tests in order and exit")

        parser.add_option("-p", "--paths", dest="paths",
                          default=None, help="Limit the tests to those within the specified comma separated list of paths")
        
        format_choices = ["list", "json"]
        format_default_choice = "list"
        format_help = "Change the format in which tests are listed. Choices include: %s. Default: %s" % (", ".join(format_choices), format_default_choice)
        parser.add_option("-f", "--format", type="choice", dest="format",
                          choices=format_choices, default=format_default_choice, help=format_help)
        
        parser.add_option("-n", "--names", dest="names",
                          default=None, help="Limit the tests to a comma separated list of names")
                          
        parser.add_option("--test-spec", dest="test_spec",
                          default=None, help="Destination path for a test spec file that can be used by the Greentea automated test tool")
        
        parser.add_option("-v", "--verbose",
                          action="store_true",
                          dest="verbose",
                          default=False,
                          help="Verbose diagnostic output")

        (options, args) = parser.parse_args()

        # Filter tests by path if specified 
        if options.paths:
            all_paths = options.paths.split(",")
        else:
            all_paths = ["."]
        
        all_tests = {}
        tests = {}
        
        # Find all tests in the relevant paths
        for path in all_paths:
            all_tests.update(find_tests(path))
        
        # Filter tests by name if specified
        if options.names:
            all_names = options.names.split(",")
            
            all_tests_keys = all_tests.keys()
            for name in all_names:
                if name in all_tests_keys:
                    tests[name] = all_tests[name]
                else:
                    print "[Warning] Test with name '%s' was not found in the available tests" % (name)
        else:
            tests = all_tests

        if options.list:
            # Print available tests in order and exit
            print_tests(tests, options.format)
        else:
            # Build all tests
            if not options.build_dir:
                print "[ERROR] You must specify a build path"
                sys.exit(1)
            
            base_source_paths = options.source_dir
            
            # Default base source path is the current directory
            if not base_source_paths:
                base_source_paths = ['.']
            
            
            target = TARGET_MAP[options.mcu]
            
            lib_build_res = build_library(base_source_paths, options.build_dir, target, options.tool,
                                            options=options.options,
                                            jobs=options.jobs,
                                            clean=options.clean,
                                            archive=False)
            
            # Build all the tests
            test_build = build_tests(tests, [options.build_dir], options.build_dir, target, options.tool,
                    options=options.options,
                    clean=options.clean,
                    jobs=options.jobs)
            
            # If a path to a test spec is provided, write it to a file
            if options.test_spec:
                test_spec_data = test_spec_from_test_build(test_build)
                
                # Create the target dir for the test spec if necessary
                # mkdir will not create the dir if it already exists
                test_spec_dir = os.path.dirname(options.test_spec)
                if test_spec_dir:
                    mkdir(test_spec_dir)
                
                try:
                    with open(options.test_spec, 'w') as f:
                        f.write(json.dumps(test_spec_data, indent=2))
                except IOError, e:
                    print "[ERROR] Error writing test spec to file"
                    print e
            
        sys.exit()

    except KeyboardInterrupt, e:
        print "\n[CTRL+c] exit"
    except Exception,e:
        import traceback
        traceback.print_exc(file=sys.stdout)
        print "[ERROR] %s" % str(e)
        sys.exit(1)
