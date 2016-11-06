# This is similar to test1, but this time B2 also inherits from B1, which allows it to override its config data.
# B2 also overrides base1_2, like D1.
# The order of inheritace in F is also reversed ([D1, B2] instead of [B2, D1])
# Since the last override of base1_2 in inheritance order is in B2, base1_2 must now
# have the value that was overriden in B2, not in D1.
# This test also shows that multiple inheritance works for a simple diamond shaped inheritance pattern

expected_results = {
    "f": {
        "desc": "test multiple target inheritance (diamond shape)",
        "target.base1_1": "v_base1_1_f",
        "target.base1_2": "v_base1_2_b2",
        "target.base1_3": "v_base1_3_b1",
        "target.derived1": "v_derived1_d1",
        "target.derived2": "v_derived2_f",
        "target.base2_1": "v_base2_1_f",
        "target.base2_2": "v_base2_2_b2",
        "target.f1_1": "v_f1_1_f_override",
        "target.f1_2": "v_f1_2_f"
    },
    "b2": {
        "desc": "another single inheritance test",
        "target.base1_1": "v_base1_1_b1",
        "target.base1_2": "v_base1_2_b2",
        "target.base1_3": "v_base1_3_b1",
        "target.base2_1": "v_base2_1_b2",
        "target.base2_2": "v_base2_2_b2"
    }
}

