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

def promote_build(args):
    data = {}
    data['buildType'] = 'Release'

    r = requests.put(urlparse.urljoin(args.url, "api/builds/" + args.build_id), headers=create_headers(args), json=data)
    finish_command('promote-build', r)

def abort_build(args):
    data = {}
    data['status'] = 'aborted'

    r = requests.put(urlparse.urljoin(args.url, "api/builds/" + args.build_id), headers=create_headers(args), json=data)
    finish_command('abort-build', r)

def add_project_runs(args):
    '''
    -------------------------------------
    Notes on 'project_run_data' structure:
    --------------------------------------
        'projectRuns' - Tree structure used to keep track of what projects have
            been logged in different report files. The tree is organized as follows:

            'projectRuns': {                - Root element of tree

                'hostOs': {                 - Host OS on which project was built/tested
                                                - ex. windows, linux, or mac

                    'platform': {           - Platform for which project was built/tested
                                              (Corresponds to platform names in targets.py)
                                                - ex. K64F, LPC1768, NRF51822, etc.

                        'toolchain': {      - Toolchain with which project was built/tested
                                              (Corresponds to TOOLCHAIN_CLASSES names in toolchains/__init__.py)
                                                - ex. ARM, uARM, GCC_ARM, etc.

                            'project': {    - Project that was build/tested
                                              (Corresponds to test id in tests.py or library id in libraries.py)
                                                - For tests, ex. MBED_A1, MBED_11, DTCT_1 etc.
                                                - For libraries, ex. MBED, RTX, RTOS, etc.

                            },
                            ...
                        },
                        ...
                    },
                    ...
                }
            }

        'platforms_set' - Set of all the platform names mentioned in the given report files

        'toolchains_set' - Set of all the toolchain names mentioned in the given report files

        'names_set' - Set of all the project names mentioned in the given report files

        'hostOses_set' - Set of all the host names given (only given by the command line arguments)
    '''

    project_run_data = {}
    project_run_data['projectRuns'] = {}
    project_run_data['platforms_set'] = set()
    project_run_data['vendors_set'] = set()
    project_run_data['toolchains_set'] = set()
    project_run_data['names_set'] = set()
    project_run_data['hostOses_set'] = set()
    project_run_data['hostOses_set'].add(args.host_os)

    if args.build_report:
        add_report(project_run_data, args.build_report, True, args.build_id, args.host_os)

    if args.test_report:
        add_report(project_run_data, args.test_report, False, args.build_id, args.host_os)

    ts_data = format_project_run_data(project_run_data, args.limit)
    total_result = True
    
    total_parts = len(ts_data)
    print "Uploading project runs in %d parts" % total_parts
    
    for index, data in enumerate(ts_data):
        r = requests.post(urlparse.urljoin(args.url, "api/projectRuns"), headers=create_headers(args), json=data)
        print("add-project-runs part %d/%d" % (index + 1, total_parts), r.status_code, r.reason)
        print(r.text)
    
        if r.status_code >= 400:
            total_result = False
    
    if total_result:
        print "'add-project-runs' completed successfully"
        sys.exit(0)
    else:
        print "'add-project-runs' failed"
        sys.exit(2)

def prep_ts_data():
    ts_data = {}
    ts_data['projectRuns'] = []
    ts_data['platforms'] = set()
    ts_data['vendors'] = set()
    ts_data['toolchains'] = set()
    ts_data['names'] = set()
    ts_data['hostOses'] = set()
    return ts_data

def finish_ts_data(ts_data, project_run_data):
    ts_data['platforms'] = list(ts_data['platforms'])
    ts_data['vendors'] = list(ts_data['vendors'])
    ts_data['toolchains'] = list(ts_data['toolchains'])
    ts_data['names'] = list(ts_data['names'])
    ts_data['hostOses'] = list(ts_data['hostOses'])
    
    # Add all vendors to every projectRun submission
    # TODO Either add "vendor" to the "project_run_data"
    #      or remove "vendor" entirely from the viewer
    ts_data['vendors'] = list(project_run_data['vendors_set'])
    
def format_project_run_data(project_run_data, limit):
    all_ts_data = []
    current_limit_count = 0
    
    ts_data = prep_ts_data()
    ts_data['projectRuns'] = []

    for hostOs_name, hostOs in project_run_data['projectRuns'].items():
        for platform_name, platform in hostOs.items():
            for toolchain_name, toolchain in platform.items():
                for project_name, project in toolchain.items():
                    if current_limit_count >= limit:
                        finish_ts_data(ts_data, project_run_data)
                        all_ts_data.append(ts_data)
                        ts_data = prep_ts_data()
                        current_limit_count = 0
                    
                    ts_data['projectRuns'].append(project)
                    ts_data['platforms'].add(platform_name)
                    ts_data['toolchains'].add(toolchain_name)
                    ts_data['names'].add(project_name)
                    ts_data['hostOses'].add(hostOs_name)
                    current_limit_count += 1
    
    if current_limit_count > 0:
        finish_ts_data(ts_data, project_run_data)
        all_ts_data.append(ts_data)
    
    return all_ts_data

def find_project_run(projectRuns, project):
    keys = ['hostOs', 'platform', 'toolchain', 'project']

    elem = projectRuns

    for key in keys:
        if not project[key] in elem:
            return None

        elem = elem[project[key]]

    return elem

def add_project_run(projectRuns, project):
    keys = ['hostOs', 'platform', 'toolchain']

    elem = projectRuns

    for key in keys:
        if not project[key] in elem:
            elem[project[key]] = {}

        elem = elem[project[key]]

    elem[project['project']] = project

