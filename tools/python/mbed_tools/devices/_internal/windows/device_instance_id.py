#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Utility in charge of finding the instance ID of a device."""
import win32con
import win32api
from mbed_tools.devices._internal.exceptions import SystemException
import logging

from typing import Optional, Any

logger = logging.getLogger(__name__)


class RegKey(object):
    """Context manager in charge of opening and closing registry keys."""

    def __init__(self, sub_registry_key: str) -> None:
        """Initialiser."""
        access = win32con.KEY_READ | win32con.KEY_ENUMERATE_SUB_KEYS | win32con.KEY_QUERY_VALUE
        try:
            self._hkey = win32api.RegOpenKey(win32con.HKEY_LOCAL_MACHINE, sub_registry_key, 0, access)
        except win32api.error as e:
            raise SystemException(f"Could not read key [{sub_registry_key}] in the registry: {e}")

    def __enter__(self) -> Any:
        """Actions on entry."""
        return self._hkey

    def __exit__(self, type: Any, value: Any, traceback: Any) -> None:
        """Actions on exit."""
        win32api.RegCloseKey(self._hkey)
        self._hkey.close()


def get_children_instance_id(pnpid: str) -> Optional[str]:
    """Gets the USB children instance ID from the plug and play ID.

    See https://docs.microsoft.com/en-us/windows-hardware/drivers/install/instance-ids.
    """
    # Although the registry should not be accessed directly
    # (See https://docs.microsoft.com/en-us/windows-hardware/drivers/install/hklm-system-currentcontrolset-enum-registry-tree), # noqa E501
    # and SetupDi functions/APIs should be used instead in a similar fashion to Miro utility
    # (See https://github.com/cool-RR/Miro/blob/7b9ecd9bc0878e463f5a5e26e8b00b675e3f98ac/tv/windows/plat/usbutils.py)
    # Most libraries seems to be reading the registry:
    #     - Pyserial: https://github.com/pyserial/pyserial/blob/master/serial/tools/list_ports_windows.py
    #     - Node serialport: https://github.com/serialport/node-serialport/blob/cd112ca5a3a3fe186e1ac6fa78eeeb5ea7396185/packages/bindings/src/serialport_win.cpp # noqa E501
    #     - USB device forensics: https://github.com/woanware/usbdeviceforensics/blob/master/pyTskusbdeviceforensics.py
    # For more details about the registry key actually looked at, See:
    # - https://stackoverflow.com/questions/3331043/get-list-of-connected-usb-devices
    # - https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/usb-device-specific-registry-settings
    key_path = f"SYSTEM\\CurrentControlSet\\Enum\\{pnpid}"
    with RegKey(key_path) as hkey:
        try:
            value = win32api.RegQueryValueEx(hkey, "ParentIdPrefix")
            return str(value[0]) if value else None
        except win32api.error as e:
            logger.debug(f"Error occurred reading `ParentIdPrefix` field of key [{key_path}] in the registry: {e}")
            return None
