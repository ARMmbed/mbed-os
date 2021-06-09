# Mbed OS development environment Docker image

This Docker image is the official Mbed OS development environment.

* It is based on Ubuntu 20.04
* Python and CMake are installed
* Arm-none-eabi-gcc toolchain is installed
* Latest released version of mbed-cli and mbed-greentea are installed
* All other Mbed OS dependency tools are installed.

# How to use the Docker image:

## Pull the Docker image
```bash
docker pull ghcr.io/armmbed/mbed-os-env:<label>
```

## Run Mbed OS environment without HW support (build Mbed images only)
Launch the Docker image by
```bash
docker run -it ghcr.io/armmbed/mbed-os-env:<label>
```
Then you will have a container with an Mbed OS development environment.
You should be able to compile Mbed commands/examples as recommended in the documentation
e.g.
```bash
mbed-tools import mbed-os-example-blinky
cd mbed-os-example-blinky
mbed-tools compile -m <TARGET> -t GCC_ARM
```

## Run Mbed OS environment with HW support (USB pass-through)
:warning: This currently works only on Linux host machines with [Mbed CLI 1](https://os.mbed.com/docs/mbed-os/v6.13/build-tools/mbed-cli-1.html).

If you want to use this Docker image to connect and flash your targets, you will need some extra command line option to pass-through your USB devices.
```bash
sudo docker run -it --privileged -v /dev/disk/by-id:/dev/disk/by-id -v /dev/serial/by-id:/dev/serial/by-id ghcr.io/armmbed/mbed-os-env:<label>
```
Then you will have a container with an Mbed OS development environment.
To make sure your Mbed targets have been detected, you might want to manually run the mount command and `mbedls` to check
```bash
mount /dev/sdb /mnt
mbedls
```
If `mbedls` detected  your connected target, then you should be able to run Mbed tests/examples as recommended in the Mbed documentation.
``` bash
mbed import mbed-os
cd mbed-os
mbed test -t GCC_ARM -m <target> 
```
