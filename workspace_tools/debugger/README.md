pyOCD
=====
pyOCD is an Open Source python library in order to program and
debug ARM Cortex-M microcontrollers using CMSIS-DAP. You can either
use a python interpreter to control your target or start a GDB server.


What allows this library?
-------------------------
1. From a python interpretor:
    * halt, step, resume execution
    * read/write memory
    * read/write block memory
    * read-write core register
    * set/remove hardware breakpoints
    * flash new binary
    * reset

2. From a GDB client, you have all the features provided by gdb:
    * load a .elf file
    * read/write memory
    * read/write core register
    * set/remove hardware breakpoints
    * high level stepping
    * ...
    
    
DEPENDANCIES:
-------------
pyOCD relies on external libraries:
* pyOCD has been tested with python 2.7
* distutils 
* Windows: [pyWinUSB](https://github.com/rene-aguirre/pywinusb):
    ```
    $ cd /path-to-pywinusb/
    $ python setup.py install
    ```
* Linux: [pyUSB](https://github.com/walac/pyusb):
    ```
    $ sudo apt-get install python libusb-1.0-0-dev
    $ cd /path-to-pyusb/
    $ sudo python setup.py install
    ```
* Mac:

    So far Mac OS X is not supported
    
    
Installation:
-------------

    $ cd /path-to-pyOCD/
    $ python setup.py install

    
Examples:
---------
## Tests
A series of tests are on the test directory:
* basic_test.py: simple test that checks:
    1. read/write core registers
    2. read/write memory
    3. stop/resume/step the execution
    4. reset the target
    5. flash a binary
* gdb_test.py: launch a gdbserver

## Hello World example:

    from pyOCD.board import MbedBoard

    import logging
    logging.basicConfig(level=logging.INFO)

    board = MbedBoard.chooseBoard()

    target = board.target
    flash = board.flash
    target.resume()
    target.halt()
    print "pc: 0x%X" % target.readCoreRegister("pc")
        pc: 0xA64
    target.step()
    print "pc: 0x%X" % target.readCoreRegister("pc")
        pc: 0xA30
    target.step()
    print "pc: 0x%X" % target.readCoreRegister("pc")
        pc: 0xA32
    flash.flashBinary("binaries/l1_lpc1768.bin")
    print "pc: 0x%X" % target.readCoreRegister("pc")
        pc: 0x10000000
    target.reset()
    target.halt()
    print "pc: 0x%X" % target.readCoreRegister("pc")
        pc: 0xAAC
    board.uninit()
 
##GDB server example:
Python:

    from pyOCD.gdbserver import GDBServer
    from pyOCD.board import MbedBoard
    
    import logging
    logging.basicConfig(level=logging.INFO)

    board = MbedBoard.chooseBoard()

    # start gdbserver
    gdb = GDBServer(board, 3333)

gdb server:

    arm-none-eabi-gdb basic.elf

    <gdb> target remote localhost:3333
    <gdb> load
    <gdb> continue


Architecture:
-------------

# interface:
An interface does the link between the target and the computer.
This package contains basic functions to write and read data to and from
an interface. You can inherit from Interface and overwrite read(), write(),...
Then declare your interface in INTERFACE (in pyOCD.interface.__init__.py)

# target:
A target defines basic functionalities such as step, resume, halt, readMemory,...
You can inherit from Target to implement your own functions.
Then declare your target in TARGET (in pyOCD.target.__init__.py)

# transport:
Defines the transport used to communicate. In particular, you can find CMSIS-DAP. 
Defines functions such as memWriteAP, memReadAP, writeDP, readDP,...
You can inherit from Transport and implement your own functions.
Then declare your transport in TRANSPORT (in pyOCD.transport.__init__.py)

# flash:
Contains flash algorithm in order to flash a new binary into the target.

# gdbserver:
Start a GDB server. The server listens on a specific port. You can then
connect a GDB client to it and debug/program the target

Then you can debug a board which is composed by an interface, a target, a transport and a flash