#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Build configuration representation."""
import logging

from collections import UserDict
from typing import Any, Iterable, Hashable, List

from mbed_tools.build._internal.config.source import Override, ConfigSetting

logger = logging.getLogger(__name__)


class Config(UserDict):
    """Mapping of config settings.

    This object understands how to populate the different 'config sections' which all have different rules for how the
    settings are collected.
    Applies overrides, appends macros, and updates config settings.
    """

    def __setitem__(self, key: Hashable, item: Any) -> None:
        """Set an item based on its key."""
        if key == CONFIG_SECTION:
            self._update_config_section(item)
        elif key == OVERRIDES_SECTION:
            self._handle_overrides(item)
        elif key == MACROS_SECTION:
            self.data[MACROS_SECTION] = self.data.get(MACROS_SECTION, set()) | item
        elif key == REQUIRES_SECTION:
            self.data[REQUIRES_SECTION] = self.data.get(REQUIRES_SECTION, set()) | item
        else:
            super().__setitem__(key, item)

    def _handle_overrides(self, overrides: Iterable[Override]) -> None:
        for override in overrides:
            logger.debug("Applying override '%s.%s'='%s'", override.namespace, override.name, repr(override.value))
            if override.name in self.data:
                _apply_override(self.data, override)
                continue

            setting = next(
                filter(
                    lambda x: x.name == override.name and x.namespace == override.namespace,
                    self.data.get(CONFIG_SECTION, []),
                ),
                None,
            )
            if setting is None:
                logger.warning(
                    f"You are attempting to override an undefined config parameter "
                    f"`{override.namespace}.{override.name}`.\n"
                    "It is an error to override an undefined configuration parameter. "
                    "Please check your target_overrides are correct.\n"
                    f"The parameter `{override.namespace}.{override.name}` will not be added to the configuration."
                )
            else:
                setting.value = override.value

    def _update_config_section(self, config_settings: List[ConfigSetting]) -> None:
        for setting in config_settings:
            logger.debug("Adding config setting: '%s.%s'", setting.namespace, setting.name)
            if setting in self.data.get(CONFIG_SECTION, []):
                raise ValueError(
                    f"Setting {setting.namespace}.{setting.name} already defined. You cannot duplicate config settings!"
                )

        self.data[CONFIG_SECTION] = self.data.get(CONFIG_SECTION, []) + config_settings


CONFIG_SECTION = "config"
MACROS_SECTION = "macros"
OVERRIDES_SECTION = "overrides"
REQUIRES_SECTION = "requires"


def _apply_override(data: dict, override: Override) -> None:
    if override.modifier == "add":
        data[override.name] |= override.value
    elif override.modifier == "remove":
        data[override.name] -= override.value
    else:
        data[override.name] = override.value
