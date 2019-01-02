
This directory tree contains Secure images released under Permissive Binary License.

Build by mbed-cli using GNU Arm Embedded - version 6.3.1

These images were compiled by the following command:

```
mbed compile -m FUTURE_SEQUANA_M0_PSA -t GCC_ARM --profile release -N psa_release_1.0
mbed compile -m FUTURE_SEQUANA_M0_PSA -t GCC_ARM --profile debug -N psa_debug_1.0
```

There are also prebuilt images for PSA tests.

Those images can be found in the test folder under a `TARGET_FUTURE_SEQUANA_PSA` directory

These images were compiled by the following command:

```
mbed test --compile -m FUTURE_SEQUANA_M0_PSA -t GCC_ARM --profile debug -n *psa-*
```

To update the prebuilt binnaries run the previous commands and then run ```python export_binnaries.py```
