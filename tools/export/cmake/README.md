# CLion CMakeLists.txt Generator

This exporter generates a CMakeLists.txt file that can be used to
develop mbed using [IntelliJ CLion](https://www.jetbrains.com/clion/).

It will not create a functional CMake build system that mimics the
mbed build system, but rather uses the mbed-cli itself to compile
the targets. The generated files help CLion to understand the
includes and dependencies of your code.

Run the following command to create/overwrite your CMakeLists.txt.
```
mbed export -i cmake_gcc_arm
```
> Run the command again if files or libraries have been added or removed.