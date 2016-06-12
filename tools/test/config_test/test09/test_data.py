# This build on top of test6 by adding an invalid override in mbed_app_override.json for b1_label.
# This will prevent the configuration for working for b1 and both, but it should still
# work for base and b2.

expected_results = {
   "base": {
        "desc": "override values based on labels with libs (no labels)",
        "app.app1": "v_app1",
        "app.app2": "v_app2",
        "lib1.p1": "v_p1_lib1",
        "lib1.p2": "v_p2_lib1",
        "lib1.p3": "v_p3_lib1",
        "lib2.p1": "v_p1_lib2",
        "lib2.p2": "v_p2_lib2"
    },
    "b1": {
        "desc": "override values based on labels with libs - invalid override (first label)",
        "exception_msg": "Attempt to override undefined parameter 'app.app_wrong' in 'application[b1_label]"
    },
    "b2": {
        "desc": "override values based on labels with libs (second label)",
        "app.app1": "v_app1",
        "app.app2": "v_app2[b2_label]",
        "lib1.p1": "v_p1_lib1",
        "lib1.p2": "v_p2_lib1[b2_label]",
        "lib1.p3": "v_p3_lib1",
        "lib2.p1": "v_p1_lib2[b2_label]",
        "lib2.p2": "v_p2_lib2[b2_label]"
    },
    "both": {
        "desc": "override values based on labels with libs - invalid override (both labels)",
        "exception_msg": "Attempt to override undefined parameter 'app.app_wrong' in 'application[b1_label]"
    },
}
