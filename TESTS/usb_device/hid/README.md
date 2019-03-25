# Testing the USB HID device with a Linux host

Before running `tests-usb_device-hid` test suite on a Linux machine, please
make sure to install the `hidapi` Python module first, otherwise some test
cases will be skipped. Due to external dependencies for Linux, this module
is not installed during the initial setup, to keep the process as simple
as possible.

For Debian-based Linux distros, the dependencies can be installed as follows
(based on module's [README][1]):

```bash
apt-get install python-dev libusb-1.0-0-dev libudev-dev
pip install --upgrade setuptools
```
To install the `hidapi` module itself, please use the attached
`TESTS/usb_device/hid/requirements.txt` file:
```bash
pip install -r requirements.txt
```

[1]: https://github.com/trezor/cython-hidapi/blob/master/README.rst#install

