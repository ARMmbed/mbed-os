# Two libraries (lib1 and lib2) define their own configuration parameters
# The application config doesn't have any parameters itself, it just overrides the parameter
# named p1 from both lib1 and lib2.

expected_results = {
   "test_target": {
        "desc": "app without its own parameters overrides parameters in other libs",
        "lib1.p1": "v_p1_lib1_app",
        "lib1.p2": "v_p2_lib1",
        "lib1.p3": "v_p3_lib1",
        "lib2.p1": "v_p1_lib2_app",
        "lib2.p2": "v_p2_lib2"
    }
}
