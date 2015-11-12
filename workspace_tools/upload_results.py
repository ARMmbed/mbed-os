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
import argparse
import xml.etree.ElementTree as ET
import requests
import urlparse

def create_headers(args):
    return { 'X-Api-Key':  args.api_key }

def finish_command(command, response):
    print(command, response.status_code, response.reason)
    print(response.text)

    if response.status_code < 400:
        sys.exit(0)
    else:
        sys.exit(2)

def create_build(args):
    build = {}
    build['buildType'] = args.build_type
    build['number'] = args.build_number
    build['source'] = args.build_source
    build['status'] = 'running'

    if build['buildType'] == 'Pull_Request':
        build['buildType'] = 'Pull Request'

    r = requests.post(urlparse.urljoin(args.url, "api/builds"), headers=create_headers(args), json=build)

    if r.status_code < 400:
        if args.property_file_format:
            print("MBED_BUILD_ID=" + r.text)
        else:
            print(r.text)

        sys.exit(0)
    else:
        sys.exit(2)

def finish_build(args):
    data = {}
    data['status'] = 'completed'

    r = requests.put(urlparse.urljoin(args.url, "api/builds/" + args.build_id), headers=create_headers(args), json=data)
    finish_command('finish-build', r)

def abort_build(args):
    data = {}
    data['status'] = 'aborted'

    r = requests.put(urlparse.urljoin(args.url, "api/builds/" + args.build_id), headers=create_headers(args), json=data)
    finish_command('abort-build', r)

def add_test_runs(args):
    tree = None

    try:
        tree = ET.parse(args.test_report)
    except:
        print(sys.exc_info()[0])
        print('Invalid path to test report.')
        sys.exit(1)

    test_suites = tree.getroot()

    ts_data = {}
    ts_data['testRuns'] = []

    platforms_set = set()
    toolchains_set = set()
    testIds_set = set()
    hostOses_set = set()

    hostOses_set.add(args.host_os)

    for test_suite in test_suites:
        platform = ""
        toolchain = ""
        for properties in test_suite.findall('properties'):
            for property in properties.findall('property'):
                if property.attrib['name'] == 'target':
                    platform = property.attrib['value']
                    platforms_set.add(platform)
                elif property.attrib['name'] == 'toolchain':
                    toolchain = property.attrib['value']
                    toolchains_set.add(toolchain)

        for test_case in test_suite.findall('testcase'):
            testRun = {}
            testRun['build'] = args.build_id
            testRun['hostOs'] = args.host_os
            testRun['platform'] = platform
            testRun['toolchain'] = toolchain
            testRun['test'] = test_case.attrib['classname'].split('.')[-1]

            testIds_set.add(testRun['test'])

            system_outs = test_case.findall('system-out')

            if system_outs:
                testRun['output'] = system_outs[0].text
            else:
                testRun['output'] = ""

            errors = test_case.findall('error')
            failures = test_case.findall('failure')

            if errors:
                testRun['pass'] = False
                testRun['result'] = errors[0].attrib['message']
            elif failures:
                testRun['pass'] = False
                testRun['result'] = failures[0].attrib['message']
            else:
                testRun['pass'] = True
                testRun['result'] = 'OK'

            ts_data['testRuns'].append(testRun)

    ts_data['platforms'] = list(platforms_set)
    ts_data['toolchains'] = list(toolchains_set)
    ts_data['testIds'] = list(testIds_set)
    ts_data['hostOses'] = list(hostOses_set)

    r = requests.post(urlparse.urljoin(args.url, "api/testRuns"), headers=create_headers(args), json=ts_data)
    finish_command('add-test-runs', r)

def main(arguments):
    # Register and parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-u', '--url', required=True, help='url to ci site')
    parser.add_argument('-k', '--api-key', required=True, help='api-key for posting data')

    subparsers = parser.add_subparsers(help='subcommand help')

    create_build_parser = subparsers.add_parser('create-build', help='create a new build')
    create_build_parser.add_argument('-b', '--build-number', required=True, help='build number')
    create_build_parser.add_argument('-T', '--build-type', choices=['Nightly', 'Limited', 'Pull_Request'], required=True, help='type of build')
    create_build_parser.add_argument('-s', '--build-source', required=True, help='url to source of build')
    create_build_parser.add_argument('-p', '--property-file-format', action='store_true', help='print result in the property file format')
    create_build_parser.set_defaults(func=create_build)

    finish_build_parser = subparsers.add_parser('finish-build', help='finish a running build')
    finish_build_parser.add_argument('-b', '--build-id', required=True, help='build id')
    finish_build_parser.set_defaults(func=finish_build)

    abort_build_parser = subparsers.add_parser('abort-build', help='abort a running build')
    abort_build_parser.add_argument('-b', '--build-id', required=True, help='build id')
    abort_build_parser.set_defaults(func=abort_build)

    add_test_runs_parser = subparsers.add_parser('add-test-runs', help='add test runs to a build')
    add_test_runs_parser.add_argument('-b', '--build-id', required=True, help='build id')
    add_test_runs_parser.add_argument('-t', '--test-report', required=True, help='path to junit xml test report')
    add_test_runs_parser.add_argument('-o', '--host-os', required=True, help='host os on which test was run')
    add_test_runs_parser.set_defaults(func=add_test_runs)

    args = parser.parse_args(arguments)
    args.func(args)

if __name__ == '__main__':
    main(sys.argv[1:])
