#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from pathlib import Path
from unittest import mock

from mbed_tools.project._internal.render_templates import (
    render_cmakelists_template,
    render_main_cpp_template,
    render_gitignore_template,
)


@mock.patch("mbed_tools.project._internal.render_templates.datetime")
class TestRenderTemplates:
    def test_renders_cmakelists_template(self, mock_datetime, tmp_path):
        the_year = 3999
        mock_datetime.datetime.now.return_value.year = the_year
        program_name = "mytestprogram"
        file_path = Path(tmp_path, "mytestpath")

        render_cmakelists_template(file_path, program_name)
        output = file_path.read_text()

        assert str(the_year) in output
        assert program_name in output

    def test_renders_main_cpp_template(self, mock_datetime, tmp_path):
        the_year = 3999
        mock_datetime.datetime.now.return_value.year = the_year
        file_path = Path(tmp_path, "mytestpath")

        render_main_cpp_template(file_path)

        assert str(the_year) in file_path.read_text()

    def test_renders_gitignore_template(self, _, tmp_path):
        file_path = Path(tmp_path, "mytestpath")

        render_gitignore_template(file_path)

        assert "cmake_build" in file_path.read_text()
        assert ".mbedbuild" in file_path.read_text()
