Silicon Labs J-Link MSD volume.

Copying a binary file here will flash it to the MCU.

Supported binary file formats:
 - Intel Hex (.hex)
 - Motorola s-records (.mot)
 - Binary (.bin/.par/.crd)

Known limitations:
 - Virtual COM port speed is currently fixed at 115200 bps
 - Using other kit functionality such as energy profiling or debugger while flashing is
   not supported.
   Workaround: Pause any energy profiling and disconnect any connected debug sessions
               before flashing.
