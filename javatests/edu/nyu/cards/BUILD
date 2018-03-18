load("//rules:junit.bzl", "junit_tests")

junit_tests(
    name = "cards",
    size = "small",
    srcs = glob(["*.java"]),
    visibility = ["//:__pkg__"],
    deps = [
        "//cards_proto:cards_proto_java",
        "//java/edu/nyu/cards",
        "@guava_repo//jar",
        "@truth_repo//jar",
    ],
)
