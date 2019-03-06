# LPC55S69_NS prebuilt secure binaries

This directory tree contains Secure images released under Permissive Binary License.

Built by mbed-cli using ARM Compiler 6.10.1

These images were compiled by the following command:

## tfm.bin

```sh
mbed compile -t ARMC6 -m LPC55S69_S --app-config mbed-os/tools/psa/tfm/mbed_app.json --profile release
```

## spm_smoke.bin

```sh
mbed test --compile -t ARMC6 -m LPC55S69_S --app-config mbed-os/tools/psa/tfm/mbed_app.json -n mbed-os-tests-psa-spm_smoke -DUSE_PSA_TEST_PARTITIONS -DUSE_SMOKE_TESTS_PART1 --profile release
```

## spm_client.bin

```sh
mbed test --compile -t ARMC6 -m LPC55S69_S --app-config mbed-os/tools/psa/tfm/mbed_app.json -n mbed-os-tests-psa-spm_client -DUSE_PSA_TEST_PARTITIONS -DUSE_CLIENT_TESTS_PART1 --profile release
```

## spm_server.bin

```sh
mbed test --compile -t ARMC6 -m LPC55S69_S --app-config mbed-os/tools/psa/tfm/mbed_app.json -n mbed-os-tests-psa-spm_server -DUSE_PSA_TEST_PARTITIONS -DUSE_SERVER_TESTS_PART1 -DUSE_SERVER_TESTS_PART2 --profile release
```

## crypto_access_control.bin

```sh
mbed test --compile -t ARMC6 -m LPC55S69_S --app-config mbed-os/tools/psa/tfm/mbed_app.json -n mbed-os-tests-psa-crypto_access_control -DUSE_PSA_TEST_PARTITIONS -DUSE_CRYPTO_ACL_TEST --profile release
```

To update the prebuilt binaries run the previous commands.
