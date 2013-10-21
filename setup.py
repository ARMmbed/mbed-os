"""
This module defines the attributes of the
PyPI package for the Mbed SDK
"""

from distutils.core import setup

LICENSE = open('LICENSE').read()
DESCRIPTION = open('description.rst')
OWNER_NAMES = 'emilmont, bogdanm'
OWNER_EMAILS = 'Emilio.Monti@arm.com, Bogdan.Marinescu@arm.com'

setup(name='mbed',
      version='0.0.1',
      description='Build and test system for mbed',
      long_description=DESCRIPTION,
      author=OWNER_NAMES,
      author_email=OWNER_EMAILS,
      maintainer=OWNER_NAMES,
      maintainer_email=OWNER_EMAILS,
      url='https://github.com/mbedmicro/mbed',
      license=LICENSE)
