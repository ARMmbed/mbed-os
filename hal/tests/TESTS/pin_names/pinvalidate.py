"""
Copyright (c) 2020 ARM Limited

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

import os
import re
import argparse
import pathlib
import json
from tabulate import tabulate

def find_target(target_name = ''):
    mbed_os_root = os.path.abspath(os.path.join(pathlib.Path(__file__).parent.absolute(), '..', '..', '..', '..'))
    targets = dict()
    
    try:
        targets_json_file = open(os.path.join(mbed_os_root, 'targets', 'targets.json'))
        target_data = json.load(targets_json_file)
    except:
        exit("Failed to open targets.json")

    for root, dirs, files in os.walk(mbed_os_root):
        for f in files:
            if f == "PinNames.h":
                path = os.path.join(root, f)
                name_components = re.findall("TARGET_([a-zA-Z0-9_]*)[\/\\\]", path)
                name = '_'.join(name_components)
                if not name:
                    continue
                if target_name:
                    if target_name not in name:
                        continue
                # try to find corresponding target in targets.json
                matches = []
                for target in target_data:
                    if "public" in target_data[target]:
                        if target_data[target]['public'] == False:
                            continue
                    if target in name:
                        matches.append(target)
                if len(matches) == 0:
                    #print("No match found in targets.json for " + name)
                    targets[name] = path
                elif len(matches) == 1:
                    targets[matches[0]] = path
                elif len(matches) > 1:
                    #print("Multipe matches found " + str(matches) + " in targets.json for " + name)
                    targets[name] = path
    return targets

def target_has_arduino_form_factor(target_name):
    mbed_os_root = os.path.abspath(os.path.join(pathlib.Path(__file__).parent.absolute(), '..', '..', '..', '..'))
    try:
        targets_json_file = open(os.path.join(mbed_os_root, 'targets', 'targets.json'))
        target_data = json.load(targets_json_file)
    except:
        exit("Failed to open targets.json")
    
    if target_name in target_data:
        if 'supported_form_factors' in target_data[target_name]:
            form_factors = target_data[target_name]['supported_form_factors']
            if 'ARDUINO' in form_factors or 'ARDUINO_UNO' in form_factors:
                return True
    
    return False

def pinName_to_dict(pinName_file_content):
    pinName_enum_dict = dict()
    pinName_enum_match = re.search("typedef enum {\n([^}]*)\n} PinName;", pinName_file_content)
    if pinName_enum_match:
        pinName_enum_body = pinName_enum_match.group(1)
        pinName_enum_dict = dict(re.findall("^\s*([a-zA-Z0-9_]+)\s*=\s*([a-zA-Z0-9_]+)", pinName_enum_body, re.MULTILINE))
    
    pinName_define_dict = dict(re.findall("^#define\s+([a-zA-Z0-9_]+)\s+([a-zA-Z0-9_]+)", pinName_file_content, re.MULTILINE))
    
    return {**pinName_enum_dict, **pinName_define_dict}

def identity_assignment_check(pinName_dict):
    invalid_items = []
    for key, val in pinName_dict.items():
        if val == key:
            message = "cannot assign value to itself"
            invalid_items.append({'key': key, 'val': val, 'message': message})
    return invalid_items

def nc_assignment_check(pinName_dict):
    invalid_items = []
    for key, val in pinName_dict.items():
        if re.match("^((LED|BUTTON)\d*|USBTX|USBRX)$", key):
            if val == 'NC':
                message = "cannot be NC"
                invalid_items.append({'key': key, 'val': val, 'message': message})
    return invalid_items

def duplicate_assignment_check(pinName_dict):
    used_pins = []
    used_pins_friendly = []
    invalid_items = []

    for key, val in pinName_dict.items():
        if re.match("^((LED|BUTTON)\d*|USBTX|USBRX)$", key):
            if val == 'NC':
                continue
            # resolve to literal
            realval = val
            depth = 0
            while not re.match("(0x[0-9a-fA-F]+|[1-9][0-9]*|0[1-7][0-7]+|0b[01]+)[uUlL]{0,2}", realval):
                try:
                    realval = pinName_dict[realval]
                    depth += 1
                except:
                    break
                if depth > 10:
                    break

            if realval in used_pins:
                message = "already assigned to " + used_pins_friendly[used_pins.index(realval)]
                invalid_items.append({'key': key, 'val': val, 'message': message})
                continue

            used_pins.append(realval)
            used_pins_friendly.append(key + ' = ' + val)
    return invalid_items

def arduino_duplicate_assignment_check(pinName_dict):
    used_pins = []
    used_pins_friendly = []
    invalid_items = []

    for key, val in pinName_dict.items():
        if re.match("^ARDUINO_UNO_[AD]\d+$", key):
            if val == 'NC':
                continue
            # resolve to literal
            realval = val
            depth = 0
            while not re.match("(0x[0-9a-fA-F]+|[1-9][0-9]*|0[1-7][0-7]+|0b[01]+)[uUlL]{0,2}", realval):
                try:
                    realval = pinName_dict[realval]
                    depth += 1
                except:
                    break
                if depth > 10:
                    break

            if realval in used_pins:
                message = "already assigned to " + used_pins_friendly[used_pins.index(realval)]
                invalid_items.append({'key': key, 'val': val, 'message': message})
                continue

            used_pins.append(realval)
            used_pins_friendly.append(key + ' = ' + val)
    return invalid_items

def arduino_existence_check(pinName_dict):
    required_pins = [
        'ARDUINO_UNO_A0',
        'ARDUINO_UNO_A1',
        'ARDUINO_UNO_A2',
        'ARDUINO_UNO_A3',
        'ARDUINO_UNO_A4',
        'ARDUINO_UNO_A5',
        'ARDUINO_UNO_D0',
        'ARDUINO_UNO_D1',
        'ARDUINO_UNO_D2',
        'ARDUINO_UNO_D3',
        'ARDUINO_UNO_D4',
        'ARDUINO_UNO_D5',
        'ARDUINO_UNO_D6',
        'ARDUINO_UNO_D7',
        'ARDUINO_UNO_D8',
        'ARDUINO_UNO_D9',
        'ARDUINO_UNO_D10',
        'ARDUINO_UNO_D11',
        'ARDUINO_UNO_D12',
        'ARDUINO_UNO_D13',
        'ARDUINO_UNO_D14',
        'ARDUINO_UNO_D15'
    ]

    invalid_items = []

    for pin in required_pins:
        if pin not in pinName_dict:
            message = pin + " not defined"
            invalid_items.append({'key': pin, 'val': '', 'message': message})
    
    return invalid_items

def arduino_nc_assignment_check(pinName_dict):
    invalid_items = []
    for key, val in pinName_dict.items():
        if re.match("^ARDUINO_UNO_[AD]\d+$", key):
            if val == 'NC':
                message = "cannot be NC"
                invalid_items.append({'key': key, 'val': val, 'message': message})
    return invalid_items

def legacy_assignment_check(pinName_content):
    invalid_items = []
    legacy_assignments = dict(re.findall("^\s*((?:LED|BUTTON)\d*)\s*=\s*([a-zA-Z0-9_]+)", pinName_content, re.MULTILINE))
    for key, val in legacy_assignments.items():
        message = "legacy assignment; LEDs and BUTTONs must be #define'd"
        invalid_items.append({'key': key, 'val': val, 'message': message})
    return invalid_items


def print_summary(report):
    targets = []
    for case in report:
        targets.append(case['platform_name'])
    targets = list(dict.fromkeys(targets))
    
    table = []
    for target in targets:
        result = 'PASSED'
        error_count = 0
        for case in report:
            if case['platform_name'] != target:
                continue
            if case['result'] == 'FAILED':
                result = 'FAILED'
                error_count += 1
        table.append([target, result, error_count])
    
    return tabulate(table, headers=['Platform name', 'Result', 'Error count'], tablefmt='grid')

def print_suite_summary(report):
    targets = []
    for case in report:
        targets.append(case['platform_name'])
    targets = list(dict.fromkeys(targets))
    
    table = []
    for target in targets:
        suites = []
        for case in report:
            if case['platform_name'] != target:
                continue
            suites.append(case['suite_name'])
        suites = list(dict.fromkeys(suites))

        for suite in suites:
            result = 'PASSED'
            error_count = 0
            for case in report:
                if case['platform_name'] != target:
                    continue
                if case['suite_name'] != suite:
                    continue
                if case['result'] == 'FAILED':
                    result = 'FAILED'
                    error_count += 1
            table.append([target, suite, result, error_count])
    
    return tabulate(table, headers=['Platform name', 'Test suite', 'Result', 'Error count'], tablefmt='grid')

def print_report(report, print_error_detail, tablefmt='grid'):
    table = []
    for case in report:
        errors_str = ""
        for error in case['errors']:
            errors_str += '\n'
            if error['key']:
                errors_str += error['key']
            if error['val']:
                errors_str += " = " + error['val']
            if error['message']:
                errors_str += " <-- " + error['message']
        
        if errors_str == '':
            errors_str = 'None'

        if print_error_detail:
            table.append([case['platform_name'], case['suite_name'], case['case_name'], case['result'], len(case['errors']), errors_str.lstrip()])
        else:
            table.append([case['platform_name'], case['suite_name'], case['case_name'], case['result'], len(case['errors'])])

    return tabulate(table, headers=['Platform name', 'Test suite', 'Test case', 'Result', 'Error count', 'Errors'], tablefmt=tablefmt)

def print_pretty_html_report(report):
    output  = "<html><head><style>table, td, tr { border: 2px solid black; border-collapse: collapse; padding: 5px; font-family: Helvetica, serif; }</style></head>"
    output += "<body><p><button onclick='e=document.getElementsByTagName(\"details\");for(var i=0;i<e.length;i++){e[i].setAttribute(\"open\",\"true\")};'>Expand all errors</button></p><table>"
    output += "<tr><th>Platform name</th><th>Test suite</th><th>Test case</th><th>Result</th><th>Error count</th><th>Errors</th></tr>"
    
    for case in report:
        output += "<tr>"

        if case['errors']:
            error_details = "<details><summary>View errors</summary><table>"
            for error in case['errors']:
                error_details += "<tr>"
                if error['key']:
                    error_details += "<td>" + error['key'] + "</td>"
                if error['val']:
                    error_details += "<td>" + error['val'] + "</td>"
                if error['message']:
                    error_details += "<td>" + error['message'] + "</td>"
                error_details += "</tr>"
            error_details += "</table></details>"
        else:
            error_details = ""
        
        output += "<td>" + case['platform_name'] + "</td>"
        output += "<td>" + case['suite_name'] + "</td>"
        output += "<td>" + case['case_name'] + "</td>"

        if case['result'] == "PASSED":
            color = "green"
            count_color = "black"
        else:
            color = "red"
            count_color = "red"
        
        output += "<td style='color:" + color + "'>" + case['result'] + "</td>"
        output += "<td style='color:" + count_color + "'>" + str(len(case['errors'])) + "</td>"
        output += "<td>" + error_details + "</td>"

        output += "</tr>"

    output += "</table></body></table>"

    return output

def all_cases_passed(report):
    result = True
    for case in report:
        if case['result'] == 'FAILED':
            result = False
    return result 

test_cases = [
    {'suite_name': 'generic', 'case_name': 'identity', 'case_function': identity_assignment_check, 'case_input': 'dict'},
    {'suite_name': 'generic', 'case_name': 'nc', 'case_function': nc_assignment_check, 'case_input': 'dict'},
    {'suite_name': 'generic', 'case_name': 'duplicate', 'case_function': duplicate_assignment_check, 'case_input': 'dict'},
    {'suite_name': 'generic', 'case_name': 'legacy', 'case_function': legacy_assignment_check, 'case_input': 'content'},
    {'suite_name': 'arduino_uno', 'case_name': 'duplicate', 'case_function': arduino_duplicate_assignment_check, 'case_input': 'dict'},
    {'suite_name': 'arduino_uno', 'case_name': 'existence', 'case_function': arduino_existence_check, 'case_input': 'dict'},
    {'suite_name': 'arduino_uno', 'case_name': 'nc', 'case_function': arduino_nc_assignment_check, 'case_input': 'dict'}
]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", '--suite_names', help="Run specific test suite. Use comma to seperate multiple suites.")
    parser.add_argument('-v', '--verbose', action='count', default=0, help="Verbosity of the report (none to -vvv). Only applies for 'prettytext' output format.")
    parser.add_argument("-f", '--full_name', action="store_true", help="Don't truncate long platform names in human-readable output formats")
    parser.add_argument("-o", '--output_format', default='prettytext', help="Set the output format: prettytext (default), json or html")
    parser.add_argument("-w", '--output_file', help="File to write output to, instead of printing to stdout")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-t", '--targets', help="Target name. Use comma to seperate multiple targets. THIS FEATURE IS EXPERIMENTAL!")
    group.add_argument("-p", '--paths', help="Path to PinNames.h file. Use comma to seperate multiple paths.")
    group.add_argument("-a", '--all', action="store_true", help="Run tests on all targets.")
    args = parser.parse_args()

    suites = []
    if args.suite_names:
        suites = args.suite_names.split(',')

    targets = dict()
    if args.paths:
        paths = args.paths.split(',')
        for path in paths:
            targets[path] = path
    elif args.targets:
        target_names = args.targets.split(',')
        for target_name in target_names:
            targets = {**targets, **find_target(target_name)}
    elif args.all:
        targets = find_target()

    report = []
    for target, path in targets.items():
        try:
            pinName_file = open(path)
            pinName_content = pinName_file.read()
        except:
            exit("Cannot open file")
        
        try:
            pinName_dict = pinName_to_dict(pinName_content)
        except:
            exit("Cannot extract PinName enum from file")

        arduino_support = target_has_arduino_form_factor(target)

        for case in test_cases:
            if suites:
                if case['suite_name'] not in suites:
                    continue
            else:
                if not arduino_support and case['suite_name'] == 'arduino_uno':
                    continue

            if case['case_input'] == 'dict':
                case_input = pinName_dict
            elif case['case_input'] == 'content':
                case_input = pinName_content

            case_output = case['case_function'](case_input)

            if case_output:
                case_result = 'FAILED'
            else:
                case_result = 'PASSED'
            
            platform_name = target
            if not args.full_name and args.output_format == 'prettytext':
                if len(platform_name) > 40:
                    platform_name = "..." + platform_name[-40:]
            
            report.append({'platform_name': platform_name, 'suite_name': case['suite_name'], 'case_name': case['case_name'], 'result': case_result, 'errors': case_output})

    if args.output_format == 'json':
        output = json.dumps(report)
    elif args.output_format == 'html':
        output = print_pretty_html_report(report)
    else:
        if args.verbose == 0:
            output = print_summary(report)
        elif args.verbose == 1:
            output = print_suite_summary(report)    
        elif args.verbose == 2:
            output = print_report(report, False)
        elif args.verbose > 2:
            output = print_report(report, True)

    if args.output_file:
        output_file = open(args.output_file, 'w')
        output_file.write(output)
        output_file.close()
    else:
        print(output)

    if all_cases_passed(report):
        exit(0)
    else:
        exit(1)

if __name__ == "__main__":
    main()
    