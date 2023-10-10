#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib

import pytest

from unittest import mock

from mbed_tools.project import initialise_project, import_project, deploy_project, get_known_libs


@pytest.fixture
def mock_libs():
    with mock.patch("mbed_tools.project.project.LibraryReferences") as libs:
        yield libs


@pytest.fixture
def mock_program():
    with mock.patch("mbed_tools.project.project.MbedProgram") as prog:
        yield prog


@pytest.fixture
def mock_git():
    with mock.patch("mbed_tools.project.project.git_utils") as gutils:
        yield gutils


class TestInitialiseProject:
    def test_fetches_mbed_os_when_create_only_is_false(self, mock_libs, mock_program):
        path = pathlib.Path()
        initialise_project(path, create_only=False)

        mock_program.from_new.assert_called_once_with(path)
        mock_libs().fetch.assert_called_once()

    def test_skips_mbed_os_when_create_only_is_true(self, mock_libs, mock_program):
        path = pathlib.Path()
        initialise_project(path, create_only=True)

        mock_program.from_new.assert_called_once_with(path)
        mock_libs().fetch.assert_not_called()


class TestImportProject:
    def test_clones_from_remote(self, mock_git):
        url = "https://git.com/gitorg/repo"
        import_project(url, recursive=False)

        mock_git.clone.assert_called_once_with(url, pathlib.Path(url.rsplit("/", maxsplit=1)[-1]))

    def test_resolves_libs_when_recursive_is_true(self, mock_git, mock_libs):
        url = "https://git.com/gitorg/repo"
        import_project(url, recursive=True)

        mock_git.clone.assert_called_once_with(url, pathlib.Path(url.rsplit("/", maxsplit=1)[-1]))
        mock_libs().fetch.assert_called_once()


class TestDeployProject:
    def test_checks_out_libraries(self, mock_libs):
        path = pathlib.Path("somewhere")
        deploy_project(path, force=False)

        mock_libs().checkout.assert_called_once_with(force=False)

    def test_resolves_libs_if_unresolved_detected(self, mock_libs):
        mock_libs().iter_unresolved.return_value = [1]
        path = pathlib.Path("somewhere")
        deploy_project(path)

        mock_libs().fetch.assert_called_once()


class TestPrintLibs:
    def test_list_libraries_gets_known_lib_list(self, mock_libs):
        path = pathlib.Path("somewhere")
        mock_libs().iter_resolved.return_value = ["", ""]

        libs = get_known_libs(path)

        assert libs == ["", ""]
