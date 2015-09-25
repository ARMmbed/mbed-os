#!/usr/bin/env python

import os
import sys
import signal
import argparse
import ast
import urllib2
import urllib
import time
import datetime
import xml.etree.ElementTree as ET

import pprint

import requests
import urlparse

pp = pprint.PrettyPrinter(indent=4)

def signal_handler(signal, frame):
    # Whenever a keyboard interrupt is received, cancel build
    print '\nKeyboard Interrupt received. Canceled build.'
    sys.exit(0)

def main(arguments):
    # Register keyboard interrupt handler
    signal.signal(signal.SIGINT, signal_handler)

    # Register and parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('info_type', choices=['build', 'testRuns'], help='info type')
    parser.add_argument('-u', '--url', help='url to ci site')
    parser.add_argument('-t', '--test-report', help='path to junit xml test report')
    parser.add_argument('-o', '--host-os', help='host os on which test was run')
    parser.add_argument('-b', '--build', help='build number')
    parser.add_argument('-k', '--api-key', help='api-key for posting data')
    args = parser.parse_args(arguments)

    headers = { 'X-Api-Key':  args.api_key }

    if args.info_type == 'build':
        build = {}
        build['id'] = args.build
        build['buildType'] = 'Nightly'
        build['source'] = 'https://github.com/mbedmicro/mbed'


        r = requests.post(urlparse.urljoin(args.url, "api/builds"), headers=headers, json=build)
        print("Build POST status", r.status_code, r.reason)
        print(r.text)

        if r.status_code <= 200:
            sys.exit(0)
        else:
            sys.exit(2)

    elif args.info_type == 'testRuns':

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
                testRun['build'] = args.build
                testRun['hostOs'] = args.host_os
                testRun['platform'] = platform
                testRun['toolchain'] = toolchain
                testRun['test'] = test_case.attrib['classname'].split('.')[-1]

                testIds_set.add(testRun['test'])

                system_outs = test_case.findall('system-out')

                testRun['output'] = system_outs[0].text

                errors = test_case.findall('error')

                if errors:
                    testRun['pass'] = False
                    testRun['result'] = errors[0].attrib['message']
                else:
                    testRun['pass'] = True
                    testRun['result'] = 'OK'

                ts_data['testRuns'].append(testRun)

        ts_data['platforms'] = list(platforms_set)
        ts_data['toolchains'] = list(toolchains_set)
        ts_data['testIds'] = list(testIds_set)
        ts_data['hostOses'] = list(hostOses_set)

        r = requests.post(urlparse.urljoin(args.url, "api/testRuns"), headers=headers, json=ts_data)
        print("testRuns POST status", r.status_code, r.reason)
        print(r.text)

        if r.status_code <= 200:
            sys.exit(0)
        else:
            sys.exit(2)

if __name__ == '__main__':
    main(sys.argv[1:])
