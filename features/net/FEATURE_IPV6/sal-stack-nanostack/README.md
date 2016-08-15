ARM IPV6/6LoWPAN stack.
=======================

This repository contains the ARM IPv6/6LoWPAN Stack for mbed OS.

The documentation is hosted in http://docs.mbed.com/projects/arm-ipv66lowpan-stack/

On mbed OS, the preferred usage is through [mbed Mesh API](https://github.com/ARMmbed/mbed-mesh-api) that provides the required wrappers between the socket abstraction layer and this stack.

To see, how the 6LoWPAN Stack works, check the example application [mbed-client-example-6lowpan](https://github.com/ARMmbed/mbed-client-example-6lowpan).

You can find he known limitations here: [sal-iface-6lowpan](https://github.com/ARMmbed/sal-iface-6lowpan).

## Changes in Nanostack 4.0:

### New APIs:

- MLE router and host lifetime configuration API
- MLE neighbor limits configuration API
- MLE token bucket configuration API
- API for adding/deleting routes
- FHSS API

### Other changes:

- Function `arm_nwk_6lowpan_link_scan_paramameter_set()` renamed to `arm_nwk_6lowpan_link_scan_parameter_set()` (due to misspelling)
- Channel mask settings API changed
- The parameters of function `cca_start()` changed

For instructions on updating your modules, see https://docs.mbed.com/docs/arm-ipv66lowpan-stack/en/latest/api_changes_to_v4_0_0/index.html.

