# Similar to test1, but this time B2 attempt to define base1_1. Since base1_1
# is already defined in B1 and F derives from both B1 and B2, this results
# in an error. However, when building for B2 instead of F, defining base1_1
# should be OK.

expected_results = {
    "f": {
        "desc": "attempt to redefine parameter in target inheritance tree",
        "exception_msg": "Parameter name 'base1_1' defined in both 'target:b2' and 'target:b1'"
    },
    "b2": {
        "desc": "it should be OK to define parameters with the same name in non-related targets",
        "target.base2_1": "v_base2_1_b2",
        "target.base2_2": "v_base2_2_b2",
        "target.base1_1": "v_base1_1_b2"
    }
}

