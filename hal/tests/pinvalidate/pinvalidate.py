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

import argparse
import json
import pathlib
import hashlib
import re
import sys
from tabulate import tabulate
from itertools import chain
from enum import Enum


class ReturnCode(Enum):
    """Return codes."""

    SUCCESS = 0
    ERROR = 1
    INVALID_OPTIONS = 2


class TestCaseError(Exception):
    """An exception for test case failure."""


class ArgumentParserWithDefaultHelp(argparse.ArgumentParser):
    """Subclass that always shows the help message on invalid arguments."""

    def error(self, message):
        """Error handler."""
        sys.stderr.write("error: {}\n".format(message))
        self.print_help()
        raise SystemExit(ReturnCode.INVALID_OPTIONS.value)

def find_target_by_path(target_path):
    """Find a target by path."""
    mbed_os_root = pathlib.Path(__file__).absolute().parents[3]

    targets = dict()

    with open(target_path) as pin_names_file:
        pin_names_file_content = pin_names_file.read()
    
    target_list_match = re.search(
        "\/* MBED TARGET LIST: ([0-9A-Z_,* \n]+)*\/",
        pin_names_file_content
    )

    target_list = []
    if target_list_match:
        target_list = list(
            re.findall(
                r"([0-9A-Z_]{3,})",
                target_list_match.group(1),
                re.MULTILINE,
            )
        )
    
    if not target_list:
        print("WARNING: MBED TARGET LIST marker invalid or not found in file " + target_path)
        print("Target could not be determined. Only the generic test suite will run. You can manually specify additional suites.")

    with (
        mbed_os_root.joinpath("targets", "targets.json")
    ).open() as targets_json_file:
        target_data = json.load(targets_json_file)

        # find target in targets.json
        for target in target_data:
            if "public" in target_data[target]:
                if not target_data[target]["public"]:
                    continue

            if target in target_list:
                targets[target] = target_path

    if len(targets) == 0:
        targets[target_path] = target_path

    return targets

def find_target_by_name(target_name=""):
    """Find a target by name."""
    mbed_os_root = pathlib.Path(__file__).absolute().parents[3]
    
    targets = dict()

    for f in mbed_os_root.joinpath('targets').rglob("PinNames.h"):
        with open(f) as pin_names_file:
            pin_names_file_content = pin_names_file.read()
        
        target_list_match = re.search(
            "\/* MBED TARGET LIST: ([0-9A-Z_,* \n]+)*\/",
            pin_names_file_content
        )
        
        target_list = []
        if target_list_match:
            target_list = list(
                re.findall(
                    r"([0-9A-Z_]{3,})",
                    target_list_match.group(1),
                    re.MULTILINE,
                )
            )

        if target_name:
            if target_name in target_list:
                targets[target_name] = f
                break
        else:
            for target in target_list:
                targets[target] = f
    
    return targets


def check_markers(test_mode=False):
    """Validate markers in PinNames.h files"""
    mbed_os_root = pathlib.Path(__file__).absolute().parents[3]

    errors = []

    with (
        mbed_os_root.joinpath("targets", "targets.json")
    ).open() as targets_json_file:
        targets_json = json.load(targets_json_file)

    if test_mode:
        search_dir = pathlib.Path(__file__).parent.joinpath('test_files').absolute()
    else:
        search_dir = mbed_os_root.joinpath('targets')

    for f in search_dir.rglob("PinNames.h"):
        with open(f) as pin_names_file:
            pin_names_file_content = pin_names_file.read()
        
        target_list_match = re.search(
            "\/* MBED TARGET LIST: ([0-9A-Z_,* \n]+)*\/",
            pin_names_file_content
        )
        
        marker_target_list = []
        if target_list_match:
            marker_target_list = list(
                re.findall(
                    r"([0-9A-Z_]{3,})",
                    target_list_match.group(1),
                    re.MULTILINE,
                )
            )
        
        if not marker_target_list:
            print("WARNING: MBED TARGET LIST marker invalid or not found in file " + str(f))
            errors.append({ "file": str(f), "error": "marker invalid or not found"})
            continue

        for target in marker_target_list:
            target_is_valid = False
            if target in targets_json:
                target_is_valid = True
                if "public" in targets_json[target]:
                    if targets_json[target]["public"] == False:
                        target_is_valid = False
            if not target_is_valid:
                print("WARNING: MBED TARGET LIST in file " + str(f) + " includes target '" + target + "' which doesn't exist in targets.json or is not public")
                errors.append({ "file": str(f), "error": "target not found"})
    
    return errors


