"""
mbed SDK
Copyright (c) 2016 ARM Limited

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
from __future__ import print_function, division, absolute_import

from copy import deepcopy
from six import moves
import json
import six
import os
from os.path import dirname, abspath, exists, join, isabs
import sys
from collections import namedtuple
from os.path import splitext, relpath
from intelhex import IntelHex
from jinja2 import FileSystemLoader, StrictUndefined
from jinja2.environment import Environment
from jsonschema import Draft4Validator, RefResolver

from ..resources import FileType
from ..utils import (json_file_to_dict, intelhex_offset, integer,
                     NotSupportedException)
from ..arm_pack_manager import Cache
from ..targets import (CUMULATIVE_ATTRIBUTES, TARGET_MAP, generate_py_target,
                       get_resolution_order, Target)

try:
    unicode
except NameError:
    unicode = str
PATH_OVERRIDES = set(["target.bootloader_img"])
ROM_OVERRIDES = set([
    # managed BL
    "target.bootloader_img", "target.restrict_size",
    "target.header_format", "target.header_offset",
    "target.app_offset",

    # unmanaged BL
    "target.mbed_app_start", "target.mbed_app_size",

    # both
    "target.mbed_rom_start", "target.mbed_rom_size",
])
RAM_OVERRIDES = set([
    # both
    "target.mbed_ram_start", "target.mbed_ram_size",
])

BOOTLOADER_OVERRIDES = ROM_OVERRIDES | RAM_OVERRIDES


ALLOWED_FEATURES = [
    "BOOTLOADER","UVISOR", "BLE", "CLIENT", "IPV4", "LWIP", "COMMON_PAL", "STORAGE",
    "NANOSTACK","CRYPTOCELL310",
    # Nanostack configurations
    "LOWPAN_BORDER_ROUTER", "LOWPAN_HOST", "LOWPAN_ROUTER", "NANOSTACK_FULL",
    "THREAD_BORDER_ROUTER", "THREAD_END_DEVICE", "THREAD_ROUTER",
    "ETHERNET_HOST",
]

# Base class for all configuration exceptions
class ConfigException(Exception):
    """Config system only exception. Makes it easier to distinguish config
    errors"""
    pass

class UndefinedParameter(ConfigException):
    def __init__(self, param, name, kind, label):
        self.param = param
        self.name = name
        self.kind = kind
        self.label = label

    def __str__(self):
        return "Attempt to override undefined parameter '{}' in '{}'".format(
            self.param,
            ConfigParameter.get_display_name(self.name, self.kind, self.label),
        )

class ConfigParameter(object):
    """This class keeps information about a single configuration parameter"""

    def __init__(self, name, data, unit_name, unit_kind):
        """Construct a ConfigParameter

        Positional arguments:
        name - the name of the configuration parameter
        data - the data associated with the configuration parameter
        unit_name - the unit (target/library/application) that defines this
                    parameter
        unit_ kind - the kind of the unit ("target", "library" or "application")
        """
        self.name = self.get_full_name(name, unit_name, unit_kind,
                                       allow_prefix=False)
        self.defined_by = self.get_display_name(unit_name, unit_kind)
        self.set_value(data.get("value", None), unit_name, unit_kind)
        self.help_text = data.get("help", None)
        self.required = data.get("required", False)
        self.macro_name = data.get("macro_name", "MBED_CONF_%s" %
                                   self.sanitize(self.name.upper()))
        self.config_errors = []

    @staticmethod
    def get_full_name(name, unit_name, unit_kind, label=None,
                      allow_prefix=True):
        """Return the full (prefixed) name of a parameter. If the parameter
        already has a prefix, check if it is valid

        Positional arguments:
        name - the simple (unqualified) name of the parameter
        unit_name - the unit (target/library/application) that defines this
                    parameter
        unit_kind - the kind of the unit ("target", "library" or "application")

        Keyword arguments:
        label - the name of the label in the 'target_config_overrides' section
        allow_prefix - True to allow the original name to have a prefix, False
                       otherwise
        """
        if name.find('.') == -1: # the name is not prefixed
            if unit_kind == "target":
                prefix = "target."
            elif unit_kind == "application":
                prefix = "app."
            else:
                prefix = unit_name + '.'
            return prefix + name
        if name in BOOTLOADER_OVERRIDES:
            return name
        # The name has a prefix, so check if it is valid
        if not allow_prefix:
            raise ConfigException("Invalid parameter name '%s' in '%s'" %
                                  (name, ConfigParameter.get_display_name(
                                      unit_name, unit_kind, label)))
        temp = name.split(".")
        # Check if the parameter syntax is correct (must be
        # unit_name.parameter_name)
        if len(temp) != 2:
            raise ConfigException("Invalid parameter name '%s' in '%s'" %
                                  (name, ConfigParameter.get_display_name(
                                      unit_name, unit_kind, label)))
        prefix = temp[0]
        # Check if the given parameter prefix matches the expected prefix
        if (unit_kind == "library" and prefix not in [unit_name, "target"]) or \
           (unit_kind == "target" and prefix != "target"):
            raise ConfigException(
                "Invalid prefix '%s' for parameter name '%s' in '%s'" %
                (prefix, name, ConfigParameter.get_display_name(
                    unit_name, unit_kind, label)))
        return name

    @staticmethod
    def get_display_name(unit_name, unit_kind, label=None):
        """Return the name displayed for a unit when interrogating the origin
        and the last set place of a parameter

        Positional arguments:
        unit_name - the unit (target/library/application) that defines this
                    parameter
        unit_kind - the kind of the unit ("target", "library" or "application")

        Keyword arguments:
        label - the name of the label in the 'target_config_overrides' section
        """
        if unit_kind == "target":
            return "target:" + unit_name
        elif unit_kind == "application":
            return "application%s" % ("[%s]" % label if label else "")
        else: # library
            return "library:%s%s" % (unit_name, "[%s]" % label if label else "")

    @staticmethod
    def sanitize(name):
        """ "Sanitize" a name so that it is a valid C macro name. Currently it
        simply replaces '.' and '-' with '_'.

        Positional arguments:
        name - the name to make into a valid C macro
        """
        return name.replace('.', '_').replace('-', '_')

    def set_value(self, value, unit_name, unit_kind, label=None):
        """ Sets a value for this parameter, remember the place where it was
        set.  If the value is a Boolean, it is converted to 1 (for True) or
        to 0 (for False).

        Positional arguments:
        value - the value of the parameter
        unit_name - the unit (target/library/application) that defines this
                   parameter
        unit_kind - the kind of the unit ("target", "library" or "application")

        Keyword arguments:
        label - the name of the label in the 'target_config_overrides' section
               (optional)
        """
        self.value = int(value) if isinstance(value, bool) else value
        self.set_by = self.get_display_name(unit_name, unit_kind, label)

    def __str__(self):
        """Return the string representation of this configuration parameter

        Arguments: None
        """
        if self.value is not None:
            return '%s = %s (macro name: "%s")' % \
                (self.name, self.value, self.macro_name)
        else:
            return '%s has no value' % self.name

    def get_verbose_description(self):
        """Return a verbose description of this configuration parameter as a
        string

        Arguments: None
        """
        desc = "Name: %s%s\n" % \
               (self.name, " (required parameter)" if self.required else "")
        if self.help_text:
            desc += "    Description: %s\n" % self.help_text
        desc += "    Defined by: %s\n" % self.defined_by
        if not self.value:
            return desc + "    No value set"
        desc += "    Macro name: %s\n" % self.macro_name
        desc += "    Value: %s (set by %s)" % (self.value, self.set_by)
        return desc

class ConfigMacro(object):
    """ A representation of a configuration macro. It handles both macros
    without a value (MACRO) and with a value (MACRO=VALUE)
    """
    def __init__(self, name, unit_name, unit_kind):
        """Construct a ConfigMacro object

        Positional arguments:
        name - the macro's name
        unit_name - the location where the macro was defined
        unit_kind - the type of macro this is
        """
        self.name = name
        self.defined_by = ConfigParameter.get_display_name(unit_name, unit_kind)
        if name.find("=") != -1:
            tmp = name.split("=")
            if len(tmp) != 2:
                raise ValueError("Invalid macro definition '%s' in '%s'" %
                                 (name, self.defined_by))
            self.macro_name = tmp[0]
            self.macro_value = tmp[1]
        else:
            self.macro_name = name
            self.macro_value = None

class ConfigCumulativeOverride(object):
    """Representation of overrides for cumulative attributes"""
    def __init__(self, name, additions=None, removals=None, strict=False):
        """Construct a ConfigCumulativeOverride object

        Positional arguments:
        name - the name of the config file this came from ?

        Keyword arguments:
        additions - macros to add to the overrides
        removals - macros to remove from the overrides
        strict - Boolean indicating that attempting to remove from an override
                 that does not exist should error
        """
        self.name = name
        if additions:
            self.additions = set(additions)
        else:
            self.additions = set()
        if removals:
            self.removals = set(removals)
        else:
            self.removals = set()
        self.strict = strict

    def remove_cumulative_overrides(self, overrides):
        """Extend the list of override removals.

        Positional arguments:
        overrides - a list of names that, when the override is evaluated, will
                    be removed
        """
        for override in overrides:
            if override in self.additions:
                raise ConfigException(
                    "Configuration conflict. The %s %s both added and removed."
                    % (self.name[:-1], override))

        self.removals |= set(overrides)

    def add_cumulative_overrides(self, overrides):
        """Extend the list of override additions.

        Positional arguments:
        overrides - a list of a names that, when the override is evaluated, will
                    be added to the list
        """
        for override in overrides:
            if override in self.removals or \
               (self.strict and override not in self.additions):
                raise ConfigException(
                    "Configuration conflict. The %s %s both added and removed."
                    % (self.name[:-1], override))

        self.additions |= set(overrides)

    def strict_cumulative_overrides(self, overrides):
        """Remove all overrides that are not the specified ones

        Positional arguments:
        overrides - a list of names that will replace the entire attribute when
                    this override is evaluated.
        """
        self.remove_cumulative_overrides(self.additions - set(overrides))
        self.add_cumulative_overrides(overrides)
        self.strict = True

    def update_target(self, target):
        """Update the attributes of a target based on this override"""
        setattr(target, self.name,
                list((set(getattr(target, self.name, []))
                      | self.additions) - self.removals))


def _process_config_parameters(data, params, unit_name, unit_kind):
    """Process a "config_parameters" section in either a target, a library,
    or the application.

    Positional arguments:
    data - a dictionary with the configuration parameters
    params - storage for the discovered configuration parameters
    unit_name - the unit (target/library/application) that defines this
                parameter
    unit_kind - the kind of the unit ("target", "library" or "application")
    """
    for name, val in data.items():
        full_name = ConfigParameter.get_full_name(name, unit_name, unit_kind)
        # If the parameter was already defined, raise an error
        if full_name in params:
            raise ConfigException(
                "Parameter name '%s' defined in both '%s' and '%s'" %
                (name, ConfigParameter.get_display_name(unit_name, unit_kind),
                 params[full_name].defined_by))
        # Otherwise add it to the list of known parameters
        # If "val" is not a dictionary, this is a shortcut definition,
        # otherwise it is a full definition
        params[full_name] = ConfigParameter(name, val if isinstance(val, dict)
                                            else {"value": val}, unit_name,
                                            unit_kind)
    return params


def _process_macros(mlist, macros, unit_name, unit_kind):
    """Process a macro definition and check for incompatible duplicate
    definitions.

    Positional arguments:
    mlist - list of macro names to process
    macros - dictionary with currently discovered macros
    unit_name - the unit (library/application) that defines this macro
    unit_kind - the kind of the unit ("library" or "application")
    """
    for mname in mlist:
        macro = ConfigMacro(mname, unit_name, unit_kind)
        if (macro.macro_name in macros) and \
           (macros[macro.macro_name].name != mname):
            # Found an incompatible definition of the macro in another module,
            # so raise an error
            full_unit_name = ConfigParameter.get_display_name(unit_name,
                                                              unit_kind)
            raise ConfigException(
                ("Macro '%s' defined in both '%s' and '%s'"
                 % (macro.macro_name, macros[macro.macro_name].defined_by,
                    full_unit_name)) +
                " with incompatible values")
        macros[macro.macro_name] = macro


Region = namedtuple("Region", "name start size active filename")
RamRegion = namedtuple("RamRegion", "name start size active")

class Config(object):
    """'Config' implements the mbed configuration mechanism"""

    # Libraries and applications have different names for their configuration
    # files
    __mbed_app_config_name = "mbed_app.json"
    __mbed_lib_config_name = "mbed_lib.json"

    __unused_overrides = set(["target.bootloader_img", "target.restrict_size",
                              "target.mbed_app_start", "target.mbed_app_size"])

    @classmethod
    def find_app_config(cls, top_level_dirs):
        app_config_location = None
        for directory in top_level_dirs:
            full_path = os.path.join(directory, cls.__mbed_app_config_name)
            if os.path.isfile(full_path):
                if app_config_location is not None:
                    raise ConfigException("Duplicate '%s' file in '%s' and '%s'"
                                            % (cls.__mbed_app_config_name,
                                               cls.app_config_location, full_path))
                else:
                    app_config_location = full_path
        return app_config_location

    def format_validation_error(self, error, path):
        if error.context:
            return self.format_validation_error(error.context[0], path)
        else:
            return "in {} element {}: {}".format(
                path, ".".join(p for p in error.absolute_path),
                error.message.replace('u\'','\''))

    def __init__(self, tgt, top_level_dirs=None, app_config=None):
        """Construct a mbed configuration

        Positional arguments:
        target - the name of the mbed target used for this configuration
                 instance

        Keyword argumets:
        top_level_dirs - a list of top level source directories (where
                         mbed_app_config.json could be found)
        app_config - location of a chosen mbed_app.json file

        NOTE: Construction of a Config object will look for the application
        configuration file in top_level_dirs. If found once, it'll parse it.
        top_level_dirs may be None (in this case, the constructor will not
        search for a configuration file).
        """
        config_errors = []
        self.config_errors = []
        self.app_config_location = app_config
        if self.app_config_location is None and top_level_dirs:
            self.app_config_location = self.find_app_config(top_level_dirs)
        try:
            self.app_config_data = json_file_to_dict(self.app_config_location) \
                                   if self.app_config_location else {}
        except ValueError as exc:
            self.app_config_data = {}
            config_errors.append(
                ConfigException("Could not parse mbed app configuration from %s"
                                % self.app_config_location))


        if self.app_config_location is not None:
            # Validate the format of the JSON file based on schema_app.json
            schema_root = os.path.dirname(os.path.abspath(__file__))
            schema_path = os.path.join(schema_root, "schema_app.json")
            schema      = json_file_to_dict(schema_path)

            url = moves.urllib.request.pathname2url(schema_path)
            uri = moves.urllib_parse.urljoin("file://", url)

            resolver = RefResolver(uri, schema)
            validator = Draft4Validator(schema, resolver=resolver)

            errors = sorted(validator.iter_errors(self.app_config_data))

            if errors:
                raise ConfigException("; ".join(
                    self.format_validation_error(x, self.app_config_location)
                    for x in errors))

        # Update the list of targets with the ones defined in the application
        # config, if applicable
        self.lib_config_data = {}
        # Make sure that each config is processed only once
        self.processed_configs = {}
        if isinstance(tgt, Target):
            self.target = tgt
        else:
            if tgt in TARGET_MAP:
                self.target = TARGET_MAP[tgt]
            else:
                self.target = generate_py_target(
                    self.app_config_data.get("custom_targets", {}), tgt)
        self.target = deepcopy(self.target)
        self.target_labels = self.target.labels
        for override in BOOTLOADER_OVERRIDES:
            _, attr = override.split(".")
            setattr(self.target, attr, None)

        self.cumulative_overrides = {key: ConfigCumulativeOverride(key)
                                     for key in CUMULATIVE_ATTRIBUTES}

        self._process_config_and_overrides(self.app_config_data, {}, "app",
                                           "application")
        self.config_errors = config_errors

    def add_config_files(self, flist):
        """Add configuration files

        Positional arguments:
        flist - a list of files to add to this configuration
        """
        for config_file in flist:
            if not config_file.endswith(self.__mbed_lib_config_name):
                continue
            full_path = os.path.normpath(os.path.abspath(config_file))
            # Check that we didn't already process this file
            if full_path in self.processed_configs:
                continue
            self.processed_configs[full_path] = True
            # Read the library configuration and add a "__full_config_path"
            # attribute to it
            try:
                cfg = json_file_to_dict(config_file)
            except ValueError as exc:
                raise ConfigException(str(exc))

            # Validate the format of the JSON file based on the schema_lib.json
            schema_root = os.path.dirname(os.path.abspath(__file__))
            schema_path = os.path.join(schema_root, "schema_lib.json")
            schema_file = json_file_to_dict(schema_path)

            url = moves.urllib.request.pathname2url(schema_path)
            uri = moves.urllib_parse.urljoin("file://", url)

            resolver = RefResolver(uri, schema_file)
            validator = Draft4Validator(schema_file, resolver=resolver)

            errors = sorted(validator.iter_errors(cfg))

            if errors:
                raise ConfigException("; ".join(
                    self.format_validation_error(x, config_file)
                    for x in errors))

            cfg["__config_path"] = full_path

            # If there's already a configuration for a module with the same
            # name, exit with error
            if cfg["name"] in self.lib_config_data:
                raise ConfigException(
                    "Library name '%s' is not unique (defined in '%s' and '%s')"
                    % (cfg["name"], full_path,
                       self.lib_config_data[cfg["name"]]["__config_path"]))
            self.lib_config_data[cfg["name"]] = cfg

    @property
    def has_regions(self):
        """Does this config have regions defined?"""
        for override in ROM_OVERRIDES:
            _, attr = override.split(".")
            if getattr(self.target, attr, None):
                return True
        return False

    @property
    def has_ram_regions(self):
        """Does this config have regions defined?"""
        for override in RAM_OVERRIDES:
            _, attr = override.split(".")
            if getattr(self.target, attr, None):
                return True
        return False

    @property
    def sectors(self):
        """Return a list of tuples of sector start,size"""
        cache = Cache(False, False)
        if self.target.device_name not in cache.index:
            raise ConfigException("Bootloader not supported on this target: "
                                  "targets.json `device_name` not found in "
                                  "arm_pack_manager index.")
        cmsis_part = cache.index[self.target.device_name]
        sectors = cmsis_part['sectors']
        if sectors:
            return sectors
        raise ConfigException("No sector info available")

    def _get_cmsis_part(self):
        if not getattr(self.target, "bootloader_supported", False):
            raise ConfigException("Bootloader not supported on this target.")
        if not hasattr(self.target, "device_name"):
            raise ConfigException("Bootloader not supported on this target: "
                                  "targets.json `device_name` not specified.")
        cache = Cache(False, False)
        if self.target.device_name not in cache.index:
            raise ConfigException("Bootloader not supported on this target: "
                                  "targets.json `device_name` not found in "
                                  "arm_pack_manager index.")
        return cache.index[self.target.device_name]

    def _get_mem_specs(self, memories, cmsis_part, exception_text):
        for memory in memories:
            try:
                size = cmsis_part['memory'][memory]['size']
                start = cmsis_part['memory'][memory]['start']
                return (start, size)
            except KeyError:
                continue
        raise ConfigException(exception_text)

    @property
    def rom(self):
        """Get rom information as a pair of start_addr, size"""
        # Override rom_start/rom_size
        #
        # This is usually done for a target which:
        # 1. Doesn't support CMSIS pack, or
        # 2. Supports TrustZone and user needs to change its flash partition
        cmsis_part = self._get_cmsis_part()
        rom_start, rom_size = self._get_mem_specs(
            ["IROM1", "PROGRAM_FLASH"],
            cmsis_part,
            "Not enough information in CMSIS packs to build a bootloader "
            "project"
        )
        rom_start = int(getattr(self.target, "mbed_rom_start", False) or rom_start, 0)
        rom_size = int(getattr(self.target, "mbed_rom_size", False) or rom_size, 0)
        return (rom_start, rom_size)

    @property
    def ram_regions(self):
        """Generate a list of ram regions from the config"""
        cmsis_part = self._get_cmsis_part()
        ram_start, ram_size = self._get_mem_specs(
            ["IRAM1", "SRAM0"],
            cmsis_part,
            "Not enough information in CMSIS packs to build a ram sharing project"
        )
        # Override ram_start/ram_size
        #
        # This is usually done for a target which:
        # 1. Doesn't support CMSIS pack, or
        # 2. Supports TrustZone and user needs to change its flash partition
        ram_start = getattr(self.target, "mbed_ram_start", False) or ram_start
        ram_size = getattr(self.target, "mbed_ram_size", False) or ram_size
        return [RamRegion("application_ram", int(ram_start, 0), int(ram_size, 0), True)]

    @property
    def regions(self):
        """Generate a list of regions from the config"""
        if  ((self.target.bootloader_img or self.target.restrict_size) and
             (self.target.mbed_app_start or self.target.mbed_app_size)):
            raise ConfigException(
                "target.bootloader_img and target.restirct_size are "
                "incompatible with target.mbed_app_start and "
                "target.mbed_app_size")
        if self.target.bootloader_img or self.target.restrict_size:
            return self._generate_bootloader_build(*self.rom)
        else:
            return self._generate_linker_overrides(*self.rom)

    @staticmethod
    def header_member_size(member):
        _, _, subtype, _ = member
        try:
            return int(subtype[:-2]) // 8
        except:
            if subtype.startswith("CRCITT32"):
                return 32 // 8
            elif subtype == "SHA256":
                return 256 // 8
            elif subtype == "SHA512":
                return 512 // 8
            else:
                raise ValueError("target.header_format: subtype %s is not "
                                 "understood" % subtype)

    @staticmethod
    def _header_size(format):
        return sum(Config.header_member_size(m) for m in format)

    def _make_header_region(self, start, header_format, offset=None):
        size = self._header_size(header_format)
        region = Region("header", start, size, False, None)
        start += size
        start = ((start + 7) // 8) * 8
        return (start, region)

    @staticmethod
    def _assign_new_offset(rom_start, start, new_offset, region_name):
        newstart = rom_start + integer(new_offset, 0)
        if newstart < start:
            raise ConfigException(
                "Can not place % region inside previous region" % region_name)
        return newstart

    def _generate_bootloader_build(self, rom_start, rom_size):
        start = rom_start
        rom_end = rom_start + rom_size
        if self.target.bootloader_img:
            if isabs(self.target.bootloader_img):
                filename = self.target.bootloader_img
            else:
                basedir = abspath(dirname(self.app_config_location))
                filename = join(basedir, self.target.bootloader_img)
            if not exists(filename):
                raise ConfigException("Bootloader %s not found" % filename)
            part = intelhex_offset(filename, offset=rom_start)
            if part.minaddr() != rom_start:
                raise ConfigException("bootloader executable does not "
                                      "start at 0x%x" % rom_start)
            part_size = (part.maxaddr() - part.minaddr()) + 1
            part_size = Config._align_ceiling(rom_start + part_size, self.sectors) - rom_start
            yield Region("bootloader", rom_start, part_size, False,
                         filename)
            start = rom_start + part_size
            if self.target.header_format:
                if self.target.header_offset:
                    start = self._assign_new_offset(
                        rom_start, start, self.target.header_offset, "header")
                start, region = self._make_header_region(
                    start, self.target.header_format)
                yield region._replace(filename=self.target.header_format)
        if self.target.restrict_size is not None:
            new_size = int(self.target.restrict_size, 0)
            new_size = Config._align_floor(start + new_size, self.sectors) - start
            yield Region("application", start, new_size, True, None)
            start += new_size
            if self.target.header_format and not self.target.bootloader_img:
                if self.target.header_offset:
                    start = self._assign_new_offset(
                        rom_start, start, self.target.header_offset, "header")
                start, region = self._make_header_region(
                    start, self.target.header_format)
                yield region
            if self.target.app_offset:
                start = self._assign_new_offset(
                    rom_start, start, self.target.app_offset, "application")
            yield Region("post_application", start, rom_end - start,
                         False, None)
        else:
            if self.target.app_offset:
                start = self._assign_new_offset(
                    rom_start, start, self.target.app_offset, "application")
            yield Region("application", start, rom_end - start,
                         True, None)
        if start > rom_start + rom_size:
            raise ConfigException("Not enough memory on device to fit all "
                                  "application regions")
    
    @staticmethod
    def _find_sector(address, sectors):
        target_size = -1
        target_start = -1
        for (start, size) in sectors:
            if address < start:
                break
            target_start = start
            target_size = size
        if (target_size < 0):
            raise ConfigException("No valid sector found")
        return target_start, target_size
        
    @staticmethod
    def _align_floor(address, sectors):
        target_start, target_size = Config._find_sector(address, sectors)
        sector_num = (address - target_start) // target_size
        return target_start + (sector_num * target_size)
    
    @staticmethod
    def _align_ceiling(address, sectors):
        target_start, target_size = Config._find_sector(address, sectors)
        sector_num = ((address - target_start) + target_size - 1) // target_size
        return target_start + (sector_num * target_size)

    @property
    def report(self):
        return {'app_config': self.app_config_location,
                'library_configs': map(relpath, self.processed_configs.keys())}

    def _generate_linker_overrides(self, rom_start, rom_size):
        if self.target.mbed_app_start is not None:
            start = int(self.target.mbed_app_start, 0)
        else:
            start = rom_start
        if self.target.mbed_app_size is not None:
            size = int(self.target.mbed_app_size, 0)
        else:
            size = (rom_size + rom_start) - start
        if start < rom_start:
            raise ConfigException("Application starts before ROM")
        if size + start > rom_size + rom_start:
            raise ConfigException("Application ends after ROM")
        yield Region("application", start, size, True, None)

    def _process_config_and_overrides(self, data, params, unit_name, unit_kind):
        """Process "config_parameters" and "target_config_overrides" into a
        given dictionary

        Positional arguments:
        data - the configuration data of the library/appliation
        params - storage for the discovered configuration parameters
        unit_name - the unit (library/application) that defines this parameter
        unit_kind - the kind of the unit ("library" or "application")
        """
        _process_config_parameters(data.get("config", {}), params, unit_name,
                                   unit_kind)
        for label, overrides in data.get("target_overrides", {}).items():
            # If the label is defined by the target or it has the special value
            # "*", process the overrides
            if (label == '*') or (label in self.target_labels):
                # Check for invalid cumulative overrides in libraries
                if (unit_kind == 'library' and
                    any(attr.startswith('target.extra_labels') for attr
                        in overrides.keys())):
                    raise ConfigException(
                        "Target override 'target.extra_labels' in " +
                        ConfigParameter.get_display_name(unit_name, unit_kind,
                                                         label) +
                        " is only allowed at the application level")

                # Parse out cumulative overrides
                for attr, cumulatives in self.cumulative_overrides.items():
                    if 'target.'+attr in overrides:
                        key = 'target.' + attr
                        if not isinstance(overrides[key], list):
                            raise ConfigException(
                                "The value of %s.%s is not of type %s" %
                                (unit_name, "target_overrides." + key,
                                 "list"))
                        cumulatives.strict_cumulative_overrides(overrides[key])
                        del overrides[key]

                    if 'target.'+attr+'_add' in overrides:
                        key = 'target.' + attr + "_add"
                        if not isinstance(overrides[key], list):
                            raise ConfigException(
                                "The value of %s.%s is not of type %s" %
                                (unit_name, "target_overrides." + key,
                                 "list"))
                        cumulatives.add_cumulative_overrides(overrides[key])
                        del overrides[key]

                    if 'target.'+attr+'_remove' in overrides:
                        key = 'target.' + attr + "_remove"
                        if not isinstance(overrides[key], list):
                            raise ConfigException(
                                "The value of %s.%s is not of type %s" %
                                (unit_name, "target_overrides." + key,
                                 "list"))
                        cumulatives.remove_cumulative_overrides(overrides[key])
                        del overrides[key]

                # Consider the others as overrides
                for name, val in overrides.items():
                    if (name in PATH_OVERRIDES and "__config_path" in data):
                        val = os.path.join(
                            os.path.dirname(data["__config_path"]), val)

                    # Get the full name of the parameter
                    full_name = ConfigParameter.get_full_name(name, unit_name,
                                                              unit_kind, label)
                    if full_name in params:
                        params[full_name].set_value(val, unit_name, unit_kind,
                                                    label)
                    elif (name.startswith("target.") and
                        (unit_kind is "application" or
                         name in BOOTLOADER_OVERRIDES)):
                        _, attribute = name.split(".")
                        setattr(self.target, attribute, val)
                        continue
                    else:
                        self.config_errors.append(
                            UndefinedParameter(
                                full_name, unit_name, unit_kind, label))

        for cumulatives in self.cumulative_overrides.values():
            cumulatives.update_target(self.target)

        return params

    def get_target_config_data(self):
        """Read and interpret configuration data defined by targets.

        We consider the resolution order for our target and sort it by level
        reversed, so that we first look at the top level target (the parent),
        then its direct children, then the children of those children and so on,
        until we reach self.target
        TODO: this might not work so well in some multiple inheritance scenarios
        At each step, look at two keys of the target data:
          - config_parameters: used to define new configuration parameters
          - config_overrides: used to override already defined configuration
                              parameters

        Arguments: None
        """
        params, json_data = {}, self.target.json_data
        resolution_order = [e[0] for e
                            in sorted(
                                self.target.resolution_order,
                                key=lambda e: e[1], reverse=True)]
        for tname in resolution_order:
            # Read the target data directly from its description
            target_data = json_data[tname]
            # Process definitions first
            _process_config_parameters(target_data.get("config", {}), params,
                                       tname, "target")
            # Then process overrides
            for name, val in target_data.get("overrides", {}).items():
                full_name = ConfigParameter.get_full_name(name, tname, "target")
                # If the parameter name is not defined or if there isn't a path
                # from this target to the target where the parameter was defined
                # in the target inheritance tree, raise an error We need to use
                # 'defined_by[7:]' to remove the "target:" prefix from
                # defined_by
                rel_names = [tgt for tgt, _ in
                             get_resolution_order(self.target.json_data, tname,
                                                  [])]
                if full_name in BOOTLOADER_OVERRIDES:
                    continue
                if (full_name not in params) or \
                   (params[full_name].defined_by[7:] not in rel_names):
                    raise UndefinedParameter(name, tname, "target", "")
                # Otherwise update the value of the parameter
                params[full_name].set_value(val, tname, "target")
        return params

    def get_lib_config_data(self, target_data):
        """ Read and interpret configuration data defined by libraries. It is
        assumed that "add_config_files" above was already called and the library
        configuration data exists in self.lib_config_data

        Arguments: None
        """
        macros = {}
        for lib_name, lib_data in self.lib_config_data.items():
            self._process_config_and_overrides(
                lib_data, target_data, lib_name, "library")
            _process_macros(lib_data.get("macros", []), macros, lib_name,
                            "library")
        return target_data, macros

    def get_app_config_data(self, params, macros):
        """ Read and interpret the configuration data defined by the target. The
        target can override any configuration parameter, as well as define its
        own configuration data.

        Positional arguments.
        params - the dictionary with configuration parameters found so far (in
                 the target and in libraries)
        macros - the list of macros defined in the configuration
        """
        app_cfg = self.app_config_data
        # The application can have a "config_parameters" and a
        # "target_config_overrides" section just like a library
        self._process_config_and_overrides(app_cfg, params, "app",
                                           "application")
        # The application can also defined macros
        _process_macros(app_cfg.get("macros", []), macros, "app",
                        "application")

    def get_config_data(self):
        """ Return the configuration data in two parts: (params, macros)
        params - a dictionary with mapping a name to a ConfigParam
        macros - the list of macros defined with "macros" in libraries and in
                 the application (as ConfigMacro instances)

        Arguments: None
        """
        all_params = self.get_target_config_data()
        lib_params, macros = self.get_lib_config_data(all_params)
        self.get_app_config_data(lib_params, macros)
        return lib_params, macros

    @staticmethod
    def _check_required_parameters(params):
        """Check that there are no required parameters without a value

        Positional arguments:
        params - the list of parameters to check

        NOTE: This function does not return. Instead, it throws a
        ConfigException when any of the required parameters are missing values
        """
        for param in params.values():
            if param.required and (param.value is None):
                raise ConfigException("Required parameter '" + param.name +
                                      "' defined by '" + param.defined_by +
                                      "' doesn't have a value")

    @staticmethod
    def parameters_to_macros(params):
        """ Encode the configuration parameters as C macro definitions.

        Positional arguments:
        params - a dictionary mapping a name to a ConfigParameter

        Return: a list of strings that encode the configuration parameters as
        C pre-processor macros
        """
        return ['%s=%s' % (m.macro_name, m.value) for m in params.values()
                if m.value is not None]

    @staticmethod
    def config_macros_to_macros(macros):
        """ Return the macro definitions generated for a dictionary of
        ConfigMacros (as returned by get_config_data).

        Positional arguments:
        params - a dictionary mapping a name to a ConfigMacro instance

        Return: a list of strings that are the C pre-processor macros
        """
        return [m.name for m in macros.values()]

    @staticmethod
    def config_to_macros(config):
        """Convert the configuration data to a list of C macros

        Positional arguments:
        config - configuration data as (ConfigParam instances, ConfigMacro
                 instances) tuple (as returned by get_config_data())
        """
        params, macros = config[0], config[1]
        Config._check_required_parameters(params)
        return Config.config_macros_to_macros(macros) + \
            Config.parameters_to_macros(params)

    def get_config_data_macros(self):
        """ Convert a Config object to a list of C macros

        Arguments: None
        """
        return self.config_to_macros(self.get_config_data())

    def get_features(self):
        """ Extract any features from the configuration data

        Arguments: None
        """
        params, _ = self.get_config_data()
        self._check_required_parameters(params)
        self.cumulative_overrides['features']\
            .update_target(self.target)

        return self.target.features

    def validate_config(self):
        """ Validate configuration settings. This either returns True or
        raises an exception

        Arguments: None
        """
        params, _ = self.get_config_data()
        for error in self.config_errors:
            if  (isinstance(error, UndefinedParameter) and
                 error.param in params):
                continue
            else:
                raise error
        return True


    @property
    def name(self):
        if "artifact_name" in self.app_config_data:
            return self.app_config_data["artifact_name"]
        else:
            return None

    def load_resources(self, resources):
        """ Load configuration data from a Resources instance and expand it
        based on defined features.

        Positional arguments:
        resources - the resources object to load from and expand
        """
        # Update configuration files until added features creates no changes
        prev_features = set()
        while True:
            # Add/update the configuration with any .json files found while
            # scanning
            self.add_config_files(
                f.path for f in resources.get_file_refs(FileType.JSON)
            )

            # Add features while we find new ones
            features = set(self.get_features())
            if features == prev_features:
                break

            resources.add_features(features)

            prev_features = features
        self.validate_config()

        if  (hasattr(self.target, "release_versions") and
             "5" not in self.target.release_versions and
             "rtos" in self.lib_config_data):
            raise NotSupportedException("Target does not support mbed OS 5")

    @staticmethod
    def config_to_header(config, fname=None):
        """ Convert the configuration data to the content of a C header file,
        meant to be included to a C/C++ file. The content is returned as a
        string.

        Positional arguments:
        config - configuration data as (ConfigParam instances, ConfigMacro
                 instances) tuple (as returned by get_config_data())

        Keyword arguments:
        fname -  also write the content is to the file called "fname".
                 WARNING: if 'fname' names an existing file, it will be
                 overwritten!
        """
        params, macros = config[0] or {}, config[1] or {}
        Config._check_required_parameters(params)
        params_with_values = [p for p in params.values() if p.value is not None]
        ctx = {
            "cfg_params": sorted([
                (p.macro_name, str(p.value), p.set_by)
                for p in params_with_values
            ]),
            "macros": sorted([
                (m.macro_name, str(m.macro_value or ""), m.defined_by)
                for m in macros.values()
            ]),
            "name_len":  max([len(m.macro_name) for m in macros.values()] +
                             [len(m.macro_name) for m in params_with_values]
                             + [0]),
            "val_len" : max([len(str(m.value)) for m in params_with_values] +
                            [len(m.macro_value or "") for m in macros.values()]
                            + [0]),
        }
        jinja_loader = FileSystemLoader(dirname(abspath(__file__)))
        jinja_environment = Environment(loader=jinja_loader,
                                        undefined=StrictUndefined)
        header_data = jinja_environment.get_template("header.tmpl").render(ctx)
        # If fname is given, write "header_data" to it
        if fname:
            with open(fname, "w+") as file_desc:
                file_desc.write(header_data)
        return header_data

    def get_config_data_header(self, fname=None):
        """ Convert a Config instance to the content of a C header file, meant
        to be included to a C/C++ file. The content is returned as a string.

        Keyword arguments:
        fname - also write the content to the file called "fname".
                WARNING: if 'fname' names an existing file, it will be
                overwritten!
        """
        return self.config_to_header(self.get_config_data(), fname)
