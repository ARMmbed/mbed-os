
This directory tree contains Secure images released under Permissive Binary License.

Build by mbed-cli using GNU Arm Embedded - version 6.3.1

These images were compiled by the following command:

```
mbed compile -m FUTURE_SEQUANA_M0_PSA -t GCC_ARM --profile release -N psa_release_1.0
mbed test --compile -m FUTURE_SEQUANA_M0_PSA -t GCC_ARM --profile debug -n *psa-spm*,*psa-crypto_access_control
```

To update the prebuilt binaries run the previous commands.
