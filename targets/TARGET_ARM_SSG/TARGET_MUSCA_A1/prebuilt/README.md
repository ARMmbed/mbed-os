# MUSCA_A1_NS prebuilt secure binaries

This directory tree contains Secure images released under Permissive Binary License.

Built by mbed-cli using GNU Arm Embedded - version 6.3.1

These images were compiled by the following command:

## mcuboot.bin
### Repository
https://git.trustedfirmware.org/trusted-firmware-m.git
### Commit SHA
6c5be4a98e4d7055ee49076ca4e515fb4b172e66
```sh
cmake ../ -G"Unix Makefiles" -DTARGET_PLATFORM=MUSCA_A -DCOMPILER=ARMCLANG -DCMAKE_BUILD_TYPE=Debug
make
```

## tfm.bin

```sh
mbed compile -t GCC_ARM -m ARM_MUSCA_A1_S --app-config tools/psa/tfm/mbed_app.json --profile release
```

## spm_smoke.bin

```sh
mbed test --compile -t GCC_ARM -m ARM_MUSCA_A1_S --app-config tools/psa/tfm/mbed_app.json -n tests-psa-spm_smoke -DUSE_PSA_TEST_PARTITIONS -DUSE_SMOKE_TESTS_PART1 --profile release
```

## spm_client.bin

```sh
mbed test --compile -t GCC_ARM -m ARM_MUSCA_A1_S --app-config tools/psa/tfm/mbed_app.json -n tests-psa-spm_client -DUSE_PSA_TEST_PARTITIONS -DUSE_CLIENT_TESTS_PART1 --profile release
```

## spm_server.bin

```sh
mbed test --compile -t GCC_ARM -m ARM_MUSCA_A1_S --app-config tools/psa/tfm/mbed_app.json -n tests-psa-spm_server -DUSE_PSA_TEST_PARTITIONS -DUSE_SERVER_TESTS_PART1 -DUSE_SERVER_TESTS_PART2 --profile release
```

To update the prebuilt binaries run the previous commands.
