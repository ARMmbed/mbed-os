"""A linting utility for targets.json

This linting utility may be called as follows:
python <path-to>/lint.py targets TARGET [TARGET ...]

all targets will be linted
"""

# mbed SDK
# Copyright (c) 2017 ARM Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from os.path import join, abspath, dirname
if __name__ == "__main__":
    import sys
    ROOT = abspath(join(dirname(__file__), "..", ".."))
    sys.path.insert(0, ROOT)
from copy import copy
from yaml import dump_all
import argparse

from tools.targets import Target, set_targets_json_location, TARGET_MAP

def must_have_keys(keys, dict):
    """Require keys in an MCU/Board

    is a generator for errors
    """
    for key in keys:
        if key not in dict:
            yield "%s not found, and is required" % key

def may_have_keys(keys, dict):
    """Disable all other keys in an MCU/Board

    is a generator for errors
    """
    for key in dict.keys():
        if key not in keys:
            yield "%s found, and is not allowed" % key

def check_extra_labels(dict):
    """Check that extra_labels does not contain any Target names

    is a generator for errors
    """
    for label in (dict.get("extra_labels", []) +
                  dict.get("extra_labels_add", [])):
        if label in Target.get_json_target_data():
            yield "%s is not allowed in extra_labels" % label

def check_release_version(dict):
    """Verify that release version 5 is combined with support for all toolcahins

    is a generator for errors
    """
    if  ("release_versions" in dict and
         "5" in dict["release_versions"] and
         "supported_toolchains" in dict):
        for toolc in ["GCC_ARM", "ARM", "IAR"]:
            if toolc not in dict["supported_toolchains"]:
                yield ("%s not found in supported_toolchains, and is "
                       "required by mbed OS 5" % toolc)

def check_inherits(dict):
    if  ("inherits" in dict and len(dict["inherits"]) > 1):
        yield "multiple inheritance is forbidden"

DEVICE_HAS_ALLOWED = ["ANALOGIN", "ANALOGOUT", "CAN", "ETHERNET", "EMAC",
                      "FLASH", "I2C", "I2CSLAVE", "I2C_ASYNCH", "INTERRUPTIN",
                      "LOWPOWERTIMER", "PORTIN", "PORTINOUT", "PORTOUT",
                      "PWMOUT", "RTC", "TRNG","SERIAL", "SERIAL_ASYNCH",
                      "SERIAL_FC", "SLEEP", "SPI", "SPI_ASYNCH", "SPISLAVE",
                      "STORAGE", "STCLK_OFF_DURING_SLEEP"]
def check_device_has(dict):
    for name in dict.get("device_has", []):
        if name not in DEVICE_HAS_ALLOWED:
            yield "%s is not allowed in device_has" % name

MCU_REQUIRED_KEYS = ["release_versions", "supported_toolchains",
                     "default_lib", "public", "inherits", "device_has"]
MCU_ALLOWED_KEYS = ["device_has_add", "device_has_remove", "core",
                    "extra_labels", "features", "features_add",
                    "features_remove", "bootloader_supported", "device_name",
                    "post_binary_hook", "default_toolchain", "config",
                    "extra_labels_add", "extra_labels_remove",
                    "target_overrides"] + MCU_REQUIRED_KEYS
def check_mcu(mcu_json, strict=False):
    """Generate a list of problems with an MCU

    :param: mcu_json the MCU's dict to check
    :param: strict enforce required keys
    """
    errors = list(may_have_keys(MCU_ALLOWED_KEYS, mcu_json))
    if strict:
        errors.extend(must_have_keys(MCU_REQUIRED_KEYS, mcu_json))
    errors.extend(check_extra_labels(mcu_json))
    errors.extend(check_release_version(mcu_json))
    errors.extend(check_inherits(mcu_json))
    errors.extend(check_device_has(mcu_json))
    if 'public' in mcu_json and mcu_json['public']:
        errors.append("public must be false")
    return errors

BOARD_REQUIRED_KEYS = ["inherits"]
BOARD_ALLOWED_KEYS = ["supported_form_factors", "is_disk_virtual",
                      "detect_code", "extra_labels", "extra_labels_add",
                      "extra_labels_remove", "public", "config",
                      "forced_reset_timeout", "target_overrides"] + BOARD_REQUIRED_KEYS
def check_board(board_json, strict=False):
    """Generate a list of problems with an board

    :param: board_json the mcus dict to check
    :param: strict enforce required keys
    """
    errors = list(may_have_keys(BOARD_ALLOWED_KEYS, board_json))
    if strict:
        errors.extend(must_have_keys(BOARD_REQUIRED_KEYS, board_json))
    errors.extend(check_extra_labels(board_json))
    errors.extend(check_inherits(board_json))
    return errors

def add_if(dict, key, val):
    """Add a value to a dict if it's non-empty"""
    if val:
        dict[key] = val