def check_duplicate_pinnames_files(test_mode=False):
    """Check for duplicate PinNames.h files"""
    mbed_os_root = pathlib.Path(__file__).absolute().parents[3]

    errors = []
    
    file_hash_dict = dict()

    if test_mode:
        search_dir = pathlib.Path(__file__).parent.joinpath('test_files').absolute()
    else:
        search_dir = mbed_os_root.joinpath('targets')

    for f in search_dir.rglob("PinNames.h"):
        with open(f) as pin_names_file:
            pin_names_file_content = pin_names_file.read()
        file_hash_dict[str(f)] = hashlib.md5(pin_names_file_content.encode('utf-8')).hexdigest()
    
    rev_dict = {} 
    for key, value in file_hash_dict.items(): 
        rev_dict.setdefault(value, set()).add(key) 
    duplicates = [key for key, values in rev_dict.items() 
                                if len(values) > 1] 
    
    for duplicate in duplicates:
        print("WARNING: Duplicate files")
        for file_path, file_hash in file_hash_dict.items():
            if file_hash == duplicate:
                errors.append({ "file": file_path, "error": "duplicate file"})
                print("\t" + file_path)
    
    return errors

def check_duplicate_markers(test_mode=False):
    """Check target markers in PinNames.h files for duplicates."""
    mbed_os_root = pathlib.Path(__file__).absolute().parents[3]

    errors = []

    markers = dict()
    
    if test_mode:
        search_dir = pathlib.Path(__file__).parent.joinpath('test_files').absolute()
    else:
        search_dir = mbed_os_root.joinpath('targets')

    for f in search_dir.rglob("PinNames.h"):
        with open(f) as pin_names_file:
            pin_names_file_content = pin_names_file.read()
        
        target_list_match = re.search(
            "\/* MBED TARGET LIST: ([0-9A-Z_,* \n]+)*\/",
            pin_names_file_content
        )
        
        marker_target_list = []
        if target_list_match:
            marker_target_list = list(
                re.findall(
                    r"([0-9A-Z_]{3,})",
                    target_list_match.group(1),
                    re.MULTILINE,
                )
            )
        
        for target in marker_target_list:
            if target in markers:
                print("WARNING: target duplicate in " + str(f) + ", " + target + " first listed in " + markers[target])
                errors.append({ "file": str(f), "error": "duplicate marker"})
            else:
                markers[target] = str(f)

    return errors


def target_has_arduino_form_factor(target_name):
    """Check if the target has the Arduino form factor."""
    mbed_os_root = pathlib.Path(__file__).absolute().parents[3]

    with (
        mbed_os_root.joinpath("targets", "targets.json")
    ).open() as targets_json_file:
        target_data = json.load(targets_json_file)

    if target_name in target_data:
        if "supported_form_factors" in target_data[target_name]:
            form_factors = target_data[target_name]["supported_form_factors"]
            if "ARDUINO_UNO" in form_factors:
                return True

    return False


def pin_name_to_dict(pin_name_file_content):
    pin_name_enum_dict = dict()
    pin_name_enum_match = re.search(
        "typedef enum {\n([^}]*)\n} PinName;", pin_name_file_content
    )
    if pin_name_enum_match:
        pin_name_enum_body = pin_name_enum_match.group(1)
        pin_name_enum_dict = dict(
            re.findall(
                r"^\s*([a-zA-Z0-9_]+)\s*=\s*([a-zA-Z0-9_]+)",
                pin_name_enum_body,
                re.MULTILINE,
            )
        )

    pin_name_define_dict = dict(
        re.findall(
            r"^#define\s+([a-zA-Z0-9_]+)\s+([a-zA-Z0-9_]+)",
            pin_name_file_content,
            re.MULTILINE,
        )
    )

    return {**pin_name_enum_dict, **pin_name_define_dict}


def identity_assignment_check(pin_name_dict):
    invalid_items = []
    for key, val in pin_name_dict.items():
        if val == key:
            message = "cannot assign value to itself"
            invalid_items.append({"key": key, "val": val, "message": message})
    return invalid_items


def nc_assignment_check(pin_name_dict):
    invalid_items = []
    for key, val in pin_name_dict.items():
        if re.match(r"^((LED|BUTTON)\d*|CONSOLE_TX|CONSOLE_RX|USBTX|USBRX)$", key):
            if val == "NC":
                message = "cannot be NC"
                invalid_items.append(
                    {"key": key, "val": val, "message": message}
                )
    return invalid_items


