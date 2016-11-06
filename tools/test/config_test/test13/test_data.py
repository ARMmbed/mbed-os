# Create two libraries named "lib1", which must raise an error

expected_results = {
    "K64F": {
        "desc": "attempt to configure two libraries named 'lib1'",
        "exception_msg": "Library name 'lib1' is not unique"
    }
}
