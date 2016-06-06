# Like test17, but this time the invalid re-definition is in lib2, not in the app

expected_results = {
    "K64F": {
        "desc": "test invalid macro re-definition in a library",
        "exception_msg": "defined in both"
    }
}
