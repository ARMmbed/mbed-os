#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
import pathlib

from unittest import mock

import pytest

from mbed_tools.devices._internal.file_parser import OnlineId, read_device_files


class TestReadDeviceFiles:
    def test_finds_daplink_compatible_device_files(self, tmp_path):
        details = pathlib.Path(tmp_path, "details.txt")
        htm = pathlib.Path(tmp_path, "mbed.htm")
        htm.write_text("code=2222")
        details.write_text("Version: 2")

        info = read_device_files([tmp_path])

        assert info.product_code is not None
        assert info.interface_details is not None

    def test_finds_jlink_device_files(self, tmp_path):
        segger_html = pathlib.Path(tmp_path, "Segger.html")
        board_html = pathlib.Path(tmp_path, "Board.html")
        segger_html.write_text(build_segger_html())
        board_html.write_text(build_board_html())

        info = read_device_files([tmp_path])

        assert info.online_id is not None
        assert info.interface_details is not None

    def test_warns_if_no_device_files_found(self, caplog, tmp_path):
        read_device_files([tmp_path])

        assert str(tmp_path) in caplog.text

    def test_skips_hidden_files(self, caplog, tmp_path):
        auth = "101000000000000000000002F7F35E602eeb0bb9b632205c51f6c357aeee7bc9"
        file_contents = (
            '<meta http-equiv="refresh" '
            f'content="0; url=http://mbed.org/start?auth={auth}&loader=11972&firmware=16457&configuration=4" />'
        )
        pathlib.Path(tmp_path, "._MBED.HTM").write_text(file_contents)
        pathlib.Path(tmp_path, "._DETAILS.TXT").write_text("Version: 2222")

        assert read_device_files([tmp_path]).product_code is None
        assert not read_device_files([tmp_path]).interface_details

    def test_handles_os_error_with_warning(self, tmp_path, caplog, monkeypatch):
        bad_htm = pathlib.Path(tmp_path, "MBED.HTM")
        bad_htm.touch()
        monkeypatch.setattr(pathlib.Path, "read_text", mock.Mock(side_effect=OSError))

        read_device_files([tmp_path])
        assert str(bad_htm) in caplog.text


class TestExtractProductCodeFromHtm:
    def test_reads_product_code_from_code_attribute(self, tmp_path):
        code = "02400201B80ECE4A45F033F2"
        file_contents = f'<meta http-equiv="refresh" content="0; url=http://mbed.org/device/?code={code}"/>'
        pathlib.Path(tmp_path, "MBED.HTM").write_text(file_contents)

        assert read_device_files([tmp_path]).product_code == code[:4]

    def test_reads_product_code_from_auth_attribute(self, tmp_path):
        auth = "101000000000000000000002F7F35E602eeb0bb9b632205c51f6c357aeee7bc9"
        file_contents = (
            '<meta http-equiv="refresh" '
            f'content="0; url=http://mbed.org/start?auth={auth}&loader=11972&firmware=16457&configuration=4" />'
        )
        pathlib.Path(tmp_path, "MBED.HTM").write_text(file_contents)

        assert read_device_files([tmp_path]).product_code == auth[:4]

    def test_none_if_no_product_code(self, tmp_path):
        file_contents = '<meta http-equiv="refresh" content="0; url=http://mbed.org/config" />'
        pathlib.Path(tmp_path, "MBED.HTM").write_text(file_contents)

        assert read_device_files([tmp_path]).product_code is None

    def test_extracts_first_product_code_found(self, tmp_path):
        auth = "101000000000000000000002F7F35E602eeb0bb9b632205c51f6c357aeee7bc9"
        file_contents_1 = (
            '<meta http-equiv="refresh" '
            f'content="0; url=http://mbed.org/start?auth={auth}&loader=11972&firmware=16457&configuration=4" />'
        )
        code = "02400201B80ECE4A45F033F2"
        file_contents_2 = f'<meta http-equiv="refresh" content="0; url=http://mbed.org/device/?code={code}"/>'
        directory_1 = pathlib.Path(tmp_path, "test-1")
        directory_1.mkdir()
        directory_2 = pathlib.Path(tmp_path, "test-2")
        directory_2.mkdir()
        pathlib.Path(directory_1, "mbed.htm").write_text(file_contents_1)
        pathlib.Path(directory_2, "mbed.htm").write_text(file_contents_2)

        result = read_device_files([directory_1, directory_2])

        assert result.product_code == auth[:4]


