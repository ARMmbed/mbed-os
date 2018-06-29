"""
mbed SDK
Copyright (c) 2011-2016 ARM Limited

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

import os
import binascii
import struct
import shutil
import inspect
import sys
from copy import copy
from inspect import getmro
from collections import namedtuple, Mapping
from tools.targets.LPC import patch
from tools.paths import TOOLS_BOOTLOADERS
from tools.utils import json_file_to_dict

__all__ = ["target", "TARGETS", "TARGET_MAP", "TARGET_NAMES", "CORE_LABELS",
           "HookError", "generate_py_target", "Target",
           "CUMULATIVE_ATTRIBUTES", "get_resolution_order"]

CORE_LABELS = {
   "Cortex-M0" : ["M0", "CORTEX_M", "LIKE_CORTEX_M0", "CORTEX"],
   "Cortex-M0+": ["M0P", "CORTEX_M", "LIKE_CORTEX_M0", "CORTEX"],
   "Cortex-M1" : ["M1", "CORTEX_M", "LIKE_CORTEX_M1", "CORTEX"],
   "Cortex-M3" : ["M3", "CORTEX_M", "LIKE_CORTEX_M3", "CORTEX"],
   "Cortex-M4" : ["M4", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M4", "CORTEX"],
   "Cortex-M4F" : ["M4", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M4", "CORTEX"],
   "Cortex-M7" : ["M7", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M7", "CORTEX"],
   "Cortex-M7F" : ["M7", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M7", "CORTEX"],
   "Cortex-M7FD" : ["M7", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M7", "CORTEX"],
   "Cortex-A9" : ["A9", "CORTEX_A", "LIKE_CORTEX_A9", "CORTEX"],
    "Cortex-M23": ["M23", "CORTEX_M", "LIKE_CORTEX_M23", "CORTEX"],
    "Cortex-M23-NS": ["M23", "CORTEX_M", "LIKE_CORTEX_M23", "CORTEX"],
    "Cortex-M33": ["M33", "CORTEX_M", "LIKE_CORTEX_M33", "CORTEX"],
    "Cortex-M33-NS": ["M33", "CORTEX_M", "LIKE_CORTEX_M33", "CORTEX"]
}

################################################################################
# Generic Target class that reads and interprets the data in targets.json

class HookError(Exception):
    """ A simple class that represents all the exceptions associated with
    hooking
    """
    pass

CACHES = {}
def cached(func):
    """A simple decorator used for automatically caching data returned by a
    function
    """
    def wrapper(*args, **kwargs):
        """The wrapped function itself"""
        if (func.__name__, args) not in CACHES:
            CACHES[(func.__name__, args)] = func(*args, **kwargs)
        return CACHES[(func.__name__, args)]
    return wrapper


# Cumulative attributes can have values appended to them, so they
# need to be computed differently than regular attributes
CUMULATIVE_ATTRIBUTES = ['extra_labels', 'macros', 'device_has', 'features']


def get_resolution_order(json_data, target_name, order, level=0):
    """ Return the order in which target descriptions are searched for
    attributes. This mimics the Python 2.2 method resolution order, which
    is what the old targets.py module used. For more details, check
    http://makina-corpus.com/blog/metier/2014/python-tutorial-understanding-python-mro-class-search-path
    The resolution order contains (name, level) tuples, where "name" is the
    name of the class and "level" is the level in the inheritance hierarchy
    (the target itself is at level 0, its first parent at level 1, its
    parent's parent at level 2 and so on)
    """
    # the resolution order can't contain duplicate target names
    if target_name not in [l[0] for l in order]:
        order.append((target_name, level))
    parents = json_data[target_name].get("inherits", [])
    for par in parents:
        order = get_resolution_order(json_data, par, order, level + 1)
    return order


def target(name, json_data):
    """Construct a target object"""
    resolution_order = get_resolution_order(json_data, name, [])
    resolution_order_names = [tgt for tgt, _ in resolution_order]
    return Target(name=name,
                  json_data={key: value for key, value in json_data.items()
                             if key in resolution_order_names},
                  resolution_order=resolution_order,
                  resolution_order_names=resolution_order_names)

def generate_py_target(new_targets, name):
    """Add one or more new target(s) represented as a Python dictionary
    in 'new_targets'. It is an error to add a target with a name that
    already exists.
    """
    base_targets = Target.get_json_target_data()
    for new_target in new_targets.keys():
        if new_target in base_targets:
            raise Exception("Attempt to add target '%s' that already exists"
                            % new_target)
    total_data = {}
    total_data.update(new_targets)
    total_data.update(base_targets)
    return target(name, total_data)

class Target(namedtuple("Target", "name json_data resolution_order resolution_order_names")):
    """An object to represent a Target (MCU/Board)"""

    # Default location of the 'targets.json' file
    __targets_json_location_default = os.path.join(
        os.path.dirname(os.path.abspath(__file__)), '..', '..', 'targets', 'targets.json')

    # Current/new location of the 'targets.json' file
    __targets_json_location = None

    # Extra custom targets files
    __extra_target_json_files = []

    @staticmethod
    @cached
    def get_json_target_data():
        """Load the description of JSON target data"""
        targets = json_file_to_dict(Target.__targets_json_location or
                                    Target.__targets_json_location_default)

        for extra_target in Target.__extra_target_json_files:
            for k, v in json_file_to_dict(extra_target).items():
                if k in targets:
                    print('WARNING: Custom target "%s" cannot replace existing '
                          'target.' % k)
                else:
                    targets[k] = v

        return targets

    @staticmethod
    def add_extra_targets(source_dir):
        extra_targets_file = os.path.join(source_dir, "custom_targets.json")
        if os.path.exists(extra_targets_file):
            Target.__extra_target_json_files.append(extra_targets_file)
            CACHES.clear()

    @staticmethod
    def set_targets_json_location(location=None):
        """Set the location of the targets.json file"""
        Target.__targets_json_location = (location or
                                          Target.__targets_json_location_default)
        Target.__extra_target_json_files = []
        # Invalidate caches, since the location of the JSON file changed
        CACHES.clear()

    @staticmethod
    @cached
    def get_module_data():
        """Get the members of this module using Python's "inspect" module"""
        return dict([(m[0], m[1]) for m in
                     inspect.getmembers(sys.modules[__name__])])

    @staticmethod
    def __add_paths_to_progen(data):
        """Modify the exporter specification ("progen") by changing all
        "template" keys to full paths
        """
        out = {}
        for key, val in data.items():
            if isinstance(val, dict):
                out[key] = Target.__add_paths_to_progen(val)
            elif key == "template":
                out[key] = [os.path.join(os.path.dirname(__file__), 'export', v)
                            for v in val]
            else:
                out[key] = val
        return out

    def __getattr_cumulative(self, attrname):
        """Look for the attribute in the class and its parents, as defined by
        the resolution order
        """
        tdata = self.json_data
        # For a cumulative attribute, figure out when it was defined the
        # last time (in attribute resolution order) then follow the "_add"
        # and "_remove" data fields
        for idx, tgt in enumerate(self.resolution_order):
            # the attribute was defined at this level in the resolution
            # order
            if attrname in tdata[tgt[0]]:
                def_idx = idx
                break
        else:
            raise AttributeError("Attribute '%s' not found in target '%s'"
                                 % (attrname, self.name))
        # Get the starting value of the attribute
        starting_value = (tdata[self.resolution_order[def_idx][0]][attrname]
                          or [])[:]
        # Traverse the resolution list in high inheritance to low
        # inheritance level, left to right order to figure out all the
        # other classes that change the definition by adding or removing
        # elements
        for idx in range(self.resolution_order[def_idx][1] - 1, -1, -1):
            same_level_targets = [tar[0] for tar in self.resolution_order
                                  if tar[1] == idx]
            for tar in same_level_targets:
                data = tdata[tar]
                # Do we have anything to add ?
                if (attrname + "_add") in data:
                    starting_value.extend(data[attrname + "_add"])
                # Do we have anything to remove ?
                if (attrname + "_remove") in data:
                    # Macros can be defined either without a value (MACRO)
                    # or with a value (MACRO=10). When removing, we specify
                    # only the name of the macro, without the value. So we
                    # need to create a mapping between the macro name and
                    # its value. This will work for extra_labels and other
                    # type of arrays as well, since they fall into the
                    # "macros without a value" category (simple definitions
                    # without a value).
                    name_def_map = {}
                    for crtv in starting_value:
                        if crtv.find('=') != -1:
                            temp = crtv.split('=')
                            if len(temp) != 2:
                                raise ValueError(
                                    "Invalid macro definition '%s'" % crtv)
                            name_def_map[temp[0]] = crtv
                        else:
                            name_def_map[crtv] = crtv
                    for element in data[attrname + "_remove"]:
                        if element not in name_def_map:
                            raise ValueError(
                                ("Unable to remove '%s' in '%s.%s' since "
                                 % (element, self.name, attrname)) +
                                "it doesn't exist")
                        starting_value.remove(name_def_map[element])
        return starting_value

    def __getattr_helper(self, attrname):
        """Compute the value of a given target attribute"""
        if attrname in CUMULATIVE_ATTRIBUTES:
            return self.__getattr_cumulative(attrname)
        else:
            tdata = self.json_data
            starting_value = None
            for tgt in self.resolution_order:
                data = tdata[tgt[0]]
                try:
                    return data[attrname]
                except KeyError:
                    pass
            else: # Attribute not found
                raise AttributeError(
                    "Attribute '%s' not found in target '%s'"
                    % (attrname, self.name))

    def __getattr__(self, attrname):
        """ Return the value of an attribute. This function only computes the
        attribute's value once, then adds it to the instance attributes (in
        __dict__), so the next time it is returned directly
        """
        result = self.__getattr_helper(attrname)
        self.__dict__[attrname] = result
        return result

    @staticmethod
    @cached
    def get_target(target_name):
        """ Return the target instance starting from the target name """
        return target(target_name, Target.get_json_target_data())


    @property
    def program_cycle_s(self):
        """Special override for program_cycle_s as it's default value depends
        upon is_disk_virtual
        """
        try:
            return self.__getattr__("program_cycle_s")
        except AttributeError:
            return 4 if self.is_disk_virtual else 1.5

    @property
    def labels(self):
        """Get all possible labels for this target"""
        names = copy(self.resolution_order_names)
        if "Target" in names:
            names.remove("Target")
        labels = (names + CORE_LABELS[self.core] + self.extra_labels)
        # Automatically define UVISOR_UNSUPPORTED if the target doesn't
        # specifically define UVISOR_SUPPORTED
        if "UVISOR_SUPPORTED" not in labels:
            labels.append("UVISOR_UNSUPPORTED")
        return labels

    def init_hooks(self, hook, toolchain):
        """Initialize the post-build hooks for a toolchain. For now, this
        function only allows "post binary" hooks (hooks that are executed
        after the binary image is extracted from the executable file)

        Positional Arguments:
        hook - the hook object to add post-binary-hooks to
        toolchain - the toolchain object for inspection
        """

        # If there's no hook, simply return
        try:
            hook_data = self.post_binary_hook
        except AttributeError:
            return
        # A hook was found. The hook's name is in the format
        # "classname.functionname"
        temp = hook_data["function"].split(".")
        if len(temp) != 2:
            raise HookError(
                ("Invalid format for hook '%s' in target '%s'"
                 % (hook_data["function"], self.name)) +
                " (must be 'class_name.function_name')")
        class_name, function_name = temp
        # "class_name" must refer to a class in this file, so check if the
        # class exists
        mdata = self.get_module_data()
        if class_name not in mdata or \
           not inspect.isclass(mdata[class_name]):
            raise HookError(
                ("Class '%s' required by '%s' in target '%s'"
                 % (class_name, hook_data["function"], self.name)) +
                " not found in targets.py")
        # "function_name" must refer to a static function inside class
        # "class_name"
        cls = mdata[class_name]
        if (not hasattr(cls, function_name)) or \
           (not inspect.isfunction(getattr(cls, function_name))):
            raise HookError(
                ("Static function '%s' " % function_name) +
                ("required by '%s' " % hook_data["function"]) +
                ("in target '%s' " % self.name) +
                ("not found in class '%s'" %  class_name))
        # Check if the hook specification also has toolchain restrictions
        toolchain_restrictions = set(hook_data.get("toolchains", []))
        toolchain_labels = set(c.__name__ for c in getmro(toolchain.__class__))
        if toolchain_restrictions and \
           not toolchain_labels.intersection(toolchain_restrictions):
            return
        # Finally, hook the requested function
        hook.hook_add_binary("post", getattr(cls, function_name))

################################################################################
# Target specific code goes in this section
# This code can be invoked from the target description using the
# "post_binary_hook" key

class LPCTargetCode(object):
    """General LPC Target patching code"""
    @staticmethod
    def lpc_patch(t_self, resources, elf, binf):
        """Patch an elf file"""
        t_self.notify.debug("LPC Patch: %s" % os.path.split(binf)[1])
        patch(binf)

class LPC4088Code(object):
    """Code specific to the LPC4088"""
    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
        """Hook to be run after an elf file is built"""
        if not os.path.isdir(binf):
            # Regular binary file, nothing to do
            LPCTargetCode.lpc_patch(t_self, resources, elf, binf)
            return
        outbin = open(binf + ".temp", "wb")
        partf = open(os.path.join(binf, "ER_IROM1"), "rb")
        # Pad the fist part (internal flash) with 0xFF to 512k
        data = partf.read()
        outbin.write(data)
        outbin.write('\xFF' * (512*1024 - len(data)))
        partf.close()
        # Read and append the second part (external flash) in chunks of fixed
        # size
        chunksize = 128 * 1024
        partf = open(os.path.join(binf, "ER_IROM2"), "rb")
        while True:
            data = partf.read(chunksize)
            outbin.write(data)
            if len(data) < chunksize:
                break
        partf.close()
        outbin.close()
        # Remove the directory with the binary parts and rename the temporary
        # file to 'binf'
        shutil.rmtree(binf, True)
        os.rename(binf + '.temp', binf)
        t_self.notify.debug("Generated custom binary file (internal flash + SPIFI)")
        LPCTargetCode.lpc_patch(t_self, resources, elf, binf)

class TEENSY3_1Code(object):
    """Hooks for the TEENSY3.1"""
    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
        """Hook that is run after elf is generated"""
        # This function is referenced by old versions of targets.json and should
        # be kept for backwards compatibility.
        pass

class MTSCode(object):
    """Generic MTS code"""
    @staticmethod
    def _combine_bins_helper(target_name, binf):
        """combine bins with the bootloader for a particular target"""
        loader = os.path.join(TOOLS_BOOTLOADERS, target_name, "bootloader.bin")
        target = binf + ".tmp"
        if not os.path.exists(loader):
            print("Can't find bootloader binary: " + loader)
            return
        outbin = open(target, 'w+b')
        part = open(loader, 'rb')
        data = part.read()
        outbin.write(data)
        outbin.write('\xFF' * (64*1024 - len(data)))
        part.close()
        part = open(binf, 'rb')
        data = part.read()
        outbin.write(data)
        part.close()
        outbin.seek(0, 0)
        data = outbin.read()
        outbin.seek(0, 1)
        crc = struct.pack('<I', binascii.crc32(data) & 0xFFFFFFFF)
        outbin.write(crc)
        outbin.close()
        os.remove(binf)
        os.rename(target, binf)

    @staticmethod
    def combine_bins_mts_dot(t_self, resources, elf, binf):
        """A hook for the MTS MDOT"""
        MTSCode._combine_bins_helper("MTS_MDOT_F411RE", binf)

    @staticmethod
    def combine_bins_mts_dragonfly(t_self, resources, elf, binf):
        """A hoof for the MTS Dragonfly"""
        MTSCode._combine_bins_helper("MTS_DRAGONFLY_F411RE", binf)

    @staticmethod
    def combine_bins_mtb_mts_dragonfly(t_self, resources, elf, binf):
        """A hook for the MTB MTS Dragonfly"""
        MTSCode._combine_bins_helper("MTB_MTS_DRAGONFLY", binf)

class MCU_NRF51Code(object):
    """NRF51 Hooks"""
    @staticmethod
    def binary_hook(t_self, resources, _, binf):
        """Hook that merges the soft device with the bin file"""
        # Scan to find the actual paths of soft device
        sdf = None
        for softdevice_and_offset_entry\
            in t_self.target.EXPECTED_SOFTDEVICES_WITH_OFFSETS:
            for hexf in resources.hex_files:
                if hexf.find(softdevice_and_offset_entry['name']) != -1:
                    t_self.notify.debug("SoftDevice file found %s."
                                        % softdevice_and_offset_entry['name'])
                    sdf = hexf

                if sdf is not None:
                    break
            if sdf is not None:
                break

        if sdf is None:
            t_self.notify.debug("Hex file not found. Aborting.")
            return

        # Look for bootloader file that matches this soft device or bootloader
        # override image
        blf = None
        if t_self.target.MERGE_BOOTLOADER is True:
            for hexf in resources.hex_files:
                if hexf.find(t_self.target.OVERRIDE_BOOTLOADER_FILENAME) != -1:
                    t_self.notify.debug("Bootloader file found %s."
                                        % t_self.target.OVERRIDE_BOOTLOADER_FILENAME)
                    blf = hexf
                    break
                elif hexf.find(softdevice_and_offset_entry['boot']) != -1:
                    t_self.notify.debug("Bootloader file found %s."
                                        % softdevice_and_offset_entry['boot'])
                    blf = hexf
                    break

        # Merge user code with softdevice
        from intelhex import IntelHex
        binh = IntelHex()
        _, ext = os.path.splitext(binf)
        if ext == ".hex":
            binh.loadhex(binf)
        elif ext == ".bin":
            binh.loadbin(binf, softdevice_and_offset_entry['offset'])

        if t_self.target.MERGE_SOFT_DEVICE is True:
            t_self.notify.debug("Merge SoftDevice file %s"
                                % softdevice_and_offset_entry['name'])
            sdh = IntelHex(sdf)
            sdh.start_addr = None
            binh.merge(sdh)

        if t_self.target.MERGE_BOOTLOADER is True and blf is not None:
            t_self.notify.debug("Merge BootLoader file %s" % blf)
            blh = IntelHex(blf)
            blh.start_addr = None
            binh.merge(blh)

        with open(binf.replace(".bin", ".hex"), "w") as fileout:
            binh.write_hex_file(fileout, write_start_addr=False)

class NCS36510TargetCode:
    @staticmethod
    def ncs36510_addfib(t_self, resources, elf, binf):
        from tools.targets.NCS import add_fib_at_start
        print("binf ", binf)
        add_fib_at_start(binf[:-4])

class RTL8195ACode:
    """RTL8195A Hooks"""
    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
        from tools.targets.REALTEK_RTL8195AM import rtl8195a_elf2bin
        rtl8195a_elf2bin(t_self, elf, binf)
################################################################################

# Instantiate all public targets
def update_target_data():
    TARGETS[:] = [Target.get_target(tgt) for tgt, obj
                  in Target.get_json_target_data().items()
                  if obj.get("public", True)]
    # Map each target name to its unique instance
    TARGET_MAP.clear()
    TARGET_MAP.update(dict([(tgt.name, tgt) for tgt in TARGETS]))
    TARGET_NAMES[:] = TARGET_MAP.keys()

TARGETS = []
TARGET_MAP = dict()
TARGET_NAMES = []

update_target_data()

# Some targets with different name have the same exporters
EXPORT_MAP = {}

# Detection APIs
def get_target_detect_codes():
    """ Returns dictionary mapping detect_code -> platform_name
    """
    result = {}
    for tgt in TARGETS:
        for detect_code in tgt.detect_code:
            result[detect_code] = tgt.name
    return result

def set_targets_json_location(location=None):
    """Sets the location of the JSON file that contains the targets"""
    # First instruct Target about the new location
    Target.set_targets_json_location(location)
    # Then re-initialize TARGETS, TARGET_MAP and TARGET_NAMES. The
    # re-initialization does not create new variables, it keeps the old ones
    # instead. This ensures compatibility with code that does
    # "from tools.targets import TARGET_NAMES"
    update_target_data()

