# Macro test: defined macros in the top level app and 2 libs, check if they
# are reported properly.
# The app defines one macro with value the same as lib2, while lib2 defined
# the same macro without value as lib1. Since the definitions are comptabile,
# no error should be raised

expected_results = {
    "test_target": {
        "desc": "test macro definitions",
        "expected_macros": ["APP1=10", "APP2", "LIB1_1=1","LIB1_2", "LIB2_1=5"]
    }
}
