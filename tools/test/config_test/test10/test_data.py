# This builds on top of test8 by adding target-conditional overrides in mbed_app_config.json.

expected_results = {
   "base": {
        "desc": "override values based on labels with libs, target params and target overrides (no labels)",
        "app.app1": "v_app1",
        "app.app2": "v_app2",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2",
        "target.par1": "v_par1_target_app",
        "target.par2": "v_par2_base",
        "target.par3": "v_par3_base"
    },
    "b1": {
        "desc": "override values based on labels with libs, target params and target overrides (first label)",
        "app.app1": "v_app1[b1_label_label]",
        "app.app2": "v_app2",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2[b1_label]",
        "target.par1": "v_par1_target_app",
        "target.par2": "v_par2_base",
        "target.par3": "v_par3_base"
    },
    "b2": {
        "desc": "override values based on labels with libs, target params and target overrides (second label)",
        "app.app1": "v_app1",
        "app.app2": "v_app2[b2_label]",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1[b2_label]",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2[b2_label]",
        "target.par1": "v_par1_target_app",
        "target.par2": "v_par2_b2",
        "target.par3": "v_par3_base"
    },
    "both": {
        "desc": "override values based on labels with libs, target params and target overrides (both labels)",
        "app.app1": "v_app1[b1_label_label]",
        "app.app2": "v_app2[b2_label]",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1[b2_label]",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app[both_label]",
        "lib2.p2": "v_p2_lib2[b2_label]",
        "target.par1": "v_par1_target_app",
        "target.par2": "v_par2_b2",
        "target.par3": "v_par3_both",
        "target.par4": "v_par4_app[both_label]"
    }
}
