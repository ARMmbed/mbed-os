#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Module in charge of CMake file generation."""
import pathlib

from typing import Any

import jinja2

from mbed_tools.build._internal.config.config import Config

TEMPLATES_DIRECTORY = pathlib.Path("_internal", "templates")
TEMPLATE_NAME = "mbed_config.tmpl"


def render_mbed_config_cmake_template(config: Config, toolchain_name: str, target_name: str) -> str:
    """Renders the mbed_config jinja template with the target and project config settings.

    Args:
        config: Config object holding information parsed from the mbed config system.
        toolchain_name: Name of the toolchain being used.
        target_name: Name of the target.

    Returns:
        The rendered mbed_config template.
    """
    env = jinja2.Environment(loader=jinja2.PackageLoader("mbed_tools.build", str(TEMPLATES_DIRECTORY)),)
    env.filters["to_hex"] = to_hex
    template = env.get_template(TEMPLATE_NAME)
    config["supported_c_libs"] = [x for x in config["supported_c_libs"][toolchain_name.lower()]]
    context = {"target_name": target_name, "toolchain_name": toolchain_name, **config}
    return template.render(context)


def to_hex(s: Any) -> str:
    """Filter to convert integers to hex."""
    return hex(int(s, 0))
