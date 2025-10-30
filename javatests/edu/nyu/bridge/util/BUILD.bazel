load("//rules:junit.bzl", "junit_tests")

junit_tests(
    name = "util",
    size = "small",
    srcs = glob(["*.java"]),
    visibility = ["//:__pkg__"],
    deps = [
        "//bridge_proto:bridge_java_proto",
        "//cards_proto:cards_java_proto",
        "//java/edu/nyu/bridge/util",
        "//java/edu/nyu/cards",
        "//java/edu/nyu/util",
        "@maven//:com_google_guava_guava",
        "@maven//:com_google_truth_truth",
    ],
)
