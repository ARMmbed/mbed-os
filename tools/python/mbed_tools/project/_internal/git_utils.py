#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Wrappers for git operations."""
from dataclasses import dataclass
from pathlib import Path

import git
import logging

from mbed_tools.project.exceptions import VersionControlError
from mbed_tools.project._internal.progress import ProgressReporter
from typing import Optional

logger = logging.getLogger(__name__)


@dataclass
class GitReference:
    """Git reference for a remote repository.

    Attributes:
        repo_url: URL of the git repository.
        ref: The reference commit sha, tag or branch.
    """

    repo_url: str
    ref: str


def clone(url: str, dst_dir: Path, ref: Optional[str] = None, depth: int = 1) -> git.Repo:
    """Clone a library repository.

    Args:
        url: URL of the remote to clone.
        dst_dir: Destination directory for the cloned repo.
        ref: An optional git commit hash, branch or tag reference to checkout
        depth: Truncate history to the specified number of commits. Defaults to
               1, to make a shallow clone.

    Raises:
        VersionControlError: Cloning the repository failed.
    """
    # Gitpython doesn't propagate the git error message when a repo is already
    # cloned, so we cannot depend on git to handle the "already cloned" error.
    # We must handle this ourselves instead.
    if dst_dir.exists() and list(dst_dir.glob("*")):
        raise VersionControlError(f"{dst_dir} exists and is not an empty directory.")

    clone_from_kwargs = {"url": url, "to_path": str(dst_dir), "progress": ProgressReporter(name=url), "depth": depth}
    if ref:
        clone_from_kwargs["branch"] = ref

    try:
        return git.Repo.clone_from(**clone_from_kwargs)
    except git.exc.GitCommandError as err:
        raise VersionControlError(f"Cloning git repository from url '{url}' failed. Error from VCS: {err}")


def checkout(repo: git.Repo, ref: str, force: bool = False) -> None:
    """Check out a specific reference in the given repository.

    Args:
        repo: git.Repo object where the checkout will be performed.
        ref: Git commit hash, branch or tag reference, must be a valid ref defined in the repo.

    Raises:
        VersionControlError: Check out failed.
    """
    try:
        git_args = [ref] + ["--force"] if force else [ref]
        repo.git.checkout(*git_args)
    except git.exc.GitCommandError as err:
        raise VersionControlError(f"Failed to check out revision '{ref}'. Error from VCS: {err}")


def fetch(repo: git.Repo, ref: str) -> None:
    """Fetch from the repo's origin.

    Args:
        repo: git.Repo object where the checkout will be performed.
        ref: Git commit hash, branch or tag reference, must be a valid ref defined in the repo.

    Raises:
        VersionControlError: Fetch failed.
    """
    try:
        repo.git.fetch("origin", ref)
    except git.exc.GitCommandError as err:
        raise VersionControlError(f"Failed to fetch. Error from VCS: {err}")


def init(path: Path) -> git.Repo:
    """Initialise a git repository at the given path.

    Args:
        path: Path where the repo will be initialised.

    Returns:
        Initialised git.Repo object.

    Raises:
        VersionControlError: initalising the repository failed.
    """
    try:
        return git.Repo.init(str(path))
    except git.exc.GitCommandError as err:
        raise VersionControlError(f"Failed to initialise git repository at path '{path}'. Error from VCS: {err}")


def get_repo(path: Path) -> git.Repo:
    """Get a git.Repo object from an existing repository path.

    Args:
        path: Path to the git repository.

    Returns:
        git.Repo object.

    Raises:
        VersionControlError: No valid git repository at this path.
    """
    try:
        return git.Repo(str(path))
    except git.exc.InvalidGitRepositoryError:
        raise VersionControlError(
            "Could not find a valid git repository at this path. Please perform a `git init` command."
        )


def get_default_branch(repo: git.Repo) -> str:
    """Get a default branch from an existing git.Repo.

    Args:
        repo: git.Repo object

    Returns:
        The default branch name as a string.

    Raises:
        VersionControlError: Could not find the default branch name.
    """
    try:
        return str(repo.git.symbolic_ref("refs/remotes/origin/HEAD").rsplit("/", maxsplit=1)[-1])
    except git.exc.GitCommandError as err:
        raise VersionControlError(f"Could not resolve default repository branch name. Error from VCS: {err}")
