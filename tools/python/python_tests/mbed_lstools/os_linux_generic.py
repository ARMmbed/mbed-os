'''
mbed SDK
Copyright (c) 2011-2018 ARM Limited

Licensed under the Apache License, Version 2.0 (the 'License');
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an 'AS IS' BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
'''

import unittest
import sys
import os
from mock import patch, mock_open
from mbed_lstools.linux import MbedLsToolsLinuxGeneric


class LinuxPortTestCase(unittest.TestCase):
    ''' Basic test cases checking trivial asserts
    '''

    def setUp(self):
        self.linux_generic = MbedLsToolsLinuxGeneric()

    def tearDown(self):
        pass

    vfat_devices = [
        b'/dev/sdb on /media/usb0 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdd on /media/usb2 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sde on /media/usb3 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdc on /media/usb1 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)'
    ]

    def test_get_mount_point_basic(self):
        with patch('mbed_lstools.linux.MbedLsToolsLinuxGeneric._run_cli_process') as _cliproc:
            _cliproc.return_value = (b'\n'.join(self.vfat_devices), None, 0)
            mount_dict = dict(self.linux_generic._fat_mounts())
            _cliproc.assert_called_once_with('mount')
        self.assertEqual('/media/usb0', mount_dict['/dev/sdb'])
        self.assertEqual('/media/usb2', mount_dict['/dev/sdd'])
        self.assertEqual('/media/usb3', mount_dict['/dev/sde'])
        self.assertEqual('/media/usb1', mount_dict['/dev/sdc'])


    vfat_devices_ext = [
        b'/dev/sdb on /media/MBED_xxx type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdd on /media/MBED___x type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sde on /media/MBED-xxx type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdc on /media/MBED_x-x type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',

        b'/dev/sda on /mnt/NUCLEO type vfat (rw,relatime,uid=999,fmask=0133,dmask=0022,codepage=437,iocharset=ascii,shortname=mixed,utf8,flush,errors=remount-ro,uhelper=ldm)',
        b'/dev/sdf on /mnt/NUCLEO_ type vfat (rw,relatime,uid=999,fmask=0133,dmask=0022,codepage=437,iocharset=ascii,shortname=mixed,utf8,flush,errors=remount-ro,uhelper=ldm)',
        b'/dev/sdg on /mnt/DAPLINK type vfat (rw,relatime,sync,uid=999,fmask=0022,dmask=0022,codepage=437,iocharset=ascii,shortname=mixed,errors=remount-ro,uhelper=ldm)',
        b'/dev/sdh on /mnt/DAPLINK_ type vfat (rw,relatime,sync,uid=999,fmask=0022,dmask=0022,codepage=437,iocharset=ascii,shortname=mixed,errors=remount-ro,uhelper=ldm)',
        b'/dev/sdi on /mnt/DAPLINK__ type vfat (rw,relatime,sync,uid=999,fmask=0022,dmask=0022,codepage=437,iocharset=ascii,shortname=mixed,errors=remount-ro,uhelper=ldm)',
    ]

    def test_get_mount_point_ext(self):
        with patch('mbed_lstools.linux.MbedLsToolsLinuxGeneric._run_cli_process') as _cliproc:
            _cliproc.return_value = (b'\n'.join(self.vfat_devices_ext), None, 0)
            mount_dict = dict(self.linux_generic._fat_mounts())
            _cliproc.assert_called_once_with('mount')
        self.assertEqual('/media/MBED_xxx', mount_dict['/dev/sdb'])
        self.assertEqual('/media/MBED___x', mount_dict['/dev/sdd'])
        self.assertEqual('/media/MBED-xxx', mount_dict['/dev/sde'])
        self.assertEqual('/media/MBED_x-x', mount_dict['/dev/sdc'])

        self.assertEqual('/mnt/NUCLEO', mount_dict['/dev/sda'])
        self.assertEqual('/mnt/NUCLEO_', mount_dict['/dev/sdf'])
        self.assertEqual('/mnt/DAPLINK', mount_dict['/dev/sdg'])
        self.assertEqual('/mnt/DAPLINK_', mount_dict['/dev/sdh'])
        self.assertEqual('/mnt/DAPLINK__', mount_dict['/dev/sdi'])

    def find_candidates_with_patch(self, mount_list, link_dict, listdir_dict, open_dict):
        if not getattr(sys.modules['os'], 'readlink', None):
            sys.modules['os'].readlink = None

        def do_open(path, mode='r'):
            path = path.replace('\\', '/')
            file_object = mock_open(read_data=open_dict[path]).return_value
            file_object.__iter__.return_value = open_dict[path].splitlines(True)
            return file_object

        with patch('mbed_lstools.linux.MbedLsToolsLinuxGeneric._run_cli_process') as _cliproc,\
             patch('os.readlink') as _readlink,\
             patch('os.listdir') as _listdir,\
             patch('os.path.abspath') as _abspath,\
             patch('mbed_os_tools.detect.linux.open', do_open) as _,\
             patch('os.path.isdir') as _isdir:
            _isdir.return_value = True
            _cliproc.return_value = (b'\n'.join(mount_list), None, 0)
            def do_readlink(link):
                # Fix for testing on Windows
                link = link.replace('\\', '/')
                return link_dict[link]
            _readlink.side_effect = do_readlink
            def do_listdir(dir):
                # Fix for testing on Windows
                dir = dir.replace('\\', '/')
                return listdir_dict[dir]
            _listdir.side_effect = do_listdir
            def do_abspath(dir):
                _, path = os.path.splitdrive(
                    os.path.normpath(os.path.join(os.getcwd(), dir)))
                path = path.replace('\\', '/')
                return path
            _abspath.side_effect = do_abspath
            ret_val = self.linux_generic.find_candidates()
            _cliproc.assert_called_once_with('mount')
            return ret_val


    listdir_dict_rpi = {
        '/dev/disk/by-id': [
            'usb-MBED_VFS_0240000028634e4500135006691700105f21000097969900-0:0',
            'usb-MBED_VFS_0240000028884e450018700f6bf000338021000097969900-0:0',
            'usb-MBED_VFS_0240000028884e45001f700f6bf000118021000097969900-0:0',
            'usb-MBED_VFS_0240000028884e450036700f6bf000118021000097969900-0:0',
            'usb-MBED_VFS_0240000029164e45001b0012706e000df301000097969900-0:0',
            'usb-MBED_VFS_0240000029164e45002f0012706e0006f301000097969900-0:0',
            'usb-MBED_VFS_9900000031864e45000a100e0000003c0000000097969901-0:0'
        ],
        '/dev/serial/by-id': [
            'usb-ARM_DAPLink_CMSIS-DAP_0240000028634e4500135006691700105f21000097969900-if01',
            'usb-ARM_DAPLink_CMSIS-DAP_0240000028884e450018700f6bf000338021000097969900-if01',
            'usb-ARM_DAPLink_CMSIS-DAP_0240000028884e450036700f6bf000118021000097969900-if01',
            'usb-ARM_DAPLink_CMSIS-DAP_0240000029164e45001b0012706e000df301000097969900-if01',
            'usb-ARM_BBC_micro:bit_CMSIS-DAP_9900000031864e45000a100e0000003c0000000097969901-if01'
        ],
        '/sys/class/block': [
            'sdb',
            'sdc',
            'sdd',
            'sde',
            'sdf',
            'sdg',
            'sdh',
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-1/1-1.2/1-1.2.6': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-4': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-5': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-6': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-7': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-8': [
            'idVendor',
            'idProduct'
        ]
    }

    open_dict_rpi = {
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-1/1-1.2/1-1.2.6/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-1/1-1.2/1-1.2.6/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-4/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-4/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-5/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-5/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-6/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-6/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-7/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-7/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-8/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-8/idProduct': '0204\n'
    }

    link_dict_rpi = {
        '/dev/disk/by-id/usb-MBED_VFS_0240000028634e4500135006691700105f21000097969900-0:0': '../../sdb',
        '/dev/disk/by-id/usb-MBED_VFS_0240000028884e450018700f6bf000338021000097969900-0:0': '../../sdc',
        '/dev/disk/by-id/usb-MBED_VFS_0240000028884e45001f700f6bf000118021000097969900-0:0': '../../sdd',
        '/dev/disk/by-id/usb-MBED_VFS_0240000028884e450036700f6bf000118021000097969900-0:0': '../../sde',
        '/dev/disk/by-id/usb-MBED_VFS_0240000029164e45001b0012706e000df301000097969900-0:0': '../../sdf',
        '/dev/disk/by-id/usb-MBED_VFS_0240000029164e45002f0012706e0006f301000097969900-0:0': '../../sdg',
        '/dev/disk/by-id/usb-MBED_VFS_9900000031864e45000a100e0000003c0000000097969901-0:0': '../../sdh',
        '/dev/serial/by-id/usb-ARM_DAPLink_CMSIS-DAP_0240000028634e4500135006691700105f21000097969900-if01': '../../ttyACM0',
        '/dev/serial/by-id/usb-ARM_DAPLink_CMSIS-DAP_0240000028884e450018700f6bf000338021000097969900-if01': '../../ttyACM1',
        '/dev/serial/by-id/usb-ARM_DAPLink_CMSIS-DAP_0240000028884e450036700f6bf000118021000097969900-if01': '../../ttyACM3',
        '/dev/serial/by-id/usb-ARM_DAPLink_CMSIS-DAP_0240000029164e45001b0012706e000df301000097969900-if01': '../../ttyACM2',
        '/dev/serial/by-id/usb-ARM_BBC_micro:bit_CMSIS-DAP_9900000031864e45000a100e0000003c0000000097969901-if01': '../../ttyACM4',
        '/sys/class/block/sdb': '../../devices/pci0000:00/0000:00:06.0/usb1/1-1/1-1.2/1-1.2.6/1-1.2.6:1.0/host8568/target8568:0:0/8568:0:0:0/block/sdb',
        '/sys/class/block/sdc': '../../devices/pci0000:00/0000:00:06.0/usb1/1-3/1-3:1.0/host4/target4:0:0/4:0:0:0/block/sdc',
        '/sys/class/block/sdd': '../../devices/pci0000:00/0000:00:06.0/usb1/1-4/1-4:1.0/host5/target5:0:0/5:0:0:0/block/sdd',
        '/sys/class/block/sde': '../../devices/pci0000:00/0000:00:06.0/usb1/1-5/1-5:1.0/host6/target6:0:0/6:0:0:0/block/sde',
        '/sys/class/block/sdf': '../../devices/pci0000:00/0000:00:06.0/usb1/1-6/1-6:1.0/host7/target7:0:0/7:0:0:0/block/sdf',
        '/sys/class/block/sdg': '../../devices/pci0000:00/0000:00:06.0/usb1/1-7/1-7:1.0/host8/target8:0:0/8:0:0:0/block/sdg',
        '/sys/class/block/sdh': '../../devices/pci0000:00/0000:00:06.0/usb1/1-8/1-7:1.0/host9/target9:0:0/9:0:0:0/block/sdh'
    }

    mount_list_rpi = [
        b'/dev/sdb on /media/usb0 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdc on /media/usb1 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdd on /media/usb2 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sde on /media/usb3 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdf on /media/usb4 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdg on /media/usb5 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdh on /media/usb6 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)'
    ]
    def test_get_detected_rpi(self):
        mbed_det = self.find_candidates_with_patch(
            self.mount_list_rpi, self.link_dict_rpi, self.listdir_dict_rpi, self.open_dict_rpi)

        self.assertIn({
            'mount_point': '/media/usb0',
            'serial_port': '/dev/ttyACM0',
            'target_id_usb_id': '0240000028634e4500135006691700105f21000097969900',
            'vendor_id': '0d28',
            'product_id': '0204'
          }, mbed_det)
        self.assertIn({
            'mount_point': '/media/usb1',
            'serial_port': '/dev/ttyACM1',
            'target_id_usb_id': '0240000028884e450018700f6bf000338021000097969900',
            'vendor_id': '0d28',
            'product_id': '0204'
          }, mbed_det)
        self.assertIn({
            'mount_point': '/media/usb4',
            'serial_port': '/dev/ttyACM2',
            'target_id_usb_id': '0240000029164e45001b0012706e000df301000097969900',
            'vendor_id': '0d28',
            'product_id': '0204'
          }, mbed_det)
        self.assertIn({
            'mount_point': '/media/usb3',
            'serial_port': '/dev/ttyACM3',
            'target_id_usb_id': '0240000028884e450036700f6bf000118021000097969900',
            'vendor_id': '0d28',
            'product_id': '0204'
          }, mbed_det)
        self.assertIn({
            'mount_point': '/media/usb6',
            'serial_port': '/dev/ttyACM4',
            'target_id_usb_id': '9900000031864e45000a100e0000003c0000000097969901',
            'vendor_id': '0d28',
            'product_id': '0204'
        }, mbed_det)


    listdir_dict_1 = {
        '/dev/disk/by-id': [
            '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM',
            '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part1',
            '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part2',
            '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part5',
            '/dev/disk/by-id/ata-TSSTcorpDVD-ROM_TS-H352C',
            '/dev/disk/by-id/usb-MBED_FDi_sk_A000000001-0:0',
            '/dev/disk/by-id/usb-MBED_microcontroller_0240020152986E5EAF6693E6-0:0',
            '/dev/disk/by-id/wwn-0x5000cca30ccffb77',
            '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part1',
            '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part2',
            '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part5',
        ],
        '/dev/serial/by-id': [
            '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_0240020152986E5EAF6693E6-if01',
            '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_A000000001-if01',
        ],
        '/sys/class/block': [
            'sdb',
            'sdc'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3': [
            'idVendor',
            'idProduct'
        ]
    }

    link_dict_1 = {
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM': '../../sda',
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part1': '../../sda1',
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part2': '../../sda2',
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part5': '../../sda5',
        '/dev/disk/by-id/ata-TSSTcorpDVD-ROM_TS-H352C': '../../sr0',
        '/dev/disk/by-id/usb-MBED_FDi_sk_A000000001-0:0': '../../sdc',
        '/dev/disk/by-id/usb-MBED_microcontroller_0240020152986E5EAF6693E6-0:0': '../../sdb',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77': '../../sda',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part1': '../../sda1',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part2': '../../sda2',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part5': '../../sda5',
        '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_0240020152986E5EAF6693E6-if01': '../../ttyACM1',
        '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_A000000001-if01': '../../ttyACM0',
        '/sys/class/block/sdb': '../../devices/pci0000:00/0000:00:06.0/usb1/1-2/1-2:1.0/host3/target3:0:0/3:0:0:0/block/sdb',
        '/sys/class/block/sdc': '../../devices/pci0000:00/0000:00:06.0/usb1/1-3/1-3:1.0/host4/target4:0:0/4:0:0:0/block/sdc'
    }

    open_dict_1 = {
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3/idProduct': '0204\n'
    }

    mount_list_1 = [
        b'/dev/sdb on /media/usb0 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdc on /media/usb1 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)'
    ]
    def test_get_detected_1_k64f(self):
        mbed_det = self.find_candidates_with_patch(
            self.mount_list_1, self.link_dict_1, self.listdir_dict_1, self.open_dict_1)
        self.assertIn({
            'mount_point': '/media/usb0',
            'serial_port': '/dev/ttyACM1',
            'target_id_usb_id': '0240020152986E5EAF6693E6',
            'vendor_id': '0d28',
            'product_id': '0204'
          }, mbed_det)

        self.assertIn({
            'mount_point': '/media/usb1',
            'serial_port': '/dev/ttyACM0',
            'target_id_usb_id': 'A000000001',
            'vendor_id': '0d28',
            'product_id': '0204'
          }, mbed_det)


    listdir_dict_2 = {
        '/dev/disk/by-id': [
            'ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM',
            'ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part1',
            'ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part2',
            'ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part5',
            'ata-TSSTcorpDVD-ROM_TS-H352C',
            'usb-MBED_FDi_sk_A000000001-0:0',
            'usb-MBED_microcontroller_02400201489A1E6CB564E3D4-0:0',
            'usb-MBED_microcontroller_0240020152986E5EAF6693E6-0:0',
            'usb-MBED_microcontroller_0240020152A06E54AF5E93EC-0:0',
            'usb-MBED_microcontroller_0672FF485649785087171742-0:0',
            'wwn-0x5000cca30ccffb77',
            'wwn-0x5000cca30ccffb77-part1',
            'wwn-0x5000cca30ccffb77-part2',
            'wwn-0x5000cca30ccffb77-part5'
        ],
        '/dev/serial/by-id': [
            'usb-MBED_MBED_CMSIS-DAP_02400201489A1E6CB564E3D4-if01',
            'usb-MBED_MBED_CMSIS-DAP_0240020152986E5EAF6693E6-if01',
            'usb-MBED_MBED_CMSIS-DAP_0240020152A06E54AF5E93EC-if01',
            'usb-MBED_MBED_CMSIS-DAP_A000000001-if01',
            'usb-STMicroelectronics_STM32_STLink_0672FF485649785087171742-if02'
        ],
        '/sys/class/block': [
            'sdb',
            'sdc',
            'sdd',
            'sde',
            'sdf'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-4': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-5': [
            'idVendor',
            'idProduct'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-6': [
            'idVendor',
            'idProduct'
        ]
    }

    open_dict_2 = {
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-3/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-4/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-4/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-5/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-5/idProduct': '0204\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-6/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-6/idProduct': '0204\n'
    }

    link_dict_2 = {
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM': '../../sda',
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part1': '../../sda1',
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part2': '../../sda2',
        '/dev/disk/by-id/ata-HDS728080PLA380_40Y9028LEN_PFDB32S7S44XLM-part5': '../../sda5',
        '/dev/disk/by-id/ata-TSSTcorpDVD-ROM_TS-H352C': '../../sr0',
        '/dev/disk/by-id/usb-MBED_FDi_sk_A000000001-0:0': '../../sdc',
        '/dev/disk/by-id/usb-MBED_microcontroller_02400201489A1E6CB564E3D4-0:0': '../../sde',
        '/dev/disk/by-id/usb-MBED_microcontroller_0240020152986E5EAF6693E6-0:0': '../../sdb',
        '/dev/disk/by-id/usb-MBED_microcontroller_0240020152A06E54AF5E93EC-0:0': '../../sdf',
        '/dev/disk/by-id/usb-MBED_microcontroller_0672FF485649785087171742-0:0': '../../sdd',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77': '../../sda',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part1': '../../sda1',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part2': '../../sda2',
        '/dev/disk/by-id/wwn-0x5000cca30ccffb77-part5': '../../sda5',
        '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_02400201489A1E6CB564E3D4-if01': '../../ttyACM3',
        '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_0240020152986E5EAF6693E6-if01': '../../ttyACM1',
        '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_0240020152A06E54AF5E93EC-if01': '../../ttyACM4',
        '/dev/serial/by-id/usb-MBED_MBED_CMSIS-DAP_A000000001-if01': '../../ttyACM0',
        '/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_0672FF485649785087171742-if02': '../../ttyACM2',
        '/sys/class/block/sdb': '../../devices/pci0000:00/0000:00:06.0/usb1/1-2/1-2:1.0/host3/target3:0:0/3:0:0:0/block/sdb',
        '/sys/class/block/sdc': '../../devices/pci0000:00/0000:00:06.0/usb1/1-3/1-3:1.0/host4/target4:0:0/4:0:0:0/block/sdc',
        '/sys/class/block/sdd': '../../devices/pci0000:00/0000:00:06.0/usb1/1-4/1-4:1.0/host5/target5:0:0/5:0:0:0/block/sdd',
        '/sys/class/block/sde': '../../devices/pci0000:00/0000:00:06.0/usb1/1-5/1-5:1.0/host6/target6:0:0/6:0:0:0/block/sde',
        '/sys/class/block/sdf': '../../devices/pci0000:00/0000:00:06.0/usb1/1-6/1-6:1.0/host7/target7:0:0/7:0:0:0/block/sdf'
    }

    mount_list_2 = [
        b'/dev/sdb on /media/usb0 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdc on /media/usb1 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdd on /media/usb2 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sde on /media/usb3 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)',
        b'/dev/sdf on /media/usb4 type vfat (rw,noexec,nodev,sync,noatime,nodiratime,gid=1000,uid=1000,dmask=000,fmask=000)'
    ]
    def test_get_detected_2_k64f(self):
        mbed_det = self.find_candidates_with_patch(
            self.mount_list_2, self.link_dict_2, self.listdir_dict_2, self.open_dict_2)

        self.assertIn({
            'mount_point': '/media/usb1',
            'serial_port': '/dev/ttyACM0',
            'target_id_usb_id': 'A000000001',
            'vendor_id': '0d28',
            'product_id': '0204'
          },
          mbed_det)
        self.assertIn({
            'mount_point': '/media/usb2',
            'serial_port': '/dev/ttyACM2',
            'target_id_usb_id': '0672FF485649785087171742',
            'vendor_id': '0d28',
            'product_id': '0204'
          },
          mbed_det)

        self.assertIn({
            'mount_point': '/media/usb4',
            'serial_port': '/dev/ttyACM4',
            'target_id_usb_id': '0240020152A06E54AF5E93EC',
            'vendor_id': '0d28',
            'product_id': '0204'
          },
          mbed_det)

        self.assertIn({
            'mount_point': '/media/usb3',
            'serial_port': '/dev/ttyACM3',
            'target_id_usb_id': '02400201489A1E6CB564E3D4',
            'vendor_id': '0d28',
            'product_id': '0204'
          },
          mbed_det)

        self.assertIn({
            'mount_point': '/media/usb0',
            'serial_port': '/dev/ttyACM1',
            'target_id_usb_id': '0240020152986E5EAF6693E6',
            'vendor_id': '0d28',
            'product_id': '0204'
          },
          mbed_det)


    listdir_dict_4 = {
        '/dev/disk/by-id': [
            'ata-VMware_Virtual_SATA_CDRW_Drive_00000000000000000001',
            'ata-VMware_Virtual_SATA_CDRW_Drive_01000000000000000001',
            'usb-MBED_VFS_0240000033514e45001f500585d40014e981000097969900-0:0'
        ],
        '/dev/serial/by-id': [
            'pci-ARM_DAPLink_CMSIS-DAP_0240000033514e45001f500585d40014e981000097969900-if01'
        ],
        '/sys/class/block': [
            'sdb'
        ],
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2': [
            'idVendor',
            'idProduct'
        ],
    }

    open_dict_4 = {
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2/idVendor': '0d28\n',
        '/sys/class/block/../../devices/pci0000:00/0000:00:06.0/usb1/1-2/idProduct': '0204\n'
    }

    link_dict_4 = {
        '/dev/disk/by-id/ata-VMware_Virtual_SATA_CDRW_Drive_00000000000000000001': '../../sr0',
        '/dev/disk/by-id/ata-VMware_Virtual_SATA_CDRW_Drive_01000000000000000001': '../../sr1',
        '/dev/disk/by-id/usb-MBED_VFS_0240000033514e45001f500585d40014e981000097969900-0:0': '../../sdb',
        '/dev/serial/by-id/pci-ARM_DAPLink_CMSIS-DAP_0240000033514e45001f500585d40014e981000097969900-if01': '../../ttyACM0',
        '/sys/class/block/sdb': '../../devices/pci0000:00/0000:00:06.0/usb1/1-2/1-2:1.0/host3/target3:0:0/3:0:0:0/block/sdb'
    }

    mount_list_4 = [
        b'/dev/sdb on /media/przemek/DAPLINK type vfat (rw,nosuid,nodev,relatime,uid=1000,gid=1000,fmask=0022,dmask=0022,codepage=437,iocharset=iso8859-1,shortname=mixed,showexec,utf8,flush,errors=remount-ro,uhelper=udisks2)'
    ]
    def test_get_detected_3_k64f(self):
        mbed_det = self.find_candidates_with_patch(
            self.mount_list_4, self.link_dict_4, self.listdir_dict_4, self.open_dict_4)

        self.assertIn({
            'mount_point': '/media/przemek/DAPLINK',
            'serial_port': '/dev/ttyACM0',
            'target_id_usb_id': '0240000033514e45001f500585d40014e981000097969900',
            'vendor_id': '0d28',
            'product_id': '0204'
          },
          mbed_det)


if __name__ == '__main__':
    unittest.main()
