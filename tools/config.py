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

# Implementation of mbed configuration mechanism
from copy import deepcopy
from collections import OrderedDict
from tools.utils import json_file_to_dict, ToolException
from tools.targets import Target
import os

# Base class for all configuration exceptions
class ConfigException(Exception):
    pass

# This class keeps information about a single configuration parameter
class ConfigParameter:
    # name: the name of the configuration parameter
    # data: the data associated with the configuration parameter
    # unit_name: the unit (target/library/application) that defines this parameter
    # unit_ kind: the kind of the unit ("target", "library" or "application")
    def __init__(self, name, data, unit_name, unit_kind):
        self.name = self.get_full_name(name, unit_name, unit_kind, allow_prefix = False)
        self.defined_by = self.get_display_name(unit_name, unit_kind)
        self.set_by = self.defined_by
        self.help_text = data.get("help", None)
        self.value = data.get("value", None)
        self.required = data.get("required", False)
        self.macro_name = data.get("macro_name", "MBED_CONF_%s" % self.sanitize(self.name.upper()))

    # Return the full (prefixed) name of a parameter.
    # If the parameter already has a prefix, check if it is valid
    # name: the simple (unqualified) name of the parameter
    # unit_name: the unit (target/library/application) that defines this parameter
    # unit_kind: the kind of the unit ("target", "library" or "application")
    # label: the name of the label in the 'target_config_overrides' section (optional)
    # allow_prefix: True to allo the original name to have a prefix, False otherwise
    @staticmethod
    def get_full_name(name, unit_name, unit_kind, label = None, allow_prefix = True):
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
            raise ConfigException("Invalid parameter name '%s' in '%s'" % (name, ConfigParameter.get_display_name(unit_name, unit_kind, label)))
        temp = name.split(".")
        # Check if the parameter syntax is correct (must be unit_name.parameter_name)
        if len(temp) != 2:
            raise ConfigException("Invalid parameter name '%s' in '%s'" % (name, ConfigParameter.get_display_name(unit_name, unit_kind, label)))
        prefix = temp[0]
        # Check if the given parameter prefix matches the expected prefix
        if (unit_kind == "library" and prefix != unit_name) or (unit_kind == "target" and prefix != "target"):
            raise ConfigException("Invalid prefix '%s' for parameter name '%s' in '%s'" % (prefix, name, ConfigParameter.get_display_name(unit_name, unit_kind, label)))
        return name

    # Return the name displayed for a unit when interogating the origin
    # and the last set place of a parameter
    # unit_name: the unit (target/library/application) that defines this parameter
    # unit_kind: the kind of the unit ("target", "library" or "application")
    # label: the name of the label in the 'target_config_overrides' section (optional)
    @staticmethod
    def get_display_name(unit_name, unit_kind, label = None):
        if unit_kind == "target":
            return "target:" + unit_name
        elif unit_kind == "application":
            return "application%s" % ("[%s]" % label if label else "")
        else: # library
            return "library:%s%s" % (unit_name, "[%s]" % label if label else "")

    # "Sanitize" a name so that it is a valid C macro name
    # Currently it simply replaces '.' and '-' with '_'
    # name: the un-sanitized name.
    @staticmethod
    def sanitize(name):
        return name.replace('.', '_').replace('-', '_')

    # Sets a value for this parameter, remember the place where it was set
    # value: the value of the parameter
    # unit_name: the unit (target/library/application) that defines this parameter
    # unit_ kind: the kind of the unit ("target", "library" or "application")
    # label: the name of the label in the 'target_config_overrides' section (optional)
    def set_value(self, value, unit_name, unit_kind, label = None):
        self.value = value
        self.set_by = self.get_display_name(unit_name, unit_kind, label)

    # Return the string representation of this configuration parameter
    def __str__(self):
        if self.value is not None:
            return '%s = %s (macro name: "%s")' % (self.name, self.value, self.macro_name)
        else:
            return '%s has no value' % self.name

    # Return a verbose description of this configuration paramater as a string
    def get_verbose_description(self):
        desc = "Name: %s%s\n" % (self.name, " (required parameter)" if self.required else "")
        if self.help_text:
            desc += "    Description: %s\n" % self.help_text
        desc += "    Defined by: %s\n" % self.defined_by
        if not self.value:
            return desc + "    No value set"
        desc += "    Macro name: %s\n" % self.macro_name
        desc += "    Value: %s (set by %s)" % (self.value, self.set_by)
        return desc

