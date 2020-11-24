README.md

Fork of mbed-os to add support for new target STM32 "NUCLEO-G431KB" Board.
Rebased to mbed-os-6.5.0

Howto (using mbed-cli):

Alt A:

    mbed import https://github.com/taunomagnusson/mbed-os-nucleo-g431kb.git
    cd mbed-os-nucleo-g431kb
    Add a "main.cpp" file, for example the main.cpp from here: https://github.com/ARMmbed/mbed-os-example-blinky/blob/master/main.cpp
    mbed compile

Alt B (2-level directory structure (works with VisualGDB)):

    mkdir g431kb
    cd g431kb
    git clone https://github.com/taunomagnusson/mbed-os-nucleo-g431kb.git
    mv mbed-os-nucleo-g431kb mbed-os
    mbed new .
    Add a "main.cpp" file, for example the main.cpp from here: https://github.com/ARMmbed/mbed-os-example-blinky/blob/master/main.cpp
    mbed compile

To run MBED tests:

    rm main.cpp
    mbed test -c

