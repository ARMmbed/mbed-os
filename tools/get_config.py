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

"""
from __future__ import print_function
import sys
from os.path import abspath, dirname, join

# Be sure that the tools directory is in the search path
ROOT = abspath(join(dirname(__file__), ".."))
sys.path.insert(0, ROOT)

from tools.utils import args_error
from tools.options import get_default_options_parser
from tools.options import extract_mcus
from tools.build_api import get_config
from tools.config import Config
from tools.utils import argparse_filestring_type

if __name__ == '__main__':
    # Parse Options
    parser = get_default_options_parser(add_clean=False, add_options=False,
                                        add_app_config=True)
    parser.add_argument(
        "--source", dest="source_dir", type=argparse_filestring_type,
        required=True, default=[], help="The source (input) directory",
        action="append")
    parser.add_argument(
        "--prefix", dest="prefix", action="append", default=[],
        help="Restrict listing to parameters that have this prefix")
    parser.add_argument(
        "-v", "--verbose", action="store_true", dest="verbose", default=False,
        help="Verbose diagnostic output")

    options = parser.parse_args()

    # Target
    if options.mcu is None :
        args_error(parser, "argument -m/--mcu is required")
    target = extract_mcus(parser, options)[0]

    options.prefix = options.prefix or [""]

    try:
        params, macros, features = get_config(
            options.source_dir,
            target,
            options.tool[0] if options.tool else None,
            app_config=options.app_config
        )
        if not params and not macros:
            print("No configuration data available.")
            sys.exit(0)
        if params:
            print("Configuration parameters")
            print("------------------------")
            for p in sorted(list(params.keys())):
                if any(p.startswith(s) for s in options.prefix):
                    if options.verbose:
                        print(params[p].get_verbose_description())
                    else:
                        print(str(params[p]))
            print("")

        print("Macros")
        print("------")
        for m in Config.config_macros_to_macros(macros):
            if any(m.startswith(s) for s in options.prefix):
                print(m)

    except KeyboardInterrupt as e:
        print("\n[CTRL+c] exit")
    except Exception as e:
        if options.verbose:
            import traceback
            traceback.print_exc(file=sys.stdout)
        else:
            print("[ERROR] %s" % str(e))

        sys.exit(1)
