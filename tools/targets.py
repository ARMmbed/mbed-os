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

CORE_LABELS = {
    "ARM7TDMI-S": ["ARM7", "LIKE_CORTEX_ARM7"],
    "Cortex-M0" : ["M0", "CORTEX_M", "LIKE_CORTEX_M0"],
    "Cortex-M0+": ["M0P", "CORTEX_M", "LIKE_CORTEX_M0"],
    "Cortex-M1" : ["M1", "CORTEX_M", "LIKE_CORTEX_M1"],
    "Cortex-M3" : ["M3", "CORTEX_M", "LIKE_CORTEX_M3"],
    "Cortex-M4" : ["M4", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M4"],
    "Cortex-M4F" : ["M4", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M4"],
    "Cortex-M7" : ["M7", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M7"],
    "Cortex-M7F" : ["M7", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M7"],
    "Cortex-M7FD" : ["M7", "CORTEX_M", "RTOS_M4_M7", "LIKE_CORTEX_M7"],
    "Cortex-A9" : ["A9", "CORTEX_A", "LIKE_CORTEX_A9"]
}

import os
import binascii
import struct
import shutil
from tools.patch import patch
from paths import TOOLS_BOOTLOADERS
import json
import inspect
import sys
from tools.utils import json_file_to_dict

########################################################################################################################
# Generic Target class that reads and interprets the data in targets.json

# A simple class that represents all the exceptions associated with hooking
class HookError(Exception):
    pass

# A simple decorator used for automatically caching data returned by a function
caches = {}
def cached(func):
    def wrapper(*args, **kwargs):
        if not caches.has_key((func.__name__, args)):
            caches[(func.__name__, args)] = func(*args, **kwargs)
        return caches[(func.__name__, args)]
    return wrapper

class Target:
    # Cumulative attributes can have values appended to them, so they
    # need to be computed differently than regular attributes
    __cumulative_attributes = ['extra_labels', 'macros', 'device_has', 'features']

    # List of targets that were added dynamically using "add_py_targets" (see below)
    __py_targets = set()

    # Default location of the 'targets.json' file
    __targets_json_location_default = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'hal', 'targets.json')

    # Current/new location of the 'targets.json' file
    __targets_json_location = None

    # Load the description of JSON target data
    @staticmethod
    @cached
    def get_json_target_data():
        return json_file_to_dict(Target.__targets_json_location or Target.__targets_json_location_default)

    # Set the location of the targets.json file
    @staticmethod
    def set_targets_json_location(location=None):
        Target.__targets_json_location = location or Target.__targets_json_location_default
        # Invalidate caches, since the location of the JSON file changed
        caches.clear()

    # Get the members of this module using Python's "inspect" module
    @staticmethod
    @cached
    def get_module_data():
        return dict([(m[0], m[1]) for m in inspect.getmembers(sys.modules[__name__])])

    # Return the order in which target descriptions are searched for attributes
    # This mimics the Python 2.2 method resolution order, which is what the old targets.py module used
    # For more details, check http://makina-corpus.com/blog/metier/2014/python-tutorial-understanding-python-mro-class-search-path
    # The resolution order contains (name, level) tuples, where "name" is the name of the class and "level"
    # is the level in the inheritance hierarchy (the target itself is at level 0, its first parent at level 1,
    # its parent's parent at level 1 and so on)
    def __get_resolution_order(self, target_name, order, level = 0):
        if not target_name in [l[0] for l in order]: # the resolution order can't contain duplicate target names
            order.append((target_name, level))
        parents = self.get_json_target_data()[target_name].get("inherits", [])
        for p in parents:
            order = self.__get_resolution_order(p, order, level + 1)
        return order

    # Modify the exporter specification ("progen") by changing all "template" keys to full paths
    @staticmethod
    def __add_paths_to_progen(data):
        out = {}
        for key, value in data.items():
            if isinstance(value, dict):
                out[key] = Target.__add_paths_to_progen(value)
            elif key == "template":
                out[key] = [os.path.join(os.path.dirname(__file__), 'export', v) for v in value]
            else:
                out[key] = value
        return out

    # Comute the value of a given target attribute
    def __getattr_helper(self, attrname):
        tdata = self.get_json_target_data()
        if attrname in self.__cumulative_attributes:
            # For a cumulative attribute, figure out when it was defined the last time (in attribute
            # resolution order) then follow the "_add" and "_remove" data fields
            for idx, t in enumerate(self.resolution_order):
                if attrname in tdata[t[0]]: # the attribute was defined at this level in the resolution order
                    def_idx = idx
                    break
            else:
                raise AttributeError("Attribute '%s' not found in target '%s'" % (attrname, self.name))
            # Get the starting value of the attribute
            v = (tdata[self.resolution_order[def_idx][0]][attrname] or [])[:]
            # Traverse the resolution list in high inheritance to low inheritance level, left to right order
            # to figure out all the other classes that change the definition by adding or removing elements
            for idx in xrange(self.resolution_order[def_idx][1] - 1, -1, -1):
                same_level_targets = [t[0] for t in self.resolution_order if t[1] == idx]
                for t in same_level_targets:
                    data = tdata[t]
                    # Do we have anything to add ?
                    if data.has_key(attrname + "_add"):
                        v.extend(data[attrname + "_add"])
                    # Do we have anything to remove ?
                    if data.has_key(attrname + "_remove"):
                        # Macros can be defined either without a value (MACRO) or with a value (MACRO=10).
                        # When removing, we specify only the name of the macro, without the value. So we need
                        # to create a mapping between the macro name and its value. This will work for
                        # extra_labels and other type of arrays as well, since they fall into the "macros
                        # without a value" category (simple definitions without a value).
                        name_def_map = {}
                        for crtv in v:
                            if crtv.find('=') != -1:
                                temp = crtv.split('=')
                                if len(temp) != 2:
                                    raise ValueError("Invalid macro definition '%s'" % crtv)
                                name_def_map[temp[0]] = crtv
                            else:
                                name_def_map[crtv] = crtv
                        for e in data[attrname + "_remove"]:
                            if not e in name_def_map:
                                raise ValueError("Unable to remove '%s' in '%s.%s' since it doesn't exist" % (e, self.name, attrname))
                            v.remove(name_def_map[e])
            return v
        # Look for the attribute in the class and its parents, as defined by the resolution order
        v = None
        for t in self.resolution_order:
            data = tdata[t[0]]
            if data.has_key(attrname):
                v = data[attrname]
                break
        else: # Attribute not found
            raise AttributeError("Attribute '%s' not found in target '%s'" % (attrname, self.name))
        # 'progen' needs the full path to the template (the path in JSON is relative to tools/export)
        return v if attrname != "progen" else self.__add_paths_to_progen(v)

    # Return the value of an attribute
    # This function only computes the attribute's value once, then adds it to the instance attributes
    # (in __dict__), so the next time it is returned directly
    def __getattr__(self, attrname):
        v = self.__getattr_helper(attrname)
        self.__dict__[attrname] = v
        return v

    # Add one or more new target(s) represented as a Python dictionary in 'new_targets'
    # It is an error to add a target with a name that already exists.
    @staticmethod
    def add_py_targets(new_targets):
        crt_data = Target.get_json_target_data()
        for tk, tv in new_targets.items():
            if crt_data.has_key(tk):
                raise Exception("Attempt to add target '%s' that already exists" % tk)
            # Add target data to the internal target dictionary
            crt_data[tk] = tv
            # Create the new target and add it to the relevant data structures
            new_target = Target(tk)
            TARGETS.append(new_target)
            TARGET_MAP[tk] = new_target
            TARGET_NAMES.append(tk)

    # Return the target instance starting from the target name
    @staticmethod
    @cached
    def get_target(name):
        return Target(name)

    def __init__(self, name):
        self.name = name

        # Compute resolution order once (it will be used later in __getattr__)
        self.resolution_order = self.__get_resolution_order(self.name, [])
        # Create also a list with only the names of the targets in the resolution order
        self.resolution_order_names = [t[0] for t in self.resolution_order]

    @property
    def program_cycle_s(self):
        try:
            return self.__getattr__("program_cycle_s")
        except AttributeError:
            return 4 if self.is_disk_virtual else 1.5

    def get_labels(self):
        labels = [self.name] + CORE_LABELS[self.core] + self.extra_labels
        # Automatically define UVISOR_UNSUPPORTED if the target doesn't specifically
        # define UVISOR_SUPPORTED
        if not "UVISOR_SUPPORTED" in labels:
            labels.append("UVISOR_UNSUPPORTED")
        return labels

    # For now, this function only allows "post binary" hooks (hooks that are executed after
    # the binary image is extracted from the executable file)
    def init_hooks(self, hook, toolchain_name):
        # If there's no hook, simply return
        try:
            hook_data = self.post_binary_hook
        except AttributeError:
            return
        # A hook was found. The hook's name is in the format "classname.functionname"
        temp = hook_data["function"].split(".")
        if len(temp) != 2:
            raise HookError("Invalid format for hook '%s' in target '%s' (must be 'class_name.function_name')" % (hook_data["function"], self.name))
        class_name, function_name = temp[0], temp[1]
        # "class_name" must refer to a class in this file, so check if the class exists
        mdata = self.get_module_data()
        if not mdata.has_key(class_name) or not inspect.isclass(mdata[class_name]):
            raise HookError("Class '%s' required by '%s' in target '%s' not found in targets.py" % (class_name, hook_data["function"], self.name))
        # "function_name" must refer to a static function inside class "class_name"
        cls = mdata[class_name]
        if (not hasattr(cls, function_name)) or (not inspect.isfunction(getattr(cls, function_name))):
            raise HookError("Static function '%s' required by '%s' in target '%s' not found in class '%s'" % (function_name, hook_data["function"], self.name, class_name))
        # Check if the hook specification also has target restrictions
        toolchain_restrictions = hook_data.get("toolchains", [])
        if toolchain_restrictions and (toolchain_name not in toolchain_restrictions):
            return
        # Finally, hook the requested function
        hook.hook_add_binary("post", getattr(cls, function_name))

########################################################################################################################
# Target specific code goes in this section
# This code can be invoked from the target description using the "post_binary_hook" key

class LPCTargetCode:
    @staticmethod
    def lpc_patch(t_self, resources, elf, binf):
        t_self.debug("LPC Patch: %s" % os.path.split(binf)[1])
        patch(binf)

class LPC4088Code:
    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
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
        # Read and append the second part (external flash) in chunks of fixed size
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
        t_self.debug("Generated custom binary file (internal flash + SPIFI)")
        LPCTargetCode.lpc_patch(t_self, resources, elf, binf)

class TEENSY3_1Code:
    @staticmethod
    def binary_hook(t_self, resources, elf, binf):
        from intelhex import IntelHex
        binh = IntelHex()
        binh.loadbin(binf, offset = 0)

        with open(binf.replace(".bin", ".hex"), "w") as f:
            binh.tofile(f, format='hex')

class MTSCode:
    @staticmethod
    def _combine_bins_helper(target_name, t_self, resources, elf, binf):
        loader = os.path.join(TOOLS_BOOTLOADERS, target_name, "bootloader.bin")
        target = binf + ".tmp"
        if not os.path.exists(loader):
            print "Can't find bootloader binary: " + loader
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
        MTSCode._combine_bins_helper("MTS_MDOT_F411RE", t_self, resources, elf, binf)

    @staticmethod
    def combine_bins_mts_dragonfly(t_self, resources, elf, binf):
        MTSCode._combine_bins_helper("MTS_DRAGONFLY_F411RE", t_self, resources, elf, binf)

class MCU_NRF51Code:
    @staticmethod
    def binary_hook(t_self, resources, elf, binf):

        # Scan to find the actual paths of soft device
        sdf = None
        for softdeviceAndOffsetEntry in t_self.target.EXPECTED_SOFTDEVICES_WITH_OFFSETS:
            for hexf in resources.hex_files:
                if hexf.find(softdeviceAndOffsetEntry['name']) != -1:
                    t_self.debug("SoftDevice file found %s." % softdeviceAndOffsetEntry['name'])
                    sdf = hexf

                if sdf is not None: break
            if sdf is not None: break

        if sdf is None:
            t_self.debug("Hex file not found. Aborting.")
            return

        # Look for bootloader file that matches this soft device or bootloader override image
        blf = None
        if t_self.target.MERGE_BOOTLOADER is True:
            for hexf in resources.hex_files:
                if hexf.find(t_self.target.OVERRIDE_BOOTLOADER_FILENAME) != -1:
                    t_self.debug("Bootloader file found %s." % t_self.target.OVERRIDE_BOOTLOADER_FILENAME)
                    blf = hexf
                    break
                elif hexf.find(softdeviceAndOffsetEntry['boot']) != -1:
                    t_self.debug("Bootloader file found %s." % softdeviceAndOffsetEntry['boot'])
                    blf = hexf
                    break

        # Merge user code with softdevice
        from intelhex import IntelHex
        binh = IntelHex()
        binh.loadbin(binf, offset=softdeviceAndOffsetEntry['offset'])

        if t_self.target.MERGE_SOFT_DEVICE is True:
            t_self.debug("Merge SoftDevice file %s" % softdeviceAndOffsetEntry['name'])
            sdh = IntelHex(sdf)
            binh.merge(sdh)

        if t_self.target.MERGE_BOOTLOADER is True and blf is not None:
            t_self.debug("Merge BootLoader file %s" % blf)
            blh = IntelHex(blf)
            binh.merge(blh)

        with open(binf.replace(".bin", ".hex"), "w") as f:
            binh.tofile(f, format='hex')

########################################################################################################################

# Instantiate all public targets
TARGETS = [Target.get_target(name) for name, value in Target.get_json_target_data().items() if value.get("public", True)]

# Map each target name to its unique instance
TARGET_MAP = dict([(t.name, t) for t in TARGETS])

TARGET_NAMES = TARGET_MAP.keys()

# Some targets with different name have the same exporters
EXPORT_MAP = {}

# Detection APIs
def get_target_detect_codes():
    """ Returns dictionary mapping detect_code -> platform_name
    """
    result = {}
    for target in TARGETS:
        for detect_code in target.detect_code:
            result[detect_code] = target.name
    return result

# Sets the location of the JSON file that contains the targets
def set_targets_json_location(location=None):
    # First instruct Target about the new location
    Target.set_targets_json_location(location)
    # Then re-initialize TARGETS, TARGET_MAP and TARGET_NAMES
    # The re-initialization does not create new variables, it keeps the old ones instead
    # This ensures compatibility with code that does "from tools.targets import TARGET_NAMES"
    TARGETS[:] = [Target.get_target(name) for name, value in Target.get_json_target_data().items() if value.get("public", True)]
    TARGET_MAP.clear()
    TARGET_MAP.update(dict([(t.name, t) for t in TARGETS]))
    TARGET_NAMES[:] = TARGET_MAP.keys()
