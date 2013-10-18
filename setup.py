"""
This module defines the attributes of the
PyPI package for the Mbed SDK
"""

from distutils.core import setup

LICENSE = open('LICENSE').read()

setup(name='Mbed',
      version='0.0.1',
      description='Package description',
      long_description='Package long description',
      author='Author name',
      author_email='Author email',
      maintainer='Maintainer name',
      maintainer_email='Maintainer email',
      url='https://github.com/mbedmicro/mbed',
      download_url='Package download url',
      license=LICENSE)
