load("//rules:junit.bzl", "junit_tests")

junit_tests(
    name = "cards",
    size = "small",
    srcs = glob(["*.java"]),
    visibility = ["//:__pkg__"],
    deps = [
        "//cards_proto:cards_java_proto",
        "//java/edu/nyu/cards",
        "//third_party/truth",
        "@com_google_guava_guava//jar",
    ],
)
