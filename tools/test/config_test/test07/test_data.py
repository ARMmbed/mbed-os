# This build on top of test6 by adding overrides for libs in the application

expected_results = {
   "base": {
        "desc": "override values based on labels with libs (no labels)",
        "app.app1": "v_app1",
        "app.app2": "v_app2",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2"
    },
    "b1": {
        "desc": "override values based on labels with libs (first label)",
        "app.app1": "v_app1[b1_label]",
        "app.app2": "v_app2",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2[b1_label]"
    },
    "b2": {
        "desc": "override values based on labels with libs (second label)",
        "app.app1": "v_app1",
        "app.app2": "v_app2[b2_label]",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1[b2_label]",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2[b2_label]"
    },
    # The values for lib2.p1 and lib2.p2 illustrate how overriding on multiple
    # labels work. In lib2, both lib2.p1 and lib2.p2 are overriden for both
    # labels (b1_label and b2_label). However, since "b2_label" is specified
    # after "b1_label", it sets the final values of the overrides.
    "both": {
        "desc": "override values based on labels with libs (both labels)",
        "app.app1": "v_app1[b1_label]",
        "app.app2": "v_app2[b2_label]",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1[b2_label]",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2[b2_label]"
    }
}
