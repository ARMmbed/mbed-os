"""
This module defines the attributes of the
PyPI package for the Mbed SDK
"""

from distutils.core import setup

LICENSE = open('LICENSE').read()
DESCRIPTION = """A set of Python scripts that can be used to compile programs written on top of the `mbed framework`_. It can also be used to export mbed projects to other build systems and IDEs (uVision, IAR, makefiles).

.. _mbed framework: http://mbed.org"""
OWNER_NAMES = 'emilmont, bogdanm'
OWNER_EMAILS = 'Emilio.Monti@arm.com, Bogdan.Marinescu@arm.com'

setup(name='mbed',
      version='0.1.6',
      description='Build and test system for mbed',
      long_description=DESCRIPTION,
      author=OWNER_NAMES,
      author_email=OWNER_EMAILS,
      maintainer=OWNER_NAMES,
      maintainer_email=OWNER_EMAILS,
      url='https://github.com/mbedmicro/mbed',
      license=LICENSE)
