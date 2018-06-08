load("@com_github_pmbethe09_dtac//rules:junit.bzl", "junit_tests")

junit_tests(
    name = "local",
    size = "small",
    srcs = glob(["*.java"]),
    visibility = ["//:__pkg__"],
    deps = [
        "//bridge_proto:bridge_java_proto",
        "//java/edu/nyu/bridge/gib/local",
        "//java/edu/nyu/bridge/player",
        "//java/edu/nyu/bridge/util",
        "//java/edu/nyu/cards",
        "@guava_repo//jar",
        "@guice_repo//jar",
        "@truth_repo//jar",
    ],
)
