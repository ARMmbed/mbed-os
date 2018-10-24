README for Mbed TLS
===================

Mbed TLS for Mbed OS
--------------------

This edition of Mbed TLS has been adapted for Mbed OS and imported from its standalone release, which you can find on [github here](https://github.com/ARMmbed/mbedtls). This edition of Mbed TLS does not include the test code or the scripts used in the development of the library. These can be found in the standalone release.


Getting Started
---------------

Several example programs are available that demonstrate the use of Mbed TLS with Mbed OS. These are a great way of getting to know the library.

1. [**TLS Client:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/tls-client) TLS Client demonstrates the use of Mbed TLS to establish a TLS connection to a remote server.

2. [**Benchmark:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/benchmark) Benchmark measures the time taken to perform basic cryptographic functions used in the library.

3. [**Hashing:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/hashing) Hashing demonstrates the various APIs for computing hashes of data (also known as message digests) with SHA-256.

4. [**Authenticated encryption:**](https://github.com/ARMmbed/mbed-os-example-tls/tree/master/authcrypt) Authcrypt demonstrates usage of the cipher API for encrypting and authenticating data with AES-CCM.


These examples are fully integrated into Mbed OS. Each of them comes with complete usage instructions as a `README.md` file.


Configuring Mbed TLS features
-----------------------------

Mbed TLS makes it easy to disable any feature during compilation, if that feature isn't required for a particular project. The default configuration enables all modern and widely-used features of the TLS protocol, which should meet the needs of most projects. It also disables all older and less common features, to minimize the code footprint.

The list of available compilation flags is available in the fully documented [`config.h` file](https://github.com/ARMmbed/mbedtls/blob/development/include/mbedtls/config.h).

If you need to adjust those flags, you can provide your own supplementary configuration adjustment file with suitable `#define` and `#undef` statements. These will be included between the default definitions and the sanity checks. Your configuration file should be in your application's include directory, and can be named freely; you just need to let Mbed TLS know the file's name. To do that, you can use the [Mbed OS Configuration system](https://docs.mbed.com/docs/mbed-os-api/en/latest/api/md_docs_config_system.html)

For example, if you wanted to enable the options, `MBEDTLS_PEM_WRITE_C` and `MBEDTLS_CMAC_C`, and provide your own additional configuration file for Mbed TLS named `my_config.h`, you could define these in a top level `mbed_app.json` configuration file in the root directory of your project.

The Mbed TLS configuration file would be specified in the `.json` file as:

```
{
    "macros" : [

        "MBEDTLS_USER_CONFIG_FILE" : "my_config.h",

        "MBEDTLS_PEM_WRITE_C",
        "MBEDTLS_CMAC_C"
    ]
    ....remainder of file...
}
```

The additional configuration file, `my_config.h`, can then be used as a normal configuration header file to include or exclude configurations. For example, it could include the following lines to include ECJPAKE, and to disable the CBC block mode:

```
    #define MBEDTLS_ECJPAKE_C
    #define MBEDTLS_KEY_EXCHANGE_ECJPAKE_ENABLED

    #undef MBEDTLS_CIPHER_MODE_CBC
```

This can be used to change any configuration normally configured in the `config.h` file.


## Getting Mbed TLS from GitHub

Mbed TLS is maintained and developed in the open, independently of Mbed OS, and its source can be found on GitHub here: [ARMmbed/mbedtls](https://github.com/ARMmbed/mbedtls). To import into an instance of Mbed OS a different version of Mbed TLS, a `Makefile` script is provided to update the local Git repository, extract a specific version, and to modify the configuration files to those used for the Mbed OS defaults.

To use the `Makefile`, you can either set `MBED_TLS_RELEASE` environment variable to the git tag or commit id of the Mbed TLS Release or version you want to use, or alternatively you can modify the `Makefile` itself. If `MBED_TLS_RELEASE` is unset, the HEAD of the main development branch will be extracted.

You should then run the following commands in the `importer` directory in the Mbed TLS directory:

```
    make update
    make
```

`make update` will pull the specified version of Mbed TLS into the local `importer/TARGET_IGNORE` directory and `make` will transform it into the `src` directory, modifying its configuration file as necessary.

Once these steps are complete, you can make your Mbed OS build normally with the new version of Mbed TLS.


## Differences between the standalone and Mbed OS editions

While the two editions share the same code base, there are still a number of differences, mainly in configuration and integration. You should keep those differences in mind if you consult our [knowledge base](https://tls.mbed.org/kb), as these refer to the standalone edition.

* The Mbed OS edition has a smaller set of features enabled by default in `config.h`, in order to reduce footprint. While the default configuration of the standalone edition puts more emphasis on maintaining interoperability with old peers, the Mbed OS edition only enables the most modern ciphers and the latest version of (D)TLS.

* The following components of Mbed TLS are disabled in the Mbed OS edition: `net_sockets.c` and `timing.c`. This is because Mbed OS includes its own equivalents.


Help and Support
----------------

For further documentation and help, you can visit the [Mbed TLS website](https://tls.mbed.org/) which contains full documentation of the library, including function-by-function descriptions, knowledge base articles and blogs. Additionally you can join our [support forum](https://forums.mbed.com/c/mbed-tls) for questions to the community or to help others.


Contributing to the Project
---------------------------

We gratefully accept bug reports and contributions from the community. There are some requirements we need to fulfill in order to be able to integrate contributions:

-   Simple bug fixes to existing code do not contain copyright themselves and we can integrate without issue. The same is true of trivial contributions.
-   For larger contributions, such as a new feature, the code can possibly fall under copyright law. We then need your consent to share in the ownership of the copyright. We have a form for this, which we will send to you in case you submit a contribution or pull request that we deem this necessary for.

Please submit contributions to the [standalone Mbed TLS project](https://github.com/ARMmbed/mbedtls), not to the version of Mbed TLS embedded within Mbed OS.
