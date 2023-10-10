#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Render jinja templates required by the project package."""
import datetime

from pathlib import Path

import jinja2

TEMPLATES_DIRECTORY = Path("_internal", "templates")


def render_cmakelists_template(cmakelists_file: Path, program_name: str) -> None:
    """Render CMakeLists.tmpl with the copyright year and program name as the app target name.

    Args:
        cmakelists_file: The path where CMakeLists.txt will be written.
        program_name: The name of the program, will be used as the app target name.
    """
    cmakelists_file.write_text(
        render_jinja_template(
            "CMakeLists.tmpl", {"program_name": program_name, "year": str(datetime.datetime.now().year)}
        )
    )


def render_main_cpp_template(main_cpp: Path) -> None:
    """Render a basic main.cpp which prints a hello message and returns.

    Args:
        main_cpp: Path where the main.cpp file will be written.
    """
    main_cpp.write_text(render_jinja_template("main.tmpl", {"year": str(datetime.datetime.now().year)}))


def render_gitignore_template(gitignore: Path) -> None:
    """Write out a basic gitignore file ignoring the build and config directory.

    Args:
        gitignore: The path where the gitignore file will be written.
    """
    gitignore.write_text(render_jinja_template("gitignore.tmpl", {}))


def render_jinja_template(template_name: str, context: dict) -> str:
    """Render a jinja template.

    Args:
        template_name: The name of the template being rendered.
        context: Data to render into the jinja template.
    """
    env = jinja2.Environment(loader=jinja2.PackageLoader("mbed_tools.project", str(TEMPLATES_DIRECTORY)))
    template = env.get_template(template_name)
    return template.render(context)
