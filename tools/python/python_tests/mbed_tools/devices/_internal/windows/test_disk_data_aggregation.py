#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import unittest
from python_tests.mbed_tools.devices.markers import windows_only


@windows_only
class TestDiskDataAggregrator(unittest.TestCase):
    def test_data_aggregation(self):
        from mbed_tools.devices._internal.windows.disk_drive import DiskDrive
        from mbed_tools.devices._internal.windows.disk_partition import DiskPartition
        from mbed_tools.devices._internal.windows.logical_disk import LogicalDisk
        from mbed_tools.devices._internal.windows.volume_set import VolumeInformation, DriveType
        from mbed_tools.devices._internal.windows.disk_aggregation import (
            AggregatedDiskData,
            DiskDataAggregator,
            WindowsUID,
        )

        disk1 = DiskDrive()
        disk1.set_data_values(
            dict(
                Availability=None,
                BytesPerSector=512,
                Capabilities=(3, 4, 7),
                CapabilityDescriptions=("Random Access", "Supports Writing", "Supports Removable Media"),
                Caption="MBED VFS USB Device",
                CompressionMethod=None,
                ConfigManagerErrorCode=0,
                ConfigManagerUserConfig=False,
                CreationClassName="Win32_DiskDrive",
                DefaultBlockSize=None,
                Description="Disk drive",
                DeviceID="\\\\.\\PHYSICALDRIVE1",
                ErrorCleared=None,
                ErrorDescription=None,
                ErrorMethodology=None,
                FirmwareRevision="0.1 ",
                Index=1,
                InstallDate=None,
                InterfaceType="USB",
                LastErrorCode=None,
                Manufacturer="(Standard disk drives)",
                MaxBlockSize=None,
                MaxMediaSize=None,
                MediaLoaded=True,
                MediaType="Removable Media",
                MinBlockSize=None,
                Model="MBED VFS USB Device",
                Name="\\\\.\\PHYSICALDRIVE1",
                NeedsCleaning=None,
                NumberOfMediaSupported=None,
                Partitions=1,
                PNPDeviceID="USBSTOR\\DISK&VEN_MBED&PROD_VFS&REV_0.1\\4454646",
                PowerManagementCapabilities=None,
                PowerManagementSupported=None,
                SCSIBus=0,
                SCSILogicalUnit=0,
                SCSIPort=0,
                SCSITargetId=0,
                SectorsPerTrack=63,
                SerialNumber="0240000034544e45001a00018aa900292011000097969900",
                Signature=1,
                Size="65802240",
                Status="OK",
                StatusInfo=None,
                SystemCreationClassName="Win32_ComputerSystem",
                SystemName="E112100",
                TotalCylinders="8",
                TotalHeads=255,
                TotalSectors="128520",
                TotalTracks="2040",
                TracksPerCylinder=255,
            )
        )
        partition1 = DiskPartition()
        partition1.set_data_values(
            dict(
                AdditionalAvailability=None,
                Availability=None,
                PowerManagementCapabilities=None,
                IdentifyingDescriptions=None,
                MaxQuiesceTime=None,
                OtherIdentifyingInfo=None,
                StatusInfo=None,
                PowerOnHours=None,
                TotalPowerOnHours=None,
                Access=None,
                BlockSize="512",
                Bootable=False,
                BootPartition=False,
                Caption="Disk #1, Partition #0",
                ConfigManagerErrorCode=None,
                ConfigManagerUserConfig=None,
                CreationClassName="Win32_DiskPartition",
                Description="16-bit FAT",
                DeviceID="Disk #1, Partition #0",
                DiskIndex=1,
                ErrorCleared=None,
                ErrorDescription=None,
                ErrorMethodology=None,
                HiddenSectors=None,
                Index=0,
                InstallDate=None,
                LastErrorCode=None,
                Name="Disk #1, Partition #0",
                NumberOfBlocks="131200",
                PNPDeviceID=None,
                PowerManagementSupported=None,
                PrimaryPartition=True,
                Purpose=None,
                RewritePartition=None,
                Size="67174400",
                StartingOffset="0",
                Status=None,
                SystemCreationClassName="Win32_ComputerSystem",
                SystemName="E112100",
                Type="16-bit FAT",
            )
        )
        logical_disk1 = LogicalDisk()
        logical_disk1.set_data_values(
            dict(
                Access=0,
                Availability=None,
                BlockSize=None,
                Caption="F:",
                ConfigManagerErrorCode=None,
                ConfigManagerUserConfig=None,
                CreationClassName="Win32_LogicalDisk",
                Description="Removable Disk",
                DeviceID="F:",
                ErrorCleared=None,
                ErrorDescription=None,
                ErrorMethodology=None,
                FreeSpace="67096576",
                InstallDate=None,
                LastErrorCode=None,
                Name="F:",
                NumberOfBlocks=None,
                PNPDeviceID=None,
                PowerManagementCapabilities=None,
                PowerManagementSupported=None,
                Purpose=None,
                Size="67104768",
                Status=None,
                StatusInfo=None,
                SystemCreationClassName="Win32_ComputerSystem",
                SystemName="E112100",
            )
        )
        disk_drives = {1: disk1}
        disk_partitions = {"Disk #1, Partition #0": partition1}
        logical_partition_relationships = {"F:": "Disk #1, Partition #0"}
        volume_information = VolumeInformation(
            Name="DAPLINK",
            SerialNumber=654449012,
            MaxComponentLengthOfAFileName=255,
            SysFlags=131590,
            FileSystem="FAT",
            UniqueName="\\\\?\\Volume{d0613192-49b4-11ea-99e5-c85b76dfd333}\\",
            DriveType=DriveType.DRIVE_REMOVABLE,
        )

        # TEST aggregation
        expected_aggregated_object = AggregatedDiskData()
        expected_aggregated_object.set_data_values(
            dict(
                uid=WindowsUID(uid="4454646", raw_uid=None, serial_number=None),
                label="F:",
                description="Removable Disk",
                free_space="67096576",
                size="67104768",
                partition_name="Disk #1, Partition #0",
                partition_type="16-bit FAT",
                volume_information=volume_information,
                caption="MBED VFS USB Device",
                physical_disk_name="\\\\.\\PHYSICALDRIVE1",
                model="MBED VFS USB Device",
                interface_type="USB",
                media_type="Removable Media",
                manufacturer="(Standard disk drives)",
                serial_number="0240000034544e45001a00018aa900292011000097969900",
                status="OK",
                pnp_device_id="USBSTOR\\DISK&VEN_MBED&PROD_VFS&REV_0.1\\4454646",
            )
        )

        self.assertTupleEqual(
            DiskDataAggregator(
                physical_disks=disk_drives,
                partition_disks=disk_partitions,
                logical_partition_relationships=logical_partition_relationships,
                lookup_volume_information=lambda ld: volume_information,
            )
            .aggregate(logical_disk1)
            .to_tuple(),
            expected_aggregated_object.to_tuple(),
        )

    def test_system_disk_data(self):
        # On Windows, C: is the default primary drive and hence we should always get information about it.
        from mbed_tools.devices._internal.windows.disk_aggregation import SystemDiskInformation
        from mbed_tools.devices._internal.windows.system_data_loader import SystemDataLoader

        disks = SystemDiskInformation(SystemDataLoader())
        c_data = disks.get_disk_information_by_label("c:")
        self.assertFalse(c_data.is_undefined)
        uid = c_data.uid
        self.assertIsNotNone(uid)
        labels = [d.label.lower() for d in disks.get_disk_information(uid)]
        self.assertTrue("c:" in labels)
