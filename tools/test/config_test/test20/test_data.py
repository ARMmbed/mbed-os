# Like test19, but this time check invalid key in a library configuration

expected_results = {
    "K64F": {
        "desc": "test invalid key in lib config",
        "exception_msg": "Unknown key(s) 'invalid_key'"
    }
}
