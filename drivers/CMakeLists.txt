# SPDX-License-Identifier: Apache-2.0

target_sources(mbed-os PRIVATE
    internal/AsyncOp.h
    internal/ByteBuffer.h
    internal/EndpointResolver.h
    internal/LinkedList.h
    internal/LinkedListBase.h
    internal/LinkEntry.h
    internal/MIDIMessage.h
    internal/OperationList.h
    internal/OperationListBase.h
    internal/PolledQueue.h
    internal/SFDP.h
    internal/Task.h
    internal/TaskBase.h
    internal/TaskQueue.h
    internal/USBAudio_Types.h
    internal/USBDescriptor.h
    internal/USBDevice.h
    internal/USBDevice_Types.h
    internal/USBHID_Types.h

    source/AnalogIn.cpp
    source/AnalogOut.cpp
    source/BufferedSerial.cpp
    source/BusIn.cpp
    source/BusInOut.cpp
    source/BusOut.cpp
    source/CAN.cpp
    source/DigitalIn.cpp
    source/DigitalInOut.cpp
    source/DigitalOut.cpp
    source/FlashIAP.cpp
    source/I2C.cpp
    source/I2CSlave.cpp
    source/InterruptIn.cpp
    source/MbedCRC.cpp
    source/PortIn.cpp
    source/PortInOut.cpp
    source/PortOut.cpp
    source/PwmOut.cpp
    source/QSPI.cpp
    source/RawSerial.cpp
    source/ResetReason.cpp
    source/Serial.cpp
    source/Serial.cpp
    source/SerialWireOutput.cpp
    source/SFDP.cpp
    source/SPI.cpp
    source/SPISlave.cpp
    source/Timeout.cpp
    source/Timer.cpp
    source/TimerEvent.cpp
    source/UARTSerial.cpp
    source/UnbufferedSerial.cpp
    source/Watchdog.cpp
    source/Watchdog.cpp

    #source/usb/AsyncOp.cpp
    #source/usb/ByteBuffer.cpp
    #source/usb/EndpointResolver.cpp
    #source/usb/LinkedListBase.cpp
    #source/usb/OperationListBase.cpp
    #source/usb/PolledQueue.cpp
    #source/usb/TaskBase.cpp
    #source/usb/USBAudio.cpp
    #source/usb/USBCDC.cpp
    #source/usb/USBCDC_ECM.cpp
    #source/usb/USBDevice.cpp
    #source/usb/USBHID.cpp
    #source/usb/USBKeyboard.cpp
    #source/usb/USBMIDI.cpp
    #source/usb/USBMouse.cpp
    #source/usb/USBMouseKeyboard.cpp
    #source/usb/USBMSD.cpp
    #source/usb/USBSerial.cpp
)

target_include_directories(mbed-os PUBLIC .)
target_include_directories(mbed-os PUBLIC internal)