class TestExtractOnlineIDFromHTM:
    def test_reads_online_id_from_url(self, tmp_path):
        url = "https://os.mbed.com/platforms/THIS-IS_a_SLUG_123/"
        file_contents = f"window.location.replace({url});"
        pathlib.Path(tmp_path, "MBED.HTM").write_text(file_contents)

        assert read_device_files([tmp_path]).online_id == OnlineId(target_type="platform", slug="THIS-IS_a_SLUG_123")

    def test_none_if_not_found(self, tmp_path):
        file_contents = "window.location.replace(https://os.mbed.com/about);"
        pathlib.Path(tmp_path, "MBED.HTM").write_text(file_contents)

        assert read_device_files([tmp_path]).online_id is None


class TestExtractsJlinkData:
    def test_reads_board_slug(self, tmp_path):
        board_html = pathlib.Path(tmp_path, "Board.html")
        board_html.write_text(build_board_html("http://test.com/test/slug"))

        info = read_device_files([tmp_path])

        assert info.online_id == OnlineId(target_type="jlink", slug="slug")

    def test_reads_board_slug_ignore_extension(self, tmp_path):
        board_html = pathlib.Path(tmp_path, "Board.html")
        board_html.write_text(build_board_html("http://test.com/slug.html"))

        info = read_device_files([tmp_path])

        assert info.online_id == OnlineId(target_type="jlink", slug="slug")

    def test_id_none_if_no_board_slug(self, tmp_path):
        board_html = pathlib.Path(tmp_path, "Board.html")
        board_html.write_text(build_board_html("http://test.com"))

        info = read_device_files([tmp_path])

        assert info.online_id is None

    def test_reads_segger_slug(self, tmp_path):
        segger_html = pathlib.Path(tmp_path, "Segger.html")
        segger_html.write_text(build_segger_html("variant"))

        info = read_device_files([tmp_path])

        assert info.interface_details.get("Version") == "variant"

    def test_interface_empty_if_not_found(self, tmp_path):
        board_html = pathlib.Path(tmp_path, "Board.html")
        board_html.write_text("<html></html>")

        info = read_device_files([tmp_path])

        assert info.interface_details == {}


# Helpers to build test data
def build_short_details_txt(version="0226", build="Aug 24 2015 17:06:30", commit_sha="27a2367", local_mods="Yes"):
    return (
        f"""Version: {version}
Build:   {build}
Git Commit SHA: {commit_sha}
Git Local mods: {local_mods}
""",
        {"Version": version, "Build": build, "Git Commit SHA": commit_sha, "Git Local mods": local_mods},
    )


def build_long_details_txt(
    interface_version="0226",
    uid="0240000029164e45002f0012706e0006f301000097969900",
    hif_id="97969900",
    auto_reset="0",
    auto_allow="0",
    daplink_mode="Interface",
    commit_sha="c76899838",
    local_mods="0",
    usb_ifaces="MSD, CDC, HID",
    iface_crc="0x26764ebf",
):
    return (
        f"""# DAPLink Firmware - see https://mbed.com/daplink
Unique ID: {uid}
HIF ID: {hif_id}
Auto Reset: {auto_reset}
Automation allowed: {auto_allow}
Daplink Mode: {daplink_mode}
Interface Version: {interface_version}
Git SHA: {commit_sha}
Local Mods: {local_mods}
USB Interfaces: {usb_ifaces}
Interface CRC: {iface_crc}
""",
        {
            "Unique ID": uid,
            "HIF ID": hif_id,
            "Auto Reset": auto_reset,
            "Automation allowed": auto_allow,
            "Daplink Mode": daplink_mode,
            "Version": interface_version,
            "Git SHA": commit_sha,
            "Local Mods": local_mods,
            "USB Interfaces": usb_ifaces,
            "Interface CRC": iface_crc,
        },
    )


def build_segger_html(model="j-link-ob"):
    return (
        """<html><head><meta http-equiv="refresh" content="0;"""
        f"""url=https://www.segger.com/products/debug-probes/j-link/models/{model}/"/>"""
        """</head><body></body></html>"""
    )


def build_board_html(target_url="http://www.nxp.com/FRDM-K64F"):
    return f"""<html><head><meta http-equiv="refresh" content="0; url={target_url}"/></head><body></body></html>"""


class TestReadDetailsTxt:
    @pytest.mark.parametrize(
        "content, expected",
        (
            build_short_details_txt(),
            build_short_details_txt(version="0777", commit_sha="99789s", local_mods="No"),
            build_long_details_txt(),
            ("", {}),
            ("\n", {}),
            ("blablablablaandbla", {}),
            ("blablabla\nblaandbla\nversion : 2\n\n", {"version": "2"}),
        ),
        ids=("short", "short2", "long", "empty", "newline", "nosep", "multiline"),
    )
    def test_parses_details_txt(self, content, expected, tmp_path):
        details_file_path = pathlib.Path(tmp_path, "DETAILS.txt")
        details_file_path.write_text(content)

        result = read_device_files([tmp_path]).interface_details
        assert result == expected
