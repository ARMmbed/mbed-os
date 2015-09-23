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
import pymysql

import pprint

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
    parser.add_argument('-H', '--host', help='mysql host url')
    parser.add_argument('-u', '--user', help='mysql username')
    parser.add_argument('-p', '--password', help='mysql password')
    parser.add_argument('-P', '--port', help='mysql port')
    parser.add_argument('-D', '--database', help='mysql database')
    parser.add_argument('-t', '--test-report', help='path to junit xml test report')
    parser.add_argument('-o', '--host-os', help='host os on which test was run')
    args = parser.parse_args(arguments)

    tree = None

    try:
        tree = ET.parse(args.test_report)
    except:
        print(sys.exc_info()[0])
        print('Invalid path to test report.')
        sys.exit(1)


    test_suites = tree.getroot()

    ts_data = {}
    ts_data['testSuites'] = []

    platforms_set = set()
    toolchains_set = set()
    testIds_set = set()


    for test_suite in test_suites:
        ts = {}
        ts['test_cases'] = []
        for properties in test_suite.findall('properties'):
            for property in properties.findall('property'):
                if property.attrib['name'] == 'target':
                    ts['platform'] = property.attrib['value']
                    platforms_set.add(ts['platform'])
                elif property.attrib['name'] == 'toolchain':
                    ts['toolchain'] = property.attrib['value']
                    toolchains_set.add(ts['toolchain'])

        for test_case in test_suite.findall('testcase'):
            tc = {}
            tc['hostOs'] = args.host_os
            tc['name'] = test_case.attrib['name']

            tc['testId'] = test_case.attrib['classname'].split('.')[-1]
            testIds_set.add(tc['testId'])

            system_outs = test_case.findall('system-out')

            tc['output'] = system_outs[0].text

            errors = test_case.findall('error')

            if errors:
                tc['pass'] = False
                tc['result'] = errors[0].attrib['message']
            else:
                tc['pass'] = True
                tc['result'] = 'OK'

            ts['test_cases'].append(tc)

        ts_data['testSuites'].append(ts)

    ts_data['platforms'] = platforms_set
    ts_data['toolchains'] = toolchains_set
    ts_data['testIds'] = testIds_set

    pp.pprint(ts_data)

    '''
    # Open database connection
    db = MySQLdb.connect(host=args.host, user=args.user, password=args.password, db=args.database,cursorclass=pymysql.cursors.DictCursor)

    # prepare a cursor object using cursor() method
    cursor = db.cursor()
    '''

if __name__ == '__main__':
    main(sys.argv[1:])
