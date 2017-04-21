"""A linting utility for targets.json"""

from os.path import join, abspath, dirname
if __name__ == "__main__":
    import sys
    ROOT = abspath(join(dirname(__file__), "..", ".."))
    sys.path.insert(0, ROOT)
from copy import copy
from yaml import dump
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


MCU_REQUIRED_KEYS = ["release_versions", "supported_toolchains",
                     "default_lib", "public", "inherits"]
MCU_ALLOWED_KEYS = ["device_has", "core", "extra_labels", "features",
                    "bootloader_supported", "device_name", "post_binary_hook",
                    "default_toolchain"] + MCU_REQUIRED_KEYS
def check_mcu(mcu_json, strict=False):
    """Generate a list of problems with an MCU

    :param: mcu_json the MCU's dict to check
    :param: strict enforce required keys
    """
    if strict:
        for err in must_have_keys(MCU_REQUIRED_KEYS, mcu_json):
            yield err
    for err in may_have_keys(MCU_ALLOWED_KEYS, mcu_json):
        yield err
    if 'public' in mcu_json and mcu_json['public']:
        yield "public must be false"
    if  ("release_versions" in mcu_json and
         "5" in mcu_json["release_versions"] and
         "supported_toolchains" in mcu_json):
        for toolc in ["GCC_ARM", "ARM", "IAR"]:
            if toolc not in mcu_json["supported_toolchains"]:
                yield ("%s not found in supported_toolchains, and is "
                       "required by mbed OS 5" % toolc)

BOARD_REQUIRED_KEYS = ["inherits"]
BOARD_ALLOWED_KEYS = ["supported_form_factors", "is_disk_virtual",
                      "detect_code", "device_name", "extra_labels",
                      "public"] + BOARD_REQUIRED_KEYS
def check_board(board_json, strict=False):
    """Generate a list of problems with an board

    :param: board_json the mcus dict to check
    :param: strict enforce required keys
    """
    if strict:
        for err in must_have_keys(BOARD_REQUIRED_KEYS, board_json):
            yield err
    for err in may_have_keys(BOARD_ALLOWED_KEYS, board_json):
        yield err


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
        global_errors.append("No name for targets: %s" % mcus[3:])
        mcus_string = MCU_FORMAT_STRING[3] % tuple(mcus[:3])
        for name in mcus[3:]:
            mcus_string += " ??? (%s) ->" % name
    else:
        mcus_string = MCU_FORMAT_STRING[len(mcus)] % tuple(mcus)

    if len(boards) < 1:
        global_errors.append("no boards found in heirarchy")
        boards_string = "???"
    elif len(boards) > 2:
        global_errors.append("no name for targets: %s" % boards[2:])
        boards_string = BOARD_FORMAT_STRING[3] % tuple(boards[:2])
        for name in boards[2:]:
            boards_string += " ??? (%s)" % name
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
    add_if(to_ret, "hierachy errors", hierachy_errors)

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


def main():
    """entry point"""
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("mcu", choices=TARGET_MAP.keys(), metavar="MCU", )
    options = parser.parse_args()
    print dump(check_hierarchy(TARGET_MAP[options.mcu]),
               default_flow_style=False)
    return 0

if __name__ == "__main__":
    sys.exit(main())

