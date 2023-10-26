#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Tests for project_data.py."""
import pathlib
import pytest

from unittest import mock

from mbed_tools.project._internal.project_data import (
    MbedProgramFiles,
    MbedOS,
    MAIN_CPP_FILE_NAME,
    APP_CONFIG_FILE_NAME_JSON
)
from python_tests.mbed_tools.project.factories import make_mbed_lib_reference, make_mbed_program_files, make_mbed_os_files


class TestMbedProgramFiles:
    def test_from_new_raises_if_program_files_already_exist(self, tmp_path):
        root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(root)

        with pytest.raises(ValueError):
            MbedProgramFiles.from_new(root)

    def test_from_new_returns_valid_program_file_set(self, tmp_path):
        root = pathlib.Path(tmp_path, "foo")
        root.mkdir()

        program = MbedProgramFiles.from_new(root)

        assert program.app_config_file.exists()
        assert program.mbed_os_ref.exists()
        assert program.cmakelists_file.exists()

    def test_from_new_calls_render_template_for_gitignore_and_main(self, tmp_path):
        with mock.patch(
            "mbed_tools.project._internal.project_data.render_cmakelists_template"
        ) as render_cmakelists_template, mock.patch(
            "mbed_tools.project._internal.project_data.render_main_cpp_template"
        ) as render_main_cpp_template, mock.patch(
            "mbed_tools.project._internal.project_data.render_gitignore_template"
        ) as render_gitignore_template:
            root = pathlib.Path(tmp_path, "foo")
            root.mkdir()
            program_files = MbedProgramFiles.from_new(root)
            render_cmakelists_template.assert_called_once_with(program_files.cmakelists_file, "foo")
            render_main_cpp_template.assert_called_once_with(root / MAIN_CPP_FILE_NAME)
            render_gitignore_template.assert_called_once_with(root / ".gitignore")

    def test_from_existing_finds_existing_program_data(self, tmp_path):
        root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(root)

        program = MbedProgramFiles.from_existing(root, pathlib.Path("K64F", "develop", "GCC_ARM"))

        assert program.app_config_file.exists()
        assert program.mbed_os_ref.exists()
        assert program.cmakelists_file.exists()

    def test_from_existing_finds_existing_program_data_app_json(self, tmp_path):
        """
        Same as test_from_existing_finds_existing_program_data() except the app config
        is json instead of json5
        """

        root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(root, APP_CONFIG_FILE_NAME_JSON)

        program = MbedProgramFiles.from_existing(root, pathlib.Path("K64F", "develop", "GCC_ARM"))

        assert program.app_config_file.exists()
        assert program.mbed_os_ref.exists()
        assert program.cmakelists_file.exists()



class TestMbedLibReference:
    def test_is_resolved_returns_true_if_source_code_dir_exists(self, tmp_path):
        root = pathlib.Path(tmp_path, "foo")
        lib = make_mbed_lib_reference(root, resolved=True)

        assert lib.is_resolved()

    def test_is_resolved_returns_false_if_source_code_dir_doesnt_exist(self, tmp_path):
        root = pathlib.Path(tmp_path, "foo")
        lib = make_mbed_lib_reference(root)

        assert not lib.is_resolved()

    def test_get_git_reference_returns_lib_file_contents(self, tmp_path):
        root = pathlib.Path(tmp_path, "foo")
        url = "https://github.com/mylibrepo"
        ref = "latest"
        references = [f"{url}#{ref}", f"{url}/#{ref}"]

        for full_ref in references:
            lib = make_mbed_lib_reference(root, ref_url=full_ref)

            reference = lib.get_git_reference()

            assert reference.repo_url == url
            assert reference.ref == ref


class TestMbedOS:
    def test_from_existing_finds_existing_mbed_os_data(self, tmp_path):
        root_path = pathlib.Path(tmp_path, "my-version-of-mbed-os")
        make_mbed_os_files(root_path)

        mbed_os = MbedOS.from_existing(root_path)

        assert mbed_os.targets_json_file == root_path / "targets" / "targets.json5"

    def test_raises_if_files_missing(self, tmp_path):
        root_path = pathlib.Path(tmp_path, "my-version-of-mbed-os")
        root_path.mkdir()

        with pytest.raises(ValueError):
            MbedOS.from_existing(root_path)
