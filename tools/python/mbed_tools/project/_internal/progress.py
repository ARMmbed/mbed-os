#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Progress bar for git operations."""
import sys

from typing import Optional, Any

from git import RemoteProgress
from tqdm import tqdm


class ProgressBar(tqdm):
    """tqdm progress bar that can be used in a callback context."""

    total: Any

    def update_progress(self, block_num: float = 1, block_size: float = 1, total_size: float = None) -> None:
        """Update the progress bar.

        Args:
            block_num: Number of the current block.
            block_size: Size of the current block.
            total_size: Total size of all expected blocks.
        """
        if total_size is not None and self.total != total_size:
            self.total = total_size
        self.update(block_num * block_size - self.n)


class ProgressReporter(RemoteProgress):
    """GitPython RemoteProgress subclass that displays a progress bar for git fetch and push operations."""

    def __init__(self, *args: Any, name: str = "", **kwargs: Any) -> None:
        """Initialiser.

        Args:
            name: The name of the git repository to report progress on.
        """
        self.name = name
        super().__init__(*args, **kwargs)

    def update(self, op_code: int, cur_count: float, max_count: Optional[float] = None, message: str = "") -> None:
        """Called whenever the progress changes.

        Args:
            op_code: Integer describing the stage of the current operation.
            cur_count: Current item count.
            max_count: Maximum number of items expected.
            message: Message string describing the number of bytes transferred in the WRITING operation.
        """
        if self.BEGIN & op_code:
            self.bar = ProgressBar(total=max_count, file=sys.stderr, leave=False)

        self.bar.desc = f"{self.name} {self._cur_line}"
        self.bar.update_progress(block_num=cur_count, total_size=max_count)

        if self.END & op_code:
            self.bar.close()
