# Test for configuration data defined in targets
# A base target (B1) defines 3 configuration parameters (base1_1, base1_2 and base1_3)
# A derived target (D1) inherits drom B1 and defines one configuration parameters (derived1 and derived2) and overrides base1_1 and base1_2
# Another base target (B2) defines its own configuration parameters (base2_1 and base2_2)
# The final target F derives from B2 and D1, defines two configuration paramaters (f1_1 and f1_2)
# and overrides base2_1, base1_1, derived2 and its own configuration parameter f1_1 (which is legal)
# Final result:
#     base1_1 must have the value defined in F
#     base1_2 must have the value defined in D1
#     base1_3 must have the value defined in B1
#     derived1 must have the value defined in D1
#     derived2 must have the value defined in F
#     base2_1 must have the value defined in F
#     base2_2 must have the value defined in B2
#     f1_1 must have the value defined and then overriden in F
#     f1_2 must have the value defined in F

expected_results = {
    "f": {
        "desc": "test multiple target inheritance",
        "target.base1_1": "v_base1_1_f",
        "target.base1_2": "v_base1_2_d1",
        "target.base1_3": "v_base1_3_b1",
        "target.derived1": "v_derived1_d1",
        "target.derived2": "v_derived2_f",
        "target.base2_1": "v_base2_1_f",
        "target.base2_2": "v_base2_2_b2",
        "target.f1_1": "v_f1_1_f_override",
        "target.f1_2": "v_f1_2_f"
    },
    "b1": {
        "desc": "test with a single base target, no inheritance",
        "target.base1_1": "v_base1_1_b1",
        "target.base1_2": "v_base1_2_b1",
        "target.base1_3": "v_base1_3_b1"
    },
    "d1": {
        "desc": "test single target inheritance",
        "target.base1_1": "v_base1_1_d1",
        "target.base1_2": "v_base1_2_d1",
        "target.base1_3": "v_base1_3_b1",
        "target.derived1": "v_derived1_d1",
        "target.derived2": "v_derived2_d1"
    }
}