def duplicate_assignment_check(pin_name_dict):
    used_pins = []
    used_pins_friendly = []
    invalid_items = []

    for key, val in pin_name_dict.items():
        if re.match(r"^((LED|BUTTON)\d*|CONSOLE_TX|CONSOLE_RX|USBTX|USBRX)$", key):
            if val == "NC":
                continue
            # resolve to literal
            realval = val
            depth = 0
            while not re.match(
                "(0x[0-9a-fA-F]+|[1-9][0-9]*|0[1-7][0-7]+|0b[01]+)[uUlL]{0,2}",
                realval,
            ):
                try:
                    realval = pin_name_dict[realval]
                    depth += 1
                except KeyError:
                    break
                if depth > 10:
                    break

            if realval in used_pins:
                message = (
                    "already assigned to "
                    + used_pins_friendly[used_pins.index(realval)]
                )
                invalid_items.append(
                    {"key": key, "val": val, "message": message}
                )
                continue

            used_pins.append(realval)
            used_pins_friendly.append(key + " = " + val)
    return invalid_items


def arduino_duplicate_assignment_check(pin_name_dict):
    used_pins = []
    used_pins_friendly = []
    invalid_items = []

    for key, val in pin_name_dict.items():
        if re.match(r"^ARDUINO_UNO_[AD]\d+$", key):
            if val == "NC":
                continue
            # resolve to literal
            realval = val
            depth = 0
            while not re.match(
                "(0x[0-9a-fA-F]+|[1-9][0-9]*|0[1-7][0-7]+|0b[01]+)[uUlL]{0,2}",
                realval,
            ):
                try:
                    realval = pin_name_dict[realval]
                    depth += 1
                except KeyError:
                    break
                if depth > 10:
                    break

            if realval in used_pins:
                message = (
                    "already assigned to "
                    + used_pins_friendly[used_pins.index(realval)]
                )
                invalid_items.append(
                    {"key": key, "val": val, "message": message}
                )
                continue

            used_pins.append(realval)
            used_pins_friendly.append(key + " = " + val)
    return invalid_items


def arduino_existence_check(pin_name_dict):
    analog_pins = (f"ARDUINO_UNO_A{i}" for i in range(6))
    digital_pins = (f"ARDUINO_UNO_D{i}" for i in range(16))

    return [
        {"key": pin, "val": "", "message": pin + " not defined"}
        for pin in chain(analog_pins, digital_pins)
        if pin not in pin_name_dict
    ]


def arduino_nc_assignment_check(pin_name_dict):
    invalid_items = []
    for key, val in pin_name_dict.items():
        if re.match(r"^ARDUINO_UNO_[AD]\d+$", key):
            if val == "NC":
                message = "cannot be NC"
                invalid_items.append(
                    {"key": key, "val": val, "message": message}
                )
    return invalid_items


def legacy_assignment_check(pin_name_content):
    invalid_items = []
    legacy_assignments = dict(
        re.findall(
            r"^\s*((?:LED|BUTTON)\d*)\s*=\s*([a-zA-Z0-9_]+)",
            pin_name_content,
            re.MULTILINE,
        )
    )
    for key, val in legacy_assignments.items():
        message = "legacy assignment; LEDs and BUTTONs must be #define'd"
        invalid_items.append({"key": key, "val": val, "message": message})
    return invalid_items

def legacy_uart_check(pin_name_dict):
    invalid_items = []
    if "CONSOLE_TX" not in pin_name_dict or "CONSOLE_RX" not in pin_name_dict:
        message = "CONSOLE_TX or CONSOLE_RX are not defined; USBTX and USBRX are deprecated"
        invalid_items.append({"key": "", "val": "", "message": message})
    return invalid_items


def print_summary(report):
    targets = set([case["platform_name"] for case in report])

    table = []
    for target in targets:
        error_count = 0

        for case in report:
            if (
                case["platform_name"] == target
                and case["result"] == "FAILED"
            ):
                error_count += 1

        table.append(
            [target, "FAILED" if error_count else "PASSED", error_count]
        )

    return tabulate(
        table,
        headers=["Platform name", "Result", "Error count"],
        tablefmt="grid",
    )


def print_suite_summary(report):
    targets = set([case["platform_name"] for case in report])

    table = []
    for target in targets:
        suites = set([
            case["suite_name"]
            for case in report
            if case["platform_name"] == target
        ])

        for suite in suites:
            result = "PASSED"
            error_count = 0
            for case in report:
                if case["platform_name"] != target:
                    continue
                if case["suite_name"] != suite:
                    continue
                if case["result"] == "FAILED":
                    result = "FAILED"
                    error_count += 1
            table.append([target, suite, result, error_count])

    return tabulate(
        table,
        headers=["Platform name", "Test suite", "Result", "Error count"],
        tablefmt="grid",
    )


