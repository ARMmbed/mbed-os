# Two libraries (lib1 and lib2) define their own configuration parameters
# lib1 tries to override a configuration parameter in lib2, which raises an error

expected_results = {
   "K64F": {
        "desc": "lib1 trying to override a config parameter in lib2",
        "exception_msg": "Invalid prefix 'lib2' for parameter name 'lib2.p1' in 'library:lib1[K64F]'"
    }
}
