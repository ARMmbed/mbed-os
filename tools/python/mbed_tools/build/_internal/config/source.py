#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Configuration source parser."""
import logging
import pathlib

from dataclasses import dataclass
from typing import Iterable, Any, Optional, List

from mbed_tools.lib.json_helpers import decode_json_file
from mbed_tools.build.exceptions import InvalidConfigOverride
from mbed_tools.lib.python_helpers import flatten_nested

logger = logging.getLogger(__name__)


def from_file(
    config_source_file_path: pathlib.Path, target_filters: Iterable[str], default_name: Optional[str] = None
) -> dict:
    """Load a JSON config file and prepare the contents as a config source."""
    return prepare(decode_json_file(config_source_file_path), source_name=default_name, target_filters=target_filters)


def prepare(
    input_data: dict, source_name: Optional[str] = None, target_filters: Optional[Iterable[str]] = None
) -> dict:
    """Prepare a config source for entry into the Config object.

    Extracts config and override settings from the source. Flattens these nested dictionaries out into
    lists of objects which are namespaced in the way the Mbed config system expects.

    Args:
        input_data: The raw config JSON object parsed from the config file.
        source_name: Optional default name to use for namespacing config settings. If the input_data contains a 'name'
            field, that field is used as the namespace.
        target_filters: List of filter string used when extracting data from target_overrides section of the config
            data.

    Returns:
        Prepared config source.
    """
    data = input_data.copy()
    namespace = data.pop("name", source_name)
    for key in data:
        data[key] = _sanitise_value(data[key])

    if "config" in data:
        data["config"] = _extract_config_settings(namespace, data["config"])

    if "overrides" in data:
        data["overrides"] = _extract_overrides(namespace, data["overrides"])

    if "target_overrides" in data:
        data["overrides"] = _extract_target_overrides(
            namespace, data.pop("target_overrides"), target_filters if target_filters is not None else []
        )

    return data


@dataclass
class ConfigSetting:
    """Representation of a config setting.

    Auto converts any list values to sets for faster operations and de-duplication of values.
    """

    namespace: str
    name: str
    value: Any
    help_text: Optional[str] = None
    macro_name: Optional[str] = None

    def __post_init__(self) -> None:
        """Convert the value to a set if applicable."""
        self.value = _sanitise_value(self.value)


@dataclass
class Override:
    """Representation of a config override.

    Checks for _add or _remove modifiers and splits them from the name.
    """

    namespace: str
    name: str
    value: Any
    modifier: Optional[str] = None

    def __post_init__(self) -> None:
        """Parse modifiers and convert list values to sets."""
        if self.name.endswith("_add") or self.name.endswith("_remove"):
            self.name, self.modifier = self.name.rsplit("_", maxsplit=1)

        self.value = _sanitise_value(self.value)


def _extract_config_settings(namespace: str, config_data: dict) -> List[ConfigSetting]:
    settings = []
    for name, item in config_data.items():
        logger.debug("Extracting config setting from '%s': '%s'='%s'", namespace, name, item)
        if isinstance(item, dict):
            macro_name = item.get("macro_name")
            help_text = item.get("help")
            value = item.get("value")
        else:
            macro_name = None
            help_text = None
            value = item

        setting = ConfigSetting(
            namespace=namespace, name=name, macro_name=macro_name, help_text=help_text, value=value,
        )
        # If the config item is about a certain component or feature
        # being present, avoid adding it to the mbed_config.cmake
        # configuration file. Instead, applications should depend on
        # the feature or component with target_link_libraries() and the
        # component's CMake file (in the Mbed OS repo) will create
        # any necessary macros or definitions.
        if setting.name == "present":
            continue

        settings.append(setting)

    return settings


def _extract_target_overrides(
    namespace: str, override_data: dict, allowed_target_labels: Iterable[str]
) -> List[Override]:
    valid_target_data = dict()
    for target_type in override_data:
        if target_type == "*" or target_type in allowed_target_labels:
            valid_target_data.update(override_data[target_type])

    return _extract_overrides(namespace, valid_target_data)


def _extract_overrides(namespace: str, override_data: dict) -> List[Override]:
    overrides = []
    for name, value in override_data.items():
        try:
            override_namespace, override_name = name.split(".")
            if override_namespace and override_namespace not in [namespace, "target"] and namespace != "app":
                raise InvalidConfigOverride(
                    "It is only possible to override config settings defined in an mbed_lib.json from mbed_app.json. "
                    f"An override was defined by the lib `{namespace}` that attempts to override "
                    f"`{override_namespace}.{override_name}`."
                )
        except ValueError:
            override_namespace = namespace
            override_name = name

        overrides.append(Override(namespace=override_namespace, name=override_name, value=value))

    return overrides


def _sanitise_value(val: Any) -> Any:
    """Convert list values to sets and return scalar values and strings unchanged.

    For whatever reason, we allowed config settings to have values of any type available in the JSON spec.
    The value type can be a list, nested list, str, int, you name it.

    When we process the config, we want to use sets instead of lists, this is for two reasons:
    * To take advantage of set operations when we deal with "cumulative" settings.
    * To prevent any duplicate settings ending up in the final config.
    """
    if isinstance(val, list):
        return set(flatten_nested(val))

    return val
