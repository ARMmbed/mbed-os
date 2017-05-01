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
from os.path import splitext, relpath, dirname, abspath, exists, join
import sys
from collections import namedtuple, OrderedDict
from intelhex import IntelHex
from jinja2 import FileSystemLoader, StrictUndefined
from jinja2.environment import Environment
from json import load
from jsonschema import Draft4Validator

from tools.utils import json_file_to_dict, intelhex_offset
from tools.arm_pack_manager import Cache
from tools.targets import CUMULATIVE_ATTRIBUTES, TARGET_MAP, \
    generate_py_target, get_resolution_order

class ConfigException(Exception):
    """Config system only exception. Makes it easier to distinguish config
    errors"""
    pass


def unit_display_name(unit_name, unit_kind, label=None):
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


def parameter_full_name(name, unit_name, unit_kind, label=None,
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
    if "." not in name:
        if unit_kind == "target":
            return "target." + name
        elif unit_kind == "application":
            return "app." + name
        else:
            return unit_name + '.' + name

    if not allow_prefix:
        raise ConfigException("Invalid parameter name '%s' in '%s'" %
                              (name, unit_display_name(
                                  unit_name, unit_kind, label)))
    try:
        prefix, _ = name.split(".")
    except ValueError:
        raise ConfigException("Invalid parameter name '%s' in '%s'" %
                              (name, unit_display_name(
                                  unit_name, unit_kind, label)))

    if  ((unit_kind == "library" and prefix != unit_name
          and prefix != "target") or
         (unit_kind == "target" and prefix != "target")):
        raise ConfigException(
            "Invalid prefix '%s' for parameter name '%s' in '%s'" %
            (prefix, name, unit_display_name(
                unit_name, unit_kind, label)))
    return name


def sanitize(name):
    """ "Sanitize" a name so that it is a valid C macro name. Currently it
    simply replaces '.' and '-' with '_'.

    Positional arguments:
    name - the name to make into a valid C macro
    """
    return name.replace('.', '_').replace('-', '_')


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
        if not isinstance(data, OrderedDict):
            data = {"value": data}
        self.name = parameter_full_name(name, unit_name, unit_kind,
                                        allow_prefix=False)
        self.defined_by = unit_display_name(unit_name, unit_kind)
        self.help_text = data.get("help", None)
        self.required = data.get("required", False)
        self.macro_name = data.get("macro_name", "MBED_CONF_%s" %
                                   sanitize(self.name.upper()))
        self.set_by = None
        self.set_value(data.get("value", None), unit_name, unit_kind)

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
        self.set_by = unit_display_name(unit_name, unit_kind, label)

    def __str__(self):
        """Return the string representation of this configuration parameter

        Arguments: None
        """
        if self.value is not None:
            return '%s = %s (macro name: "%s")' % \
                (self.name, self.value, self.macro_name)
        else:
            return '%s has no value' % self.name


class CumulativeOverride(object):
    """Representation of overrides for cumulative attributes"""
    def __init__(self, name):
        """Construct a CumulativeOverride object

        Positional arguments:
        name - the name of the config file this came from ?

        Keyword arguments:
        additions - macros to add to the overrides
        removals - macros to remove from the overrides
        strict - Boolean indicating that attempting to remove from an override
                 that does not exist should error
        """
        self.name = name
        self.additions = set()
        self.removals = set()
        self.is_strict = False

    def remove(self, overrides):
        """Extend the list of override removals.

        Positional arguments:
        overrides - a list of names that, when the override is evaluated, will
                    be removed
        """
        overrides = set(overrides)
        added_and_removed = overrides.intersection(self.additions)
        if len(added_and_removed) > 0:
            raise ConfigException(
                "Configuration conflict. The values %s are both added and removed from %s."
                % (", ".join(added_and_removed), self.name))

        self.removals |= set(overrides)

    def add(self, overrides):
        """Extend the list of override additions.

        Positional arguments:
        overrides - a list of a names that, when the override is evaluated, will
                    be added to the list
        """
        overrides = set(overrides)
        added_and_removed = overrides.intersection(self.removals)
        if len(added_and_removed) > 0:
            raise ConfigException(
                "Configuration conflict. The values %s are both added and removed from %s."
                % (", ".join(added_and_removed), self.name))

        self.additions |= set(overrides)

    def strict(self, overrides):
        """Remove all overrides that are not the specified ones

        Positional arguments:
        overrides - a list of names that will replace the entire attribute when
                    this override is evaluated.
        """
        self.remove(self.additions - set(overrides))
        self.add(overrides)
        self.is_strict = True

    def get_value(self, target):
        """Get the overridden version of this property"""
        return list((set(getattr(target, self.name, []))
                     | self.additions) - self.removals)


ConfigMacro = namedtuple("ConfigMacro",
                         "name defined_by macro_name macro_value")

Region = namedtuple("Region", "name start size active filename")

class Config(object):
    """'Config' implements the mbed configuration mechanism"""

    # Libraries and applications have different names for their configuration
    # files
    app_config_name = "mbed_app.json"
    lib_config_name = "mbed_lib.json"

    APP_SCHEMA = Draft4Validator(
        load(open(join(dirname(__file__), "mbed_app.schema"))),
        types={"object": (OrderedDict, dict)})
    LIB_SCHEMA = Draft4Validator(
        load(open(join(dirname(__file__), "mbed_lib.schema"))),
        types={"object": (OrderedDict, dict)})

    __not_config = set(
        ["target.bootloader_img", "target.restrict_size",
         "taregt.mbed_app_start", "target.mbed_app_size"] +
        ["target.%s" % key for key in CUMULATIVE_ATTRIBUTES] +
        ["target.%s_add" % key for key in CUMULATIVE_ATTRIBUTES] +
        ["target.%s_remove" % key for key in CUMULATIVE_ATTRIBUTES])

    # Allowed features in configurations
    __allowed_features = [
        "UVISOR", "BLE", "CLIENT", "IPV4", "LWIP", "COMMON_PAL", "STORAGE",
        "NANOSTACK",
        # Nanostack configurations
        "LOWPAN_BORDER_ROUTER", "LOWPAN_HOST", "LOWPAN_ROUTER",
        "NANOSTACK_FULL", "THREAD_BORDER_ROUTER", "THREAD_END_DEVICE",
        "THREAD_ROUTER", "ETHERNET_HOST"
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
        self.cumulative_overrides = {key: CumulativeOverride(key)
                                     for key in CUMULATIVE_ATTRIBUTES}
        self.lib_config_data = {}
        self.processed_configs = {}
        self.params = {}
        self._macros = {}
        config_errors = []
        self.app_config_location = app_config
        if self.app_config_location is None:
            for directory in top_level_dirs or []:
                full_path = os.path.join(directory, self.app_config_name)
                if os.path.isfile(full_path):
                    if self.app_config_location is not None:
                        raise ConfigException(
                            "Duplicate '%s' file in '%s' and '%s'"
                            % (self.app_config_name,
                               self.app_config_location, full_path))
                    else:
                        self.app_config_location = full_path
        try:
            self.app_config_data = json_file_to_dict(self.app_config_location) \
                                   if self.app_config_location else OrderedDict()
        except ValueError:
            self.app_config_data = OrderedDict()
            config_errors.append(
                ConfigException("Could not parse mbed app configuration from %s"
                                % self.app_config_location))

        if not self.APP_SCHEMA.is_valid(self.app_config_data):
            raise ConfigException(
                "\n".join(str(err) for err in
                          self.APP_SCHEMA.iter_errors(self.app_config_data)))

        # Update the list of targets with the ones defined in the application
        # config, if applicable
        # Make sure that each config is processed only once
        if isinstance(tgt, basestring):
            if tgt in TARGET_MAP:
                self.target = TARGET_MAP[tgt]
            else:
                self.target = generate_py_target(
                    self.app_config_data.get("custom_targets", {}), tgt)
        else:
            self.target = tgt
        self.target = deepcopy(self.target)


        self._override_config(self.app_config_data, "app", "application")
        self.config_errors = config_errors

    def add_config_files(self, flist):
        """Add configuration files

        Positional arguments:
        flist - a list of files to add to this configuration
        """
        wipe_cache = False
        for config_file in flist:
            full_path = os.path.normpath(os.path.abspath(config_file))
            if  (not config_file.endswith(self.lib_config_name) or
                 full_path in self.processed_configs):
                continue
            self.processed_configs[full_path] = True
            try:
                cfg = json_file_to_dict(config_file)
            except ValueError as exc:
                sys.stderr.write(str(exc) + "\n")
                continue
            if not self.LIB_SCHEMA.is_valid(cfg):
                raise ConfigException(
                    "\n".join(str(err) for err in
                              self.LIB_SCHEMA.iter_errors(cfg)))
            cfg["__config_path"] = full_path
            if cfg["name"] in self.lib_config_data:
                raise ConfigException(
                    "Library name '%s' is not unique." % cfg["name"])
            self.lib_config_data[cfg["name"]] = cfg
            wipe_cache = True
        if wipe_cache:
            self._macros = {}
            self.params = {}
            self.cumulative_overrides = {key: CumulativeOverride(key)
                                         for key in CUMULATIVE_ATTRIBUTES}
    @property
    def macros(self):
        self.config_data
        return self.cumulative_overrides["macros"].get_value(self.target)

    @property
    def device_has(self):
        self.config_data
        return self.cumulative_overrides["device_has"].get_value(self.target)

    @property
    def extra_labels(self):
        self.config_data
        extra_labels = self.cumulative_overrides["extra_labels"].get_value(self.target)
        if "UVISOR_SUPPORTED" not in extra_labels + self.target.labels:
            extra_labels.append("UVISOR_UNSUPPORTED")
        return extra_labels

    @property
    def features(self):
        return self.get_features()

    @property
    def name(self):
        if "artifact_name" in self.app_config_data:
            return self.app_config_data["artifact_name"]
        else:
            return None

    @property
    def parameters_as_macros(self):
        """ Encode the configuration parameters as C macro definitions.

        Positional arguments:
        params - a dictionary mapping a name to a ConfigParameter

        Return: a list of strings that encode the configuration parameters as
        C pre-processor macros
        """
        return ['%s=%s' % (m.macro_name, m.value) for m
                in self.params.values() if m.value is not None]

    @property
    def config_macros_as_macros(self):
        """ Return the macro definitions generated for a dictionary of
        ConfigMacros (as returned by get_config_data).

        Positional arguments:
        params - a dictionary mapping a name to a ConfigMacro instance

        Return: a list of strings that are the C pre-processor macros
        """
        return [m.name for m in self._macros.values()]

    @property
    def config_as_macros(self):
        """Convert the configuration data to a list of C macros

        Positional arguments:
        config - configuration data as (ConfigParam instances, ConfigMacro
                 instances) tuple (as returned by get_config_data())
        """
        self._check_required_parameters()
        return self.config_macros_as_macros + self.parameters_as_macros

    @property
    def valid(self):
        """ Validate configuration settings. This either returns True or
        raises an exception
        """
        self.config_data
        if self.config_errors:
            raise self.config_errors[0]
        return True

    @property
    def has_regions(self):
        """Does this config have regions defined?"""
        if 'target_overrides' in self.app_config_data:
            target_overrides = self.app_config_data['target_overrides'].get(
                self.target.name, {})
            return ('target.bootloader_img' in target_overrides or
                    'target.restrict_size' in target_overrides or
                    'target.mbed_app_start' in target_overrides or
                    'target.mbed_app_size' in target_overrides)
        else:
            return False

    @property
    def regions(self):
        """Generate a list of regions from the config"""
        if not self.target.bootloader_supported:
            raise ConfigException("Bootloader not supported on this target.")
        cmsis_part = Cache(False, False).index[self.target.device_name]
        target_overrides = self.app_config_data['target_overrides'].get(
            self.target.name, {})
        if  (('target.bootloader_img' in target_overrides or
              'target.restrict_size' in target_overrides) and
             ('target.mbed_app_start' in target_overrides or
              'target.mbed_app_size' in target_overrides)):
            raise ConfigException(
                "target.bootloader_img and target.restirct_size are "
                "incompatible with target.mbed_app_start and "
                "target.mbed_app_size")
        try:
            rom_size = int(cmsis_part['memory']['IROM1']['size'], 0)
            rom_start = int(cmsis_part['memory']['IROM1']['start'], 0)
        except KeyError:
            raise ConfigException("Not enough information in CMSIS packs to "
                                  "build a bootloader project")
        if  ('target.bootloader_img' in target_overrides or
             'target.restrict_size' in target_overrides):
            return self._generate_booloader_build(target_overrides,
                                                  rom_size, rom_size)
        elif ('target.mbed_app_start' in target_overrides or
              'target.mbed_app_size' in target_overrides):
            return self._generate_linker_overrides(target_overrides,
                                                   rom_start, rom_size)
        else:
            raise ConfigException(
                "Bootloader build requested but no bootlader configuration")

    @staticmethod
    def _generate_booloader_build(target_overrides, rom_start, rom_size):
        start = 0
        if 'target.bootloader_img' in target_overrides:
            filename = target_overrides['target.bootloader_img']
            if not exists(filename):
                raise ConfigException("Bootloader %s not found" % filename)
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

    @property
    def report(self):
        return {'app_config': self.app_config_location,
                'library_configs': map(relpath, self.processed_configs.keys())}

    @staticmethod
    def _generate_linker_overrides(target_overrides, rom_start, rom_size):
        if 'target.mbed_app_start' in target_overrides:
            start = int(target_overrides['target.mbed_app_start'], 0)
        else:
            start = rom_start
        if 'target.mbed_app_size' in target_overrides:
            size = int(target_overrides['target.mbed_app_size'], 0)
        else:
            size = (rom_size + rom_start) - start
        if start < rom_start:
            raise ConfigException("Application starts before ROM")
        if size + start > rom_size + rom_start:
            raise ConfigException("Application ends after ROM")
        yield Region("application", start, size, True, None)

    def _add_unit_config(self, data, unit_name, unit_kind):
        """Process a "config" section in a config file

        Positional arguments:
        data - a dictionary with the configuration parameters
        params - storage for the discovered configuration parameters
        unit_name - the unit (target/library/application) that defines this
                    parameter
        unit_kind - the kind of the unit ("target", "library" or "application")
        """
        new_params = {parameter_full_name(name, unit_name, unit_kind):
                      ConfigParameter(name, val, unit_name, unit_kind) for
                      name, val in data.items()}
        overlap = set(new_params).intersection(set(self.params))
        if overlap:
            raise ConfigException(
                "Parameters named '%s' multiply defined" % "', '".join(overlap))
        self.params.update(new_params)


    def _add_unit_macros(self, mlist, unit_name, unit_kind):
        """Process a macro definition and check for incompatible duplicate
        definitions.

        Positional arguments:
        mlist - list of macro names to process
        unit_name - the unit (library/application) that defines this macro
        unit_kind - the kind of the unit ("library" or "application")
        """
        for mname in mlist:
            defined_by = unit_display_name(unit_name, unit_kind)
            if "=" in mname:
                try:
                    macro_name, macro_value = mname.split("=")
                    macro = ConfigMacro(mname, defined_by, macro_name,
                                        macro_value)
                except ValueError:
                    raise ValueError("Invalid macro definition '%s' in '%s'" %
                                     (mname, defined_by))
            else:
                macro = ConfigMacro(mname, defined_by, mname, None)
            if  ((macro.macro_name in self._macros) and
                 (self._macros[macro.macro_name].name != mname)):
                full_unit_name = unit_display_name(unit_name, unit_kind)
                raise ConfigException(
                    "Macro '%s' defined in both '%s' and '%s' with incompatible values"
                    % (macro.macro_name,
                       self._macros[macro.macro_name].defined_by,
                       full_unit_name))
            self._macros[macro.macro_name] = macro


    def _override_config(self, data, unit_name, unit_kind):
        """Process "target_overrides" section of a config file

        Positional arguments:
        data - the configuration data of the library/appliation
        unit_name - the unit (library/application) that defines this parameter
        unit_kind - the kind of the unit ("library" or "application")
        """
        for label, overrides in data.items():
            if label not in self.target.labels + ['*']:
                continue
            if  (unit_kind == 'library' and
                 any(attr.startswith('target.extra_labels') for attr
                     in overrides.iterkeys())):
                raise ConfigException(
                    "Target override 'target.extra_labels' in " +
                    unit_display_name(unit_name, unit_kind, label) +
                    " is only allowed at the application level")

            for attr, cumulatives in self.cumulative_overrides.iteritems():
                strict = 'target.%s' % attr
                add = 'target.%s_add' % attr
                remove = 'target.%s_remove' % attr
                if strict in overrides:
                    cumulatives.strict(overrides[strict])
                if add in overrides:
                    cumulatives.add(overrides[add])
                if remove in overrides:
                    cumulatives.remove(overrides[remove])

            for name, val in overrides.iteritems():
                if name in self.__not_config:
                    continue
                full_name = parameter_full_name(
                    name, unit_name, unit_kind, label)
                try:
                    self.params[full_name].set_value(
                        val, unit_name, unit_kind, label)
                except KeyError:
                    self.config_errors.append(
                        ConfigException(
                            "Attempt to override undefined parameter"
                            " '%s' in '%s'"
                            % (full_name,
                               unit_display_name(unit_name, unit_kind, label))))

    def unit_config_then_overrides(self, data, name, kind):
        """Process all of the data a unit could have: adding config parameters,
        overriding config parameters and adding macros.
        """
        if data.get("config", {}):
            self._add_unit_config(data["config"], name, kind)
        if data.get("target_overrides", {}):
            self._override_config(data["target_overrides"], name, kind)
        if data.get("macros", {}):
            self._add_unit_macros(data["macros"], name, kind)

    @property
    def target_config_data(self):
        """Read and interpret configuration data defined by targets.

        We consider the resolution order for our target and sort it by level
        reversed, so that we first look at the top level target (the parent),
        then its direct children, then the children of those children and so on,
        until we reach self.target

        Arguments: None
        Yield: arguments to self.unit_config_then_overrides
        """
        resolution_order = [e[0] for e
                            in sorted(
                                self.target.resolution_order,
                                key=lambda e: e[1], reverse=True)]
        for tname in resolution_order:
            target_data = self.target.json_data[tname]
            to_yield = {}
            if target_data.get("config", {}):
                to_yield["config"] = target_data["config"]
            if target_data.get("overrides"):
                to_yield["target_overrides"] = {tname: target_data["overrides"]}
            yield (to_yield, tname, "target")

    @property
    def config_data(self):
        """Finalize and return the configuration data in two parts:
        (params, macros)
        params - a dictionary with mapping a name to a ConfigParam
        macros - the list of macros defined with "macros" in libraries and in
                 the application (as ConfigMacro instances)

        Arguments: None

        Just calls unit_config_then_overrides on the correct units in the right
        order
        """
        if not self.params:
            self.config_errors = []
            for data, name, kind in (list(self.target_config_data) +
                                     [(data, name, "library") for name, data
                                      in self.lib_config_data.items()] +
                                     [(self.app_config_data, "app",
                                       "application")]):
                self.unit_config_then_overrides(data, name, kind)
        return self.params, self._macros

    def _check_required_parameters(self):
        """Check that there are no required parameters without a value

        Positional arguments:
        params - the list of parameters to check

        NOTE: This function does not return. Instead, it throws a
        ConfigException when any of the required parameters are missing values
        """
        self.config_data
        for param in self.params.values():
            if param.required and (param.value is None):
                raise ConfigException(
                    "Required parameter '%s' defined by '%s' has no value"
                    % (param.name, param.defined_by))

    def get_features(self):
        """ Extract any features from the configuration data

        Arguments: None
        """
        self._check_required_parameters()
        features = self.cumulative_overrides['features'].get_value(self.target)

        for feature in features:
            if feature not in self.__allowed_features:
                raise ConfigException(
                    "Feature '%s' is not a supported features" % feature)

        return features

    def load_resources(self, resources):
        """ Load configuration data from a Resources instance and expand it
        based on defined features.

        Positional arguments:
        resources - the resources object to load from and expand

        Side effects:
        updates the resources object to include all of the files in the features
        """
        prev_features = set()
        while True:
            self.add_config_files(resources.json_files)

            features = set(self.get_features())
            if features == prev_features:
                break

            for feature in features:
                if feature in resources.features:
                    resources.add(resources.features[feature])

            prev_features = features
        assert self.valid

    def get_config_data_header(self, fname=None):
        """ Convert the configuration data to the content of a C header file,
        meant to be included to a C/C++ file. The content is returned as a
        string.

        Keyword arguments:
        fname -  also write the content is to the file called "fname".
                 WARNING: if 'fname' names an existing file, it will be
                 overwritten!
        """
        self._check_required_parameters()
        params_with_values = [p for p in self.params.values()
                              if p.value is not None]
        ctx = {
            "cfg_params" : [(p.macro_name, str(p.value), p.set_by)
                            for p in params_with_values],
            "macros": [(m.macro_name, str(m.macro_value or ""), m.defined_by)
                       for m in self._macros.values()],
            "name_len":  max([len(m.macro_name) for m in self._macros.values()]
                             + [len(m.macro_name) for m in params_with_values]
                             + [0]),
            "val_len" : max([len(str(m.value)) for m in params_with_values] +
                            [len(m.macro_value or "") for m in
                             self._macros.values()]
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
