package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache License 2.0

# Convenience library that sucks in dependencies of truth
java_library(
    name = "truth",
    testonly = True,
    exports = ["@com_google_truth_truth//jar"],
    runtime_deps = [
        "@com_google_guava_guava//jar",
        "@com_googlecode_diffutils//jar",
    ],
)
