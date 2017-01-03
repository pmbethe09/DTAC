load("//rules:junit.bzl", "junit_tests")

junit_tests(
    name = "util",
    size = "small",
    srcs = glob(["*.java"]),
    visibility = ["//:__pkg__"],
    deps = [
        "//bridge_proto:bridge_proto_java",
        "//cards_proto:cards_proto_java",
        "//java/edu/nyu/bridge/util",
        "//java/edu/nyu/cards",
        "//java/edu/nyu/util",
        "@guava_repo//jar",
    ],
)
