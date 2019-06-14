"""
Copyright (c) 2017-2019 ARM Limited. All rights reserved.

SPDX-License-Identifier: Apache-2.0

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations
"""

"""
This script make easier to analyse which targets are filled in which test phase
"""

import json
from prettytable import PrettyTable
import argparse


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", "-c", default="examples.json", help="Configuration file to be analysed")
    parser.add_argument("--test-phase", "-p", default="pull_request_test", help="Test phase to be analysed")
    parser.add_argument("--targets", "-t", default=None, help="Targets to analyse in csv format")

    return parser


def main(args):
    configuration = json.loads(open(args.config).read())

    all_targets = list()
    for x in configuration["examples"]:
        all_targets.extend(x[args.test_phase]['targets'])
    all_targets = list(set(all_targets))

    if args.targets is not None:
        filter_targets = args.targets.split(',')
        all_targets.extend(filter_targets)
        all_targets = list(set(all_targets))
        all_targets = list(filter(lambda x: x in filter_targets, all_targets))

    all_targets = sorted(all_targets)
    x = PrettyTable()
    x.field_names = ["Example"] + all_targets + ["Sum"]

    for example in list(configuration["examples"]):
        row = [example["name"]]
        count = 0
        targets = example[args.test_phase]['targets']
        if type(targets) == list:
            # Empty list means that allowed everywhere
            if len(targets) == 0:
                row.extend(['x'] * len(all_targets))
                count = len(all_targets)
            else:
                for compare_target in all_targets:
                    if compare_target in targets:
                        row.append("x")
                        count += 1
                    else:
                        row.append(" ")
        row.append(count)
        x.add_row(row)

    # Print table
    print(x)


if __name__ == "__main__":
    parser = get_parser()
    main(parser.parse_args())
