#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Defines a generic Win32 component."""
import logging
from abc import ABC, abstractmethod
from typing import List, Any, Generator, Optional, NamedTuple, cast

import pythoncom
import win32com.client

from mbed_tools.devices._internal.windows.component_descriptor_utils import (
    UNKNOWN_VALUE,
    is_undefined_data_object,
)

NAMED_TUPLE_FIELDS_ATTRIBUTE = "_fields"

logger = logging.getLogger(__name__)


class ComponentDescriptor(ABC):
    """Win32 component descriptor."""

    def __init__(self, win32_definition: type, win32_class_name: str, win32_filter: Optional[str] = None):
        """Initialiser.

        Args:
            win32_definition: definition of the Windows component as defined in MSDN.
            win32_class_name: Win32 class name of the component
            win32_filter: Any extra filter to apply such as a subcategory of a Win32 class.
        """
        self._win32_definition = win32_definition
        self._win32_class_name = win32_class_name
        self._win32_filter = win32_filter

    def set_data_values(self, fields_values: dict) -> None:
        """Sets fields values based on what is defined in dictionary."""
        for k, v in fields_values.items():
            setattr(self, k, v)

    @property
    def win32_definition(self) -> type:
        """Gets descriptor definition."""
        return self._win32_definition

    @property
    def win32_class_name(self) -> str:
        """Returns the name of the Win32 Class."""
        return self._win32_class_name

    @property
    def field_names(self) -> List[str]:
        """Returns the names of all the fields of the descriptor."""
        return [k for k in getattr(self._win32_definition, NAMED_TUPLE_FIELDS_ATTRIBUTE)]

    @property
    @abstractmethod
    def component_id(self) -> str:
        """Returns the device ID field."""

    @property
    def win32_filter(self) -> Optional[str]:
        """Filter applied on a Win32 category.

        For instance, the current component can be a subclass/subcategory of a component exposed by Win32.
        """
        return self._win32_filter

    def to_tuple(self) -> NamedTuple:
        """Translates into named tuple."""
        return cast(NamedTuple, self.win32_definition(**{k: self.get(k) for k in self.field_names}))

    @property
    def is_undefined(self) -> bool:
        """Determines whether the structure is undefined or not i.e. none of the fields are actually defined."""
        return is_undefined_data_object(self.to_tuple())

    def get(self, field_name: str) -> Any:
        """Gets the field value."""
        try:
            return getattr(self, field_name)
        except AttributeError as e:
            logger.debug(f"Attribute [{field_name}] is undefined on this instance {self}: {e}")
            return UNKNOWN_VALUE

    def __str__(self) -> str:
        """String representation."""
        values = {k: v for k, v in self.__dict__.items() if not k.startswith("_")}
        return f"{self.__class__.__name__}({values})"


class Win32Wrapper:
    """Wraps win32 objects and methods in order to simplify their use."""

    def __init__(self) -> None:
        """Wrapper initialisation."""
        # Setting pyWin32 so that it can be used across multiple threads
        # See:
        # https://stackoverflow.com/questions/37258257/why-does-this-script-not-work-with-threading-python
        # https://gist.github.com/vlasenkov/f8fe40d5b2d9e43fd46ad8363067acce
        # https://stackoverflow.com/questions/26764978/using-win32com-with-multithreading/27966218#27966218
        pythoncom.CoInitialize()
        self.wmi = win32com.client.GetObject("winmgmts:")

    def _read_cdispatch_fields(self, win32_element: Any, element_fields_list: List[str]) -> dict:
        """Reads all the fields from a cdispatch object returned by pywin32."""
        if not win32_element:
            return dict()
        return {k: self._read_cdispatch_field(win32_element, k) for k in element_fields_list}

    def _read_cdispatch_field(self, win32_element: Any, key: str) -> Any:
        """Reads a specific field on a cdispatch object."""
        try:
            return getattr(win32_element, key)
        except AttributeError as e:
            logger.debug(f"Failed getting an attribute value: {e}")
            return UNKNOWN_VALUE

    def map_element(self, win32_element: Any, to_cls: type) -> ComponentDescriptor:
        """Maps a win32 element into an element of the class `to_cls`."""
        instance = to_cls()
        instance.set_data_values(self._read_cdispatch_fields(win32_element, instance.field_names))
        return cast(ComponentDescriptor, instance)

    def _get_list_iterator(self, win32_class_name: str, list_filter: Optional[str]) -> Generator[Any, None, None]:
        if list_filter:
            query = f"Select * from {win32_class_name} where {list_filter}"
            return self.wmi.ExecQuery(query)  # type: ignore
        return self.wmi.InstancesOf(win32_class_name)  # type: ignore

    def element_generator(
        self, to_cls: type, win32_class_name: str, list_filter: Optional[str]
    ) -> Generator["ComponentDescriptor", None, None]:
        """Gets a generator over all elements currently registered in the system."""
        for element in self._get_list_iterator(win32_class_name, list_filter):
            yield self.map_element(win32_element=element, to_cls=to_cls)


class ComponentDescriptorWrapper:
    """Wraps a component descriptor."""

    def __init__(self, cls: type):
        """initialiser."""
        self._cls = cls
        self._win32_wrapper = Win32Wrapper()

    def element_generator(self) -> Generator["ComponentDescriptor", None, None]:
        """Gets a generator over all elements currently registered in the system."""
        instance = self._cls()
        return self._win32_wrapper.element_generator(self._cls, instance.win32_class_name, instance.win32_filter)