def print_report(report, print_error_detail, tablefmt="grid"):
    table = []
    for case in report:
        errors_str = []
        for error in case["errors"]:
            errors_str.append("\n")
            if error["key"]:
                errors_str.append(error["key"])
            if error["val"]:
                errors_str.append(" = ")
                errors_str.append(error["val"])
            if error["message"]:
                errors_str.append(" <-- ")
                errors_str.append(error["message"])

        if not errors_str:
            errors_str = "None"

        if print_error_detail:
            table.append(
                (
                    case["platform_name"],
                    case["suite_name"],
                    case["case_name"],
                    case["result"],
                    len(case["errors"]),
                    "None" if not errors_str else "".join(errors_str).lstrip(),
                )
            )
        else:
            table.append(
                (
                    case["platform_name"],
                    case["suite_name"],
                    case["case_name"],
                    case["result"],
                    len(case["errors"]),
                )
            )

    return tabulate(
        table,
        headers=[
            "Platform name",
            "Test suite",
            "Test case",
            "Result",
            "Error count",
            "Errors",
        ],
        tablefmt=tablefmt,
    )


def print_pretty_html_report(report):
    output = []
    output.append("<html><head><style>table, td, tr { border: 2px solid black; border-collapse: collapse; padding: 5px; font-family: Helvetica, serif; }</style></head>")
    output.append('<body><p><button onclick=\'e=document.getElementsByTagName("details");for(var i=0;i<e.length;i++){e[i].setAttribute("open","true")};\'>Expand all errors</button></p><table>')
    output.append("<tr><th>Platform name</th><th>Test suite</th><th>Test case</th><th>Result</th><th>Error count</th><th>Errors</th></tr>")

    for case in report:
        output.append("<tr>")

        if case["errors"]:
            error_details = ["<details><summary>View errors</summary><table>"]
            for error in case["errors"]:
                error_details.append("<tr>")
                if error["key"]:
                    error_details.append("<td>")
                    error_details.append(error["key"])
                    error_details.append("</td>")
                if error["val"]:
                    error_details.append("<td>")
                    error_details.append(error["val"])
                    error_details.append("</td>")
                if error["message"]:
                    error_details.append("<td>")
                    error_details.append(error["message"])
                    error_details.append("</td>")
                error_details.append("</tr>")
            error_details.append("</table></details>")
        else:
            error_details = []

        output.append("<td>")
        output.append(case["platform_name"])
        output.append("</td>")
        output.append("<td>")
        output.append(case["suite_name"])
        output.append("</td>")
        output.append("<td>")
        output.append(case["case_name"])
        output.append("</td>")

        if case["result"] == "PASSED":
            color = "green"
            count_color = "black"
        else:
            color = "red"
            count_color = "red"

        output.append("<td style='color:")
        output.append(color)
        output.append("'>")
        output.append(case["result"])
        output.append("</td>")
        
        output.append("<td style='color:")
        output.append(count_color)
        output.append("'>")
        output.append(str(len(case["errors"])))
        output.append("</td>")
        
        output.append("<td>")
        output.extend(error_details)
        output.append("</td>")

        output.append("</tr>")

    output.append("</table></body></table>")

    return "".join(output)


def has_passed_all_test_cases(report):
    """Check that all test cases passed."""
    for case in report:
        if case["result"] == "FAILED":
            return False

    return True


test_cases = [
    {
        "suite_name": "generic",
        "case_name": "identity",
        "case_function": identity_assignment_check,
        "case_input": "dict",
    },
    {
        "suite_name": "generic",
        "case_name": "nc",
        "case_function": nc_assignment_check,
        "case_input": "dict",
    },
    {
        "suite_name": "generic",
        "case_name": "duplicate",
        "case_function": duplicate_assignment_check,
        "case_input": "dict",
    },
    {
        "suite_name": "generic",
        "case_name": "legacy",
        "case_function": legacy_assignment_check,
        "case_input": "content",
    },
    {
        "suite_name": "generic",
        "case_name": "uart",
        "case_function": legacy_uart_check,
        "case_input": "content",
    },
    {
        "suite_name": "arduino_uno",
        "case_name": "duplicate",
        "case_function": arduino_duplicate_assignment_check,
        "case_input": "dict",
    },
    {
        "suite_name": "arduino_uno",
        "case_name": "existence",
        "case_function": arduino_existence_check,
        "case_input": "dict",
    },
    {
        "suite_name": "arduino_uno",
        "case_name": "nc",
        "case_function": arduino_nc_assignment_check,
        "case_input": "dict",
    },
]


