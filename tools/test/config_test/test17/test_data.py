# Build on top of test16
# Adds an invalid macro redefinition in the app

expected_results = {
    "K64F": {
        "desc": "test invalid macro re-definition in the app",
        "exception_msg": "Macro 'LIB2_1' defined in both 'library:lib2' and 'application' with incompatible values"
    }
}
