# This tests overriding configuration values based on target labels.
# Four targets are defined:
#    - "base" is the base target, it doesn't define any extra labels
#    - "b1" inherits from "base" and adds the "b1_label" label
#    - "b2" inherits from "base" and adds the "b2_label" label
#    - "both" inherits from both "b1" and "b2", so it inherits both labels

expected_results = {
    "b1": {
        "desc": "override values based on labels (first label)",
        "app.app1": "v_app1[b1_label]",
        "app.app2": "v_app2"
    },
    "b2": {
        "desc": "override values based on labels (second label)",
        "app.app1": "v_app1",
        "app.app2": "v_app2[b2_label]"
    },
    "both": {
        "desc": "override values based on labels (both labels)",
        "app.app1": "v_app1[b1_label]",
        "app.app2": "v_app2[b2_label]"
    },
    "base": {
        "desc": "override values based on labels (no labels)",
        "app.app1": "v_app1",
        "app.app2": "v_app2"
    }
}
