## README for Mbed TLS

### Mbed TLS for Mbed OS

This edition of Mbed TLS has been adapted for Mbed OS and imported from its standalone release, which you can find on [GitHub](https://github.com/ARMmbed/mbedtls). This edition of Mbed TLS does not include the test code or scripts used in the development of the library. You can find these in the standalone release.

### Getting started

Several example programs are available that demonstrate Mbed TLS with Mbed OS. These can help you become familiar with the library:

* [**TLS Client:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/tls-client) TLS Client demonstrates the use of Mbed TLS to establish a TLS connection to a remote server.

* [**Benchmark:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/benchmark) Benchmark measures the time taken to perform basic cryptographic functions used in the library.

* [**Hashing:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/hashing) Hashing demonstrates the various APIs for computing hashes of data (also known as message digests) with SHA-256.

* [**Authenticated encryption:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/authcrypt) Authcrypt demonstrates usage of the cipher API for encrypting and authenticating data with AES-CCM.

These examples are fully integrated into Mbed OS. Each of them comes with complete usage instructions as a `README.md` file.

### Configuring Mbed TLS features

With Mbed TLS, you can disable any unneeded features during compilation for a particular project. The default configuration enables widely used features of the TLS protocol, which meets the needs of most projects. It also disables all older and less common features to minimize the code footprint.

The list of available compilation flags is available in the fully documented [`config.h` file](https://github.com/ARMmbed/mbedtls/blob/development/include/mbedtls/config.h).

If you need to adjust these flags, you can provide your own supplementary configuration adjustment file with suitable `#define` and `#undef` statements. These are included between the default definitions and the sanity checks. Your configuration file should be in your application's include directory and can be named freely, but you then need to tell Mbed TLS the file's name. To do that, you can use the [Mbed OS configuration system](https://os.mbed.com/docs/latest/reference/configuration.html).

For example, if you wanted to enable the options `MBEDTLS_PEM_WRITE_C` and `MBEDTLS_CMAC_C` and provide your own additional configuration file for Mbed TLS named `my_config.h`, you could define these in a top level `mbed_app.json` configuration file in the root directory of your project.

The Mbed TLS configuration file would be specified in the `.json` file as:

```
{
    "macros" : [

        "MBEDTLS_USER_CONFIG_FILE" : "my_config.h",

        "MBEDTLS_PEM_WRITE_C",
        "MBEDTLS_CMAC_C"
    ]
    [remainder of file]
}
```

You can then use the additional configuration file `my_config.h` as a normal configuration header file to include or exclude configurations. For example, it could include the following lines to include ECJPAKE, and to disable the CBC block mode:

```
    #define MBEDTLS_ECJPAKE_C
    #define MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED

    #undef MBEDTLS_CIPHER_MODE_CBC
```

You can use this to change any configuration normally in the `config.h` file.

### Getting Mbed TLS from GitHub

We maintain and develop Mbed TLS in the open, independently of Mbed OS, and you can find its source on GitHub here: [ARMmbed/mbedtls](https://github.com/ARMmbed/mbedtls). To import a different version of Mbed TLS into an instance of Mbed OS, there is a `Makefile` script to update the local Git repository, extract a specific version and modify the configuration files to Mbed OS defaults.

To use the `Makefile`, you can either set `MBED_TLS_RELEASE` environment variable to the Git tag or commit ID of the Mbed TLS release or version you want to use, or modify the `Makefile` itself. If `MBED_TLS_RELEASE` is not set, the HEAD of the main development branch will be extracted.

Run the following commands in the `importer` directory in the Mbed TLS directory:

```
    make update
    make
```

The `make update` command pulls the specified version of Mbed TLS into the local `importer/TARGET_IGNORE` directory, and `make` transforms it into the `src` directory, modifying its configuration file as necessary.

Once these steps are complete, you can build Mbed OS normally with the new version of Mbed TLS.

### Differences between the standalone and Mbed OS editions

Although the two editions share the same code base, there are differences, mainly in configuration and integration. Remember these differences if you consult our [knowledge base](https://tls.mbed.org/kb), as the knowledge base articles refer to the standalone edition.

* The Mbed OS edition has a smaller set of features enabled by default in `config.h`, to reduce footprint. Although the default configuration of the standalone edition puts more emphasis on maintaining interoperability with old peers, the Mbed OS edition only enables the most modern ciphers and the latest version of (D)TLS.

* The following components of Mbed TLS are disabled in the Mbed OS edition: `net_sockets.c` and `timing.c`. This is because Mbed OS includes its own equivalents.

### Help and support

For further documentation and help, you can visit the [Mbed TLS website](https://tls.mbed.org/), which contains full documentation of the library, including function-by-function descriptions, knowledge base articles and blogs. Additionally, you can join our [support forum](https://forums.mbed.com/c/mbed-tls) for questions to the community or to help others.

### Contributing to the project

We gratefully accept bug reports and contributions from the community. Contributions should be submitted to the [standalone Mbed TLS project](https://github.com/ARMmbed/mbedtls), not to the version of Mbed TLS embedded within Mbed OS.

Please see the [contributing guidelines](https://github.com/ARMmbed/mbedtls/blob/development/CONTRIBUTING.md) of the project for details on how to do this.
