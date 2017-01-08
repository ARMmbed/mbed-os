# VSCode exporter
Exporting for Microsoft Visual Studio code works on Windows, Mac and Linux but will expect a few system dependnecies to be installed. The exporter creates task files and launch files that make it easy to compile and debug within the IDE. Building is done using the created makefile. Debugging with GDB and pyOCD

## Setup
- compiler
    - GNU ARM Embedded
    - IAR
    - ARMC5
- mbed CLI
    - mbed tools requirement (pip install -U requirements.txt)
- make

```
mbed config [--global] target <board_name>
mbed export -i vscode_armc5
mbed export -i vscode_iar
mbed export -i vscode_gcc_arm
```

## Quickstart
- Windows
    - compile: ctrl + shift + B
- Mac
    - compile: cmd + shift + B
- Linux
    - compile: ctrl + shift + B

# ToDo
- launch GDB for Windows
- verify Linux installation location for pyOCD
- verify Mac installation location for pyOCD
- verify Windows installation location for pyOCD
- document udev rule for pyOCD on Linux
- .vscode added to .gitignore for all applications etc.
- filtered code completion in settings.json for the specific target that is exported for
- keybindings.json for common mbed actions (possibly to match the online IDE key bindings)
- add pictures!