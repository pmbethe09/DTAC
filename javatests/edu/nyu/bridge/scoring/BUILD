load("//rules:junit.bzl", "junit_tests")

junit_tests(
    name = "scoring",
    size = "small",
    srcs = glob(["*.java"]),
    visibility = ["//:__pkg__"],
    deps = [
        "//bridge_proto:bridge_java_proto",
        "//cards_proto:cards_java_proto",
        "//java/edu/nyu/bridge/scoring",
        "//java/edu/nyu/bridge/util",
        "@maven//:com_google_truth_truth",
    ],
)
