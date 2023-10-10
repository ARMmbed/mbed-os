#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a Disk partition/Logical disk relationship."""

from typing import NamedTuple

from mbed_tools.devices._internal.windows.component_descriptor import ComponentDescriptor


class DiskToPartitionMsdnDefinition(NamedTuple):
    """Msdn definition of a disk partition - logical disk relationship.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-logicaldisktopartition
    """

    EndingAddress: int
    StartingAddress: int
    Antecedent: str
    Dependent: str


class DiskPartitionLogicalDiskRelationship(ComponentDescriptor):
    """Disk partition as defined in Windows API.

    See https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/win32-logicaldisktopartition
    """

    def __init__(self) -> None:
        """Initialiser."""
        super().__init__(DiskToPartitionMsdnDefinition, win32_class_name="Win32_LogicalDiskToPartition")

    @property
    def component_id(self) -> str:
        """Returns the device id field."""
        return f"{self.get('Antecedent')}->{self.get('Dependent')}"

    @staticmethod
    def _parse_reference(ref: str) -> str:
        """Parses a Win32 to only retain the value."""
        return ref.replace("'", "").replace('"', "").split("=")[1]

    @property
    def logical_disk_id(self) -> str:
        """Gets the logical disk id."""
        return DiskPartitionLogicalDiskRelationship._parse_reference(self.get("Dependent"))

    @property
    def disk_partition_id(self) -> str:
        """Gets the disk partition id."""
        return DiskPartitionLogicalDiskRelationship._parse_reference(self.get("Antecedent"))
