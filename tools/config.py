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

from copy import deepcopy
import os
import sys
from collections import namedtuple
from os.path import splitext
from intelhex import IntelHex
# Implementation of mbed configuration mechanism
from tools.utils import json_file_to_dict, intelhex_offset
from tools.arm_pack_manager import Cache
from tools.targets import CUMULATIVE_ATTRIBUTES, TARGET_MAP, \
    generate_py_target, get_resolution_order

# Base class for all configuration exceptions
class ConfigException(Exception):
    """Config system only exception. Makes it easier to distinguish config
    errors"""
    pass

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
        if (unit_kind == "library" and prefix != unit_name) or \
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


def check_dict_types(dict, type_dict, dict_loc):
    for key, value in dict.iteritems():
        if not isinstance(value, type_dict[key]):
            raise ConfigException("The value of %s.%s is not of type %s" %
                                  (dict_loc, key, type_dict[key].__name__))

Region = namedtuple("Region", "name start size active filename")

class Config(object):
    """'Config' implements the mbed configuration mechanism"""

    # Libraries and applications have different names for their configuration
    # files
    __mbed_app_config_name = "mbed_app.json"
    __mbed_lib_config_name = "mbed_lib.json"

    # Allowed keys in configuration dictionaries, and their types
    # (targets can have any kind of keys, so this validation is not applicable
    # to them)
    __allowed_keys = {
        "library": {"name": str, "config": dict, "target_overrides": dict,
                    "macros": list, "__config_path": str},
        "application": {"config": dict, "target_overrides": dict,
                        "macros": list, "__config_path": str}
    }

    __unused_overrides = set(["target.bootloader_img", "target.restrict_size"])

    # Allowed features in configurations
    __allowed_features = [
        "UVISOR", "BLE", "CLIENT", "IPV4", "LWIP", "COMMON_PAL", "STORAGE", "NANOSTACK",
        # Nanostack configurations
        "LOWPAN_BORDER_ROUTER", "LOWPAN_HOST", "LOWPAN_ROUTER", "NANOSTACK_FULL", "THREAD_BORDER_ROUTER", "THREAD_END_DEVICE", "THREAD_ROUTER", "ETHERNET_HOST"
        ]

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
        app_config_location = app_config
        if app_config_location is None:
            for directory in top_level_dirs or []:
                full_path = os.path.join(directory, self.__mbed_app_config_name)
                if os.path.isfile(full_path):
                    if app_config_location is not None:
                        raise ConfigException("Duplicate '%s' file in '%s' and '%s'"
                                              % (self.__mbed_app_config_name,
                                                 app_config_location, full_path))
                    else:
                        app_config_location = full_path
        try:
            self.app_config_data = json_file_to_dict(app_config_location) \
                                   if app_config_location else {}
        except ValueError as exc:
            self.app_config_data = {}
            config_errors.append(
                ConfigException("Could not parse mbed app configuration from %s"
                                % app_config_location))

        # Check the keys in the application configuration data
        unknown_keys = set(self.app_config_data.keys()) - \
                       set(self.__allowed_keys["application"].keys())
        if unknown_keys:
            raise ConfigException("Unknown key(s) '%s' in %s" %
                                  (",".join(unknown_keys),
                                   self.__mbed_app_config_name))
        check_dict_types(self.app_config_data, self.__allowed_keys["application"],
                         "app-config")
        # Update the list of targets with the ones defined in the application
        # config, if applicable
        self.lib_config_data = {}
        # Make sure that each config is processed only once
        self.processed_configs = {}
        if isinstance(tgt, basestring):
            if tgt in TARGET_MAP:
                self.target = TARGET_MAP[tgt]
            else:
                self.target = generate_py_target(
                    self.app_config_data.get("custom_targets", {}), tgt)

        else:
            self.target = tgt
        self.target = deepcopy(self.target)
        self.target_labels = self.target.labels

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
            if self.processed_configs.has_key(full_path):
                continue
            self.processed_configs[full_path] = True
            # Read the library configuration and add a "__full_config_path"
            # attribute to it
            try:
                cfg = json_file_to_dict(config_file)
            except ValueError as exc:
                sys.stderr.write(str(exc) + "\n")
                continue

            cfg["__config_path"] = full_path

            if "name" not in cfg:
                raise ConfigException(
                    "Library configured at %s has no name field." % full_path)
            # If there's already a configuration for a module with the same
            # name, exit with error
            if self.lib_config_data.has_key(cfg["name"]):
                raise ConfigException(
                    "Library name '%s' is not unique (defined in '%s' and '%s')"
                    % (cfg["name"], full_path,
                       self.lib_config_data[cfg["name"]]["__config_path"]))
            self.lib_config_data[cfg["name"]] = cfg

    @property
    def has_regions(self):
        """Does this config have regions defined?"""
        if 'target_overrides' in self.app_config_data:
            target_overrides = self.app_config_data['target_overrides'].get(
                self.target.name, {})
            return ('target.bootloader_img' in target_overrides or
                    'target.restrict_size' in target_overrides)
        else:
            return False

    @property
    def regions(self):
        """Generate a list of regions from the config"""
        if not self.target.bootloader_supported:
            raise ConfigException("Bootloader not supported on this target.")
        cmsis_part = Cache(False, False).index[self.target.device_name]
        start = 0
        target_overrides = self.app_config_data['target_overrides'].get(
            self.target.name, {})
        try:
            rom_size = int(cmsis_part['memory']['IROM1']['size'], 0)
            rom_start = int(cmsis_part['memory']['IROM1']['start'], 0)
        except KeyError:
            raise ConfigException("Not enough information in CMSIS packs to "
                                  "build a bootloader project")
        if 'target.bootloader_img' in target_overrides:
            filename = target_overrides['target.bootloader_img']
            part = intelhex_offset(filename, offset=rom_start)
            if part.minaddr() != rom_start:
                raise ConfigException("bootloader executable does not "
                                      "start at 0x%x" % rom_start)
            part_size = (part.maxaddr() - part.minaddr()) + 1
            yield Region("bootloader", rom_start + start, part_size, False,
                         filename)
            start += part_size
        if 'target.restrict_size' in target_overrides:
            new_size = int(target_overrides['target.restrict_size'], 0)
            yield Region("application", rom_start + start, new_size, True, None)
            start += new_size
            yield Region("post_application", rom_start +start, rom_size - start,
                         False, None)
        else:
            yield Region("application", rom_start + start, rom_size - start,
                         True, None)
        if start > rom_size:
            raise ConfigException("Not enough memory on device to fit all "
                                  "application regions")

    def _process_config_and_overrides(self, data, params, unit_name, unit_kind):
        """Process "config_parameters" and "target_config_overrides" into a
        given dictionary

        Positional arguments:
        data - the configuration data of the library/appliation
        params - storage for the discovered configuration parameters
        unit_name - the unit (library/application) that defines this parameter
        unit_kind - the kind of the unit ("library" or "application")
        """
        self.config_errors = []
        _process_config_parameters(data.get("config", {}), params, unit_name,
                                   unit_kind)
        for label, overrides in data.get("target_overrides", {}).items():
            # If the label is defined by the target or it has the special value
            # "*", process the overrides
            if (label == '*') or (label in self.target_labels):
                # Check for invalid cumulative overrides in libraries
                if (unit_kind == 'library' and
                    any(attr.startswith('target.extra_labels') for attr
                        in overrides.iterkeys())):
                    raise ConfigException(
                        "Target override 'target.extra_labels' in " +
                        ConfigParameter.get_display_name(unit_name, unit_kind,
                                                         label) +
                        " is only allowed at the application level")

                # Parse out cumulative overrides
                for attr, cumulatives in self.cumulative_overrides.iteritems():
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
                    # Get the full name of the parameter
                    full_name = ConfigParameter.get_full_name(name, unit_name,
                                                              unit_kind, label)
                    if full_name in params:
                        params[full_name].set_value(val, unit_name, unit_kind,
                                                    label)
                    elif name in self.__unused_overrides:
                        pass
                    else:
                        self.config_errors.append(
                            ConfigException(
                                "Attempt to override undefined parameter" +
                                (" '%s' in '%s'"
                                 % (full_name,
                                    ConfigParameter.get_display_name(unit_name,
                                                                     unit_kind,
                                                                     label)))))

        for cumulatives in self.cumulative_overrides.itervalues():
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
                if full_name in self.__unused_overrides:
                    continue
                if (full_name not in params) or \
                   (params[full_name].defined_by[7:] not in rel_names):
                    raise ConfigException(
                        "Attempt to override undefined parameter '%s' in '%s'"
                        % (name,
                           ConfigParameter.get_display_name(tname, "target")))
                # Otherwise update the value of the parameter
                params[full_name].set_value(val, tname, "target")
        return params

    def get_lib_config_data(self):
        """ Read and interpret configuration data defined by libraries. It is
        assumed that "add_config_files" above was already called and the library
        configuration data exists in self.lib_config_data

        Arguments: None
        """
        all_params, macros = {}, {}
        for lib_name, lib_data in self.lib_config_data.items():
            unknown_keys = (set(lib_data.keys()) -
                            set(self.__allowed_keys["library"].keys()))
            if unknown_keys:
                raise ConfigException("Unknown key(s) '%s' in %s" %
                                      (",".join(unknown_keys), lib_name))
            check_dict_types(lib_data, self.__allowed_keys["library"], lib_name)
            all_params.update(self._process_config_and_overrides(lib_data, {},
                                                                 lib_name,
                                                                 "library"))
            _process_macros(lib_data.get("macros", []), macros, lib_name,
                            "library")
        return all_params, macros

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
        lib_params, macros = self.get_lib_config_data()
        all_params.update(lib_params)
        self.get_app_config_data(all_params, macros)
        return all_params, macros

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

        for feature in self.target.features:
            if feature not in self.__allowed_features:
                raise ConfigException(
                    "Feature '%s' is not a supported features" % feature)

        return self.target.features

    def validate_config(self):
        """ Validate configuration settings. This either returns True or
        raises an exception

        Arguments: None
        """
        if self.config_errors:
            raise self.config_errors[0]
        return True


    def load_resources(self, resources):
        """ Load configuration data from a Resources instance and expand it
        based on defined features.

        Positional arguments:
        resources - the resources object to load from and expand
        """
        # Update configuration files until added features creates no changes
        prev_features = set()
        self.validate_config()
        while True:
            # Add/update the configuration with any .json files found while
            # scanning
            self.add_config_files(resources.json_files)

            # Add features while we find new ones
            features = set(self.get_features())
            if features == prev_features:
                break

            for feature in features:
                if feature in resources.features:
                    resources.add(resources.features[feature])

            prev_features = features
        self.validate_config()

        return resources

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
        params, macros = config[0], config[1]
        Config._check_required_parameters(params)
        header_data = "// Automatically generated configuration file.\n"
        header_data += "// DO NOT EDIT, content will be overwritten.\n\n"
        header_data += "#ifndef __MBED_CONFIG_DATA__\n"
        header_data += "#define __MBED_CONFIG_DATA__\n\n"
        # Compute maximum length of macro names for proper alignment
        max_param_macro_name_len = (max([len(m.macro_name) for m
                                         in params.values()
                                         if m.value is not None])
                                    if params else 0)
        max_direct_macro_name_len = (max([len(m.macro_name) for m
                                         in macros.values()])
                                     if macros else 0)
        max_macro_name_len = max(max_param_macro_name_len,
                                 max_direct_macro_name_len)
        # Compute maximum length of macro values for proper alignment
        max_param_macro_val_len = (max([len(str(m.value)) for m
                                       in params.values()
                                       if m.value is not None])
                                   if params else 0)
        max_direct_macro_val_len = max([len(m.macro_value or "") for m
                                        in macros.values()]) if macros else 0
        max_macro_val_len = max(max_param_macro_val_len,
                                max_direct_macro_val_len)
        # Generate config parameters first
        if params:
            header_data += "// Configuration parameters\n"
            for macro in params.values():
                if macro.value is not None:
                    header_data += ("#define {0:<{1}} {2!s:<{3}} " +
                                    "// set by {4}\n")\
                        .format(macro.macro_name, max_macro_name_len,
                                macro.value, max_macro_val_len, macro.set_by)
        # Then macros
        if macros:
            header_data += "// Macros\n"
            for macro in macros.values():
                if macro.macro_value:
                    header_data += ("#define {0:<{1}} {2!s:<{3}}" +
                                    " // defined by {4}\n")\
                        .format(macro.macro_name, max_macro_name_len,
                                macro.macro_value, max_macro_val_len,
                                macro.defined_by)
                else:
                    header_data += ("#define {0:<{1}}" +
                                    " // defined by {2}\n")\
                        .format(macro.macro_name,
                                max_macro_name_len + max_macro_val_len + 1,
                                macro.defined_by)
        header_data += "\n#endif\n"
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
