# Toolchain Profiles User Perspective

A Toolchain or build system Profile is a set of flags that is garenteed to be passed to the underlieing compiler suite.
These flags are stored in a JSON file that may be merged with other JSON files of the same structure.

## JSON Toolchain Profile Format

The JSON object that represents a Toolchain Profile is a dict mapping from Toolchains, like `GCC_ARM`, to their flags, like `-O3`.
The structure is as follows: Each toolchain supported by a Toolchain Profile has an dict in the root dict. 
This dict contains a mapping from a flag type to a list of flags that should be passed the corresponding part of the compiler suite.
The required flag types are:

| Key      | Description                           |
|:---------|:--------------------------------------|
| `c`      | Flags for the C Compiler              |
| `cxx`    | Flags for the C++ Compiler            |
| `common` | Flags for both the C and C++ Compilers|
| `asm`    | Flags for the Assembler               |

## Example

An example of a Toolchain Profile is given below:
```json
{
    "GCC_ARM": {
        "common": ["-c", "-Wall", "-Wextra",
                   "-Wno-unused-parameter", "-Wno-missing-field-initializers",
                   "-fmessage-length=0", "-fno-exceptions", "-fno-builtin",
                   "-ffunction-sections", "-fdata-sections", "-funsigned-char",
                   "-MMD", "-fno-delete-null-pointer-checks",
                   "-fomit-frame-pointer", "-Os"],
        "asm": ["-x", "assembler-with-cpp"],
        "c": ["-std=gnu99"],
        "cxx": ["-std=gnu++98", "-fno-rtti", "-Wvla"],
        "ld": ["-Wl,--gc-sections", "-Wl,--wrap,main", "-Wl,--wrap,_malloc_r",
               "-Wl,--wrap,_free_r", "-Wl,--wrap,_realloc_r",
               "-Wl,--wrap,_calloc_r", "-Wl,--wrap,exit", "-Wl,--wrap,atexit"]
    },
    "ARM": {
        "common": ["-c", "--gnu", "-Otime", "--split_sections",
                   "--apcs=interwork", "--brief_diagnostics", "--restrict",
                   "--multibyte_chars", "-O3"],
        "asm": [],
        "c": ["--md", "--no_depend_system_headers", "--c99", "-D__ASSERT_MSG"],
        "cxx": ["--cpp", "--no_rtti", "--no_vla"],
        "ld": []
    },
    "IAR": {
        "common": [
            "--no_wrap_diagnostics", "non-native end of line sequence", "-e",
            "--diag_suppress=Pa050,Pa084,Pa093,Pa082", "-Oh"],
        "asm": [],
        "c": ["--vla"],
        "cxx": ["--guard_calls", "--no_static_destruction"],
        "ld": ["--skip_dynamic_initialization", "--threaded_lib"]
    }
}
```

From this Toolchain profile, we can tell that:
 - `GCC_ARM`, `ARM`, and `IAR` compiler suites are supported.
 - The `ARM` C and C++ Compilers will be using optimization level `-O3`
 - The `IAR` linker will skip dynamic initialization
 - etc.

# Toolchain Profile API Perspective

The Toolchains no longer take in an optional argument, `build_profile`, that will contain a map from flag types to lists of flags.
This looks exactly the same in python as it does in the JSON format above.
The meaning of the flags, and which ones are required is the same as the User Perspective
A developer using the API must parse the User provided files themselves and extract the appropriate sub-dict from the file afterwards.
A convienence function that does this for a developer is `tools.options.extract_profile` and will call args_error when a Toolchain Profile JSON file does not provide flags for the selected Toolchain.