def _split_boards(resolution_order, tgt):
    """Split the resolution order between boards and mcus"""
    mcus = []
    boards = []
    iterable = iter(resolution_order)
    for name in iterable:
        mcu_json = tgt.json_data[name]
        if  (len(list(check_mcu(mcu_json, True))) >
             len(list(check_board(mcu_json, True)))):
            boards.append(name)
        else:
            mcus.append(name)
            break
    mcus.extend(iterable)
    mcus.reverse()
    boards.reverse()
    return mcus, boards


MCU_FORMAT_STRING = {1: "MCU (%s) ->",
                     2: "Family (%s) -> MCU (%s) ->",
                     3: "Family (%s) -> SubFamily (%s) -> MCU (%s) ->"}
BOARD_FORMAT_STRING = {1: "Board (%s)",
                       2: "Module (%s) -> Board (%s)"}
def _generate_hierarchy_string(mcus, boards):
    global_errors = []
    if len(mcus) < 1:
        global_errors.append("No MCUS found in heirarchy")
        mcus_string = "??? ->"
    elif len(mcus) > 3:
        global_errors.append("No name for targets %s" % ", ".join(mcus[3:]))
        mcus_string = MCU_FORMAT_STRING[3] % tuple(mcus[:3])
        for name in mcus[3:]:
            mcus_string += " ??? (%s) ->" % name
    else:
        mcus_string = MCU_FORMAT_STRING[len(mcus)] % tuple(mcus)

    if len(boards) < 1:
        global_errors.append("no boards found in heirarchy")
        boards_string = "???"
    elif len(boards) > 2:
        global_errors.append("no name for targets %s" % ", ".join(boards[2:]))
        boards_string = BOARD_FORMAT_STRING[2] % tuple(boards[:2])
        for name in boards[2:]:
            boards_string += " -> ??? (%s)" % name
    else:
        boards_string = BOARD_FORMAT_STRING[len(boards)] % tuple(boards)
    return mcus_string + " " + boards_string, global_errors


def check_hierarchy(tgt):
    """Atempts to assign labels to the heirarchy"""
    resolution_order = copy(tgt.resolution_order_names[:-1])
    mcus, boards = _split_boards(resolution_order, tgt)

    target_errors = {}
    hierachy_string, hierachy_errors = _generate_hierarchy_string(mcus, boards)
    to_ret = {"hierarchy": hierachy_string}
    add_if(to_ret, "hierarchy errors", hierachy_errors)

    for name in mcus[:-1]:
        add_if(target_errors, name, list(check_mcu(tgt.json_data[name])))
    if len(mcus) >= 1:
        add_if(target_errors, mcus[-1],
               list(check_mcu(tgt.json_data[mcus[-1]], True)))
    for name in boards:
        add_if(target_errors, name, list(check_board(tgt.json_data[name])))
    if len(boards) >= 1:
        add_if(target_errors, boards[-1],
               list(check_board(tgt.json_data[boards[-1]], True)))
    add_if(to_ret, "target errors", target_errors)
    return to_ret

PARSER = argparse.ArgumentParser(prog="targets/lint.py")
SUBPARSERS = PARSER.add_subparsers(title="Commands")

def subcommand(name, *args, **kwargs):
    def __subcommand(command):
        kwargs['description'] = command.__doc__
        subparser = SUBPARSERS.add_parser(name, **kwargs)
        for arg in args:
            arg = dict(arg)
            opt = arg['name']
            del arg['name']

            if isinstance(opt, basestring):
                subparser.add_argument(opt, **arg)
            else:
                subparser.add_argument(*opt, **arg)

        def _thunk(parsed_args):
            argv = [arg['dest'] if 'dest' in arg else arg['name']
                    for arg in args]
            argv = [(arg if isinstance(arg, basestring)
                     else arg[-1]).strip('-').replace('-', '_')
                    for arg in argv]
            argv = {arg: vars(parsed_args)[arg] for arg in argv
                    if vars(parsed_args)[arg] is not None}

            return command(**argv)

        subparser.set_defaults(command=_thunk)
        return command
    return __subcommand

@subcommand("targets",
            dict(name="mcus", nargs="+", metavar="MCU",
                 choices=TARGET_MAP.keys(), type=str.upper))
def targets_cmd(mcus=[]):
    """Find and print errors about specific targets"""
    print dump_all([check_hierarchy(TARGET_MAP[m]) for m in mcus],
                   default_flow_style=False)

@subcommand("all-targets")
def all_targets_cmd():
    """Print all errors about all parts"""
    print dump_all([check_hierarchy(m) for m in TARGET_MAP.values()],
                   default_flow_style=False)

@subcommand("orphans")
def orphans_cmd():
    """Find and print all orphan targets"""
    orphans = Target.get_json_target_data().keys()
    for tgt in TARGET_MAP.values():
        for name in tgt.resolution_order_names:
            if name in orphans:
                orphans.remove(name)
    if orphans:
        print dump_all([orphans], default_flow_style=False)
    return len(orphans)

def main():
    """entry point"""
    options = PARSER.parse_args()
    return options.command(options)

if __name__ == "__main__":
    sys.exit(main())

