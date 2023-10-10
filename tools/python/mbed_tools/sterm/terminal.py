#
# Copyright (c) 2020-2021 Arm Limited and Contributors. All rights reserved.
# SPDX-License-Identifier: Apache-2.0
#
"""Serial terminal implementation based on pyserial.tools.miniterm.

The Mbed serial terminal makes the following modifications to the default Miniterm.
* Custom help menu text
* Constrained set of menu keys
* CTRL-H to show help
* CTRL-B sends serial break to the target

To start the terminal clients should call the "run" function, this is the entry point to the module.
"""
from typing import Any

from serial import Serial
from serial.tools.miniterm import Miniterm


def run(port: str, baud: int, echo: bool = True) -> None:
    """Run the serial terminal.

    This function is blocking as it waits for the terminal thread to finish executing before returning.

    Args:
        port: The serial port to open a terminal on.
        baud: Serial baud rate.
        echo: Echo user input back to the console.
    """
    term = SerialTerminal(Serial(port=port, baudrate=str(baud)), echo=echo)
    term.start()

    try:
        term.join(True)
    except KeyboardInterrupt:
        pass
    finally:
        term.join()
        term.close()


class SerialTerminal(Miniterm):
    """An implementation of Miniterm that implements the additional Mbed terminal functionality.

    Overrides the `writer` method to implement modified menu key handling behaviour.
    Overrides the Miniterm::get_help_text method to return the Mbed custom help text.
    Adds a `reset` method so users can send a reset signal to the device.
    """

    def __init__(self, *args: Any, **kwargs: Any) -> None:
        """Set the rx/tx encoding and special characters."""
        super().__init__(*args, **kwargs)
        self.exit_character = CTRL_C
        self.menu_character = CTRL_T
        self.reset_character = CTRL_B
        self.help_character = CTRL_H
        self.set_rx_encoding("UTF-8")
        self.set_tx_encoding("UTF-8")

    def reset(self) -> None:
        """Send a reset signal."""
        self.serial.sendBreak()

    def get_help_text(self) -> str:
        """Return the text displayed when the user requests help."""
        return HELP_TEXT

    def writer(self) -> None:
        """Implements terminal behaviour."""
        menu_active = False
        while self.alive:
            try:
                input_key = self.console.getkey()
            except KeyboardInterrupt:
                input_key = self.exit_character

            if (menu_active and input_key in VALID_MENU_KEYS) or (input_key == self.help_character):
                self.handle_menu_key(input_key)
                menu_active = False

            elif input_key == self.menu_character:
                menu_active = True

            elif input_key == self.reset_character:
                self.reset()

            elif input_key == self.exit_character:
                self.stop()
                break

            else:
                self._write_transformed_char(input_key)

                if self.echo:
                    self._echo_transformed_char(input_key)

    def _write_transformed_char(self, text: str) -> None:
        for transformation in self.tx_transformations:
            text = transformation.tx(text)

        self.serial.write(self.tx_encoder.encode(text))

    def _echo_transformed_char(self, text: str) -> None:
        for transformation in self.tx_transformations:
            text = transformation.echo(text)

        self.console.write(text)


CTRL_B = "\x02"
CTRL_C = "\x03"
CTRL_H = "\x08"
CTRL_T = "\x14"
VALID_MENU_KEYS = ("p", "b", "\t", "\x01", "\x04", "\x05", "\x06", "\x0c", CTRL_C, CTRL_T)
HELP_TEXT = """--- Mbed Serial Terminal
--- Based on miniterm from pySerial
---
--- Ctrl+b     Send Break (reset target)
--- Ctrl+c     Exit terminal
--- Ctrl+h     Help
--- Ctrl+t     Menu escape key, followed by:
---    p       Change COM port
---    b       Change baudrate
---    Tab     Show detailed terminal info
---    Ctrl+a  Change encoding (default UTF-8)
---    Ctrl+f  Edit filters
---    Ctrl+e  Toggle local echo
---    Ctrl+l  Toggle EOL
---    Ctrl+r  Toggle RTS
---    Ctrl+d  Toggle DTR
---    Ctrl+c  Send control character to remote
---    Ctrl+t  Send control character to remote
"""
