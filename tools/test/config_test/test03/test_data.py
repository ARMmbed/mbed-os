# Similar to test1, but this time B2 attempt to override base1_1. Since B2 doesn't directly inherit
# from B1, this must raise an error

expected_results = {
    "f": {
        "desc": "attempt to override undefined parameter in inherited target",
        "exception_msg": "Attempt to override undefined parameter 'base1_1' in 'target:b2'"
    },
    "d1": {
        "desc": "single target inheritance again",
        "target.base1_1": "v_base1_1_d1",
        "target.base1_2": "v_base1_2_d1",
        "target.base1_3": "v_base1_3_b1",
        "target.derived1": "v_derived1_d1",
        "target.derived2": "v_derived2_d1"
    }
}

