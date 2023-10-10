#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import os
import pathlib
import pytest


from mbed_tools.project import MbedProgram
from mbed_tools.project.exceptions import ExistingProgram, ProgramNotFound, MbedOSNotFound
from mbed_tools.project.mbed_program import _find_program_root, parse_url
from mbed_tools.project._internal.project_data import MbedProgramFiles
from python_tests.mbed_tools.project.factories import make_mbed_program_files, make_mbed_os_files


DEFAULT_BUILD_SUBDIR = pathlib.Path("K64F", "develop", "GCC_ARM")


def from_new_set_target_toolchain(program_root):
    program = MbedProgram.from_new(program_root)
    parent_build_dir = program.files.cmake_build_dir
    program.files.cmake_build_dir = parent_build_dir / DEFAULT_BUILD_SUBDIR
    return program


class TestInitialiseProgram:
    def test_from_new_local_dir_raises_if_path_is_existing_program(self, tmp_path):
        program_root = pathlib.Path(tmp_path, "programfoo")
        program_root.mkdir()
        (program_root / "mbed-os.lib").touch()

        with pytest.raises(ExistingProgram):
            MbedProgram.from_new(program_root)

    def test_from_new_local_dir_generates_valid_program_creating_directory(self, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        fs_root.mkdir()
        program_root = fs_root / "programfoo"

        program = from_new_set_target_toolchain(program_root)

        assert program.files == MbedProgramFiles.from_existing(program_root, DEFAULT_BUILD_SUBDIR)

    def test_from_new_local_dir_generates_valid_program_creating_directory_in_cwd(self, tmp_path):
        old_cwd = os.getcwd()
        try:
            fs_root = pathlib.Path(tmp_path, "foo")
            fs_root.mkdir()
            os.chdir(fs_root)
            program_root = pathlib.Path("programfoo")

            program = from_new_set_target_toolchain(program_root)

            assert program.files == MbedProgramFiles.from_existing(program_root, DEFAULT_BUILD_SUBDIR)
        finally:
            os.chdir(old_cwd)

    def test_from_new_local_dir_generates_valid_program_existing_directory(self, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        fs_root.mkdir()
        program_root = fs_root / "programfoo"
        program_root.mkdir()

        program = from_new_set_target_toolchain(program_root)

        assert program.files == MbedProgramFiles.from_existing(program_root, DEFAULT_BUILD_SUBDIR)

    def test_from_existing_raises_if_path_is_not_a_program(self, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        fs_root.mkdir()
        program_root = fs_root / "programfoo"

        with pytest.raises(ProgramNotFound):
            MbedProgram.from_existing(program_root, DEFAULT_BUILD_SUBDIR)

    def test_from_existing_raises_if_no_mbed_os_dir_found_and_check_mbed_os_is_true(self, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(fs_root)

        with pytest.raises(MbedOSNotFound):
            MbedProgram.from_existing(fs_root, DEFAULT_BUILD_SUBDIR, check_mbed_os=True)

    def test_from_existing_returns_valid_program(self, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(fs_root)
        make_mbed_os_files(fs_root / "mbed-os")

        program = MbedProgram.from_existing(fs_root, DEFAULT_BUILD_SUBDIR)

        assert program.files.app_config_file.exists()
        assert program.mbed_os.root.exists()

    def test_from_existing_with_mbed_os_path_returns_valid_program(self, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        mbed_os_path = fs_root / "extern/mbed-os"
        mbed_os_path.mkdir(parents=True)
        make_mbed_program_files(fs_root)
        make_mbed_os_files(mbed_os_path)

        program = MbedProgram.from_existing(fs_root, DEFAULT_BUILD_SUBDIR, mbed_os_path)

        assert program.files.app_config_file.exists()
        assert program.mbed_os.root.exists()


class TestParseURL:
    def test_creates_url_and_dst_dir_from_name(self):
        name = "mbed-os-example-blows-up-board"
        data = parse_url(name)

        assert data["url"] == f"https://github.com/armmbed/{name}"
        assert data["dst_path"] == name

    def test_creates_valid_dst_dir_from_url(self):
        url = "https://superversioncontrol/superorg/mbed-os-example-numskull"
        data = parse_url(url)

        assert data["url"] == url
        assert data["dst_path"] == "mbed-os-example-numskull"

    def test_creates_valid_dst_dir_from_ssh_url(self):
        url = "git@superversioncontrol:superorg/mbed-os-example-numskull"
        data = parse_url(url)
        assert data["url"] == url
        assert data["dst_path"] == "mbed-os-example-numskull"


class TestFindProgramRoot:
    def test_finds_program_higher_in_dir_tree(self, tmp_path):
        program_root = pathlib.Path(tmp_path, "foo")
        pwd = program_root / "subprojfoo" / "libbar"
        make_mbed_program_files(program_root)
        pwd.mkdir(parents=True)

        assert _find_program_root(pwd) == program_root.resolve()

    def test_finds_program_at_current_path(self, tmp_path):
        program_root = pathlib.Path(tmp_path, "foo")
        make_mbed_program_files(program_root)

        assert _find_program_root(program_root) == program_root.resolve()

    def test_raises_if_no_program_found(self, tmp_path):
        program_root = pathlib.Path(tmp_path, "foo")
        program_root.mkdir()

        with pytest.raises(ProgramNotFound):
            _find_program_root(program_root)