def update_project_run_results(project_to_update, project, is_build):
    if is_build:
        project_to_update['buildPass'] = project['buildPass']
        project_to_update['buildResult'] = project['buildResult']
        project_to_update['buildOutput'] = project['buildOutput']
    else:
        project_to_update['testPass'] = project['testPass']
        project_to_update['testResult'] = project['testResult']
        project_to_update['testOutput'] = project['testOutput']

def update_project_run(projectRuns, project, is_build):
    found_project = find_project_run(projectRuns, project)
    if found_project:
        update_project_run_results(found_project, project, is_build)
    else:
        add_project_run(projectRuns, project)

def add_report(project_run_data, report_file, is_build, build_id, host_os):
    tree = None

    try:
        tree = ET.parse(report_file)
    except:
        print(sys.exc_info()[0])
        print('Invalid path to report: %s', report_file)
        sys.exit(1)

    test_suites = tree.getroot()

    for test_suite in test_suites:
        platform = ""
        toolchain = ""
        vendor = ""
        for properties in test_suite.findall('properties'):
            for property in properties.findall('property'):
                if property.attrib['name'] == 'target':
                    platform = property.attrib['value']
                    project_run_data['platforms_set'].add(platform)
                elif property.attrib['name'] == 'toolchain':
                    toolchain = property.attrib['value']
                    project_run_data['toolchains_set'].add(toolchain)
                elif property.attrib['name'] == 'vendor':
                    vendor = property.attrib['value']
                    project_run_data['vendors_set'].add(vendor)

        for test_case in test_suite.findall('testcase'):
            projectRun = {}
            projectRun['build'] = build_id
            projectRun['hostOs'] = host_os
            projectRun['platform'] = platform
            projectRun['toolchain'] = toolchain
            projectRun['project'] = test_case.attrib['classname'].split('.')[-1]
            projectRun['vendor'] = vendor

            project_run_data['names_set'].add(projectRun['project'])

            should_skip = False
            skips = test_case.findall('skipped')

            if skips:
                should_skip = skips[0].attrib['message'] == 'SKIP'

            if not should_skip:
                system_outs = test_case.findall('system-out')

                output = ""
                if system_outs:
                    output = system_outs[0].text

                if is_build:
                    projectRun['buildOutput'] = output
                else:
                    projectRun['testOutput'] = output

                errors = test_case.findall('error')
                failures = test_case.findall('failure')
                projectRunPass = None
                result = None

                if errors:
                    projectRunPass = False
                    result = errors[0].attrib['message']
                elif failures:
                    projectRunPass = False
                    result = failures[0].attrib['message']
                elif skips:
                    projectRunPass = True
                    result = skips[0].attrib['message']
                else:
                    projectRunPass = True
                    result = 'OK'

                if is_build:
                    projectRun['buildPass'] = projectRunPass
                    projectRun['buildResult'] = result
                else:
                    projectRun['testPass'] = projectRunPass
                    projectRun['testResult'] = result

                update_project_run(project_run_data['projectRuns'], projectRun, is_build)

def main(arguments):
    # Register and parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-u', '--url', required=True, help='url to ci site')
    parser.add_argument('-k', '--api-key', required=True, help='api-key for posting data')

    subparsers = parser.add_subparsers(help='subcommand help')

    create_build_parser = subparsers.add_parser('create-build', help='create a new build')
    create_build_parser.add_argument('-b', '--build-number', required=True, help='build number')
    create_build_parser.add_argument('-T', '--build-type', choices=['Nightly', 'Limited', 'Pull_Request', 'Release_Candidate'], required=True, help='type of build')
    create_build_parser.add_argument('-s', '--build-source', required=True, help='url to source of build')
    create_build_parser.add_argument('-p', '--property-file-format', action='store_true', help='print result in the property file format')
    create_build_parser.set_defaults(func=create_build)

    finish_build_parser = subparsers.add_parser('finish-build', help='finish a running build')
    finish_build_parser.add_argument('-b', '--build-id', required=True, help='build id')
    finish_build_parser.set_defaults(func=finish_build)

    finish_build_parser = subparsers.add_parser('promote-build', help='promote a build to a release')
    finish_build_parser.add_argument('-b', '--build-id', required=True, help='build id')
    finish_build_parser.set_defaults(func=promote_build)

    abort_build_parser = subparsers.add_parser('abort-build', help='abort a running build')
    abort_build_parser.add_argument('-b', '--build-id', required=True, help='build id')
    abort_build_parser.set_defaults(func=abort_build)

    add_project_runs_parser = subparsers.add_parser('add-project-runs', help='add project runs to a build')
    add_project_runs_parser.add_argument('-b', '--build-id', required=True, help='build id')
    add_project_runs_parser.add_argument('-r', '--build-report', required=False, help='path to junit xml build report')
    add_project_runs_parser.add_argument('-t', '--test-report', required=False, help='path to junit xml test report')
    add_project_runs_parser.add_argument('-o', '--host-os', required=True, help='host os on which test was run')
    add_project_runs_parser.add_argument('-l', '--limit', required=False, type=int, default=1000, help='Limit the number of project runs sent at a time to avoid HTTP errors (default is 1000)')
    add_project_runs_parser.set_defaults(func=add_project_runs)

    args = parser.parse_args(arguments)
    args.func(args)

if __name__ == '__main__':
    main(sys.argv[1:])