def validate_pin_names(args):
    """Entry point for validating the Pin names."""
    suites = []
    if args.suite_names:
        suites = args.suite_names.split(",")

    targets = dict()
    if args.paths:
        paths = args.paths.split(",")
        for path in paths:
            targets = {**targets, **find_target_by_path(path)}
    elif args.targets:
        target_names = args.targets.split(",")
        for target_name in target_names:
            targets = {**targets, **find_target_by_name(target_name)}
    elif args.all:
        targets = find_target_by_name()
    elif args.check_markers:
        check_markers()
        check_duplicate_pinnames_files()
        check_duplicate_markers()
        return

    report = []
    for target, path in targets.items():
        pin_name_content = open(path).read()

        pin_name_dict = pin_name_to_dict(pin_name_content)

        arduino_support = target_has_arduino_form_factor(target)

        for case in test_cases:
            if suites:
                if case["suite_name"] not in suites:
                    continue
            else:
                if not arduino_support and case["suite_name"] == "arduino_uno":
                    continue

            if case["case_input"] == "dict":
                case_input = pin_name_dict
            elif case["case_input"] == "content":
                case_input = pin_name_content

            case_output = case["case_function"](case_input)

            case_result = "FAILED" if case_output else "PASSED"

            platform_name = target
            if not args.full_name and args.output_format == "prettytext":
                if len(platform_name) > 40:
                    platform_name = "..." + platform_name[-40:]

            report.append(
                {
                    "platform_name": platform_name,
                    "suite_name": case["suite_name"],
                    "case_name": case["case_name"],
                    "result": case_result,
                    "errors": case_output,
                }
            )

    generate_output(report, args.output_format, args.verbose, args.output_file)

    if not has_passed_all_test_cases(report):
        raise TestCaseError("One or more test cases failed")


def generate_output(report, output_format, verbosity, output_file):
    """Generate the output."""
    if output_format == "json":
        output = json.dumps(report)
    elif output_format == "html":
        output = print_pretty_html_report(report)
    else:
        if verbosity == 0:
            output = print_summary(report)
        elif verbosity == 1:
            output = print_suite_summary(report)
        elif verbosity == 2:
            output = print_report(report, False)
        elif verbosity > 2:
            output = print_report(report, True)

    if output_file:
        with open(output_file, "w") as out_file:
            out_file.write(output)
    else:
        print(output)


def parse_args():
    """Parse the command line args."""
    parser = ArgumentParserWithDefaultHelp(
        description="Pin names validation",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )

    parser.add_argument(
        "-n",
        "--suite_names",
        help="Run specific test suite. Use comma to seperate multiple suites.",
    )

    parser.add_argument(
        "-v",
        "--verbose",
        action="count",
        default=0,
        help=(
            "Verbosity of the report (none to -vvv)."
            " Only applies for 'prettytext' output format."
        ),
    )

    parser.add_argument(
        "-f",
        "--full_name",
        action="store_true",
        help=(
            "Don't truncate long platform names in"
            " human-readable output formats"
        ),
    )

    parser.add_argument(
        "-o",
        "--output_format",
        default="prettytext",
        help="Set the output format: prettytext (default), json or html",
    )

    parser.add_argument(
        "-w",
        "--output_file",
        help="File to write output to, instead of printing to stdout",
    )

    group = parser.add_mutually_exclusive_group(required=True)

    group.add_argument(
        "-t",
        "--targets",
        help=(
            "Target name. Use comma to seperate multiple targets."
            "THIS FEATURE IS EXPERIMENTAL!"
        ),
    )

    group.add_argument(
        "-p",
        "--paths",
        help="Path to PinNames.h file. Use comma to seperate multiple paths.",
    )

    group.add_argument(
        "-a", "--all", action="store_true", help="Run tests on all targets."
    )

    group.add_argument(
        "-m",
        "--check-markers",
        action="store_true",
        help="Check all PinNames.h for the MBED TARGET LIST marker."
    )

    parser.set_defaults(func=validate_pin_names)

    args_namespace = parser.parse_args()

    # We want to fail gracefully, with a consistent
    # help message, in the no argument case.
    # So here's an obligatory hasattr hack.
    if not hasattr(args_namespace, "func"):
        parser.error("No arguments given!")
    else:
        return args_namespace


def run_pin_validate():
    """Application main algorithm."""
    args = parse_args()
    args.func(args)


def _main():
    """Run pinvalidate."""
    try:
        run_pin_validate()
    except Exception as error:
        print(error)
        return ReturnCode.ERROR.value
    else:
        return ReturnCode.SUCCESS.value


if __name__ == "__main__":
    sys.exit(_main())
