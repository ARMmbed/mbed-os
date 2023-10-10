
"""
mbed SDK
Copyright (c) 2011-2018 ARM Limited

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
import json
import argparse
from collections import defaultdict

# Make sure that any global generic setup is run
from . import lstools_base
from mbed_os_tools.detect.main import (
    create,
    mbed_os_support,
    mbed_lstools_os_info,
    mock_platform
)

import logging
logger = logging.getLogger("mbedls.main")
logger.addHandler(logging.NullHandler())
del logging

def get_version():
    """! Get mbed-ls Python module version string """
    import mbed_os_tools
    return mbed_os_tools.VERSION

def print_version(mbeds, args):
    print(get_version())

def print_mbeds(mbeds, args, simple):
    devices = mbeds.list_mbeds(unique_names=True, read_details_txt=True)
    if devices:
        from prettytable import PrettyTable, HEADER
        columns = ['platform_name', 'platform_name_unique', 'mount_point',
                    'serial_port', 'target_id', 'daplink_version']
        columns_header = ['platform_name', 'platform_name_unique', 'mount_point',
                    'serial_port', 'target_id', 'interface_version']
        pt = PrettyTable(columns_header, junction_char="|", hrules=HEADER)
        pt.align = 'l'
        for d in devices:
            pt.add_row([d.get(col, None) or 'unknown' for col in columns])
        print(pt.get_string(border=not simple, header=not simple,
                            padding_width=1, sortby='platform_name_unique'))

def print_table(mbeds, args):
    return print_mbeds(mbeds, args, False)

def print_simple(mbeds, args):
    return print_mbeds(mbeds, args, True)

def list_platforms(mbeds, args):
    print(mbeds.list_manufacture_ids())

def mbeds_as_json(mbeds, args):
    print(json.dumps(mbeds.list_mbeds(unique_names=True,
                                      read_details_txt=True),
                     indent=4, sort_keys=True))

def json_by_target_id(mbeds, args):
    print(json.dumps({m['target_id']: m for m
                      in mbeds.list_mbeds(unique_names=True,
                                          read_details_txt=True)},
                     indent=4, sort_keys=True))

def json_platforms(mbeds, args):
    platforms = set()
    for d in mbeds.list_mbeds():
        platforms |= set([d['platform_name']])
    print(json.dumps(list(platforms), indent=4, sort_keys=True))

def json_platforms_ext(mbeds, args):
    platforms = defaultdict(lambda: 0)
    for d in mbeds.list_mbeds():
        platforms[d['platform_name']] += 1
    print(json.dumps(platforms, indent=4, sort_keys=True))

def parse_cli(to_parse):
    """! Parse the command line

    @return Retrun a namespace that contains:
     * command - python function to run
     * skip_retarget - bool indicting to skip retargeting
     * list_unmounted - list boards that are not mounted
     * debug - turn on debug logging
    """
    parser = argparse.ArgumentParser()
    parser.set_defaults(command=print_table)

    commands = parser.add_argument_group('sub commands')\
                     .add_mutually_exclusive_group()
    commands.add_argument(
        '-s', '--simple', dest='command', action='store_const',
        const=print_simple,
        help='list attached targets without column headers and borders')
    commands.add_argument(
        '-j', '--json', dest='command', action='store_const',
        const=mbeds_as_json,
        help='list attached targets with detailed information in JSON format')
    commands.add_argument(
        '-J', '--json-by-target-id', dest='command', action='store_const',
        const=json_by_target_id,
        help='map attached targets from their target ID to their detailed '
        'information in JSON format')
    commands.add_argument(
        '-p', '--json-platforms', dest='command', action='store_const',
        const=json_platforms,
        help='list attached platform names in JSON format.')
    commands.add_argument(
        '-P', '--json-platforms-ext', dest='command', action='store_const',
        const=json_platforms_ext,
        help='map attached platform names to the number of attached boards in '
        'JSON format')
    commands.add_argument(
        '-l', '--list', dest='command', action='store_const',
        const=list_platforms,
        help='list all target IDs and their corresponding platform names '
        'understood by mbed-ls')
    commands.add_argument(
        '--version', dest='command', action='store_const', const=print_version,
        help='print package version and exit')
    commands.add_argument(
        '-m', '--mock', metavar='ID:NAME',
        help='substitute or create a target ID to platform name mapping used'
        'when invoking mbedls in the current directory')

    parser.add_argument(
        '--skip-retarget', dest='skip_retarget', default=False,
        action="store_true",
        help='skip parsing and interpretation of the re-target file,'
        ' `./mbedls.json`')
    parser.add_argument(
        '-u', '--list-unmounted', dest='list_unmounted', default=False,
        action='store_true',
        help='list mbeds, regardless of whether they are mounted or not')
    parser.add_argument(
        '-d', '--debug', dest='debug', default=False, action="store_true",
        help='outputs extra debug information useful when creating issues!')

    args = parser.parse_args(to_parse)
    if args.mock:
        args.command = mock_platform
    return args

def start_logging():
    try:
        import colorlog
        colorlog.basicConfig(
            format='%(log_color)s%(levelname)s%(reset)s:%(name)s:%(message)s')
    except ImportError:
        import logging
        logging.basicConfig()
        del logging

def mbedls_main():
    """! Function used to drive CLI (command line interface) application
    @return Function exits with success code
    """
    start_logging()

    args = parse_cli(sys.argv[1:])

    import logging
    root_logger = logging.getLogger("mbedls")
    if args.debug:
        root_logger.setLevel(logging.DEBUG)
    else:
        root_logger.setLevel(logging.INFO)
    del logging
    logger.debug("mbed-ls ver. %s", get_version())
    logger.debug("host: %s",  str(mbed_lstools_os_info()))

    mbeds = create(skip_retarget=args.skip_retarget,
                   list_unmounted=args.list_unmounted,
                   force_mock=args.command is mock_platform)

    if mbeds is None:
        logger.critical('This platform is not supported! Pull requests welcome at github.com/ARMmbed/mbed-ls')
        sys.exit(1)

    ret_code = args.command(mbeds, args)
    if not ret_code:
        ret_code = 0

    logger.debug("Return code: %d", ret_code)

    sys.exit(ret_code)
