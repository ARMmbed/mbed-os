#! /usr/bin/env python
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
"""
import sys
from time import time
from os.path import join, abspath, dirname, normpath
from optparse import OptionParser
import json

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.build_api import build_library
from tools.build_api import write_build_report
from tools.targets import TARGET_MAP, TARGET_NAMES
from tools.toolchains import TOOLCHAINS
from tools.test_exporters import ReportExporter, ResultExporterType
from tools.test_api import find_tests, build_tests, test_spec_from_test_builds
from tools.build_release import OFFICIAL_MBED_LIBRARY_BUILD

if __name__ == '__main__':
    try:
        parser = OptionParser()
        
        parser.add_option("--source", dest="source_dir",
                          default=None, help="The source (input) directory (for sources other than tests). Defaults to current directory.", action="append")
                          
        parser.add_option("--build", dest="build_dir",
                          default=None, help="The build (output) directory")
        
        parser.add_option('-c', '--clean',
                          dest='clean',
                          metavar=False,
                          action="store_true",
                          help='Clean the build directory')
        
        parser.add_option('-a', '--all', dest="all", default=False, action="store_true",
                          help="Build every target (including unofficial targets) and with each of the supported toolchains")
                          
        parser.add_option('-o', '--official', dest="official_only", default=False, action="store_true",
                          help="Build using only the official toolchain for each target")
                          
        parser.add_option("-D", "",
                          action="append",
                          dest="macros",
                          help="Add a macro definition")
                          
        parser.add_option("-j", "--jobs", type="int", dest="jobs",
                          default=0, help="Number of concurrent jobs. Default: 0/auto (based on host machine's number of CPUs)")
                          
        parser.add_option("-v", "--verbose", action="store_true", dest="verbose",
                          default=False, help="Verbose diagnostic output")
                          
        parser.add_option("-t", "--toolchains", dest="toolchains", help="Use toolchains names separated by comma")

        parser.add_option("-p", "--platforms", dest="platforms", default="", help="Build only for the platform namesseparated by comma")

        parser.add_option("", "--config", action="store_true", dest="list_config",
                          default=False, help="List the platforms and toolchains in the release in JSON")
        
        parser.add_option("", "--test-spec", dest="test_spec",
                          default=None, help="Destination path for a test spec file that can be used by the Greentea automated test tool")

        parser.add_option("", "--build-report-junit", dest="build_report_junit", help="Output the build results to an junit xml file")
        
        parser.add_option("--continue-on-build-fail", action="store_true", dest="continue_on_build_fail",
                          default=False, help="Continue trying to build all tests if a build failure occurs")

        options, args = parser.parse_args()
        
        # Get set of valid targets
        all_platforms = set(TARGET_NAMES)
        bad_platforms = set()
        platforms = set()
        if options.platforms != "":
            platforms = set(options.platforms.split(","))
            bad_platforms = platforms.difference(all_platforms)
            platforms = platforms.intersection(all_platforms)
        elif options.all:
            platforms = all_platforms
        else:
            platforms = set(x[0] for x in OFFICIAL_MBED_LIBRARY_BUILD)
            bad_platforms = platforms.difference(all_platforms)
            platforms = platforms.intersection(all_platforms)
        
        for bad_platform in bad_platforms:
            print "Platform '%s' is not a valid platform. Skipping." % bad_platform
        
        if options.platforms:
            print "Limiting build to the following platforms: %s" % ",".join(platforms)
        
        # Get set of valid toolchains
        all_toolchains = set(TOOLCHAINS)
        bad_toolchains = set()
        toolchains = set()
        
        if options.toolchains:
            toolchains = set(options.toolchains.split(","))
            bad_toolchains = toolchains.difference(all_toolchains)
            toolchains = toolchains.intersection(all_toolchains)
        else:
            toolchains = all_toolchains
        
        for bad_toolchain in bad_toolchains:
            print "Toolchain '%s' is not a valid toolchain. Skipping." % bad_toolchain
        
        if options.toolchains:
            print "Limiting build to the following toolchains: %s" % ",".join(toolchains)
        
        build_config = {}
        
        for platform in platforms:
            target = TARGET_MAP[platform]
            
            if options.official_only:
                default_toolchain = getattr(target, 'default_toolchain', 'ARM')
                build_config[platform] = list(toolchains.intersection(set([default_toolchain])))
            else:
                build_config[platform] = list(toolchains.intersection(set(target.supported_toolchains)))
        
        if options.list_config:
            print json.dumps(build_config, indent=4)
            sys.exit(0)
        
        # Ensure build directory is set
        if not options.build_dir:
            print "[ERROR] You must specify a build path"
            sys.exit(1)
        
        # Default base source path is the current directory
        base_source_paths = options.source_dir
        if not base_source_paths:
            base_source_paths = ['.']
        
        
        start = time()    
        build_report = {}
        build_properties = {}
        test_builds = {}
        total_build_success = True

        for target_name, target_toolchains in build_config.iteritems():
            target = TARGET_MAP[target_name]
            
            for target_toolchain in target_toolchains:
                library_build_success = True
                
                try:
                    build_directory = join(options.build_dir, target_name, target_toolchain)
                    # Build sources
                    build_library(base_source_paths, build_directory, target, target_toolchain,
                                                    jobs=options.jobs,
                                                    clean=options.clean,
                                                    report=build_report,
                                                    properties=build_properties,
                                                    name="mbed-os",
                                                    macros=options.macros,
                                                    verbose=options.verbose,
                                                    archive=False)
                except Exception, e:
                    library_build_success = False
                    print "Failed to build library"
                    print e
                    
                if options.continue_on_build_fail or library_build_success:
                    # Build all the tests
                    all_tests = find_tests(base_source_paths[0], target_name, toolchain_name)
                    test_build_success, test_build = build_tests(all_tests, [build_directory], build_directory, target, target_toolchain,
                            clean=options.clean,
                            report=build_report,
                            properties=build_properties,
                            macros=options.macros,
                            verbose=options.verbose,
                            jobs=options.jobs,
                            continue_on_build_fail=options.continue_on_build_fail)
                            
                    if not test_build_success:
                        total_build_success = False
                        print "Failed to build some tests, check build log for details"
                    
                    test_builds.update(test_build)
                else:
                    total_build_success = False
                    break
            
        # If a path to a test spec is provided, write it to a file
        if options.test_spec:
            test_spec_data = test_spec_from_test_builds(test_builds)
            
            # Create the target dir for the test spec if necessary
            # mkdir will not create the dir if it already exists
            test_spec_dir = dirname(options.test_spec)
            if test_spec_dir:
                mkdir(test_spec_dir)
            
            try:
                with open(options.test_spec, 'w') as f:
                    f.write(json.dumps(test_spec_data, indent=2))
            except IOError, e:
                print "[ERROR] Error writing test spec to file"
                print e
        
        # If a path to a JUnit build report spec is provided, write it to a file
        if options.build_report_junit:
            report_exporter = ReportExporter(ResultExporterType.JUNIT)
            report_exporter.report_to_file(build_report, options.build_report_junit, test_suite_properties=build_properties)

        print "\n\nCompleted in: (%.2f)s" % (time() - start)

        print_report_exporter = ReportExporter(ResultExporterType.PRINT, package="build")
        status = print_report_exporter.report(build_report)

        if status:
            sys.exit(0)
        else:
            sys.exit(1)
    
    except KeyboardInterrupt, e:
        print "\n[CTRL+c] exit"
    except Exception,e:
        import traceback
        traceback.print_exc(file=sys.stdout)
        print "[ERROR] %s" % str(e)
        sys.exit(1)
