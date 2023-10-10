#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Internal helper to retrieve target information from the online database."""

import pathlib
from http import HTTPStatus
import json
from json.decoder import JSONDecodeError
import logging
from typing import List, Optional, Dict, Any

import requests

from mbed_tools.targets._internal.exceptions import ResponseJSONError, BoardAPIError

from mbed_tools.targets.env import env


INTERNAL_PACKAGE_DIR = pathlib.Path(__file__).parent
SNAPSHOT_FILENAME = "board_database_snapshot.json"

logger = logging.getLogger(__name__)


def get_board_database_path() -> pathlib.Path:
    """Return the path to the offline board database."""
    return pathlib.Path(INTERNAL_PACKAGE_DIR, "data", SNAPSHOT_FILENAME)


_BOARD_API = "https://os.mbed.com/api/v4/targets"


def get_offline_board_data() -> Any:
    """Loads board data from JSON stored in offline snapshot.

    Returns:
        The board database as retrieved from the local database snapshot.

    Raises:
        ResponseJSONError: error decoding the local database JSON.
    """
    boards_snapshot_path = get_board_database_path()
    try:
        return json.loads(boards_snapshot_path.read_text())
    except JSONDecodeError as json_err:
        raise ResponseJSONError(f"Invalid JSON received from '{boards_snapshot_path}'.") from json_err


def get_online_board_data() -> List[dict]:
    """Retrieves board data from the online API.

    Returns:
        The board database as retrieved from the boards API

    Raises:
        ResponseJSONError: error decoding the response JSON.
        BoardAPIError: error retrieving data from the board API.
    """
    board_data: List[dict] = [{}]
    response = _get_request()
    if response.status_code != HTTPStatus.OK:
        warning_msg = _response_error_code_to_str(response)
        logger.warning(warning_msg)
        logger.debug(f"Response received from API:\n{response.text}")
        raise BoardAPIError(warning_msg)

    try:
        json_data = response.json()
    except JSONDecodeError as json_err:
        warning_msg = f"Invalid JSON received from '{_BOARD_API}'."
        logger.warning(warning_msg)
        logger.debug(f"Response received from API:\n{response.text}")
        raise ResponseJSONError(warning_msg) from json_err

    try:
        board_data = json_data["data"]
    except KeyError as key_err:
        warning_msg = f"JSON received from '{_BOARD_API}' is missing the 'data' field."
        logger.warning(warning_msg)
        keys_found = ", ".join(json_data.keys())
        logger.debug(f"Fields found in JSON Response: {keys_found}")
        raise ResponseJSONError(warning_msg) from key_err

    return board_data


def _response_error_code_to_str(response: requests.Response) -> str:
    if response.status_code == HTTPStatus.UNAUTHORIZED:
        return (
            f"Authentication failed for '{_BOARD_API}'. Please check that the environment variable "
            f"'MBED_API_AUTH_TOKEN' is correctly configured with a private access token."
        )
    else:
        return f"An HTTP {response.status_code} was received from '{_BOARD_API}'."


def _get_request() -> requests.Response:
    """Make a GET request to the API, ensuring the correct headers are set."""
    header: Optional[Dict[str, str]] = None
    mbed_api_auth_token = env.MBED_API_AUTH_TOKEN
    if mbed_api_auth_token:
        header = {"Authorization": f"Bearer {mbed_api_auth_token}"}

    try:
        return requests.get(_BOARD_API, headers=header)
    except requests.exceptions.ConnectionError as connection_error:
        if isinstance(connection_error, requests.exceptions.SSLError):
            logger.warning("Unable to verify an SSL certificate with requests.")
        elif isinstance(connection_error, requests.exceptions.ProxyError):
            logger.warning("Failed to connect to proxy. Please check your proxy configuration.")

        logger.warning("Unable to connect to the online database. Please check your internet connection.")
        raise BoardAPIError("Failed to connect to the online database.") from connection_error
