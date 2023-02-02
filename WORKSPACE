workspace(name = "com_github_pmbethe09_dtac")

load(":dtac_deps.bzl", "dtac_deps")

dtac_deps()

load("@rules_jvm_external//:repositories.bzl", "rules_jvm_external_deps")

rules_jvm_external_deps()

load("@rules_jvm_external//:setup.bzl", "rules_jvm_external_setup")

rules_jvm_external_setup()

load(":dtac_loads.bzl", "dtac_loads")

dtac_loads()
