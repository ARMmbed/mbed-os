#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib

import pytest

from unittest import mock

from mbed_tools.project._internal.libraries import MbedLibReference, LibraryReferences
from mbed_tools.project.exceptions import VersionControlError
from python_tests.mbed_tools.project.factories import make_mbed_lib_reference


@pytest.fixture
def mock_clone():
    with mock.patch("mbed_tools.project._internal.git_utils.clone") as clone:
        yield clone


@pytest.fixture
def mock_checkout():
    with mock.patch("mbed_tools.project._internal.git_utils.checkout") as checkout:
        yield checkout


@pytest.fixture
def mock_fetch():
    with mock.patch("mbed_tools.project._internal.git_utils.fetch") as fetch:
        yield fetch


@pytest.fixture
def mock_get_repo():
    with mock.patch("mbed_tools.project._internal.git_utils.get_repo") as get_repo:
        yield get_repo


@pytest.fixture
def mock_get_default_branch():
    with mock.patch("mbed_tools.project._internal.git_utils.get_default_branch") as get_default_branch:
        yield get_default_branch


@pytest.fixture
def mock_repo():
    with mock.patch("mbed_tools.project._internal.git_utils.git.Repo") as repo:
        yield repo


class TestLibraryReferences:
    def test_hydrates_top_level_library_references(self, mock_clone, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        lib = make_mbed_lib_reference(fs_root, ref_url="https://git")
        mock_clone.side_effect = lambda url, dst_dir, *args: dst_dir.mkdir()

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.fetch()

        mock_clone.assert_called_once_with(lib.get_git_reference().repo_url, lib.source_code_path)
        assert lib.is_resolved()

    def test_hydrates_recursive_dependencies(self, mock_clone, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        lib = make_mbed_lib_reference(fs_root, ref_url="https://git")
        # Create a lib reference without touching the fs at this point, we want to mock the effects of a recursive
        # reference lookup and we need to assert the reference was resolved.
        lib2 = MbedLibReference(
            reference_file=(lib.source_code_path / "lib2.lib"), source_code_path=(lib.source_code_path / "lib2")
        )
        # Here we mock the effects of a recursive reference lookup. We create a new lib reference as a side effect of
        # the first call to the mock. Then we create the src dir, thus resolving the lib, on the second call.
        mock_clone.side_effect = lambda url, dst_dir, *args: (
            make_mbed_lib_reference(pathlib.Path(dst_dir), name=lib2.reference_file.name, ref_url="https://valid2"),
            lib2.source_code_path.mkdir(),
        )

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.fetch()

        assert lib.is_resolved()
        assert lib2.is_resolved()

    def test_does_perform_checkout_of_default_repo_branch_if_no_git_ref_exists(
        self, mock_get_repo, mock_checkout, mock_fetch, mock_get_default_branch, mock_clone, tmp_path
    ):
        fs_root = pathlib.Path(tmp_path, "foo")
        make_mbed_lib_reference(fs_root, ref_url="https://git", resolved=True)

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.checkout(force=False)

        mock_fetch.assert_called_once_with(mock_get_repo(), mock_get_default_branch())
        mock_checkout.assert_called_once_with(mock_get_repo(), "FETCH_HEAD", force=False)

    def test_performs_checkout_if_git_ref_exists(self, mock_get_repo, mock_checkout, mock_fetch, mock_clone, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        lib = make_mbed_lib_reference(fs_root, ref_url="https://git#lajdhalk234", resolved=True)

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.checkout(force=False)

        mock_fetch.assert_called_once_with(mock_get_repo(), lib.get_git_reference().ref)
        mock_checkout.assert_called_once_with(mock_get_repo.return_value, "FETCH_HEAD", force=False)

    def test_fetch_does_not_perform_checkout_if_no_git_ref_exists(
        self, mock_get_repo, mock_checkout, mock_fetch, mock_clone, tmp_path
    ):
        fs_root = pathlib.Path(tmp_path, "foo")
        make_mbed_lib_reference(fs_root, ref_url="https://git")
        mock_clone.side_effect = lambda url, dst_dir, *args: dst_dir.mkdir()

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.fetch()

        mock_fetch.assert_not_called()
        mock_checkout.assert_not_called()

    def test_fetch_performs_checkout_if_ref_is_hash(
        self, mock_get_repo, mock_clone, mock_fetch, mock_checkout, tmp_path
    ):
        num_times_called = 0

        def clone_side_effect(url, dst_dir, *args):
            nonlocal num_times_called
            if num_times_called == 0:
                num_times_called += 1
                raise VersionControlError("Failed to clone")
            elif num_times_called == 1:
                num_times_called += 1
                dst_dir.mkdir()
            else:
                assert False

        fs_root = pathlib.Path(tmp_path, "foo")
        lib = make_mbed_lib_reference(fs_root, ref_url="https://git#398bc1a63370")
        mock_clone.side_effect = clone_side_effect

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.fetch()

        mock_clone.assert_called_with(lib.get_git_reference().repo_url, lib.source_code_path)
        mock_fetch.assert_called_once_with(None, lib.get_git_reference().ref)
        mock_checkout.assert_called_once_with(None, "FETCH_HEAD")

    def test_raises_when_no_such_ref(self, mock_repo, mock_clone, mock_fetch, mock_checkout, tmp_path):
        num_times_called = 0

        def clone_side_effect(url, dst_dir, *args):
            nonlocal num_times_called
            if num_times_called == 0:
                num_times_called += 1
                raise VersionControlError("Failed to clone")
            elif num_times_called == 1:
                num_times_called += 1
                dst_dir.mkdir()
            else:
                assert False

        fs_root = pathlib.Path(tmp_path, "foo")
        make_mbed_lib_reference(fs_root, ref_url="https://git#lajdhalk234")

        mock_clone.side_effect = clone_side_effect
        mock_fetch.side_effect = None
        mock_checkout.side_effect = VersionControlError("Failed to checkout")

        with pytest.raises(VersionControlError, match="Failed to checkout"):
            lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
            lib_refs.fetch()

    def test_doesnt_fetch_for_branch_or_tag(self, mock_clone, mock_fetch, mock_checkout, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        make_mbed_lib_reference(fs_root, ref_url="https://git#lajdhalk234")

        mock_clone.side_effect = lambda url, dst_dir, *args: dst_dir.mkdir()

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.fetch()

        mock_fetch.assert_not_called()
        mock_checkout.assert_not_called()

    def test_does_not_resolve_references_in_ignore_paths(self, mock_get_repo, mock_checkout, mock_clone, tmp_path):
        fs_root = pathlib.Path(tmp_path, "mbed-os")
        make_mbed_lib_reference(fs_root, ref_url="https://git#lajdhalk234")

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.fetch()

        mock_clone.assert_not_called()

    def test_fetches_only_requested_ref(self, mock_repo, tmp_path):
        fs_root = pathlib.Path(tmp_path, "foo")
        fake_ref = "28eeee2b4c169739192600b92e7970dbbcabd8d0"
        make_mbed_lib_reference(fs_root, ref_url=f"https://git#{fake_ref}", resolved=True)

        lib_refs = LibraryReferences(fs_root, ignore_paths=["mbed-os"])
        lib_refs.checkout(force=False)

        mock_repo().git.fetch.assert_called_once_with("origin", fake_ref)
