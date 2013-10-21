"""
This module defines the attributes of the
PyPI package for the Mbed SDK
"""

from distutils.core import setup

LICENSE = open('LICENSE').read()
DESCRIPTION = open('description.rst')

setup(name='Mbed',
      version='0.0.1',
      description='Build and test system for mbed',
      long_description=DESCRIPTION,
      author='Author name',
      author_email='Author email',
      maintainer='Maintainer name',
      maintainer_email='Maintainer email',
      url='https://github.com/mbedmicro/mbed',
      license=LICENSE)
