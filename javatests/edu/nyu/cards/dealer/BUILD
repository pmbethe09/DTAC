load("//rules:junit.bzl", "junit_tests")

junit_tests(
    name = "dealer",
    size = "small",
    srcs = glob(["*.java"]),
    visibility = ["//:__pkg__"],
    deps = [
        "//cards_proto:cards_java_proto",
        "//java/edu/nyu/cards",
        "//java/edu/nyu/cards/dealer",
        "@com_google_truth_truth//jar",
    ],
)
