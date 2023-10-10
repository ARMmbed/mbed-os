#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
from pathlib import Path
from unittest import mock

import pytest

from mbed_tools.project.exceptions import VersionControlError
from mbed_tools.project._internal import git_utils


@pytest.fixture
def mock_repo():
    with mock.patch("mbed_tools.project._internal.git_utils.git.Repo") as repo:
        yield repo


@pytest.fixture
def mock_progress():
    with mock.patch("mbed_tools.project._internal.git_utils.ProgressReporter") as progress:
        yield progress


class TestClone:
    def test_returns_repo(self, mock_progress, mock_repo, tmp_path):
        url = "https://blah"
        path = Path(tmp_path, "dst")
        repo = git_utils.clone(url, path)

        assert repo is not None
        mock_repo.clone_from.assert_called_once_with(url=url, to_path=str(path), progress=mock_progress(), depth=1)

    def test_returns_repo_for_ref(self, mock_progress, mock_repo, tmp_path):
        url = "https://example.com/org/repo.git"
        ref = "development"
        path = Path(tmp_path, "repo")
        repo = git_utils.clone(url, path, ref)

        assert repo is not None
        mock_repo.clone_from.assert_called_once_with(
            url=url, to_path=str(path), progress=mock_progress(), depth=1, branch=ref
        )

    def test_raises_when_fails_due_to_bad_url(self, tmp_path):
        with pytest.raises(VersionControlError, match="from url 'bad' failed"):
            git_utils.clone("bad", Path(tmp_path, "dst"))

    def test_raises_when_fails_due_to_bad_url_with_ref(self, mock_progress, mock_repo, tmp_path):
        url = "https://example.com/org/repo.git"
        ref = "development"
        path = Path(tmp_path, "repo")

        mock_repo.clone_from.side_effect = git_utils.git.exc.GitCommandError("git clone", 255)

        with pytest.raises(VersionControlError, match=f"Cloning git repository from url '{url}' failed."):
            git_utils.clone(url, path, ref)

    def test_raises_when_fails_due_to_existing_nonempty_dst_dir(self, mock_repo, tmp_path):
        dst_dir = Path(tmp_path, "dst")
        dst_dir.mkdir()
        (dst_dir / "some_file.txt").touch()

        with pytest.raises(VersionControlError, match="exists and is not an empty directory"):
            git_utils.clone("https://blah", dst_dir)

    def test_can_clone_to_empty_existing_dst_dir(self, mock_repo, tmp_path, mock_progress):
        dst_dir = Path(tmp_path, "dst")
        dst_dir.mkdir()
        url = "https://repo"

        repo = git_utils.clone(url, dst_dir)

        assert repo is not None
        mock_repo.clone_from.assert_called_once_with(url=url, to_path=str(dst_dir), progress=mock_progress(), depth=1)


class TestInit:
    def test_returns_initialised_repo(self, mock_repo):
        repo = git_utils.init(Path())

        assert repo is not None
        mock_repo.init.assert_called_once_with(str(Path()))

    def test_raises_when_init_fails(self, mock_repo):
        mock_repo.init.side_effect = git_utils.git.exc.GitCommandError("git init", 255)

        with pytest.raises(VersionControlError):
            git_utils.init(Path())


class TestGetRepo:
    def test_returns_repo_object(self, mock_repo):
        repo = git_utils.get_repo(Path())

        assert isinstance(repo, mock_repo().__class__)

    def test_raises_version_control_error_when_no_git_repo_found(self, mock_repo):
        mock_repo.side_effect = git_utils.git.exc.InvalidGitRepositoryError

        with pytest.raises(VersionControlError):
            git_utils.get_repo(Path())


class TestCheckout:
    def test_git_lib_called_with_correct_command(self, mock_repo):
        git_utils.checkout(mock_repo, "master")

        mock_repo.git.checkout.assert_called_once_with("master")

    def test_git_lib_called_with_correct_command_with_force(self, mock_repo):
        git_utils.checkout(mock_repo, "master", force=True)

        mock_repo.git.checkout.assert_called_once_with("master", "--force")

    def test_raises_version_control_error_when_git_checkout_fails(self, mock_repo):
        mock_repo.git.checkout.side_effect = git_utils.git.exc.GitCommandError("git checkout", 255)

        with pytest.raises(VersionControlError):
            git_utils.checkout(mock_repo, "bad")


class TestFetch:
    def test_does_a_fetch(self, mock_repo):
        ref = "b23a8eb1c3f80292c8eb40689106759fae83a4c6"
        git_utils.fetch(mock_repo, ref)

        mock_repo.git.fetch.assert_called_once_with("origin", ref)

    def test_raises_when_fetch_fails(self, mock_repo):
        ref = "v2.7.9"
        mock_repo.git.fetch.side_effect = git_utils.git.exc.GitCommandError("git fetch", 255)

        with pytest.raises(VersionControlError):
            git_utils.fetch(mock_repo, ref)


class TestGetDefaultBranch:
    def test_returns_default_branch_name(self, mock_repo):
        mock_repo().git.symbolic_ref.return_value = "refs/remotes/origin/main"

        branch_name = git_utils.get_default_branch(mock_repo())

        assert branch_name == "main"

    def test_raises_version_control_error_when_git_command_fails(self, mock_repo):
        mock_repo().git.symbolic_ref.side_effect = git_utils.git.exc.GitCommandError("git symbolic_ref", 255)

        with pytest.raises(VersionControlError):
            git_utils.get_default_branch(mock_repo())
