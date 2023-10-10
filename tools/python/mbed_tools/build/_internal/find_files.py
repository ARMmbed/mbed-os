#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Find files in MbedOS program directory."""
from pathlib import Path
import fnmatch
from typing import Callable, Iterable, Optional, List, Tuple

from mbed_tools.lib.json_helpers import decode_json_file


def find_files(filename: str, directory: Path) -> List[Path]:
    """Proxy to `_find_files`, which applies legacy filtering rules."""
    # Temporary workaround, which replicates hardcoded ignore rules from old tools.
    # Legacy list of ignored directories is longer, however "TESTS" and
    # "TEST_APPS" were the only ones that actually exist in the MbedOS source.
    # Ideally, this should be solved by putting an `.mbedignore` file in the root of MbedOS repo,
    # similarly to what the code below pretends is happening.
    legacy_ignore = MbedignoreFilter(("*/TESTS", "*/TEST_APPS"))
    return _find_files(filename, directory, [legacy_ignore])


def _find_files(filename: str, directory: Path, filters: Optional[List[Callable]] = None) -> List[Path]:
    """Recursively find files by name under a given directory.

    This function automatically applies rules from .mbedignore files found during traversal.

    It is important to realise that applied filters are "greedy". The moment a directory is filtered out,
    its children won't be traversed.

    Args:
        filename: Name of the file to look for.
        directory: Location where search starts.
        filters: Optional list of exclude filters to apply.
    """
    if filters is None:
        filters = []

    result: List[Path] = []

    # Directories and files to process
    children = list(directory.iterdir())

    # If .mbedignore is one of the children, we need to add it to filter list,
    # as it might contain rules for currently processed directory, as well as its descendants.
    mbedignore = Path(directory, ".mbedignore")
    if mbedignore in children:
        filters = filters + [MbedignoreFilter.from_file(mbedignore)]

    # Remove files and directories that don't match current set of filters
    filtered_children = filter_files(children, filters)

    for child in filtered_children:
        if child.is_symlink():
            child = child.absolute().resolve()

        if child.is_dir():
            # If processed child is a directory, recurse with current set of filters
            result += _find_files(filename, child, filters)

        if child.is_file() and child.name == filename:
            # We've got a match
            result.append(child)

    return result


def filter_files(files: Iterable[Path], filters: Iterable[Callable]) -> Iterable[Path]:
    """Filter given paths to files using filter callables."""
    return [file for file in files if all(f(file) for f in filters)]


class RequiresFilter:
    """Filter out mbed libraries not needed by application.

    The 'requires' config option in mbed_app.json can specify list of mbed
    libraries (mbed_lib.json) that application requires. Apply 'requires'
    filter to remove mbed_lib.json files not required by application.
    """

    def __init__(self, requires: Iterable[str]):
        """Initialise the filter attributes.

        Args:
            requires: List of required mbed libraries.
        """
        self._requires = requires

    def __call__(self, path: Path) -> bool:
        """Return True if no requires are specified or our lib name is in the list of required libs."""
        return decode_json_file(path).get("name", "") in self._requires or not self._requires


class LabelFilter:
    """Filter out given paths using path labelling rules.

    If a path is labelled with given type, but contains label value which is
    not allowed, it will be filtered out.

    An example of labelled path is "/mbed-os/rtos/source/TARGET_CORTEX/mbed_lib.json",
    where label type is "TARGET" and label value is "CORTEX".

    For example, given a label type "FEATURE" and allowed values ["FOO"]:
    - "/path/FEATURE_FOO/somefile.txt" will not be filtered out
    - "/path/FEATURE_BAZ/somefile.txt" will be filtered out
    - "/path/FEATURE_FOO/FEATURE_BAR/somefile.txt" will be filtered out
    """

    def __init__(self, label_type: str, allowed_label_values: Iterable[str]):
        """Initialise the filter attributes.

        Args:
            label_type: Type of the label to filter with. In filtered paths, it prefixes the value.
            allowed_label_values: Values which are allowed for the given label type.
        """
        self._label_type = label_type
        self._allowed_labels = set(f"{label_type}_{label_value}" for label_value in allowed_label_values)

    def __call__(self, path: Path) -> bool:
        """Return True if given path contains only allowed labels - should not be filtered out."""
        labels = set(part for part in path.parts if self._label_type in part)
        return labels.issubset(self._allowed_labels)


class MbedignoreFilter:
    """Filter out given paths based on rules found in .mbedignore files.

    Patterns in .mbedignore use unix shell-style wildcards (fnmatch). It means
    that functionality, although similar is different to that found in
    .gitignore and friends.
    """

    def __init__(self, patterns: Tuple[str, ...]):
        """Initialise the filter attributes.

        Args:
            patterns: List of patterns from .mbedignore to filter against.
        """
        self._patterns = patterns

    def __call__(self, path: Path) -> bool:
        """Return True if given path doesn't match .mbedignore patterns - should not be filtered out."""
        stringified = str(path)
        return not any(fnmatch.fnmatch(stringified, pattern) for pattern in self._patterns)

    @classmethod
    def from_file(cls, mbedignore_path: Path) -> "MbedignoreFilter":
        """Return new instance with patterns read from .mbedignore file.

        Constructed patterns are rooted in the directory of .mbedignore file.
        """
        lines = mbedignore_path.read_text().splitlines()
        pattern_lines = (line for line in lines if line.strip() and not line.startswith("#"))
        ignore_root = mbedignore_path.parent
        patterns = tuple(str(ignore_root.joinpath(pattern)) for pattern in pattern_lines)
        return cls(patterns)
