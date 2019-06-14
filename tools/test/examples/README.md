# Examples testing script

Scripts:
* **examples.py** makes easier to test examples. Used actions are:
    * **import** - Imports each of the repos and its dependencies (.lib files) associated
        with the specific examples name from the json config file. Note if
        there is already a clone of the repo then it will first be removed to
        ensure a clean, up to date cloning.
    * **clone** - clones each of the repos associated with the specific examples name from the
        json config file. Note if there is already a clone of the repo then it will first
        be removed to ensure a clean, up to date cloning.
    * **deploy** - If the example directory exists as provided by the json config file,
        pull in the examples dependencies by using `mbed-cli deploy`.
    * **tag** - For each example repo identified in the config json object, update the version of
        mbed-os to that specified by the supplied GitHub tag. This function assumes that each
        example repo has already been cloned.
    * **compile** - Compiles combinations of example programs, targets and compile chains.
    * **export** - Exports and builds combinations of example programs, targets and IDEs.
* **analyse.py** helps configuration file analysis by printing test phase targets configuration as table

 Configuration example:
 ```
 {
  "examples": [
    {
      "name": "mbed-os-example-blinky",
      "github": "https://github.com/ARMmbed/mbed-os-example-blinky",
      "mbed": ["https://os.mbed.com/teams/mbed-os-examples/code/mbed-os-example-blinky"],
      "test-repo-source": "github",
      "features": [],
      "baud_rate": 9600,
      "compare_log": ["mbed-os-example-blinky/tests/blinky.log"],
      "auto-update": true,
      "pull_request_test": {
        "targets": ["K64F"],
        "toolchains": ["GCC_ARM"],
        "exporters": [],
        "compile": true,
        "export": true,
        "test": false
      },
      "nightly_test": {
        "targets": ["K64F", "K66F"],
        "toolchains": ["GCC_ARM", "ARM"],
        "exporters": [],
        "compile": true,
        "export": true,
        "test": true
      },
      "release_test": {
        "targets": ["K64F", "K66F", "NUCLEO_F429ZI"],
        "toolchains": [],
        "exporters": ["IAR"],
        "compile": true,
        "export": true,
        "test": true
      }
    }
  ]
}
 ```

Values:
* [] means all possible alternatives

Fields:
 * **name** is example name used mostly with prints
 * **github** is github repository address
 * **mbed** Mercurial repositories in os.mbed.com
 * **test-repo-source** specify which repo source to use for testing
 * **features** The features that must be in the features array of a target in targets.json
 * **baud_rate** used baud rate
 * **compare_log** Log which is compared to command line output when testing
 * Testing phases:
   * **pull_request_test** This configuration is used for every pull request testing
   * **nightly_test** This is used in nightly test
   * **release_test** This is used weekly tests and testing new release
 * Fields in testing phases:
     * **targets** list of targeted development boards
     * **toolchains** toolchain to use for compile
     * **exporters** allowed exporters
     * **compile** enable compiling
     * **export** enable exporting
     * **test** enable testing
     * **auto-update**
