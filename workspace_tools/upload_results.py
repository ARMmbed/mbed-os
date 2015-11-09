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

def add_project_runs(args):
    project_run_data = {}
    project_run_data['projectRuns'] = {}
    project_run_data['platforms_set'] = set()
    project_run_data['toolchains_set'] = set()
    project_run_data['names_set'] = set()
    project_run_data['hostOses_set'] = set()
    project_run_data['hostOses_set'].add(args.host_os)

    add_report(project_run_data, args.build_report, True, args.build_id, args.host_os)

    if (args.test_report):
        add_report(project_run_data, args.test_report, False, args.build_id, args.host_os)


    ts_data = format_project_run_data(project_run_data)
    r = requests.post(urlparse.urljoin(args.url, "api/projectRuns"), headers=create_headers(args), json=ts_data)
    finish_command('add-project-runs', r)

def format_project_run_data(project_run_data):
    ts_data = {}
    ts_data['projectRuns'] = []

    for hostOs in project_run_data['projectRuns'].keys():
        for platform in project_run_data['projectRuns'][hostOs].keys():
            for toolchain in project_run_data['projectRuns'][hostOs][platform].keys():
                for project in project_run_data['projectRuns'][hostOs][platform][toolchain].keys():
                    ts_data['projectRuns'].append(project_run_data['projectRuns'][hostOs][platform][toolchain][project])

    ts_data['platforms'] = list(project_run_data['platforms_set'])
    ts_data['toolchains'] = list(project_run_data['toolchains_set'])
    ts_data['names'] = list(project_run_data['names_set'])
    ts_data['hostOses'] = list(project_run_data['hostOses_set'])

    return ts_data

# prd - Project Run Data - Datastructure containing all project runs
# pt - Project Run - Data specific to one project run
def project_run_exists_full(prd, pr):
    if prd['projectRuns'][pr['hostOs']]:
        if prd['projectRuns'][pr['hostOs']][pr['platform']]:
            if prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']]:
                if prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']][pr['project']]:
                    return True

    return False

# prd - Project Run Data - Datastructure containing all project runs
# pt - Project Run - Data specific to one project run
def project_run_exists(prd, pr):
    if pr['project'] in prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']]:
        return True
    else:
        return False

# prd - Project Run Data - Datastructure containing all project runs
# pt - Project Run - Data specific to one project run
def prep_project_run(prd, pr):
    if not pr['hostOs'] in prd['projectRuns']:
        prd['projectRuns'][pr['hostOs']] = {}

    if not pr['platform'] in prd['projectRuns'][pr['hostOs']]:
        prd['projectRuns'][pr['hostOs']][pr['platform']] = {}

    if not pr['toolchain'] in prd['projectRuns'][pr['hostOs']][pr['platform']]:
        prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']] = {}

# prd - Project Run Data - Datastructure containing all project runs
# pt - Project Run - Data specific to one project run
def update_project_run(prd, pr):
    prep_project_run(prd, pr)

    if project_run_exists(prd, pr):
        prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']][pr['project']]['pass'] = pr['pass']
        prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']][pr['project']]['result'] = pr['result']

        if 'buildOutput' in pr:
            prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']][pr['project']]['buildOutput'] = pr['buildOutput']
        else:
            prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']][pr['project']]['testOutput'] = pr['testOutput']

    else:
        prd['projectRuns'][pr['hostOs']][pr['platform']][pr['toolchain']][pr['project']] = pr

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
        for properties in test_suite.findall('properties'):
            for property in properties.findall('property'):
                if property.attrib['name'] == 'target':
                    platform = property.attrib['value']
                    project_run_data['platforms_set'].add(platform)
                elif property.attrib['name'] == 'toolchain':
                    toolchain = property.attrib['value']
                    project_run_data['toolchains_set'].add(toolchain)

        for test_case in test_suite.findall('testcase'):
            projectRun = {}
            projectRun['build'] = build_id
            projectRun['hostOs'] = host_os
            projectRun['platform'] = platform
            projectRun['toolchain'] = toolchain
            projectRun['project'] = test_case.attrib['classname'].split('.')[-1]

            project_run_data['names_set'].add(projectRun['project'])

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

            if errors:
                projectRun['pass'] = False
                projectRun['result'] = errors[0].attrib['message']
            elif failures:
                projectRun['pass'] = False
                projectRun['result'] = failures[0].attrib['message']
            else:
                projectRun['pass'] = True
                projectRun['result'] = 'OK'

            update_project_run(project_run_data, projectRun)

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

    add_project_runs_parser = subparsers.add_parser('add-project-runs', help='add project runs to a build')
    add_project_runs_parser.add_argument('-b', '--build-id', required=True, help='build id')
    add_project_runs_parser.add_argument('-r', '--build-report', required=True, help='path to junit xml build report')
    add_project_runs_parser.add_argument('-t', '--test-report', required=False, help='path to junit xml test report')
    add_project_runs_parser.add_argument('-o', '--host-os', required=True, help='host os on which test was run')
    add_project_runs_parser.set_defaults(func=add_project_runs)

    args = parser.parse_args(arguments)
    args.func(args)

if __name__ == '__main__':
    main(sys.argv[1:])