# A representation of a configuration macro. It handles both macros without a value (MACRO)
# and with a value (MACRO=VALUE)
class ConfigMacro:
    def __init__(self, name, unit_name, unit_kind):
        self.name = name
        self.defined_by = ConfigParameter.get_display_name(unit_name, unit_kind)
        if name.find("=") != -1:
            tmp = name.split("=")
            if len(tmp) != 2:
                raise ValueError("Invalid macro definition '%s' in '%s'" % (name, self.defined_by))
            self.macro_name = tmp[0]
        else:
            self.macro_name = name

# 'Config' implements the mbed configuration mechanism
class Config:
    # Libraries and applications have different names for their configuration files
    __mbed_app_config_name = "mbed_app.json"
    __mbed_lib_config_name = "mbed_lib.json"

    # Allowed keys in configuration dictionaries
    # (targets can have any kind of keys, so this validation is not applicable to them)
    __allowed_keys = {
        "library": set(["name", "config", "target_overrides", "macros", "__config_path"]),
        "application": set(["config", "custom_targets", "target_overrides", "macros", "__config_path"])
    }

    # The initialization arguments for Config are:
    #     target: the name of the mbed target used for this configuration instance
    #     top_level_dirs: a list of top level source directories (where mbed_abb_config.json could be found)
    # __init__ will look for the application configuration file in top_level_dirs.
    # If found once, it'll parse it and check if it has a custom_targets function.
    # If it does, it'll update the list of targets if need.
    # If found more than once, an exception is raised
    # top_level_dirs can be None (in this case, mbed_app_config.json will not be searched)
    def __init__(self, target, top_level_dirs = []):
        app_config_location = None
        for s in (top_level_dirs or []):
            full_path = os.path.join(s, self.__mbed_app_config_name)
            if os.path.isfile(full_path):
                if app_config_location is not None:
                    raise ConfigException("Duplicate '%s' file in '%s' and '%s'" % (self.__mbed_app_config_name, app_config_location, full_path))
                else:
                    app_config_location = full_path
        self.app_config_data = json_file_to_dict(app_config_location) if app_config_location else {}
        # Check the keys in the application configuration data
        unknown_keys = set(self.app_config_data.keys()) - self.__allowed_keys["application"]
        if unknown_keys:
            raise ConfigException("Unknown key(s) '%s' in %s" % (",".join(unknown_keys), self.__mbed_app_config_name))
        # Update the list of targets with the ones defined in the application config, if applicable
        Target.add_py_targets(self.app_config_data.get("custom_targets", {}))
        self.lib_config_data = {}
        # Make sure that each config is processed only once
        self.processed_configs = {}
        self.target = target if isinstance(target, str) else target.name
        self.target_labels = Target.get_target(self.target).get_labels()
        self.target_instance = Target.get_target(self.target)

    # Add one or more configuration files
    def add_config_files(self, flist):
        for f in flist:
            if not f.endswith(self.__mbed_lib_config_name):
                continue
            full_path = os.path.normpath(os.path.abspath(f))
            # Check that we didn't already process this file
            if self.processed_configs.has_key(full_path):
                continue
            self.processed_configs[full_path] = True
            # Read the library configuration and add a "__full_config_path" attribute to it
            cfg = json_file_to_dict(f)
            cfg["__config_path"] = full_path
            # If there's already a configuration for a module with the same name, exit with error
            if self.lib_config_data.has_key(cfg["name"]):
                raise ConfigException("Library name '%s' is not unique (defined in '%s' and '%s')" % (cfg["name"], full_path, self.lib_config_data[cfg["name"]]["__config_path"]))
            self.lib_config_data[cfg["name"]] = cfg

    # Helper function: process a "config_parameters" section in either a target, a library or the application
    # data: a dictionary with the configuration parameters
    # params: storage for the discovered configuration parameters
    # unit_name: the unit (target/library/application) that defines this parameter
    # unit_kind: the kind of the unit ("target", "library" or "application")
    def _process_config_parameters(self, data, params, unit_name, unit_kind):
        for name, v in data.items():
            full_name = ConfigParameter.get_full_name(name, unit_name, unit_kind)
            # If the parameter was already defined, raise an error
            if full_name in params:
                raise ConfigException("Parameter name '%s' defined in both '%s' and '%s'" % (name, ConfigParameter.get_display_name(unit_name, unit_kind), params[full_name].defined_by))
            # Otherwise add it to the list of known parameters
            # If "v" is not a dictionary, this is a shortcut definition, otherwise it is a full definition
            params[full_name] = ConfigParameter(name, v if isinstance(v, dict) else {"value": v}, unit_name, unit_kind)
        return params

    # Helper function: process "config_parameters" and "target_config_overrides" in a given dictionary
    # data: the configuration data of the library/appliation
    # params: storage for the discovered configuration parameters
    # unit_name: the unit (library/application) that defines this parameter
    # unit_kind: the kind of the unit ("library" or "application")
    def _process_config_and_overrides(self, data, params, unit_name, unit_kind):
        self._process_config_parameters(data.get("config", {}), params, unit_name, unit_kind)
        for label, overrides in data.get("target_overrides", {}).items():
            # If the label is defined by the target or it has the special value "*", process the overrides
            if (label == '*') or (label in self.target_labels):
                # Parse out cumulative attributes
                for attr in Target._Target__cumulative_attributes:
                    attrs = getattr(self.target_instance, attr)

                    if attr+'_add' in overrides:
                        attrs.extend(overrides[attr+'_add'])
                        del overrides[attr+'_add']

                    if attr+'_remove' in overrides:
                        for a in overrides[attr+'_remove']:
                            attrs.remove(a)
                        del overrides[attr+'_remove']

                    setattr(self.target_instance, attr, attrs)

                # Consider the others as overrides
                for name, v in overrides.items():
                    # Get the full name of the parameter
                    full_name = ConfigParameter.get_full_name(name, unit_name, unit_kind, label)
                    # If an attempt is made to override a parameter that isn't defined, raise an error
                    if not full_name in params:
                        raise ConfigException("Attempt to override undefined parameter '%s' in '%s'" % (full_name, ConfigParameter.get_display_name(unit_name, unit_kind, label)))
                    params[full_name].set_value(v, unit_name, unit_kind, label)
        return params

    # Read and interpret configuration data defined by targets
    def get_target_config_data(self):
        # We consider the resolution order for our target and sort it by level reversed,
        # so that we first look at the top level target (the parent), then its direct children,
        # then the children's children and so on, until we reach self.target
        # TODO: this might not work so well in some multiple inheritance scenarios
        # At each step, look at two keys of the target data:
        #   - config_parameters: used to define new configuration parameters
        #   - config_overrides: used to override already defined configuration parameters
        params, json_data = {}, Target.get_json_target_data()
        resolution_order = [e[0] for e in sorted(Target.get_target(self.target).resolution_order, key = lambda e: e[1], reverse = True)]
        for tname in resolution_order:
            # Read the target data directly from its description
            t = json_data[tname]
            # Process definitions first
            self._process_config_parameters(t.get("config", {}), params, tname, "target")
            # Then process overrides
            for name, v in t.get("overrides", {}).items():
                full_name = ConfigParameter.get_full_name(name, tname, "target")
                # If the parameter name is not defined or if there isn't a path from this target to the target where the
                # parameter was defined in the target inheritance tree, raise an error
                # We need to use 'defined_by[7:]' to remove the "target:" prefix from defined_by
                if (not full_name in params) or (not params[full_name].defined_by[7:] in Target.get_target(tname).resolution_order_names):
                    raise ConfigException("Attempt to override undefined parameter '%s' in '%s'" % (name, ConfigParameter.get_display_name(tname, "target")))
                # Otherwise update the value of the parameter
                params[full_name].set_value(v, tname, "target")
        return params

    # Helper function: process a macro definition, checking for incompatible duplicate definitions
    # mlist: list of macro names to process
    # macros: dictionary with currently discovered macros
    # unit_name: the unit (library/application) that defines this macro
    # unit_kind: the kind of the unit ("library" or "application")
    def _process_macros(self, mlist, macros, unit_name, unit_kind):
        for mname in mlist:
            m = ConfigMacro(mname, unit_name, unit_kind)
            if (m.macro_name in macros) and (macros[m.macro_name].name != mname):
                # Found an incompatible definition of the macro in another module, so raise an error
                full_unit_name = ConfigParameter.get_display_name(unit_name, unit_kind)
                raise ConfigException("Macro '%s' defined in both '%s' and '%s' with incompatible values" % (m.macro_name, macros[m.macro_name].defined_by, full_unit_name))
            macros[m.macro_name] = m

    # Read and interpret configuration data defined by libs
    # It is assumed that "add_config_files" above was already called and the library configuration data
    # exists in self.lib_config_data
    def get_lib_config_data(self):
        all_params, macros = {}, {}
        for lib_name, lib_data in self.lib_config_data.items():
            unknown_keys = set(lib_data.keys()) - self.__allowed_keys["library"]
            if unknown_keys:
                raise ConfigException("Unknown key(s) '%s' in %s" % (",".join(unknown_keys), lib_name))
            all_params.update(self._process_config_and_overrides(lib_data, {}, lib_name, "library"))
            self._process_macros(lib_data.get("macros", []), macros, lib_name, "library")
        return all_params, macros

    # Read and interpret the configuration data defined by the target
    # The target can override any configuration parameter, as well as define its own configuration data
    # params: the dictionary with configuration parameters found so far (in the target and in libraries)
    # macros: the list of macros defined in the configuration
    def get_app_config_data(self, params, macros):
        app_cfg = self.app_config_data
        # The application can have a "config_parameters" and a "target_config_overrides" section just like a library
        self._process_config_and_overrides(app_cfg, params, "app", "application")
        # The application can also defined macros
        self._process_macros(app_cfg.get("macros", []), macros, "app", "application")

    # Return the configuration data in two parts:
    #   - params: a dictionary with (name, ConfigParam) entries
    #   - macros: the list of macros defined with "macros" in libraries and in the application
    def get_config_data(self):
        all_params = self.get_target_config_data()
        lib_params, macros = self.get_lib_config_data()
        all_params.update(lib_params)
        self.get_app_config_data(all_params, macros)
        return all_params, [m.name for m in macros.values()]

    # Helper: verify if there are any required parameters without a value in 'params'
    def _check_required_parameters(self, params):
        for p in params.values():
            if p.required and (p.value is None):
                raise ConfigException("Required parameter '%s' defined by '%s' doesn't have a value" % (p.name, p.defined_by))

    # Return the macro definitions generated for a dictionary of configuration parameters
    # params: a dictionary of (name, ConfigParameters instance) mappings
    @staticmethod
    def parameters_to_macros(params):
        return ['%s=%s' % (m.macro_name, m.value) for m in params.values() if m.value is not None]

    # Return the configuration data converted to a list of C macros
    def get_config_data_macros(self):
        params, macros = self.get_config_data()
        self._check_required_parameters(params)
        return macros + self.parameters_to_macros(params)
