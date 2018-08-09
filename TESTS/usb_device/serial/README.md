# `udev` rules for Mbed USB CDC device

Before running `tests-usb_device-serial` test suite on Debian-based Linux
distros, make sure to [update the `udev` rules][1] as follows:

```bash
sudo tee /etc/udev/rules.d/99-ttyacms.rules >/dev/null <<EOF
ATTRS{idVendor}=="1f00" ATTRS{idProduct}=="2013", ENV{ID_MM_DEVICE_IGNORE}="1"
ATTRS{idVendor}=="1f00" ATTRS{idProduct}=="2012", ENV{ID_MM_DEVICE_IGNORE}="1"
EOF
sudo udevadm control --reload-rules
```

This will prevent the `ModemManager` daemon from automatically opening the port
and sending the `AT commands`, which it does for every new `/dev/ttyACM` device
registered in system.

[1]: https://linux-tips.com/t/prevent-modem-manager-to-capture-usb-serial-devices/284
