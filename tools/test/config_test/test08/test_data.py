# This build on top of test7 by adding some configuration values in targets
# and overriding them in the application

expected_results = {
   "base": {
        "desc": "override values based on labels with libs and target params (no labels)",
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
        "desc": "override values based on labels with libs and target params (first label)",
        "app.app1": "v_app1[b1_label]",
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
        "desc": "override values based on labels with libs and target params (second label)",
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
        "desc": "override values based on labels with libs and target params (both labels)",
        "app.app1": "v_app1[b1_label]",
        "app.app2": "v_app2[b2_label]",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1[b2_label]",
        "lib1.p3": "v_p3_lib1_app",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2[b2_label]",
        "target.par1": "v_par1_target_app",
        "target.par2": "v_par2_b2",
        "target.par3": "v_par3_both",
        "target.par4": "v_par4_both"
    }
}
