#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Environment options for `mbed-targets`.

All the env configuration options can be set either via environment variables or using a `.env` file
containing the variable definitions as follows:

```
VARIABLE=value
```

Environment variables take precendence, meaning the values set in the file will be overriden
by any values previously set in your environment.

.. WARNING::
   Do not upload `.env` files containing private tokens to version control! If you use this package
   as a dependency of your project, please ensure to include the `.env` in your `.gitignore`.
"""
import os

import dotenv

dotenv.load_dotenv(dotenv.find_dotenv(usecwd=True))


class Env:
    """Provides access to environment variables.

    Ensures variables are reloaded when environment changes during runtime.
    """

    @property
    def MBED_API_AUTH_TOKEN(self) -> str:
        """Token to use when accessing online API.

        Mbed Targets uses the online mbed board database at os.mbed.com as its data source.
        A snapshot of the board database is shipped with the package, for faster lookup of known
        boards. Only public boards are stored in the database snapshot. If you are fetching data
        for a private board, mbed-targets will need to contact the online database.

        To fetch data about private boards from the online database, the user must have an account
        on os.mbed.com and be member of a vendor team that has permissions to see the private board.
        An authentication token for the team member must be provided in an environment variable named
        `MBED_API_AUTH_TOKEN`.
        """
        return os.getenv("MBED_API_AUTH_TOKEN", "")

    @property
    def MBED_DATABASE_MODE(self) -> str:
        """Database mode to use when retrieving board data.

        Mbed Targets supports an online and offline mode, which controls where to look up the board database.

        The board lookup can be from either the online or offline database, depending
        on the value of an environment variable called `MBED_DATABASE_MODE`.

        The mode can be set to one of the following:

        - `AUTO`: the offline database is searched first, if the board isn't found the online database is searched.
        - `ONLINE`: the online database is always used.
        - `OFFLINE`: the offline database is always used.

        If `MBED_DATABASE_MODE` is not set, it defaults to `AUTO`.
        """
        return os.getenv("MBED_DATABASE_MODE", "AUTO")


env = Env()
"""Instance of `Env` class."""
